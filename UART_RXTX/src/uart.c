/**
 * @file    uart.c
 * @author  Marcos E. Mancia Jr.
 * @brief   UART driver implementation for STM32F411RET6.
 * All info found in Data Sheet, Reference Manual, and User Guide
 * @version 1.0
 * @date    2026-03-19
 **/

#include "uart.h"

#define GPIOAEN				(1U<<0)
#define UART2EN				(1U<<17)

#define CR1_RE				(1U<<2)
#define CR1_TE				(1U<<3)
#define CR1_UE				(1U<<13)

#define SR_RXNE				(1U<<5)
#define SR_TXE				(1U<<7)

#define SYS_FREQ			16000000
#define APB1_CLK			SYS_FREQ

#define UART_BAUDRATE		115200

static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t Baudrate);
static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t Baudrate);

void uart2_rxtx_init(void)
{
	/*************Configure UART GPIO pin*****************/
	/*Enable CLK access to GPIOA*/
	RCC->AHB1ENR |= GPIOAEN;

	/*Set PA2 mode to alternate function mode*/
	GPIOA->MODER |=  (1U<<5);
	GPIOA->MODER &= ~(1U<<4);

	/*Set PA3 mode to alternate function mode*/
	GPIOA->MODER |=  (1U<<7);
	GPIOA->MODER &= ~(1U<<6);

	/*Set PA2 alternate function type to UART_TX (AF07)*/
	GPIOA->AFR[0] &= ~(1U<<11);
	GPIOA->AFR[0] |=  (7U<<8);

	/*Set PA3 alternate function type to UART_RX (AF07)*/
	GPIOA->AFR[0] &= ~(1U<<15);
	GPIOA->AFR[0] |=  (7U<<12);

	/*************Configure UART module*****************/
	/*Enable CLK access to UART2*/
	RCC->APB1ENR |= UART2EN;

	/*Configure baudrate*/
	uart_set_baudrate(USART2, APB1_CLK, UART_BAUDRATE);

	/*Configure transfer direction*/
	USART2->CR1 = (CR1_RE | CR1_TE);

	/*Enable UART2 module*/
	USART2->CR1 |= CR1_UE;
}

void uart2_write(int ch)
{
	/*Ensure transmit data register is empty*/
	while(!(USART2->SR & SR_TXE)) { }

	/*Write to transmit data register*/
	USART2->DR = (ch & 0xFF);
}

char uart2_read(void)
{
	/*Make sure the receive data register is NOT empty*/
	while(!(USART2->SR & SR_RXNE)) {}

	/*Read data*/
	return USART2->DR;
}

static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t Baudrate)
{
	// Baudrate Register
	USARTx->BRR = compute_uart_bd(PeriphClk, Baudrate);
}

static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t Baudrate)
{
	return (PeriphClk + (Baudrate/2U))/Baudrate;
}
