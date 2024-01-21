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

#ifndef WASTELAND_FOD_DATA_MAP_H
#define WASTELAND_FOD_DATA_MAP_H

#include "common/serializer.h"

namespace Wasteland {
namespace FOD {
namespace Data {

enum MapFlag { MAPFLAG_8000 = 0x8000 };

struct Map {
	struct MapTile {
		uint16 _id = 0;
		byte _flags = 0;
		byte _field3 = 0;
		uint16 _field4 = 0;
		uint16 _field6 = 0;

		void synchronize(Common::Serializer &s);
	};
	struct MapPerson {
		Common::String _name;
		byte _field50 = 0;
		byte _mapX = 0;
		byte _mapY = 0;
		uint16 _talkId = 0;

		void synchronize(Common::Serializer &s);
	};
	struct MapEntry4 {
		byte _field1;
		byte _mapX;
		byte _mapY;
		byte _field5;
		byte _flags;
		byte _fieldC;

		void synchronize(Common::Serializer &s);
	};

private:
	void synchronizeCore(Common::Serializer &s);

public:
	uint16 _mapNum = 0;
	uint16 &_mapPosX;
	uint16 &_mapPosY;

	uint16 _width = 0;
	uint16 _height = 0;
	uint16 _flags = 0;
	uint16 _field6 = 0;
	byte _field8 = 0;
	byte _peopleCount = 0;
	byte _fieldA = 0;
	byte _count4 = 0;
	byte _fieldC = 0;
	byte _edgeMessage = 0;
	byte _unknown[1014];

	uint16 _offset1 = 0;
	uint16 _offsetPeople = 0;
	uint16 _offset3 = 0;
	uint16 _offset4 = 0;
	uint16 _offset5 = 0;
	uint16 _field40e = 0;
	uint16 _field410 = 0;
	uint16 _field412 = 0;

	Common::Array<MapTile> _tiles;
	Common::Array<MapPerson> _people;
	Common::Array<MapEntry4> _array4;

	const MapTile *_mapTileTopLeft = nullptr;

	Map(uint16 &mapX, uint16 &mapY);

	void load(Common::SeekableReadStream &src);
};

} // namespace Data
} // namespace FOD
} // namespace Wasteland

#endif
