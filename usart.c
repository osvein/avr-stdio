/*
avr-stdio - AVR peripherals via stdio
Copyright (C) 2015  Oskar Sveinsen

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/sfr_defs.h>

#include "ring.h"
#include "usart.h"

static int usart_putc(char, FILE *);
static int usart_getc(FILE *);
static FILE usart_stream = FDEV_SETUP_STREAM(usart_putc, usart_getc, _FDEV_SETUP_RW);

static volatile struct ring_buf *volatile usart_buf_rx;
static volatile struct ring_buf *volatile usart_buf_tx;

FILE *usart_open(char mode, unsigned int baudrate, bool doublespeed, unsigned int stopsize, unsigned int charsize, volatile struct ring_buf *volatile buf_rx, volatile struct ring_buf *volatile buf_tx) {
	UCSR0C &= ~_BV(UMSEL01);
	switch (mode & USART_MODE_SYNC) {
		case USART_MODE_SYNC_NONE:
			UCSR0C &= ~_BV(UMSEL00);
			break;
		case USART_MODE_SYNC_RTFR:
			UCSR0C |= _BV(UMSEL00);
			UCSR0C &= ~_BV(UCPOL0);
			break;
		case USART_MODE_SYNC_FTRR:
			UCSR0C |= _BV(UMSEL00) | _BV(UCPOL0);
			break;
		default:
			return NULL;
	}

	switch (mode & USART_MODE_PARITY) {
		case USART_MODE_PARITY_NONE:
			UCSR0C &= ~(_BV(UPM00) | _BV(UPM01));
			break;
		case USART_MODE_PARITY_EVEN:
			UCSR0C &= ~_BV(UPM00);
			UCSR0C |= _BV(UPM01);
			break;
		case USART_MODE_PARITY_ODD:
			UCSR0C |= _BV(UPM00) | _BV(UPM01);
			break;
		default:
			return NULL;
	}

	if (baudrate > 0) {
		UBRR0L = (char)baudrate;
		UBRR0H = (char)(baudrate >> 8);

		if (doublespeed && mode == USART_MODE_SYNC_NONE) {
			UCSR0A |= _BV(U2X0);
		}
		else {
			UCSR0A &= ~_BV(U2X0);
		}
	}

	switch (stopsize) {
		case 1:
			UCSR0C &= ~_BV(USBS0);
			break;
		case 2:
			UCSR0C |= _BV(USBS0);
			break;
		default:
			return NULL;
	}

	switch (charsize) {
		case 5:
			UCSR0C &= ~(_BV(UCSZ00) | _BV(UCSZ01));
			UCSR0B &= ~_BV(UCSZ02);
			break;
		case 6:
			UCSR0C |= _BV(UCSZ00);
			UCSR0C &= ~_BV(UCSZ01);
			UCSR0B &= ~_BV(UCSZ02);
			break;
		case 7:
			UCSR0C &= ~_BV(UCSZ00);
			UCSR0C |= _BV(UCSZ01);
			UCSR0B &= ~_BV(UCSZ02);
			break;
		case 8:
			UCSR0C |= _BV(UCSZ00) | _BV(UCSZ01);
			UCSR0B &= ~_BV(UCSZ02);
			break;
//		case 9:
//			UCSR0C |= _BV(UCSZ00) | _BV(UCSZ01);
//			UCSR0B |= _BV(UCSZ02);
//			break;
		default:
			return NULL;
	}

	usart_buf_rx = buf_rx;
	if (usart_buf_rx->limit_tail == NULL) {
		UCSR0B &= ~_BV(RXCIE0);
	}
	else {
		UCSR0B |= _BV(RXCIE0);
	}

	usart_buf_tx = buf_tx;
	if (usart_buf_tx->limit_tail == NULL) {
		UCSR0B &= ~_BV(UDRIE0);
	}
	else {
		UCSR0B |= _BV(UDRIE0);
	}

	return &usart_stream;
}

int usart_putc(char c, FILE *stream) {
	if (usart_buf_tx->limit_tail == NULL) {
		loop_until_bit_is_set(UCSR0A, UDRE0);
		UDR0 = c;
		return c; // make sure to return c, and not UDR0
	}
	else {
		return ring_putc(c, usart_buf_tx);
	}
}

int usart_getc(FILE *stream) {
	if (usart_buf_rx->limit_tail == NULL) {
		loop_until_bit_is_set(UCSR0A, RXC0);
		return UDR0;
	}
	else {
		return ring_getc(usart_buf_rx);
	}
}

ISR(USART_UDRE_vect) {
	UDR0 = ring_getc(usart_buf_tx);
}

ISR(USART_RX_vect) {
	ring_putc(UDR0, usart_buf_rx);
}
