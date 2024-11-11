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

#ifndef GLK_GLULXE_TYPES
#define GLK_GLULXE_TYPES

#include "common/scummsys.h"
#include "glk/glulx/glulxe.h"

namespace Glk {
namespace Glulx {

class Glulx;

typedef struct accelentry_struct {
	glui32 addr;
	glui32 index;
	acceleration_func func;
	struct accelentry_struct *next;
} accelentry_t;

typedef struct heapblock_struct {
	glui32 addr;
	glui32 len;
	int isfree;
	struct heapblock_struct *next;
	struct heapblock_struct *prev;
} heapblock_t;

typedef struct cacheblock_struct {
	int depth; /* 1 to 4 */
	int type;
	union {
		struct cacheblock_struct *branches;
		unsigned char ch;
		glui32 uch;
		glui32 addr;
	} u;
} cacheblock_t;


typedef struct arrayref_struct arrayref_t;
struct arrayref_struct {
	void *array;
	glui32 addr;
	glui32 elemsize;
	glui32 len; /* elements */
	int retained;
	arrayref_t *next;
};


typedef struct classref_struct classref_t;
struct classref_struct {
	void *obj;
	glui32 id;
	int bucknum;
	classref_t *next;
};

#define CLASSHASH_SIZE (31)
typedef struct classtable_struct {
	glui32 lastid;
	classref_t *bucket[CLASSHASH_SIZE];
} classtable_t;


typedef struct dispatch_splot_struct {
	int numwanted;
	int maxargs;
	gluniversal_t *garglist;
	glui32 *varglist;
	int numvargs;
	glui32 *retval;
} dispatch_splot_t;

/* This structure allows us to write either to a Glk stream or to
   a dynamically-allocated memory chunk. */
typedef struct dest_struct {
	int ismem;

	/* If it's a Glk stream: */
	strid_t str;

	/* If it's a block of memory: */
	unsigned char *ptr;
	glui32 pos;
	glui32 size;
} dest_t;

enum gestulx {
	gestulx_GlulxVersion = 0,
	gestulx_TerpVersion  = 1,
	gestulx_ResizeMem    = 2,
	gestulx_Undo         = 3,
	gestulx_IOSystem     = 4,
	gestulx_Unicode      = 5,
	gestulx_MemCopy      = 6,
	gestulx_MAlloc       = 7,
	gestulx_MAllocHeap   = 8,
	gestulx_Acceleration = 9,
	gestulx_AccelFunc    = 10,
	gestulx_Float        = 11
};

} // End of namespace Glulx
} // End of namespace Glk

#endif
