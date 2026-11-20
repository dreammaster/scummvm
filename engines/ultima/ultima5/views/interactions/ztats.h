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

#ifndef ULTIMA5_VIEWS_INTERACTIONS_ZSTATS_H
#define ULTIMA5_VIEWS_INTERACTIONS_ZSTATS_H

#include "ultima/ultima5/logic/inventory_logic.h"
#include "ultima/ultima5/views/interactions/interaction.h"

namespace Ultima {
namespace Ultima5 {
namespace Views {
namespace Interactions {

/**
 * Shows the pages of what the party has: the stats and arms of each of its members, then
 * what they carry between them, which are read through with the arrow keys
 */
class Ztats : public Interaction {
private:
	// The pages after those of the members, which have two each
	enum Page {
		PAGE_EQUIPMENT = 0xC,
		PAGE_REAGENTS = 0xD,
		PAGE_SPELLS = 0xE,
		PAGE_ITEMS = 0xF,
		PAGE_ARMAMENTS = 0x10
	};

	PartyMemberChooser _chooser;
	bool _choosing = true;
	int _page = 0;
	byte _items[InventoryLogic::SPECIAL_ITEMS];
	ItemList _list;
	bool _empty = false;

	/**
	 * Gets a page ready, for those that have to be
	 */
	void enterPage();

	/**
	 * Goes to another page for a key, or finishes
	 * @returns		True when it's done
	 */
	bool turnPage(int key);

	void drawMemberPage(View &view, bool arms);
	void drawEquipmentPage(View &view);
	void drawListPage(View &view);

public:
	Ztats(MessageLog &log);

	bool keypress(const KeypressMessage &msg) override;
	void draw(View &view) override;
};

} // namespace Interactions
} // namespace Views
} // namespace Ultima5
} // namespace Ultima

#endif
