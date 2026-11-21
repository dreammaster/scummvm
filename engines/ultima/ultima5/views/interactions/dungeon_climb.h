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

#ifndef ULTIMA5_VIEWS_INTERACTIONS_DUNGEON_CLIMB_H
#define ULTIMA5_VIEWS_INTERACTIONS_DUNGEON_CLIMB_H

#include "ultima/ultima5/views/interactions/interaction.h"

namespace Ultima {
namespace Ultima5 {
namespace Views {
namespace Interactions {

/**
 * Climbs a ladder of a dungeon, up or down, asking which if both are there
 */
class DungeonClimb : public Interaction {
private:
	bool _up = false;
	bool _down = false;
	bool _asking = false;
	bool _time = false;

	/**
	 * Climbs the way that has been chosen
	 */
	void go();

public:
	DungeonClimb(MessageLog &log);

	/**
	 * Returns true if it was settled without a key
	 */
	bool done() const {
		return !_asking;
	}

	bool keypress(const KeypressMessage &msg) override;

	bool takesTime() const override {
		return _time;
	}
};

} // namespace Interactions
} // namespace Views
} // namespace Ultima5
} // namespace Ultima

#endif
