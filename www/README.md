# STM32F407 FreeRTOS integration

The `www` target is the STM32F407 application. It starts the FreeRTOS scheduler
after CubeMX peripheral initialization and creates these tasks:

- `camera-uart`: receives line-oriented commands from ESP32-CAM on USART1, 115200 8N1.
- `control`: dispatches camera commands to the local actuator and display.
- `drive`: converts the latest velocity command into four A4988 wheel speeds.
- `demo`: runs once after boot: forward 2 seconds, stop, reverse 2 seconds,
  stop, rotate 1.5 seconds, then stop. Any `VEL` command cancels it.

Commands sent by ESP32-CAM:

```text
VEL:0.50,0.00,0.00\n
SERVO:1,10\n
QR:123+456+789+012\n
```

`VEL` maps X, Y, and Z to the four-wheel chassis kinematics. The QR task
accepts exactly four three-digit groups separated by `+`, for example
`123+456+789+012`, and shows it in a two-line, 80-pixel-high font (about 12 mm
on a common 2.4-inch 320x240 TFT). ESP32-CAM now forwards newly detected
browser barcode values to `/qr`, which emits `QR:<value>\n` on its UART0.

`SERVO:<number>,<angle>` addresses servo 1 through 4, with an angle from 0 to
20. The original `SERVO:<angle>` form remains supported for servo 1.

## Wiring

Connect ESP32-CAM UART0 TX (GPIO1) to STM32 USART1 RX (PA10), ESP32-CAM UART0
RX (GPIO3) to STM32 USART1 TX (PA9), and join grounds. Both boards use 3.3 V
logic. The four A4988 drivers use PC6/PA2/PA4/PC7 for STEP (front-left,
front-right, rear-left, rear-right), PA1/PA3/PA5/PA7 for DIR, and PC13 for the
shared active-low ENABLE signal. TIM2 produces 50 kHz step timing. Servo PWM
signals are servo 1: TIM1_CH1/PE9, servo 2: TIM5_CH1/PA0, servo 3:
TIM3_CH1/PA6, and servo 4: TIM4_CH1/PD12.