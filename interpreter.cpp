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
	&Interpreter::wipe_window,
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
	&Interpreter::restoreLevel_objects,
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
		params.push_back(_stack.popEntry());

	// Get the method number
	uint32 methodNum = _stack.pop();
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
	byte *srcP = _ds32 + offset;
	_stack.pushPtr((BYTE *)srcP);
}

void Interpreter::cmdLAB() {
	uint32 offset = READ_LE_UINT16(_code); _code += 2;
	assert((offset % 4) == 0);

	uint32 v = _stack[_stackBase + offset / 4] & 0xff;
	_stack.top() = v;
}

void Interpreter::cmdLAW() {
	uint32 offset = READ_LE_UINT16(_code); _code += 2;
	assert((offset % 4) == 0);

	uint32 v = _stack[_stackBase + offset / 4] & 0xffff;
	_stack.top() = v;
}

void Interpreter::cmdLAD() {
	uint32 offset = READ_LE_UINT16(_code); _code += 2;
	assert((offset % 4) == 0);

	uint32 v = _stack[_stackBase + offset / 4];
	_stack.top() = v;
}

void Interpreter::cmdSAB() {
	uint32 v = READ_LE_UINT16(_code) / 4; _code += 2;
	uint32 offset = _stack[_stackBase + v] & 0xff;
	_stack[offset] = _stack.top();
}

void Interpreter::cmdSAW() {
	uint32 v = READ_LE_UINT16(_code) / 4; _code += 2;
	uint32 offset = _stack[_stackBase + v] & 0xffff;
	_stack[offset] = _stack.top();
}

void Interpreter::cmdSAD() {
	uint32 v = READ_LE_UINT16(_code) / 4; _code += 2;
	uint32 offset = _stack[_stackBase + v];
	_stack[offset] = _stack.top();
}

void Interpreter::cmdLABA() {
	int offset = READ_LE_UINT16(_code); _code += 2;
	int subIndex = _stack.pop();
	assert((offset % 4) == 0 && (subIndex % 4) == 0);

	uint32 v = _stack[_stackBase + (offset - subIndex) / 4] & 0xff;
	_stack.push(v);
}

void Interpreter::cmdLAWA() {
	int offset = READ_LE_UINT16(_code); _code += 2;
	int subIndex = _stack.pop();
	assert((offset % 4) == 0 && (subIndex % 4) == 0);

	uint32 v = _stack[_stackBase + (offset - subIndex) / 4] & 0xffff;
	_stack.push(v);
}

void Interpreter::cmdLADA() {
	int offset = READ_LE_UINT16(_code); _code += 2;
	int subIndex = _stack.pop();
	assert((offset % 4) == 0 && (subIndex % 4) == 0);

	uint32 v = _stack[_stackBase + (offset - subIndex) / 4];
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

	byte *ptr = (byte *)&_stack[_stackBase + (offset / 4)];
	_stack.pop();
	_stack.pushPtr((BYTE *)ptr);
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
	uint32 offset = READ_LE_UINT16(_code); _code += 2;
	byte *srcP = (byte *)_thunk + _staticOffset + offset + _stack.pop();

	_stack.push(*srcP);
}

void Interpreter::cmdLSWA() {
	uint32 offset = READ_LE_UINT16(_code); _code += 2;
	byte *srcP = (byte *)_thunk + _staticOffset + offset + _stack.pop();

	_stack.push(READ_LE_UINT16(srcP));
}

void Interpreter::cmdLSDA() {
	uint32 offset = READ_LE_UINT16(_code); _code += 2;
	byte *srcP = (byte *)_thunk + _staticOffset + offset + _stack.pop();

	_stack.push(READ_LE_UINT32(srcP));
}

void Interpreter::cmdSSBA() {
	uint32 val = _stack.pop();
	uint32 offset = READ_LE_UINT16(_code); _code += 2;
	byte *destP = (byte *)_thunk + _staticOffset + offset + _stack.pop();

	*destP = val;
}

void Interpreter::cmdSSWA() {
	uint32 val = _stack.pop();
	uint32 offset = READ_LE_UINT16(_code); _code += 2;
	byte *destP = (byte *)_thunk + _staticOffset + offset + _stack.pop();

	WRITE_LE_UINT16(destP, val);
}

void Interpreter::cmdSSDA() {
	uint32 val = _stack.pop();
	uint32 offset = READ_LE_UINT16(_code); _code += 2;
	byte *destP = (byte *)_thunk + _staticOffset + offset + _stack.pop();

	WRITE_LE_UINT32(destP, val);
}

void Interpreter::cmdLESA() {
	uint32 offset = READ_LE_UINT16(_code); _code += 2;
	BYTE *srcP = (BYTE *)_thunk + _staticOffset + offset;

	_stack.topPtr() = srcP;
}

void Interpreter::cmdLXB() {
	uint32 offset = READ_LE_UINT16(_code); _code += 2;
	byte *instP = (byte *)_thunk + _externOffset + offset;
	uint32 instanceOffset = READ_LE_UINT16(instP);

	int objIndex = _stack.pop();
	byte *srcP = (byte *)Resources::addr(_objList[objIndex]) + instanceOffset;

	_stack.push(*srcP);
}

void Interpreter::cmdLXW() {
	uint32 offset = READ_LE_UINT16(_code); _code += 2;
	byte *instP = (byte *)_thunk + _externOffset + offset;
	uint32 instanceOffset = READ_LE_UINT16(instP);

	int objIndex = _stack.pop();
	byte *srcP = (byte *)Resources::addr(_objList[objIndex]) + instanceOffset;

	_stack.push(READ_LE_UINT16(srcP));
}

void Interpreter::cmdLXD() {
	uint32 offset = READ_LE_UINT16(_code); _code += 2;
	byte *instP = (byte *)_thunk + _externOffset + offset;
	uint32 instanceOffset = READ_LE_UINT16(instP);

	int objIndex = _stack.pop();
	byte *srcP = (byte *)Resources::addr(_objList[objIndex]) + instanceOffset;

	_stack.push(READ_LE_UINT32(srcP));
}

void Interpreter::cmdSXB() {
	uint32 val = _stack.pop() & 0xff;

	uint32 offset = READ_LE_UINT16(_code); _code += 2;
	byte *instP = (byte *)_thunk + _externOffset + offset;
	uint32 instanceOffset = READ_LE_UINT16(instP);

	int objIndex = _stack.pop();
	byte *destP = (byte *)Resources::addr(_objList[objIndex]) + instanceOffset;

	*destP = val;
	_stack.push(val);
}

void Interpreter::cmdSXW() {
	uint32 val = _stack.pop();

	uint32 offset = READ_LE_UINT16(_code); _code += 2;
	byte *instP = (byte *)_thunk + _externOffset + offset;
	uint32 instanceOffset = READ_LE_UINT16(instP);

	int objIndex = _stack.pop();
	byte *destP = (byte *)Resources::addr(_objList[objIndex]) + instanceOffset;

	WRITE_LE_UINT16(destP, val);
	_stack.push(val);
}

void Interpreter::cmdSXD() {
	uint32 val = _stack.pop();

	uint32 offset = READ_LE_UINT16(_code); _code += 2;
	byte *instP = (byte *)_thunk + _externOffset + offset;
	uint32 instanceOffset = READ_LE_UINT16(instP);

	int objIndex = _stack.pop();
	byte *destP = (byte *)Resources::addr(_objList[objIndex]) + instanceOffset;

	WRITE_LE_UINT32(destP, val);
	_stack.push(val);
}

void Interpreter::cmdLXBA() {
	uint32 offset = READ_LE_UINT16(_code); _code += 2;
	byte *instP = (byte *)_thunk + _externOffset + offset;
	uint32 instanceOffset = READ_LE_UINT16(instP);

	uint32 v = _stack.pop();
	instanceOffset += v >> 16;
	int objIndex = v & 0xffff;

	byte *srcP = (byte *)Resources::addr(_objList[objIndex]) + instanceOffset;
	_stack.push(*srcP);
}

void Interpreter::cmdLXWA() {
	uint32 offset = READ_LE_UINT16(_code); _code += 2;
	byte *instP = (byte *)_thunk + _externOffset + offset;
	uint32 instanceOffset = READ_LE_UINT16(instP);

	uint32 v = _stack.pop();
	instanceOffset += v >> 16;
	int objIndex = v & 0xffff;

	byte *srcP = (byte *)Resources::addr(_objList[objIndex]) + instanceOffset;
	_stack.push(READ_LE_UINT16(srcP));
}

void Interpreter::cmdLXDA() {
	uint32 offset = READ_LE_UINT16(_code); _code += 2;
	byte *instP = (byte *)_thunk + _externOffset + offset;
	uint32 instanceOffset = READ_LE_UINT16(instP);

	uint32 v = _stack.pop();
	instanceOffset += v >> 16;
	int objIndex = v & 0xffff;

	byte *srcP = (byte *)Resources::addr(_objList[objIndex]) + instanceOffset;
	_stack.push(READ_LE_UINT32(srcP));
}

void Interpreter::cmdSXBA() {
	uint32 val = _stack.pop();

	uint32 offset = READ_LE_UINT16(_code); _code += 2;
	byte *instP = (byte *)_thunk + _externOffset + offset;
	uint32 instanceOffset = READ_LE_UINT16(instP);

	uint32 v = _stack.pop();
	instanceOffset += v >> 16;
	int objIndex = v & 0xffff;

	byte *destP = (byte *)Resources::addr(_objList[objIndex]) + instanceOffset;
	*destP = val & 0xff;
	_stack.push(val);
}

void Interpreter::cmdSXWA() {
	uint32 val = _stack.pop();

	uint32 offset = READ_LE_UINT16(_code); _code += 2;
	byte *instP = (byte *)_thunk + _externOffset + offset;
	uint32 instanceOffset = READ_LE_UINT16(instP);

	uint32 v = _stack.pop();
	instanceOffset += v >> 16;
	int objIndex = v & 0xffff;

	byte *destP = (byte *)Resources::addr(_objList[objIndex]) + instanceOffset;
	*destP = val & 0xffff;
	_stack.push(val);
}

void Interpreter::cmdSXDA() {
	uint32 val = _stack.pop();

	uint32 offset = READ_LE_UINT16(_code); _code += 2;
	byte *instP = (byte *)_thunk + _externOffset + offset;
	uint32 instanceOffset = READ_LE_UINT16(instP);

	uint32 v = _stack.pop();
	instanceOffset += v >> 16;
	int objIndex = v & 0xffff;

	byte *destP = (byte *)Resources::addr(_objList[objIndex]) + instanceOffset;
	*destP = val & 0xff;
	_stack.push(val);
}

void Interpreter::cmdLEXA() {
	uint32 offset = READ_LE_UINT16(_code); _code += 2;
	byte *instP = (byte *)_thunk + _externOffset + offset;
	uint32 instanceOffset = READ_LE_UINT16(instP);

	uint32 v = _stack.pop();
	instanceOffset += v >> 16;
	int objIndex = v & 0xffff;

	byte *ptr = (byte *)Resources::addr(_objList[objIndex]) + instanceOffset;
	_stack.pushPtr((BYTE *)ptr);
}

void Interpreter::cmdSXAS() {
	uint32 index = _stack.pop();
	_stack.top() |= index << 16;
}

void Interpreter::cmdLECA() {
	uint32 val = _stack.pop();
	_stack.pushPtr((BYTE *)_ds32 + val);
}

void Interpreter::cmdSOLE() {
	HRES hres = _objList[_stack.pop()];
	_stack.pushPtr((BYTE *)hres);
}

void Interpreter::cmdEND() {
	_breakFlag = true;
}

void Interpreter::cmdBRK() {
	error("BRK encountered");
}

int Interpreter::loadString(Parameters params) {
	load_string(params.size(), params[0]._ptr, params[1]._val);
	return 0;
}

int Interpreter::loadResource(Parameters params) {
	load_resource(params.size(), params[0]._ptr, params[1]._val);
	return 0;
}

int Interpreter::copyString(Parameters params) {
	copy_string(params.size(), params[0]._ptr, params[1]._ptr);
	return 0;
}

int Interpreter::stringForceLower(Parameters params) {
	string_force_lower(params.size(), params[0]._ptr);
	return 0;
}

int Interpreter::stringForceUpper(Parameters params) {
	string_force_upper(params.size(), params[0]._ptr);
	return 0;
}

int Interpreter::stringLen(Parameters params) {
	return string_len(params.size(), params[0]._ptr);
}

int Interpreter::stringCompare(Parameters params) {
	return string_compare(params.size(), params[0]._ptr, params[1]._ptr);
}

int Interpreter::strVal(Parameters params) {
	return strval(params.size(), params[0]._ptr);
}

int Interpreter::envVal(Parameters params) {
	return envval(params.size(), params[0]._ptr);
}

int Interpreter::beep(Parameters params) {
	::Aesop::beep();
	return 0;
}

int Interpreter::pokeMem(Parameters params) {
	pokemem(params.size(), (LONG *)params[0]._ptr, params[1]._val);
	return 0;
}

int Interpreter::peekMem(Parameters params) {
	return peekmem(params.size(), (LONG *)params[0]._ptr);
}

int Interpreter::rnd(Parameters params) {
	return ::Aesop::rnd(params.size(), params[0]._val, params[1]._val);
}

int Interpreter::dice(Parameters params) {
	return 0;
}

int Interpreter::absv(Parameters params) {
	return 0;
}

int Interpreter::minv(Parameters params) {
	return 0;
}

int Interpreter::maxv(Parameters params) {
	return 0;
}

int Interpreter::diagnose(Parameters params) {
	return 0;
}

int Interpreter::heapfree(Parameters params) {
	return 0;
}

int Interpreter::notify(Parameters params) {
	return 0;
}

int Interpreter::cancel(Parameters params) {
	return 0;
}

int Interpreter::drainEventQueue(Parameters params) {
	return 0;
}

int Interpreter::postEvent(Parameters params) {
	return 0;
}

int Interpreter::sendEvent(Parameters params) {
	return 0;
}

int Interpreter::peekEvent(Parameters params) {
	return 0;
}

int Interpreter::dispatchEvent(Parameters params) {
	return 0;
}

int Interpreter::flushEventQueue(Parameters params) {
	return 0;
}

int Interpreter::flushInputEvents(Parameters params) {
	return 0;
}

int Interpreter::initInterface(Parameters params) {
	return 0;
}

int Interpreter::shutdownInterface(Parameters params) {
	return 0;
}

int Interpreter::setMousePointer(Parameters params) {
	return 0;
}

int Interpreter::setWaitPointer(Parameters params) {
	return 0;
}

int Interpreter::standbyCursor(Parameters params) {
	return 0;
}

int Interpreter::resumeCursor(Parameters params) {
	return 0;
}

int Interpreter::showMouse(Parameters params) {
	return 0;
}

int Interpreter::hideMouse(Parameters params) {
	return 0;
}

int Interpreter::mouseXY(Parameters params) {
	return 0;
}

int Interpreter::mouseInWindow(Parameters params) {
	return 0;
}

int Interpreter::lockMouse(Parameters params) {
	return 0;
}

int Interpreter::unlockMouse(Parameters params) {
	return 0;
}

int Interpreter::getKey(Parameters params) {
	return 0;
}

int Interpreter::initGraphics(Parameters params) {
	return 0;
}

int Interpreter::drawDot(Parameters params) {
	return 0;
}

int Interpreter::drawLine(Parameters params) {
	return 0;
}

int Interpreter::lineTo(Parameters params) {
	return 0;
}

int Interpreter::drawRectangle(Parameters params) {
	return 0;
}

int Interpreter::fillRectangle(Parameters params) {
	return 0;
}

int Interpreter::hashRectangle(Parameters params) {
	return 0;
}

int Interpreter::getBitmapHeight(Parameters params) {
	return 0;
}

int Interpreter::drawBitmap(Parameters params) {
	return 0;
}

int Interpreter::visibleBitmapRect(Parameters params) {
	return 0;
}

int Interpreter::setPalette(Parameters params) {
	return 0;
}

int Interpreter::refreshWindow(Parameters params) {
	return 0;
}

int Interpreter::wipe_window(Parameters params) {
	return 0;
}

int Interpreter::shutdownGraphics(Parameters params) {
	return 0;
}

int Interpreter::waitVerticalRetrace(Parameters params) {
	return 0;
}

int Interpreter::readPalette(Parameters params) {
	return 0;
}

int Interpreter::writePalette(Parameters params) {
	return 0;
}

int Interpreter::pixelFade(Parameters params) {
	return 0;
}

int Interpreter::colorFade(Parameters params) {
	return 0;
}

int Interpreter::lightFade(Parameters params) {
	return 0;
}

int Interpreter::assignWindow(Parameters params) {
	return 0;
}

int Interpreter::assignSubWindow(Parameters params) {
	return 0;
}

int Interpreter::releaseWindow(Parameters params) {
	return 0;
}

int Interpreter::getLeft(Parameters params) {
	return 0;
}

int Interpreter::getRight(Parameters params) {
	return 0;
}

int Interpreter::getTop(Parameters params) {
	return 0;
}

int Interpreter::getBottom(Parameters params) {
	return 0;
}

int Interpreter::setLeft(Parameters params) {
	return 0;
}

int Interpreter::setRight(Parameters params) {
	return 0;
}

int Interpreter::setTop(Parameters params) {
	return 0;
}

int Interpreter::setBottom(Parameters params) {
	return 0;
}

int Interpreter::textWindow(Parameters params) {
	return 0;
}

int Interpreter::textStyle(Parameters params) {
	return 0;
}

int Interpreter::textXy(Parameters params) {
	return 0;
}

int Interpreter::textColor(Parameters params) {
	return 0;
}

int Interpreter::textRefreshWindow(Parameters params) {
	return 0;
}

int Interpreter::getTextX(Parameters params) {
	return 0;
}

int Interpreter::getTextY(Parameters params) {
	return 0;
}

int Interpreter::home(Parameters params) {
	return 0;
}

int Interpreter::print(Parameters params) {
	return 0;
}

int Interpreter::sPrint(Parameters params) {
	return 0;
}

int Interpreter::dPrint(Parameters params) {
	return 0;
}

int Interpreter::aPrint(Parameters params) {
	return 0;
}

int Interpreter::crOut(Parameters params) {
	return 0;
}

int Interpreter::charWidth(Parameters params) {
	return 0;
}

int Interpreter::fontHeight(Parameters params) {
	return 0;
}

int Interpreter::solidBarGraph(Parameters params) {
	return 0;
}

int Interpreter::initSound(Parameters params) {
	return 0;
}

int Interpreter::shutdownSound(Parameters params) {
	return 0;
}

int Interpreter::loadSoundBlock(Parameters params) {
	return 0;
}

int Interpreter::soundEffect(Parameters params) {
	return 0;
}

int Interpreter::playSequence(Parameters params) {
	return 0;
}

int Interpreter::loadMusic(Parameters params) {
	return 0;
}

int Interpreter::unloadMusic(Parameters params) {
	return 0;
}

int Interpreter::setSoundStatus(Parameters params) {
	return 0;
}

int Interpreter::createObject(Parameters params) {
	return 0;
}

int Interpreter::createProgram(Parameters params) {
	return 0;
}

int Interpreter::destroyObject(Parameters params) {
	return 0;
}

int Interpreter::flushCache(Parameters params) {
	return 0;
}

int Interpreter::thrashCache(Parameters params) {
	return 0;
}

int Interpreter::stepX(Parameters params) {
	return 0;
}

int Interpreter::stepY(Parameters params) {
	return 0;
}

int Interpreter::stepFDIR(Parameters params) {
	return 0;
}

int Interpreter::stepSquareX(Parameters params) {
	return 0;
}

int Interpreter::stepSquareY(Parameters params) {
	return 0;
}

int Interpreter::stepRegion(Parameters params) {
	return 0;
}

int Interpreter::distance(Parameters params) {
	return 0;
}

int Interpreter::seekDirection(Parameters params) {
	return 0;
}

int Interpreter::spellRequest(Parameters params) {
	return 0;
}

int Interpreter::spellList(Parameters params) {
	return 0;
}

int Interpreter::magicField(Parameters params) {
	return 0;
}

int Interpreter::doDots(Parameters params) {
	return 0;
}

int Interpreter::doIce(Parameters params) {
	return 0;
}

int Interpreter::readSaveDirectory(Parameters params) {
	return 0;
}

int Interpreter::savegameTitle(Parameters params) {
	return 0;
}

int Interpreter::writeSaveDirectory(Parameters params) {
	return 0;
}

int Interpreter::saveGame(Parameters params) {
	return 0;
}

int Interpreter::suspendGame(Parameters params) {
	return 0;
}

int Interpreter::resumeItems(Parameters params) {
	return 0;
}

int Interpreter::resumeLevel(Parameters params) {
	return 0;
}

int Interpreter::changeLevel(Parameters params) {
	return 0;
}

int Interpreter::restoreItems(Parameters params) {
	return 0;
}

int Interpreter::restoreLevel_objects(Parameters params) {
	return 0;
}

int Interpreter::readInitialItems(Parameters params) {
	return 0;
}

int Interpreter::writeInitialTempfiles(Parameters params) {
	return 0;
}

int Interpreter::createInitialBinaryFiles(Parameters params) {
	return 0;
}

int Interpreter::launch(Parameters params) {
	return 0;
}

int Interpreter::openTransferFile(Parameters params) {
	return 0;
}

int Interpreter::closeTransferFile(Parameters params) {
	return 0;
}

int Interpreter::playerAttrib(Parameters params) {
	return 0;
}

int Interpreter::itemAttrib(Parameters params) {
	return 0;
}

int Interpreter::arrowCount(Parameters params) {
	return 0;
}

} // End of namespace Aesop
