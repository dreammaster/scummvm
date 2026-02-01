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

#ifndef ULTIMA_SHARED_GFX_FONT_H
#define ULTIMA_SHARED_GFX_FONT_H

#include "common/array.h"
#include "common/stream.h"
#include "graphics/font.h"

namespace Ultima {
namespace Shared {
namespace Gfx {

class Font : public Graphics::Font {
private:
	const byte *_data;
	uint _startingChar, _endingChar;
public:
	Font(const byte *data, size_t startingChar = 0, size_t charCount = 256);
	~Font() override {}

	/**
	 * Return the maximum width of the font.
	 *
	 * @return Maximum font width in pixels.
	 */
	int getMaxCharWidth() const override {
		return 8;
	}

	/**
	 * Return the width of a specific character.
	 *
	 * @param chr  The character to query the width of.
	 *
	 * @return The width of the character in pixels.
	 */
	int getCharWidth(uint32 chr) const {
		return 8;
	}

	/**
	 * Return the height of the font.
	 *
	 * @return Font height in pixels.
	 */
	int getFontHeight() const override {
		return 8;
	}

	/**
	 * Draw a character at a specific point on the surface.
	 *
	 * Note that the point describes the top left edge point where to draw
	 * the character. This can be different from the top left edge point of the
	 * character's bounding box. For example, TTF fonts sometimes move
	 * characters like 't' by one (or more) pixels to the left to create better
	 * visual results. To query the actual bounding box of a character, use
	 * getBoundingBox.
	 * @see getBoundingBox
	 *
	 * The Font implementation should take care of not drawing outside of the
	 * specified surface.
	 *
	 * @param dst   The surface to draw on.
	 * @param chr   The character to draw.
	 * @param x     The x coordinate where to draw the character.
	 * @param y     The y coordinate where to draw the character.
	 * @param color The color of the character.
	 */
	void drawChar(Graphics::Surface *dst, uint32 chr, int x, int y, uint32 color) const override;
};

} // namespace Gfx
} // namespace Shared
} // namespace Ultima

#endif
