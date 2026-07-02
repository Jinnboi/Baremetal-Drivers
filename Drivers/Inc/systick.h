/**
 * @file 		systick.h
 * @brief 		Contains function prototypes for SysTick driver
 * @author 		Marcos E. Mancia Jr.
 * @date 		2026-07-01
 * @version 	1.3
 */
#ifndef SYSTICK_H_
#define SYSTICK_H_

/***** FUNCTION PROTOTYPES *****/
void systickDelayMs(int delay);
void systick_interrupt_init(void);

void systick_test(void);
void systick_interrupt_test(void);

#endif /* SYSTICK_H_ */