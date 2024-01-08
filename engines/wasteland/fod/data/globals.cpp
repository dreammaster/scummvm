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

#include "common/file.h"
#include "wasteland/fod/data/globals.h"

namespace Wasteland {
namespace FOD {
namespace Data {

bool Globals::load() {
	Common::File f;
	if (!f.open("GLOBALS"))
		return false;

	_field0 = f.readByte();
	_field1 = f.readByte();
	_field2 = f.readUint16LE();
	_field4 = f.readUint16LE();
	_field6 = f.readUint16LE();
	f.skip(34);

	for (int i = 0; i < GLOBALS_ITEMS_COUNT; ++i)
		_items[i].load(f);

	return true;
}

void GlobalItem::load(Common::SeekableReadStream &src) {
	char buf[19];
	src.read(buf, 18);
	buf[18] = '\0';
	_name = Common::String(buf);

	_field12 = src.readUint16LE();
	_field14 = src.readByte();
	_ac = src.readByte();
	_field16 = src.readByte();
	_field17 = src.readByte();
}

} // namespace Data
} // namespace FOD
} // namespace Wasteland
