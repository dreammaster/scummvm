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

#ifndef ULTIMA5_LOGIC_LOCATION_LOGIC_H
#define ULTIMA5_LOGIC_LOCATION_LOGIC_H

#include "ultima/ultima5/logic/world_logic.h"
#include "ultima/ultima5/views/message_log.h"

namespace Ultima {
namespace Ultima5 {

/**
 * Going in and out of the towns, castles, keeps and dwellings, and
 * moving about in them
 */
class LocationLogic {
private:
	Views::MessageLog &_log;

	/**
	 * Goes into the location the party is at the entrance to
	 * @param kind		What the entrance is, for the message
	 * @returns			True if time passes
	 */
	bool goIn(const char *kind);

public:
	enum WalkResult {
		WALK_BLOCKED,
		WALK_MOVED,
		WALK_AT_EDGE
	};

	LocationLogic(Views::MessageLog &log) : _log(log) {}

	/**
	 * Makes sure the map of the level of the location the party is in is loaded
	 * @returns		False if it can not be loaded
	 */
	static bool load();

	/**
	 * Changes the drawbridges and the lit windows at dusk and dawn
	 */
	static void updateForHour();

	/**
	 * Sets up a location the party has just come to, with the people who are
	 * in it at this time of day
	 * @returns		False if its people couldn't be loaded
	 */
	static bool arrive();

	/**
	 * Goes into the town, castle, keep or dwelling the party is on the entrance to
	 * @returns		True if time passes
	 */
	bool enter();

	/**
	 * Moves the party a step, or reports why it can't. At the edge of the map
	 * the party can leave, but it's for the player to decide
	 */
	WalkResult walk(Direction direction);

	/**
	 * Leaves the location for the map of Britannia or the underworld
	 */
	void leave();
};

} // namespace Ultima5
} // namespace Ultima

#endif
