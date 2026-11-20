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

#ifndef ULTIMA5_LOGIC_USE_LOGIC_H
#define ULTIMA5_LOGIC_USE_LOGIC_H

#include "ultima/ultima5/views/message_log.h"

namespace Ultima {
namespace Ultima5 {

/**
 * What using the things that aren't weapons and armour does: reading scrolls, drinking potions, burying
 * moonstones, wearing the amulet and the rest. Some need a direction or a member of the
 * party to be picked, which are asked for and given when they've been chosen
 */
class UseLogic {
public:
	enum Need {
		NEED_NONE,
		NEED_DIRECTION,
		NEED_MEMBER
	};

private:
	// What's to be done with what's asked for
	enum Stage {
		STAGE_WIND,
		STAGE_SKULL_KEY,
		STAGE_POTION,
		STAGE_RESURRECTION
	};

	Views::MessageLog &_log;
	Stage _stage = STAGE_WIND;
	int _potion = 0;

	// Whether it worked: 1 does, 0 doesn't and has "Failed!" said, and -1 is neither
	int _result = 1;

	Need scroll(int which);
	Need potion(int which);
	void moonstone(int which);
	Need other(int item);
	void gemShard(int which);
	void sextant();
	void watch();
	void sceptre();
	void carpet();
	void worn(int icon, const char *intro, const char *line);
	bool removeWorn(int icon);
	void setIcon(int icon, int duration);
	Need finish();
	void drinkPotion(int which, int member);
	void promptDirection();
	void promptMember();

public:
	UseLogic(Views::MessageLog &log) : _log(log) {}

	/**
	 * Uses one of the things in the list of items
	 */
	Need use(int item);

	/**
	 * Gives the direction that was asked for, as the arrow keys are numbered by the original
	 * game, or zero if it was passed
	 */
	Need directionChosen(int key);

	/**
	 * Gives the member of the party that was asked for, or -1 for none
	 */
	Need memberChosen(int member);
};

} // namespace Ultima5
} // namespace Ultima

#endif
