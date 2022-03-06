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

#include "legend/memory.h"
#include "legend/globals.h"

namespace Legend {

#define MAX_MEMORY 0x7FFFFF

void MemoryBlock::clear() {
	_ptr = nullptr;
	_handleIndex = 0;
	_size = 0;
	_flags = 0;
	_type = MEMTYPE_0;
}

static void compact_memory() {
	if (!_G(disable_compact_memory) && _G(master_table_end) > 0) {
		uint index = 0;
		MemoryBlock *mb2, *mb = _G(master_table);

		for (; index < _G(master_table_end); ++index, ++mb) {
			mb2 = mb + 1;
			byte *ptrEnd = (byte *)mb->_ptr + mb->_size;
			byte *ptrNext = (byte *)mb2->_ptr;

			if (ptrEnd < ptrNext && (mb2->_flags & MEMFLAG_80) != 0
					&& (mb2->_flags & MEMFLAG_10) == 0) {
				void *handle = _G(handle_table)[mb2->_handleIndex];
				if (handle) {
					memmove(ptrEnd, handle, mb2->_size);
					mb2->_ptr = ptrEnd;
					_G(handle_table)[mb2->_handleIndex] = ptrEnd;
				}
			}
		}
	}
}

static MemoryBlock *find_master(void *ptr) {
	if (_G(master_table_end) > 1) {
		MemoryBlock *tableEnd = _G(master_table) +
			_G(master_table_end);
		for (MemoryBlock *mb = _G(master_table); mb < tableEnd; ++mb) {
			if (mb->_ptr == ptr)
				return mb;
		}
	}

	return nullptr;
}

void init_memory() {
	// Return if the memory manager is already initialized
	if (_G(master_ptr))
		return;

	_G(memory_avail) = MAX_MEMORY;
	_G(master_ptr) = new byte[MAX_MEMORY];
	if (!_G(master_ptr))
		error("Could not allocate memory");

	int alignOfs = (uint)_G(master_ptr) % 0x10;
	int align = alignOfs == 0 ? 0 : 16 - alignOfs;
	_G(memory_avail) -= align;
	_G(master_table) = (MemoryBlock *)(_G(master_ptr) + align);
	Common::fill((byte *)_G(master_table), (byte *)_G(master_table) +
		_G(master_table_size) * sizeof(MemoryBlock), 0);

	MemoryBlock *mb = _G(master_table);
	mb->_ptr = nullptr;
	mb->_handleIndex = -1;
	mb->_size = 0;
	mb->_flags = MEMFLAG_10;
	mb->_type = MEMTYPE_1;

	++mb;
	mb->_ptr = nullptr;
	mb->_handleIndex = -1;
	mb->_size = 0;
	mb->_flags = MEMFLAG_10;
	mb->_type = MEMTYPE_1;

	_G(master_table_end) = 1;
	mb = insert_master(_G(master_table),
		_G(master_table_size) * sizeof(MemoryBlock));
	mb->_type = MEMTYPE_2;

	// TODO: DPMI real table entry created.
	// Not sure if needed in ScummVM

	//_G(handle_table) = (void **)new_pointer(_G(max_handles) * 4, 2);
	// TODO: Other stuff
}

MemoryBlock *insert_master(void *ptr, size_t size) {
	uint index1 = 1;

	if (size < _G(master_table_end)) {
		// Scan for existing memory block with the pointer
		for (MemoryBlock *mb = _G(master_table);
			index1 <= _G(master_table_end) && mb->_ptr != ptr;
			++mb, ++index1) {
		}
	}

	int index2 = _G(master_table_end) - index1 + 1;
	MemoryBlock *mb1 = &_G(master_table)[index1],
		*mb2 = &_G(master_table)[index2],
		*mb3 = &_G(master_table)[index1 + 1];
	memmove((byte *)mb3, (byte *)mb1,
		(byte *)mb2 - (byte *)mb1);
	++_G(master_table_end);

	mb1->clear();
	mb1->_ptr = ptr;
	mb1->_handleIndex = -1;
	mb1->_size = size;
	mb1->_flags = MEMFLAG_10;
	mb1->_type = MEMTYPE_0;

	return mb1;
}

void set_pointer_type(void *ptr, int type) {
	MemoryBlock *mb = find_master(ptr);
	if (mb && (mb->_flags & MEMFLAG_80) == 0) {
		mb->_flags &= ~MEMFLAG_10;

		if (type) {
			assert(type < 20);
			if (_G(purge_vector_tbl)[type])
				mb->_flags |= MEMFLAG_HAS_TYPE;
		} else {
			mb->_flags &= ~MEMFLAG_HAS_TYPE;
		}

		mb->_type = type;
	}
}

} // namespace Legend
