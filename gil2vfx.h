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

#ifndef AESOP_GIL2VFX_H
#define AESOP_GIL2VFX_H

#include "aesop/defs.h"
#include "aesop/stubs.h"

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

struct TEXTWINDOW {
   LONG window;
   LONG htab,vtab;
   FONT *font;
   LONG delay;
   LONG (*continueFunction)(LONG);
   char *txtbuf;
   char *txtpnt;
   LONG justify;
   UBYTE lookaside[256];
};

LONG GIL2VFX_char_width(LONG ch);
void GIL2VFX_print(LONG operation, const char *format, ...);
void GIL2VFX_print_buffer(LONG linenum);

void GIL2VFX_copy_window(ULONG src, ULONG dst);
LONG GIL2VFX_assign_window(LONG x1, LONG y1, LONG x2, LONG y2);
void GIL2VFX_release_window(ULONG wnd);
void GIL2VFX_wipe_window(LONG wnd, LONG color);
void GIL2VFX_refresh_window(ULONG source, ULONG target);

LONG GIL2VFX_assign_subwindow(ULONG wnd,LONG x1, LONG y1, LONG x2, LONG y2);
void GIL2VFX_release_subwindow(ULONG wnd);

LONG GIL2VFX_get_x1(ULONG wnd);
LONG GIL2VFX_get_y1(ULONG wnd);
LONG GIL2VFX_get_x2(ULONG wnd);
LONG GIL2VFX_get_y2(ULONG wnd);
void GIL2VFX_set_x1(ULONG wnd, LONG val);
void GIL2VFX_set_y1(ULONG wnd, LONG val);
void GIL2VFX_set_x2(ULONG wnd, LONG val);
void GIL2VFX_set_y2(ULONG wnd, LONG val);


void GIL2VFX_init();
void GIL2VFX_shutdown_driver();

LONG GIL2VFX_get_bitmap_width(void *shape_table, LONG shape_num);
LONG GIL2VFX_get_bitmap_height(void *shape_table, LONG shape_num);
LONG GIL2VFX_visible_bitmap_rect(LONG x1, LONG y1, LONG mirror,
                                 UBYTE *shapes, LONG shape_num,WORD *bounds);
void GIL2VFX_draw_bitmap(LONG wnd,LONG x, LONG y, LONG mirror, LONG scale,
                         UBYTE *fade_table, UBYTE *shapes, LONG shape_num);

void GIL2VFX_draw_dot(LONG wnd, LONG x, LONG y, LONG color);
LONG GIL2VFX_read_dot(LONG wnd, LONG x, LONG y);
void GIL2VFX_draw_line(LONG wnd,LONG x1, LONG y1, LONG x2, LONG y2,
                       LONG color);
void GIL2VFX_draw_rect(LONG wnd,LONG x1,LONG y1,LONG x2,LONG y2,LONG color);
void GIL2VFX_fill_rect(LONG wnd,LONG x1,LONG y1,LONG x2,LONG y2,LONG color);
void GIL2VFX_hash_rect(LONG wnd,LONG x1,LONG y1,LONG x2,LONG y2,LONG color);

void GIL2VFX_light_fade(LONG src_wnd, LONG color);
void GIL2VFX_color_fade(LONG src_wnd, LONG dst_wnd);
void GIL2VFX_pixel_fade(LONG src_wnd, LONG dest_wnd, LONG intervals);

void GIL2VFX_select_text_window(TEXTWINDOW *tw);
LONG GIL2VFX_char_width(LONG ch);
void GIL2VFX_home(void);
void GIL2VFX_remap_font_color(LONG current, LONG newColor);
LONG GIL2VFX_test_overlap(LONG wnd, LONG x1, LONG y1, UBYTE *shapes, LONG shape_num);
void GIL2VFX_print(LONG operation, const char *format, ...);
void GIL2VFX_scroll_window(LONG wnd, LONG dx, LONG dy, PaneScroll flags, LONG background);
void GIL2VFX_print_buffer(LONG linenum);
void GIL2VFX_cout(LONG c);

} // End of namespace Aesop

#endif

