# Baremetal-Drivers

Bare-metal peripheral drivers for the **STM32F411** (Nucleo-F411RE), written in C directly against the register map, with no HAL, no LL, or vendor abstraction layers. Each driver is paired with a small test/demo function so it can be exercised standalone from `main.c`.

Every driver talks straight to the peripheral registers (`RCC`, `GPIOx`, `USART2`, `ADC1`, `TIMx`, `EXTI`, `SysTick`) using bitmasks defined at the top of each source file, mirroring how the reference manual describes each register.

## Features

| Peripheral | Driver | What it demonstrates |
|---|---|---|
| **GPIO** | `gpio.c` / `gpio.h` | Output via `ODR`, atomic set/reset via `BSRR`, digital input polling |
| **UART (USART2)** | `uart.c` / `uart.h` | Polled TX, polled RX, full RX/TX, RX via interrupt, TX via DMA1 |
| **ADC (ADC1)** | `adc.c` / `adc.h` | Single conversion, continuous conversion, conversion-complete interrupt |
| **SysTick** | `systick.c` / `systick.h` | Blocking millisecond delay, SysTick interrupt-driven timing |
| **Timers (TIM2/TIM3)** | `tim.c` / `tim.h` | General-purpose counting, output compare, input capture, update interrupt |
| **EXTI** | `exti.c` / `exti.h` | External interrupt on the on-board user button (PC13) |

All drivers assume the on-board LED on **PA5** and user button on **PC13** (according to Nucleo-F411RE pinout), with UART2 mapped to the ST-LINK virtual COM port for serial output/logging.

## Repository Structure

```
Baremetal-Drivers/
├── Drivers/
│   ├── Inc/                   # Driver headers + CMSIS / device headers
│   │   ├── gpio.h
│   │   ├── uart.h
│   │   ├── adc.h
│   │   ├── systick.h
│   │   ├── tim.h
│   │   ├── exti.h
│   │   ├── stm32f411xe.h       # Device register definitions
│   │   ├── core_cm4.h          # CMSIS Cortex-M4 core
│   │   ├── cmsis_gcc.h
│   │   ├── cmsis_compiler.h
│   │   ├── cmsis_version.h
│   │   ├── mpu_armv7.h
│   │   └── system_stm32f4xx.h
│   └── Src/
│       ├── gpio.c
│       ├── uart.c
│       ├── adc.c
│       ├── systick.c
│       ├── tim.c
│       ├── exti.c
│       └── main.c              # Test-suite runner / entry point
└── README.md
```

## Getting Started

### Hardware

- STM32F411-based board (developed with the **Nucleo-F411RE**)
- ST-LINK (on-board on the Nucleo) for flashing and serial-over-USB
- A serial terminal (PuTTY, Tera Term, etc.) at **115200 baud** for UART-based tests

### Toolchain

- ARM GCC (`arm-none-eabi-gcc`)
- **STM32CubeIDE**, or your own Makefile/OpenOCD + `arm-none-eabi-gdb` setup

### Building & Running a Test

`main.c` works as a single-target test harness: exactly one `TEST_TARGET_x` macro must be defined before building, and `main()` dispatches to the matching test function.

1. Open `Drivers/Src/main.c`.
2. Set the desired target, e.g.:
   ```c
   #define TEST_TARGET_UART2_TX
   ```
3. Build and flash to the board.
4. If the test involves UART, open a serial terminal at 115200 baud to view output.

Available targets:

```
TEST_TARGET_GPIO_OUTPUT
TEST_TARGET_GPIO_BSRR
TEST_TARGET_GPIO_INPUT

TEST_TARGET_UART2_TX
TEST_TARGET_UART2_RX
TEST_TARGET_UART2_RXTX
TEST_TARGET_UART2_RX_INT
TEST_TARGET_UART2_DMA1

TEST_TARGET_ADC_SINGLE
TEST_TARGET_ADC_CONT
TEST_TARGET_ADC_INT

TEST_TARGET_SYSTICK
TEST_TARGET_SYSTICK_INT

TEST_TARGET_TIM_GEN
TEST_TARGET_TIM_OC
TEST_TARGET_TIM_IC
TEST_TARGET_TIM_INT

TEST_TARGET_EXTI
```

Only one may be defined at a time — the build fails with `#error "No valid test target defined in main.c!"` if none is set.

## Driver API Overview

- **GPIO** — `gpio_output_test()`, `gpio_bsrr_test()`, `gpio_input_test()`
- **UART** — `uart2_tx_init()`, `uart2_rx_init()`, `uart2_rxtx_init()`, `uart2_rx_interrupt_init()`, `uart2_dma1_init(src, dst, len)`, `my_put(text)`
- **ADC** — `adc_init()`, `adc_interrupt_init()`
- **SysTick** — `systickDelayMs(delay)`, `systick_interrupt_init()`
- **Timers** — `tim2_init()`, `tim2_output_compare()`, `tim3_input_capture()`, `tim2_interrupt_init()`
- **EXTI** — `exti_init()`

Each `_init()` function configures the peripheral; the corresponding `*_test()` function ties initialization together with GPIO/UART to demonstrate the behavior end-to-end.

## Future Work
 
Planned additions to round out interrupt- and DMA-driven communication:
 
- **UART TX interrupt** — non-blocking transmit using `TXE`/`TC` interrupts instead of polling, to complement the existing RX-interrupt driver.
- **UART full RX/TX interrupt-driven mode** — combined `uart2_rxtx_interrupt_*` driver so both directions run off ISRs with buffering, rather than the current polled `uart2_rxtx_test()`.
- **UART RX via DMA** — mirror the existing TX-via-DMA1 driver with a receive-side DMA stream/channel for USART2.
- **DMA for other peripherals** — extend the DMA pattern beyond UART, e.g.:
  - **ADC-to-memory DMA** for continuous sampling without CPU polling/interrupt overhead per conversion.
  - **Timer-triggered DMA** (e.g. TIM update event driving a DMA transfer) for periodic register updates such as PWM duty cycle changes.
- **Double-buffered / circular DMA** examples for streaming use cases (continuous ADC capture, continuous UART logging).

## Author

**Marcos E. Mancia Jr.**

## Acknowledgements

Much of this code was programmed following the guidance of Udemy's course, ["Embedded Systems Bare-Metal Programming Ground Up™ (STM32)"](https://www.udemy.com/course/embedded-systems-bare-metal-programming/).

## License

No license has been specified for this project yet.