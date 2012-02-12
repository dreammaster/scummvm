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
#include "common/debug.h"

namespace AGS {

#define SCOM_VERSION 89

static Common::String readString(Common::SeekableReadStream *dta) {
	Common::String str;
	while (true) {
		char c = (char)dta->readByte();
		if (!c)
			break;
		str += c;
	}
	return str;
}

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
	for (uint i = 0; i < codeSize; ++i)
		_code[i] = dta->readUint32LE();

	_strings.resize(stringsSize);
	if (stringsSize)
		dta->read(&_strings[0], stringsSize);

	uint32 fixupsCount = dta->readUint32LE();
	_fixups.resize(fixupsCount);
	for (uint i = 0; i < fixupsCount; ++i)
		_fixups[i]._type = dta->readByte();
	for (uint i = 0; i < fixupsCount; ++i)
		_fixups[i]._index = dta->readUint32LE();

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

} // End of namespace AGS
