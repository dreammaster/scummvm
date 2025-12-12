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

#include "aesop/ail32.h"
#include "aesop/defs.h"
#include "aesop/shared.h"
#include "aesop/rtmsg.h"
#include "aesop/rtres.h"
#include "aesop/rtsystem.h"
#include "aesop/rt.h"
#include "aesop/rtlink.h"
#include "aesop/rtobject.h"
#include "aesop/rtcode.h"
#include "aesop/event.h"

namespace Aesop {

//
// Amount of memory to reserve for scaling buffer (64K) + PAGE2 (64K) + misc.
// (22K)
//

#define  WINDOW_SIZE    150000   

//
// Amount of memory to reserve for DLL loading/linking (100K)
// 

#define DLL_HEADROOM    100000

//
// Amount of memory to reserve for miscellaneous malloc() calls (32K)
//

#define MALLOC_HEADROOM 32768

//
// Amount of memory to reserve for AESOP interpreter stack (16K)
//

#define STK_SIZE        16384

//
// Minimum AESOP resource cache size permissible (600K)
//

#define MIN_RES_SIZE    600000

//
// Maximum useful AESOP resource cache size (800K)
//

#define MAX_RES_SIZE    800000

//
// Globals
//

RTR_class *RTR;

HRES HROED;
uint32 heap_size;

BYTE *pathname;

/*************************************************************/
void main(int argc, char *argv[]) {
	BYTE RES_name[256];
	BYTE code_name[256];
	uint32 i;
	uint32 code;
	int32 rtn;

	pathname = (BYTE *)argv[0];

	setbuf(stdout, NULL);

	ENABLED = 1;

	AIL_startup();
	mem_init();

	if (argc < 3)
	{
		printf(MSG_BANNER);
		printf(MSG_SYN_1);
		abend(NULL);
	}

	strcpy((char *)RES_name, argv[1]);
	for (i = 0; i < strlen((char *)RES_name); i++)
		if (RES_name[i] == '.')
		{
			RES_name[i] = 0;
			break;
		}
	strcat((char *)RES_name, ".RES");

	strcpy((char *)code_name, argv[2]);

	heap_size = mem_avail() -
		WINDOW_SIZE -
		DLL_HEADROOM -
		MALLOC_HEADROOM -
		STK_SIZE;

	if (heap_size < MIN_RES_SIZE)
	{
		abend(MSG_NO_DOS);
	}

	if (heap_size > MAX_RES_SIZE)
		heap_size = MAX_RES_SIZE;

	RTR = RTR_construct(mem_alloc(heap_size), heap_size, MAX_OBJ_TYPES, RES_name);

	if (RTR == NULL)
		abend(MSG_RIF, RES_name);

	init_object_list();
	init_notify_list();
	init_event_queue();

	RT_init(RTR, STK_SIZE, objlist);

	HROED = RTR_get_resource_handle(RTR, ROED, DA_TEMPORARY | DA_EVANESCENT);
	RTR_lock(RTR, HROED);
	code = ascnum(RTD_lookup(HROED, code_name));
	RTR_unlock(HROED);

	if (code == (uint32)-1L)
		abend(MSG_SPNF);

	rtn = create_program(1, bootstrap, (uint32)code);
	rtn = destroy_object(1, rtn);

	for (i = 0; i < RTR->nentries; i++)
	{
		uint32 f;

		f = RTR->dir[i].flags;

		if ((f & DA_FREE) && (f & DA_DISCARDED) && (!RTR->dir[i].seg))
			break;
	}

	if (envval(0, (BYTE *)"AESOP_DIAG") == 1)
	{
		printf("Entries avail: %u\n", RTR->nentries);
		printf("       In use: %u\n\n", i);

		printf("%u names in use\n", RTR->nd_entries);
	}

	RTR_destroy(RTR, RTR_FREEBASE);
	RT_shutdown();

	mem_shutdown();
	AIL_shutdown(MSG_AIL);

	if (envval(0, (BYTE *)"AESOP_DIAG") == 1)
	{
		printf("%lu bytes in heap\n", heap_size);
		printf("%lu bytes left\n", mem_headroom());
	}

	exit(rtn);
}

} // namespace Aesop
