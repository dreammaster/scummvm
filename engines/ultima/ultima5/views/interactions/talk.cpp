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

#include "ultima/ultima5/views/interactions/talk.h"
#include "ultima/ultima5/ultima5.h"

namespace Ultima {
namespace Ultima5 {
namespace Views {
namespace Interactions {

constexpr byte TILE_BED = 0xAB;
constexpr byte TILE_MIRROR = 0x9D;

// Furniture that is between a person and whoever they're talking to
static const byte COUNTER_TILES[] = {
	0x29, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0xA5, 0xAE, 0xBA, 0xBB, 0xBE, 0xCA, 0xCB
};

bool Talk::isCounter(byte tile) {
	for (uint i = 0; i < ARRAYSIZE(COUNTER_TILES); ++i) {
		if (COUNTER_TILES[i] == tile)
			return true;
	}

	return false;
}

int Talk::objectAt(int x, int y) {
	const Data::Savegame &sg = _G(savegame);

	for (int i = 1; i < Data::ACTOR_COUNT; ++i) {
		const Data::ActorRecord actor = sg.getActor(i);
		if (actor._x == x && actor._y == y && actor._z == sg._mapLevel)
			return actor._tile != 0 ? i : 0;
	}

	return 0;
}

int Talk::personOf(int object) {
	const Data::Savegame &sg = _G(savegame);

	for (int i = 0; i < Data::NPC_COUNT; ++i) {
		const Data::NpcState state = sg.getNpcState(i);
		if (state._object == object && sg._npcTypes[i] != 0 && state._present != 0)
			return i;
	}

	return -1;
}

bool Talk::keypress(const KeypressMessage &msg) {
	if (_talking)
		return _logic.keypress(msg);

	const DirectionChooser::Result result = _chooser.handleKey(msg, _log);
	if (result == DirectionChooser::PENDING)
		return false;
	if (result == DirectionChooser::CANCELLED)
		return true;

	const Data::Savegame &sg = _G(savegame);
	if (sg._mapId == 0) {
		_log.print("Funny, no response!\n");
		return true;
	}

	static const int8 DX[] = { 0, 1, 0, -1 };
	static const int8 DY[] = { -1, 0, 1, 0 };

	const int dx = DX[_chooser.direction()];
	const int dy = DY[_chooser.direction()];
	int x = sg._mapX + dx;
	int y = sg._mapY + dy;

	// Someone behind a counter is talked to across it
	if (objectAt(x, y) == 0 && isCounter(g_engine->getMapTile(x, y))) {
		x += dx;
		y += dy;
	}

	const int object = objectAt(x, y);
	if (object == 0) {
		_log.print("\nNobody's here!\n");
		return true;
	}

	switch (g_engine->getMapTile(x, y)) {
	case TILE_BED:
		_log.print("\n\"Zzzzzz...\"\n");
		return true;

	case TILE_MIRROR:
		_log.print("\nNo response!\n");
		return true;

	default:
		break;
	}

	const int npc = personOf(object);
	if (npc < 0) {
		_log.print("No response!\n");
		return true;
	}

	if (_logic.start(npc))
		return true;

	_talking = true;
	return false;
}

} // namespace Interactions
} // namespace Views
} // namespace Ultima5
} // namespace Ultima
