
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

#ifndef YENDOR_GFX_FONT_H
#define YENDOR_GFX_FONT_H

#include "common/str.h"
#include "graphics/surface.h"

namespace Yendor {

#define FONT_W 6
#define FONT_H 6
#define FONT_CHAR_COUNT (128 - 32)

class Font {
private:
	byte _data[FONT_CHAR_COUNT * FONT_H];
public:
	static bool _bgTransparent;
	static byte _fgColor;
	static byte _bgColor;
public:
	/**
	 * Load the font data
	 */
	void load(const Common::Path &filename);

	/**
	 * Write a character at a specified position
	 */
	void writeChar(Graphics::Surface &surf, int x, int y, char c);

	/**
	 * Write a string at a specified position
	 */
	void writeString(Graphics::Surface &surf, int x, int y,
		const Common::String &str);
};

} // namespace Yendor

#endif
