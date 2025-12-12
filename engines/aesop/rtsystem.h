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

namespace Aesop {

#ifdef __cplusplus
extern "C" {
#endif

#define TF_BUFSIZE 4096    // size of buffer allocated by file functions

	typedef struct
	{
		HRES hbuf;
		BYTE *buffer;
		WORD p;
		WORD file;
		WORD mode;
		LONG len;
		LONG pos;
	}
	TF_class;

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

	WORD cdecl copy_file(BYTE *src_filename, BYTE *dest_filename);
	WORD cdecl delete_file(BYTE *filename);
	ULONG cdecl file_time(BYTE *filename);

	//
	// Text file management
	//

	TF_class *cdecl TF_construct(BYTE *filename, WORD oflag);
	WORD cdecl TF_destroy(TF_class *TF);
	WORD cdecl TF_wchar(TF_class *TF, BYTE ch);
	BYTE cdecl TF_rchar(TF_class *TF);
	WORD cdecl TF_readln(TF_class *TF, BYTE *buffer, WORD maxlen);
	WORD cdecl TF_writeln(TF_class *TF, BYTE *buffer);

	//
	// Binary file management
	//

	LONG file_size(BYTE *filename);
	BYTE *cdecl read_file(BYTE *filename, void *dest);
	WORD cdecl write_file(BYTE *filename, void *buf, ULONG len);
	WORD cdecl append_file(BYTE *filename, void *buf, ULONG len);

	//
	// Memory heap management
	//

	void cdecl mem_init(void);
	void cdecl mem_shutdown(void);
	ULONG cdecl mem_avail(void);
	void *cdecl mem_alloc(ULONG bytes);
	BYTE *cdecl str_alloc(BYTE *string);
	void cdecl mem_free(void *ptr);
	ULONG cdecl mem_headroom(void);

	//
	// Misc. routines
	//

	LONG cdecl ascnum(BYTE *string);
	void cdecl opcode_fault(void *PC, void *stk);
	void cdecl abend(char *msg, ...);
	void cdecl curpos(WORD *x, WORD *y);
	void cdecl locate(WORD x, WORD y);

#ifdef __cplusplus
}
#endif

} // namespace Aesop

#endif
