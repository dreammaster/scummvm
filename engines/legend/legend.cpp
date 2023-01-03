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

#include "common/scummsys.h"
#include "common/config-manager.h"
#include "common/debug-channels.h"
#include "common/events.h"
#include "common/system.h"
#include "legend/legend.h"
#include "legend/detection.h"

namespace Legend {

LegendEngine *g_engine;

LegendEngine::LegendEngine(OSystem *syst, const LegendGameDescription *gameDesc) :
		Engine(syst), _gameDescription(gameDesc), _randomSource("Legend") {
	g_engine = this;
}

LegendEngine::~LegendEngine() {
	delete _mouseCursor;
	delete _picFile;
	delete _res;
}

uint32 LegendEngine::getFeatures() const {
	return _gameDescription->desc.flags;
}

GameType LegendEngine::getGameId() const {
	return _gameDescription->gameID;
}

bool LegendEngine::isLater() const {
	auto id = _gameDescription->gameID;
	return id == GType_CompanionsOfXanth ||
		id == GType_Shannara ||
		id == GType_DeathGate;
}

Common::String LegendEngine::getGameFilePrefix() const {
	switch (getGameId()) {
	case GType_Gateway:
		return "gate";
	case GType_CompanionsOfXanth:
		return "xanth";
	default:
		return "";
	}
}


Common::Error LegendEngine::run() {
	if (initialize()) {
		runGame();
		deinitialize();
	}

	return Common::kNoError;
}

bool LegendEngine::initialize() {
	_mouseCursor = new Gfx::MouseCursor();
	_picFile = new Gfx::PicFile();
	_res = new Resources(this);
	return true;
}

void LegendEngine::deinitialize() {
}

Common::Error LegendEngine::syncGame(Common::Serializer &s) {
	// The Serializer has methods isLoading() and isSaving()
	// if you need to specific steps; for example setting
	// an array size after reading it's length, whereas
	// for saving it would write the existing array's length
	int dummy = 0;
	s.syncAsUint32LE(dummy);

	return Common::kNoError;
}

Common::String LegendEngine::getFilename(FileType fileType, int fileNumber) {
	static const char *const EXTENSIONS[] = { "PIC", "RGN", "FNT", "MUS", "SAV", "SAV" };

	switch (getGameId()) {
	case GType_Gateway:
		return Common::String::format("gate_%03d.%s", fileNumber, EXTENSIONS[fileType]);
	case GType_CompanionsOfXanth:
		return Common::String::format("xanth_%02d.%s", fileNumber, EXTENSIONS[fileType]);
	default:
		error("Unknown game");
	}
}

} // End of namespace Legend
