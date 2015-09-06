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

#ifndef AESOP_STUBS_H
#define AESOP_STUBS_H

#include "common/scummsys.h"
#include "common/rect.h"
#include "aesop/defs.h"

namespace Aesop {

struct FONT {
	// TODO: Figure out what's needed
	int font_background;
	int char_height;
};

struct DWORDREGS {
	unsigned long edi;
	unsigned long esi;
	unsigned long ebp;
	unsigned long cflag;
	unsigned long ebx;
	unsigned long edx;
	unsigned long ecx;
	unsigned long eax;
	unsigned short eflags;
};

struct DWORDREGS_W {
	unsigned long di;
	unsigned long si;
	unsigned long bp;
	unsigned long cflag;
	unsigned long bx;
	unsigned long dx;
	unsigned long cx;
	unsigned long ax;
	unsigned short flags;
};

struct WORDREGS {
	unsigned short di, _upper_di;
	unsigned short si, _upper_si;
	unsigned short bp, _upper_bp;
	unsigned short cflag, _upper_cflag;
	unsigned short bx, _upper_bx;
	unsigned short dx, _upper_dx;
	unsigned short cx, _upper_cx;
	unsigned short ax, _upper_ax;
	unsigned short flags;
};

struct BYTEREGS {
	unsigned short di, _upper_di;
	unsigned short si, _upper_si;
	unsigned short bp, _upper_bp;
	unsigned long cflag;
	unsigned char bl;
	unsigned char bh;
	unsigned short _upper_bx;
	unsigned char dl;
	unsigned char dh;
	unsigned short _upper_dx;
	unsigned char cl;
	unsigned char ch;
	unsigned short _upper_cx;
	unsigned char al;
	unsigned char ah;
	unsigned short _upper_ax;
	unsigned short flags;
};

union REGS {
	struct DWORDREGS d;
	struct DWORDREGS x;
	struct WORDREGS w;
	struct BYTEREGS h;
};

struct VFX_DESC {
	int scrn_width, scrn_height;
};

typedef unsigned long HTIMER;

extern int int386(int intNum, REGS *inregs, REGS *outregs);

extern bool aesop_isspace(const char c);
extern bool aesop_isalpha(const char c);
extern bool aesop_isdigit(const char c);
extern bool aesop_isalnum(const char c);
extern bool aesop_ispunct(const char c);

extern void PollMod();
extern int kbhit();

extern HTIMER AIL_register_timer(void(*fn)());
extern void AIL_set_timer_frequency(HTIMER timer, int freq);
extern void AIL_start_timer(HTIMER htimer);
extern void AIL_release_timer_handle(HTIMER htimer);
extern void AIL_shutdown();


extern void shutdown_sound();

/*
extern void VFX_wait_vblank_leading();
extern void VFX_DAC_read(int index, RGB *rgb);
extern void VFX_DAC_write(int index, RGB *rgb);
extern VFX_DESC *VFX_describe_driver();
extern LONG VFX_pane_copy(PANE *source, LONG sx, LONG sy,
    PANE *target, LONG tx, LONG ty, LONG fill);
extern void VFX_init_driver(void);
extern void VFX_shutdown_driver(void);
extern LONG VFX_shape_bounds(void *shape_table, LONG shape_num);
extern void VFX_shape_visible_rectangle(void *shape_table,
    LONG shape_number, LONG hotX, LONG hotY, LONG mirror, LONG *rectangle);
extern LONG VFX_pane_wipe(PANE *pane, LONG color);
extern LONG VFX_pixel_read(PANE *pane, LONG x, LONG y);
extern LONG VFX_pixel_write(PANE *pane, LONG x, LONG y, ULONG color);
extern LONG VFX_line_draw(PANE *pane, LONG x0, LONG y0,
    LONG x1, LONG y1, LineDraw mode, LONG parm);
extern LONG VFX_pixel_fade(PANE *source, PANE *destination,
    LONG intervals, LONG rnd = 0);
extern LONG VFX_character_width(void *font, LONG character);

extern LONG VFX_rectangle_hash(PANE *pane, LONG x0, LONG y0,
    LONG x1, LONG y1, ULONG color);
extern LONG VFX_pane_scroll(PANE *pane, LONG dx, LONG dy,
    PaneScroll mode, LONG parm);
    */

} // End of namespace Aesop

#endif

