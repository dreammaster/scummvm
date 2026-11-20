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

#ifndef ULTIMA5_VIEWS_INTERACTIONS_COMBAT_H
#define ULTIMA5_VIEWS_INTERACTIONS_COMBAT_H

#include "common/ptr.h"
#include "ultima/ultima5/logic/attack_logic.h"
#include "ultima/ultima5/logic/combat_logic.h"
#include "ultima/ultima5/views/interactions/interaction.h"

namespace Ultima {
namespace Ultima5 {
namespace Views {
namespace Interactions {

/**
 * A fight, which lasts until the party has won, been beaten or got away. It
 * shows the map of the fight in place of the map of the game
 */
class Combat : public Interaction {
private:
	enum State {
		STATE_COMMAND,
		STATE_AIM,
		STATE_SUB,
		STATE_DONE
	};

	CombatLogic _logic;
	State _state = STATE_COMMAND;
	Common::ScopedPtr<Interaction> _sub;
	int _subKey = 0;
	AttackLogic::Fight _fight;
	bool _hasFight = false;

	/**
	 * Carries on with the fight until it needs the player again
	 * @returns		True if the fight is over
	 */
	bool carryOn();

	/**
	 * Deals with what a command came to
	 * @returns		True if the fight is over
	 */
	bool handle(CombatLogic::Outcome outcome, int key);

public:
	Combat(MessageLog &log);

	/**
	 * Sets up the fight
	 * @param kind		The kind of enemy
	 * @param map		The number of the map of the fight, in brit.cbt
	 * @param lone		Whether a lone enemy is met in places where they come one at a time
	 * @returns			False if the map couldn't be loaded
	 */
	bool start(int kind, int map, bool lone);

	/**
	 * Has what the fight was against taken off the map once it's over
	 */
	void setAftermath(const AttackLogic::Fight &fight) {
		_fight = fight;
		_hasFight = true;
	}

	/**
	 * Returns true if the fight was over before the player had a turn
	 */
	bool done() const {
		return _state == STATE_DONE;
	}

	bool takesTime() const override {
		return false;
	}

	bool keypress(const KeypressMessage &msg) override;
	void draw(View &view) override;
};

} // namespace Interactions
} // namespace Views
} // namespace Ultima5
} // namespace Ultima

#endif
