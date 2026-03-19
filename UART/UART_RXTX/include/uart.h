/**
 * @file    uart.h
 * @author  Marcos E. Mancia Jr.
 * @version 1.0
 * @date    2026-03-19
 **/

#ifndef UART_H_
#define UART_H_

#include <stdint.h>
#include "stm32f4xx.h"

void uart2_rxtx_init(void);
void uart2_write(int ch);
char uart2_read(void);

#endif /* UART_H_ */
