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

#include "common/debug.h"
#include "legend/memory.h"
#include "legend/globals.h"

namespace Legend {

#define MAX_MEMORY 0x7FFFFF

static const char *MEM_TYPE[16] = {
	"DOS",
	"SYSTEM",
	"FILE",
	"INI",
	"GRAFX",
	"PIC",
	"PIC (PATCH)",
	"PIC (ICON)",
	"WINDOW",
	"REGION",
	"FONT",
	"AUDIO",
	"MUSIC",
	"SOUND",
	"STRING",
	"UNDO"
};

void MemoryBlock::clear() {
	_ptr = nullptr;
	_handleIndex = 0;
	_size = 0;
	_flags = 0;
	_type = MEMTYPE_DOS;
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

static int freemem_block() {
	int size = 0;

	if (_G(master_table_end) > 1) {
		MemoryBlock *mb1 = _G(master_table),
			*mb2 = _G(master_table) + 1;
		for (uint i = 0; i < _G(master_table_end); ++i, ++mb1, ++mb2) {
			byte *ptrEnd = (byte *)mb1->_ptr + mb1->_size;
			byte *ptrNext = (byte *)mb2->_ptr;

			size = MAX(ptrEnd - ptrNext, size);
		}
	}

	return 0;
}

static void purge_memory(int size) {
	for (int i = 1; i <= 64; ++i) {
		if (freemem_block() >= size)
			break;
	}
}

static MemoryBlock *get_master(size_t size, int type) {
	if (size < 1)
		return nullptr;

	int roundedSize = (size + 15) & 0xFFFFF0;
	bool flag = false;
	MemoryBlock *result = nullptr;
	MemoryBlock *mb1, *mb2;
	int i;

	for (;;) {
		switch (type) {
		case 0:
			mb1 = _G(master_table) + (_G(master_table_end) - 1);
			mb2 = mb1 + 1;

			for (i = _G(master_table_end) - 1; i >= 0; --i, --mb1, --mb2) {
				byte *ptrEnd = (byte *)mb1->_ptr + mb1->_size;
				byte *ptrNext = (byte *)mb2->_ptr;

				int diff = ptrNext - ptrEnd;
				if (roundedSize <= diff) {
					result = insert_master((byte *)mb2->_ptr - roundedSize, roundedSize);
					break;
				}
			}
			break;

		case 1:
			if (_G(master_table_end) > 0) {
				mb1 = _G(master_table);
				mb2 = _G(master_table) + 1;

				for (i = 0; i < (int)_G(master_table_end); ++i) {
					byte *ptrEnd = (byte *)mb1->_ptr + mb1->_size;
					byte *ptrNext = (byte *)mb2->_ptr;

					// TODO: Numeric comparison of pointer is weird
					if ((long)ptrNext < 0x100000) {
						int diff = ptrNext - ptrEnd;
						if (roundedSize <= diff) {
							result = insert_master(ptrEnd, roundedSize);
							break;
						}
					}
				}
			}

			if ((!result || flag) && _G(master_table_end) > 0) {
				mb1 = _G(master_table) + (_G(master_table_end) - 1);
				mb2 = mb1 + 1;

				for (i = _G(master_table_end) - 1; i >= 0; --i, --mb1, --mb2) {
					byte *ptrEnd = (byte *)mb1->_ptr + mb1->_size;
					byte *ptrNext = (byte *)mb2->_ptr;

					int diff = ptrNext - ptrEnd;
					if (roundedSize <= diff) {
						result = insert_master((byte *)mb2->_ptr - roundedSize, roundedSize);
						break;
					}
				}
			}
			break;

		case 2:
			mb1 = _G(master_table);
			mb2 = _G(master_table) + 1;

			for (i = 0; i < (int)_G(master_table_end); ++i) {
				byte *ptrEnd = (byte *)mb1->_ptr + mb1->_size;
				byte *ptrNext = (byte *)mb2->_ptr;

				// TODO: Numeric comparison of pointer is weird
				if ((long)ptrNext < 0x100000) {
					int diff = ptrNext - ptrEnd;
					if (roundedSize <= diff) {
						result = insert_master(ptrEnd, roundedSize);
						break;
					}
				}
			}
			break;

		default:
			break;
		}

		if (result || !flag)
			return result;

		purge_memory(roundedSize);
		flag = true;	// Only allow a second loop
	}
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
	mb->_type = MEMTYPE_SYSTEM;

	++mb;
	mb->_ptr = nullptr;
	mb->_handleIndex = -1;
	mb->_size = 0;
	mb->_flags = MEMFLAG_10;
	mb->_type = MEMTYPE_SYSTEM;

	_G(master_table_end) = 1;
	mb = insert_master(_G(master_table),
		_G(master_table_size) * sizeof(MemoryBlock));
	mb->_type = MEMTYPE_FILE;

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
	mb1->_type = MEMTYPE_DOS;

	return mb1;
}

void *new_pointer(size_t size) {
	compact_memory();
	MemoryBlock *mb = get_master(size, 0);
	int memSize = freemem_block();

	if (memSize < _G(min_memory)) {
		_G(min_memory) = MIN(_G(min_memory), memSize);
	}

	return mb ? mb->_ptr : nullptr;
}

void set_pointer_type(void *ptr, MemType type) {
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

void dump_master_table() {
	int handleCount = 0;
	for (int i = 0; i < _G(max_handles); ++i) {
		if (_G(handle_table)[i])
			++handleCount;
	}

	debug("memmgr: managing %d total bytes", _G(memory_avail));
	debug("%d of %d master table entries used",
		_G(master_table_end), _G(master_table_size));
	debug("%d of %d handles allocated", handleCount,
		_G(max_handles));
/*
	MemoryBlock *mb1 = &_G(master_table)[0];
	MemoryBlock *mb2 = &_G(master_table)[1];
	for (int idx = 0; idx < _G(master_table_end); ++idx) {
		printf()
		(mb1->_flags & MEMFLAG_HAS_TYPE) ? 
	}
	*/
}

} // namespace Legend
