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

namespace Legend {
namespace Early {

class Logics {
public:
	/**
	 * Array indexing operator
	 */
	virtual LogicBase *operator[](int index) const = 0;

	/**
	 * Size of the logic array
	 */
	virtual size_t size() const = 0;

	/**
	 * Synchronize logic data
	 */
	void synchronize(Common::Serializer &s);

	int getVal1(int logicNum) const {
		return (*this)[logicNum]->_val1;
	}
	void setVal1(int logicNum, int val1) {
		(*this)[logicNum]->_val1 = val1;
	}
	int getVal2(int logicNum) const {
		return (*this)[logicNum]->_val2;
	}
	void setVal2(int logicNum, int val2) {
		(*this)[logicNum]->_val1 = val2;
	}
	int getPrehandlerId(int logicNum) const {
		return (*this)[logicNum]->_prehandlerId;
	}
	void setPrehandlerId(int logicNum, int id) {
		(*this)[logicNum]->_prehandlerId = id;
	}

	void proc1(int logicNum, int v2, int v3);
};

} // namespace Early
} // namespace Legend

#endif
