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

#ifndef MADS_CORE_MOUSE_H
#define MADS_CORE_MOUSE_H

#include "mads/madsv2/core/general.h"

namespace MADS {
namespace MADSV2 {

#define MOUSE_TIMING_ONE        7       /* Mouse 1st repeat delay (ticks) */
#define MOUSE_TIMING_TWO        2       /* Mouse 2nd repeat delay (ticks) */

#define MOUSE_DOUBLE_TIMING     5       /* Double click threshold (ticks) */
#define MOUSE_BALLISTIC_TIMING  36      /* Ballistic threshold    (ticks) */

extern Buffer mouse_cursor_buffer;      /* Points at cursor pixel map     */

extern word mouse_driver;       /* True if mouse driver is currently active  */
extern word mouse_known_mode;   /* True if mouse in a 320x200 graphics mode  */
extern int  mouse_video_mode;   /* Rom BIOS video mode # of current cursor   */
extern byte mouse_showing;      /* Mouse cursor showing status (0 = show)    */

extern int mouse_button;        /* Last button pressed (0 = left, 1 = right) */
extern int mouse_status;        /* Button status flags                       */
extern int mouse_x, mouse_y;    /* Most recent cursor position               */
extern int mouse_start_stroke;  /* True if new button press this round       */
extern int mouse_stroke_going;  /* True if any button currently down         */
extern int mouse_changed;       /* True if position or any button changed    */
extern int mouse_latched;       /* Internal use (same as mouse_stroke_going) */
extern int mouse_stop_stroke;   /* True if button released this round        */
extern int mouse_any_stroke;    /* True if button down or just now released  */
extern int mouse_old_x;         /* Cursor X position on previous round       */
extern int mouse_old_y;         /* Cursor Y position on previous round       */
extern long mouse_clock;        /* Timing clock to insure at least 1 tick    */


/* mouse_1.asm */
int   mouse_init(int driver_flag, int mouse_video_mode);

/* mouse_1.asm */
int   mouse_set_hotspot(int spot_x, int spot_y);

void  mouse_change_cursor_begin(void);
void  mouse_change_cursor_end(void);

/* mouse_1.asm */
void  mouse_screen_swap(int mouse_video_mode);
int   mouse_get_video_mode(void);

/* mouse_1.asm */
void  mouse_begin_double(int first_video_mode, int second_video_mode,
	int mono_to_right, int auto_freedom);
void  mouse_check_double(void);
void  mouse_end_double(void);
void  mouse_double_freedom(int freedom_flag);

/* mouse_1.asm */
void  mouse_show(void);
void  mouse_hide(void);
int   mouse_get_status(int *x, int *y);

/*
int   mouse_press_info    (int button, int *status, int *x, int *y);
int   mouse_release_info  (int button, int *status, int *x, int *y);

int   mouse_in_box_now    (int ul_x, int ul_y, int lr_x, int lr_y);
*/

/* mouse_1.asm */
void  mouse_timing(void);

/* mouse_1.asm */
void  mouse_freeze(void);
void  mouse_thaw(void);


/* mouse_1.asm */
void  mouse_horiz_bound(int min_x, int max_x);
void  mouse_vert_bound(int min_y, int max_y);

/* mouse_1.asm */
void  mouse_force(int x, int y);

/* mouse_1.asm */
void  mouse_set_work_buffer(byte *work_buffer,
	int wrap_value);
void  mouse_set_view_port_loc(int x1, int y1, int x2, int y2);
void  mouse_set_view_port(int dx, int dy);

/* mouse_1.asm */
int   mouse_refresh_view_port(void);
void  mouse_refresh_done(void);

/* mouse_1.asm */
void mouse_disable_scale(void);

/* mouse_1.asm */
void mouse_hard_cursor_mode(int mode, Palette *mypal);

byte *mouse_get_stack(void);



/* mouse_2.c */
int  mouse_in_box(int ul_x, int ul_y, int lr_x, int lr_y);
void mouse_init_cycle(void);
void mouse_begin_cycle(int double_flag);
void mouse_end_cycle(int double_flag, int timing_flag);


/* mouse_3.c */
void mouse_cursor_sprite(void *series, int id);


/* mouse_4.c */
void mouse_video_init(void);
void mouse_video_update(int from_x, int from_y,
	int unto_x, int unto_y,
	int size_x, int size_y);

} // namespace MADSV2
} // namespace MADS

#endif
