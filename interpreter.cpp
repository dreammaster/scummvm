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

const OpcodeMethod Interpreter::_opcodes[] = {
	&Interpreter::cmdBRT,
	&Interpreter::cmdBRF,
	&Interpreter::cmdBRA,
	&Interpreter::cmdCASE,
	&Interpreter::cmdPUSH,
	&Interpreter::cmdDUP,
	&Interpreter::cmdNOT,
	&Interpreter::cmdSETB,
	&Interpreter::cmdNEG,
	&Interpreter::cmdADD,
	&Interpreter::cmdSUB,
	&Interpreter::cmdMUL,
	&Interpreter::cmdDIV,
	&Interpreter::cmdMOD,
	&Interpreter::cmdEXP,
	&Interpreter::cmdBAND,
	&Interpreter::cmdBOR,
	&Interpreter::cmdXOR,
	&Interpreter::cmdBNOT,
	&Interpreter::cmdSHL,
	&Interpreter::cmdSHR,
	&Interpreter::cmdLT,
	&Interpreter::cmdLE,
	&Interpreter::cmdEQ,
	&Interpreter::cmdNE,
	&Interpreter::cmdGE,
	&Interpreter::cmdGT,
	&Interpreter::cmdINC,
	&Interpreter::cmdDEC,
	&Interpreter::cmdSHTC,
	&Interpreter::cmdINTC,
	&Interpreter::cmdLNGC,
	&Interpreter::cmdRCRS,
	&Interpreter::cmdCALL,
	&Interpreter::cmdSEND,
	&Interpreter::cmdPASS,
	&Interpreter::cmdJSR,
	&Interpreter::cmdRTS,
	&Interpreter::cmdAIM,
	&Interpreter::cmdAIS,
	&Interpreter::cmdLTBA,
	&Interpreter::cmdLTWA,
	&Interpreter::cmdLTDA,
	&Interpreter::cmdLETA,
	&Interpreter::cmdLAB,
	&Interpreter::cmdLAW,
	&Interpreter::cmdLAD,
	&Interpreter::cmdSAB,
	&Interpreter::cmdSAW,
	&Interpreter::cmdSAD,
	&Interpreter::cmdLABA,
	&Interpreter::cmdLAWA,
	&Interpreter::cmdLADA,
	&Interpreter::cmdSABA,
	&Interpreter::cmdSAWA,
	&Interpreter::cmdSADA,
	&Interpreter::cmdLEAA,
	&Interpreter::cmdLSB,
	&Interpreter::cmdLSW,
	&Interpreter::cmdLSD,
	&Interpreter::cmdSSB,
	&Interpreter::cmdSSW,
	&Interpreter::cmdSSD,
	&Interpreter::cmdLSBA,
	&Interpreter::cmdLSWA,
	&Interpreter::cmdLSDA,
	&Interpreter::cmdSSBA,
	&Interpreter::cmdSSWA,
	&Interpreter::cmdSSDA,
	&Interpreter::cmdLESA,
	&Interpreter::cmdLXB,
	&Interpreter::cmdLXW,
	&Interpreter::cmdLXD,
	&Interpreter::cmdSXB,
	&Interpreter::cmdSXW,
	&Interpreter::cmdSXD,
	&Interpreter::cmdLXBA,
	&Interpreter::cmdLXWA,
	&Interpreter::cmdLXDA,
	&Interpreter::cmdSXBA,
	&Interpreter::cmdSXWA,
	&Interpreter::cmdSXDA,
	&Interpreter::cmdLEXA,
	&Interpreter::cmdSXAS,
	&Interpreter::cmdLECA,
	&Interpreter::cmdSOLE,
	&Interpreter::cmdEND,
	&Interpreter::cmdBRK
};

Interpreter::Interpreter(AesopEngine *vm, HRES *objList, int stackSize) : _vm(vm) {
	_objList = objList;

	_currentIndex = 0;
	_currentMsg = 0;
	_currentThis = 0;
	_instance = HRES_NULL;
	_thunk = nullptr;
	_ds32 = nullptr;
	_code = nullptr;
	_stackBase = 0;
	_hPrg = HRES_NULL;
	_staticOffset = 0;
	_externOffset = 0;
	_breakFlag = false;
}

Interpreter::~Interpreter() {
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
	const int OPCODES_COUNT = sizeof(_opcodes) / sizeof(OpcodeMethod);

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
	uint codeOffset = curVector->handler;
	_hPrg = sdEntry->SOP;
	_staticOffset = sdEntry->static_base;
	_externOffset = sdEntry->extern_base;

	// Set up the code pointer
	res.lock(_hPrg);
	_ds32 = (const byte *)Resources::addr(_hPrg);
	_code = _ds32 + codeOffset;
	_breakFlag = false;

	// Initialize the stack
	_stackBase = 0;
	_stack.push(index);
	int count = READ_LE_UINT16(_code) / 4;
	_code += 2;
	_stack.reserve(count);
	
	// Main opcode execution loop
	while (!_vm->shouldQuit() && !_breakFlag) {
		int opcode = *_code++;
		if (opcode >= OPCODES_COUNT)
			error("Invalid opcode encountered");

		(this->*_opcodes[opcode])();
	}

	res.unlock(_hPrg);
	return 0;
}

void Interpreter::deref() {
	/*
	int diff = (const byte *)_code - (const byte *)_ds32;
	_ds32 = Resources::addr(_hPrg);
	_code = (HRES)((const byte *)_ds32 + diff);
	*/
}

void Interpreter::cmdBRT() {
	if (_stack.top()) {
		// Branch
		_code = _ds32 + READ_LE_UINT16(_code);
	} else {
		// Don't branch
		_code += 2;
	}
}

void Interpreter::cmdBRF() {
	if (!_stack.top()) {
		// Branch
		_code = _ds32 + READ_LE_UINT16(_code);
	} else {
		// Don't branch
		_code += 2;
	}
}

void Interpreter::cmdBRA() {
	_code = _ds32 + READ_LE_UINT16(_code);
}

void Interpreter::cmdCASE() {
	// Get value of test expression
	uint32 v = _stack.pop();
	int numCases = READ_LE_UINT16(_code); _code += 2;

	// Scan through case list to find a matching value
	for (int idx = 0; idx < numCases; ++idx, _code += 6) {
		if (READ_LE_UINT32(_code) == v) {
			_code += 4;
			break;
		}
	}

	cmdBRA();
}

void Interpreter::cmdPUSH() {
	_stack.push(0);
}

void Interpreter::cmdDUP() {
	_stack.push(_stack.top());
}

void Interpreter::cmdNOT() {
	uint32 v = _stack.pop();
	_stack.push(v ? 0 : 1);
}

void Interpreter::cmdSETB() {
	uint32 v = _stack.pop();
	_stack.push(v ? 0xffff : 0);
}

void Interpreter::cmdNEG() {
	uint32 v = _stack.pop();
	_stack.push(v ^ v);
}

void Interpreter::cmdADD() {
	uint32 v1 = _stack.pop();
	uint32 v2 = _stack.pop();
	_stack.push(v2 + v1);
}

void Interpreter::cmdSUB() {
	uint32 v1 = _stack.pop();
	uint32 v2 = _stack.pop();
	_stack.push(v2 - v1);
}

void Interpreter::cmdMUL() {
	uint32 v1 = _stack.pop();
	uint32 v2 = _stack.pop();
	_stack.push(v2 * v1);
}

void Interpreter::cmdDIV() {
	uint32 v1 = _stack.pop();
	uint32 v2 = _stack.pop();
	_stack.push(v2 / v1);
}

void Interpreter::cmdMOD() {
	uint32 v1 = _stack.pop();
	uint32 v2 = _stack.pop();
	_stack.push(v2 % v1);
}

void Interpreter::cmdEXP() {
	uint32 v1 = _stack.pop() & 0xffff;
	int result = 1;

	if (v1 == 0) {
		_stack.pop();
	} else {
		uint32 v2 = _stack.pop();
		result = v2;
		for (uint idx = 2; idx < v1; ++idx)
			result *= v2;
	}

	_stack.push(result);
}

void Interpreter::cmdBAND() {
	uint32 v1 = _stack.pop();
	uint32 v2 = _stack.pop();
	_stack.push(v2 & v1);
}

void Interpreter::cmdBOR() {
	uint32 v1 = _stack.pop();
	uint32 v2 = _stack.pop();
	_stack.push(v2 | v1);
}

void Interpreter::cmdXOR() {
	uint32 v1 = _stack.pop();
	uint32 v2 = _stack.pop();
	_stack.push(v2 ^ v1);
}

void Interpreter::cmdBNOT() {
	uint32 v = _stack.pop();
	_stack.push(v ^ v);
}

void Interpreter::cmdSHL() {
	uint32 v1 = _stack.pop() & 0xffff;
	uint32 v2 = _stack.pop();
	_stack.push(v2 << v1);
}

void Interpreter::cmdSHR() {
	uint32 v1 = _stack.pop() & 0xffff;
	uint32 v2 = _stack.pop();
	_stack.push(v2 >> v1);
}

void Interpreter::cmdLT() {
	uint32 v1 = _stack.pop();
	uint32 v2 = _stack.pop();
	_stack.push(v2 < v1 ? 1 : 0);
}

void Interpreter::cmdLE() {
	uint32 v1 = _stack.pop();
	uint32 v2 = _stack.pop();
	_stack.push(v2 <= v1 ? 1 : 0);
}

void Interpreter::cmdEQ() {
	uint32 v1 = _stack.pop();
	uint32 v2 = _stack.pop();
	_stack.push(v2 == v1 ? 1 : 0);
}

void Interpreter::cmdNE() {
	uint32 v1 = _stack.pop();
	uint32 v2 = _stack.pop();
	_stack.push(v2 != v1 ? 1 : 0);
}

void Interpreter::cmdGE() {
	uint32 v1 = _stack.pop();
	uint32 v2 = _stack.pop();
	_stack.push(v2 >= v1 ? 1 : 0);
}

void Interpreter::cmdGT() {
	uint32 v1 = _stack.pop();
	uint32 v2 = _stack.pop();
	_stack.push(v2 > v1 ? 1 : 0);
}

void Interpreter::cmdINC() {
	_stack.push(_stack.pop() + 1);
}

void Interpreter::cmdDEC() {
	_stack.push(_stack.pop() - 1);
}

void Interpreter::cmdSHTC() {
	_stack.push(*_code++);
}	

void Interpreter::cmdINTC() {
	uint16 v = READ_LE_UINT16(_code); _code += 2;
	_stack.push(v);
}

void Interpreter::cmdLNGC() {
	uint32 v = READ_LE_UINT32(_code); _code += 4;
	_stack.push(v);
}

void Interpreter::cmdRCRS() {
	// Get index into external offset list
	uint offset = READ_LE_UINT16(_code) + _externOffset;
	_code += 2;

	const byte *data = (const byte *)_thunk + offset;
	_stack.push(READ_LE_UINT32(data));
}

void Interpreter::cmdCALL() {
	error("TODO: opcode");
}

void Interpreter::cmdSEND() {
	error("TODO: opcode");
}

void Interpreter::cmdPASS() {
	error("TODO: opcode");
}

void Interpreter::cmdJSR() {
	// Get the subroutine method offset, and save details for where to resume
	uint16 methodOffset = READ_LE_UINT16(_code); _code += 2;
	_methodStack.push(MethodStackEntry(_ds32, _code, _stack.size(), _stackBase));
	
	// Set code and stack start
	_code = _ds32 + methodOffset;
	_stackBase = _stack.size();

	// Initialize manifest THIS var	
	_stack.push(_currentIndex);

	// Set up the method autos
	uint numAutos = READ_LE_UINT16(_code); _code += 2;
	for (uint idx = 0; idx < numAutos / 4; ++idx)
		_stack.push(0);
}

void Interpreter::cmdRTS() {
	uint32 returnValue = _stack.pop();
	MethodStackEntry stackEntry = _methodStack.pop();
	_stackBase = stackEntry._stackBase;
	_stack.resize(stackEntry._stackSize);
	_code = stackEntry._code;
	_ds32 = stackEntry._ds32;

	// PUsh return value back onto the stack
	_stack.push(returnValue);
}

void Interpreter::cmdAIM() {
	uint32 v1 = _stack.pop() & 0xffff;
	uint32 v2 = READ_LE_UINT16(_code); _code += 2;
	_stack.push(_stack.pop() + v1 * v2);
}

void Interpreter::cmdAIS() {
	uint32 v1 = _stack.pop() & 0xffff;
	uint32 v2 = *_code++;
	_stack.push(_stack.pop() + (v1 << v2));
}

void Interpreter::cmdLTBA() {
	uint32 offset = READ_LE_UINT16(_code); _code += 2;
	const byte *srcP = _ds32 + offset + _stack.pop();
	_stack.push(*srcP);
}

void Interpreter::cmdLTWA() {
	uint32 offset = READ_LE_UINT16(_code); _code += 2;
	const byte *srcP = _ds32 + offset + _stack.pop();
	_stack.push(READ_LE_UINT16(srcP));
}

void Interpreter::cmdLTDA() {
	uint32 offset = READ_LE_UINT16(_code); _code += 2;
	const byte *srcP = _ds32 + offset + _stack.pop();
	_stack.push(READ_LE_UINT32(srcP));
}

void Interpreter::cmdLETA() {
	uint32 offset = READ_LE_UINT16(_code); _code += 2;
	const byte *srcP = _ds32 + offset;
	_stack.pushPtr(srcP);
}

void Interpreter::cmdLAB() {
	error("TODO: opcode");
}

void Interpreter::cmdLAW() {
	error("TODO: opcode");
}

void Interpreter::cmdLAD() {
	error("TODO: opcode");
}

void Interpreter::cmdSAB() {
	error("TODO: opcode");
}

void Interpreter::cmdSAW() {
	error("TODO: opcode");
}

void Interpreter::cmdSAD() {
	error("TODO: opcode");
}

void Interpreter::cmdLABA() {
	error("TODO: opcode");
}

void Interpreter::cmdLAWA() {
	error("TODO: opcode");
}

void Interpreter::cmdLADA() {
	error("TODO: opcode");
}

void Interpreter::cmdSABA() {
	error("TODO: opcode");
}

void Interpreter::cmdSAWA() {
	error("TODO: opcode");
}

void Interpreter::cmdSADA() {
	error("TODO: opcode");
}

void Interpreter::cmdLEAA() {
	error("TODO: opcode");
}

void Interpreter::cmdLSB() {
	error("TODO: opcode");
}

void Interpreter::cmdLSW() {
	error("TODO: opcode");
}

void Interpreter::cmdLSD() {
	error("TODO: opcode");
}

void Interpreter::cmdSSB() {
	error("TODO: opcode");
}

void Interpreter::cmdSSW() {
	error("TODO: opcode");
}

void Interpreter::cmdSSD() {
	error("TODO: opcode");
}

void Interpreter::cmdLSBA() {
	error("TODO: opcode");
}

void Interpreter::cmdLSWA() {
	error("TODO: opcode");
}

void Interpreter::cmdLSDA() {
	error("TODO: opcode");
}

void Interpreter::cmdSSBA() {
	error("TODO: opcode");
}

void Interpreter::cmdSSWA() {
	error("TODO: opcode");
}

void Interpreter::cmdSSDA() {
	error("TODO: opcode");
}

void Interpreter::cmdLESA() {
	error("TODO: opcode");
}

void Interpreter::cmdLXB() {
	error("TODO: opcode");
}

void Interpreter::cmdLXW() {
	error("TODO: opcode");
}

void Interpreter::cmdLXD() {
	error("TODO: opcode");
}

void Interpreter::cmdSXB() {
	error("TODO: opcode");
}

void Interpreter::cmdSXW() {
	error("TODO: opcode");
}

void Interpreter::cmdSXD() {
	error("TODO: opcode");
}

void Interpreter::cmdLXBA() {
	error("TODO: opcode");
}

void Interpreter::cmdLXWA() {
	error("TODO: opcode");
}

void Interpreter::cmdLXDA() {
	error("TODO: opcode");
}

void Interpreter::cmdSXBA() {
	error("TODO: opcode");
}

void Interpreter::cmdSXWA() {
	error("TODO: opcode");
}

void Interpreter::cmdSXDA() {
	error("TODO: opcode");
}

void Interpreter::cmdLEXA() {
	error("TODO: opcode");
}

void Interpreter::cmdSXAS() {
	error("TODO: opcode");
}

void Interpreter::cmdLECA() {
	error("TODO: opcode");
}

void Interpreter::cmdSOLE() {
	error("TODO: opcode");
}

void Interpreter::cmdEND() {
	error("TODO: opcode");
}

void Interpreter::cmdBRK() {
	error("TODO: opcode");
}

} // End of namespace Aesop
