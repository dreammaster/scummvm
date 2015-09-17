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
//#include "aesop/ail32.h"
#include "aesop/aesop.h"
#include "aesop/gil2vfxa.h"
#include "aesop/mouse.h"
#include "aesop/stubs.h"
#include "aesop/defs.h"
#include "aesop/shared.h"
#include "aesop/rtsystem.h"
#include "aesop/rtmsg.h"
#include "aesop/resources.h"
#include "aesop/rtcode.h"
#include "aesop/intrface.h"

namespace Aesop {

extern VFX_DESC *VFX;

LONG interface_active = 0;

HTIMER htimer;
ULONG volatile heartbeat;
ULONG volatile in_BIOS;

ULONG cur_table;
ULONG cur_number;
LONG cur_hot_X;
LONG cur_hot_Y;

ULONG wait_number;
LONG wait_hot_X;
LONG wait_hot_Y;

ULONG save_number;
LONG save_hot_X;
LONG save_hot_Y;

LONG ptr_valid;
LONG wait_ptr_valid;
LONG wait_ptr_state;

HRES pointer_set;
ULONG pointer_set_entry;
ULONG pointer_num;
ULONG pointer_fade_table;
ULONG pointer_fade_level;
ULONG pointer_scale;
LONG last_cursor_X, last_cursor_Y;

// DPMI real-mode interrupt structure
struct DPMI_RMI {
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
};



/*********************************************************/
void init_interface(void) {
	pointer_set = HRES_NULL;
	pointer_set_entry = (ULONG) - 1;
	ptr_valid = 0;
	wait_ptr_valid = 0;
	wait_ptr_state = 0;

}



/*********************************************************/
void set_mouse_pointer(LONG argcnt, ULONG table, ULONG number, LONG hot_X,
                       LONG hot_Y, ULONG scale, ULONG fade_table,
                       ULONG fade_level) {
	/*
	Resources &res = *_vm->_resources;
	ND_entry *entry;

	if ((wait_ptr_state != 0) && (argcnt != 0)) {
		save_number = number;
		save_hot_X = hot_X;
		save_hot_Y = hot_Y;

		return;
	}

	cur_table = table;
	cur_number = number;
	cur_hot_X = hot_X;
	cur_hot_Y = hot_Y;

	if ((table == pointer_set_entry) &&
	        (number == pointer_num) &&
	        (scale == pointer_scale) &&
	        (fade_table == pointer_fade_table) &&
	        (fade_level == pointer_fade_level))
		return;

	if (table != pointer_set_entry) {
		if (pointer_set != HRES_NULL) {
			res.unlock(pointer_set);
		}

		if ((entry = res.search_name_dir(table)) == NULL)
			pointer_set = res.get_resource_handle(table, DA_DEFAULT);
		else
			pointer_set = entry->handle;

		res.lock(pointer_set);

		pointer_set_entry = table;
	}

	pointer_num = number;
	pointer_scale = scale;
	pointer_fade_table = fade_table;
	pointer_fade_level = fade_level;

	ptr_valid = 1;

	mouse_set_pointer(res.addr(pointer_set), pointer_num, hot_X, hot_Y);
	*/
}

/*********************************************************/

void set_wait_pointer(LONG argcnt, ULONG number, LONG hot_X, LONG hot_Y)

{
	if (number == (ULONG) - 1) {
		wait_ptr_valid = 0;
		return;

	}

	wait_number = number;
	wait_hot_X = hot_X;
	wait_hot_Y = hot_Y;

	wait_ptr_valid = 1;
}

/*********************************************************/
void standby_cursor(void) {
	if (!wait_ptr_valid) return;
	if (!ptr_valid) return;

	++wait_ptr_state;

	if (wait_ptr_state == 1) {
		save_number = cur_number;
		save_hot_X = cur_hot_X;
		save_hot_Y = cur_hot_Y;

		set_mouse_pointer(0, cur_table, wait_number, wait_hot_X, wait_hot_Y,
		                  pointer_scale, pointer_fade_table, pointer_fade_level);
	}
}

/*********************************************************/
//
// Turn off hourglass cursor
//
/*********************************************************/

void resume_cursor(void) {
	if (!wait_ptr_valid) return;
	if (!ptr_valid) return;

	--wait_ptr_state;

	if (wait_ptr_state == 0)
		set_mouse_pointer(0, cur_table, save_number, save_hot_X, save_hot_Y,
		                  pointer_scale, pointer_fade_table, pointer_fade_level);
}

} // End of namespace Aesop

