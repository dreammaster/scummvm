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

#ifndef AESOP_RESOURCES_H
#define AESOP_RESOURCES_H

#include "common/file.h"
#include "aesop/resource_structures.h"

namespace Aesop {

#define MAX_OBJ_TYPES  2450      // # of possible resource names
#define DIR_BLK        256       // # of cache directory entries/block

//
// RTR_member macro allows access to HD_entry structure members
//

#define RTR_member(x,y)   ( (HD_entry *)(x)->y )

#define SIZE_DB (DIR_BLK * sizeof(HD_entry))

class AesopEngine;

class Resources {
private:
	AesopEngine *_vm;
	Common::File _file;
	RF_file_hdr _RFH;
	RF_entry_hdr _REH;
	OD_block _OD;
	ULONG _cur_blk;

	UWORD _LRU_cnt;
	void *_base;
	void *_next_M;
	void *_last_F;

	HRES _name_dir;
	WORD _nd_entries;

	/**
	 * Initialize all entries in resource directory block
	 */
	void init_dir(ULONG first);

	/**
	 * Discard a given resource to expand free space in cache (MOVEABLE resources only)
	 * If do_move=false, caller is responsible for garbage collection
	 *
	 * If discarded object is EVANESCENT, free its entry
	 * @returns     Size of discarded object in bytes (rounded to next paragraph)
	 */
	ULONG discard(ULONG index, bool do_move);

	/**
	 * Return index of least-recently-used, unlocked, MOVEABLE resource
	 */
	ULONG LRU();

	/**
	 * Create /goal/ bytes of free space in resource cache:
	 * 1) If /goal/ bytes already free, return immediately
	 * 2) Discard all MOVABLE FREE entries which are still present
	 * 3) If /goal/ bytes free, return
	 * 4) Discard all unlocked TEMPORARY entries
	 * 5) While free < goal
	 *
	 * Discard least-recently-used valid candidate
	 * If no LRU candidates available, return false
	 *
	 * 6) Return true
	 */
	ULONG make_room(ULONG goal);

	/**
	 * Get /bytes/ free space for /entry/, establish selector fields
	 * Note: Does not initialize user field
	 */
	ULONG assign_space(ULONG bytes, ULONG attrib, HRES entry);

	/**
	 * Return next available entry (-1 if error) in resource
	 * directory, expanding directory if necessary
	 */
	HRES new_entry();

	/**
	 * Read resource image into cache at entry /entry/
	 * Assumes resource file pointer -> start at resource data
	 */
	void read(HRES entry);

	/**
	 * Sorting method used by create_thunk
	 */
	static int sort_by_msg(const void *a, const void *b);

	/**
	* Sorting method used by create_thunk
	*/
	static int sort_by_class(const void *a, const void *b);
public:
	HD_entry *_dir;
	UWORD _nentries;
	ULONG _free;
public:
	Resources(AesopEngine *vm);
	~Resources();

	/**
	 * Discard entries until amount of space is available in cache
	 * (-1UL for maximum)
	 *
	 * Returns 1 on success, 0 if space could not be obtained
	 */
	ULONG force_discard(ULONG goal);

	/**
	 * Allocate /bytes/ of memory in resource cache and return segment
	 * address of entry descriptor (handle)
	 *
	 * Initializes user field to -1
	 *
	 * Returns -1 if sufficient memory could not be allocated
	 *
	 * Currently only named resources (resource file entries) should be
	 * marked DISCARDABLE, since the client is not notified when a block
	 * is discarded
	 */
	HRES alloc(ULONG bytes, ULONG attrib);

	/**
	 * Release cache memory previously allocated with RTR_alloc()
	 *
	 * Also frees name directory entry, if used
	 *
	 * If block being freed is last MOVABLE or first FIXED, and it hasn't
	 * already been discarded, discard it (along with any free neighbors
	 * if FIXED; this is the only way FIXED entries can ever be discarded at
	 * present)
	 */
	void free(HRES entry);

	/**
	 * "Lock" a resource cache entry
	 *
	 * A locked resource cannot be automatically discarded until the lock count is
	 * decremented to zero via call(s) to RTR_unlock()
	 *
	 * Locking does NOT guarantee that the resource will not be moved when other
	 * resources are discarded, only that it will always be instantly accessible
	 * by dereferencing the handle segment pointer
	 */
	void lock(HRES entry);

	/**
	 * "Unlock" a resource cache entry
	 *
	 * A locked resource cannot be automatically discarded until the lock count is
	 * decremented to zero via call(s) to RTR_unlock()
	 */
	void unlock(HRES entry);

	/**
	 * Return the size of a cached resource (not necessarily present)
	 */
	static ULONG size(HRES entry);

	/**
	 * Return handle to cached image of resource /resource/
	 * (or 0 if not valid resource)
	 *
	 * If /resource/ has not been loaded, load it and enter its
	 * handle in the (sorted) resource name directory for faster access
	 * in future calls to this function
	 *
	 * This is the preferred way to access most resources in the resource file
	 */
	HRES get_resource_handle(ULONG resource, ULONG attrib);

	/**
	 * Free a resource by name
	 *
	 * This function allows resource cache entries to be freed based on
	 * the resource ordinal entry number (or "name") of the cached entry,
	 * rather than the entry's handle
	 */
	void free_resource(ULONG resource);

	/**
	 * Allocate cache memory and load resource /resource/ with attributes /attrib/
	 *
	 * Use resource's default attributes if /attrib/ = DA_DEFAULT
	 *
	 * Normally this function should not be called to load named resources,
	 * since RTR_get_resource_handle() maintains a name directory to avoid
	 * redundantly loading named resources
	 *
	 * Returns -1U if resource could not be loaded
	 */
	HRES loadResource(ULONG resource, ULONG attrib);

	/**
	 * Load a string resource into a SOP instance's array
	 *
	 * Determines array offset in instance, in case resource load causes
	 * instance to move in memory
	 *
	 * WARNING: The array must not be of automatic or external scope!
	*/
	void loadString(byte *array, uint string);

	/**
	 * Read from resource file into a specified block of memory
	 *
	 * seek() must first have been called to set the resource file
	 * pointer position to the desired resource
	 */
	void read_resource(void *dest, ULONG len);

	/**
	 * Read resource header to get data attribs & len, return w/file
	 * pointer positioned at start of resource data
	 *
	 * Maintain copy of current ordinal directory block as part of
	 * RTR structure to reduce excessive seeking
	 *
	 * Return size of resource in bytes
	 *
	 * Return 0 if resource was created by reference and never
	 * subsequently written (PLACEHOLDER)
	 */
	ULONG seek(ULONG rnum);

	/**
	 * Return the current address of a cached resource
	 *
	 * Addresses of FIXED resources may safely be stored and re-used at will
	 * by the client program, since FIXED resources are neither MOVEABLE nor
	 * (at least for the time being) DISCARDABLE.
	 *
	 * However, addresses of MOVEABLE resources may change any time a block of
	 * memory is allocated from the resource cache.  Additionally, DISCARDABLE
	 * resources MUST be locked via lock() before calling this function.
	 * Do not expect pointers to MOVEABLE resources to remain valid after
	 * any function which can load resources or otherwise allocate cache memory.
	 * The fix_segment() function may be used to adjust segment addresses
	 * of moveable resources.
	 */
	static void *addr(HRES entry);

	void fixup(void **ptr, HRES entry);

	/**
	 * Perform binary search of name directory for specified resource
	 *
	 * Return pointer to name directory entry or NULL if entry
	 * deleted or not found
	 */
	ND_entry *search_name_dir(ULONG resource);

	/**
	 * Create SOP instance, building thunk if necessary
	 */
	HRES create_instance(ULONG object);

	/**
	 * Destroy instance of specified SOP
	 * Delete thunk if no other instances of this object exist/
	 */
	void destroy_instance(HRES instance);

	/**
	 * Create thunk for specified object
	 */
	HRES construct_thunk(Resources *lnk, ULONG object);
};

} // End of namespace Aesop

#endif

