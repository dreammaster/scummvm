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
#include "common/debug.h"

namespace AGS {

#define SCOM_VERSION 89

#define FIXUP_GLOBALDATA  1     // code[fixup] += &globaldata[0]
#define FIXUP_FUNCTION    2     // code[fixup] += &code[0]
#define FIXUP_STRING      3     // code[fixup] += &strings[0]
#define FIXUP_IMPORT      4     // code[fixup] = &imported_thing[code[fixup]]
#define FIXUP_DATADATA    5     // globaldata[fixup] += &globaldata[0]
#define FIXUP_STACK       6     // code[fixup] += &stack[0]
#define EXPORT_FUNCTION   1
#define EXPORT_DATA       2

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

#define CC_NUM_REGISTERS  8
#define INSTF_SHAREDATA   1
#define INSTF_ABORTED     2
#define INSTF_FREE        4
#define INSTF_RUNNING     8   // set by main code to confirm script isn't stuck
#define CC_STACK_SIZE     4000
#define MAX_CALL_STACK    100

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

	_stack.resize(CC_STACK_SIZE);

	_registers.resize(CC_NUM_REGISTERS);
	for (uint i = 0; i < _registers.size(); ++i) {
		_registers[i]._value = 0;
	}

	// FIXME: check all imports can be resolved?

	_pc = 0;
	_script->_instances++;
	if ((_script->_instances == 1) && autoImport) {
		// FIXME: import all the exported stuff from this script
	}
}

void ccInstance::runTextScript(const Common::String &name) {
	// FIXME
}

} // End of namespace AGS
