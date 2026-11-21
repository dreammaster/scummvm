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

#include "ultima/ultima5/views/interactions/dungeon_item.h"
#include "ultima/ultima5/logic/dungeon_logic.h"
#include "ultima/ultima5/views/keys.h"
#include "ultima/ultima5/ultima5.h"

namespace Ultima {
namespace Ultima5 {
namespace Views {
namespace Interactions {

constexpr int KEY_SPACE = ' ';
constexpr int KEY_LEFT = 1;
constexpr int KEY_RIGHT = 2;
constexpr int KEY_UP = 3;
constexpr int KEY_DOWN = 4;

DungeonItem::DungeonItem(MessageLog &log, Kind kind) : Interaction(log), _kind(kind) {
	Data::Savegame &sg = _G(savegame);
	const byte here = DungeonLogic::cell(sg._mapX, sg._mapY, sg._mapLevel);

	switch (_kind) {
	case KIND_GET:
		DungeonItems(_log).take();
		return;

	case KIND_OPEN:
		if ((here & 0xF0) == 0x40) {
			findWho();
		} else if ((here & 0xF0) == 0x70) {
			_log.print("Already Open!\n");
		} else {
			_log.print("What?\n");
		}
		return;

	case KIND_JIMMY:
		_log.print("\n");
		findWho();
		return;

	default:
		findWho();
		return;
	}
}

// Works out who does it: the one that has been picked to lead, or the only one that can
void DungeonItem::findWho() {
	const Data::Savegame &sg = _G(savegame);

	if (sg._activeChar != Data::NO_ACTIVE_CHARACTER) {
		_who = sg._activeChar;
		proceed();
		return;
	}

	int count = 0;
	for (int i = 0; i < sg._partySize; ++i) {
		if (sg._party[i]._status == Data::STATUS_GOOD || sg._party[i]._status == Data::STATUS_POISONED) {
			_who = i;
			++count;
		}
	}

	if (count > 1) {
		_log.print("Player: ");
		_chooser = PartyMemberChooser();
		_state = STATE_WHO;
		return;
	}

	if (count == 0)
		_who = -1;

	proceed();
}

void DungeonItem::proceed() {
	const Data::Savegame &sg = _G(savegame);
	DungeonItems items(_log);

	_state = STATE_DONE;

	if (_who == -1) {
		_log.print("None!\n");
		return;
	}

	switch (_kind) {
	case KIND_OPEN:
		items.open(_who);
		return;

	case KIND_JIMMY:
		items.jimmy(_who);
		return;

	default:
		break;
	}

	// Looking and searching need light, and a place to look
	if (sg._unk0300 == 0 && sg._unk0301 == 0) {
		_log.print(_kind == KIND_LOOK ? "You see:\ndarkness.\n" : "\nYou find:\ndarkness.\n");
		return;
	}

	_log.print("Dir-");
	_state = STATE_DIRECTION;
}

bool DungeonItem::keypress(const KeypressMessage &msg) {
	DungeonItems items(_log);
	const int key = originalKey(msg);

	switch (_state) {
	case STATE_WHO: {
		const PartyMemberChooser::Result result = _chooser.handleKey(msg);
		if (result == PartyMemberChooser::PENDING)
			return false;

		const Data::Savegame &sg = _G(savegame);
		if (result == PartyMemberChooser::CANCELLED) {
			_log.print("None!\n");
			return true;
		}

		const Data::Character &c = sg._party[_chooser.index()];
		if (c._status != Data::STATUS_GOOD && c._status != Data::STATUS_POISONED) {
			_log.print("Disabled!\n\n");
			_log.print("Player: ");
			_chooser = PartyMemberChooser();
			return false;
		}

		_log.print(c._name);
		if (_log.column() != 0)
			_log.printChar('\n');

		_who = _chooser.index();
		proceed();
		return _state == STATE_DONE;
	}

	case STATE_DIRECTION:
		if (key == KEY_SPACE) {
			_log.print("Pass\n");
			return true;
		}

		if (key != KEY_UP && key != KEY_DOWN && key != KEY_RIGHT && key != KEY_LEFT)
			return false;

		_log.print(key == KEY_UP ? "Ahead\n" : key == KEY_DOWN ? "Here\n" : key == KEY_RIGHT ? "Right\n" : "Left\n");
		DungeonItems::target(key, _x, _y);

		if (_kind == KIND_SEARCH) {
			items.search(_who, _x, _y);
			return true;
		}

		if (items.look(_x, _y)) {
			_state = STATE_DRINK;
			return false;
		}
		return true;

	case STATE_DRINK:
		if (key == 'N') {
			_log.print("No.\n");
			return true;
		}

		if (key == 'Y') {
			items.drink(_who, _x, _y);
			return true;
		}
		return false;

	default:
		return true;
	}
}

void DungeonItem::draw(View &view) {
	if (_state == STATE_WHO)
		_chooser.draw(view);
}

} // namespace Interactions
} // namespace Views
} // namespace Ultima5
} // namespace Ultima
