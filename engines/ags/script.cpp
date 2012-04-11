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
 * You may also modify/distribute the code in this file under that license.
 */

#include "engines/ags/ags.h"
#include "engines/ags/script.h"
#include "engines/ags/scripting/scripting.h"
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

		if (fixupTypes[i] == FIXUP_DATADATA) {
			// patch to global data
			// (usually strings, there aren't many of these)
			_globalFixups.push_back(fixupIndex);
		} else if (fixupTypes[i] && fixupTypes[i] <= 6) {
			// patch to code
			if (fixupIndex >= _code.size())
				error("fixup for %d is beyond code size %d", fixupIndex, _code.size());
			if (_code[fixupIndex]._fixupType)
				error("duplicate fixup (index %d, old type %d, new type %d)", fixupIndex,
					_code[fixupIndex]._fixupType, fixupTypes[i]);
			_code[fixupIndex]._fixupType = fixupTypes[i];

			// do some in-range sanity checks (note that imports are handled below)
			uint32 data = _code[fixupIndex]._data;
			if ((fixupTypes[i] == FIXUP_GLOBALDATA) && (data >= _globalData.size()))
				error("out-of-range global data fixup (at %d, for data at %d)", fixupIndex, data);
			else if ((fixupTypes[i] == FIXUP_STRING) && (data >= _strings.size()))
				error("out-of-range string fixup (at %d, for data at %d)", fixupIndex, data);
		} else {
			// invalid/unknown type
			error("invalid fixup type %d", fixupTypes[i]);
		}
	}
	Common::sort(_globalFixups.begin(), _globalFixups.end());

	debug(3, "script has %d fixups for %d code entries", fixupsCount, codeSize);

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
		uint32 exportAddress = dta->readUint32LE();
		// high byte is type
		_exports[i]._address = exportAddress & 0xffffff;
		byte exportType = (exportAddress >> 24) & 0xff;
		switch (exportType) {
		case EXPORT_FUNCTION:
			debug(5, "script function export '%s'", _exports[i]._name.c_str());
			if (_exports[i]._address >= _code.size())
				error("out-of-range script function export '%s' (%d)", _exports[i]._name.c_str(), _exports[i]._address);
			_exports[i]._type = sitScriptFunction;
			break;
		case EXPORT_DATA:
			debug(5, "script data export '%s'", _exports[i]._name.c_str());
			if (_exports[i]._address >= _globalData.size())
				error("out-of-range script data export '%s' (%d)", _exports[i]._name.c_str(), _exports[i]._address);
			_exports[i]._type = sitScriptData;
			break;
		default:
			error("script export '%s' had unknown type %d", _exports[i]._name.c_str(), exportType);
		}
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
		_globalObjects = fork->_globalObjects;
		_flags |= INSTF_SHAREDATA;
	} else {
		// create our own memory space
		_globalData = new Common::Array<byte>(script->_globalData);
		_globalObjects = new Common::HashMap<uint32, RuntimeValue>();
	}

	// resolve all the imports
	_resolvedImports.resize(script->_imports.size());
	for (uint i = 0; i < script->_imports.size(); ++i) {
		_resolvedImports[i] = _vm->resolveImport(script->_imports[i]);
	}

	_pc = 0;
	_script->_instances++;
	if ((_script->_instances == 1) && autoImport) {
		// import all the exported stuff from this script
		GlobalScriptState *state = _vm->getScriptState();
		for (uint i = 0; i < _script->_exports.size(); ++i) {
			ScriptImport import;

			import._type = _script->_exports[i]._type;
			import._owner = this;
			import._offset = _script->_exports[i]._address;

			state->addImport(_script->_exports[i]._name, import);

			if (!_script->_exports[i]._name.contains('$'))
				continue;

			// if the name is mangled, we also want to export the non-mangled version
			Common::String mangledName = _script->_exports[i]._name;
			while (mangledName[mangledName.size() - 1] != '$')
				mangledName.deleteLastChar();
			mangledName.deleteLastChar();

			state->addImport(mangledName, import);
		}
	}

	_registers.resize(CC_NUM_REGISTERS);
	_stack.resize(CC_STACK_SIZE);
	for (uint i = 0; i < _stack.size(); ++i)
		_stack[i].invalidate();
}

ccInstance::~ccInstance() {
	_script->_instances--;
	if (_script->_instances == 0) {
		// FIXME: must make sure that nothing is referencing these!

		GlobalScriptState *state = _vm->getScriptState();
		for (uint i = 0; i < _script->_exports.size(); ++i) {
			// remove import if it came from us
			ScriptImport im = _vm->resolveImport(_script->_exports[i]._name, false);
			if (im._owner == this)
				state->removeImport(_script->_exports[i]._name);

			if (!_script->_exports[i]._name.contains('$'))
				continue;

			// see comment in constructor
			Common::String mangledName = _script->_exports[i]._name;
			while (mangledName[mangledName.size() - 1] != '$')
				mangledName.deleteLastChar();
			mangledName.deleteLastChar();

			// remove mangled import if it came from us
			im = _vm->resolveImport(mangledName, false);
			if (im._owner == this)
				state->removeImport(mangledName);
		}
	}

	if (!(_flags & INSTF_SHAREDATA)) {
		delete _globalData;
		delete _globalObjects;
	}
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

void ccInstance::call(const Common::String &name, const Common::Array<RuntimeValue> &params) {
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

		if (symbol._type != sitScriptFunction)
			error("attempt to call() '%s' which isn't a function", symbol._name.c_str());
		codeLoc = symbol._address;
		break;
	}

	if (codeLoc == 0xffffffff) {
		error("attempt to call() function '%s' which doesn't exist", name.c_str());
	}

	debugN(2, "running function: '%s'@%d", name.c_str(), codeLoc);
	//for (uint i = 0; i < params.size(); i++)
	//	debugN(2, " %d", params[i]);
	debug(2, " ");

	// vm setup

	// clear stack/registers
	//_stack.clear();
	//_stack.resize(CC_STACK_SIZE);
	//for (uint i = 0; i < _stack.size(); ++i)
	//	_stack[i]._type = rvtInvalid;
	//_registers.clear();
	//_registers.resize(CC_NUM_REGISTERS);

	// object pointer needs to start zeroed
	_registers[SREG_OP] = 0;

	// initialize stack pointer
	_registers[SREG_SP] = 0;
	_registers[SREG_SP]._type = rvtStackPointer;

	// push parameters onto stack in reverse order
	for (uint i = params.size(); i > 0; --i)
		pushValue(params[i - 1]);
	// push return address onto stack
	pushValue(0);

	_runningInst = this;
	runCodeFrom(codeLoc);

	// check the stack was left in a sane state
	if (_registers[SREG_SP]._type != rvtStackPointer)
		error("runCodeFrom(): SP got clobbered (now type %d, value %d)", _registers[SREG_SP]._type, _registers[SREG_SP]._value);
	if (_registers[SREG_SP]._value != params.size() * 4)
		error("call(): SP invalid at end of call (%d, for %d params)", _registers[SREG_SP]._value, params.size() * 4);

	_pc = 0;

	// FIXME: abort/free cleanup
}

uint32 ccInstance::getReturnValue() {
	if (_returnValue._type != rvtInteger)
		error("getReturnValue(): last return value was of type %d, not integer", _returnValue._type);

	return _returnValue._value;
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
	{ "$callscr", 1, iatRegister, iatNone },
	{ "thisaddr", 1, iatInteger, iatNone },
	{ "setfuncargs", 1, iatInteger, iatNone },
	{ "$$mod", 2, iatRegisterInt, iatRegisterInt },
	{ "$$xor", 2, iatRegisterInt, iatRegisterInt },
	{ "$not", 1, iatRegisterInt, iatRegisterInt },
	{ "$$shl", 2, iatRegisterInt, iatRegisterInt },
	{ "$$shr", 2, iatRegisterInt, iatRegisterInt },
	{ "$callobj", 1, iatRegister, iatNone },
	{ "$checkbounds", 2, iatRegisterInt, iatInteger },
	{ "$memwrite.ptr", 1, iatRegister, iatNone },
	{ "$memread.ptr", 1, iatRegister, iatNone },
	{ "memwrite.ptr.0", 0, iatNone, iatNone },
	{ "$meminit.ptr", 1, iatRegister, iatNone },
	{ "load.sp.offs", 1, iatInteger, iatNone },
	{ "checknull.ptr", 0, iatNone, iatNone },
	{ "$f.add", 2, iatRegisterFloat, iatRegisterInt },
	{ "$f.sub", 2, iatRegisterFloat, iatRegisterInt },
	{ "$$f.mul", 2, iatRegisterFloat, iatRegisterFloat },
	{ "$$f.div", 2, iatRegisterFloat, iatRegisterFloat },
	{ "$$f.add", 2, iatRegisterFloat, iatRegisterFloat },
	{ "$$f.sub", 2, iatRegisterFloat, iatRegisterFloat },
	{ "$$f.gt", 2, iatRegisterFloat, iatRegisterFloat },
	{ "$$f.lt", 2, iatRegisterFloat, iatRegisterFloat },
	{ "$$f.gte", 2, iatRegisterFloat, iatRegisterFloat },
	{ "$$f.lte", 2, iatRegisterFloat, iatRegisterFloat },
	{ "zeromem", 1, iatInteger, iatNone },
	{ "$newstring", 1, iatRegister, iatNone },
	{ "$$strcmp", 2, iatRegister, iatRegister },
	{ "$$strnotcmp", 2, iatRegister, iatRegister },
	{ "$checknull", 1, iatRegister, iatNone },
	{ "loopcheckoff", 0, iatNone, iatNone },
	{ "memwrite.ptr.0.nd", 0, iatNone, iatNone },
	{ "jnz", 1, iatInteger, iatNone },
	{ "$dynamicbounds", 1, iatRegisterInt, iatNone },
	{ "$newarray", 3, iatRegisterInt, iatInteger }
};

static const char *regnames[] = { "null", "sp", "mar", "ax", "bx", "cx", "op", "dx" };

#define MAX_FUNC_PARAMS 20 // maximum size of externalStack
#define MAXNEST 50 // number of recursive function calls allowed

void ccInstance::runCodeFrom(uint32 start) {
	ccInstance *inst = _runningInst;
	ccScript *script = inst->_script;

	_returnValue = (uint)-1;

	assert(start < script->_code.size());
	_pc = start;
	const Common::Array<ScriptCodeEntry> &code = script->_code;

	// this allows scripts to disable the loop iteration sanity check
	uint32 loopIterationCheckDisabledCount = 0;

	Common::Stack<RuntimeValue> externalStack;
	bool nextCallNeedsObject = false;
	bool recoverFromCallAs = false;
	uint32 funcArgumentCount = (uint)-1;

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

		debugN(4, "%06d: %s", _pc, info.name);

		ScriptCodeEntry arg[2];
		RuntimeValue argVal[2];
		for (uint v = 0; v < neededArgs && v < 2; ++v) {
			arg[v] = code[_pc + 1 + v];

			// work out which argument type we're expecting
			InstArgumentType argType = info.arg1Type;
			if (v == 1)
				argType = info.arg2Type;

			argVal[v]._value = arg[v]._data;
			uint32 argValue = arg[v]._data;
			switch (arg[v]._fixupType) {
			case FIXUP_NONE:
				if (argType == iatRegister || argType == iatRegisterInt || argType == iatRegisterFloat)
					debugN(4, " %s", regnames[argValue]);
				else
					debugN(4, " %d", argValue);
				break;
			case FIXUP_GLOBALDATA:
				argVal[v]._type = rvtScriptData;
				argVal[v]._instance = inst;
				debugN(4, " data@%d", argValue);
				// (note that you can't apply the fixup here, since we don't know the offset yet)
				break;
			case FIXUP_FUNCTION:
				argVal[v]._type = rvtFunction;
				debugN(4, " func@%d", argValue);
				break;
			case FIXUP_STRING:
				argVal[v] = new ScriptConstString(Common::String((const char *)&script->_strings[argValue]));
				argVal[v]._object->DecRef();
				debugN(4, " string@%d\"%s\"", argValue, &script->_strings[argValue]);
				break;
			case FIXUP_IMPORT:
				switch (inst->_resolvedImports[argValue]._type) {
				case sitSystemFunction:
					argVal[v]._type = rvtSystemFunction;
					argVal[v]._function = inst->_resolvedImports[argValue]._function;
					// preserve the import index for debugging purposes
					argVal[v]._value = argValue;
					break;
				case sitSystemObject:
					argVal[v] = inst->_resolvedImports[argValue]._object;
					break;
				case sitScriptFunction:
					argVal[v]._type = rvtScriptFunction;
					argVal[v]._value = inst->_resolvedImports[argValue]._offset;
					argVal[v]._instance = inst->_resolvedImports[argValue]._owner;

					// if the next instruction is a system call instruction (CALLEXT), change it to a script one instead
					// TODO: this is what the original engine does, but can't we fix it up in CALLEXT instead?
					if (code[_pc + v + 2]._data == SCMD_CALLEXT)
						script->_code[_pc + v + 2]._data = SCMD_CALLAS;
					break;
				case sitScriptData:
					argVal[v]._type = rvtScriptData;
					argVal[v]._value = inst->_resolvedImports[argValue]._offset;
					argVal[v]._instance = inst->_resolvedImports[argValue]._owner;
					break;
				default:
					error("internal inconsistency (got import fixup with import type %d)", inst->_resolvedImports[argValue]._type);
				}
				debugN(4, " import@%d:%s", argValue, script->_imports[argValue].c_str());
				break;
			case FIXUP_STACK:
				argVal[v]._type = rvtStackPointer;
				debugN(4, " stack@%d", argValue);
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
		debug(4, " ");

		const RuntimeValue &val2 = argVal[1];
		int32 int1 = (int)argVal[0]._value, int2 = (int)argVal[1]._value;

		// temporary variables
		RuntimeValue tempVal;
		ScriptObject *tempObj;
		ScriptString *tempStr1, *tempStr2;
		uint32 *fixup;
		ccScript *instScript;
		Common::Array<RuntimeValue> params;

		switch (instruction) {
		case SCMD_LINENUM:
			// debug info - source code line number
			_lineNumber = int1;
			break;
		case SCMD_ADD:
			// reg1 += arg2
			_registers[int1]._signedValue += int2;
			break;
		case SCMD_SUB:
			// reg1 -= arg2
			_registers[int1]._signedValue -= int2;
			break;
		case SCMD_REGTOREG:
			// reg2 = reg1;
			_registers[int2] = _registers[int1];
			break;
		case SCMD_WRITELIT:
			// m[MAR] = arg2 (copy arg1 bytes)
			// "poss something dodgy about this routine"
			// But, conveniently, this should only ever be used to write 4-byte null pointers.
			// (Or apparently, in older versions, to invalidate stack variables?)
			if (_registers[SREG_MAR]._type == rvtStackPointer) {
				if (int1 > 4 || int2 != 0)
					error("script tried using WRITELIT unsafely on stack (%d bytes of %d) on line %d",
						int1, int2, _lineNumber);
				// FIXME: check bounds
				_stack[_registers[SREG_MAR]._value] = 0;
				// TODO: good?
				for (uint i = 1; i < (uint)int1; ++i)
					_stack[i + _registers[SREG_MAR]._value].invalidate();
				break;
			}
			if (int1 != 4 || int2 != 0)
				error("script tried using WRITELIT unsafely (%d bytes of %d) on line %d",
					int1, int2, _lineNumber);
			writePointer(_registers[SREG_MAR], NULL);
			break;
		case SCMD_RET:
			// return from subroutine

			// only sabotage the sanity check until returning from the function which disabled it
			if (loopIterationCheckDisabledCount)
				loopIterationCheckDisabledCount--;

			// pop return address
			_pc = popIntValue();
			if (_pc == 0) {
				debug(4, "(returning to caller)");
				_returnValue = _registers[SREG_AX];
				return;
			}
			// TODO: set current instance?

			currentBase.pop();
			currentStart.pop();
			// TODO: restore call stack (line number etc)

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
			case rvtScriptData:
				// FIXME: bounds checks
				instScript = tempVal._instance->_script;
				if (tempVal._instance->_globalObjects->contains(tempVal._value)) {
					// resolves to an object
					_registers[int1] = (*tempVal._instance->_globalObjects)[tempVal._value];
					break;
				}
				fixup = Common::find(instScript->_globalFixups.begin(), instScript->_globalFixups.end(), tempVal._value);
				_registers[int1] = READ_LE_UINT32(&(*tempVal._instance->_globalData)[tempVal._value]);
				if (fixup != instScript->_globalFixups.end()) {
					// this resolves to another offset!
					_registers[int1].invalidate();
					_registers[int1]._type = rvtScriptData;
					_registers[int1]._instance = tempVal._instance;
				}
				break;
			case rvtSystemObject:
				_registers[int1] = tempVal._object->readUint32(tempVal._value);
				break;
			case rvtStackPointer:
				if (tempVal._value + 4 >= _stack.size())
					error("script tried to MEMREAD from out-of-bounds stack@%d on line %d",
						tempVal._value, _lineNumber);
				if (_stack[tempVal._value]._type == rvtInvalid)
					error("script tried to MEMREAD from invalid stack@%d on line %d",
						tempVal._value, _lineNumber);
				// TODO: make sure the other stack entries didn't get prodded at in the meantime
				_registers[int1] = _stack[tempVal._value];
				break;
			default:
				error("script tried to MEMREAD from runtime value of type %d (value %d) on line %d",
					tempVal._type, tempVal._value, _lineNumber);
			}
			break;
		case SCMD_MEMWRITE:
			// m[MAR] = reg1
			tempVal = _registers[SREG_MAR];
			switch (tempVal._type) {
			case rvtScriptData:
				// FIXME: bounds checks
				instScript = tempVal._instance->_script;
				fixup = Common::find(instScript->_globalFixups.begin(), instScript->_globalFixups.end(), tempVal._value);
				if (fixup != instScript->_globalFixups.end())
					error("script tried to MEMWRITE script data at %d with a fixup on line %d",
						tempVal._value, _lineNumber);
				if (_registers[int1]._type == rvtSystemObject) {
					// writing an object to script global data
					WRITE_LE_UINT32(&(*tempVal._instance->_globalData)[tempVal._value], 0);
					(*tempVal._instance->_globalObjects)[tempVal._value] = _registers[int1];
					break;
				}
				// FIXME: agh, float horror
				if (_registers[int1]._type != rvtInteger && _registers[int1]._type != rvtFloat)
					error("script tried to MEMWRITE runtime value of type %d (value %d) on line %d",
						_registers[int1]._type, _registers[int1]._value, _lineNumber);
				if (tempVal._instance->_globalObjects->contains(tempVal._value))
					tempVal._instance->_globalObjects->erase(tempVal._value);
				WRITE_LE_UINT32(&(*tempVal._instance->_globalData)[tempVal._value], _registers[int1]._value);
				break;
			case rvtSystemObject:
				if (_registers[int1]._type != rvtInteger)
					error("script tried to MEMWRITE runtime value of type %d (value %d) on line %d",
						_registers[int1]._type, _registers[int1]._value, _lineNumber);
				if (!tempVal._object->writeUint32(tempVal._value, _registers[int1]._value))
					error("script failed to write uint32 to offset %d of a %s on line %d",
						tempVal._value, tempVal._object->getObjectTypeName(), _lineNumber);
				break;
			case rvtStackPointer:
				if (tempVal._value + 4 >= _stack.size())
					error("script tried to MEMWRITE to out-of-bounds stack@%d on line %d",
						tempVal._value, _lineNumber);
				_stack[tempVal._value] = _registers[int1];
				_stack[tempVal._value + 1].invalidate();
				_stack[tempVal._value + 2].invalidate();
				_stack[tempVal._value + 3].invalidate();
				break;
			default:
				error("script tried to MEMWRITE to runtime value of type %d (value %d) on line %d",
					tempVal._type, tempVal._value, _lineNumber);
			}
			break;
		case SCMD_LOADSPOFFS:
			// MAR = SP - arg1 (optimization for local var access)
			_registers[SREG_MAR] = _registers[SREG_SP];
			if ((uint32)int1 > _registers[SREG_SP]._value)
				error("load.sp.offs tried going %d back in a stack of size %d on line %d",
					int1, _registers[SREG_SP]._value, _lineNumber);
			_registers[SREG_MAR]._value -= int1;
			break;
		case SCMD_MULREG:
			// reg1 *= reg2
			_registers[int1]._signedValue *= _registers[int2]._signedValue;
			break;
		case SCMD_DIVREG:
			// reg1 /= reg2
			if (_registers[int2]._signedValue == 0)
				error("script tried to divide by zero on line %d", _lineNumber);
			_registers[int1]._signedValue /= _registers[int2]._signedValue;
			break;
		case SCMD_ADDREG:
			// reg1 += reg2
			_registers[int1]._signedValue += _registers[int2]._signedValue;
			break;
		case SCMD_SUBREG:
			// reg1 -= reg2
			_registers[int1]._signedValue -= _registers[int2]._signedValue;
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
			_registers[int1] = _registers[int1].equalTo(_registers[int2]) ? 1 : 0;
			break;
		case SCMD_NOTEQUAL:
			// reg1 != reg2
			_registers[int1] = _registers[int1].equalTo(_registers[int2]) ? 0 : 1;
			break;
		case SCMD_GREATER:
			// reg1 > reg2
			_registers[int1] = (_registers[int1]._signedValue > _registers[int2]._signedValue) ? 1 : 0;
			break;
		case SCMD_LESSTHAN:
			// reg1 < reg2
			_registers[int1] = (_registers[int1]._signedValue < _registers[int2]._signedValue) ? 1 : 0;
			break;
		case SCMD_GTE:
			// reg1 >= reg2
			_registers[int1] = (_registers[int1]._signedValue >= _registers[int2]._signedValue) ? 1 : 0;
			break;
		case SCMD_LTE:
			// reg1 <= reg2
			_registers[int1] = (_registers[int1]._signedValue <= _registers[int2]._signedValue) ? 1 : 0;
			break;
		case SCMD_AND:
			// (reg1!=0) && (reg2!=0) -> reg1
			_registers[int1] = (_registers[int1]._value && _registers[int2]._value) ? 1 : 0;
			break;
		case SCMD_OR:
			// (reg1!=0) || (reg2!=0) -> reg1
			_registers[int1] = (_registers[int1]._value || _registers[int2]._value) ? 1 : 0;
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

			// TODO: store call stack (line number etc)

			// push return value onto stack
			pushValue(_pc + neededArgs + 1);

			_pc = _registers[int1]._value;
			if (currentBase.top() != 0) {
				_pc += currentStart.top();
				_pc -= currentBase.top();
			}

			// (note: scripts will explode on CALLEXT if this isn't here)
			nextCallNeedsObject = false;

			// sabotage the sanity check until returning from the function which disabled it
			if (loopIterationCheckDisabledCount)
				loopIterationCheckDisabledCount++;

			currentBase.push(0);
			currentStart.push(_pc);
			break;
		case SCMD_MEMREADB:
			// reg1 = m[MAR] (1 byte)
			tempVal = _registers[SREG_MAR];
			// FIXME: check range?
			switch (tempVal._type) {
			case rvtScriptData:
				// FIXME: bounds checks
				instScript = tempVal._instance->_script;
				if (tempVal._instance->_globalObjects->contains(tempVal._value))
					error("script tried MEMREADB on object on line %d", _lineNumber);
				fixup = Common::find(instScript->_globalFixups.begin(), instScript->_globalFixups.end(), tempVal._value);
				if (fixup != instScript->_globalFixups.end())
					error("script tried MEMREADB on fixup on line %d", _lineNumber);
				_registers[int1] = (byte)(*tempVal._instance->_globalData)[tempVal._value];
				break;
			case rvtSystemObject:
				_registers[int1] = tempVal._object->readByte(tempVal._value);
				break;
			case rvtStackPointer:
				if (tempVal._value + 2 >= _stack.size())
					error("script tried to MEMREADB from out-of-bounds stack@%d on line %d",
						tempVal._value, _lineNumber);
				if (_stack[tempVal._value]._type != rvtInteger)
					error("script tried to MEMREADB from invalid stack@%d on line %d",
						tempVal._value, _lineNumber);
				_registers[int1] = (byte)_stack[tempVal._value]._value;
				break;
			default:
				error("script tried to MEMREADB from runtime value of type %d (value %d) on line %d",
					tempVal._type, tempVal._value, _lineNumber);
			}
			break;
		case SCMD_MEMREADW:
			// reg1 = m[MAR] (2 bytes)
			tempVal = _registers[SREG_MAR];
			// FIXME: check range?
			switch (tempVal._type) {
			case rvtScriptData:
				// FIXME: bounds checks
				instScript = tempVal._instance->_script;
				if (tempVal._instance->_globalObjects->contains(tempVal._value))
					error("script tried MEMREADW on object on line %d", _lineNumber);
				fixup = Common::find(instScript->_globalFixups.begin(), instScript->_globalFixups.end(), tempVal._value);
				if (fixup != instScript->_globalFixups.end())
					error("script tried MEMREADW on fixup on line %d", _lineNumber);
				_registers[int1] = (int16)READ_LE_UINT16(&(*tempVal._instance->_globalData)[tempVal._value]);
				break;
			case rvtSystemObject:
				_registers[int1] = (int16)tempVal._object->readUint16(tempVal._value);
				break;
			case rvtStackPointer:
				if (tempVal._value + 2 >= _stack.size())
					error("script tried to MEMREADW from out-of-bounds stack@%d on line %d",
						tempVal._value, _lineNumber);
				if (_stack[tempVal._value]._type != rvtInteger)
					error("script tried to MEMREADW from invalid stack@%d on line %d",
						tempVal._value, _lineNumber);
				_registers[int1] = (int16)_stack[tempVal._value]._value;
				break;
			default:
				error("script tried to MEMREADW from runtime value of type %d (value %d) on line %d",
					tempVal._type, tempVal._value, _lineNumber);
			}
			break;
		case SCMD_MEMWRITEB:
			// m[MAR] = reg1 (1 byte)
			tempVal = _registers[SREG_MAR];
			if (_registers[int1]._type != rvtInteger)
				error("script tried to MEMWRITEB runtime value of type %d (value %d) on line %d",
					_registers[int1]._type, _registers[int1]._value, _lineNumber);
			// FIXME: check range?
			switch (tempVal._type) {
			case rvtScriptData:
				// FIXME: bounds checks
				instScript = tempVal._instance->_script;
				fixup = Common::find(instScript->_globalFixups.begin(), instScript->_globalFixups.end(), tempVal._value);
				if (fixup != instScript->_globalFixups.end())
					error("script tried MEMWRITEB on fixup on %d", _lineNumber);
				if (tempVal._instance->_globalObjects->contains(tempVal._value))
					tempVal._instance->_globalObjects->erase(tempVal._value);
				(*tempVal._instance->_globalData)[tempVal._value] = (byte)_registers[int1]._value;
				break;
			case rvtSystemObject:
				tempVal._object->writeByte(tempVal._value, (byte)_registers[int1]._value);
				break;
			case rvtStackPointer:
				if (tempVal._value + 1 >= _stack.size())
					error("script tried to MEMWRITEB to out-of-bounds stack@%d on line %d",
						tempVal._value, _lineNumber);
				_stack[tempVal._value] = (byte)_registers[int1]._value;
				break;
			default:
				error("script tried to MEMWRITEB to runtime value of type %d (value %d) on line %d",
					tempVal._type, tempVal._value, _lineNumber);
			}
			break;
		case SCMD_MEMWRITEW:
			// m[MAR] = reg1 (2 bytes)
			tempVal = _registers[SREG_MAR];
			if (_registers[int1]._type != rvtInteger)
				error("script tried to MEMWRITEW runtime value of type %d (value %d) on line %d",
					_registers[int1]._type, _registers[int1]._value, _lineNumber);
			// FIXME: check range?
			switch (tempVal._type) {
			case rvtScriptData:
				// FIXME: bounds checks
				instScript = tempVal._instance->_script;
				fixup = Common::find(instScript->_globalFixups.begin(), instScript->_globalFixups.end(), tempVal._value);
				if (fixup != instScript->_globalFixups.end())
					error("script tried MEMWRITEW on fixup on %d", _lineNumber);
				if (tempVal._instance->_globalObjects->contains(tempVal._value))
					tempVal._instance->_globalObjects->erase(tempVal._value);
				WRITE_LE_UINT16(&(*tempVal._instance->_globalData)[tempVal._value], (int16)_registers[int1]._value);
				break;
			case rvtSystemObject:
				tempVal._object->writeUint16(tempVal._value, (int16)_registers[int1]._value);
				break;
			case rvtStackPointer:
				if (tempVal._value + 2 >= _stack.size())
					error("script tried to MEMWRITEW to out-of-bounds stack@%d on line %d",
						tempVal._value, _lineNumber);
				_stack[tempVal._value] = (int16)_registers[int1]._value;
				_stack[tempVal._value + 1].invalidate();
				break;
			default:
				error("script tried to MEMWRITEW to runtime value of type %d (value %d) on line %d",
					tempVal._type, tempVal._value, _lineNumber);
			}
			break;
		case SCMD_JZ:
			// jump if ax==0 by arg1
			if (_registers[SREG_AX]._type != rvtInteger && _registers[SREG_AX]._type != rvtFloat)
				break;
			if (_registers[SREG_AX]._type == rvtFloat && _registers[SREG_AX]._floatValue != 0.0f)
				break;
			if (_registers[SREG_AX]._type == rvtInteger && _registers[SREG_AX]._value != 0)
				break;
			_pc += int1;
			break;
		case SCMD_JNZ:
			// jump by arg1 if ax!=0
			if (_registers[SREG_AX]._type == rvtFloat && _registers[SREG_AX]._floatValue == 0.0f)
				break;
			if (_registers[SREG_AX]._type == rvtInteger && _registers[SREG_AX]._value == 0)
				break;
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
				break;
			if (int1 > 0)
				break;
			// FIXME: make sure the script isn't stuck in a loop
			break;
		case SCMD_MUL:
			// reg1 *= arg2
			_registers[int1]._signedValue *= int2;
			break;
		case SCMD_CHECKBOUNDS:
			// check reg1 is between 0 and arg2
			if (_registers[int1]._type != rvtInteger)
				error("script error: checkbounds got value of type %d (not integer)", _registers[int1]._type);
			if (_registers[int1]._value >= (uint32)int2)
				error("script error: checkbounds value %d was not in range 0 to %d", _registers[int1]._value, int2);
			break;
		case SCMD_DYNAMICBOUNDS:
			// check reg1 is between 0 and m[MAR-4]
			// FIXME
			error("unimplemented %s", info.name);
			break;
		case SCMD_MEMREADPTR:
			// reg1 = m[MAR] (adjust ptr addr)
			tempObj = getObjectFrom(_registers[SREG_MAR]);
			if (tempObj)
				_registers[int1] = tempObj;
			else
				_registers[int1] = 0;
			break;
		case SCMD_MEMWRITEPTR:
			// m[MAR] = reg1 (adjust ptr addr)
			tempObj = NULL;
			if (_registers[int1]._type != rvtInteger || _registers[int1]._value != 0) {
				tempObj = getObjectFrom(_registers[int1]);
				if (!tempObj)
					error("script tried to MEMWRITEPTR using runtime value of type %d (value %d) on line %d",
						_registers[int1]._type, _registers[int1]._value, _lineNumber);
			}
			writePointer(_registers[SREG_MAR], tempObj);
			break;
		case SCMD_MEMINITPTR:
			// m[MAR] = reg1 (but don't free old one)
			tempObj = NULL;
			if (_registers[int1]._type != rvtInteger || _registers[int1]._value != 0) {
				tempObj = getObjectFrom(_registers[int1]);
				if (!tempObj)
					error("script tried to MEMINITPTR using runtime value of type %d (value %d) on line %d",
						_registers[int1]._type, _registers[int1]._value, _lineNumber);
			}
			writePointer(_registers[SREG_MAR], tempObj);
			break;
		case SCMD_MEMZEROPTR:
			// m[MAR] = 0    (blank ptr)
			writePointer(_registers[SREG_MAR], NULL);
			break;
		case SCMD_MEMZEROPTRND:
			// m[MAR] = 0    (blank ptr, no dispose if = ax)
			writePointer(_registers[SREG_MAR], NULL);
			break;
		case SCMD_CHECKNULL:
			// error if MAR==0
			if (_registers[SREG_MAR]._type == rvtInteger && _registers[SREG_MAR]._value == 0)
				error("script tried to dereference null pointer on line %d", _lineNumber);
			break;
		case SCMD_CHECKNULLREG:
			// error if reg1 == NULL
			if (_registers[int1]._type == rvtInteger && _registers[int1]._value == 0)
				error("script tried to dereference null pointer on line %d", _lineNumber);
			break;
		case SCMD_NUMFUNCARGS:
			// setfuncargs: number of arguments for ext func call
			funcArgumentCount = int1;
			break;
		case SCMD_CALLAS:
			// $callscr: call external script function
			{
			if (_registers[int1]._type != rvtScriptFunction)
				error("script tried to CALLAS non-script-function runtime value of type %d (value %d) on line %d",
					_registers[int1]._type, _registers[int1]._value, _lineNumber);
			// TODO: store call stack (line number etc)

			// save current state
			ccInstance *wasRunning = _runningInst;
			uint32 oldpc = _pc;

			// push the parameters on the stack
			uint startArg = 0;
			if (funcArgumentCount != (uint)-1) {
				// only push arguments for this call
				// (there might be nested CALLAS calls?)
				startArg = externalStack.size() - funcArgumentCount;
			}
			for (uint i = startArg; i < externalStack.size(); ++i) {
				pushValue(externalStack[i]);
			}

			uint32 oldsp = _registers[SREG_SP]._value;

			// push 0, so that the runCodeFrom returns
			pushValue(0);

			// call the script function
			_runningInst = _registers[int1]._instance;
			runCodeFrom(_registers[int1]._value);

			if (_registers[SREG_SP]._value != oldsp)
				error("stack corrupt after CALLAS on line %d (was %d, now %d)", _lineNumber, oldsp, _registers[SREG_SP]._value);

			// restore previous state
			_pc = oldpc;
			_runningInst = wasRunning;
			// TODO: restore call stack (line number etc)

			recoverFromCallAs = !externalStack.empty();
			funcArgumentCount = (uint)-1;
			nextCallNeedsObject = false;
			}
			break;
		case SCMD_CALLEXT:
			// farcall: call external (imported) function reg1
			if (_registers[int1]._type != rvtSystemFunction)
				error("script tried to CALLEXT non-system-function runtime value of type %d (value %d) on line %d",
					_registers[int1]._type, _registers[int1]._value, _lineNumber);
			debug(3, "calling external function '%s'", script->_imports[_registers[int1]._value].c_str());

			recoverFromCallAs = false;
			if (funcArgumentCount == (uint)-1)
				funcArgumentCount = externalStack.size();

			// construct the parameter list (in reverse order)
			params.resize(funcArgumentCount);
			for (uint i = 0; i < funcArgumentCount; ++i)
				params[i] = externalStack[externalStack.size() - i - 1];

			if (nextCallNeedsObject) {
				if (_registers[SREG_OP]._type != rvtSystemObject)
					error("script tried to CALLEXT on non-system-object runtime value of type %d (value %d) on line %d",
						tempVal._type, tempVal._value, _lineNumber);
				uint32 offset = _registers[SREG_OP]._value;
				ScriptObject *object = _registers[SREG_OP]._object->getObjectAt(offset);
				if (offset != 0)
					error("script tried to CALLEXT on system-object with offset %d (resolved to %d) on line %d",
						_registers[SREG_OP]._value, offset, _lineNumber);

				_registers[SREG_AX] = callImportedFunction(_registers[int1]._function, object, params);
			} else {
				_registers[SREG_AX] = callImportedFunction(_registers[int1]._function, NULL, params);
			}

			// TODO: unfinished
			funcArgumentCount = (uint)-1;
			nextCallNeedsObject = false;
			break;
		case SCMD_PUSHREAL:
			// farpush: push reg1 onto real stack
			if (externalStack.size() >= MAX_FUNC_PARAMS)
				error("external call stack overflow at line %d", _lineNumber);
			externalStack.push(_registers[int1]);
			break;
		case SCMD_SUBREALSTACK:
			// farsubsp
			if (recoverFromCallAs) {
				for (uint i = 0; i < (uint)int1; ++i)
					popValue();
				recoverFromCallAs = false;
			}
			if (externalStack.size() < (uint)int1)
				error("script tried to farsubsp %d parameters, but there were only %d", int1, externalStack.size());
			for (uint i = 0; i < (uint32)int1; ++i)
				externalStack.pop();
			break;
		case SCMD_CALLOBJ:
			// $callobj: next call is member function of reg1
			nextCallNeedsObject = true;
			// set the OP register
			// (we don't check for validity here because the next call might not be CALLEXT)
			_registers[SREG_OP] = _registers[int1];
			break;
		case SCMD_SHIFTLEFT:
			// reg1 = reg1 << reg2
			_registers[int1]._signedValue <<= _registers[int2]._signedValue;
			break;
		case SCMD_SHIFTRIGHT:
			// reg1 = reg1 >> reg2
			_registers[int1]._signedValue >>= _registers[int2]._signedValue;
			break;
		case SCMD_THISBASE:
			// thisaddr: current relative address
			currentBase.pop();
			currentBase.push(int1);
			break;
		case SCMD_NEWARRAY:
			// reg1 = new array of reg1 elements, each of size arg2 (arg3=managed type?)
			// FIXME
			error("unimplemented %s", info.name);
			break;
		case SCMD_FADD:
			// reg1 += arg2 (float,int)
			_registers[int1]._floatValue += int2;
			break;
		case SCMD_FSUB:
			// reg1 -= arg2 (float,int)
			_registers[int1]._floatValue -= int2;
			break;
		case SCMD_FMULREG:
			// reg1 *= reg2 (float)
			_registers[int1]._floatValue *= _registers[int2]._floatValue;
			break;
		case SCMD_FDIVREG:
			// reg1 /= reg2 (float)
			if (_registers[int2]._floatValue == 0.0)
				error("script tried to divide by fp zero on line %d", _lineNumber);
			_registers[int1]._floatValue /= _registers[int2]._floatValue;
			break;
		case SCMD_FADDREG:
			// reg1 += reg2 (float)
			_registers[int1]._floatValue += _registers[int2]._floatValue;
			break;
		case SCMD_FSUBREG:
			// reg1 -= reg2 (float)
			_registers[int1]._floatValue -= _registers[int2]._floatValue;
			break;
		case SCMD_FGREATER:
			// reg1 > reg2 (float)
			_registers[int1] = (_registers[int1]._floatValue > _registers[int2]._floatValue) ? 1.0f : 0.0f;
			break;
		case SCMD_FLESSTHAN:
			// reg1 < reg2 (float)
			_registers[int1] = (_registers[int1]._floatValue < _registers[int2]._floatValue) ? 1.0f : 0.0f;
			break;
		case SCMD_FGTE:
			// reg1 >= reg2 (float)
			_registers[int1] = (_registers[int1]._floatValue >= _registers[int2]._floatValue) ? 1.0f : 0.0f;
			break;
		case SCMD_FLTE:
			// reg1 <= reg2 (float)
			_registers[int1] = (_registers[int1]._floatValue <= _registers[int2]._floatValue) ? 1.0f : 0.0f;
			break;
		case SCMD_ZEROMEMORY:
			// m[MAR]..m[MAR+(arg1-1)] = 0
			tempVal = _registers[SREG_MAR];
			switch (tempVal._type) {
			case rvtStackPointer:
				if (tempVal._value + (uint32)int1 >= _stack.size())
					error("script tried to ZEROMEMORY out-of-bounds stack@%d-%d on line %d",
						tempVal._value, int1, _lineNumber);
				for (uint i = 0; i < (uint32)int1; ++i)
					_stack[tempVal._value + i] = 0;
				break;
			default:
				error("script tried to ZEROMEMORY using runtime value of type %d (value %d) on line %d",
					tempVal._type, tempVal._value, _lineNumber);
			}
			break;
		case SCMD_CREATESTRING:
			// reg1 = new String(reg1)
			tempStr1 = createStringFrom(_registers[int1]);
			_registers[int1] = new ScriptMutableString(tempStr1->getString());
			_registers[int1]._object->DecRef();
			tempStr1->DecRef();
			break;
		case SCMD_STRINGSEQUAL:
			// (char*)reg1 == (char*)reg2   reg1=1 if true, =0 if not
			tempStr1 = createStringFrom(_registers[int1]);
			tempStr2 = createStringFrom(_registers[int2]);
			_registers[int1] = tempStr1->getString().equals(tempStr2->getString()) ? 1 : 0;
			tempStr1->DecRef();
			tempStr2->DecRef();
			break;
		case SCMD_STRINGSNOTEQ:
			// (char*)reg1 != (char*)reg2
			tempStr1 = createStringFrom(_registers[int1]);
			tempStr2 = createStringFrom(_registers[int2]);
			_registers[int1] = tempStr1->getString().equals(tempStr2->getString()) ? 0 : 1;
			tempStr1->DecRef();
			tempStr2->DecRef();
			break;
		case SCMD_LOOPCHECKOFF:
			// no loop checking for this function
			if (loopIterationCheckDisabledCount == 0)
				loopIterationCheckDisabledCount++;
			break;
		default:
			error("runCodeFrom(): invalid instruction %d", instruction);
		}

		if (_registers[SREG_SP]._type != rvtStackPointer || _registers[SREG_SP]._value < 4 || _registers[SREG_SP]._value >= _stack.size())
			error("runCodeFrom(): SP got clobbered (now type %d, value %d) on line %d",
				_registers[SREG_SP]._type, _registers[SREG_SP]._value, _lineNumber);

		// increment to next instruction, skipping any arguments
		_pc += (1 + neededArgs);
	}
}

const uint kOldScriptStringLength = 200;

class ScriptStackString : public ScriptString {
public:
	ScriptStackString(ccInstance *instance, uint32 offset) : _instance(instance), _offset(offset) { }

	const Common::String getString() {
		if (_instance->_stack.size() <= _offset + kOldScriptStringLength)
			error("ScriptStackString: offset %d is too high", _offset);

		// FIXME: sanity-check types
		Common::String text;
		for (uint i = 0; i < kOldScriptStringLength; ++i) {
			if (_instance->_stack[_offset + i]._value == 0)
				break;
			if (i == kOldScriptStringLength - 1)
				error("ScriptStackString: string isn't null-terminated");
			text += (char)_instance->_stack[_offset + i]._value;
		}
		return text;
	}

	void setString(const Common::String &text) {
		// FIXME: check bounds
		if (text.size() >= kOldScriptStringLength)
			error("ScriptStackString: new string is too large (%d)", text.size());

		for (uint i = 0; i < text.size(); ++i) {
			_instance->_stack[_offset + i] = (uint)text[i];
		}
		_instance->_stack[_offset + text.size()] = 0;
	}

protected:
	ccInstance *_instance;
	uint32 _offset;
};

class ScriptDataString : public ScriptString {
public:
	ScriptDataString(ccInstance *instance, uint32 offset) : _instance(instance), _offset(offset) { }

	const Common::String getString() {
		if (_instance->_globalData->size() <= _offset + kOldScriptStringLength)
			error("ScriptDataString: offset %d is too high", _offset);

		Common::String text;
		for (uint i = 0; i < kOldScriptStringLength; ++i) {
			if ((*_instance->_globalData)[_offset + i] == 0)
				break;
			if (i == kOldScriptStringLength - 1)
				error("ScriptDataString: string isn't null-terminated");
			text += (char)(*_instance->_globalData)[_offset + i];
		}
		return text;
	}

	void setString(const Common::String &text) {
		if (text.size() >= kOldScriptStringLength)
			error("ScriptDataString: new string is too large (%d)", text.size());

		// FIXME: kill any objects?
		memcpy(&(*_instance->_globalData)[_offset], text.c_str(), text.size() + 1);
	}


protected:
	ccInstance *_instance;
	uint32 _offset;
};

ScriptString *ccInstance::createStringFrom(RuntimeValue &value) {
	if (value._type == rvtStackPointer)
		return new ScriptStackString(this, value._value);
	else if (value._type == rvtScriptData) {
		ccScript *script = value._instance->_script;
		uint32 *fixup = Common::find(script->_globalFixups.begin(), script->_globalFixups.end(), value._value);
		if (fixup != script->_globalFixups.end())
			error("createStringFrom called with (fixup) pointer to string");
		return new ScriptDataString(value._instance, value._value);
	} else if (value._type == rvtSystemObject && value._object->isOfType(sotString)) {
		if (value._value != 0)
			error("createStringFrom failed to create a string from string object with offset %d", value._value);
		ScriptString *string = (ScriptString *)value._object;
		string->IncRef();
		return string;
	}

	error("createStringFrom failed to create a string from value of type %d", value._type);
}

RuntimeValue ccInstance::callImportedFunction(const ScriptSystemFunctionInfo *function,
	ScriptObject *object, Common::Array<RuntimeValue> &params) {

	// check the signature
	uint pos = 0;
	while (function->signature[pos] != '\0' && function->signature[pos] != '.') {
		if (pos == params.size())
			error("not enough parameters (%d) to '%s'", params.size(), function->name);

		char sigEntry = function->signature[pos];
		switch (sigEntry) {
		case 'i':
		case 'c':
			// integer
			if (params[pos]._type != rvtInteger)
				error("expected integer for param %d of '%s', got type %d",
					pos + 1, function->name, params[pos]._type);
			break;
		case 'f':
			// float
			if (params[pos]._type == rvtInteger) {
				// FIXME: This is horrible.
				params[pos]._type = rvtFloat;
			}
			if (params[pos]._type != rvtFloat)
				error("expected float for param %d of '%s', got type %d",
					pos + 1, function->name, params[pos]._type);
			break;
		case 'p':
			// object OR null
			if (params[pos]._type == rvtInteger && params[pos]._value == 0)
				break;
			// (fallthrough)
		case 'o':
			// object
			if (params[pos]._type != rvtSystemObject)
				error("expected object for param %d of '%s', got type %d",
					pos + 1, function->name, params[pos]._type);
			uint32 offset;
			offset = params[pos]._value;
			params[pos] = params[pos]._object->getObjectAt(offset);
			params[pos]._value = offset;
			break;
		case 't':
			// string OR null
			if (params[pos]._type == rvtInteger && params[pos]._value == 0)
				break;
			// (fallthrough)
		case 's':
			// string
			if (params[pos]._type == rvtStackPointer || params[pos]._type == rvtScriptData) {
				params[pos] = createStringFrom(params[pos]);
				params[pos]._object->DecRef();
				break;
			}
			if (params[pos]._type != rvtSystemObject || !params[pos]._object->isOfType(sotString))
				error("expected string for param %d of '%s', got type %d",
					pos + 1, function->name, params[pos]._type);
			if (params[pos]._value != 0)
				error("unexpected offset %d when creating string object for param %d of '%s'",
					params[pos]._value, pos + 1, function->name);
			break;
		default:
			error("unknown entry in signature '%s' for '%s'", function->signature, function->name);
		}
		++pos;
	}

	if (function->objectType && !object)
		error("expected '%s' to be called on an object", function->name);

	// if this is a member function, make sure it's being called on an object of the right type
	if (object) {
		if (!object->isOfType(function->objectType))
			error("'%s' was passed an object with the wrong type '%s'", function->name, object->getObjectTypeName());
	}

	if (function->signature[pos] == '.') {
		// variable argument function
		while (pos < params.size()) {
			if (params[pos]._type != rvtInteger && params[pos]._type != rvtFloat) {
				// assume it's a string
				params[pos] = createStringFrom(params[pos]);
				params[pos]._object->DecRef();
			}
			++pos;
		}
	} else if (pos < params.size())
		error("too many parameters (%d) to '%s'", params.size(), function->name);

	return function->function(_vm, object, params);
}

void ccInstance::pushValue(const RuntimeValue &value) {
	// TODO: shouldn't be assert?
	assert(_registers[SREG_SP]._type == rvtStackPointer);

	uint32 stackValue = _registers[SREG_SP]._value;
	if (stackValue + 4 > _stack.size())
		error("script caused VM stack overflow");

	_stack[stackValue] = value;
	_stack[stackValue + 1].invalidate();
	_stack[stackValue + 2].invalidate();
	_stack[stackValue + 3].invalidate();

	_registers[SREG_SP]._value += 4;
}

RuntimeValue ccInstance::popValue() {
	// TODO: shouldn't be assert?
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

ScriptObject *ccInstance::getObjectFrom(const RuntimeValue &value) {
	switch (value._type) {
	case rvtScriptData:
		ccScript *instScript;
		uint32 *fixup;
		instScript = value._instance->_script;
		if (value._instance->_globalObjects->contains(value._value))
			return getObjectFrom((*value._instance->_globalObjects)[value._value]);

		// no object, might still have a null pointer?
		fixup = Common::find(instScript->_globalFixups.begin(), instScript->_globalFixups.end(), value._value);
		if (fixup != instScript->_globalFixups.end())
			error("getObjectFrom got fixup for data@%d on line %d", value._value, _lineNumber);
		// FIXME: bounds check
		uint32 val;
		val = READ_LE_UINT32(&(*value._instance->_globalData)[value._value]);
		if (val == 0)
			return NULL;
		else
			error("script tried to get object using global data (offset %d) on line %d",
				value._value, _lineNumber);
	case rvtSystemObject:
		return value._object;
	case rvtStackPointer:
		if (value._value + 4 > _stack.size())
			error("script tried to get object from beyond the end of the stack (value %d) on line %d",
				value._value, _lineNumber);
		if (_stack[value._value]._type == rvtSystemObject)
			return _stack[value._value]._object;
		else if (_stack[value._value]._type == rvtInteger && _stack[value._value]._value == 0)
			return NULL;
		else
			error("script tried to get object using stack (offset %d) on line %d",
				value._value, _lineNumber);
	default:
		error("script tried to get object using runtime value of type %d (value %d) on line %d",
			value._type, value._value, _lineNumber);
	}
}

void ccInstance::writePointer(const RuntimeValue &value, ScriptObject *object) {
	ccScript *instScript;
	uint32 *fixup;
	switch (value._type) {
	case rvtScriptData:
		// FIXME: bounds checks
		instScript = value._instance->_script;
		fixup = Common::find(instScript->_globalFixups.begin(), instScript->_globalFixups.end(), value._value);
		// FIXME: *wrong*, this should be a pointer?
		// FIXME	argVal[v]._value = (*inst->_globalData)[argValue];
		if (fixup != instScript->_globalFixups.end())
			error("writePointer fixup fail");
		// writing an object to script global data
		WRITE_LE_UINT32(&(*value._instance->_globalData)[value._value], 0);
		if (object)
			(*value._instance->_globalObjects)[value._value] = object;
		else
			value._instance->_globalObjects->erase(value._value);
		break;
	case rvtSystemObject:
		// FIXME: !!!
		error("script tried to writePointer to system object (value %d) on line %d",
			value._value, _lineNumber);
		break;
	case rvtStackPointer:
		if (value._value + 4 >= _stack.size())
			error("script tried to writePointer to out-of-bounds stack@%d on line %d",
				value._value, _lineNumber);
		if (object)
			_stack[value._value] = object;
		else
			_stack[value._value] = 0;
		_stack[value._value + 1].invalidate();
		_stack[value._value + 2].invalidate();
		_stack[value._value + 3].invalidate();
		break;
	default:
		error("script tried to writePointer to runtime value of type %d (value %d) on line %d",
			value._type, value._value, _lineNumber);
	}
}

} // End of namespace AGS
