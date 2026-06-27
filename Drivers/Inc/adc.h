/**
 * @file 		adc.h
 * @brief 		Contains function prototypes and necessary includes for ADC driver
 * @author 		Marcos E. Mancia Jr.
 * @date 		2026-06-25
 * @version 	1.0
 */
#ifndef ADC_H_
#define ADC_H_

#include <stdint.h>

void adc_init(void);
uint32_t adc_read(void);
void start_conversion(uint8_t cont);

void adc_single_test(void);
void adc_continuous_test(void);

#endif /* ADC_H_ */
