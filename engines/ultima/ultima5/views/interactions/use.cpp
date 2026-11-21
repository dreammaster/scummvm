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

#include "ultima/ultima5/views/interactions/use.h"
#include "ultima/ultima5/views/interactions/gem_view.h"
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

Use::Use(MessageLog &log) : Interaction(log), _logic(log) {
	InventoryLogic::specialItems(_G(savegame), _items);
	_list.start(InventoryLogic::LIST_ITEMS, _items, InventoryLogic::SPECIAL_ITEMS, InventoryLogic::NOTHING, true);

	if (_list.empty()) {
		_log.print("No usable items!\n");
		_state = STATE_DONE;
		return;
	}

	_log.print("Item: ");
}

Use::~Use() {
	delete _next;
}

bool Use::apply(UseLogic::Need need) {
	switch (need) {
	case UseLogic::NEED_VIEW:
		_next = new GemView(_log);
		_state = STATE_DONE;
		return true;

	case UseLogic::NEED_DIRECTION:
		_state = STATE_DIRECTION;
		return false;

	case UseLogic::NEED_MEMBER:
		_members.reset();
		_state = STATE_MEMBER;
		return false;

	default:
		_state = STATE_DONE;
		return true;
	}
}

bool Use::keypress(const KeypressMessage &msg) {
	switch (_state) {
	case STATE_PICKING: {
		const int key = originalKey(msg);

		if (key == KEY_ENTER || key == KEY_SPACE)
			return apply(_logic.use(_list.selectedItem()));

		if (key == KEY_ESCAPE) {
			_log.print("None!\n");
			_state = STATE_DONE;
			return true;
		}

		// Whatever key it is, the list is drawn again
		_list.scroll(key);
		_list.redraw();
		return false;
	}

	case STATE_DIRECTION: {
		const int key = originalKey(msg);

		if ((key >= 1 && key <= 4) || key == KEY_SPACE)
			return apply(_logic.directionChosen(key == KEY_SPACE ? 0 : key));

		return false;
	}

	case STATE_MEMBER: {
		const PartyMemberChooser::Result result = _members.handleKey(msg);
		if (result == PartyMemberChooser::PENDING)
			return false;

		return apply(_logic.memberChosen(result == PartyMemberChooser::CHOSEN ? _members.index() : -1));
	}

	default:
		return true;
	}
}

void Use::draw(View &view) {
	if (_state == STATE_MEMBER) {
		_members.draw(view);
	} else if (_state == STATE_PICKING) {
		view.drawLabel("Items:");
		view.drawTextGrid(_list.grid(), WINDOW_TOP);
		view.drawScrollArrows(INDICATOR_ROW, _list.arrows());
	}
}

} // namespace Interactions
} // namespace Views
} // namespace Ultima5
} // namespace Ultima
