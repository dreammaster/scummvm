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

namespace Aesop {

#define  PAGE1    0
#define  PAGE2    1

#define WND 0           // print() operations: output directly to window 
#define BUF 1           //                     output to start of buffer 
#define APP 2           //                     append to end of buffer   

#define J_LEFT 0        // justification modes for buffered text output
#define J_RIGHT 1
#define J_CENTER 2
#define J_FILL 3

#define NO_MIRROR 0     // mirror equates for draw_bitmap()
#define X_MIRROR 1
#define Y_MIRROR 2
#define XY_MIRROR 3

typedef struct
{
	int32 window;
	int32 htab, vtab;
	FONT *font;
	int32 delay;
	int32(*continueFunction)();
	char *txtbuf;
	char *txtpnt;
	int32 justify;
	UBYTE lookaside[256];
}
TEXTWINDOW;

int32 GIL2VFX_char_width(int32 ch);
void GIL2VFX_print(int32 operation, const char *format, ...);
void GIL2VFX_print_buffer(int32 linenum);

void GIL2VFX_copy_window(uint32 src, uint32 dst);
int32 GIL2VFX_assign_window(int32 x1, int32 y1, int32 x2, int32 y2);
void GIL2VFX_release_window(uint32 wnd);
void GIL2VFX_wipe_window(int32 wnd, int32 color);
void GIL2VFX_refresh_window(uint32 source, uint32 target);

int32 GIL2VFX_assign_subwindow(uint32 wnd, int32 x1, int32 y1, int32 x2, int32 y2);
void GIL2VFX_release_subwindow(uint32 wnd);

int32 GIL2VFX_get_x1(uint32 wnd);
int32 GIL2VFX_get_y1(uint32 wnd);
int32 GIL2VFX_get_x2(uint32 wnd);
int32 GIL2VFX_get_y2(uint32 wnd);
void GIL2VFX_set_x1(uint32 wnd, int32 val);
void GIL2VFX_set_y1(uint32 wnd, int32 val);
void GIL2VFX_set_x2(uint32 wnd, int32 val);
void GIL2VFX_set_y2(uint32 wnd, int32 val);


void GIL2VFX_init();
void GIL2VFX_shutdown_driver();

int32 GIL2VFX_get_bitmap_width(void *shape_table, int32 shape_num);
int32 GIL2VFX_get_bitmap_height(void *shape_table, int32 shape_num);
int32 GIL2VFX_visible_bitmap_rect(int32 x1, int32 y1, int32 mirror,
	UBYTE *shapes, int32 shape_num, WORD *bounds);
void GIL2VFX_draw_bitmap(int32 wnd, int32 x, int32 y, int32 mirror, int32 scale,
	UBYTE *fade_table, UBYTE *shapes, int32 shape_num);

void GIL2VFX_draw_dot(int32 wnd, int32 x, int32 y, int32 color);
int32 GIL2VFX_read_dot(int32 wnd, int32 x, int32 y);
void GIL2VFX_draw_line(int32 wnd, int32 x1, int32 y1, int32 x2, int32 y2,
	int32 color);
void GIL2VFX_draw_rect(int32 wnd, int32 x1, int32 y1, int32 x2, int32 y2, int32 color);
void GIL2VFX_fill_rect(int32 wnd, int32 x1, int32 y1, int32 x2, int32 y2, int32 color);
void GIL2VFX_hash_rect(int32 wnd, int32 x1, int32 y1, int32 x2, int32 y2, int32 color);

void GIL2VFX_light_fade(int32 src_wnd, int32 color);
void GIL2VFX_color_fade(int32 src_wnd, int32 dst_wnd);
void GIL2VFX_pixel_fade(int32 src_wnd, int32 dest_wnd, int32 intervals);

void GIL2VFX_select_text_window(TEXTWINDOW *tw);
int32 GIL2VFX_char_width(int32 ch);
void GIL2VFX_home(void);
void GIL2VFX_remap_font_color(int32 current, int32 new);
int32 GIL2VFX_test_overlap(int32 wnd, int32 x1, int32 y1, UBYTE *shapes, int32 shape_num);
void GIL2VFX_print(int32 operation, const char *format, ...);
void GIL2VFX_scroll_window(int32 wnd, int32 dx, int32 dy, int32 flags, int32 background);
void GIL2VFX_print_buffer(int32 linenum);
void GIL2VFX_cout(int32 c);

} // namespace Aesop
