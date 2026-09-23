# ESP8266 + A4988 single motor

This is a standalone Arduino sketch for one 42-stepper motor and one A4988 driver. It does not modify the STM32 project.

## Pin wiring

Default target: NodeMCU ESP8266.

| ESP8266 | A4988 |
|---|---|
| D1 / GPIO5 | STEP |
| D2 / GPIO4 | DIR |
| D6 / GPIO12 | ENABLE |
| 3.3V | VDD |
| GND | GND |

`ENABLE` is active low. The sketch drives it low during startup.

Connect `RESET` and `SLEEP` together and pull them to VDD. Connect `MS1`, `MS2`, and `MS3` to GND for full-step mode. Connect the motor supply to `VMOT` and its negative terminal to A4988 GND. Add at least a 100 uF electrolytic capacitor close to VMOT/GND. ESP8266 ground, A4988 logic ground, and motor-supply negative must be common.

Never connect or disconnect the motor while the A4988 is powered.

## Arduino setup

1. Install the ESP8266 board package in Arduino IDE.
2. Select an ESP8266 board, such as `NodeMCU 1.0 (ESP-12E Module)`.
3. Open `arduino_esp8266_single_motor.ino`.
4. Upload at 115200 baud.

The sketch uses the ESP8266 Arduino core Timer1 API and does not require an external library.

## Serial commands

Open Serial Monitor at 115200 baud:

- `F`: forward at 30 RPM
- `B`: reverse at 30 RPM
- `S`: stop
- `+`: increase speed by 5 RPM
- `-`: decrease speed by 5 RPM
- `?`: print help and current speed

The conversion is the same as the STM32 version:

`step frequency = abs(RPM) * 200 / 60`

The default wheel circumference is 0.20 m, so 30 RPM corresponds to 0.10 m/s if the motor directly drives the wheel.

## Important Timer1 note

With `TIM_DIV16`, the ESP8266 timer clock is 5 MHz. The interrupt period must be 250 timer ticks for a 20 kHz phase-update rate. If the board package uses a different Timer1 API, adjust the `timer1_write()` call according to that package's timer tick definition.
