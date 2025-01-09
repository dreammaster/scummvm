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

#include "aesop/rtobject.h"
#include "aesop/shared.h"
#include "aesop/rtres.h"
#include "aesop/rtlink.h"
#include "aesop/rtsystem.h"
#include "aesop/rtmsg.h"
#include "aesop/interface.h"
#include "aesop/rtobject.h"
#include "aesop/rt.h"
#include "aesop/event.h"
#include "aesop/graphics.h"
#include "aesop/vars.h"
#include "aesop/system/files.h"

namespace Aesop {

extern ULONG check_on;

//
// objlist:  List of handles to all entities in "universe"
//

HRES objlist[NUM_OBJECTS];

//
// Other static vars
//

static BYTE linbuf[256];
static BYTE name[256];

/***************************************************/
//
// Initialize object lists
//
// All objlist entries = -1 (available)
//
/***************************************************/

void init_object_list(void) {
	LONG i;

	for (i = 0; i < NUM_OBJECTS; i++)
		objlist[i] = (HRES)-1;
}

/***************************************************/
//
// Return index of first free object list entry within given
// range, or -1 if none available
//
/***************************************************/

LONG find_free_entry(LONG min, LONG end) {
	LONG i;

	for (i = min; i < end; i++)
		if (objlist[i] == (ULONG)-1)
			break;

	if (i == end)
		return -1;
	else
		return i;
}

/***************************************************/
//
// Create an object at a given objlist index
//
/***************************************************/

void create_SOP_instance(ULONG newName, LONG index) {
	objlist[index] = create_instance(RTR, newName);

	RT_execute(index, MSG_CREATE, (ULONG)-1);
}

/***************************************************/
//
// Create a SOP object and return its object list index
// 
// Returns -1 if no space in list
// 
/***************************************************/

#pragma off (unreferenced)
LONG create_object(LONG argcnt, ULONG newName)
#pragma on (unreferenced)
{
	LONG index;

	index = find_free_entry(0, NUM_ENTITIES);

	if (index != -1)
		create_SOP_instance(newName, index);

	return index;
}

/***************************************************/
//
// Create a SOP program object at specified object list index
// 
// Dynamically assign an unused program object index if specified 
// index == -1
// 
/***************************************************/

#pragma off (unreferenced)
LONG create_program(LONG argcnt, LONG index, ULONG newName)
#pragma on (unreferenced)
{
	if (index == -1)
		index = find_free_entry(NUM_ENTITIES, NUM_OBJECTS);

	if (index != -1)
		create_SOP_instance(newName, index);

	return index;
}

/***************************************************/
//
// Delete SOP object from object list
// 
// Cancel any notifications for object, and release any
// windows allocated by object
// 
/***************************************************/

#pragma off (unreferenced)
LONG destroy_object(LONG argcnt, LONG index)
#pragma on (unreferenced)
{
	LONG rtn;

	rtn = RT_execute(index, MSG_DESTROY, (ULONG)-1);

	cancel_entity_requests(index);
	release_owned_windows(index);

	destroy_instance(RTR, objlist[index]);

	objlist[index] = (ULONG)-1;

	return rtn;
}

/***************************************************/
//
// Thrash resource cache (for diagnostic purposes)
//
/***************************************************/

void thrash_cache(void) {
	LONG i, handles[50];

	for (i = 0; i < 3; i++)
	{
		handles[i] = RTR_alloc(RTR, i * 20000, DA_MOVEABLE | DA_DISCARDABLE);
	}

	for (i = 0; i < 3; i++)
	{
		RTR_free(RTR, handles[i]);
	}
}

/***************************************************/
//
// Flush resource cache (for performance-tuning purposes)
//
/***************************************************/

#pragma off (unreferenced)
ULONG flush_cache(LONG argcnt, ULONG goal)
#pragma on (unreferenced)
{
	return RTR_force_discard(RTR, goal);
}

/***************************************************/
//
// Dump object's static context to text file
//
/***************************************************/

void dump_static_context(ULONG index, TF_class *TF) {
	LONG i;
	ULONG n, p, offset, asize;
	HRES instance, thunk, expt;
	void *tptr;
	SD_entry *SD;
	THDR thdr;
	BYTE val[32];
	UBYTE *dict;
	BYTE type, *tag, *def, *size;
	void *inst, *d;

	Common::strcpy_s(linbuf, "Entry ");
	ltoa(index, &linbuf[6], 10);

	i = strlen(linbuf);
	linbuf[i] = ':';
	linbuf[i + 1] = ' ';
	linbuf[i + 2] = 0;

	instance = objlist[index];

	if (instance == (ULONG)-1)
	{
		Common::strcat_s(linbuf, "Available");
		TF_writeln(TF, linbuf);
		return;
	}

	thunk = ((IHDR *)RTR_addr(instance))->thunk;

	tptr = RTR_addr(thunk);
	thdr = *((THDR *)tptr);

	SD = (SD_entry *)add_ptr(tptr, thdr.SD_list);

	expt = RTR_get_resource_handle(RTR, SD[thdr.nprgs - 1].exports,
		DA_TEMPORARY | DA_EVANESCENT);
	RTR_lock(RTR, expt);
	linbuf[i + 2] = '"';
	Common::strcpy_s(&linbuf[i + 3], 253 - i, (const char *)RTD_lookup(expt, "N:OBJECT"));
	Common::strcat_s(&linbuf[i + 3], 253 - i, "\"");

	RTR_unlock(expt);

	TF_writeln(TF, linbuf);
	TF_writeln(TF, "{");

	for (p = 0; p < thdr.nprgs; p++)
	{
		tptr = RTR_addr(thunk);
		SD = (SD_entry *)add_ptr(tptr, thdr.SD_list);

		offset = SD[p].static_base;

		expt = RTR_get_resource_handle(RTR, SD[p].exports,
			DA_TEMPORARY | DA_EVANESCENT);
		RTR_lock(RTR, expt);

		def = RTD_lookup(expt, "N:OBJECT");

		if (p) TF_writeln(TF, "");
		linbuf[0] = '[';
		Common::strcpy_s(&linbuf[1], 255, def);
		Common::strcat_s(linbuf, "]");
		TF_writeln(TF, linbuf);

		inst = add_ptr(RTR_addr(instance), offset);

		dict = (UBYTE *) RTD_first(RTR_addr(expt));
		while ((dict = (UBYTE *)RTD_iterate(RTR_addr(expt), dict, &tag, &def)) != NULL)
		{
			type = tag[0];
			if ((type != 'B') && (type != 'W') && (type != 'L'))
				continue;

			Common::strcpy_s(linbuf, "   ");
			Common::strcpy_s(&linbuf[3], 253, tag);
			Common::strcat_s(linbuf, " = ");

			d = add_ptr(inst, ascnum(def));
			if ((size = strchr(def, ',')) != NULL)
				asize = (UWORD)ascnum(size + 1);
			else
				asize = 1;

			for (n = 0; n < asize; n++)
			{
				switch (type)
				{
				case 'B': ltoa(*(BYTE *)d, val, 10); d = add_ptr(d, 1L); break;
				case 'W': ltoa(*(WORD *)d, val, 10); d = add_ptr(d, 2L); break;
				case 'L': ltoa(*(LONG *)d, val, 10); d = add_ptr(d, 4L); break;
				}

				Common::strcat_s(linbuf, val);

				if (n != asize - 1)
				{
					Common::strcat_s(linbuf, ",");

					if (!((n + 1) % 10))
					{
						TF_writeln(TF, linbuf);
						Common::strcpy_s(linbuf, "      ");
					}
				}

			}

			TF_writeln(TF, linbuf);
		}

		RTR_unlock(expt);
	}

	TF_writeln(TF, "}");
}

/***************************************************/
//
// Read line from context file, skipping comment lines
//
/***************************************************/

#pragma off (unreferenced)
LONG readln(TF_class *TF, BYTE *buffer, LONG maxlen)
#pragma on (unreferenced)
{
	LONG status;

	do
		status = TF_readln(TF, linbuf, sizeof(linbuf));
	while (status && (linbuf[0] == ';'));

	return status;
}

/***************************************************/
//
// Fetch slot and ordinal entry # from context file
//
// Return NULL if EOF reached
//
/***************************************************/

CDESC *read_context_descriptor(TF_class *TF) {
	static CDESC c;
	HRES hroed;
	BYTE *num;
	BYTE *cName;
	BYTE *def;

	if (!readln(TF, linbuf, sizeof(linbuf)))
		return NULL;

	num = &linbuf[6];

	c.size = (UWORD)-1;
	c.slot = (UWORD)ascnum(num);

	cName = strchr(num, '"');
	if (cName == NULL)
	{
		c.name = (ULONG)-1;
		return &c;
	}

	cName++;
	cName[strlen(cName) - 1] = 0;

	hroed = RTR_get_resource_handle(RTR, ROED, DA_TEMPORARY | DA_EVANESCENT);
	RTR_lock(RTR, hroed);

	def = RTD_lookup(hroed, cName);

	if (def == NULL)
		abend(MSG_OMCR, cName, c.slot);

	c.name = ascnum(def);

	RTR_unlock(hroed);

	return &c;
}

/***************************************************/
//
// Restore object's static context from text file
//
/***************************************************/

void restore_static_context(HRES instance, TF_class *TF) {
	ULONG n, i, p, offset = 0, asize;
	HRES thunk, expt = 0;
	void *d, *tptr;
	SD_entry *SD;
	THDR thdr;
	BYTE *tag, *def, *size, *chrpnt;

	thunk = ((IHDR *)RTR_addr(instance))->thunk;
	thdr = *((THDR *)RTR_addr(thunk));

	while (readln(TF, linbuf, sizeof(linbuf)))
	{
		if (linbuf[0] == '{')
			continue;
		else if (linbuf[0] == '}')
			break;
		else if (linbuf[0] == '[')
		{
			if ((tag = strchr(linbuf, ']')) != NULL)
				*tag = 0;
			tag = strchr(linbuf, '[') + 1;

			for (p = 0; p < thdr.nprgs; p++)
			{
				tptr = RTR_addr(thunk);
				SD = (SD_entry *)add_ptr(tptr, thdr.SD_list);

				offset = SD[p].static_base;

				expt = RTR_get_resource_handle(RTR, SD[p].exports,
					DA_TEMPORARY | DA_EVANESCENT);

				RTR_lock(RTR, expt);
				def = RTD_lookup(expt, "N:OBJECT");

				RTR_unlock(expt);

				if (!strcmp(def, tag))
					break;
			}

			if (p == thdr.nprgs)
				abend(MSG_CMCR, tag);       //"Class '%s' missing; cannot restore"
		} else
		{
			tag = linbuf;

			//Skip leading white-space

			while (*tag == ' ')
				tag++;

			//Get Variable Type and Name

			for (i = 0; (tag[i] != ' ') && tag[i]; i++)
				name[i] = tag[i];
			name[i] = 0;
			if (!i)
				continue;

			RTR_lock(RTR, expt);
			def = RTD_lookup(expt, name);

			RTR_unlock(expt);

			if (def == NULL)
				abend(MSG_UVR, name);       //"Unresolved variable reference '%s'"

			d = (void *)((ULONG)RTR_addr(instance) + ascnum(def) + offset);

			if ((size = strchr(def, ',')) != NULL)
				asize = ascnum(size + 1);
			else
				asize = 1;

			chrpnt = strchr(linbuf, '=');
			if (chrpnt == NULL)
				abend(MSG_BDIE);           //"Bad data item entry"

			for (n = 0; n < asize; n++)
			{

				//Skip to data

				while ((!isnum(*chrpnt)) && (*chrpnt != '\''))
				{
					if (!(*chrpnt))
						break;
					else
						++chrpnt;
				}

				//Get Next Line if needed

				if ((!(*chrpnt)) && (n != asize - 1))
				{
					readln(TF, linbuf, sizeof(linbuf));
					chrpnt = linbuf;
				}

				//Store data from text file in Instance

				switch (name[0])
				{
				case 'B': *(BYTE *)d = (BYTE)ascnum(chrpnt); break;
				case 'W': *(WORD *)d = (WORD)ascnum(chrpnt); break;
				case 'L': *(LONG *)d = (LONG)ascnum(chrpnt); break;
				}

				if (n != asize - 1)
				{
					switch (name[0])
					{
					case 'B': d = ((BYTE *)d + 1); break;
					case 'W': d = ((BYTE *)d + 2); break;
					case 'L': d = ((BYTE *)d + 4); break;
					}

					while (isnum(*chrpnt) || (*chrpnt == ' ') || (*chrpnt == '\''))
						chrpnt++;
				}
			} // for
		} // else
	} // while
}

/*********************************************************/
//
// Save static context of range of objects to text or binary file
//
// Return 1 if write succeeded (disk full or other error if 0)
//
/*********************************************************/

LONG save_range(BYTE *filename, LONG filetype, LONG first, LONG last) {
	LONG good, index;
	UBYTE typetest;
	Common::Stream *handle;
	TF_class *TF;
	HRES instance, thunk;
	HD_entry *hd_inst;
	void *tptr;
	THDR thdr;
	CDESC CD;

	good = 1;

	if (filetype == SF_TXT)
	{
		TF = TF_construct(filename, TF_WRITE);
		if (TF == NULL)
			return 0;

		for (index = first; index <= last; index++)
		{
			dump_static_context(index, TF);

			if (!TF_writeln(TF, ""))
			{
				good = 0;
				break;
			};
		}

		TF_destroy(TF);
	} else
	{
		handle = open(filename, O_CREAT | O_RDWR | O_TRUNC | O_BINARY);
		if (!handle) return 0;

		typetest = 26;
		write(handle, &typetest, 1);

		for (index = first; index <= last; index++)
		{
			CD.slot = index;

			instance = objlist[index];
			if (instance == (ULONG)-1)
			{
				CD.name = (ULONG)-1;
				CD.size = 0;
			} else
			{
				thunk = ((IHDR *)RTR_addr(instance))->thunk;

				tptr = RTR_addr(thunk);
				thdr = *((THDR *)tptr);

				hd_inst = (HD_entry *)instance;
				CD.name = hd_inst->user;
				CD.size = thdr.isize - sizeof(IHDR);
			}

			write(handle, &CD, sizeof(CDESC));

			if (CD.size)
				if (write(handle,
					add_ptr(RTR_addr(objlist[index]), sizeof(IHDR)),
					CD.size) != CD.size)
				{
					good = 0;
					break;
				}
		}

		close(handle);
	}

	return good;
}

/*********************************************************/
//
// Restore static context of range of objects from text or binary file
//
// For each object list entry between first and last:
//
//   Read entry's context descriptor
//
//   If EOF or CDESC.slot != entry #
//       Report corrupted context file
//
//   If CDESC.name == -1 (i.e., slot unused in file)
//       If slot entry exists, destroy it
//       Continue
//
//   If slot entry exists 
//       If CDESC.name != current entry's name
//          Destroy the current entry to make room for the new one
//       Else
//          Destruction of current entry not necessary; deallocate any
//          assigned resources but leave entry otherwise intact
//
//   If slot entry available (perhaps newly destroyed), create the object
//
//   Restore object's instance data from context file
//
//   If restoring, send "restore" message to newly instantiated object
//
// Errors during context restoration are fatal at the system level;
// this routine will not return if restoration fails
//
/*********************************************************/

void restore_range(BYTE *filename, ULONG first, ULONG last, ULONG restoring) {
	ULONG bad, txttype;
	ULONG index;
	UBYTE typetest;
	Common::Stream *handle;
	TF_class *TF = nullptr;
	HRES cur;
	HD_entry *sel;
	CDESC stat_C;
	CDESC *CD;

	UWORD CDslot;       // object list index
	ULONG CDname;       // code object name
	UWORD CDsize;       // size of instance data (unused in text files)

	txttype = 0;
	bad = 0;

	handle = open(filename, O_RDONLY | O_BINARY);
	if (!handle)
		bad = 1;
	else
	{
		typetest = 0;
		read(handle, &typetest, 1);

		if (typetest != 26)
		{
			close(handle);
			txttype = 1;
		}
	}

	if (txttype)
	{
		TF = TF_construct(filename, TF_READ);
		bad = (TF == NULL);
	}

	if (bad)
		abend(MSG_CNOCF, filename);          //"Could not open context file '%s'"

	for (index = first; index <= last; index++)
	{
		cur = objlist[index];

		if (txttype)
		{
			CD = read_context_descriptor(TF);

			bad = (CD == NULL);
		} else
		{
			CD = &stat_C;
			bad = (read(handle, CD, sizeof(CDESC)) != sizeof(CDESC));
		}

		if ((bad) || (CD->slot != index))
			abend(MSG_CFCAE, index);          //"Context file corrupt at entry %u"

		CDslot = CD->slot;
		CDname = CD->name;
		CDsize = CD->size;

		if (CD->name == (ULONG)-1)
		{
			if (cur != -1)
			{
				destroy_object(0, index);
			}

			continue;
		}

		if (cur != -1)
		{
			sel = (HD_entry *)cur;

			if (sel->user != CD->name)
			{
				destroy_object(0, index);
			} else
			{
				cancel_entity_requests(index);

				release_owned_windows(index);
			}
		}

		if (objlist[index] == (ULONG)-1)
		{
			create_SOP_instance(CD->name, index);
		}

		if (txttype)
		{
			restore_static_context(objlist[index], TF);
		} else
			if (CD->size)
			{
				read(handle, (BYTE *)RTR_addr(objlist[index]) + sizeof(IHDR), CD->size);
			}

		if (restoring)
		{
			RT_execute(index, MSG_RESTORE, (ULONG)-1);
		}
	}

	if (txttype)
		TF_destroy(TF);
	else
		close(handle);
}

/*********************************************************/
//
// Translate static context file from text to binary format
//
// For each entry in file:
// 
// Read entry's context descriptor
// Save descriptor to binary file
//
//   If EOF or CDESC.slot != entry #
//       Report corrupted context file
//
//   If CDESC.name == -1 (i.e., slot unused in file)
//       Continue
//
//   Create source object instance
//   Restore instance from text file
//   Save instance to binary file
//   Destroy instance
// 
// Errors during context file translation are fatal at the system level;
// this routine will not return if translation fails
//
/*********************************************************/

void translate_file(BYTE *TXT_filename, BYTE *BIN_filename, ULONG first,
	ULONG last) {
	TF_class *TF;
	CDESC *CD;
	CDESC CD_out;
	Common::Stream *handle;
	LONG index;
	HRES instance, thunk;
	void *tptr;
	THDR thdr;
	UBYTE typetest;

	handle = open(BIN_filename, O_CREAT | O_RDWR | O_TRUNC | O_BINARY);
	if (!handle)
		abend(MSG_COOFFT);      //"Couldn't open output file for translation"

	typetest = 26;
	write(handle, &typetest, 1);

	TF = TF_construct(TXT_filename, TF_READ);
	if (TF == NULL)
		abend(MSG_COIFFT);      //"Couldn't open input file for translation"

	for (index = first; index <= (LONG)last; index++)
	{
		CD = read_context_descriptor(TF);
		if ((CD == NULL) || (CD->slot != index))
			abend(MSG_CTCFE, index); //"Couldn't translate context file entry %u"

		CD_out.name = CD->name;
		CD_out.slot = index;

		if (CD->name != (ULONG)-1)
		{
			instance = create_instance(RTR, CD->name);

			thunk = ((IHDR *)RTR_addr(instance))->thunk;

			tptr = RTR_addr(thunk);
			thdr = *((THDR *)tptr);

			CD_out.size = thdr.isize - sizeof(IHDR);

			write(handle, &CD_out, sizeof(CDESC));

			restore_static_context(instance, TF);

			if (CD_out.size)
			{
				if (write(handle, add_ptr(RTR_addr(instance), sizeof(IHDR)), CD_out.size)
					!= CD_out.size)
				{
					abend(MSG_CWTE, index);  //"Couldn't write translated entry %u"
				}
			}

			destroy_instance(RTR, instance);
		} else
		{
			CD_out.size = 0;
			write(handle, &CD_out, sizeof(CDESC));
		}
	}

	TF_destroy(TF);
	close(handle);
}

} // namespace Aesop
