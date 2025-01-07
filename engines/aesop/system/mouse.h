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
// C type definitions & mouse API prototypes
//

#ifndef AESOP_SYSTEM_MOUSE_H
#define AESOP_SYSTEM_MOUSE_H

#include "aesop/defs.h"
#include "aesop/system/vfx.h"

namespace Aesop {

extern LONG mouse_init(LONG xsize, LONG ysize);
extern void mouse_shutdown();

extern void mouse_show();
extern void mouse_hide();
extern void mouse_set_pointer(void *table, LONG shape, LONG uhot_x, LONG uhot_y);

extern void mouse_register_mouse_event_callback(void(*fn)
	(LONG x, LONG y));
extern void mouse_register_button_event_callback(void(*fn)
	(LONG left, LONG right, LONG center));
extern void mouse_register_watchdog_callback(LONG(*fn)
	(RECT *area));

extern void mouse_lock();
extern void mouse_unlock();
extern void mouse_hold();
extern void mouse_release();

extern LONG mouse_visible_area(RECT *area);
extern LONG mouse_shape_in_area(RECT *area);

extern void mouse_serve();

extern void mouse_window_refresh(WINDOW *target, LONG x0, LONG y0,
	LONG x1, LONG y1);

extern void mouse_pane_refresh(PANE *source, PANE *target);

} // namespace Aesop

#endif
