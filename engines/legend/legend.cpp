/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include "legend/legend.h"
#include "common/scummsys.h"
#include "common/config-manager.h"
#include "common/debug-channels.h"

namespace Legend {

LegendEngine *_vm;

LegendEngine::LegendEngine(OSystem *syst, const LegendGameDescription *gameDesc) :
		Engine(syst), _gameDescription(gameDesc), _randomSource("Legend") {
	_vm = this;
	_debugger = nullptr;
	_events = nullptr;
	_res = nullptr;
	_scene = nullptr;
	_screen = nullptr;
}

LegendEngine::~LegendEngine() {
	delete _debugger;
	delete _events;
	delete _picFile;
	delete _res;
	delete _scene;
	delete _screen;
}

void LegendEngine::initialize() {
	DebugMan.addDebugChannel(kDebugLevelScript,      "scripts", "Script debug level");

	Font::init();
	Message::init(this);
	_debugger = Debugger::init(this);
	_events = new Events(this);
	_picFile = new PicFile();
	_res = new Resources(this);
	_scene = new Scene();
	_screen = Screen::init(this);
}

void LegendEngine::deinitialize() {
	Font::deinit();
}

Common::Error LegendEngine::run() {
	// Initialize the engine
	initialize();

	playGame();

	deinitialize();
	return Common::kNoError;
}

void LegendEngine::playGame() {
	Picture *pic = _picFile->load(31);
	_screen->blitFrom(*pic);
	_screen->setPalette();
	delete pic;

	while (!shouldQuit()) {
		_events->pollEventsAndWait();
	}
}

Common::String LegendEngine::getGameFilePrefix() const {
	switch (getGameID()) {
	case GType_Gateway:
		return "gate";
	case GType_CompanionsOfXanth:
		return "xanth";
	default:
		return "";
	}
}

} // End of namespace Legend
