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

#ifndef LEGEND_EARLY_PARSER_PARSER_HANDLERS_H
#define LEGEND_EARLY_PARSER_PARSER_HANDLERS_H

#include "legend/utils/string.h"

namespace Legend {
namespace Early {
namespace Parser {

typedef int (*HandlerFunction)();

struct ParserHandlerEntry {
	int _minId = 0;
	int _maxId = 0;
	int _val3 = 0;
	HandlerFunction _fn = nullptr;
	ParserHandlerEntry() {}
	ParserHandlerEntry(int minId, int maxId, int val3, HandlerFunction fn) :
		_minId(minId), _maxId(maxId), _val3(val3), _fn(fn) {}
};

struct HandlerDataSub {
	int _val1 = 0;
	void *_ptr = 0;
	int _val2 = 0;
	int _val3 = 0;
	int _val4 = 0;

	void clear();
};

/**
 * Special parser handling for character instructions,
 * like "ask", "order", "tell", etc.
 */
struct ParserHandlers {
	Common::Array<ParserHandlerEntry> _functions;

	int _minVocabId;
	int _maxVocabId;
	HandlerDataSub _sub1;
	int _field2c;
	int _newVocabId;
	int _field30;
	int _field32;
	int _field34;
	HandlerDataSub _sub2;
	HandlerDataSub _sub3;
	int _field52;

	ParserHandlers() { clear(); }
	void clear();

	void add(const ParserHandlerEntry &entry) {
		_functions.push_back(entry);
	}

	/**
	 * Return handler function to use
	 */
	int getIndex();

	/**
	 * Compare handler state to a given handler entry
	 */
	int compare(const ParserHandlerEntry &entry) const;
};

} // namespace Parser
} // namespace Early
} // namespace Legend

#endif
