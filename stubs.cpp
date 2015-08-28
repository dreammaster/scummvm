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

#include "aesop/stubs.h"

namespace Aesop {

int int386(int intNum, REGS *inregs, REGS *outregs) {
	return 0;
}

bool aesop_isspace(const char c) {
	return c == ' ' || c == '\t';
}

extern bool aesop_isalpha(const char c) {
	return toupper(c) >= 'A' && toupper(c) <= 'Z';
}

bool aesop_isdigit(const char c) {
	return c >= '0' && c <= '9';
}

bool aesop_isalnum(const char c) {
	return aesop_isalpha(c) || aesop_isdigit(c);
}

bool aesop_ispunct(const char c) {
	return !aesop_isalnum(c);
}

void PollMod() {
}

int kbhit() {
	return 0;
}

HTIMER AIL_register_timer(void(*fn)()) { return 0; }
void AIL_set_timer_frequency(HTIMER timer, int freq) {}
void AIL_start_timer(HTIMER htimer) {}
void AIL_release_timer_handle(HTIMER htimer) {}

void VFX_wait_vblank_leading() {}
void VFX_DAC_write(int index, RGB *rgb) {}
void VFX_DAC_read(int index, RGB *rgb) {}

static VFX_DESC screen;
VFX_DESC *VFX_describe_driver() {
	screen.scrn_width = 320;
	screen.scrn_height = 200;
	return &screen;
}

LONG VFX_pane_copy(PANE *source, LONG sx, LONG sy,
	PANE *target, LONG tx, LONG ty, LONG fill) { return 0; }
void VFX_init_driver(void) {}
void VFX_shutdown_driver(void) {}
LONG VFX_shape_bounds(void *shape_table, LONG shape_num) { return 0; }
void VFX_shape_visible_rectangle(void *shape_table,
	LONG shape_number, LONG hotX, LONG hotY,
	LONG mirror, LONG *rectangle) {}
void VFX_fixed_mul(FIXED16 M1, FIXED16 M2, FIXED16 *result) {}
void VFX_shape_draw(PANE *pane, void *shape_table,
	LONG shape_number, LONG hotX, LONG hotY) {}
void VFX_shape_lookaside(UBYTE *table) {}
void VFX_shape_transform(PANE *pane,
	void *shape_table, LONG shape_number, LONG hotX, LONG hotY,
	void *buffer, LONG rot, LONG x_scale, LONG y_scale, LONG flags) {}
LONG VFX_pane_wipe(PANE *pane, LONG color) { return 0; }
void VFX_window_refresh(WINDOW *target, LONG x0, LONG y0,
	LONG x1, LONG y1) {}
LONG VFX_pixel_read(PANE *pane, LONG x, LONG y) { return 0; }
LONG VFX_pixel_write(PANE *pane, LONG x, LONG y, ULONG color) { return 0; }
LONG VFX_line_draw(PANE *pane, LONG x0, LONG y0,
	LONG x1, LONG y1, LineDraw mode, LONG parm) { return 0; }
LONG VFX_pixel_fade(PANE *source, PANE *destination,
	LONG intervals, LONG rnd) { return 0; }
LONG VFX_shape_resolution(void *shape_table, LONG shape_num) { return 0; }
LONG VFX_character_width(void *font, LONG character) { return 0; }
LONG VFX_character_draw(PANE *pane, LONG x, LONG y, void *font,
	LONG character, UBYTE *color_translate) { return 0; }
void VFX_window_fade(WINDOW *buffer, RGB *palette, LONG intervals) {}
LONG VFX_rectangle_hash(PANE *pane, LONG x0, LONG y0, LONG x1, LONG y1, ULONG color) { return 0; }
LONG VFX_color_scan(PANE *pane, ULONG *colors) { return 0; }
LONG VFX_pane_scroll(PANE *pane, LONG dx, LONG dy, PaneScroll mode, LONG parm) { return 0; }

}
