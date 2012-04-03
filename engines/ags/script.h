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

#ifndef AGS_SCRIPT_H
#define AGS_SCRIPT_H

#include "common/array.h"
#include "common/hash-str.h"
#include "common/stream.h"

#include "engines/ags/scriptobj.h"

namespace AGS {

enum ScriptImportType {
	sitInvalid,
	sitSystemFunction,
	sitSystemObject,
	sitScriptFunction,
	sitScriptData
};

struct ScriptCodeEntry {
	uint32 _data;
	byte _fixupType; // global data/string area/ etc
};

struct ScriptExport {
	Common::String _name;
	ScriptImportType _type;
	uint32 _address;
};

// 'sections' allow the interpreter to find out which bit
// of the code came from header files, and which from the main file
struct ScriptSection {
	Common::String _name;
	uint32 _offset;
};

enum RuntimeValueType {
	rvtInvalid = 0,
	// constants
	rvtInteger,
	rvtFloat,
	// local code
	rvtFunction,
	// script data
	rvtScriptData,
	// other imports
	rvtScriptFunction,
	rvtSystemFunction,
	rvtSystemObject,
	// local stack
	rvtStackPointer
};

class AGSEngine;
class ccInstance;
struct RuntimeValue;
struct ScriptSystemFunctionInfo;

struct RuntimeValue {
	RuntimeValue() : _type(rvtInteger), _value(0) { }
	RuntimeValue(uint32 intValue) : _type(rvtInteger), _value(intValue) { }
	RuntimeValue(int intValue) : _type(rvtInteger), _signedValue(intValue) { }
	RuntimeValue(float floatValue) : _type(rvtFloat), _floatValue(floatValue) { }

	// support for object reference counting
	RuntimeValue(ScriptObject *obj) : _type(rvtSystemObject), _value(0), _object(obj) { _object->IncRef(); }
	~RuntimeValue() { if (_type == rvtSystemObject) _object->DecRef(); }
	RuntimeValue(const RuntimeValue &v) {
		_type = v._type;
		_value = v._value;
		_object = v._object;
		if (_type == rvtSystemObject)
			_object->IncRef();
	}
	RuntimeValue &operator=(const RuntimeValue &v) {
		if (_type == rvtSystemObject)
			_object->DecRef();
		_type = v._type;
		_value = v._value;
		_object = v._object;
		if (_type == rvtSystemObject)
			_object->IncRef();
		return *this;
	}

	// the type and associated data
	RuntimeValueType _type;
	union {
		// integer value, offset, etc
		uint32 _value;
		int32 _signedValue;
		float _floatValue;
	};
	union {
		ccInstance *_instance;
		ScriptObject *_object;
		const ScriptSystemFunctionInfo *_function;
	};

	RuntimeValue &operator=(int32 intValue) {
		if (_type == rvtSystemObject)
			_object->DecRef();
		_type = rvtInteger;
		_signedValue = intValue;
		return *this;
	}
	RuntimeValue &operator=(uint32 intValue) {
		if (_type == rvtSystemObject)
			_object->DecRef();
		_type = rvtInteger;
		_value = intValue;
		return *this;
	}
	RuntimeValue &operator=(float floatValue) {
		if (_type == rvtSystemObject)
			_object->DecRef();
		_type = rvtFloat;
		_floatValue = floatValue;
		return *this;
	}

	bool equalTo(const RuntimeValue &value) const {
		// FIXME: check instance for rvtScriptData, rvtScriptFunction
		if (_type == rvtSystemObject && value._type == rvtSystemObject) {
			// two objects, resolve their offsets and compare the result
			uint32 offset1 = _value;
			ScriptObject *object1 = _object->getObjectAt(offset1);
			uint32 offset2 = value._value;
			ScriptObject *object2 = value._object->getObjectAt(offset2);
			return (offset1 == offset2) && (object1 == object2);
		} else {
			return (_type == value._type) && (_value == value._value);
		}
	}

	void invalidate() {
		if (_type == rvtSystemObject)
			_object->DecRef();
		_type = rvtInvalid;
	}
};

// the data for a script
struct ccScript {
	void readFrom(Common::SeekableReadStream *dta);

	Common::Array<byte> _globalData;
	Common::Array<uint32> _globalFixups;

	Common::Array<ScriptCodeEntry> _code;
	Common::Array<byte> _strings;
	Common::Array<Common::String> _imports;
	Common::Array<ScriptExport> _exports;
	uint32 _instances;
	Common::Array<ScriptSection> _sections;
};

struct ScriptImport {
	ScriptImportType _type;

	// function pointer (system)
	union {
		const ScriptSystemFunctionInfo *_function;
		class ScriptObject *_object;
	};

	// script instance (script)
	ccInstance *_owner;
	// code/data offset (script)
	uint32 _offset;
};

struct CallStackEntry {
	uint32 _lineNumber;
	uint32 _address;
	class ccInstance *_instance;
};

// a running instance of a script
class ccInstance {
	friend class ScriptStackString;
	friend class ScriptDataString;

public:
	ccInstance(AGSEngine *vm, ccScript *script, bool autoImport = false, ccInstance *fork = NULL);
	~ccInstance();

	bool isRunning() { return (_pc != 0); }
	bool exportsSymbol(const Common::String &name);
	void call(const Common::String &name, const Common::Array<RuntimeValue> &params);

	uint32 getReturnValue();

protected:
	void runCodeFrom(uint32 start);
	ScriptString *createStringFrom(RuntimeValue &value);
	RuntimeValue callImportedFunction(const ScriptSystemFunctionInfo *function, ScriptObject *object,
		Common::Array<RuntimeValue> &params);

	AGSEngine *_vm;
	ccScript *_script;
	uint32 _flags;

	Common::Array<byte> *_globalData;
	Common::HashMap<uint32, RuntimeValue> *_globalObjects;

	uint32 _pc;
	RuntimeValue _returnValue;
	uint32 _lineNumber;
	Common::Array<RuntimeValue> _registers;
	Common::Array<CallStackEntry> _callStack;
	Common::Array<RuntimeValue> _stack;
	Common::Array<ScriptImport> _resolvedImports;
	// might point to another instance if in far call
	ccInstance *_runningInst;

	void pushValue(const RuntimeValue &value);
	RuntimeValue popValue();
	uint32 popIntValue();

	ScriptObject *getObjectFrom(const RuntimeValue &value);
	void writePointer(const RuntimeValue &value, ScriptObject *object);
};

} // End of namespace AGS

#endif // AGS_SCRIPT_H
