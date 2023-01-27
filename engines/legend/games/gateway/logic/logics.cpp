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

#include "common/textconsole.h"
#include "legend/games/gateway/logic/logics.h"
#include "legend/games/gateway/engine.h"

namespace Legend {
namespace Early {
namespace Gateway {
namespace Logic {

#define INIT10(PREFIX) \
	&_logic##PREFIX##0, &_logic##PREFIX##1, &_logic##PREFIX##2, &_logic##PREFIX##3, &_logic##PREFIX##4, &_logic##PREFIX##5, \
	&_logic##PREFIX##6, &_logic##PREFIX##7, &_logic##PREFIX##8, &_logic##PREFIX##9
#define INIT100(PREFIX) \
	INIT10(PREFIX##0), INIT10(PREFIX##1), INIT10(PREFIX##2), \
	INIT10(PREFIX##3), INIT10(PREFIX##4), INIT10(PREFIX##5), \
	INIT10(PREFIX##6), INIT10(PREFIX##7), INIT10(PREFIX##8), \
	INIT10(PREFIX##9)

#define INIT001 \
	&_logic001, &_logic002, &_logic003, &_logic004, &_logic005, \
	&_logic006, &_logic007, &_logic008, &_logic009
#define INIT001_100 \
	INIT001, INIT10(01), INIT10(02), INIT10(03), INIT10(04), INIT10(05), \
	INIT10(06), INIT10(07), INIT10(08), INIT10(09)

Logics::Logics() : _logics{
	INIT001_100, INIT100(1), INIT100(2), INIT100(3), INIT100(4),
	INIT100(5), INIT100(6),
	INIT10(70), INIT10(71), INIT10(72),
	&_logic730, &_logic731, &_logic732, &_logic733, &_logic734
} {
}

LogicBase *Logics::operator[](int index) const {
	if (index < 1 || index > LOGICS_COUNT)
		error("Invalid logic specified");

	// Call the appropriate method
	return _logics[index];
}

String Logics::printObj(int action, int logicNum) {
	String article;
	bool capitalize = false;
	int actionMask = action & 7;
	String desc;

	if (logicNum < 1 || logicNum > size())
		return nullptr;

	String str1;
	String str = getString1(logicNum);

	if (actionMask) {
		if (action & 16) {
			capitalize = true;
		} else {
			desc += ' ';
			capitalize = false;
		}

		switch (actionMask) {
		case 2:
			if (!getBit(logicNum, 4))
				article = "the";
			break;
		case 3:
			if (!getBit(logicNum, 4)) {
				if (getBit(logicNum, 6) || getBit(logicNum, 1))
					article = "some";
				else if (getBit(logicNum, 5))
					article = "an";
				else
					article = "a";
			}
			break;
		case 4:
			if (logicNum == g_engine->_parser->_val2 || logicNum == g_engine->_logicNum1) {
				article = "you";
			} else if (getBit(logicNum, 6) && !getBit(logicNum, 1)) {
				article = "they";
			} else {
				const LogicBase *lb = (*this)[logicNum];
				if (lb->_type == LT_7) {
					article = getBit(logicNum, 40) ? "she" : "he";
				} else {
					article = "it";
				}
			}
			break;
		case 5:
			if (logicNum == g_engine->_parser->_val2 || logicNum == g_engine->_logicNum1) {
				article = "your";
			} else if (getBit(logicNum, 6) && !getBit(logicNum, 1)) {
				article = "their";
			} else {
				const LogicBase *lb = (*this)[logicNum];

				if (lb->_type == LT_7) {
					article = getBit(logicNum, 40) ? "her" : "his";
				} else {
					article = "its";
				}
			}
			break;
		case 6:
			if (logicNum == g_engine->_parser->_val2 || logicNum == g_engine->_logicNum1) {
				article = "you";
			} else if (getBit(logicNum, 6) && !getBit(logicNum, 1)) {
				article = "them";
			} else {
				const LogicBase *lb = (*this)[logicNum];

				if (lb->_type == LT_7) {
					article = getBit(logicNum, 40) ? "her" : "his";
				} else {
					article = "its";
				}
			}
			break;
		case 7:
			if (logicNum == g_engine->_parser->_val2) {
				article = "yourself";
			} else if (getBit(logicNum, 6) && !getBit(logicNum, 1)) {
				article = "themselves";
			} else {
				const LogicBase *lb = (*this)[logicNum];

				if (lb->_type == LT_7) {
					article = getBit(logicNum, 40) ? "herself" : "himself";
				} else {
					article = "itself";
				}
			}
		default:
			break;
		}

		if (!article.empty()) {
			if (capitalize)
				article.capitalize();
			desc += article;
			desc += ' ';
		}

		if (actionMask >= 1 && actionMask <= 3) {
			if (!str.empty()) {
				if (capitalize)
					str.capitalize();
				desc += str;

			}
		}
	}

	if (action & 0x40) {

	}


	// TODO
	return desc;
}

} // namespace Logic
} // namespace Gateway
} // namespace Early
} // namespace Legend
