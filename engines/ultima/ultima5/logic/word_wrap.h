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

#ifndef ULTIMA5_LOGIC_WORD_WRAP_H
#define ULTIMA5_LOGIC_WORD_WRAP_H

#include "common/scummsys.h"

namespace Ultima {
namespace Ultima5 {

/**
 * Something that text is printed to a character at a time, in a window of
 * a certain width, that can have whole strings of text printed to it with
 * the lines broken between words the way the original game did it
 */
class WordWrapPrinter {
public:
	virtual ~WordWrapPrinter() {}

	/**
	 * Prints a character at the cursor, wrapping at the edge of the window
	 */
	virtual void printChar(char ch) = 0;

	/**
	 * Returns the column the cursor is in
	 */
	virtual int column() const = 0;

	/**
	 * Prints text, breaking lines between words
	 * @param text		The text, which can have newlines in it
	 * @param width		The number of the last column of the window
	 */
	void printString(const char *text, int width);
};

} // namespace Ultima5
} // namespace Ultima

#endif
