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

typedef bool(*PurgeMethod)(void *ptr);

enum MemFlag {
	MEMFLAG_LOCKED = 0x10,
	MEMFLAG_HAS_TYPE = 0x20,
	MEMFLAG_40 = 0x40,
	MEMFLAG_HAS_HANDLE = 0x80
};

enum MemType {
	MEMTYPE_DOS = 0,
	MEMTYPE_SYSTEM = 1,
	MEMTYPE_FILE = 2,
	MEMTYPE_INI = 3,
	MEMTYPE_GRAFX = 4,
	MEMTYPE_PIC = 5,
	MEMTYPE_PIC_PATCH = 6,
	MEMTYPE_PIC_ICON = 7,
	MEMTYPE_WINDOW = 8,
	MEMTYPE_REGION = 9,
	MEMTYPE_FONT = 10,
	MEMTYPE_AUDIO = 11,
	MEMTYPE_MUSIC = 12,
	MEMTYPE_SOUND = 13,
	MEMTYPE_STRING = 14,
	MEMTYPE_UNDO = 15
};

enum MemArea {
	MEMAREA_DEFAULT = 0,
	MEMAREA_1 = 1,
	MEMAREA_REAL = 2
};

struct MemoryBlock {
	void *_ptr;
	int16 _handleIndex;
	size_t _size;
	byte _flags;
	MemType _type;

	void clear();
};

extern void init_memory();
extern void set_purge_routine(MemType type, PurgeMethod proc);
extern int add_purge_routine(PurgeMethod proc, int memType);
extern MemoryBlock *insert_master(void *ptr, size_t size);
extern void *new_pointer(size_t size);
extern void *new_real_pointer(size_t size);
extern void set_pointer_type(void *ptr, MemType type);
extern void *resize_pointer(void *ptr, size_t newSize);
extern int kill_pointer(void *ptr);

extern void *new_handle(size_t size);
extern size_t get_handle_size(void **handle);
extern void set_handle_type(void **handle, MemType type);
extern int lock_handle(void **handle);
extern int unlock_handle(void **handle);
extern void **resize_handle(void **handle, size_t newSize);
extern int kill_handle(void **handle);

extern void dump_master_table();
extern void memsetx(void *ptr, byte v, size_t count);
extern void memxor(void *dest, const void *src, size_t count);
extern void memcpynotn(const void *src, void *dest, size_t count, byte val);

} // namespace Legend

#endif
