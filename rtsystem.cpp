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

//#include "aesop/vfx.h"
//#include "aesop/ail32.h"
//#include "aesop/gil2vfx.h"

#include "common/scummsys.h"
#include "common/file.h"
#include "common/stream.h"
#include "common/textconsole.h"
#include "aesop/aesop.h"
#include "aesop/defs.h"
#include "aesop/shared.h"
#include "aesop/rtsystem.h"
#include "aesop/rtmsg.h"
#include "aesop/resources.h"
#include "aesop/rtcode.h"
#include "aesop/intrface.h"
#include "aesop/sound.h"
#include "aesop/graphics.h"
#include "aesop/stubs.h"

namespace Aesop {

void breakpoint(void);
#pragma aux breakpoint = "int 3";

ULONG headroom;
ULONG checksum;
ULONG init;

WORD disk_err;

void mem_init(void) {
	headroom = init = mem_avail();

	checksum = 0L;
}

void mem_shutdown(void) {
	ULONG end;

	end = mem_avail();

	if ((init != end) || (checksum != 0L)) {
//    abend(MSG_UH);    (unbalanced heap normal in flat-model version)
	}
}

ULONG mem_avail(void) {
	REGS inregs, outregs;
	ULONG memarray[12];

	inregs.x.eax = 0x0500;
	inregs.x.edi = (ULONG) memarray;
	int386(0x31, &inregs, &outregs);

	return memarray[0];
}

ULONG mem_headroom(void) {
	return headroom;
}

void *mem_alloc(ULONG bytes) {
	ULONG left;
	void *ptr;

	ptr = (void *) malloc(bytes);

	left = mem_avail();
	if (left < headroom) headroom = left;

	if (ptr == nullptr)
		abend(MSG_OODM);

	checksum ^= (ULONG) ptr;

	return ptr;
}

BYTE *str_alloc(BYTE *str) {
	BYTE *ptr;

	ptr = (BYTE *)mem_alloc(strlen((const char *)str) + 1);
	strcpy((char *)ptr, (const char *)str);

	return ptr;
}

void mem_free(void *ptr) {
	checksum ^= (ULONG) ptr;

	free(ptr);
}


/***************************************************/
//
// AESOP interpreter opcode fault handler
//
/***************************************************/

void opcode_fault(void *PC, void *stk) {
	abend(MSG_IAO, *(unsigned char *) PC, PC, stk);
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
	error("%s", msg);
	/*
	va_list argptr;
	WORD recover;
	WORD x,y;

	curpos(&x,&y);
	if (y > 25)
	  locate(0,0);

	if (msg != nullptr) {
	  va_start(argptr,msg);
	  vprintf(msg,argptr);
	  va_end(argptr);
	  printf("\n");

	  if (envval(0,"AESOP_DIAG") == 1)
	     printf(MSG_MIE,current_msg,current_index,current_event_type);
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
	  */
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

	inregs.h.bh = outregs.h.bh;
	inregs.h.ah = 0x03;
	int386(0x10, &inregs, &outregs);
	*x = outregs.h.dl;
	*y = outregs.h.dh;
}

} // End of namespace Aesop

