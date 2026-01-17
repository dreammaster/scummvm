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

#include "legend/early/core/memory.h"
#include "legend/early/engine.h"

namespace Legend {
namespace Early {

static void compact_memory();
static HANDLE *insert_handle(void *ptr, size_t size);
static int find_free_handle();
static HANDLE *get_handle(void *ptr);

void init_memory() {
	auto &e = *g_engine;

	e.memory_data.resize(MAX_MEMORY);
	e.handles_master_list.resize(MAX_MASTER_HANDLES);
	e.handles_list.reserve(MAX_HANDLES);

	// Original created first handle as a locked area to hold
	// the handle list itself. We don't need that for ScummVM

	// First handle entry to point to the start of the available memory with 0 size.
	// The original had space for the handles_list and handles_master_list, but in
	// ScummVM these are arrays, so don't use the memory block
	e.handles_list.push_back(HANDLE(&e.memory_data[0], -1, 0, HFLAG_LOCKED));
}

HANDLE *get_master(size_t size, bool lowAlloc) {
	auto &e = *g_engine;
	HANDLE *result = nullptr;

	/* Align size to 16 bytes */
	size = (size + 0x0F) & 0xFFF0;

	if (size == 0)
		error("get_master: bytes requested = 0");

	size_t reqSize = size;

	// LOW allocation: search from start upward
	if (lowAlloc) {
		for (int i = 0; i < (int)e.handles_list.size() - 1; i++) {
			HANDLE *h = &e.handles_list[i];

			intptr endPtr = (intptr)h->_ptr + h->_size;
			intptr startPtr = (intptr)e.handles_list[i + 1]._ptr;

			// Is there enough free space at the end of the block before the next?
			if (startPtr - endPtr >= (int)reqSize) {
				result = insert_handle((void *)endPtr, size);
				break;
			}
		}
	} else {
		// HIGH allocation: search from end downward
		for (int i = (int)e.handles_list.size() - 1; i > 0; i--) {
			HANDLE *h = &e.handles_list[i - 1];

			intptr endPtr = (intptr)h->_ptr + h->_size;
			intptr startPtr = (intptr)e.handles_list[i]._ptr;

			if (startPtr - endPtr >= (int)reqSize) {
				intptr allocPtr = startPtr - reqSize;

				result = insert_handle((void *)allocPtr, size);
				break;
			}
		}
	}

	// Handle table overflow check
	if (e.handles_list.size() == MAX_HANDLES)
		error("OUCH! OUCH OUCH Ouch ouch... master table overflow. "
			"More than %d handles please.", MAX_HANDLES);

	return result;
}

void *new_fixed_handle(size_t size) {
	compact_memory();
	HANDLE *h = get_master(size);

	if (h) {
		h->_masterListIndex = -1;
		h->_flags = HFLAG_LOCKED;
		return h->_ptr;
	}

	return h;
}

void *new_handle(size_t size) {
	auto &e = *g_engine;
	int handleIndex = find_free_handle();

	if (handleIndex >= 0) {
		compact_memory();
		HANDLE *h = get_master(size);
		assert(h);
		h->_masterListIndex = handleIndex;
		h->_flags = HFLAG_ACTIVE;
		e.handles_master_list[handleIndex] = h->_ptr;

		return h->_ptr;

	} else {
		error("new_handle : ERROR: out of handles");
	}
}

int lock_handle(void **ptr) {
	auto &e = *g_engine;

	if (!ptr)
		return -1;

	HANDLE *h = get_handle(*ptr);
	if (!h || !(h->_flags & HFLAG_ACTIVE))
		error("lock_handle : ERROR: not a valid handle");

	int masterIndex = h->_masterListIndex;
	if (e.handles_master_list[masterIndex] != h->_ptr)
		error("lock_handle : ERROR: Handle ptr does not match master list ptr");

	h->_flags |= HFLAG_LOCKED;
	return 0;
}

int unlock_handle(void **ptr) {
	auto &e = *g_engine;

	if (!ptr)
		return -1;

	HANDLE *h = get_handle(*ptr);
	if (!h || !(h->_flags & HFLAG_ACTIVE))
		error("lock_handle : ERROR: not a valid handle");

	int masterIndex = h->_masterListIndex;
	if (e.handles_master_list[masterIndex] != h->_ptr)
		error("lock_handle : ERROR: Handle ptr does not match master list ptr");

	h->_flags &= ~HFLAG_LOCKED;
	return 0;
}

/**
 * Compacts the memory, assigning moveable blocks to consecutive memory.
 * This removes any previously de-allocated memory block space.
 */
void compact_memory() {
	auto &e = *g_engine;

	if (e.handles_list.size() < 2)
		return;

	for (int i = 0; i < (int)e.handles_list.size() - 1; i++) {
		HANDLE *cur = &e.handles_list[i];
		HANDLE *next = &e.handles_list[i + 1];

		byte *blockEnd =  (byte *)cur->_ptr + cur->_size;
		byte *nextPtr = (byte *)next->_ptr;

		// If no gap, nothing to do
		if (blockEnd >= nextPtr)
			continue;

		// Only active, movable blocks can be compacted
		if (!(next->_flags & HFLAG_ACTIVE))
			continue;
		if (next->_flags & HFLAG_LOCKED)
			continue;

		// Verify master pointer consistency
		void **masterEntry = &e.handles_master_list[next->_masterListIndex];

		if (*masterEntry != next->_ptr) {
			error("compact_memory : ERROR: the handle and "
				"master pointer are out of sync");
		}

		void *dest = blockEnd;
		memmove(dest, next->_ptr, next->_size);

		// Update handle and master list pointers
		next->_ptr = dest;
		*masterEntry = dest;
	}
}

/**
 * Inserts a new handle into the handles_list, sorting by ascending ptr address
 * @param ptr		Block pointer
 * @param size		Block size
 * @return			Pointer to new HANDLE structure
*/
HANDLE *insert_handle(void *ptr, size_t size) {
	auto &e = *g_engine;
	int insertIndex = 0;

	// Find insertion point (sorted by _ptr)
	if (!e.handles_list.empty()) {
		for (; insertIndex < (int)e.handles_list.size(); ++insertIndex) {
			const HANDLE *h = &e.handles_list[insertIndex];
			if ((intptr)ptr < (intptr)h->_ptr)
				break;
		}
	}

	// Insert the new entry
	e.handles_list.insert_at(insertIndex, HANDLE(ptr, -1, size, 0));
	return &e.handles_list[insertIndex];
}

/**
 * Find a free slot in the handles_master_list
 * @return	Index of the free slot
 */
int find_free_handle() {
	auto &e = *g_engine;

	for (uint i = 0; i < e.handles_master_list.size(); ++i) {
		if (!e.handles_master_list[i])
			return i;
	}

	return -1;
}

/**
 * Scans the handles_list to find the handle matching a given ptr
 */
HANDLE *get_handle(void *ptr) {
	auto &e = *g_engine;
	for (HANDLE &h : e.handles_list) {
		if (h._ptr == ptr)
			return &h;
	}

	return nullptr;
}

} // namespace Early
} // namespaceCo Legend
