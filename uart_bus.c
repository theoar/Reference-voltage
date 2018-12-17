/*
 * uart_bus.c
 *
 *  Created on: 21-10-2017
 *      Author: theoar
 */

#define BAUD 9600

#include <util/setbaud.h>
#include <avr/io.h>

#include "uart_bus.h"

void uart_putchar(char c, FILE *stream)
{
	if (c == '\n')
		uart_putchar('\r', stream);

	loop_until_bit_is_set(UCSR0A, UDRE0);
	UDR0 = c;
}

void uart_putchar_(char c)
{
	if (c == '\n')
		uart_putchar_('\r');

	loop_until_bit_is_set(UCSR0A, UDRE0);
	UDR0 = c;
}

char uart_getchar(FILE *stream)
{
	loop_until_bit_is_set(UCSR0A, RXC0);
	/* Wait until data exists. */
	return UDR0;
}

void init_uart(void)
{

	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;

#if USE_2X
	UCSR0A |= _BV(U2X0);
#else
	UCSR0A &= ~(_BV(U2X0));
#endif

	UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); /* 8-bit data */
	UCSR0B = _BV(RXEN0) | _BV(TXEN0); /* Enable RX and TX */

	FILE File_out = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);
	FILE File_in = FDEV_SETUP_STREAM(NULL, uart_getchar, _FDEV_SETUP_READ);

	uart_output = File_out;
	uart_input = File_in;

	stdout = &uart_output;
	stdin = &uart_input;
}


