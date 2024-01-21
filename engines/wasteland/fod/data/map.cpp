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
#include "wasteland/fod/data/map.h"

namespace Wasteland {
namespace FOD {
namespace Data {

void Map::MapTile::synchronize(Common::Serializer &s) {
	s.syncAsUint16LE(_id);
	s.syncAsByte(_flags);
	s.syncAsByte(_field3);
	s.syncAsUint16LE(_field4);
	s.syncAsUint16LE(_field6);
}

void Map::MapPerson::synchronize(Common::Serializer &s) {
	byte buf[0x68];
	s.syncBytes(buf, 0x68);

	if (s.isLoading()) {
		buf[14] = '\0';
		_name = Common::String((const char *)buf);

		_field50 = buf[0x50];
		_mapX = buf[0x5b];
		_mapY = buf[0x5c];
		_talkId = READ_LE_UINT16(&buf[0x66]);
	}
}

void Map::MapEntry4::synchronize(Common::Serializer &s) {
	byte buf[0x66];
	s.syncBytes(buf, 0x66);

	if (s.isLoading()) {
		_field1 = buf[1];
		_mapX = buf[2];
		_mapY = buf[3];
		_field5 = buf[5];
		_flags = buf[9];
		_fieldC = buf[12];
	}
}

void Map::synchronizeCore(Common::Serializer &s) {
	s.syncAsUint16LE(_width);
	s.syncAsUint16LE(_height);
	s.syncAsUint16LE(_flags);
	s.syncAsUint16LE(_field6);
	s.syncAsByte(_field8);
	s.syncAsByte(_peopleCount);
	s.syncAsByte(_fieldA);
	s.syncAsByte(_count4);
	s.syncAsByte(_fieldC);
	s.syncAsByte(_edgeMessage);
	s.syncBytes(_unknown, 1014);

	s.syncAsUint16LE(_offset1);
	s.syncAsUint16LE(_offsetPeople);
	s.syncAsUint16LE(_offset3);
	s.syncAsUint16LE(_offset4);
	s.syncAsUint16LE(_offset5);
	s.syncAsUint16LE(_field40e);
	s.syncAsUint16LE(_field410);
	s.syncAsUint16LE(_field412);

	if (s.isLoading())
		_tiles.resize(_width * _height);

	for (uint i = 0; i < _tiles.size(); ++i)
		_tiles[i].synchronize(s);
}

void Map::load(Common::SeekableReadStream &src) {
	// Get the core data and tiles
	Common::Serializer s(&src, nullptr);
	synchronizeCore(s);

	// Load the specials
	_people.clear();
	_people.resize(_peopleCount);
	src.seek(_offsetPeople);

	for (uint i = 0; i < _peopleCount; ++i)
		_people[i].synchronize(s);

	// Load array 4
	_array4.clear();
	_array4.resize(_count4);
	src.seek(_offset4);

	for (uint i = 0; i < _count4; ++i)
		_array4[i].synchronize(s);
}

} // namespace Data
} // namespace FOD
} // namespace Wasteland
