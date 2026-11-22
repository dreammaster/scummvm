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

#ifndef ULTIMA5_LOGIC_ENDGAME_LOGIC_H
#define ULTIMA5_LOGIC_ENDGAME_LOGIC_H

#include "common/array.h"
#include "common/str.h"

namespace Ultima {
namespace Ultima5 {

/**
 * What the end of the game says: what Lord British tells the party, and the certificate of
 * the quest with the date it was done on
 */
class EndgameLogic {
public:
	// A line of the certificate
	struct Line {
		Common::String _text;
		bool _runes = false;
		bool _inverse = false;
	};

	/**
	 * Gives what Lord British says, from the file of it
	 * @param offset	Where it is, from the start
	 */
	static Common::String message(int offset);

	/**
	 * Gives the tile that a member of the party is shown with
	 */
	static int memberTile(char cls);

	/**
	 * Makes the lines of the certificate, which says when it was done and how long it took
	 */
	static Common::Array<Line> certificate();
};

} // namespace Ultima5
} // namespace Ultima

#endif
