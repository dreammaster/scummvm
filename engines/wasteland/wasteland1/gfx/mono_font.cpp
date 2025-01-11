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

#include "wasteland/core/file.h"
#include "wasteland/wasteland1/gfx/mono_font.h"

namespace Wasteland {
namespace Wasteland1 {
namespace Gfx {

#define FIRST_CHAR 32
#define CHAR_COUNT 128

void MonoFont::load() {
	File f("mono.fnt");
	assert(f.size() == (CHAR_COUNT * 8));

	_data.resize((CHAR_COUNT * 8));
	f.read(&_data[0], (CHAR_COUNT * 8));
}

void MonoFont::drawChar(Graphics::Surface *dst, uint32 chr, int x, int y, uint32 color) const {
	assert(chr >= FIRST_CHAR && chr < (FIRST_CHAR + CHAR_COUNT));
	const byte *src = &_data[(chr - FIRST_CHAR) * 8];
	byte *dest;
	byte bits;

	bool isReverse = ((int32)color) < 0;
	byte col = (byte)ABS((int32)color);

	for (int yc = 0; yc < FONT_H; ++yc, ++y, ++src) {
		bits = *src;
		dest = (byte *)dst->getBasePtr(x, y);

		for (int xc = 0; xc < FONT_W; ++xc, ++dest, bits <<= 1) {
			if (((bits & 0x80) && !isReverse) ||
					(!(bits & 0x80) && isReverse))
				*dest = col;
		}
	}
}

} // namespace Gfx
} // namespace Wasteland1
} // namespace Wasteland
