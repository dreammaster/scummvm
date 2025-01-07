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

#include "common/textconsole.h"
#include "aesop/system/vfx.h"

namespace Aesop {

static VFX_DESC g_vfx;
VFX_DESC *VFX = &g_vfx;

void VFX_register_driver(void *drvr) {
}

VFX_DESC *VFX_describe_driver() {
	return VFX;
}

void VFX_init_driver() {
}

void VFX_shutdown_driver() {
}

void VFX_wait_vblank_leading() {
}

void VFX_shape_draw(PANE *pane, void *shapes, int shapeNum, int x, int y) {
	error("TODO");
}

LONG VFX_shape_bounds(void *shapes, int shapeNum) {
	error("TODO");
}

void VFX_shape_visible_rectangle(UBYTE *shapes, LONG shape_num,
	LONG x1, LONG y1, LONG mirror, LONG *rectangle) {
	error("TODO");
}
LONG VFX_shape_resolution(void *table, LONG shape) { error("TODO"); }
LONG VFX_shape_minxy(void *table, LONG shape) { error("TODO"); }
void VFX_shape_lookaside(UBYTE *fade_table) { error("TODO"); }
void VFX_shape_transform(PANE *pane, UBYTE *shapes, LONG shape_num,
	LONG xp, LONG yp, BYTE *bitmap_buffer, LONG zero,
	LONG x_scale, LONG y_scale, LONG flags) { error("TODO"); }

void VFX_window_refresh(WINDOW *window, int x0, int y0, int x1, int y1) { error("TODO"); }
void VFX_window_read(WINDOW *window, int x0, int y0, int x1, int y1) { error("TODO"); }
void VFX_window_fade(WINDOW *window, const RGB *pal, LONG interval) { error("TODO"); }

void VFX_pane_copy(PANE *source, int x0, int y0,
	PANE *target, int x1, int y1, int flags) { error("TODO"); }
void VFX_pane_wipe(PANE *pane, UBYTE color) { error("TODO"); }
void VFX_pane_scroll(PANE *pane, LONG dx, LONG dy,
	LONG flags, LONG background) { error("TODO"); }

void VFX_pixel_fade(PANE *src, PANE *dest, LONG interval) { error("TODO"); }
void VFX_pixel_write(PANE *pane, LONG xp, LONG yp, UBYTE color) { error("TODO"); }
UBYTE VFX_pixel_read(PANE *pane, LONG xp, LONG yp) { error("TODO"); }
void VFX_line_draw(PANE *pane, LONG x0, LONG y0, LONG x1, LONG y1,
	byte flags, UBYTE color) { error("TODO"); }
void VFX_rectangle_hash(PANE *pane, LONG x0, LONG y0, LONG x1, LONG y1, UBYTE color) { error("TODO"); }

void VFX_fixed_mul(LONG val1, LONG val2, LONG *result) { error("TODO"); }
void VFX_DAC_read(UBYTE color, RGB *rgb) { error("TODO"); }
void VFX_DAC_write(UBYTE color, const RGB *rgb) { error("TODO"); }
ULONG VFX_color_scan(PANE *pane, const ULONG *colors) { error("TODO"); }

LONG VFX_character_width(FONT *font, LONG ch) { error("TODO"); }
LONG VFX_character_draw(PANE *pane, LONG x, LONG y,
	const FONT *font, char c, UBYTE lookaside[256]) { error("TODO"); }


} // namespace Aesop
