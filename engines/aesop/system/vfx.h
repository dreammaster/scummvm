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

#ifndef AESOP_SYSTEM_VFX_H
#define AESOP_SYSTEM_VFX_H

#include "common/scummsys.h"
#include "aesop/defs.h"

namespace Aesop {

#define NO_COLOR 1
#define LD_DRAW 1
#define ST_XLAT 1
#define PS_NOWRAP 1

struct VFX_DESC {
	int scrn_width = 0;
	int scrn_height = 0;
};

struct WINDOW : public RECT {
	byte *buffer = nullptr;
};

struct PANE : public RECT {
	WINDOW *window = nullptr;
};

struct FONT {
	int char_height = 0;
	UBYTE font_background = 0;
};

#include "common/pack-start.h"	// START STRUCT PACKING
struct RGB {
	byte r;
	byte g;
	byte b;
} PACKED_STRUCT;
#include "common/pack-end.h"	// END STRUCT PACKING

extern VFX_DESC *VFX;

extern void VFX_register_driver(void *drvr);
extern VFX_DESC *VFX_describe_driver();
extern void VFX_init_driver();
extern void VFX_shutdown_driver();

extern void VFX_wait_vblank_leading();
extern void VFX_shape_draw(PANE *pane, void *shapes, int shapeNum, int x, int y);
extern LONG VFX_shape_bounds(void *shapes, int shapeNum);
extern void VFX_shape_visible_rectangle(UBYTE *shapes, LONG shape_num,
	LONG x1, LONG y1, LONG mirror, LONG *rectangle);
extern LONG VFX_shape_resolution(void *table, LONG shape);
extern LONG VFX_shape_minxy(void *table, LONG shape);
extern void VFX_shape_lookaside(UBYTE *fade_table);
extern void VFX_shape_transform(PANE *pane, UBYTE *shapes, LONG shape_num,
	LONG xp, LONG yp, BYTE *bitmap_buffer, LONG zero,
	LONG x_scale, LONG y_scale, LONG flags);

extern void VFX_window_refresh(WINDOW *window, int x0, int y0, int x1, int y1);
extern void VFX_window_read(WINDOW *window, int x0, int y0, int x1, int y1);
extern void VFX_window_fade(WINDOW *window, const RGB *pal, LONG interval);

extern void VFX_pane_copy(PANE *source, int x0, int y0,
	PANE *target, int x1, int y1, int flags);
extern void VFX_pane_wipe(PANE *pane, UBYTE color);
extern void VFX_pane_scroll(PANE *pane, LONG dx, LONG dy,
	LONG flags, LONG background);

extern void VFX_pixel_fade(PANE *src, PANE *dest, LONG interval);
extern void VFX_pixel_write(PANE *pane, LONG xp, LONG yp, UBYTE color);
extern UBYTE VFX_pixel_read(PANE *pane, LONG xp, LONG yp);
extern void VFX_line_draw(PANE *pane, LONG x0, LONG y0, LONG x1, LONG y1,
	byte flags, UBYTE color);
extern void VFX_rectangle_hash(PANE *pane, LONG x0, LONG y0, LONG x1, LONG y1, UBYTE color);

extern void VFX_fixed_mul(LONG val1, LONG val2, LONG *result);
extern void VFX_DAC_read(UBYTE color, RGB *rgb);
extern void VFX_DAC_write(UBYTE color, const RGB *rgb);
extern ULONG VFX_color_scan(PANE *pane, const ULONG *colors);

extern LONG VFX_character_width(FONT *font, LONG ch);
extern LONG VFX_character_draw(PANE *pane, LONG x, LONG y,
	const FONT *font, char c, UBYTE lookaside[256]);

} // namespace Aesop

#endif
