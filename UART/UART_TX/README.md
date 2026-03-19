# UART_TX Test Driver
This driver implements a bare-metal UART Transmitter. It demonstrates basic data transmission by pushing data from the microcontroller to the terminal.

### Functional Logic
The test driver performs a basic transmission loop (sending "Transmitting!") to ensure the baud rate and start/stop bit configurations are correctly synchronized with the terminal.

### Hardware Setup
* TX Pin: PA2 (D1)
* Baudrate: 115200