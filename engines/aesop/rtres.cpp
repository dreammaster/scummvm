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

#include "aesop/rtres.h"
#include "aesop/rtsystem.h"
#include "aesop/rtmsg.h"
#include "aesop/rt.h"
#include "aesop/interface.h"
#include "aesop/graphics.h"   // for dprint()
#include "aesop/system/ail32.h"
#include "aesop/system/files.h"

namespace Aesop {

#define FAST_LOCK 1     // user field = resource name if 0, file offset if 1

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

static ULONG RTR_discard(RTR_class *rtr, ULONG index, ULONG do_move) {
	ULONG i, n;
	void *dest, *src;
	ULONG nbytes, size;
	HD_entry *sel;

	sel = &rtr->dir[index];

	size = sel->size;
	dest = sel->seg;
	src = add_ptr(dest, size);
	nbytes = ptr_dif(rtr->next_M, src);

	n = rtr->nentries;
	for (i = 0; i < n; i++)
	{
		if (rtr->dir[i].flags & (DA_FIXED | DA_DISCARDED))
			continue;

		if (ptr_dif(rtr->dir[i].seg, dest) > 0L)
			rtr->dir[i].seg = (BYTE *)rtr->dir[i].seg - size;
	}

	if (do_move)
	{
		PollMod();
		far_memmove(dest, src, nbytes);
	}

	sel->flags |= DA_DISCARDED;

	rtr->next_M = (BYTE *)rtr->next_M - size;
	rtr->free = rtr->free + size;

	if (sel->flags & DA_EVANESCENT)
	{
		RTR_free(rtr, (HRES)sel);
	}

	return size;
}

/***************************************************/
//                                                   
// Return index of least-recently-used, unlocked, MOVEABLE resource
//
/***************************************************/

static ULONG RTR_LRU(RTR_class *rtr) {
	ULONG i, oldest;
	ULONG n, age;

	n = rtr->nentries;
	oldest = age = (ULONG)-1;

	for (i = 0; i < n; i++)
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

static ULONG RTR_make_room(RTR_class *rtr, ULONG goal) {
	LONG i;
	WORD first, next;
	ULONG index;
	ULONG next_seg, size_deleted, nbytes;
	void *dest, *src, *end;

	//
	// 1) If /goal/ bytes already free, return immediately
	//

	if (rtr->free >= goal) return 1;

	standby_cursor();

	//
	// 2) Discard all MOVABLE FREE entries which are still present
	//

	for (i = rtr->nentries - 1; i >= 0; i--)
		if ((rtr->dir[i].flags & DA_FREE) &&
			(!(rtr->dir[i].flags & (DA_FIXED | DA_DISCARDED))))
		{
			RTR_discard(rtr, i, 1);
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

	for (first = rtr->nentries - 1; first >= 0; first--)
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

		for (next = first - 1; next >= 0; next--)
		{
			next_seg = (ULONG)rtr->dir[next + 1].seg;

			if (rtr->dir[next].flags &
				(DA_FIXED | DA_PRECIOUS | DA_DISCARDABLE | DA_DISCARDED))
				break;

			if (rtr->dir[next].locks > 0)
				break;

			if (((ULONG)rtr->dir[next].seg + rtr->dir[next].size) != next_seg)
				break;
		}

		end = rtr->next_M;
		dest = rtr->dir[next + 1].seg;

		size_deleted = 0L;
		for (i = next + 1; i <= first; i++)
			size_deleted += RTR_discard(rtr, i, 0);

		src = add_ptr(dest, size_deleted);
		nbytes = ptr_dif(end, src);

		PollMod();
		far_memmove(dest, src, nbytes);

		first = next + 1;
#else
		RTR_discard(rtr, first, 1);
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
		RTR_discard(rtr, index, 1);
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

static ULONG RTR_assign_space(RTR_class *rtr, ULONG bytes, ULONG attrib,
	HRES entry) {
	HD_entry *sel;

	sel = (HD_entry *)entry;

	if (!RTR_make_room(rtr, bytes)) return 0;

	sel->flags = attrib & 0xffffffff;
	sel->locks = 0;
	sel->size = bytes;
	sel->history = rtr->LRU_cnt;

	rtr->free -= bytes;

	if (attrib & DA_FIXED)
	{
		rtr->last_F = (UBYTE *)rtr->last_F - bytes;
		sel->seg = rtr->last_F;
	} else
	{
		sel->seg = rtr->next_M;
		rtr->next_M = (UBYTE *)rtr->next_M + bytes;
	}

	return 1;
}

/***************************************************/
//
// Initialize all entries in resource directory block
//
/***************************************************/

static void RTR_init_dir(RTR_class *rtr, ULONG first) {
	ULONG i, j;

	for (i = 0, j = first; i < DIR_BLK; i++, j++)
	{
		rtr->dir[j].size = 0L;
		rtr->dir[j].flags = DA_FREE | DA_DISCARDED;
		rtr->dir[j].history = 0;
		rtr->dir[j].locks = 0;
		rtr->dir[j].user = (ULONG)-1;
		rtr->dir[j].seg = 0;
	}
}

/***************************************************/
//
// Return next available entry (-1 if error) in resource 
// directory, expanding directory if necessary
//
/***************************************************/

static HRES RTR_new_entry(RTR_class *rtr) {
	ULONG i, f, n;
	ULONG nbytes;
	void *dest, *src;

	n = rtr->nentries;

	if (n >= (unsigned)(-DIR_BLK)) return (ULONG)-1;

	for (i = 0; i < n; i++)
	{
		f = rtr->dir[i].flags;

		if ((f & DA_FREE) && (f & DA_DISCARDED))
			return (HRES)&rtr->dir[i];
	}

	if (!RTR_make_room(rtr, SIZE_DB))
		return (ULONG)-1;

	for (i = 0; i < n; i++)
	{
		if (rtr->dir[i].flags & (DA_DISCARDED | DA_FIXED))
			continue;
		rtr->dir[i].seg = ((BYTE *)rtr->dir[i].seg) + SIZE_DB;
	}

	src = add_ptr(rtr->dir, n * sizeof(HD_entry));
	dest = add_ptr(src, SIZE_DB);
	nbytes = ptr_dif(rtr->next_M, src);

	PollMod();
	far_memmove(dest, src, nbytes);

	rtr->next_M = (BYTE *)rtr->next_M + SIZE_DB;
	rtr->free -= SIZE_DB;
	rtr->nentries += DIR_BLK;

	RTR_init_dir(rtr, n);

	return (HRES)&rtr->dir[n];
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

ULONG RTR_seek(RTR_class *rtr, ULONG rnum) {
	ULONG dirblk, next;
	UWORD dirent;

	dirblk = (rnum / (ULONG)OD_SIZE);
	dirent = (UWORD)(rnum % (ULONG)OD_SIZE);

	if (rtr->cur_blk != dirblk)
	{
		rtr->cur_blk = dirblk;

		next = rtr->RFH.FOB;

		do
		{
			lseek(rtr->file, next, SEEK_SET);
			read(rtr->file, &rtr->OD, sizeof(OD_block));
			next = rtr->OD.next;

			PollMod();
		} while (dirblk--);
	}

	lseek(rtr->file, rtr->OD.index[dirent], SEEK_SET);
	read(rtr->file, &rtr->REH, sizeof(RF_entry_hdr));

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

static void RTR_read(RTR_class *rtr, HRES entry) {
	UBYTE *ptr;
	HD_entry *sel;
	ULONG len;

	sel = (HD_entry *)entry;

	ptr = (UBYTE *)sel->seg;
	len = sel->size;

	while (len > (ULONG)DOS_BUFFSIZE)
	{
		PollMod();

		read(rtr->file, ptr, DOS_BUFFSIZE);
		len -= DOS_BUFFSIZE;
		ptr += DOS_BUFFSIZE;
	}
	read(rtr->file, ptr, len);

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

RTR_class *RTR_construct(void *base, ULONG size, ULONG nnames, BYTE *filename) {
	RTR_class *rtr;
	void *beg, *end;

	rtr = (RTR_class *)mem_alloc(sizeof(RTR_class));

	rtr->file = open(filename, O_RDONLY | O_BINARY);
	if (!rtr->file) return NULL;

	read(rtr->file, &rtr->RFH, sizeof(RF_file_hdr));

	end = add_ptr(base, size);
	beg = base;

	rtr->base = base;

	rtr->nentries = DIR_BLK;
	rtr->dir = (HD_entry *)beg;

	rtr->next_M = add_ptr(beg, SIZE_DB);
	rtr->last_F = end;

	rtr->free = ptr_dif(rtr->last_F, rtr->next_M);

	rtr->LRU_cnt = 0;

	RTR_init_dir(rtr, 0);

	rtr->cur_blk = (ULONG)-1L;

	rtr->name_dir = RTR_alloc(rtr, (ULONG)((ULONG)nnames * sizeof(ND_entry)),
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

void RTR_destroy(RTR_class *rtr, ULONG flags) {
	close(rtr->file);

	if (flags & RTR_FREEBASE)
	{
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

HRES RTR_alloc(RTR_class *rtr, ULONG bytes, ULONG attrib) {
	HRES entry;
	HD_entry *sel;

	entry = RTR_new_entry(rtr);
	if (entry == (ULONG)-1)
		return (ULONG)-1;

	if (!RTR_assign_space(rtr, bytes, attrib, entry))
		return (ULONG)-1;

	sel = (HD_entry *)entry;
	sel->user = (ULONG)-1;

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

void RTR_free(RTR_class *rtr, HRES entry) {
	ULONG i, n;
	HD_entry *sel;
	ND_entry *dir;

	if (entry == (ULONG)-1) return;

	sel = (HD_entry *)entry;

	sel->flags |= DA_FREE;
	sel->locks = 0;

	for (i = 0, dir = (ND_entry *)RTR_addr(rtr->name_dir); i < (ULONG)rtr->nd_entries; i++, dir++)
		if (dir->handle == entry)
			dir->handle = 0;
		else if (dir->thunk == entry)
			dir->thunk = (ULONG)-1;

	if (!(sel->flags & DA_FIXED)) {
		while ((!(sel->flags & DA_FIXED)) &&
			(!(sel->flags & DA_DISCARDED)) &&
			((ULONG)sel->seg + sel->size == (ULONG)rtr->next_M)
			)
		{
			RTR_discard(rtr, (entry - (ULONG)rtr->dir) / sizeof(HD_entry), 1);

			n = rtr->nentries;
			for (i = 0; i < n; i++)
				if ((!(rtr->dir[i].flags & DA_FIXED)) &&
					(rtr->dir[i].flags & DA_FREE) &&
					(!(rtr->dir[i].flags & DA_DISCARDED)) &&
					((ULONG)rtr->dir[i].seg + sel->size == (ULONG)rtr->next_M)
					)
				{
					sel = &(rtr->dir[i]);
					entry = (HRES)sel;
					break;
				}
		}
	} else {
		while ((sel->flags & DA_FIXED)
			&& (!(sel->flags & DA_DISCARDED))
			&& (sel->seg == rtr->last_F))
		{
			rtr->last_F = (UBYTE *)rtr->last_F + sel->size;
			rtr->free += sel->size;
			sel->flags |= DA_DISCARDED;

			n = rtr->nentries;
			for (i = 0; i < n; i++)
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

ULONG RTR_force_discard(RTR_class *rtr, ULONG goal) {
	return RTR_make_room(rtr, goal);
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

void RTR_lock(RTR_class *rtr, HRES entry) {
	ULONG i, n;
	HD_entry *sel;

	PollMod();

	sel = (HD_entry *)entry;

	if (sel->flags & (DA_FIXED | DA_PRECIOUS | DA_FREE))
		return;

	if ((sel->flags & DA_DISCARDED) && (sel->user != -1L))
	{
		if (RTR_assign_space(rtr, sel->size, sel->flags, entry) == (ULONG)-1)
			return;

#if FAST_LOCK
		lseek(rtr->file, sel->user, SEEK_SET);
#else
		RTR_seek(rtr, sel->user);
#endif

		RTR_read(rtr, entry);

		sel->flags &= (~DA_DISCARDED);
	}

	++sel->locks;

	sel->history = ++rtr->LRU_cnt;

	if (rtr->LRU_cnt == 65535U)
	{
		n = rtr->nentries;
		for (i = 0; i < n; i++)
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

void RTR_unlock(HRES entry) {
	HD_entry *sel;

	sel = (HD_entry *)entry;

	if (sel->locks > 0)
		--sel->locks;
}

/***************************************************/
//
// Return the size of a cached resource (not necessarily
// present)
//
/***************************************************/

ULONG RTR_size(HRES entry) {
	HD_entry *sel;

	sel = (HD_entry *)entry;

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

void *RTR_addr(HRES entry) {
	return (void *)(*(ULONG *)entry);
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

void RTR_read_resource(RTR_class *rtr, void *dest, ULONG len) {
	while (len > (ULONG)DOS_BUFFSIZE)
	{
		read(rtr->file, dest, DOS_BUFFSIZE);
		len -= DOS_BUFFSIZE;
		dest = add_ptr(dest, DOS_BUFFSIZE);
	}
	read(rtr->file, dest, len);
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
// Returns (ULONG)-1 if resource could not be loaded
//
/***************************************************/

HRES RTR_load_resource(RTR_class *rtr, ULONG resource, ULONG attrib) {
	HD_entry *sel;
	HRES entry;

	if (!RTR_seek(rtr, resource))
		return (ULONG)-1;

	entry = RTR_alloc(rtr, rtr->REH.data_size,
		(attrib == DA_DEFAULT) ? rtr->REH.data_attrib : attrib);

	if (entry != (ULONG)-1)
	{
		sel = (HD_entry *)entry;

#if FAST_LOCK
		sel->user = tell(rtr->file);
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

HRES RTR_get_resource_handle(RTR_class *rtr, ULONG resource, ULONG attrib) {
	LONG i, insert, replace;
	ND_entry *dir;
	void *dest, *src;
	ULONG nbytes;

	dir = RTR_search_name_dir(rtr, resource);

	if (dir == NULL)
	{
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
			dest = add_ptr(src, (ULONG)sizeof(ND_entry));
			nbytes = (ULONG)((rtr->nd_entries - i) * sizeof(ND_entry));

			PollMod();
			far_memmove(dest, src, nbytes);
		}

		if (!replace)
			++rtr->nd_entries;

		dir->OE = resource;
		dir->handle = RTR_load_resource(rtr, resource, attrib);
		dir->thunk = (ULONG)-1;
	}

	if (dir->handle == (ULONG)-1)
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

void RTR_free_resource(RTR_class *rtr, ULONG resource) {
	ND_entry *dir;

	dir = RTR_search_name_dir(rtr, resource);

	if (dir == NULL)
		return;

	RTR_free(rtr, dir->handle);
}

/***************************************************/
//
// Perform binary search of name directory for specified resource
//
// Return pointer to name directory entry or NULL if entry 
// deleted or not found
//
/***************************************************/

ND_entry *RTR_search_name_dir(RTR_class *rtr, ULONG resource) {
	LONG min, max, mid;
	ND_entry *dir, *attempt;
	ULONG entry;

	dir = (ND_entry *)RTR_addr(rtr->name_dir);

	min = 0;
	max = rtr->nd_entries - 1;

	while (min <= max)
	{
		mid = (min + max) >> 1;

		attempt = &dir[mid];

		entry = attempt->OE;

		if (entry > resource)
			max = mid - 1;
		else if (entry < resource)
			min = mid + 1;
		else
			return (attempt->handle == 0) ? NULL : attempt;
	}

	return NULL;
}

} // namespace Aesop
