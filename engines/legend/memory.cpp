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

			if (ptrEnd < ptrNext && (mb2->_flags & MEMFLAG_HAS_HANDLE) != 0
				&& (mb2->_flags & MEMFLAG_LOCKED) == 0) {
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

static void purge_block(int shift, size_t size) {
	do {
		if (_G(master_table_end) <= 0)
			break;

		uint shiftSize = size / shift;
		bool purgeResult = false;
		MemoryBlock *mb = _G(master_table);
		for (uint i = 0; !purgeResult && i < _G(master_table_end); ++i, ++mb) {
			if (mb->_size >= shiftSize && !(mb->_flags & MEMFLAG_LOCKED) &&
					(mb->_flags & MEMFLAG_HAS_TYPE) &&
					_G(purge_vector_tbl)[mb->_type]) {
				void *ptr = (mb->_flags & MEMFLAG_HAS_HANDLE) ?
					_G(handle_table)[mb->_handleIndex] : mb->_ptr;
				purgeResult = _G(purge_vector_tbl)[mb->_type](ptr);
			}
		}
		if (!purgeResult)
			break;

		compact_memory();
	} while (freemem_block() <= (int)size);
}

static void purge_memory(size_t size) {
	for (int i = 1; freemem_block() >= (int)size && i <= 64; i *= 2) {
		purge_block(i, size);
	}
}

static MemoryBlock *get_master(size_t size, MemArea type) {
	if (size < 1)
		return nullptr;

	int roundedSize = (size + 15) & 0xFFFFF0;
	bool flag = false;
	MemoryBlock *result = nullptr;
	MemoryBlock *mb1, *mb2;
	int i;

	for (;;) {
		switch (type) {
		case MEMAREA_DEFAULT:
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

		case MEMAREA_1:
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

		case MEMAREA_REAL:
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
/*
static int freemem_min(int amount) {
	int total = 0;
	MemoryBlock *mbEnd = _G(master_table) +
		(_G(master_table_end) - 1),
		*mb1 = _G(master_table),
		*mb2 = _G(master_table) + 1;

	for (; mb1 < mbEnd; ++mb1, ++mb2) {
		// edi
		byte *ptrEnd = (byte *)mb1->_ptr + mb1->_size;
		byte *ptrNext = (byte *)mb2->_ptr;
		int diff = ptrNext - ptrEnd;

		if (diff >= amount)
			total += diff;
	}

	return total;
}
*/
/*
static MemType get_pointer_type(void *ptr) {
	MemoryBlock *mb = find_master(ptr);
	return mb ? mb->_type : MEMTYPE_DOS;
}
*/
static void delete_master(MemoryBlock *mb) {
	MemoryBlock *tableEnd = _G(master_table) + _G(master_table_end),
		*mb1 = mb + 1;
	memmove(mb, mb1, (tableEnd - mb1) * sizeof(MemoryBlock));

	tableEnd->clear();
	_G(master_table_end)--;
}

static int get_handle_num() {
	if (_G(handle_table)) {
		for (int i = 0; i < _G(max_handles); ++i) {
			if (!_G(handle_table)[i])
				return i;
		}
	}

	return -1;
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
	mb->_flags = MEMFLAG_LOCKED;
	mb->_type = MEMTYPE_SYSTEM;

	++mb;
	mb->_ptr = nullptr;
	mb->_handleIndex = -1;
	mb->_size = 0;
	mb->_flags = MEMFLAG_LOCKED;
	mb->_type = MEMTYPE_SYSTEM;

	_G(master_table_end) = 1;
	mb = insert_master(_G(master_table),
		_G(master_table_size) * sizeof(MemoryBlock));
	mb->_type = MEMTYPE_FILE;

	// TODO: real setup/setting
	_G(master_table)[0]._size = _G(real_avail);
	mb = insert_master(_G(real_ptr), _G(real_avail));

	_G(handle_table) = (void **)new_pointer(_G(max_handles) * sizeof(void *));
	set_pointer_type(_G(handle_table), MEMTYPE_FILE);
	Common::fill(_G(handle_table), _G(handle_table) + _G(max_handles), nullptr);

	Common::fill(_G(purge_vector_tbl), _G(purge_vector_tbl), nullptr);
	_G(next_handle_type) = 17;

	_G(max_memory) = _G(min_memory) = _G(low_memory) = _G(memory_avail);
}

void release_memory() {
	delete[] _G(master_ptr);
	delete[] _G(real_ptr);
}

void set_purge_routine(MemType type, PurgeMethod proc) {
	if (type >= MEMTYPE_INI && type < _G(next_handle_type))
		_G(purge_vector_tbl)[type] = proc;
}

int add_purge_routine(PurgeMethod proc, int memType) {
	int next = _G(next_handle_type);
	if (next < 20) {
		_G(next_handle_type)++;
		_G(mem_type)[next] = memType;
		set_purge_routine((MemType)next, proc);
		return next;
	} else {
		return 0;
	}
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
	mb1->_flags = MEMFLAG_LOCKED;
	mb1->_type = MEMTYPE_DOS;

	return mb1;
}

void *new_pointer(size_t size) {
	compact_memory();
	MemoryBlock *mb = get_master(size, MEMAREA_DEFAULT);
	int memSize = freemem_block();

	if (memSize < _G(min_memory)) {
		_G(min_memory) = MIN(_G(min_memory), memSize);
	}

	return mb ? mb->_ptr : nullptr;
}

void *new_real_pointer(size_t size) {
	compact_memory();
	MemoryBlock *mb = get_master(size, MEMAREA_REAL);
	int memSize = freemem_block();

	if (memSize < _G(min_memory)) {
		_G(min_memory) = MIN(_G(min_memory), memSize);
	}

	return mb ? mb->_ptr : nullptr;
}

void set_pointer_type(void *ptr, MemType type) {
	MemoryBlock *mb = find_master(ptr);
	if (mb && (mb->_flags & MEMFLAG_HAS_HANDLE) == 0) {
		mb->_flags &= ~MEMFLAG_LOCKED;

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

int kill_pointer(void *ptr) {
	if (ptr) {
		MemoryBlock *mb = find_master(ptr);
		if (mb && !(mb->_flags & MEMFLAG_HAS_HANDLE)) {
			delete_master(mb);
			return 0;
		}
	}

	return -1;
}

void *resize_pointer(void *ptr, size_t newSize) {
	MemoryBlock *mb = find_master(ptr);
	assert(mb);
	MemType type = mb->_type;
	size_t oldSize = mb->_size;

	// Destroy the old pointer and recreate with new size
	kill_pointer(ptr);
	_G(disable_compact_memory) = true;
	void *newPtr = new_pointer(newSize);
	_G(disable_compact_memory) = false;

	if (newPtr) {
		if (ptr) {
			memmove(newPtr, ptr, MIN(oldSize, newSize));
			set_pointer_type(newPtr, type);
		}
	}

	return newPtr;
}

void *new_handle(size_t size) {
	int handleNum = get_handle_num();

	if (handleNum >= 0) {
		compact_memory();
		MemoryBlock *mb = get_master(size, MEMAREA_1);
		if (mb) {
			mb->_flags |= MEMFLAG_HAS_HANDLE;
			mb->_handleIndex = handleNum;
			_G(handle_table)[handleNum] = mb->_ptr;
		}

		int freeMem = freemem_block();
		if (freeMem < _G(min_memory)) {
			_G(low_memory) = MIN(freeMem, _G(low_memory));
			_G(min_memory) = freeMem;
		}

		if (mb)
			return mb->_ptr;
	}

	return 0;
}

size_t get_handle_size(void **handle) {
	MemoryBlock *mb = find_master(*handle);
	return mb && (mb->_flags & MEMFLAG_HAS_HANDLE) ?
		mb->_size : 0;
}

void set_handle_type(void **handle, MemType type) {
	MemoryBlock *mb = find_master(*handle);

	if (mb && (mb->_flags & MEMFLAG_HAS_HANDLE)) {
		if (!type) {
			mb->_flags &= ~MEMFLAG_HAS_TYPE;
		} else if (_G(purge_vector_tbl)[type]) {
			mb->_flags |= MEMFLAG_HAS_TYPE;
		}

		mb->_type = type;
	}
}

int lock_handle(void **handle) {
	if (handle) {
		MemoryBlock *mb = find_master(*handle);
		if (mb && (mb->_flags & MEMFLAG_HAS_HANDLE)) {
			void *&ptr = _G(handle_table)[mb->_handleIndex];
			if (ptr == *handle) {
				mb->_flags |= MEMFLAG_LOCKED;
				return 0;
			}
		}
	}

	return -1;
}

int unlock_handle(void **handle) {
	if (handle) {
		MemoryBlock *mb = find_master(*handle);
		if (mb && (mb->_flags & MEMFLAG_HAS_HANDLE)) {
			void *&ptr = _G(handle_table)[mb->_handleIndex];
			if (ptr == *handle) {
				mb->_flags |= MEMFLAG_LOCKED;
				return 0;
			}
		}
	}

	return -1;
}

void **resize_handle(void **handle, size_t newSize) {
	if (!handle)
		return nullptr;

	MemoryBlock *mb = find_master(*handle);
	if (mb && (mb->_flags & MEMFLAG_HAS_HANDLE)) {
		void *ptr = _G(handle_table)[mb->_handleIndex];
		if (ptr == *handle && newSize <= mb->_size) {
			mb->_size = (newSize + 15) & 0xFFFFF0;
		}
	}

	return handle;
}

int kill_handle(void **handle) {
	if (handle) {
		MemoryBlock *mb = find_master(*handle);
		if (mb && (mb->_flags & MEMFLAG_HAS_HANDLE)) {
			void *&ptr = _G(handle_table)[mb->_handleIndex];
			if (ptr == *handle) {
				ptr = nullptr;
				delete_master(mb);
			}
		}
	}

	return -1;
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

void memsetx(void *ptr, byte v, size_t count) {
	for (byte *p = (byte *)ptr; count > 0; --count, ++p)
		*p ^= v;	
}

void memxor(void *dest, const void *src, size_t count) {
	byte *destP = (byte *)dest;
	const byte *srcP = (const byte *)src;

	for (; count > 0; --count, ++srcP, ++destP)
		*destP ^= *srcP;
}

void memcpynotn(const void *src, void *dest, size_t count, byte val) {
	byte *destP = (byte *)dest;
	const byte *srcP = (const byte *)src;

	for (; count > 0; --count, ++srcP, ++destP) {
		if (*srcP != val)
			*destP = *srcP;
	}
}

} // namespace Legend
