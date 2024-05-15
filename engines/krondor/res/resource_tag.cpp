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

#include "krondor/res/resource_tag.h"

namespace Krondor {

void ResourceTag::load(Common::SeekableReadStream *src) {
	int n = src->readUint16LE();

	for (int i = 0; i < n; i++) {
		uint16 id = src->readUint16LE();
		Common::String name = src->readString();
		_tagMap[id] = name;
	}
}

bool ResourceTag::find(const unsigned int id, Common::String &name) const {
	if (_tagMap.contains(id)) {
		name = _tagMap[id];
		return true;
	}

	return false;
}

} // namespace Krondor
