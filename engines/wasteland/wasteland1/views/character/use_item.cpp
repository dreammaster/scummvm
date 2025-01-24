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
	_enter.addKeycode(Common::KEYCODE_SPACE);
}

void UseItem::draw() {
	Pane::draw();

	Surface s = getSurface();
	int msgRow = _canReload ? 7 : 6;

	if (_mode == INITIAL || _mode == NO_TRADE || _mode == JAMMED) {
		s.writeString("Do you want to:");
		s.writeString("Drop", 0, 2);
		s.writeString("Trade", 0, 3);
		s.writeString("Equip/unequip", 0, 4);
		if (_canReload)
			s.writeString("Reload", 0, 5);
	} else if (_mode == TRADE) {
		s.writeString("Who wants it?");
	}

	if (_mode == NO_TRADE)
		s.writeString("No one to trade with.", 0, msgRow);
	if (_mode == JAMMED) {
		s.writeString("Your weapon is jammed.", 0, msgRow);
	}
}

bool UseItem::msgGame(const GameMessage &msg) {
	if (msg._name == "UseItem") {
		show(msg._value);
		return true;
	}

	return Pane::msgGame(msg);
}

void UseItem::show(int selectedItem) {
	_selectedItem = selectedItem;
	_mode = INITIAL;

	const Data::InventoryItem &item = g_engine->_currentChar->_items[selectedItem];
	const Data::ItemDetails &weapon = *g_engine->_currentChar->getEquippedWeaponDetails();
	_canReload = weapon._ammunitionId == item._id;

	addView();
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

		case Common::KEYCODE_r:
			if (_canReload)
				reload();
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
				trade(charNum);
			}
			return true;
		} else {
			return Pane::msgKeypress(msg);
		}

	default:
		break;
	}

	return false;
}

bool UseItem::msgUnfocus(const UnfocusMessage &msg) {
	// Ensure that the Enter key isn't visible, and Esc is
	_children.remove(&_esc);
	if (!_children.contains(&_enter))
		_children.push_back(&_enter);

	return Pane::msgUnfocus(msg);
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

void UseItem::reload() {
	auto *weapon = g_engine->_currentChar->getEquippedWeapon();
	const Data::InventoryItem &item = g_engine->_currentChar->_items[_selectedItem];
	const auto *ammunition = item.getItemDetails();
	assert(weapon);

	if (weapon->isJammed()) {
		_mode = JAMMED;
		_children.remove(&_esc);
		_children.push_back(&_enter);
	} else {
		// Replace the weapon's quantity with the ammunition's clip size
		weapon->_quantity = (weapon->_quantity & ~0x3f) | ammunition->_clipSize;
		g_engine->_currentChar->_items.remove_at(_selectedItem);
		close();
	}
}

void UseItem::trade(int destCharNum) {

}

} // namespace Character
} // namespace Views
} // namespace Wasteland1
} // namespace Wasteland
