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

#include "common/system.h"
#include "common/savefile.h"
#include "common/textconsole.h"
#include "aesop/lib/vfx.h"
#include "aesop/ail32.h"
#include "aesop/gil2vfx.h"
#include "aesop/defs.h"
#include "aesop/shared.h"
#include "aesop/rtsystem.h"
#include "aesop/rtmsg.h"
#include "aesop/rtres.h"
#include "aesop/rt.h"
#include "aesop/rtcode.h"
#include "aesop/interface.h"
#include "aesop/event.h"
#include "aesop/sound.h"
#include "aesop/graphics.h"

namespace Aesop {

void breakpoint(void);
#pragma aux breakpoint = "int 3";

uint32 headroom;
uint32 checksum;
uint32 init;

WORD disk_err;

void mem_init(void) {
	headroom = init = mem_avail();

	checksum = 0L;
}

void mem_shutdown(void) {
	uint32 end;

	end = mem_avail();

	if ((init != end) || (checksum != 0L))
	{
		//    abend(MSG_UH);    (unbalanced heap normal in flat-model version)
	}
}

uint32 mem_avail(void) {
	return 999999;
}

uint32 mem_headroom(void) {
	return headroom;
}

void *mem_alloc(uint32 bytes) {
	uint32 left;
	void *ptr;

	ptr = (void *)malloc(bytes);

	left = mem_avail();
	if (left < headroom) headroom = left;

	if (ptr == NULL)
		abend(MSG_OODM);

	checksum ^= (uint32)ptr;

	return ptr;
}

char *str_alloc(const char *str) {
	return scumm_strdup(str);
}

void mem_free(void *ptr) {
	checksum ^= (uint32)ptr;

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

int32 ascnum(const char *string) {
	int32 i, j, len, base, neg, chr;
	int32 total;

	while (Common::isSpace(*string)) string++;

	neg = 0;
	switch (*string) {
	case '-': neg = 1; string++; break;
	case '+':          string++; break;
	}

	if (*string == '\'')
		return (int32)(*(string + 1));

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
// LUM 070203: added support for storing the error into a file
/***************************************************/

void abend(const char *msg, ...) {
	if (msg != NULL) {
		va_list argptr;
		va_start(argptr, msg);
		Common::String str = Common::String::vformat(msg, argptr);
		va_end(argptr);

		error("%s", str.c_str());
	}

	breakpoint();

	shutdown_sound();
	shutdown_interface();
	AIL_shutdown("Abend");
	GIL2VFX_shutdown_driver();

	error("Abnormal end");
}

/***************************************************/
//
// Open a text file for reading/writing
//
/***************************************************/

TF_class *TF_construct(const char *filename, WORD oflag) {
	TF_class *TF;
	Common::InSaveFile *isf = nullptr;
	Common::OutSaveFile *osf = nullptr;
	Common::Stream *file;
	HRES hbuf;

	if (oflag == TF_WRITE) {
		osf = g_system->getSavefileManager()->openForSaving(filename);
		file = osf;
	} else {
		isf = g_system->getSavefileManager()->openForLoading(filename);
		file = isf;
	}

	if (file == nullptr)
		return NULL;

	hbuf = RTR_alloc(RTR, TF_BUFSIZE, DA_FIXED | DA_PRECIOUS);
	if (hbuf == (HRES)-1) return NULL;

	TF = (TF_class *)mem_alloc(sizeof(TF_class));

	TF->file = file;
	TF->hbuf = hbuf;
	TF->buffer = (BYTE *)RTR_addr(TF->hbuf);
	TF->p = 0;
	TF->mode = oflag;
	TF->len = isf ? isf->size() : osf->size();
	TF->pos = 0L;

	if (isf)
		isf->read(TF->buffer, TF_BUFSIZE);

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

	Common::WriteStream *ws = dynamic_cast<Common::WriteStream *>(TF->file);
	if (ws && TF->p != 0)
		ws->write(TF->buffer, TF->p);

	delete TF->file;
	TF->file = nullptr;

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

		Common::WriteStream *ws = dynamic_cast<Common::WriteStream *>(TF->file);
		assert(ws);
		ws->write(TF->buffer, TF_BUFSIZE);
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

	Common::SeekableReadStream *rs = dynamic_cast<Common::SeekableReadStream *>(TF->file);
	assert(rs);
	rs->read(TF->buffer, TF_BUFSIZE);

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

WORD TF_readln(TF_class *TF, char *buffer, WORD maxlen) {
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

WORD TF_writeln(TF_class *TF, BYTE *buffer) {
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

WORD delete_file(const char *filename) {
	return g_system->getSavefileManager()->removeSavefile(filename) ? 1 : 0;
}

/***************************************************/
//
// Copy a file
//
// Return 0 if source file not found, -1 if copy error occurred,
// else 1 if copied OK
//
/***************************************************/

WORD copy_file(const char *src_filename, const char *dest_filename) {
	Common::InSaveFile *s = g_system->getSavefileManager()->openForLoading(src_filename);
	if (!s)
		return 0;

	Common::OutSaveFile *d = g_system->getSavefileManager()->openForSaving(dest_filename);
	if (!d) {
		delete s;
		return -1;
	}

	d->writeStream(s);

	delete s;
	delete d;

	return 1;
}

/****************************************************************************/
//
// Determine the size in bytes of a file
//
/****************************************************************************/

int32 file_size(const char *filename) {
	Common::InSaveFile *sf = g_system->getSavefileManager()->openForLoading(filename);
	if (!sf)
		return -1;

	int32 len = sf->size();
	delete sf;
	return len;
}

/****************************************************************************/
//
// Read a file directly into memory
//
/****************************************************************************/

BYTE *read_file(const char *filename, void *dest) {
	uint32 len;
	BYTE *buf;

	Common::InSaveFile *sf = g_system->getSavefileManager()->openForLoading(filename);

	disk_err = 0;

	if (!sf) {
		disk_err = FILE_NOT_FOUND;
		return NULL;
	}

	len = sf->size();
	buf = (dest == NULL) ? (BYTE *)mem_alloc(len) : (BYTE *)dest;

	if (buf == NULL) {
		disk_err = OUT_OF_MEMORY;
		return NULL;
	}

	sf->read(buf, len);
	delete sf;

	return buf;
}

/****************************************************************************/
//
// Write a binary file to disk
// 
/****************************************************************************/

WORD write_file(const char *filename, void *buf, uint32 len) {
	Common::OutSaveFile *handle;

	disk_err = 0;

	handle = g_system->getSavefileManager()->openForSaving(filename, false);
	if (!handle) {
		disk_err = CANT_WRITE_FILE;
		return 0;
	}

	handle->write(buf, len);
	handle->finalize();
	delete handle;

	return 1;
}

/****************************************************************************/
//
// Append binary data to an existing file
//
/****************************************************************************/

WORD append_file(const char *filename, void *buf, uint32 len) {
	error("TODO: append_file");

	return 1;
}

/****************************************************************************/
//
// Get file's timestamp
//
/****************************************************************************/

uint32 file_time(const char *filename) {
	warning("TODO: file_time");

	uint16_t cx = 0x0000; // 00:00:00
	uint16_t dx = 0x5C21; // 1 Jan 2026

	return ((uint32_t)dx << 16) | cx;
}

/****************************************************************************/
//
// Position text cursor
//
/****************************************************************************/

void locate(WORD x, WORD y) {
#if 0
	union REGS inregs, outregs;

	inregs.h.bh = 0x00;
	inregs.h.ah = 0x02;
	inregs.h.dh = y;
	inregs.h.dl = x;
	int386(0x10, &inregs, &outregs);
#endif
}

/****************************************************************************/
//
// Get text cursor location
//
/****************************************************************************/

void curpos(WORD *x, WORD *y) {
	warning("TODO: curpos");
	*x = *y = 0;
}

} // namespace Aesop
