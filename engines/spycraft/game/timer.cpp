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

#include "spycraft/game/timer.h"
#include "spycraft/game/game.h"
#include "spycraft/game/user.h"

namespace Spycraft {

TimerList *timers;

Timer::Timer() {
	name = "Timer";
	cycles = ticks = seconds = 0;
	old_ticks = 0;
	old_seconds = 0;
	caller = NULL;
}

void Timer::pause(int tOrF) {
	if (tOrF)
		timers->release(this);
	else
		timers->add(this);
}

void Timer::setCycles(int c) {
	cycles = c;
	timers->add(this);
}

void Timer::setCycles(int cy, Object *c) {
	caller = c;
	cycles = cy;
	timers->add(this);
}

void Timer::setTicks(int t) {
	ticks = t;
	timers->add(this);
}

void Timer::setTicks(int t, Object *c) {
	caller = c;
	ticks = t;
	timers->add(this);
}

void Timer::setReal(int s) {
	seconds = s;
	timers->add(this);
}

void Timer::setReal(int s, Object *c) {
	caller = c;
	seconds = s;
	timers->add(this);
}

void Timer::doit() {
	long t;

	if (cycles > 0) {
		cycles--;
		if (cycles <= 0) {
			//cueList->add(this);
			cue();
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
			//cueList->add(this);
			cue();
		}
	}

	if (ticks > 0) {
		if (!old_ticks)
			old_ticks = gameTime;
		ticks -= (gameTime - old_ticks);
		old_ticks = gameTime;
		if (ticks <= 0) {
			ticks = 0;
			//cueList->add(this);
			cue();
		}
	}
}

void Timer::cue() {
	if (caller) {
		//cueList->add(caller);
		caller->cue();
		caller = 0;
	}

	dispose();
}

void Timer::dispose() {
	deathList->add(this);
}

Timer::~Timer() {
	timers->release(this);
}

TimerList::TimerList() {
	name = "timers";
}

} // namespace Spycraft
