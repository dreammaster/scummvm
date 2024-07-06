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

#include "wasteland/wasteland1/views/dialogs/roster.h"
#include "wasteland/wasteland1/gfx/pics.h"
#include "wasteland/wasteland1/wasteland1.h"

namespace Wasteland {
namespace Wasteland1 {
namespace Views {
namespace Dialogs {

Roster::Roster() : Dialog("Roster"),
		_create(this, "Create", "CREATE", 11, 24),
		_delete(this, "Delete", "DELETE", 18, 24),
		_play(this, "Play", "PLAY", 25, 24) {
}

void Roster::draw() {
	Surface s = getSurface();
	s.clear();

	drawFrame(Common::Rect(0, 0, 39, 24));
	drawFrame(Common::Rect(0, 0, 39, 23));
	drawFrame(Common::Rect(0, 0, 13, 11));
	drawFrame(Common::Rect(14, 0, 39, 11));

	Gfx::PicsDecoder &pics = g_engine->_pics;
	const Gfx::PicsAnimation &anim = pics.getAnimation(0);
	const auto &frameSets = anim.getFrameSets();
	const auto &frameSet = *frameSets.begin();
	const auto &frames = frameSet.getFrames();

	s.blitFrom(*frames[0], Common::Point(10, 10));
}

bool Roster::msgFocus(const FocusMessage &msg) {
	Dialog::msgFocus(msg);
	return true;
}

bool Roster::msgUnfocus(const UnfocusMessage &msg) {
	Dialog::msgUnfocus(msg);
	return true;
}

bool Roster::msgGame(const GameMessage &msg) {

	return true;
}


} // namespace Dialogs
} // namespace Views
} // namespace Wasteland1
} // namespace Wasteland
