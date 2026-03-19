/**
 * @file    main.c
 * @author  Marcos E. Mancia Jr.
 * @brief   UART driver implementation for STM32F411RET6.
 * Handles basic RX/TX functionality for serial communication.
 * @version 1.0
 * @date    2026-03-19
 **/

#include "../include/uart.h"
#define LED_PIN				(1U<<5)

void my_put(char *text);

char key;

int main(void)
{
    uart2_rxtx_init();

    /* Enable General Purpose Output Mode for PA5*/
	GPIOA->MODER |=  (1U<<10);
	GPIOA->MODER &= ~(1U<<11);
    
    while(1)
    {
        key = uart2_read();

        if(key == '@') {
            GPIOA->ODR |= LED_PIN;
            my_put("\n\rFound key: '@' !!!\n\r");
        }else {
            GPIOA->ODR &= ~LED_PIN;
            uart2_write(key);
        }
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
