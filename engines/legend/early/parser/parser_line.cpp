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

/**
 * This file contains part of the Parser class implementation,
 * specifically dealing with parsing the content of inputted lines
 */

#include "legend/early/parser/parser.h"
#include "legend/early/engine.h"

namespace Legend {
namespace Early {
namespace Parser {

void Parser::parseLineSetup() {
	// TODO
}

int Parser::handleLine() {
	parseLine(_inputLine);

	// TODO
	return 0;
}

void Parser::parseLine(ParserString &inputLine) {
	bool flag1 = false, flag2 = false;
	bool flag3 = false, flag4 = false;
	bool flag5 = false;
	StateVocab *statePtr = nullptr;
	int savedIndex = 0;
	int wordResult = 0, wordVocabId = 0;
//	int val1, val2;

	for (int idx = 3; ; ++idx) {
		if (!flag1 || idx == 2)
			statePtr = _stateArray;

		//charIndex = flag2 ? savedIndex : inputLine._charIndex;
		do {
			if (flag2) {
				flag2 = false;
				statePtr->_status = wordResult;
				statePtr->_vocabId = wordVocabId;
			} else {
				statePtr->_status = parseWord(inputLine);
				statePtr->_vocabId = _vocabId;
			}

			if (statePtr->_status < PR_UNKNOWN && statePtr->_vocabId > 0 &&
					statePtr->_vocabId <= g_engine->_vocab->size()) {
				if ((*g_engine->_vocab)[statePtr->_vocabId]._flags & VFLAG_1)
					flag4 = true;
			}

			switch (statePtr->_status) {
			case PR_17:
			case PR_SEMICOLON:
			case PR_END_OF_STRING:
			case PR_UNKNOWN:
				flag3 = true;
				break;

			case PR_PERIOD:
				savedIndex = inputLine._charIndex;
				wordResult = parseWord(inputLine);
				wordVocabId = _vocabId;
				flag2 = true;

				if (wordResult != PR_END_OF_STRING)
					flag3 = true;
				break;

			case PR_21:
				break;

			case PR_13:
				savedIndex = inputLine._charIndex;
				wordResult = parseWord(inputLine);
				wordVocabId = _vocabId;
				flag2 = true;

				if (wordResult != PR_17 && wordResult < PR_UNKNOWN &&
						wordVocabId > 0 && wordVocabId <= g_engine->_vocab->size()) {
					const VocabEntry &ve = (*g_engine->_vocab)[wordVocabId];
					if ((ve._flags & VFLAG_1) & !(ve._flags & (VFLAG_4 | VFLAG_2))) {
						flag1 = true;
						flag3 = true;
					}
				}
				break;

			default:
				break;
			}

			++statePtr;
		} while (!flag3);

		statePtr->_status = PR_END_OF_LIST;
		statePtr->_vocabId = PR_END_OF_LIST;
		_mode = PM_0;

		if (flag5) {
			idx = 4;
			_mode = PM_5;

		} else {
			// TODO: switch
		}
	}
}

} // namespace Parser
} // namespace Early
} // namespace Legend
