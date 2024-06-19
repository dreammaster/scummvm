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

#include "common/algorithm.h"
#include "wasteland/core/serializer.h"

namespace Wasteland {

void Serializer::syncAsInt3(int &val) {
	byte tmp[3];

	if (isLoading()) {
		syncBytes(tmp, 3);
		val = (int)tmp[0] | ((int)tmp[1] << 8) | ((int)tmp[2] << 16);
	} else {
		tmp[0] = val & 0xff;
		tmp[1] = (val >> 8) & 0xff;
		tmp[2] = (val >> 16) & 0xff;
		syncBytes(tmp, 3);
	}
}

void Serializer::syncAsString(Common::String &str, size_t size) {
	char *tmp = new char[size];

	if (isLoading()) {
		syncBytes((byte *)tmp, size);
		tmp[size - 1] = '\0';
		str = Common::String(tmp);
	} else {
		Common::fill(tmp, tmp + size, 0);
		Common::strcpy_s(tmp, size, str.c_str());
		syncBytes((byte *)tmp, 14);
	}

	delete[] tmp;
}

} // namespace Wasteland
