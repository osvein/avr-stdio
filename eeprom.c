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

#include <stdint.h>
#include <stdio.h>
#include <avr/eeprom.h>

#include "eeprom.h"

static int eeprom_putc(char, FILE *);
static int eeprom_getc(FILE *);
static FILE eeprom_stream = FDEV_SETUP_STREAM(eeprom_putc, eeprom_getc, _FDEV_SETUP_RW);

static uint8_t *eeprom_ptr_min;
static const uint8_t *eeprom_ptr_max;
static uint8_t *eeprom_ptr_put;
static const uint8_t *eeprom_ptr_get;

FILE *eeprom_open(char *min, const char *max, char *initial_put, const char *initial_get) {
	eeprom_ptr_min = (uint8_t *)min;
	eeprom_ptr_max = (const uint8_t *)max;
	eeprom_ptr_put = (uint8_t *)initial_put;
	eeprom_ptr_get = (const uint8_t *)initial_get;

	return &eeprom_stream;
}

int eeprom_putc(char c, FILE *stream) {
	if (++eeprom_ptr_put > eeprom_ptr_max) {
		eeprom_ptr_put = eeprom_ptr_min;
	}
	eeprom_write_byte(eeprom_ptr_put, (uint8_t)c);
	return c;
}

int eeprom_getc(FILE *stream) {
	if (++eeprom_ptr_get > eeprom_ptr_max) {
		eeprom_ptr_get = eeprom_ptr_min;
	}
	return (char)eeprom_read_byte(eeprom_ptr_get);
}
