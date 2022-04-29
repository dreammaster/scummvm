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
	MEMAREA_END,
	MEMAREA_START	
};

class Memory;
struct MemoryBlock {
	void *_ptr;		// Pointer to data when block is locked
	int16 _handleIndex;
	size_t _size;
	byte _flags;
	MemType _type;

	void clear();
private:
	friend class Memory;
	void *_rawPtr;
};

class Memory {
private:
	// Size of the memory allocated to the memory manager
	// TODO: Figure out a suitable size
	const size_t MEMORY_AVAILABLE = 0xffffff;
	byte *_master_ptr;
	MemoryBlock *_master_table;
	size_t _master_table_size = 0x300;
	size_t _real_avail = 0xfffff;
	size_t _master_table_end = 0;
	int _max_handles = 0x200;
	int _next_handle_type = 0;
	void **_handle_table;
	bool _disable_compact_memory = false;
	PurgeMethod _purge_vector_tbl[MEMTYPE_UNDO + 1] = { nullptr };
	//int _mem_type[20] = { 0 };
	int _min_memory = -1, __min_memory = -1;
	int _max_memory = 0;
	int _low_memory = 0;

private:
	void compact_memory();
	int freemem_block();
	MemoryBlock *find_master(void *ptr);
	void *purge_block(size_t size, int *purgeIndex);
	MemoryBlock *get_master(size_t size, MemArea memArea);
	void delete_master(MemoryBlock *mb);
	int get_handle_num();

public:
	Memory();
	~Memory();

	void set_purge_routine(MemType type, PurgeMethod proc);
	int add_purge_routine(PurgeMethod proc);
	MemoryBlock *insert_master(void *ptr, size_t size);
	void *new_pointer(size_t size);
	void *resize_pointer(void *ptr, size_t newSize);
	int kill_pointer(void *ptr);

	void *new_handle(size_t size);
	size_t get_handle_size(void **handle);
	void set_handle_type(void **handle, MemType type);
	int lock_handle(void **handle);
	int unlock_handle(void **handle);
	int kill_handle(void **handle);

	void *new_real_pointer(size_t size) {
		return new_pointer(size);
	}
	void kill_real_pointer(void *ptr) {
		kill_pointer(ptr);
	}
	size_t get_low_memory() {
		int result = _min_memory * 16;
		_min_memory = -1;
		return result;
	}
	size_t get_min_memory() const {
		return __min_memory * 16;
	}

	void dump_master_table();
};

extern Memory *g_mem;

extern void memsetx(void *ptr, byte v, size_t count);
extern void memxor(void *dest, const void *src, size_t count);
extern void memcpynotn(const void *src, void *dest, size_t count, byte val);

} // namespace Legend

#endif
