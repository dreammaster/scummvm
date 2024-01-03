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
#include "wasteland/fod/views/title.h"
#include "wasteland/engine.h"

namespace Wasteland {
namespace FOD {
namespace Views {

bool Title::msgKeypress(const KeypressMessage &msg) {
	// Any keypress to close the view
	close();
	return true;
}

void Title::draw() {
	// Draw a bunch of squares on screen
	Surface s = getSurface();
	s.fillRect(Common::Rect(0, 0, 320, 200), 0xff);
	for (int i = 0; i < 16; ++i)
		s.fillRect(Common::Rect(i * 10, 190, i * 10 + 10, 200), i);

	// Test font drawing
	g_engine->_font->drawString(&s, "This is a test", 50, 50,
		200, 0xff);
}

} // namespace Views
} // namespace FOD
} // namespace Wasteland
