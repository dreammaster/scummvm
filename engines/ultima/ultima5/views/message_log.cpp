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

void MessageLog::printChar(char ch) {
	if (ch == '\n') {
		_x = 0;
		++_y;
	} else {
		_cells[_y][_x++] = ch;
		if (_x == LOG_COLUMNS) {
			_x = 0;
			++_y;
		}
	}

	if (_y == LOG_ROWS)
		scroll();

	redraw();
}

void MessageLog::backspace(int count) {
	for (; count > 0 && (_x != 0 || _y != 0); --count) {
		if (_x == 0) {
			--_y;
			_x = LOG_COLUMNS;
		}

		_cells[_y][--_x] = ' ';
	}

	redraw();
}

void MessageLog::print(const Common::String &text) {
	const char *s = text.c_str();
	const int width = LOG_COLUMNS - 1;
	char line[LOG_COLUMNS + 2];
	int pos = 0;
	bool breakFirst = false;
	bool finished = false;

	if (*s == '\0')
		return;

	// Text is cut into a line at a time, each ending at a space where it
	// can, and the rest carries on the next line
	do {
		if (breakFirst)
			printChar('\n');

		bool skipSpaces = false;
		int start = 0;
		int length = 0;
		breakFirst = false;
		const int remaining = width - _x;

		for (; s[pos] != '\n' && s[pos] != '\r' && s[pos] != '\0' && length <= remaining; ++pos, ++length)
			line[length] = s[pos];

		if (length == 0) {
			if (s[pos] != '\0')
				printChar(s[pos++]);
			else
				finished = true;
			continue;
		}

		if (length > remaining) {
			const int fullLength = length;
			const int fullPos = pos;

			for (; s[pos] != ' ' && length != 0 && s[pos] != '\n' && s[pos] != '\0' && s[pos] != '\r'; --length, --pos) {
			}

			if (length == 0) {
				// A word too long for the line is split
				length = fullLength;
				pos = fullPos;

				if (_x != 0) {
					printChar('\n');
					skipSpaces = true;
				}
			} else {
				do {
					--length;
				} while (line[length] == ' ' || line[length] == '\r' || line[length] == '\n');

				if (s[pos] != '\0')
					++pos;

				if (length < remaining)
					breakFirst = true;
			}

			if (length > remaining)
				--length;
		} else {
			--length;
		}

		if (skipSpaces) {
			while (line[start] == ' ')
				++start;
		}

		for (; start <= length; ++start)
			printChar(line[start]);
	} while (!finished);
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
