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
#include "wasteland/fod/data/weapons.h"

namespace Wasteland {
namespace FOD {
namespace Data {

bool Weapons::load() {
	Common::File f;
	if (!f.open("WEAPONS"))
		return false;

	clear();
	resize(f.readUint16LE());

	for (uint i = 0; i < size(); ++i) {
		char buf[0x3c];
		f.read(buf, 0x3c);

		Weapon &w = (*this)[i];
		buf[15] = '\0';
		w._name = Common::String((const char *)buf);
		w._flags = buf[0x18];
		w._field38 = READ_LE_UINT16(buf + 0x38);
		w._field3a = READ_LE_UINT16(buf + 0x3a);
	}

	return true;
}

} // namespace Data
} // namespace FOD
} // namespace Wasteland
