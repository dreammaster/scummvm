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
	USE = 2495
};

Parser::Parser() : Legend::Early::Parser::Parser(
		String(_("[I beg your pardon?]\n")),
		PARSER_AGAIN, PARSER_ALL, PARSER_AND, PARSER_BUT, PARSER_OF,
		PARSER_OOPS, PARSER_THEN, PARSER_TO, PARSER_UNDO) {
	_handlers.add(Early::Parser::ParserHandlerEntry(ASK, 0, 12, speakHandler));
	_handlers.add(Early::Parser::ParserHandlerEntry(ORDER, 0, 12, speakHandler));
	_handlers.add(Early::Parser::ParserHandlerEntry(TELL, 0, 12, speakHandler));
	_handlers.add(Early::Parser::ParserHandlerEntry(USE, 0, 15, zeroHandler));
}

int Parser::speakHandler() {
	// TODO: Gateway default handler
	return 0;
}

void Parser::pleaseRephrase() {
	g_engine->send(TextMessage(String(0xc402)));
}

} // namespace Parser
} // namespace Gateway
} // namespace Early
} // namespace Legend
