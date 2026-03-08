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

#ifndef MADS_CORE_WINDOW_H
#define MADS_CORE_WINDOW_H

#include "mads/madsv2/core/general.h"

namespace MADS {
namespace MADSV2 {

#define WINDOW_DETECT_VALID              0
#define WINDOW_DETECT_START             -1
#define WINDOW_DETECT_NOTHING           -2
#define WINDOW_DETECT_SCROLL            -3
#define WINDOW_DETECT_SCROLL_LOWER      -4
#define WINDOW_DETECT_SCROLL_HIGHER     -5
#define WINDOW_DETECT_PAGE_LOWER        -6
#define WINDOW_DETECT_PAGE_HIGHER       -7
#define WINDOW_DETECT_LOWER             -8
#define WINDOW_DETECT_HIGHER            -9

/* frame drawing character defines */

#define window_shadow_color     colorbyte(hi_black,black)
#define abort_color             colorbyte(black, white)

#define ul_corner               0x0da
#define ur_corner               0x0bf
#define ll_corner               0x0c0
#define lr_corner               0x0d9

#define horiz_frame             0x0c4
#define vert_frame              0x0b3

#define upper_join              0x0c2
#define lower_join              0x0c1
#define left_join               0x0c3
#define right_join              0x0b4

#define four_way                0x0c5

/* scroll bar drawing character defines */

#define scroll_bar              0x0b2
#define thumb_mark              0x008
#define up_mark                 0x018
#define down_mark               0x019
#define left_mark               0x01b
#define right_mark              0x01a

#define WINDOW_MAX_TRAP_ROUTINES        8

#define WINDOW_SINGLE           0x01
#define WINDOW_DOUBLE           0x02
#define WINDOW_OBESE            0x03

struct WindowBox {
	int ul_x, ul_y, lr_x, lr_y;
	byte *storage;
};

typedef struct WindowBox Window;
typedef Window *WindowPtr;

typedef struct {
	WindowPtr window;
	int vertical;
	int side;
	int normal_color;
	int select_color;
	long min_value;
	long max_value;
	long scroll_value;
	long page_value;
} ScrollBar;

typedef ScrollBar *ScrollBarPtr;


extern int window_box_mode;


/* window_1 */
void window_set(WindowPtr window, int ul_x, int ul_y, int lr_x, int lr_y);

/* window_2 */
void window_line_across(WindowPtr window, short line_y);

/* window_3 */
void window_color(WindowPtr window, short new_color);
void window_wipe(WindowPtr window);
void window_shadow(WindowPtr window);

/* window_4 */
void window_define_scrollbar(WindowPtr window,
	ScrollBarPtr scroll,
	int vertical,
	int left_or_bottom,
	long min_value,
	long max_value,
	long scroll_value,
	long page_value,
	int normal_color,
	int select_color);

/* window_4 */
void window_draw_scrollbar(ScrollBarPtr scroll);

/* window_4 */
long window_translate_thumb(ScrollBarPtr scroll,
	long current_value);

long window_make_thumb(ScrollBarPtr scroll, long current_value);
long window_draw_thumb(ScrollBarPtr scroll, long current_value);

/* window_4 */
int window_scrollbar_detect(ScrollBarPtr scroll,
	int x, int y,
	long value, long *new_value,
	int prior_detect);

/* window_4 */
int window_detect(ScrollBarPtr scroll, int x, int y,
	long value, long *new_value,
	int prior_detect,
	long base_value, long value_mult);


int window_vert_detect(WindowPtr window, int x, int y,
	int tight_vert, int tight_horiz);

/* window_4 */
void window_horiz_scrollbar(WindowPtr window, short bar_color);
void window_vert_scrollbar(WindowPtr window, short bar_color);


/* window_5 */
void window_title(WindowPtr window, char *title,
	short title_color, short background_color);

/* window_6 */
void window_abort(char *message);
byte *window_create(WindowPtr window);
void      window_destroy(WindowPtr window);

/* window_7 */
int window_normal_color(WindowPtr window);
int window_line_width(WindowPtr window);
void window_text_setup(WindowPtr window, int follow);

/* window_8 */
void window_init_screen(WindowPtr main_window, char *prog_name,
	char *prog_author, char *prog_version,
	char *lib_version_no, char *compile_date);

/* window_9 */
void window_draw_box(WindowPtr window, int type);


/* window_a */
extern int  window_num_trap_routines;
extern byte *window_trap_routine[WINDOW_MAX_TRAP_ROUTINES];
extern char window_trap_string[WINDOW_MAX_TRAP_ROUTINES][80];

extern byte *window_incoming_string;
extern int window_display_string;

/* window_a */
void window_trap_output(WindowPtr window,
	void (*(any_char_routine))(),
	char *trap_string, ...);

void window_restore_output(void);


/* window_c.cpp */
void window_define(WindowPtr window,
	int x1, int y1, int x2, int y2,
	int color, int shadow, int save);

/* window_d.cpp */
void window_center(WindowPtr window, char *text,
	int line, int color);

/* window_e.cpp */
int window_show(WindowPtr window, char *text,
	int xx, int yy);

/* window_f.cpp */
void window_repeat(WindowPtr window, char wipe_item,
	int x1, int x2, int y1);
void window_wipe_line(WindowPtr window, int yy);

/* window_g.cpp */
int window_printf(WindowPtr window, int x, int y, char *string, ...);

/* window_h.cpp */
void window_clear(WindowPtr window);

/* window_i.cpp */
int window_show_color(WindowPtr window, char *text, int xx, int yy, int color);

/* window_j.cpp */
void window_10_install(void);
void window_10_remove(void);

} // namespace MADSV2
} // namespace MADS

#endif
