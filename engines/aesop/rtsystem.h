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

#ifndef AESOP_RTSYSTEM_H
#define AESOP_RTSYSTEM_H

#include "common/stream.h"
#include "aesop/defs.h"

namespace Aesop {

#define TF_BUFSIZE 4096    // size of buffer allocated by file functions

struct TF_class {
	HRES hbuf;
	BYTE *buffer;
	WORD p;
	Common::Stream *file;
	WORD mode;
	int32 len;
	int32 pos;
};

#define TF_WRITE 0
#define TF_READ  1

#define isnum(x) (((x) == '-') || (((x) >= '0') && ((x) <= '9')))

	//
	// Universal disk error codes
	//

#define NO_ERROR        0
#define IO_ERROR        1
#define OUT_OF_MEMORY   2
#define FILE_NOT_FOUND  3
#define CANT_WRITE_FILE 4
#define CANT_READ_FILE  5
#define DISK_FULL       6

//
// General file management
//

WORD copy_file(const char *src_filename, const char *dest_filename);
WORD delete_file(const char *filename);
uint32 file_time(const char *filename);

//
// Text file management
//

TF_class *TF_construct(const char *filename, WORD oflag);
WORD TF_destroy(TF_class *TF);
WORD TF_wchar(TF_class *TF, BYTE ch);
BYTE TF_rchar(TF_class *TF);
WORD TF_readln(TF_class *TF, char *buffer, WORD maxlen);
WORD TF_writeln(TF_class *TF, const char *buffer);

//
// Binary file management
//

int32 file_size(BYTE *filename);
BYTE *read_file(BYTE *filename, void *dest);
WORD write_file(BYTE *filename, void *buf, uint32 len);
WORD append_file(BYTE *filename, void *buf, uint32 len);

//
// Memory heap management
//

void mem_init(void);
void mem_shutdown(void);
uint32 mem_avail(void);
void *mem_alloc(uint32 bytes);
char *str_alloc(const char *string);
void mem_free(void *ptr);
uint32 mem_headroom(void);

//
// Misc. routines
//

int32 ascnum(const char *string);
void opcode_fault(void *PC, void *stk);
void abend(const char *msg, ...);
void curpos(WORD *x, WORD *y);
void locate(WORD x, WORD y);

} // namespace Aesop

#endif
