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

#include "wasteland/wasteland1/views/character/use_item.h"
#include "wasteland/wasteland1/wasteland1.h"
#include "wasteland/wasteland1/data/text.h"

namespace Wasteland {
namespace Wasteland1 {
namespace Views {
namespace Character {

UseItem::UseItem() : Pane("CharacterUseItem"),
		_esc(this, "Escape", "ESC", 18, 0, KEYBIND_ESCAPE),
		_enter(nullptr, "Enter", "ENTER", 22, 13, Common::KEYCODE_RETURN) {
}

void UseItem::draw() {
	Pane::draw();

	Surface s = getSurface();

	if (_mode == INITIAL || _mode == NO_TRADE) {
		s.writeString("Do you want to:");
		s.writeString("Drop", 0, 2);
		s.writeString("Trade", 0, 3);
		s.writeString("Equip/unequip", 0, 4);
	}

	if (_mode == NO_TRADE)
		s.writeString("No one to trade with.", 0, 6);

	if (_mode == TRADE)
		s.writeString("Who wants it?");
}

bool UseItem::msgGame(const GameMessage &msg) {
	if (msg._name == "UseItem") {
		_selectedItem = msg._value;
		_mode = INITIAL;
		addView();
		return true;
	}

	return Pane::msgGame(msg);
}

bool UseItem::msgKeypress(const KeypressMessage &msg) {
	switch (_mode) {
	case INITIAL:
		switch (msg.keycode) {
		case Common::KEYCODE_d:
			drop();
			break;

		case Common::KEYCODE_t:
			trade();
			break;

		case Common::KEYCODE_e:
			equip();
			break;

		default:
			return Pane::msgKeypress(msg);
		}

		return true;

	case TRADE:
		if (msg.keycode >= Common::KEYCODE_1 && msg.keycode <= Common::KEYCODE_9) {
			uint charNum = msg.keycode - Common::KEYCODE_1 + 1;

			if (charNum <= g_engine->_party.size() &&
					g_engine->_party[charNum] != g_engine->_currentChar) {

			}

		}
	}

	return false;
}

void UseItem::drop() {
	g_engine->_currentChar->_items.remove_at(_selectedItem);
	_selectedItem = 0;
	close();
}

void UseItem::trade() {
	if (g_engine->_party.size() == 1) {
		_mode = NO_TRADE;
	} else {
		_mode = TRADE;
	}

	redraw();
}

void UseItem::equip() {
}

} // namespace Character
} // namespace Views
} // namespace Wasteland1
} // namespace Wasteland
