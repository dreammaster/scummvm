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

#include "graphics/cursorman.h"
#include "krondor/gfx/mouse_cursor.h"
#include "krondor/res/resources.h"
#include "krondor/events.h"

namespace Krondor {
namespace Gfx {

MouseCursors::MouseCursors() {
	CursorMan.showMouse(true);
}

void MouseCursors::load() {
	_cursors.load("pointerg.bmx");
	assert(_cursors._images.size() == 27);

	setCursor(PT_SWORD);
}

void MouseCursors::setCursor(uint cursorNum) {
	assert(cursorNum < _cursors._images.size());

	_currentCursor = cursorNum;
	CursorMan.replaceCursor(_cursors._images[cursorNum], 0, 0, 0);
}

} // namespace Gfx
} // namespace Krondor
