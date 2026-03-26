/**
 * @file    main.c
 * @author  Marcos E. Mancia Jr.
 * @brief   ADC test driver for STM32F411RET6.
 * Handles basic ADC single conversion functionality
 * @version 1.0
 * @date    2026-03-26
 **/
#include "../include/uart.h"
#include "../include/adc.h"

void print_num(uint32_t num);

uint32_t data;

int main(void)
{
	uart2_tx_init();
	pa1_adc_init();

	while(1)
	{
		start_conversion();
		data = adc_read();
		my_putc("ADC Data: ");
		print_num(data);
		my_putc("\n\r");
		for(volatile int i = 0; i < 100000; i++);
	}
}

void print_num(uint32_t num)
{
	char buf[11];
	int i = 10;

	buf[i] = '\0';
    if (num == 0)
    {
        buf[--i] = '0';
    }
    else
    {
        while (num > 0) {
        	// Implicitly cast uint32_t to char and floor divide
        	buf[--i] = '0' + (num % 10);
	        num /= 10;
	    }
	}

    // Print from first digit, not beginning of buffer
    my_putc(&buf[i]);
}

void _init(void) {}
void _fini(void) {}