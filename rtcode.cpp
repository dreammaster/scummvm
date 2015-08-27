//лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
//лл                                                                        лл
//лл  RTCODE.C                                                              лл
//лл                                                                        лл
//лл  AESOP runtime code resource handlers for Eye III engine               лл
//лл                                                                        лл
//лл  Version: 1.00 of 6-May-92 -- Initial version                          лл
//лл                                                                        лл
//лл  Project: Eye III                                                      лл
//лл   Author: John Miles                                                   лл
//лл                                                                        лл
//лл  C source compatible with Borland C++ v3.0 or later                    лл
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

#include "common/scummsys.h"
#include "common/textconsole.h"
#include "common/util.h"
#include "aesop/defs.h"
#include "aesop/shared.h"
#include "aesop/rtsystem.h"
#include "aesop/rtmsg.h"
#include "aesop/rtres.h"
#include "aesop/rtlink.h"
#include "aesop/rtcode.h"
#include "aesop/rt.h"
#include "aesop/rtobject.h"
#include "aesop/stubs.h"

#include "aesop/eye.h"               // Application code resource header

namespace Aesop {

ULONG diag_flag = 0;

//
// Load a string resource into a SOP instance's array
//
// Determines array offset in instance, in case resource load causes
// instance to move in memory
//
// WARNING: The array must not be of automatic or external scope!
//

#pragma off (unreferenced)
void load_string(LONG argcnt, BYTE *array, ULONG string)
#pragma on (unreferenced)
{
   HRES handle;
   BYTE *ptr;
   BYTE *new_array;
   ULONG array_offset;

   array_offset = (ULONG) array - (ULONG) RTR_addr(objlist[current_this]);

   handle = RTR_get_resource_handle(RTR,string,DA_DEFAULT);

   RTR_lock(RTR,handle);

   new_array = (BYTE *)add_ptr(RTR_addr(objlist[current_this]),array_offset);

   ptr = (BYTE *)RTR_addr(handle);

   switch (*(UWORD *) ptr)
      {
      case ':S':
         far_memmove(new_array,ptr+2,RTR_size(handle)-2L);
         break;

      default:
         abend(MSG_SRRLS);
      }

   RTR_unlock(handle);
}
     
//
// Load a resource into a SOP instance's array
//
// Determines array offset in instance, in case resource load causes
// instance to move in memory
//
// WARNING: The array must not be of automatic or external scope!
//

#pragma off (unreferenced)
void load_resource(LONG argcnt, BYTE *array, ULONG resource)
#pragma on (unreferenced)
{
   HRES handle;
   ULONG array_offset;
   BYTE *new_array;

   // TODO: Double-check removal of FP_OFF
   array_offset = array - RTR_addr(objlist[current_this]);

   handle = RTR_get_resource_handle(RTR,resource,DA_DEFAULT);

   RTR_lock(RTR,handle);

   new_array = (BYTE *)add_ptr(RTR_addr(objlist[current_this]),array_offset);

   far_memmove(new_array,RTR_addr(handle),RTR_size(handle));

   RTR_unlock(handle);
}

#pragma off (unreferenced)
void copy_string(LONG argcnt, BYTE *src, BYTE *dest)
#pragma on (unreferenced)
{
   strcpy((char *)dest, (const char *)src);
}

#pragma off (unreferenced)
void string_force_lower(LONG argcnt, BYTE *dest)
#pragma on (unreferenced)
{
   strlwr((char *)dest);
}

#pragma off (unreferenced)
void string_force_upper(LONG argcnt, BYTE *dest)
#pragma on (unreferenced)
{
   strupr((char *)dest);
}

#pragma off (unreferenced)
ULONG string_len(LONG argcnt, BYTE *string)
#pragma on (unreferenced)
{
   return strlen((const char *)string);
}

#pragma off (unreferenced)
ULONG string_compare(LONG argcnt, BYTE *str1, BYTE *str2)
#pragma on (unreferenced)
{
   return scumm_stricmp((const char *)str1, (const char *)str2);
}

//
// Return numeric value of string, or -1 if not valid string
//

#pragma off (unreferenced)
LONG strval(LONG argcnt, BYTE *string)
#pragma on (unreferenced)
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

#pragma off (unreferenced)
LONG envval(LONG argcnt, BYTE *name)
#pragma on (unreferenced)
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

void beep(void)
{
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

#pragma off (unreferenced)
void pokemem(LONG argcnt, LONG *addr, LONG data)
#pragma on (unreferenced)
{
   *addr = data;
}

#pragma off (unreferenced)
LONG peekmem(LONG argcnt, LONG *addr)
#pragma on (unreferenced)
{
   return *addr;
}

#pragma off (unreferenced)
ULONG rnd(LONG argcnt, ULONG low, ULONG high)
#pragma on (unreferenced)
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

#pragma off (unreferenced)
ULONG dice(LONG argcnt, ULONG ndice, ULONG nsides, ULONG bonus)
#pragma on (unreferenced)
{
   ULONG n,total;

   total = bonus;

   for (n=0;n<ndice;n++)
      total += rnd(0,1,nsides);

   return total;
}

ULONG inkey(void)
{
   return (ULONG) kbhit();
}

#pragma off (unreferenced)
ULONG absv(LONG argcnt, LONG val)
#pragma on (unreferenced)
{
   return (val < 0L) ? -val:val;
}

#pragma off (unreferenced)
LONG minv(LONG argcnt, LONG val1, LONG val2)
#pragma on (unreferenced)
{
   return MIN(val1,val2);
}

#pragma off (unreferenced)
LONG maxv(LONG argcnt, LONG val1, LONG val2)
#pragma on (unreferenced)
{
   return MAX(val1,val2);
}

#pragma off (unreferenced)
void diagnose(LONG argcnt, ULONG dtype, ULONG parm)
#pragma on (unreferenced)
{
   switch (dtype)
      {
      case 1:
         warning("%X ", parm);
         break;

      case 2:
         diag_flag = parm;
         break;
      }
}

ULONG heapfree(void)
{
   return RTR->free;
}

} // End of namespace Aesop
