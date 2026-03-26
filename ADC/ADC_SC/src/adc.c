/**
 * @file    adc.c
 * @author  Marcos E. Mancia Jr.
 * @brief   ADC driver implementation for STM32F411RET6.
 * @version 1.0
 * @date    2026-03-26
 **/
#include "../include/adc.h"

#define GPIOAEN			(1U<<0)
#define ADC1EN			(1U<<8)

#define ADC_CH1			(1U<<0)
#define ADC_SEQ_LEN_1	(0x00)

#define CR2_ADON		(1U<<0)
#define CR2_SWSTART		(1U<<30)

#define SR_EOC			(1U<<1)

void pa1_adc_init(void)
{
	/*********Configure ADC GPIO pin*********/

	/*Enable CLK access to GPIOA*/
	RCC->AHB1ENR |= GPIOAEN;

	/*Set the mode of PA1 to analog*/
	GPIOA->MODER |= (3U<<2);

	/*********Configure ADC module*********/

	/*Enable CLK access to ADC*/
	RCC->APB2ENR |= ADC1EN;

	/*Conversion sequence start*/
	// 1 Channel, Channel 1 of ADC
	ADC1->SQR3 = ADC_CH1;

	/*Conversion sequence length*/
	ADC1->SQR1 = ADC_SEQ_LEN_1;

	/*Enable ADC module*/
	ADC1->CR2 |= CR2_ADON;
}

void start_conversion(void)
{
	/*Start adc conversion*/
	ADC1->CR2 |= CR2_SWSTART;
}

uint32_t adc_read(void)
{
	/*Wait for conversion to be complete*/
	while(!(ADC1->SR & SR_EOC)) {}

	/*Read converted result*/
	return ADC1->DR;
}