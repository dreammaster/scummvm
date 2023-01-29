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
#include "legend/early/engine.h"
#include "legend/events.h"

namespace Legend {
namespace Early {
namespace Parser {

Parser::Parser(const String &emptyLine, int againId, int allId,
		int andId, int butId, int ofId, int oopsId, int thenId,
		int toId, int undoId) :
		_EMPTY_LINE(emptyLine), _AGAIN(againId), _ALL(allId), _AND(andId),
		_BUT(butId), _OF(ofId), _OOPS(oopsId), _THEN(thenId), _TO(toId),
		_UNDO(undoId) {
}

void Parser::parse(const String &srcLine) {
	// Trim and lowercase the source line
	_inputLine = srcLine;
	if (!_inputLine.empty() && Common::isSpace(_inputLine.firstChar()))
		_inputLine.deleteChar(0);
	if (!_inputLine.empty() && Common::isSpace(_inputLine.lastChar()))
		_inputLine.deleteLastChar();

	// If the line is empty, abort parsing
	if (_inputLine.empty()) {
		g_events->send(TextMessage(_EMPTY_LINE));
		return;
	}

	_val3 = _val2;
	_val4 = 0;

	if (_OOPS) {
		// Start processing line
		(void)parseWord(_inputLine);

		if (_vocabId) {
			const VocabEntry &ve = (*g_engine->_vocab)[_vocabId];
			if ((ve._flags & VFLAG_1) &&
					(_vocabId == _OOPS || ve._altVocabId == _OOPS)) {
				if (oops() > 0)
					parseLoop();
				return;
			}
		}

		_inputLine._charIndex = 0;
	}

	if (_UNDO) {
		(void)parseWord(_inputLine);
		if (_vocabId) {
			const VocabEntry &ve = (*g_engine->_vocab)[_vocabId];
			if (ve._flags & VFLAG_1) {
				if (_vocabId == _UNDO || ve._altVocabId == _UNDO) {
					if (!performUndo())
						_val5 = 0;
					return;
				}
			}
		}

		_inputLine._charIndex = 0;
	}

	parseLoop();
}

void Parser::parseLoop() {
	for (;;) {
		if (_AGAIN) {
			(void)parseWord(_inputLine);

			if (_vocabId) {
				const VocabEntry &ve = (*g_engine->_vocab)[_vocabId];

				if (!(ve._flags & VFLAG_1)) {
					_inputLine._charIndex = 0;
				} else if (_vocabId != _AGAIN && ve._altVocabId != _AGAIN) {
					_inputLine._charIndex = 0;
					_val8 = 0;
					_val9 = 0;
				} else if (proc3() <= 0) {
					_val5 = 0;
					return;
				} else {
					_val4 = 1;
					_val8 = 0;
					_val9 = 0;
				}
			}
		}

		if (proc4() <= 0) {
			_val11 = 0;
			_val5 = 0;
			return;
		}

		if (_handlers._minVocabId != 0) {
			int handlerIndex = _handlers.getIndex();
			if (handlerIndex >= 0) {
				HandlerFunction fn = _handlers._functions[handlerIndex]._fn;
				if ((*fn)() != 0)
					return;
			} else if (_handlers._field34 >= 2 || !_handlers._newVocabId) {
				pleaseRephrase();
				return;
			} else {
				const VocabEntry &ve = (*g_engine->_vocab)[_handlers._newVocabId];
				if (!(ve._flags & (VFLAG_2 | VFLAG_4))) {
					pleaseRephrase();
					return;
				}


			}
		} else {

		}


		// TODO
	}
}

#define GET_NUMBER \
	while (Common::isDigit(c)) { \
		if (_wordIndex < 127) { \
			++_wordIndex; \
			_word += c; \
		} \
		\
		_number = _number * 10 + (c - '\0'); \
		c = line[line._charIndex++]; \
	}
#define SET_RESULT(VAL) \
	if (_startIndex != firstIndex) \
		_val1 = _result; \
	\
	_result = VAL; \
	_startIndex = firstIndex
#define CHECK_VOCAB(ID) (ID != 0 && (_vocabId == ID || ve._altVocabId == ID))

int Parser::parseWord(ParserString &line) {
	if (line._charIndex <= _startIndex)
		_val1 = -1;

	for (;;) {
		_vocabId = 0;
		_wordIndex = 0;
		_word.clear();

		// Skip over any whitespaces
		while (Common::isSpace(line[line._charIndex]))
			++line._charIndex;

		int firstIndex = line._charIndex++;
		char c = line[firstIndex];
		if (!c) {
			_val1 = _result;
			_result = PR_END_OF_STRING;
			_startIndex = firstIndex;
			break;
		}

		if (Common::isAlpha(c) && c == '\'') {
			while (c != '\0' && strchr(" \t.,;!?\"", c) == nullptr) {
				if (_wordIndex < 127) {
					++_wordIndex;
					_word += c;
				}
			}
			line._charIndex--;

		} else if (Common::isDigit(c)) {
			_number = 0;
			GET_NUMBER;

			if (c == ':') {
				uint32 num1 = _number * 60;
				_number = 0;
				c = line[line._charIndex++];
				GET_NUMBER;

				_number += num1;
				_vocabId = 5;
			} else {
				_vocabId = 3;
			}

			--line._charIndex;

		} else if (c == '"') {
			_quotedString = "";

			do {
				c = line[line._charIndex++];
				if (_wordIndex < 127) {
					_word += c;
					++_wordIndex;
				}

				if (c != '"' && _quotedString.size() < 127)
					_quotedString += c;
			} while (c != '\0' && c != '"');

			_vocabId = 4;
			_startIndex = firstIndex;

		} else {
			if (_wordIndex < 127) {
				_word += c;
				++_wordIndex;
			}
		}

		if (_word == ",") {
			SET_RESULT(PR_COMMA);
			break;

		} else if (_word == "." || _word == "!") {
			SET_RESULT(PR_PERIOD);
			break;

		} else if (_word == ";") {
			SET_RESULT(PR_SEMICOLON);
			break;
		}

		if (!_vocabId)
			_vocabId = g_engine->_vocab->indexOf(_word);

		if (!_vocabId) {
			_unknownWord = _word.c_str();
			_unknownFirstIndex = firstIndex;
			_unknownWordIndex = _wordIndex;

			_startIndex = firstIndex;
			_val1 = _result;
			_result = PR_UNKOWN;
			break;
		}

		const VocabEntry &ve = (*g_engine->_vocab)[_vocabId];
		if (CHECK_VOCAB(_ALL)) {
			SET_RESULT(PR_15);
			break;
		} else if (CHECK_VOCAB(_AND)) {
			SET_RESULT(PR_13);
			break;
		} else if (CHECK_VOCAB(_BUT)) {
			SET_RESULT(PR_16);
			break;
		} else if (CHECK_VOCAB(_THEN)) {
			SET_RESULT(PR_17);
			break;
		} else if (CHECK_VOCAB(_TO)) {
			SET_RESULT(PR_14);
			break;
		} else if (_OF != 0 && _vocabId == _OF) {
			if (_result == PR_12) {
				if (_val1 == PR_END_OF_STRING || _val1 == PR_PERIOD ||
						_val1 == PR_COMMA || _val1 == PR_SEMICOLON ||
						_val1 == PR_17 || _val1 == PR_13 || _val1 == PR_14) {
					SET_RESULT(PR_3);
					break;
				} else {
					continue;
				}
			} else if (_result == PR_3) {
				continue;
			} else if (_result == PR_2 || _result == PR_5) {
				SET_RESULT(PR_3);
				break;
			} else {
				SET_RESULT(PR_4);
				break;
			}
		} else if (ve._flags & VFLAG_1) {
			if ((ve._flags & VFLAG_20) && _result == PR_END_OF_STRING) {
				_result = PR_2;
				_startIndex = firstIndex;
				break;
			} else if (ve._flags & VFLAG_8) {
				if (_result == PR_12) {
					if (_val1 == -1) {
						SET_RESULT(PR_12);
						break;
					} else if (_val1 == PR_PERIOD) {
						SET_RESULT(PR_PERIOD);
						break;
					} else {
						if (_val1 == PR_COMMA || _val1 == PR_SEMICOLON ||
								_val1 == PR_17 || _val1 == PR_13 || _val1 == PR_14) {
							SET_RESULT(PR_12);
							break;
						} else {
							continue;
						}
					}
				} else if (_result == PR_3) {
					SET_RESULT(PR_12);
					break;
				} else {
					continue;
				}
			} else if (ve._flags & VFLAG_4) {
				int result = (ve._flags & VFLAG_2) ? PR_10 : PR_9;
				SET_RESULT(result);
				break;
			} else {
				if (ve._flags & VFLAG_2)
					_val1 = _result;
				_result = PR_2;
				_startIndex = firstIndex;
				break;
			}
		} else if (ve._flags & VFLAG_4) {
			int result = (ve._flags & VFLAG_2) ? PR_11 : PR_4;
			SET_RESULT(result);
			break;
		} else if (ve._flags & VFLAG_2) {
			SET_RESULT(PR_5);
			break;
		} else if (ve._flags & VFLAG_8) {
			if (_result == PR_12) {
				if (_val1 == PR_END_OF_STRING || _val1 == PR_PERIOD ||
					_val1 == PR_COMMA || _val1 == PR_SEMICOLON ||
					_val1 == PR_17 || _val1 == PR_13 || _val1 == PR_14) {
					SET_RESULT(PR_3);
					break;
				} else {
					continue;
				}
			} else if (_result == PR_3) {
				continue;
			} else {
				SET_RESULT(PR_3);
				break;
			}
		} else if (ve._flags & VFLAG_10) {
			SET_RESULT(VFLAG_10);
			break;
		} else if (_word.size() == 1) {
			_result = _word[0];
			break;
		} else {
			SET_RESULT(PR_21);
			break;
		}
	}

	return _result;
}

#undef GET_NUMBER
#undef SET_RESULT
#undef CHECK_VOCAB

void Parser::addLine(const Common::U32String &line) {
	g_engine->send(TextMessage(line));
}

int Parser::oops() {
	String *inputLinePtr = &_inputLine;
	int unknownWordIndex = _unknownFirstIndex;

	if (!_unknownWordIndex) {
		addLine(_("[There was no misspelled word to replace.]\n"));
		return 0;
	}

	int wordResult = parseWord(_inputLine);

	if (wordResult >= PR_21) {
		if (wordResult <= PR_COMMA) {
			addLine(_("[You can't use that word with \"oops.\"]\n"));
			return -1;
		} else if (wordResult == PR_END_OF_STRING) {
			addLine(_("[Please specify a word to replace the unknown one.]\n"));
			return -1;
		}
	}

	int vocabId = _vocabId;
	int wordIndex = _wordIndex;
	String tempWord = _word;

	if (vocabId) {
		wordResult = parseWord(_inputLine);
		if (wordResult == PR_PERIOD)
			wordResult = parseWord(_inputLine);
		if (wordResult == PR_END_OF_STRING)
			addLine(_("[Everything except the first word after \"oops\" is ignored.]\n"));
	}

	_inputLine = _inputLineCopy;
	int index1 = wordIndex - (unknownWordIndex >> 8);
	int index2 = (unknownWordIndex & 0xff) + (unknownWordIndex >> 8);
	int lineLen = _inputLine.size();

	if (index1) {
		int tmp = index2;
		lineLen = index2;
		if (lineLen >= 0x100) {
			tmp -= lineLen - 0xff;
			lineLen = 0xff;
		}

		if (tmp) {
			// TODO: Double-check. I think this inserts space for the
			// replacement word at the tiven index
			for (int i = 0; i < tmp; ++i)
				inputLinePtr->insertChar(' ', index2);
		}
	}

	// Insert in the replacement word
	for (uint i = 0; i < tempWord.size(); ++i)
		inputLinePtr->setChar(tempWord[i], unknownWordIndex + i);

	if (vocabId) {
		_inputLine._charIndex = 0;
		return 1;
	}

	_inputLineCopy = _inputLine;
	_unknownWord = _inputLine.c_str();
	_unknownFirstIndex = unknownWordIndex;
	_unknownWordIndex = _wordIndex;

	youDontNeed(_word);
	return -1;
}

int Parser::proc3() {
	int charIndex = _inputLine._charIndex;
	int wordResult = parseWord(_inputLine);

	if (wordResult == PR_PERIOD || _vocabId == _THEN) {
		charIndex = _inputLine._charIndex;
		wordResult = parseWord(_inputLine);
	}

	if (wordResult == PR_END_OF_STRING) {
		_subLine.clear();
	} else {
		_subLine = String(_inputLine.c_str() + charIndex);
	}

	_val10 = 0;

	if (!_val11) {
		addLine(_("[You can't use \"again\" after that.]\n"));
		return -1;
	} else {
		if (_val8 || _val9) {
			_val8 = 0;
			_val9 = 0;
			_val5 = 0;
		}

		_inputLine = _inputLineCopy;
		_inputLine._charIndex = 0;
		return 1;
	}
}

int Parser::proc4() {
	// TODO
	return 0;
}

void Parser::youDontNeed(const String &word) {
	String msg(0xc401);
	addLine(String::format(msg.c_str(), word.c_str()));
}

bool Parser::performUndo() {
	if (_val6 && _val7) {
		if (g_engine->loadGame(LOAD_UNDO))
			return true;
	}

	addLine(
		_val7 ? _("[You can't use \"undo\" after that.]\n") :
		_("[There isn't enough memory to use \"undo\" right now.]\n")
	);
	return false;
}

} // namespace Parser
} // namespace Early
} // namespace Legend
