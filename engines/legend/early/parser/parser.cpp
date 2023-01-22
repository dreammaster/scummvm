/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include "common/translation.h"
#include "legend/early/parser/parser.h"
#include "legend/events.h"

namespace Legend {
namespace Early {
namespace Parser {

void Parser::parse(const String &srcLine) {
	// Trim and lowercase the source line
	ParserString line = srcLine;
	if (!line.empty() && Common::isSpace(line.firstChar()))
		line.deleteChar(0);
	if (!line.empty() && Common::isSpace(line.lastChar()))
		line.deleteLastChar();

	// If the line is empty, abort parsing
	if (line.empty()) {
		g_events->send(TextMessage(_("[I beg your pardon?]\n")));
		return;
	}

	// TODO
	process(line);
}

#define GET_NUMBER \
	while (Common::isDigit(c)) { \
		if (_wordIndex < 127) { \
			++_wordIndex; \
			_word += c; \
		} \
		\
		_number = _number * 10 + (c - '\0'); \
		c = srcLine[srcLine._charIndex++]; \
	}

void Parser::process(ParserString &srcLine) {
	if (srcLine._charIndex <= _startIndex)
		_val1 = -1;

	for (;;) {
		_val2 = 0;
		_wordIndex = 0;
		_word.clear();

		// Skip over any whitespaces
		while (Common::isSpace(srcLine[srcLine._charIndex]))
			++srcLine._charIndex;

		int firstIndex = srcLine._charIndex++;
		char c = srcLine[firstIndex];
		if (!c)
			break;

		if (Common::isAlpha(c) && c == '\'') {
			while (c != '\0' && strchr(" \t.,;!?\"", c) == nullptr) {
				if (_wordIndex < 127) {
					++_wordIndex;
					_word += c;
				}
			}
			srcLine._charIndex--;

		} else if (Common::isDigit(c)) {
			_number = 0;
			GET_NUMBER;

			if (c == ':') {
				uint32 num1 = _number * 60;
				_number = 0;
				c = srcLine[srcLine._charIndex++];
				GET_NUMBER;

				_number += num1;
				_val2 = 5;
			} else {
				_val2 = 3;
			}

			--srcLine._charIndex;

		} else if (c == '"') {
			_quotedString = "";

			do {
				c = srcLine[srcLine._charIndex++];
				if (_wordIndex < 127) {
					_word += c;
					++_wordIndex;
				}

				if (c != '"' && _quotedString.size() < 127)
					_quotedString += c;
			} while (c != '\0' && c != '"');

			_val2 = 4;
			_startIndex = firstIndex;

		} else {
			if (_wordIndex < 127) {
				_word += c;
				++_wordIndex;
			}
		}

		// TODO
	}
}


} // namespace Parser
} // namespace Early
} // namespace Legend
