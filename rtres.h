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

//
//  Runtime heap/resource class
//

#ifndef AESOP_RTRES_H
#define AESOP_RTRES_H

#include "common/stream.h"

namespace Aesop {

#define MAX_OBJ_TYPES  2450      // # of possible resource names
#define DIR_BLK        256       // # of cache directory entries/block
#define OD_SIZE        128       // # of entries/ordinal file directory block

#define DA_FIXED       0x00000001U   // Entry attribute equates
#define DA_MOVEABLE    0x00000000U   // (only low word preserved in cache)
#define DA_PRECIOUS    0x00000020U
#define DA_DISCARDABLE 0x00000010U
#define DA_TEMPORARY   0x00000000U

#define DA_EVANESCENT  0x00000040U   // Special runtime attribute equates
#define DA_DISCARDED   0x00000100U
#define DA_FREE        0x00000200U
#define DA_DEFAULT     0xFFFFFFFFU

#define DA_PLACEHOLDER 0x10000000L

#define SA_UNUSED      0x00000001    // Storage attribute flag equates
#define SA_DELETED     0x00000002
                        
#define ROED 0                   // Resource Ordinal Entry Directory
#define RDES 1                   // Resource Description Directory
#define RDEP 2                   // Resource Dependency Directory
#define CRFD 3                   // Code Resource Function Directory

#define RTR_FREEBASE   0x0001U   // TRUE for destructor to free heap memory

#define SIZE_DB (DIR_BLK * sizeof(HD_entry))

#define DOS_BUFFSIZE 32768U

// cached resource entry descriptor
struct HD_entry {
   void *seg;                    // pointer to resource data

   ULONG size;                   // size of resource in bytes
   ULONG flags;                  // DA_ flags
   ULONG history;                // LRU counter value
   ULONG locks;                  // locking depth
   ULONG user;                   // .RES file offset or instance object name
};

// resource file header
struct RF_file_hdr {
   BYTE  signature[16];
   ULONG file_size;
   ULONG lost_space;
   ULONG FOB;
   ULONG create_time;
   ULONG modify_time;
};

// resource file entry header
struct RF_entry_hdr {
   ULONG timestamp;
   ULONG data_attrib;
   ULONG data_size;
};

struct OD_block {
   ULONG next;
   UBYTE flags[OD_SIZE];
   ULONG index[OD_SIZE];
};

// name directory entry
struct ND_entry {
   ULONG OE;                     // public
   HRES thunk;                   // public
   HRES handle;                  // public
};

struct RTR_class {
   HD_entry *dir;             // public
   UWORD nentries;            // public

   Common::SeekableReadStream *file;                    
   RF_file_hdr RFH;              
   RF_entry_hdr REH;             
   OD_block OD;                  
   ULONG cur_blk;              

   UWORD LRU_cnt;                    
   void *base;
   void *next_M;                 
   ULONG free;                   
   void *last_F;

   HRES name_dir;
   WORD nd_entries;
};

//
// RTR_addr macro returns current address of resource cache entry
// (See comments in RTRES.C)
//

#define RTR_addr(x) ((void *) (* (ULONG *) (x)))

//
// RTR_member macro allows access to HD_entry structure members
//

#define RTR_member(x,y)   ( (HD_entry *)(x)->y )

//
// Extern *RTR allows public access to application's main RTR class
//

extern RTR_class *LNK;
extern RTR_class *RTR;

RTR_class *RTR_construct(void *base, ULONG size, ULONG nnames, BYTE *filename);
void RTR_destroy(RTR_class *RTR, ULONG flags);

ULONG RTR_force_discard(RTR_class *RTR, ULONG goal);

HRES RTR_alloc(RTR_class *RTR, ULONG bytes, ULONG attrib);
void RTR_free(RTR_class *RTR, HRES entry);

void RTR_lock(RTR_class *RTR, HRES entry);
void RTR_unlock(HRES entry);

ULONG RTR_size(HRES entry);

HRES RTR_get_resource_handle(RTR_class *RTR, ULONG resource, ULONG attrib);
void RTR_free_resource(RTR_class *RTR, ULONG resource);

HRES RTR_load_resource(RTR_class *RTR, ULONG resource, ULONG attrib);
void RTR_read_resource(RTR_class *RTR, void *dest, ULONG len);
ULONG RTR_seek(RTR_class *RTR, ULONG rnum);

#ifndef RTR_addr
void *RTR_addr(HRES entry);
#endif
void RTR_fixup(void **ptr, HRES entry);

ND_entry *RTR_search_name_dir(RTR_class *RTR, ULONG resource);

BYTE *ASCII_name(ULONG name);
void RTR_dump(RTR_class *RTR);

void RTR_HRES_chksum(BYTE *situation);
ULONG RTR_chksum(HRES entry);

} // End of namespace Aesop

#endif

