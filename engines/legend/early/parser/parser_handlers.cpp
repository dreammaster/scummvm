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

#include "legend/early/parser/parser_handlers.h"
#include "legend/early/engine.h"

namespace Legend {
namespace Early {
namespace Parser {

void HandlerDataSub::clear() {
	_val1 = 0;
	_ptr = 0;
	_val2 = 0;
	_val3 = 0;
	_val4 = 0;
}

void ParserHandlers::clear() {
	_minVocabId = 0;
	_maxVocabId = 0;
	_sub1.clear();
	_field2c = 0;
	_newVocabId = 0;
	_field30 = 0;
	_field32 = 0;
	_field34 = 0;
	_sub2.clear();
	_sub3.clear();
	_field52 = 0;
}

int ParserHandlers::getIndex() {
	for (uint idx = 0; idx < _functions.size(); ++idx) {
		const ParserHandlerEntry &entry = _functions[idx];
		if (compare(entry) == 0) {
			_field2c = entry._val3;
			return idx;
		}
	}

	return -1;
}

int ParserHandlers::compare(const ParserHandlerEntry &entry) const {
	int minVocabId = _minVocabId;
	if (minVocabId) {
		const VocabEntry &ve = (*g_engine->_vocab)[minVocabId];
		if ((ve._logicNum & 1) && ve._altVocabId)
			minVocabId = ve._altVocabId;
	}

	int maxVocabId = _maxVocabId;
	if (maxVocabId) {
		const VocabEntry &ve = (*g_engine->_vocab)[maxVocabId];
		if ((ve._logicNum & 1) && ve._altVocabId)
			maxVocabId = ve._altVocabId;
	}

	if (minVocabId >= entry._minId) {
		return -1;
	} else if (minVocabId < entry._minId) {
		return 1;
	} else if (entry._maxId > maxVocabId) {
		return -1;
	} else if (entry._maxId < minVocabId) {
		return 1;
	} else {
		return 0;
	}
}

} // namespace Parser
} // namespace Early
} // namespace Legend
