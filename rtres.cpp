//лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
//лл                                                                        лл
//лл  RTRES.C                                                               лл
//лл                                                                        лл
//лл  AESOP runtime memory, resource, and data structure management         лл
//лл                                                                        лл
//лл  Version: 1.00 of 6-May-92 -- Initial version                          лл
//лл                                                                        лл
//лл  Project: Extensible State-Object Processor (AESOP/16)                 лл
//лл   Author: John Miles                                                   лл
//лл                                                                        лл
//лл  C source compatible with Borland C++ v3.0                             лл
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
#include "common/file.h"
#include "common/textconsole.h"
#include "aesop/defs.h"
#include "aesop/rtsystem.h"
#include "aesop/rtmsg.h"
#include "aesop/rtres.h"
#include "aesop/rt.h"
#include "aesop/intrface.h"
#include "aesop/graphics.h"   // for dprint()
#include "aesop/stubs.h"
//#include "aesop/modsnd32.h"

namespace Aesop {

#define FAST_LOCK 1     // user field = resource name if 0, file offset if 1

/******************************************/
//
// Diagnostic code
//
/******************************************/

#if 0
ULONG RTR_chksum(HRES entry)
{
   HD_entry *sel;
   UBYTE *ptr;
   ULONG chksum;

   sel = (HD_entry *) entry;
   ptr = sel->seg;

   chksum = 0;
   while ( ((ULONG) ptr) < ((ULONG)(sel->seg) + sel->size) )
      {
      chksum += (ULONG) *ptr;
      ptr++;
      }

   return (ULONG) chksum;
}

void RTR_double_check(ULONG caller, HRES entry)
{
   HD_entry *sel;
   UBYTE *ptr;
   ULONG chksum;
   UBYTE *cname;

   sel = (HD_entry *) entry;
   ptr = sel->seg;

   chksum = 0;
   while ( ((ULONG) ptr) < ((ULONG)(sel->seg) + sel->size) )
      {
      chksum += (ULONG) *ptr;
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
   ULONG res = CHKRES;
   LONG i;

   if (CHKRES == 0)
      return;

   if (RTR==NULL)
      return;

   for (i=0;i<RTR->nentries;i++)
      if (RTR->dir[i].user == res)
         break;

   if (i!=RTR->nentries)
      {
      if ( !(RTR->dir[i].flags & DA_DISCARDED) )
         RTR_double_check((ULONG)situation, (HRES) &RTR->dir[i]);
      else
         printf("Resource # %d discarded %s\n",res, situation);
      }
}

void RTR_flag_check(BYTE *situation)
{
   HD_entry *sel;

   if ((ULONG) situation > 10)
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

//   RTR_double_check((ULONG)situation, (HRES) HRES_to_check);
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

static ULONG RTR_discard(RTR_class *RTR, ULONG index, ULONG do_move)
{
   ULONG i,n;
   void *dest, *src;
   ULONG nbytes,size;
   HD_entry *sel;

   sel = &RTR->dir[index];

   size = sel->size;
   dest = sel->seg;
   src = add_ptr(dest,size);
   nbytes = ptr_dif(RTR->next_M, src);

   n = RTR->nentries;
   for (i=0;i<n;i++)
      {
      if (RTR->dir[i].flags & (DA_FIXED | DA_DISCARDED))
         continue;

      if (ptr_dif(RTR->dir[i].seg,dest) > 0L)
         RTR->dir[i].seg = (BYTE *) RTR->dir[i].seg - size;
      }

   if (do_move)
      {
      PollMod();
      far_memmove(dest,src,nbytes);
      }

   sel->flags |= DA_DISCARDED;

   RTR->next_M = (BYTE *) RTR->next_M - size;
   RTR->free = RTR->free + size;

   if (sel->flags & DA_EVANESCENT)
      {
      RTR_free(RTR,(HRES) sel);
      }

   return size;
}

/***************************************************/
//                                                   
// Return index of least-recently-used, unlocked, MOVEABLE resource
//
/***************************************************/

static ULONG RTR_LRU(RTR_class *RTR)
{
   ULONG i,oldest;
   ULONG n,age;

   n = RTR->nentries;
   oldest = age = -1;

   for (i=0;i<n;i++)
      {
      if (RTR->dir[i].flags &
         (DA_FIXED | DA_PRECIOUS | DA_DISCARDED | DA_FREE))
         continue;

      if (RTR->dir[i].locks > 0)
         continue;

      if (RTR->dir[i].history < age)
         {
         age = RTR->dir[i].history;
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
//       If no LRU candidates available, return false
//       
// 6) Return TRUE
//
/***************************************************/

static ULONG RTR_make_room(RTR_class *RTR, ULONG goal)
{
   LONG i;
   WORD first,next;
   ULONG index;
   ULONG next_seg,size_deleted,nbytes; 
   void *dest,*src,*end;

   //
   // 1) If /goal/ bytes already free, return immediately
   //

   if (RTR->free >= goal) return 1;

   standby_cursor();

   //
   // 2) Discard all MOVABLE FREE entries which are still present
   //

   for (i=RTR->nentries-1;i>=0;i--)
      if ((RTR->dir[i].flags & DA_FREE) &&
        (!(RTR->dir[i].flags & (DA_FIXED | DA_DISCARDED))))
         {
         RTR_discard(RTR,i,1);
         }

   //
   // 3) If /goal/ bytes free, return
   //

   if (RTR->free >= goal)
      {
      resume_cursor();
      return 1;
      }

   //
   // 4) Discard all unlocked TEMPORARY entries
   //

   for (first=RTR->nentries-1;first>=0;first--)
      {
      if (RTR->dir[first].flags &
         (DA_FIXED | DA_PRECIOUS | DA_DISCARDABLE | DA_DISCARDED))
         continue;

      if (RTR->dir[first].locks > 0)
         continue;
#if 1
      //
      // Found at least one unlocked TEMPORARY entry; try to merge adjacent
      // entries' blocks to avoid redundant calls to far_memmove()
      //

      for (next = first-1; next >= 0; next--)
         {
         next_seg = (ULONG) RTR->dir[next+1].seg;

         if (RTR->dir[next].flags &
            (DA_FIXED | DA_PRECIOUS | DA_DISCARDABLE | DA_DISCARDED))
            break;

         if (RTR->dir[next].locks > 0)
            break;

         if ( ((ULONG)RTR->dir[next].seg + RTR->dir[next].size) != next_seg )
            break;
         }

      end = RTR->next_M;
      dest = RTR->dir[next+1].seg;

      size_deleted = 0L;
      for (i=next+1;i<=first;i++)
         size_deleted += RTR_discard(RTR,i,0);

      src = add_ptr(dest,size_deleted);
      nbytes = ptr_dif(end,src);

      PollMod();
      far_memmove(dest,src,nbytes);

      first = next+1;
#else
      RTR_discard(RTR,first,1);
#endif
      }

   //
   // 5) While free < goal
   //

   while (RTR->free < goal)
      {
      //
      // If no LRU candidates available, return false
      //       
      index = RTR_LRU(RTR);
      if (index == -1)
         {
         resume_cursor();
         return 0;
         }

      //       
      // Discard least-recently-used valid candidate
      //
      RTR_discard(RTR,index,1);
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

static ULONG RTR_assign_space(RTR_class *RTR, ULONG bytes, ULONG attrib,
   HRES entry)
{
   HD_entry *sel;

   sel = (HD_entry *) entry;

   if (!RTR_make_room(RTR,bytes)) return 0;

   sel->flags = attrib & 0xffffffff;
   sel->locks = 0;
   sel->size = bytes;
   sel->history = RTR->LRU_cnt;

   RTR->free -= bytes;

   if (attrib & DA_FIXED)
      {
      RTR->last_F = (UBYTE *) RTR->last_F - bytes;
      sel->seg = RTR->last_F;
      }                                   
   else
      {
      sel->seg = RTR->next_M;
      RTR->next_M = (UBYTE *) RTR->next_M + bytes;
      }
   
   return 1;
}

/***************************************************/
//
// Initialize all entries in resource directory block
//
/***************************************************/

static void RTR_init_dir(RTR_class *RTR, ULONG first)
{
   ULONG i,j;

   for (i=0,j=first;i<DIR_BLK;i++,j++)
      {
      RTR->dir[j].size = 0L;
      RTR->dir[j].flags = DA_FREE | DA_DISCARDED;
      RTR->dir[j].history = 0;
      RTR->dir[j].locks = 0;
      RTR->dir[j].user = -1L;
      RTR->dir[j].seg = 0;
      }
}

/***************************************************/
//
// Return next available entry (-1 if error) in resource 
// directory, expanding directory if necessary
//
/***************************************************/

static HRES RTR_new_entry(RTR_class *RTR)
{
   ULONG i,f,n;
   ULONG nbytes;
   void *dest, *src;
   
   n = RTR->nentries;

   if (n >= (unsigned) (-DIR_BLK)) return -1U;

   for (i=0;i<n;i++)
      {
      f = RTR->dir[i].flags;

      if ((f & DA_FREE) && (f & DA_DISCARDED))
         return (HRES) &RTR->dir[i];
      }

   if (!RTR_make_room(RTR,SIZE_DB))
      return -1U;

   for (i=0;i<n;i++)
      {
      if (RTR->dir[i].flags & (DA_DISCARDED | DA_FIXED))
         continue;
      RTR->dir[i].seg = ((BYTE *) RTR->dir[i].seg) + SIZE_DB;
      }

   src    = add_ptr(RTR->dir ,n * sizeof(HD_entry));
   dest   = add_ptr(src, SIZE_DB);
   nbytes = ptr_dif(RTR->next_M, src);

   PollMod();
   far_memmove(dest,src,nbytes);

   RTR->next_M =(BYTE *) RTR->next_M + SIZE_DB;
   RTR->free -= SIZE_DB;
   RTR->nentries += DIR_BLK;

   RTR_init_dir(RTR,n);

   return (HRES) &RTR->dir[n];
}

/***************************************************/
//
// Read resource header to get data attribs & len, return w/file 
// pointer positioned at start of resource data
//
// Maintain copy of current ordinal directory block as part of 
// RTR structure to reduce excessive seeking
//
// Return size of resource in bytes
//
// Return 0 if resource was created by reference and never
// subsequently written (PLACEHOLDER) 
//
/***************************************************/

ULONG RTR_seek(RTR_class *RTR, ULONG rnum)
{
   ULONG dirblk,next;
   UWORD dirent;

   dirblk = (rnum / (ULONG) OD_SIZE);
   dirent = (UWORD) (rnum % (ULONG) OD_SIZE);

   if (RTR->cur_blk != dirblk)
      {
      RTR->cur_blk = dirblk;

      next = RTR->RFH.FOB;

      do
         {
			 RTR->file->seek(next,SEEK_SET);
			 error("TODO: OD_block::load");
			//read(RTR->file,&RTR->OD,sizeof(OD_block));
         next = RTR->OD.next;

         PollMod();
         }
      while (dirblk--);
      }

   RTR->file->seek(RTR->OD.index[dirent], SEEK_SET);
   error("TODO: RF_entry_hdr::load");
   //read(RTR->file,&RTR->REH,sizeof(RF_entry_hdr));

   if (RTR->REH.data_attrib & DA_PLACEHOLDER)
      return 0;

   return RTR->REH.data_size;
}

/***************************************************/
//
// Read resource image into cache at entry /entry/
//
// Assumes resource file pointer -> start at resource data
//
/***************************************************/

static void RTR_read(RTR_class *RTR, HRES entry)
{
   UBYTE *ptr;
   HD_entry *sel;
   ULONG len;

   sel = (HD_entry *) entry;

   ptr = (UBYTE *)sel->seg;
   len = sel->size;

   while (len > (ULONG) DOS_BUFFSIZE)
      {
      PollMod();

      RTR->file->read(ptr, DOS_BUFFSIZE);
      len -= DOS_BUFFSIZE;
      ptr += DOS_BUFFSIZE;
      }
   RTR->file->read(ptr,len);

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

RTR_class *RTR_construct(void *base, ULONG size, ULONG nnames, const char *filename)
{
   RTR_class *RTR;
   void *beg,*end;

   Common::File f;
   if (!f.open(filename)) return nullptr;

   RTR = (RTR_class *)mem_alloc(sizeof(RTR_class));
   RTR->file = f.readStream(f.size());

   error("TODO: RF_file_hdr::load");
   //read(RTR->file,&RTR->RFH,sizeof(RF_file_hdr));

   end = add_ptr(base,size);
   beg = base;

   RTR->base = base;

   RTR->nentries = DIR_BLK;
   RTR->dir = (HD_entry *)beg;

   RTR->next_M = add_ptr(beg,SIZE_DB);
   RTR->last_F = end;

   RTR->free = ptr_dif(RTR->last_F,RTR->next_M);

   RTR->LRU_cnt = 0;

   RTR_init_dir(RTR,0);

   RTR->cur_blk = (ULONG) -1L;

   RTR->name_dir = RTR_alloc(RTR,(ULONG) ((ULONG) nnames * sizeof(ND_entry)),
      DA_FIXED | DA_PRECIOUS);
   RTR->nd_entries = 0;

   return RTR;
}

/***************************************************/
//
// Destroy run-time resource cache class instance
//
// flags: See RTRES.H
//
/***************************************************/

void RTR_destroy(RTR_class *RTR, ULONG flags)
{
   delete RTR->file;

   if (flags & RTR_FREEBASE)
      {
      mem_free(RTR->base);
      }

   mem_free(RTR);
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

HRES RTR_alloc(RTR_class *RTR, ULONG bytes, ULONG attrib)
{
   HRES entry;
   HD_entry *sel;

   entry = RTR_new_entry(RTR);
   if (entry == -1U)
      return -1U;

   if (!RTR_assign_space(RTR,bytes,attrib,entry))
      return -1U;

   sel = (HD_entry *) entry;
   sel->user = -1L;

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

void RTR_free(RTR_class *RTR, HRES entry)
{
   ULONG i,n;
   HD_entry *sel;
   ND_entry *dir;

   if (entry == -1U) return;

   sel = (HD_entry *) entry;

   sel->flags |= DA_FREE;
   sel->locks = 0;

   for (i=0,dir = (ND_entry *)RTR_addr(RTR->name_dir);i<RTR->nd_entries;i++,dir++)
      if (dir->handle == entry)
         dir->handle = 0;
      else if (dir->thunk == entry)
         dir->thunk = -1U;

   if (!(sel->flags & DA_FIXED))
      {
      while ( ( !(sel->flags & DA_FIXED) ) &&
              ( !(sel->flags & DA_DISCARDED) ) &&
              ( (ULONG) sel->seg + sel->size == (ULONG) RTR->next_M )
            )
         {
         RTR_discard(RTR, (entry - (ULONG) RTR->dir)/sizeof(HD_entry), 1);

         n = RTR->nentries;
         for (i=0;i<n;i++)
            if ( ( !(RTR->dir[i].flags & DA_FIXED)       ) &&
                 (   RTR->dir[i].flags & DA_FREE         ) &&
                 ( !(RTR->dir[i].flags & DA_DISCARDED)   ) &&
                 (  (ULONG)RTR->dir[i].seg + sel->size == (ULONG)RTR->next_M )
               )
               {
               sel = &(RTR->dir[i]);
               entry = (HRES) sel;
               break;
               }
         }
      }
   else
      {  
      while  ((sel->flags & DA_FIXED)
         && (!(sel->flags & DA_DISCARDED))
         &&   (sel->seg == RTR->last_F))
         {
         RTR->last_F = (UBYTE *) RTR->last_F + sel->size;
         RTR->free += sel->size;
         sel->flags |= DA_DISCARDED;

         n = RTR->nentries;
         for (i=0;i<n;i++)
            if ((RTR->dir[i].flags & DA_FIXED)
            && (RTR->dir[i].flags & DA_FREE)
          && (!(RTR->dir[i].flags & DA_DISCARDED))
            && (RTR->dir[i].seg == RTR->last_F))
               {
               sel = &RTR->dir[i];
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

ULONG RTR_force_discard(RTR_class *RTR, ULONG goal)
{
   return RTR_make_room(RTR,goal);
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

void RTR_lock(RTR_class *RTR, HRES entry)
{
   ULONG i,n;
   HD_entry *sel;

   PollMod();

   sel = (HD_entry *) entry;

   if (sel->flags & (DA_FIXED | DA_PRECIOUS | DA_FREE))
      return;

   if ( (sel->flags & DA_DISCARDED) && (sel->user != -1L) )
      {
      if (RTR_assign_space(RTR,sel->size,sel->flags,entry) == -1U) 
         return;

#if FAST_LOCK
      RTR->file->seek(sel->user,SEEK_SET);
#else
      RTR_seek(RTR,sel->user);
#endif

      RTR_read(RTR,entry);

      sel->flags &= (~DA_DISCARDED);
      }

   ++sel->locks;

   sel->history = ++RTR->LRU_cnt;

   if (RTR->LRU_cnt == 65535U)
      {
      n = RTR->nentries;
      for (i=0;i<n;i++)
         RTR->dir[i].history >>= 3;

      RTR->LRU_cnt >>= 3;
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

ULONG RTR_size(HRES entry)
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
   return (void *) (* (ULONG *)entry);
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

void RTR_read_resource(RTR_class *RTR, void *dest, ULONG len)
{
   while (len > (ULONG) DOS_BUFFSIZE)
      {
      RTR->file->read(dest,DOS_BUFFSIZE);
      len -= DOS_BUFFSIZE;
      dest = add_ptr(dest,DOS_BUFFSIZE);
      }
   RTR->file->read(dest, len);
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

HRES RTR_load_resource(RTR_class *RTR, ULONG resource, ULONG attrib)
{
   HD_entry *sel;
   HRES entry;

   if (!RTR_seek(RTR,resource))
      return -1U;

   entry = RTR_alloc(RTR, RTR->REH.data_size,
      (attrib==DA_DEFAULT) ? RTR->REH.data_attrib : attrib);

   if (entry != -1U)
      {
      sel = (HD_entry *) entry;

#if FAST_LOCK
      sel->user = RTR->file->pos();
#else
      sel->user = resource;
#endif

      RTR_read(RTR,entry);
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

HRES RTR_get_resource_handle(RTR_class *RTR, ULONG resource, ULONG attrib)
{
   LONG i,insert,replace;
   ND_entry *dir;
   void *dest,*src;
   ULONG nbytes;

   dir = RTR_search_name_dir(RTR,resource);

   if (dir == NULL)
      {
      dir = (ND_entry *)RTR_addr(RTR->name_dir);

      for (i=replace=insert=0;i<RTR->nd_entries;i++)
         {
         if (dir->OE > resource)
            {
            insert=1;
            break;
            }
         
         if (dir->OE == resource)
            {
            replace=1;
            break;
            }
         
         dir++;
         }

      if (insert)
         {
         src = dir;
         dest = add_ptr(src,(ULONG) sizeof(ND_entry));
         nbytes = (ULONG) ((RTR->nd_entries - i) * sizeof(ND_entry));

         PollMod();
         far_memmove(dest,src,nbytes);
         }

      if (!replace)
         ++RTR->nd_entries;

      dir->OE = resource;
      dir->handle = RTR_load_resource(RTR,resource,attrib);
      dir->thunk = -1U;
      }

   if (dir->handle == -1U)
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

void RTR_free_resource(RTR_class *RTR, ULONG resource)
{
   ND_entry *dir;

   dir = RTR_search_name_dir(RTR,resource);

   if (dir == NULL)
      return;

   RTR_free(RTR,dir->handle);
}

/***************************************************/
//
// Perform binary search of name directory for specified resource
//
// Return pointer to name directory entry or NULL if entry 
// deleted or not found
//
/***************************************************/

ND_entry *RTR_search_name_dir(RTR_class *RTR, ULONG resource)
{
   LONG min,max,mid;
   ND_entry *dir,*tryEntry;
   ULONG entry;

   dir = (ND_entry *)RTR_addr(RTR->name_dir);

   min = 0;
   max = RTR->nd_entries-1;

   while (min <= max)
      {
      mid = (min+max) >> 1;

	  tryEntry = &dir[mid];

	  entry = tryEntry->OE;

      if (entry > resource)
         max = mid-1;
      else if (entry < resource)
         min = mid+1;
      else
		  return (tryEntry->handle == 0) ? nullptr : tryEntry;
      }

   return nullptr;
}

#if 0

/***************************************************/
//
// Show ASCII resource name
//
/***************************************************/

BYTE *ASCII_name(ULONG name)
{
   extern HRES HROED;
   static BYTE aname[35];
   UBYTE *dict;
   BYTE *tag,*def;
   ULONG num;
   LONG i;

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

void RTR_dump(RTR_class *RTR)
{
   ULONG i,j,f;
   HD_entry R;
   ULONG cs;
   UBYTE *ptr;
   ULONG cnt;
   extern ULONG heap_size;
   ULONG present;

   fprintf(stdout," # of entries: %u\n",RTR->nentries);
   fprintf(stdout,"         Size: %lu\n",heap_size);
   fprintf(stdout,"         Base: %Fp\n\n",RTR->base);

   fprintf(stdout,"Next moveable: %Fp\n",RTR->next_M);
   fprintf(stdout,"   Last fixed: %Fp\n",RTR->last_F);
   fprintf(stdout,"         Free: %lu\n\n",RTR->free);

   present = 0L;

   for (i=0;i<RTR->nentries;i++)
      {
      f = RTR->dir[i].flags;

      if ((f & DA_FREE) && (f & DA_DISCARDED) && (!RTR->dir[i].seg))
         break;
      }

   fprintf(stdout,"Entries avail: %u\n",RTR->nentries);
   fprintf(stdout,"       In use: %u\n\n",i);

   for (j=0;j<i;j++)
      {
      R = RTR->dir[j];

      for (cnt=0L,cs=0,ptr = RTR_addr(norm(&RTR->dir[j]));
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

} // End of namespace Aesop



