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

#ifndef ULTIMA5_VIEWS_INTERACTIONS_TALK_H
#define ULTIMA5_VIEWS_INTERACTIONS_TALK_H

#include "ultima/ultima5/logic/talk_logic.h"
#include "ultima/ultima5/views/interactions/interaction.h"

namespace Ultima {
namespace Ultima5 {
namespace Views {
namespace Interactions {

/**
 * Talks to the person next to the party in a chosen direction, or across a counter
 */
class Talk : public Interaction {
private:
	DirectionChooser _chooser;
	TalkLogic _logic;
	bool _talking = false;

	/**
	 * Finds the object at a place on the level the party is on, or zero
	 */
	static int objectAt(int x, int y);

	/**
	 * Returns the person that an object is, or -1 if it isn't one
	 */
	static int personOf(int object);

	/**
	 * Returns true if the tile is something that people talk across
	 */
	static bool isCounter(byte tile);

public:
	Talk(MessageLog &log) : Interaction(log), _logic(log) {}

	bool keypress(const KeypressMessage &msg) override;
};

} // namespace Interactions
} // namespace Views
} // namespace Ultima5
} // namespace Ultima

#endif
