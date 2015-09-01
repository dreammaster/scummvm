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

#include "aesop/aesop.h"
#include "aesop/defs.h"
#include "aesop/interpreter.h"
#include "aesop/rtsystem.h"

namespace Aesop {

// fundamental stack value structure
struct STKVAL {
	ULONG val;
	UWORD type;
};

enum {
	TYP_CRES,                  // data type: code resource address
	TYP_SRES,                  // data type: string resource
	TYP_VSHR,                  // data type: short integer variable
	TYP_VLNG,                  // data type: long integer variable
	TYP_SVAR,                  // data type: string variable
};

// Pointer and memory block management

#define norm(x) ((void *) (x))

/*----------------------------------------------------------------*/

Interpreter::Interpreter(AesopEngine *vm, HRES *objList, int stackSize) : _vm(vm) {
	_objList = objList;
	_stackBase = (byte *)mem_alloc(stackSize);
	_stackSize = stackSize;

	_currentIndex = 0;
	_currentMsg = 0;
	_currentThis = 0;
	_stackOffset = 0;
	_instance = HRES_NULL;
	_thunk = nullptr;
	_ds32 = 0;
	_fptr = 0;
	_hPrg = HRES_NULL;
}

Interpreter::~Interpreter() {
	mem_free(_stackBase);
}

uint Interpreter::first(HRES dictionary) {
	// Skip over hash table
	const byte *srcP = (const byte *)dictionary + READ_LE_UINT16(dictionary) * 4 + 2;

	int nameLen = READ_LE_UINT16(srcP);
	return !nameLen ? 0 : srcP - (const byte *)dictionary;
}

uint Interpreter::iterate(const void *base, uint cur, const char **tag, const char **def) {
	if (!cur)
		return 0;

	// Check whether we're at the end of the chain list
	const char *srcP = (const char *)base + cur;
	if (READ_LE_UINT16(srcP) == 0) {
		srcP += 2;
		if (READ_LE_UINT16(srcP) == 0)
			return 0;
	}

	// We're not, so save a pointer to the tag and definition
	*tag = srcP + 2;

	srcP += READ_LE_UINT16(srcP) + 2;
	*def = srcP + 2;

	// Return the offset of the next chain entry
	srcP += READ_LE_UINT16(srcP) + 2;
	return srcP - (const char *)base;
}

const char *Interpreter::lookup(HRES dictionary, const Common::String &key) {
	const byte *es32 = (byte *)static_cast<HD_entry *>(dictionary)->_seg;
	int hashSize = READ_LE_UINT16(es32);

	// Calculate the hash for the key
	const char *keyP = key.c_str();
	int hashTotal = 0;
	while (*keyP)
		hashTotal += *keyP++;
	int hash = hashTotal % hashSize;

	// Get the entry, and check whether there's any resource names in that slot
	uint32 namesOffset = READ_LE_UINT32(es32 + hash * 4 + 2);
	if (!namesOffset)
		return nullptr;

	// Scan through the list of names with that hash to see if our resource is one of them
	const char *names = (const char *)es32 + namesOffset;
	int keySize = key.size() + 1;
	int nameLen;
	for (;;) {
		nameLen = READ_LE_UINT16(names);
		if (!nameLen)
			break;

		if (keySize == nameLen && !strcmp(key.c_str(), (const char *)(names + 2)))
			// Found a name match
			return names + 2 + nameLen + 2;

		names += nameLen + 2;
		names += READ_LE_UINT16(names) + 2;
	}

	// At this point, the key wasn't found, so return nothing
	return nullptr;
}

void Interpreter::arguments(void *base, ULONG size) {
	error("TODO: Reimplement ASM as CPP code");
}

LONG Interpreter::execute(LONG index, LONG msgNum, HRES vector) {
	Resources &res = *_vm->_resources;

	// Check the passed index
	if (index == -1)
		return -1;
	_currentIndex = index;

	// Get a handle to the object
	HRES handle = _objList[index];
	if (handle == HRES_NULL)
		return -1;
	_instance = handle;

	IHDR *hdr = (IHDR *)res.addr(handle);
	_thunk = (THDR *)res.addr(hdr->_thunk);
	const MV_entry *curVector;

	if (vector == HRES_NULL || vector == (HRES)0xffff) {
		const MV_entry *mvList = (const MV_entry *)((const byte *)_thunk + _thunk->_mvList);
		UWORD maxMsg = _thunk->_maxMsg;

		if (maxMsg == (UWORD)-1)
			return -1;
		_currentMsg = msgNum;

		// Find the vector using a binary search
		int idx;
		for (idx = 0; mvList[(maxMsg + idx) & 0xfffe].msg != msgNum; ) {			
			int offset = (maxMsg + idx) & 0xfffe;
			int msg = mvList[offset].msg;

			if (msg > msgNum) {
				// Too high
				maxMsg = offset / 2 - 1;
				if (idx > maxMsg)
					return -1;
			} else {
				// Too low
				idx = offset / 2 + 1;
				if (idx > maxMsg)
					return -1;
			}
		}

		curVector = &mvList[(maxMsg + idx) & 0xfffe];
		++curVector;

		do {
			--curVector;
		} while (curVector > mvList && (curVector - 1)->msg == msgNum);
	} else {
		curVector = (MV_entry *)vector;
	}

	const SD_entry *sdEntry = (const SD_entry *)((const byte *)_thunk + curVector->SD_offset);
	_thisOffset = curVector->handler;
	_hPrg = sdEntry->SOP;
	UWORD staticOffset = sdEntry->static_base;
	UWORD externOffset = sdEntry->extern_base;

	res.lock(_hPrg);
	_ds32 = 0;

	deref();

	_fptr = _stackOffset;
	//TODO

	while (!_vm->shouldQuit()) {
		error("TODO");
	}

	return 0;
}

void Interpreter::deref() {
	_thisOffset -= _ds32;
	
	_thisOffset += _ds32;
}

} // End of namespace Aesop
