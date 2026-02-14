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

#include "ultima/ultima2/views/create_character.h"

namespace Ultima {
namespace Ultima2 {
namespace Views {

bool CreateCharacter::msgFocus(const FocusMessage &msg) {
	_pointsRemaining = POINTS_REMAINING;
	_cursor.show(Common::Point(26, 4), 2, true);
	return true;
}

void CreateCharacter::draw() {
	auto s = getSurface();
	s.clear();

	s.writeString(Common::Point(0, 0), "          PLAYER GENERATION :");
	s.writeString(Common::Point(0, 2), "     POINTS LEFT TO DISTRIBUTE : ");

	// TODO: Reverse text
	s.writeString(Common::String::format("%d", _pointsRemaining));

	s.writeString(Common::Point(0, 4), "           STRENGTH.......\n"
		"           AGILITY........\n"
		"           STAMINA........\n"
		"           CHARISMA.......\n"
		"           WISDOM.........\n"
		"           INTELLIGENCE...\n\n"
		"                M/F-\n"
		"               RACE-\n"
		"               TYPE-\n"
		"               NAME-\n\n"
		"          SATISFACTORY (Y/N)-\n\n"
		"        RACES:         TYPES:\n"
		"         1-HUMAN        1-FIGHTER\n"
		"         2-ELF          2-CLERIC\n"
		"         3-DWARF        3-WIZARD\n"
		"         4-HOBBIT       4-THIEF");
}

bool CreateCharacter::msgGame(const GameMessage &msg) {
	return true;
}

} // namespace Views
} // namespace Ultima2
} // namespace Ultima
