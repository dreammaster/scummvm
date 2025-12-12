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

#define NO_OLD_SYS_FUNCTIONS
// LUM include ail32.h instead of mss.h
#include "aesop/lib/ail32.h"
#include "aesop/lib/mouse.h"

namespace Aesop {

//
// Mouse system variables
//
            
static LONG volatile x;
static LONG volatile y;
static LONG volatile left;
static LONG volatile right;
static LONG volatile center;

static LONG volatile last_x;
static LONG volatile last_y;
static LONG volatile last_left;
static LONG volatile last_right;
static LONG volatile last_center;

static LONG volatile locked;
static LONG volatile held;  
static RECT saved;

#ifdef DPMI
static LONG   real_event_sel;
#endif

static LONG   real_event_seg;
static HTIMER timer;
static WINDOW save;
static WINDOW work;
static PANE   workp;
static LONG   buffer_size;
static void  *pointer_table;
static LONG   pointer;
static LONG   ptr_width;
static LONG   ptr_height;
static LONG   hot_x;
static LONG   hot_y;
static LONG   scrn_max_x;
static LONG   scrn_max_y;
static LONG   hidecnt;
static LONG   excluded;
static RECT   exclude_region;
// LUM added
static LONG   mouse_active=0; 

LONG cdecl (*watchdog_callback)(RECT *area);
void cdecl (*MOUSE_event_callback)(LONG x, LONG y);
void cdecl (*button_event_callback)(LONG left, LONG right, LONG center);

#ifdef DPMI

typedef struct             // DPMI real-mode interrupt structure
{
   LONG edi;
   LONG esi;
   LONG ebp;
   LONG reserved;
   LONG ebx;
   LONG edx;
   LONG ecx;
   LONG eax;
   WORD flags;
   WORD es;
   WORD ds;
   WORD fs;
   WORD gs;
   WORD ip;
   WORD cs;
   WORD sp;
   WORD ss;
}
RMI_STRUCT;

#else

typedef struct             // INT21 (Phar Lap) real-mode interrupt structure
{
   WORD intnum;
   WORD ds;
   WORD es;
   WORD fs;
   WORD gs;
   LONG eax;
   LONG edx;
}
RMI_STRUCT;

#endif

#undef min
#undef max
#define min(a,b) ((a) < (b) ? (a) : (b))
#define max(a,b) ((a) > (b) ? (a) : (b))

//============================================================================
//==                                                                        ==
//== Save area underneath visible portion of mouse cursor, and draw cursor  ==
//== shape on screen                                                        ==
//==                                                                        ==
//============================================================================

static void MOUSE_draw(void)
{
   LONG shp_x,shp_y;

   // LUM added
   if (!mouse_active) return;

   MOUSE_lock();

   //
   // Create windows "save" and "work" based on visible cursor area, clipping
   // to the edge of the screen if necessary
   //
   // If area under cursor is entirely offscreen, exit
   // If area under cursor lies within an exclusion area, exit
   //

   saved.x0 = x+hot_x;
   saved.y0 = y+hot_y;
   saved.x1 = saved.x0 + ptr_width  - 1;
   saved.y1 = saved.y0 + ptr_height - 1;

   if ((saved.x0 > scrn_max_x) ||
       (saved.y0 > scrn_max_y) ||
       (saved.x1 < 0)          ||
       (saved.y1 < 0))
      {
      excluded = 1;

      MOUSE_unlock();
      return;
      }

   if (saved.x0 < 0)          saved.x0 = 0;
   if (saved.y0 < 0)          saved.y0 = 0;
   if (saved.x1 > scrn_max_x) saved.x1 = scrn_max_x;
   if (saved.y1 > scrn_max_y) saved.y1 = scrn_max_y;

   if (exclude_region.x0 != -1)
      if (MOUSE_shape_in_area(&exclude_region))
         {
         excluded = 1;

         MOUSE_unlock();
         return;
         }

   if (watchdog_callback != NULL)
      if (!watchdog_callback(&saved))
         {
         excluded = 1;

         MOUSE_unlock();
         return;
         }

   excluded = 0;

   workp.x1 = work.x_max = save.x_max = saved.x1-saved.x0;
   workp.y1 = work.y_max = save.y_max = saved.y1-saved.y0;

   //
   // Read two copies of the screen area under the cursor
   //
   // "save" will be used later to restore the background
   // "work" will be used to overlay the cursor shape on the screen
   //
   // Warning: If the compiler's "memmove" function is not re-entrant,
   // replace the memmove() call below with another VFX_window_read()
   // call for the "work" window
   //

   VFX_window_read(&save,saved.x0,saved.y0,saved.x1,saved.y1);
   memmove(work.buffer,save.buffer,buffer_size);

   //
   // Calculate the offset of the pointer in the (0,0)-based "work" window
   //
   // If "work" is clipped to screen x=0 or y=0, draw at the actual X and
   // Y locations; else -hot_x,-hot_y will align the shape with "work"'s
   // top-left corner
   //
   // Finally, copy "work" back to the physical screen
   //

   shp_x = (x+hot_x < 0) ? x : -hot_x;
   shp_y = (y+hot_y < 0) ? y : -hot_y;

   VFX_shape_draw(&workp,pointer_table,pointer,shp_x,shp_y);

   VFX_window_refresh(&work,saved.x0,saved.y0,saved.x1,saved.y1);

   MOUSE_unlock();
}

//============================================================================
//==                                                                        ==
//== Restore area underneath mouse cursor                                   ==
//==                                                                        ==
//============================================================================

static void MOUSE_restore_area(void)
{

   // LUM added
   if (!mouse_active) return;
    
   MOUSE_lock();

   //
   // If cursor not visible, exit
   //

   if (excluded)
      {
      MOUSE_unlock();
      return;
      }

   //
   // Copy the "save" window (written by MOUSE_draw() prior to drawing the 
   // mouse cursor) to the screen
   //

   VFX_window_refresh(&save,saved.x0,saved.y0,saved.x1,saved.y1);

   MOUSE_unlock();
}

//============================================================================
//==                                                                        ==
//== Declare an area of the screen to be off limits to any visible part of  ==
//== the mouse cursor; the cursor will not be drawn if any portion falls    ==
//== within this area                                                       ==
//==                                                                        ==
//== If x0 < 0, the previously registered exclusion area will be cancelled  ==
//==                                                                        ==
//== This exclusion area is internal to the mouse system; for general mouse ==
//== area exclusion, use the MOUSE_register_watchdog_callback() function    ==
//==                                                                        ==
//============================================================================

static void MOUSE_exclude_area(LONG x0, LONG y0, LONG x1, LONG y1)
{
   exclude_region.x0 = x0;
   exclude_region.y0 = y0;
   exclude_region.x1 = x1;
   exclude_region.y1 = y1;
}

//============================================================================
//==                                                                        ==
//== Install a real-mode mouse event handler in lower 1MB of RAM            ==
//==                                                                        ==
//== To avoid the overhead of switching between real and protected mode, we ==
//== don't take advantage of the DOS extender's built-in support for mouse  ==
//== event handlers.  Instead, we make a real-mode interrupt call to the    ==
//== Microsoft INT 33H interface to install a very short real-mode event    ==
//== handler in the lower 1MB of memory.  Each time the mouse state changes,==
//== the event handler records the new state in variables accessible to     ==
//== MOUSE.C.  These variables are then monitored at regular intervals by   ==
//== the protected-mode timer event handler MOUSE_serve(), which can then   ==
//== simulate the action of a true protected-mode mouse event handler       ==
//== with minimal overhead.                                                 ==
//==                                                                        ==
//============================================================================

static LONG MOUSE_install_handler(void)
{
   static unsigned char real_stub[] =
      {
      0xbe,0x00,0x00,         // mov si,0
      0x8e,0xde,              // mov ds,si

      0x89,0x1e,0xf0,0x04,    // mov [4f0],bx   (mouse button state)
      0x89,0x0e,0xf2,0x04,    // mov [4f2],cx   (pointer X coordinate)
      0x89,0x16,0xf4,0x04,    // mov [4f4],dx   (pointer Y coordinate)

      0xcb                    // retf
      };

   //
   // Rational Systems DOS/4GW version
   // 

   {
#ifdef DPMI             

   union REGS inregs,outregs;
   struct SREGS sregs;
   static RMI_STRUCT RMI;

   inregs.x.eax = 0x100;
   inregs.x.ebx = ((sizeof(real_stub)+15) / 16);

   int386(0x31,&inregs,&outregs);

   if (outregs.x.cflag)
      return 0;

   real_event_seg = outregs.x.eax & 0xffff;
   real_event_sel = outregs.x.edx & 0xffff;

   memmove((void *) (real_event_seg*16),
           real_stub,
           sizeof(real_stub));

   memset(&RMI,0,sizeof(RMI));

   RMI.eax = 0x000c;
   RMI.ecx = 0x007f;
   RMI.es  = (UWORD) (real_event_seg & 0xffff);

   inregs.x.eax = 0x300;
   inregs.x.ebx = 0x33;
   inregs.x.ecx = 0;
   inregs.x.edi = FP_OFF((void far *) &RMI);
   sregs.es = sregs.ds = FP_SEG((void far *) &RMI);

   int386x(0x31,&inregs,&outregs,&sregs);

#else

   //
   // MetaWare / Phar Lap version
   //

   ULONG i;
   FARPTR stub;
   union REGS inregs,outregs;
   static RMI_STRUCT RMI;

   inregs.w.ebx = ((sizeof(real_stub)+15) / 16);

   inregs.w.eax = 0x25c0;
   int86(0x21,&inregs,&outregs);

   if (outregs.w.cflag)
      return 0;

   real_event_seg = outregs.w.eax & 0xffff;

   stub.part.seg = 0x34;
   stub.part.off = real_event_seg * 16;

   for (i=0;i<sizeof(real_stub);i++)
      {
      ((_Far UBYTE *) stub.ptr)[i] = real_stub[i];
      }

   inregs.w.edx = (ULONG) &RMI;
   inregs.w.ecx = 0x007f;
   RMI.intnum = 0x33;
   RMI.eax = 0x000c;
   RMI.edx = 0; 
   RMI.es = (UWORD) (real_event_seg & 0xffff);

   inregs.w.eax = 0x2511;
   int86(0x21,&inregs,&outregs);

#endif

   return 1;
   }
}

//============================================================================
//==                                                                        ==
//== Disable real-mode event handler                                        ==
//==                                                                        ==
//============================================================================

static void MOUSE_remove_handler(void)
{
   //
   // Rational Systems DOS/4GW version
   // 

#ifdef DPMI             

   union REGS inregs,outregs;
   struct SREGS sregs;
   static RMI_STRUCT RMI;

   memset(&RMI,0,sizeof(RMI));

   RMI.eax = 0x000c;
   RMI.ecx = 0x0000;
   RMI.es  = 0x0000;

   inregs.x.eax = 0x300;
   inregs.x.ebx = 0x33;
   inregs.x.ecx = 0;
   inregs.x.edi = FP_OFF((void far *) &RMI);
   sregs.es = sregs.ds = FP_SEG((void far *) &RMI);

   int386x(0x31,&inregs,&outregs,&sregs);

#else

   union REGS inregs,outregs;
   static RMI_STRUCT RMI;

   inregs.w.edx = (ULONG) &RMI;
   inregs.w.ecx = 0;
   RMI.intnum = 0x33;
   RMI.eax = 0x000c;
   RMI.edx = 0; 
   RMI.es = 0;

   inregs.w.eax = 0x2511;
   int86(0x21,&inregs,&outregs);

#endif
}

//============================================================================
//==                                                                        ==
//== Inhibit mouse service; stop tracking movement and button activity      ==
//==                                                                        ==
//============================================================================

void MOUSE_lock(void)
{
   ++locked;
}

//============================================================================
//==                                                                        ==
//== Enable mouse service; resume movement and button tracking              ==
//==                                                                        ==
//============================================================================

void MOUSE_unlock(void)
{
   --locked;
}

//============================================================================
//==                                                                        ==
//== Inhibit mouse movement tracking (buttons still monitored)              ==
//==                                                                        ==
//============================================================================

void MOUSE_hold(void)
{
   ++held;
}

//============================================================================
//==                                                                        ==
//== Enable mouse movement tracking                                         ==
//==                                                                        ==
//============================================================================

void MOUSE_release(void)
{
   --held;  
}

//============================================================================
//==                                                                        ==
//== Return physical area of screen currently occupied by visible portion   ==
//== of mouse cursor                                                        ==
//==                                                                        ==
//== Validates *area RECT structure; returns 0 if mouse pointer hidden,     ==
//== excluded, or visible portion offscreen                                 ==
//==                                                                        ==
//============================================================================

LONG MOUSE_visible_area(RECT *area)
{
   MOUSE_lock();

   if ((excluded) || (hidecnt < 0))
      {
      MOUSE_unlock();
      return 0;
      }

   *area = saved;

   MOUSE_unlock();

   return 1;
}

//============================================================================
//==                                                                        ==
//== Return 1 if any visible portion of mouse cursor lies within given      ==
//== rectangular area; 0 otherwise                                          ==
//==                                                                        ==
//============================================================================

LONG MOUSE_shape_in_area(RECT *area)
{
   RECT cur;

   if (!MOUSE_visible_area(&cur))
      return 0;

   if ((cur.x0 > area->x1) ||
       (cur.x1 < area->x0) ||
       (cur.y1 < area->y0) ||
       (cur.y0 > area->y1))
      return 0;

   return 1;
}

//============================================================================
//==                                                                        ==
//== Show mouse cursor                                                      ==
//==                                                                        ==
//============================================================================

void MOUSE_show(void)
{
   MOUSE_lock();

   if (hidecnt)
      {
      ++hidecnt;

      if (!hidecnt)
         MOUSE_draw();
      }

   MOUSE_unlock();
}

//============================================================================
//==                                                                        ==
//== Hide mouse cursor                                                      ==
//==                                                                        ==
//============================================================================

void MOUSE_hide(void)
{
   MOUSE_lock();

   if (!hidecnt)
      MOUSE_restore_area();

   --hidecnt;

   MOUSE_unlock();
}

//============================================================================
//==                                                                        ==
//== Set mouse pointer shape                                                ==
//==                                                                        ==
//== Note: This routine may be called from a background interrupt handler   ==
//==       or mouse event callback function                                 ==
//==                                                                        ==
//============================================================================

void MOUSE_set_pointer(void *table, LONG shape)
{
   LONG hot,res;
   LONG w,h;

   if ((pointer_table == table) &&
       (pointer       == shape))
      return;

   MOUSE_lock();
   MOUSE_hide();

   pointer_table = table;
   pointer       = shape;

   res = VFX_shape_resolution(table,shape);
   hot = VFX_shape_minxy     (table,shape);

   ptr_width  = w = res >> 16;
   ptr_height = h = res & 0xffff;

   hot_x = (LONG) (WORD) (hot >> 16);
   hot_y = (LONG) (WORD) (hot & 0xffff);

   //
   // Warning: buffer_size must be <= MAX_MOUSE_BUFFER_SIZE, or memory 
   // overwrites will result!
   //

   buffer_size = w*h;

   MOUSE_show();
   MOUSE_unlock();
}

//============================================================================
//==                                                                        ==
//== Return current mouse coordinates and button status                     ==
//==                                                                        ==
//== *mx,*my,*ml,*mr,*mc = locations to store mouse status variables for    ==
//== X,Y,left button, right button, center button respectively              ==
//==                                                                        ==
//== Pass NULL pointers for unwanted variables                              ==
//==                                                                        ==
//============================================================================

void MOUSE_status(LONG *mx, LONG *my, LONG *ml, LONG *mr, LONG *mc)
{
   MOUSE_lock();

   if (mx != NULL) *mx = x;
   if (my != NULL) *my = y;
   if (ml != NULL) *ml = left;
   if (mr != NULL) *mr = right;
   if (mc != NULL) *mc = center;

   MOUSE_unlock();
}

//============================================================================
//==                                                                        ==
//== Manually alter the mouse's coordinates                                 ==
//==                                                                        ==
//============================================================================

void MOUSE_force_move(LONG new_x, LONG new_y)
{
   union REGS inregs,outregs;

#ifdef __HIGHC__
   FARPTR loc;
#endif

   MOUSE_lock();

#ifdef DPMI

   //
   // Reset mouse position manually
   //

   inregs.x.eax = 4;
   inregs.x.ecx = new_x << 3;
   inregs.x.edx = new_y << 3;
   int386(0x33,&inregs,&outregs);

   //
   // Initialize shared status variables for new location
   //

   inregs.x.eax = 3;
   int386(0x33,&inregs,&outregs);
   *(WORD *) 0x4f0 = outregs.w.bx;
   *(WORD *) 0x4f2 = outregs.w.cx;
   *(WORD *) 0x4f4 = outregs.w.dx;

#else
#ifdef __HIGHC__

   //
   // Reset mouse position manually
   //

   inregs.w.eax = 4;
   inregs.w.ecx = new_x << 3;
   inregs.w.edx = new_y << 3;
   int86(0x33,&inregs,&outregs);

   //
   // Initialize shared status variables for new location
   //

   inregs.w.eax = 3;
   int86(0x33,&inregs,&outregs);

   loc.part.seg = 0x34; loc.part.off = 0x4f0;
   *(_Far UWORD *) loc.ptr = outregs.x.bx;

   loc.part.seg = 0x34; loc.part.off = 0x4f2;
   *(_Far UWORD *) loc.ptr = outregs.x.cx;

   loc.part.seg = 0x34; loc.part.off = 0x4f4;
   *(_Far UWORD *) loc.ptr = outregs.x.dx;

#endif
#endif

   MOUSE_unlock();
   MOUSE_serve();
}

//============================================================================
//==                                                                        ==
//== Install an application event handler for mouse movement events         ==
//==                                                                        ==
//== This function should be passed the address of a function with the      ==
//== following prototype:                                                   ==
//==                                                                        ==
//==   void cdecl MOUSE_event_callback(LONG x, LONG y)                      ==
//==                                                                        ==
//== This function will be called whenever the mouse's location changes,    ==
//== regardless of whether the cursor is hidden or excluded.                ==
//==                                                                        ==
//== Passing NULL to this function will cancel further event callbacks      ==
//==                                                                        ==
//============================================================================

void MOUSE_register_mouse_event_callback(void cdecl (*fn)(LONG x, LONG y))
{
   MOUSE_event_callback = fn;
}

//============================================================================
//==                                                                        ==
//== Install an application event handler for mouse button events           ==
//==                                                                        ==
//== This function should be passed the address of a function with the      ==
//== following prototype:                                                   ==
//==                                                                        ==
//==   void cdecl button_event_callback(LONG left, LONG right, LONG center) ==
//==                                                                        ==
//== This function will be called whenever the mouse's button status        ==
//== changes, regardless of whether the cursor is hidden, excluded, or held.==
//==                                                                        ==
//== Passing NULL to this function will cancel further event callbacks      ==
//==                                                                        ==
//============================================================================

void MOUSE_register_button_event_callback(void cdecl (*fn)(LONG left,
   LONG right, LONG center))
{
   button_event_callback = fn;
}

//============================================================================
//==                                                                        ==
//== Install an application "watchdog" handler for mouse pointer exclusion  ==
//==                                                                        ==
//== This function should be passed the address of a function with the      ==
//== following prototype:                                                   ==
//==                                                                        ==
//==   LONG cdecl watchdog_callback(RECT *area)                             ==
//==                                                                        ==
//== This function will be called whenever the mouse cursor is about to be  ==
//== drawn to the screen overlaying *area. If it returns a nonzero value,   ==
//== the cursor will be drawn normally.  Otherwise, the cursor will not be  ==
//== drawn.                                                                 ==
//==                                                                        ==
//== Passing NULL to this function will cancel further watchdog callbacks   ==
//==                                                                        ==
//============================================================================

void MOUSE_register_watchdog_callback(LONG cdecl (*fn)(RECT *area))
{
   watchdog_callback = fn;
}

//============================================================================
//==                                                                        ==
//== Timer callback routine simulates protected-mode mouse event handler    ==
//== by periodically inspecting variables maintained by real-mode handler   ==
//==                                                                        ==
//== The real-mode event handler shares data with this timer handler via    ==
//== three variables defined at 0:4F0-0:4F5, an area of low memory          ==
//== designated for interprocess communication                              ==
//==                                                                        ==
//== To eliminate all mouse flicker during movement, add a call to          ==
//== VFX_wait_vblank_leading() just before the MOUSE_restore_area() call    ==
//== below.  However, this can cause a substantial performance loss during  ==
//== periods of rapid mouse movement, especially in EVGA.                   ==
//==                                                                        ==
//== Notes: This routine is called from an asynchronous interrupt handler!  ==
//==        May also be called manually to force status update              ==
//==                                                                        ==
//============================================================================

// LUM this function had to be changed for AIL 2 (__cdecl not __pascal and parameter user was removed)!
// it is needed, otherwise it crashes when the function is called!

void __cdecl MOUSE_timer_serve()
{
   MOUSE_serve();
}

void __cdecl MOUSE_serve()
{
#ifdef __HIGHC__
   FARPTR loc;
#endif

   if (locked > 0) return;

   ++locked;

   last_left   = left;
   last_right  = right;
   last_center = center;

#ifdef DPMI
   left   =  ((*(WORD *) 0x4f0) & 0x01) != 0;
   right  =  ((*(WORD *) 0x4f0) & 0x02) != 0;
   center =  ((*(WORD *) 0x4f0) & 0x04) != 0;
#else
#ifdef PHARLAP
   loc.part.seg = 0x34; loc.part.off = 0x4f0;

   left   =  ((*(_Far UBYTE *) loc.ptr) & 0x01) != 0;
   right  =  ((*(_Far UBYTE *) loc.ptr) & 0x02) != 0;
   center =  ((*(_Far UBYTE *) loc.ptr) & 0x04) != 0;
#endif
#endif

   if ((left != last_left) || (right != last_right) || (center != last_center))
      if (button_event_callback != NULL)
         button_event_callback(left,right,center);

   if (held > 0)
      {
      --locked;
      return;
      }

   last_x = x;
   last_y = y;

#ifdef DPMI
   x = (LONG) (*(WORD *) 0x4f2) >> 3;
   y = (LONG) (*(WORD *) 0x4f4) >> 3;
#else
#ifdef PHARLAP              
   loc.part.seg = 0x34; loc.part.off = 0x4f2;
   x = (LONG) (*(_Far UWORD *) loc.ptr) >> 3;

   loc.part.seg = 0x34; loc.part.off = 0x4f4;
   y = (LONG) (*(_Far UWORD *) loc.ptr) >> 3;
#endif
#endif

   if ((x != last_x) || (y != last_y))
      {
      if (MOUSE_event_callback != NULL)
         MOUSE_event_callback(x,y);

      if (hidecnt >= 0)
         {
         MOUSE_restore_area();         // restore area at old cursor position
         MOUSE_draw();                 // draw the cursor
         }
      }

   --locked;
}

//============================================================================
//==                                                                        ==
//== Wrapper for VFX_window_refresh() which automatically handles mouse     ==
//== cursor maintenance if the mouse cursor falls within the target screen  ==
//== coordinates                                                            ==
//==                                                                        ==
//============================================================================

void MOUSE_window_refresh(WINDOW *target, LONG x0, LONG y0, LONG x1, LONG y1)
{
   LONG   shp_x,shp_y;
   LONG   bw,bh;
   PANE   bkgnd,client;
   LONG   mx,my,ml,mr,mt,mb,xr;
   UBYTE *in;

   //
   // If mouse hidden, just call the driver and return
   //

   if (hidecnt < 0)
      {
      VFX_window_refresh(target,x0,y0,x1,y1);
      return;
      }

   //
   // Freeze the mouse to ensure valid coordinates
   //

   MOUSE_hold();

   //
   // If mouse is currently outside the region to be refreshed, register the
   // region as an exclusion area, release the mouse, do the refresh, 
   // cancel the exclusion area, and force an update
   //

   if ((saved.x0 > x1) ||
       (saved.x1 < x0) ||
       (saved.y1 < y0) ||
       (saved.y0 > y1))
      {
      MOUSE_exclude_area(x0,y0,x1,y1);

      MOUSE_release();

      VFX_window_refresh(target,x0,y0,x1,y1);

      MOUSE_exclude_area(-1,-1,-1,-1);

      MOUSE_serve();
      return;
      }

   //
   // If this is a scaled window refresh, do a hide/refresh/show cycle and
   // return
   //

   if (((x1-x0) != target->x_max) ||
       ((y1-y0) != target->y_max))
      {
      VFX_wait_vblank_leading();
      MOUSE_hide();
      VFX_window_refresh(target,x0,y0,x1,y1);
      MOUSE_show();

      MOUSE_release();
      MOUSE_serve();
      return;
      }

   //
   // If client window has a stencil, do a hide/refresh/show cycle if
   // the mouse cursor overlaps a non-transparent area of the stencil
   //

   if (target->stencil != NULL)
      {
      //
      // Find edges of visible mouse cursor relative to 
      // window being refreshed
      //

      ml = max(x0,saved.x0) - x0;
      mr = min(x1,saved.x1) - x0;
      mt = max(y0,saved.y0) - y0;
      mb = min(y1,saved.y1) - y0;

      //
      // See if a non-transparent stencil packet overlaps mouse cursor
      //

      for (my=mt; my <= mb; my++)
         {
         mx = 0;

         in = (UBYTE *) target->stencil +
            (((ULONG *) target->stencil)[my]);

         while (mx <= mr)
            {
            xr = mx + (*in & 0x7f) - 1;

            if (*in < 128)
               {
               if (!((mx > mr) || (xr < ml)))
                  {
                  VFX_wait_vblank_leading();
                  MOUSE_hide();
                  VFX_window_refresh(target,x0,y0,x1,y1);
                  MOUSE_show();

                  MOUSE_release();
                  MOUSE_serve();
                  return;
                  }
               }

            in++;
            mx = xr + 1;
            }
         }
      }

   //
   // If mouse is inside the region to be refreshed, temporarily merge the 
   // pointer shape with the client's window so that it will not be erased
   // during the refresh, do the refresh, and restore the client's window
   // contents
   //

   //
   // Define pane to describe part of client window which will be overlaid
   // by cursor
   //

   client.window = target;
   client.x0 = max(x0,saved.x0) - x0;
   client.x1 = min(x1,saved.x1) - x0;
   client.y0 = max(y0,saved.y0) - y0;
   client.y1 = min(y1,saved.y1) - y0;

   //
   // Define pane to describe part of saved background window which 
   // lies within client screen area
   //

   bw = saved.x1-saved.x0;
   bh = saved.y1-saved.y0;

   bkgnd.window = &save;
   bkgnd.x0 = (saved.x0 < x0) ? x0 -  saved.x0     : 0;
   bkgnd.x1 = (saved.x1 > x1) ? bw - (saved.x1-x1) : bw;
   bkgnd.y0 = (saved.y0 < y0) ? y0 -  saved.y0     : 0;
   bkgnd.y1 = (saved.y1 > y1) ? bh - (saved.y1-y1) : bh;

   //
   // Update background preservation window by copying client source 
   // pane to background pane
   //

   VFX_pane_copy(&client,0,0,&bkgnd,0,0,NO_COLOR);

   //
   // Draw the mouse pointer into the client's source window, so it will
   // appear onscreen at the correct place when the window is refreshed
   //

   shp_x = (x+hot_x < x0) ? x-x0 : -hot_x;
   shp_y = (y+hot_y < y0) ? y-y0 : -hot_y;

   VFX_shape_draw(&client,pointer_table,pointer,shp_x,shp_y);

   //
   // Copy client's window to screen, including the overlaid mouse pointer
   //

   VFX_window_refresh(target,x0,y0,x1,y1);

   //
   // Finally, restore the part of the client's window which we overwrote
   // with the mouse pointer (not necessary if client doesn't intend to 
   // re-use contents of window)
   //

   VFX_pane_copy(&bkgnd,0,0,&client,0,0,NO_COLOR);
       
   //
   // Release the mouse and force service in case coordinates 
   // need updating
   //

   MOUSE_release();
   MOUSE_serve();
}

//============================================================================
//==                                                                        ==
//== Wrapper for VFX_pane_refresh() which automatically handles mouse       ==
//== cursor maintenance if the mouse cursor falls within the target screen  ==
//== coordinates                                                            ==
//==                                                                        ==
//============================================================================

void MOUSE_pane_refresh(PANE *target, LONG x0, LONG y0, LONG x1, LONG y1)
{
   LONG   shp_x,shp_y;
   LONG   bw,bh;
   PANE   bkgnd,client;
   LONG   mx,my,ml,mr,mt,mb,xr;
   UBYTE *in;

   //
   // If mouse hidden, just call the driver and return
   //

   if (hidecnt < 0)
      {
      VFX_pane_refresh(target,x0,y0,x1,y1);
      return;
      }

   //
   // Freeze the mouse to ensure valid coordinates
   //

   MOUSE_hold();

   //
   // If mouse is currently outside the region to be refreshed, register the
   // region as an exclusion area, release the mouse, do the refresh, 
   // cancel the exclusion area, and force an update
   //

   if ((saved.x0 > x1) ||
       (saved.x1 < x0) ||
       (saved.y1 < y0) ||
       (saved.y0 > y1))
      {
      MOUSE_exclude_area(x0,y0,x1,y1);

      MOUSE_release();

      VFX_pane_refresh(target,x0,y0,x1,y1);

      MOUSE_exclude_area(-1,-1,-1,-1);

      MOUSE_serve();
      return;
      }

   //
   // If this is a scaled pane refresh, do a hide/refresh/show cycle and
   // return
   //

   if (((x1-x0) !=
         (min(target->x1,target->window->x_max) - max(target->x0,0))) ||
       ((y1-y0) !=
         (min(target->y1,target->window->y_max) - max(target->y0,0))))
      {
      VFX_wait_vblank_leading();
      MOUSE_hide();
      VFX_pane_refresh(target,x0,y0,x1,y1);
      MOUSE_show();

      MOUSE_release();
      MOUSE_serve();
      return;
      }

   //
   // If client window has a stencil, do a hide/refresh/show cycle if
   // the mouse cursor overlaps a non-transparent area of the stencil
   //

   if (target->window->stencil != NULL)
      {
      //
      // Find edges of visible mouse cursor relative to 
      // window being refreshed
      //

      ml = max(x0,saved.x0) - x0 + target->x0;
      mr = min(x1,saved.x1) - x0 + target->x0;
      mt = max(y0,saved.y0) - y0 + target->y0;
      mb = min(y1,saved.y1) - y0 + target->y0;

      //
      // See if a non-transparent stencil packet overlaps mouse cursor
      //

      for (my=mt; my <= mb; my++)
         {
         mx = 0;

         in = (UBYTE *) target->window->stencil +
            (((ULONG *) target->window->stencil)[my]);

         while (mx <= mr)
            {
            xr = mx + (*in & 0x7f) - 1;

            if (*in < 128)
               {
               if (!((mx > mr) || (xr < ml)))
                  {
                  VFX_wait_vblank_leading();
                  MOUSE_hide();
                  VFX_pane_refresh(target,x0,y0,x1,y1);
                  MOUSE_show();

                  MOUSE_release();
                  MOUSE_serve();
                  return;
                  }
               }

            in++;
            mx = xr + 1;
            }
         }
      }

   //
   // If mouse is inside the region to be refreshed, temporarily merge the 
   // pointer shape with the client's pane so that it will not be erased
   // during the refresh, do the refresh, and restore the client's pane
   // contents
   //

   //
   // Define pane to describe part of client window which will be overlaid
   // by cursor
   //
   // Set client pane coordinates relative to target pane
   //

   client.window = target->window;
   client.x0 = max(x0,saved.x0) - x0 + target->x0;
   client.x1 = min(x1,saved.x1) - x0 + target->x0;
   client.y0 = max(y0,saved.y0) - y0 + target->y0;
   client.y1 = min(y1,saved.y1) - y0 + target->y0;

   //
   // Define pane to describe part of saved background window which 
   // lies within client screen area
   //

   bw = saved.x1-saved.x0;
   bh = saved.y1-saved.y0;

   bkgnd.window = &save;
   bkgnd.x0 = (saved.x0 < x0) ? x0 -  saved.x0     : 0;
   bkgnd.x1 = (saved.x1 > x1) ? bw - (saved.x1-x1) : bw;
   bkgnd.y0 = (saved.y0 < y0) ? y0 -  saved.y0     : 0;
   bkgnd.y1 = (saved.y1 > y1) ? bh - (saved.y1-y1) : bh;

   //
   // Update background preservation window by copying client source 
   // pane to background pane
   //

   VFX_pane_copy(&client,0,0,&bkgnd,0,0,NO_COLOR);

   //
   // Draw the mouse pointer into the client's source window, so it will
   // appear onscreen at the correct place when the window is refreshed
   //

   shp_x = (x+hot_x < x0) ? x-x0 : -hot_x;
   shp_y = (y+hot_y < y0) ? y-y0 : -hot_y;

   VFX_shape_draw(&client,pointer_table,pointer,shp_x,shp_y);

   //
   // Copy client's pane to screen, including the overlaid mouse pointer
   //

   VFX_pane_refresh(target,x0,y0,x1,y1);

   //
   // Finally, restore the part of the client's window which we overwrote
   // with the mouse pointer (not necessary if client doesn't intend to 
   // re-use contents of window)
   //

   VFX_pane_copy(&bkgnd,0,0,&client,0,0,NO_COLOR);
       
   //
   // Release the mouse and force service in case coordinates 
   // need updating
   //

   MOUSE_release();
   MOUSE_serve();
}

//============================================================================
//==                                                                        ==
//== Perform MOUSE_pane_refresh() calls for all entries in a PANE_LIST      ==
//==                                                                        ==
//============================================================================

void MOUSE_pane_list_refresh(PANE_LIST *list)
{
   ULONG i;
   PANE *a;

   for (i=0; i<list->size; i++)
      {
      if (list->flags[i] == PL_VALID)
         {
         a = &list->array[i];

         MOUSE_pane_refresh(a,a->x0,a->y0,a->x1,a->y1);
         }
      }
}

//============================================================================
//==                                                                        ==
//== Initialize mouse system                                                ==
//==                                                                        ==
//============================================================================

LONG MOUSE_init(LONG xsize, LONG ysize, LONG background)
{
   union REGS inregs,outregs;

   //
   // Initialize mouse system variables: by default, mouse is unlocked,
   // released, and hidden, with no valid pointer shape table and no exclusion
   // area
   //

   locked =  0;
   held   =  0;
   hidecnt = -1;
   pointer_table = NULL;
   excluded = 0;

   MOUSE_exclude_area(-1,-1,-1,-1);

   MOUSE_event_callback  = NULL;
   button_event_callback = NULL;
   watchdog_callback     = NULL;

   x = y = left = right = center = -1;

   scrn_max_x = xsize-1;
   scrn_max_y = ysize-1;

#ifdef DPMI             

   //
   // Init mouse driver, returning 0 on failure
   // 

   inregs.x.eax = 0;
   int386(0x33,&inregs,&outregs);

   if (outregs.w.ax != 0xffff)
      return 0;

   //
   // Set horizontal and vertical limits for mouse movement
   // Multiply limits by 8 to obtain single-pixel resolution
   //

   inregs.x.eax = 7;
   inregs.x.ecx = 0;
   inregs.x.edx = scrn_max_x << 3;
   int386(0x33,&inregs,&outregs);

   inregs.x.eax = 8;
   inregs.x.ecx = 0;
   inregs.x.edx = scrn_max_y << 3;
   int386(0x33,&inregs,&outregs);

   //
   // Set mouse movement rate = 1 mickey/pixel
   //

   inregs.x.eax = 0x0f;
   inregs.x.ecx = 1;
   inregs.x.edx = 1;
   int386(0x33,&inregs,&outregs);

#else
#ifdef __HIGHC__

   //
   // Init mouse driver, returning 0 on failure
   // 

   inregs.w.eax = 0;
   int86(0x33,&inregs,&outregs);

   if (outregs.x.ax != 0xffff)
      return 0;

   //
   // Set horizontal and vertical limits for mouse movement
   // Multiply limits by 8 to obtain single-pixel resolution
   //

   inregs.w.eax = 7;
   inregs.w.ecx = 0;
   inregs.w.edx = scrn_max_x << 3;
   int86(0x33,&inregs,&outregs);

   inregs.w.eax = 8;
   inregs.w.ecx = 0;
   inregs.w.edx = scrn_max_y << 3;
   int86(0x33,&inregs,&outregs);

   //
   // Set mouse movement rate = 1 mickey/pixel
   //

   inregs.w.eax = 0x0f;
   inregs.w.ecx = 1;
   inregs.w.edx = 1;
   int86(0x33,&inregs,&outregs);

#endif
#endif

   if (!MOUSE_install_handler())
      return 0;

   //
   // Initialize windows used by pointer rendering
   //

   work.buffer = malloc(MAX_MOUSE_BUFFER_SIZE);
   save.buffer = malloc(MAX_MOUSE_BUFFER_SIZE);

   work.stencil = work.shadow = NULL;
   save.stencil = save.shadow = NULL;

   workp.x0 = 0;
   workp.y0 = 0;

   workp.window = &work;

   //
   // Force service to validate mouse status variables
   //

   MOUSE_force_move(xsize/2,ysize/2);

   //
   // Arrange for periodic background mouse service
   //
   // The mouse service rate should equal the vertical refresh period
   // for smoothest cursor movement.  In 200-line mode, we assume a period
   // of 70 Hz; otherwise, a 60 Hz timer rate is used.
   //
   // If AIL/32 timer services are unavailable, the application must arrange
   // for another source of periodic service.  The MOUSE_serve() routine may
   // either be polled, or called from an interrupt handler.  To eliminate
   // all mouse flicker, call MOUSE_serve() from an interrupt handler synced
   // to the actual vertical retrace timing signal.
   //

   timer = -1;

   if (background)
      {
      timer = AIL_register_timer(MOUSE_timer_serve);

      if (timer != -1)
         {
         if (ysize >= 400)
            AIL_set_timer_frequency(timer,60);
         else
            AIL_set_timer_frequency(timer,70);

         AIL_start_timer(timer);
         }
      }

   // LUM added
   mouse_active = 1;

   return 1;
}

//============================================================================
//==                                                                        ==
//== Shut down mouse system                                                 ==
//==                                                                        ==
//============================================================================

void MOUSE_shutdown(void)
{
   union REGS inregs,outregs;

   // LUM added
   mouse_active = 0;

   MOUSE_hide();

   if (timer != -1)
      {
      AIL_stop_timer(timer);
      AIL_release_timer_handle(timer);
      }

   MOUSE_remove_handler();

   //
   // Free pointer background buffer memory
   //

   free(save.buffer);
   free(work.buffer);

   //
   // Free real-mode memory used by event handler stub 
   //

#ifdef DPMI             
   inregs.x.eax = 0x101;
   inregs.x.edx = real_event_sel;
   int386(0x31,&inregs,&outregs);

#else
#ifdef PHARLAP

   inregs.w.eax = 0x25c1;
   inregs.w.ecx = real_event_seg;
   int86(0x21,&inregs,&outregs);

#endif
#endif
}

#ifdef DEMO

//============================================================================
//==                                                                        ==
//== Exercise mouse system (compiled only if DEMO defined)                  ==
//==                                                                        ==
//============================================================================

LONG volatile local_X;
LONG volatile local_Y;
LONG volatile local_left;
LONG volatile local_right;

//
// Callback functions for simulated mouse event handler
//
// Note: These functions are called from within an AIL/32 timer
// interrupt handler
//

void cdecl MOUSE_event_fn(LONG x, LONG y)
{
   local_X = x;
   local_Y = y;
}

void cdecl button_event_fn(LONG l, LONG r, LONG c)
{
   c++;              // avoid warning
   local_left  = l;
   local_right = r;
}

void main(int argc, char *argv[])
{
   VFX_DESC *VFX;
   LONG w,h;
   void *DLL,*drvr;

   WINDOW window,twnd;
   PANE pane,text;

   void *shapes;
   FONT *font;
   LONG i;

   static UBYTE colors[256];
   static BYTE strbuf[256];

   //
   // Get name of driver to use
   //

   if (argc != 2)
      {
      printf("\nUsage example: MOUSE drv\\vesa480.dll\n");
      exit(1);
      }

   //
   // Load the driver DLL
   //

   DLL = FILE_read(argv[1],NULL);

   if (DLL == NULL)
      {
      printf("Missing or invalid 386FX driver\n");
      exit(1);
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

   VFX = VFX_describe_driver();

   w = VFX->scrn_width;
   h = VFX->scrn_height;

   printf("\n  Driver name: %s\n",argv[1]);
   printf(" Screen width: %d\n",w);
   printf("Screen height: %d\n",h);
   getch();

   //
   // Turn graphics on; set up window and pane
   //

   VFX_init_driver();

        window.buffer = calloc(w*h,1);
   window.stencil = window.shadow = NULL;
        pane.x0 = 0;
        pane.y0 = 0;
        pane.x1 = window.x_max = w-1;
        pane.y1 = window.y_max = h-1;
        pane.window = &window;

   for (i=0;i<h/4;i++)
      {
      VFX_line_draw(&pane,i,i,w-1-i,i,LD_DRAW,i);
      VFX_line_draw(&pane,i,i,i,h-1-i,LD_DRAW,i);

      VFX_line_draw(&pane,i,h-1-i,w-1-i,h-1-i,LD_DRAW,i);
      VFX_line_draw(&pane,w-1-i,i,w-1-i,h-1-i,LD_DRAW,i);
      }

   VFX_window_refresh(&window,0,0,w-1,h-1);

   //
   // Init AIL (for timer services)
   //

   AIL_startup();

   //
   // Init mouse system
   //

   MOUSE_init(w,h,1);

   shapes = FILE_read("mouse.shp",NULL);
   MOUSE_set_pointer(shapes,0);

   MOUSE_register_mouse_event_callback(MOUSE_event_fn);
   MOUSE_register_button_event_callback(button_event_fn);

   MOUSE_show();

   //
   // Test MOUSE_refresh_window()
   //

   font = FILE_read("demo.fnt",NULL);
   for (x=0;x<256;x++)
      colors[x]=x;
   colors[font->font_background]=15;

   text.window = &twnd;
   text.x0 = 0;
   text.y0 = 0;
   text.x1 = w-1-(i-1)-(i-1);
   text.y1 = h-1-(i-1)-(i-1);

   twnd.stencil = twnd.shadow = NULL;
   twnd.x_max = w-1-(i-1)-(i-1);
   twnd.y_max = h-1-(i-1)-(i-1);
   twnd.buffer = calloc((twnd.x_max+1)*(twnd.y_max+1),1);

   VFX_pane_wipe(&text,15);

   //
   // Demonstrate mouse system
   //

   while (!kbhit())
      {
      colors[1] = (++colors[1]) & 0x0f;

      VFX_pane_scroll(&text,0,-(font->char_height),PS_NOWRAP,NO_COLOR);

      sprintf(strbuf,"(%d,%d) (%d,%d)           ",
         local_X,local_Y,local_left,local_right);

      VFX_string_draw(&text,4,text.y1-font->char_height+1,font,strbuf,colors);

      MOUSE_window_refresh(&twnd,i-1,i-1,w-1-(i-1),h-1-(i-1));
      }
   getch();

   //
   // Shut everything down
   //

   MOUSE_shutdown();
   VFX_shutdown_driver();
   AIL_shutdown();
}

#endif   // #ifdef DEMO

} // namespace Aesop
