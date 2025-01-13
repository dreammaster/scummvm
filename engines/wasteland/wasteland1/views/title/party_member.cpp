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

#include "wasteland/wasteland1/views/title/party_member.h"
#include "wasteland/wasteland1/wasteland1.h"
#include "wasteland/wasteland1/data/text.h"

namespace Wasteland {
namespace Wasteland1 {
namespace Views {
namespace Title {

PartyMember::PartyMember(UIElement *parent, uint memberIndex) :
		ClickableText(parent, Common::String::format("TitleMember%d", memberIndex)),
		_memberIndex(memberIndex) {
	setBounds(Common::Rect(FONT_W, (15 + _memberIndex) * FONT_H,
		320 - FONT_W, (16 + _memberIndex) * FONT_H));
}

void PartyMember::setSelected(bool val, bool shouldRedraw) {
	_selected = val;

	if (shouldRedraw)
		draw();
}

void PartyMember::draw() {
	static const byte CONDITION_CHARS[] = {
		0x85, 0x9A, 0x9B, 0x9C, 0x9D, 0x84
	};

	Surface s = getSurface();
	s.clear();

	if (!isValid())
		return;

	s.setInverseColor(_selected);
	s.writeChar('0' + _memberIndex, 0, 0);
	s.writeChar('>');
	s.setInverseColor(false);

	auto &saved = g_engine->_saved;
	saved.setCurrentCharacter(_memberIndex);
	const auto &member = *saved._currentCharacter;

	s.writeString(member._name);
	s.setTextPos(16, 0);
	s.writeString(Common::String::format("%2d", member._ac));

	int weapon = member._weapon;
	int ammunition = 0;

	if (weapon) {
		auto &invItem = member._items[weapon];
		if (!invItem.hasNoAmmunition() && invItem._quantity)
			ammunition = invItem._quantity & 63;
	}
	s.writeString(Common::String::format("%4d", ammunition));

	// Write the max constitution. If the member is afflicted,
	// show it in inverse colors
	s.setTextPos(23, 0);
	if (member._afflictions)
		s.setInverseColor(true);
	s.writeString(Common::String::format("%3d", member._maxCon));
	s.setInverseColor(false);

	// Write the current constitution
	s.setTextPos(27, 0);
	if (member._con < 0) {
		s.writeChar(CONDITION_CHARS[member.getConditionIndex()]);
	} else if (member._con == 0) {
		s.writeChar(CONDITION_CHARS[5]);
	} else {
		s.writeString(Common::String::format("%3d", member._con));
	}

	// Write out the weapon
	s.setTextPos(31, 0);
	if (weapon) {
		auto &invItem = member._items[weapon];
		s.setInverseColor(invItem._quantity < 0);	// reverse if broken
		s.writeString(Data::getText(36 + invItem._id));
		s.setInverseColor(false);
	}
}

bool PartyMember::msgMouseDown(const MouseDownMessage &msg) {
	if (isValid())
		_parent->send(GameMessage("SELECT_MEMBER", _memberIndex));
	return true;
}

bool PartyMember::msgKeypress(const KeypressMessage &msg) {
	if (isValid() && msg.keycode == (Common::KeyCode)(Common::KEYCODE_1 + _memberIndex - 1)) {
		_parent->send(GameMessage("SELECT_MEMBER", _memberIndex));
		return true;
	}

	return false;
}

bool PartyMember::isValid() const {
	// Party members don't have a _text filled out, since we do custom drawing.
	// So treat the member as valid if it's within the size of the party
	auto &saved = g_engine->_saved;
	return _memberIndex <= saved._saved2._membersInGroup;
}

} // namespace Title
} // namespace Views
} // namespace Wasteland1
} // namespace Wasteland
