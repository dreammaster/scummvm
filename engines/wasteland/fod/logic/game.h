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

#ifndef WASTELAND_FOD_LOGIC_GAME_H
#define WASTELAND_FOD_LOGIC_GAME_H

#include "common/rect.h"
#include "common/serializer.h"

namespace Wasteland {
namespace FOD {
namespace Logic {

enum Direction {
	DIR_NORTH = 1, DIR_SOUTH = 2, DIR_EAST = 3, DIR_WEST = 4,
	DIR_NONE = 5
};

enum PersonId {
	PERSON_PARTY = 1, PERSON_2 = 2, PERSON_4 = 4
};

class Game {
protected:
	uint16 _personIcons[21];
	int _personIconsSection = 0;
	Direction _currentDir = DIR_NONE;
	Common::Point _oldMapPos;

	/**
	 * Sets the slot for the party in the person icons array
	 * based on the new direction the party is facing
	 */
	void setPartyIcon(Direction dir);

	/**
	 * Move the party in the specified direction
	 */
	void move(Direction dir, bool flag);

	void nothing(bool flag);

	void moved(int mapX, int mapY);

	/**
	 * Show a movement message
	 */
	virtual void showMoveMessage(int msgNum) = 0;

public:
	Game();
	virtual ~Game() {}
};

} // namespace Logic
} // namespace FOD
} // namespace Wasteland

#endif
