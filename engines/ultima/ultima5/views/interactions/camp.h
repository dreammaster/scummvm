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

#ifndef ULTIMA5_VIEWS_INTERACTIONS_CAMP_H
#define ULTIMA5_VIEWS_INTERACTIONS_CAMP_H

#include "ultima/ultima5/views/interactions/interaction.h"

namespace Ultima {
namespace Ultima5 {
namespace Views {
namespace Interactions {

/**
 * Holes up in the open for some hours, with someone keeping watch if there is
 * someone to, or mends a ship at anchor
 */
class Camp : public Interaction {
private:
	enum State {
		STATE_HOURS,
		STATE_WATCH,
		STATE_GUARD,
		STATE_DONE
	};

	State _state = STATE_DONE;
	PartyMemberChooser _chooser;
	int _hours = 0;
	Interaction *_next = nullptr;

	/**
	 * Has the party rest, with a member of the party keeping watch or -1
	 */
	void rest(int guard);

public:
	Camp(MessageLog &log);
	~Camp() override;

	/**
	 * Returns true if it was all over at once
	 */
	bool done() const {
		return _state == STATE_DONE;
	}

	bool keypress(const KeypressMessage &msg) override;
	void draw(View &view) override;

	int musicTrack() const override {
		return Music::TRACK_STONES;
	}

	bool takesTime() const override {
		return false;
	}

	Interaction *takeNext() override {
		Interaction *next = _next;
		_next = nullptr;
		return next;
	}
};

/**
 * Sleeps in a bed of a town, for a number of hours that is asked for
 */
class Bed : public Interaction {
public:
	Bed(MessageLog &log);

	bool keypress(const KeypressMessage &msg) override;

	bool takesTime() const override {
		return false;
	}
};

/**
 * The apparition of Lord British that sometimes comes to a party that has rested
 */
class Apparition : public Interaction {
private:
	enum State {
		STATE_MEMBER,
		STATE_KARMA,
		STATE_DONE
	};

	State _state = STATE_MEMBER;
	int _member = 0;

	/**
	 * Heals the party member by member until one has to be told something
	 */
	void run();

public:
	Apparition(MessageLog &log);

	bool keypress(const KeypressMessage &msg) override;

	bool takesTime() const override {
		return false;
	}
};

} // namespace Interactions
} // namespace Views
} // namespace Ultima5
} // namespace Ultima

#endif
