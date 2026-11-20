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

#ifndef ULTIMA5_VIEWS_INTERACTIONS_USE_H
#define ULTIMA5_VIEWS_INTERACTIONS_USE_H

#include "ultima/ultima5/logic/inventory_logic.h"
#include "ultima/ultima5/logic/use_logic.h"
#include "ultima/ultima5/views/interactions/interaction.h"

namespace Ultima {
namespace Ultima5 {
namespace Views {
namespace Interactions {

/**
 * Picks one of the things that aren't weapons and armour, and uses it
 */
class Use : public Interaction {
private:
	enum State {
		STATE_PICKING,
		STATE_DIRECTION,
		STATE_MEMBER,
		STATE_DONE
	};

	State _state = STATE_PICKING;
	byte _items[InventoryLogic::SPECIAL_ITEMS];
	ItemList _list;
	UseLogic _logic;
	PartyMemberChooser _members;

	bool apply(UseLogic::Need need);

public:
	Use(MessageLog &log);

	/**
	 * Returns true if there was nothing to pick and it's already over
	 */
	bool done() const {
		return _state == STATE_DONE;
	}

	bool keypress(const KeypressMessage &msg) override;
	void draw(View &view) override;
};

} // namespace Interactions
} // namespace Views
} // namespace Ultima5
} // namespace Ultima

#endif
