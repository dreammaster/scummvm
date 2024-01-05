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

#ifndef WASTELAND_FOD_DATA_ARCHETYPES_H
#define WASTELAND_FOD_DATA_ARCHETYPES_H

#include "common/stream.h"

namespace Wasteland {
namespace FOD {
namespace Data {

#define PROFESSIONS_COUNT 5

struct Profession {
	Common::String _name;
	byte _attributes[7];
	byte _attributePoints;
	byte _activeSkills[16];
	byte _passiveSkills[16];
	uint16 _unkMin, _unkMax;
	byte _field7B;

	void load(Common::SeekableReadStream &src);
};

struct Archetypes {
	Profession _professions[PROFESSIONS_COUNT];

	bool load();
};

} // namespace Data
} // namespace FOD
} // namespace Wasteland

#endif // WASTELAND_H
