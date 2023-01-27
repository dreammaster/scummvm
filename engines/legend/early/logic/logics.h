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

#ifndef LEGEND_EARLY_LOGIC_LOGICS_H
#define LEGEND_EARLY_LOGIC_LOGICS_H

#include "common/serializer.h"
#include "legend/early/logic/logic_base.h"
#include "legend/early/logic/logic_strings.h"

namespace Legend {
namespace Early {

class Logics {
	struct LogicType {
		const int _handlerCount;
		const int _unkHandlerCount;
		const uint _size;
		LogicType(int handlerCount, int unkHandlerCount, uint size) :
			_handlerCount(handlerCount), _unkHandlerCount(unkHandlerCount),
			_size(size) {}
	};
	struct ArrayEntry {
		byte _index;
		byte _typeBits;
		ArrayEntry(uint16 index, uint16 typeBits) : _index(index), _typeBits(typeBits) {}
	};
private:
	const LogicType _METADATA[8];
	const ArrayEntry _ARRAY[40];	// TODO: Is this Gateway specific?
protected:
	LogicStrings _strings;
public:
	Logics();
	virtual ~Logics() {}

	/**
	 * Array indexing operator
	 */
	virtual LogicBase *operator[](int index) const = 0;

	/**
	 * Size of the logic array
	 */
	virtual int size() const = 0;

	/**
	 * Synchronize logic data
	 */
	void synchronize(Common::Serializer &s);

	int getVal1(int logicNum) const {
		return (logicNum < 1 || logicNum > size()) ? 0 :
			(*this)[logicNum]->_val1;
	}
	void setVal1(int logicNum, int val1) {
		(*this)[logicNum]->_val1 = val1;
	}
	int getVal2(int logicNum) const {
		return (logicNum < 1 || logicNum > size()) ? 0 :
			(*this)[logicNum]->_val2;
	}
	void setVal2(int logicNum, int val2) {
		(*this)[logicNum]->_val2 = val2;
	}
	int getVal3(int logicNum) const {
		return (logicNum < 1 || logicNum > size()) ? 0 :
			(*this)[logicNum]->_val3;
	}
	void setVal3(int logicNum, int val3) {
		(*this)[logicNum]->_val3 = val3;
	}
	int getVal4(int logicNum) const {
		return (logicNum < 1 || logicNum > size()) ? 0 :
			(*this)[logicNum]->_val4;
	}
	void setVal4(int logicNum, int val4) {
		(*this)[logicNum]->_val4 = val4;
	}
	int getPrehandler(int logicNum) const {
		return (logicNum < 1 || logicNum > size()) ? 0 :
			(*this)[logicNum]->_prehandlerId[0];
	}
	void setPrehandler(int logicNum, int id) {
		(*this)[logicNum]->_prehandlerId[0] = id;
	}

	int getPrehandlerMode(int logicNum, int val1) const;
	void setPrehandlerMode(int logicNum, int val1, int newId);
	int getUnkHandler(int logicNum, int handlerIndex) const;
	void setUnkHandler(int logicNum, int handlerIndex, int newId);
	bool isPrehandler1(int logicNum, int val) const;

	byte *getBits(int logicNum);
	byte *getBitPtr(int logicNum, int index, int *bitNum);
	int getBit(int logicNum, int index);
	void setBit(int logicNum, int index);
	void clearBit(int logicNum, int index);

	const char *getName(int logicNum);
	const char *getString1(int logicNum);
	virtual String printObj(int val1, int logicNum) = 0;
	String printObjLower(int logicNum);

	void updateHandler(int logicNum, int handlerId, int handlerIndex);
};

} // namespace Early
} // namespace Legend

#endif
