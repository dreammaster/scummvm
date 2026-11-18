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

#include "ultima/ultima5/views/message_log.h"
#include "ultima/ultima5/views/view.h"

namespace Ultima {
namespace Ultima5 {
namespace Views {

constexpr int FRAMES_PER_CURSOR_STEP = 3;
constexpr byte GLYPH_CURSOR = 5;
constexpr int CURSOR_FRAME_COUNT = 4;

MessageLog::MessageLog(Graphics::Views::UIElement *parent) : Shared::UIElement("MessageLog", parent) {
	clear();
	delayFrames(FRAMES_PER_CURSOR_STEP);
}

void MessageLog::clear() {
	memset(_cells, ' ', sizeof(_cells));
	_x = _y = 0;
	redraw();
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

	redraw();
}

void MessageLog::draw() {
	auto s = getSurface();
	s.setColor(COLOR_WHITE, COLOR_BLACK);

	for (int y = 0; y < LOG_ROWS; ++y)
		s.writeString(Common::Point(0, y), Common::String(_cells[y], LOG_COLUMNS));

	s.writeChar(Common::Point(_x, _y), GLYPH_CURSOR + _cursorFrame);
}

void MessageLog::timeout() {
	_cursorFrame = (_cursorFrame + 1) % CURSOR_FRAME_COUNT;
	redraw();
	delayFrames(FRAMES_PER_CURSOR_STEP);
}

} // namespace Views
} // namespace Ultima5
} // namespace Ultima
