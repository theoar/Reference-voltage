/*
 * uart_bus.h
 *
 *  Created on: 21-10-2017
 *      Author: theoar
 */

#ifndef UART_BUS_H_
#define UART_BUS_H_

#include <stdio.h>

FILE uart_output;
FILE uart_input;

#ifdef __cplusplus
extern "C"{
void uart_putchar(char c, FILE *stream);
void uart_putchar_(char c);
char uart_getchar(FILE *stream);
void init_uart(void);

}
#endif

#endif /* UART_BUS_H_ */
