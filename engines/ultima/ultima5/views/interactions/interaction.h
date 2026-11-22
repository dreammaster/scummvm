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

#ifndef ULTIMA5_VIEWS_INTERACTIONS_INTERACTION_H
#define ULTIMA5_VIEWS_INTERACTIONS_INTERACTION_H

#include "ultima/ultima5/audio/music.h"
#include "ultima/ultima5/logic/world_logic.h"
#include "ultima/ultima5/views/message_log.h"
#include "ultima/ultima5/views/view.h"

namespace Ultima {
namespace Ultima5 {
namespace Views {
namespace Interactions {

// What an interaction may say of the music that goes with it, besides the tracks of the music
constexpr int MUSIC_AUTO = -2;
constexpr int MUSIC_KEEP = -3;

// What an interaction that goes on by itself says after a step of time
constexpr int TICK_NONE = 0;
constexpr int TICK_REDRAW = 1;
constexpr int TICK_DONE = 2;

/**
 * The remainder of a command that needs more input than the key that
 * started it, such as a direction or a party member. It reads the keys
 * that follow and writes its prompts and results to the message window
 */
class Interaction {
protected:
	MessageLog &_log;

public:
	Interaction(MessageLog &log) : _log(log) {}
	virtual ~Interaction() {}

	/**
	 * Handles a keypress
	 * @returns		True once the command is finished
	 */
	virtual bool keypress(const KeypressMessage &msg) = 0;

	/**
	 * Returns true if time goes by in the game once it's over
	 */
	virtual bool takesTime() const {
		return true;
	}

	/**
	 * Gives what is to follow once this is over, if anything, and hands over its ownership
	 */
	virtual Interaction *takeNext() {
		return nullptr;
	}

	/**
	 * Gives the tune that is to play while this is going on: none for silence, whichever there is for where
	 * the party is, or to leave what is playing
	 */
	virtual int musicTrack() const {
		return MUSIC_AUTO;
	}

	/**
	 * Lets time go by for what acts without being pressed a key
	 * @returns		Whether there is something to redraw, or that it is over
	 */
	virtual int tick() {
		return TICK_NONE;
	}

	/**
	 * Draws over the game view
	 */
	virtual void draw(View &) {}
};

/**
 * Asks for one of the four directions
 */
class DirectionChooser {
private:
	Direction _direction = DIR_NORTH;

public:
	enum Result {
		PENDING,
		CANCELLED,
		CHOSEN
	};

	/**
	 * Handles a keypress, echoing the direction picked. Escape and
	 * the space bar cancel
	 */
	Result handleKey(const KeypressMessage &msg, MessageLog &log);

	Direction direction() const {
		return _direction;
	}
};

/**
 * Asks for a member of the party, shown by highlighting their name. The
 * arrows move the highlight, a number jumps to that member, and Enter or
 * the space bar picks one
 */
class PartyMemberChooser {
private:
	int _index = 0;
	bool _allowAll = false;

public:
	enum Result {
		PENDING,
		CANCELLED,
		CHOSEN,
		ALL
	};

	/**
	 * Lets the zero key pick everyone, for things that can show the party's shared belongings
	 */
	void allowAll() {
		_allowAll = true;
	}

	/**
	 * Handles a keypress. Escape cancels
	 */
	Result handleKey(const KeypressMessage &msg);

	/**
	 * Returns the member highlighted, counting from zero
	 */
	int index() const {
		return _index;
	}

	/**
	 * Starts again with the first member highlighted
	 */
	void reset() {
		_index = 0;
	}

	/**
	 * Draws the highlight and the label that says to select
	 */
	void draw(View &view) const;
};

/**
 * Takes a line of text that is typed, which is echoed to the message window. The return key
 * ends it and escape throws it away
 */
class LineInput {
private:
	Common::String _text;

public:
	/**
	 * Handles a keypress
	 * @returns		True once the line is finished
	 */
	bool handleKey(const KeypressMessage &msg, MessageLog &log, int maxLength);

	const Common::String &text() const {
		return _text;
	}

	void clear() {
		_text.clear();
	}
};

/**
 * Finds out which member of the party does something that one has to: the one picked to
 * lead, or the only one that's fit, or else whichever the player picks
 */
class ActingMemberChooser {
private:
	PartyMemberChooser _chooser;
	bool _asking = false;
	int _who = -1;

public:
	enum Result {
		PENDING,
		CANCELLED,
		CHOSEN
	};

	/**
	 * Starts, asking if there's more than one that could do it. Says that nobody can
	 * if that is so
	 */
	Result begin(MessageLog &log);

	/**
	 * Handles a keypress while asking
	 */
	Result handleKey(const KeypressMessage &msg, MessageLog &log);

	/**
	 * Returns the member who is to do it
	 */
	int who() const {
		return _who;
	}

	bool asking() const {
		return _asking;
	}

	void draw(View &view) const {
		if (_asking)
			_chooser.draw(view);
	}
};

} // namespace Interactions
} // namespace Views
} // namespace Ultima5
} // namespace Ultima

#endif
