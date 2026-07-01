/**
 * @file 		adc.h
 * @brief 		Contains function prototypes and necessary includes for ADC driver
 * @author 		Marcos E. Mancia Jr.
 * @date 		2026-07-01
 * @version 	1.3
 */
#ifndef ADC_H_
#define ADC_H_

#include <stdint.h>

/***** FUNCTION PROTOTYPES *****/
void adc_init(void);
void adc_interrupt_init(void);

void adc_single_test(void);
void adc_continuous_test(void);
void adc_interrupt_test(void);

#endif /* ADC_H_ */