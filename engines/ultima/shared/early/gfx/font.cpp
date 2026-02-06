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

#include "ultima/shared/early/gfx/font.h"
#include "graphics/managed_surface.h"

namespace Ultima {
namespace Shared {
namespace Gfx {

Font::Font(const byte *data, size_t startingChar, size_t charCount) :
	_data(data), _startingChar(startingChar), _endingChar(startingChar + charCount - 1) {}

void Font::drawChar(Graphics::Surface *dst, uint32 chr, int x, int y, uint32 color) const {
	assert(chr >= _startingChar && chr <= _endingChar);
	const byte *charP = _data + (chr - _startingChar) * 8;

	for (int yp = 0; yp < 8; ++yp, ++y) {
		byte *lineP = (byte *)dst->getBasePtr(x, y);
		byte lineData = charP[yp];

		for (int xp = 0; xp < 8; ++xp, lineData <<= 1, ++lineP) {
			if (lineData & 0x80)
				*lineP = color;
		}
	}
}

} // End of namespace Gfx
} // End of namespace Shared
} // End of namespace Ultima
