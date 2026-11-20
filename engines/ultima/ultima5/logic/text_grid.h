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

#ifndef ULTIMA5_LOGIC_TEXT_GRID_H
#define ULTIMA5_LOGIC_TEXT_GRID_H

#include "ultima/ultima5/logic/word_wrap.h"

namespace Ultima {
namespace Ultima5 {

/**
 * The window that goes over the party, that lists of things and a person's stats
 * are printed in. It's a grid of characters, some of which are from the rune font, and
 * some of which have their colours swapped
 */
class TextGrid : public WordWrapPrinter {
public:
	static constexpr int COLUMNS = 16;
	static constexpr int ROWS = 9;

	struct Cell {
		byte _ch = ' ';
		bool _rune = false;
		bool _inverse = false;
		bool _set = false;
	};

private:
	Cell _cells[ROWS][COLUMNS];
	int _x = 0;
	int _y = 0;
	bool _rune = false;
	bool _inverse = false;

public:
	/**
	 * Empties the window and puts the cursor at its top left
	 */
	void clear();

	void setPosition(int x, int y) {
		_x = x;
		_y = y;
	}

	int row() const {
		return _y;
	}

	int column() const override {
		return _x;
	}

	void setRune(bool rune) {
		_rune = rune;
	}

	void toggleInverse() {
		_inverse = !_inverse;
	}

	void printChar(char ch) override;

	/**
	 * Prints text the way the original game printed strings
	 */
	void print(const char *text);

	/**
	 * Prints a number with at least a width, filled to it with a character
	 */
	void printNumber(int value, int width, char filler);

	/**
	 * Draws the box that's around lists, to a row from the top
	 */
	void drawFrame(int bottom);

	const Cell &cell(int x, int y) const {
		return _cells[y][x];
	}
};

} // namespace Ultima5
} // namespace Ultima

#endif
