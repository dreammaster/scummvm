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
#include "ultima/ultima5/ultima5.h"

namespace Ultima {
namespace Ultima5 {
namespace Views {

constexpr int FRAMES_PER_STEP = 3;

bool Game::msgFocus(const FocusMessage &msg) {
	_frame.load();
	_cursorFrame = 0;

	delayFrames(FRAMES_PER_STEP);
	return Shared::View::msgFocus(msg);
}

void Game::draw() {
	auto s = getSurface();
	_frame.drawFrame(s);
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

} // namespace Views
} // namespace Ultima5
} // namespace Ultima
