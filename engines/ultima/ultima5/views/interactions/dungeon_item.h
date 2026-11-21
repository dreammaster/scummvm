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

#ifndef ULTIMA5_VIEWS_INTERACTIONS_DUNGEON_ITEM_H
#define ULTIMA5_VIEWS_INTERACTIONS_DUNGEON_ITEM_H

#include "ultima/ultima5/logic/dungeon_items.h"
#include "ultima/ultima5/views/interactions/interaction.h"

namespace Ultima {
namespace Ultima5 {
namespace Views {
namespace Interactions {

/**
 * The commands that deal with what is in the cells of a dungeon: look, search, open,
 * jimmy and get. Most of them ask which member of the party does it, and some which way
 */
class DungeonItem : public Interaction {
public:
	enum Kind {
		KIND_SEARCH,
		KIND_LOOK,
		KIND_OPEN,
		KIND_GET,
		KIND_JIMMY
	};

private:
	enum State {
		STATE_WHO,
		STATE_DIRECTION,
		STATE_DRINK,
		STATE_DONE
	};

	Kind _kind;
	State _state = STATE_DONE;
	ActingMemberChooser _actor;
	int _who = -1;
	int _x = 0;
	int _y = 0;

	/**
	 * Finds out who is to do it, asking if need be
	 */
	void findWho();

	/**
	 * Goes on once it is known who
	 */
	void proceed();

public:
	DungeonItem(MessageLog &log, Kind kind);

	/**
	 * Returns true if it was all done at once
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
