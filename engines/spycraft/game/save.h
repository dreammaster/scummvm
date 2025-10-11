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

#ifndef SPYCRAFT_GAME_SAVE_H
#define SPYCRAFT_GAME_SAVE_H

#include "spycraft/game/object.h"
#include "spycraft/game/game.h"

namespace Spycraft {

class SaveGame : public Object {
public:
	SaveGame();
	int save(char *);
	int restore(char *);
};

class RestRoom : public Room {
public:
	char *theNextRoom;
	char *nextGame;
	RestRoom(char *oldGame);
	void init();
};

} // namespace Spycraft

#endif
