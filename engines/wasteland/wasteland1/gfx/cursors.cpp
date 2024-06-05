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
#include "wasteland/wasteland1/gfx/cursors.h"

namespace Wasteland {
namespace Wasteland1 {
namespace W1Gfx {

#define CURSOR_W 16
#define CURSOR_H 16

void Cursors::load(CursorArray &cursors) {
	File f("curs");
	cursors.resize(8);

	int cursorNum;
	int b;
	int x, y;
	int type, bit, pixel;
	Graphics::ManagedSurface *image;

	for (cursorNum = 0; cursorNum < 8; ++cursorNum) {
		Cursor &curs = cursors[cursorNum];
		curs._image.create(CURSOR_W, CURSOR_H);
		curs._mask.create(CURSOR_W, CURSOR_H);

		for (bit = 0; bit < 4; bit++) {
			for (y = 0; y < CURSOR_H; y++) {
				for (type = 0; type < 2; type++) {
					image = type == 0 ? &curs._mask : &curs._image;

					for (x = CURSOR_W - 8; x >= 0; x -= 8) {
						b = f.readByte();

						for (pixel = 0; pixel < 8; pixel++) {
							image->setPixel(x + pixel, y,
								image->getPixel(x + pixel, y)
								| (((b >> (7 - pixel)) & 1) << bit));
						}
					}
				}
			}
		}

		for (y = 0; y < CURSOR_H; ++y) {
			for (x = 0; x < CURSOR_W; ++x) {
				curs._mask.setPixel(x, y,
					curs._mask.getPixel(x, y) ?
					kCursorMaskOpaque : kCursorMaskTransparent);
			}
		}
	}
}

} // namespace W1Gfx
} // namespace Wasteland1
} // namespace Wasteland
