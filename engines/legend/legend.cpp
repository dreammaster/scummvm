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
#include "legend/early/gateway/gateway_project_item.h"
#include "legend/later/xanth/xanth_project_item.h"

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
	_vocab = nullptr;
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
	delete _vocab;
	delete _window;
}

void LegendEngine::initialize() {
	DebugMan.addDebugChannel(kDebugLevelScript,      "scripts", "Script debug level");

	Gfx::Font::init();
	_debugger = Debugger::init(this);
	_events = new Events(this);
	_res = new Resources(this);
	_screen = Gfx::Screen::init(this);
	_gfx = new Gfx::Gfx();
	_mouseCursor = new MouseCursor(this);
	_picFile = new Gfx::PicFile();
	_vocab = isLater() ? nullptr : new Early::Vocab();

	_window = new MainGameWindow(this);
	_window->applicationStarting();
}

void LegendEngine::deinitialize() {
	Gfx::Font::deinit();
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

ProjectItem *LegendEngine::createProject() const {
	switch (getGameID()) {
	case GType_Gateway:
		return new Early::Gateway::GatewayProjectItem();
	case GType_CompanionsOfXanth:
		return new Later::Xanth::XanthProjectItem();
	default:
		error("Unknown game");
	}
}

String LegendEngine::getGameFilePrefix() const {
	switch (getGameID()) {
	case GType_Gateway:
		return "gate";
	case GType_CompanionsOfXanth:
		return "xanth";
	default:
		return "";
	}
}

String LegendEngine::getFilename(FileType fileType, int fileNumber) {
	static const char *const EXTENSIONS[] = { "PIC", "RGN", "FNT", "MUS", "SAV", "SAV" };
	
	switch (getGameID()) {
	case GType_Gateway:
		return String::format("gate_%03d.%s", fileNumber, EXTENSIONS[fileType]);
	case GType_CompanionsOfXanth:
		return String::format("xanth_%02d.%s", fileNumber, EXTENSIONS[fileType]);
	default:
		error("Unknown game");
	}
}

} // End of namespace Legend
