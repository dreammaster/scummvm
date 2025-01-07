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

#ifndef AESOP_SYSTEM_TIMERS_H
#define AESOP_SYSTEM_TIMERS_H

#include "common/list.h"

namespace Aesop {

typedef void (*TimerCallback)();

struct Timer {
	int _frequency = 0;
	uint32 _nextTimeout = 0;
	TimerCallback _callback = nullptr;
	bool _active = false;

	Timer();
	Timer(TimerCallback callback, int frequency = 0) :
		_frequency(frequency), _callback(callback) {}
};

struct Timers : Common::List<Timer> {
public:
	Timers();
	~Timers();

	void poll();
};

typedef Common::List<Timer>::iterator TimerIterator;
typedef TimerIterator HTIMER;

extern HTIMER AIL_register_timer(TimerCallback callback);
extern void AIL_release_timer_handle(HTIMER timer);
extern void AIL_set_timer_frequency(HTIMER timer, int freq);
extern void AIL_start_timer(HTIMER timer);

} // namespace Aesop

#endif
