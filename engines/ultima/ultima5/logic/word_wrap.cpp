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

#include "ultima/ultima5/logic/word_wrap.h"

namespace Ultima {
namespace Ultima5 {

constexpr int LINE_MAX = 64;

void WordWrapPrinter::printString(const char *s, int width) {
	char line[LINE_MAX];
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
		const int remaining = width - column();

		for (; s[pos] != '\n' && s[pos] != '\r' && s[pos] != '\0' && length <= remaining && length < LINE_MAX - 1;
				++pos, ++length)
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

				if (column() != 0) {
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

} // namespace Ultima5
} // namespace Ultima
