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

#ifndef LEGEND_EARLY_LOGIC_LOGIC_BASE_H
#define LEGEND_EARLY_LOGIC_LOGIC_BASE_H

#include "common/scummsys.h"

namespace Legend {
namespace Early {

enum LogicType {
	LT_NULL = 0, LT_ROOM = 1, LT_2 = 2, LT_3 = 3, LT_4 = 4,
	LT_5 = 5, LT_6 = 6, LT_7 = 7, LT_8 = 8
};

struct LogicBase {
	LogicType _type;
	LogicBase(LogicType type) : _type(type) {}

	virtual int call(int param = 0) const = 0;
};


struct LogicType2 : public LogicBase {
	LogicType2() : LogicBase(LT_2) {}
};
struct LogicType3 : public LogicBase {
	LogicType3() : LogicBase(LT_3) {}
};
struct LogicType4 : public LogicBase {
	LogicType4() : LogicBase(LT_4) {}
};
struct LogicType5 : public LogicBase {
	LogicType5() : LogicBase(LT_5) {}
};
struct LogicType6 : public LogicBase {
	LogicType6() : LogicBase(LT_6) {}
};
struct LogicType7 : public LogicBase {
	LogicType7() : LogicBase(LT_7) {}
};
struct LogicType8 : public LogicBase {
	LogicType8() : LogicBase(LT_8) {}
};

/**
 * Placeholder for not yet implemented logic
 */
struct LogicNull : public LogicBase {
	LogicNull() : LogicBase(LT_NULL) {}

	int call(int param) const override {
		return 0;
	}
};

} // namespace Early
} // namespace Legend

#endif
