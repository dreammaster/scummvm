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

#ifndef LEGEND_EARLY_PARSER_PARSER_H
#define LEGEND_EARLY_PARSER_PARSER_H

#include "legend/utils/string.h"

namespace Legend {
namespace Early {
namespace Parser {

enum ParserResult {
	PR_UNKOWN = 0, PR_END_OF_STRING = -1,
	PR_2 = -2, PR_3 = -3, PR_4 = -4, PR_8 = -8, PR_9 = -9,
	PR_10 = -10, PR_13 = -13, PR_14 = -14, PR_15 = -15,
	PR_16 = -16, PR_17 = -17, PR_COMMA = -18, PR_PERIOD = -19,
	PR_SEMICOLON = -20
};

/**
 * Encapulates a string and a character index to represent
 * the current point in parsing the string
 */
class ParserString : public String {
public:
	int _charIndex = 0;

	ParserString() {}
	ParserString(const String &str, int charIndex = 0) : String(str) {}
};

/**
 * Main parser
 */
class Parser {
private:
	int _startIndex = 0;
	int _val1 = -1;
	int _vocabId = 0;
	int _result = -1;
	int _wordIndex = 0;
	uint32 _number = 0;
	String _word;
	String _quotedString;
	String _unknownWord;
	int _unknownFirstIndex = 0;
	int _unknownWordIndex = 0;

	int process(ParserString &srcLine);

public:
	/**
	 * Parses a passed input line
	 */
	void parse(const String &srcLine);
};

} // namespace Parser
} // namespace Early
} // namespace Legend

#endif
