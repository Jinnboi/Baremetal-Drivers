# UART_TX Driver

Polling-based UART transmitter that repeatedly sends a fixed string ("Transmitting!") to a serial terminal.

## Overview

This driver configures USART2 in transmit-only mode by directly programming the `USART_CR1`
and `USART_BRR` registers. No interrupts or DMA are used — the driver polls the TXE
(Transmit Data Register Empty) flag in `USART_SR` to determine when the data register is
ready for the next byte. The baud rate is derived from the APB1 bus clock using the formula
defined in RM0383 (19.3.4).

## Hardware Setup

| Iteam    | Detail        |
|----------|---------------|
| MCU      | STM32F411RET6 |
| Board    | NUCLEO-F411RE |
| Baudrate | 115200        |

### Pin Mapping

| Signal | Pin | Abstraction    |
|--------|-----|----------------|
| TX     | PA2 | D1             |

## Functional Logic

The test application enters an infinite loop, transmitting a fixed string over USART2 on
each iteration.

| Condition  | Action                                 |
|------------|----------------------------------------|
| Always     | Transmit "Transmitting!" to terminal   |

## Key Registers

| Register      | Purpose                                              |
|---------------|------------------------------------------------------|
| `USART_CR1`   | Enable USART, configure word length, enable transmitter |
| `USART_BRR`   | Set baud rate divisor                                |
| `USART_SR`    | Poll TXE flag to detect when data register is empty  |
| `USART_DR`    | Write byte to transmit                               |
| `GPIOA_MODER` | Configure PA2 as alternate function                  |
| `GPIOA_AFR`   | Map PA2 to AF7 (USART2)                              |

## Initialization Sequence

1. Enable peripheral clocks (`RCC_AHB1ENR`, `RCC_APB1ENR`)
2. Configure PA2 as alternate function, mapped to AF7 (USART2)
3. Set baud rate via `USART_BRR`
4. Enable the USART and transmitter via `USART_CR1`
5. Enter loop — for each byte, poll TXE in `USART_SR`, then write to `USART_DR`

## How to Run

1. Configure the hardware per the pin mapping above
2. Open this subfolder as a project in STM32CubeIDE
3. Build and flash (`Run > Debug` or `Run > Run`)
4. Open a serial terminal at **115200** baud
5. Observe the repeated string output in the terminal

## Notes & Limitations

- Polling-based — CPU is blocked during transmission, not suitable for high-throughput use
- No error handling for transmission faults

## Reference

- [STM32F411 Reference Manual (RM0383)](https://www.st.com/resource/en/reference_manual/rm0383-stm32f411xce-advanced-armbased-32bit-mcus-stmicroelectronics.pdf) — relevant sections: 19 (USART), 8 (GPIO), 6 (RCC)
- [STM32F411xE Datasheet](https://www.st.com/resource/en/datasheet/stm32f411re.pdf)