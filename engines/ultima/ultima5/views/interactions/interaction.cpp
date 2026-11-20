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

#include "ultima/ultima5/views/interactions/interaction.h"
#include "ultima/ultima5/ultima5.h"

namespace Ultima {
namespace Ultima5 {
namespace Views {
namespace Interactions {

// The party names are drawn in rows at the top right, and a row is highlighted
// by inverting its pixels
constexpr int PARTY_LEFT = 0xC0;
constexpr int PARTY_RIGHT = 0x138;
constexpr int PARTY_TOP = 8;
constexpr int PARTY_ROW_HEIGHT = 8;
constexpr byte INVERT_MASK = 0x0F;

// The label is set in the border above the party
constexpr int LABEL_COLUMN = 27;
constexpr int LABEL_TEXT_COLUMN = 28;
constexpr int LABEL_END_COLUMN = 35;

DirectionChooser::Result DirectionChooser::handleKey(const KeypressMessage &msg, MessageLog &log) {
	switch (msg.keycode) {
	case Common::KEYCODE_UP:
		_direction = DIR_NORTH;
		log.print("North\n");
		return CHOSEN;
	case Common::KEYCODE_RIGHT:
		_direction = DIR_EAST;
		log.print("East\n");
		return CHOSEN;
	case Common::KEYCODE_DOWN:
		_direction = DIR_SOUTH;
		log.print("South\n");
		return CHOSEN;
	case Common::KEYCODE_LEFT:
		_direction = DIR_WEST;
		log.print("West\n");
		return CHOSEN;
	case Common::KEYCODE_ESCAPE:
	case Common::KEYCODE_SPACE:
		log.print("Pass\n");
		return CANCELLED;
	default:
		return PENDING;
	}
}

PartyMemberChooser::Result PartyMemberChooser::handleKey(const KeypressMessage &msg) {
	const int partySize = _G(savegame)._partySize;

	switch (msg.keycode) {
	case Common::KEYCODE_ESCAPE:
		return CANCELLED;
	case Common::KEYCODE_RETURN:
	case Common::KEYCODE_KP_ENTER:
	case Common::KEYCODE_SPACE:
		return CHOSEN;
	case Common::KEYCODE_LEFT:
	case Common::KEYCODE_UP:
		_index = MAX(_index - 1, 0);
		return PENDING;
	case Common::KEYCODE_RIGHT:
	case Common::KEYCODE_DOWN:
		_index = MIN(_index + 1, partySize - 1);
		return PENDING;
	default:
		break;
	}

	if (_allowAll && msg.ascii == '0')
		return ALL;

	const int number = msg.ascii - '1';
	if (number >= 0 && number < MIN(partySize, 6))
		_index = number;

	return PENDING;
}

void PartyMemberChooser::draw(View &view) const {
	auto s = view.getSurface();

	const int top = PARTY_TOP + _index * PARTY_ROW_HEIGHT;
	for (int y = top; y < top + PARTY_ROW_HEIGHT; ++y) {
		byte *p = (byte *)s.getBasePtr(PARTY_LEFT, y);

		for (int x = PARTY_LEFT; x < PARTY_RIGHT; ++x, ++p)
			*p ^= INVERT_MASK;
	}

	view.drawCaps(LABEL_COLUMN * 8, 0, true);
	view.print(LABEL_TEXT_COLUMN, 0, "Select:", COLOR_WHITE, COLOR_BLACK);
	view.drawCaps(LABEL_END_COLUMN * 8, 0, false);
}

} // namespace Interactions
} // namespace Views
} // namespace Ultima5
} // namespace Ultima
