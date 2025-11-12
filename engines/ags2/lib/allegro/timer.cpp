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
#include "ags2/lib/allegro/timer.h"

namespace AGS2 {

struct TimerEntry {
	uint32 _timeInterval = 0;
	uint32 _nextExpiryTime = 0;
	TimerProc _proc = nullptr;
	TimerEntry *_next = nullptr;

	TimerEntry() {
	}
	TimerEntry(TimerProc proc, uint32 timeInterval, TimerEntry *next = nullptr) :
		_proc(proc), _timeInterval(timeInterval), _next(next) {
	}
};
static TimerEntry *timers;

void install_timer() {
	timers = nullptr;
}

void remove_timer() {
	TimerEntry *entry, *next;
	for (entry = timers; entry; entry = next) {
		next = entry->_next;
		delete entry;
	}

	timers = nullptr;
}

int install_int_ex(TimerProc proc, int speed) {
	timers = new TimerEntry(proc, BPS_TO_TIMER(speed), timers);
	return 0;
}

void poll_timers() {
	uint32 millis = g_system->getMillis();
	TimerEntry *entry, *next;

	for (entry = timers; entry; entry = next) {
		next = entry->_next;

		if (millis >= entry->_nextExpiryTime) {
			entry->_nextExpiryTime += entry->_timeInterval;
			(*entry->_proc)();
		}
	}
}

} // namespace AGS2
