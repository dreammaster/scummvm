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

#include "common/scummsys.h"
#include "common/textconsole.h"
#include "common/util.h"
#include "aesop/aesop.h"
#include "aesop/defs.h"
#include "aesop/eye.h"
#include "aesop/resources.h"
#include "aesop/rtsystem.h"
#include "aesop/rtmsg.h"
#include "aesop/rtcode.h"
#include "aesop/shared.h"
#include "aesop/stubs.h"
#include "aesop/utils.h"

namespace Aesop {

ULONG diag_flag = 0;

//
// Load a resource into a SOP instance's array
//
// Determines array offset in instance, in case resource load causes
// instance to move in memory
//
// WARNING: The array must not be of automatic or external scope!
//

void load_resource(LONG argcnt, BYTE *array, ULONG resource) {
	Objects &objects = *_vm->_objects;
	Resources &res = *_vm->_resources;
	HRES handle;
	ULONG array_offset;
	BYTE *new_array;

	// TODO: Double-check removal of FP_OFF
	array_offset = array - (BYTE *)res.addr(objects[Interpreter::_currentThis]);

	handle = res.get_resource_handle(resource, DA_DEFAULT);

	res.lock(handle);

	new_array = (BYTE *)add_ptr(res.addr(objects[Interpreter::_currentThis]), array_offset);

	far_memmove(new_array, res.addr(handle), res.size(handle));

	res.unlock(handle);
}


void copy_string(LONG argcnt, BYTE *src, BYTE *dest)

{
	strcpy((char *)dest, (const char *)src);
}


void string_force_lower(LONG argcnt, BYTE *dest)

{
	for (char *c = (char *)dest; *c; ++c)
		*c = tolower(*c);
}


void string_force_upper(LONG argcnt, BYTE *dest) {
	for (char *c = (char *)dest; *c; ++c)
		*c = toupper(*c);
}


ULONG string_len(LONG argcnt, BYTE *string)

{
	return strlen((const char *)string);
}


ULONG string_compare(LONG argcnt, BYTE *str1, BYTE *str2)

{
	return scumm_stricmp((const char *)str1, (const char *)str2);
}

//
// Return numeric value of string, or -1 if not valid string
//


LONG strval(LONG argcnt, BYTE *string)

{
	if (string == NULL)
		return -1L;

	return ascnum((const char *)string);
}

//
// Evaluate and return the numeric value of a given DOS environment
// variable's contents
//
// Return -1 if the variable does not exist or cannot be evaluated
//


LONG envval(LONG argcnt, BYTE *name)

{
	warning("Refactor uses of getenv");
	return -1;
	/*
	const char *env;

	if ((env = getenv(name)) == NULL)
	  return -1L;

	return ascnum(env);
	*/
}

//
// Generate speaker beep
//

void beep(void) {
	/*
	UWORD dx,ax;

	outp(0x43, 0x0b6);
	outp(0x42, 169);
	outp(0x42, 4);

	outp(0x61, (inp(0x61) | 3));

	for(dx=5; dx>0; dx--)
	  for(ax=65535; ax>0; ax--);

	outp(0x61, (inp(0x61) & 0x0fc));
	*/
}


void pokemem(LONG argcnt, LONG *addr, LONG data) {
	if (addr)
		*addr = data;
}


LONG peekmem(LONG argcnt, LONG *addr) {
	return addr ? *addr : 0;
}

ULONG rnd(LONG argcnt, ULONG low, ULONG high)

{
	error("TODO: Hook up random number generator");
	/*
	static init=0;

	if (!init)
	  {
	  init = 1;
	  srand(*(UWORD *) 0x0000046c);
	  }

	return low + ((ULONG) rand() % (high-low+1L));
	*/
}


ULONG dice(LONG argcnt, ULONG ndice, ULONG nsides, ULONG bonus)

{
	ULONG n, total;

	total = bonus;

	for (n = 0; n < ndice; n++)
		total += rnd(0, 1, nsides);

	return total;
}

ULONG inkey(void) {
	return (ULONG) kbhit();
}


ULONG absv(LONG argcnt, LONG val)

{
	return (val < 0L) ? -val : val;
}


LONG minv(LONG argcnt, LONG val1, LONG val2)

{
	return MIN(val1, val2);
}


LONG maxv(LONG argcnt, LONG val1, LONG val2)

{
	return MAX(val1, val2);
}


void diagnose(LONG argcnt, ULONG dtype, ULONG parm)

{
	switch (dtype) {
	case 1:
		warning("%X ", (uint)parm);
		break;

	case 2:
		diag_flag = parm;
		break;
	}
}

ULONG heapfree(void) {
	Resources &res = *_vm->_resources;
	return res._free;
}

} // End of namespace Aesop

