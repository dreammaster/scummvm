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

#ifndef ULTIMA5_VIEWS_INTERACTIONS_CONFRONT_H
#define ULTIMA5_VIEWS_INTERACTIONS_CONFRONT_H

#include "ultima/ultima5/logic/talk_logic.h"
#include "ultima/ultima5/views/interactions/interaction.h"

namespace Ultima {
namespace Ultima5 {
namespace Views {
namespace Interactions {

/**
 * What a person of a town does on coming up to the party: speaking to it, or setting upon it. The guards
 * arrest it, and others have to be fought
 */
class Confront : public Interaction {
public:
	enum Kind {
		KIND_ATTACK,	// Comes to blows
		KIND_TALK,		// Wants to speak
		KIND_ARREST		// Is to arrest the party
	};

private:
	enum State {
		STATE_TALK,
		STATE_ARREST,
		STATE_DONE
	};

	TalkLogic _logic;
	State _state = STATE_DONE;
	int _npc;
	Interaction *_next = nullptr;

	/**
	 * Has the guard say that the party is arrested
	 */
	void arrest();

	/**
	 * Ends it with a fight, if the person is one that fights, and otherwise sends them away
	 */
	void fight();

	/**
	 * Goes on from where the person has had their say
	 */
	void talked();

public:
	Confront(MessageLog &log, int npc, Kind kind);
	~Confront() override;

	/**
	 * Returns true if it was over as soon as it began
	 */
	bool done() const {
		return _state == STATE_DONE;
	}

	bool keypress(const KeypressMessage &msg) override;

	bool takesTime() const override {
		return false;
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
