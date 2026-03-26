/**
 * @file    adc.h
 * @author  Marcos E. Mancia Jr.
 * @brief   ADC driver header for STM32F411RET6.
 * @version 1.0
 * @date    2026-03-26
 **/

#ifndef ADC_H_
#define ADC_H_

#include <stdint.h>
#include "stm32f4xx.h"

void pa1_adc_init(void);
uint32_t adc_read(void);
void start_conversion(void);

#endif /* ADC_H_ */