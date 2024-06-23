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

#include "wasteland/wasteland1/gfx/pic.h"
#include "wasteland/wasteland1/files/vertical_xor_stream.h"

namespace Wasteland {
namespace Wasteland1 {
namespace Gfx {

Pic *Pic::read(Common::SeekableReadStream *stream,
		int w, int h, bool encoded) {
	Common::SeekableReadStream *xorStream;
	Pic *pic;
	int b;
	int x, y;

	pic = new Pic(w, h);
	if (encoded) {
		xorStream = new VerticalXorStream(stream, w, DisposeAfterUse::NO);
	} else {
		xorStream = stream;
	}

	for (y = 0; y < h; y++) {
		for (x = 0; x < w; x += 2) {
			b = xorStream->readByte();

			pic->setPixel(x, y, b >> 4);
			pic->setPixel(x + 1, y, b & 0xf);
		}
	}

	return pic;
}

} // namespace Gfx
} // namespace Wasteland1
} // namespace Wasteland
