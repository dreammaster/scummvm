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

#include "common/system.h"
#include "engines/util.h"
#include "mads/madsv2/engine.h"
#include "mads/madsv2/core/mouse.h"
#include "mads/madsv2/phantom/main.h"
#include "mads/core/sound.h"

namespace MADS {
namespace MADSV2 {

constexpr int GAME_FRAME_RATE = 50;
constexpr int GAME_FRAME_TIME = 1000 / GAME_FRAME_RATE;

MADSV2Engine *g_engine;

MADSV2Engine::MADSV2Engine(OSystem *syst, const MADSGameDescription *gameDesc) :
		MADSEngine(syst, gameDesc) {
	g_engine = this;
}

MADSV2Engine::~MADSV2Engine() {
	g_engine = nullptr;
	delete _screen;
	delete _soundManager;
}

void MADSV2Engine::pollEvents() {
	// Check for screen update time
	uint32 time = g_system->getMillis();
	if (time >= _nextFrameTime) {
		_screen->update();
		_nextFrameTime = time + GAME_FRAME_TIME;
	}

	// Poll for events
	Common::Event e;
	while (g_system->getEventManager()->pollEvent(e) && !shouldQuit()) {
		bool isMouse = false;
		switch (e.type) {
		case Common::EVENT_LBUTTONDOWN:
			mouse_buttons |= 1;
			isMouse = true;
			break;
		case Common::EVENT_LBUTTONUP:
			mouse_buttons &= ~1;
			isMouse = true;
			break;
		case Common::EVENT_RBUTTONDOWN:
			mouse_buttons |= 2;
			isMouse = true;
			break;
		case Common::EVENT_RBUTTONUP:
			mouse_buttons &= ~2;
			isMouse = true;
			break;
		case Common::EVENT_MBUTTONDOWN:
			mouse_buttons |= 4;
			isMouse = true;
			break;
		case Common::EVENT_MBUTTONUP:
			mouse_buttons &= ~4;
			isMouse = true;
			break;
		default:
			break;
		}

		if (isMouse) {
			mouse_x = e.mouse.x;
			mouse_y = e.mouse.y;
		}

		if (e.type == Common::EVENT_KEYDOWN)
			_keyEvents.push(e);
	}
}

bool MADSV2Engine::hasPendingKey() {
	pollEvents();

	return !_keyEvents.empty();
}

int MADSV2Engine::getKey() {
	pollEvents();

	if (!_keyEvents.empty()) {
		Common::Event e = _keyEvents.pop();
		return (e.kbd.keycode & 0xff) | 0x100;
	}

	return 0;
}

void MADSV2Engine::flushKeys() {
	pollEvents();

	_keyEvents.clear();
}

uint32 MADSV2Engine::getMillis() {
	pollEvents();
	return g_system->getMillis();
}


} // namespace MADSV2
} // namespace MADS
