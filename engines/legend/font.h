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

#ifndef LEGEND_FONT_H
#define LEGEND_FONT_H

#include "common/rect.h"
#include "legend/memory.h"

namespace Legend {

struct Font {
	int _fontNum = 0;
	int16 _height1 = 0;
	int16 _height2 = 0;
	int8 _width1 = 0;
	int8 _width2 = 0;
	MemoryBlock *_data1 = nullptr;
	MemoryBlock *_data2 = nullptr;

	/**
	 * Get a character width
	 */
	int getWidth(int c) const;
};

class Fonts {
private:
	/**
	 * Returns a reference to the current font
	 */
	static Font &getFont() {
		assert(_font);
		return *_font;
	}
public:
	static Font *_font;
	static int _tab_width;
	static int _font_color;
	static int _font_outline;
	static int _font_bk_color;
	static int _half_font_w;
	static int _half_font_h;
	static int _font_w;
	static int _font_h;
public:
	Fonts();

	/**
	 * Returns the current font
	 */
	static Font *get_font() { return _font; }

	/**
	 * Sets the width of tab characters
	 */
	static int set_tab_width(int w);

	/**
	 * Get the current font's height
	 */
	static int get_font_height();

	/**
	 * Get a character width in the current font
	 */
	static int get_char_width(int c);
};

} // namespace Legend

#endif
