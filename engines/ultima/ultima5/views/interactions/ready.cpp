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

#include "ultima/ultima5/views/interactions/ready.h"
#include "ultima/ultima5/views/keys.h"
#include "ultima/ultima5/ultima5.h"

namespace Ultima {
namespace Ultima5 {
namespace Views {
namespace Interactions {

constexpr int WINDOW_TOP = 1;
constexpr int INDICATOR_ROW = 10;
constexpr int KEY_ESCAPE = 27;
constexpr int KEY_ENTER = 13;
constexpr int KEY_SPACE = ' ';

Ready::Ready(MessageLog &log) : Interaction(log) {
	_log.print("Player: ");
}

bool Ready::keypress(const KeypressMessage &msg) {
	Data::Savegame &sg = _G(savegame);

	if (_choosing) {
		const PartyMemberChooser::Result result = _chooser.handleKey(msg);
		if (result == PartyMemberChooser::PENDING)
			return false;

		if (result == PartyMemberChooser::CANCELLED) {
			_log.print("None!\n");
			return true;
		}

		_who = _chooser.index();
		_log.print(sg._party[_who]._name);

		if (_log.column() != 0)
			_log.printChar('\n');

		_list.start(InventoryLogic::LIST_ARMAMENTS, sg._equipment, InventoryLogic::ARMAMENTS, _who);
		if (_list.empty()) {
			_log.print("Thou art empty-\nhanded!\n");
			return true;
		}

		_log.print("Item: ");
		_choosing = false;
		return false;
	}

	const int key = originalKey(msg);

	switch (key) {
	case KEY_ENTER:
	case KEY_SPACE:
		if (_list.selectedItem() >= 0) {
			const InventoryLogic::EquipResult result = InventoryLogic::equip(sg, _who, _list.selectedItem());
			_log.print(result._message);

			if (result._ringVanished)
				return true;
		}
		break;

	case KEY_ESCAPE:
		_log.print("Done\n");
		return true;

	default:
		_list.scroll(key);
		break;
	}

	_list.redraw();
	return false;
}

void Ready::draw(View &view) {
	if (_choosing) {
		_chooser.draw(view);
		return;
	}

	view.drawLabel(_G(savegame)._party[_who]._name);
	view.drawTextGrid(_list.grid(), WINDOW_TOP);
	view.drawScrollArrows(INDICATOR_ROW, _list.arrows());
}

} // namespace Interactions
} // namespace Views
} // namespace Ultima5
} // namespace Ultima
