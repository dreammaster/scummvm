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

#ifndef AGS_SCRIPT_H
#define AGS_SCRIPT_H

#include "common/array.h"
#include "common/stream.h"

namespace AGS {

struct ScriptFixup {
	uint32 _index; // code array index to fixup (in longs)
	byte _type; // global data/string area/ etc
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

struct ccScript {
	void readFrom(Common::SeekableReadStream *dta);

	Common::Array<byte> _globalData;
	Common::Array<uint32> _code;
	Common::Array<byte> _strings;
	Common::Array<ScriptFixup> _fixups;
	Common::Array<Common::String> _imports;
	Common::Array<ScriptExport> _exports;
	uint32 _instances;
	Common::Array<ScriptSection> _sections;
};

} // End of namespace AGS

#endif // AGS_SCRIPT_H
