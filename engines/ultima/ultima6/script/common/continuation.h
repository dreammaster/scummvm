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

#ifndef NUVIE_SCRIPT_COMMON_CONTINUATION_H
#define NUVIE_SCRIPT_COMMON_CONTINUATION_H

#include "ultima/ultima6/core/map.h"
#include "ultima/ultima6/script/script.h"

namespace Ultima {
namespace Ultima6 {
namespace Scripts {

/**
 * A script that has to wait for the player or for game time. Subclasses split
 * their logic into numbered steps in run(), and return from it with the state
 * to wait in (NUVIE_SCRIPT_GET_TARGET and so on) after saying which step to
 * continue at with wait(), or NUVIE_SCRIPT_FINISHED when done. The value the
 * engine supplies when resuming is left in the matching member.
 */
class Continuation : public ScriptThread {
private:
	uint8 _state = NUVIE_SCRIPT_NOT_STARTED;
	uint32 _data = 0;

	uint8 advance() {
		_state = run();
		return _state;
	}

protected:
	int _step = 0;
	MapCoord _location;
	NuvieDir _direction = (NuvieDir)0;
	uint8 _spellNum = 0;
	Obj *_obj = nullptr;

	/** Set when the engine resumed without a value, such as when the player cancelled a prompt */
	bool _resumedWithNil = false;

	/** Runs from the current step until the script finishes or has to wait */
	virtual uint8 run() = 0;

	/**
	 * Returns the state to wait in. The data is the actor number for an
	 * inventory object or talk request, or the number of turns to wait.
	 */
	uint8 wait(uint8 state, int nextStep, uint32 data = 0) {
		_step = nextStep;
		_data = data;
		return state;
	}

public:
	uint32 get_data() override {
		return _data;
	}
	uint8 get_state() override {
		return _state;
	}
	bool finished() override {
		return _state == NUVIE_SCRIPT_FINISHED || _state == NUVIE_SCRIPT_ERROR ||
			_state == NUVIE_SCRIPT_NOT_STARTED;
	}

	uint8 start() override {
		return advance();
	}
	uint8 resume_with_location(const MapCoord &loc) override {
		_resumedWithNil = false;
		_location = loc;
		return advance();
	}
	uint8 resume_with_direction(NuvieDir dir) override {
		_resumedWithNil = false;
		_direction = dir;
		return advance();
	}
	uint8 resume_with_spell_num(uint8 spellNum) override {
		_resumedWithNil = false;
		_spellNum = spellNum;
		return advance();
	}
	uint8 resume_with_obj(Obj *obj) override {
		_resumedWithNil = obj == nullptr;
		_obj = obj;
		return advance();
	}
	uint8 resume_with_nil() override {
		_resumedWithNil = true;
		return advance();
	}
};

/** A script with nothing to do */
class FinishedThread : public Continuation {
protected:
	uint8 run() override {
		return NUVIE_SCRIPT_FINISHED;
	}
};

} // End of namespace Scripts
} // End of namespace Ultima6
} // End of namespace Ultima

#endif
