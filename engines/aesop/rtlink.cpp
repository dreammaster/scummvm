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

#include "aesop/defs.h"
#include "aesop/rtres.h"
#include "aesop/rt.h"
#include "aesop/rtlink.h"
#include "aesop/rtcode.h"
#include "aesop/rtsystem.h"
#include "aesop/rtmsg.h"

namespace Aesop {

typedef struct
{
	ULONG val;
}
XCR_entry;                    // External code reference entry

typedef struct
{
	UWORD offset;
}
XDR_entry;                    // External data reference entry

/***************************************************/
//
// Utility subs for thunk creation
//
/***************************************************/

int sort_by_msg(const void *a, const void *b) {
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

int sort_by_class(const void *a, const void *b) {
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

/***************************************************/
//
// Create thunk for specified object
//
/***************************************************/

HRES construct_thunk(RTR_class *rtr, RTR_class *lnk, ULONG object) {
	UWORD depth;              // 0..MAX_G = derived object..base class
	ULONG cclass, xclass;
	ULONG tsize;
	ULONG source;
	UWORD target;
	WORD i;
	UWORD j, k, m, n, mcnt;
	PRG_HDR prg, xprg;
	THDR thdr;
	UBYTE *dict;
	BYTE *tag, *tagbase, *def;
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

	HCRFD = RTR_get_resource_handle(rtr, CRFD, DA_TEMPORARY | DA_EVANESCENT);
	RTR_lock(rtr, HCRFD);

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
	cclass = object;

	while (cclass != -1L)
	{
		code[depth] = RTR_get_resource_handle(rtr, cclass, DA_DEFAULT);

		if (!code[depth])
			abend(MSG_PONF, cclass);     // "Program object %lu not found"

		RTR_lock(rtr, code[depth]);

		prg = *(PRG_HDR *)RTR_addr(code[depth]);

		++thdr.nprgs;
		tsize += sizeof(SD_entry);
		XR_list += sizeof(SD_entry);

		exports[depth] = prg.exports;

		impt[depth] = RTR_get_resource_handle(lnk, prg.imports,
			DA_TEMPORARY | DA_EVANESCENT);
		RTR_lock(lnk, impt[depth]);

		expt[depth] = RTR_get_resource_handle(lnk, prg.exports,
			DA_TEMPORARY | DA_EVANESCENT);
		RTR_lock(lnk, expt[depth]);


		//
		// Calculate Size of Message Vector List   
		//

		mcnt = 0;
		dict = (UBYTE *)RTD_first(RTR_addr(expt[depth]));
		while ((dict = (UBYTE *)RTD_iterate(RTR_addr(expt[depth]), dict, &tag, &def)) != NULL)
		{
			switch (tag[0])
			{
			case 'M':               // Message
				++thdr.max_msg;
				++mcnt;
				thdr.SD_list += sizeof(MV_entry);
				XR_list += sizeof(MV_entry);
				tsize += sizeof(MV_entry);
				break;
			}
		}

		//
		// Calculate Size of External Reference List
		//

		dict = (UBYTE *)RTD_first(RTR_addr(impt[depth]));
		while ((dict = (UBYTE *)RTD_iterate(RTR_addr(impt[depth]), dict, &tag, &def)) != NULL)
		{
			switch (tag[0])
			{
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
			}
		}

		s_S[depth] += prg.static_size;
		thdr.isize += prg.static_size;

		cclass = prg.parent;

		if (++depth == MAX_G)
			abend(MSG_AILE);           // "AESOP inheritance limit exceeded"

		if (tsize > 65535L)
			abend(MSG_TTL);            // "Thunk too large"

	}


	//
	// Allocate memory for thunk and construct it
	//

	thunk = RTR_alloc(rtr, tsize, DA_MOVEABLE | DA_PRECIOUS);

	*(THDR *)RTR_addr(thunk) = thdr;

	SD = (SD_entry *)thdr.SD_list;

	i = depth - 1;
	j = 0;
	k = sizeof(IHDR);
	m = XR_list;
	n = thdr.SD_list;

	while (i >= 0)
	{
		SDarray = (SD_entry *)((ULONG)RTR_addr(thunk) + (ULONG)SD);
		SDarray[j].SOP = code[i];
		SDarray[j].exports = exports[i];
		SDarray[j].static_base = k;
		SDarray[j].extern_base = m;

		SD_offset[i] = n;

		XR = (void *)m;

		dict = (UBYTE *)RTD_first(RTR_addr(impt[i]));
		while ((dict = (UBYTE *)RTD_iterate(RTR_addr(impt[i]), dict, &tag, &def)) != NULL)
		{
			tagbase = (BYTE *)RTR_addr(impt[i]);
			switch (tag[0])
			{
			case 'C':               // Code
				offset = (UWORD)ascnum(RTD_lookup(HCRFD, &tag[2]));
				if (offset == (ULONG)-1)
					abend(MSG_MCR, &tag[2]); // "Missing code resource '%s'"

				thunk_ptr = RTR_addr(thunk);
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

				while (xclass != -1L)
				{
					xcode = RTR_get_resource_handle(rtr, xclass,
						DA_DEFAULT);

					if (!xcode)
						abend(MSG_FPNF, xclass); //"Friend program %lu not found"

					RTR_lock(rtr, xcode);
					tag = tag - tagbase + (BYTE *)RTR_addr(impt[i]);

					xprg = *(PRG_HDR *)RTR_addr(xcode);

					xclass = xprg.parent;

					if (!found)
					{
						xexpt = RTR_get_resource_handle(lnk, xprg.exports,
							DA_TEMPORARY | DA_EVANESCENT);

						RTR_lock(lnk, xexpt);
						tag = tag - tagbase + (BYTE *)RTR_addr(impt[i]);

						offset = (UWORD)ascnum(RTD_lookup(xexpt, tag));

						if (offset != (ULONG)-1)
						{
							found = 1;
							index += offset;
						}

						RTR_unlock(xexpt);
					} else
						index += xprg.static_size;

					RTR_unlock(xcode);
				}

				if (!found)
					abend(MSG_UER, tag);  //"Unresolved external reference '%s'"

				thunk_ptr = RTR_addr(thunk);
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

	MV = (MV_entry *)add_offset(RTR_addr(thunk), thdr.MV_list);

	for (i = m = 0; i < depth; i++)
	{
		dict = (UBYTE *)RTD_first(RTR_addr(expt[i]));
		while ((dict = (UBYTE *)RTD_iterate(RTR_addr(expt[i]), dict, &tag, &def)) != NULL)
			if (tag[0] == 'M')
			{
				MV[m].msg = (UWORD)ascnum(&tag[2]);
				MV[m].handler = (ULONG)ascnum(def);
				MV[m].SD_offset = SD_offset[i];
				++m;
			}
	}

	if (m)
	{
		//
		// Sort all message vectors by ascending message number
		//

		qsort(MV, m, sizeof(MV_entry), sort_by_msg);

		//
		// Sort identical message vectors by ascending class
		//

		k = m - 1;
		for (i = 0; i < k; i++)
			if (MV[i + 1].msg == MV[i].msg)
			{
				for (j = i + 2; j < m; j++)
					if (MV[j].msg != MV[i].msg) break;

				qsort(&MV[i], j - i, sizeof(MV_entry), sort_by_class);

				i = j - 1;
			}
	}

	//
	// Unlock dictionaries and exit w/handle to thunk
	// 

	RTR_unlock(HCRFD);
	for (i = 0; i < depth; i++)
	{
		RTR_unlock(impt[i]);
		RTR_unlock(expt[i]);
		RTR_unlock(code[i]);
	}

	return thunk;
}

/***************************************************/
//
// Create SOP instance, building thunk if necessary
//
/***************************************************/

HRES create_instance(RTR_class *rtr, ULONG object) {
	HRES thunk, instance;
	HD_entry *sel;
	ND_entry *entry;
	THDR *thdr;
	IHDR ihdr;

	entry = RTR_search_name_dir(rtr, object);

	if ((entry == NULL) || (entry->thunk == (ULONG)-1))
	{
		thunk = construct_thunk(rtr, rtr, object);

		RTR_search_name_dir(rtr, object)->thunk = thunk;
	} else
		thunk = (HRES)entry->thunk;

	ihdr.thunk = thunk;

	thdr = (THDR *)RTR_addr(thunk);
	++thdr->use_cnt;

	instance = RTR_alloc(rtr, thdr->isize, DA_MOVEABLE | DA_PRECIOUS);

	sel = (HD_entry *)instance;

	sel->user = object;

	*(IHDR *)RTR_addr(instance) = ihdr;

	return instance;
}

/***************************************************/
//
// Destroy instance of specified SOP
//             
// Delete thunk if no other instances of this object exist
//
/***************************************************/

void destroy_instance(RTR_class *rtr, HRES instance) {
	HRES thunk;

	thunk = ((IHDR *)RTR_addr(instance))->thunk;

	if (!(--(((THDR *)RTR_addr(thunk))->use_cnt)))
		RTR_free(rtr, thunk);

	RTR_free(rtr, instance);
}

} // namespace Aesop
