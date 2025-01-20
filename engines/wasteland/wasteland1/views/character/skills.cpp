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

#include "wasteland/wasteland1/views/character/skills.h"
#include "wasteland/wasteland1/wasteland1.h"
#include "wasteland/wasteland1/data/text.h"

namespace Wasteland {
namespace Wasteland1 {
namespace Views {
namespace Character {

Skills::Skills() : ReorderPane("CharacterSkills") {
}

void Skills::resetLines() {
	const Data::Skills &skills = g_engine->_currentChar->_skills;
	setClickable(false);

	for (uint i = 1; i <= skills.size(); ++i) {
		if (!skills[i]._level)
			break;

		addLine(Common::String::format("%3d %s", skills[i]._level,
			Data::TEXT_STRINGS[skills[i]._id]));
	}
}

void Skills::draw() {
	ReorderPane::draw();

	Surface s = getSurface();
	s.writeString("LVL   SKILL", 3, 0);
}

bool Skills::msgGame(const GameMessage &msg) {
	if (ReorderPane::msgGame(msg))
		return true;

	if (msg._name == "Next") {
		close();
		return true;
	}

	return false;
}

} // namespace Character
} // namespace Views
} // namespace Wasteland1
} // namespace Wasteland
