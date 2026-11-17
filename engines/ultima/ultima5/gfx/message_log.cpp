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

#include "ultima/ultima5/gfx/message_log.h"

namespace Ultima {
namespace Ultima5 {
namespace Gfx {

constexpr int LOG_FIRST_COLUMN = 24;
constexpr int LOG_FIRST_ROW = 11;

void MessageLog::clear() {
	memset(_cells, ' ', sizeof(_cells));
	_x = _y = 0;
}

void MessageLog::scroll() {
	memmove(_cells[0], _cells[1], (LOG_ROWS - 1) * LOG_COLUMNS);
	memset(_cells[LOG_ROWS - 1], ' ', LOG_COLUMNS);
	_y = LOG_ROWS - 1;
}

void MessageLog::print(const Common::String &text) {
	for (uint i = 0; i < text.size(); ++i) {
		if (text[i] == '\n') {
			_x = 0;
			++_y;
		} else {
			_cells[_y][_x++] = text[i];
			if (_x == LOG_COLUMNS) {
				_x = 0;
				++_y;
			}
		}

		if (_y == LOG_ROWS)
			scroll();
	}
}

void MessageLog::draw(const TextScreen &screen, Graphics::ManagedSurface &dst, bool showCursor,
		int cursorFrame) const {
	for (int y = 0; y < LOG_ROWS; ++y) {
		for (int x = 0; x < LOG_COLUMNS; ++x)
			screen.drawCell(dst, LOG_FIRST_COLUMN + x, LOG_FIRST_ROW + y, _cells[y][x], TEXT_WHITE, TEXT_BLACK);
	}

	if (showCursor)
		screen.drawCursor(dst, LOG_FIRST_COLUMN + _x, LOG_FIRST_ROW + _y, cursorFrame);
}

} // namespace Gfx
} // namespace Ultima5
} // namespace Ultima
