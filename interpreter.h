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

#ifndef AESOP_INTERPRETER_H
#define AESOP_INTERPRETER_H

#include "aesop/resources.h"

namespace Aesop {

extern ULONG current_this;

#define add_ptr(base,offset) ((void *)((ULONG)(base) + (ULONG)(offset)))
#define ptr_dif(top,bot) (((BYTE *)(top) - (BYTE *)(bot)))
#define far_memmove(dest, src, len) ((void *)memmove((dest),(src),(len)))
#define add_offset(s,o) ((void *)((ULONG)(s) + (ULONG)(o)))

class Interpreter {
public:
	~Interpreter();

	uint first(HRES dictionary);
	uint iterate(const void *base, uint cur, const char **tag, const char **def);

	const char *lookup(HRES dictionary, const Common::String &key);

	void init(Resources *RTR, ULONG stackSize, HRES *objlist);
	void shutdown(void);
	void arguments(void *base, ULONG size);
	LONG execute(ULONG index, ULONG msgNum, ULONG vector);
};

} // End of namespace Aesop

#endif

