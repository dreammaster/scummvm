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

#ifndef ULTIMA5_LOGIC_NPC_MOVEMENT_H
#define ULTIMA5_LOGIC_NPC_MOVEMENT_H

#include "ultima/ultima5/data/location_map.h"
#include "ultima/ultima5/data/savegame.h"
#include "ultima/ultima5/logic/game_random.h"

namespace Ultima {
namespace Ultima5 {

/**
 * Moves the people of a location around it, each turn, between the places
 * of their schedules
 */
class NpcMovement {
private:
	Data::Savegame &_sg;
	const Data::LocationMap &_map;
	GameRandom &_random;

	// The map as the path finder sees it, and the place it found
	byte _grid[0x440] = {};
	int _foundX = 0;
	int _foundY = 0;

	int tileAt(int x, int y) const;
	int slotOf(int npc) const;

	// The data kept for a person's walk
	int walkIndex(int npc) const;
	void setWalkIndex(int npc, int index);
	int stuckCount(int npc) const;
	void setStuckCount(int npc, int count);
	byte &walkData(int npc, int index);

	// The person's place on the map
	void setActorPosition(int object, int x, int y);
	void moveTo(int npc, int x, int y);
	void setState(int npc, int state);
	void setSlot(int npc, int slot);
	int actorAt(int x, int y, int level) const;

	static int distance(int x1, int y1, int x2, int y2);
	void step(int direction);

	void buildGrid(int x, int y, int targetX, int targetY, int mode, int npc);
	bool search(int x, int y, int targetX, int targetY, int mode, int npc);
	bool searchLadder(int x, int y, bool up, int npc);
	int buildWalk(int npc, int direction, int targetX, int targetY);

	int canStand(int x, int y, int level, int npc, int slot) const;
	int canStep(int x, int y, int npc, int slot);
	int updateState(int npc, int hour);
	bool onStairs(int npc, int slot);
	void wander(int npc, int maxDistance, int slot);
	void approachOrFlee(int npc, int slot);
	void act(int npc, int slot);
	void moveHorses();

public:
	NpcMovement(Data::Savegame &savegame, const Data::LocationMap &map, GameRandom &random) :
		_sg(savegame), _map(map), _random(random) {}

	/**
	 * Lets each of the people of the location take a turn
	 */
	void turn();
};

} // namespace Ultima5
} // namespace Ultima

#endif
