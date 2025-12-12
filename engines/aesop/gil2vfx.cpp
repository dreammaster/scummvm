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

#include "aesop/lib/vfx.h"
#include "aesop/lib/dll.h"
#include "aesop/lib/gil2vfx.h"
#include "aesop/lib/gil2vfxa.h"
#include "aesop/defs.h"
#include "aesop/rtmsg.h"
#include "aesop/rtsystem.h"
#include "aesop/mouse.h"
#include "aesop/rtres.h"

namespace Aesop {

#define  MAX_WINDOWS    50
#define  MAX_PANES      250

#define  FADE_INTERVALS 20

WINDOW   windows[MAX_WINDOWS];
PANE     panes[MAX_WINDOWS + MAX_PANES];
BYTE *bitmap_buffer;

int32     gil2vfx_active = FALSE;

TEXTWINDOW *twptr;

extern VFX_DESC *VFX;
extern BYTE *pathname;

extern int32 eop_reached;
extern int32 pending_flag;

void GIL2VFX_copy_window(uint32 src, uint32 dst) {
	if (gil2vfx_active != TRUE)
		return;

	VFX_pane_copy(&panes[src], 0, 0, &panes[dst], 0, 0, NO_COLOR);
}


int32 GIL2VFX_assign_window(int32 x1, int32 y1, int32 x2, int32 y2) {
	int32 i;

	for (i = 0; i < MAX_WINDOWS; i++)
	{
		if (windows[i].buffer == NULL)
		{
			windows[i].buffer = mem_alloc((x2 - x1 + 1) * (y2 - y1 + 1));
			// LUM WINDOW structure changes
			//windows[i].x0 = x1;
			//windows[i].y0 = y1;
			//windows[i].x1 = x2;
			//windows[i].y1 = y2;
			windows[i].x_max = x2;
			windows[i].y_max = y2;

			panes[i].window = &windows[i];
			panes[i].x0 = x1;
			panes[i].y0 = y1;
			panes[i].x1 = x2;
			panes[i].y1 = y2;

			return i;
		}
	}

	abend(MSG_TFW);
	return -1;
}

void GIL2VFX_init() {
	int32 dummy;

	void *DLL, *drvr;
	int32 w, h;

	//
	// Load the driver
	//

	DLL = FILE_read("MCGA.DLL", NULL);
	if (DLL == NULL)
	{
		printf("Missing or invalid 386FX driver\n");
		exit(1);
	}

	drvr = DLL_load(DLL, DLLMEM_ALLOC | DLLSRC_MEM, NULL);
	if (drvr == NULL)
	{
		printf("Invalid DLL image\n");
		exit(1);
	}
	free(DLL);

	//
	// Register the driver with the API
	//

	VFX_register_driver(drvr);

	VFX = VFX_describe_driver();

	w = VFX->scrn_width;
	h = VFX->scrn_height;

	//
	// Turn graphics on; set up window and pane
	//

	VFX_init_driver();

	gil2vfx_active = TRUE;

	bitmap_buffer = mem_alloc(320 * 200);

	windows[0].buffer = (void *)0x0a0000;       // Page 1 = 0
	// LUM WINDOW structure changes
	//windows[0].x0 = 0;
	//windows[0].y0 = 0;
	//windows[0].x1 = 319;
	//windows[0].y1 = 199;
	windows[0].x_max = 319;
	windows[0].y_max = 199;

	panes[0].window = &windows[0];
	panes[0].x0 = 0;
	panes[0].y0 = 0;
	panes[0].x1 = 319;
	panes[0].y1 = 199;

	dummy = GIL2VFX_assign_window(0, 0, 319, 199);  // Page 2 = 1

}

void GIL2VFX_shutdown_driver() {
	uint32 i;

	if (gil2vfx_active != TRUE)
		return;

	gil2vfx_active = FALSE;

	for (i = 0; i < MAX_WINDOWS; i++)
	{
		GIL2VFX_release_window(i);
	}

	mem_free(bitmap_buffer);

	VFX_shutdown_driver();
}


void GIL2VFX_release_window(uint32 wnd) {
	if (wnd < MAX_WINDOWS)
	{
		if (windows[wnd].buffer != NULL)
		{
			mem_free(windows[wnd].buffer);
			windows[wnd].buffer = NULL;
		}
	}
	GIL2VFX_release_subwindow(wnd);
}

int32 GIL2VFX_assign_subwindow(uint32 wnd, int32 x1, int32 y1, int32 x2, int32 y2) {
	uint32 i;

	for (i = MAX_WINDOWS; i < MAX_WINDOWS + MAX_PANES; i++)
	{
		if (panes[i].window == NULL)
		{
			if (wnd < MAX_WINDOWS)
				panes[i].window = &windows[wnd];
			else
				panes[i].window = panes[wnd].window;

			panes[i].x0 = x1;
			panes[i].y0 = y1;
			panes[i].x1 = x2;
			panes[i].y1 = y2;

			return i;
		}
	}

	abend(MSG_TFSW);

	return -1;
}

void GIL2VFX_release_subwindow(uint32 wnd) {
	panes[wnd].window = NULL;
}

int32 GIL2VFX_get_bitmap_width(void *shape_table, int32 shape_num) {
	return (VFX_shape_bounds(shape_table, shape_num) >> 16);
}

int32 GIL2VFX_get_bitmap_height(void *shape_table, int32 shape_num) {
	return ((int32)(WORD)VFX_shape_bounds(shape_table, shape_num));
}

int32 GIL2VFX_visible_bitmap_rect(int32 x1, int32 y1, int32 mirror,
	UBYTE *shapes, int32 shape_num, WORD *bounds) {
	int32 rectangle[4];
	int32 bm_width, bm_height;

	VFX_shape_visible_rectangle(shapes, shape_num, x1, y1, mirror, rectangle);

	if (mirror)
	{
		bm_width = VFX_shape_bounds(shapes, shape_num);
		bm_height = (int32)(WORD)bm_width;
		bm_width = bm_width >> 16;
	}


	if (mirror & X_MIRROR)
	{
		bounds[0] = (WORD)(bm_width + rectangle[0]);
		bounds[2] = (WORD)(bm_width + rectangle[2]);
	} else
	{
		bounds[0] = (WORD)rectangle[0];
		bounds[2] = (WORD)rectangle[2];
	}

	if (mirror & Y_MIRROR)
	{
		bounds[1] = (WORD)(bm_height + rectangle[1]);
		bounds[3] = (WORD)(bm_height + rectangle[3]);
	} else
	{
		bounds[1] = (WORD)rectangle[1];
		bounds[3] = (WORD)rectangle[3];
	}


	bounds[0] = (bounds[0] > panes[0].x0) ? bounds[0] : (WORD)panes[0].x0;
	bounds[1] = (bounds[1] > panes[0].y0) ? bounds[1] : (WORD)panes[0].y0;

	bounds[2] = (bounds[2] < panes[0].x1) ? bounds[2] : (WORD)panes[0].x1;
	bounds[3] = (bounds[3] < panes[0].y1) ? bounds[3] : (WORD)panes[0].y1;

	if ((bounds[0] > bounds[2]) || (bounds[1] > bounds[3]))
		return 0;            // Nothing visible
	else
		return 1;
}

void GIL2VFX_draw_bitmap(int32 wnd, int32 x, int32 y, int32 mirror, int32 scale,
	UBYTE *fade_table, UBYTE *shapes, int32 shape_num) {
	int32 xp = x - panes[wnd].x0;
	int32 yp = y - panes[wnd].y0;
	int32 x_scale, y_scale;
	int32 flags;
	int32 xs, ys;

	if (gil2vfx_active != TRUE)
		return;

	if ((scale == NULL) && (mirror == NO_MIRROR))
	{
		VFX_shape_draw(&panes[wnd], shapes, shape_num, xp, yp);
	} else
	{
		x_scale = ((scale) ? (scale << 8) : 0x10000);
		y_scale = ((scale) ? (scale << 8) : 0x10000);

		if (x_scale != 0x10000 || y_scale != 0x10000)
		{
			VFX_fixed_mul(VFX_shape_bounds(shapes, shape_num) & 0xffff0000,
				0x10000 - x_scale, &xs);
			VFX_fixed_mul(VFX_shape_bounds(shapes, shape_num) << 16,
				0x10000 - y_scale, &ys);

			if (mirror & X_MIRROR) xs = -xs;
			if (mirror & Y_MIRROR) ys = -ys;

			xp += xs >> 17;
			yp += ys >> 17;
		}

		switch (mirror)
		{
		case X_MIRROR:
			x_scale = -x_scale;
			xp += (VFX_shape_bounds(shapes, shape_num) >> 16);
			break;
		case Y_MIRROR:
			y_scale = -y_scale;
			yp += ((int32)(WORD)VFX_shape_bounds(shapes, shape_num));
			break;
		case XY_MIRROR:
			x_scale = -x_scale;
			y_scale = -y_scale;
			xp += (VFX_shape_bounds(shapes, shape_num) >> 16) - 1;
			yp += ((int32)(WORD)VFX_shape_bounds(shapes, shape_num)) - 1;
			break;
		case NO_MIRROR:
		default:
			break;
		}

		if (fade_table != NULL && scale != 0)
		{
			VFX_shape_lookaside(fade_table);
			flags = ST_XLAT;
		} else
			flags = NULL;

		VFX_shape_transform(&panes[wnd], shapes, shape_num, xp, yp,
			bitmap_buffer, 0, x_scale, y_scale, flags);
	}
}

int32 GIL2VFX_get_x1(uint32 wnd) {
	if (panes[wnd].window != NULL)
		return panes[wnd].x0;
	else
		return NULL;
}
int32 GIL2VFX_get_y1(uint32 wnd) {
	if (panes[wnd].window != NULL)
		return panes[wnd].y0;
	else
		return NULL;
}
int32 GIL2VFX_get_x2(uint32 wnd) {
	if (panes[wnd].window != NULL)
		return panes[wnd].x1;
	else
		return NULL;
}
int32 GIL2VFX_get_y2(uint32 wnd) {
	if (panes[wnd].window != NULL)
		return panes[wnd].y1;
	else
		return NULL;
}

void GIL2VFX_set_x1(uint32 wnd, int32 val) {
	panes[wnd].x0 = val;
}
void GIL2VFX_set_y1(uint32 wnd, int32 val) {
	panes[wnd].y0 = val;
}
void GIL2VFX_set_x2(uint32 wnd, int32 val) {
	panes[wnd].x1 = val;
}
void GIL2VFX_set_y2(uint32 wnd, int32 val) {
	panes[wnd].y1 = val;
}

void GIL2VFX_wipe_window(int32 wnd, int32 color) {
	if (gil2vfx_active != TRUE)
		return;

	VFX_pane_wipe(&panes[wnd], color);
}

void GIL2VFX_draw_dot(int32 wnd, int32 x, int32 y, int32 color) {
	int32 xp = x - panes[wnd].x0;
	int32 yp = y - panes[wnd].y0;

	if (gil2vfx_active != TRUE)
		return;

	VFX_pixel_write(&panes[wnd], xp, yp, (UBYTE)color);
}

int32 GIL2VFX_read_dot(int32 wnd, int32 x, int32 y) {
	int32 xp = x - panes[wnd].x0;
	int32 yp = y - panes[wnd].y0;

	if (gil2vfx_active != TRUE)
		return NULL;

	return VFX_pixel_read(&panes[wnd], xp, yp);
}

void GIL2VFX_draw_line(int32 wnd, int32 x1, int32 y1, int32 x2, int32 y2,
	int32 color) {
	int32 x1p = x1 - panes[wnd].x0;
	int32 y1p = y1 - panes[wnd].y0;
	int32 x2p = x2 - panes[wnd].x0;
	int32 y2p = y2 - panes[wnd].y0;

	if (gil2vfx_active != TRUE)
		return;

	VFX_line_draw(&panes[wnd], x1p, y1p, x2p, y2p, LD_DRAW, color);
}

void GIL2VFX_draw_rect(int32 wnd, int32 x1, int32 y1, int32 x2, int32 y2, int32 color) {
	int32 x1p = x1 - panes[wnd].x0;
	int32 y1p = y1 - panes[wnd].y0;
	int32 x2p = x2 - panes[wnd].x0;
	int32 y2p = y2 - panes[wnd].y0;

	if (gil2vfx_active != TRUE)
		return;

	VFX_line_draw(&panes[wnd], x1p, y1p, x2p, y1p, LD_DRAW, color);
	VFX_line_draw(&panes[wnd], x2p, y1p, x2p, y2p, LD_DRAW, color);
	VFX_line_draw(&panes[wnd], x2p, y2p, x1p, y2p, LD_DRAW, color);
	VFX_line_draw(&panes[wnd], x1p, y2p, x1p, y1p, LD_DRAW, color);
}

void GIL2VFX_fill_rect(int32 wnd, int32 x1, int32 y1, int32 x2, int32 y2, int32 color) {
	PANE rpane;

	if (gil2vfx_active != TRUE)
		return;

	rpane.window = panes[wnd].window;
	rpane.x0 = x1;
	rpane.y0 = y1;
	rpane.x1 = x2;
	rpane.y1 = y2;

	VFX_pane_wipe(&rpane, color);
}

void GIL2VFX_hash_rect(int32 wnd, int32 x1, int32 y1, int32 x2, int32 y2, int32 color) {
	int32 x1p = x1 - panes[wnd].x0;
	int32 y1p = y1 - panes[wnd].y0;
	int32 x2p = x2 - panes[wnd].x0;
	int32 y2p = y2 - panes[wnd].y0;

	if (gil2vfx_active != TRUE)
		return;

	VFX_rectangle_hash(&panes[wnd], x1p, y1p, x2p, y2p, color);
}

void GIL2VFX_light_fade(int32 src_wnd, int32 color) {
	RGB palette[256], clr;
	int32 i;

	if (gil2vfx_active != TRUE)
		return;

	VFX_DAC_read(color, &clr);

	for (i = 0; i < 256; i++)
		palette[i] = clr;

	VFX_window_fade(panes[src_wnd].window, palette, FADE_INTERVALS);
}

void GIL2VFX_color_fade(int32 src_wnd, int32 dst_wnd) {
	RGB palette[256], clr;
	uint32 colors[256], num_colors;
	int32 i;

	if (gil2vfx_active != TRUE)
		return;

	num_colors = VFX_color_scan(&panes[src_wnd], colors);

	for (i = 0; i < 256; i++)
	{
		VFX_DAC_read(i, &palette[i]);
	}

	clr = palette[*(panes[dst_wnd].window->buffer)];
	for (i = 0; i < num_colors; i++)
	{
		VFX_DAC_write(colors[i], &clr);
	}

	VFX_pane_copy(&panes[src_wnd], 0, 0, &panes[dst_wnd], 0, 0, NO_COLOR);

	VFX_window_refresh(&windows[0], 0, 0, VFX->scrn_width - 1, VFX->scrn_height - 1);

	VFX_window_fade(panes[dst_wnd].window, &palette, FADE_INTERVALS);
}

void GIL2VFX_pixel_fade(int32 src_wnd, int32 dest_wnd, int32 intervals) {
	if (gil2vfx_active != TRUE)
		return;

	VFX_pixel_fade(&panes[src_wnd], &panes[dest_wnd], intervals, 0); // LUM added last parameter
}

void GIL2VFX_select_text_window(TEXTWINDOW *tw) {
	twptr = tw;
}

int32 GIL2VFX_char_width(int32 ch) {
	return VFX_character_width(twptr->font, ch);
}

void GIL2VFX_home(void) {
	if (gil2vfx_active != TRUE)
		return;

	VFX_pane_wipe(&panes[twptr->window],
		((FONT *)(twptr->font))->font_background);

	twptr->htab = panes[twptr->window].x0;
	twptr->vtab = panes[twptr->window].y0;
}

void GIL2VFX_remap_font_color(int32 current, int32 new) {
	twptr->lookaside[current] = new;
}


int32 GIL2VFX_test_overlap(int32 wnd, int32 x1, int32 y1, UBYTE *shapes, int32 shape_num) {
	int32 x2 = (x1 + (VFX_shape_resolution(shapes, shape_num) >> 16));
	int32 y2 = (y1 + ((int32)(WORD)VFX_shape_resolution(shapes, shape_num)));

	if ((x1 <= panes[wnd].x1) && (x2 >= panes[wnd].x0) &&
		(y1 <= panes[wnd].y1) && (y2 >= panes[wnd].y0))
		return 1;
	else
		return 0;
}

void GIL2VFX_print(int32 operation, const char *format, ...) {
	va_list  arglist;
	int32     cw;

	va_start(arglist, format);

	if (operation == BUF)
	{
		cw = vsprintf(twptr->txtbuf, format, arglist);
		twptr->txtpnt = twptr->txtbuf + cw;
	} else if (operation == APP)
	{
		cw = vsprintf(twptr->txtpnt, format, arglist);
		twptr->txtpnt += cw;
	}
}

void GIL2VFX_scroll_window(int32 wnd, int32 dx, int32 dy, int32 flags, int32 background) {
	if (gil2vfx_active != TRUE)
		return;

	VFX_pane_scroll(&panes[wnd], dx, dy, flags, background);
}

void GIL2VFX_print_buffer(int32 linenum) {
	GIL2VFXA_print_buffer(&panes[twptr->window], linenum);
}

void GIL2VFX_cout(int32 c) {
	int32  cvtab, nvtab, htab;

	if (c == 10)
	{
		htab = twptr->htab = panes[twptr->window].x0;    // Carriage Return

		cvtab = twptr->vtab - panes[twptr->window].y0;
		cvtab += twptr->font->char_height;

		nvtab = cvtab + twptr->font->char_height;

		if (nvtab > panes[twptr->window].y1 - panes[twptr->window].y0)
		{
			if (twptr->continueFunction != NULL)
			{
				if ((twptr->continueFunction(twptr->htab)) == 0)
				{
					twptr->htab = htab;
					return;
				}
			}
			twptr->htab = htab;

			VFX_pane_scroll(&panes[twptr->window], 0, -twptr->font->char_height, PS_NOWRAP, twptr->font->font_background);
		} else
		{
			twptr->vtab += twptr->font->char_height;
		}
	} else if (c == 13)
	{
		twptr->htab = panes[twptr->window].x0;    // Carriage Return
	} else
	{
		twptr->htab += VFX_character_draw(&panes[twptr->window],
			twptr->htab - panes[twptr->window].x0,
			twptr->vtab - panes[twptr->window].y0,
			twptr->font, c, twptr->lookaside);
	}

}

void GIL2VFX_refresh_window(uint32 source, uint32 target) {
	//mouse_pane_refresh(&panes[source], &panes[target]);
	// LUM the function parameter has changed
	// check whether this is correct!
	PANE *loTargetPane = &panes[target];
	MOUSE_pane_refresh(&panes[source], loTargetPane->x0, loTargetPane->y0, loTargetPane->x1, loTargetPane->y1);
}

} // namespace Aesop
