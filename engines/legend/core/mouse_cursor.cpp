/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include "graphics/cursorman.h"
#include "legend/core/mouse_cursor.h"
#include "legend/legend.h"

namespace Legend {

MouseCursor::MouseCursor(LegendEngine *vm) {
	int count;
	const MouseCursorData *data;

	switch (vm->getGameID()) {
	case GType_CompanionsOfXanth:
		data = Later::Xanth::XANTH_CURSORS;
		count = 4;
		break;
	default:
		return;
	}

	// Copy the cursors into an array. This will give better protention against
	// invalid cursor indexes later on
	_cursors.resize(count);
	Common::copy(&data[0], &data[count], &_cursors[0]);
	
	_cursorId = -1;
	setCursor(0);
}

void MouseCursor::setCursor(int cursorId) {
	// No need to do anything if we're already showing the desired cursor
	if (cursorId == _cursorId)
		return;
	_cursorId = cursorId;

	// Set up a temporary surface for rendering the cursor onto
	Graphics::ManagedSurface s(16, 16);
	s.fillRect(s.getBounds(), 0xff);
	
	const MouseCursorData &data = _cursors[cursorId];
	const uint16 *pixelsP = data._pixels, *maskP = data._masks;

	// Iterate trhough the lines to build up the cursor
	for (int y = 0; y < CURSOR_HEIGHT; ++y) {
		uint16 pixVal = *pixelsP++, maskVal = *maskP++;
		int bitMask = 0x8000;
		byte *destP = (byte *)s.getBasePtr(0, y);

		for (int x = 0; x < CURSOR_WIDTH; ++x, ++destP, bitMask >>= 1) {
			if (pixVal & bitMask)
				*destP = 15;
			else if (!(maskVal & bitMask))
				*destP = 0;
		}
	}

	// Pass the generated surface onto the ScummVM cursor manager
	CursorMan.replaceCursor(s.getPixels(), CURSOR_WIDTH, CURSOR_HEIGHT, data._hotspotX, data._hotspotY, 0xff);
}

void MouseCursor::show() {
	CursorMan.showMouse(true);
}

void MouseCursor::hide() {
	CursorMan.showMouse(false);
}

} // End of namespace Legend
