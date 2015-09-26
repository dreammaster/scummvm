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

#define add_ptr(base,offset) ((void *)((byte *)(base) + (uint)(offset)))
#define ptr_dif(top,bot) (((byte *)(top) - (byte *)(bot)))
#define far_memmove(dest, src, len) ((void *)memmove((dest),(src),(len)))
#define add_offset(s,o) ((void *)((byte *)(s) + (ULONG)(o)))

enum StartupState { SS_NONE = 0, SS_CINE = 1, SS_VICT = 2, SS_CHGN = 3, SS_INTR = 4 };

class AesopEngine;
class Interpreter;

class Parameter {
private:
	uint32 _val;
	void *_ptr;
	const void *_ptrConst;
public:
	Parameter() : _val(0), _ptr(nullptr), _ptrConst(nullptr) {}
	Parameter(ULONG v) : _val(v), _ptr(nullptr), _ptrConst(nullptr) {}
	Parameter(BYTE *v) : _val(0), _ptr(v), _ptrConst(v) {}
	Parameter(void *v) : _val(0), _ptr(v), _ptrConst(v) {}
	Parameter(const BYTE *v) : _val(0), _ptr(nullptr), _ptrConst(v) {}
	Parameter(const void *v) : _val(0), _ptr(nullptr), _ptrConst(v) {}
	Parameter(const char *v) : _val(0), _ptr(nullptr), _ptrConst(v) {}

	operator ULONG() const { return _val; }
	operator BYTE *() const { return (BYTE *)_ptr; }
	operator WORD *() const { return (WORD *)_ptr; }
	operator LONG *() const { return (LONG *)_ptr; }
	operator ULONG *() const { return (ULONG *)_ptr; }
	operator const BYTE *() const { return (const BYTE *)_ptrConst; }
	operator byte *() { return (byte *)_ptr; }
	operator const void *() const { return _ptrConst; }
	operator const char *() const { return (const char *)_ptrConst; }

	const Parameter operator+(int other) const;
	const Parameter operator-(int other) const;
	Parameter operator&(int other);
	Parameter &operator|=(int other);
	bool operator==(Parameter &rhs) const;
	bool operator!=(Parameter &rhs) const;
};

class Parameters : public Common::Array<Parameter> {
private:
	Parameter _empty;
public:
	Parameters() : Common::Array<Parameter>() {}
	Parameters(const Parameter &v) : Common::Array<Parameter>() {
		push_back(v);
	}

	Parameter &operator[](int idx) {
		return (idx >= (int)size()) ? _empty : Common::Array<Parameter>::operator[](idx);
	}
	const Parameter &operator[](int idx) const {
		return (idx >= (int)size()) ? _empty : Common::Array<Parameter>::operator[](idx);
	}

	/**
	 * Remove a parameter from the front
	 */
	Parameter pop();
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

	bool empty() const {
		return _stack.empty();
	}

	void clear() {
		_stack.clear();
	}

	void resize(int newSize) {
		_stack.resize(newSize);
	}

	void push(Parameter p) {
		_stack.push_back(p);
	}

	void reserve(size_type count) {
		while (count-- > 0)
			_stack.push_back(Parameter());
	}

	Parameter &top() {
		return _stack.back();
	}

	const Parameter &top() const {
		return _stack.back();
	}

	Parameter pop() {
		Parameter tmp = _stack.back();
		_stack.pop_back();
		return tmp;
	}
	size_type size() const {
		return _stack.size();
	}

	Parameter &operator[](size_type i) {
		return _stack[i];
	}
	const Parameter &operator[](size_type i) const {
		return _stack[i];
	}
};

typedef void (Interpreter::*OpcodeMethod)();
typedef Parameter(Interpreter::*ExternMethod)(Parameters param);

class Interpreter {
private:
	AesopEngine *_vm;
	static const OpcodeMethod _opcodes[];
	static const ExternMethod _methods[];
	InterpreterStack _stack;
	Common::Stack<MethodStackEntry> _methodStack;
	LONG _currentIndex;
	ULONG _currentMsg;
	LONG _offThis;
	IHDR *_instance;
	THDR *_thunk;
	byte *_ds32;
	byte *_code;
	int _stackBase;
	HRES _hPrg;
	UWORD _staticOffset;
	UWORD _externOffset;
	bool _breakFlag;
	const MV_entry *_currentVector;
private:
	/**
	 * When debug information is turned on, prints out the details of the current instruction
	 */
	void printInstruction(int opcode);

	/**
	 * Gets a stack offset from the script, and convert it to a stack index
	 * and byte offset within the stack entry
	 */
	void getStackIndex(int &stackIndex, int &byteNum, int dataSize);

	/**
	 * In case the memory for the object has changed in memory, re-adjust the memory pointers
	 * relative to the current start of the object
	 */
	void deref();
private:
	void cmdBRT();      // BRanch if True
	void cmdBRF();      // BRanch if False
	void cmdBRA();      // BRanch Always
	void cmdCASE();     // CASE selection
	void cmdPUSH();     // PUSH 0 onto stack
	void cmdDUP();      // DUPlicate top of stack
	void cmdNOT();      // Logical NOT(unary)
	void cmdSETB();     // SET Boolean value(unary)
	void cmdNEG();      // NEGate(unary)
	void cmdADD();      // ADD(binary)
	void cmdSUB();      // SUBtract(binary)
	void cmdMUL();      // MULtiply(binary)
	void cmdDIV();      // DIVide(binary)
	void cmdMOD();      // MODulus(binary)
	void cmdEXP();      // EXPonent(binary)
	void cmdBAND();     // Bitwise AND(binary)
	void cmdBOR();      // Bitwise OR(binary)
	void cmdXOR();      // Bitwise XOR(binary)
	void cmdBNOT();     // Bitwise NOT(unary)
	void cmdSHL();      // SHift Left(binary)
	void cmdSHR();      // SHift Right(binary)
	void cmdLT();       // Less Than(binary)
	void cmdLE();       // Less than or Equal(binary)
	void cmdEQ();       // EQual(binary)
	void cmdNE();       // Not Equal(binary)
	void cmdGE();       // Greater than or Equal(binary)
	void cmdGT();       // Greather Than(binary)
	void cmdINC();      // INCrement(unary)
	void cmdDEC();      // DECrement(unary)
	void cmdSHTC();     // Load SHorT Constant(0 - 256)
	void cmdINTC();     // Load INTeger Constant(256 - 64K)
	void cmdLNGC();     // Load LoNG Constant(64K - 4G)
	void cmdRCRS();     // Reference Code ReSource address
	void cmdCALL();     // CALL code resource handler
	void cmdSEND();     // SEND message
	void cmdPASS();     // PASS message to parent class
	void cmdJSR();      // Jump to SubRoutine
	void cmdRTS();      // ReTurn from Subroutine
	void cmdAIM();      // Array Index Multiply
	void cmdAIS();      // Array Index Shift
	void cmdLTBA();     // Load Table Byte Array
	void cmdLTWA();     // Load Table Word Array
	void cmdLTDA();     // Load Table Dword Array
	void cmdLETA();     // Load Effective Table Address
	void cmdLAB();      // Load Auto Byte
	void cmdLAW();      // Load Auto Word
	void cmdLAD();      // Load Auto Dword
	void cmdSAB();      // Store Auto Byte
	void cmdSAW();      // Store Auto Word
	void cmdSAD();      // Store Auto Dword
	void cmdLABA();     // Load Auto Byte Array
	void cmdLAWA();     // Load Auto Word Array
	void cmdLADA();     // Load Auto Dword Array
	void cmdSABA();     // Store Auto Byte Array
	void cmdSAWA();     // Store Auto Word Array
	void cmdSADA();     // Store Auto Dword Array
	void cmdLEAA();     // Load Effective Auto Address
	void cmdLSB();      // Load Static Byte
	void cmdLSW();      // Load Static Word
	void cmdLSD();      // Load Static Dword
	void cmdSSB();      // Store Static Byte
	void cmdSSW();      // Store Static Word
	void cmdSSD();      // Store Static Dword
	void cmdLSBA();     // Load Static Byte Array
	void cmdLSWA();     // Load Static Word Array
	void cmdLSDA();     // Load Static Dword Array
	void cmdSSBA();     // Store Static Byte Array
	void cmdSSWA();     // Store Static Word Array
	void cmdSSDA();     // Store Static Dword Array
	void cmdLESA();     // Load Effective Static Address
	void cmdLXB();      // Load eXtern Byte
	void cmdLXW();      // Load eXtern Word
	void cmdLXD();      // Load eXtern Dword
	void cmdSXB();      // Store eXtern Byte
	void cmdSXW();      // Store eXtern Word
	void cmdSXD();      // Store eXtern Dword
	void cmdLXBA();     // Load eXtern Byte Array
	void cmdLXWA();     // Load eXtern Word Array
	void cmdLXDA();     // Load eXtern Dword Array
	void cmdSXBA();     // Store eXtern Byte Array
	void cmdSXWA();     // Store eXtern Word Array
	void cmdSXDA();     // Store eXtern Dword Array
	void cmdLEXA();     // Load Effective eXtern Address
	void cmdSXAS();     // Set eXtern Array Source
	void cmdLECA();     // Load Effective Code Address
	void cmdSOLE();     // Selector for Object List Entry
	void cmdEND();      // END of handler
	void cmdBRK();      // BReaKpoint for debugging
private:
	// Miscellaneous functions
	Parameter loadString(Parameters params);
	Parameter loadResource(Parameters params);
	Parameter copyString(Parameters params);
	Parameter stringForceLower(Parameters params);
	Parameter stringForceUpper(Parameters params);
	Parameter stringLen(Parameters params);
	Parameter stringCompare(Parameters params);
	Parameter strVal(Parameters params);
	Parameter envVal(Parameters params);
	Parameter beep(Parameters params);
	Parameter pokeMem(Parameters params);
	Parameter peekMem(Parameters params);
	Parameter rnd(Parameters params);
	Parameter dice(Parameters params);
	Parameter absv(Parameters params);
	Parameter minv(Parameters params);
	Parameter maxv(Parameters params);
	Parameter diagnose(Parameters params);
	Parameter heapfree(Parameters params);

	// Event functions
	Parameter notify(Parameters params);
	Parameter cancel(Parameters params);
	Parameter drainEventQueue(Parameters params);
	Parameter postEvent(Parameters params);
	Parameter sendEvent(Parameters params);
	Parameter peekEvent(Parameters params);
	Parameter dispatchEvent(Parameters params);
	Parameter flushEventQueue(Parameters params);
	Parameter flushInputEvents(Parameters params);

	// Interface functions
	Parameter initInterface(Parameters params);
	Parameter shutdownInterface(Parameters params);
	Parameter setMousePointer(Parameters params);
	Parameter setWaitPointer(Parameters params);
	Parameter standbyCursor(Parameters params);
	Parameter resumeCursor(Parameters params);
	Parameter showMouse(Parameters params);
	Parameter hideMouse(Parameters params);
	Parameter mouseXY(Parameters params);
	Parameter mouseInWindow(Parameters params);
	Parameter lockMouse(Parameters params);
	Parameter unlockMouse(Parameters params);
	Parameter getKey(Parameters params);

	// Graphics-related functions
	Parameter initGraphics(Parameters params);
	Parameter drawDot(Parameters params);
	Parameter drawLine(Parameters params);
	Parameter lineTo(Parameters params);
	Parameter drawRectangle(Parameters params);
	Parameter fillRectangle(Parameters params);
	Parameter hashRectangle(Parameters params);
	Parameter getBitmapHeight(Parameters params);
	Parameter drawBitmap(Parameters params);
	Parameter visibleBitmapRect(Parameters params);
	Parameter setPalette(Parameters params);
	Parameter refreshWindow(Parameters params);
	Parameter wipeWindow(Parameters params);
	Parameter shutdownGraphics(Parameters params);
	Parameter waitVerticalRetrace(Parameters params);
	Parameter readPalette(Parameters params);
	Parameter writePalette(Parameters params);
	Parameter pixelFade(Parameters params);
	Parameter colorFade(Parameters params);
	Parameter lightFade(Parameters params);

	Parameter assignWindow(Parameters params);
	Parameter assignSubWindow(Parameters params);
	Parameter releaseWindow(Parameters params);
	Parameter getLeft(Parameters params);
	Parameter getRight(Parameters params);
	Parameter getTop(Parameters params);
	Parameter getBottom(Parameters params);
	Parameter setLeft(Parameters params);
	Parameter setRight(Parameters params);
	Parameter setTop(Parameters params);
	Parameter setBottom(Parameters params);

	Parameter textWindow(Parameters params);
	Parameter textStyle(Parameters params);
	Parameter textXy(Parameters params);
	Parameter textColor(Parameters params);
	Parameter textRefreshWindow(Parameters params);
	Parameter getTextX(Parameters params);
	Parameter getTextY(Parameters params);
	Parameter home(Parameters params);
	Parameter print(Parameters params);
	Parameter sPrint(Parameters params);
	Parameter dPrint(Parameters params);
	Parameter aPrint(Parameters params);
	Parameter crOut(Parameters params);
	Parameter charWidth(Parameters params);
	Parameter fontHeight(Parameters params);

	Parameter solidBarGraph(Parameters params);

	// Sound-related functions
	Parameter initSound(Parameters params);
	Parameter shutdownSound(Parameters params);
	Parameter loadSoundBlock(Parameters params);
	Parameter soundEffect(Parameters params);
	Parameter playSequence(Parameters params);
	Parameter loadMusic(Parameters params);
	Parameter unloadMusic(Parameters params);
	Parameter setSoundStatus(Parameters params);

	// Eye III object management
	Parameter createObject(Parameters params);
	Parameter createProgram(Parameters params);
	Parameter destroyObject(Parameters params);
	Parameter flushCache(Parameters params);
	Parameter thrashCache(Parameters params);

	// Eye III support functions
	Parameter stepX(Parameters params);
	Parameter stepY(Parameters params);
	Parameter stepFDIR(Parameters params);

	Parameter stepSquareX(Parameters params);
	Parameter stepSquareY(Parameters params);
	Parameter stepRegion(Parameters params);

	Parameter distance(Parameters params);
	Parameter seekDirection(Parameters params);

	Parameter spellRequest(Parameters params);
	Parameter spellList(Parameters params);
	Parameter magicField(Parameters params);
	Parameter doDots(Parameters params);
	Parameter doIce(Parameters params);

	Parameter readSaveDirectory(Parameters params);
	Parameter savegameTitle(Parameters params);
	Parameter writeSaveDirectory(Parameters params);

	Parameter saveGame(Parameters params);
	Parameter suspendGame(Parameters params);
	Parameter resumeItems(Parameters params);
	Parameter resumeLevel(Parameters params);
	Parameter changeLevel(Parameters params);
	Parameter restoreItems(Parameters params);
	Parameter restoreLevelObjects(Parameters params);
	Parameter readInitialItems(Parameters params);
	Parameter writeInitialTempfiles(Parameters params);
	Parameter createInitialBinaryFiles(Parameters params);
	Parameter launch(Parameters params);

	// Eye II savegame file access
	Parameter openTransferFile(Parameters params);
	Parameter closeTransferFile(Parameters params);
	Parameter playerAttrib(Parameters params);
	Parameter itemAttrib(Parameters params);
	Parameter arrowCount(Parameters params);
public:
	static StartupState _startupState;
	static ULONG _currentThis;
public:
	Interpreter(AesopEngine *vm);
	~Interpreter();
	static void init(AesopEngine *vm);

	static uint first(HRES dictionary);
	static uint iterate(const void *base, uint cur, const char **tag, const char **def);
	static const char *lookup(HRES dictionary, const Common::String &key);

	void addArgument(const Parameter &param);
	LONG execute(LONG index, LONG msgNum, HRES vector = HRES_NULL);
};

} // End of namespace Aesop

#endif

