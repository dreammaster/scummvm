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

#include "legend/early/views/help_screen.h"
#include "legend/early/gfx/screen.h"
#include "legend/utils/file.h"

namespace Legend {
namespace Early {
namespace Views {

void HelpScreen::draw() {
	Legend::Gfx::View::draw();

	// Fill out the background
	Graphics::ManagedSurface s = getSurface();
	s.clear(Gfx::WHITE);
	_font->setColor(Gfx::DEFAULT_TEXT_COLOR);

	// Write out the help
	File f("help.txt");

	for (int idx = 0; !f.eos(); ++idx) {
		Common::String msg2, msg1 = f.readLine();
		size_t p = msg1.findFirstOf('\t');

		if (p != Common::String::npos) {
			// If a tab is present, split the line in two
			msg2 = Common::String(msg1.c_str() + p + 1);
			msg1 = Common::String(msg1.c_str(), msg1.c_str() + p);
		}

		int xp = idx < 22 ? 24 : 312;
		int yp = (idx >= 22 ? idx - 22 : idx) * Legend::Gfx::Font::_lineHeight
			+ 64 + _font->_yCenter;

		int xs;
		if (idx < 11)
			xs = 56;
		else if (idx < 18)
			xs = 112;
		else
			xs = 120;

		if (!msg2.empty()) {
			// It's a key/purpose pair
			writeString(Common::Point(xp + 16, yp), msg1);
			writeString(Common::Point(xp + xs, yp), msg2);
		} else if (!msg1.empty()) {
			// It's a section title
			writeString(Common::Point(xp, yp), msg1);
		}
	}

	f.close();
}

bool HelpScreen::msgMouseDown(const MouseDownMessage &msg) {
	replaceView("Scene");
	return true;
}

bool HelpScreen::msgKeypress(const KeypressMessage &msg) {
	replaceView("Scene");
	return true;
}

} // namespace Views
} // namespace Early
} // namespace Legend
