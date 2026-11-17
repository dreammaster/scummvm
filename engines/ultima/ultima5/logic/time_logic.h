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

#ifndef ULTIMA5_LOGIC_TIME_LOGIC_H
#define ULTIMA5_LOGIC_TIME_LOGIC_H

#include "ultima/ultima5/data/savegame.h"

namespace Ultima {
namespace Ultima5 {

/**
 * The passing of time in the game: the clock and calendar, and what
 * depends on the hour, like the light
 */
class TimeLogic {
private:
	/**
	 * Works out how bright it is from the hour and the light the party carries
	 */
	static void updateLight(Data::Savegame &savegame);

public:
	/**
	 * Lets some minutes go by. Zero just refreshes the things that depend on the time
	 */
	static void advance(Data::Savegame &savegame, int minutes);
};

} // namespace Ultima5
} // namespace Ultima

#endif
