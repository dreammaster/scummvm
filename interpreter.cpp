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
#include "aesop/interpreter.h"
#include "aesop/defs.h"
#include "aesop/eye.h"
#include "aesop/graphics.h"
#include "aesop/rtmsg.h"
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

const Parameter Parameter::operator+(int other) const {
	Parameter p;
	p._val = _val + other;
	if (_ptr)
		p._ptr = (void *)((byte *)_ptr + other);
	if (_ptrConst)
		p._ptrConst = (const void *)((const byte *)_ptrConst + other);

	return p;
}

const Parameter Parameter::operator-(int other) const {
	Parameter p;
	p._val = _val - other;
	if (_ptr)
		p._ptr = (void *)((byte *)_ptr - other);
	if (_ptrConst)
		p._ptrConst = (const void *)((const byte *)_ptrConst - other);

	return p;
}

Parameter Parameter::operator&(int other) {
	return Parameter(_val & other);
}

Parameter &Parameter::operator|=(int other) {
	_val |= other;
	return *this;
}

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

const ExternMethod Interpreter::_methods[] = {
	&Interpreter::loadString,
	&Interpreter::loadResource,
	&Interpreter::copyString,
	&Interpreter::stringForceLower,
	&Interpreter::stringForceUpper,
	&Interpreter::stringLen,
	&Interpreter::stringCompare,
	&Interpreter::strVal,
	&Interpreter::envVal,
	&Interpreter::beep,
	&Interpreter::pokeMem,
	&Interpreter::peekMem,
	&Interpreter::rnd,
	&Interpreter::dice,
	&Interpreter::absv,
	&Interpreter::minv,
	&Interpreter::maxv,
	&Interpreter::diagnose,
	&Interpreter::heapfree,

	// Event functions
	&Interpreter::notify,
	&Interpreter::cancel,
	&Interpreter::drainEventQueue,
	&Interpreter::postEvent,
	&Interpreter::sendEvent,
	&Interpreter::peekEvent,
	&Interpreter::dispatchEvent,
	&Interpreter::flushEventQueue,
	&Interpreter::flushInputEvents,

	// Interface functions
	&Interpreter::initInterface,
	&Interpreter::shutdownInterface,
	&Interpreter::setMousePointer,
	&Interpreter::setWaitPointer,
	&Interpreter::standbyCursor,
	&Interpreter::resumeCursor,
	&Interpreter::showMouse,
	&Interpreter::hideMouse,
	&Interpreter::mouseXY,
	&Interpreter::mouseInWindow,
	&Interpreter::lockMouse,
	&Interpreter::unlockMouse,
	&Interpreter::getKey,

	// Graphics-related functions
	&Interpreter::initGraphics,
	&Interpreter::drawDot,
	&Interpreter::drawLine,
	&Interpreter::lineTo,
	&Interpreter::drawRectangle,
	&Interpreter::fillRectangle,
	&Interpreter::hashRectangle,
	&Interpreter::getBitmapHeight,
	&Interpreter::drawBitmap,
	&Interpreter::visibleBitmapRect,
	&Interpreter::setPalette,
	&Interpreter::refreshWindow,
	&Interpreter::wipeWindow,
	&Interpreter::shutdownGraphics,
	&Interpreter::waitVerticalRetrace,
	&Interpreter::readPalette,
	&Interpreter::writePalette,
	&Interpreter::pixelFade,
	&Interpreter::colorFade,
	&Interpreter::lightFade,

	&Interpreter::assignWindow,
	&Interpreter::assignSubWindow,
	&Interpreter::releaseWindow,
	&Interpreter::getLeft,
	&Interpreter::getRight,
	&Interpreter::getTop,
	&Interpreter::getBottom,
	&Interpreter::setLeft,
	&Interpreter::setRight,
	&Interpreter::setTop,
	&Interpreter::setBottom,

	&Interpreter::textWindow,
	&Interpreter::textStyle,
	&Interpreter::textXy,
	&Interpreter::textColor,
	&Interpreter::textRefreshWindow,
	&Interpreter::getTextX,
	&Interpreter::getTextY,
	&Interpreter::home,
	&Interpreter::print,
	&Interpreter::sPrint,
	&Interpreter::dPrint,
	&Interpreter::aPrint,
	&Interpreter::crOut,
	&Interpreter::charWidth,
	&Interpreter::fontHeight,

	&Interpreter::solidBarGraph,

	// Sound-related functions
	&Interpreter::initSound,
	&Interpreter::shutdownSound,
	&Interpreter::loadSoundBlock,
	&Interpreter::soundEffect,
	&Interpreter::playSequence,
	&Interpreter::loadMusic,
	&Interpreter::unloadMusic,
	&Interpreter::setSoundStatus,

	// Eye III object management
	&Interpreter::createObject,
	&Interpreter::createProgram,
	&Interpreter::destroyObject,
	&Interpreter::flushCache,
	&Interpreter::thrashCache,

	// Eye III support functions
	&Interpreter::stepX,
	&Interpreter::stepY,
	&Interpreter::stepFDIR,

	&Interpreter::stepSquareX,
	&Interpreter::stepSquareY,
	&Interpreter::stepRegion,

	&Interpreter::distance,
	&Interpreter::seekDirection,

	&Interpreter::spellRequest,
	&Interpreter::spellList,
	&Interpreter::magicField,
	&Interpreter::doDots,
	&Interpreter::doIce,

	&Interpreter::readSaveDirectory,
	&Interpreter::savegameTitle,
	&Interpreter::writeSaveDirectory,

	&Interpreter::saveGame,
	&Interpreter::suspendGame,
	&Interpreter::resumeItems,
	&Interpreter::resumeLevel,
	&Interpreter::changeLevel,
	&Interpreter::restoreItems,
	&Interpreter::restoreLevelObjects,
	&Interpreter::readInitialItems,
	&Interpreter::writeInitialTempfiles,
	&Interpreter::createInitialBinaryFiles,
	&Interpreter::launch,

	// Eye II savegame file access
	&Interpreter::openTransferFile,
	&Interpreter::closeTransferFile,
	&Interpreter::playerAttrib,
	&Interpreter::itemAttrib,
	&Interpreter::arrowCount
};

/*----------------------------------------------------------------*/

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
	_ds32 = (byte *)Resources::addr(_hPrg);
	_code = _ds32 + codeOffset;
	_offThis = _currentIndex;
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
	return _stack.top();
}

void Interpreter::cmdBRT() {
	if ((ULONG)_stack.top()) {
		// Branch
		_code = _ds32 + READ_LE_UINT16(_code);
	} else {
		// Don't branch
		_code += 2;
	}
}

void Interpreter::cmdBRF() {
	if (!(ULONG)_stack.top()) {
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
	ULONG v = _stack.pop();
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
	_stack.push(Parameter());
}

void Interpreter::cmdDUP() {
	_stack.push(_stack.top());
}

void Interpreter::cmdNOT() {
	ULONG v = _stack.pop();
	_stack.push(v ? 0 : 1);
}

void Interpreter::cmdSETB() {
	ULONG v = _stack.pop();
	_stack.push(v ? 0xffff : 0);
}

void Interpreter::cmdNEG() {
	ULONG v = _stack.pop();
	_stack.push(v ^ v);
}

void Interpreter::cmdADD() {
	ULONG v1 = _stack.pop();
	ULONG v2 = _stack.pop();
	_stack.push(v2 + v1);
}

void Interpreter::cmdSUB() {
	ULONG v1 = _stack.pop();
	ULONG v2 = _stack.pop();
	_stack.push(v2 - v1);
}

void Interpreter::cmdMUL() {
	ULONG v1 = _stack.pop();
	ULONG v2 = _stack.pop();
	_stack.push(v2 * v1);
}

void Interpreter::cmdDIV() {
	ULONG v1 = _stack.pop();
	ULONG v2 = _stack.pop();
	_stack.push(v2 / v1);
}

void Interpreter::cmdMOD() {
	ULONG v1 = _stack.pop();
	ULONG v2 = _stack.pop();
	_stack.push(v2 % v1);
}

void Interpreter::cmdEXP() {
	ULONG v1 = _stack.pop() & 0xffff;
	int result = 1;

	if (v1 == 0) {
		_stack.pop();
	} else {
		ULONG v2 = _stack.pop();
		result = v2;
		for (uint idx = 2; idx < v1; ++idx)
			result *= v2;
	}

	_stack.push(result);
}

void Interpreter::cmdBAND() {
	ULONG v1 = _stack.pop();
	ULONG v2 = _stack.pop();
	_stack.push(v2 & v1);
}

void Interpreter::cmdBOR() {
	ULONG v1 = _stack.pop();
	ULONG v2 = _stack.pop();
	_stack.push(v2 | v1);
}

void Interpreter::cmdXOR() {
	ULONG v1 = _stack.pop();
	ULONG v2 = _stack.pop();
	_stack.push(v2 ^ v1);
}

void Interpreter::cmdBNOT() {
	ULONG v = _stack.pop();
	_stack.push(v ^ v);
}

void Interpreter::cmdSHL() {
	ULONG v1 = _stack.pop() & 0xffff;
	ULONG v2 = _stack.pop();
	_stack.push(v2 << v1);
}

void Interpreter::cmdSHR() {
	ULONG v1 = _stack.pop() & 0xffff;
	ULONG v2 = _stack.pop();
	_stack.push(v2 >> v1);
}

void Interpreter::cmdLT() {
	ULONG v1 = _stack.pop();
	ULONG v2 = _stack.pop();
	_stack.push(v2 < v1 ? 1 : 0);
}

void Interpreter::cmdLE() {
	ULONG v1 = _stack.pop();
	ULONG v2 = _stack.pop();
	_stack.push(v2 <= v1 ? 1 : 0);
}

void Interpreter::cmdEQ() {
	ULONG v1 = _stack.pop();
	ULONG v2 = _stack.pop();
	_stack.push(v2 == v1 ? 1 : 0);
}

void Interpreter::cmdNE() {
	ULONG v1 = _stack.pop();
	ULONG v2 = _stack.pop();
	_stack.push(v2 != v1 ? 1 : 0);
}

void Interpreter::cmdGE() {
	ULONG v1 = _stack.pop();
	ULONG v2 = _stack.pop();
	_stack.push(v2 >= v1 ? 1 : 0);
}

void Interpreter::cmdGT() {
	ULONG v1 = _stack.pop();
	ULONG v2 = _stack.pop();
	_stack.push(v2 > v1 ? 1 : 0);
}

void Interpreter::cmdINC() {
	_stack.push(_stack.pop() + 1);
}

void Interpreter::cmdDEC() {
	_stack.push(_stack.pop() - 1);
}

void Interpreter::cmdSHTC() {
	_stack.top() = *_code++;
}	

void Interpreter::cmdINTC() {
	uint16 v = READ_LE_UINT16(_code); _code += 2;
	_stack.top() = v;
}

void Interpreter::cmdLNGC() {
	uint32 v = READ_LE_UINT32(_code); _code += 4;
	_stack.push(v);
}

void Interpreter::cmdRCRS() {
	// Get index into method offset list
	uint offset = READ_LE_UINT16(_code); _code += 2;
	uint32 methodNum = READ_LE_UINT32((const byte *)_thunk + _externOffset + offset);

	// Validate that it's a correctly formatted method number, then push it to the stack
	assert((methodNum >> 16) == 0x1234);
	_stack.push(methodNum);
}

void Interpreter::cmdCALL() {
	_currentThis = _currentIndex;

	// Load the arguments
	Parameters params;
	int argCount = *_code++;
	for (int idx = 0; idx < argCount; ++idx)
		params.push_back(_stack.pop());

	// Get the method number
	ULONG methodNum = _stack.pop();
	assert((methodNum >> 16) == 0x1234);

	// Handle the call
	uint stackSize = _stack.size();
	int result = (this->*_methods[methodNum & 0xffff])(params);

	_stack.resize(stackSize);
	_stack.push(result);
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
	_offThis = _currentIndex;

	// Initialize manifest THIS var	
	_stack.push(_currentIndex);

	// Set up the method autos
	uint numAutos = READ_LE_UINT16(_code); _code += 2;
	for (uint idx = 0; idx < numAutos / 4; ++idx)
		_stack.push(Parameter());
}

void Interpreter::cmdRTS() {
	ULONG returnValue = _stack.pop();
	MethodStackEntry stackEntry = _methodStack.pop();
	_stackBase = stackEntry._stackBase;
	_stack.resize(stackEntry._stackSize);
	_code = stackEntry._code;
	_ds32 = stackEntry._ds32;

	// PUsh return value back onto the stack
	_stack.push(returnValue);
}

void Interpreter::cmdAIM() {
	ULONG v1 = _stack.pop() & 0xffff;
	ULONG v2 = READ_LE_UINT16(_code); _code += 2;
	int index = v1 * v2;
	_stack.push(_stack.pop() + index);
}

void Interpreter::cmdAIS() {
	ULONG v1 = _stack.pop() & 0xffff;
	ULONG v2 = *_code++;
	int index = v1 << v2;
	_stack.push(_stack.pop() + index);
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
	BYTE *srcP = (BYTE *)_ds32 + offset;
	_stack.push((BYTE *)srcP);
}

void Interpreter::cmdLAB() {
	uint32 offset = READ_LE_UINT16(_code); _code += 2;
	assert((offset % 4) == 0);

	ULONG v = _stack[_stackBase + offset / 4] & 0xff;
	_stack.top() = v;
}

void Interpreter::cmdLAW() {
	uint32 offset = READ_LE_UINT16(_code); _code += 2;
	assert((offset % 4) == 0);

	ULONG v = _stack[_stackBase + offset / 4] & 0xffff;
	_stack.top() = v;
}

void Interpreter::cmdLAD() {
	uint32 offset = READ_LE_UINT16(_code); _code += 2;
	assert((offset % 4) == 0);

	ULONG v = _stack[_stackBase + offset / 4];
	_stack.top() = v;
}

void Interpreter::cmdSAB() {
	int index4 = READ_LE_UINT16(_code); _code += 2;
	assert(index4 == 0 || (index4 % 4) == 2);
	int index = index4 ? (index4 + 2) / 4 : 0;

	int offset = _stackBase + index;
	_stack[offset] = ((ULONG)_stack[offset] & 0xffffff00) | (_stack.pop() & 0xff);
}

void Interpreter::cmdSAW() {
	int index4 = READ_LE_UINT16(_code); _code += 2;
	assert(index4 == 0 || (index4 % 4) == 2);
	int index = index4 ? (index4 + 2) / 4 : 0;

	int offset = _stackBase + index;
	_stack[offset] = ((ULONG)_stack[offset] & 0xffff0000) | (_stack.pop() & 0xffff);
}

void Interpreter::cmdSAD() {
	int index4 = READ_LE_UINT16(_code); _code += 2;
	assert(index4 == 0 || (index4 % 4) == 2);
	int index = index4 ? (index4 + 2) / 4 : 0;

	_stack[_stackBase + index] = _stack.top();
}

void Interpreter::cmdLABA() {
	int index4 = READ_LE_UINT16(_code); _code += 2;
	assert(index4 == 0 || (index4 % 4) == 2);
	int index = index4 ? (index4 + 2) / 4 : 0;

	int subIndex = _stack.pop();
	assert((subIndex % 4) == 0);

	ULONG v = _stack[_stackBase + index +  subIndex / 4] & 0xff;
	_stack.push(v);
}

void Interpreter::cmdLAWA() {
	int index4 = READ_LE_UINT16(_code); _code += 2;
	assert(index4 == 0 || (index4 % 4) == 2);
	int index = index4 ? (index4 + 2) / 4 : 0;

	int subIndex = _stack.pop();
	assert((subIndex % 4) == 0);

	ULONG v = _stack[_stackBase + index + subIndex / 4] & 0xffff;
	_stack.push(v);
}

void Interpreter::cmdLADA() {
	int index4 = READ_LE_UINT16(_code); _code += 2;
	assert(index4 == 0 || (index4 % 4) == 2);
	int index = index4 ? (index4 + 2) / 4 : 0;

	int subIndex = _stack.pop();
	assert((subIndex % 4) == 0);

	ULONG v = _stack[_stackBase + index + subIndex / 4];
	_stack.push(v);
}

void Interpreter::cmdSABA() {
	int val = _stack.pop();
	int offset = READ_LE_UINT16(_code); _code += 2;
	int subIndex = _stack.pop();
	assert((offset % 4) == 0 && (subIndex % 4) == 0);

	_stack[_stackBase + (offset - subIndex) / 4] = val & 0xff;
}

void Interpreter::cmdSAWA() {
	int val = _stack.pop();
	int offset = READ_LE_UINT16(_code); _code += 2;
	int subIndex = _stack.pop();
	assert((offset % 4) == 0 && (subIndex % 4) == 0);

	_stack[_stackBase + (offset - subIndex) / 4] = val & 0xffff;
}

void Interpreter::cmdSADA() {
	int val = _stack.pop();
	int offset = READ_LE_UINT16(_code); _code += 2;
	int subIndex = _stack.pop();
	assert((offset % 4) == 0 && (subIndex % 4) == 0);

	_stack[_stackBase + (offset - subIndex) / 4] = val;
}

void Interpreter::cmdLEAA() {
	int offset = READ_LE_UINT16(_code); _code += 2;
	assert((offset % 4) == 0);

	BYTE *ptr = (BYTE *)&_stack[_stackBase + (offset / 4)];
	_stack.pop();
	_stack.push(ptr);
}

void Interpreter::cmdLSB() {
	uint32 offset = READ_LE_UINT32(_code); _code += 2;
	byte *srcP = (byte *)_thunk + _staticOffset + offset;

	_stack.top() = *srcP;
}

void Interpreter::cmdLSW() {
	uint32 offset = READ_LE_UINT32(_code); _code += 2;
	byte *srcP = (byte *)_thunk + _staticOffset + offset;

	_stack.top() = READ_LE_UINT16(srcP);
}

void Interpreter::cmdLSD() {
	uint32 offset = READ_LE_UINT32(_code); _code += 2;
	byte *srcP = (byte *)_thunk + _staticOffset + offset;

	_stack.top() = READ_LE_UINT32(srcP);
}

void Interpreter::cmdSSB() {
	uint32 offset = READ_LE_UINT32(_code); _code += 2;
	byte *destP = (byte *)_thunk + _staticOffset + offset;

	*destP = _stack.top() & 0xff;
}

void Interpreter::cmdSSW() {
	uint32 offset = READ_LE_UINT32(_code); _code += 2;
	byte *destP = (byte *)_thunk + _staticOffset + offset;

	WRITE_LE_UINT16(destP, _stack.top() & 0xffff);
}

void Interpreter::cmdSSD() {
	uint32 offset = READ_LE_UINT32(_code); _code += 2;
	byte *destP = (byte *)_thunk + _staticOffset + offset;

	WRITE_LE_UINT32(destP, _stack.top());
}

void Interpreter::cmdLSBA() {
	ULONG offset = READ_LE_UINT16(_code); _code += 2;
	byte *srcP = (byte *)_thunk + _staticOffset + offset + _stack.pop();

	_stack.push(*srcP);
}

void Interpreter::cmdLSWA() {
	ULONG offset = READ_LE_UINT16(_code); _code += 2;
	byte *srcP = (byte *)_thunk + _staticOffset + offset + _stack.pop();

	_stack.push(READ_LE_UINT16(srcP));
}

void Interpreter::cmdLSDA() {
	ULONG offset = READ_LE_UINT16(_code); _code += 2;
	byte *srcP = (byte *)_thunk + _staticOffset + offset + _stack.pop();

	_stack.push(READ_LE_UINT32(srcP));
}

void Interpreter::cmdSSBA() {
	ULONG val = _stack.pop();
	ULONG offset = READ_LE_UINT16(_code); _code += 2;
	byte *destP = (byte *)_thunk + _staticOffset + offset + _stack.pop();

	*destP = val;
}

void Interpreter::cmdSSWA() {
	ULONG val = _stack.pop();
	ULONG offset = READ_LE_UINT16(_code); _code += 2;
	byte *destP = (byte *)_thunk + _staticOffset + offset + _stack.pop();

	WRITE_LE_UINT16(destP, val);
}

void Interpreter::cmdSSDA() {
	ULONG val = _stack.pop();
	ULONG offset = READ_LE_UINT16(_code); _code += 2;
	byte *destP = (byte *)_thunk + _staticOffset + offset + _stack.pop();

	WRITE_LE_UINT32(destP, val);
}

void Interpreter::cmdLESA() {
	ULONG offset = READ_LE_UINT16(_code); _code += 2;
	BYTE *srcP = (BYTE *)_thunk + _staticOffset + offset;

	_stack.top() = srcP;
}

void Interpreter::cmdLXB() {
	ULONG offset = READ_LE_UINT16(_code); _code += 2;
	byte *instP = (byte *)_thunk + _externOffset + offset;
	ULONG instanceOffset = READ_LE_UINT16(instP);

	int objIndex = _stack.pop();
	byte *srcP = (byte *)Resources::addr(_objList[objIndex]) + instanceOffset;

	_stack.push(*srcP);
}

void Interpreter::cmdLXW() {
	ULONG offset = READ_LE_UINT16(_code); _code += 2;
	byte *instP = (byte *)_thunk + _externOffset + offset;
	ULONG instanceOffset = READ_LE_UINT16(instP);

	int objIndex = _stack.pop();
	byte *srcP = (byte *)Resources::addr(_objList[objIndex]) + instanceOffset;

	_stack.push(READ_LE_UINT16(srcP));
}

void Interpreter::cmdLXD() {
	ULONG offset = READ_LE_UINT16(_code); _code += 2;
	byte *instP = (byte *)_thunk + _externOffset + offset;
	ULONG instanceOffset = READ_LE_UINT16(instP);

	int objIndex = _stack.pop();
	byte *srcP = (byte *)Resources::addr(_objList[objIndex]) + instanceOffset;

	_stack.push(READ_LE_UINT32(srcP));
}

void Interpreter::cmdSXB() {
	ULONG val = _stack.pop() & 0xff;

	ULONG offset = READ_LE_UINT16(_code); _code += 2;
	byte *instP = (byte *)_thunk + _externOffset + offset;
	ULONG instanceOffset = READ_LE_UINT16(instP);

	int objIndex = _stack.pop();
	byte *destP = (byte *)Resources::addr(_objList[objIndex]) + instanceOffset;

	*destP = val;
	_stack.push(val);
}

void Interpreter::cmdSXW() {
	ULONG val = _stack.pop();

	ULONG offset = READ_LE_UINT16(_code); _code += 2;
	byte *instP = (byte *)_thunk + _externOffset + offset;
	ULONG instanceOffset = READ_LE_UINT16(instP);

	int objIndex = _stack.pop();
	byte *destP = (byte *)Resources::addr(_objList[objIndex]) + instanceOffset;

	WRITE_LE_UINT16(destP, val);
	_stack.push(val);
}

void Interpreter::cmdSXD() {
	ULONG val = _stack.pop();

	ULONG offset = READ_LE_UINT16(_code); _code += 2;
	byte *instP = (byte *)_thunk + _externOffset + offset;
	ULONG instanceOffset = READ_LE_UINT16(instP);

	int objIndex = _stack.pop();
	byte *destP = (byte *)Resources::addr(_objList[objIndex]) + instanceOffset;

	WRITE_LE_UINT32(destP, val);
	_stack.push(val);
}

void Interpreter::cmdLXBA() {
	ULONG offset = READ_LE_UINT16(_code); _code += 2;
	byte *instP = (byte *)_thunk + _externOffset + offset;
	ULONG instanceOffset = READ_LE_UINT16(instP);

	ULONG v = _stack.pop();
	instanceOffset += v >> 16;
	int objIndex = v & 0xffff;

	byte *srcP = (byte *)Resources::addr(_objList[objIndex]) + instanceOffset;
	_stack.push(*srcP);
}

void Interpreter::cmdLXWA() {
	ULONG offset = READ_LE_UINT16(_code); _code += 2;
	byte *instP = (byte *)_thunk + _externOffset + offset;
	ULONG instanceOffset = READ_LE_UINT16(instP);

	ULONG v = _stack.pop();
	instanceOffset += v >> 16;
	int objIndex = v & 0xffff;

	byte *srcP = (byte *)Resources::addr(_objList[objIndex]) + instanceOffset;
	_stack.push(READ_LE_UINT16(srcP));
}

void Interpreter::cmdLXDA() {
	ULONG offset = READ_LE_UINT16(_code); _code += 2;
	byte *instP = (byte *)_thunk + _externOffset + offset;
	ULONG instanceOffset = READ_LE_UINT16(instP);

	ULONG v = _stack.pop();
	instanceOffset += v >> 16;
	int objIndex = v & 0xffff;

	byte *srcP = (byte *)Resources::addr(_objList[objIndex]) + instanceOffset;
	_stack.push(READ_LE_UINT32(srcP));
}

void Interpreter::cmdSXBA() {
	ULONG val = _stack.pop();

	ULONG offset = READ_LE_UINT16(_code); _code += 2;
	byte *instP = (byte *)_thunk + _externOffset + offset;
	ULONG instanceOffset = READ_LE_UINT16(instP);

	ULONG v = _stack.pop();
	instanceOffset += v >> 16;
	int objIndex = v & 0xffff;

	byte *destP = (byte *)Resources::addr(_objList[objIndex]) + instanceOffset;
	*destP = val & 0xff;
	_stack.push(val);
}

void Interpreter::cmdSXWA() {
	ULONG val = _stack.pop();

	ULONG offset = READ_LE_UINT16(_code); _code += 2;
	byte *instP = (byte *)_thunk + _externOffset + offset;
	ULONG instanceOffset = READ_LE_UINT16(instP);

	ULONG v = _stack.pop();
	instanceOffset += v >> 16;
	int objIndex = v & 0xffff;

	byte *destP = (byte *)Resources::addr(_objList[objIndex]) + instanceOffset;
	*destP = val & 0xffff;
	_stack.push(val);
}

void Interpreter::cmdSXDA() {
	ULONG val = _stack.pop();

	ULONG offset = READ_LE_UINT16(_code); _code += 2;
	byte *instP = (byte *)_thunk + _externOffset + offset;
	ULONG instanceOffset = READ_LE_UINT16(instP);

	ULONG v = _stack.pop();
	instanceOffset += v >> 16;
	int objIndex = v & 0xffff;

	byte *destP = (byte *)Resources::addr(_objList[objIndex]) + instanceOffset;
	*destP = val & 0xff;
	_stack.push(val);
}

void Interpreter::cmdLEXA() {
	ULONG offset = READ_LE_UINT16(_code); _code += 2;
	byte *instP = (byte *)_thunk + _externOffset + offset;
	ULONG instanceOffset = READ_LE_UINT16(instP);

	ULONG v = _stack.pop();
	instanceOffset += v >> 16;
	int objIndex = v & 0xffff;

	byte *ptr = (byte *)Resources::addr(_objList[objIndex]) + instanceOffset;
	_stack.push((BYTE *)ptr);
}

void Interpreter::cmdSXAS() {
	ULONG index = _stack.pop();
	_stack.top() |= index << 16;
}

void Interpreter::cmdLECA() {
	ULONG val = _stack.pop();
	_stack.push((BYTE *)_ds32 + val);
}

void Interpreter::cmdSOLE() {
	HRES hres = _objList[_stack.pop()];
	_stack.push((BYTE *)hres);
}

void Interpreter::cmdEND() {
	_breakFlag = true;
}

void Interpreter::cmdBRK() {
	error("BRK encountered");
}

Parameter Interpreter::loadString(Parameters params) {
	load_string(params.size(), params[0], params[1]);
	return Parameter();
}

Parameter Interpreter::loadResource(Parameters params) {
	load_resource(params.size(), params[0], params[1]);
	return Parameter();
}

Parameter Interpreter::copyString(Parameters params) {
	copy_string(params.size(), params[0], params[1]);
	return Parameter();
}

Parameter Interpreter::stringForceLower(Parameters params) {
	string_force_lower(params.size(), params[0]);
	return Parameter();
}

Parameter Interpreter::stringForceUpper(Parameters params) {
	string_force_upper(params.size(), params[0]);
	return Parameter();
}

Parameter Interpreter::stringLen(Parameters params) {
	return string_len(params.size(), params[0]);
}

Parameter Interpreter::stringCompare(Parameters params) {
	return string_compare(params.size(), params[0], params[1]);
}

Parameter Interpreter::strVal(Parameters params) {
	return strval(params.size(), params[0]);
}

Parameter Interpreter::envVal(Parameters params) {
	return envval(params.size(), params[0]);
}

Parameter Interpreter::beep(Parameters params) {
	::Aesop::beep();
	return Parameter();
}

Parameter Interpreter::pokeMem(Parameters params) {
	pokemem(params.size(), params[0], params[1]);
	return Parameter();
}

Parameter Interpreter::peekMem(Parameters params) {
	return peekmem(params.size(), params[0]);
}

Parameter Interpreter::rnd(Parameters params) {
	return ::Aesop::rnd(params.size(), params[0], params[1]);
}

Parameter Interpreter::dice(Parameters params) {
	return ::Aesop::dice(params.size(), params[0], params[1], params[2]);
}

Parameter Interpreter::absv(Parameters params) {
	return ::Aesop::absv(params.size(), params[0]);
}

Parameter Interpreter::minv(Parameters params) {
	return ::Aesop::minv(params.size(), params[0], params[1]);
}

Parameter Interpreter::maxv(Parameters params) {
	return ::Aesop::maxv(params.size(), params[0], params[1]);
}

Parameter Interpreter::diagnose(Parameters params) {
	::Aesop::diagnose(params.size(), params[0], params[1]);
	return Parameter();
}

Parameter Interpreter::heapfree(Parameters params) {
	return ::Aesop::heapfree();
}

Parameter Interpreter::notify(Parameters params) {
	::Aesop::notify(params.size(), params[0], params[1], params[2], params[3]);
	return Parameter();
}

Parameter Interpreter::cancel(Parameters params) {
	::Aesop::cancel(params.size(), params[0], params[1], params[2], params[3]);
	return Parameter();
}

Parameter Interpreter::drainEventQueue(Parameters params) {
	drain_event_queue();
	return Parameter();
}

Parameter Interpreter::postEvent(Parameters params) {
	post_event(params.size(), params[0], params[1], params[2]);
	return Parameter();
}

Parameter Interpreter::sendEvent(Parameters params) {
	send_event(params.size(), params[0], params[1], params[2]);
	return Parameter();
}

Parameter Interpreter::peekEvent(Parameters params) {
	return peek_event();
}

Parameter Interpreter::dispatchEvent(Parameters params) {
	dispatch_event();
	return Parameter();
}

Parameter Interpreter::flushEventQueue(Parameters params) {
	flush_event_queue(params.size(), params[0], params[1], params[2]);
	return Parameter();
}

Parameter Interpreter::flushInputEvents(Parameters params) {
	flush_input_events();
	return Parameter();
}

Parameter Interpreter::initInterface(Parameters params) {
	init_interface();
	return Parameter();
}

Parameter Interpreter::shutdownInterface(Parameters params) {
	shutdown_interface();
	return Parameter();
}

Parameter Interpreter::setMousePointer(Parameters params) {
	set_mouse_pointer(params.size(), params[0], params[1], params[2],
		params[3], params[4], params[5], params[6]);
	return Parameter();
}

Parameter Interpreter::setWaitPointer(Parameters params) {
	set_wait_pointer(params.size(), params[0], params[1], params[2]);
	return Parameter();
}

Parameter Interpreter::standbyCursor(Parameters params) {
	standby_cursor();
	return Parameter();
}

Parameter Interpreter::resumeCursor(Parameters params) {
	resume_cursor();
	return Parameter();
}

Parameter Interpreter::showMouse(Parameters params) {
	show_mouse();
	return Parameter();
}

Parameter Interpreter::hideMouse(Parameters params) {
	hide_mouse();
	return Parameter();
}

Parameter Interpreter::mouseXY(Parameters params) {
	return mouse_XY();
}

Parameter Interpreter::mouseInWindow(Parameters params) {
	return mouse_in_window(params.size(), params[0]);
}

Parameter Interpreter::lockMouse(Parameters params) {
	lock_mouse();
	return Parameter();
}

Parameter Interpreter::unlockMouse(Parameters params) {
	unlock_mouse();
	return Parameter();
}

Parameter Interpreter::getKey(Parameters params) {
	::Aesop::getkey();
	return Parameter();
}

Parameter Interpreter::initGraphics(Parameters params) {
	init_graphics();
	return Parameter();
}

Parameter Interpreter::drawDot(Parameters params) {
	draw_dot(params.size(), params[0], params[1], params[2], params[3]);
	return Parameter();
}

Parameter Interpreter::drawLine(Parameters params) {
	draw_line(params.size(), params[0], params[1], params[2], params[3],
		params[4], params[5]);
	return Parameter();
}

Parameter Interpreter::lineTo(Parameters params) {
	line_to(params.size(), params[0], params[1], params[2]);
	return Parameter();
}

Parameter Interpreter::drawRectangle(Parameters params) {
	draw_rectangle(params.size(), params[0], params[1], params[2],
		params[3], params[4], params[5]);
	return Parameter();
}

Parameter Interpreter::fillRectangle(Parameters params) {
	fill_rectangle(params.size(), params[0], params[1], params[2],
		params[3], params[4], params[5]);
	return Parameter();
}

Parameter Interpreter::hashRectangle(Parameters params) {
	hash_rectangle(params.size(), params[0], params[1], params[2],
		params[3], params[4], params[5]);
	return Parameter();
}

Parameter Interpreter::getBitmapHeight(Parameters params) {
	return get_bitmap_height(params.size(), params[0], params[1]);
}

Parameter Interpreter::drawBitmap(Parameters params) {
	draw_bitmap(params.size(), params[0], params[1], params[2],
		params[3], params[4], params[5], params[6],
		params[7], params[8]);
	return Parameter();
}

Parameter Interpreter::visibleBitmapRect(Parameters params) {
	assert(params[5]);
	return visible_bitmap_rect(params.size(), params[0], params[1],
		params[2], params[3], params[4], params[5]);
}

Parameter Interpreter::setPalette(Parameters params) {
	set_palette(params.size(), params[0], params[1]);
	return Parameter();
}

Parameter Interpreter::refreshWindow(Parameters params) {
	refresh_window(params.size(), params[0], params[1]);
	return Parameter();
}

Parameter Interpreter::wipeWindow(Parameters params) {
	wipe_window(params.size(), params[0], params[1]);
	return Parameter();
}

Parameter Interpreter::shutdownGraphics(Parameters params) {
	shutdown_graphics();
	return Parameter();
}

Parameter Interpreter::waitVerticalRetrace(Parameters params) {
	wait_vertical_retrace();
	return Parameter();
}

Parameter Interpreter::readPalette(Parameters params) {
	return read_palette(params.size(), params[0]);
}

Parameter Interpreter::writePalette(Parameters params) {
	write_palette(params.size(), params[0], params[1]);
	return Parameter();
}

Parameter Interpreter::pixelFade(Parameters params) {
	pixel_fade(params.size(), params[0], params[1], params[2]);
	return Parameter();
}

Parameter Interpreter::colorFade(Parameters params) {
	color_fade(params.size(), params[0], params[1]);
	return Parameter();
}

Parameter Interpreter::lightFade(Parameters params) {
	light_fade(params.size(), params[0], params[1]);
	return Parameter();
}

Parameter Interpreter::assignWindow(Parameters params) {
	assign_window(params.size(), params[0], params[1], params[2], params[3], params[4]);
	return Parameter();
}

Parameter Interpreter::assignSubWindow(Parameters params) {
	return assign_subwindow(params.size(), params[0], params[1], params[2], params[3],
		params[4], params[5]);
}

Parameter Interpreter::releaseWindow(Parameters params) {
	release_window(params.size(), params[0]);
	return Parameter();
}

Parameter Interpreter::getLeft(Parameters params) {
	return get_x1(params.size(), params[0]);
}

Parameter Interpreter::getRight(Parameters params) {
	return get_x2(params.size(), params[0]);
}

Parameter Interpreter::getTop(Parameters params) {
	return get_y1(params.size(), params[0]);
}

Parameter Interpreter::getBottom(Parameters params) {
	return get_y2(params.size(), params[0]);
}

Parameter Interpreter::setLeft(Parameters params) {
	set_x1(params.size(), params[0], params[1]);
	return Parameter();
}

Parameter Interpreter::setRight(Parameters params) {
	set_x2(params.size(), params[0], params[1]);
	return Parameter();
}

Parameter Interpreter::setTop(Parameters params) {
	set_y1(params.size(), params[0], params[1]);
	return Parameter();
}

Parameter Interpreter::setBottom(Parameters params) {
	set_y2(params.size(), params[0], params[1]);
	return Parameter();
}

Parameter Interpreter::textWindow(Parameters params) {
	text_window(params.size(), params[0], params[1]);
	return Parameter();
}

Parameter Interpreter::textStyle(Parameters params) {
	text_style(params.size(), params[0], params[1], params[2]);
	return Parameter();
}

Parameter Interpreter::textXy(Parameters params) {
	text_xy(params.size(), params[0], params[1], params[2]);
	return Parameter();
}

Parameter Interpreter::textColor(Parameters params) {
	text_color(params.size(), params[0], params[1], params[2]);
	return Parameter();
}

Parameter Interpreter::textRefreshWindow(Parameters params) {
	text_refresh_window(params.size(), params[0], params[1]);
	return Parameter();
}

Parameter Interpreter::getTextX(Parameters params) {
	return get_text_x(params.size(), params[0]);
}

Parameter Interpreter::getTextY(Parameters params) {
	return get_text_y(params.size(), params[0]);
}

Parameter Interpreter::home(Parameters params) {
	::Aesop::home(params.size(), params[0]);
	return Parameter();
}

Parameter Interpreter::print(Parameters params) {
	error("TODO: version of print() method accepting parameter array");
	return Parameter();
}

Parameter Interpreter::sPrint(Parameters params) {
	error("TODO: version of sprint() method accepting parameter array");
	return Parameter();
}

Parameter Interpreter::dPrint(Parameters params) {
	error("TODO: version of dprint() method accepting parameter array");
	return Parameter();
}

Parameter Interpreter::aPrint(Parameters params) {
	error("TODO: version of aprint() method accepting parameter array");
	return Parameter();
}

Parameter Interpreter::crOut(Parameters params) {
	::Aesop::crout(params.size(), params[0]);
	return Parameter();
}

Parameter Interpreter::charWidth(Parameters params) {
	return char_width(params.size(), params[0], params[1]);
}

Parameter Interpreter::fontHeight(Parameters params) {
	return font_height(params.size(), params[0]);
}

Parameter Interpreter::solidBarGraph(Parameters params) {
	solid_bar_graph(params.size(), params[0], params[1], params[2],
		params[3], params[4], params[5], params[6], params[7],
		params[8], params[9], params[10], params[11],
		params[12], params[13]);
	return Parameter();
}

Parameter Interpreter::initSound(Parameters params) {
	init_sound(params.size(), params[0]);
	return Parameter();
}

Parameter Interpreter::shutdownSound(Parameters params) {
	shutdown_sound();
	return Parameter();
}

Parameter Interpreter::loadSoundBlock(Parameters params) {
	assert(params[2]);
	load_sound_block(params.size(), params[0], params[1], params[2]);
	return Parameter();
}

Parameter Interpreter::soundEffect(Parameters params) {
	sound_effect(params.size(), params[0]);
	return Parameter();
}

Parameter Interpreter::playSequence(Parameters params) {
	play_sequence(params.size(), params[0], params[1], params[2]);
	return Parameter();
}

Parameter Interpreter::loadMusic(Parameters params) {
	load_music();
	return Parameter();
}

Parameter Interpreter::unloadMusic(Parameters params) {
	unload_music();
	return Parameter();
}

Parameter Interpreter::setSoundStatus(Parameters params) {
	set_sound_status(params.size(), params[0]);
	return Parameter();
}

Parameter Interpreter::createObject(Parameters params) {
	return create_object(params.size(), params[0]);
}

Parameter Interpreter::createProgram(Parameters params) {
	return create_program(params.size(), params[0], params[1]);
}

Parameter Interpreter::destroyObject(Parameters params) {
	return destroy_object(params.size(), params[0]);
}

Parameter Interpreter::flushCache(Parameters params) {
	return flush_cache(params.size(), params[0]);
}

Parameter Interpreter::thrashCache(Parameters params) {
	thrash_cache();
	return Parameter();
}

Parameter Interpreter::stepX(Parameters params) {
	return step_X(params.size(), params[0], params[1], params[2], params[3]);
}

Parameter Interpreter::stepY(Parameters params) {
	return step_Y(params.size(), params[0], params[1], params[2], params[3]);
}

Parameter Interpreter::stepFDIR(Parameters params) {
	return step_FDIR(params.size(), params[0], params[1]);
}

Parameter Interpreter::stepSquareX(Parameters params) {
	return step_square_X(params.size(), params[0], params[1], params[2]);
}

Parameter Interpreter::stepSquareY(Parameters params) {
	return step_square_Y(params.size(), params[0], params[1], params[2]);
}

Parameter Interpreter::stepRegion(Parameters params) {
	return step_region(params.size(), params[0], params[1]);
}

Parameter Interpreter::distance(Parameters params) {
	return ::Aesop::distance(params.size(), params[0], params[1], params[2], params[3]);
}

Parameter Interpreter::seekDirection(Parameters params) {
	return seek_direction(params.size(), params[0], params[1], params[2], params[3]);
}

Parameter Interpreter::spellRequest(Parameters params) {
	assert(params[0] && params[1]);
	return spell_request(params.size(), params[0], params[1], params[2], params[3]);
}

Parameter Interpreter::spellList(Parameters params) {
	assert(params[0] && params[3]);
	return spell_list(params.size(), params[0], params[1], params[2], params[3], params[4]);
}

Parameter Interpreter::magicField(Parameters params) {
	magic_field(params.size(), params[0], params[1], params[2], params[3]);
	return Parameter();
}

Parameter Interpreter::doDots(Parameters params) {
	assert(params[9]);
	do_dots(params.size(), params[0], params[1], params[2], params[3],
		params[4], params[5], params[6], params[7],
		params[8], params[9]);
	return Parameter();
}

Parameter Interpreter::doIce(Parameters params) {
	assert(params[6]);
	do_ice(params.size(), params[0], params[1], params[2], params[3],
		params[4], params[5], params[6]);
	return Parameter();
}

Parameter Interpreter::readSaveDirectory(Parameters params) {
	read_save_directory();
	return Parameter();
}

Parameter Interpreter::savegameTitle(Parameters params) {
	error("savegame_title returns char *");
	return Parameter();
}

Parameter Interpreter::writeSaveDirectory(Parameters params) {
	write_save_directory();
	return Parameter();
}

Parameter Interpreter::saveGame(Parameters params) {
	return save_game(params.size(), params[0], params[1]);
}

Parameter Interpreter::suspendGame(Parameters params) {
	suspend_game(params.size(), params[0]);
	return Parameter();
}

Parameter Interpreter::resumeItems(Parameters params) {
	resume_items(params.size(), params[0], params[1], params[2]);
	return Parameter();
}

Parameter Interpreter::resumeLevel(Parameters params) {
	resume_level(params.size(), params[0]);
	return Parameter();
}

Parameter Interpreter::changeLevel(Parameters params) {
	change_level(params.size(), params[0], params[1]);
	return Parameter();
}

Parameter Interpreter::restoreItems(Parameters params) {
	restore_items(params.size(), params[0]);
	return Parameter();
}

Parameter Interpreter::restoreLevelObjects(Parameters params) {
	restore_level_objects(params.size(), params[0], params[1]);
	return Parameter();
}

Parameter Interpreter::readInitialItems(Parameters params) {
	read_initial_items();
	return Parameter();
}

Parameter Interpreter::writeInitialTempfiles(Parameters params) {
	write_initial_tempfiles();
	return Parameter();
}

Parameter Interpreter::createInitialBinaryFiles(Parameters params) {
	create_initial_binary_files();
	return Parameter();
}

Parameter Interpreter::launch(Parameters params) {
	::Aesop::launch(params.size(), params[0], params[1], params[2], params[3]);
	return Parameter();
}

Parameter Interpreter::openTransferFile(Parameters params) {
	error("open_transfer_file");
	return Parameter();
}

Parameter Interpreter::closeTransferFile(Parameters params) {
	close_transfer_file();
	return Parameter();
}

Parameter Interpreter::playerAttrib(Parameters params) {
	return player_attrib(params.size(), params[0], params[1], params[2]);
}

Parameter Interpreter::itemAttrib(Parameters params) {
	return item_attrib(params.size(), params[0], params[1], params[2]);
}

Parameter Interpreter::arrowCount(Parameters params) {
	return arrow_count(params.size(), params[0]);
}

} // End of namespace Aesop
