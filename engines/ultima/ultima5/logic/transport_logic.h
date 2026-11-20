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

#ifndef ULTIMA5_LOGIC_TRANSPORT_LOGIC_H
#define ULTIMA5_LOGIC_TRANSPORT_LOGIC_H

#include "ultima/ultima5/logic/world_logic.h"

namespace Ultima {
namespace Ultima5 {

/**
 * Getting on and off the things the party can travel in, which are horses, flying
 * carpets, skiffs and frigates, and moving them: turning, the wind, and what happens
 * when a ship hits something
 */
class TransportLogic {
private:
	Views::MessageLog &_log;

	/**
	 * Finds the object at a place on the level the party is on, giving its tile and which it is, or zero
	 */
	static int objectAt(int x, int y, int &index);
	bool onFoot();
	bool canBoardShip();
	bool landNearby() const;
	static bool isLandNear(int x, int y);

public:
	TransportLogic(Views::MessageLog &log) : _log(log) {}

	/**
	 * Gets on what the party is standing on
	 * @returns		False if it wasn't anything to get on, so no time went by
	 */
	bool board();

	/**
	 * Gets off what the party is on
	 */
	void getOff();

	/**
	 * Turns what the party is in to face the way it's being steered, if it has to be
	 * @returns		True if that was all that was done, as the vehicle turns before it goes
	 */
	bool steer(Direction direction);

	/**
	 * Works out whether what the party is in can move a step, saying why if it can't, and
	 * what happens to a ship that's run into something
	 */
	bool canMove(int dx, int dy);

	/**
	 * Hurts the ship the party is in, which can sink, or the party if it's in something else
	 */
	void damage();

	/**
	 * Returns what a ship that's sailing is heading, as one of the directions, or -1 if it isn't
	 */
	static int heading();

	/**
	 * Lets a moment of sailing go by
	 * @param planTick	The ship's plans make time go twice as slow every other moment; this is
	 *					kept between calls
	 * @returns			True if it's time for the ship to move on a step, rather than wait
	 */
	static bool sailTick(bool &planTick);

	/**
	 * Puts the ship that's been bought, if there's one, where it's meant to wait
	 */
	static void placePurchasedShip();

	/**
	 * Makes it so that a heading is only kept while sailing
	 */
	static void clearHeadingIfNotSailing();
};

} // namespace Ultima5
} // namespace Ultima

#endif
