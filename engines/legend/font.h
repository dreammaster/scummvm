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
#include "legend/resources.h"

namespace Legend {

#define FONT_COUNT 2

struct Font {
	int _counter = 0;
	int _fontNum = 0;
	int16 _height1 = 0;
	int16 _height2 = 0;
	int8 _width1 = 0;
	int8 _width2 = 0;
	MemoryBlock *_charWidths = nullptr;
	MemoryBlock *_charSpacings = nullptr;
	MemoryBlock *_pixelData = nullptr;

	/**
	 * Get a character width
	 */
	int getWidth(int c) const;

	/**
	 * Frees the data associated with the font
	 */
	void close();
};

class Fonts {
private:
	/**
	 * Returns a reference to the current font
	 */
	Font &getFont() {
		assert(_font);
		return *_font;
	}

	Font *get_font_rec();
public:
	Font _fonts[FONT_COUNT];
	Font *_font = nullptr;
	int _tab_width = 0;
	uint32 _font_color = 0;
	uint32 _font_bk_color = 0;
	int _font_outline = 0;
	int _half_font_w = 0;
	int _half_font_h = 0;
	int _font_w = 0;
	int _font_h = 0;
	int _font_x = 0, _font_y = 0;
	bool _cursor_on = false;
	bool _print_char_flag = false;
	int _print_char_col_fg = 0;
	int _print_char_col_bg = 0;

public:
	Fonts() = default;

	/**
	 * Sets the font
	 */
	Font *set_font(int fontNum);

	/**
	 * Closes a font
	 */
	void close_font(Font *font);

	/**
	 * Returns the current font
	 */
	Font *get_font() { return _font; }

	/**
	 * Sets the width of tab characters
	 */
	int set_tab_width(int w);

	/**
	 * Get the current font's height
	 */
	int get_font_height();

	/**
	 * Get a character width in the current font
	 */
	int get_char_width(int c);

	/**
	 * Returns a string width
	 */
	int get_string_width(const StringPtr &str);

	/**
	 * Gets the font color
	 */
	void get_font_color(uint32 *fg, uint32 *bg);

	/**
	 * Sets the font color
	 */
	void set_font_color(uint32 fg, uint32 bg);

	/**
	 * Sets the screen position for writing text
	 */
	void set_font_position(int x, int y);

	/**
	 * Prints out a text string
	 */
	void print_font(const StringPtr &str);

	/**
	 * Prints out a character
	 */
	void print_font_char(int c);

	/**
	 * Toggles display of the text cursor
	 */
	void toggle_cursor();
};

} // namespace Legend

#endif
