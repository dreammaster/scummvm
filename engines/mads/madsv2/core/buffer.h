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

#ifndef MADS_CORE_BUFFER_H
#define MADS_CORE_BUFFER_H

#include "mads/madsv2/core/general.h"
#include "mads/madsv2/core/font.h"

namespace MADS {
namespace MADSV2 {

#define MAX_PEEL_VALUE                  80

#define BUFFER_CREATED_PAGE_HANDLE      16384   /* Mask if EMS page handle created on the fly */

#define BUFFER_PRESERVE                 -1      /* Normal preserve attempt (EMS or disk) */
#define BUFFER_PRESERVE_RAM             -2      /* Disallows disk preservation           */
#define BUFFER_ATTEMPT_CONVENTIONAL     -8      /* Attempt conventional preserve         */

#define BUFFER_PRESERVED_CONVENTIONAL   -1      /* Preserved in conventional memory */
#define BUFFER_NOT_PRESERVED            -3      /* All attempts to preserve failed  */
#define BUFFER_PRESERVED_DISK           -10     /* Preserved on disk                */


extern int buffer_restore_keep_flag;


/* buffer_1.cpp */
int buffer_init(Buffer *buf, word x, word y);
int buffer_init_name(Buffer *buf, word x, word y, const char *block_name);

/* buffer_2.cpp */
int buffer_free(Buffer *buf);

/* buffer_3.cpp */
int buffer_fill(Buffer target, byte value);

/* buffer_4.cpp */
int buffer_rect_copy(Buffer from, Buffer unto,
	int    ul_x, int    ul_y,
	int    size_x, int    size_y);

/* buffer_5.cpp */
int buffer_rect_fill(Buffer target,
	int    ul_x, int    ul_y,
	int    size_x, int    size_y,
	byte   value);
/* buffer_6.cpp */
int buffer_rect_copy_2(Buffer from, Buffer unto,
	int    from_x, int    from_y,
	int    unto_x, int    unto_y,
	int    size_x, int    size_y);

/* buffer_7.cpp */
void buffer_put_pixel(Buffer buf, word x,
	word y, byte c);

/* buffer_8.cpp */
byte buffer_get_pixel(Buffer buf, word x, word y);

/* buffer_9.cpp */
void buffer_hline(Buffer buf, word x1, word x2,
	word y, byte color);

/* buffer_a.cpp */
void buffer_vline(Buffer buf, word x, word y1,
	word y2, byte color);

/* buffer_b.cpp */
void buffer_draw_box(Buffer buf, word x1, word y1,
	word x2, word y2, byte color);

/* buffer_d.cpp */
void buffer_hline_xor(Buffer buf, int x1, int x2, int y);

/* buffer_e.cpp */
void buffer_vline_xor(Buffer buf, int x, int y1, int y2);

/* buffer_f.cpp */
void buffer_draw_crosshair(Buffer buf, int x, int y);

/* buffer_g.cpp */
void buffer_draw_box_xor(Buffer buf, int x1, int y1, int x2, int y2);

/* buffer_h.cpp */
int buffer_get_delta_bounds(Buffer buf1, Buffer buf2,
	byte newcol, word *xl, word *xh,
	word *yl, word *yh);

/* buffer_i.cpp */
byte *fastcall buffer_pointer(Buffer *buf, int x, int y);
int buffer_conform(Buffer *buffer, int *x, int *y,
	int *xs, int *ys);

/* buffer_j.cpp */
int  buffer_inter_merge_2(Buffer from, Buffer unto,
	int    from_x, int    from_y,
	int    unto_x, int    unto_y,
	int    size_x, int    size_y);

/* buffer_k.cpp */
void buffer_line(Buffer target, int x1, int y1, int x2, int y2,
	int color);

/* buffer_l.cpp */
void buffer_line_xor(Buffer target, int x1, int y1,
	int x2, int y2);

/* buffer_m.cpp */
int buffer_legal(Buffer walk, int orig_wrap,
	int x1, int y1, int x2, int y2);

/* buffer_n.cpp */
extern word pattern_control_value;
extern int  auto_pattern;

word buffer_rect_fill_pattern(Buffer target,
	int    ul_x, int    ul_y,
	int    size_x, int    size_y,
	int    base_x, int    base_y,
	int    base_xs,
	byte   value1, byte   value2,
	word   start_accum,
	word   note_line);


/* buffer_o.cpp */
int buffer_rect_fill_swap(Buffer target,
	int    ul_x, int    ul_y,
	int    size_x, int    size_y,
	byte   value1, byte   value2);

/* buffer_p.cpp */
void buffer_peel_horiz(Buffer *target, int peel);

/* buffer_q.cpp */
void buffer_peel_vert(Buffer *target, int peel,
	byte *work_memory, long work_size);

/* buffer_r.cpp */
int  buffer_to_disk(Buffer *source, int x, int y,
	int xs, int ys);
void buffer_from_disk(Buffer *source, int buffer_id,
	int keep_flag,
	int x, int y,
	int xs, int ys);

/* buffer_s.cpp */
int buffer_to_ems(Buffer *source, int page_handle,
	int source_ems_handle,
	int x, int y, int xs, int ys);
int buffer_from_ems(Buffer *source, int page_handle,
	int target_ems_handle,
	int x, int y, int xs, int ys);

/* buffer_t.cpp */
int  buffer_preserve(Buffer *source, int flags,
	int source_ems_handle,
	int x, int y, int xs, int ys);
void buffer_restore(Buffer *source, int preserve_handle,
	int target_ems_handle,
	int x, int y, int xs, int ys);


/* buffer_u.cpp */
int buffer_rect_translate(Buffer from, Buffer unto,
	int    from_x, int    from_y,
	int    unto_x, int    unto_y,
	int    size_x, int    size_y,
	byte *table);


/* buffer_w.cpp */
int buffer_scan(Buffer *buffer, int magic,
	int base_x, int base_y,
	int size_x, int size_y);

/* buffer_w.cpp */
int buffer_compare(Buffer *buffer0, Buffer *buffer1,
	int base_x, int base_y,
	int base_x2, int base_y2,
	int size_x, int size_y);

} // namespace MADSV2
} // namespace MADS

#endif
