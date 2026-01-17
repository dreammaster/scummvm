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

#ifndef LEGEND_EARLY_CORE_MEMORY_H
#define LEGEND_EARLY_CORE_MEMORY_H

#include "common/serializer.h"

namespace Legend {
namespace Early {

constexpr int MAX_HANDLES = 256;
constexpr int MAX_MASTER_HANDLES = 192;
constexpr size_t MAX_MEMORY = 0x42510;	// This matches how much DosBox has available

enum HFLAG {
	HFLAG_LOCKED = 2,
	HFLAG_ACTIVE = 0x80
};

struct HANDLE {
	void *_ptr = nullptr;
	int _masterListIndex = 0;
	size_t _size = 0;
	uint16 _flags = 0;

	HANDLE();
	HANDLE(void *ptr, int masterListIndex, size_t size, uint16 flags) :
		_ptr(ptr), _masterListIndex(masterListIndex), _size(size), _flags(flags) {}
};

/**
 * Initialize the memory manager
 */
extern void init_memory();

/**
 * Allocates a memory block and returns a pointer to it
 * @param size			Block size
 * @param lowAlloc		Whether to retrieve blocks from the low end of memory,
 *						versus the high end (backwards)
 * @return				Pointer to HANDLE structure
*/
extern HANDLE *get_master(size_t size, bool lowAlloc = true);

/**
 * Allocates a memory block and returns a pointer to the actual memory.
 * This memory block is marked as locked, meaning it won't have an entry in
 * the handles_master_list, and can't shift around in memory.
 */
extern void *new_fixed_handle(size_t size);

/**
 * Allocates a memory block and returns a pointer to the memory.
 * The memory isn't marked as locked, so isn't guaranteed to remain in
 * the same place after future allocations and memory compacts.
 */
extern void *new_handle(size_t size);

/**
 * Locks a memory block, preventing it from being moved
 */
extern int lock_handle(void **ptr);

/**
 * Unlocks a memory block, allowing it to be moved in memory over time.
 */
extern int unlock_handle(void **ptr);

} // End of namespace Early
} // End of namespace Legend

#endif
