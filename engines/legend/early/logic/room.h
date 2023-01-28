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

#ifndef LEGEND_EARLY_LOGIC_ROOM_H
#define LEGEND_EARLY_LOGIC_ROOM_H

#include "legend/early/logic/logic_base.h"

namespace Legend {
namespace Early {

enum RoomAction {
	ROOMACTION_GET_INPUT = 28
};

struct Room : public LogicBase {
	Room() : LogicBase(LT_ROOM) {}

	/**
	 * Synchronize the data fields of the logic
	 */
	void synchronize(Common::Serializer &s) override;

	/**
	 * Method stubs
	 */
	virtual int action01() { return 0; }
	virtual int action02() { return 0; }
	virtual int action03() { return 0; }
	virtual int action04() { return 0; }
	virtual int action05() { return 0; }
	virtual int action06() { return 0; }
	virtual int action07() { return 0; }
	virtual int action08() { return 0; }
	virtual int action09() { return 0; }
	virtual int action10() { return 0; }
	virtual int action11() { return 0; }
	virtual int action12() { return 0; }
	virtual int action13() { return 0; }
	virtual int action14() { return 0; }
	virtual int action15() { return 0; }
	virtual int action16() { return 0; }
	virtual int action17() { return 0; }
	virtual int action18() { return 0; }
	virtual int action19() { return 0; }
	virtual int action20() { return 0; }
	virtual int action21() { return 0; }
	virtual int action22() { return 0; }
	virtual int action23() { return 0; }

	/**
	 * Perform an action
	 */
	int call(int action) override;
};

} // namespace Early
} // namespace Legend

#endif
