# Baremetal-Drivers

A collection of lightweight, register-level peripheral drivers for the **STM32F411RET6** MCU, written in C with no HAL or vendor middleware dependencies.

Built to develop and demonstrate a deep understanding of ARM Cortex-M4 hardware. Every driver is implemented by directly configuring peripheral registers using the reference manual and associated documentation.

## Target Hardware

| Item | Detail |
|---|---|
| MCU | STM32F411RET6 |
| Architecture | ARM Cortex-M4 |
| Core Clock | Up to 100 MHz |
| Flash | 512 KB |
| RAM | 128 KB |
| Board | [NUCLEO-F411RE](https://www.st.com/en/evaluation-tools/nucleo-f411re.html) |

## Project Structure

Each driver lives in its own top-level folder and contains:
- Register-level driver implementation
- A self-contained test/demo application
- A `README.md` covering hardware setup, pin mapping, and functional logic

```
Baremetal-Drivers/
├── UART/
│   ├── UART_RX/        # Polling-based UART receive
│   ├── UART_TX/        # Polling-based UART transmit
│   └── UART_RXTX/      # Full-duplex UART echo
├── ADC/                # (In progress)
└── ...
```

## Drivers

### UART
Bare-metal UART driver covering receive, transmit, and full-duplex operation. Configured directly via USART registers — no HAL. Tested at 115200 baud over a serial terminal (Tera Term).

→ See [`UART/`](./UART)

### ADC *(In progress)*

→ See [`ADC/`](./ADC) *(coming soon)*

## Getting Started

### Prerequisites
- [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html) or an arm-none-eabi GCC toolchain
- ST-Link programmer (built into the NUCLEO board)
- A serial terminal (e.g. Tera Term or STM32CubeIDE console) for UART drivers

### Building & Flashing
Each driver subfolder is a self-contained STM32CubeIDE project.

1. Open STM32CubeIDE → `File > Open Projects from File System`
2. Navigate to the desired driver subfolder (e.g. `UART/UART_RX`)
3. Build with **Ctrl+B**
4. Flash via **Run > Debug** or **Run > Run**

Refer to each driver's `README.md` for expected behavior.

## Design Philosophy

- **No HAL or middleware.** Every peripheral is configured by writing directly to memory-mapped registers.
- **Specialized Drivers** Each subfolder is a minimal, focused implementation — no shared frameworks or abstractions between drivers.
- **Reference manual first.** Implementations follow the STM32F411 reference manual (RM0383) for register definitions and initialization sequences.

## Roadmap

- [✓] UART (RX, TX, RXTX)
- [ ] ADC
- [ ] SPI
- [ ] I2C
- [ ] GPIO
- [ ] Timers
- [ ] DMA

## Reference

- [STM32F411xE Datasheet](https://www.st.com/resource/en/datasheet/stm32f411re.pdf)
- [STM32F411 Reference Manual (RM0383)](https://www.st.com/resource/en/reference_manual/rm0383-stm32f411xce-advanced-armbased-32bit-mcus-stmicroelectronics.pdf)
- [NUCLEO-F411RE User Manual](https://www.st.com/resource/en/user_manual/um1724-stm32-nucleo64-boards-mb1136-stmicroelectronics.pdf)

## License

MIT