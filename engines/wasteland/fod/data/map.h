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

	uint16 _width = 0;
	uint16 _height = 0;
	uint16 _flags = 0;
	byte _unknown[1022];

	uint16 _offset1 = 0;
	uint16 _offset2 = 0;
	uint16 _offset3 = 0;
	uint16 _offset4 = 0;
	uint16 _offset5 = 0;
	uint16 _field40e = 0;
	uint16 _field410 = 0;
	uint16 _field412 = 0;

	Common::Array<MapTile> _tiles;

	const MapTile *_mapTileTopLeft = nullptr;

	void synchronize(Common::Serializer &s);
};

} // namespace Data
} // namespace FOD
} // namespace Wasteland

#endif
