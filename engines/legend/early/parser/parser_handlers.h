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

class Parser;
typedef int (Parser::*HandlerFunction)();

struct ParserHandlerEntry {
	HandlerFunction _fn = nullptr;
	int _minId = 0;
	int _maxId = 0;
	int _val3 = 0;
	ParserHandlerEntry() {}
	ParserHandlerEntry(int maxId, int val3, HandlerFunction fn) :
		_minId(0), _maxId(maxId), _val3(val3), _fn(fn) {}
};

struct HandlerData2 {
	int _val1;
	int _val2;
	int _val3;
	uint16 _vocabIds[16];
	int _val4;

	HandlerData2() {
		clear();
	}
	void clear();
};

struct HandlerDataSub {
	int _val1 = 0;
	HandlerData2 *_ptr = nullptr;
	int _val2 = 0;
	int _val3 = 0;
	int _val4 = 0;

	void clear();
};

struct HandlerDataSub2 {
	HandlerDataSub _sub;
	int _val5 = 0;
	int _val6 = 0;
	int _val7 = 0;
	int _val8 = 0;
	int _val9 = 0;
	int _val10 = 0;

	HandlerDataSub2() { clear(); }
	void clear();
};

struct ParserHandlerArrEntry1 {
	int _vocabId1;
	int _vocabId2;
	int _val3;
	int _val4;
	byte _array[4];
};

struct ParserHandlerArrEntry2 {
	int _vocabId;
	byte _arr[4];
};

/**
 * Special parser handling for character instructions,
 * like "ask", "order", "tell", etc.
 */
struct ParserHandlers {
	Common::Array<ParserHandlerEntry> _functions;

	int _minVocabId;
	int _maxVocabId;
	HandlerDataSub2 _sub1;
	int _field2c;
	int _newVocabId1;
	int _newVocabId2;
	int _field32;
	int _field34;
	int _field35;
	int _field36;
	int _field37;
	int _field38;
	int _field39;
//	byte _arr[4];
	HandlerDataSub _sub2;
	HandlerDataSub _sub3;
	int _field52;
	HandlerData2 _data1;

	const ParserHandlerArrEntry1 *_array1;
	int _array1_size = 0;
	const ParserHandlerArrEntry2 *_array2;
	int _array2_offset1, _array2_size1;

	int _arrIndex = 0;
	int _val2 = 0;

	ParserHandlers() { clear(); }
	void clear();

	void add(const ParserHandlerEntry &entry) {
		_functions.push_back(entry);
	}

	void setArray(const ParserHandlerArrEntry1 *arr, size_t size) {
		_array1 = arr;
		_array1_size = size;
	}

	/**
	 * Return handler function to use
	 */
	int getIndex();

	/**
	 * Compare handler state to a given handler entry
	 */
	int compare(const ParserHandlerEntry &entry) const;

	int arrayGetIndex();
	int array1Compare(const ParserHandlerArrEntry1 *entry) const;
	int array2Compare(const ParserHandlerArrEntry2 *entry) const;
};

} // namespace Parser
} // namespace Early
} // namespace Legend

#endif
