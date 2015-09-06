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
#include "aesop/screen.h"

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

#define FBEG 0
#define FCNT 192

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

void draw_bitmap(LONG argcnt, ULONG page, ULONG table, ULONG number, LONG x, LONG y,
		ULONG scale, ULONG flip, ULONG fade_table, ULONG fade_level) {
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
void wait_vertical_retrace(void)
{
//   VFX_wait_vblank_leading();
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

   va_start(argptr,format);

   _vm->_screen->textWindows(0).textColor(15, DK_RED);
   _vm->_screen->textWindows(0).textColor(0, 255);
   _vm->_screen->textWindows(0).vsprint(format, argptr);

   va_end(argptr);
}


void aprint(LONG argcnt, const char *format, ...) {
	error("TODO: aprint with Paramater array");
/*
	va_list args;
	char buffer[100];

	va_start(args, format);
	vsnprintf(buffer, 100, format, args);
	va_end(args);

	debug(buffer);
*/
}

} // End of namespace Aesop

