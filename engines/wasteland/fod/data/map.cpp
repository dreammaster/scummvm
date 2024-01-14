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
	s.syncAsByte(_field2);
	s.syncAsByte(_field3);
	s.syncAsUint16LE(_field4);
	s.syncAsUint16LE(_field6);
}

void Map::synchronize(Common::Serializer &s) {
	s.syncAsUint16LE(_width);
	s.syncAsUint16LE(_height);
	s.syncAsUint16LE(_flags);
	s.syncBytes(_unknown, 1022);

	s.syncAsUint16LE(_offset1);
	s.syncAsUint16LE(_offset2);
	s.syncAsUint16LE(_offset3);
	s.syncAsUint16LE(_offset4);
	s.syncAsUint16LE(_offset5);
	s.syncAsUint16LE(_field40e);
	s.syncAsUint16LE(_field410);
	s.syncAsUint16LE(_field412);

	if (s.isLoading()) {
		_tiles.resize(_width * _height);
		for (uint i = 0; i < _tiles.size(); ++i)
			_tiles[i].synchronize(s);
	}
}

} // namespace Data
} // namespace FOD
} // namespace Wasteland
