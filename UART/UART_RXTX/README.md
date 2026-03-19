# UART_RXTX Test Driver
This driver implements a bare-metal UART Receiver/Transmitter Pair. It demonstrates basic data acquisition by controlling an on-board LED based on terminal input and data transmission by sending confirmation of characters typed.

### Functional Logic
* Key Character: '@'
* Echo Test: Validates that the characters received is correctly stored and re-transmitted
* Stability: Tests the driver's ability to handle incoming data without dropped bytes or buffer overflows.

### Hardware Setup
* RX Pin: PA3 (D0)
* TX Pin: PA2 (D1)
* LED Pin: PA5 (D13)
* Baudrate: 115200
