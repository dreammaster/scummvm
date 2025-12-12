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

#include "aesop/lib/vfx.h"
#include "aesop/lib/ail32.h"
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
	union REGS inregs, outregs;
	uint32 memarray[12];

	inregs.x.eax = 0x0500;
	inregs.x.edi = (uint32)memarray;
	int386(0x31, &inregs, &outregs);

	return memarray[0];
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

BYTE *str_alloc(BYTE *str) {
	BYTE *ptr;

	ptr = mem_alloc(strlen(str) + 1);
	strcpy(ptr, str);

	return ptr;
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

int32 ascnum(BYTE *string) {
	int32 i, j, len, base, neg, chr;
	int32 total;

	while (isspace(*string)) string++;

	neg = 0;
	switch (*string)
	{
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
		if (isdigit(*string))
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

void abend(char *msg, ...) {
	va_list argptr;
	WORD recover;
	WORD x, y;
	char loErrorBuffer[1000 + 1]; // max length + 1

	curpos(&x, &y);
	if (y > 25)
		locate(0, 0);

	if (msg != NULL)
	{
		FILE *loErrorFile;
		printf("Error: ");

		va_start(argptr, msg);
		vsnprintf(loErrorBuffer, 1000, msg, argptr);
		loErrorBuffer[1000] = '\0';  // just to be sure there is a terminator even if limit is filled
		va_end(argptr);
		printf("%s\n", loErrorBuffer);

		// attempt to make an error file
		loErrorFile = fopen("aesop_e.dbg", "wt");
		if (loErrorFile != NULL)
		{
			// write error
			fprintf(loErrorFile, "The AESOP/32 engine terminated with the error:\n");
			fprintf(loErrorFile, "%s\n", loErrorBuffer);
			if (envval(0, "AESOP_DIAG") == 1)
			{
				fprintf(loErrorFile, MSG_MIE, current_msg, current_index, current_event_type);
			}
			fclose(loErrorFile);
		}

		if (envval(0, "AESOP_DIAG") == 1)
		{
			printf(MSG_MIE, current_msg, current_index, current_event_type);
		}
	}

	recover = 0;

	breakpoint();

	if (!recover)
	{
		shutdown_sound();
		shutdown_interface();
		AIL_shutdown("Abend");
		GIL2VFX_shutdown_driver();

		exit(1);
	}
}

/***************************************************/
//
// Open a text file for reading/writing
//
/***************************************************/

TF_class *TF_construct(BYTE *filename, WORD oflag) {
	TF_class *TF;
	WORD file;
	HRES hbuf;

	if (oflag == TF_WRITE)
		oflag = O_CREAT | O_TRUNC | O_WRONLY;
	else
		oflag = O_RDONLY;

	file = open(filename, oflag | O_BINARY, S_IREAD | S_IWRITE);
	if (file == -1) return NULL;

	hbuf = RTR_alloc(RTR, TF_BUFSIZE, DA_FIXED | DA_PRECIOUS);
	if (hbuf == -1U) return NULL;

	TF = mem_alloc(sizeof(TF_class));

	TF->file = file;
	TF->hbuf = hbuf;
	TF->buffer = RTR_addr(TF->hbuf);
	TF->p = 0;
	TF->mode = oflag;
	TF->len = filelength(file);
	TF->pos = 0L;

	if (!(oflag & O_WRONLY))
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

	if ((TF->mode & O_WRONLY) && (TF->p != 0))
		e = write(TF->file, TF->buffer, TF->p);

	close(TF->file);

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

	if (TF->p == TF_BUFSIZE)
	{
		TF->p = 0;
		if (write(TF->file, TF->buffer, TF_BUFSIZE) != TF_BUFSIZE) return 0;
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

	read(TF->file, TF->buffer, TF_BUFSIZE);

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

WORD delete_file(BYTE *filename) {
	if (!unlink(filename))
		return 1;

	if (errno == ENOENT)
		return 0;

	return -1;
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
	HRES hbuf;
	BYTE *buffer;
	WORD status;
	WORD s, d, n;

	s = open(src_filename, O_RDONLY | O_BINARY);

	if (s == -1)
		return 0;

	d = open(dest_filename, O_BINARY | O_CREAT | O_TRUNC | O_WRONLY,
		S_IREAD | S_IWRITE);

	if (d == -1)
	{
		close(s);
		return -1;
	}

	hbuf = RTR_alloc(RTR, TF_BUFSIZE, DA_FIXED | DA_PRECIOUS);
	if (hbuf == -1U)
	{
		close(s);
		close(d);
		return -1;
	}

	buffer = RTR_addr(hbuf);
	status = 1;

	while ((n = read(s, buffer, TF_BUFSIZE)) != 0)
	{
		if (n == -1)
		{
			status = -1;
			break;
		}

		if (write(d, buffer, n) != n)
		{
			status = -1;
			break;
		}
	}

	close(s);
	close(d);

	RTR_free(RTR, hbuf);

	return status;
}

/****************************************************************************/
//
// Determine the size in bytes of a file
//
/****************************************************************************/

int32 file_size(BYTE *filename) {
	WORD handle;
	uint32 len;

	disk_err = 0;

	handle = open(filename, O_RDONLY | O_BINARY);
	if (handle == -1)
	{
		disk_err = FILE_NOT_FOUND;
		return -1L;
	}

	len = filelength(handle);
	if (len == -1L) disk_err = CANT_READ_FILE;

	close(handle);
	return len;
}

/****************************************************************************/
//
// Read a file directly into memory
//
/****************************************************************************/

BYTE *read_file(BYTE *filename, void *dest) {
	WORD i, handle;
	uint32 len;
	BYTE *buf, *mem;

	disk_err = 0;

	len = file_size(filename);
	if (len == -1L)
	{
		disk_err = FILE_NOT_FOUND;
		return NULL;
	}

	buf = mem = (dest == NULL) ? mem_alloc(len) : dest;

	if (buf == NULL)
	{
		disk_err = OUT_OF_MEMORY;
		return NULL;
	}

	handle = open(filename, O_RDONLY | O_BINARY);
	if (handle == -1)
	{
		mem_free(mem);
		disk_err = FILE_NOT_FOUND;
		return NULL;
	}

	while (len >= DOS_BUFFSIZE)
	{
		i = read(handle, buf, DOS_BUFFSIZE);
		if (i != (WORD)DOS_BUFFSIZE)
		{
			mem_free(mem);
			disk_err = CANT_READ_FILE;
			return NULL;
		}
		len -= DOS_BUFFSIZE;
		buf = add_ptr(buf, DOS_BUFFSIZE);
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

WORD write_file(BYTE *filename, void *buf, uint32 len) {
	WORD i, handle;

	disk_err = 0;

	handle = open(filename, O_CREAT | O_RDWR | O_TRUNC | O_BINARY,
		S_IREAD | S_IWRITE);
	if (handle == -1)
	{
		disk_err = CANT_WRITE_FILE;
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
// Append binary data to an existing file
//
/****************************************************************************/

WORD append_file(BYTE *filename, void *buf, uint32 len) {
	WORD i, handle;

	disk_err = 0;

	handle = open(filename, O_APPEND | O_RDWR | O_BINARY);
	if (handle == -1)
	{
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

uint32 file_time(BYTE *filename) {
	union REGS in, out;
	WORD handle;

	handle = open(filename, O_RDONLY);

	if (handle == -1) return 0L;

	in.w.ax = 0x5700;
	in.w.bx = handle;
	intdos(&in, &out);

	close(handle);

	return (uint32)out.w.cx + ((uint32)out.w.dx << 16);
}

/****************************************************************************/
//
// Position text cursor
//
/****************************************************************************/

void locate(WORD x, WORD y) {
	union REGS inregs, outregs;

	inregs.h.bh = 0x00;
	inregs.h.ah = 0x02;
	inregs.h.dh = y;
	inregs.h.dl = x;
	int386(0x10, &inregs, &outregs);
}

/****************************************************************************/
//
// Get text cursor location
//
/****************************************************************************/

void curpos(WORD *x, WORD *y) {
	union REGS inregs, outregs;

	inregs.h.ah = 0x0f;
	int386(0x10, &inregs, &outregs);

	inregs.h.bh = outregs.h.bh; inregs.h.ah = 0x03;
	int386(0x10, &inregs, &outregs);
	*x = outregs.h.dl; *y = outregs.h.dh;
}

} // namespace Aesop
