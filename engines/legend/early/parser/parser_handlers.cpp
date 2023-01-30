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

void HandlerDataSub2::clear() {
	_sub.clear();
	_val5 = 0;
	_val6 = 0;
	_val7 = 0;
	_val8 = 0;
	_val9 = 0;
	_val10 = 0;
}

void HandlerData2::clear() {
	_prefixVocabId = 0;
	_val2 = 0;
	_vocabCount = 0;
	Common::fill(&_vocabIds[0], &_vocabIds[16], 0);
	_suffixVocabId = 0;
}

void HandlerData2::display() const {
	if (!_suffixVocabId)
		g_engine->_vocab->display(_prefixVocabId);

	for (int idx = 0; idx < _vocabCount; ++idx) {
		g_engine->_vocab->display(_vocabIds[idx]);
		g_engine->addText(" ");
	}

	g_engine->_vocab->display(_suffixVocabId);
}

void ParserHandlers::clear() {
	_minVocabId = 0;
	_maxVocabId = 0;
	_sub1.clear();
	_field2c = 0;
	_newVocabId1 = 0;
	_newVocabId2 = 0;
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

int ParserHandlers::arrayGetIndex() {
	_arrIndex = -1;
	_val2 = -1;

	int limit = _array1_size;
	int offset = 0;
	int compResult = 0;
	int middle = 0;
	int idx1;
	int val5 = 0;

	for (idx1 = limit; (idx1 - offset) >= 8; ) {
		middle = (offset + idx1) / 2;
		const ParserHandlerArrEntry1 &ae = _array1[middle];

		compResult = array1Compare(&ae);
		if (compResult < 0) {
			idx1 = middle;
		} else if (compResult > 0) {
			offset = middle;
		} else {
			offset = middle;
		}
	}

	compResult = array1Compare(&_array1[offset]);

	if (compResult == 0) {
		middle = offset; 
		if (middle) {
			while (!compResult) {
				--middle;
				compResult = array1Compare(&_array1[middle]);
			}

			++middle;
			compResult = 0;
		}
	} else if (compResult > 0) {
		for (middle = offset + 1; middle < idx1; ++middle) {
			compResult = array1Compare(&_array1[middle]);
			if (compResult <= 0)
				break;
		}
	}

	if (compResult)
		return 0;
	
	const ParserHandlerArrEntry1 *entry = &_array1[_arrIndex];
	for (_arrIndex = middle; _arrIndex < _array1_size && !array1Compare(entry);
			++_arrIndex, ++entry) {
		_field35 = entry->_val3;
		_field37 = entry->_val4;

		int val1 = _field34;
		if (val1 >= 2 || _field32) {
			int count = entry->_array[3];
			if (count) {
				offset = entry->_array[2];
				const ParserHandlerArrEntry2 *aePtr = nullptr;

				for (int idx = 0; idx < count; ++idx) {
					const ParserHandlerArrEntry2 *ae = &_array2[offset + idx];
					if (array2Compare(ae)) {
						_val2 = offset + idx;
						aePtr = ae;
						break;
					}
				}

				if (aePtr) {
					for (; (_val2 - offset) < count; ++_val2, ++aePtr) {
						if (array2Compare(aePtr))
							break;
						
						_field36 = aePtr->_arr[0];
						_field38 = aePtr->_arr[1];
						val5 = aePtr->_arr[1 + val1];

						if (val5) {
							_field52 = val5;
							return 1;
						} else if (val1 >= 2) {
							if (aePtr->_arr[3]) {
								_field52 = aePtr->_arr[3];
								return -1;
							}
						} else {
							for (int idx = 0; idx < _array2_size1; ++idx) {
								const ParserHandlerArrEntry2 *ae2 = &_array2[_array2_offset1 + idx];
								if (ae2->_vocabId == _val2) {
									_arrIndex = ae2->_arr[0];
									_val2 = ae2->_arr[2];

									const ParserHandlerArrEntry1 *ae1 = &_array1[_arrIndex];
									_field35 = ae1->_val3;
									_field36 = _array2[_val2]._arr[0];
									_field37 = ae1->_val4;
									_field38 = _array2[_val2]._arr[1];

									SWAP(_sub2, _sub3);

									_field52 = _array2[_val2]._arr[3];
									return 1;
								}
							}
						}
					}
				}
			}
		} else {
			val5 = entry->_array[val1];

			if (val5) {
				_field52 = val5;
				return 1;	
			}

			if (val1) {
				int count = entry->_array[3];
				if (count) {
					for (_val2 = entry->_array[2]; _val2 < count; ++_val2) {
						const ParserHandlerArrEntry2 * aePtr = &_array2[_val2];
						if (aePtr->_arr[3]) {
							_field36 = aePtr->_arr[0];
							_field38 = aePtr->_arr[1];
							_field52 = aePtr->_arr[3];
							return -1;
						}
					}
				}
			} else {
				if (entry->_array[1]) {
					_field52 = entry->_array[1];
					return 1;
				}
			}
		}
	}

	_arrIndex = -1;
	_val2 = -1;
	return 0;
}

int ParserHandlers::array1Compare(const ParserHandlerArrEntry1 *entry) const {
	int vocabId1 = _newVocabId1;
	if (vocabId1) {
		const VocabEntry &ve = (*g_engine->_vocab)[vocabId1];
		int logicNum = ve._logicNum;
		if ((logicNum & 1) && ve._altVocabId != 0)
			vocabId1 = ve._altVocabId;
	}

	int vocabId2 = _newVocabId2;
	if (vocabId2) {
		const VocabEntry &ve = (*g_engine->_vocab)[vocabId2];
		int logicNum = ve._logicNum;
		if ((logicNum & 1) && ve._altVocabId != 0)
			vocabId2 = ve._altVocabId;
	}

	if (entry->_vocabId1 > vocabId1) {
		return -1;
	} else if (entry->_vocabId1 < vocabId1) {
		return 1;
	} else if (entry->_vocabId2 > vocabId2) {
		return -1;
	} else if (entry->_vocabId2 < vocabId2) {
		return 1;
	} else {
		return 0;
	}
}

int ParserHandlers::array2Compare(const ParserHandlerArrEntry2 *entry) const {
	int vocabId = _field32;
	if (vocabId) {
		const VocabEntry &ve = (*g_engine->_vocab)[vocabId];

		if ((ve._logicNum & 8) && ve._altVocabId != 0)
			vocabId = ve._altVocabId;
	}

	if (entry->_vocabId > vocabId)
		return -1;
	else if (entry->_vocabId < vocabId)
		return 1;
	else
		return 0;
}

} // namespace Parser
} // namespace Early
} // namespace Legend
