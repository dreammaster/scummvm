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

#include "common/config-manager.h"
#include "common/debug-channels.h"
#include "common/events.h"
#include "common/file.h"
#include "common/util.h"
#include "engines/util.h"
#include "legend/legend.h"
#include "legend/detection.h"
#include "legend/globals.h"
#include "legend/memory.h"

namespace Legend {

LegendEngine *g_engine;

LegendEngine::LegendEngine(OSystem *syst, const LegendGameDescription *gameDesc) : Engine(syst),
		_gameDescription(gameDesc), _randomSource("Legend"),
		_globals(nullptr), _screen(nullptr) {
	g_engine = this;
	g_globals = nullptr;
}

LegendEngine::~LegendEngine() {
	delete _globals;
	delete _screen;
}

void LegendEngine::initialize() {
	_globals = new Globals();
	_screen = new Graphics::Screen();
}

uint32 LegendEngine::getFeatures() const {
	return _gameDescription->_desc.flags;
}

GameId LegendEngine::getGameId() const {
	return _gameDescription->_gameId;
}

Common::Error LegendEngine::run() {
	return Common::kNoError;
}

} // namespace Legend
