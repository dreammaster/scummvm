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

#include "ultima/ultima5/views/interactions/camp.h"
#include "ultima/ultima5/logic/camp_logic.h"
#include "ultima/ultima5/views/interactions/combat.h"
#include "ultima/ultima5/views/keys.h"
#include "ultima/ultima5/ultima5.h"

namespace Ultima {
namespace Ultima5 {
namespace Views {
namespace Interactions {

constexpr int KEY_SPACE = ' ';
constexpr byte TILE_FRIGATE_FIRST = 0x20;
constexpr byte TILE_FRIGATE_DOCKED = 0x24;

Camp::Camp(MessageLog &log) : Interaction(log) {
	const Data::Savegame &sg = _G(savegame);

	_log.print("Hole up & ");

	if ((sg._partyTile & 0xF8) == TILE_FRIGATE_FIRST) {
		_log.print("\nrepair...\n\n");

		if (sg._partyTile < TILE_FRIGATE_DOCKED) {
			_log.print("Sails must be\nlowered!\n\n");
		} else {
			MonsterLogic::Result attack;

			if (!CampLogic::repairShip(_log, attack) && attack._fight)
				_next = Combat::forObject(_log, attack._actor);
		}

		return;
	}

	_log.print("camp!\n\n");

	if (CampLogic::check(_log) == CampLogic::BLOCK_NONE) {
		_log.print("For how many hours? (1-9) ");
		_state = STATE_HOURS;
	}
}

Camp::~Camp() {
	delete _next;
}

void Camp::rest(int guard) {
	Combat *combat = new Combat(_log);
	bool apparition = false;

	_state = STATE_DONE;
	if (!combat->startCamp(guard, _hours, apparition)) {
		delete combat;
		return;
	}

	if (!combat->done())
		_next = combat;
	else if (apparition)
		_next = new Apparition(_log);

	if (combat != _next)
		delete combat;
}

bool Camp::keypress(const KeypressMessage &msg) {
	const int key = originalKey(msg);

	switch (_state) {
	case STATE_HOURS:
		if (key != KEY_SPACE && (key < '0' || key > '9'))
			return false;

		_log.printChar((char)key);
		_log.printChar('\n');

		if (key == KEY_SPACE || key == '0')
			return true;

		_hours = key - '0';

		if (CampLogic::fitMembers() > 1) {
			_log.print("\nWilt thou set a watch? ");
			_state = STATE_WATCH;
			return false;
		}

		rest(-1);
		return true;

	case STATE_WATCH:
		if (key == 'N') {
			_log.print("No\n\n");
			rest(-1);
			return true;
		}

		if (key == 'Y') {
			_log.print("Yes\n\nWho will stand guard? ");
			_chooser = PartyMemberChooser();
			_state = STATE_GUARD;
		}
		return false;

	case STATE_GUARD: {
		const PartyMemberChooser::Result result = _chooser.handleKey(msg);
		if (result == PartyMemberChooser::PENDING)
			return false;

		_log.printChar('\n');

		int guard = result == PartyMemberChooser::CHOSEN ? _chooser.index() : -1;
		if (guard == -1 || _G(savegame)._party[guard]._status != Data::STATUS_GOOD) {
			guard = -1;
			_log.print("None posted!\n\n");
		}

		rest(guard);
		return true;
	}

	default:
		return true;
	}
}

void Camp::draw(View &view) {
	if (_state == STATE_GUARD)
		_chooser.draw(view);
}

Bed::Bed(MessageLog &log) : Interaction(log) {
	_log.print("For how many hours? ");
}

bool Bed::keypress(const KeypressMessage &msg) {
	const int key = originalKey(msg);

	if (key != KEY_SPACE && (key < '0' || key > '9'))
		return false;

	if (key != KEY_SPACE && key != '0') {
		_log.printChar((char)key);
		_log.printChar('\n');
		CampLogic::sleepInBed(_log, key - '0');
	}

	return true;
}

Apparition::Apparition(MessageLog &log) : Interaction(log) {
	_log.print("An apparition!\n");
	run();
}

void Apparition::run() {
	Data::Savegame &sg = _G(savegame);

	while (_member < sg._partySize) {
		Data::Character &c = sg._party[_member];
		const int member = _member++;

		if (c._status == Data::STATUS_DEAD)
			continue;

		c._hp = c._maxHp;
		c._status = Data::STATUS_GOOD;

		switch (c._class) {
		case 'A':
		case 'M':
			c._mag = c._int;
			break;
		case 'B':
			c._mag = c._int >> 1;
			break;
		}

		// Those that have grown are told, and a key is waited for
		if (CampLogic::levelUp(_log, member))
			return;
	}

	_log.print("\n\"");
	_log.print(CampLogic::karmaText());
	_log.print("\"");
	_state = STATE_KARMA;
}

bool Apparition::keypress(const KeypressMessage &) {
	if (_state == STATE_KARMA) {
		_log.print("\n\nThe strangely familiar old man vanishes...\n");
		_state = STATE_DONE;
		return true;
	}

	_log.printChar('\n');
	run();
	return _state == STATE_DONE;
}

} // namespace Interactions
} // namespace Views
} // namespace Ultima5
} // namespace Ultima
