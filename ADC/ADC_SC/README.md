# ADC_Single_Conversion Driver

Polling-based ADC driver that performs a single conversion on a analog input and prints the raw 12-bit result to a serial terminal over USART2.

## Overview

This driver configures ADC1 in single-conversion mode by directly programming the
`ADC_CR2`, `ADC_SQR1` and `ADC_SQR3` registers. No interrupts or DMA are used — the
driver polls the EOC (End of Conversion) flag in `ADC_SR` to determine when a result
is ready, then reads the 12-bit value from `ADC_DR`. The result is formatted as a
decimal string and transmitted to a serial terminal via USART2, reusing the same
transmit logic described in the UART_TX driver. Sampling is performed on PA1
(ADC1_IN1) configured in analog input mode.

## Demo

<img src="/assets/adc_sc_demo.gif" width="400"/>

The full 12-bit range of values can be cycled through with a potentiometer connected
to 3V3 and GND.

## Hardware Setup

| Item       | Detail         |
|------------|----------------|
| MCU        | STM32F411RET6  |
| Board      | NUCLEO-F411RE  |
| Resolution | 12-bit (0–4095)|
| Baudrate   | 115200         |

### Pin Mapping

| Signal   | Pin | Abstractions   |
|----------|-----|----------------|
| ADC IN   | PA1 | A1             |
| TX       | PA2 | D1             |

## Functional Logic

The test application performs a single ADC conversion on each loop iteration and
transmits the raw result to the terminal.

| Condition | Action                                         |
|-----------|------------------------------------------------|
| Always    | Trigger conversion, wait for EOC, print result |

**Example terminal output:**
```
ADC Data: 1913
ADC Data: 158
ADC Data: 390
```

## Key Registers

| Register     | Purpose                                                         |
|--------------|-----------------------------------------------------------------|
| `ADC_CR2`    | Enable ADC, set SWSTART trigger, configure single conversion    |
| `ADC_SQR3`   | Select IN1 as the first (and only) channel in the sequence      |
| `ADC_SR`     | Poll EOC flag to detect end of conversion                       |
| `ADC_DR`     | Read the 12-bit conversion result                               |
| `GPIOA_MODER`| Configure PA1 as analog input, PA2 as alternate function (TX)   |
| `GPIOA_AFR`  | Map PA2 to AF7 (USART2)                                         |
| `USART_CR1`  | Enable USART and transmitter                                    |
| `USART_BRR`  | Set baud rate divisor                                           |
| `USART_SR`   | Poll TXE flag before each byte transmission                     |
| `USART_DR`   | Write formatted result bytes to transmit                        |

## Initialization Sequence

1. Enable peripheral clocks (`RCC_AHB1ENR`, `RCC_APB1ENR`, `RCC_APB2ENR`)
2. Configure PA1 as analog input (no pull-up/pull-down) via `GPIOA_MODER`
3. Configure PA2 as alternate function mapped to AF7 (USART2) via `GPIOA_AFR`
4. Select IN1 as the conversion channel via `ADC_SQR3`
5. Enable ADC1 via `ADC_CR2`
6. Configure USART2 baud rate via `USART_BRR`, enable via `USART_CR1`
7. Enter loop:
    - Set SWSTART in `ADC_CR2` to trigger conversion
    - Poll EOC in `ADC_SR` until conversion is complete
    - Read result from `ADC_DR`
    - Format as decimal string, transmit byte-by-byte over USART2

## How to Run

1. Connect an analog signal source (e.g. potentiometer, sensor) to PA1 (A1)
2. Open this subfolder as a project in STM32CubeIDE
3. Build and flash (`Run > Debug` or `Run > Run`)
4. Open a serial terminal at **115200** baud
5. Observe the raw ADC readings printed continuously to the terminal
6. Adjust the input voltage on PA1 and verify the output changes accordingly
   - 0V → `0`, 3.3V → `4095`

## Notes & Limitations

- Polling-based — CPU is blocked waiting on EOC and TXE flags throughout
- No averaging or filtering — readings will vary slightly due to ADC noise; a
  moving average would improve stability for sensor applications
- No overrun handling — if a conversion completes before the previous result is
  read, the `OVR` flag in `ADC_SR` will be set silently
- Sampling time is a tradeoff between accuracy and speed — longer sampling times
  reduce noise on high-impedance sources but reduce throughput

## Reference

- [STM32F411 Reference Manual (RM0383)](https://www.st.com/resource/en/reference_manual/rm0383-stm32f411xce-advanced-armbased-32bit-mcus-stmicroelectronics.pdf) — relevant sections: 11 (ADC), 19 (USART), 8 (GPIO), 6 (RCC)
- [STM32F411xE Datasheet](https://www.st.com/resource/en/datasheet/stm32f411re.pdf)

[def]: assets/adc_sc_demo.gif