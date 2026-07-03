/**
 * @file 		uart.h
 * @brief 		Contains function prototypes and includes for UART driver
 * @author 		Marcos E. Mancia Jr.
 * @date 		2026-07-02
 * @version 	1.5
 */
#ifndef UART_H_
#define UART_H_

#include <stdint.h>

/***** FUNCTION PROTOTYPES *****/
void uart2_tx_init(void);
void uart2_rx_init(void);
void uart2_rxtx_init(void);
void uart2_rx_interrupt_init(void);
void uart2_dma1_init(uint32_t src, uint32_t dst, uint32_t len);
void my_put(const char *text);

void uart2_tx_test(void);
void uart2_rx_test(void);
void uart2_rxtx_test(void);
void uart2_rx_interrupt_test(void);
void uart2_dma1_test(void);

#endif /* UART_H_ */