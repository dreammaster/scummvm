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

#ifndef AGS2_COMMON_EVENTS_H
#define AGS2_COMMON_EVENTS_H

#include "common/keyboard.h"
#include "ags2/lib/allegro/keyboard.h"

namespace AGS2 {

#define MAXEVENTS 15

enum {
	TS_REPEAT   = 1,
	TS_KEYPRESS = 2,
	TS_MCLICK   = 3
};

enum {
	KB_NUMLOCK_FLAG = Common::KBD_NUM,
	KB_CAPSLOCK_FLAG = Common::KBD_CAPS,
	KB_SCROLOCK_FLAG = Common::KBD_SCRL
};

// mouse cursor functions:
// set_mouse_cursor: changes visual appearance to specified cursor
extern void set_mouse_cursor(int newcurs);
extern void set_default_cursor();
extern int readkey();
extern bool keypressed();

extern int misbuttondown(int buno);
extern void mgetgraphpos();

} // namespace AGS2

#endif
