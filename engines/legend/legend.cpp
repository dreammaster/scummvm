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

#include "common/scummsys.h"
#include "common/config-manager.h"
#include "common/debug-channels.h"
#include "legend/legend.h"
#include "legend/gateway/gateway_project_item.h"
#include "legend/xanth/xanth_project_item.h"

namespace Legend {

LegendEngine *g_vm;

LegendEngine::LegendEngine(OSystem *syst, const LegendGameDescription *gameDesc) :
		Engine(syst), _gameDescription(gameDesc), _randomSource("Legend") {
	g_vm = this;
	_debugger = nullptr;
	_events = nullptr;
	_gfx = nullptr;
	_mouseCursor = nullptr;
	_res = nullptr;
	_screen = nullptr;
	_window = nullptr;
}

LegendEngine::~LegendEngine() {
	delete _debugger;
	delete _events;
	delete _gfx;
	delete _mouseCursor;
	delete _picFile;
	delete _res;
	delete _screen;
	delete _window;
}

void LegendEngine::initialize() {
	DebugMan.addDebugChannel(kDebugLevelScript,      "scripts", "Script debug level");

	Font::init();
	_debugger = Debugger::init(this);
	_events = new Events(this);
	_gfx = new Gfx();
	_mouseCursor = new MouseCursor(this);
	_picFile = new PicFile();
	_res = new Resources(this);
	_screen = Screen::init(this);
	_window = new MainGameWindow(this);

	_window->applicationStarting();
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

ProjectItem *LegendEngine::createProject() const {
	switch (getGameID()) {
	case GType_Gateway:
		return new Gateway::GatewayProjectItem();
	case GType_CompanionsOfXanth:
		return new Xanth::XanthProjectItem();
	default:
		error("Unknown game");
	}
}

} // End of namespace Legend
