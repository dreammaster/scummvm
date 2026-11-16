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

#include "common/file.h"
#include "graphics/surface.h"
#include "ultima/ultima5/gfx/charset.h"

namespace Ultima {
namespace Ultima5 {
namespace Gfx {

CharSet::CharSet(const char *filename) {
	Common::File f;
	if (!f.open(filename) || f.read(_data, sizeof(_data)) != sizeof(_data))
		error("Could not load %s", filename);
}

void CharSet::drawChar(Graphics::Surface *dst, uint32 chr, int x, int y, uint32 color) const {
	if (chr >= CHARSET_GLYPHS)
		return;

	const byte *glyph = &_data[chr * 8];

	for (int yy = 0; yy < 8; ++yy) {
		if (y + yy < 0 || y + yy >= dst->h)
			continue;

		for (int xx = 0; xx < 8; ++xx) {
			if ((glyph[yy] & (0x80 >> xx)) && x + xx >= 0 && x + xx < dst->w)
				*(byte *)dst->getBasePtr(x + xx, y + yy) = color;
		}
	}
}

} // namespace Gfx
} // namespace Ultima5
} // namespace Ultima
