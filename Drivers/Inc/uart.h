/**
 * @file 		uart.h
 * @brief 		Contains function prototypes for UART driver
 * @author 		Marcos E. Mancia Jr.
 * @date 		2026-06-25
 * @version 	1.0
 */
#ifndef UART_H_
#define UART_H_

void uart2_tx_init(void);
void uart2_rx_init(void);
void uart2_rxtx_init(void);

void uart2_tx_test(void);
void uart2_rx_test(void);
void uart2_rxtx_test(void);

#endif /* UART_H_ */