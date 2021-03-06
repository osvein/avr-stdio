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

#include <stdbool.h>
#include <stdio.h>

#include "ring.h"

#ifndef USART_H
#define USART_H

#define USART_MODE_SYNC	0x03
#define USART_MODE_SYNC_NONE	0x00 // asynchronous operation
#define USART_MODE_SYNC_RTFR	0x02 // synchronous operation, rising on transmit, falling on receive
#define USART_MODE_SYNC_FTRR	0x03 // synchronous operation, falling on transmit, rising on receive
#define USART_MODE_PARITY	0x0C
#define USART_MODE_PARITY_NONE	0x00 // no parity generation/check
#define USART_MODE_PARITY_EVEN	0x08 // even parity generation/check
#define USART_MODE_PARITY_ODD	0x0C // odd parity generation/check

#ifdef __cplusplus
extern "C" {
#endif

FILE *usart_open(char mode, unsigned int baudrate, bool doublespeed, unsigned int stopsize, unsigned int charsize, volatile struct ring_buf *volatile buf_rx, volatile struct ring_buf *volatile buf_tx);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // USART_H
