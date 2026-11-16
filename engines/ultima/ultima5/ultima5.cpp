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
#include "ultima/ultima5/ultima5.h"
#include "ultima/ultima5/console.h"
#include "ultima/ultima5/metaengine.h"
#include "ultima/ultima5/views/views.h"

namespace Ultima {
namespace Ultima5 {

Ultima5Engine *g_engine;

Ultima5Engine::Ultima5Engine(OSystem *syst, const Ultima::UltimaGameDescription *gameDesc) :
		Engine(syst), Shared::Events("ultima5"), _gameDescription(gameDesc) {
	g_engine = this;
}

Ultima5Engine::~Ultima5Engine() {
}

uint32 Ultima5Engine::getFeatures() const {
	return _gameDescription->desc.flags;
}

Common::String Ultima5Engine::getGameId() const {
	return _gameDescription->desc.gameId;
}

Common::Error Ultima5Engine::run() {
	MetaEngine::setKeybindingMode(KBMODE_MINIMAL);

	// Initialize 320x200 graphics mode
	initGraphics(320, 200);

	// Set the engine's debugger console
	setDebugger(new Console());

	Views::Views views;
	addView("Title");
	runGame();
	(void)views;		// Suppress any warnings of unused local

	return Common::kNoError;
}

void Ultima5Engine::returnToMainMenu() {
	replaceView("Title", true);
}

bool Ultima5Engine::canSaveGameStateCurrently(Common::U32String *) {
	return false;
}

bool Ultima5Engine::canLoadGameStateCurrently(Common::U32String *) {
	return false;
}

Common::Error Ultima5Engine::syncGame(Common::Serializer &s) {
	// TODO: Savegame data
	return Common::kNoError;
}

} // namespace Ultima5
} // namespace Ultima
