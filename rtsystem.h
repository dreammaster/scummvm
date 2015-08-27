/* RogueVM - RPG Engines
 *
 * RogueVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

//
// AESOP interpreter system services
//

#ifndef AESOP_RTSYSTEM_H
#define AESOP_RTSYSTEM_H

#include "common/stream.h"

namespace Aesop {

#define TF_BUFSIZE 4096    // size of buffer allocated by file functions

struct TF_class {
   HRES hbuf;
   BYTE *buffer;
   WORD p;
   Common::SeekableReadStream *file;
   WORD mode;
   LONG len;
   LONG pos;
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

WORD copy_file(BYTE *src_filename, BYTE *dest_filename);
WORD delete_file(BYTE *filename);
ULONG file_time(BYTE *filename);

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

LONG file_size(BYTE *filename);
BYTE *read_file(BYTE *filename, void *dest);
WORD write_file(BYTE *filename, void *buf, ULONG len);
WORD append_file(BYTE *filename, void *buf, ULONG len);

//
// Memory heap management
//

void mem_init(void);
void mem_shutdown(void);
ULONG mem_avail(void);
void *mem_alloc(ULONG bytes);
BYTE *str_alloc(BYTE *string);
void mem_free(void *ptr);
ULONG mem_headroom(void);

//
// Misc. routines
//

LONG ascnum(const char *string);
void opcode_fault(void *PC, void *stk);
void abend(const char *msg, ...);
void curpos(WORD *x, WORD *y);
void locate(WORD x, WORD y);

} // End of namespace Aesop

#endif

