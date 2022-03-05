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

#include "legend/legend.h"
#include "legend/detection.h"
#include "common/scummsys.h"
#include "common/config-manager.h"
#include "common/debug-channels.h"
#include "common/events.h"
#include "common/file.h"
#include "common/util.h"
#include "engines/util.h"

namespace Legend {

LegendEngine *g_engine;

LegendEngine::LegendEngine(OSystem *syst, const LegendGameDescription *gameDesc) : Engine(syst),
	_gameDescription(gameDesc), _randomSource("Legend") {
	g_engine = this;
}

LegendEngine::~LegendEngine() {
}

uint32 LegendEngine::getFeatures() const {
	return _gameDescription->desc.flags;
}

Common::String LegendEngine::getGameId() const {
	return _gameDescription->desc.gameId;
}

Common::Error LegendEngine::run() {
	return Common::kNoError;
}

} // namespace Legend
