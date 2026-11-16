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

#ifndef ULTIMA5_GFX_CHARSET_H
#define ULTIMA5_GFX_CHARSET_H

#include "graphics/font.h"

namespace Ultima {
namespace Ultima5 {
namespace Gfx {

constexpr int CHARSET_GLYPHS = 128;

/**
 * A font from one of the game's .CH files: 128 glyphs of 8x8 pixels at one
 * bit per pixel, with the top bit of each row being the leftmost pixel
 */
class CharSet : public Graphics::Font {
private:
	byte _data[CHARSET_GLYPHS * 8] = {};

public:
	/**
	 * Loads a charset, such as IBM.CH for text or RUNES.CH
	 */
	CharSet(const char *filename = "IBM.CH");

	int getFontHeight() const override {
		return 8;
	}

	int getMaxCharWidth() const override {
		return 8;
	}

	int getCharWidth(uint32) const override {
		return 8;
	}

	void drawChar(Graphics::Surface *dst, uint32 chr, int x, int y, uint32 color) const override;
};

} // namespace Gfx
} // namespace Ultima5
} // namespace Ultima

#endif
