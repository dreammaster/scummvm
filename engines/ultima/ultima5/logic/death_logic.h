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

#ifndef ULTIMA5_LOGIC_DEATH_LOGIC_H
#define ULTIMA5_LOGIC_DEATH_LOGIC_H

#include "ultima/ultima5/views/message_log.h"

namespace Ultima {
namespace Ultima5 {

/**
 * What happens when the whole of the party has died: Lord British brings it back
 */
class DeathLogic {
private:
	Views::MessageLog &_log;

public:
	DeathLogic(Views::MessageLog &log) : _log(log) {}

	/**
	 * Tells if none of the party can act any more, as they are all dead
	 */
	static bool partyDead();

	/**
	 * Says what the party sees as it passes away, and what is said to it
	 */
	void begin();

	/**
	 * Brings the party back, and takes it to the castle of Lord British
	 */
	void finish();
};

} // namespace Ultima5
} // namespace Ultima

#endif
