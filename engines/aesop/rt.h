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

#ifndef AESOP_RT_H
#define AESOP_RT_H

#include "aesop/rtres.h"

namespace Aesop {

typedef struct                // fundamental stack value structure
{
   uint32 val;
   UWORD type;
}
STKVAL;

enum
{
   TYP_CRES,                  // data type: code resource address
   TYP_SRES,                  // data type: string resource
   TYP_VSHR,                  // data type: short integer variable
   TYP_VLNG,                  // data type: long integer variable
   TYP_SVAR,                  // data type: string variable
};

extern uint32 diag_flag;
extern uint32 current_this;
extern uint32 current_msg;
extern uint32 current_index;

// Pointer and memory block management

#define norm(x) ((void *) (x))
#define add_offset(s,o) ((void *)((uint32)(s) + (uint32)(o)))

#define add_ptr(base,offset) ((void *)((uint32)(base) + (uint32)(offset)))

#define ptr_dif(top,bot) (((BYTE *)(top) - (BYTE *)(bot)))

#define far_memmove(dest, src, len) ((void *)memmove((dest),(src),(len)))

// Assorted speed-critical .ASM routines

void *RTD_first(void *dictionary);
void *RTD_iterate(void *base, void *cur, char **tag, char **def);

BYTE *RTD_lookup(HRES dictionary, const char *key);

// Runtime interpreter calls

void RT_init(RTR_class *RTR, uint32 stack_size, HRES *objlist);
void RT_shutdown(void);
void RT_arguments(void *base, uint32 size);
int32 RT_execute(uint32 index, uint32 msg_num, uint32 vector);

} // namespace Aesop

#endif
