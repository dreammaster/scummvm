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

#include "common/endian.h"
#include "common/file.h"
#include "ultima/ultima5/data/look_names.h"

namespace Ultima {
namespace Ultima5 {
namespace Data {

constexpr int NAME_COUNT = 512;
constexpr uint MAX_NAME_LENGTH = 0x80;

bool LookNames::load() {
	Common::File f;
	if (!f.open("look2.dat") || f.size() < NAME_COUNT * 2)
		return false;

	_data.resize(f.size());
	return f.read(&_data[0], _data.size()) == _data.size();
}

Common::String LookNames::nameAt(int index) const {
	assert(index >= 0 && index < NAME_COUNT);
	Common::String result;

	for (uint pos = READ_LE_UINT16(&_data[index * 2]); pos < _data.size() && _data[pos] != 0 &&
			result.size() < MAX_NAME_LENGTH; ++pos)
		result += (char)_data[pos];

	return result;
}

} // namespace Data
} // namespace Ultima5
} // namespace Ultima
