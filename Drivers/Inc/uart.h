/**
 * @file 		uart.h
 * @brief 		Contains function prototypes for UART driver
 * @author 		Marcos E. Mancia Jr.
 * @date 		2026-07-01
 * @version 	1.4
 */
#ifndef UART_H_
#define UART_H_

/***** FUNCTION PROTOTYPES *****/
void uart2_tx_init(void);
void uart2_rx_init(void);
void uart2_rxtx_init(void);
void uart2_rx_interrupt_init(void);
void my_put(const char *text);

void uart2_tx_test(void);
void uart2_rx_test(void);
void uart2_rxtx_test(void);
void uart2_rx_interrupt_test(void);

#endif /* UART_H_ */