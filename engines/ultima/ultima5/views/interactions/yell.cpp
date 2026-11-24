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

#include "ultima/ultima5/views/interactions/yell.h"
#include "ultima/ultima5/data/shrine_data.h"
#include "ultima/ultima5/logic/shrine_logic.h"
#include "ultima/ultima5/ultima5.h"

namespace Ultima {
namespace Ultima5 {
namespace Views {
namespace Interactions {

constexpr int MAX_WORD = 30;
constexpr int MAX_MEDITATION = 15;
constexpr int MANTRA_COUNT = 3;
constexpr int FIRST_LOCATION_MAP = 1;
constexpr int LAST_LOCATION_MAP = 0x20;

Yell::Yell(MessageLog &log) : Interaction(log) {
	_log.print("what?\n:");
}

void Yell::askMantra() {
	_log.print("\nMantra:");
	_line.clear();
	_state = STATE_MANTRA;
}

// Sees if the meditation was right, and if so raises the shrine
void Yell::conclude() {
	if (_focused && Data::SHRINE_X[_shrine] == _x && Data::SHRINE_Y[_shrine] == _y) {
		ShrineLogic::restore(_shrine, _x, _y);
		_log.print("\n\nThe Shrine is\nrestored!\n");
		g_engine->shake();
	} else {
		_log.printChar('\n');
	}

	_state = STATE_DONE;
}

bool Yell::keypress(const KeypressMessage &msg) {
	switch (_state) {
	case STATE_WORD: {
		if (!_line.handleKey(msg, _log, MAX_WORD))
			return false;

		if (_line.text().empty()) {
			_log.print("Nothing\n");
			return true;
		}

		_log.printChar('\n');

		const byte map = _G(savegame)._mapId;
		if (map >= FIRST_LOCATION_MAP && map <= LAST_LOCATION_MAP) {
			_passes = !ShrineLogic::callShadowlord(_line.text().c_str(), _log);
			return true;
		}

		if (map != 0) {
			_log.print("\nNo effect!\n");
			return true;
		}

		if (ShrineLogic::speak(_line.text().c_str(), _log, _shrine, _x, _y) != ShrineLogic::SPOKEN_RUINS)
			return true;

		_log.print("\nUpon what virtue\ndost thou\nmeditate?\n\n:");
		_line.clear();
		_state = STATE_VIRTUE;
		return false;
	}

	case STATE_VIRTUE:
		if (!_line.handleKey(msg, _log, MAX_MEDITATION))
			return false;

		_focused = ShrineLogic::find(Data::SHRINE_VIRTUES[_shrine], _line.text().c_str()) != -1;
		_mantras = 0;
		askMantra();
		return false;

	case STATE_MANTRA:
		if (!_line.handleKey(msg, _log, MAX_MEDITATION))
			return false;

		if (ShrineLogic::find(Data::SHRINE_MANTRAS[_shrine], _line.text().c_str()) == -1)
			_focused = false;

		if (++_mantras < MANTRA_COUNT) {
			askMantra();
			return false;
		}

		conclude();
		return true;

	default:
		return true;
	}
}

} // namespace Interactions
} // namespace Views
} // namespace Ultima5
} // namespace Ultima
