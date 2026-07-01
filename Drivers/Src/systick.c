/**
 * @file        systick.c
 * @brief       Tests SysTick-based blocking delay for STM32F411xe
 * @author      Marcos E. Mancia Jr.
 * @date        2026-06-30
 * @version     1.1
 */
#include "stm32f411xe.h"
#include "systick.h"
#include "uart.h"

/***** USEFUL MACROS *****/
#define GPIOAEN					(1U<<0)
#define LED_PIN         		(1U<<5)

#define SYSTICK_LOAD_VAL		16000
#define CTRL_ENABLE				(1U<<0)
#define CTRL_CLKSRC				(1U<<2)
#define CTRL_COUNTFLAG			(1U<<16)

/***** TEST FUNCTIONS *****/

/**
 * @brief  		Tests the SysTick timer by toggling an LED and
 * 		   		printing a message every second
 */
void systick_test(void) {
    /*Enable UART peripheral (which also enables CLK access to GPIOA)*/
    uart2_tx_init();

    /*Set PA5 (connected to on-board LED) as output pin*/
	GPIOA->MODER |= (1U<<10);
    GPIOA->MODER &= ~(1U<<11);

	while(1) {
		my_put("A second has passed... \n\r");
		GPIOA->ODR ^= LED_PIN;

		/*Use SysTick peripheral to wait a second*/
		systickDelayMs(1000);
	}
}

/***** HELPER FUNCTIONS *****/

/**
 * @brief       Generates a synchronous blocking delay in milliseconds using the SysTick peripheral
 * @param       delay: Number of milliseconds to block execution
 */
void systickDelayMs(int delay) {
	/*Reload with number of clocks per millisecond*/
	SysTick->LOAD = SYSTICK_LOAD_VAL;

	/*Clear systick current value register*/
	SysTick->VAL = 0;

	/*Enable systick and select internal clk src*/
	SysTick->CTRL = CTRL_ENABLE | CTRL_CLKSRC;

	for(int i=0;i<delay;i++) {
		/*Wait until the COUNTFLAG is set*/
		while((SysTick->CTRL & CTRL_COUNTFLAG)==0) {}
	}

    /*Disable SysTick peripheral after delay*/
	SysTick->CTRL = 0;
}