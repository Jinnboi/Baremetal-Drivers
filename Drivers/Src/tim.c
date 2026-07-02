/**
 * @file        tim.c
 * @brief       Tests Timer peripheral via serial terminal
 * @author      Marcos E. Mancia Jr.
 * @date        2026-07-01
 * @version     1.1
 */
#include "stm32f411xe.h"
#include "tim.h"
#include "uart.h"

/***** USEFUL MACROS *****/
#define GPIOAEN			(1U<<0)
#define LED_PIN			(1U<<5)

#define AFR5_TIM		(1U<<20)
#define AFR6_TIM		(1U<<25)

#define TIM2EN			(1U<<0)
#define TIM3EN			(1U<<1)

#define CR1_CEN			(1U<<0)
#define OC_TOGGLE		(1U<<4) | (1U<<5)
#define CCER_CC1E		(1U<<0)
#define CCER_CC1S		(1U<<0)

#define SR_UIF			(1U<<0)
#define SR_CC1IF 		(1U<<1)
#define DIER_UIE		(1U<<0)

/***** TEST FUNCTIONS *****/

/**
 * @brief       Tests basic timer functionality by polling the
 * 				update interrupt flag
 */
void timer_general_test(void) {
	/*Initialize necessary peripherals for test*/
	uart2_tx_init();
	tim2_init();

	while(1) {
		/*Wait for UIF*/
		while(!(TIM2->SR & SR_UIF)) {}

		/*Clear UIF*/
		TIM2->SR &= ~SR_UIF;
		my_put("A second has passed...\n\r");
	}
}

/**
 * @brief       Tests the Output Compare functionality by toggling on-board LED
 */
void timer_output_compare_test(void) {
	/*Initialize necessary peripherals for test*/
	tim2_output_compare();

	/*The timer directly interacts with the LED, removing the additional
	PA5 set-up code*/
	while(1) {}
}

/**
 * @brief       Tests the Input Capture functionality by measuring time elapsed
 * 				between edges
 * @note		Connect a jumper wire from PA5 to PA6 and, using Debug mode,
 * 				add "timestamp" to Live Expressions panel
 */
void timer_input_compare_test(void) {
	/*Initialize necessary peripherals for test*/
	tim2_output_compare();
	tim3_input_capture();

	int timestamp = 0;
	while(1) {
		/*Wait until edge is captured*/
		while(!(TIM3->SR & SR_CC1IF)) {}

		/*Read value*/
		timestamp = TIM3->CCR1;
	}
}

/**
 * @brief		Tests Timer peripheral's interrupt mode by printing message
 * 				to the terminal and toggling LED every second
 */
void timer_interrupt_test(void) {
	/*Enable CLK access to GPIOA*/
	RCC->AHB1ENR |= GPIOAEN;

	/*Enable General Purpose Output Mode for PA5*/
	GPIOA->MODER |= (1U<<10);
	GPIOA->MODER &= ~(1U<<11);

	/*Enable necessary peripherals for test*/
	uart2_tx_init();
	tim2_interrupt_init();

	/*Interrupt programming removes polling-based code in loop*/
	while(1) {}
}

/***** HELPER FUNCTIONS *****/

/**
 * @brief       Initializes TIM2 in basic timer mode to tick at a 1 Hz rate
 */
void tim2_init(void) {
	/*Enable CLK access to TIM2*/
	RCC->APB1ENR |= TIM2EN;

	/*Set the prescaler value*/
	TIM2->PSC = 1600 - 1;	// 16 000 000 / 1600 = 10 000
    /*Set auto-reload value*/
	TIM2->ARR = 10000 - 1;	// 10 000 / 10 000 = 1 hz

    /*Clear counter*/
	TIM2->CNT = 0;

	/*Enable timer*/
	TIM2->CR1 = CR1_CEN;
}

/**
 * @brief       Configures TIM2 Channel 1 in Output Compare Toggle mode on pin PA5
 */
void tim2_output_compare(void) {
	/*Enable CLK access to GPIOA*/
	RCC->AHB1ENR |= GPIOAEN;

	/*Set PA5 to AF mode*/
	GPIOA->MODER &= ~(1U<<10);
	GPIOA->MODER |= (1U<<11);

	/*Set PA5 AF type to TIM2CH1 (AF01)*/
	GPIOA->AFR[0] |= AFR5_TIM;

	/*Enable CLK access to TIM2*/
	RCC->APB1ENR |= TIM2EN;

	/*Set the prescaler value*/
	TIM2->PSC = 1600 - 1;	// 16 000 000 / 1600 = 10 000
	/*Set auto-reload value*/
	TIM2->ARR = 10000 - 1;	// 10 000 / 10 000 = 1 Hz

	/*Set output compare toggle mode*/
	TIM2->CCMR1 = OC_TOGGLE;

	/*Enable TIM2 CH1 in compare mode*/
	TIM2->CCER |= CCER_CC1E;

	/*Clear counter*/
	TIM2->CNT = 0;

	/*Enable timer*/
	TIM2->CR1 = CR1_CEN;
}

/**
 * @brief  Configures TIM3 Channel 1 in Input Capture mode on pin PA6
 */
void tim3_input_capture(void) {
	/*Enable CLK access to GPIOA*/
	RCC->AHB1ENR |= GPIOAEN;

	/*Set PA5 to AF mode*/
	GPIOA->MODER &= ~(1U<<12);
	GPIOA->MODER |= (1U<<13);

	/*Set PA5 AF type to TIM3CH1 (AF02)*/
	GPIOA->AFR[0] |= AFR6_TIM;

	/*Enable clock access to TIM3*/
	RCC->APB1ENR |= TIM3EN;

	/*Set prescaler*/
	TIM3->PSC = 16000 - 1;	// 16 000 000 / 16 000
	/*Set CH1 to input capture*/
	TIM3->CCMR1 = CCER_CC1S;

	/*Set CH1 to capture at rising edge*/
	TIM3->CCER = CCER_CC1E;

	/*Enable TIM3*/
	TIM3->CR1 = CR1_CEN;
}

/**
 * @brief		Initializes TIM2 to create an interrupt event every second
 * 				(1 Hz)
 */
void tim2_interrupt_init(void) {
	/*Enable CLK access to TIM2*/
	RCC->APB1ENR |= TIM2EN;

	/*Set the prescaler value*/
	TIM2->PSC = 1600 - 1;	// 16 000 000 / 1600 = 10 000
	/*Set auto-reload value*/
	TIM2->ARR = 10000 - 1;	// 10 000 / 10 000 = 1 hz
	/*Clear counter*/
	TIM2->CNT = 0;

	/*Enable timer*/
	TIM2->CR1 = CR1_CEN;

	/*Enable TIM interrupt*/
	TIM2->DIER |= DIER_UIE;

	/*Enable TIM interrupt in NVIC*/
	NVIC_EnableIRQ(TIM2_IRQn);
}

/**
 * @brief		Prints a string to the terminal and toggles LED
 */
static void tim_callback(void) {
	my_put("A second has passed...\n\r");
	GPIOA->ODR ^= LED_PIN;
}

/**
 * @brief		ISR for TIM2 global interrupts
 */
void TIM2_IRQHandler(void) {
	/*Clear UIF - update interrupt flag*/
	TIM2->SR &= ~SR_UIF;

	tim_callback();
}