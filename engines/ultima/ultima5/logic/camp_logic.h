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

#ifndef ULTIMA5_LOGIC_CAMP_LOGIC_H
#define ULTIMA5_LOGIC_CAMP_LOGIC_H

#include "common/str.h"
#include "ultima/ultima5/logic/monster_logic.h"
#include "ultima/ultima5/views/message_log.h"

namespace Ultima {
namespace Ultima5 {

/**
 * Holing up in the open, to rest or to mend a ship
 */
class CampLogic {
public:
	// Why the party can't camp where it is
	enum Block {
		BLOCK_NONE,
		BLOCK_WATER,
		BLOCK_ON_FOOT
	};

	/**
	 * Says whether the party can camp where it is, and says why if not
	 */
	static Block check(Views::MessageLog &log);

	/**
	 * Gives how many of the party are fit to take part
	 */
	static int fitMembers();

	/**
	 * Mends a ship over a few turns, if the sails are lowered
	 * @param attack	What the monsters did in that time if they came to blows
	 * @returns			False if it was broken off
	 */
	static bool repairShip(Views::MessageLog &log, MonsterLogic::Result &attack);

	/**
	 * Sleeps in a bed of a town for some hours, which the people of the town are going about during, until
	 * someone takes the bed
	 * @returns		False if somebody came for the party before it slept
	 */
	static bool sleepInBed(Views::MessageLog &log, int hours);

	/**
	 * Raises a member of the party to the level their experience is worth, and says so
	 * @returns		True if there was a change
	 */
	static bool levelUp(Views::MessageLog &log, int member);

	/**
	 * What the apparition says, which depends on the karma
	 */
	static Common::String karmaText();
};

} // namespace Ultima5
} // namespace Ultima

#endif
