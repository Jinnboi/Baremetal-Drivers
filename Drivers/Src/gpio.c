/*
 * @file 		gpio.c
 * @brief 		Tests GPIO peripheral modes (Output, BSRR, and Input) via
 * 				on-board LED and User Button
 * @author 		Marcos E. Mancia Jr.
 * @date 		2026-06-25
 * @version 	1.1
 */
#include "gpio.h"
#include "stm32f411xe.h"

/***** USEFUL MACROS *****/
#define GPIOAEN				(1U<<0)
#define GPIOCEN				(1U<<2)
#define LED_PIN				(1U<<5)
#define BTN_PIN				(1U<<13)

/***** FUNCTION PROTOTYPES *****/
void gpio_output_test(void);
void gpio_bsrr_test(void);
void gpio_input_test(void);

/***** TEST FUNCTIONS *****/

/*
 * @brief		Toggles the on-board LED using the ODR register
 */
void gpio_output_test(void) {
	/*Enable CLK access to GPIOA*/
	RCC->AHB1ENR |= GPIOAEN;

	/*Set PA5 (connected to on-board LED) as output pin*/
	GPIOA->MODER |=  (1U<<10);
	GPIOA->MODER &= ~(1U<<11);

	while(1) {
		/*Toggle LED_PIN using output data register*/
		GPIOA->ODR ^= LED_PIN;
		for(volatile int i=0;i<1000000;i++) {}
	}
}

/*
 * @brief		Toggles the on-board LED using atomic writes via the BSRR register
 */
void gpio_bsrr_test(void) {
	/*Enable CLK access to GPIOA*/
	RCC->AHB1ENR |= GPIOAEN;

	/*Set PA5 (connected to on-board LED) as output pin*/
	GPIOA->MODER |=  (1U<<10);
	GPIOA->MODER &= ~(1U<<11);

	while(1) {
		/*Set corresponding ODR bit of PA5*/
		GPIOA->BSRR = LED_PIN;
		for(volatile int i=0;i<1000000;i++) {}

		/*Reset corresponding ODR bit of PA5*/
		GPIOA->BSRR = (LED_PIN << 16);
		for(volatile int i=0;i<1000000;i++) {}
	}
}

/*
 * @brief		Polls the User Button state and drives the LED
 */
void gpio_input_test(void) {
	/*Enable CLK access to GPIOA and GPIOC*/
	RCC->AHB1ENR |= (GPIOAEN | GPIOCEN);

	/*Set PA5 (connected to on-board LED) as output pin*/
	GPIOA->MODER |=  (1U<<10);
	GPIOA->MODER &= ~(1U<<11);

	/*Set PC13 (connected to on-board button) as input pin*/
	GPIOC->MODER &= ~(1U<<26);
	GPIOC->MODER &= ~(1U<<27);

	while(1) {
		/*Check if BTN is pressed*/
		if(GPIOC->IDR & BTN_PIN) {
			/*Set corresponding ODR bit of PA5*/
			GPIOA->BSRR |= LED_PIN;
		}else {
			/*Reset corresponding ODR bit of PA5*/
			GPIOA->BSRR |= (LED_PIN << 16);
		}
	}
}