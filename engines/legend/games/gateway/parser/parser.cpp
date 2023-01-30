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

#include "common/translation.h"
#include "legend/games/gateway/parser/parser.h"
#include "legend/games/gateway/engine.h"

namespace Legend {
namespace Early {
namespace Gateway {
namespace Parser {

enum {
	PARSER_AGAIN = 27,
	PARSER_ALL = 41,
	PARSER_AND = 50,
	PARSER_BUT = 273,
	PARSER_OF = 1521,
	PARSER_OOPS = 1536,
	PARSER_THEN = 2346,
	PARSER_TO = 2382,
	PARSER_UNDO = 2473,

	ASK = 83,
	ORDER = 1548,
	TELL = 2325,
	USE = 2495,
	MY = 1439
};

#define ARRAY2_OFFSET1 176
#define ARRAY2_COUNT1 13

Parser::Parser() : Legend::Early::Parser::Parser(
		String(_("[I beg your pardon?]\n")),
		PARSER_AGAIN, PARSER_ALL, PARSER_AND, PARSER_BUT, PARSER_OF,
		PARSER_OOPS, PARSER_THEN, PARSER_TO, PARSER_UNDO) {
	_handlers._functions[0]._minId = ASK;
	_handlers._functions[0]._minId = ORDER;
	_handlers._functions[0]._minId = TELL;
	_handlers._functions[0]._minId = USE;

	// TODO: Set arrays and sizes
}

void Parser::pleaseRephrase() {
	g_engine->addText(0xc402);
}

void Parser::whatDoYouWant() {
	g_engine->addText(0xc403);

	int objNum = g_engine->_logics->_logicNum211;
	if (_val2 != objNum)
		g_engine->_logics->printObj(130, objNum);

	g_engine->addText(" to ");
	g_engine->_vocab->display(_handlers._newVocabId1);

	int vocabId = _handlers._newVocabId2;
	if (vocabId) {
		g_engine->addText(" ");
		g_engine->_vocab->display(vocabId);
	}

	if (_handlers._field34) {
		bool flag = false;

		for (int idx = 0; idx < _handlers._sub2._val1; ++idx) {
			if (idx > 0) {
				if ((idx + 1) == _handlers._sub2._val1) {
					g_engine->addText(" and");
				} else {
					g_engine->addText(",");
				}
			}

			const Early::Parser::HandlerData2 *entryP = &_handlers._sub2._ptr[idx];
			for (int idx2 = 0; idx2 < entryP->_vocabCount && !flag; ++idx2) {
				flag = entryP->_vocabIds[idx2] == MY;
			}

			if (!flag && !entryP->_val2)
				g_engine->addText(" the ");
			else
				g_engine->addText(" ");

			entryP->display();
		}

		for (int idx = 0; idx < _handlers._sub2._val2; ++idx) {
			if (idx == 0) {
				g_engine->addText(" but");
			} else if ((idx + 1) == _handlers._sub2._val2) {
				g_engine->addText(" and");
			} else {
				g_engine->addText(",");
			}

			const Early::Parser::HandlerData2 *entry = &_handlers._sub2._ptr2[idx];
			if (!entry->_val2)
				g_engine->addText(" the ");
			else
				g_engine->addText(" ");
			entry->display();
		}
	}

	g_engine->addText("?]\n");
}

} // namespace Parser
} // namespace Gateway
} // namespace Early
} // namespace Legend
