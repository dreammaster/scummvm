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
#include "graphics/paletteman.h"
#include "wasteland/fod/views/main_menu/title.h"
#include "wasteland/fod/fod.h"
#include "wasteland/gfx/image_decoder.h"

namespace Wasteland {
namespace FOD {
namespace Views {

bool Title::msgKeypress(const KeypressMessage &msg) {
	timeout();
	return true;
}

bool Title::msgMouseDown(const MouseDownMessage &msg) {
	timeout();
	return true;
}

void Title::draw() {
	Surface s = getSurface();
	Gfx::ImageDecoder decoder;

	if (!decoder.load("TPICT", 320, 200))
		error("Could not load TPICT");

	s.blitFrom(*decoder.getSurface());

#if 0
	// Draw a bunch of squares on screen
	Surface s = getSurface();
	s.fillRect(Common::Rect(0, 0, 320, 200), 0xff);
	for (int i = 0; i < 16; ++i)
		s.fillRect(Common::Rect(i * 10, 190, i * 10 + 10, 200), i);

	// Test font drawing
	s.writeString("This is a test", 5, 5);
	#endif
}

bool Title::msgFocus(const FocusMessage &msg) {
	delaySeconds(1);
	return true;
}

void Title::timeout() {
	replaceView(g_engine->_disk1._partyCount > 0 ? "Game" : "MainMenu");
}

} // namespace Views
} // namespace FOD
} // namespace Wasteland
