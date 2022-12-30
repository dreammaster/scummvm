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

#include "engines/util.h"
#include "legend/games/gateway/engine.h"
#include "legend/utils/engine_data.h"
#include "legend/console.h"
#include "common/file.h"

namespace Legend {
namespace Early {
namespace Gateway {

bool GatewayEngine::initialize() {
	// Initialize 640x480 paletted graphics mode
	initGraphics(640, 480);

	// Set the engine's debugger console
	setDebugger(new Console());

	Common::U32String errorMsg;
	if (!load_engine_data("gateway", true, 1, 0, errorMsg)) {
		GUIErrorMessage(errorMsg);
		return false;
	}

	return true;
}

} // namespace Gateway
} // namespace Early
} // namespace Legend
