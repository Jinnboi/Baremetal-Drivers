/**
 * @file    main.c
 * @author  Marcos E. Mancia Jr.
 * @brief   UART driver implementation for STM32F411RET6.
 * Handles basic RX functionality byturning on/off on-board LED
 * @version 1.0
 * @date    2026-03-19
 **/

#include "../include/uart.h"

#define LED_PIN				(1U<<5)

char key;

int main(void)
{
    /* Initialize UART Receiver*/
	uart2_rx_init();

    /* Enable General Purpose Output Mode for PA5*/
	GPIOA->MODER |=  (1U<<10);
	GPIOA->MODER &= ~(1U<<11);

	while(1)
	{
        /* Read character from terminal */
		key = uart2_read();

		if(key == '1') {
			GPIOA->ODR |= LED_PIN;
		}else {
			GPIOA->ODR &= ~LED_PIN;
		}
	}
}

void _init(void) {}
void _fini(void) {}
