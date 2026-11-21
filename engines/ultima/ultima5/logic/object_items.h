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

#ifndef ULTIMA5_LOGIC_OBJECT_ITEMS_H
#define ULTIMA5_LOGIC_OBJECT_ITEMS_H

#include "ultima/ultima5/logic/combat_logic.h"
#include "ultima/ultima5/logic/dungeon_items.h"

namespace Ultima {
namespace Ultima5 {

/**
 * What the party does with the things of a town or of the outdoors: taking what lies
 * about, opening doors and chests, picking locks and searching
 */
class ObjectItems {
public:
	enum Kind {
		KIND_GET,
		KIND_OPEN,
		KIND_JIMMY,
		KIND_SEARCH,
		KIND_KLIMB,
		KIND_PUSH,
		KIND_FIRE
	};

	enum Need {
		NEED_NONE,
		NEED_WHO
	};

private:
	Views::MessageLog &_log;
	DungeonItems _items;
	Kind _kind;
	int _dx = 0;
	int _dy = 0;
	int _x = 0;
	int _y = 0;
	int _object = 0;
	byte _tile = 0;
	bool _passes = true;

	enum Lock {
		LOCK_DOOR,
		LOCK_PRISONER,
		LOCK_CHEST
	};
	Lock _lock = LOCK_DOOR;

	static byte tileAt(int x, int y);
	static void setTileAt(int x, int y, byte tile);
	static int freeSlot();
	static int pickSlot();
	static int scanSlots(int lowest, int highest, bool offscreen);
	static void place(int slot, int tile, int x, int y, int z, int amount);
	static void clear(int slot);

	void describe(int tile);
	void get();
	Need open();
	void openChest(int who);
	Need jimmy();
	void jimmyDoor(int who);
	void jimmyPrisoner(int who);
	void jimmyChest(int who);
	void search(int who);
	void searchBody(int object, int who);
	bool searchMoonstone(int x, int y);
	bool searchHerbs(int x, int y);
	void searchHidden(int x, int y);
	void spawnLoot(int power);
	void spawn(int kind, int amount, int power, bool &found);
	void trapSense(int object, int who);
	bool climbReady();
	void climb();
	void push();
	bool fireReady();
	void fireCannon();
	void fireShip();

public:
	/**
	 * @param combat	The fight that it's done in, if it is
	 */
	ObjectItems(Views::MessageLog &log, Kind kind, CombatLogic *combat = nullptr);
	~ObjectItems();

	/**
	 * Shuts the door that was opened before, if it is still open
	 */
	static void closeDoor();

	/**
	 * Says if a direction is to be asked for. Picking a lock takes a key, and climbing
	 * needs a hook or a ladder under the party
	 */
	bool ready();

	/**
	 * Says if time goes by once it's over
	 */
	bool passes() const {
		return _passes;
	}

	/**
	 * Called when no direction was given
	 */
	void cancel();

	/**
	 * Does what it can with what is in the direction chosen. If the party member that does it
	 * has to be known, says so, and finish() is to be called with who it is
	 */
	Need begin(int dx, int dy);

	/**
	 * Does the rest
	 */
	void finish(int who);
};

} // namespace Ultima5
} // namespace Ultima

#endif
