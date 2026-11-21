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

#include "ultima/ultima5/views/interactions/dungeon_climb.h"
#include "ultima/ultima5/logic/dungeon_logic.h"
#include "ultima/ultima5/views/keys.h"
#include "ultima/ultima5/ultima5.h"

namespace Ultima {
namespace Ultima5 {
namespace Views {
namespace Interactions {

constexpr int KEY_SPACE = ' ';
constexpr int KEY_UP = 3;
constexpr int KEY_DOWN = 4;

DungeonClimb::DungeonClimb(MessageLog &log) : Interaction(log) {
	bool hook;
	DungeonLogic::climbOptions(_up, _down, hook);

	if (_up && _down) {
		_log.print("Klimb-U/D-");
		_asking = true;
	} else if (_up || _down) {
		_log.print("Klimb-");
		go();
	} else if (hook) {
		_log.print("Klimb-\nWith What?\n");
	} else {
		_log.print("Klimb-what?\n");
	}
}

void DungeonClimb::go() {
	DungeonLogic logic(_log);
	_time = true;

	if ((_up && logic.climb(-1)) || (!_up && _down && logic.climb(1)))
		logic.leave();
}

bool DungeonClimb::keypress(const KeypressMessage &msg) {
	const int key = originalKey(msg);

	switch (key) {
	case KEY_UP:
	case 'U':
		_down = false;
		break;

	case KEY_DOWN:
	case 'D':
		_up = false;
		break;

	case KEY_SPACE:
		_log.print("Pass\n\n");
		_up = _down = false;
		_time = true;
		_asking = false;
		return true;

	default:
		return false;
	}

	_asking = false;
	go();
	return true;
}

} // namespace Interactions
} // namespace Views
} // namespace Ultima5
} // namespace Ultima
