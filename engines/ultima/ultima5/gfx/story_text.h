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

#ifndef ULTIMA5_GFX_STORY_TEXT_H
#define ULTIMA5_GFX_STORY_TEXT_H

#include "common/str.h"
#include "ultima/ultima5/gfx/bit_image_file.h"

namespace Ultima {
namespace Ultima5 {
namespace Gfx {

/**
 * Where proportional text may go. Text flows within the left and right edges of
 * the first region, or of the second one for the lines whose y is between
 * the split limits
 */
struct StoryLayout {
	int _left[2] = { 0, 0 };
	int _right[2] = { 320, 320 };
	int _splitTop = 200;
	int _splitBottom = 200;
	int _spaceWidth = 5;
};

/**
 * Draws justified, word wrapped text in the proportional font of proport.pcs.
 * An underscore in a word marks where it can be hyphenated, and a brace
 * is an indent
 */
class StoryText {
private:
	BitImageFile _glyphs;

public:
	/**
	 * Loads the font
	 * @returns		False if it couldn't be loaded
	 */
	bool load();

	/**
	 * Draws text starting at a position, which is advanced to where
	 * the text ended
	 */
	void draw(Graphics::ManagedSurface &dst, const Common::String &text, int &x, int &y,
		const StoryLayout &layout) const;
};

} // namespace Gfx
} // namespace Ultima5
} // namespace Ultima

#endif
