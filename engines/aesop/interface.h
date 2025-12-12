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

#ifndef AESOP_INTERFACE_H
#define AESOP_INTERFACE_H

namespace Aesop {

#ifdef __cplusplus
extern "C" {
#endif

extern LONG wait_ptr_state;

void cdecl init_interface(void);
void cdecl shutdown_interface(void);
void cdecl standby_cursor(void);
void cdecl resume_cursor(void);
void cdecl lock_mouse(void);
void cdecl unlock_mouse(void);
void cdecl show_mouse(void);
void cdecl hide_mouse(void);
void cdecl copy_mouse(int window);
ULONG cdecl mouse_in_window(LONG argcnt, ULONG wnd);
void cdecl refresh_window(LONG argcnt, ULONG src, ULONG target);

#ifdef __cplusplus
}
#endif

} // namespace Aesop

#endif
