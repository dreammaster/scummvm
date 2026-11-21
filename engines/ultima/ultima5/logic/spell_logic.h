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

#ifndef ULTIMA5_LOGIC_SPELL_LOGIC_H
#define ULTIMA5_LOGIC_SPELL_LOGIC_H

#include "ultima/ultima5/data/savegame.h"
#include "ultima/ultima5/views/message_log.h"

namespace Ultima {
namespace Ultima5 {

/**
 * Casting the spells of the game that can be cast away from a fight, and mixing them from
 * reagents. Some need a direction or a member of the party to be asked for, and they are
 * given when they've been chosen
 */
class SpellLogic {
public:
	enum Need {
		NEED_NONE,
		NEED_DIRECTION,
		NEED_MEMBER,
		NEED_VIEW,
		NEED_KEY
	};

	// What typing a name of a spell can come to
	static constexpr int SPELL_NONE = -1;
	static constexpr int SPELL_UNKNOWN = -2;

private:
	Views::MessageLog &_log;
	int _caster = 0;
	int _spell = 0;

	// Whether it worked: 1 does and says so, 0 doesn't and says that, and -1 is neither
	int _result = -1;
	bool _passes = true;

	Need finish();
	void promptDirection();
	void promptMember();
	void setIcon(int icon, int duration);
	Need field(int kind);
	Need unlockSpell();
	Need destroyField();
	Need windSpell(int key);
	void showPosition();

public:
	SpellLogic(Views::MessageLog &log) : _log(log) {}

	/**
	 * Works out which spell the letters typed make up, which can be none or not one
	 */
	static int identify(const char *letters, int count);

	/**
	 * Tells if there are enough reagents to take an amount of each that a recipe needs
	 */
	static bool hasReagents(int recipe, int amount);

	/**
	 * Mixes spells from the reagents picked. What was picked has to be what the spell is
	 * made of, or it goes wrong
	 */
	void mixSpell(int spell, int recipe, int amount);

	/**
	 * Casts the spell for a member of the party, if it can be
	 */
	Need cast(int caster, int spell);

	Need directionChosen(int key);
	Need memberChosen(int member);

	/**
	 * Goes on with a spell that asked for a key, which was typed
	 */
	Need keyChosen(int key);

	/**
	 * Returns true if time goes by after the cast
	 */
	bool passes() const {
		return _passes;
	}
};

} // namespace Ultima5
} // namespace Ultima

#endif
