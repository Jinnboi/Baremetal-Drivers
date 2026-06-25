/**
 * @file 		gpio.h
 * @brief 		Contains function prototypes and necessary includes for GPIO driver
 * @author 		Marcos E. Mancia Jr.
 * @date 		2026-06-25
 * @version 	1.0
 */
#ifndef GPIO_H_
#define GPIO_H_

#include "stm32f411xe.h"

void gpio_output_test(void);
void gpio_bsrr_test(void);
void gpio_input_test(void);

#endif /* GPIO_H_ */