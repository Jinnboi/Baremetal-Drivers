/**
 * @file        uart.c
 * @brief       Tests USART2 peripheral modes (RX, TX, RXTX) via serial terminal
 * @author      Marcos E. Mancia Jr.
 * @date        2026-06-25
 * @version     1.0
 */
#include "uart.h"

/***** USEFUL MACROS *****/
#define GPIOAEN				(1U<<0)
#define UART2EN				(1U<<17)
#define LED_PIN             (1U<<5)

#define CR1_RE				(1U<<2)
#define CR1_TE				(1U<<3)
#define CR1_UE				(1U<<13)

#define SR_RXNE				(1U<<5)
#define SR_TXE				(1U<<7)

#define SYS_FREQ			16000000
#define APB1_CLK			SYS_FREQ
#define UART_BAUDRATE		115200

/***** FUNCTION PROTOTYPES *****/
static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t Baudrate);
static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t Baudrate);

/***** TEST FUNCTIONS *****/

/**
 * @brief		Prints a string to the serial terminal
 */
void uart2_tx_test(void) {
    /*Initialize USART2 in TX mode*/
    uart2_tx_init();
	
    while(1) {
		printf("Is this working???\n\r");
	}
}

/**
 * @brief		Reads keyboard input from serial terminal and toggles LED if
 * 				key == 1
 */
void uart2_rx_test(void) {
    /*Initialize USART2 in RX mode*/
    uart2_rx_init();

	/*Set PA5 (connected to on-board LED) as output pin*/
	GPIOA->MODER |=  (1U<<10);
	GPIOA->MODER &= ~(1U<<11);

    char key;
	while(1) {
		key = uart2_read();
		if(key == '1') {
			GPIOA->ODR |= LED_PIN;
		}else {
			GPIOA->ODR &= ~LED_PIN;
		}
	}
}

/**
 * @brief		Reads keyboard input from serial terminal, toggles LED if
 * 				key == 1, and prints each key to the serial terminal 
 */
void uart2_rxtx_test(void) {
    /*Initialize USART2 in Full Duplex TX/RX mode*/
    uart2_rxtx_init();

	/* Enable General Purpose Output Mode for PA5*/
	GPIOA->MODER |=  (1U<<10);
	GPIOA->MODER &= ~(1U<<11);

    char key;
	while(1) {
		key = uart2_read();

		if(key == '@') {
			GPIOA->ODR |= LED_PIN;
			uart2_write(key);
		}else {
			GPIOA->ODR &= ~LED_PIN;
			uart2_write(key);
		}
	}
}

/***** HELPER FUNCTIONS *****/

/**
 * @brief       Initializes USART2 for Transmitter (TX) mode only
 */
void uart2_tx_init(void) {
	/*Enable CLK access to GPIOA*/
	RCC->AHB1ENR |= GPIOAEN;

	/*Set PA2 mode to alternate function mode*/
	GPIOA->MODER |=  (1U<<5);
	GPIOA->MODER &= ~(1U<<4);

	/*Set PA2 alternate function type to UART_TX (AF07)*/
	GPIOA->AFR[0] &= ~(1U<<11);
	GPIOA->AFR[0] |=  (7U<<8);

	/*Enable CLK access to UART2*/
	RCC->APB1ENR |= UART2EN;

	/*Configure baudrate*/
	uart_set_baudrate(USART2, APB1_CLK, UART_BAUDRATE);

	/*Configure transfer direction*/
	USART2->CR1 = CR1_TE;
	/*Setting stop bit # (in CR2) is not necessary since it is autofilled correctly from reset*/

	/*Enable UART2 module*/
	USART2->CR1 |= CR1_UE;
}

/**
 * @brief       Initializes USART2 for Receiver (RX) mode only
 */
void uart2_rx_init(void) {
	/*Enable CLK access to GPIOA*/
	RCC->AHB1ENR |= GPIOAEN;

	/*Set PA3 mode to alternate function mode*/
	GPIOA->MODER |=  (1U<<7);
	GPIOA->MODER &= ~(1U<<6);

	/*Set PA3 alternate function type to UART_RX (AF07)*/
	GPIOA->AFR[0] &= ~(1U<<15);
	GPIOA->AFR[0] |=  (7U<<12);

	/*Enable CLK access to UART2*/
	RCC->APB1ENR |= UART2EN;

	/*Configure baudrate*/
	uart_set_baudrate(USART2, APB1_CLK, UART_BAUDRATE);

	/*Configure transfer direction*/
	USART2->CR1 = CR1_RE;		
	
	/*Enable UART2 module*/
	USART2->CR1 |= CR1_UE;
}

/**
 * @brief  Initializes USART2 for Full-Duplex Transmit and Receive (TX/RX) mode
 */
void uart2_rxtx_init(void) {
	/*Enable CLK access to GPIOA*/
	RCC->AHB1ENR |= GPIOAEN;

	/*Set PA2 mode to alternate function mode*/
	GPIOA->MODER |=  (1U<<5);
	GPIOA->MODER &= ~(1U<<4);

	/*Set PA3 mode to alternate function mode*/
	GPIOA->MODER |=  (1U<<7);
	GPIOA->MODER &= ~(1U<<6);

	/*Set PA2 alternate function type to UART_TX (AF07)*/
	GPIOA->AFR[0] &= ~(1U<<11); //__IO AFR[2] == AFRL and AFRH
	GPIOA->AFR[0] |=  (7U<<8);

	/*Set PA3 alternate function type to UART_RX (AF07)*/
	GPIOA->AFR[0] &= ~(1U<<15);
	GPIOA->AFR[0] |=  (7U<<12);

	/*Enable CLK access to UART2*/
	RCC->APB1ENR |= UART2EN;

	/*Configure baudrate*/
	uart_set_baudrate(USART2, APB1_CLK, UART_BAUDRATE);

	/*Configure transfer direction*/
	USART2->CR1 = (CR1_TE | CR1_RE);
	/*Setting stop bit # (in CR2) is not necessary since it is autofilled correctly from reset*/

    /*Enable UART2 module*/
	USART2->CR1 |= CR1_UE;
}

/**
 * @brief       Blocks until a character is received, then reads it from the DR register
 * @return      Received character byte
 */
char uart2_read(void) {
	/*Ensure the receive data register is NOT empty*/
	while(!(USART2->SR & SR_RXNE)) {}

	/*Read and return byte*/
	return (char)(USART2->DR & 0xFF);
}

/**
 * @brief       Blocks until the Transmit DR register is empty, then writes a character
 * @param       ch: Character byte to transmit
 */
void uart2_write(int ch) {
	/*Ensure transmit data register is empty*/
	while(!(USART2->SR & SR_TXE)) {}

	/*Write to transmit data register*/
	USART2->DR = (ch & 0xFF);
}

/**
 * @brief       Updates the Baud Rate Register for a given USART instance
 * @param       USARTx: Pointer to the target USART peripheral base register map structure
 * @param       PeriphClk: Frequency of the peripheral clock source feeding the module
 * @param       Baudrate: Desired communication speed (ex: 115200)
 */
static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t Baudrate) {
	// Baudrate Register
	USARTx->BRR = compute_uart_bd(PeriphClk, Baudrate);
}

/**
 * @brief       Calculates the required BRR register value
 * @param       PeriphClk: Frequency of the peripheral clock source
 * @param       Baudrate: Desired communication speed (ex: 115200)
 * @return      16-bit value corresponding to desired baudrate
 */
static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t Baudrate) {
	return (uint16_t)(PeriphClk + (Baudrate/2U))/Baudrate;
}

int __io_putchar(int ch) {
	uart2_write(ch);
	return ch;
}