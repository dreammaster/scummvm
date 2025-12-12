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

extern int32 MOUSE_init(int32 xsize, int32 ysize, int32 background);
extern void MOUSE_shutdown(void);

extern void MOUSE_show(void);
extern void MOUSE_hide(void);
extern void MOUSE_set_pointer(void *table, int32 shape);
extern void MOUSE_status(int32 *mx, int32 *my, int32 *ml, int32 *mr, int32 *mc);
extern void MOUSE_force_move(int32 new_x, int32 new_y);

extern void MOUSE_register_mouse_event_callback(void (*fn)
   (int32 x, int32 y));
extern void MOUSE_register_button_event_callback(void (*fn)
   (int32 left, int32 right, int32 center));
extern void MOUSE_register_watchdog_callback(int32 (*fn)
   (RECT *area));

extern void MOUSE_lock(void);
extern void MOUSE_unlock(void);
extern void MOUSE_hold(void);
extern void MOUSE_release(void);

extern int32 MOUSE_visible_area(RECT *area);
extern int32 MOUSE_shape_in_area(RECT *area);

extern void __MOUSE_serve(void);

extern void MOUSE_window_refresh(WINDOW *target, int32 x0, int32 y0, 
   int32 x1, int32 y1);
extern void MOUSE_pane_refresh(PANE *target, int32 x0, int32 y0, 
   int32 x1, int32 y1);

extern void MOUSE_pane_list_refresh(PANE_LIST *list);

#ifdef __cplusplus
}
#endif

} // namespace Aesop

#endif
