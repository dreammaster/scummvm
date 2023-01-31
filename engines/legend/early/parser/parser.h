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
#include "legend/early/parser/parser_handlers.h"

namespace Legend {
namespace Early {
namespace Parser {

enum ParserResult {
	PR_UNKNOWN = 0, PR_END_OF_STRING = -1,
	PR_2 = -2, PR_3 = -3, PR_4 = -4, PR_5 = -5, PR_8 = -8,
	PR_9 = -9, PR_10 = -10, PR_11 = -11, PR_12 = -12, PR_13 = -13,
	PR_14 = -14, PR_15 = -15, PR_16 = -16, PR_17 = -17,
	PR_COMMA = -18, PR_PERIOD = -19, PR_SEMICOLON = -20, PR_21 = -21,
	PR_END_OF_LIST = -22
};

enum ParserMode {
	PM_0, PM_1, PM_2, PM_3, PM_4, PM_5, PM_6, PM_7
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
 * Holds a pair of the parser result from parsing a word and it's vocab Id
 */
struct StateVocab {
	int _status;
	int _vocabId;
};

/**
 * Main parser
 */
class Parser {
private:
	int _startIndex = 0;
	int _vocabId = 0;
	int _val1 = -1;
	int _result = -1;
	int _val4 = 0;
	int _val5 = 0;
	int _val6 = 0;
	int _val7 = 0;
	int _val8 = 0;
	int _val9 = 0;
	int _val10 = 0;
	int _val11 = 0;
	int _val12 = 0;

	int _wordIndex = 0;
	ParserString _inputLine;
	ParserString _inputLineCopy;
	String _subLine;
	String _word;
	String _quotedString;
	const char *_unknownWord;
	int _unknownFirstIndex = 0;
	int _unknownWordIndex = 0;
	HandlerDataSub2 _tmpSub;
	VocabSet _tmpSub2;
	ParserMode _mode = PM_0;
	StateVocab _stateArray[100];
	bool _firstTimeSetup = false;

	/**
	 * Adds a line to the text window
	 */
	void addLine(const Common::U32String &line);

	void parseLoop();
	int oops();
	void parseLine(ParserString &inputLine);
	void parseLineSetup();
	int handleLine();
	int proc3();

	/**
	 * Gets the index of the parser handler function
	 */

	/**
	 * Displays that you don't need a given word
	 */
	void youDontNeed(const String &word);

	/**
	 * Displays that an undo can't be done
	 */
	bool performUndo();

	/**
	 * Parses a word within the sentence
	 */
	int parseWord(ParserString &line);

	/**
	 * Special handler for speaking to characters
	 */
	int speakHandler();

	/**
	 * No special handler for use actions
	 */
	int useHandler() { return 0; }

protected:
	ParserHandlers _handlers;
	const String _EMPTY_LINE;
	const int _AGAIN;
	const int _ALL;
	const int _AND;
	const int _BUT;
	const int _OF;
	const int _OOPS;
	const int _THEN;
	const int _TO;
	const int _UNDO;

	/**
	 * Asks the user to rephrase the question
	 */
	virtual void pleaseRephrase() = 0;

	/**
	 * What do you want to do method
	 */
	virtual void whatDoYouWant() = 0;

	virtual int getAltVocab(int vocabId1, int vocabId2) const = 0;

public:
	static constexpr int NUMBER_VOCAB_ID = 3;
	uint32 _number = 0;
	int _val2 = 211;
	int _val20 = 0;
	int _val21 = 0;

public:
	/**
	 * Constructor
	 */
	Parser(const String &emptyLine,
		int againId, int allId, int andId, int butId,
		int ofId, int oopsId, int thenId, int toId, int undoId);

	/**
	 * Parses a passed input line
	 */
	void parse(const String &srcLine);
};

} // namespace Parser
} // namespace Early
} // namespace Legend

#endif
