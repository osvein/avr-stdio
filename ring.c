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

#include "ring.h"

int ring_putc(char c, struct ring_buf *buf) {
    buf->head++;
    if (buf->head > buf->limit_head || buf->head < buf->limit_tail) {
        buf->head = buf->limit_tail;
    }
    return *(buf->head) = c;
}

int ring_getc(struct ring_buf *buf) {
    char c = *(buf->tail++);
    if (buf->tail > buf->limit_head || buf->tail < buf->limit_tail) {
        buf->tail = buf->limit_tail;
    }
    return c;
}
