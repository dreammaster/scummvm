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

#include "spycraft/game/script.h"
#include "spycraft/game/user.h"
#include "spycraft/game/list.h"

namespace Spycraft {

Script::Script() {
	name = "Script";
	state = -1;
	cycles = ticks = seconds = 0;
	scratch = NULL;
}

void Script::init(Object *theClient, Object *theCaller, void *reg) {
	client = theClient;
	caller = theCaller;
	scratch = reg;
	old_ticks = 0;
	old_seconds = 0;
	cue();
}

void Script::doit() {
	long t;

	Object::doit();

	if (cycles > 0) {
		cycles--;
		if (cycles <= 0) {
			cue();
			return;
		}
	}

	if (seconds > 0) {
		if (!old_seconds)
			old_seconds = gameTime / 60;
		t = gameTime / 60;
		seconds -= (t - old_seconds);
		old_seconds = t;
		if (seconds <= 0) {
			seconds = 0;
			cue();
			return;
		}
	}

	if (ticks > 0) {
		if (!old_ticks)
			old_ticks = gameTime;
		ticks -= (gameTime - old_ticks);
		old_ticks = gameTime;
		if (ticks <= 0) {
			ticks = 0;
			cue();
			return;
		}
	}

}

void Script::cue() {
	__index = 0;
	changeState(state++);
}

void Script::dispose() {
	client->script = NULL;

	if (caller) {
		caller->cue();
		caller = NULL;
	}

	deathList->add(this);
}

} // namespace Spycraft
