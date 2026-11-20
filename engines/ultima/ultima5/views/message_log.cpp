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
#include "ultima/ultima5/logic/word_wrap.h"
#include "ultima/ultima5/views/view.h"
#include "ultima/ultima5/ultima5.h"

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
	memset(_runes, 0, sizeof(_runes));
	_x = _y = 0;
	redraw();
}

void MessageLog::scroll() {
	memmove(_cells[0], _cells[1], (LOG_ROWS - 1) * LOG_COLUMNS);
	memset(_cells[LOG_ROWS - 1], ' ', LOG_COLUMNS);
	memmove(_runes[0], _runes[1], (LOG_ROWS - 1) * LOG_COLUMNS);
	memset(_runes[LOG_ROWS - 1], 0, LOG_COLUMNS);
	_y = LOG_ROWS - 1;
}

void MessageLog::printChar(char ch) {
	if (ch == '\n') {
		_x = 0;
		++_y;
	} else {
		_runes[_y][_x] = _runeMode;
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

		_runes[_y][_x - 1] = false;
		_cells[_y][--_x] = ' ';
	}

	redraw();
}

// Prints to the window a character at a time, for the printing of strings
class LogPrinter : public WordWrapPrinter {
private:
	MessageLog &_log;

public:
	LogPrinter(MessageLog &log) : _log(log) {}

	void printChar(char ch) override {
		_log.printChar(ch);
	}

	int column() const override {
		return _log.column();
	}
};

void MessageLog::print(const Common::String &text) {
	LogPrinter(*this).printString(text.c_str(), LOG_COLUMNS - 1);
}

void MessageLog::draw() {
	auto s = getSurface();
	s.setColor(COLOR_WHITE, COLOR_BLACK);

	for (int y = 0; y < LOG_ROWS; ++y)
		s.writeString(Common::Point(0, y), Common::String(_cells[y], LOG_COLUMNS));

	// The rune font is drawn over the places that have it
	for (int y = 0; y < LOG_ROWS; ++y) {
		for (int x = 0; x < LOG_COLUMNS; ++x) {
			if (_runes[y][x]) {
				s.fillRect(Common::Rect(x * 8, y * 8, x * 8 + 8, y * 8 + 8), COLOR_BLACK);
				g_engine->_runes->drawChar(s.surfacePtr(), (byte)_cells[y][x], x * 8, y * 8, COLOR_WHITE);
			}
		}
	}

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
