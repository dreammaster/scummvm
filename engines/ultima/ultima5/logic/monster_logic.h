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

#ifndef ULTIMA5_LOGIC_MONSTER_LOGIC_H
#define ULTIMA5_LOGIC_MONSTER_LOGIC_H

#include "ultima/ultima5/data/savegame.h"
#include "ultima/ultima5/views/message_log.h"

namespace Ultima {
namespace Ultima5 {

/**
 * The monsters of Britannia and the underworld, which turn up, come after the
 * party and attack it as the turns go by
 */
class MonsterLogic {
public:
	/**
	 * What the monsters did, if it was to come to blows
	 */
	struct Result {
		bool _fight = false;
		int _actor = 0;		// The object that attacked
		int _attacks = 0;	// How many came to blows or shot at the party
	};

private:
	Views::MessageLog &_log;

	// What the monsters remember between turns
	static byte _lastX;
	static byte _lastY;
	static bool _quickToggle;
	static bool _horseToggle;

	static bool isMonster(int tile);
	static int spawnChance();
	static int pickWeighted(const byte *weights);
	static byte monsterFor(int ground);
	static void pickSpot(int &x, int &y);
	static int tileAt(int x, int y);
	static int objectAt(int x, int y);
	static bool canGo(int object, int x, int y);
	static void step(int object, int dx, int dy);
	static void wander(int object);
	static void chase(int object);
	static void move(int object);
	static void remove(int object);

	void spawn();
	int attackCheck(int object, Result &result);
	void attacked(int object, Result &result);
	bool shotReaches(int object, int kind);
	void pirateShot(int object, int dx, int dy);

public:
	MonsterLogic(Views::MessageLog &log) : _log(log) {}

	/**
	 * Carries on with the monsters for a turn
	 */
	Result turn();
};

} // namespace Ultima5
} // namespace Ultima

#endif
