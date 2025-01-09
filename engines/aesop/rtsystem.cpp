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

#include "common/textconsole.h"
#include "common/str.h"
#include "aesop/rtsystem.h"
#include "common/file.h"
#include "common/savefile.h"
#include "common/system.h"
#include "aesop/gil2vfx.h"
#include "aesop/defs.h"
#include "aesop/ail_sound.h"
#include "aesop/shared.h"
#include "aesop/rtmsg.h"
#include "aesop/rtres.h"
#include "aesop/rt.h"
#include "aesop/rtcode.h"
#include "aesop/interface.h"
#include "aesop/event.h"
#include "aesop/sound.h"
#include "aesop/graphics.h"
#include "aesop/system/ail32.h"
#include "aesop/system/files.h"
#include "aesop/system/vfx.h"

namespace Aesop {

static void breakpoint() {}

ULONG headroom;
ULONG checksum;
ULONG init;

WORD disk_err;

void mem_init() {
	headroom = init = mem_avail();

	checksum = 0L;
}

void mem_shutdown() {
	ULONG end;

	end = mem_avail();
}

ULONG mem_avail() {
	return 999999;
}

ULONG mem_headroom() {
	return headroom;
}

void *mem_alloc(ULONG bytes) {
	ULONG left;
	void *ptr;

	ptr = (void *)malloc(bytes);

	left = mem_avail();
	if (left < headroom) headroom = left;

	if (ptr == NULL)
		abend(MSG_OODM);

	checksum ^= (ULONG)ptr;

	return ptr;
}

BYTE *str_alloc(BYTE *str) {
	return (BYTE *)scumm_strdup((const char *)str);
}

void mem_free(void *ptr) {
	checksum ^= (ULONG)ptr;

	free(ptr);
}

/***************************************************/
//
// Convert string to number, returning -1 if not valid numeric
// string
//
// Skip leading whitespace; handles unary -/+ operators
//
// Accepts binary numbers with '0b' prefix,
// hexadecimal numbers with '0x' prefix; decimal numbers
// handled via atol() library function for speed
// 
// Accepts single ASCII characters with '\'' prefix
// 
/***************************************************/

LONG ascnum(BYTE *string) {
	LONG i, j, len, base, neg, chr;
	LONG total;

	while (Common::isSpace(*string)) string++;

	neg = 0;
	switch (*string)
	{
	case '-': neg = 1; string++; break;
	case '+':          string++; break;
	}

	if (*string == '\'')
		return (LONG)(*(string + 1));

	switch (*(UWORD *)string)
	{
	case 'x0': base = 16; string += 2; break;
	case 'b0': base = 2;  string += 2; break;
	default:   base = 10; break;
	}

	if (base == 10)
		if (Common::isDigit(*string))
			return neg ? -atol(string) : atol(string);
		else
			return -1;

	total = 0L;
	len = strlen(string);

	for (i = 0; i < len; i++)
	{
		chr = toupper(string[i]);

		for (j = 0; j < base; j++)
			if (chr == "0123456789ABCDEF"[j])
			{
				total = (total * base) + j;
				break;
			}

		if (j == base) return -1;
	}

	return total;
}

/***************************************************/
//
// AESOP interpreter opcode fault handler
//
/***************************************************/

void opcode_fault(void *PC, void *stk) {
	abend(MSG_IAO, *(unsigned char *)PC, PC, stk);
}

/***************************************************/
//
// Abnormal program termination handler
//
// Give debugger a chance to return to the failing function, else
// shut everything down gracefully and exit to DOS
//
/***************************************************/

void abend(const char *msg, ...) {
	va_list argptr;
	WORD recover;
	WORD x, y;

	curpos(&x, &y);
	if (y > 25)
		locate(0, 0);

	va_start(argptr, msg);
	Common::String str = Common::String::vformat(msg, argptr);
	va_end(argptr);

	if (!str.empty())
		error("%s", str.c_str());

	recover = 0;

	breakpoint();

	if (!recover) {
		shutdown_sound();
		shutdown_interface();
		AIL_shutdown("Abend");
		GIL2VFX_shutdown_driver();

		error("Abend");
	}
}

/***************************************************/
//
// Open a text file for reading/writing
//
/***************************************************/

TF_class *TF_construct(BYTE *filename, WORD oflag) {
	TF_class *TF;
	Common::Stream *file = nullptr;
	HRES hbuf;

	file = open(filename, oflag);
	if (!file)
		return NULL;

	hbuf = RTR_alloc(RTR, TF_BUFSIZE, DA_FIXED | DA_PRECIOUS);
	if (hbuf == (uint32)-1)
		return NULL;

	TF = (TF_class *)mem_alloc(sizeof(TF_class));

	TF->file = file;
	TF->hbuf = hbuf;
	TF->buffer = (BYTE *)RTR_addr(TF->hbuf);
	TF->p = 0;
	TF->mode = oflag;
	TF->len = (oflag == TF_WRITE) ? 0 : dynamic_cast<Common::SeekableReadStream *>(file)->size();
	TF->pos = 0L;

	if (oflag != TF_WRITE)
		read(TF->file, TF->buffer, TF_BUFSIZE);

	return TF;
}

/***************************************************/
//
// Close text file/dealloc buffer
//
// Return 0 if write attempt failed
//
/***************************************************/

WORD TF_destroy(TF_class *TF) {
	WORD e, f;

	e = f = TF->p;

	delete TF->file;

	RTR_free(RTR, TF->hbuf);
	mem_free(TF);

	return (e == f);
}

/***************************************************/
//
// Write character to text file
//
// Return 0 if write attempt failed
//
/***************************************************/

WORD TF_wchar(TF_class *TF, BYTE ch) {
	TF->buffer[TF->p++] = ch;

	if (TF->p == TF_BUFSIZE) {
		TF->p = 0;
		if (dynamic_cast<Common::WriteStream *>(TF->file)->write(TF->buffer, TF_BUFSIZE) != TF_BUFSIZE)
			return 0;
	}

	return 1;
}

/***************************************************/
//
// Read character from text file
//
// Return 0 if EOF reached
//
/***************************************************/

BYTE TF_rchar(TF_class *TF) {
	if (TF->pos >= TF->len)
		return 0;

	++TF->pos;

	if (TF->p != TF_BUFSIZE)
		return TF->buffer[TF->p++];

	dynamic_cast<Common::ReadStream *>(TF->file)->read(TF->buffer, TF_BUFSIZE);

	TF->p = 1;
	return TF->buffer[0];
}

/***************************************************/
//
// Read text file line into buffer
//
// \r's are skipped
// \n's are truncated, replaced with \0
//
// Blank lines are ignored
//
// Return 0 if EOF reached
//
/***************************************************/

WORD TF_readln(TF_class *TF, BYTE *buffer, WORD maxlen) {
	WORD b, c;

	do
	{
		b = 0;

		while (b != maxlen - 1)
		{
			c = TF_rchar(TF);

			if (c == '\n') break;
			if (c == '\r') continue;

			buffer[b++] = c;

			if (!c) return 0;
		}

		if (b == maxlen - 1)
			while ((c = TF_rchar(TF)) != '\n')
				if (!c) return 0;

		buffer[b] = 0;
	} while (!strlen(buffer));

	return 1;
}

/***************************************************/
//
// Write buffer line to text file
// Return 0 if write attempt failed
//
// \r\n added at end of each buffer line
//
/***************************************************/

WORD TF_writeln(TF_class *TF, const BYTE *buffer) {
	WORD b, c;

	b = 0;

	while ((c = buffer[b++]) != 0)
		if (!TF_wchar(TF, c)) return 0;

	TF_wchar(TF, '\r');
	return TF_wchar(TF, '\n');
}

/***************************************************/
//
// Delete a file
//
// Return 0 if file did not exist, -1 if deletion failed,
// else 1 if deleted OK
//
/***************************************************/

WORD delete_file(BYTE *filename) {
	return g_system->getSavefileManager()->removeSavefile(filename) ? 1 : -1;
}

/***************************************************/
//
// Copy a file
//
// Return 0 if source file not found, -1 if copy error occurred,
// else 1 if copied OK
//
/***************************************************/

WORD copy_file(BYTE *src_filename, BYTE *dest_filename) {
	Common::File f;
	if (!f.open(src_filename))
		return 0;

	Common::OutSaveFile *dest = g_system->getSavefileManager()->openForSaving(dest_filename, false);
	assert(dest);
	dest->writeStream(&f);
	dest->finalize();

	delete dest;
	f.close();

	return 1;
}

/****************************************************************************/
//
// Determine the size in bytes of a file
//
/****************************************************************************/

LONG file_size(BYTE *filename) {
	Common::InSaveFile *sf;
	Common::File f;
	size_t size = 0;

	if ((sf = g_system->getSavefileManager()->openForLoading(filename)) != nullptr) {
		size = sf->size();
		delete sf;
	} else if (f.open(filename)) {
		size = f.size();
	}

	return size;
}

/****************************************************************************/
//
// Read a file directly into memory
//
/****************************************************************************/

BYTE *read_file(BYTE *filename, void *dest) {
	WORD i;
	Common::Stream *handle;
	ULONG len;
	BYTE *buf, *mem;

	disk_err = 0;

	len = file_size(filename);
	if (len == -1L)
	{
		disk_err = FILE_NOT_FOUND;
		return NULL;
	}

	buf = mem = (dest == NULL) ? (BYTE *)mem_alloc(len) : (BYTE *)dest;

	if (buf == NULL)
	{
		disk_err = OUT_OF_MEMORY;
		return NULL;
	}

	handle = open(filename, O_RDONLY | O_BINARY);
	if (!handle) {
		mem_free(mem);
		disk_err = FILE_NOT_FOUND;
		return NULL;
	}

	while (len >= DOS_BUFFSIZE) {
		i = read(handle, buf, DOS_BUFFSIZE);
		if (i != (WORD)DOS_BUFFSIZE) {
			mem_free(mem);
			disk_err = CANT_READ_FILE;
			return NULL;
		}

		len -= DOS_BUFFSIZE;
		buf = (BYTE *)add_ptr(buf, DOS_BUFFSIZE);
	}

	i = read(handle, buf, (UWORD)len);
	if (i != (UWORD)len)
	{
		mem_free(mem);
		disk_err = CANT_READ_FILE;
		return NULL;
	}

	close(handle);
	return mem;
}

/****************************************************************************/
//
// Write a binary file to disk
// 
/****************************************************************************/

WORD write_file(BYTE *filename, void *buf, ULONG len) {
	WORD i;
	Common::Stream *handle;

	disk_err = 0;

	handle = open(filename, O_CREAT | O_RDWR | O_TRUNC | O_BINARY);
	if (!handle) {
		disk_err = CANT_WRITE_FILE;
		return 0;
	}

	while (len >= DOS_BUFFSIZE) {
		i = write(handle, buf, DOS_BUFFSIZE);
		if (i == -1)
		{
			disk_err = CANT_WRITE_FILE;
			return 0;
		}
		if (i != (WORD)DOS_BUFFSIZE)
		{
			disk_err = DISK_FULL;
			return 0;
		}
		len -= DOS_BUFFSIZE;
		buf = add_ptr(buf, DOS_BUFFSIZE);
	}

	i = write(handle, buf, (UWORD)len);
	if (i == -1)
	{
		disk_err = CANT_WRITE_FILE;
		return 0;
	}
	if (i != (UWORD)len)
	{
		disk_err = DISK_FULL;
		return 0;
	}

	close(handle);

	return 1;
}

/****************************************************************************/
//
// Append binary data to an existing file
//
/****************************************************************************/

WORD append_file(BYTE *filename, void *buf, ULONG len) {
	WORD i;
	Common::Stream *handle;

	disk_err = 0;

	handle = open(filename, O_APPEND | O_RDWR | O_BINARY);
	if (!handle) {
		disk_err = FILE_NOT_FOUND;
		return 0;
	}

	while (len >= DOS_BUFFSIZE)
	{
		i = write(handle, buf, DOS_BUFFSIZE);
		if (i == -1)
		{
			disk_err = CANT_WRITE_FILE;
			return 0;
		}
		if (i != (WORD)DOS_BUFFSIZE)
		{
			disk_err = DISK_FULL;
			return 0;
		}
		len -= DOS_BUFFSIZE;
		buf = add_ptr(buf, DOS_BUFFSIZE);
	}

	i = write(handle, buf, (UWORD)len);
	if (i == -1)
	{
		disk_err = CANT_WRITE_FILE;
		return 0;
	}
	if (i != (UWORD)len)
	{
		disk_err = DISK_FULL;
		return 0;
	}

	close(handle);

	return 1;
}

/****************************************************************************/
//
// Get file's timestamp
//
/****************************************************************************/

ULONG file_time(BYTE *filename) {
	warning("TODO: file_time");

	return 0;
}

/****************************************************************************/
//
// Position text cursor
//
/****************************************************************************/

void locate(WORD x, WORD y) {
	warning("TODO: locate");
}

/****************************************************************************/
//
// Get text cursor location
//
/****************************************************************************/

void curpos(WORD *x, WORD *y) {
	warning("TODO: curpos");
	*x = 0;
	*y = 0;
}

} // namespace Aesop
