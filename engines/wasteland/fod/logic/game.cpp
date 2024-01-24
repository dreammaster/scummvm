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

Game *Game::_owner;
bool Game::_flag;

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
	Data::Disk1 &disk1 = g_engine->_disk1;
	auto &map = g_engine->_disk._map;

	// Weird decision in the original, but there was a minor random
	// chance of hiccuping, and moving in a random direction
	if (!flag && disk1.isPartyAlive()) {
		if (g_engine->getRandomNumber(1, 100) < 10) {
			dir = (Direction)g_engine->getRandomNumber(4);
			showMoveMessage(-1);
		}
	}

	// Figure out new position
	int newX = disk1._mapPosX + DELTAS[dir][0];
	int newY = disk1._mapPosY + DELTAS[dir][1];

	// Set the party icon to show facing in the movement direction
	setPartyIcon(dir);

	if (newX < 0 || newY < 0 || newX >= map._width || newY >= map._height) {
		// Show optional message for trying to leave edge of map
		int msgNum = g_engine->_disk._map._edgeMessage;
		showMoveMessage(msgNum);
		nothing(false);

	} else {
		_currMapNum = map._mapNum;
		_oldMapPos = Common::Point(disk1._mapPosX, disk1._mapPosY);
		_newMapPos = Common::Point(newX, newY);
		_owner = this;
		_flag = flag;

		g_engine->_scripts.execute(&map._tiles[newX][newY]._actionId, 1, 0,
			[]() { _owner->move2(); });		
	}
}

void Game::move2() {
	auto &map = g_engine->_disk._map;
	Data::Disk1 &disk1 = g_engine->_disk1;

	// Only do further movement if the map/position hasn't changed,
	// considering we haven't applied our own move change yet
	if (map._mapNum == _currMapNum && disk1._mapPosX == _oldMapPos.x &&
		disk1._mapPosY == _oldMapPos.y) {
		// Scan for people at the new position
		uint personNum = 0;
		for (personNum = 0; personNum < map._people.size(); ++personNum) {
			const auto &person = map._people[personNum];
			if (person._mapX == _newMapPos.x && person._mapY == _newMapPos.y)
				break;
		}

		if (personNum < map._people.size()) {
			// Moving onto a person, to trigger conversation
			auto &person = map._people[personNum];
			person._field50 &= ~0x80;
			g_engine->_scripts.execute(&person._talkId, 6, personNum,
				[]() {
					_owner->nothing(_owner->_flag);
				});

		} else {
			if (canMove(_newMapPos.x, _newMapPos.y))
				moveTo(_newMapPos.x, _newMapPos.y);

			// TODO: method calls
			g_engine->_scripts.execute(&map._tiles[_newMapPos.x][_newMapPos.y]._actionId, 0, 0,
				[]() { _owner->move3(); });
		}

	} else {
		nothing(_flag);
		moved(disk1._mapPosX, disk1._mapPosY);
	}
}

void Game::move3() {
	Data::Disk1 &disk1 = g_engine->_disk1;

	if (!_flag)
		checkForTrouble();

	moved(disk1._mapPosX, disk1._mapPosY);
}

bool Game::canMove(int newX, int newY) const {
	const auto &map = g_engine->_disk._map;

	if (newX < 0 || newY < 0 || newX >= map._width || newY >= map._height)
		return false;

	const auto &tile = map._tiles[newX][newY];
	if (!(tile._flags & 1))
		return false;

	// Check if there's any person on the cell
	for (uint i = 0; i < map._peopleCount; ++i) {
		const auto &person = map._people[i];
		if (person._mapX == newX && person._mapY == newY)
			return false;
	}

	// Scan for any other obstruction
	for (uint i = 0; i < map._count4; ++i) {
		const auto &entry = map._array4[i];
		if ((entry._flags & 0xf) && entry._mapX == newX && entry._mapY == newY)
			return false;
	}

	return true;
}

void Game::moveTo(int newX, int newY) {
	auto &disk1 = g_engine->_disk1;
	disk1._mapPosX = newX;
	disk1._mapPosY = newY;

	_gameVal1 = 0;
	_gameVal2 = 0;
}

void Game::nothing(bool flag) {

}

void Game::moved(int mapX, int mapY) {

}

void Game::checkForTrouble() {

}

} // namespace Logic
} // namespace FOD
} // namespace Wasteland
