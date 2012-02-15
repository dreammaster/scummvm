/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

/* Based on the Adventure Game Studio source code, copyright 1999-2011 Chris Jones,
 * which is licensed under the Artistic License 2.0.
 */

#include "engines/ags/script.h"
#include "engines/ags/util.h"
#include "engines/ags/vm.h"
#include "common/debug.h"
#include "common/stack.h"

namespace AGS {

#define SCOM_VERSION 89

#define CC_NUM_REGISTERS  8
#define INSTF_SHAREDATA   1
#define INSTF_ABORTED     2
#define INSTF_FREE        4
#define INSTF_RUNNING     8   // set by main code to confirm script isn't stuck
#define CC_STACK_SIZE     4000
#define MAX_CALL_STACK    100

void ccScript::readFrom(Common::SeekableReadStream *dta) {
	_instances = 0;

	uint32 magic = dta->readUint32BE();
	if (magic != MKTAG('S','C','O','M'))
		error("ccScript had invalid magic %x", magic);
	uint32 version = dta->readUint32LE();
	if (version > SCOM_VERSION)
		error("ccScript had invalid version %d", version);

	uint32 globalDataSize = dta->readUint32LE();
	uint32 codeSize = dta->readUint32LE();
	uint32 stringsSize = dta->readUint32LE();

	_globalData.resize(globalDataSize);
	if (globalDataSize)
		dta->read(&_globalData[0], globalDataSize);

	_code.resize(codeSize);
	for (uint i = 0; i < codeSize; ++i) {
		_code[i]._data = dta->readUint32LE();
		_code[i]._fixupType = 0;
	}

	_strings.resize(stringsSize);
	if (stringsSize)
		dta->read(&_strings[0], stringsSize);

	uint32 fixupsCount = dta->readUint32LE();
	Common::Array<byte> fixupTypes;
	fixupTypes.resize(fixupsCount);
	for (uint i = 0; i < fixupsCount; ++i)
		fixupTypes[i] = dta->readByte();

	// AGS uses the fixups to patch the code 'live' by adding pointer values.
	// We don't want to do that, so we store the fixups for use at runtime.
	// (This also allows us to share the code among different instances.)
	// TODO: We can probably optimise this by just setting high bits of the
	//       code data, if memory usage turns out to be a problem.
	for (uint i = 0; i < fixupsCount; ++i) {
		// this is the code array index (i.e. not bytes)
		uint32 fixupIndex = dta->readUint32LE();
		if (fixupIndex >= _code.size())
			error("fixup for %d is beyond code size %d", fixupIndex, _code.size());

		if (fixupTypes[i] == FIXUP_DATADATA) {
			// patch to global data
			// (usually strings, there aren't many of these)
			_globalFixups.push_back(fixupIndex);
		} else if (fixupTypes[i] && fixupTypes[i] <= 6) {
			// patch to code
			if (_code[fixupIndex]._fixupType)
				error("duplicate fixup (index %d, old type %d, new type %d)", fixupIndex,
					_code[fixupIndex]._fixupType, fixupTypes[i]);
			_code[fixupIndex]._fixupType = fixupTypes[i];
		} else {
			// invalid/unknown type
			error("invalid fixup type %d", fixupTypes[i]);
		}
	}
	Common::sort(_globalFixups.begin(), _globalFixups.end());

	debug(1, "script has %d fixups for %d code entries", fixupsCount, codeSize);

	uint32 importsCount = dta->readUint32LE();
	_imports.resize(importsCount);
	for (uint i = 0; i < importsCount; ++i) {
		_imports[i] = readString(dta);
		debug(5, "script import '%s'", _imports[i].c_str());
	}

	for (uint i = 0; i < _code.size(); ++i) {
		if (_code[i]._fixupType == FIXUP_IMPORT) {
			uint32 importId = _code[i]._data;
			if (importId >= _imports.size())
				error("invalid fixup import (at %d, for import %d)", i, importId);
		}
	}

	uint32 exportsCount = dta->readUint32LE();
	_exports.resize(exportsCount);
	for (uint i = 0; i < exportsCount; ++i) {
		_exports[i]._name = readString(dta);
		_exports[i]._address = dta->readUint32LE();
		debug(5, "script export '%s'", _exports[i]._name.c_str());
	}

	if (version >= 83) {
		uint32 sectionsCount = dta->readUint32LE();
		_sections.resize(sectionsCount);
		for (uint i = 0; i < sectionsCount; ++i) {
			_sections[i]._name = readString(dta);
			debug(5, "script section '%s'", _sections[i]._name.c_str());
			_sections[i]._offset = dta->readUint32LE();
		}
	}

	uint32 endsig = dta->readUint32LE();
	if (endsig != 0xbeefcafe)
		error("incorrect end signature %x for script", endsig);
}

ccInstance::ccInstance(AGSEngine *vm, ccScript *script, bool autoImport, ccInstance *fork) : _vm(vm), _script(script) {
	_flags = 0;

	if (fork) {
		// share memory space with an existing instance (ie. this is a thread/fork)
		_globalData = fork->_globalData;
		_flags |= INSTF_SHAREDATA;
	} else {
		// create our own memory space
		_globalData = new Common::Array<byte>(script->_globalData);
	}

	// FIXME: check all imports can be resolved?

	_pc = 0;
	_script->_instances++;
	if ((_script->_instances == 1) && autoImport) {
		// FIXME: import all the exported stuff from this script
	}
}

ccInstance::~ccInstance() {
	if (!(_flags & INSTF_SHAREDATA))
		delete _globalData;
}

bool ccInstance::exportsSymbol(const Common::String &name) {
	Common::String mangledName = name + '$';

	for (uint i = 0; i < _script->_exports.size(); ++i) {
		const ScriptExport &symbol = _script->_exports[i];

		if (symbol._name.hasPrefix(mangledName))
			return true;
		else if (symbol._name == name)
			return true;
	}

	return false;
}

void ccInstance::call(const Common::String &name, const Common::Array<uint32> &params) {
	if (params.size() >= 20)
		error("too many arguments %d to function '%s'", params.size(), name.c_str());

	if (_pc != 0)
		error("attempt to call() on a running instance, when calling function '%s'", name.c_str());

	Common::String mangledName = name + '$';
	uint32 codeLoc = 0xffffffff;
	for (uint i = 0; i < _script->_exports.size(); ++i) {
		const ScriptExport &symbol = _script->_exports[i];
		if (symbol._name.hasPrefix(mangledName)) {
			// mangled name
			uint32 paramCount = atoi(symbol._name.c_str() + mangledName.size());
			if (paramCount != params.size())
				error("tried calling function '%s' with %d parameters, but it takes %d",
					symbol._name.c_str(), params.size(), paramCount);
		} else if (symbol._name != name) {
			// not an unmangled name from old compiler, no match
			continue;
		}

		if (((symbol._address >> 24) & 0xff) != EXPORT_FUNCTION)
			error("attempt to call() '%s' which isn't a function", symbol._name.c_str());
		codeLoc = symbol._address & 0xffffff;
		break;
	}

	if (codeLoc == 0xffffffff) {
		error("attempt to call() function '%s' which doesn't exist", name.c_str());
	}

	debugN(2, "running function: '%s'@%d", name.c_str(), codeLoc);
	for (uint i = 0; i < params.size(); i++)
		debugN(2, " %d", params[i]);
	debug(2, " ");

	// vm setup

	// clear stack/registers
	_stack.clear();
	_stack.resize(CC_STACK_SIZE);
	for (uint i = 0; i < _stack.size(); ++i)
		_stack[i]._type = rvtInvalid;
	_registers.clear();
	_registers.resize(CC_NUM_REGISTERS);

	// initialize stack pointer
	_registers[SREG_SP]._type = rvtStackPointer;
	_registers[SREG_SP]._value = 0;

	// push parameters onto stack in reverse order
	for (uint i = params.size(); i > 0; ++i)
		pushValue(params[i - 1]);
	// push return address onto stack
	pushValue(0);

	runCodeFrom(codeLoc);

	// check the stack was left in a sane state
	if (_registers[SREG_SP]._type != rvtStackPointer)
		error("runCodeFrom(): SP got clobbered (now type %d, value %d)", _registers[SREG_SP]._type, _registers[SREG_SP]._value);
	if (_registers[SREG_SP]._value != params.size() * 4)
		error("call(): SP invalid at end of call (%d, for %d params)", _registers[SREG_SP]._value, params.size() * 4);

	_pc = 0;

	// FIXME: abort/free cleanup
}

enum InstArgumentType {
	iatAny, // anything
	iatInteger, // integer
	iatRegister, // valid register
	iatRegisterInt, // valid register containing integer
	iatRegisterFloat, // valid register containing float
	iatNone // nothing
};

struct InstructionInfo {
	const char *name;
	short numArgs;
	InstArgumentType arg1Type;
	InstArgumentType arg2Type;
};

#define NUM_INSTRUCTIONS SCMD_NEWARRAY
static InstructionInfo instructionInfo[NUM_INSTRUCTIONS + 1] = {
	{ "NULL", 0, iatNone, iatNone },
	{ "$add", 2, iatRegisterInt, iatInteger },
	{ "$sub", 2, iatRegisterInt, iatInteger },
	{ "$$mov", 2, iatRegister, iatRegister },
	{ "memwritelit", 2, iatInteger, iatAny },
	{ "ret", 0, iatNone, iatNone },
	{ "$mov", 2, iatRegister, iatAny },
	{ "$memread", 1, iatRegister, iatNone },
	{ "$memwrite", 1, iatRegister, iatNone },
	{ "$$mul", 2, iatRegisterInt, iatRegisterInt },
	{ "$$div", 2, iatRegisterInt, iatRegisterInt },
	{ "$$add", 2, iatRegisterInt, iatRegisterInt },
	{ "$$sub", 2, iatRegisterInt, iatRegisterInt },
	{ "$$bit_and", 2, iatRegisterInt, iatRegisterInt },
	{ "$$bit_or", 2, iatRegisterInt, iatRegisterInt },
	{ "$$cmp", 2, iatRegister, iatRegister },
	{ "$$ncmp", 2, iatRegister, iatRegister },
	{ "$$gt", 2, iatRegisterInt, iatRegisterInt },
	{ "$$lt", 2, iatRegisterInt, iatRegisterInt },
	{ "$$gte", 2, iatRegisterInt, iatRegisterInt },
	{ "$$lte", 2, iatRegisterInt, iatRegisterInt },
	{ "$$and", 2, iatRegisterInt, iatRegisterInt },
	{ "$$or", 2, iatRegisterInt, iatRegisterInt },
	{ "$call", 1, iatRegister, iatNone },
	{ "$memread.b", 1, iatRegister, iatNone },
	{ "$memread.w", 1, iatRegister, iatNone },
	{ "$memwrite.b", 1, iatRegister, iatNone },
	{ "$memwrite.w", 1, iatRegister, iatNone },
	{ "jz", 1, iatInteger, iatNone },
	{ "$push", 1, iatRegister, iatNone },
	{ "$pop", 1, iatRegister, iatNone },
	{ "jmp", 1, iatInteger, iatNone },
	{ "$mul", 2, iatRegister, iatInteger },
	{ "$farcall", 1, iatRegister, iatNone },
	{ "$farpush", 1, iatRegister, iatNone },
	{ "farsubsp", 1, iatInteger, iatNone },
	{ "sourceline", 1, iatInteger, iatNone },
	{ "$callscr", 1, iatAny, iatNone }, // TODO
	{ "thisaddr", 1, iatInteger, iatNone },
	{ "setfuncargs", 1, iatInteger, iatNone },
	{ "$$mod", 2, iatRegisterInt, iatRegisterInt },
	{ "$$xor", 2, iatRegisterInt, iatRegisterInt },
	{ "$not", 1, iatRegisterInt, iatRegisterInt },
	{ "$$shl", 2, iatRegisterInt, iatRegisterInt },
	{ "$$shr", 2, iatRegisterInt, iatRegisterInt },
	{ "$callobj", 1, iatAny, iatNone }, // TODO
	{ "$checkbounds", 2, iatRegisterInt, iatInteger },
	{ "$memwrite.ptr", 1, iatAny, iatNone }, // TODO
	{ "$memread.ptr", 1, iatAny, iatNone }, // TODO
	{ "memwrite.ptr.0", 0, iatNone, iatNone },
	{ "$meminit.ptr", 1, iatAny, iatNone }, // TODO
	{ "load.sp.offs", 1, iatAny, iatNone }, // TODO
	{ "checknull.ptr", 0, iatNone, iatNone },
	{ "$f.add", 2, iatRegisterFloat, iatRegisterFloat },
	{ "$f.sub", 2, iatRegisterFloat, iatRegisterFloat },
	{ "$$f.mul", 2, iatRegisterFloat, iatRegisterFloat },
	{ "$$f.div", 2, iatRegisterFloat, iatRegisterFloat },
	{ "$$f.add", 2, iatRegisterFloat, iatRegisterFloat },
	{ "$$f.sub", 2, iatRegisterFloat, iatRegisterFloat },
	{ "$$f.gt", 2, iatRegisterFloat, iatRegisterFloat },
	{ "$$f.lt", 2, iatRegisterFloat, iatRegisterFloat },
	{ "$$f.gte", 2, iatRegisterFloat, iatRegisterFloat },
	{ "$$f.lte", 2, iatRegisterFloat, iatRegisterFloat },
	{ "zeromem", 1, iatAny, iatNone }, // TODO
	{ "$newstring", 1, iatAny, iatNone }, // TODO
	{ "$$strcmp", 2, iatAny, iatAny }, // TODO
	{ "$$strnotcmp", 2, iatAny, iatAny }, // TODO
	{ "$checknull", 1, iatAny, iatNone }, // TODO
	{ "loopcheckoff", 0, iatNone, iatNone },
	{ "memwrite.ptr.0.nd", 0, iatNone, iatNone },
	{ "jnz", 1, iatInteger, iatNone },
	{ "$dynamicbounds", 1, iatAny, iatNone }, // TODO
	{ "$newarray", 3, iatAny, iatAny } // TODO
};

static const char *regnames[] = { "null", "sp", "mar", "ax", "bx", "cx", "op", "dx" };

#define MAX_FUNC_PARAMS 20 // maximum size of externalStack
#define MAXNEST 50 // number of recursive function calls allowed

void ccInstance::runCodeFrom(uint32 start) {
	assert(start < _script->_code.size());
	_pc = start;
	const Common::Array<ScriptCodeEntry> &code = _script->_code;

	// this allows scripts to disable the loop iteration sanity check
	uint32 loopIterationCheckDisabledCount = 0;

	Common::Stack<RuntimeValue> externalStack;
	bool nextCallNeedsObject = false;
	uint32 funcArgumentCount = 0xffffffff;

	Common::Stack<uint32> currentBase;
	currentBase.push(0);
	Common::Stack<uint32> currentStart;
	currentStart.push(_pc);

	while (true) {
		uint32 instruction = code[_pc]._data;
		if (instruction > NUM_INSTRUCTIONS)
			error("runCodeFrom(): invalid instruction %d", instruction);
		InstructionInfo &info = instructionInfo[instruction];

		uint32 neededArgs = info.numArgs;
		if (_pc + neededArgs >= code.size())
			error("runCodeFrom(): needed %d arguments for %s on line %d", neededArgs,
				info.name, _lineNumber);

		debugN(2, "%06d: %s", _pc, info.name);

		ScriptCodeEntry arg[2];
		RuntimeValue argVal[2];
		for (uint v = 0; v < neededArgs && v < 2; ++v) {
			arg[v] = code[_pc + 1 + v];

			// work out which argument type we're expecting
			InstArgumentType argType = info.arg1Type;
			if (v == 1)
				argType = info.arg2Type;

			argVal[v]._value = arg[v]._data;
			switch (arg[v]._fixupType) {
			case FIXUP_NONE:
				if (argType == iatRegister || argType == iatRegisterInt || argType == iatRegisterFloat)
					debugN(2, " %s", regnames[argVal[v]._value]);
				else
					debugN(2, " %d", argVal[v]._value);
				break;
			case FIXUP_GLOBALDATA:
				argVal[v]._type = rvtGlobalData;
				debugN(2, " data@%d", argVal[v]._value);
				break;
			case FIXUP_FUNCTION:
				argVal[v]._type = rvtFunction;
				debugN(2, " func@%d", argVal[v]._value);
				break;
			case FIXUP_STRING:
				argVal[v]._type = rvtString;
				debugN(2, " string@%d", argVal[v]._value);
				break;
			case FIXUP_IMPORT:
				argVal[v]._type = rvtImport;
				debugN(2, " import@%d:%s", argVal[v]._value, _script->_imports[argVal[v]._value].c_str());
				break;
			case FIXUP_STACK:
				argVal[v]._type = rvtStackPointer;
				debugN(2, " stack@%d", argVal[v]._value);
				break;
			case FIXUP_DATADATA:
			default:
				error("internal inconsistency (got fixup type %d)", arg[v]._fixupType);
			}

			// sanity-check the argument we got
			if (argType == iatAny)
				continue;
			if (argType == iatNone)
				error("internal inconsistency in argument type table");
			if (arg[v]._fixupType)
				error("expected integer for param %d of %s on line %d, got fixup (type %d)",
					v + 1, info.name, _lineNumber, arg[v]._fixupType);
			if (argType == iatRegister && arg[v]._data >= _registers.size())
				error("expected valid register for param %d of %s on line %d, got %d",
					v + 1, info.name, _lineNumber, arg[v]._data);
			// FIXME: check iatRegisterInt, iatRegisterFloat
		}
		debug(2, " ");

		const RuntimeValue &val1 = argVal[0], &val2 = argVal[1];
		int32 int1 = (int)argVal[0]._value, int2 = (int)argVal[1]._value;

		// temporary variables
		RuntimeValue tempVal;
		Common::Array<RuntimeValue> params;

		switch (instruction) {
		case SCMD_LINENUM:
			_lineNumber = int1;
			break;
		case SCMD_ADD:
			// reg1 += arg2
			_registers[int1]._value += int2;
			break;
		case SCMD_SUB:
			// reg1 -= arg2
			_registers[int1]._value -= int2;
			break;
		case SCMD_REGTOREG:
			// reg2 = reg1;
			_registers[int2] = _registers[int1];
			break;
		case SCMD_WRITELIT:
			// m[MAR] = arg2 (copy arg1 bytes)
			// "poss something dodgy about this routine"
			error("no WRITELIT yet");
			break;
		case SCMD_RET:
			// return from subroutine

			// only sabotage the sanity check until returning from the function which disabled it
			if (loopIterationCheckDisabledCount)
				loopIterationCheckDisabledCount--;

			// pop return address
			_pc = popIntValue();
			if (_pc == 0) {
				// FIXME: store SREG_AX?
				return;
			}
			// FIXME: set current instance?

			// continue so that the PC doesn't get overwritten
			continue;
		case SCMD_LITTOREG:
			// set reg1 to literal value arg2
			_registers[int1] = val2;
			break;
		case SCMD_MEMREAD:
			// reg1 = m[MAR]
			tempVal = _registers[SREG_MAR];
			switch (tempVal._type) {
			case rvtStackPointer:
				if (tempVal._value + 4 >= _stack.size())
					error("script tried to MEMREAD from out-of-bounds stack@%d on line %d",
						tempVal._value, _lineNumber);
				if (_stack[tempVal._value]._type == rvtInvalid)
					error("script tried to MEMREAD from invalid stack@%d on line %d",
						tempVal._value, _lineNumber);
				_registers[int1] = _stack[tempVal._value];
				break;
			default:
				error("script tried to MEMREAD from runtime value of type %d (value %d) on line %d",
					tempVal._type, tempVal._value, _lineNumber);
			}
			break;
		case SCMD_MEMWRITE:
			// m[MAR] = reg1
			// FIXME
			error("no MEMWRITE yet");
			break;
		case SCMD_LOADSPOFFS:
			// MAR = SP - arg1 (optimization for local var access)
			_registers[SREG_MAR] = _registers[SREG_SP];
			if ((uint32)int1 > _registers[SREG_SP]._value - 4)
				error("load.sp.offs tried going %d back in a stack of size %d on line %d",
					int1, _registers[SREG_SP]._value, _lineNumber);
			_registers[SREG_MAR]._value -= int1;
			break;
		case SCMD_MULREG:
			// reg1 *= reg2
			_registers[int1]._value *= _registers[int2]._value;
			break;
		case SCMD_DIVREG:
			// reg1 /= reg2
			if (_registers[int2]._value == 0)
				error("script tried to divide by zero on line %d", _lineNumber);
			_registers[int1]._value /= _registers[int2]._value;
			break;
		case SCMD_ADDREG:
			// reg1 += reg2
			_registers[int1]._value += _registers[int2]._value;
			break;
		case SCMD_SUBREG:
			// reg1 -= reg2
			_registers[int1]._value -= _registers[int2]._value;
			break;
		case SCMD_BITAND:
			// reg1 &= reg2
			_registers[int1]._value &= _registers[int2]._value;
			break;
		case SCMD_BITOR:
			// reg1 |= reg2
			_registers[int1]._value |= _registers[int2]._value;
			break;
		case SCMD_ISEQUAL:
			// reg1 == reg2   reg1=1 if true, =0 if not
			_registers[int1]._value = (_registers[int1]._value == _registers[int2]._value);
			break;
		case SCMD_NOTEQUAL:
			// reg1 != reg2
			_registers[int1]._value = (_registers[int1]._value == _registers[int2]._value);
			break;
		case SCMD_GREATER:
			// reg1 > reg2
			_registers[int1]._value = (_registers[int1]._value > _registers[int2]._value);
			break;
		case SCMD_LESSTHAN:
			// reg1 < reg2
			_registers[int1]._value = (_registers[int1]._value < _registers[int2]._value);
			break;
		case SCMD_GTE:
			// reg1 >= reg2
			_registers[int1]._value = (_registers[int1]._value >= _registers[int2]._value);
			break;
		case SCMD_LTE:
			// reg1 <= reg2
			_registers[int1]._value = (_registers[int1]._value <= _registers[int2]._value);
			break;
		case SCMD_AND:
			// (reg1!=0) && (reg2!=0) -> reg1
			_registers[int1]._value = (_registers[int1]._value && _registers[int2]._value);
			break;
		case SCMD_OR:
			// (reg1!=0) || (reg2!=0) -> reg1
			_registers[int1]._value = (_registers[int1]._value || _registers[int2]._value);
			break;
		case SCMD_XORREG:
			// reg1 ^= reg2
			_registers[int1]._value ^= _registers[int2]._value;
			break;
		case SCMD_MODREG:
			// reg1 %= reg2
			if (_registers[int2]._value == 0)
				error("script tried to divide (modulo) by zero on line %d", _lineNumber);
			_registers[int1]._value %= _registers[int2]._value;
			break;
		case SCMD_NOTREG:
			// reg1 = !reg1
			_registers[int1]._value = !_registers[int1]._value;
			break;
		case SCMD_CALL:
			// jump to subroutine at reg1
			if (_registers[int1]._type != rvtFunction)
				error("script tried to CALL non-function runtime value of type %d (value %d) on line %d",
					tempVal._type, tempVal._value, _lineNumber);

			if (currentStart.size() > MAXNEST - 1)
				error("too many nested script calls on line %d, infinite recursion?", _lineNumber);

			// FIXME: store call stack

			// push return value onto stack
			pushValue(_pc + neededArgs + 1);

			_pc = _registers[int1]._value;
			if (currentBase.top() != 0) {
				_pc += currentStart.top();
				_pc -= currentBase.top();
			}

			// FIXME: next call needs object?

			// sabotage the sanity check until returning from the function which disabled it
			if (loopIterationCheckDisabledCount)
				loopIterationCheckDisabledCount++;

			currentBase.push(0);
			currentStart.push(_pc);
			break;
		case SCMD_MEMREADB:
			// reg1 = m[MAR] (1 byte)
			error("unimplemented %s", info.name);
			break;
		case SCMD_MEMREADW:
			// reg1 = m[MAR] (2 bytes)
			error("unimplemented %s", info.name);
			break;
		case SCMD_MEMWRITEB:
			// m[MAR] = reg1 (1 byte)
			error("unimplemented %s", info.name);
			break;
		case SCMD_MEMWRITEW:
			// m[MAR] = reg1 (2 bytes)
			error("unimplemented %s", info.name);
			break;
		case SCMD_JZ:
			// jump if ax==0 by arg1
			if (_registers[SREG_AX]._value == 0)
				_pc += int1;
			break;
		case SCMD_JNZ:
			// jump by arg1 if ax!=0
			if (_registers[SREG_AX]._value != 0)
				_pc += int1;
			break;
		case SCMD_PUSHREG:
			// m[sp]=reg1; sp++
			pushValue(_registers[int1]);
			break;
		case SCMD_POPREG:
			// sp--; reg1=m[sp]
			_registers[int1] = popValue();
			break;
		case SCMD_JMP:
			_pc += int1;
			// check whether the script is stuck in a loop
			if (loopIterationCheckDisabledCount)
				continue;
			if (int1 > 0)
				continue;
			// FIXME: make sure the script isn't stuck in a loop
			break;
		case SCMD_MUL:
		case SCMD_CHECKBOUNDS:
		case SCMD_DYNAMICBOUNDS:
		case SCMD_MEMREADPTR:
		case SCMD_MEMWRITEPTR:
		case SCMD_MEMINITPTR:
		case SCMD_MEMZEROPTR:
		case SCMD_MEMZEROPTRND:
		case SCMD_CHECKNULL:
		case SCMD_CHECKNULLREG:
			// FIXME
			error("unimplemented %s", info.name);
			break;
		case SCMD_NUMFUNCARGS:
			// setfuncargs: number of arguments for ext func call
			funcArgumentCount = int1;
			break;
		case SCMD_CALLAS:
			// $callscr: call external script function
			// FIXME
			error("unimplemented %s", info.name);
			break;
		case SCMD_CALLEXT:
			// farcall: call external (imported) function reg1
			if (_registers[int1]._type != rvtImport)
				error("script tried to CALLEXT non-import runtime value of type %d (value %d) on line %d",
					tempVal._type, tempVal._value, _lineNumber);

			if (funcArgumentCount == 0xffffffff)
				funcArgumentCount = externalStack.size();

			// construct the parameter list (in reverse order)
			params.resize(funcArgumentCount);
			for (uint i = 0; i < funcArgumentCount; ++i)
				params[i] = externalStack[externalStack.size() - i - 1];

			if (nextCallNeedsObject) {
				// FIXME
				error("don't support object calls yet");
			} else {
				_registers[SREG_AX] = callImportedFunction(_registers[int1]._value, params);
			}

			// FIXME: unfinished
			funcArgumentCount = 0xffffffff;
			break;
		case SCMD_PUSHREAL:
			// farpush: push reg1 onto real stack
			if (externalStack.size() >= MAX_FUNC_PARAMS)
				error("external call stack overflow at line %d", _lineNumber);
			externalStack.push(_registers[int1]);
			break;
		case SCMD_SUBREALSTACK:
			// farsubsp
			// FIXME: CALLAS handling
			for (uint i = 0; i < int1; ++i)
				externalStack.pop();
			break;
		case SCMD_CALLOBJ:
			// $callobj: next call is member function of reg1
			// FIXME
			error("unimplemented %s", info.name);
			break;
		case SCMD_SHIFTLEFT:
			// reg1 = reg1 << reg2
			// FIXME
			error("unimplemented %s", info.name);
			break;
		case SCMD_SHIFTRIGHT:
			// reg1 = reg1 >> reg2
			// FIXME
			error("unimplemented %s", info.name);
			break;
		case SCMD_THISBASE:
			// thisaddr: current relative address
			currentBase.pop();
			currentBase.push(int1);
			break;
		case SCMD_NEWARRAY:
		case SCMD_FADD:
		case SCMD_FSUB:
		case SCMD_FMULREG:
		case SCMD_FDIVREG:
		case SCMD_FADDREG:
		case SCMD_FSUBREG:
		case SCMD_FGREATER:
		case SCMD_FLESSTHAN:
		case SCMD_FGTE:
		case SCMD_FLTE:
		case SCMD_ZEROMEMORY:
		case SCMD_CREATESTRING:
		case SCMD_STRINGSEQUAL:
		case SCMD_STRINGSNOTEQ:
			// FIXME
			error("unimplemented %s", info.name);
			break;
		case SCMD_LOOPCHECKOFF:
			// no loop checking for this function
			if (loopIterationCheckDisabledCount == 0)
				loopIterationCheckDisabledCount++;
			break;
		default:
			// FIXME
			warning("runCodeFrom(): invalid instruction %d", instruction);
		}

		if (_registers[SREG_SP]._type != rvtStackPointer || _registers[SREG_SP]._value < 4 || _registers[SREG_SP]._value >= _stack.size())
			error("runCodeFrom(): SP got clobbered (now type %d, value %d) on line %d",
				_registers[SREG_SP]._type, _registers[SREG_SP]._value, _lineNumber);

		// increment to next instruction, skipping any arguments
		_pc += (1 + neededArgs);
	}
}

RuntimeValue ccInstance::callImportedFunction(uint32 importId, const Common::Array<RuntimeValue> &params) {
	assert(importId < _script->_imports.size());

	Common::String name = _script->_imports[importId];
	debug(2, "trying to call import '%s'", name.c_str());

	// FIXME: actually do the call
	return RuntimeValue();
}

void ccInstance::pushValue(const RuntimeValue &value) {
	assert(_registers[SREG_SP]._type == rvtStackPointer);

	uint32 stackValue = _registers[SREG_SP]._value;
	if (stackValue + 4 > _stack.size())
		error("script caused VM stack overflow");

	_stack[stackValue] = value;
	_stack[stackValue + 1]._type = rvtInvalid;
	_stack[stackValue + 2]._type = rvtInvalid;
	_stack[stackValue + 3]._type = rvtInvalid;

	_registers[SREG_SP]._value += 4;
}

RuntimeValue ccInstance::popValue() {
	assert(_registers[SREG_SP]._type == rvtStackPointer && _registers[SREG_SP]._value >= 4);

	_registers[SREG_SP]._value -= 4;
	uint32 stackValue = _registers[SREG_SP]._value;
	if (stackValue + 4 > _stack.size())
		error("script caused VM stack underflow(?!) on line %d", _lineNumber);
	if (_stack[stackValue]._type == rvtInvalid)
		error("script tried to pop invalid value from stack on line %d", _lineNumber);

	return _stack[stackValue];
}

uint32 ccInstance::popIntValue() {
	RuntimeValue val = popValue();

	if (val._type != rvtInteger)
		error("expected to pop an integer value off the stack (got type %d) on line %d", val._type, _lineNumber);
	return val._value;
}

} // End of namespace AGS
