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

#include "ags2/ac/actimer.h"
#include "ags2/lib/allegro/timer.h"
#include "ags2/vars.h"

namespace AGS2 {

void dj_timer_handler() {
	globalTimerCounter++;
	if (mvolcounter > 0)
		mvolcounter++;
}

void set_game_speed(int fpsSpeed) {
	frames_per_second = fpsSpeed;
	time_between_timers = 1000 / fpsSpeed;
	install_int_ex(dj_timer_handler, MSEC_TO_TIMER(time_between_timers));
}

} // namespace AGS2
