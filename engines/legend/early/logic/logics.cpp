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
		LogicType(2, 2, 24)
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

void Logics::setPrehandlerMode(int logicNum, int handlerIndex, uint newId) {
	if (logicNum != 0 && logicNum <= size()) {
		LogicBase *lb = (*this)[logicNum];
		if (lb->_type != LT_6) {
			setPrehandler(logicNum, newId);
		} else if (handlerIndex >= 0 && handlerIndex < _METADATA[6 - 1]._handlerCount) {
			lb->_prehandlerId[handlerIndex] = newId;
		}
	}
}

void Logics::proc1(int logicNum, int v2, int v3) {
	
	// TODO
}

} // namespace Early
} // namespace Legend
