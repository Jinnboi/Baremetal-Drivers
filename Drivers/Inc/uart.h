/**
 * @file 		uart.h
 * @brief 		Contains function prototypes and necessary includes for UART driver
 * @author 		Marcos E. Mancia Jr.
 * @date 		2026-06-25
 * @version 	1.0
 */
#ifndef UART_H_
#define UART_H_

#include "stm32f411xe.h"
#include <stdint.h>

void uart2_tx_init(void);
void uart2_rx_init(void);
void uart2_rxtx_init(void);

void uart2_tx_test(void);
void uart2_rx_test(void);
void uart2_rxtx_test(void);

#endif /* UART_H_ */