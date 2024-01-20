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
#include "wasteland/fod/logic/game.h"
#include "wasteland/fod/fod.h"

namespace Wasteland {
namespace FOD {
namespace Logic {

/**
 * Table of offsets into tile data for the party facing in
 * different directions
 */
static const uint16 PARTY_ICONS_TABLE[2][4] = {
	{ 0x1E70, 0x1E68, 0x1E78, 0x1E80 },
	{ 0x1E90, 0x1E88, 0x1E98, 0x1EA0 }
};

/**
 * Table of offsets into tile data for different characters
 */
static const uint16 PERSON_ICONS[21] = {
	0x0000, 0x1E68, 0x1E60, 0x1F38, 0x1E48, 0x1E50,
	0x1E58, 0x1EC8, 0x1ED0, 0x1ED8, 0x1EE0, 0x1EE8,
	0x1EF0, 0x1EF8, 0x1F00, 0x1F08, 0x1F10, 0x1F18,
	0x1F20, 0x1F28, 0x1F30
};

Game::Game() {
	Common::copy(PERSON_ICONS, PERSON_ICONS + 21, _personIcons);
}

void Game::setPartyIcon(Direction dir) {
	if (dir != DIR_NONE) {
		_currentDir = dir;
		_personIcons[1] = PARTY_ICONS_TABLE[_personIconsSection][dir - 1];
	}
}

void Game::move(Direction dir, bool flag) {
	static const int8 DELTAS[5][2] = { { 0, 0 }, { 0, -1 }, { 0, 1 }, { 1, 0 }, { -1, 0 } };
	const auto &map = g_engine->_disk._map;

	// Weird decision in the original, but there was a minor random
	// chance of hiccuping, and moving in a random direction
	if (!flag && g_engine->_disk1.isPartyAlive()) {
		if (g_engine->getRandomNumber(1, 100) < 10) {
			dir = (Direction)g_engine->getRandomNumber(4);
			showMoveMessage(-1);
		}
	}

	// Figure out new position
	int newX = g_engine->_disk1._mapPosX + DELTAS[dir][0];
	int newY = g_engine->_disk1._mapPosY + DELTAS[dir][1];

	// Set the party icon to show facing in the movement direction
	setPartyIcon(dir);

	if (newX < 0 || newY < 0 || newX >= map._width || newY >= map._height) {
		// Show optional message for trying to leave edge of map
		int msgNum = g_engine->_disk._map._edgeMessage;
		showMoveMessage(msgNum);

	} else {

		// TODO:
		g_engine->_disk1._mapPosX = newX;
		g_engine->_disk1._mapPosY = newY;
	}
}

} // namespace Logic
} // namespace FOD
} // namespace Wasteland
