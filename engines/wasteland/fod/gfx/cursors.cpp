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
#include "wasteland/fod/gfx/cursors.h"

namespace Wasteland {
namespace FOD {
namespace Gfx {

#define CURSOR_W 8
#define CURSOR_H 8

static const byte CURSOR_ARROW_DATA[CURSOR_W * CURSOR_H] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,
	0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00,
	0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00
};

void Cursors::load(CursorArray &cursors) {
	cursors.resize(1);
	Cursor &c = cursors[0];
	c._image.create(CURSOR_W, CURSOR_H);
	c._mask.create(CURSOR_W, CURSOR_H);

	const byte *src = CURSOR_ARROW_DATA;
	byte *dest = (byte *)c._image.getPixels();
	byte *mask = (byte *)c._mask.getPixels();

	for (int i = 0; i < CURSOR_W * CURSOR_H; ++i, ++src, ++dest, ++mask) {
		*dest = *src;
		*mask = *src ? kCursorMaskOpaque : kCursorMaskTransparent;
	}
}

} // namespace Gfx
} // namespace FOD
} // namespace Wasteland
