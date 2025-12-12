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

#ifndef AESOP_MOUSE_H
#define AESOP_MOUSE_H

#ifndef VFX_H
#include "aesop/lib/vfx.h"
#endif

namespace Aesop {

#ifdef __cplusplus
extern "C" {
#endif

//
// MAX_MOUSE_BUFFER_SIZE should be set to the size of the visible area
// in bytes of the largest mouse cursor to be used
//

#define MAX_MOUSE_BUFFER_SIZE 16384

extern LONG MOUSE_init(LONG xsize, LONG ysize, LONG background);
extern void MOUSE_shutdown(void);

extern void MOUSE_show(void);
extern void MOUSE_hide(void);
extern void MOUSE_set_pointer(void *table, LONG shape);
extern void MOUSE_status(LONG *mx, LONG *my, LONG *ml, LONG *mr, LONG *mc);
extern void MOUSE_force_move(LONG new_x, LONG new_y);

extern void MOUSE_register_mouse_event_callback(void cdecl (*fn)
   (LONG x, LONG y));
extern void MOUSE_register_button_event_callback(void cdecl (*fn)
   (LONG left, LONG right, LONG center));
extern void MOUSE_register_watchdog_callback(LONG cdecl (*fn)
   (RECT *area));

extern void MOUSE_lock(void);
extern void MOUSE_unlock(void);
extern void MOUSE_hold(void);
extern void MOUSE_release(void);

extern LONG MOUSE_visible_area(RECT *area);
extern LONG MOUSE_shape_in_area(RECT *area);

extern void __cdecl MOUSE_serve(void);

extern void MOUSE_window_refresh(WINDOW *target, LONG x0, LONG y0, 
   LONG x1, LONG y1);
extern void MOUSE_pane_refresh(PANE *target, LONG x0, LONG y0, 
   LONG x1, LONG y1);

extern void MOUSE_pane_list_refresh(PANE_LIST *list);

#ifdef __cplusplus
}
#endif

} // namespace Aesop

#endif
