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

#ifndef ULTIMA5_LOGIC_INVENTORY_LOGIC_H
#define ULTIMA5_LOGIC_INVENTORY_LOGIC_H

#include "common/str.h"
#include "ultima/ultima5/data/savegame.h"
#include "ultima/ultima5/logic/text_grid.h"

namespace Ultima {
namespace Ultima5 {

/**
 * What the party carries and what its members have ready, and the rules about
 * putting things on
 */
class InventoryLogic {
public:
	// Which belongings a list shows
	enum ListKind {
		LIST_REAGENTS,
		LIST_SPELLS,
		LIST_ITEMS,
		LIST_ARMAMENTS
	};

	// What's in the slots of what a character has ready
	enum Slot {
		SLOT_HELM = 0,
		SLOT_ARMOUR = 1,
		SLOT_HAND = 2,
		SLOT_OFF_HAND = 3,
		SLOT_RING = 4,
		SLOT_AMULET = 5
	};

	static constexpr byte NOTHING = 0xFF;
	static constexpr int ARMAMENTS = 0x30;
	static constexpr int SPECIAL_ITEMS = 0x26;

	/**
	 * Returns true if a character has an item ready
	 */
	static bool isReady(const Data::Character &who, int item);

	/**
	 * Takes an item off a character, returning true if they had it ready
	 */
	static bool unready(Data::Savegame &savegame, int who, int item);

	/**
	 * Works out what's free in the hands, which is both of them (2), the first (0), the
	 * second (1), or none (0xFF)
	 */
	static int freeHands(const Data::Character &who);

	/**
	 * What happened when something was put on or taken off
	 */
	struct EquipResult {
		Common::String _message;	// What's said in the message window
		bool _ringVanished = false;	// If so the list is closed
	};

	/**
	 * Puts an item on a character, or takes it off if it's on
	 */
	static EquipResult equip(Data::Savegame &savegame, int who, int item);

	/**
	 * Brings a dead member of the party back, with the experience that the karma allows and
	 * only a point of health
	 */
	static void resurrect(Data::Savegame &savegame, int who);

	/**
	 * Gets the amounts of the different items that aren't weapons and armour
	 * that the party has, with 0xFF where there's just one
	 */
	static void specialItems(const Data::Savegame &savegame, byte counts[SPECIAL_ITEMS]);

	/**
	 * Finds the previous or next thing in a list that's owned, or that's ready by
	 * a character if there's one given, or -1 if there isn't one
	 */
	static int prevOwned(int from, const byte *counts, int who);
	static int nextOwned(int from, int count, const byte *counts, int who);

	/**
	 * Prints the row of a list for something, with its amount, a marker and its name
	 */
	static void printRow(TextGrid &grid, ListKind kind, int index, const byte *counts, byte marker);
};

/**
 * A window of a list of what the party has, which shows some of it at a
 * time and scrolls. It can just be looked through, or a row picked
 */
class ItemList {
private:
	InventoryLogic::ListKind _kind = InventoryLogic::LIST_REAGENTS;
	const byte *_counts = nullptr;
	int _count = 0;
	int _who = InventoryLogic::NOTHING;
	bool _pick = false;
	byte _marker = '-';
	TextGrid _grid;

	int _first = -1;
	int _last = -1;
	int _row = 1;
	int _shown = 0;
	int _item = -1;
	int _arrows = 0;

public:
	/**
	 * Starts at the top of a list. Giving a character has it for picking
	 * what to ready, with what's ready by them marked and shown even if there's none left,
	 * and without one it can still have a row picked
	 */
	void start(InventoryLogic::ListKind kind, const byte *counts, int count, int who = InventoryLogic::NOTHING,
		bool pick = false);

	/**
	 * Returns true if there's nothing in the list
	 */
	bool empty() const;

	/**
	 * Prints what's shown in the window again, which has to be done after anything changes it
	 */
	void redraw();

	/**
	 * Moves through the list for the keys that do, and returns true if it was one
	 */
	bool scroll(int key);

	/**
	 * Returns the item that's picked
	 */
	int selectedItem() const {
		return _item;
	}

	/**
	 * Returns what's in the window, and which ways the list goes on: 0 for neither,
	 * 1 for down, 2 for up and 3 for both
	 */
	const TextGrid &grid() const {
		return _grid;
	}

	int arrows() const {
		return _arrows;
	}
};

} // namespace Ultima5
} // namespace Ultima

#endif
