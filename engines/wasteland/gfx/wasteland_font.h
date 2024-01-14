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

#ifndef WASTELAND_GFX_WASTELAND_FONT_H
#define WASTELAND_GFX_WASTELAND_FONT_H

#include "common/array.h"
#include "common/rect.h"
#include "graphics/font.h"
#include "graphics/managed_surface.h"

namespace Wasteland {
namespace Gfx {

#define FONT_W 8
#define FONT_H 8

/**
 * Implements a font stored as a grid on a passed surface
 */
class WastelandFont : public Graphics::Font {
private:
	size_t _count;
	byte *_data = nullptr;

public:
	WastelandFont() : Graphics::Font() {}
	~WastelandFont();

	/**
	 * Loads the font from the specified file
	 */
	bool load(const Common::Path &filename);

	/**
	 * Get the font height
	 */
	int getFontHeight() const override { return FONT_H; }

	/**
	 * Get the maximum character width
	 */
	int getMaxCharWidth() const override { return FONT_W; }

	/**
	 * Get the width of the given character
	 */
	int getCharWidth(uint32 chr) const override { return FONT_W; }

	/**
	 * Draw a character
	 */
	void drawChar(Graphics::Surface *dst, uint32 chr, int x, int y, uint32 color) const override;
};

} // namespace Gfx
} // namespace Wasteland

#endif
