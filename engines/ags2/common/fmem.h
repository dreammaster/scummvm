/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef AGS2_COMMON_FMEM_H
#define AGS2_COMMON_FMEM_H

#include "common/scummsys.h"

namespace AGS2 {

struct FMEM {
	char *data;
	long len;                     // length of data in array
	long size;                    // size of data array allocated
	long magic;
	long pos;                     // current seeked position
};

extern FMEM *fmem_create();
extern FMEM *fmem_open(const char *);
extern void fmem_close(FMEM *);
extern void fmem_write(const char *, long, FMEM *);
extern void fmem_putc(char, FMEM *);
extern void fmem_puts(char *, FMEM *);
extern int fmem_getc(FMEM *);
extern int fmem_peekc(FMEM *);
extern int fmem_eof(FMEM *);
extern void fmem_gets(FMEM *, char *);

} // namespace AGS2

#endif
