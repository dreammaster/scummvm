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
#include "wasteland/wasteland1/files/nibble_stream.h"
#include "wasteland/wasteland1/files/vertical_xor_stream.h"

namespace Wasteland {
namespace Wasteland1 {
namespace Gfx {

Pic *Pic::read(Common::ReadStream *stream,
		int w, int h, bool encoded) {
	Common::ReadStream *xorStream;
	Pic *pic;

	// Create the pic object
	pic = new Pic(w, h);

	// Set up decoder stream
	if (encoded)
		xorStream = new VerticalXorStream(stream, w, DisposeAfterUse::NO);
	else
		xorStream = new NibbleStream(stream, DisposeAfterUse::NO);

	// Read the pixels
	xorStream->read(pic->getPixels(), w * h);

	delete xorStream;
	return pic;
}

Pic *Pic::clone() const {
	Pic *copy = new Pic(w, h);
	copy->blitFrom(*this);

	return copy;
}

} // namespace Gfx
} // namespace Wasteland1
} // namespace Wasteland
