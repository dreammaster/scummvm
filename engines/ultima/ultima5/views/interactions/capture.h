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

#ifndef ULTIMA5_VIEWS_INTERACTIONS_CAPTURE_H
#define ULTIMA5_VIEWS_INTERACTIONS_CAPTURE_H

#include "common/array.h"
#include "ultima/ultima5/logic/capture_logic.h"
#include "ultima/ultima5/views/interactions/interaction.h"

namespace Ultima {
namespace Ultima5 {
namespace Views {
namespace Interactions {

/**
 * The party is taken to a cell of the palace of Blackthorn, where it is asked for the mantra of a shrine
 * under the threat of a blade that falls a little further with each wrong answer
 */
class Capture : public Interaction {
private:
	enum State {
		STATE_RUNNING,
		STATE_KEY,
		STATE_ANSWER,
		STATE_DONE
	};

	enum StepType {
		STEP_PRINT,
		STEP_WAIT,
		STEP_SCRIPT,
		STEP_KEY,
		STEP_BLACKOUT,
		STEP_CELL,
		STEP_PLACE,
		STEP_MAP,
		STEP_QUIZ,
		STEP_LEAVE,
		STEP_FINISH
	};

	struct Step {
		StepType _type;
		int _value = 0;
		int _tile = 0;
		int _x = 0;
		int _y = 0;
		Common::String _text;
		const byte *_script = nullptr;
	};

	// Something drawn on the map of the cell
	struct Sprite {
		byte _tile = 0;
		int _x = 0;
		int _y = 0;
	};

	enum Screen {
		SCREEN_GAME,
		SCREEN_BLACK,
		SCREEN_CELL
	};

	State _state = STATE_RUNNING;
	Screen _screen = SCREEN_GAME;
	Common::Array<Step> _steps;
	uint _next = 0;
	int _insert = -1;
	int _delay = 0;

	byte _map[11 * 16] = {};
	Sprite _sprites[32];

	// The script that is being acted out
	const byte *_script = nullptr;
	int _repeat = 1;
	bool _pair = false;
	bool _sound = true;
	int _moveLeft = 0;
	int _first = 0;
	int _firstX = 0;
	int _firstY = 0;
	int _second = -1;
	int _secondX = 0;
	int _secondY = 0;

	// The questions
	int _shrine = -1;
	int _alive = 0;
	int _question = 0;
	bool _threatened = false;
	LineInput _line;

	void add(StepType type, int value = 0);
	void add(StepType type, const Common::String &text);
	void addScript(const byte *script);
	void addPlace(int actor, int tile, int x, int y);
	void addMap(int x, int y, int tile);

	/**
	 * Has the whole scene of the cell, up to the questions, be lined up
	 */
	void planScene();
	void startScript(const byte *script);

	/**
	 * Does what is lined up until it comes to something that has to be waited for
	 */
	void run();
	void placeParty();
	void loadCell();

	/**
	 * Acts out the script by a step of time
	 */
	void scriptTick();
	void moveUnit();
	static int actorOf(byte code, int &dx, int &dy);

	void askQuestion();
	void answered();
	void sliceOff(bool wrong);
	void endQuestions();
	void guardsLeave();

public:
	Capture(MessageLog &log);

	bool takesTime() const override {
		return false;
	}

	// The tune stops for the capture
	int musicTrack() const override {
		return Music::TRACK_NONE;
	}

	bool keypress(const KeypressMessage &msg) override;
	int tick() override;
	void draw(View &view) override;
};

} // namespace Interactions
} // namespace Views
} // namespace Ultima5
} // namespace Ultima

#endif
