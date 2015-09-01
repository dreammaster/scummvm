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

//#include "aesop/vfx.h"
#include "aesop/aesop.h"
#include "aesop/mouse.h"
#include "aesop/gil2vfx.h"
#include "aesop/gil2vfxa.h"

#include "aesop/defs.h"
#include "aesop/shared.h"
#include "aesop/rtsystem.h"
#include "aesop/rtmsg.h"
#include "aesop/resources.h"
#include "aesop/intrface.h"
#include "aesop/rtobject.h"
#include "aesop/graphics.h"
#include "aesop/stubs.h"

namespace Aesop {

VFX_DESC *VFX;

LONG window_owner[256];

ULONG lastg_x;
ULONG lastg_y;
ULONG lastg_p;

TEXTWINDOW tw[NTW];
//WINDOW tw_refresh[NTW];
LONG tw_refresh[NTW];

char strbuf[256];              // used for string resource buffering
char txtbuf[2402];             // used for word-wrapped text (also used as
                               // buffer for dot pattern effects in EYE.C!)
                               // Final 2 bytes used for overflow checking
#define FBEG 0
#define FCNT 192

UBYTE  F_fade[11][256];        // fixed      00-AF (also initializes B0-FF)
UBYTE  W_fade[11][16];         // wallset    B0-BF
UBYTE M1_fade[11][32];         // monster #1 C0-DF
UBYTE M2_fade[11][32];         // monster #2 E0-FF

UBYTE M1_gry[32];
UBYTE M1_wht[32];
UBYTE M2_gry[32];
UBYTE M2_wht[32];
UBYTE M1_blu[32];
UBYTE M2_blu[32];
UBYTE M1_grn[32];
UBYTE M2_grn[32];
UBYTE M1_brn[32];
UBYTE M2_brn[32];
UBYTE F_grn[256];
UBYTE F_blu[256];
UBYTE F_red[256];
UBYTE F_gry[256];

UWORD first_color[5] = { 0x00,0xb0,0xc0,0xe0,0xb0 };
UWORD num_colors[5] =  {  256,  16,  32,  32,  80 };

UBYTE *fade_tables[5][16] =
                            { { F_fade[0],F_fade[1],F_fade[2],F_fade[3],
                                F_fade[4],F_fade[5],F_fade[6],F_fade[7],
                                F_fade[8],F_fade[9],F_fade[10],
                                F_blu,F_grn,F_red,F_gry,NULL },

                              { W_fade[0],W_fade[1],W_fade[2],W_fade[3],
                                W_fade[4],W_fade[5],W_fade[6],W_fade[7],
                                W_fade[8],W_fade[9],W_fade[10],
                                NULL,NULL,NULL,NULL,NULL },

                              { M1_fade[0],M1_fade[1],M1_fade[2],M1_fade[3],
                                M1_fade[4],M1_fade[5],M1_fade[6],M1_fade[7],
                                M1_fade[8],M1_fade[9],M1_fade[10],
                                M1_gry,M1_wht,M1_grn,M1_blu,M1_brn },

                              { M2_fade[0],M2_fade[1],M2_fade[2],M2_fade[3],
                                M2_fade[4],M2_fade[5],M2_fade[6],M2_fade[7],
                                M2_fade[8],M2_fade[9],M2_fade[10],
                                M2_gry,M2_wht,M2_grn,M2_blu,M2_brn },

                              { NULL,NULL,NULL,NULL,
                                NULL,NULL,NULL,NULL,
                                NULL,NULL,NULL,
                                NULL,NULL,NULL,NULL,NULL } };



//
// Fixed-palette color ranges for magical auras, poison effects, 
// petrification effects, damage effects, etc.
//

#define BLU_BEG 0x55
#define BLU_NUM 11
#define GRN_BEG 0x70
#define GRN_NUM 11
#define GRY_BEG 0x67
#define GRY_NUM 9
#define RED_BEG 0x22
#define RED_NUM 9
#define BRN_BEG 0x96
#define BRN_NUM 7
#define FIX_WHT 0x0b

UBYTE blu_inten[BLU_NUM];
UBYTE grn_inten[GRN_NUM];
UBYTE gry_inten[GRY_NUM];
UBYTE red_inten[RED_NUM];
UBYTE brn_inten[BRN_NUM];

UBYTE text_colors[9] =
   {
   DK_GRN,
   LT_GRN,
   YEL,   
   LT_RED,
   DK_RED,
   BLK,   
   WHT,
   WHT,
   WHT
   };

/*********************************************************/
void init_graphics(void)
{
   ULONG i, j;

   GIL2VFX_init();
   
   for (i=0;i<256;i++)
      window_owner[i] = -1;

   GIL2VFX_wipe_window(PAGE2,0);

   txtbuf[sizeof(txtbuf)-1] = 0x69;    // constants for integrity checking
   txtbuf[sizeof(txtbuf)-2] = 0x77;

   for (i=0;i<NTW;i++)
      {
      tw[i].font = NULL;
      tw[i].window = 0;
      tw[i].htab = tw[i].vtab = 0;
      tw[i].delay = 0;
      tw[i].continueFunction = NULL;
      tw[i].txtbuf = txtbuf;
      tw[i].txtpnt = txtbuf;
      tw[i].justify = J_LEFT;

      for (j=0; j<256; j++)
         tw[i].lookaside[j] = j;

      tw_refresh[i] = -1;

      tw[i].font = (FONT *)-1;
      }
}

/*********************************************************/
void shutdown_graphics(void)
{
   GIL2VFX_shutdown_driver();
}

/*********************************************************/
//
// Release all windows assigned to owner, where owner refers to
// any program or entity object
//
// If owner = -1, release all assigned windows except PAGE1,
// PAGE2, and windows assigned to program objects
//
/*********************************************************/

void release_owned_windows(LONG owner)
{
   LONG i;

   if (owner == -1)
      {
      for (i=PAGE2+1;i<256;i++)
         if ((window_owner[i] != -1) &&
            (window_owner[i] < NUM_ENTITIES))
            {
            window_owner[i] = -1;
            GIL2VFX_release_window(i);
            }
      }
   else
      {
      for (i=PAGE2+1;i<256;i++)
         if (window_owner[i] == owner)
            {
            window_owner[i] = -1;
            GIL2VFX_release_window(i);
            }
      }
}

/*********************************************************/

void draw_dot(LONG argcnt, ULONG page, ULONG x, ULONG y, ULONG color)

{
   GIL2VFX_draw_dot(lastg_p=page, lastg_x=x, lastg_y=y, color);
}

/*********************************************************/

void draw_line(LONG argcnt, ULONG page, ULONG x1, ULONG y1, ULONG x2, ULONG y2,

   ULONG color)
{
   GIL2VFX_draw_line(lastg_p=page, x1, y1, lastg_x=x2, lastg_y=y2, color);
}

/*********************************************************/

void line_to(LONG argcnt, ULONG x, ULONG y, ULONG color, ...)

{
   ULONG i;
   va_list argptr;
   ULONG cx,cy,cc,lx,ly;

   va_start(argptr,argcnt);

   for (i=0;i<(ULONG)argcnt;i+=3)
      {
      cx = va_arg(argptr,ULONG);
      cy = va_arg(argptr,ULONG);
      cc = va_arg(argptr,ULONG);

      GIL2VFX_draw_line(lastg_p,lastg_x,lastg_y,lx=cx,ly=cy,cc);

      lastg_x = lx;
      lastg_y = ly;
      }

   va_end(argptr);
}

/*********************************************************/

void draw_rectangle(LONG argcnt, ULONG wndnum, LONG x1, LONG y1, LONG x2,
                          LONG y2, ULONG color)

{
   GIL2VFX_draw_rect(wndnum,x1,y1,x2,y2,color);
}

/*********************************************************/

void fill_rectangle(LONG argcnt, ULONG wndnum, LONG x1, LONG y1, LONG x2,
                          LONG y2, ULONG color)

{
   GIL2VFX_fill_rect(wndnum,x1,y1,x2,y2,color);
}

/*********************************************************/

void hash_rectangle(LONG argcnt, ULONG wndnum, LONG x1, LONG y1, LONG x2,
                          LONG y2, ULONG color)

{
   GIL2VFX_hash_rect(wndnum,x1,y1,x2,y2,color);
}

/*********************************************************/

ULONG get_bitmap_height(LONG argcnt, ULONG table, ULONG number) {
	Resources &res = *_vm->_resources;
	HRES handle;
   ULONG h;

   handle = res.get_resource_handle(table,DA_DEFAULT);

   res.lock(handle);

   h = GIL2VFX_get_bitmap_height(res.addr(handle), number);

   res.unlock(handle);

   return h;
}

/*********************************************************/

void draw_bitmap(LONG argcnt, ULONG page, ULONG table, ULONG number,
                       LONG x, LONG y, ULONG scale, ULONG flip,
                       ULONG fade_table, ULONG fade_level) {
	Resources &res = *_vm->_resources;
	HRES handle;
   static HRES last_handle = 0;
   static ULONG last_table;
   UBYTE *lookaside;

   if (table == last_table)
      handle = last_handle;
   else
      {
      handle = res.get_resource_handle(table,DA_DEFAULT);
      last_handle = handle;
      last_table = table;
      }

   if ((fade_level > 10) && (!scale))
      scale = 256;

   lookaside = fade_tables[fade_table][fade_level]-first_color[fade_table];

   res.lock(handle);

   GIL2VFX_draw_bitmap(page,x,y,flip,scale,lookaside,(UBYTE *)res.addr(handle),number);

   res.unlock(handle);
}

/*********************************************************/
//
// Fill a 4-element array of words with the coordinates of the rectangle
// enclosing the visible part of a bitmap
//
// Return 0 if bitmap empty
//
// Determines array offset in instance, in case resource load causes
// instance to move in memory
//
// WARNING: The array must not be of automatic or external scope!
//
/*********************************************************/

ULONG visible_bitmap_rect(LONG argcnt, LONG x, LONG y, ULONG flip,
	ULONG table, ULONG number, WORD *array) {
	Interpreter &interp = *_vm->_interpreter;
	Resources &res = *_vm->_resources;
	HRES handle;
	ULONG array_offset;
	BYTE *new_array;
	WORD bounds[4];
	ULONG result;

	array_offset = (ULONG)array - (ULONG)res.addr(objlist[interp._currentThis]);

	handle = res.get_resource_handle(table, DA_DEFAULT);

	res.lock(handle);

	result = GIL2VFX_visible_bitmap_rect(x, y, flip, (UBYTE *)res.addr(handle), number, bounds);

	new_array = (BYTE *)add_ptr(res.addr(objlist[interp._currentThis]), array_offset);

	far_memmove(new_array, bounds, sizeof(bounds));

	res.unlock(handle);

	return result;
}

/*********************************************************/
//
// Set a palette color range from a palette resource file
//
// Copy "fade tables" from resource file; dynamically build 
// special shade tables for blue/green/red/gray/white/etc. effects
//
/*********************************************************/

void set_palette(LONG argcnt, ULONG region, ULONG resource) {
	Resources &res = *_vm->_resources;
	HRES handle;
	PAL_HDR *PHDR;
	RGB *array;
	LONG i, j, k, n, f, m, dm, d;
	UBYTE *fade;

	handle = res.get_resource_handle(resource, DA_DEFAULT);
	res.lock(handle);

	PHDR = (PAL_HDR *)res.addr(handle);

	if ((region == PAL_FIXED) ||
		(region == PAL_WALLS) ||
		(region == PAL_M1) ||
		(region == PAL_M2))
	{
		for (i = 0; i < 11; i++)
		{
			fade = (UBYTE *)add_ptr(PHDR, PHDR->fade[i]);

			for (j = 0; j < PHDR->ncolors; j++)
				fade_tables[region][i][j] = first_color[region] + fade[j];
		}
	}

	array = (RGB *)add_ptr(PHDR, PHDR->RGB);

	for (i = 0; i < PHDR->ncolors; i++)
	{
		VFX_DAC_write(i + first_color[region], &array[i]);

		if (!(i & 0x0f)) VFX_wait_vblank_leading();
	}

	switch (region)
	{
	case PAL_FIXED:

		for (n = 0, i = BLU_BEG; n < BLU_NUM; n++, i++)
			blu_inten[n] = array[i].r + array[i].g + array[i].b;
		for (n = 0, i = RED_BEG; n < RED_NUM; n++, i++)
			red_inten[n] = array[i].r + array[i].g + array[i].b;
		for (n = 0, i = GRN_BEG; n < GRN_NUM; n++, i++)
			grn_inten[n] = array[i].r + array[i].g + array[i].b;
		for (n = 0, i = GRY_BEG; n < GRY_NUM; n++, i++)
			gry_inten[n] = array[i].r + array[i].g + array[i].b;
		for (n = 0, i = BRN_BEG; n < BRN_NUM; n++, i++)
			brn_inten[n] = array[i].r + array[i].g + array[i].b;

		f = first_color[region];
		n = num_colors[region];
		for (i = 0; i < n; i++)
		{
			j = array[i].r + array[i].g + array[i].b;

			m = 0;
			dm = 32767;
			for (k = 0; k < BLU_NUM; k++)
			{
				d = abs(j - blu_inten[k]);
				if (d < dm)
				{
					dm = d;
					m = k;
				}
			}

			F_blu[i] = BLU_BEG + m;

			m = 0;
			dm = 32767;
			for (k = 0; k < GRN_NUM; k++)
			{
				d = abs(j - grn_inten[k]);
				if (d < dm)
				{
					dm = d;
					m = k;
				}
			}

			F_grn[i] = GRN_BEG + m;

			m = 0;
			dm = 32767;
			for (k = 0; k < RED_NUM; k++)
			{
				d = abs(j - red_inten[k]);
				if (d < dm)
				{
					dm = d;
					m = k;
				}
			}

			F_red[i] = RED_BEG + m;

			m = 0;
			dm = 32767;
			for (k = 0; k < GRY_NUM; k++)
			{
				d = abs(j - gry_inten[k]);
				if (d < dm)
				{
					dm = d;
					m = k;
				}
			}

			F_gry[i] = GRY_BEG + m;
		}
		break;

	case PAL_M1:
	case PAL_M2:

		f = first_color[region];
		n = num_colors[region];
		for (i = 0; i < n; i++)
		{
			j = array[i].r + array[i].g + array[i].b;

			m = 0;
			dm = 32767;
			for (k = 0; k < BRN_NUM; k++)
			{
				d = abs(j - brn_inten[k]);
				if (d < dm)
				{
					dm = d;
					m = k;
				}
			}

			if (region == PAL_M1)
				M1_brn[i] = BRN_BEG + m;
			else
				M2_brn[i] = BRN_BEG + m;

			m = 0;
			dm = 32767;
			for (k = 0; k < GRY_NUM; k++)
			{
				d = abs(j - gry_inten[k]);
				if (d < dm)
				{
					dm = d;
					m = k;
				}
			}

			if (region == PAL_M1)
				M1_gry[i] = GRY_BEG + m;
			else
				M2_gry[i] = GRY_BEG + m;

			m = 0;
			dm = 32767;
			for (k = 0; k < GRN_NUM; k++)
			{
				d = abs(j - grn_inten[k]);
				if (d < dm)
				{
					dm = d;
					m = k;
				}
			}

			if (region == PAL_M1)
				M1_grn[i] = GRN_BEG + m;
			else
				M2_grn[i] = GRN_BEG + m;

			m = 0;
			dm = 32767;
			for (k = 0; k < BLU_NUM; k++)
			{
				d = abs(j - blu_inten[k]);
				if (d < dm)
				{
					dm = d;
					m = k;
				}
			}

			if (region == PAL_M1)
				M1_blu[i] = BLU_BEG + m;
			else
				M2_blu[i] = BLU_BEG + m;

			if (region == PAL_M1)
				M1_wht[i] = FIX_WHT;
			else
				M2_wht[i] = FIX_WHT;
		}
		break;
	}

	res.unlock(handle);
}

/*********************************************************/
void wait_vertical_retrace(void)
{
   VFX_wait_vblank_leading();
}

/*********************************************************/

ULONG read_palette(LONG argcnt, ULONG regnum) {
   RGB triplet;
   ULONG val;

   VFX_DAC_read(regnum, &triplet);
   val = ((ULONG) triplet.r << 12) | ((ULONG) triplet.g << 6) | ((ULONG) triplet.b);

   return val;
}

/*********************************************************/

void write_palette(LONG argcnt, ULONG regnum, ULONG value)

{
   RGB triplet;

   triplet.r = value >> 12;
   triplet.g = (value >> 6) & 63L;
   triplet.b = value & 63L;

   VFX_DAC_write(regnum, &triplet);
}

/*********************************************************/

void pixel_fade(LONG argcnt, ULONG src_wnd, ULONG dest_wnd, ULONG intervals)

{
   GIL2VFX_pixel_fade(src_wnd,dest_wnd,intervals);
}

/*********************************************************/

void color_fade(LONG argcnt, ULONG src_wnd, ULONG dest_wnd)

{
   GIL2VFX_color_fade(src_wnd,dest_wnd);
}

/*********************************************************/

void light_fade(LONG argcnt, ULONG src_wnd, ULONG color)

{
   GIL2VFX_light_fade(src_wnd,color);
}

/*********************************************************/

ULONG assign_window(LONG argcnt, ULONG owner, ULONG x1, ULONG y1,
                          ULONG x2, ULONG y2)

{
   LONG window;

   window = GIL2VFX_assign_window(x1,y1,x2,y2);

   window_owner[window] = owner;

   return window;
}

/*********************************************************/

ULONG assign_subwindow(LONG argcnt, ULONG owner, ULONG parent,
                             ULONG x1, ULONG y1, ULONG x2, ULONG y2)

{
   LONG window;

   window = GIL2VFX_assign_subwindow(parent,x1,y1,x2,y2);

   window_owner[window] = owner;

   return window;
}

/*********************************************************/

void release_window(LONG argcnt, ULONG window)

{
   GIL2VFX_release_window(window);
   window_owner[window] = -1;
}

/*********************************************************/

ULONG get_x1(LONG argcnt, ULONG window)

{
   return GIL2VFX_get_x1(window);
}

/*********************************************************/

ULONG get_x2(LONG argcnt, ULONG window)

{
   return GIL2VFX_get_x2(window);
}

/*********************************************************/

ULONG get_y1(LONG argcnt, ULONG window)

{
   return GIL2VFX_get_y1(window);
}

/*********************************************************/

ULONG get_y2(LONG argcnt, ULONG window)

{
   return GIL2VFX_get_y2(window);
}

/*********************************************************/

void set_x1(LONG argcnt, ULONG window, ULONG x1)

{
   GIL2VFX_set_x1(window,x1);
}

/*********************************************************/

void set_x2(LONG argcnt, ULONG window, ULONG x2)

{
   GIL2VFX_set_x2(window,x2);
}

/*********************************************************/

void set_y1(LONG argcnt, ULONG window, ULONG y1)

{
   GIL2VFX_set_y1(window,y1);
}

/*********************************************************/

void set_y2(LONG argcnt, ULONG window, ULONG y2)

{
   GIL2VFX_set_y2(window,y2);
}

/*********************************************************/

void wipe_window(LONG argcnt, ULONG window, ULONG color)

{
   GIL2VFX_wipe_window(window,color);
}

/*********************************************************/

void text_window(LONG argcnt, ULONG wndnum, ULONG wnd)

{
   tw[wndnum].window = wnd;
}

/*********************************************************/
//
// Establish font name and justification mode for a given text window
//
// The font is guaranteed to remain locked in memory until
// another text_style() call is issued for the window
//
/*********************************************************/

void text_style(LONG argcnt, ULONG wndnum, ULONG font,
	ULONG justify) {
	Resources &res = *_vm->_resources;
	HRES hfont;
	FONT *vfont;

	tw[wndnum].justify = justify;

	hfont = (HRES)tw[wndnum].font;

	if (hfont != HRES_NULL)
	{
		res.unlock(hfont);
	}

	tw[wndnum].font = (FONT *)res.get_resource_handle(font, DA_DEFAULT);

	res.lock((HRES)tw[wndnum].font);

	vfont = (FONT *)res.addr((HRES)tw[wndnum].font);

	tw[wndnum].lookaside[vfont->font_background] = 255;
}

/*********************************************************/

void text_xy(LONG argcnt, ULONG wndnum, ULONG htab, ULONG vtab)

{
   tw[wndnum].htab = htab;
   tw[wndnum].vtab = vtab;
}

/*********************************************************/

LONG get_text_x(LONG argcnt, ULONG wndnum)

{
   return tw[wndnum].htab;
}

/*********************************************************/

LONG get_text_y(LONG argcnt, ULONG wndnum)

{
   return tw[wndnum].vtab;
}

/*********************************************************/

void home(LONG argcnt, ULONG wndnum)

{
	HRES hfont;

   hfont = (HRES) tw[wndnum].font;
   tw[wndnum].font = (FONT *)Resources::addr(hfont);

   GIL2VFX_select_text_window(&tw[wndnum]);
   GIL2VFX_home();

   tw[wndnum].font = (FONT *) hfont;
}

/*********************************************************/

void text_color(LONG argcnt, ULONG wndnum, ULONG current, ULONG newColor)

{
	HRES hfont;

   hfont = (HRES) tw[wndnum].font;
   tw[wndnum].font = (FONT *)Resources::addr(hfont);

   GIL2VFX_select_text_window(&tw[wndnum]);
   GIL2VFX_remap_font_color(current, newColor);

   tw[wndnum].font = (FONT *)hfont;
}

/*********************************************************/

void text_refresh_window(LONG argcnt, ULONG wndnum, LONG wnd)

{
   tw_refresh[wndnum] = wnd;
}

/*********************************************************/
//
// Main function for AESOP text output
//
// Warning: This routine stores the font resource's address --
// update it after any resource loads!
//
/*********************************************************/


void vsprint(LONG argcnt, ULONG wndnum, const char *format, va_list argptr)

{
	Resources &res = *_vm->_resources;
	HRES hfont, str;
	BYTE c, *s;
	BYTE buff[32];
	const char *p;

	hfont = (HRES)tw[wndnum].font;
	tw[wndnum].font = (FONT *)res.addr(hfont);

	GIL2VFX_select_text_window(&tw[wndnum]);

	GIL2VFX_print(BUF, "");
	txtbuf[0] = 0;       // bug in GIL?  GIL_print("") above should do this!

	p = format;

	while ((c = *p++) != 0)
	{
		if (c != '%')
		{
			GIL2VFX_print(APP, "%c", c);
			continue;
		}

		switch (c = (*p++))
		{
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			GIL2VFX_remap_font_color(15, text_colors[c - '0']);
			break;

		case 'd':
			GIL2VFX_print(APP, "%s", ltoa(va_arg(argptr, LONG), (char *)buff, 10));
			break;

		case 'u':
			GIL2VFX_print(APP, "%s", ultoa(va_arg(argptr, ULONG), (char *)buff, 10));
			break;

		case 'x':
		case 'X':
			GIL2VFX_print(APP, "%s", ltoa(va_arg(argptr, LONG), (char *)buff, 16));
			break;

		case 's':
			str = res.get_resource_handle(va_arg(argptr, ULONG),
				DA_DEFAULT);

			res.lock(str);

			s = (BYTE *)res.addr(str);

			switch (*(UWORD *)s)
			{
			case ':S':
				GIL2VFX_print(APP, "%s", &s[2]);
				break;

			default:
				abend(MSG_SRRV);
			}

			res.unlock(str);

			tw[wndnum].font = (FONT *)res.addr(hfont);
			break;

		case 'a':
			GIL2VFX_print(APP, (const char *)va_arg(argptr, BYTE *));
			break;

		case 'c':
			GIL2VFX_print(APP, "%c", (UBYTE)(va_arg(argptr, ULONG) & 0xffL));
			break;
		}
	}

	if ((txtbuf[sizeof(txtbuf) - 1] != 0x69)
		||
		(txtbuf[sizeof(txtbuf) - 2] != 0x77))
		abend(MSG_TBO);

	GIL2VFX_print_buffer(0);

	tw[wndnum].font = (FONT *)hfont;

	if (tw_refresh[wndnum] != -1)
		refresh_window(0, tw[wndnum].window, tw_refresh[wndnum]);
}

/*********************************************************/
//
// Warning: Strings > 256 characters long must _not_ contain
// any '%s' specifiers, since these cause resource loads to 
// occur in vsprint() which may move the original print() format
// specifier string in memory.
//
// Excessively long strings with '%s' specifiers should be broken up
// into smaller ones -- the buffering action of this routine is for
// convenience in printing short game text strings only, rather than
// for outtakes, etc.
//
/*********************************************************/

void print(LONG argcnt, ULONG wndnum, ULONG format, ...)
{
	Resources &res = *_vm->_resources;
   va_list argptr;
   HRES hstring;
   const char *p;
   UWORD *w;

   hstring = res.get_resource_handle(format,DA_DEFAULT);

   res.lock(hstring);

   p = (const char *)res.addr(hstring);

   w = (UWORD *) p;

   switch (*w)
      {
      case ':S':
         p += 2;
         break;

      default:
         abend(MSG_SRRP);
      }

   va_start(argptr,format);

   if (strlen(p) < 256)
      {
      strcpy(strbuf,p);
      p = strbuf;
      }

   vsprint(argcnt,wndnum,p,argptr);

   va_end(argptr);

   res.unlock(hstring);
}

/*********************************************************/
//
// String print -- allows use of literal ASCII byte array as            
// format specifier, instead of string resource
//
// See note for print() above
//
/*********************************************************/

void sprint(LONG argcnt, ULONG wndnum, const char *format, ...)
{
   va_list argptr;
   const char *p;

   p = format;

   va_start(argptr,format);

   if (strlen(p) < 256)
      {
      strcpy(strbuf,p);
      p = strbuf;
      }

   vsprint(argcnt,wndnum,p,argptr);

   va_end(argptr);
}

/*********************************************************/
//
// Diagnostic string print -- sets red-on-transparent color for TW_MAIN,
// and prints string
//
/*********************************************************/

void dprint(LONG argcnt, const char *format, ...)
{
   va_list argptr;
   const char *p;

   p = format;

   va_start(argptr,format);

   text_color(0,0,15,DK_RED);
   text_color(0,0,0,255);

   vsprint(argcnt,0,p,argptr);

   va_end(argptr);
}

/*********************************************************/
void crout(LONG argcnt, ULONG wndnum)
{
   sprint(argcnt,wndnum,"\n");
}

/*********************************************************/

ULONG char_width(LONG argcnt, ULONG wndnum, ULONG ch) {
	HRES hfont;
   ULONG w;

   hfont = (HRES) tw[wndnum].font;
   tw[wndnum].font = (FONT *)Resources::addr(hfont);

   GIL2VFX_select_text_window(&tw[wndnum]);
   w = GIL2VFX_char_width(ch);

   tw[wndnum].font = (FONT *) hfont;

   return w;
}

/*********************************************************/

ULONG font_height(LONG argcnt, ULONG wndnum) {
	HRES hfont;
	ULONG w;

	hfont = (HRES)tw[wndnum].font;

	w = ((FONT *)(Resources::addr(hfont)))->char_height;

	return w;
}

/*********************************************************/

void solid_bar_graph(LONG argcnt, LONG x0, LONG y0, LONG x1, LONG y1,
                           ULONG lb_border, ULONG tr_border, ULONG bkgnd,
                           ULONG grn, ULONG yel, ULONG red, LONG val,
                           LONG min, LONG crit, LONG max) {
   LONG btop,bbtm,blft,brgt,grayx;
   LONG range,point,width;
   LONG color;

   GIL2VFX_draw_line(PAGE2,x0,y0,x0,y1,lb_border);
   GIL2VFX_draw_line(PAGE2,x0,y1,x1,y1,lb_border);
   GIL2VFX_draw_line(PAGE2,x1,y1-1,x1,y0,tr_border);
   GIL2VFX_draw_line(PAGE2,x1,y0,x0+1,y0,tr_border);

   btop = y0+1;
   bbtm = y1-1;

   blft = x0+1;
   brgt = x1-1;

   width = brgt - blft;

   if (val > max)
      val = max;
   else if (val < min)
      val = min;

   range = max - min;
   point = val - min;

   grayx = blft + (point * width / range);

   if (grayx != brgt)
      GIL2VFX_fill_rect(PAGE2,grayx,btop,brgt,bbtm,bkgnd);

   if (val <= crit)
      color = red;
   else
      color = (val * 3 >= max) ? grn : yel;

   if ((val != min) && (grayx == blft))
      grayx = blft+1;

   if (grayx != blft)
      GIL2VFX_fill_rect(PAGE2,blft,btop,grayx,bbtm,color);
}

void aprint(LONG argcnt, const char *format, ...) {
	va_list args;
	char buffer[100];

	va_start(args, format);
	vsnprintf(buffer, 100, format, args);
	va_end(args);

	debug(buffer);
}

} // End of namespace Aesop
