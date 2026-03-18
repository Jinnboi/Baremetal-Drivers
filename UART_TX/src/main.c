/**
 * @file    main.c
 * @author  Marcos E. Mancia Jr.
 * @brief   UART driver implementation for STM32F411RET6.
 * Handles basic TX functionality for serial communication.
 * @version 1.0
 * @date    2026-03-18
 **/

#include "uart.h"

void my_put(char *text);

int main(void)
{
    uart2_tx_init();
    
    while(1)
    {
        my_put("Transmitting!\n\r");
    }
}

void my_put(char *text) {
    int idx = 0;
    while(*(text + idx)) {
	uart2_write(*(text + idx++));
    }

    return;
}

void _init(void) {}
void _fini(void) {}
