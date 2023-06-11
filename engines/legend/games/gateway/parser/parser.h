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

#ifndef LEGEND_GAMES_GATEWAY_PARSER_PARSER_H
#define LEGEND_GAMES_GATEWAY_PARSER_PARSER_H

#include "legend/early/parser/parser.h"

namespace Legend {
namespace Early {
namespace Gateway {
namespace Parser {

class Parser : public Legend::Early::Parser::Parser {
protected:
	/**
	 * Asks the user to rephrase the question
	 */
	void pleaseRephrase() override;

	/**
	 * What do you want to do method
	 */
	virtual void whatDoYouWant() override;

	int getAltVocab(int vocabId1, int vocabId2) const override;

public:
	Parser();
	virtual ~Parser() {}
};

} // namespace Parser
} // namespace Gateway
} // namespace Early
} // namespace Legend

#endif
