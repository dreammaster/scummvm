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

#include "common/translation.h"
#include "engines/util.h"
#include "krondor/krondor.h"
#include "krondor/console.h"
#include "krondor/files/resource_archive.h"

namespace Krondor {

KrondorEngine *g_engine;

KrondorEngine::KrondorEngine(OSystem *syst, const ADGameDescription *gameDesc) : Engine(syst),
	_gameDescription(gameDesc), _randomSource("Krondor") {
	g_engine = this;
}

KrondorEngine::~KrondorEngine() {
}

uint32 KrondorEngine::getFeatures() const {
	return _gameDescription->flags;
}

Common::String KrondorEngine::getGameId() const {
	return _gameDescription->gameId;
}

Common::Error KrondorEngine::run() {
	// Initialize 320x200 paletted graphics mode
	initGraphics(320, 200);

	// Set the engine's debugger console
	setDebugger(new Console());

	if (!ResourceArchive::initialize()) {
		GUIErrorMessage(_("Could not load resources"));
		return Common::kNoError;
	}

	Vars *vars = new Vars();

	runGame();

	delete vars;
	return Common::kNoError;
}

Common::Error KrondorEngine::syncGame(Common::Serializer &s) {
	// The Serializer has methods isLoading() and isSaving()
	// if you need to specific steps; for example setting
	// an array size after reading it's length, whereas
	// for saving it would write the existing array's length
	int dummy = 0;
	s.syncAsUint32LE(dummy);

	return Common::kNoError;
}

} // End of namespace Krondor
