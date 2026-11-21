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

#ifndef ULTIMA5_VIEWS_INTERACTIONS_OBJECT_COMMAND_H
#define ULTIMA5_VIEWS_INTERACTIONS_OBJECT_COMMAND_H

#include "ultima/ultima5/logic/object_items.h"
#include "ultima/ultima5/views/interactions/interaction.h"

namespace Ultima {
namespace Ultima5 {
namespace Views {
namespace Interactions {

/**
 * The commands that take, open, unlock and search what is in the way of the party in a
 * town or outdoors: a direction, and often who does it
 */
class ObjectCommand : public Interaction {
private:
	enum State {
		STATE_DIRECTION,
		STATE_WHO,
		STATE_DONE
	};

	ObjectItems _items;
	State _state = STATE_DIRECTION;
	DirectionChooser _direction;
	ActingMemberChooser _actor;

	/**
	 * Goes on to find who does it
	 */
	bool startWho();

public:
	/**
	 * @param combat	The fight it is in, if it is
	 */
	ObjectCommand(MessageLog &log, ObjectItems::Kind kind, CombatLogic *combat = nullptr);

	/**
	 * Returns true if it was over as it started
	 */
	bool done() const {
		return _state == STATE_DONE;
	}

	bool takesTime() const override {
		return _items.passes();
	}

	bool keypress(const KeypressMessage &msg) override;
	void draw(View &view) override;
};

} // namespace Interactions
} // namespace Views
} // namespace Ultima5
} // namespace Ultima

#endif
