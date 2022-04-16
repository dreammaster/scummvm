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

#include "common/events.h"
#include "legend/font.h"
//#include "legend/globals.h"

namespace Legend {

Fonts::Fonts() {
	_font = nullptr;
	_tab_width = 0;
	_font_color = 0;
	_font_outline = 0;
	_font_bk_color = 0;
	_half_font_w = _half_font_h = 0;
	_font_w = _font_h = 0;
}

int Fonts::set_tab_width(int w) {
	int oldWidth = _tab_width;
	if (w > 0)
		_tab_width = w;
	return oldWidth;
}

int Fonts::get_font_height() {
	Font &f = getFont();
	return (f._height2 > 1) ? f._height1 + f._height2 : 8;
}

int Fonts::get_char_width(int c) {
	Font &f = getFont();
	int w;

	if (c == '\t') {
		w = _tab_width;
	} else if (c == '\n' || c == 18) {
		return 0;
	} else {
		w = f.getWidth(c);
	}

	if (_font_outline >= 0)
		++w;

	return w;
}

int Font::getWidth(int c) const {
	assert(c != '\t' && c != '\n' && c != 18);

	if (_width2 < 0) {
		int w1 = static_cast<const byte *>(_data1->_ptr)[c];
		int w2 = static_cast<const byte *>(_data1->_ptr)[c];
		return w1 + (w2 >> 4) + (w2 & 0xf);

	} else if (_width1 < 0) {
		int w1 = static_cast<const byte *>(_data1->_ptr)[c];
		int w2 = _width2;
		return w1 + w2;

	} else {
		return _width1 + _width2;
	}
}

} // namespace Legend
