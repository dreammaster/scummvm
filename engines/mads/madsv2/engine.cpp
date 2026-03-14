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
#include "mads/madsv2/phantom/main.h"

namespace MADS {
namespace MADSV2 {

MADSV2Engine *g_engine;

MADSV2Engine::MADSV2Engine(OSystem *syst, const MADSGameDescription *gameDesc) :
	Engine(syst), _gameDescription(gameDesc) {
	g_engine = this;
}

MADSV2Engine::~MADSV2Engine() {
	delete _screen;
	g_engine = nullptr;
}

Common::Error MADSV2Engine::run() {
	initGraphics(320, 200);
	_screen = new Graphics::Screen();

	Phantom::phantom_main();

	return Common::kNoError;
}

void MADSV2Engine::pollEvents() {
	Common::Event e;
	while (g_system->getEventManager()->pollEvent(e) && !shouldQuit())
		_events.push_back(e);
}

bool MADSV2Engine::hasPendingKey() {
	pollEvents();

	for (auto it = _events.begin(); it != _events.end(); ++it) {
		if (it->type == Common::EVENT_KEYDOWN)
			return true;
	}

	return false;
}

int MADSV2Engine::getKey() {
	pollEvents();

	for (auto it = _events.begin(); it != _events.end(); ++it) {
		if (it->type == Common::EVENT_KEYDOWN) {
			Common::Event e = *it;
			_events.erase(it);
			return (e.kbd.keycode & 0xff) | 0x100;
		}
	}

	return 0;
}

void MADSV2Engine::flushKeys() {
	pollEvents();

	for (auto it = _events.begin(); it != _events.end(); ) {
		if (it->type == Common::EVENT_KEYDOWN) {
			Common::Event e = *it;
			it = _events.erase(it);
		} else {
			++it;
		}
	}
}

} // namespace MADSV2
} // namespace MADS
