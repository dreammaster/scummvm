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

#ifndef ULTIMA5_VIEWS_MESSAGE_LOG_H
#define ULTIMA5_VIEWS_MESSAGE_LOG_H

#include "common/str.h"
#include "ultima/shared/engine/events.h"

namespace Ultima {
namespace Ultima5 {
namespace Views {

constexpr int LOG_COLUMNS = 16;
constexpr int LOG_ROWS = 13;

/**
 * The scrolling text window at the bottom right of the screen, where the game
 * reports what's happening. It's an element of the game view, drawn within
 * the bounds it's given
 */
class MessageLog : public Shared::UIElement {
private:
	char _cells[LOG_ROWS][LOG_COLUMNS];
	bool _runes[LOG_ROWS][LOG_COLUMNS];
	bool _runeMode = false;
	int _x = 0;
	int _y = 0;
	int _cursorFrame = 0;

	void scroll();

protected:
	void timeout() override;

public:
	MessageLog(Graphics::Views::UIElement *parent);
	~MessageLog() override {}

	/**
	 * Empties the window
	 */
	void clear();

	/**
	 * Prints text at the cursor, breaking lines between words and scrolling
	 * the window when it gets to the bottom. A newline starts a new line
	 */
	void print(const Common::String &text);

	/**
	 * Prints one character at the cursor, wrapping at the edge of the window
	 */
	void printChar(char ch);

	/**
	 * Returns the column the cursor is in
	 */
	int column() const {
		return _x;
	}

	/**
	 * Has what is printed next be in the rune font, for the coordinates of places
	 */
	void setRunes(bool runes) {
		_runeMode = runes;
	}

	/**
	 * Rubs out characters before the cursor
	 */
	void backspace(int count);

	void draw() override;
};

} // namespace Views
} // namespace Ultima5
} // namespace Ultima

#endif
