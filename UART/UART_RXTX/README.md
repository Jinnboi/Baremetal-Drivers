# UART_RXTX Driver

Full-duplex UART driver that echoes non-key characters to the terminal and transmits a formatted response string when a specific key character is detected.

## Overview

This driver configures USART2 in full-duplex mode, enabling both the transmitter and
receiver via `USART_CR1`. It polls the RXNE flag to detect incoming bytes and the TXE
flag before writing each outgoing byte to `USART_DR`. When a received character matches
the key (`@`), the driver transmits a formatted alert string. All other characters are
echoed back as-is. No interrupts or DMA are used.

## Hardware Setup

| Item     | Detail        |
|----------|---------------|
| MCU      | STM32F411RET6 |
| Board    | NUCLEO-F411RE |
| Baudrate | 115200        |

### Pin Mapping

| Signal | Pin | Abstractions   |
|--------|-----|----------------|
| TX     | PA2 | D1             |
| RX     | PA3 | D0             |

## Functional Logic

The test application polls for incoming characters and responds based on whether the key
character (`@`) is detected.

| Input         | Action                              |
|---------------|-------------------------------------|
| `'@'`         | Transmit `"Found key: '@' !!!"` to terminal |
| Any other character | Echo character back to terminal |

## Key Registers

| Register      | Purpose                                                    |
|---------------|------------------------------------------------------------|
| `USART_CR1`   | Enable USART, word length, enable transmitter and receiver |
| `USART_BRR`   | Set baud rate divisor                                      |
| `USART_SR`    | Poll RXNE (byte received) and TXE (ready to transmit) flags|
| `USART_DR`    | Read received byte / write byte to transmit                |
| `GPIOA_MODER` | Configure PA2 and PA3 as alternate function                |
| `GPIOA_AFR`   | Map PA2 and PA3 to AF7 (USART2)                            |

## Initialization Sequence

1. Enable peripheral clocks (`RCC_AHB1ENR`, `RCC_APB1ENR`)
2. Configure PA2 (TX) and PA3 (RX) as alternate function, mapped to AF7 (USART2)
3. Set baud rate via `USART_BRR`
4. Enable the USART, transmitter, and receiver via `USART_CR1`
5. Enter polling loop:
   - Poll RXNE in `USART_SR` — read byte from `USART_DR` when ready
   - If byte == `'@'`: transmit alert string byte-by-byte, polling TXE before each write
   - Otherwise: echo byte back, polling TXE before writing to `USART_DR`

## How to Run

1. Configure the hardware per the pin mapping above
2. Open this subfolder as a project in STM32CubeIDE
3. Build and flash (`Run > Debug` or `Run > Run`)
4. Open a serial terminal at **115200** baud
5. Type any character and it will be echoed back
6. Type `@` and the terminal will display `Found key: '@' !!!`

---

## Notes & Limitations

- Polling-based — CPU is blocked waiting on both RXNE and TXE flags
- No handling of USART error flags (`FE`, `ORE`, `NE` in `USART_SR`)
- Single key character only — key detection logic could be extended to a lookup table for multiple keys

## Reference

- [STM32F411 Reference Manual (RM0383)](https://www.st.com/resource/en/reference_manual/rm0383-stm32f411xce-advanced-armbased-32bit-mcus-stmicroelectronics.pdf) — relevant sections: 19 (USART), 8 (GPIO), 6 (RCC)
- [STM32F411xE Datasheet](https://www.st.com/resource/en/datasheet/stm32f411re.pdf)