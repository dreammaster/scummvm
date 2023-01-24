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

namespace Legend {
namespace Early {
namespace Gateway {
namespace Parser {

#define PARSER_AGAIN 27
#define PARSER_ALL 41
#define PARSER_AND 50
#define PARSER_BUT 273
#define PARSER_OF 1521
#define PARSER_THEN 2346
#define PARSER_TO 2382
#define PARSER_UNDO 2473

Parser::Parser() : Legend::Early::Parser::Parser(
		String(_("[I beg your pardon?]\n")),
		PARSER_AGAIN, PARSER_ALL, PARSER_AND, PARSER_BUT,
		PARSER_OF, PARSER_THEN, PARSER_TO, PARSER_UNDO) {
}

} // namespace Parser
} // namespace Gateway
} // namespace Early
} // namespace Legend
