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

LegendEngine::LegendEngine(OSystem *syst, const LegendGameDescription *gameDesc) :
		Engine(syst), _gameDescription(gameDesc), _randomSource("Legend") {
	_debugger = nullptr;
	_events = nullptr;
}

LegendEngine::~LegendEngine() {
	delete _debugger;
	delete _events;
}

void LegendEngine::initialize() {
	DebugMan.addDebugChannel(kDebugLevelScript,      "scripts", "Script debug level");

	_debugger = Debugger::init(this);
	_events = new Events(this);
}

Common::Error LegendEngine::run() {
	// Initialize the engine
	initialize();

	return Common::kNoError;
}

} // End of namespace Legend
