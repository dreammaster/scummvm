//лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
//лл                                                                        лл
//лл  INTERP.C                                                              лл
//лл                                                                        лл
//лл  AESOP runtime host interpreter                                        лл
//лл                                                                        лл
//лл  Version: 1.00 of 6-May-92 -- Initial version                          лл
//лл                                                                        лл
//лл  Project: Extensible State-Object Processor (AESOP/16)                 лл
//лл   Author: John Miles                                                   лл
//лл                                                                        лл
//лл  C source compatible with IBM PC ANSI C/C++ implementations            лл
//лл  Large memory model (16-bit DOS)                                       лл
//лл                                                                        лл
//лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
//лл                                                                        лл
//лл  Copyright (C) 1992 Miles Design, Inc.                                 лл
//лл                                                                        лл
//лл  Miles Design, Inc.                                                    лл
//лл  10926 Jollyville #308                                                 лл
//лл  Austin, TX 78759                                                      лл
//лл  (512) 345-2642 / BBS (512) 454-9990 / FAX (512) 338-9630              лл
//лл                                                                        лл
//лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

//#include "aesop/ail32.h"

#include "aesop/defs.h"
#include "aesop/shared.h"
#include "aesop/rtmsg.h"
#include "aesop/resources.h"
#include "aesop/rtsystem.h"
#include "aesop/rt.h"
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

HRES HROED;

BYTE *pathname;

/*************************************************************/
void aesop_main(int argc, char *argv[])
{
	/*
   BYTE RES_name[256];
   BYTE code_name[256];
   ULONG i;
   ULONG code;
   LONG rtn;

   pathname = argv[0];
   
   setbuf(stdout,NULL);

   ENABLED = 1;

   AIL_startup();
   mem_init();

   if (argc < 3)
      {
      printf(MSG_BANNER);
      printf(MSG_SYN_1);
      abend(NULL);
      }

   strcpy(RES_name,argv[1]);
   for (i=0;i<strlen(RES_name);i++)
      if (RES_name[i] == '.')
         {
         RES_name[i] = 0;
         break;
         }
   strcat(RES_name,".RES");

   strcpy(code_name,argv[2]);

   heap_size = mem_avail()     -
               WINDOW_SIZE     -
               DLL_HEADROOM    -
               MALLOC_HEADROOM -
               STK_SIZE;

   if (heap_size < MIN_RES_SIZE)
      {
      abend(MSG_NO_DOS);
      }

   if (heap_size > MAX_RES_SIZE)
      heap_size = MAX_RES_SIZE;

   RTR = RTR_construct(mem_alloc(heap_size),heap_size,MAX_OBJ_TYPES,RES_name);

   if (RTR==NULL)
      abend(MSG_RIF,RES_name);

   init_object_list();
   init_notify_list();
   init_event_queue();

   RT_init(RTR,STK_SIZE,objlist);

   HROED = RTR_get_resource_handle(RTR,ROED,DA_TEMPORARY | DA_EVANESCENT);
   RTR_lock(RTR,HROED);
   code = ascnum(RTD_lookup(HROED,code_name));
   RTR_unlock(HROED);

   if (code == (ULONG) -1L)
      abend(MSG_SPNF);

   rtn = create_program(1,bootstrap,(ULONG) code);
   rtn = destroy_object(1,rtn);

   for (i=0;i<RTR->nentries;i++)
      {
      ULONG f;

      f = RTR->dir[i].flags;

      if ((f & DA_FREE) && (f & DA_DISCARDED) && (!RTR->dir[i].seg))
         break;
      }

   if (envval(0,"AESOP_DIAG") == 1)
      {
      printf("Entries avail: %u\n",RTR->nentries);
      printf("       In use: %u\n\n",i);

      printf("%u names in use\n",RTR->nd_entries);
      }

   RTR_destroy(RTR,RTR_FREEBASE);
   RT_shutdown();

   mem_shutdown();
   AIL_shutdown(MSG_AIL);

   if (envval(0,"AESOP_DIAG") == 1)
      {
      printf("%lu bytes in heap\n",heap_size);
      printf("%lu bytes left\n",mem_headroom());
      }

   exit(rtn);
   */
}

} // End of namespace Aesop
