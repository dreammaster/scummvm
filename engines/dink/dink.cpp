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

#include "dink/dink.h"
#include "dink/detection.h"
#include "dink/events.h"
#include "dink/freedink.h"
#include "dink/globals.h"
#include "dink/update_frame.h"
#include "dink/var.h"
#include "common/scummsys.h"
#include "common/config-manager.h"
#include "common/debug-channels.h"
#include "common/events.h"
#include "common/file.h"
#include "common/util.h"
#include "engines/util.h"

namespace Dink {

DinkEngine *g_engine;

DinkEngine::DinkEngine(OSystem *syst, const DinkGameDescription *gameDesc) : Engine(syst),
	_gameDescription(gameDesc), _randomSource("Dink") {
	g_engine = this;
}

DinkEngine::~DinkEngine() {
	delete _events;
	delete _screen;
}

uint32 DinkEngine::getFeatures() const {
	return _gameDescription->desc.flags;
}

Common::String DinkEngine::getGameId() const {
	return _gameDescription->desc.gameId;
}

Common::Error DinkEngine::run() {
	sound_support = getRandomNumber(1, 200000);
	mycode = getRandomNumber(1, 200000);

	if (isDemo()) {
		//if (chdir("story"))
		error("Error finding story dir.");
		mcc = sound_support;

		if (dir_num("dink\\story") != 250 + 9) {
			mcc = 0;
			error("Shareware version cannot run player made quests.");
		}
	}

	initializeFlags();
	initialize();

	if (doInit()) {
		while (!shouldQuit()) {
			_events->pollEvents();
			updateFrame();
		}
	}

	return Common::kNoError;
}

void DinkEngine::initialize() {
	// Add in the dink subfolder for easy access
	const Common::FSNode gameDataDir(ConfMan.get("path"));
	SearchMan.addSubDirectoryMatching(gameDataDir, "dink", 0, 2);

	_events = new EventsManager();

	if (Common::File::exists("cd.dat"))
		burn_revision = 1;

	Graphics::PixelFormat format = truecolor ?
	                               Graphics::PixelFormat(4, 8, 8, 8, 8, 24, 16, 8, 0) :
	                               Graphics::PixelFormat::createFormatCLUT8();
	initGraphics(640, 480, &format);
	_screen = new Graphics::Screen();

	// create arrow cursor

	lpDD = new IDirectDraw();
}

void DinkEngine::initializeFlags() {
	if (ConfMan.hasKey("dinkpal"))
		dinkpal = ConfMan.getBool("dinkpal");

	if (ConfMan.hasKey("truecolor"))
		truecolor = ConfMan.getBool("truecolor");

	debug_mode = gDebugLevel > 0;

	if (ConfMan.hasKey("nojoy"))
		disablejoystick = ConfMan.hasKey("nojoy");

	//if (scumm_strnicmp(shit, "-nosound", strlen("-nosound")) == 0)  sound_on = false;
}

} // namespace Dink
