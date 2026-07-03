/**
 * @file 		main.c
 * @brief 		Test suite runner for custom STM32F411 bare-metal drivers
 * @author 		Marcos E. Mancia Jr.
 * @date 		2026-07-02
 * @version 	1.0
 */
#include "stm32f411xe.h"
#include "gpio.h"
#include "uart.h"
#include "adc.h"
#include "systick.h"
#include "tim.h"
#include "exti.h"

/*Mandatory Configuration: Enable exactly one test target*/
#define TEST_TARGET_GPIO_OUTPUT

int main(void) {

	#if defined(TEST_TARGET_GPIO_OUTPUT)
        gpio_output_test();
    #elif defined(TEST_TARGET_GPIO_BSRR)
        gpio_bsrr_test();
    #elif defined(TEST_TARGET_GPIO_INPUT)
        gpio_input_test();

    #elif defined(TEST_TARGET_UART2_TX)
        uart2_tx_test();
    #elif defined(TEST_TARGET_UART2_RX)
        uart2_rx_test();
    #elif defined(TEST_TARGET_UART2_RXTX)
        uart2_rxtx_test();
    #elif defined(TEST_TARGET_UART2_RX_INT)
        uart2_rx_interrupt_test();
    #elif defined(TEST_TARGET_UART2_DMA1)
        uart2_dma1_test();

    #elif defined(TEST_TARGET_ADC_SINGLE)
        adc_single_test();
    #elif defined(TEST_TARGET_ADC_CONT)
        adc_continuous_test();
    #elif defined(TEST_TARGET_ADC_INT)
        adc_interrupt_test();

    #elif defined(TEST_TARGET_SYSTICK)
        systick_test();
    #elif defined(TEST_TARGET_SYSTICK_INT)
        systick_interrupt_test();

    #elif defined(TEST_TARGET_TIM_GEN)
        timer_general_test();
    #elif defined(TEST_TARGET_TIM_OC)
        timer_output_compare_test();
    #elif defined(TEST_TARGET_TIM_IC)
        timer_input_compare_test();
    #elif defined(TEST_TARGET_TIM_INT)
        timer_interrupt_test();

    #elif defined(TEST_TARGET_EXTI)
        exti_test();

    #else
        #error "No valid test target defined in main.c!"
    #endif

    /*Outer loop that should not be reached under normal circumstances*/
	while(1) {
		__WFI();
	}
}