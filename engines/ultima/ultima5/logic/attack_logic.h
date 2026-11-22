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

#ifndef ULTIMA5_LOGIC_ATTACK_LOGIC_H
#define ULTIMA5_LOGIC_ATTACK_LOGIC_H

#include "ultima/ultima5/data/savegame.h"
#include "ultima/ultima5/views/message_log.h"

namespace Ultima {
namespace Ultima5 {

/**
 * Attacking what's next to the party: the people of the towns, and what's met out
 * in the open, which can lead to a fight
 */
class AttackLogic {
public:
	/**
	 * What an attack came to, if it was a fight
	 */
	struct Fight {
		bool _started = false;
		int _actor = 0;		// The object that is fought
		int _npc = -1;		// The person it is, in a town
		int _kind = 0;		// The kind of enemy
		int _map = 0;		// The map of the fight
		bool _lone = true;	// Whether it is one enemy where they come that way
	};

private:
	Views::MessageLog &_log;

	/**
	 * Finds the object at a place on the level the party is on, or zero
	 */
	static int findObject(int x, int y);

	static void lowerKarma();
	static void callGuard(int npc);
	static void provoke(int npc);

	bool townAttack(int dx, int dy, Fight &fight);
	void outdoorAttack(int dx, int dy, Fight &fight);

public:
	AttackLogic(Views::MessageLog &log) : _log(log) {}

	/**
	 * Finds the person that an object is, or -1
	 */
	static int findNpc(int object);

	/**
	 * Makes the guards, and some of the others, take against the party
	 */
	static void callGuards();

	/**
	 * Has a person turn on the party, hurting it
	 */
	void begone(int npc);

	/**
	 * Takes the party to the cells of the castle, to wake in the morning
	 */
	static void imprison();

	/**
	 * Says why, and gives false, if the party can't attack from where it is
	 */
	bool canAttack();

	/**
	 * Attacks what's next to the party
	 * @returns		True if time goes by
	 */
	bool attack(int dx, int dy, Fight &fight);

	/**
	 * Works out what the fight with an object is, and announces it
	 */
	void prepare(Fight &fight);

	/**
	 * Takes what was fought off the map once the fight is over
	 */
	static void afterFight(const Fight &fight, bool won);
};

} // namespace Ultima5
} // namespace Ultima

#endif
