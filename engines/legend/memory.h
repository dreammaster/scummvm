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

#ifndef LEGEND_MEMORY_H
#define LEGEND_MEMORY_H

#include "common/scummsys.h"

namespace Legend {

enum MemFlag {
	MEMFLAG_10 = 0x10,
	MEMFLAG_HAS_TYPE = 0x20,
	MEMFLAG_40 = 0x40,
	MEMFLAG_80 = 0x80
};

enum MemType {
	MEMTYPE_0 = 0,
	MEMTYPE_1 = 1,
	MEMTYPE_2 = 2
};

struct MemoryBlock {
	void *_ptr;
	int16 _handleIndex;
	size_t _size;
	byte _flags;
	byte _type;

	void clear();
};

extern void init_memory();
extern MemoryBlock *insert_master(void *ptr, size_t size);
extern void set_pointer_type(void *ptr, int type);

} // namespace Legend

#endif
