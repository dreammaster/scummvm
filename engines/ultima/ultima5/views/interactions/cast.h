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

#ifndef ULTIMA5_VIEWS_INTERACTIONS_CAST_H
#define ULTIMA5_VIEWS_INTERACTIONS_CAST_H

#include "ultima/ultima5/logic/spell_logic.h"
#include "ultima/ultima5/logic/text_grid.h"
#include "ultima/ultima5/views/interactions/interaction.h"

namespace Ultima {
namespace Ultima5 {
namespace Views {
namespace Interactions {

/**
 * Takes the name of a spell typed as the words of power that its letters stand for
 */
class SpellEntry {
private:
	char _letters[4] = {};
	byte _width[4] = {};
	int _count = 0;

public:
	enum Result {
		PENDING,
		DONE
	};

	/**
	 * Handles a keypress: letters add a word, backspace takes one away, and the
	 * return key or space bar ends it, while escape throws it away
	 */
	Result handleKey(const KeypressMessage &msg, MessageLog &log);

	/**
	 * Returns which spell it came to, none if nothing was typed or one that isn't
	 */
	int spell() const;
};

/**
 * Casts a spell, which a member of the party does if there's more than one to pick
 */
class Cast : public Interaction {
private:
	enum State {
		STATE_WHO,
		STATE_ENTRY,
		STATE_DIRECTION,
		STATE_MEMBER,
		STATE_KEY,
		STATE_DONE
	};

	State _state = STATE_DONE;
	ActingMemberChooser _actor;
	SpellEntry _entry;
	PartyMemberChooser _members;
	SpellLogic _logic;
	Interaction *_next = nullptr;

	void askSpell();
	bool apply(SpellLogic::Need need);

public:
	/**
	 * @param combat	The fight it is in, if it is
	 */
	Cast(MessageLog &log, CombatLogic *combat = nullptr);
	~Cast() override;

	/**
	 * Returns true if it was over as it started
	 */
	bool done() const {
		return _state == STATE_DONE;
	}

	bool takesTime() const override {
		return _logic.passes();
	}

	Interaction *takeNext() override {
		Interaction *next = _next;
		_next = nullptr;
		return next;
	}

	bool keypress(const KeypressMessage &msg) override;
	void draw(View &view) override;
};

/**
 * Mixes spells from the reagents that are picked from what the party has
 */
class Mix : public Interaction {
private:
	enum State {
		STATE_ENTRY,
		STATE_REAGENTS,
		STATE_AMOUNT,
		STATE_DONE
	};

	State _state = STATE_DONE;
	SpellEntry _entry;
	SpellLogic _logic;
	TextGrid _grid;
	byte _owned[8] = {};
	int _ownedCount = 0;
	int _row = 0;
	int _picked = 0;
	int _spell = 0;
	int _amount = 0;
	int _digits = 0;

	void showReagents();
	void drawGrid();
	void askAmount();

public:
	Mix(MessageLog &log);

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
