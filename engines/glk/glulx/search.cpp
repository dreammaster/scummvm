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

/* Glulxe code for built-in search opcodes
 */

#include "glk/glk.h"
#include "glk/glulx/glulxe.h"

namespace Glk {
namespace Glulx {

#define serop_KeyIndirect (0x01)
#define serop_ZeroKeyTerminates (0x02)
#define serop_ReturnIndex (0x04)
/* ### KeyZeroBounded? variants? */
/* ### LowerBoundKey? */

/* In general, these search functions look through a bunch of structures
   in memory, searching for one whose key (a fixed-size sequence of bytes
   within the structure) matches a given key. The result can indicate a
   particular structure within the bunch, or it can be NULL ("not found".)

   Any or all of these options can be applied:

   KeyIndirect: If this is true, the key argument is taken to be the
   start of an array of bytes in memory (whose length is keysize).
   If it is false, the key argument contains the key itself. In
   this case, keysize *must* be 1, 2, or 4. The key is stored in the
   lower bytes of the key argument, big-endian. (The upper bytes are
   ignored.)

   ZeroKeyTerminates: If this is true, when the search reaches a struct
   whose key is all zeroes, the search terminates (and returns NULL).
   If the searched-for key happens to also be zeroes, the key-match
   (returning the struct) takes precedence over the zero-match (returning
   NULL.)

   ReturnIndex: If this is false, the return value is the memory address
   of the matching struct, or 0 to indicate NULL. If true, the return value
   is the array index of the matching struct, or -1 to indicate NULL.
*/

static void fetchkey(unsigned char *keybuf, glui32 key, glui32 keysize,
	glui32 options);

/* linear_search():
   An array of data structures is stored in memory, beginning at start,
   each structure being structsize bytes. Within each struct, there is
   a key value keysize bytes long, starting at position keyoffset (from
   the start of the structure.) Search through these in order. If one
   is found whose key matches, return it. If numstructs are searched
   with no result, return NULL.

   numstructs may be -1 (0xFFFFFFFF) to indicate no upper limit to the
   number of structures to search. The search will continue until a match
   is found, or (if ZeroKeyTerminates is set) a zero key.

   The KeyIndirect, ZeroKeyTerminates, and ReturnIndex options may be
   used.
*/
glui32 linear_search(glui32 key, glui32 keysize,
	glui32 start, glui32 structsize, glui32 numstructs,
	glui32 keyoffset, glui32 options) {
	unsigned char keybuf[4];
	glui32 count;
	int ix;
	int retindex = ((options & serop_ReturnIndex) != 0);
	int zeroterm = ((options & serop_ZeroKeyTerminates) != 0);

	fetchkey(keybuf, key, keysize, options);

	for (count = 0; count < numstructs; count++, start += structsize) {
		int match = TRUE;
		if (keysize <= 4) {
			for (ix = 0; match && ix < (int)keysize; ix++) {
				if (Mem1(start + keyoffset + ix) != keybuf[ix])
					match = FALSE;
			}
		} else {
			for (ix = 0; match && ix < (int)keysize; ix++) {
				if (Mem1(start + keyoffset + ix) != Mem1(key + ix))
					match = FALSE;
			}
		}

		if (match) {
			if (retindex)
				return count;
			else
				return start;
		}

		if (zeroterm) {
			match = TRUE;
			for (ix = 0; match && ix < (int)keysize; ix++) {
				if (Mem1(start + keyoffset + ix) != 0)
					match = FALSE;
			}
			if (match) {
				break;
			}
		}
	}

	if (retindex)
		return (glui32)-1;
	else
		return 0;
}

/* binary_search():
   An array of data structures is in memory, as above. However, the
   structs must be stored in forward order of their keys (taking each key
   to be a multibyte unsigned integer.) There can be no duplicate keys.
   numstructs must indicate the exact length of the array; it cannot
   be -1.

   The KeyIndirect and ReturnIndex options may be used.
*/
glui32 binary_search(glui32 key, glui32 keysize,
	glui32 start, glui32 structsize, glui32 numstructs,
	glui32 keyoffset, glui32 options) {
	unsigned char keybuf[4];
	unsigned char byte, byte2;
	glui32 top, bot, val, addr;
	int ix;
	int retindex = ((options & serop_ReturnIndex) != 0);

	fetchkey(keybuf, key, keysize, options);

	bot = 0;
	top = numstructs;
	while (bot < top) {
		int cmp = 0;
		val = (top + bot) / 2;
		addr = start + val * structsize;

		if (keysize <= 4) {
			for (ix = 0; (!cmp) && ix < (int)keysize; ix++) {
				byte = Mem1(addr + keyoffset + ix);
				byte2 = keybuf[ix];
				if (byte < byte2)
					cmp = -1;
				else if (byte > byte2)
					cmp = 1;
			}
		} else {
			for (ix = 0; (!cmp) && ix < (int)keysize; ix++) {
				byte = Mem1(addr + keyoffset + ix);
				byte2 = Mem1(key + ix);
				if (byte < byte2)
					cmp = -1;
				else if (byte > byte2)
					cmp = 1;
			}
		}

		if (!cmp) {
			if (retindex)
				return val;
			else
				return addr;
		}

		if (cmp < 0) {
			bot = val + 1;
		} else {
			top = val;
		}
	}

	if (retindex)
		return (glui32)-1;
	else
		return 0;
}

/* linked_search():
   The structures may be anywhere in memory, in any order. They are
   linked by a four-byte address field, which is found in each struct
   at position nextoffset. If this field contains zero, it indicates
   the end of the linked list.

   The KeyIndirect and ZeroKeyTerminates options may be used.
*/
glui32 linked_search(glui32 key, glui32 keysize,
	glui32 start, glui32 keyoffset, glui32 nextoffset, glui32 options) {
	unsigned char keybuf[4];
	int ix;
	glui32 val;
	int zeroterm = ((options & serop_ZeroKeyTerminates) != 0);

	fetchkey(keybuf, key, keysize, options);

	while (start != 0) {
		int match = TRUE;
		if (keysize <= 4) {
			for (ix = 0; match && ix < (int)keysize; ix++) {
				if (Mem1(start + keyoffset + ix) != keybuf[ix])
					match = FALSE;
			}
		} else {
			for (ix = 0; match && ix < (int)keysize; ix++) {
				if (Mem1(start + keyoffset + ix) != Mem1(key + ix))
					match = FALSE;
			}
		}

		if (match) {
			return start;
		}

		if (zeroterm) {
			match = TRUE;
			for (ix = 0; match && ix < (int)keysize; ix++) {
				if (Mem1(start + keyoffset + ix) != 0)
					match = FALSE;
			}
			if (match) {
				break;
			}
		}

		val = start + nextoffset;
		start = Mem4(val);
	}

	return 0;
}

/* fetchkey():
   This massages the key into a form that's easier to handle. When it
   returns, the key will be stored in keybuf if keysize <= 4; otherwise,
   it will be in memory.
*/
static void fetchkey(unsigned char *keybuf, glui32 key, glui32 keysize,
	glui32 options) {
	int ix;

	if (options & serop_KeyIndirect) {
		if (keysize <= 4) {
			for (ix = 0; ix < (int)keysize; ix++)
				keybuf[ix] = Mem1(key + ix);
		}
	} else {
		switch (keysize) {
		case 4:
			Write4(keybuf, key);
			break;
		case 2:
			Write2(keybuf, key);
			break;
		case 1:
			Write1(keybuf, key);
			break;
		default:
			fatal_error("Direct search key must hold one, two, or four bytes.");
		}
	}
}

} // namespace Glulx
} // namespace Glk
