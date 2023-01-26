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

#include "legend/early/logic/logics.h"

namespace Legend {
namespace Early {

Logics::Logics() : _METADATA{
		LogicType(0, 1, 26),
		LogicType(1, 3, 36),
		LogicType(0, 0, 12),
		LogicType(0, 2, 20),
		LogicType(0, 2, 22),
		LogicType(2, 2, 24),
		LogicType(1, 3, 32),
		LogicType(1, 0, 24)
	},
	_ARRAY{
		ArrayEntry(0, 255),
		ArrayEntry(1, 255),
		ArrayEntry(2, 255),
		ArrayEntry(3, 255),
		ArrayEntry(4, 255),
		ArrayEntry(5, 255),
		ArrayEntry(6, 255),
		ArrayEntry(7, 242),
		ArrayEntry(8, 211),
		ArrayEntry(9, 218),
		ArrayEntry(10, 210),
		ArrayEntry(11, 50),
		ArrayEntry(12, 146),
		ArrayEntry(13, 50),
		ArrayEntry(14, 34),
		ArrayEntry(15, 34),
		ArrayEntry(16, 66),
		ArrayEntry(17, 130),
		ArrayEntry(18, 18),
		ArrayEntry(19, 130),
		ArrayEntry(20, 130),
		ArrayEntry(21, 3),
		ArrayEntry(22, 130),
		ArrayEntry(23, 130),
		ArrayEntry(24, 130),
		ArrayEntry(25, 130),
		ArrayEntry(26, 130),
		ArrayEntry(27, 18),
		ArrayEntry(28, 130),
		ArrayEntry(29, 2),
		ArrayEntry(30, 2),
		ArrayEntry(31, 2),
		ArrayEntry(32, 2),
		ArrayEntry(33, 2),
		ArrayEntry(34, 2),
		ArrayEntry(35, 2),
		ArrayEntry(36, 2),
		ArrayEntry(37, 2),
		ArrayEntry(7, 1),
		ArrayEntry(11, 64)
	} {
}

void Logics::synchronize(Common::Serializer &s) {
	size_t count = size();
	for (size_t i = 0; i < count; ++i)
		(*this)[i]->synchronize(s);
}

int Logics::getPrehandlerMode(int logicNum, int handlerIndex) {
	if (logicNum < 1 || logicNum > size()) {
		return 0;
	} else {
		const LogicBase *lb = (*this)[logicNum];
		if (lb->_type != LT_6)
			return getPrehandler(logicNum);

		if (handlerIndex < 0 || handlerIndex >= _METADATA[6 - 1]._handlerCount)
			return 0;

		return lb->_prehandlerId[handlerIndex];
	}
}

void Logics::setPrehandlerMode(int logicNum, int handlerIndex, int newId) {
	if (logicNum >= 1 && logicNum <= size()) {
		LogicBase *lb = (*this)[logicNum];
		if (lb->_type != LT_6) {
			setPrehandler(logicNum, newId);
		} else if (handlerIndex >= 0 && handlerIndex < _METADATA[6 - 1]._handlerCount) {
			lb->_prehandlerId[handlerIndex] = newId;
		}
	}
}

int Logics::getUnkHandler(int logicNum, int handlerIndex) {
	if (logicNum < 1 || logicNum > size()) {
		return 0;
	} else {
		const LogicBase *lb = (*this)[logicNum];
		if (handlerIndex < 0 || handlerIndex >= _METADATA[lb->_type - 1]._unkHandlerCount)
			return 0;

		return lb->_unkHandlerId[handlerIndex];
	}
}

void Logics::setUnkHandler(int logicNum, int handlerIndex, int newId) {
	if (logicNum >= 1 && logicNum <= size()) {
		LogicBase *lb = (*this)[logicNum];

		if (handlerIndex >= 0 && handlerIndex < _METADATA[lb->_type - 1]._unkHandlerCount) {
			lb->_unkHandlerId[handlerIndex] = newId;
		}
	}
}

byte *Logics::getBits(int logicNum) {
	if (logicNum < 1 || logicNum > size()) {
		return nullptr;
	} else {
		LogicBase *lb = (*this)[logicNum];
		if (lb->_type != LT_7)
			return nullptr;

		return &static_cast<LogicType7 *>(lb)->_data1[0];
	}
}

byte *Logics::getBitPtr(int logicNum, int index, int *bitNum) {
	byte *result = nullptr;

	if (logicNum >= 1 && logicNum <= size()) {
		LogicBase *lb = (*this)[logicNum];
		byte *dataPtr = getBits(logicNum);

		if (index > 0 && dataPtr) {
			byte typeBits = _ARRAY[index - 1]._typeBits;
			if ((1 << (lb->_type - 1)) & typeBits) {
				// Is enabled for this type
				byte newIndex = _ARRAY[index - 1]._index;
				*bitNum = newIndex & 7;

				assert((newIndex >> 3) < 4);
				result = dataPtr + (newIndex >> 3);
			}
		}
	}

	return result;
}

int Logics::getBit(int logicNum, int index) {
	int bitNum = 0;
	byte *data = getBitPtr(logicNum, index, &bitNum);

	return data ? (*data >> bitNum) & 1 : 0;
}

const char *Logics::getName(int logicNum) {
	// TODO
	return nullptr;
}

const char *Logics::getString1(int logicNum) {
	// TODO
	return nullptr;
}

void Logics::updateHandler(int logicNum, int handlerId, int handlerIndex) {
	int prehandlerId = getPrehandler(logicNum);
	int newIndex = getVal2(logicNum);
	int unkHandlerId = getUnkHandler(prehandlerId, newIndex);


	if (logicNum == unkHandlerId) {
		setUnkHandler(prehandlerId, newIndex, getVal1(logicNum));
	} else {
		int newId;
		for (; unkHandlerId; unkHandlerId = newId) {
			newId = getVal1(unkHandlerId);
			if (logicNum == newId) {
				setVal1(unkHandlerId, getVal1(logicNum));
				break;
			}
		}
	}

	setPrehandler(logicNum, handlerId);
	setVal2(logicNum, handlerIndex);

	if (handlerId) {
		unkHandlerId = getUnkHandler(handlerId, handlerIndex);
		setUnkHandler(handlerId, handlerIndex, logicNum);
		setVal1(logicNum, unkHandlerId);
	}
}

} // namespace Early
} // namespace Legend
