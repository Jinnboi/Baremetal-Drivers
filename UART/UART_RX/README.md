# UART_RX Driver

Polling-based UART receiver that controls an on-board LED based on terminal input.

## Overview

This driver configures USART2 in receive-only mode by directly programming the USART_CR1 and USART_BRR registers. No interrupts or DMA are used — the driver polls the RXNE (Read Data Register Not Empty) flag in the USART_SR register to detect incoming data. The baud rate is derived from the APB1 bus clock using the formula defined in the reference manual (19.3.4).

## Hardware Setup

| Item      | Detail       |
|-----------|--------------|
| MCU       | STM32F411RET6|
| Board     | NUCLEO-F411RE|
| Baudrate  | 115200       |

### Pin Mapping

| Signal | Pin | Abstractions   |
|--------|-----|----------------|
| RX     | PA3 | D0             |
| LED    | PA5 | D13            |

## Functional Logic

| Input | Action |
|-------|--------|
| `'1'` | LED turns ON |
| Any other character  | LED turns OFF |

## Key Registers

| Register | Purpose |
|----------|---------|
| `USART_CR1` | Enable USART, configure word length, enable receiver |
| `USART_BRR` | Set baud rate divisor |
| `USART_SR` | Poll RXNE flag to detect received data |
| `GPIOA_MODER` | Configure PA3 as alternate function, PA5 as output |
| `GPIOA_AFR` | Map PA3 to AF7 (USART2) |

## Initialization Sequence

1. Enable peripheral clocks (`RCC_AHB1ENR`, `RCC_APB1ENR`)
2. Configure GPIO pins (mode, alternate function, speed)
3. Set baud rate via `USART_BRR`
4. Enable the USART and receiver via `USART_CR1`
5. Enter polling loop — check `RXNE`, read `USART_DR`, act on data

## How to Run

1. Configure the hardware per the pin mapping above
2. Open this subfolder as a project in STM32CubeIDE
3. Build and flash (`Run > Debug` or `Run > Run`)
4. Open a serial terminal at **115200** baud
5. Send characters and observe the LED behavior

## Notes & Limitations

- Polling-based — not suitable for time-critical or high-throughput applications
- No error handling for framing or overrun errors (`FE`, `ORE` flags in `USART_SR`)

## Reference

- [STM32F411 Reference Manual (RM0383)](https://www.st.com/resource/en/reference_manual/rm0383-stm32f411xce-advanced-armbased-32bit-mcus-stmicroelectronics.pdf) — relevant sections: 19 (USART), 8 (GPIO), 6 (RCC)
- [STM32F411xE Datasheet](https://www.st.com/resource/en/datasheet/stm32f411re.pdf)