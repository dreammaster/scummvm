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

#ifndef AESOP_INTERPRETER_H
#define AESOP_INTERPRETER_H

#include "common/stack.h"
#include "aesop/resources.h"

namespace Aesop {

#define add_ptr(base,offset) ((void *)((ULONG)(base) + (ULONG)(offset)))
#define ptr_dif(top,bot) (((BYTE *)(top) - (BYTE *)(bot)))
#define far_memmove(dest, src, len) ((void *)memmove((dest),(src),(len)))
#define add_offset(s,o) ((void *)((ULONG)(s) + (ULONG)(o)))

class AesopEngine;
class Interpreter;

struct Parameter {
	uint32 _val;
	BYTE *_ptr;

	Parameter() : _val(0), _ptr(nullptr) {}
	Parameter(uint32 v) : _val(v), _ptr(nullptr) {}
	Parameter(BYTE *v) : _val(0), _ptr(v) {}
};

class Parameters : public Common::Array<Parameter> {
private:
	Parameter _empty;
public:
	Parameter &operator[](int idx) {
		return (idx >= (int)size()) ? _empty : Common::Array<Parameter>::operator[](idx);
	}
};

struct MethodStackEntry {
	byte *_ds32;
	byte *_code;
	uint _stackSize, _stackBase;

	MethodStackEntry() : _ds32(nullptr), _code(nullptr), _stackSize(0), _stackBase(0) {}
	MethodStackEntry(byte *ds32, byte *code, int stackSize, int stackBase) :
		_ds32(ds32), _code(code), _stackSize(stackSize), _stackBase(stackBase) {}
};

class InterpreterStack {
private:
	Common::Array<Parameter> _stack;
public:
	typedef Common::Array<Parameter>::size_type size_type;
	InterpreterStack() {}

	bool empty() const { return _stack.empty(); }

	void clear() { _stack.clear(); }

	void resize(int newSize) { _stack.resize(newSize); }

	void push(uint32 x) { _stack.push_back(x); }

	void pushPtr(BYTE *x) { _stack.push_back(x); }

	void reserve(size_type count) {
		while (count-- > 0)
			_stack.push_back(Parameter());
	}

	uint32 &top() { return _stack.back()._val; }

	const uint32 &top() const { return _stack.back()._val; }

	BYTE *&topPtr() { return _stack.back()._ptr; }

	Parameter popEntry() {
		Parameter tmp = _stack.back();
		_stack.pop_back();
		return tmp;
	}

	uint32 pop() { return popEntry()._val; }

	BYTE *popPtr() { return popEntry()._ptr; }

	size_type size() const {
		return _stack.size();
	}

	uint32 &operator[](size_type i) { return _stack[i]._val; }
	const uint32 &operator[](size_type i) const { return _stack[i]._val; }
};

typedef void (Interpreter::*OpcodeMethod)();
typedef int (Interpreter::*ExternMethod)(Parameters param);

class Interpreter {
private:
	AesopEngine *_vm;
	static const OpcodeMethod _opcodes[];
	static const ExternMethod _methods[];
	InterpreterStack _stack;
	Common::Stack<MethodStackEntry> _methodStack;
	LONG _currentIndex;
	LONG _offThis;
	HRES *_objList;
	HRES _instance;
	THDR *_thunk;
	byte *_ds32;
	byte *_code;
	int _stackBase;
	HRES _hPrg;
	UWORD _staticOffset;
	UWORD _externOffset;
	bool _breakFlag;
private:
	void cmdBRT();		// BRanch if True
	void cmdBRF();		// BRanch if False
	void cmdBRA();		// BRanch Always
	void cmdCASE();		// CASE selection
	void cmdPUSH();		// PUSH 0 onto stack
	void cmdDUP();		// DUPlicate top of stack
	void cmdNOT();		// Logical NOT(unary)
	void cmdSETB();		// SET Boolean value(unary)
	void cmdNEG();		// NEGate(unary)
	void cmdADD();		// ADD(binary)
	void cmdSUB();		// SUBtract(binary)
	void cmdMUL();		// MULtiply(binary)
	void cmdDIV();		// DIVide(binary)
	void cmdMOD();		// MODulus(binary)
	void cmdEXP();		// EXPonent(binary)
	void cmdBAND();		// Bitwise AND(binary)
	void cmdBOR();		// Bitwise OR(binary)
	void cmdXOR();		// Bitwise XOR(binary)
	void cmdBNOT();		// Bitwise NOT(unary)
	void cmdSHL();		// SHift Left(binary)
	void cmdSHR();		// SHift Right(binary)
	void cmdLT();		// Less Than(binary)
	void cmdLE();		// Less than or Equal(binary)
	void cmdEQ();		// EQual(binary)
	void cmdNE();		// Not Equal(binary)
	void cmdGE();		// Greater than or Equal(binary)
	void cmdGT();		// Greather Than(binary)
	void cmdINC();		// INCrement(unary)
	void cmdDEC();		// DECrement(unary)
	void cmdSHTC();		// Load SHorT Constant(0 - 256)
	void cmdINTC();		// Load INTeger Constant(256 - 64K)
	void cmdLNGC();		// Load LoNG Constant(64K - 4G)
	void cmdRCRS();		// Reference Code ReSource address
	void cmdCALL();		// CALL code resource handler
	void cmdSEND();		// SEND message
	void cmdPASS();		// PASS message to parent class
	void cmdJSR();		// Jump to SubRoutine
	void cmdRTS();		// ReTurn from Subroutine
	void cmdAIM();		// Array Index Multiply
	void cmdAIS();		// Array Index Shift
	void cmdLTBA();		// Load Table Byte Array
	void cmdLTWA();		// Load Table Word Array
	void cmdLTDA();		// Load Table Dword Array
	void cmdLETA();		// Load Effective Table Address
	void cmdLAB();		// Load Auto Byte
	void cmdLAW();		// Load Auto Word
	void cmdLAD();		// Load Auto Dword
	void cmdSAB();		// Store Auto Byte
	void cmdSAW();		// Store Auto Word
	void cmdSAD();		// Store Auto Dword
	void cmdLABA();		// Load Auto Byte Array
	void cmdLAWA();		// Load Auto Word Array
	void cmdLADA();		// Load Auto Dword Array
	void cmdSABA();		// Store Auto Byte Array
	void cmdSAWA();		// Store Auto Word Array
	void cmdSADA();		// Store Auto Dword Array
	void cmdLEAA();		// Load Effective Auto Address
	void cmdLSB();		// Load Static Byte
	void cmdLSW();		// Load Static Word
	void cmdLSD();		// Load Static Dword
	void cmdSSB();		// Store Static Byte
	void cmdSSW();		// Store Static Word
	void cmdSSD();		// Store Static Dword
	void cmdLSBA();		// Load Static Byte Array
	void cmdLSWA();		// Load Static Word Array
	void cmdLSDA();		// Load Static Dword Array
	void cmdSSBA();		// Store Static Byte Array
	void cmdSSWA();		// Store Static Word Array
	void cmdSSDA();		// Store Static Dword Array
	void cmdLESA();		// Load Effective Static Address
	void cmdLXB();		// Load eXtern Byte
	void cmdLXW();		// Load eXtern Word
	void cmdLXD();		// Load eXtern Dword
	void cmdSXB();		// Store eXtern Byte
	void cmdSXW();		// Store eXtern Word
	void cmdSXD();		// Store eXtern Dword
	void cmdLXBA();		// Load eXtern Byte Array
	void cmdLXWA();		// Load eXtern Word Array
	void cmdLXDA();		// Load eXtern Dword Array
	void cmdSXBA();		// Store eXtern Byte Array
	void cmdSXWA();		// Store eXtern Word Array
	void cmdSXDA();		// Store eXtern Dword Array
	void cmdLEXA();		// Load Effective eXtern Address
	void cmdSXAS();		// Set eXtern Array Source
	void cmdLECA();		// Load Effective Code Address
	void cmdSOLE();		// Selector for Object List Entry
	void cmdEND();		// END of handler
	void cmdBRK();		// BReaKpoint for debugging
private:
	// Miscellaneous functions
	int loadString(Parameters params);
	int loadResource(Parameters params);
	int copyString(Parameters params);
	int stringForceLower(Parameters params);
	int stringForceUpper(Parameters params);
	int stringLen(Parameters params);
	int stringCompare(Parameters params);
	int strVal(Parameters params);
	int envVal(Parameters params);
	int beep(Parameters params);
	int pokeMem(Parameters params);
	int peekMem(Parameters params);
	int rnd(Parameters params);
	int dice(Parameters params);
	int absv(Parameters params);
	int minv(Parameters params);
	int maxv(Parameters params);
	int diagnose(Parameters params);
	int heapfree(Parameters params);

	// Event functions
	int notify(Parameters params);
	int cancel(Parameters params);
	int drainEventQueue(Parameters params);
	int postEvent(Parameters params);
	int sendEvent(Parameters params);
	int peekEvent(Parameters params);
	int dispatchEvent(Parameters params);
	int flushEventQueue(Parameters params);
	int flushInputEvents(Parameters params);

	// Interface functions
	int initInterface(Parameters params);
	int shutdownInterface(Parameters params);
	int setMousePointer(Parameters params);
	int setWaitPointer(Parameters params);
	int standbyCursor(Parameters params);
	int resumeCursor(Parameters params);
	int showMouse(Parameters params);
	int hideMouse(Parameters params);
	int mouseXY(Parameters params);
	int mouseInWindow(Parameters params);
	int lockMouse(Parameters params);
	int unlockMouse(Parameters params);
	int getKey(Parameters params);

	// Graphics-related functions
	int initGraphics(Parameters params);
	int drawDot(Parameters params);
	int drawLine(Parameters params);
	int lineTo(Parameters params);
	int drawRectangle(Parameters params);
	int fillRectangle(Parameters params);
	int hashRectangle(Parameters params);
	int getBitmapHeight(Parameters params);
	int drawBitmap(Parameters params);
	int visibleBitmapRect(Parameters params);
	int setPalette(Parameters params);
	int refreshWindow(Parameters params);
	int wipeWindow(Parameters params);
	int shutdownGraphics(Parameters params);
	int waitVerticalRetrace(Parameters params);
	int readPalette(Parameters params);
	int writePalette(Parameters params);
	int pixelFade(Parameters params);
	int colorFade(Parameters params);
	int lightFade(Parameters params);

	int assignWindow(Parameters params);
	int assignSubWindow(Parameters params);
	int releaseWindow(Parameters params);
	int getLeft(Parameters params);
	int getRight(Parameters params);
	int getTop(Parameters params);
	int getBottom(Parameters params);
	int setLeft(Parameters params);
	int setRight(Parameters params);
	int setTop(Parameters params);
	int setBottom(Parameters params);

	int textWindow(Parameters params);
	int textStyle(Parameters params);
	int textXy(Parameters params);
	int textColor(Parameters params);
	int textRefreshWindow(Parameters params);
	int getTextX(Parameters params);
	int getTextY(Parameters params);
	int home(Parameters params);
	int print(Parameters params);
	int sPrint(Parameters params);
	int dPrint(Parameters params);
	int aPrint(Parameters params);
	int crOut(Parameters params);
	int charWidth(Parameters params);
	int fontHeight(Parameters params);

	int solidBarGraph(Parameters params);

	// Sound-related functions
	int initSound(Parameters params);
	int shutdownSound(Parameters params);
	int loadSoundBlock(Parameters params);
	int soundEffect(Parameters params);
	int playSequence(Parameters params);
	int loadMusic(Parameters params);
	int unloadMusic(Parameters params);
	int setSoundStatus(Parameters params);

	// Eye III object management
	int createObject(Parameters params);
	int createProgram(Parameters params);
	int destroyObject(Parameters params);
	int flushCache(Parameters params);
	int thrashCache(Parameters params);

	// Eye III support functions
	int stepX(Parameters params);
	int stepY(Parameters params);
	int stepFDIR(Parameters params);

	int stepSquareX(Parameters params);
	int stepSquareY(Parameters params);
	int stepRegion(Parameters params);

	int distance(Parameters params);
	int seekDirection(Parameters params);

	int spellRequest(Parameters params);
	int spellList(Parameters params);
	int magicField(Parameters params);
	int doDots(Parameters params);
	int doIce(Parameters params);

	int readSaveDirectory(Parameters params);
	int savegameTitle(Parameters params);
	int writeSaveDirectory(Parameters params);

	int saveGame(Parameters params);
	int suspendGame(Parameters params);
	int resumeItems(Parameters params);
	int resumeLevel(Parameters params);
	int changeLevel(Parameters params);
	int restoreItems(Parameters params);
	int restoreLevelObjects(Parameters params);
	int readInitialItems(Parameters params);
	int writeInitialTempfiles(Parameters params);
	int createInitialBinaryFiles(Parameters params);
	int launch(Parameters params);

	// Eye II savegame file access
	int openTransferFile(Parameters params);
	int closeTransferFile(Parameters params);
	int playerAttrib(Parameters params);
	int itemAttrib(Parameters params);
	int arrowCount(Parameters params);
public:
	ULONG _currentMsg;
	ULONG _currentThis;
public:
	Interpreter(AesopEngine *vm, HRES *objList, int stackSize);
	~Interpreter();

	uint first(HRES dictionary);
	uint iterate(const void *base, uint cur, const char **tag, const char **def);

	const char *lookup(HRES dictionary, const Common::String &key);

	void shutdown(void);
	void arguments(void *base, ULONG size);
	LONG execute(LONG index, LONG msgNum, HRES vector);
};

} // End of namespace Aesop

#endif

