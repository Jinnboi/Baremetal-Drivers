/**
 * @file 		tim.h
 * @brief 		Contains function prototypes for Timer driver
 * @author 		Marcos E. Mancia Jr.
 * @date 		2026-07-01
 * @version 	1.1
 */
#ifndef TIM_H_
#define TIM_H_

/***** FUNCTION PROTOTYPES *****/
void tim2_init(void);
void tim2_output_compare(void);
void tim3_input_capture(void);
void tim2_interrupt_init(void);

void timer_general_test(void);
void timer_output_compare_test(void);
void timer_input_compare_test(void);
void timer_interrupt_test(void);

#endif /* TIM_H_ */