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

#include "aesop/defs.h"
#include "aesop/rtres.h"
#include "aesop/rtsystem.h"
#include "aesop/shared.h"
#include "aesop/event.h"
#include "aesop/system/mouse.h"
#include "aesop/system/vfx.h"
#include "aesop/system/ail32.h"
#include "aesop/system/timers.h"

namespace Aesop {

//
// Mouse system variables
//

static LONG x;
static LONG y;
static LONG left;
static LONG right;
static LONG center;

static LONG last_x;
static LONG last_y;
static LONG last_left;
static LONG last_right;
static LONG last_center;

static LONG locked;
static LONG held;
static RECT saved;

static LONG   real_event_sel;
static LONG   real_event_seg;
static HTIMER timer;
static WINDOW save;
static WINDOW work;
static PANE   workp;
static LONG   buffer_size;
static void *pointer_table;
static LONG   pointer;
static LONG   ptr_width;
static LONG   ptr_height;
static LONG   hot_x;
static LONG   hot_y;
static LONG   user_x;
static LONG   user_y;
static LONG   scrn_max_x;
static LONG   scrn_max_y;
static LONG   hidecnt;
static LONG   excluded;
static RECT   exclude_region;
static LONG   mouse_active = 0;

LONG (*watchdog_callback)(RECT *area);
void (*mouse_event_callback)(LONG x, LONG y);
void (*button_event_callback)(LONG left, LONG right, LONG center);

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
DPMI_RMI;

#endif

//============================================================================
//==                                                                        ==
//== Save area underneath visible portion of mouse cursor, and draw cursor  ==
//== shape on screen                                                        ==
//==                                                                        ==
//============================================================================

static void mouse_draw() {
	LONG shp_x, shp_y;

	if (!mouse_active) return;

	mouse_lock();

	//
	// Create windows "save" and "work" based on visible cursor area, clipping
	// to the edge of the screen if necessary
	//
	// If area under cursor is entirely offscreen, exit
	// If area under cursor lies within an exclusion area, exit
	//

	saved.x0 = x + hot_x + user_x;
	saved.y0 = y + hot_y + user_y;
	saved.x1 = saved.x0 + ptr_width - 1;
	saved.y1 = saved.y0 + ptr_height - 1;

	if ((saved.x0 > scrn_max_x) ||
		(saved.y0 > scrn_max_y) ||
		(saved.x1 < 0) ||
		(saved.y1 < 0))
	{
		excluded = 1;

		mouse_unlock();
		return;
	}

	if (saved.x0 < 0)          saved.x0 = 0;
	if (saved.y0 < 0)          saved.y0 = 0;
	if (saved.x1 > scrn_max_x) saved.x1 = scrn_max_x;
	if (saved.y1 > scrn_max_y) saved.y1 = scrn_max_y;

	if (exclude_region.x0 != -1)
		if (mouse_shape_in_area(&exclude_region))
		{
			excluded = 1;

			mouse_unlock();
			return;
		}

	if (watchdog_callback != NULL)
		if (!watchdog_callback(&saved))
		{
			excluded = 1;

			mouse_unlock();
			return;
		}

	excluded = 0;

	workp.x1 = work.x1 = save.x1 = saved.x1 - saved.x0;
	workp.y1 = work.y1 = save.y1 = saved.y1 - saved.y0;

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

	VFX_window_read(&save, saved.x0, saved.y0, saved.x1, saved.y1);
	memmove(work.buffer, save.buffer, buffer_size);

	//
	// Calculate the offset of the pointer in the (0,0)-based "work" window
	//
	// If "work" is clipped to screen x=0 or y=0, draw at the actual X and
	// Y locations; else -hot_x,-hot_y will align the shape with "work"'s
	// top-left corner
	//
	// Finally, copy "work" back to the physical screen
	//

	shp_x = (x + hot_x + user_x < 0) ? x + user_x : -hot_x;
	shp_y = (y + hot_y + user_y < 0) ? y + user_y : -hot_y;

	VFX_shape_draw(&workp, pointer_table, pointer, shp_x, shp_y);

	VFX_window_refresh(&work, saved.x0, saved.y0, saved.x1, saved.y1);

	mouse_unlock();
}

//============================================================================
//==                                                                        ==
//== Restore area underneath mouse cursor                                   ==
//==                                                                        ==
//============================================================================

static void mouse_restore_area() {
	if (!mouse_active) return;

	mouse_lock();

	//
	// If cursor not visible, exit
	//

	if (excluded)
	{
		mouse_unlock();
		return;
	}

	//
	// Copy the "save" window (written by mouse_draw() prior to drawing the 
	// mouse cursor) to the screen
	//

	VFX_window_refresh(&save, saved.x0, saved.y0, saved.x1, saved.y1);

	mouse_unlock();
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
//== area exclusion, use the mouse_register_watchdog_callback() function    ==
//==                                                                        ==
//============================================================================

static void mouse_exclude_area(LONG x0, LONG y0, LONG x1, LONG y1) {
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
//== the protected-mode timer event handler mouse_serve(), which can then   ==
//== simulate the action of a true protected-mode mouse event handler       ==
//== with minimal overhead.                                                 ==
//==                                                                        ==
//============================================================================

static LONG mouse_install_handler() {
	static unsigned char real_stub[] =
	{
	0xbe,0x00,0x00,         // mov si,0
	0x8e,0xde,              // mov ds,si

	0x89,0x1e,0xf4,0x04,    // mov [4f4],bx   (mouse button state)
	0x89,0x0e,0xf6,0x04,    // mov [4f6],cx   (pointer X coordinate)
	0x89,0x16,0xf8,0x04,    // mov [4f8],dx   (pointer Y coordinate)

	0xcb                    // retf
	};

	//
	// Rational Systems DOS/4GW version
	// 

#ifdef DPMI             

	union REGS inregs, outregs;
	struct SREGS sregs;
	static DPMI_RMI RMI;

	inregs.x.eax = 0x100;
	inregs.x.ebx = ((sizeof(real_stub) + 16) / 16);

	int386(0x31, &inregs, &outregs);

	if (outregs.x.cflag)
		return 0;

	real_event_seg = outregs.x.eax & 0xffff;
	real_event_sel = outregs.x.edx & 0xffff;

	memmove((void *)(real_event_seg * 16),
		real_stub,
		sizeof(real_stub));

	memset(&RMI, 0, sizeof(RMI));

	RMI.eax = 0x000c;
	RMI.ecx = 0x007f;
	RMI.es = (UWORD)(real_event_seg & 0xffff);

	inregs.x.eax = 0x300;
	inregs.x.ebx = 0x33;
	inregs.x.ecx = 0;
	inregs.x.edi = FP_OFF((void far *) & RMI);
	sregs.es = sregs.ds = FP_SEG((void far *) & RMI);

	int386x(0x31, &inregs, &outregs, &sregs);

#endif

	return 1;
}

//============================================================================
//==                                                                        ==
//== Disable real-mode event handler                                        ==
//==                                                                        ==
//============================================================================

static void mouse_remove_handler() {
	//
	// Rational Systems DOS/4GW version
	// 

#ifdef DPMI             

	union REGS inregs, outregs;
	struct SREGS sregs;
	static DPMI_RMI RMI;

	memset(&RMI, 0, sizeof(RMI));

	RMI.eax = 0x000c;
	RMI.ecx = 0x0000;
	RMI.es = 0x0000;

	inregs.x.eax = 0x300;
	inregs.x.ebx = 0x33;
	inregs.x.ecx = 0;
	inregs.x.edi = FP_OFF((void far *) & RMI);
	sregs.es = sregs.ds = FP_SEG((void far *) & RMI);

	int386x(0x31, &inregs, &outregs, &sregs);

#endif
}

//============================================================================
//==                                                                        ==
//== Inhibit mouse service; stop tracking movement and button activity      ==
//==                                                                        ==
//============================================================================

void mouse_lock() {
	++locked;
}

//============================================================================
//==                                                                        ==
//== Enable mouse service; resume movement and button tracking              ==
//==                                                                        ==
//============================================================================

void mouse_unlock() {
	--locked;
}

//============================================================================
//==                                                                        ==
//== Inhibit mouse movement tracking (buttons still monitored)              ==
//==                                                                        ==
//============================================================================

void mouse_hold() {
	++held;
}

//============================================================================
//==                                                                        ==
//== Enable mouse movement tracking                                         ==
//==                                                                        ==
//============================================================================

void mouse_release() {
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

LONG mouse_visible_area(RECT *area) {
	mouse_lock();

	if ((excluded) || (hidecnt < 0))
	{
		mouse_unlock();
		return 0;
	}

	*area = saved;

	mouse_unlock();

	return 1;
}

//============================================================================
//==                                                                        ==
//== Return 1 if any visible portion of mouse cursor lies within given      ==
//== rectangular area; 0 otherwise                                          ==
//==                                                                        ==
//============================================================================

LONG mouse_shape_in_area(RECT *area) {
	RECT cur;

	if (!mouse_visible_area(&cur))
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

void mouse_show() {
	mouse_lock();

	if (hidecnt)
	{
		++hidecnt;

		if (!hidecnt)
			mouse_draw();
	}

	mouse_unlock();
}

//============================================================================
//==                                                                        ==
//== Hide mouse cursor                                                      ==
//==                                                                        ==
//============================================================================

void mouse_hide() {
	mouse_lock();

	if (!hidecnt)
		mouse_restore_area();

	--hidecnt;

	mouse_unlock();
}

//============================================================================
//==                                                                        ==
//== Set mouse pointer shape                                                ==
//==                                                                        ==
//== Note: This routine may be called from a background interrupt handler   ==
//== if the calls to malloc() and free() are replaced with references to    ==
//== fixed static buffers.                                                  ==
//==                                                                        ==
//============================================================================

void mouse_set_pointer(void *table, LONG shape, LONG uhot_x, LONG uhot_y) {
	LONG hot, res;
	LONG w, h;

	if ((pointer_table == table) &&
		(pointer == shape))
		return;

	mouse_lock();
	mouse_hide();

	pointer_table = table;
	pointer = shape;

	res = VFX_shape_resolution(table, shape);
	hot = VFX_shape_minxy(table, shape);

	ptr_width = w = res >> 16;
	ptr_height = h = res & 0xffff;

	hot_x = ((LONG)(WORD)(hot >> 16));
	hot_y = ((LONG)(WORD)(hot & 0xffff));

	user_x = -uhot_x;
	user_y = -uhot_y;

	buffer_size = w * h;

	if (work.buffer != NULL)
		mem_free(work.buffer);

	if (save.buffer != NULL)
		mem_free(save.buffer);

	save.buffer = (byte *)mem_alloc(buffer_size);
	work.buffer = (byte *)mem_alloc(buffer_size);

	mouse_show();
	mouse_unlock();
}

//============================================================================
//==                                                                        ==
//== Install an application event handler for mouse movement events         ==
//==                                                                        ==
//== This function should be passed the address of a function with the      ==
//== following prototype:                                                   ==
//==                                                                        ==
//==   void mouse_event_callback(LONG x, LONG y)                      ==
//==                                                                        ==
//== This function will be called whenever the mouse's location changes,    ==
//== regardless of whether the cursor is hidden or excluded.                ==
//==                                                                        ==
//== Passing NULL to this function will cancel further event callbacks      ==
//==                                                                        ==
//============================================================================

void mouse_register_mouse_event_callback(void(*fn)(LONG x, LONG y)) {
	mouse_event_callback = fn;
}

//============================================================================
//==                                                                        ==
//== Install an application event handler for mouse button events           ==
//==                                                                        ==
//== This function should be passed the address of a function with the      ==
//== following prototype:                                                   ==
//==                                                                        ==
//==   void button_event_callback(LONG left, LONG right, LONG center) ==
//==                                                                        ==
//== This function will be called whenever the mouse's button status        ==
//== changes, regardless of whether the cursor is hidden, excluded, or held.==
//==                                                                        ==
//== Passing NULL to this function will cancel further event callbacks      ==
//==                                                                        ==
//============================================================================

void mouse_register_button_event_callback(void(*fn)(LONG left,
	LONG right, LONG center)) {
	button_event_callback = fn;
}

//============================================================================
//==                                                                        ==
//== Install an application "watchdog" handler for mouse pointer exclusion  ==
//==                                                                        ==
//== This function should be passed the address of a function with the      ==
//== following prototype:                                                   ==
//==                                                                        ==
//==   LONG watchdog_callback(RECT *area)                             ==
//==                                                                        ==
//== This function will be called whenever the mouse cursor is about to be  ==
//== drawn to the screen overlaying *area. If it returns a nonzero value,   ==
//== the cursor will be drawn normally.  Otherwise, the cursor will not be  ==
//== drawn.                                                                 ==
//==                                                                        ==
//== Passing NULL to this function will cancel further watchdog callbacks   ==
//==                                                                        ==
//============================================================================

void mouse_register_watchdog_callback(LONG(*fn)(RECT *area)) {
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
//== VFX_wait_vblank_leading() just before the mouse_restore_area() call    ==
//== below.  However, this can cause a substantial performance loss during  ==
//== periods of rapid mouse movement, especially in EVGA.                   ==
//==                                                                        ==
//== Notes: This routine is called from an asynchronous interrupt handler!  ==
//==        May also be called manually to force status update              ==
//==                                                                        ==
//============================================================================

void mouse_serve() {
	if (ENABLED <= 0) return;

	if (locked > 0) return;

	++locked;

	last_left = left;
	last_right = right;
	last_center = center;

	left = ((*(WORD *)0x4f4) & 0x01) != 0;
	right = ((*(WORD *)0x4f4) & 0x02) != 0;
	center = ((*(WORD *)0x4f4) & 0x04) != 0;

	if ((left != last_left) || (right != last_right) || (center != last_center))
		if (button_event_callback != NULL)
			button_event_callback(left, right, center);

	if (held > 0)
	{
		--locked;
		return;
	}

	last_x = x;
	last_y = y;

	x = (LONG)(*(WORD *)0x4f6) >> 3;
	y = (LONG)(*(WORD *)0x4f8) >> 3;

	if ((x != last_x) || (y != last_y))
	{
		if (mouse_event_callback != NULL)
			mouse_event_callback(x, y);

		if (hidecnt >= 0)
		{
			mouse_restore_area();         // restore area at old cursor position
			mouse_draw();                 // draw the cursor
		}
	}

	--locked;
}

//============================================================================
//==                                                                        ==
//== Wrapper for MCGA only, which automatically handles mouse               ==
//== cursor maintenance if the mouse cursor falls within the target screen  ==
//== coordinates.                                                           ==
//==                                                                        ==
//============================================================================

void mouse_pane_refresh(PANE *source, PANE *target) {
	LONG shp_x, shp_y;
	LONG bw, bh;
	PANE bkgnd, client;

	//
	// If mouse hidden, just call the driver and return
	//

	if (hidecnt < 0)
	{
		VFX_pane_copy(source, 0, 0, target, 0, 0, NO_COLOR);
		return;
	}

	//
	// Freeze the mouse to ensure valid coordinates
	//

	mouse_hold();

	//
	// If mouse is currently outside the region to be refreshed, register the
	// region as an exclusion area, release the mouse, do the refresh, 
	// cancel the exclusion area, and force an update
	//

	if ((saved.x0 > target->x1) ||
		(saved.x1 < target->x0) ||
		(saved.y1 < target->y0) ||
		(saved.y0 > target->y1))
	{
		mouse_exclude_area(target->x0, target->y0, target->x1, target->y1);

		mouse_release();

		VFX_pane_copy(source, 0, 0, target, 0, 0, NO_COLOR);

		mouse_exclude_area(-1, -1, -1, -1);

		mouse_serve();
		return;
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

	client.window = source->window;
	client.x0 = MAX(target->x0, saved.x0);// - target->x0;
	client.x1 = MIN(target->x1, saved.x1);// - target->x0;
	client.y0 = MAX(target->y0, saved.y0);// - target->y0;
	client.y1 = MIN(target->y1, saved.y1);// - target->y0;

	//
	// Define pane to describe part of saved background window which 
	// lies within client screen area
	//

	bw = saved.x1 - saved.x0;
	bh = saved.y1 - saved.y0;

	bkgnd.window = &save;
	bkgnd.x0 = (saved.x0 < target->x0) ? target->x0 - saved.x0 : 0;
	bkgnd.x1 = (saved.x1 > target->x1) ? bw - (saved.x1 - target->x1) : bw;
	bkgnd.y0 = (saved.y0 < target->y0) ? target->y0 - saved.y0 : 0;
	bkgnd.y1 = (saved.y1 > target->y1) ? bh - (saved.y1 - target->y1) : bh;

	//
	// Update background preservation window by copying client source 
	// pane to background pane
	//

	VFX_pane_copy(&client, 0, 0, &bkgnd, 0, 0, NO_COLOR);

	//
	// Draw the mouse pointer into the client's source window, so it will
	// appear onscreen at the correct place when the window is refreshed
	//

	shp_x = (x + hot_x < target->x0) ? x - target->x0 : -hot_x;
	shp_y = (y + hot_y < target->y0) ? y - target->y0 : -hot_y;

	VFX_shape_draw(&client, pointer_table, pointer, shp_x, shp_y);

	//
	// Copy client's window to screen, including the overlaid mouse pointer
	//

	VFX_pane_copy(source, 0, 0, target, 0, 0, NO_COLOR);

	//
	// Finally, restore the part of the client's window which we overwrote
	// with the mouse pointer (not necessary if client doesn't intend to 
	// re-use contents of window)
	//

	VFX_pane_copy(&bkgnd, 0, 0, &client, 0, 0, NO_COLOR);

	//
	// Release the mouse and force service in case coordinates 
	// need updating
	//

	mouse_release();
	mouse_serve();
}
//============================================================================
//==                                                                        ==
//== Initialize mouse system                                                ==
//==                                                                        ==
//============================================================================

LONG mouse_init(LONG xsize, LONG ysize) {
	//union REGS inregs, outregs;

	scrn_max_x = xsize - 1;
	scrn_max_y = ysize - 1;

#ifdef DPMI             
	//
	// Init mouse driver, returning 0 on failure
	// 

	inregs.x.eax = 0;
	int386(0x33, &inregs, &outregs);

	if (outregs.w.ax != -1)
		return 0;

	//
	// Set horizontal and vertical limits for mouse movement
	// Multiply limits by 8 to obtain single-pixel resolution
	//

	inregs.x.eax = 7;
	inregs.x.ecx = 0;
	inregs.x.edx = scrn_max_x << 3;
	int386(0x33, &inregs, &outregs);

	inregs.x.eax = 8;
	inregs.x.ecx = 0;
	inregs.x.edx = scrn_max_y << 3;
	int386(0x33, &inregs, &outregs);

	//
	// Set mouse movement rate = 1 mickey/pixel
	//

	inregs.x.eax = 0x0f;
	inregs.x.ecx = 1;
	inregs.x.edx = 1;
	int386(0x33, &inregs, &outregs);

	//
	// Set mouse position to center of area
	//

	inregs.x.eax = 4;
	inregs.x.ecx = xsize << 2;
	inregs.x.edx = ysize << 2;
	int386(0x33, &inregs, &outregs);

	//
	// Initialize shared status variables for new location
	//

	inregs.x.eax = 3;
	int386(0x33, &inregs, &outregs);
	*(WORD *)0x4f4 = outregs.w.bx;
	*(WORD *)0x4f6 = outregs.w.cx;
	*(WORD *)0x4f8 = outregs.w.dx;
#endif

	if (!mouse_install_handler())
		return 0;

	//
	// Initialize windows used by pointer rendering
	//

	work.buffer = NULL;
	save.buffer = NULL;

	save.x0 = work.x0 = workp.x0 = 0;
	save.y0 = work.y0 = workp.y0 = 0;

	workp.window = &work;

	//
	// Initialize mouse system variables: by default, mouse is unlocked,
	// released, and hidden, with no valid pointer shape table and no exclusion
	// area
	//

	locked = 0;
	held = 0;
	hidecnt = -1;
	pointer_table = NULL;
	excluded = 0;

	mouse_exclude_area(-1, -1, -1, -1);

	mouse_event_callback = NULL;
	button_event_callback = NULL;
	watchdog_callback = NULL;

	//
	// Force service to validate mouse status variables
	//

	x = y = left = right = center = -1;
	mouse_serve();

	//
	// Arrange for periodic background mouse service
	//
	// The mouse service rate should equal the vertical refresh period
	// for smoothest cursor movement.  In 200-line mode, we assume a period
	// of 70 Hz; otherwise, a 60 Hz timer rate is used
	//
	// If AIL/32 timer services are unavailable, the application must arrange
	// for another source of periodic service.  The mouse_serve() routine may
	// either be polled, or called from an interrupt handler.  To eliminate
	// all mouse flicker, call mouse_serve() from an interrupt handler synced
	// to the actual vertical retrace timing signal
	//

	timer = AIL_register_timer(mouse_serve);

	if (ysize >= 400)
		AIL_set_timer_frequency(timer, 60);
	else
		AIL_set_timer_frequency(timer, 70);

	AIL_start_timer(timer);

	mouse_active = 1;

	return 1;
}

//============================================================================
//==                                                                        ==
//== Shut down mouse system                                                 ==
//==                                                                        ==
//============================================================================

void mouse_shutdown() {
	//union REGS inregs, outregs;

	mouse_active = 0;

	AIL_stop_timer(timer);
	AIL_release_timer_handle(timer);

	mouse_remove_handler();

	//
	// Free real-mode memory used by event handler stub 
	//

#ifdef DPMI             
	inregs.x.eax = 0x101;
	inregs.x.edx = real_event_sel;
	int386(0x31, &inregs, &outregs);
#endif
}

} // namespace Aesop
