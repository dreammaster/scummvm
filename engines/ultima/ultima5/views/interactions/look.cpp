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

#include "ultima/ultima5/views/interactions/look.h"
#include "ultima/ultima5/ultima5.h"

namespace Ultima {
namespace Ultima5 {
namespace Views {
namespace Interactions {

// Parts of large things are tiles that point to the next part
constexpr byte TILE_PART_NORTH = 0xE0;
constexpr byte TILE_PART_EAST = 0xE1;
constexpr byte TILE_PART_WEST = 0xE2;

// Terrain that has to be described in other words
static const char *const SPECIAL_NAME = "*";

bool Look::keypress(const KeypressMessage &msg) {
	const DirectionChooser::Result result = _chooser.handleKey(msg, _log);
	if (result == DirectionChooser::PENDING)
		return false;
	if (result == DirectionChooser::CANCELLED)
		return true;

	static const int8 DX[] = { 0, 1, 0, -1 };
	static const int8 DY[] = { -1, 0, 1, 0 };

	const Data::Savegame &sg = _G(savegame);
	byte x = sg._mapX + DX[_chooser.direction()];
	byte y = sg._mapY + DY[_chooser.direction()];
	byte tile = g_engine->getMapTile(x, y);

	_log.print("\nThou dost see\n");

	// An object stands in front of what's beneath it
	for (int i = 1; i < Data::ACTOR_COUNT; ++i) {
		const Data::ActorRecord actor = sg.getActor(i);
		if (actor._x == x && actor._y == y && actor._z == sg._mapLevel && actor._tile != 0) {
			_log.print(_G(lookNames).objectName(actor._tile) + "\n");
			return true;
		}
	}

	while (tile == TILE_PART_NORTH || tile == TILE_PART_EAST || tile == TILE_PART_WEST) {
		if (tile == TILE_PART_NORTH)
			--y;
		else if (tile == TILE_PART_EAST)
			++x;
		else
			--x;

		tile = g_engine->getMapTile(x, y);
	}

	const Common::String name = _G(lookNames).tileName(tile);
	if (name != SPECIAL_NAME)
		_log.print(name);

	_log.print("\n");
	return true;
}

} // namespace Interactions
} // namespace Views
} // namespace Ultima5
} // namespace Ultima
