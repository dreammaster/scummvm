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

//
// Memory heap management
//

extern void mem_init(void);
extern void mem_shutdown(void);
extern ULONG mem_avail(void);
extern void *mem_alloc(ULONG bytes);
extern BYTE *str_alloc(BYTE *string);
extern void mem_free(void *ptr);
extern ULONG mem_headroom(void);

//
// Misc. routines
//

extern void opcode_fault(void *PC, void *stk);
extern void abend(const char *msg, ...);
extern void curpos(WORD *x, WORD *y);
extern void locate(WORD x, WORD y);

} // End of namespace Aesop

#endif

