/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef WASTELAND_FOD_DATA_GLOBALS_H
#define WASTELAND_FOD_DATA_GLOBALS_H

#include "common/str.h"
#include "common/stream.h"

namespace Wasteland {
namespace FOD {
namespace Data {

#define GLOBALS_ITEMS_COUNT 118

enum {
	GIFLAG_1 = 1,
	GIFLAG_4 = 4,
	GIFLAG_8 = 8,
	GIFLAG_16 = 16
};

struct GlobalItem {
	Common::String _name;
	uint16 _field12;
	byte _flags;
	byte _weaponIndex;
	byte _field16;
	byte _bitShift;

	byte &_ac = _weaponIndex;

	void load(Common::SeekableReadStream &src);

	bool proc1() const {
		return (_flags & (GIFLAG_4 | GIFLAG_8 | GIFLAG_16)) ==
			(GIFLAG_4 | GIFLAG_8 | GIFLAG_16);
	}
	void proc2();
};

struct Globals {
	byte _field0;
	byte _field1;
	uint16 _field2;
	uint16 _field4;
	uint16 _field6;

	GlobalItem _items[118];

	bool load();

	GlobalItem &operator[](uint idx) {
		return _items[idx];
	}
};

} // namespace Data
} // namespace FOD
} // namespace Wasteland

#endif
