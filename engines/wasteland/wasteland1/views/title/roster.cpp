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

#include "wasteland/wasteland1/views/title/roster.h"
#include "wasteland/wasteland1/gfx/pics.h"
#include "wasteland/wasteland1/wasteland1.h"

namespace Wasteland {
namespace Wasteland1 {
namespace Views {
namespace Title {

Roster::Roster() : Dialog("Roster"),
		_create(this, "Create", "CREATE", 11, 24),
		_delete(this, "Delete", "DELETE", 18, 24),
		_play(this, "Play", "PLAY", 25, 24),
		_animation("RosterAnimation", this, 3, 0),
		_roster1(this, 1),
		_roster2(this, 2),
		_roster3(this, 3),
		_roster4(this, 4),
		_roster5(this, 5),
		_roster6(this, 6) {
	_roster[1] = &_roster1;
	_roster[2] = &_roster2;
	_roster[3] = &_roster3;
	_roster[4] = &_roster4;
	_roster[5] = &_roster5;
	_roster[6] = &_roster6;

	for (int i = 1; i <= 6; ++i)
		_children.push_back(_roster[i]);
}

void Roster::draw() {
	static unsigned char HEADER_ROW1[] = {
		0x9B, 0x57, 0x5B, 0x57, 0x9E, 0x9C, 0x9C, 0x9C,
		0x50, 0x53, 0x52, 0x51, 0x9C, 0x9C, 0x9C, 0x9C,
		0x9E, 0x53, 0x55, 0x9E, 0x53, 0x52, 0x52, 0x9E,
		0x52, 0x53, 0x54, 0x9E, 0x55, 0x56, 0x50, 0x9E,
		0x60, 0x51, 0x53, 0x61, 0x56, 0x50, 0x9F, 0x9A,
		0x00
	};
	static unsigned char HEADER_ROW2[] = {
		0xA1, 0x95, 0x99, 0x95, 0xA4, 0xA2, 0xA2, 0xA2,
		0x8C, 0x8F, 0x8E, 0x8D, 0xA2, 0xA2, 0xA2, 0xA2,
		0xA4, 0x8F, 0x91, 0xA4, 0x8F, 0x8E, 0x8E, 0xA4,
		0x8E, 0x8F, 0x90, 0xA4, 0x91, 0x94, 0x8C, 0xA4,
		0x92, 0x8D, 0x8F, 0x93, 0x94, 0x8C, 0xA5, 0xA0,
		0x00
	};

	Surface s = getSurface();
	s.clear();

	drawFrame(Common::Rect(0, 0, 39, 24));
	drawFrame(Common::Rect(0, 0, 39, 23));
	drawFrame(Common::Rect(0, 0, 13, 13));
	drawFrame(Common::Rect(14, 0, 39, 13));

	// Draw the location name below the animation
	const auto &saved = g_engine->_saved;
	const auto &saved2 = saved._saved2;
	s.writeString(saved._saveLocationName, 1, 12);

	// Write out header row buttons
	s.setFont(1);
	HEADER_ROW1[1] = 0x57 + saved2._activePartyGroup;
	HEADER_ROW2[1] = 0x57 + 0x3e + saved2._activePartyGroup;
	HEADER_ROW1[3] = 0x57 + saved2._totalPartyGroups;
	HEADER_ROW2[3] = 0x57 + 0x3e + saved2._totalPartyGroups;

	s.writeString(HEADER_ROW1, 0, 14);
	s.writeString(HEADER_ROW2, 0, 15);

	Dialog::draw();
}

bool Roster::msgGame(const GameMessage &msg) {
	if (msg._name == "SELECT_MEMBER") {
		// Select new member
		for (int i = 1; i <= 6; ++i)
			_roster[i]->setSelected(i == msg._value);

		addView("CharacterSummary");		// Show character summary
		return true;
	}

	return false;
}

} // namespace Title
} // namespace Views
} // namespace Wasteland1
} // namespace Wasteland
