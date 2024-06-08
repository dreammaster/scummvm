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
#include "wasteland/wasteland1/gfx/color_font.h"

namespace Wasteland {
namespace Wasteland1 {
namespace W1Gfx {

void ColorFont::load() {
	File f("colorf.fnt");
	_chars.resize(172);

	int i;
	int b;
	int y;
	int bit, pixel;

	for (i = 0; i < 172; ++i) {
		Graphics::ManagedSurface &s = _chars[i];
		s.create(FONT_W, FONT_H);

		for (bit = 0; bit < 4; bit++) {
			for (y = 0; y < 8; y++) {
				b = f.readByte();

				for (pixel = 0; pixel < 8; pixel++) {
					s.setPixel(pixel, y, s.getPixel(pixel, y)
						| (((b >> (7 - pixel)) & 1) << bit));
				}
			}
		}
	}
}

void ColorFont::drawChar(Graphics::Surface *dst, uint32 chr, int x, int y, uint32 color) const {
	assert(chr >= 0 && chr < _chars.size());
	const Graphics::ManagedSurface &charImage = _chars[chr];

	Graphics::ManagedSurface dest(dst, DisposeAfterUse::NO);
	dest.blitFrom(charImage, Common::Point(x, y));
}

} // namespace W1Gfx
} // namespace Wasteland1
} // namespace Wasteland
