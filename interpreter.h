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

//
//  AESOP runtime ASM calls
//

#ifndef AESOP_INTERPRETER_H
#define AESOP_INTERPRETER_H

#include "aesop/resources.h"

namespace Aesop {

#define add_ptr(base,offset) ((void *)((ULONG)(base) + (ULONG)(offset)))
#define ptr_dif(top,bot) (((BYTE *)(top) - (BYTE *)(bot)))
#define far_memmove(dest, src, len) ((void *)memmove((dest),(src),(len)))
#define add_offset(s,o) ((void *)((ULONG)(s) + (ULONG)(o)))

class AesopEngine;
class Interpreter;

typedef void (Interpreter::*OpcodeMethod)();

class Interpreter {
private:
	AesopEngine *_vm;
	static const OpcodeMethod _opcodes[];
	LONG _currentIndex;
	uint16 *_stackBase;
	uint16 *_stackPtr;
	HRES *_objList;
	HRES _instance;
	THDR *_thunk;
	const byte *_ds32;
	const byte *_code;
	uint16 *_fptr;
	HRES _hPrg;
	UWORD _staticOffset;
	UWORD _externOffset;
	bool _breakFlag;

	/**
	 * Dereference an offset to a pointer
	 */
	void deref();

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