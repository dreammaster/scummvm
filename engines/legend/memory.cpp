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

Memory *g_mem;

#define MAX_MEMORY 0x7FFFFF
#define MAX_BLOCKS 512
#define MAX_HANDLES 256

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
	_ptr = _rawPtr = nullptr;
	_handleIndex = 0;
	_size = 0;
	_flags = 0;
	_type = MEMTYPE_DOS;
}

/*-------------------------------------------------------*/

Memory::Memory() {
	g_mem = this;
	_master_ptr = new byte[MEMORY_AVAILABLE];
	assert(_master_ptr);
	Common::fill(_master_ptr, _master_ptr + MEMORY_AVAILABLE, 0);

	_master_table = (MemoryBlock *)_master_ptr;

	MemoryBlock *mb = _master_table;
	mb->_ptr = mb->_rawPtr = _master_ptr;
	mb->_handleIndex = -1;
	mb->_size = sizeof(MemoryBlock) * MAX_BLOCKS;
	mb->_flags = MEMFLAG_LOCKED;
	mb->_type = MEMTYPE_DOS;

	++mb;
	mb->_ptr = mb->_rawPtr = _master_ptr + (MEMORY_AVAILABLE / 16);
	mb->_handleIndex = -1;
	mb->_size = 0;
	mb->_flags = MEMFLAG_LOCKED;
	mb->_type = MEMTYPE_DOS;

	_master_table_end = 1;

	_handle_table = (void **)new_pointer(sizeof(void *) * MAX_HANDLES);
	Common::fill(&_handle_table[0], &_handle_table[MAX_HANDLES], nullptr);
}

Memory::~Memory() {
	g_mem = nullptr;
	delete[] _master_ptr;
}

void *Memory::new_pointer(size_t size) {
	compact_memory();

	MemoryBlock *mb = get_master(size, MEMAREA_END);
	if (mb) {
		mb->_handleIndex = -1;
		mb->_flags = MEMFLAG_LOCKED;
	}

	int memSize = freemem_block();
	if (memSize < _min_memory) {
		_min_memory = memSize;
		__min_memory = MIN(__min_memory, memSize);
	}

	return mb ? mb->_ptr : nullptr;
}

void *Memory::new_handle(size_t size) {
	int handleNum = get_handle_num();

	if (handleNum >= 0) {
		compact_memory();

		MemoryBlock *mb = get_master(size, MEMAREA_START);
		if (mb) {
			mb->_handleIndex = handleNum;
			mb->_flags = MEMFLAG_HAS_HANDLE;
			_handle_table[handleNum] = mb->_ptr;
		}

		int memSize = freemem_block();
		if (memSize < _min_memory) {
			_min_memory = memSize;
			__min_memory = MIN(__min_memory, memSize);
		}

		return _handle_table[handleNum];

	} else {
		return nullptr;
	}
}

void Memory::compact_memory() {
	if (!_disable_compact_memory && _master_table_end > 0) {
		uint index = 0;
		MemoryBlock *mb2, *mb = _master_table;

		for (; index < _master_table_end; ++index, ++mb) {
			mb2 = mb + 1;
			byte *ptrEnd = (byte *)mb->_rawPtr + mb->_size;
			byte *ptrNext = (byte *)mb2->_rawPtr;

			if (ptrEnd < ptrNext && (mb2->_flags & MEMFLAG_HAS_HANDLE) != 0
					&& (mb2->_flags & MEMFLAG_LOCKED) == 0) {
				void *handle = _handle_table[mb2->_handleIndex];
				if (handle) {
					memmove(ptrEnd, handle, mb2->_size);
					mb2->_ptr = mb2->_rawPtr = ptrEnd;
					_handle_table[mb2->_handleIndex] = ptrEnd;
				}
			}
		}
	}
}

MemoryBlock *Memory::find_master(void *ptr) {
	if (_master_table_end > 1) {
		MemoryBlock *tableEnd = _master_table +
			_master_table_end;
		for (MemoryBlock *mb = _master_table; mb < tableEnd; ++mb) {
			if (mb->_rawPtr == ptr)
				return mb;
		}
	}

	return nullptr;
}

int Memory::freemem_block() {
	if (_master_table_end > 1) {
		MemoryBlock *mb1 = _master_table,
			*mb2 = _master_table + 1;
		int size = 0;

		for (uint i = 0; i < _master_table_end; ++i, ++mb1, ++mb2) {
			byte *ptrEnd = (byte *)mb1->_rawPtr + mb1->_size;
			byte *ptrNext = (byte *)mb2->_rawPtr;

			size = MAX(ptrEnd - ptrNext, size);
		}

		return size / 16;
	}

	return 0;
}

void *Memory::purge_block(size_t size, int *purgeIndex) {
	void *result = nullptr;
	if (_master_table_end < 0)
		return nullptr;

	MemoryBlock *mb = _master_table;
	for (size_t i = 0; i < _master_table_end; ++i, ++mb) {
		if ((mb->_flags & MEMFLAG_HAS_HANDLE) &&
				!(mb->_flags & MEMFLAG_LOCKED) &&
				(mb->_flags & MEMFLAG_HAS_TYPE)) {
			PurgeMethod fn = _purge_vector_tbl[mb->_type];
			if (fn) {
				*purgeIndex = mb->_type;
				result = _handle_table[mb->_handleIndex];
				break;
			}
		}
	}

	return result;
}

MemoryBlock *Memory::get_master(size_t size, MemArea memArea) {
	if (size == 0)
		return nullptr;

	size = (size + 15) & 0xFFFFF0;

	MemoryBlock *result = nullptr;
	MemoryBlock *mb1, *mb2;
	int i;

	for (int tryNum = 0; tryNum < 2; ++tryNum) {
		switch (memArea) {
		case MEMAREA_END:
			mb1 = _master_table + (_master_table_end - 1);
			mb2 = mb1 + 1;

			for (i = _master_table_end - 1; i > 0; --i, --mb1, --mb2) {
				byte *ptrEnd = (byte *)mb1->_rawPtr + mb1->_size;
				byte *ptrNext = (byte *)mb2->_rawPtr;

				int diff = ptrNext - ptrEnd;
				if ((int)size <= diff) {
					result = insert_master((byte *)mb2->_rawPtr - size, size);
					break;
				}
			}
			break;
			
		case MEMAREA_START:
			mb1 = _master_table;
			mb2 = _master_table + 1;

			for (i = 0; i < (int)_master_table_end; ++i) {
				byte *ptrEnd = (byte *)mb1->_rawPtr + mb1->_size;
				byte *ptrNext = (byte *)mb2->_rawPtr;

				int diff = ptrNext - ptrEnd;
				if ((int)size <= diff) {
					result = insert_master((byte *)mb2->_rawPtr - size, size);
					break;
				}
			}
			break;
		}

		if (result)
			return result;

		int purgeIndex = 0;
		void *purgePtr = purge_block(size, &purgeIndex);
		if (!purgePtr || !(*_purge_vector_tbl)(purgePtr))
			break;
	}

	return nullptr;
}

void Memory::delete_master(MemoryBlock *mb) {
	MemoryBlock *tableEnd = _master_table + _master_table_end;
	Common::copy(mb + 1, tableEnd, mb);

	tableEnd->clear();
	_master_table_end--;
}

int Memory::get_handle_num() {
	if (_handle_table) {
		for (int i = 0; i < _MAX_BLOCKS; ++i) {
			if (!_handle_table[i])
				return i;
		}
	}

	return -1;
}

int Memory::add_purge_routine(PurgeMethod proc) {
	int next = _next_handle_type;
	if (next <= MEMTYPE_UNDO) {
		_next_handle_type++;
		set_purge_routine((MemType)next, proc);
		return next;
	} else {
		return 0;
	}
}

void Memory::set_purge_routine(MemType type, PurgeMethod proc) {
	if (type >= MEMTYPE_INI && type < _next_handle_type)
		_purge_vector_tbl[type] = proc;
}

MemoryBlock *Memory::insert_master(void *ptr, size_t size) {
	MemoryBlock *mb = _master_table;
	uint index1 = 0;
	for (; index1 < _master_table_end; ++index1, ++mb) {
		if (ptr >= mb->_rawPtr)
			break;
	}

	int index2 = _master_table_end - index1 + 1;
	MemoryBlock *mb1 = &_master_table[index1],
		*mb2 = &_master_table[index2],
		*mb3 = &_master_table[index1 + 1];
	memmove((byte *)mb3, (byte *)mb1,
		(byte *)mb2 - (byte *)mb1);
	++_master_table_end;

	mb1->clear();
	mb1->_ptr = mb1->_rawPtr = ptr;
	mb1->_size = size;

	return mb1;
}

int Memory::kill_pointer(void *ptr) {
	if (ptr) {
		MemoryBlock *mb = find_master(ptr);
		if (mb && !(mb->_flags & MEMFLAG_HAS_HANDLE)) {
			delete_master(mb);
			return 0;
		}
	}

	return -1;
}

void *Memory::resize_pointer(void *ptr, size_t newSize) {
	kill_pointer(ptr);

	_disable_compact_memory = true;
	void *newPtr = new_pointer(newSize);
	_disable_compact_memory = false;

	if (newPtr && ptr) {
		Common::copy((const byte *)ptr, (const byte *)ptr + newSize, (byte *)newPtr);
	}

	return newPtr;
}

size_t Memory::get_handle_size(void **handle) {
	MemoryBlock *mb = find_master(*handle);
	return mb && (mb->_flags & MEMFLAG_HAS_HANDLE) ?
		mb->_size : 0;
}

void Memory::set_handle_type(void **handle, MemType type) {
	MemoryBlock *mb = find_master(*handle);

	if (mb && (mb->_flags & MEMFLAG_HAS_HANDLE) &&
			!(mb->_flags & MEMFLAG_LOCKED)) {
		if (type == MEMTYPE_DOS) {
			mb->_flags &= ~MEMFLAG_HAS_TYPE;
		} else {
			mb->_flags |= MEMFLAG_HAS_TYPE;
		}

		mb->_type = type;
	}
}

int Memory::lock_handle(void **handle) {
	if (handle) {
		MemoryBlock *mb = find_master(*handle);
		if (mb && (mb->_flags & MEMFLAG_HAS_HANDLE)) {
			void *&ptr = _handle_table[mb->_handleIndex];
			if (ptr == *handle) {
				mb->_flags |= MEMFLAG_LOCKED;
				return 0;
			}
		}
	}

	return -1;
}

int Memory::unlock_handle(void **handle) {
	if (handle) {
		MemoryBlock *mb = find_master(*handle);
		if (mb && (mb->_flags & MEMFLAG_HAS_HANDLE)) {
			void *&ptr = _handle_table[mb->_handleIndex];
			if (ptr == *handle) {
				mb->_flags &= ~MEMFLAG_LOCKED;
				return 0;
			}
		}
	}

	return -1;
}

int Memory::kill_handle(void **handle) {
	if (handle) {
		MemoryBlock *mb = find_master(*handle);
		if (mb && (mb->_flags & MEMFLAG_HAS_HANDLE)) {
			void *&ptr = _handle_table[mb->_handleIndex];
			if (ptr == *handle) {
				ptr = nullptr;
				delete_master(mb);
				return 0;
			}
		}
	}

	return -1;
}

void Memory::dump_master_table() {
#ifdef TODO
	int handleCount = 0;
	for (int i = 0; i < _MAX_BLOCKS; ++i) {
		if (_handle_table[i])
			++handleCount;
	}

	debug("memmgr: managing %d total bytes", _memory_avail);
	debug("%d of %d master table entries used",
		_master_table_end, _master_table_size);
	debug("%d of %d handles allocated", handleCount,
		_MAX_BLOCKS);
/*
	MemoryBlock *mb1 = &_master_table[0];
	MemoryBlock *mb2 = &_master_table[1];
	for (int idx = 0; idx < _master_table_end; ++idx) {
		printf()
		(mb1->_flags & MEMFLAG_HAS_TYPE) ? 
	}
	*/
#endif
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
