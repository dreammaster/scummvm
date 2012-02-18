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
	// constants
	rvtInteger,
	rvtFloat,
	// local data/code
	rvtGlobalData,
	rvtFunction,
	rvtString,
	// imports
	rvtScriptFunction,
	rvtScriptData,
	rvtSystemFunction,
	rvtSystemObject,
	// local stack
	rvtStackPointer
};

class AGSEngine;
class ccInstance;
struct RuntimeValue;

typedef RuntimeValue ScriptAPIFunction(AGSEngine *vm, const Common::Array<RuntimeValue> &params);

struct RuntimeValue {
	RuntimeValue() : _type(rvtInteger), _value(0) { }
	RuntimeValue(uint32 intValue) : _type(rvtInteger), _value(intValue) { }

	RuntimeValueType _type;
	union {
		uint32 _value;
		int32 _signedValue;
	};
	union {
		ccInstance *_instance;
		ScriptObject *_object;
		ScriptAPIFunction *_function;
	};

	RuntimeValue &operator=(uint32 intValue) {
		_type = rvtInteger;
		_value = intValue;
		return *this;
	}
};

struct ScriptImport {
	ScriptImportType _type;

	// function pointer (system)
	union {
		ScriptAPIFunction *_function;
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
public:
	ccInstance(AGSEngine *vm, ccScript *script, bool autoImport = false, ccInstance *fork = NULL);
	~ccInstance();

	bool isRunning() { return (_pc != 0); }
	bool exportsSymbol(const Common::String &name);
	void call(const Common::String &name, const Common::Array<uint32> &params);

protected:
	void runCodeFrom(uint32 start);

	AGSEngine *_vm;
	ccScript *_script;
	uint32 _flags;

	Common::Array<byte> *_globalData;

	uint32 _pc;
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
};

// array of (system) script objects; for characters[], gui[], etc
template<class T> class ScriptObjectArray : public ScriptObject {
public:
	ScriptObjectArray(Common::Array<T> &array, uint32 elementSize) : _array(array), _elementSize(elementSize) { }
	virtual ScriptObject *getObjectAt(uint32 &offset) {
		uint32 objectId = offset / _elementSize;
		if (objectId >= _array.size())
			return NULL;
		offset = offset % _elementSize;
		return &_array[objectId];
	}

protected:
	uint32 _elementSize;
	Common::Array<T> &_array;
};

// specialization of above for arrays containing pointers
template<class T> class ScriptObjectArray<T *> : public ScriptObject {
public:
	ScriptObjectArray(Common::Array<T *> &array, uint32 elementSize) : _array(array), _elementSize(elementSize) { }
	virtual ScriptObject *getObjectAt(uint32 &offset) {
		uint32 objectId = offset / _elementSize;
		if (objectId >= _array.size())
			return NULL;
		offset = offset % _elementSize;
		return _array[objectId];
	}

protected:
	uint32 _elementSize;
	Common::Array<T *> &_array;
};


} // End of namespace AGS

#endif // AGS_SCRIPT_H
