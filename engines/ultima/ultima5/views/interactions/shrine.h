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

#ifndef ULTIMA5_VIEWS_INTERACTIONS_SHRINE_H
#define ULTIMA5_VIEWS_INTERACTIONS_SHRINE_H

#include "ultima/ultima5/logic/shrine_logic.h"
#include "ultima/ultima5/views/interactions/interaction.h"

namespace Ultima {
namespace Ultima5 {
namespace Views {
namespace Interactions {

/**
 * The inside of a shrine of one of the virtues, or of the Codex, where the party meditates
 * and the words that are told to it are read
 */
class Shrine : public Interaction {
private:
	enum State {
		STATE_VIRTUE,
		STATE_MANTRA,
		STATE_OFFER,
		STATE_WAIT,
		STATE_DONE
	};

	// What follows a key that is waited for
	enum Step {
		STEP_QUEST,
		STEP_RETURN,
		STEP_CODEX_OPEN,
		STEP_CODEX_PAGE,
		STEP_CODEX_FIND,
		STEP_CODEX_ANSWER,
		STEP_CODEX_WIND,
		STEP_CODEX_READ
	};

	State _state = STATE_DONE;
	Step _step = STEP_QUEST;
	bool _codex;
	int _shrine = 0;
	int _mantras = 0;
	bool _focused = true;
	bool _kneeling = false;
	int _reading = 0;
	int _answer = -1;
	Common::String _typed;
	byte _map[11 * 16] = {};

	void print(int offset);
	void printRunes(int offset);
	void askVirtue();
	void askMantra();
	void askOffer();
	void wait(Step step);
	void meditated();
	void finish();
	bool lineKey(const KeypressMessage &msg);
	bool waitKey();
	bool codexStep();

public:
	/**
	 * @param codex		True for the shrine of the Codex
	 */
	Shrine(MessageLog &log, bool codex);

	bool keypress(const KeypressMessage &msg) override;
	void draw(View &view) override;
};

} // namespace Interactions
} // namespace Views
} // namespace Ultima5
} // namespace Ultima

#endif
