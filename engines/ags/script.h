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

#ifndef AGS_SCRIPT_H
#define AGS_SCRIPT_H

#include "common/array.h"
#include "common/stream.h"

namespace AGS {

struct ScriptCodeEntry {
	uint32 _data;
	byte _fixupType; // global data/string area/ etc
};

struct ScriptExport {
	Common::String _name; // name of export
	uint32 _address; // high byte is type; low 24-bits are offset
};

// 'sections' allow the interpreter to find out which bit
// of the code came from header files, and which from the main file
struct ScriptSection {
	Common::String _name;
	uint32 _offset;
};

// the data for a script
struct ccScript {
	void readFrom(Common::SeekableReadStream *dta);

	Common::Array<byte> _globalData;
	Common::Array<ScriptCodeEntry> _code;
	Common::Array<byte> _strings;
	Common::Array<uint32> _globalFixups;
	Common::Array<Common::String> _imports;
	Common::Array<ScriptExport> _exports;
	uint32 _instances;
	Common::Array<ScriptSection> _sections;
};

enum RuntimeValueType {
	rvtInvalid = 0,
	rvtInteger,
	rvtGlobalData,
	rvtFunction,
	rvtString,
	rvtImport,
	rvtStackPointer
};

struct RuntimeValue {
	RuntimeValue() : _type(rvtInteger), _value(0) { }
	RuntimeValue(uint32 intValue) : _type(rvtInteger), _value(intValue) { }

	RuntimeValueType _type;
	uint32 _value;

	RuntimeValue &operator=(uint32 intValue) {
		_type = rvtInteger;
		_value = intValue;
		return *this;
	}
};

struct CallStackEntry {
	uint32 _lineNumber;
	uint32 _address;
	class ccInstance *_instance;
};

class AGSEngine;

// a running instance of a script
class ccInstance {
public:
	ccInstance(AGSEngine *vm, ccScript *script, bool autoImport = false, ccInstance *fork = NULL);

	bool isRunning() { return (_pc != 0); }
	bool exportsSymbol(const Common::String &name);
	void call(const Common::String &name, const Common::Array<uint32> &params);

protected:
	void runCodeFrom(uint32 start);
	RuntimeValue callImportedFunction(uint32 importId, const Common::Array<RuntimeValue> &params);

	AGSEngine *_vm;
	ccScript *_script;
	uint32 _flags;

	Common::Array<byte> *_globalData;

	uint32 _pc;
	uint32 _lineNumber;
	Common::Array<RuntimeValue> _registers;
	Common::Array<CallStackEntry> _callStack;
	Common::Array<RuntimeValue> _stack;
	// might point to another instance if in far call
	ccInstance *_runningInst;

	void pushValue(const RuntimeValue &value);
	RuntimeValue popValue();
	uint32 popIntValue();
};

} // End of namespace AGS

#endif // AGS_SCRIPT_H
