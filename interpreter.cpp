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
#include "aesop/rtmsg.h"
#include "aesop/rtsystem.h"
#include "aesop/shapes.h"

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
    TYP_SVAR                   // data type: string variable
};

#define GAME_STARTUP_STATE 0x4f0

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

static const char *const OPCODE_NAMES[] = {
	"BRT", "BRF", "BRA", "CASE", "PUSH", "DUP", "NOT", "SETB", "NEG", "ADD", "SUB", "MUL",
	"DIV", "MOD", "EXP", "BAND", "BOR", "XOR", "BNOT", "SHL", "SHR", "LT", "LE", "EQ",
	"NE", "GE", "GT", "INC", "DEC", "SHTC", "INTC", "LNGC", "RCRS", "CALL", "SEND", "PASS",
	"JSR", "RTS", "AIM", "AIS", "LTBA", "LTWA", "LTDA", "LETA", "LAB", "LAW", "LAD", "SAB",
	"SAW", "SAD", "LABA", "LAWA", "LADA", "SABA", "SAWA", "SADA", "LEAA", "LSB", "LSW",
	"LSD", "SSB", "SSW", "SSD", "LSBA", "LSWA", "LSDA", "SSBA", "SSWA", "SSDA", "LESA",
	"LXB", "LXW", "LXD", "SXB", "SXW", "SXD", "LXBA", "LXWA", "LXDA", "SXBA", "SXWA",
	"SXDA", "LEXA", "SXAS", "LECA", "SOLE", "END", "BRK"
};

static const char *const METHOD_NAMES[] = {
	"loadString", "loadResource", "copyString", "stringForceLower", "stringForceUpper",
	"stringLen", "stringCompare", "strVal", "envVal", "beep", "pokeMem", "peekMem",
	"rnd", "dice", "absv", "minv", "maxv", "diagnose", "heapfree",

	"notify", "cancel", "drainEventQueue", "postEvent", "sendEvent", "peekEvent",
	"dispatchEvent", "flushEventQueue", "flushInputEvents",

	"initInterface", "shutdownInterface", "setMousePointer", "setWaitPointer",
	"standbyCursor", "resumeCursor", "showMouse", "hideMouse", "mouseXY",
	"mouseInWindow", "lockMouse", "unlockMouse", "getKey",

	"initGraphics", "drawDot", "drawLine", "lineTo", "drawRectangle", "fillRectangle",
	"hashRectangle", "getBitmapHeight", "drawBitmap", "visibleBitmapRect", "setPalette",
	"refreshWindow", "wipeWindow", "shutdownGraphics", "waitVerticalRetrace",
	"readPalette", "writePalette", "pixelFade", "colorFade", "lightFade",

	"assignWindow", "assignSubWindow", "releaseWindow", "getLeft", "getRight",
	"getTop", "getBottom", "setLeft", "setRight", "setTop", "setBottom",

	"textWindow", "textStyle", "textXy", "textColor", "textRefreshWindow", "getTextX",
	"getTextY", "home", "print", "sPrint", "dPrint", "aPrint", "crOut", "charWidth",
	"fontHeight", "solidBarGraph",

	"initSound", "shutdownSound", "loadSoundBlock", "soundEffect", "playSequence",
	"loadMusic", "unloadMusic", "setSoundStatus",

	"createObject", "createProgram", "destroyObject", "flushCache", "thrashCache",

	"stepX", "stepY", "stepFDIR",

	"stepSquareX", "stepSquareY", "stepRegion",

	"distance", "seekDirection",

	"spellRequest", "spellList", "magicField", "doDots", "doIce",

	"readSaveDirectory", "savegameTitle", "writeSaveDirectory", "saveGame", "suspendGame",
	"resumeItems", "resumeLevel", "changeLevel", "restoreItems", "restoreLevelObjects",
	"readInitialItems", "writeInitialTempfiles", "createInitialBinaryFiles", "launch",

	"openTransferFile", "closeTransferFile", "playerAttrib", "itemAttrib", "arrowCount"
};

StartupState Interpreter::_startupState;
ULONG Interpreter::_currentThis;

/*----------------------------------------------------------------*/

Interpreter *Interpreter::init(AesopEngine *vm) {
	Interpreter::_currentThis = 0;
	Interpreter::_startupState = SS_CINE;
	return new Interpreter(vm);
}

Interpreter::Interpreter(AesopEngine *vm) : _vm(vm) {
	_currentIndex = 0;
	_currentMsg = 0;
	_instance = nullptr;
	_thunk = nullptr;
	_ds32 = nullptr;
	_code = nullptr;
	_stackBase = 0;
	_hPrg = HRES_NULL;
	_staticOffset = 0;
	_externOffset = 0;
	_breakFlag = false;
	_currentVector = nullptr;
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

void Interpreter::addArgument(const Parameter &param) {
	_stack.push(param);
}

LONG Interpreter::execute(LONG index, LONG msgNum, HRES vector) {
	Objects &objects = *_vm->_objects;
	Resources &res = *_vm->_resources;
	const int OPCODES_COUNT = sizeof(_opcodes) / sizeof(OpcodeMethod);
	debugC(DEBUG_BASIC, kDebugScripts, "Interpreter::execute(%d, %d, %x)", index, msgNum, (ULONG)vector);

	// Check the passed index
	if (index == -1)
		return -1;
	_currentIndex = index;

	// Get a handle to the object
	HRES handle = objects[index];
	if (handle == HRES_NULL)
		return -1;

	_instance = (IHDR *)res.addr(handle);
	_thunk = (THDR *)res.addr(_instance->_thunk);

	if (vector == HRES_NULL || vector == (HRES)0xffff) {
		const MV_entry *mvList = (const MV_entry *)((const byte *)_thunk + _thunk->_mvList);
		WORD maxMsg = (WORD)_thunk->_maxMsg;

		if (maxMsg == -1)
			return -1;
		_currentMsg = msgNum;

		// Find the correct vector to use
		int idx;
		for (idx = 0; mvList[(maxMsg + idx) / 2].msg != msgNum;) {
			int offset = (maxMsg + idx) / 2;
			int msg = mvList[offset].msg;

			if (msg > msgNum) {
				// Too high
				maxMsg = offset - 1;
				if (idx > maxMsg)
					return -1;
			} else {
				// Too low
				idx = offset + 1;
				if (idx > maxMsg)
					return -1;
			}
		}

		_currentVector = &mvList[(maxMsg + idx) / 2];
		++_currentVector;

		do {
			--_currentVector;
		} while (_currentVector > mvList && (_currentVector - 1)->msg == msgNum);
	} else {
		_currentVector = (MV_entry *)vector;
	}

	const SD_entry *sdEntry = (const SD_entry *)((const byte *)_thunk + _currentVector->SD_offset);
	uint codeOffset = _currentVector->handler;
	_hPrg = sdEntry->SOP;
	_staticOffset = sdEntry->static_base;
	_externOffset = sdEntry->extern_base;

	// Set up the code pointer
	res.lock(_hPrg);
	_ds32 = (byte *)Resources::addr(_hPrg);
	_code = _ds32 + codeOffset;
	_offThis = _currentIndex;
	_breakFlag = false;

	_stackBase = _stack.size();
	_stack.push(index);
	int count = READ_LE_UINT16(_code);
	_code += 2;
	assert((count % 2) == 0);
	_stack.resize(_stack.size() + (count + 2) / 4);

	// Main opcode execution loop
	while (!_vm->shouldQuit() && !_breakFlag) {
		int opcode = *_code;
		if (opcode >= OPCODES_COUNT)
			error("Invalid opcode encountered");

		if (gDebugLevel >= DEBUG_INTERMEDIATE)
			printInstruction(opcode);

		++_code;
		(this->*_opcodes[opcode])();
	}

	res.unlock(_hPrg);
	return _stack.top();
}

void Interpreter::printInstruction(int opcode) {
	Common::String msg = Common::String::format("%.4x %s", _code - _ds32, OPCODE_NAMES[opcode]);
	if (opcode == 33)
		msg += Common::String::format("(%s)", METHOD_NAMES[_stack[_stack.size() - 1 - *(_code + 1)] & 0xff]);
	
	if (gDebugLevel == DEBUG_INTERMEDIATE) {
		msg += Common::String::format(" |%d|", _stack.size());
	} else {
		msg += " |";
		
		for (uint idx = 0; idx < _stack.size(); ++idx) {
			if (idx > 0)
				msg += ", ";

			if ((BYTE *)_stack[idx] != nullptr)
				msg += Common::String::format("%p", (BYTE *)_stack[idx]);
			else
				msg += Common::String::format("%x", (ULONG)_stack[idx]);
		}

		msg += "|";
	}

	debugC(DEBUG_BASIC, kDebugScripts, "%s", msg.c_str());
}

void Interpreter::getStackIndex(int &stackIndex, int &byteNum, int dataSize) {
	int index4 = (int16)READ_LE_UINT16(_code);
	_code += 2;

	if (index4 <= 0) {
		// Backwards into the stack for retrieving subroutine parameters
		assert((index4 % 4) == 0);
		stackIndex = _stackBase - 1 - ABS(index4) / 4;
		byteNum = 0;
	} else if (index4 == 2) {
		// Specifying the "this"/Index for the script
		stackIndex = _stackBase;
		byteNum = 0;
	} else {
		assert((index4 % 2) == 0);
		stackIndex = (index4 - 2) / 4;
		byteNum = 3 - (index4 - 3) % 4;

		assert((dataSize == 4 && byteNum == 0) ||
			(dataSize == 2 && (byteNum == 0 || byteNum == 2)) ||
			dataSize == 1);
	}
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
	int numCases = READ_LE_UINT16(_code);
	_code += 2;

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
	LONG v = _stack.pop();
	_stack.push(-v);
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
	uint16 v = READ_LE_UINT16(_code);
	_code += 2;
	_stack.top() = v;
}

void Interpreter::cmdLNGC() {
	uint32 v = READ_LE_UINT32(_code);
	_code += 4;
	_stack.top() = v;
}

void Interpreter::cmdRCRS() {
	// Get index into method offset list
	uint offset = READ_LE_UINT16(_code);
	_code += 2;
	uint32 methodNum = READ_LE_UINT32((const byte *)_thunk + _externOffset + offset);

	// Validate that it's a correctly formatted method number, then push it to the stack
	assert((methodNum >> 16) == 0x1234);
	_stack.top() = methodNum;
}

void Interpreter::cmdCALL() {
	_currentThis = _currentIndex;

	// Load the arguments in reverse so they'll be in the correct order
	Parameters params;
	int argCount = *_code++;
	for (int idx = 0; idx < argCount; ++idx)
		params.insert_at(0, _stack.pop());

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
	// Instantiate a new interpreter
	Interpreter *interp = new Interpreter(_vm);
	
	// Load the arguments in reverse so they'll be in the correct order
	Parameters params;
	int argCount = *_code++;
	for (int idx = 0; idx < argCount; ++idx)
		params.insert_at(0, _stack.pop());
	for (int idx = 0; idx < argCount; ++idx)
		interp->addArgument(params[idx]);

	// Get the execution parameters
	int msgNum = READ_LE_UINT16(_code);
	_code += 2;
	int index = _stack.pop();

	// Execute the new code
	int result = interp->execute(index, msgNum);
	delete interp;

	// Push the result onto the stack of the current interpreter
	_stack.push(result);
}

void Interpreter::cmdPASS() {
	// Get the number of arguments
	int argCount = *_code++;

	// Get the next higher-level handler
	const MV_entry *oldVector = _currentVector;
	const MV_entry *newVector = _currentVector + 1;

	if ((const byte *)newVector >= ((const byte *)_thunk + _thunk->_sdList))
		// Past the end of the list, so it's not a valid vector
		return;

	if (newVector->msg != _currentMsg)
		// No further parent class do dispatch to, so exit
		return;

	// Load the arguments in reverse so they'll be in the correct order
	Parameters params;
	for (int idx = 0; idx < argCount; ++idx)
		params.insert_at(0, _stack.pop());

	// Save the stack size
	uint stackSize = _stack.size();

	// Execute the parent vector
	int result = execute(_currentIndex, _currentMsg, (HRES)newVector);

	// Restore saved fields
	_currentVector = oldVector;

	// Reset the stack back to it's previous size, and push the result
	_stack.resize(stackSize);
	_stack.push(result);
}

void Interpreter::cmdJSR() {
	// Get the subroutine method offset, and save details for where to resume
	uint16 methodOffset = READ_LE_UINT16(_code);
	_code += 2;
	_methodStack.push(MethodStackEntry(_ds32, _code, _stack.size(), _stackBase));

	// Set code and stack start
	_code = _ds32 + methodOffset;
	_stackBase = _stack.size();
	_offThis = _currentIndex;

	// Initialize manifest THIS var
	_stack.push(_currentIndex);

	// Set up the method autos
	uint autosSize = READ_LE_UINT16(_code);
	_code += 2;
	assert((autosSize % 4) == 0);
	for (uint idx = 0; idx <= autosSize / 4; ++idx)
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
	ULONG v2 = READ_LE_UINT16(_code);
	_code += 2;
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
	uint32 offset = READ_LE_UINT16(_code);
	_code += 2;
	const byte *srcP = _ds32 + offset + _stack.pop();
	_stack.push(*srcP);
}

void Interpreter::cmdLTWA() {
	uint32 offset = READ_LE_UINT16(_code);
	_code += 2;
	const byte *srcP = _ds32 + offset + _stack.pop();
	_stack.push(READ_LE_UINT16(srcP));
}

void Interpreter::cmdLTDA() {
	uint32 offset = READ_LE_UINT16(_code);
	_code += 2;
	const byte *srcP = _ds32 + offset + _stack.pop();
	_stack.push(READ_LE_UINT32(srcP));
}

void Interpreter::cmdLETA() {
	uint32 offset = READ_LE_UINT16(_code);
	_code += 2;
	BYTE *srcP = (BYTE *)_ds32 + offset;
	_stack.top() = (BYTE *)srcP;
}

void Interpreter::cmdLAB() {
	int stackIndex, byteNum;
	getStackIndex(stackIndex, byteNum, 1);

	ULONG v = _stack[stackIndex] & 0xff;
	_stack.top() = v;
}

void Interpreter::cmdLAW() {
	int stackIndex, byteNum;
	getStackIndex(stackIndex, byteNum, 2);

	ULONG v = _stack[stackIndex];
	if (byteNum == 2)
		v >>= 16;
	else
		v &= 0xffff;

	_stack.top() = v;
}

void Interpreter::cmdLAD() {
	int stackIndex, byteNum;
	getStackIndex(stackIndex, byteNum, 4);

	ULONG v = _stack[stackIndex];
	_stack.top() = v;
}

void Interpreter::cmdSAB() {
	int stackIndex, byteNum;
	getStackIndex(stackIndex, byteNum, 1);

	_stack[stackIndex] = ((ULONG)_stack[stackIndex] & 0xffffff00) | (_stack.top() & 0xff);
}

void Interpreter::cmdSAW() {
	int stackIndex, byteNum;
	getStackIndex(stackIndex, byteNum, 2);

	if (byteNum == 2)
		_stack[stackIndex] = ((ULONG)_stack[stackIndex] & 0xffff) | (_stack.top() << 16);
	else
		_stack[stackIndex] = ((ULONG)_stack[stackIndex] & 0xffff0000) | (_stack.top() & 0xffff);
}

void Interpreter::cmdSAD() {
	int stackIndex, byteNum;
	getStackIndex(stackIndex, byteNum, 4);

	_stack[stackIndex] = _stack.top();
}

void Interpreter::cmdLABA() {
	int stackIndex, byteNum;
	getStackIndex(stackIndex, byteNum, 4);
	int subIndex = _stack.pop();

	ULONG v = _stack[stackIndex - subIndex / 4] & (0xff << ((subIndex % 4) * 8));
	_stack.push(v);
}

void Interpreter::cmdLAWA() {
	int stackIndex, byteNum;
	getStackIndex(stackIndex, byteNum, 4);
	int subIndex = _stack.pop();

	ULONG v = (ULONG)_stack[stackIndex - subIndex / 4] & ((subIndex % 4) == 2 ? 0xffff0000 : 0xffff);
	_stack.push(v);
}

void Interpreter::cmdLADA() {
	int stackIndex, byteNum;
	getStackIndex(stackIndex, byteNum, 4);
	int subIndex = _stack.pop();
	assert((subIndex % 4) == 0);

	ULONG v = _stack[stackIndex - subIndex / 4];
	_stack.push(v);
}

void Interpreter::cmdSABA() {
	int stackIndex, byteNum;
	getStackIndex(stackIndex, byteNum, 4);
	uint val = _stack.pop() & 0xffff;
	int offset = _stack.pop();
	int bitShift = (offset % 4) * 8;

	_stack[_stackBase - offset / 4] = (_stack[_stackBase - offset / 4] & (0xf << bitShift))
		| (val << bitShift);
}

void Interpreter::cmdSAWA() {
	int stackIndex, byteNum;
	getStackIndex(stackIndex, byteNum, 4);
	uint val = _stack.pop() & 0xffff;
	int offset = _stack.pop();
	int bitShift = (offset % 4) * 8;
	assert((offset % 2) == 0);

	_stack[_stackBase - offset / 4] = (_stack[_stackBase - offset / 4] & (0xffff << bitShift))
		| (val << bitShift);
}

void Interpreter::cmdSADA() {
	int stackIndex, byteNum;
	getStackIndex(stackIndex, byteNum, 4);
	int val = _stack.pop();
	int offset = _stack.pop();
	assert((offset % 4) == 0);

	_stack[_stackBase - offset / 4] = val;
}

void Interpreter::cmdLEAA() {
	int offset = READ_LE_UINT16(_code);
	_code += 2;
	assert((offset % 4) == 0);

	BYTE *ptr = (BYTE *)&_stack[_stackBase + (offset / 4)];
	_stack.pop();
	_stack.push(ptr);
}

void Interpreter::cmdLSB() {
	uint32 offset = READ_LE_UINT16(_code);
	_code += 2;
	byte *srcP = (byte *)_instance + _staticOffset + offset;

	_stack.top() = *srcP;
}

void Interpreter::cmdLSW() {
	uint32 offset = READ_LE_UINT16(_code);
	_code += 2;
	byte *srcP = (byte *)_instance + _staticOffset + offset;

	_stack.top() = READ_LE_UINT16(srcP);
}

void Interpreter::cmdLSD() {
	uint32 offset = READ_LE_UINT16(_code);
	_code += 2;
	byte *srcP = (byte *)_instance + _staticOffset + offset;

	_stack.top() = READ_LE_UINT32(srcP);
}

void Interpreter::cmdSSB() {
	uint32 offset = READ_LE_UINT16(_code);
	_code += 2;
	byte *destP = (byte *)_instance + _staticOffset + offset;

	*destP = _stack.top() & 0xff;
}

void Interpreter::cmdSSW() {
	uint32 offset = READ_LE_UINT16(_code);
	_code += 2;
	byte *destP = (byte *)_instance + _staticOffset + offset;

	WRITE_LE_UINT16(destP, _stack.top() & 0xffff);
}

void Interpreter::cmdSSD() {
	uint32 offset = READ_LE_UINT16(_code);
	_code += 2;
	byte *destP = (byte *)_instance + _staticOffset + offset;

	WRITE_LE_UINT32(destP, _stack.top());
}

void Interpreter::cmdLSBA() {
	ULONG offset = READ_LE_UINT16(_code);
	_code += 2;
	byte *srcP = (byte *)_instance + _staticOffset + offset + _stack.pop();

	_stack.push(*srcP);
}

void Interpreter::cmdLSWA() {
	ULONG offset = READ_LE_UINT16(_code);
	_code += 2;
	byte *srcP = (byte *)_instance + _staticOffset + offset + _stack.pop();

	_stack.push(READ_LE_UINT16(srcP));
}

void Interpreter::cmdLSDA() {
	ULONG offset = READ_LE_UINT16(_code);
	_code += 2;
	byte *srcP = (byte *)_instance + _staticOffset + offset + _stack.pop();

	_stack.push(READ_LE_UINT32(srcP));
}

void Interpreter::cmdSSBA() {
	ULONG val = _stack.pop();
	ULONG offset = READ_LE_UINT16(_code);
	_code += 2;
	byte *destP = (byte *)_instance + _staticOffset + offset + _stack.pop();

	*destP = val;
	_stack.push(val);
}

void Interpreter::cmdSSWA() {
	ULONG val = _stack.pop();
	ULONG offset = READ_LE_UINT16(_code);
	_code += 2;
	byte *destP = (byte *)_instance + _staticOffset + offset + _stack.pop();

	WRITE_LE_UINT16(destP, val);
	_stack.push(val);
}

void Interpreter::cmdSSDA() {
	ULONG val = _stack.pop();
	ULONG offset = READ_LE_UINT16(_code);
	_code += 2;
	byte *destP = (byte *)_instance + _staticOffset + offset + _stack.pop();

	WRITE_LE_UINT32(destP, val);
	_stack.push(val);
}

void Interpreter::cmdLESA() {
	ULONG offset = READ_LE_UINT16(_code);
	_code += 2;
	byte *srcP = (byte *)_instance + _staticOffset + offset;

	_stack.top() = srcP;
}

void Interpreter::cmdLXB() {
	Objects &objects = *_vm->_objects;
	ULONG offset = READ_LE_UINT16(_code);
	_code += 2;
	byte *instP = (byte *)_thunk + _externOffset + offset;
	ULONG instanceOffset = READ_LE_UINT16(instP);

	int objIndex = _stack.pop();
	byte *srcP = (byte *)Resources::addr(objects[objIndex]) + instanceOffset;

	_stack.push(*srcP);
}

void Interpreter::cmdLXW() {
	Objects &objects = *_vm->_objects;
	ULONG offset = READ_LE_UINT16(_code);
	_code += 2;
	byte *instP = (byte *)_thunk + _externOffset + offset;
	ULONG instanceOffset = READ_LE_UINT16(instP);

	int objIndex = _stack.pop();
	byte *srcP = (byte *)Resources::addr(objects[objIndex]) + instanceOffset;

	_stack.push(READ_LE_UINT16(srcP));
}

void Interpreter::cmdLXD() {
	Objects &objects = *_vm->_objects;
	ULONG offset = READ_LE_UINT16(_code);
	_code += 2;
	byte *instP = (byte *)_thunk + _externOffset + offset;
	ULONG instanceOffset = READ_LE_UINT16(instP);

	int objIndex = _stack.pop();
	byte *srcP = (byte *)Resources::addr(objects[objIndex]) + instanceOffset;

	_stack.push(READ_LE_UINT32(srcP));
}

void Interpreter::cmdSXB() {
	Objects &objects = *_vm->_objects;
	ULONG val = _stack.pop() & 0xff;

	ULONG offset = READ_LE_UINT16(_code);
	_code += 2;
	byte *instP = (byte *)_thunk + _externOffset + offset;
	ULONG instanceOffset = READ_LE_UINT16(instP);

	int objIndex = _stack.pop();
	byte *destP = (byte *)Resources::addr(objects[objIndex]) + instanceOffset;

	*destP = val;
	_stack.push(val);
}

void Interpreter::cmdSXW() {
	Objects &objects = *_vm->_objects;
	ULONG val = _stack.pop();

	ULONG offset = READ_LE_UINT16(_code);
	_code += 2;
	byte *instP = (byte *)_thunk + _externOffset + offset;
	ULONG instanceOffset = READ_LE_UINT16(instP);

	int objIndex = _stack.pop();
	byte *destP = (byte *)Resources::addr(objects[objIndex]) + instanceOffset;

	WRITE_LE_UINT16(destP, val);
	_stack.push(val);
}

void Interpreter::cmdSXD() {
	Objects &objects = *_vm->_objects;
	ULONG val = _stack.pop();

	ULONG offset = READ_LE_UINT16(_code);
	_code += 2;
	byte *instP = (byte *)_thunk + _externOffset + offset;
	ULONG instanceOffset = READ_LE_UINT16(instP);

	int objIndex = _stack.pop();
	byte *destP = (byte *)Resources::addr(objects[objIndex]) + instanceOffset;

	WRITE_LE_UINT32(destP, val);
	_stack.push(val);
}

void Interpreter::cmdLXBA() {
	Objects &objects = *_vm->_objects;
	ULONG offset = READ_LE_UINT16(_code);
	_code += 2;
	byte *instP = (byte *)_thunk + _externOffset + offset;
	ULONG instanceOffset = READ_LE_UINT16(instP);

	ULONG v = _stack.pop();
	instanceOffset += v >> 16;
	int objIndex = v & 0xffff;

	byte *srcP = (byte *)Resources::addr(objects[objIndex]) + instanceOffset;
	_stack.push(*srcP);
}

void Interpreter::cmdLXWA() {
	Objects &objects = *_vm->_objects;
	ULONG offset = READ_LE_UINT16(_code);
	_code += 2;
	byte *instP = (byte *)_thunk + _externOffset + offset;
	ULONG instanceOffset = READ_LE_UINT16(instP);

	ULONG v = _stack.pop();
	instanceOffset += v >> 16;
	int objIndex = v & 0xffff;

	byte *srcP = (byte *)Resources::addr(objects[objIndex]) + instanceOffset;
	_stack.push(READ_LE_UINT16(srcP));
}

void Interpreter::cmdLXDA() {
	Objects &objects = *_vm->_objects;
	ULONG offset = READ_LE_UINT16(_code);
	_code += 2;
	byte *instP = (byte *)_thunk + _externOffset + offset;
	ULONG instanceOffset = READ_LE_UINT16(instP);

	ULONG v = _stack.pop();
	instanceOffset += v >> 16;
	int objIndex = v & 0xffff;

	byte *srcP = (byte *)Resources::addr(objects[objIndex]) + instanceOffset;
	_stack.push(READ_LE_UINT32(srcP));
}

void Interpreter::cmdSXBA() {
	Objects &objects = *_vm->_objects;
	ULONG val = _stack.pop();

	ULONG offset = READ_LE_UINT16(_code);
	_code += 2;
	byte *instP = (byte *)_thunk + _externOffset + offset;
	ULONG instanceOffset = READ_LE_UINT16(instP);

	ULONG v = _stack.pop();
	instanceOffset += v >> 16;
	int objIndex = v & 0xffff;

	byte *destP = (byte *)Resources::addr(objects[objIndex]) + instanceOffset;
	*destP = val & 0xff;
	_stack.push(val);
}

void Interpreter::cmdSXWA() {
	Objects &objects = *_vm->_objects;
	ULONG val = _stack.pop();

	ULONG offset = READ_LE_UINT16(_code);
	_code += 2;
	byte *instP = (byte *)_thunk + _externOffset + offset;
	ULONG instanceOffset = READ_LE_UINT16(instP);

	ULONG v = _stack.pop();
	instanceOffset += v >> 16;
	int objIndex = v & 0xffff;

	byte *destP = (byte *)Resources::addr(objects[objIndex]) + instanceOffset;
	*destP = val & 0xffff;
	_stack.push(val);
}

void Interpreter::cmdSXDA() {
	Objects &objects = *_vm->_objects;
	ULONG val = _stack.pop();

	ULONG offset = READ_LE_UINT16(_code);
	_code += 2;
	byte *instP = (byte *)_thunk + _externOffset + offset;
	ULONG instanceOffset = READ_LE_UINT16(instP);

	ULONG v = _stack.pop();
	instanceOffset += v >> 16;
	int objIndex = v & 0xffff;

	byte *destP = (byte *)Resources::addr(objects[objIndex]) + instanceOffset;
	*destP = val & 0xff;
	_stack.push(val);
}

void Interpreter::cmdLEXA() {
	Objects &objects = *_vm->_objects;
	ULONG offset = READ_LE_UINT16(_code);
	_code += 2;
	byte *instP = (byte *)_thunk + _externOffset + offset;
	ULONG instanceOffset = READ_LE_UINT16(instP);

	ULONG v = _stack.pop();
	instanceOffset += v >> 16;
	int objIndex = v & 0xffff;

	byte *ptr = (byte *)Resources::addr(objects[objIndex]) + instanceOffset;
	_stack.push((BYTE *)ptr);
}

void Interpreter::cmdSXAS() {
	ULONG index = _stack.pop();
	_stack.top() |= index << 16;
}

void Interpreter::cmdLECA() {
	ULONG val = READ_LE_UINT16(_code);
	_code += 2;
	_stack.top() = (BYTE *)_ds32 + val;
}

void Interpreter::cmdSOLE() {
	Objects &objects = *_vm->_objects;
	HRES hres = objects[_stack.pop()];
	_stack.push((BYTE *)hres);
}

void Interpreter::cmdEND() {
	_breakFlag = true;
}

void Interpreter::cmdBRK() {
	error("BRK encountered");
}

Parameter Interpreter::loadString(Parameters params) {
	_vm->_resources->loadString(params[0], params[1]);
	return Parameter();
}

Parameter Interpreter::loadResource(Parameters params) {
	_vm->_resources->loadResource(params[0], params[1]);
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
	if (params[0] == GAME_STARTUP_STATE) {
		uint32 newVal = params[1];
		switch (newVal) {
		case 0:
			_startupState = SS_NONE;
			break;
		case MKTAG('C', 'I', 'N', 'E'):
			_startupState = SS_CINE;
			break;
		case MKTAG('V', 'I', 'C', 'T'):
			_startupState = SS_VICT;
			break;
		case MKTAG('C', 'H', 'G', 'N'):
			_startupState = SS_CHGN;
			break;
		case MKTAG('I', 'N', 'T', 'R'):
			_startupState = SS_INTR;
			break;
		default:
			break;
		}
	} else {
		pokemem(params.size(), params[0], params[1]);
	}

	return Parameter();
}

Parameter Interpreter::peekMem(Parameters params) {
	if (params[0] == GAME_STARTUP_STATE) {
		switch (_startupState) {
		case SS_CINE:
			return MKTAG('C', 'I', 'N', 'E');
		case SS_VICT:
			return MKTAG('V', 'I', 'C', 'T');
		case SS_CHGN:
			return MKTAG('C', 'H', 'G', 'N');
		case SS_INTR:
			return MKTAG('I', 'N', 'T', 'R');
		default:
			return Parameter();
		}
	} else {
		return peekmem(params.size(), params[0]);
	}
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
	_vm->_events->notify(params[0], params[1], params[2], params[3]);
	return Parameter();
}

Parameter Interpreter::cancel(Parameters params) {
	_vm->_events->cancel(params[0], params[1], params[2], params[3]);
	return Parameter();
}

Parameter Interpreter::drainEventQueue(Parameters params) {
	_vm->_events->drainEventQueue();
	return Parameter();
}

Parameter Interpreter::postEvent(Parameters params) {
	_vm->_events->postEvent(params[0], params[1], params[2]);
	return Parameter();
}

Parameter Interpreter::sendEvent(Parameters params) {
	_vm->_events->sendEvent(params[0], params[1], params[2]);
	return Parameter();
}

Parameter Interpreter::peekEvent(Parameters params) {
	return _vm->_events->peekEvent() ? 1 : 0;
}

Parameter Interpreter::dispatchEvent(Parameters params) {
	_vm->_events->dispatchEvent();
	return Parameter();
}

Parameter Interpreter::flushEventQueue(Parameters params) {
	_vm->_events->flushEventQueue(params[0], params[1], params[2]);
	return Parameter();
}

Parameter Interpreter::flushInputEvents(Parameters params) {
	_vm->_events->flushInputEvents();
	return Parameter();
}

Parameter Interpreter::initInterface(Parameters params) {
	// Handled during engine startup
	return Parameter();
}

Parameter Interpreter::shutdownInterface(Parameters params) {
	// Handled during engine shutdown
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
	_vm->_events->showMouse();
	return Parameter();
}

Parameter Interpreter::hideMouse(Parameters params) {
	_vm->_events->hideMouse();
	return Parameter();
}

Parameter Interpreter::mouseXY(Parameters params) {
	return _vm->_events->getXY();
}

Parameter Interpreter::mouseInWindow(Parameters params) {
	return _vm->_events->mouseInWindow(params[0]);
}

Parameter Interpreter::lockMouse(Parameters params) {
	_vm->_events->lockMouse();
	return Parameter();
}

Parameter Interpreter::unlockMouse(Parameters params) {
	_vm->_events->unlockMouse();
	return Parameter();
}

Parameter Interpreter::getKey(Parameters params) {
	_vm->_events->getKey();
	return Parameter();
}

Parameter Interpreter::initGraphics(Parameters params) {
	// The Screen class already initializes the graphics
	return Parameter();
}

Parameter Interpreter::drawDot(Parameters params) {
	_vm->_screen->panes(params[0]).drawDot(Common::Point(params[1], params[2]), params[3]);
	return Parameter();
}

Parameter Interpreter::drawLine(Parameters params) {
	_vm->_screen->panes(params[0]).drawLine(Common::Point(params[1], params[2]),
	                                        Common::Point(params[3], params[4]), params[5]);
	return Parameter();
}

Parameter Interpreter::lineTo(Parameters params) {
	_vm->_screen->lineTo(params);
	return Parameter();
}

Parameter Interpreter::drawRectangle(Parameters params) {
	_vm->_screen->panes(params[0]).drawRect(Common::Rect(params[1], params[2],
	                                        params[3], params[4]), params[5]);
	return Parameter();
}

Parameter Interpreter::fillRectangle(Parameters params) {
	_vm->_screen->panes(params[0]).fillRect(Common::Rect(params[1], params[2],
	                                        params[3], params[4]), params[5]);
	return Parameter();
}

Parameter Interpreter::hashRectangle(Parameters params) {
	_vm->_screen->panes(params[0]).hashRect(Common::Rect(params[1], params[2],
	                                        params[3], params[4]), params[5]);
	return Parameter();
}

Parameter Interpreter::getBitmapHeight(Parameters params) {
	return Shapes::getBitmapHeight(params[0], params[1]);
}

Parameter Interpreter::drawBitmap(Parameters params) {
	_vm->_screen->drawBitmap(params[0], params[1], params[2], Common::Point(params[3], params[4]),
		params[5], params[6], params[7], params[8]);
	return Parameter();
}

Parameter Interpreter::visibleBitmapRect(Parameters params) {
	assert(params[5]);
	return Shapes::visibleBitmapRect(params[0], params[1], params[2], params[3], params[4], (uint16 *)((BYTE *)params[5]));
}

Parameter Interpreter::setPalette(Parameters params) {
	_vm->_screen->setPalette(params[0], params[1]);
	return Parameter();
}

Parameter Interpreter::refreshWindow(Parameters params) {
	// Uneeded call to mouse_pane_refresh
	return Parameter();
}

Parameter Interpreter::wipeWindow(Parameters params) {
	_vm->_screen->windows(params[0]).wipeWindow(params[1]);
	return Parameter();
}

Parameter Interpreter::shutdownGraphics(Parameters params) {
	// Screen class handles shutting down graphics
	return Parameter();
}

Parameter Interpreter::waitVerticalRetrace(Parameters params) {
	Events &events = *_vm->_events;
	uint32 frameCounter = events.frameCounter();

	while (!_vm->shouldQuit() && frameCounter == events.frameCounter())
		events.delay(5);

	return Parameter();
}

Parameter Interpreter::readPalette(Parameters params) {
	return _vm->_screen->readPalette(params[0]);
}

Parameter Interpreter::writePalette(Parameters params) {
	_vm->_screen->writePalette(params[0], (ULONG)params[1]);
	return Parameter();
}

Parameter Interpreter::pixelFade(Parameters params) {
	_vm->_screen->pixelFade(params[0], params[1], params[2]);
	return Parameter();
}

Parameter Interpreter::colorFade(Parameters params) {
	_vm->_screen->colorFade(params[0], params[1]);
	return Parameter();
}

Parameter Interpreter::lightFade(Parameters params) {
	_vm->_screen->lightFade(params[0], params[1]);
	return Parameter();
}

Parameter Interpreter::assignWindow(Parameters params) {
	_vm->_screen->assignWindow(params[0], Common::Rect(params[1], params[2], params[3], params[4]));
	return Parameter();
}

Parameter Interpreter::assignSubWindow(Parameters params) {
	return _vm->_screen->assignSubWindow(params[0], params[1], Common::Rect(params[2], params[3],
	                                     params[4], params[5]));
}

Parameter Interpreter::releaseWindow(Parameters params) {
	_vm->_screen->releaseWindow(params[0]);
	return Parameter();
}

Parameter Interpreter::getLeft(Parameters params) {
	return _vm->_screen->panes(params[0]).getLeft();
}

Parameter Interpreter::getRight(Parameters params) {
	return _vm->_screen->panes(params[0]).getRight();
}

Parameter Interpreter::getTop(Parameters params) {
	return _vm->_screen->panes(params[0]).getTop();
}

Parameter Interpreter::getBottom(Parameters params) {
	return _vm->_screen->panes(params[0]).getBottom();
}

Parameter Interpreter::setLeft(Parameters params) {
	_vm->_screen->panes(params[0]).setLeft(params[1]);
	return Parameter();
}

Parameter Interpreter::setRight(Parameters params) {
	_vm->_screen->panes(params[0]).setRight(params[1]);
	return Parameter();
}

Parameter Interpreter::setTop(Parameters params) {
	_vm->_screen->panes(params[0]).setTop(params[1]);
	return Parameter();
}

Parameter Interpreter::setBottom(Parameters params) {
	_vm->_screen->panes(params[0]).setBottom(params[1]);
	return Parameter();
}

Parameter Interpreter::textWindow(Parameters params) {
	_vm->_screen->setTextWindow(params[0], params[1]);
	return Parameter();
}

Parameter Interpreter::textStyle(Parameters params) {
	_vm->_screen->textStyle(params[0], params[1], params[2]);
	return Parameter();
}

Parameter Interpreter::textXy(Parameters params) {
	_vm->_screen->textWindows(params[0]).setPosition(Common::Point(params[1], params[2]));
	return Parameter();
}

Parameter Interpreter::textColor(Parameters params) {
	_vm->_screen->textWindows(params[0]).textColor(params[1], params[2]);
	return Parameter();
}

Parameter Interpreter::textRefreshWindow(Parameters params) {
	_vm->_screen->textWindows(params[0]).refreshWindow(params[1]);
	return Parameter();
}

Parameter Interpreter::getTextX(Parameters params) {
	return _vm->_screen->textWindows(params[0]).getX();
}

Parameter Interpreter::getTextY(Parameters params) {
	return _vm->_screen->textWindows(params[0]).getY();
}

Parameter Interpreter::home(Parameters params) {
	_vm->_screen->textWindows(params[0]).home();
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
	_vm->_screen->textWindows(params[0]).crout();
	return Parameter();
}

Parameter Interpreter::charWidth(Parameters params) {
	return _vm->_screen->textWindows(params[0]).charWidth(params[1]);
}

Parameter Interpreter::fontHeight(Parameters params) {
	return _vm->_screen->textWindows(params[0]).fontHeight();
}

Parameter Interpreter::solidBarGraph(Parameters params) {
	_vm->_screen->solidBarGraph(Common::Rect(params[0], params[1], params[2], params[3]),
	                            params[4], params[5], params[6], params[7], params[8], params[9], params[10],
	                            params[11], params[12], params[13]);
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
	assert((BYTE *)params[2]);
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
	return _vm->_objects->createObject(params[0]);
}

Parameter Interpreter::createProgram(Parameters params) {
	return _vm->_objects->createProgram(params[0], params[1]);
}

Parameter Interpreter::destroyObject(Parameters params) {
	return _vm->_objects->destroyObject(params[0]);
}

Parameter Interpreter::flushCache(Parameters params) {
	return _vm->_objects->flushCache(params[0]);
}

Parameter Interpreter::thrashCache(Parameters params) {
	_vm->_objects->thrashCache();
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

