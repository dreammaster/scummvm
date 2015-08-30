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

#include "common/scummsys.h"
#include "common/file.h"
#include "common/textconsole.h"
#include "aesop/aesop.h"
#include "aesop/defs.h"
#include "aesop/eye.h"
#include "aesop/rtsystem.h"
#include "aesop/rtmsg.h"
#include "aesop/resources.h"
#include "aesop/rt.h"
#include "aesop/intrface.h"
#include "aesop/graphics.h"   // for dprint()
#include "aesop/stubs.h"

namespace Aesop {

// user field = resource name if 0, file offset if 1
#define FAST_LOCK		1				
// AESOP resource cache size (800K)
#define MAX_RES_SIZE	800000

#undef FARPROC
typedef void(*FARPROC)();
extern FARPROC code_resources[];

/*----------------------------------------------------------------*/

RF_file_hdr::RF_file_hdr() {
	Common::fill(&_signature[0], &_signature[16], 0);
	_fileSize = 0;
	_lostSpace = 0;
	_FOB = 0;
	_createTime = 0;
	_modifyTime = 0;
}

void RF_file_hdr::load(Common::SeekableReadStream &s) {
	s.read(&_signature[0], 16);
	_fileSize = s.readUint32LE();
	_lostSpace = s.readUint32LE();
	_FOB = s.readUint32LE();
	_createTime = s.readUint32LE();
	_modifyTime = s.readUint32LE();
}

/*----------------------------------------------------------------*/

OD_block::OD_block() {
	_next = 0;
	Common::fill(&_flags[0], &_flags[OD_SIZE], 0);
	Common::fill(&_index[0], &_index[OD_SIZE], 0);
}

void OD_block::load(Common::SeekableReadStream &s) {
	_next = s.readUint32LE();
	for (int idx = 0; idx < OD_SIZE; ++idx)
		_flags[idx] = s.readByte();
	for (int idx = 0; idx < OD_SIZE; ++idx)
		_index[idx] = s.readUint32LE();
}

/*----------------------------------------------------------------*/

RF_entry_hdr::RF_entry_hdr() {
	_timestamp = 0;
	_dataAttrib = 0;
	_dataSize = 0;
}

void RF_entry_hdr::load(Common::SeekableReadStream &s) {
	_timestamp = s.readUint32LE();
	_dataAttrib = s.readUint32LE();
	_dataSize = s.readUint32LE();
}

/*----------------------------------------------------------------*/

Resources::Resources(AesopEngine *vm): _vm(vm) {
	const Common::String filename = vm->getGameID() == GType_EOB3 ? "eye.res" : "hack.res";
	void *beg,*end;

	if (!_file.open(filename))
		error("Could not locate %s", filename.c_str());

	_RFH.load(_file);

	_base = new byte[MAX_RES_SIZE];
	beg = _base;
	end = add_ptr(_base, MAX_RES_SIZE);

	_nentries = DIR_BLK;
	_dir = (HD_entry *)beg;

	_next_M = add_ptr(beg,SIZE_DB);
	_last_F = end;

	_free = ptr_dif(_last_F,_next_M);

	_LRU_cnt = 0;

	init_dir(0);

	_cur_blk = (ULONG)-1;

	_name_dir = alloc(MAX_OBJ_TYPES * sizeof(ND_entry), DA_FIXED | DA_PRECIOUS);
	_nd_entries = 0;
}

Resources::~Resources() {
	_file.close();
	delete[] _base;
}

ULONG Resources::discard(ULONG index, bool do_move) {
	ULONG i, n;
	void *dest, *src;
	ULONG nbytes, size;
	HD_entry *sel;

	sel = &_dir[index];

	size = sel->_size;
	dest = sel->_seg;
	src = add_ptr(dest, size);
	nbytes = ptr_dif(_next_M, src);

	n = _nentries;
	for (i = 0; i < n; i++) {
		if (_dir[i]._flags & (DA_FIXED | DA_DISCARDED))
			continue;

		if (ptr_dif(_dir[i]._seg, dest) > 0L)
			_dir[i]._seg = (BYTE *)_dir[i]._seg - size;
	}

	if (do_move) {
		PollMod();
		far_memmove(dest, src, nbytes);
	}

	sel->_flags |= DA_DISCARDED;

	_next_M = (BYTE *)_next_M - size;
	_free = _free + size;

	if (sel->_flags & DA_EVANESCENT) {
		free((HRES)sel);
	}

	return size;
}

ULONG Resources::LRU() {
	ULONG i, oldest;
	ULONG n, age;

	n = _nentries;
	oldest = age = (ULONG)-1;

	for (i = 0; i < n; ++i) {
		if (_dir[i]._flags & (DA_FIXED | DA_PRECIOUS | DA_DISCARDED | DA_FREE))
			continue;

		if (_dir[i]._locks > 0)
			continue;

		if (_dir[i]._history < age) {
			age = _dir[i]._history;
			oldest = i;
		}
	}

	return oldest;
}

ULONG Resources::make_room(ULONG goal) {
	LONG i;
	WORD first, next;
	ULONG index;
	ULONG next_seg, size_deleted, nbytes;
	void *dest, *src, *end;

	//
	// 1) If /goal/ bytes already free, return immediately
	//

	if (_free >= goal)
		return 1;

	//
	// 2) Discard all MOVABLE FREE entries which are still present
	//

	for (i = _nentries - 1; i >= 0; --i) {
		if ((_dir[i]._flags & DA_FREE) && (!(_dir[i]._flags & (DA_FIXED | DA_DISCARDED)))) {
			discard(i, 1);
		}
	}

	//
	// 3) If /goal/ bytes free, return
	//

	if (_free >= goal) {
		return 1;
	}

	//
	// 4) Discard all unlocked TEMPORARY entries
	//

	for (first = _nentries - 1; first >= 0; first--)
	{
		if (_dir[first]._flags & (DA_FIXED | DA_PRECIOUS | DA_DISCARDABLE | DA_DISCARDED))
			continue;

		if (_dir[first]._locks > 0)
			continue;

		//
		// Found at least one unlocked TEMPORARY entry; try to merge adjacent
		// entries' blocks to avoid redundant calls to far_memmove()
		//

		for (next = first - 1; next >= 0; --next) {
			next_seg = (ULONG)_dir[next + 1]._seg;

			if (_dir[next]._flags & (DA_FIXED | DA_PRECIOUS | DA_DISCARDABLE | DA_DISCARDED))
				break;

			if (_dir[next]._locks > 0)
				break;

			if (((ULONG)_dir[next]._seg + _dir[next]._size) != next_seg)
				break;
		}

		end = _next_M;
		dest = _dir[next + 1]._seg;

		size_deleted = 0L;
		for (i = next + 1; i <= first; i++)
			size_deleted += discard(i, 0);

		src = add_ptr(dest, size_deleted);
		nbytes = ptr_dif(end, src);

		PollMod();
		far_memmove(dest, src, nbytes);

		first = next + 1;
	}

	//
	// 5) While free < goal
	//

	while (_free < goal) {
		//
		// If no LRU candidates available, return false
		//       
		index = LRU();
		if (index == -1) {
			return 0;
		}

		//       
		// Discard least-recently-used valid candidate
		//
		discard(index, 1);
	}

	return 1;
}

ULONG Resources::assign_space(ULONG bytes, ULONG attrib, HRES entry) {
	HD_entry *sel = (HD_entry *)entry;

	if (!make_room(bytes))
		return 0;

	sel->_flags = attrib & 0xffffffff;
	sel->_locks = 0;
	sel->_size = bytes;
	sel->_history = _LRU_cnt;

	_free -= bytes;

	if (attrib & DA_FIXED)
	{
		_last_F = (UBYTE *)_last_F - bytes;
		sel->_seg = _last_F;
	}
	else
	{
		sel->_seg = _next_M;
		_next_M = (UBYTE *)_next_M + bytes;
	}

	return 1;
}

void Resources::init_dir(ULONG first) {
	for (int i = 0, j = first; i < DIR_BLK; ++i, ++j) {
		_dir[j]._size = 0L;
		_dir[j]._flags = DA_FREE | DA_DISCARDED;
		_dir[j]._history = 0;
		_dir[j]._locks = 0;
		_dir[j]._user = (ULONG)-1;
		_dir[j]._seg = 0;
	}
}

HRES Resources::new_entry() {
	ULONG i, f, n;
	ULONG nbytes;
	void *dest, *src;

	n = _nentries;

	if (n >= (unsigned)(-DIR_BLK))
		return HRES_NULL;

	for (i = 0; i < n; ++i) {
		f = _dir[i]._flags;

		if ((f & DA_FREE) && (f & DA_DISCARDED))
			return (HRES)&_dir[i];
	}

	if (!make_room(SIZE_DB))
		return HRES_NULL;

	for (i = 0; i < n; i++) {
		if (_dir[i]._flags & (DA_DISCARDED | DA_FIXED))
			continue;
		_dir[i]._seg = ((BYTE *)_dir[i]._seg) + SIZE_DB;
	}

	src = add_ptr(_dir, n * sizeof(HD_entry));
	dest = add_ptr(src, SIZE_DB);
	nbytes = ptr_dif(_next_M, src);

	PollMod();
	far_memmove(dest, src, nbytes);

	_next_M = (BYTE *)_next_M + SIZE_DB;
	_free -= SIZE_DB;
	_nentries += DIR_BLK;

	init_dir(n);

	return (HRES)&_dir[n];
}

ULONG Resources::seek(ULONG rnum) {
	ULONG dirblk, next;
	UWORD dirent;

	dirblk = (rnum / (ULONG)OD_SIZE);
	dirent = (UWORD)(rnum % (ULONG)OD_SIZE);

	if (_cur_blk != dirblk) {
		_cur_blk = dirblk;

		next = _RFH._FOB;

		do {
			_file.seek(next, SEEK_SET);
			_OD.load(_file);
			next = _OD._next;

			PollMod();
		} while (dirblk--);
	}

	_file.seek(_OD._index[dirent], SEEK_SET);
	_REH.load(_file);

	if (_REH._dataAttrib & DA_PLACEHOLDER)
		return 0;

	return _REH._dataSize;
}

void Resources::read(HRES entry) {
	UBYTE *ptr;
	HD_entry *sel;
	ULONG len;

	sel = (HD_entry *)entry;

	ptr = (UBYTE *)sel->_seg;
	len = sel->_size;

	while (len > (ULONG)DOS_BUFFSIZE) {
		PollMod();

		_file.read(ptr, DOS_BUFFSIZE);
		len -= DOS_BUFFSIZE;
		ptr += DOS_BUFFSIZE;
	}

	_file.read(ptr, len);
	PollMod();
}

HRES Resources::alloc(ULONG bytes, ULONG attrib) {
	HRES entry;
	HD_entry *sel;

	entry = new_entry();
	if (entry == HRES_NULL)
		return HRES_NULL;

	if (!assign_space(bytes, attrib, entry))
		return HRES_NULL;

	sel = (HD_entry *)entry;
	sel->_user = (ULONG)-1;

	return entry;
}

void Resources::free(HRES entry) {
	ULONG i, n;
	HD_entry *sel;
	ND_entry *dir;

	if (entry == HRES_NULL)
		return;

	sel = (HD_entry *)entry;

	sel->_flags |= DA_FREE;
	sel->_locks = 0;

	for (i = 0, dir = (ND_entry *)addr(_name_dir); i < (ULONG)_nd_entries; ++i, ++dir) {
		if (dir->handle == entry)
			dir->handle = 0;
		else if (dir->thunk == entry)
			dir->thunk = HRES_NULL;
	}

	if (!(sel->_flags & DA_FIXED))
	{
		while ((!(sel->_flags & DA_FIXED)) &&
			(!(sel->_flags & DA_DISCARDED)) &&
			((ULONG)sel->_seg + sel->_size == (ULONG)_next_M)
		) {
			discard(((byte *)entry - (byte *)_dir) / sizeof(HD_entry), 1);

			n = _nentries;
			for (i = 0; i < n; i++)
				if ((!(_dir[i]._flags & DA_FIXED)) &&
					(_dir[i]._flags & DA_FREE) &&
					(!(_dir[i]._flags & DA_DISCARDED)) &&
					((ULONG)_dir[i]._seg + sel->_size == (ULONG)_next_M)
					)
				{
					sel = &(_dir[i]);
					entry = (HRES)sel;
					break;
				}
		}
	} else {
		while ((sel->_flags & DA_FIXED)
			&& (!(sel->_flags & DA_DISCARDED))
			&& (sel->_seg == _last_F))
		{
			_last_F = (UBYTE *)_last_F + sel->_size;
			_free += sel->_size;
			sel->_flags |= DA_DISCARDED;

			n = _nentries;
			for (i = 0; i < n; i++)
				if ((_dir[i]._flags & DA_FIXED)
					&& (_dir[i]._flags & DA_FREE)
					&& (!(_dir[i]._flags & DA_DISCARDED))
					&& (_dir[i]._seg == _last_F))
				{
					sel = &_dir[i];
					break;
				}
		}
	}
}

ULONG Resources::force_discard(ULONG goal) {
	return make_room(goal);
}

void Resources::lock(HRES entry) {
	ULONG i, n;
	HD_entry *sel;

	PollMod();

	sel = (HD_entry *)entry;

	if (sel->_flags & (DA_FIXED | DA_PRECIOUS | DA_FREE))
		return;

	if ((sel->_flags & DA_DISCARDED) && (sel->_user != -1L)) {
		if (assign_space(sel->_size, sel->_flags, entry) == (ULONG)-1)
			return;

#if FAST_LOCK
		_file.seek(sel->_user, SEEK_SET);
#else
		seek(sel->user);
#endif

		read(entry);

		sel->_flags &= (~DA_DISCARDED);
	}

	++sel->_locks;

	sel->_history = ++_LRU_cnt;

	if (_LRU_cnt == 65535U) {
		n = _nentries;
		for (i = 0; i < n; i++)
			_dir[i]._history >>= 3;

		_LRU_cnt >>= 3;
	}
}

void Resources::unlock(HRES entry) {
	HD_entry *sel;

	sel = (HD_entry *)entry;

	if (sel->_locks > 0)
		--sel->_locks;
}

ULONG Resources::size(HRES entry) {
	HD_entry *sel;

	sel = (HD_entry *)entry;
	return sel->_size;
}

void *Resources::addr(HRES entry) {
	return (void *)(*(ULONG *)entry);
}

void Resources::read_resource(void *dest, ULONG len) {
	while (len > (ULONG)DOS_BUFFSIZE) {
		_file.read(dest, DOS_BUFFSIZE);
		len -= DOS_BUFFSIZE;
		dest = add_ptr(dest, DOS_BUFFSIZE);
	}

	_file.read(dest, len);
}

HRES Resources::load_resource(ULONG resource, ULONG attrib) {
	HD_entry *sel;
	HRES entry;

	if (!seek(resource))
		return HRES_NULL;

	entry = alloc(_REH._dataSize,
		(attrib == DA_DEFAULT) ? _REH._dataAttrib : attrib);

	if (entry != HRES_NULL) {
		sel = (HD_entry *)entry;

#if FAST_LOCK
		sel->_user = _file.pos();
#else
		sel->user = resource;
#endif

		read(entry);
	}

	return entry;
}

HRES Resources::get_resource_handle(ULONG resource, ULONG attrib) {
	LONG i, insert, replace;
	ND_entry *dir;
	void *dest, *src;
	ULONG nbytes;

	dir = search_name_dir(resource);

	if (dir == NULL) {
		dir = (ND_entry *)addr(_name_dir);

		for (i = replace = insert = 0; i < _nd_entries; ++i) {
			if (dir->OE > resource) {
				insert = 1;
				break;
			}

			if (dir->OE == resource) {
				replace = 1;
				break;
			}

			++dir;
		}

		if (insert) {
			src = dir;
			dest = add_ptr(src, (ULONG) sizeof(ND_entry));
			nbytes = (ULONG)((_nd_entries - i) * sizeof(ND_entry));

			PollMod();
			far_memmove(dest, src, nbytes);
		}

		if (!replace)
			++_nd_entries;

		dir->OE = resource;
		dir->handle = load_resource(resource, attrib);
		dir->thunk = HRES_NULL;
	}

	if (dir->handle == HRES_NULL)
		dir->handle = 0;

	return dir->handle;
}

void Resources::free_resource(ULONG resource) {
	ND_entry *dir;

	dir = search_name_dir(resource);

	if (dir == NULL)
		return;

	free(dir->handle);
}

ND_entry *Resources::search_name_dir(ULONG resource) {
	LONG min, max, mid;
	ND_entry *dir, *tryEntry;
	ULONG entry;

	dir = (ND_entry *)addr(_name_dir);

	min = 0;
	max = _nd_entries - 1;

	while (min <= max) {
		mid = (min + max) >> 1;

		tryEntry = &dir[mid];

		entry = tryEntry->OE;

		if (entry > resource)
			max = mid - 1;
		else if (entry < resource)
			min = mid + 1;
		else
			return (tryEntry->handle == 0) ? nullptr : tryEntry;
	}

	return nullptr;
}

HRES Resources::create_instance(ULONG object) {
	Resources &res = *_vm->_resources;
	HRES thunk, instance;
	HD_entry *sel;
	ND_entry *entry;
	THDR *thdr;
	IHDR ihdr;

	entry = search_name_dir(object);

	if ((entry == NULL) || (entry->thunk == HRES_NULL)) {
		thunk = res.construct_thunk(_vm->_linkResources, object);
		search_name_dir(object)->thunk = thunk;
	} else {
		thunk = (HRES)entry->thunk;
	}
	ihdr.thunk = thunk;

	thdr = (THDR *)addr(thunk);
	++thdr->use_cnt;

	instance = alloc(thdr->isize, DA_MOVEABLE | DA_PRECIOUS);

	sel = (HD_entry *)instance;

	sel->_user = object;

	*(IHDR *)addr(instance) = ihdr;

	return instance;
}

void Resources::destroy_instance(HRES instance) {
	HRES thunk = ((IHDR *)addr(instance))->thunk;

	if (!(--(((THDR *)addr(thunk))->use_cnt)))
		free(thunk);

	free(instance);
}

/***************************************************/
//
// Create thunk for specified object
//
/***************************************************/

HRES Resources::construct_thunk(Resources *lnk, ULONG object) {
	UWORD depth;              // 0..MAX_G = derived object..base class
	ULONG class1, xclass;
	ULONG tsize;
	ULONG source;
	UWORD target;
	WORD i;
	UWORD j, k, m, n, mcnt;
	PRG_HDR prg, xprg;
	THDR thdr;
	UBYTE *dict;
	BYTE *tag, *tagbase;
	char *def;
	HRES thunk;
	HRES HCRFD;
	HRES impt[MAX_G], expt[MAX_G], code[MAX_G];
	HRES xexpt, xcode;
	ULONG exports[MAX_G];
	UWORD s_S[MAX_G], x_S[MAX_G];
	UWORD SD_offset[MAX_G];
	UWORD index, offset, found;
	UWORD XR_list;
	SD_entry *SD, *SDarray;
	MV_entry *MV;
	void *XR;
	XDR_entry *xdr_ptr;

	void *thunk_ptr;
	ULONG def_off;
	ULONG *XR_ptr, *CR_ptr;

	//
	// Load programs and dictionaries, calculate thunk size
	//

	HCRFD = get_resource_handle(CRFD, DA_TEMPORARY | DA_EVANESCENT);
	lock(HCRFD);

	thdr.MV_list = sizeof(THDR);
	thdr.SD_list = sizeof(THDR);
	thdr.max_msg = (UWORD)-1;
	thdr.nprgs = 0;
	thdr.isize = sizeof(IHDR);
	thdr.use_cnt = 0;

	XR_list = sizeof(THDR);

	tsize = sizeof(THDR);

	for (i = 0; i < MAX_G; i++)
		s_S[i] = x_S[i] = 0;

	depth = 0;
	class1 = object;

	while (class1 != -1L) {
		code[depth] = get_resource_handle(class1, DA_DEFAULT);

		if (!code[depth])
			abend(MSG_PONF, class1);     // "Program object %lu not found"

		lock(code[depth]);

		prg = *(PRG_HDR *)addr(code[depth]);

		++thdr.nprgs;
		tsize += sizeof(SD_entry);
		XR_list += sizeof(SD_entry);

		exports[depth] = prg.exports;

		impt[depth] = lnk->get_resource_handle(prg.imports, DA_TEMPORARY | DA_EVANESCENT);
		lnk->lock(impt[depth]);

		expt[depth] = lnk->get_resource_handle(prg.exports, DA_TEMPORARY | DA_EVANESCENT);
		lnk->lock(expt[depth]);

		//
		// Calculate Size of Message Vector List   
		//

		mcnt = 0;
		dict = (UBYTE *)RTD_first(addr(expt[depth]));
		while ((dict = (UBYTE *)RTD_iterate(addr(expt[depth]), dict, &tag, (BYTE **)&def)) != NULL) {
			switch (tag[0]) {
			case 'M':               // Message
				++thdr.max_msg;
				++mcnt;
				thdr.SD_list += sizeof(MV_entry);
				XR_list += sizeof(MV_entry);
				tsize += sizeof(MV_entry);
				break;

			default:
				break;
			}
		}

		//
		// Calculate Size of External Reference List
		//

		dict = (UBYTE *)RTD_first(addr(impt[depth]));
		while ((dict = (UBYTE *)RTD_iterate(addr(impt[depth]), dict, &tag, (BYTE **)&def)) != NULL) {
			switch (tag[0]) {
			case 'C':               // Code
				tsize += sizeof(XCR_entry);
				x_S[depth] += sizeof(XCR_entry);
				break;

			case 'B':               // Byte
			case 'W':               // Word
			case 'L':               // Long
				tsize += sizeof(XDR_entry);
				x_S[depth] += sizeof(XDR_entry);
				break;

			default:
				break;
			}
		}

		s_S[depth] += prg.static_size;
		thdr.isize += prg.static_size;

		class1 = prg.parent;

		if (++depth == MAX_G)
			abend(MSG_AILE);           // "AESOP inheritance limit exceeded"

		if (tsize > 65535L)
			abend(MSG_TTL);            // "Thunk too large"

	}


	//
	// Allocate memory for thunk and construct it
	//

	thunk = alloc(tsize, DA_MOVEABLE | DA_PRECIOUS);

	*(THDR *)addr(thunk) = thdr;

	SD = (SD_entry *)thdr.SD_list;

	i = depth - 1;
	j = 0;
	k = sizeof(IHDR);
	m = XR_list;
	n = thdr.SD_list;

	while (i >= 0) {
		SDarray = (SD_entry *)((ULONG)addr(thunk) + (ULONG)SD);
		SDarray[j].SOP = code[i];
		SDarray[j].exports = exports[i];
		SDarray[j].static_base = k;
		SDarray[j].extern_base = m;

		SD_offset[i] = n;

		XR = (void *)m;

		dict = (UBYTE *)RTD_first(addr(impt[i]));
		while ((dict = (UBYTE *)RTD_iterate(addr(impt[i]), dict, &tag, (BYTE **)&def)) != NULL) {
			tagbase = (BYTE *)addr(impt[i]);
			switch (tag[0]) {
			case 'C':               // Code
				offset = (UWORD)ascnum((const char *)RTD_lookup(HCRFD, (const char *)&tag[2]));
				if (offset == (UWORD)-1)
					abend(MSG_MCR, &tag[2]); // "Missing code resource '%s'"

				thunk_ptr = addr(thunk);
				def_off = ascnum(def);
				XR_ptr = (ULONG *)((ULONG)thunk_ptr + (ULONG)XR + def_off);
				CR_ptr = (ULONG *)((ULONG)&code_resources + offset);
				*XR_ptr = *CR_ptr;

				break;

			case 'B':               // Byte
			case 'W':               // Word
			case 'L':               // Long   
				target = (UWORD)ascnum(def);
				source = ascnum(strchr(def, ',') + 1);

				xclass = source;
				index = sizeof(IHDR);
				found = 0;

				while (xclass != -1L) {
					xcode = get_resource_handle(xclass, DA_DEFAULT);

					if (!xcode)
						abend(MSG_FPNF, xclass); //"Friend program %lu not found"

					lock(xcode);
					tag = tag - tagbase + (BYTE *)addr(impt[i]);

					xprg = *(PRG_HDR *)addr(xcode);

					xclass = xprg.parent;

					if (!found)
					{
						xexpt = lnk->get_resource_handle(xprg.exports,
							DA_TEMPORARY | DA_EVANESCENT);

						lnk->lock(xexpt);
						tag = tag - tagbase + (BYTE *)addr(impt[i]);

						offset = (UWORD)ascnum((const char *)RTD_lookup(xexpt, (const char *)tag));

						if (offset != (UWORD)-1) {
							found = 1;
							index += offset;
						}

						unlock(xexpt);
					}
					else {
						index += xprg.static_size;
					}

					unlock(xcode);
				}

				if (!found)
					abend(MSG_UER, tag);  //"Unresolved external reference '%s'"

				thunk_ptr = addr(thunk);
				xdr_ptr = (XDR_entry *)((ULONG)thunk_ptr + (ULONG)XR + target);
				xdr_ptr->offset = index;

				break;
			}
		}

		n += sizeof(SD_entry);
		m += x_S[i];
		k += s_S[i];
		j++;
		i--;
	}

	MV = (MV_entry *)add_offset(addr(thunk), thdr.MV_list);

	for (i = m = 0; i < depth; i++) {
		dict = (UBYTE *)RTD_first(addr(expt[i]));
		while ((dict = (UBYTE *)RTD_iterate(addr(expt[i]), dict, &tag, (BYTE **)&def)) != NULL) {
			if (tag[0] == 'M') {
				MV[m].msg = (UWORD)ascnum((const char *)&tag[2]);
				MV[m].handler = (ULONG)ascnum(def);
				MV[m].SD_offset = SD_offset[i];
				++m;
			}
		}
	}

	if (m) {
		//
		// Sort all message vectors by ascending message number
		//

		qsort(MV, m, sizeof(MV_entry), sort_by_msg);

		//
		// Sort identical message vectors by ascending class
		//

		k = m - 1;
		for (i = 0; i < k; i++) {
			if (MV[i + 1].msg == MV[i].msg) {
				for (j = i + 2; j < m; j++)
					if (MV[j].msg != MV[i].msg) break;

				qsort(&MV[i], j - i, sizeof(MV_entry), sort_by_class);

				i = j - 1;
			}
		}
	}

	//
	// Unlock dictionaries and exit w/handle to thunk
	// 

	unlock(HCRFD);
	for (i = 0; i < depth; ++i) {
		unlock(impt[i]);
		unlock(expt[i]);
		unlock(code[i]);
	}

	return thunk;
}

int Resources::sort_by_msg(const void *a, const void *b) {
	UWORD na, nb;

	na = ((MV_entry *)a)->msg;
	nb = ((MV_entry *)b)->msg;

	if (na > nb)
		return 1;
	else if (na == nb)
		return 0;
	else
		return -1;
}

int Resources::sort_by_class(const void *a, const void *b) {
	UWORD na, nb;

	na = ((MV_entry *)a)->SD_offset;
	nb = ((MV_entry *)b)->SD_offset;

	if (na < nb)
		return 1;
	else if (na == nb)
		return 0;
	else
		return -1;
}

} // End of namespace Aesop



