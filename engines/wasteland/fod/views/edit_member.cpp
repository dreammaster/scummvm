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

#include "wasteland/fod/views/edit_member.h"
#include "wasteland/fod/fod.h"

namespace Wasteland {
namespace FOD {
namespace Views {

EditMember::EditMember() : MenuView("EditMember"),
	_mainArea(0, 14, 39, 19) {
}

bool EditMember::msgGame(const GameMessage& msg) {
	if (msg._name == "ADD_MEMBER") {
		addMember();
		return true;

	} else if (msg._name == "EDIT_MEMBER") {
		editMember(msg._value);
		return true;
	}

	return false;
}

void EditMember::addMember() {
	addView("EditMember");
}

void EditMember::editMember(int rosterNum) {
	_rosterNum = rosterNum;
	addView("EditMember");
}

bool EditMember::msgKeypress(const KeypressMessage &msg) {
	
	return true;
}

void EditMember::draw() {
	MenuView::draw();
	writePortraitText("Yo");

	Surface main = getSurface(_mainArea);
//	main.writeCenteredString("Choose a function:", 1);

//	UIElement::draw();
}

} // namespace Views
} // namespace FOD
} // namespace Wasteland
