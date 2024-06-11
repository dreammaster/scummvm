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

#ifndef WASTELAND_WASTELAND1_GFX_COLOR_FONT_H
#define WASTELAND_WASTELAND1_GFX_COLOR_FONT_H

#include "common/array.h"
#include "common/rect.h"
#include "graphics/font.h"
#include "graphics/managed_surface.h"
#include "wasteland/gfx/surface.h"

namespace Wasteland {
namespace Wasteland1 {
namespace Gfx {

/**
 * Implements the special font in colorf.fnt. This contains special
 * glyphs like the different edge pieces of dialogs, and subsets of
 * the alphabet used for dialog titles and button contents
 */
class ColorFont : public Graphics::Font {
private:
	Common::Array<Graphics::ManagedSurface> _chars;

public:
	ColorFont() : Graphics::Font() {}
	~ColorFont() override {}

	/**
	 * Loads the font from the specified file
	 */
	void load();

	/**
	 * Get the font height
	 */
	int getFontHeight() const override {
		return FONT_H;
	}

	/**
	 * Get the maximum character width
	 */
	int getMaxCharWidth() const override {
		return FONT_W;
	}

	/**
	 * Get the width of the given character
	 */
	int getCharWidth(uint32 chr) const override {
		return FONT_W;
	}

	/**
	 * Draw a character
	 */
	void drawChar(Graphics::Surface *dst, uint32 chr, int x, int y, uint32 color) const override;
};

} // namespace Gfx
} // namespace Wasteland1
} // namespace Wasteland

#endif
