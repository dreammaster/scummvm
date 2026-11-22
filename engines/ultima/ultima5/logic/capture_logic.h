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

#ifndef ULTIMA5_LOGIC_CAPTURE_LOGIC_H
#define ULTIMA5_LOGIC_CAPTURE_LOGIC_H

#include "common/scummsys.h"
#include "common/str.h"

namespace Ultima {
namespace Ultima5 {

/**
 * The party's capture in the palace of Blackthorn, who has them brought to a cell and asks of them
 * the mantra of a shrine
 */
class CaptureLogic {
public:
	// The movements that are acted out in the cell
	static const byte SCRIPT_RELEASE[];
	static const byte SCRIPT_THREAT[];
	static const byte SCRIPT_ARRIVE[];
	static const byte SCRIPT_ORDER[];
	static const byte SCRIPT_LEAVE[];

	// Where the members of the party are chained, by how many there are
	static const byte CHAIN_ORDER[48];
	static const byte CHAIN_X[6];
	static const byte CHAIN_Y[6];

	/**
	 * Gives the text of the scene that is at an offset of the file of them
	 */
	static Common::String message(int offset);

	/**
	 * Gives the first shrine whose mantra has not been taken from the party, or -1
	 */
	static int shrineToTake();

	/**
	 * Gives what is asked of the party, which the last time is without the name of a virtue
	 */
	static Common::String question(int number, int shrine);

	/**
	 * Tells if what was said holds the mantra of a shrine
	 */
	static bool isMantra(const Common::String &answer, int shrine);

	/**
	 * Has the shrine be lost, and the party be the less for it
	 */
	static void takeShrine(int shrine);

	/**
	 * Has the party's second fit member killed and gone from it
	 * @returns		The name of whoever it was
	 */
	static Common::String sacrifice();

	/**
	 * Tells how many of the party are alive
	 */
	static int aliveCount();

	/**
	 * Puts the party back in the palace once it's over, with the keys gone
	 */
	static void release();
};

} // namespace Ultima5
} // namespace Ultima

#endif
