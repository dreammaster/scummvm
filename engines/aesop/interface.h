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

//
//  Run-time interface subs
//

#ifndef AESOP_INTRFACE_H
#define AESOP_INTRFACE_H

#include "aesop/defs.h"

namespace Aesop {

extern LONG wait_ptr_state;

void init_interface();
void shutdown_interface();
void standby_cursor();
void resume_cursor();
void lock_mouse();
void unlock_mouse();
void show_mouse();
void hide_mouse();
void copy_mouse(int window);
ULONG mouse_in_window(LONG argcnt, ULONG wnd);
void refresh_window(LONG argcnt, ULONG src, ULONG target);

} // namespace Aesop

#endif

