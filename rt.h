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
//  AESOP runtime ASM calls
//

#ifndef AESOP_RT_H
#define AESOP_RT_H

#include "aesop/resources.h"

namespace Aesop {

// fundamental stack value structure
struct STKVAL {
   ULONG val;
   UWORD type;
};

enum {
   TYP_CRES,                  // data type: code resource address
   TYP_SRES,                  // data type: string resource
   TYP_VSHR,                  // data type: short integer variable
   TYP_VLNG,                  // data type: long integer variable
   TYP_SVAR,                  // data type: string variable
};

extern ULONG diag_flag;
extern ULONG current_this;
extern ULONG current_msg;
extern ULONG current_index;

// Pointer and memory block management

#define norm(x) ((void *) (x))
#define add_offset(s,o) ((void *)((ULONG)(s) + (ULONG)(o)))

#define add_ptr(base,offset) ((void *)((ULONG)(base) + (ULONG)(offset)))

#define ptr_dif(top,bot) (((BYTE *)(top) - (BYTE *)(bot)))

#define far_memmove(dest, src, len) ((void *)memmove((dest),(src),(len)))

// Assorted speed-critical .ASM routines

extern void *RTD_first(void *dictionary);
extern void *RTD_iterate(void *base, void *cur, BYTE **tag, BYTE **def);

extern BYTE *RTD_lookup(HRES dictionary, const void *key);

// Runtime interpreter calls

extern void RT_init(Resources *RTR, ULONG stack_size, HRES *objlist);
extern void RT_shutdown(void);
extern void RT_arguments(void *base, ULONG size);
extern LONG RT_execute(ULONG index, ULONG msg_num, ULONG vector);

} // End of namespace Aesop

#endif

