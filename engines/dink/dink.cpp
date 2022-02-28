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
}

uint32 DinkEngine::getFeatures() const {
	return _gameDescription->desc.flags;
}

Common::String DinkEngine::getGameId() const {
	return _gameDescription->desc.gameId;
}

Common::Error DinkEngine::run() {
	initialize();
	return Common::kNoError;
}

void DinkEngine::initialize() {
	_events = new EventsManager();
}


} // namespace Dink
