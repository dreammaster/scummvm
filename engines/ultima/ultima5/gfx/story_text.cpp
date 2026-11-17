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

#include "ultima/ultima5/gfx/story_text.h"

namespace Ultima {
namespace Ultima5 {
namespace Gfx {

constexpr int INDENT_WIDTH = 0x0F;
constexpr int HYPHEN_WIDTH = 3;
constexpr int HYPHEN_GLYPH = 0x0D;
constexpr int LINE_HEIGHT = 9;
constexpr int BOTTOM_LIMIT = 0xC0;
constexpr byte TEXT_COLOR = 15;

// Advance widths of the glyphs from '!', not counting the 1 pixel gap
static const byte GLYPH_WIDTHS[] = {
	0x02, 0x06, 0x07, 0x07, 0x07, 0x07, 0x03, 0x04,
	0x04, 0x05, 0x06, 0x03, 0x03, 0x02, 0x07, 0x06,
	0x05, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
	0x06, 0x02, 0x03, 0x05, 0x05, 0x05, 0x06, 0x07,
	0x07, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
	0x04, 0x07, 0x07, 0x06, 0x07, 0x06, 0x06, 0x07,
	0x06, 0x07, 0x06, 0x06, 0x06, 0x06, 0x07, 0x06,
	0x06, 0x06, 0x03, 0x07, 0x03, 0x07, 0x08, 0x03,
	0x05, 0x05, 0x04, 0x05, 0x05, 0x05, 0x05, 0x05,
	0x02, 0x03, 0x05, 0x02, 0x07, 0x06, 0x05, 0x05,
	0x05, 0x04, 0x04, 0x04, 0x05, 0x05, 0x07, 0x05,
	0x05, 0x04
};

static int glyphWidth(byte ch) {
	const uint idx = ch - 0x21;
	return idx < ARRAYSIZE(GLYPH_WIDTHS) ? GLYPH_WIDTHS[idx] : 0;
}

bool StoryText::load() {
	return _glyphs.load("proport.pcs");
}

void StoryText::draw(Graphics::ManagedSurface &dst, const Common::String &text, int &x, int &y,
		const StoryLayout &layout) const {
	auto at = [&](int i) -> byte {
		return i < (int)text.size() ? (byte)text[i] : 0;
	};
	auto regionAt = [&](int posY) {
		return (posY > layout._splitTop && posY < layout._splitBottom) ? 1 : 0;
	};

	int region = regionAt(y);
	int lineWidth = layout._right[region] - layout._left[region];
	int used = x - layout._left[region];
	int pos = 0;

	while (at(pos) != 0) {
		// Take as many characters as fit on the line
		int spaces = 0;
		int scan = pos;
		while (at(scan) != 0 && used < lineWidth && at(scan) != '\n') {
			const byte c = at(scan);
			if (c <= 0x20) {
				used += layout._spaceWidth;
				++spaces;
			} else if (c == '{') {
				used += INDENT_WIDTH;
			} else if (c != '_') {
				used += glyphWidth(c) + 1;
			}

			++scan;
		}

		// Then back up to a space or hyphenation point if the line overflowed
		for (;;) {
			if (at(scan) == 0 && used < lineWidth)
				break;
			if (at(scan) == '\n')
				break;
			if (--scan <= pos)
				break;

			if (at(scan) == ' ') {
				used -= layout._spaceWidth;
				--spaces;
				break;
			}

			if (at(scan) == '_' && used + HYPHEN_WIDTH + 1 < lineWidth) {
				used += HYPHEN_WIDTH + 1;
				break;
			}

			if (at(scan) != '_' && at(scan) != '{')
				used -= glyphWidth(at(scan)) + 1;
		}

		// Draw the line, sharing the leftover width between its spaces
		int slack = lineWidth - used;
		const int end = scan;
		for (int i = pos; i < end; ++i) {
			const byte c = at(i);
			if (c <= 0x20) {
				x += layout._spaceWidth;
				if (spaces != 0 && at(end) != 0 && at(end) != '\n') {
					x += slack / spaces;
					slack -= slack / spaces;
					--spaces;
				}
			} else if (c == '{') {
				x += INDENT_WIDTH;
			} else if (c != '_') {
				if (y < BOTTOM_LIMIT)
					_glyphs.draw(dst, c - 0x20, x, y, TEXT_COLOR);
				x += glyphWidth(c) + 1;
			}
		}

		pos = end;
		if (at(pos) != 0) {
			if (at(pos) == '_' && y < BOTTOM_LIMIT)
				_glyphs.draw(dst, HYPHEN_GLYPH, x, y, TEXT_COLOR);

			used = 0;
			y += LINE_HEIGHT;
			region = regionAt(y);
			x = layout._left[region];
			lineWidth = layout._right[region] - x;
			++pos;
		}
	}
}

} // namespace Gfx
} // namespace Ultima5
} // namespace Ultima
