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

#include "wasteland/wasteland1/views/character/inventory.h"
#include "wasteland/wasteland1/wasteland1.h"
#include "wasteland/wasteland1/data/text.h"

namespace Wasteland {
namespace Wasteland1 {
namespace Views {
namespace Character {

Inventory::Inventory() : ReorderPane("CharacterInventory") {
}

void Inventory::resetLines() {
	const Data::InventoryItems &items = g_engine->_currentChar->_items;
	setClickable(true);
	clearLines();

	for (uint i = 1; i <= items.size(); ++i) {
		auto &item = items[i];
		if (!item._id)
			break;

		addLine(Data::getText(item._id + 36));
	}
}

void Inventory::reordered() {
	Data::InventoryItems &items = g_engine->_currentChar->_items;
	Data::InventoryItems temp;

	// Copy over items in new order
	for (uint i = 1; i <= _newOrder.size(); ++i)
		temp[i] = items[_newOrder[i]];

	// Overwrite with newly ordered list
	items = temp;

	resetLines();
	redraw();
}

void Inventory::draw() {
	ReorderPane::draw();

	Surface s = getSurface();
	s.writeString("ITEM", 4, 0);
}

bool Inventory::msgGame(const GameMessage &msg) {
	if (ReorderPane::msgGame(msg))
		return true;

	if (msg._name == "Divide" || msg._name == "Pool") {
		uint total = 0;
		for (uint i = 1; i <= g_engine->_party.size(); ++i)
			total += g_engine->_party[i]->_money;

		if (msg._name == "Pool") {
			for (uint i = 1; i <= g_engine->_party.size(); ++i)
				g_engine->_party[i]->_money = 0;
			g_engine->_currentChar->_money = total;
		} else {
			uint perTotal = total / g_engine->_party.size();
			for (uint i = 1; i <= g_engine->_party.size(); ++i) {
				g_engine->_party[i]->_money = (i == g_engine->_party.size()) ?
					total : perTotal;
				total -= perTotal;
			}
		}

		redraw();
		return true;
	} else if (msg._name == "Next") {
		replaceView("CharacterSkills");
		return true;

	} else if (msg._name == "Line") {
		send("CharacterUseItem", GameMessage("UseItem", msg._value));
		return true;
	}

	return false;
}


} // namespace Character
} // namespace Views
} // namespace Wasteland1
} // namespace Wasteland
