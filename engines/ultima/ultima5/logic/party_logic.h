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

#ifndef ULTIMA5_LOGIC_PARTY_LOGIC_H
#define ULTIMA5_LOGIC_PARTY_LOGIC_H

#include "ultima/ultima5/views/message_log.h"

namespace Ultima {
namespace Ultima5 {

/**
 * What the party does with its members and its supplies
 */
class PartyLogic {
private:
	Views::MessageLog &_log;

public:
	PartyLogic(Views::MessageLog &log) : _log(log) {}

	/**
	 * Lights a torch, if the party has one
	 */
	void igniteTorch();

	/**
	 * Swaps two members of the party, who can't be the leader
	 */
	void swapMembers(int first, int second);

	/**
	 * Does an amount of damage to a member of the party, who dies if they've none left
	 */
	static void hurtMember(int member, int amount);

	/**
	 * What goes on with the party each turn: poison, hunger, the hour's food, the spell
	 * that wears off and the rings that heal
	 */
	void turnUpkeep();

	/**
	 * Sleepers wake now and then in a town
	 */
	static void wakeSleepers();

	/**
	 * Tells if none of the party can act as those that are alive are all asleep
	 */
	static bool allAsleep();

	/**
	 * Treading on poisonous ground poisons those with the worst footing
	 * @param lowest	The lowest the roll to beat can be
	 */
	void stepOnPoison(int lowest);

	/**
	 * Treading on burning ground hurts everyone
	 */
	void stepOnFire();

	/**
	 * Hurts each of the living members of the party by a few points
	 */
	static void hurtParty();
};

} // namespace Ultima5
} // namespace Ultima

#endif
