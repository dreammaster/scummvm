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

#include "common/debug.h"
#include "legend/font.h"
#include "legend/graphics.h"
#include "legend/globals.h"

namespace Legend {

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

Fonts::Fonts() {
	_font = nullptr;
	_tab_width = 0;
	_font_color = 0;
	_font_outline = 0;
	_font_bk_color = 0;
	_half_font_w = _half_font_h = 0;
	_font_w = _font_h = 0;
	_cursor_on = false;
	_print_char_col_fg = 0;
	_print_char_col_bg = 0;
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

int Fonts::get_string_width(const StringPtr &str) {
	const char *s = str;
	int w = 0;

	for (; *s; ++s)
		w += get_char_width(*s);

	return w;
}

void Fonts::get_font_color(uint32 *fg, uint32 *bg) {
	*fg = _font_color;
	*bg = _font_bk_color;
}

void Fonts::set_font_color(uint32 fg, uint32 bg) {
	_font_color = fg;
	_font_bk_color = bg;
}

void Fonts::set_font_position(int x, int y) {
	_font_x = x;
	_font_y = y;
}

void Fonts::print_font(const StringPtr &str) {
	const char *s = str;
	if (_font->_fontNum == -1) {
		debug("%s", s);
	} else {
		for (; *s; ++s)
			print_font_char(*s);
	}
}

void Fonts::print_font_char(int c) {
	Font &f = getFont();
	if (_cursor_on)
		toggle_cursor();

	if (c == '\n') {
		_font_x = 0;
		_font_y += _font_h;
	} else if (c == '\t') {
		int adjust = (_font_x >= 0) ? 0 : -1;
		_font_x += _tab_width - adjust;

		if (_font_x >= _G(video_x2)) {
			_font_x = 0;
			_font_y += _font_h;
		}
	} else if (c == 18) {
		if (!_print_char_flag) {
			_print_char_col_fg = _font_color;
			_print_char_col_bg = _font_bk_color;
			SWAP(_font_color, _font_bk_color);
			_print_char_flag = true;
		} else {
			_font_color = _print_char_col_fg;
			_font_bk_color = _print_char_col_bg;
			_print_char_flag = false;
		}
	} else {
		int x1 = f._width1 >= 0 ? f._width1 :
			static_cast<byte *>(f._data1->_ptr)[c];
		int x2 = x1 + (f._width2 >= 0 ? f._width2 :
			static_cast<byte *>(f._data2->_ptr)[c]);
		int h2 = f._height2;

		if (_font_bk_color && _font_outline < 0) {
			gxSaveColors(0, _font_bk_color);
			int y2 = _font_y + _font_h - 1;

			// TODO
		}
	}

	if (_cursor_on)
		toggle_cursor();
}

void Fonts::toggle_cursor() {
	draw_xor(_font_x, _font_y, _font_w, _font_h, 13);
}

} // namespace Legend
