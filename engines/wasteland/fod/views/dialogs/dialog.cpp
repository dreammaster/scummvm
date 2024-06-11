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

#include "wasteland/fod/views/dialogs/dialog.h"

namespace Wasteland {
namespace FOD {
namespace Views {
namespace Dialogs {

Dialog::Dialog(const Common::String &name, int w, int h) : UIElement(name) {
	setBounds(Common::Rect(160 - (w * FONT_W / 2), 100 - (h * FONT_H / 2),
		160 + (w * FONT_W / 2), 100 + (h * FONT_H / 2)));
	_bounds.setBorderSize(FONT_W);
}

Dialog::Dialog(const Common::String &name, const Window &win) : UIElement(name) {
	setBounds( Window(win.left - 1, win.top - 1, win.right + 1, win.bottom + 1,
		win._xOffset, win._yOffset));
}

void Dialog::draw() {
	_bounds.setBorderSize(0);		// We want to draw in the borders area
	Surface s = getSurface();
	s.clear();

	// Top border
	s.writeChar(0x84);
	s.writeChar(0x80);
	for (int i = 0; i < _bounds.width() / FONT_W - 4; ++i)
		s.writeChar(0x81);
	s.writeChar(0x80);
	s.writeChar(0x84);

	// Bottom border
	s.setTextPos(0, _bounds.height() / FONT_H - 1);
	s.writeChar(0x84);
	s.writeChar(0x80);
	for (int i = 0; i < _bounds.width() / FONT_W - 4; ++i)
		s.writeChar(0x81);
	s.writeChar(0x80);
	s.writeChar(0x84);

	// Left/right borders
	int r = _bounds.width() / FONT_W - 1;
	s.setTextPos(0, 1);
	s.writeChar(0x82);
	s.setTextPos(r, 1);
	s.writeChar(0x82);

	for (int y = 2; y < (_bounds.height() / FONT_H - 1); ++y) {
		s.setTextPos(0, y);
		s.writeChar(0xb5);
		s.setTextPos(r, y);
		s.writeChar(0xb5);
	}

	s.setTextPos(0, _bounds.height() / FONT_H - 2);
	s.writeChar(0x82);
	s.setTextPos(r, _bounds.height() / FONT_H - 2);

	_bounds.setBorderSize(FONT_W);
}

} // namespace Dialogs
} // namespace Views
} // namespace FOD
} // namespace Wasteland
