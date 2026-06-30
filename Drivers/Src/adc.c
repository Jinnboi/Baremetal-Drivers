/**
 * @file        adc.c
 * @brief       Tests ADC peripheral modes (Single, Continuous)
 * @author      Marcos E. Mancia Jr.
 * @date        2026-06-30
 * @version     1.2
 */
#include "stm32f411xe.h"
#include "adc.h"
#include "uart.h"

/***** USEFUL MACROS *****/
#define GPIOAEN					(1U<<0)
#define ADC1EN					(1U<<8)

#define ADC_CH1					(1U<<0)
#define ADC_SEQ_LEN_1			(0x00)

#define CR2_ADON				(1U<<0)
#define CR2_SWSTART				(1U<<30)
#define CR2_CONT				(1U<<1)

#define SR_EOC					(1U<<1)

#define SINGLE_CONVERSION		(0U)
#define CONT_CONVERSION			(1U)

/***** FUNCTION PROTOTYPES *****/
void start_conversion(uint8_t cont);
uint32_t adc_read(void);
void print_num(uint32_t num);

/***** TEST FUNCTIONS *****/

/**
 * @brief  		Demonstrates ADC in Single Conversion Mode
 */
void adc_single_test(void) {
	/*Initialize necessary peripherals for test*/
	uart2_tx_init();
	adc_init();

	uint32_t sensor_val;
	while(1) {
		/*Start the conversion process for a single sensor value*/
		start_conversion(SINGLE_CONVERSION);
		sensor_val = adc_read();

		/*Print out the sensor value to the terminal via UART*/
		my_put("Sensor value (Single):");
		print_num(sensor_val);
		my_put("\n\r");
		for(volatile int i=0;i<100000;i++);
	}
}

/**
 * @brief  		Demonstrates ADC in Continuous Conversion Mode
 */
void adc_continuous_test(void) {
	/*Initialize necessary peripherals for test*/
	uart2_tx_init();
	adc_init();

	/*Set the ADC to continuous conversion mode*/
	/*This mode removes the need to start a conversion manually*/
	start_conversion(CONT_CONVERSION);

	uint32_t sensor_val;
	while(1) {
		/*Read the sensor value and print it to the terminal via UART*/
		sensor_val = adc_read();
		my_put("Sensor value (Continuous):");
		print_num(sensor_val);
		my_put("\n\r");
		for(volatile int i=0;i<100000;i++);
	}
}

/***** HELPER FUNCTIONS *****/

/**
 * @brief  		Initializes GPIOA Pin 1 for Analog mode and configures ADC1 for Channel 1
 */
void adc_init(void) {
	/*Enable CLK access to GPIOA*/
	RCC->AHB1ENR |= GPIOAEN;

	/*Set PA1 mode to analog mode*/
	GPIOA->MODER |= (1U<<2);
    GPIOA->MODER |= (1U<<3);

	/*Enable CLK access to ADC*/
	RCC->APB2ENR |= ADC1EN;

	/*Conversion sequence start*/
	ADC1->SQR3 = ADC_CH1;

	/*Conversion sequence length*/
	ADC1->SQR1 = ADC_SEQ_LEN_1;

	/*Enable ADC module*/
	ADC1->CR2 |= CR2_ADON;
}

/**
 * @brief  		Configures conversion mode and triggers the conversion via software
 * @param  		cont: Mode selector. Use SINGLE_CONVERSION (0) or CONT_CONVERSION (1)
 */
void start_conversion(uint8_t cont) {
	if(cont) {
		/*Enable continuous conversion*/
		ADC1->CR2 |= CR2_CONT;
	}else {
		/*Disables continuous conversion*/
		ADC1->CR2 &= ~CR2_CONT;
	}

	/*Start adc conversion*/
	ADC1->CR2 |= CR2_SWSTART;
}

/**
 * @brief  		Blocks until the End of Conversion (EOC) flag is raised, then reads the data
 * @return 		32-bit unsigned integer containing the right-aligned conversion result
 */
uint32_t adc_read(void) {
	/*Wait for conversion to be complete*/
	while(!(ADC1->SR & SR_EOC)) {}

	/*Read converted result*/
	return (uint32_t)(ADC1->DR);
}

/**
 * @brief  		Converts an unsigned 32-bit integer to an ASCII string and prints it via UART
 * @param  		num: The 32-bit unsigned integer to be printed
 */
void print_num(uint32_t num) {
	char buf[11];
	int i = 10;

	buf[i] = '\0';
    if(num == 0) {
        buf[--i] = '0';
    }else {
        while(num > 0) {
        	// Implicitly cast uint32_t to char and floor divide
        	buf[--i] = '0' + (num % 10);
	        num /= 10;
	    }
	}

    // Print from first digit, not beginning of buffer
    my_put(&buf[i]);
}
