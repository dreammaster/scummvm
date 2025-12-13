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

#include "common/system.h"
#include "common/savefile.h"
#include "aesop/defs.h"
#include "aesop/rtsystem.h"
#include "aesop/rtmsg.h"
#include "aesop/rtres.h"
#include "aesop/rt.h"
#include "aesop/interface.h"
#include "aesop/graphics.h"   // for dprint()
#include "aesop/modsnd32.h"

namespace Aesop {

#define FAST_LOCK 1     // user field = resource name if 0, file offset if 1

/******************************************/
//
// Diagnostic code
//
/******************************************/

#if 0
uint32 RTR_chksum(HRES entry)
{
   HD_entry *sel;
   UBYTE *ptr;
   uint32 chksum;

   sel = (HD_entry *) entry;
   ptr = sel->seg;

   chksum = 0;
   while ( ((uint32) ptr) < ((uint32)(sel->seg) + sel->size) )
      {
      chksum += (uint32) *ptr;
      ptr++;
      }

   return (uint32) chksum;
}

void RTR_double_check(uint32 caller, HRES entry)
{
   HD_entry *sel;
   UBYTE *ptr;
   uint32 chksum;
   UBYTE *cname;

   sel = (HD_entry *) entry;
   ptr = sel->seg;

   chksum = 0;
   while ( ((uint32) ptr) < ((uint32)(sel->seg) + sel->size) )
      {
      chksum += (uint32) *ptr;
      ptr++;
      }

   if (sel->chksum != chksum)
      {
      if (caller > 10)
         {
         cname = (UBYTE *) caller;
         }
      else
         {
         switch (caller)
            {
            case 6:
               cname = "before RT_execute: do_END";
               break;
            case 5:
               cname = "in RT_execute";
               break;
            case 4:
               cname = "before get_res_handle";
               break;
            case 3:
               cname = "during PASS";
               break;
            case 2:
               cname = "during SEND";
               break;
            case 1:
               cname = "during LOCK";
               break;
            case 0:            
               cname = "during CALL";
               break;
            }
         }
      abend("RES# %d trashed %s.\n  HRES=%x -> %x  Size = %x  Flags = %x  Old=%x New=%x\n",
         sel->user, cname, sel, sel->seg, sel->size, sel->flags, sel->chksum, chksum);
      }

}

void resource_chksum(BYTE *situation)
{
   uint32 res = CHKRES;
   int32 i;

   if (CHKRES == 0)
      return;

   if (rtr==NULL)
      return;

   for (i=0;i<rtr->nentries;i++)
      if (rtr->dir[i].user == res)
         break;

   if (i!=rtr->nentries)
      {
      if ( !(rtr->dir[i].flags & DA_DISCARDED) )
         RTR_double_check((uint32)situation, (HRES) &rtr->dir[i]);
      else
         printf("Resource # %d discarded %s\n",res, situation);
      }
}

void RTR_flag_check(BYTE *situation)
{
   HD_entry *sel;

   if ((uint32) situation > 10)
      {
      sel = (HD_entry *) HRES_to_check;

      if ( (sel->flags & DA_DISCARDED) || (sel->flags & DA_FREE) )
         {
         abend("RES# %d flags trashed %s.\n  HRES=%x -> %x  Size = %x  Flags = %x \n",
            sel->user, situation, sel, sel->seg, sel->size, sel->flags);
         }
      }
}



void RTR_HRES_chksum(BYTE *situation)
{
   if (HRES_to_check == 0)
      return;

   if (check_on == 0)
      return;

//   RTR_flag_check(situation);

//   RTR_double_check((uint32)situation, (HRES) HRES_to_check);
}
#endif

/***************************************************/
//
// Discard a given resource to expand free space
// in cache (MOVEABLE resources only)
//
// If do_move=0, caller is responsible for garbage collection
//
// If discarded object is EVANESCENT, free its entry
//
// Returns size of discarded object in bytes (rounded to next paragraph)
//
/***************************************************/

static uint32 RTR_discard(RTR_class *rtr, uint32 index, uint32 do_move)
{
   uint32 i,n;
   void *dest, *src;
   uint32 nbytes,size;
   HD_entry *sel;

   sel = &rtr->dir[index];

   size = sel->size;
   dest = sel->seg;
   src = add_ptr(dest,size);
   nbytes = ptr_dif(rtr->next_M, src);

   n = rtr->nentries;
   for (i=0;i<n;i++)
      {
      if (rtr->dir[i].flags & (DA_FIXED | DA_DISCARDED))
         continue;

      if (ptr_dif(rtr->dir[i].seg,dest) > 0L)
         rtr->dir[i].seg = (BYTE *) rtr->dir[i].seg - size;
      }

   if (do_move)
      {
      PollMod();
      far_memmove(dest,src,nbytes);
      }

   sel->flags |= DA_DISCARDED;

   rtr->next_M = (BYTE *) rtr->next_M - size;
   rtr->free = rtr->free + size;

   if (sel->flags & DA_EVANESCENT)
      {
      RTR_free(rtr,(HRES) sel);
      }

   return size;
}

/***************************************************/
//                                                   
// Return index of least-recently-used, unlocked, MOVEABLE resource
//
/***************************************************/

static uint32 RTR_LRU(RTR_class *rtr)
{
   uint32 i,oldest;
   uint32 n,age;

   n = rtr->nentries;
   oldest = age = (uint32)-1;

   for (i=0;i<n;i++)
      {
      if (rtr->dir[i].flags &
         (DA_FIXED | DA_PRECIOUS | DA_DISCARDED | DA_FREE))
         continue;

      if (rtr->dir[i].locks > 0)
         continue;

      if (rtr->dir[i].history < age)
         {
         age = rtr->dir[i].history;
         oldest = i;
         }
      }

   return oldest;
}

/***************************************************/
//
// Create /goal/ bytes of free space in resource cache:
//
// 1) If /goal/ bytes already free, return immediately
//
// 2) Discard all MOVABLE FREE entries which are still present
//
// 3) If /goal/ bytes free, return
//
// 4) Discard all unlocked TEMPORARY entries
//
// 5) While free < goal
//
//       Discard least-recently-used valid candidate
//
//       If no LRU candidates available, return FALSE
//       
// 6) Return TRUE
//
/***************************************************/

static uint32 RTR_make_room(RTR_class *rtr, uint32 goal)
{
   int32 i;
   WORD first,next;
   uint32 index;
   uint32 next_seg,size_deleted,nbytes; 
   void *dest,*src,*end;

   //
   // 1) If /goal/ bytes already free, return immediately
   //

   if (rtr->free >= goal) return 1;

   standby_cursor();

   //
   // 2) Discard all MOVABLE FREE entries which are still present
   //

   for (i=rtr->nentries-1;i>=0;i--)
      if ((rtr->dir[i].flags & DA_FREE) &&
        (!(rtr->dir[i].flags & (DA_FIXED | DA_DISCARDED))))
         {
         RTR_discard(rtr,i,1);
         }

   //
   // 3) If /goal/ bytes free, return
   //

   if (rtr->free >= goal)
      {
      resume_cursor();
      return 1;
      }

   //
   // 4) Discard all unlocked TEMPORARY entries
   //

   for (first=rtr->nentries-1;first>=0;first--)
      {
      if (rtr->dir[first].flags &
         (DA_FIXED | DA_PRECIOUS | DA_DISCARDABLE | DA_DISCARDED))
         continue;

      if (rtr->dir[first].locks > 0)
         continue;
#if 1
      //
      // Found at least one unlocked TEMPORARY entry; try to merge adjacent
      // entries' blocks to avoid redundant calls to far_memmove()
      //

      for (next = first-1; next >= 0; next--)
         {
         next_seg = (uint32) rtr->dir[next+1].seg;

         if (rtr->dir[next].flags &
            (DA_FIXED | DA_PRECIOUS | DA_DISCARDABLE | DA_DISCARDED))
            break;

         if (rtr->dir[next].locks > 0)
            break;

         if ( ((uint32)rtr->dir[next].seg + rtr->dir[next].size) != next_seg )
            break;
         }

      end = rtr->next_M;
      dest = rtr->dir[next+1].seg;

      size_deleted = 0L;
      for (i=next+1;i<=first;i++)
         size_deleted += RTR_discard(rtr,i,0);

      src = add_ptr(dest,size_deleted);
      nbytes = ptr_dif(end,src);

      PollMod();
      far_memmove(dest,src,nbytes);

      first = next+1;
#else
      RTR_discard(rtr,first,1);
#endif
      }

   //
   // 5) While free < goal
   //

   while (rtr->free < goal)
      {
      //
      // If no LRU candidates available, return FALSE
      //       
      index = RTR_LRU(rtr);
      if (index == -1)
         {
         resume_cursor();
         return 0;
         }

      //       
      // Discard least-recently-used valid candidate
      //
      RTR_discard(rtr,index,1);
      }

   resume_cursor();

   return 1;
}

/***************************************************/
//
// Get /bytes/ free space for /entry/, establish selector fields
//
// Note: Does not initialize user field
//
/***************************************************/

static uint32 RTR_assign_space(RTR_class *rtr, uint32 bytes, uint32 attrib,
   HRES entry)
{
   HD_entry *sel;

   sel = (HD_entry *) entry;

   if (!RTR_make_room(rtr,bytes)) return 0;

   sel->flags = attrib & 0xffffffff;
   sel->locks = 0;
   sel->size = bytes;
   sel->history = rtr->LRU_cnt;

   rtr->free -= bytes;

   if (attrib & DA_FIXED)
      {
      rtr->last_F = (UBYTE *) rtr->last_F - bytes;
      sel->seg = rtr->last_F;
      }                                   
   else
      {
      sel->seg = rtr->next_M;
      rtr->next_M = (UBYTE *) rtr->next_M + bytes;
      }
   
   return 1;
}

/***************************************************/
//
// Initialize all entries in resource directory block
//
/***************************************************/

static void RTR_init_dir(RTR_class *rtr, uint32 first)
{
   uint32 i,j;

   for (i=0,j=first;i<DIR_BLK;i++,j++)
      {
      rtr->dir[j].size = 0L;
      rtr->dir[j].flags = DA_FREE | DA_DISCARDED;
      rtr->dir[j].history = 0;
      rtr->dir[j].locks = 0;
      rtr->dir[j].user = (uint32)-1;
      rtr->dir[j].seg = 0;
      }
}

/***************************************************/
//
// Return next available entry (-1 if error) in resource 
// directory, expanding directory if necessary
//
/***************************************************/

static HRES RTR_new_entry(RTR_class *rtr)
{
   uint32 i,f,n;
   uint32 nbytes;
   void *dest, *src;
   
   n = rtr->nentries;

   if (n >= (unsigned) (-DIR_BLK)) return (HRES)-1;

   for (i=0;i<n;i++)
      {
      f = rtr->dir[i].flags;

      if ((f & DA_FREE) && (f & DA_DISCARDED))
         return (HRES) &rtr->dir[i];
      }

   if (!RTR_make_room(rtr,SIZE_DB))
      return (HRES)-1;

   for (i=0;i<n;i++)
      {
      if (rtr->dir[i].flags & (DA_DISCARDED | DA_FIXED))
         continue;
      rtr->dir[i].seg = ((BYTE *) rtr->dir[i].seg) + SIZE_DB;
      }

   src    = add_ptr(rtr->dir ,n * sizeof(HD_entry));
   dest   = add_ptr(src, SIZE_DB);
   nbytes = ptr_dif(rtr->next_M, src);

   PollMod();
   far_memmove(dest,src,nbytes);

   rtr->next_M =(BYTE *) rtr->next_M + SIZE_DB;
   rtr->free -= SIZE_DB;
   rtr->nentries += DIR_BLK;

   RTR_init_dir(rtr,n);

   return (HRES) &rtr->dir[n];
}

/***************************************************/
//
// Read resource header to get data attribs & len, return w/file 
// pointer positioned at start of resource data
//
// Maintain copy of current ordinal directory block as part of 
// rtr structure to reduce excessive seeking
//
// Return size of resource in bytes
//
// Return 0 if resource was created by reference and never
// subsequently written (PLACEHOLDER) 
//
/***************************************************/

uint32 RTR_seek(RTR_class *rtr, uint32 rnum) {
	uint32 dirblk, next;
	UWORD dirent;

	dirblk = (rnum / (uint32)OD_SIZE);
	dirent = (UWORD)(rnum % (uint32)OD_SIZE);

	if (rtr->cur_blk != dirblk)
	{
		rtr->cur_blk = dirblk;

		next = rtr->RFH.FOB;

		do
		{
			rtr->file->seek(next, SEEK_SET);
			rtr->OD.load(rtr->file);
			next = rtr->OD.next;

			PollMod();
		} while (dirblk--);
	}

	rtr->file->seek(rtr->OD.index[dirent], SEEK_SET);
	rtr->REH.load(rtr->file);

	if (rtr->REH.data_attrib & DA_PLACEHOLDER)
		return 0;

	return rtr->REH.data_size;
}

/***************************************************/
//
// Read resource image into cache at entry /entry/
//
// Assumes resource file pointer -> start at resource data
//
/***************************************************/

static void RTR_read(RTR_class *rtr, HRES entry)
{
   UBYTE *ptr;
   HD_entry *sel;
   uint32 len;

   sel = (HD_entry *) entry;

   ptr = (UBYTE *)sel->seg;
   len = sel->size;

   while (len > (uint32)DOS_BUFFSIZE) {
	   PollMod();

	   rtr->file->read(ptr, DOS_BUFFSIZE);
	   len -= DOS_BUFFSIZE;
	   ptr += DOS_BUFFSIZE;
   }

   rtr->file->read(ptr, len);

   PollMod();
}

/***************************************************/
//
// Create run-time resource cache class instance
//
//     base: Address in memory for resource cache (need not be aligned)
//
//     size: Cache size in bytes (need not be paragraph multiple)
//
//   nnames: Max. # of entries in cache's name directory
//
// filename: Name of resource file
//
/***************************************************/

RTR_class *RTR_construct(void *base, uint32 size, uint32 nnames, const char *filename) {
   RTR_class *rtr;
   void *beg,*end;

   rtr = (RTR_class *)mem_alloc(sizeof(RTR_class));

   rtr->file = g_system->getSavefileManager()->openForLoading(filename);
   if (rtr->file == nullptr) return NULL;

   rtr->RFH.load(rtr->file);

   end = add_ptr(base,size);
   beg = base;

   rtr->base = base;

   rtr->nentries = DIR_BLK;
   rtr->dir = (HD_entry *)beg;

   rtr->next_M = add_ptr(beg,SIZE_DB);
   rtr->last_F = end;

   rtr->free = ptr_dif(rtr->last_F,rtr->next_M);

   rtr->LRU_cnt = 0;

   RTR_init_dir(rtr,0);

   rtr->cur_blk = (uint32) -1L;

   rtr->name_dir = RTR_alloc(rtr,(uint32) ((uint32) nnames * sizeof(ND_entry)),
      DA_FIXED | DA_PRECIOUS);
   rtr->nd_entries = 0;

   return rtr;
}

/***************************************************/
//
// Destroy run-time resource cache class instance
//
// flags: See RTRES.H
//
/***************************************************/

void RTR_destroy(RTR_class *rtr, uint32 flags) {
	delete rtr->file;
	rtr->file = nullptr;

	if (flags & RTR_FREEBASE) {
		mem_free(rtr->base);
	}

	mem_free(rtr);
}

/***************************************************/
//
// Allocate /bytes/ of memory in resource cache and return segment
// address of entry descriptor (handle)
//
// Initializes user field to -1
//
// Returns -1 if sufficient memory could not be allocated
//
// Currently only named resources (resource file entries) should be
// marked DISCARDABLE, since the client is not notified when a block
// is discarded
//
/***************************************************/

HRES RTR_alloc(RTR_class *rtr, uint32 bytes, uint32 attrib) {
	HRES entry;
	HD_entry *sel;

	entry = RTR_new_entry(rtr);
	if (entry == (HRES)-1)
		return (HRES)-1;

	if (!RTR_assign_space(rtr, bytes, attrib, entry))
		return (HRES)-1;

	sel = (HD_entry *)entry;
	sel->user = (uint32)-1;

	return entry;
}

/***************************************************/
//
// Release cache memory previously allocated with RTR_alloc()
//
// Also frees name directory entry, if used
//
// If block being freed is last MOVABLE or first FIXED, and it hasn't
// already been discarded, discard it (along with any free neighbors
// if FIXED; this is the only way FIXED entries can ever be discarded at
// present)
//
/***************************************************/

void RTR_free(RTR_class *rtr, HRES entry)
{
   uint32 i,n;
   HD_entry *sel;
   ND_entry *dir;

   if (entry == (HRES)-1)
	   return;

   sel = (HD_entry *) entry;

   sel->flags |= DA_FREE;
   sel->locks = 0;

   for (i=0,dir = (ND_entry *)RTR_addr(rtr->name_dir); i < (uint32)rtr->nd_entries; i++, dir++)
      if (dir->handle == entry)
         dir->handle = 0;
      else if (dir->thunk == entry)
         dir->thunk = (HRES)-1;

   if (!(sel->flags & DA_FIXED))
      {
      while ( ( !(sel->flags & DA_FIXED) ) &&
              ( !(sel->flags & DA_DISCARDED) ) &&
              ( (uint32) sel->seg + sel->size == (uint32) rtr->next_M )
            )
         {
         RTR_discard(rtr, (entry - (uint32) rtr->dir)/sizeof(HD_entry), 1);

         n = rtr->nentries;
         for (i=0;i<n;i++)
            if ( ( !(rtr->dir[i].flags & DA_FIXED)       ) &&
                 (   rtr->dir[i].flags & DA_FREE         ) &&
                 ( !(rtr->dir[i].flags & DA_DISCARDED)   ) &&
                 (  (uint32)rtr->dir[i].seg + sel->size == (uint32)rtr->next_M )
               )
               {
               sel = &(rtr->dir[i]);
               entry = (HRES) sel;
               break;
               }
         }
      }
   else
      {  
      while  ((sel->flags & DA_FIXED)
         && (!(sel->flags & DA_DISCARDED))
         &&   (sel->seg == rtr->last_F))
         {
         rtr->last_F = (UBYTE *) rtr->last_F + sel->size;
         rtr->free += sel->size;
         sel->flags |= DA_DISCARDED;

         n = rtr->nentries;
         for (i=0;i<n;i++)
            if ((rtr->dir[i].flags & DA_FIXED)
            && (rtr->dir[i].flags & DA_FREE)
          && (!(rtr->dir[i].flags & DA_DISCARDED))
            && (rtr->dir[i].seg == rtr->last_F))
               {
               sel = &rtr->dir[i];
               break;
               }
         }
      }
}

/***************************************************/
//
// Discard entries until amount of space is available in cache 
// (-1UL for maximum)
//
// Returns 1 on success, 0 if space could not be obtained
//
/***************************************************/

uint32 RTR_force_discard(RTR_class *rtr, uint32 goal)
{
   return RTR_make_room(rtr,goal);
}

/***************************************************/
//
// "Lock" a resource cache entry
// 
// A locked resource cannot be automatically discarded until the lock count is
// decremented to zero via call(s) to RTR_unlock()
// 
// Locking does NOT guarantee that the resource will not be moved when other 
// resources are discarded, only that it will always be instantly accessible
// by dereferencing the handle segment pointer
//
/***************************************************/

void RTR_lock(RTR_class *rtr, HRES entry)
{
   uint32 i,n;
   HD_entry *sel;

   PollMod();

   sel = (HD_entry *) entry;

   if (sel->flags & (DA_FIXED | DA_PRECIOUS | DA_FREE))
      return;

   if ( (sel->flags & DA_DISCARDED) && (sel->user != -1L) )
      {
      if (RTR_assign_space(rtr,sel->size,sel->flags,entry) == (uint32)-1)
         return;

#if FAST_LOCK
      rtr->file->seek(sel->user, SEEK_SET);
#else
      RTR_seek(rtr,sel->user);
#endif

      RTR_read(rtr,entry);

      sel->flags &= (~DA_DISCARDED);
      }

   ++sel->locks;

   sel->history = ++rtr->LRU_cnt;

   if (rtr->LRU_cnt == 65535U)
      {
      n = rtr->nentries;
      for (i=0;i<n;i++)
         rtr->dir[i].history >>= 3;

      rtr->LRU_cnt >>= 3;
      }
}

/***************************************************/
//
// "Unlock" a resource cache entry
// 
// A locked resource cannot be automatically discarded until the lock count is
// decremented to zero via call(s) to RTR_unlock()
// 
/***************************************************/

void RTR_unlock(HRES entry)
{
   HD_entry *sel;

   sel = (HD_entry *) entry;

   if (sel->locks > 0)
      --sel->locks;
}

/***************************************************/
//
// Return the size of a cached resource (not necessarily
// present)
//
/***************************************************/

uint32 RTR_size(HRES entry)
{
   HD_entry *sel;

   sel = (HD_entry *) entry;

   return sel->size;
}

/***************************************************/
//
// Return the current address of a cached resource
//
// Addresses of FIXED resources may safely be stored and re-used at will 
// by the client program, since FIXED resources are neither MOVEABLE nor
// (at least for the time being) DISCARDABLE.
//
// However, addresses of MOVEABLE resources may change any time a block of
// memory is allocated from the resource cache.  Additionally, DISCARDABLE
// resources MUST be locked via RTR_lock() before calling this function.
// Do not expect pointers to MOVEABLE resources to remain valid after 
// any function which can load resources or otherwise allocate cache memory.
// The RTR_fix_segment() function may be used to adjust segment addresses
// of moveable resources.
//
// This function is normally implemented via a macro #defined in RTRES.H.
//
/***************************************************/

#ifndef RTR_addr

void *RTR_addr(HRES entry)
{
   return (void *) (* (uint32 *)entry);
}

#endif

/***************************************************/
//
// Read from resource file into a specified block of memory
//
// RTR_seek() must first have been called to set the resource file
// pointer position to the desired resource
//
/***************************************************/

void RTR_read_resource(RTR_class *rtr, void *dest, uint32 len) {
	while (len > (uint32)DOS_BUFFSIZE) {
		rtr->file->read(dest, DOS_BUFFSIZE);
		len -= DOS_BUFFSIZE;
		dest = add_ptr(dest, DOS_BUFFSIZE);
	}

	rtr->file->read(dest, len);
}

/***************************************************/
//
// Allocate cache memory and load resource /resource/ with attributes
// /attrib/
//
// Use resource's default attributes if /attrib/ = DA_DEFAULT
//
// Normally this function should not be called to load named resources,
// since RTR_get_resource_handle() maintains a name directory to avoid
// redundantly loading named resources
//
// Returns -1U if resource could not be loaded
//
/***************************************************/

HRES RTR_load_resource(RTR_class *rtr, uint32 resource, uint32 attrib) {
	HD_entry *sel;
	HRES entry;

	if (!RTR_seek(rtr, resource))
		return (HRES)-1;

	entry = RTR_alloc(rtr, rtr->REH.data_size,
		(attrib == DA_DEFAULT) ? rtr->REH.data_attrib : attrib);

	if (entry != (HRES)-1) {
		sel = (HD_entry *)entry;

#if FAST_LOCK
		sel->user = rtr->file->pos();
#else
		sel->user = resource;
#endif

		RTR_read(rtr, entry);
	}

	return entry;
}

/***************************************************/
//
// Return handle to cached image of resource /resource/
// (or 0 if not valid resource)
//
// If /resource/ has not been loaded, load it and enter its
// handle in the (sorted) resource name directory for faster access
// in future calls to this function
//
// This is the preferred way to access most resources in the resource file
//
/***************************************************/

HRES RTR_get_resource_handle(RTR_class *rtr, uint32 resource, uint32 attrib) {
	int32 i, insert, replace;
	ND_entry *dir;
	void *dest, *src;
	uint32 nbytes;

	dir = RTR_search_name_dir(rtr, resource);

	if (dir == NULL) {
		dir = (ND_entry *)RTR_addr(rtr->name_dir);

		for (i = replace = insert = 0; i < rtr->nd_entries; i++)
		{
			if (dir->OE > resource)
			{
				insert = 1;
				break;
			}

			if (dir->OE == resource)
			{
				replace = 1;
				break;
			}

			dir++;
		}

		if (insert)
		{
			src = dir;
			dest = add_ptr(src, (uint32)sizeof(ND_entry));
			nbytes = (uint32)((rtr->nd_entries - i) * sizeof(ND_entry));

			PollMod();
			far_memmove(dest, src, nbytes);
		}

		if (!replace)
			++rtr->nd_entries;

		dir->OE = resource;
		dir->handle = RTR_load_resource(rtr, resource, attrib);
		dir->thunk = (HRES)-1;
	}

	if (dir->handle == (HRES)-1)
		dir->handle = 0;

	return dir->handle;
}

/***************************************************/
//
// Free a resource by name
//
// This function allows resource cache entries to be freed based on
// the resource ordinal entry number (or "name") of the cached entry,
// rather than the entry's handle
//
/***************************************************/

void RTR_free_resource(RTR_class *rtr, uint32 resource)
{
   ND_entry *dir;

   dir = RTR_search_name_dir(rtr,resource);

   if (dir == NULL)
      return;

   RTR_free(rtr,dir->handle);
}

/***************************************************/
//
// Perform binary search of name directory for specified resource
//
// Return pointer to name directory entry or NULL if entry 
// deleted or not found
//
/***************************************************/

ND_entry *RTR_search_name_dir(RTR_class *rtr, uint32 resource)
{
   int32 min,max,mid;
   ND_entry *dir, *try_;
   uint32 entry;

   dir = (ND_entry *)RTR_addr(rtr->name_dir);

   min = 0;
   max = rtr->nd_entries-1;

   while (min <= max)
      {
      mid = (min+max) >> 1;

      try_ = &dir[mid];

      entry = try_->OE;

      if (entry > resource)
         max = mid-1;
      else if (entry < resource)
         min = mid+1;
      else
         return (try_->handle == 0) ? NULL : try_;
      }

   return NULL;
}

#if 0

/***************************************************/
//
// Show ASCII resource name
//
/***************************************************/

BYTE *ASCII_name(uint32 name)
{
   extern HRES HROED;
   static BYTE aname[35];
   UBYTE *dict;
   BYTE *tag,*def;
   uint32 num;
   int32 i;

   dict = RTD_first(RTR_addr(HROED));
   while ((dict = RTD_iterate(RTR_addr(HROED),dict,&tag,&def)) != NULL)
      {
      num = ascnum(def);

      if (num == name)
         break;
      }

   if (dict == NULL)
      sprintf(aname,"Not found (%ld)",name);
   else
      strncpy(aname,tag,34);

   for (i=strlen(aname);i<34;i++)
      aname[i] = ' ';

   aname[34] = 0;

   return aname;
}

/***************************************************/
//
// Show resource cache stats (for diagnostic purposes)
//
/***************************************************/

void RTR_dump(RTR_class *rtr)
{
   uint32 i,j,f;
   HD_entry R;
   uint32 cs;
   UBYTE *ptr;
   uint32 cnt;
   extern uint32 heap_size;
   uint32 present;

   fprintf(stdout," # of entries: %u\n",rtr->nentries);
   fprintf(stdout,"         Size: %lu\n",heap_size);
   fprintf(stdout,"         Base: %Fp\n\n",rtr->base);

   fprintf(stdout,"Next moveable: %Fp\n",rtr->next_M);
   fprintf(stdout,"   Last fixed: %Fp\n",rtr->last_F);
   fprintf(stdout,"         Free: %lu\n\n",rtr->free);

   present = 0L;

   for (i=0;i<rtr->nentries;i++)
      {
      f = rtr->dir[i].flags;

      if ((f & DA_FREE) && (f & DA_DISCARDED) && (!rtr->dir[i].seg))
         break;
      }

   fprintf(stdout,"Entries avail: %u\n",rtr->nentries);
   fprintf(stdout,"       In use: %u\n\n",i);

   for (j=0;j<i;j++)
      {
      R = rtr->dir[j];

      for (cnt=0L,cs=0,ptr = RTR_addr(norm(&rtr->dir[j]));
         cnt<R.size;cnt++)
         cs += ptr[cnt];

      fprintf(stdout,"%s %.06lu bytes, lock %.02d, LRU %.05u) ",
         ASCII_name(R.user),R.size,R.locks,R.history);

      if (R.flags & DA_FIXED)
         fprintf(stdout,"Fix ");
      else
         fprintf(stdout,"Mov ");

      if (R.flags & DA_PRECIOUS) fprintf(stdout,"Prec ");
      if (R.flags & DA_DISCARDABLE) fprintf(stdout,"D'ble ");
      if (R.flags & DA_EVANESCENT) fprintf(stdout,"Ev ");

      if (R.flags & DA_DISCARDED) fprintf(stdout,"DISC ");
      if (R.flags & DA_FREE) fprintf(stdout,"FREE ");

      if (!(R.flags & (DA_DISCARDED | DA_FREE)))
         present += R.size;

      fprintf(stdout,"\n");
      }

   fprintf(stdout,"\n%lu bytes present\n",present);
}

#endif

/*--------------------------------------------*/

void RF_file_hdr::load(Common::SeekableReadStream *rs) {
	rs->read(signature, 16);
	file_size = rs->readUint32LE();
	lost_space = rs->readUint32LE();
	FOB = rs->readUint32LE();
	create_time = rs->readUint32LE();
	modify_time = rs->readUint32LE();
}

/*--------------------------------------------*/

void RF_entry_hdr::load(Common::SeekableReadStream *rs) {
	timestamp = rs->readUint32LE();
	data_attrib = rs->readUint32LE();
	data_size = rs->readUint32LE();
}

/*--------------------------------------------*/

void OD_block::load(Common::SeekableReadStream *rs) {
	next = rs->readUint32LE();
	rs->read(flags, OD_SIZE);
	for (int i = 0; i < OD_SIZE; ++i)
		index[i] = rs->readUint32LE();
}

} // namespace Aesop
