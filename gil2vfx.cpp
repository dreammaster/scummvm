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
//#include "aesop/dll.h"
#include "aesop/gil2vfx.h"
#include "aesop/gil2vfxa.h"
#include "aesop/defs.h"
#include "aesop/rtmsg.h"
#include "aesop/rtsystem.h"
#include "aesop/mouse.h"
#include "aesop/resources.h"
#include "aesop/stubs.h"

namespace Aesop {

#define  MAX_WINDOWS    50
#define  MAX_PANES      250

#define  FADE_INTERVALS 20

WINDOW   windows[MAX_WINDOWS];
PANE     panes[MAX_WINDOWS+MAX_PANES];
BYTE     *bitmap_buffer;

bool     gil2vfx_active = false;

TEXTWINDOW *twptr;

extern VFX_DESC *VFX;
extern BYTE *pathname;

extern LONG eop_reached;
extern LONG pending_flag;

void GIL2VFX_copy_window(ULONG src, ULONG dst)
{
   if (!gil2vfx_active)
      return;

   VFX_pane_copy(&panes[src],0,0,&panes[dst],0,0,NO_COLOR);
}


LONG GIL2VFX_assign_window(LONG x1, LONG y1, LONG x2, LONG y2)
{
   LONG i;

   for (i=0;i<MAX_WINDOWS;i++)
      {
      if (windows[i].buffer == NULL)
         {
         windows[i].buffer = (byte *)mem_alloc((x2-x1+1) * (y2-y1+1));
         windows[i].left = x1;
         windows[i].top = y1;
         windows[i].right = x2;
         windows[i].bottom = y2;

         panes[i].window = &windows[i];
         panes[i].left = x1;
         panes[i].top = y1;
         panes[i].right = x2;
         panes[i].bottom = y2;

         return i;
         }
      }

   abend(MSG_TFW);
   return -1;
}

void GIL2VFX_init()
{
   //LONG dummy;

   //void *DLL,*drvr;
   //LONG w,h;

   //
   // Load the driver
   //
   /*
   DLL = FILE_read("MCGA.DLL",NULL);
   if (DLL == NULL)
      {
      error("Missing or invalid 386FX driver\n");
      }

   drvr = DLL_load(DLL,DLLMEM_ALLOC | DLLSRC_MEM,NULL);
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
   */
   VFX = VFX_describe_driver();

//   w = VFX->scrn_width;
//   h = VFX->scrn_height;

   //
   // Turn graphics on; set up window and pane
   //

   VFX_init_driver();

   gil2vfx_active = true;

   bitmap_buffer = (BYTE *)mem_alloc(320 * 200);

   windows[0].buffer = (byte *)0x0a0000;       // Page 1 = 0
   windows[0].left = 0;
   windows[0].top = 0;
   windows[0].right = 319;
   windows[0].bottom = 199;

   panes[0].window = &windows[0];
   panes[0].left = 0;
   panes[0].top = 0;
   panes[0].right = 319;
   panes[0].bottom = 199;

   GIL2VFX_assign_window(0,0,319,199);  // Page 2 = 1

}

void GIL2VFX_shutdown_driver()
{
   ULONG i;

   if (!gil2vfx_active)
      return;

   gil2vfx_active = false;

   for (i=0;i<MAX_WINDOWS;i++)
      {
      GIL2VFX_release_window(i);
      }

   mem_free(bitmap_buffer);

   VFX_shutdown_driver();
}


void GIL2VFX_release_window(ULONG wnd)
{
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

LONG GIL2VFX_assign_subwindow(ULONG wnd,LONG x1, LONG y1, LONG x2, LONG y2)
{
   ULONG i;

   for (i=MAX_WINDOWS;i<MAX_WINDOWS+MAX_PANES;i++)
      {
      if (panes[i].window == NULL)
         {
         if (wnd < MAX_WINDOWS)
            panes[i].window = &windows[wnd];
         else
            panes[i].window = panes[wnd].window;

         panes[i].left = x1;
         panes[i].top = y1;
         panes[i].right = x2;
         panes[i].bottom = y2;

         return i;
         }
      }

   abend(MSG_TFSW);

   return -1;
}

void GIL2VFX_release_subwindow(ULONG wnd)
{
   panes[wnd].window = NULL;
}

LONG GIL2VFX_get_bitmap_width(void *shape_table, LONG shape_num)
{
   return (VFX_shape_bounds(shape_table, shape_num) >> 16);
}

LONG GIL2VFX_get_bitmap_height(void *shape_table, LONG shape_num)
{
   return ((LONG) (WORD) VFX_shape_bounds(shape_table, shape_num) );
}

LONG GIL2VFX_visible_bitmap_rect(LONG x1, LONG y1, LONG mirror,
                                 UBYTE *shapes, LONG shape_num,WORD *bounds)
{
   LONG rectangle[4];
   LONG bm_width, bm_height;

   VFX_shape_visible_rectangle(shapes, shape_num, x1, y1, mirror, rectangle);

   if (mirror)
      {
      bm_width = VFX_shape_bounds(shapes,shape_num);
      bm_height = (LONG) (WORD)bm_width;
      bm_width = bm_width >> 16;
      }


   if (mirror & X_MIRROR)
      {
      bounds[0] = (WORD) (bm_width + rectangle[0]);
      bounds[2] = (WORD) (bm_width + rectangle[2]);
      }
   else
      {
      bounds[0] = (WORD) rectangle[0];
      bounds[2] = (WORD) rectangle[2];
      }
   
   if (mirror & Y_MIRROR)
      {
      bounds[1] = (WORD) (bm_height + rectangle[1]);
      bounds[3] = (WORD) (bm_height + rectangle[3]);
      }
   else
      {
      bounds[1] = (WORD) rectangle[1];
      bounds[3] = (WORD) rectangle[3];
      }
   

   bounds[0] = (bounds[0] > panes[0].left) ? bounds[0] : (WORD) panes[0].left;
   bounds[1] = (bounds[1] > panes[0].top) ? bounds[1] : (WORD) panes[0].top;

   bounds[2] = (bounds[2] < panes[0].right) ? bounds[2] : (WORD) panes[0].right;
   bounds[3] = (bounds[3] < panes[0].bottom) ? bounds[3] : (WORD) panes[0].bottom;

   if ( (bounds[0] > bounds[2]) || (bounds[1] > bounds[3]) )
      return 0;            // Nothing visible
   else
      return 1;
}

void GIL2VFX_draw_bitmap(LONG wnd,LONG x, LONG y, LONG mirror, LONG scale,
   UBYTE *fade_table, UBYTE *shapes, LONG shape_num)
{
   LONG xp = x - panes[wnd].left;
   LONG yp = y - panes[wnd].top;
   LONG x_scale, y_scale;
   LONG flags;
   LONG xs,ys;

   if (gil2vfx_active != true)
      return;

   if ( (scale == 0) && (mirror == NO_MIRROR) )
      {
      VFX_shape_draw(&panes[wnd], shapes, shape_num, xp, yp);
      }
   else
      {
      x_scale = ((scale) ? (scale << 8) : 0x10000);
      y_scale = ((scale) ? (scale << 8) : 0x10000);

      if (x_scale != 0x10000 || y_scale != 0x10000)
         {
         VFX_fixed_mul( VFX_shape_bounds(shapes, shape_num) & 0xffff0000,
                        0x10000-x_scale, &xs);
         VFX_fixed_mul( VFX_shape_bounds(shapes, shape_num) << 16,
                        0x10000-y_scale, &ys);

         if (mirror & X_MIRROR) xs=-xs;
         if (mirror & Y_MIRROR) ys=-ys;

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
            yp += ((LONG)(WORD)VFX_shape_bounds(shapes, shape_num));
            break;
         case XY_MIRROR:
            x_scale = -x_scale;
            y_scale = -y_scale;
            xp += (VFX_shape_bounds(shapes, shape_num) >> 16)-1;
            yp += ((LONG)(WORD)VFX_shape_bounds(shapes, shape_num))-1;
            break;
         case NO_MIRROR:
         default:
            break;
         }

      if (fade_table != NULL && scale != 0)
         {
         VFX_shape_lookaside(fade_table);
         flags = ST_XLAT;
         }
      else
         flags = 0;

      VFX_shape_transform(&panes[wnd], shapes, shape_num, xp, yp,
                          bitmap_buffer, 0, x_scale, y_scale, flags);
      }
}

LONG GIL2VFX_get_x1(ULONG wnd)
{
   if (panes[wnd].window != NULL)
      return panes[wnd].left;
   else
      return 0;
}
LONG GIL2VFX_get_y1(ULONG wnd)
{
   if (panes[wnd].window != NULL)
      return panes[wnd].top;
   else
      return 0;
}
LONG GIL2VFX_get_x2(ULONG wnd)
{
   if (panes[wnd].window != NULL)
      return panes[wnd].right;
   else
      return 0;
}
LONG GIL2VFX_get_y2(ULONG wnd)
{
   if (panes[wnd].window != NULL)
      return panes[wnd].bottom;
   else
      return 0;
}

void GIL2VFX_set_x1(ULONG wnd, LONG val)
{
   panes[wnd].left = val;
}
void GIL2VFX_set_y1(ULONG wnd, LONG val)
{
   panes[wnd].top = val;
}
void GIL2VFX_set_x2(ULONG wnd, LONG val)
{
   panes[wnd].right = val;
}
void GIL2VFX_set_y2(ULONG wnd, LONG val)
{
   panes[wnd].bottom = val;
}

void GIL2VFX_wipe_window(LONG wnd, LONG color)
{
   if (gil2vfx_active != true)
      return;

   VFX_pane_wipe(&panes[wnd], color);
}

void GIL2VFX_draw_dot(LONG wnd, LONG x, LONG y, LONG color)
{
   LONG xp = x - panes[wnd].left;
   LONG yp = y - panes[wnd].top;

   if (gil2vfx_active != true)
      return;

   VFX_pixel_write(&panes[wnd], xp, yp, (UBYTE) color);
}

LONG GIL2VFX_read_dot(LONG wnd, LONG x, LONG y)
{
   LONG xp = x - panes[wnd].left;
   LONG yp = y - panes[wnd].top;

   if (gil2vfx_active != true)
      return 0;

   return VFX_pixel_read(&panes[wnd], xp, yp);
}

void GIL2VFX_draw_line(LONG wnd,LONG x1, LONG y1, LONG x2, LONG y2,
      LONG color)
{
   LONG x1p = x1 - panes[wnd].left;
   LONG y1p = y1 - panes[wnd].top;
   LONG x2p = x2 - panes[wnd].left;
   LONG y2p = y2 - panes[wnd].top;

   if (gil2vfx_active != true)
      return;

   VFX_line_draw(&panes[wnd], x1p, y1p, x2p, y2p, LD_DRAW, color);
}

void GIL2VFX_draw_rect(LONG wnd,LONG x1,LONG y1,LONG x2,LONG y2,LONG color)
{
   LONG x1p = x1 - panes[wnd].left;
   LONG y1p = y1 - panes[wnd].top;
   LONG x2p = x2 - panes[wnd].left;
   LONG y2p = y2 - panes[wnd].top;

   if (gil2vfx_active != true)
      return;

   VFX_line_draw(&panes[wnd], x1p, y1p, x2p, y1p, LD_DRAW, color);
   VFX_line_draw(&panes[wnd], x2p, y1p, x2p, y2p, LD_DRAW, color);
   VFX_line_draw(&panes[wnd], x2p, y2p, x1p, y2p, LD_DRAW, color);
   VFX_line_draw(&panes[wnd], x1p, y2p, x1p, y1p, LD_DRAW, color);
}

void GIL2VFX_fill_rect(LONG wnd,LONG x1,LONG y1,LONG x2,LONG y2,LONG color)
{
   PANE rpane;

   if (gil2vfx_active != true)
      return;

   rpane.window = panes[wnd].window;
   rpane.left = x1;
   rpane.top = y1;
   rpane.right = x2;
   rpane.bottom = y2;

   VFX_pane_wipe(&rpane, color);
}

void GIL2VFX_hash_rect(LONG wnd,LONG x1,LONG y1,LONG x2,LONG y2,LONG color)
{
   LONG x1p = x1 - panes[wnd].left;
   LONG y1p = y1 - panes[wnd].top;
   LONG x2p = x2 - panes[wnd].left;
   LONG y2p = y2 - panes[wnd].top;

   if (gil2vfx_active != true)
      return;

   VFX_rectangle_hash(&panes[wnd], x1p, y1p, x2p, y2p, color);
}

void GIL2VFX_light_fade(LONG src_wnd, LONG color)
{
   RGB palette[256], clr;
   LONG i;

   if (gil2vfx_active != true)
      return;

   VFX_DAC_read(color, &clr);

   for (i=0;i<256;i++)
      palette[i] = clr;

   VFX_window_fade(panes[src_wnd].window, palette, FADE_INTERVALS);
}

void GIL2VFX_color_fade(LONG src_wnd, LONG dst_wnd)
{
   RGB palette[256], clr;
   ULONG colors[256], num_colors;
   ULONG i;

   if (gil2vfx_active != true)
      return;

   num_colors = VFX_color_scan(&panes[src_wnd], colors);   

   for (i=0;i<256;i++)
      {
      VFX_DAC_read(i, &palette[i]);
      }

   clr = palette[*(panes[dst_wnd].window->buffer)];
   for (i=0;i<num_colors;i++)
      {
      VFX_DAC_write(colors[i], &clr);
      }

   VFX_pane_copy(&panes[src_wnd],0,0,&panes[dst_wnd],0,0,NO_COLOR);

   VFX_window_refresh(&windows[0],0,0,VFX->scrn_width-1,VFX->scrn_height-1);

   VFX_window_fade(panes[dst_wnd].window, palette, FADE_INTERVALS);
}

void GIL2VFX_pixel_fade(LONG src_wnd, LONG dest_wnd, LONG intervals)
{
   if (gil2vfx_active != true)
      return;

   VFX_pixel_fade(&panes[src_wnd], &panes[dest_wnd], intervals);
}

void GIL2VFX_select_text_window(TEXTWINDOW *tw)
{
   twptr = tw;
}

LONG GIL2VFX_char_width(LONG ch)
{
   return VFX_character_width(twptr->font, ch);
}

void GIL2VFX_home(void)
{
   if (gil2vfx_active != true)
      return;

   VFX_pane_wipe(&panes[twptr->window],
      ((FONT *)(twptr->font))->font_background);

   twptr->htab = panes[twptr->window].left;
   twptr->vtab = panes[twptr->window].top;
}

void GIL2VFX_remap_font_color(LONG current, LONG newColor)
{
	twptr->lookaside[current] = newColor;
}


LONG GIL2VFX_test_overlap(LONG wnd, LONG x1, LONG y1, UBYTE *shapes, LONG shape_num)
{
   LONG x2 = (x1 + (VFX_shape_resolution(shapes, shape_num) >> 16)    );
   LONG y2 = (y1 + ((LONG)(WORD)VFX_shape_resolution(shapes, shape_num)));

   if ( (x1 <= panes[wnd].right) && (x2 >= panes[wnd].left) &&
        (y1 <= panes[wnd].bottom) && (y2 >= panes[wnd].top) )
      return 1;
   else
      return 0;
}

void GIL2VFX_print(LONG operation, const char *format, ...)
{
   va_list  arglist;
   LONG     cw;

   va_start(arglist,format);
   
   if (operation == BUF)
      {
      cw = vsprintf(twptr->txtbuf,format,arglist);
      twptr->txtpnt = twptr->txtbuf + cw;
      }
   else if (operation == APP)
      {
      cw = vsprintf(twptr->txtpnt,format,arglist);
      twptr->txtpnt += cw;
      }
}

void GIL2VFX_scroll_window(LONG wnd, LONG dx, LONG dy, PaneScroll flags, LONG background)
{
   if (gil2vfx_active != true)
      return;

   VFX_pane_scroll(&panes[wnd], dx, dy, flags, background);
}

void GIL2VFX_print_buffer(LONG linenum)
{
   GIL2VFXA_print_buffer(&panes[twptr->window], linenum);
}

void GIL2VFX_cout(LONG c)
{
   LONG  cvtab,nvtab,htab;   

   if (c==10)
      {
      htab = twptr->htab = panes[twptr->window].left;    // Carriage Return

      cvtab = twptr->vtab - panes[twptr->window].top;
      cvtab += twptr->font->char_height;

      nvtab = cvtab + twptr->font->char_height;

      if (nvtab > panes[twptr->window].bottom - panes[twptr->window].top)
         {
         if (twptr->continueFunction != NULL)
            {
            if ( ((*twptr->continueFunction)(twptr->htab)) == 0)
               {
               twptr->htab = htab;
               return;
               }
            }
         twptr->htab = htab;

         VFX_pane_scroll(&panes[twptr->window], 0, -twptr->font->char_height, PS_NOWRAP, twptr->font->font_background);
         }
      else
         {
         twptr->vtab += twptr->font->char_height;
         }
      }
   else if (c==13)
      {
      twptr->htab = panes[twptr->window].left;    // Carriage Return
      }  
   else
      {
      twptr->htab += VFX_character_draw(&panes[twptr->window],
                      twptr->htab - panes[twptr->window].left,
                      twptr->vtab - panes[twptr->window].top,
                      twptr->font, c, twptr->lookaside);
      }

}

void GIL2VFX_refresh_window(ULONG source, ULONG target)
{
   mouse_pane_refresh(&panes[source], &panes[target]);
}

} // End of namespace Aesop

