# UART_RX Test Driver
This driver implements a bare-metal UART Receiver. It demonstrates basic data acquisition by controlling an on-board LED based on terminal input.

### Functional Logic
* Key Character: '1'
* Actions:
    * Receiving '1' from the terminal turns the on-board LED ON
    * Receiving any other character from the terminal turns the on-board LED OFF

### Hardware Setup
* RX Pin: PA3 (D0)
* LED Pin: PA5 (D13)
* Baudrate: 115200