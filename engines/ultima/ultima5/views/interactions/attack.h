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

#ifndef ULTIMA5_VIEWS_INTERACTIONS_ATTACK_H
#define ULTIMA5_VIEWS_INTERACTIONS_ATTACK_H

#include "common/ptr.h"
#include "ultima/ultima5/logic/attack_logic.h"
#include "ultima/ultima5/views/interactions/interaction.h"

namespace Ultima {
namespace Ultima5 {
namespace Views {
namespace Interactions {

/**
 * Attacks what is next to the party in a chosen direction, which can start a fight
 */
class Attack : public Interaction {
private:
	DirectionChooser _chooser;
	AttackLogic _logic;
	bool _done = false;
	bool _time = false;
	Interaction *_next = nullptr;

public:
	Attack(MessageLog &log);
	~Attack() override;

	/**
	 * Returns true if the party couldn't attack at all, and it's already over
	 */
	bool done() const {
		return _done;
	}

	bool keypress(const KeypressMessage &msg) override;

	bool takesTime() const override {
		return _time;
	}

	Interaction *takeNext() override {
		Interaction *next = _next;
		_next = nullptr;
		return next;
	}
};

} // namespace Interactions
} // namespace Views
} // namespace Ultima5
} // namespace Ultima

#endif
