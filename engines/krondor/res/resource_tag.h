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

#ifndef KRONDOR_RES_RESOURCE_TAG_H
#define KRONDOR_RES_RESOURCE_TAG_H

#include "common/hashmap.h"
#include "common/stream.h"

namespace Krondor {

class ResourceTag {
private:
	Common::HashMap<uint, Common::String> _tagMap;

public:
	ResourceTag() {}
	void load(Common::SeekableReadStream *src);
	bool find(uint id, Common::String &name) const;
};

} // namespace Krondor

#endif
