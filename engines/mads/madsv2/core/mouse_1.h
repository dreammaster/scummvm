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

#ifndef MADS_CORE_MOUSE_1_H
#define MADS_CORE_MOUSE_1_H

#include "mads/madsv2/core/general.h"
#include "mads/madsv2/core/sprite.h"

namespace MADS {
namespace MADSV2 {

#define MOUSE_TIMING_ONE        7       /* Mouse 1st repeat delay (ticks) */
#define MOUSE_TIMING_TWO        2       /* Mouse 2nd repeat delay (ticks) */

#define MOUSE_DOUBLE_TIMING     5       /* Double click threshold (ticks) */
#define MOUSE_BALLISTIC_TIMING  36      /* Ballistic threshold    (ticks) */

extern Buffer mouse_cursor_buffer;

extern word mouse_driver, mouse_known_mode;
extern int  mouse_video_mode;
extern byte mouse_showing;

extern int mouse_button, mouse_status, mouse_x, mouse_y;
extern bool mouse_start_stroke, mouse_stroke_going, mouse_changed;
extern bool mouse_latched, mouse_stop_stroke;
extern int mouse_old_x, mouse_old_y;

/* mouse_1.cpp */

extern int mouse_init(int driver_flag, int mouse_video_mode);

extern int mouse_set_hotspot(int spot_x, int spot_y);

extern void mouse_change_cursor_begin();
extern void mouse_change_cursor_end();

extern void mouse_screen_swap(int mouse_video_mode);
extern int mouse_get_video_mode();

extern void mouse_begin_double(int first_video_mode, int second_video_mode, int mono_to_right, int auto_freedom);
extern void mouse_check_double();
extern void mouse_end_double();
extern void mouse_double_freedom(int freedom_flag);

extern void mouse_show();
extern void mouse_hide();

extern void mouse_timing();

extern void mouse_freeze();
extern void mouse_thaw();

extern int mouse_get_status(int *x, int *y);

extern int mouse_press_info(int button, int *status, int *x, int *y);
extern int mouse_release_info(int button, int *status, int *x, int *y);

extern int mouse_scan(int *status, int *x, int *y);

extern int mouse_in_box_now(int ul_x, int ul_y, int lr_x, int lr_y);

extern void mouse_horiz_bound(int min_x, int max_x);
extern void mouse_vert_bound(int min_y, int max_y);

extern void mouse_force(int x, int y);

extern void mouse_set_work_buffer(unsigned char *work_buffer, int wrap_value);
extern void mouse_set_view_port_loc(int x1, int y1, int x2, int y2);

extern void mouse_set_view_port(int dx, int dy);

extern int mouse_refresh_view_port();
extern void mouse_refresh_done();

} // namespace MADSV2
} // namespace MADS

#endif
