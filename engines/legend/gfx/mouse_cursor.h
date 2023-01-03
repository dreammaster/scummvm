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

#ifndef LEGEND_GFX_MOUSE_CURSOR_H
#define LEGEND_GFX_MOUSE_CURSOR_H

#include "common/scummsys.h"
#include "common/rect.h"
#include "common/stream.h"
#include "legend/utils/string.h"

namespace Legend {

#define CURSOR_WIDTH 16
#define CURSOR_HEIGHT 16

class LegendEngine;

namespace Early {

enum CursorId {
	CURSOR_ARROW = 0
};

} // End of namespace Early

namespace Later {
namespace Xanth {
namespace Gfx {

enum CursorId {
	CURSOR_BIG_ARROW = 0,
	CURSOR_SMALL_ARROW = 1,
	CURSOR_TICK = 2,
	CURSOR_HOURGLASS = 3
};

} // namespace Gfx
} // namespace Xanth
} // namespace Later

namespace Gfx {

/**
 * Handles the dislay and management of the on-screen mouse cursor
 */
class MouseCursor {
	struct MouseCursorData {
		Common::Point _hotspot;
		uint16 _pixels[16];
		uint16 _mask[16];

		void load(Common::SeekableReadStream &s);
	};
private:
	Common::Array<MouseCursorData> _cursors;
	int _cursorId;
private:
	/**
	 * Loads the mosue cursors from the specified resource
	 */
	void loadCursors(const String &name);
public:
	MouseCursor();

	/**
	 * Sets the active cursor
	 */
	void setCursor(int cursorId);

	/**
	 * Returns the curent cursor
	 */
	int getCursor() const {
		return _cursorId;
	}

	/**
	 * Make the mouse cursor visible
	 */
	void show();

	/**
	 * Hide the mouse cursor
	 */
	void hide();
};

} // namespace Gfx
} // namespace Legend

#endif
