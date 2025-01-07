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

#include "common/system.h"
#include "aesop/system/timers.h"

namespace Aesop {

static Timers *g_timers = nullptr;

Timers::Timers() {
	g_timers = this;
}

Timers::~Timers() {
	g_timers = nullptr;
}

void Timers::poll() {
	uint32 currentTime = g_system->getMillis();

	for (TimerIterator it = begin(); it != end(); ++it) {
		Timer &timer = *it;

		if (timer._active && timer._frequency != 0 &&
				currentTime >= timer._nextTimeout) {
			timer._callback();
			timer._nextTimeout = currentTime + (1000 / timer._frequency);
		}
	}
}


HTIMER AIL_register_timer(TimerCallback callback) {
	g_timers->push_back(Timer(callback));
	return g_timers->end();
}

void AIL_release_timer_handle(HTIMER timer) {
	g_timers->erase(timer);
}

void AIL_set_timer_frequency(HTIMER timer, int freq) {
	timer->_frequency = freq;
}

void AIL_start_timer(HTIMER timer) {
	timer->_active = true;
}

} // namespace Aesop
