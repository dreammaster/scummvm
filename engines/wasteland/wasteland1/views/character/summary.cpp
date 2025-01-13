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

#include "wasteland/wasteland1/views/character/summary.h"
#include "wasteland/wasteland1/wasteland1.h"
#include "wasteland/wasteland1/data/text.h"

namespace Wasteland {
namespace Wasteland1 {
namespace Views {
namespace Character {

void Summary::draw() {
	Pane::draw();

	Surface s = getSurface();
	const auto &c = *g_engine->_saved._currentCharacter;
	s.writeString("NAME: ");
	s.writeString(c._name);

	s.setTextPos(0, 2);
	s.writeString(Common::String::format("ST =%3d IQ =%3d LK =%3d",
		c._strength, c._iq, c._luck));
	s.setTextPos(0, 3);
	s.writeString(Common::String::format("SP =%3d AGL=%3d DEX=%3d",
		c._speed, c._agility, c._dexterity));
	s.setTextPos(0, 4);
	s.writeString(Common::String::format("CHR=%3d SKP=%3d",
		c._charisma, c._skillPoints));

	s.setTextPos(0, 5);
	s.writeString("RANK: ");
	s.writeString(c._rank);
	s.setTextPos(0, 6);
	s.writeString(Common::String::format("MAXCON=%d", c._maxCon));
	s.setTextPos(0, 7);
	s.writeString(Common::String::format("$=%d", c._money));
	s.setTextPos(0, 8);
	s.writeString("SEX: ");
	s.writeString(Data::TEXT_STRINGS[160 + c._gender]);
	s.setTextPos(0, 9);
	s.writeString("NATIONALITY: ");
	s.writeString(Data::TEXT_STRINGS[143 + c._nationality]);
}

} // namespace Character
} // namespace Views
} // namespace Wasteland1
} // namespace Wasteland
