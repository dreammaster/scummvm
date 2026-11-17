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

#include "ultima/ultima5/views/game.h"
#include "ultima/ultima5/views/keys.h"
#include "ultima/ultima5/logic/world_logic.h"
#include "ultima/ultima5/ultima5.h"

namespace Ultima {
namespace Ultima5 {
namespace Views {

constexpr int FRAMES_PER_STEP = 3;

// The id of the map of Britannia and the underworld
constexpr byte OUTDOOR_MAP = 0;

bool Game::msgFocus(const FocusMessage &msg) {
	_frame.load();
	_cursorFrame = 0;

	if (_G(savegame)._mapId == OUTDOOR_MAP && !g_engine->loadWorld())
		error("Could not load the map");

	delayFrames(FRAMES_PER_STEP);
	return Shared::View::msgFocus(msg);
}

void Game::draw() {
	auto s = getSurface();
	_frame.drawFrame(s);

	if (_G(savegame)._mapId == OUTDOOR_MAP) {
		WorldLogic::syncPartyObject(_G(savegame));
		_worldView.draw(s, _G(tileset), _G(world), _G(savegame));
	}

	_frame.drawParty(s, _G(savegame));
	_frame.drawStatus(s, _G(savegame));
	_frame.drawWind(s, _G(savegame));
	_G(messages).draw(_frame, s, true, _cursorFrame);
}

void Game::timeout() {
	_cursorFrame = (_cursorFrame + 1) & 3;
	redraw();
	delayFrames(FRAMES_PER_STEP);
}

bool Game::msgKeypress(const KeypressMessage &msg) {
	if (isModifierKey(msg.keycode) || _G(savegame)._mapId != OUTDOOR_MAP)
		return false;

	WorldLogic logic;
	switch (msg.keycode) {
	case Common::KEYCODE_UP:
		logic.walk(DIR_NORTH);
		break;
	case Common::KEYCODE_RIGHT:
		logic.walk(DIR_EAST);
		break;
	case Common::KEYCODE_DOWN:
		logic.walk(DIR_SOUTH);
		break;
	case Common::KEYCODE_LEFT:
		logic.walk(DIR_WEST);
		break;
	case Common::KEYCODE_SPACE:
		logic.pass();
		break;
	default:
		return false;
	}

	redraw();
	return true;
}

} // namespace Views
} // namespace Ultima5
} // namespace Ultima
