#include <Arduino.h>

// NodeMCU ESP8266 pin mapping.
constexpr uint8_t STEP_PIN = D1;  // GPIO5
constexpr uint8_t DIR_PIN = D2;   // GPIO4
constexpr uint8_t ENABLE_PIN = D6; // GPIO12, active low

constexpr float STEPS_PER_REV = 200.0f; // A4988 full-step mode
constexpr float WHEEL_CIRCUMFERENCE_M = 0.20f;
constexpr float MAX_RPM = 120.0f;
constexpr uint32_t TIMER_TICK_HZ = 20000U;

volatile uint32_t stepPhase = 0U;
volatile uint32_t stepIncrement = 0U;
volatile bool stepOutputHigh = false;

float motorRpm = 0.0f;

void ICACHE_RAM_ATTR stepTimerIsr()
{
    const uint32_t previousPhase = stepPhase;
    stepPhase += stepIncrement;

    if (stepPhase < previousPhase) {
        stepOutputHigh = !stepOutputHigh;
        digitalWrite(STEP_PIN, stepOutputHigh ? HIGH : LOW);
    }
}

void setMotorRpm(float rpm)
{
    rpm = constrain(rpm, -MAX_RPM, MAX_RPM);
    motorRpm = rpm;

    digitalWrite(DIR_PIN, rpm >= 0.0f ? HIGH : LOW);
    delayMicroseconds(5); // A4988 DIR setup time before the next STEP edge.

    const float stepFrequency = fabsf(rpm) * STEPS_PER_REV / 60.0f;
    noInterrupts();
    stepIncrement = static_cast<uint32_t>(stepFrequency * 4294967296.0f / TIMER_TICK_HZ);
    if (stepFrequency == 0.0f) {
        stepPhase = 0U;
        stepOutputHigh = false;
        digitalWrite(STEP_PIN, LOW);
    }
    interrupts();
}

void setMotorLinearVelocity(float metersPerSecond)
{
    setMotorRpm(metersPerSecond * 60.0f / WHEEL_CIRCUMFERENCE_M);
}

void printStatus()
{
    Serial.print(F("RPM: "));
    Serial.print(motorRpm, 2);
    Serial.print(F(", linear velocity: "));
    Serial.print(motorRpm * WHEEL_CIRCUMFERENCE_M / 60.0f, 3);
    Serial.println(F(" m/s"));
}

void handleSerial()
{
    if (!Serial.available()) return;

    const char command = Serial.read();
    switch (command) {
    case 'f':
    case 'F':
        setMotorRpm(30.0f);
        printStatus();
        break;
    case 'b':
    case 'B':
        setMotorRpm(-30.0f);
        printStatus();
        break;
    case 's':
    case 'S':
        setMotorRpm(0.0f);
        printStatus();
        break;
    case '+':
        setMotorRpm(motorRpm + 5.0f);
        printStatus();
        break;
    case '-':
        setMotorRpm(motorRpm - 5.0f);
        printStatus();
        break;
    case '?':
        Serial.println(F("F forward, B reverse, S stop, +/- change RPM"));
        printStatus();
        break;
    default:
        break;
    }
}

void setup()
{
    pinMode(STEP_PIN, OUTPUT);
    pinMode(DIR_PIN, OUTPUT);
    pinMode(ENABLE_PIN, OUTPUT);

    digitalWrite(STEP_PIN, LOW);
    digitalWrite(DIR_PIN, HIGH);
    digitalWrite(ENABLE_PIN, LOW); // Enable A4988.

    Serial.begin(115200);
    Serial.println();
    Serial.println(F("ESP8266 single A4988 full-step controller"));
    Serial.println(F("Send ? for commands."));

    timer1_attachInterrupt(stepTimerIsr);
    timer1_enable(TIM_DIV16, TIM_EDGE, TIM_LOOP);
    timer1_write(250); // 80 MHz / 16 / 20 kHz = 250 timer ticks.

    setMotorRpm(0.0f);
}

void loop()
{
    handleSerial();
}
