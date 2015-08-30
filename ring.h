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

#ifndef RING_H
#define RING_H

#ifdef __cplusplus
extern "C" {
#endif

struct ring_buf {
    volatile char *const limit_tail;
    const char *const limit_head;
    volatile char *tail; // where the next character is to be read
    volatile char *head; // where the previous character was written
};

int ring_putc(char, struct ring_buf *);
int ring_getc(struct ring_buf *);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // RING_H
