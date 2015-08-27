/* RogueVM - RPG Engines
 *
 * RogueVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

//
//  Run-time interface subs
//

#ifndef AESOP_INTRFACE_H
#define AESOP_INTRFACE_H

namespace Aesop {

extern LONG wait_ptr_state;

void init_interface(void);
void shutdown_interface(void);
void standby_cursor(void);
void resume_cursor(void);
void lock_mouse(void);
void unlock_mouse(void);
void show_mouse(void);
void hide_mouse(void);
void copy_mouse(int window);
ULONG mouse_in_window(LONG argcnt, ULONG wnd);
void refresh_window(LONG argcnt, ULONG src, ULONG target);

} // End of namespace Aesop

#endif

