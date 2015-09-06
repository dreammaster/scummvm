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

#ifndef AESOP_MOUSE_H
#define AESOP_MOUSE_H

#include "common/rect.h"
#include "aesop/defs.h"
#include "aesop/stubs.h"

namespace Aesop {

#ifndef VFX_H
//#include "aesop/vfx.h"
#endif

extern LONG mouse_init(LONG xsize, LONG ysize);
extern void mouse_shutdown(void);

extern void mouse_set_pointer(void *table, LONG shape, LONG uhot_x, LONG uhot_y);

extern void mouse_register_mouse_event_callback(void (*fn)(LONG x, LONG y));
extern void mouse_register_button_event_callback(void (*fn)(LONG left, LONG right, LONG center));
extern void mouse_register_watchdog_callback(LONG (*fn)(Common::Rect *area));

extern void mouse_unlock(void);
extern void mouse_hold(void);
extern void mouse_release(void);

extern LONG mouse_visible_area(Common::Rect *area);
extern LONG mouse_shape_in_area(Common::Rect *area);

/*
extern void mouse_serve(void);

extern void mouse_window_refresh(WINDOW *target, LONG x0, LONG y0, LONG x1, LONG y1);

extern void mouse_pane_refresh(PANE *source, PANE *target);
*/

} // End of namespace Aesop

#endif

