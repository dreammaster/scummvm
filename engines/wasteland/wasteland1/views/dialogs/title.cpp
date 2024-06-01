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

#include "common/system.h"
#include "graphics/palette.h"
#include "wasteland/wasteland1/views/dialogs/title.h"
#include "wasteland/core/file.h"

namespace Wasteland {
namespace Wasteland1 {
namespace Views {
namespace Dialogs {

#define TITLE_W 288
#define TITLE_H 128

bool Title::msgKeypress(const KeypressMessage &msg) {
	timeout();
	return true;
}

bool Title::msgMouseDown(const MouseDownMessage &msg) {
	timeout();
	return true;
}

void Title::draw() {
	Surface s = getSurface();
	s.blitFrom(_surface);
}

bool Title::msgFocus(const FocusMessage &msg) {
	// Read title in
	File f("title.pic");
	byte tmp[TITLE_W / 2 * TITLE_H];
	f.read(tmp, TITLE_W / 2 * TITLE_H);

	// Do a vertical xor decoding
	byte *s1 = tmp;
	byte *s2 = tmp + TITLE_W / 2;
	for (int i = 0; i < (TITLE_W / 2 * (TITLE_H - 1)); ++i, ++s1, ++s2)
		*s2 ^= *s1;

	// Load into surface
	_surface.create(TITLE_W, TITLE_H);

	const byte *pSrc = tmp;
	byte *pDest = (byte *)_surface.getPixels();
	for (int i = 0; i < (TITLE_W / 2 * (TITLE_H - 1)); ++i, ++pSrc) {
		*pDest++ = *pSrc & 0xf;
		*pDest++ = *pSrc >> 4;
	}

	//delaySeconds(1);
	return true;
}

void Title::timeout() {
}

} // namespace Dialogs
} // namespace Views
} // namespace Wasteland1
} // namespace Wasteland
