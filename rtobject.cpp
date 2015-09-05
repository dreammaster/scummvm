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

#include "common/file.h"
#include "aesop/aesop.h"
#include "aesop/defs.h"
#include "aesop/shared.h"
#include "aesop/resources.h"
#include "aesop/rtsystem.h"
#include "aesop/rtmsg.h"
#include "aesop/intrface.h"
#include "aesop/rtobject.h"
#include "aesop/event.h"
#include "aesop/graphics.h"

namespace Aesop {

extern ULONG check_on;

//
// objlist:  List of handles to all entities in "universe"
//

HRES objlist[NUM_OBJECTS];

//
// Other static vars
//

static char linbuf[256];
//static char name[256];

/***************************************************/
//
// Initialize object lists
//
// All objlist entries = -1 (available)
//
/***************************************************/

void init_object_list(void) {
   LONG i;

   for (i=0;i<NUM_OBJECTS;i++)
      objlist[i] = HRES_NULL;
}

/***************************************************/
//
// Return index of first free object list entry within given
// range, or -1 if none available
//
/***************************************************/

LONG find_free_entry(LONG min, LONG end)
{
   LONG i;

   for (i=min;i<end;i++)
      if (objlist[i] == HRES_NULL)
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

void create_SOP_instance(ULONG name, LONG index)
{
	Interpreter &interp = *_vm->_interpreter;
	Resources &res = *_vm->_resources;
	objlist[index] = res.create_instance(name);

	interp.execute(index, MSG_CREATE, HRES_NULL);
}

/***************************************************/
//
// Create a SOP object and return its object list index
// 
// Returns -1 if no space in list
// 
/***************************************************/


LONG create_object(LONG argcnt, ULONG name)

{
   LONG index;

   index = find_free_entry(0,NUM_ENTITIES);

   if (index != -1)
      create_SOP_instance(name,index);

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


LONG create_program(LONG argcnt, LONG index, ULONG name)

{
   if (index == -1)
      index = find_free_entry(NUM_ENTITIES,NUM_OBJECTS);

   if (index != -1)
      create_SOP_instance(name,index);

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


LONG destroy_object(LONG argcnt, LONG index) {
	Interpreter &interp = *_vm->_interpreter;
	Resources &res = *_vm->_resources;
	LONG rtn;

	rtn = interp.execute(index, MSG_DESTROY, HRES_NULL);

	cancel_entity_requests(index);
	release_owned_windows(index);

	res.destroy_instance(objlist[index]);

	objlist[index] = HRES_NULL;

	return rtn;
}

/***************************************************/
//
// Thrash resource cache (for diagnostic purposes)
//
/***************************************************/

void thrash_cache(void)
{
	Resources &res = *_vm->_resources;
	LONG i;
	HRES handles[50];

   for (i=0;i<3;i++)
      {
      handles[i] = res.alloc(i*20000,DA_MOVEABLE | DA_DISCARDABLE);
      }

   for (i=0;i<3;i++)
      {
      res.free(handles[i]);
      }
}

/***************************************************/
//
// Flush resource cache (for performance-tuning purposes)
//
/***************************************************/


ULONG flush_cache(LONG argcnt, ULONG goal)

{
	Resources &res = *_vm->_resources;
	return res.force_discard(goal);
}

/***************************************************/
//
// Dump object's static context to text file
//
/***************************************************/

void dump_static_context(ULONG index, TF_class *TF) {
	Interpreter &interp = *_vm->_interpreter;
	Resources &res = *_vm->_resources;
	LONG i;
	ULONG n, p, offset, asize;
	HRES instance, thunk, expt;
	void *tptr;
	SD_entry *SD;
	THDR thdr;
	char val[32];
	uint dict;
	BYTE type;
	const char *tag;
	const char *def, *size;
	void *inst, *d;

	strcpy(linbuf, "Entry ");
	sprintf(&linbuf[6], "%d", (int)index);	

	i = strlen(linbuf);
	linbuf[i] = ':';
	linbuf[i + 1] = ' ';
	linbuf[i + 2] = 0;

	instance = objlist[index];

	if (instance == HRES_NULL)
	{
		strcat(linbuf, "Available");
		TF_writeln(TF, linbuf);
		return;
	}

	thunk = ((IHDR *)Resources::addr(instance))->_thunk;

	tptr = Resources::addr(thunk);
	thdr = *((THDR *)tptr);

	SD = (SD_entry *)add_ptr(tptr, thdr._sdList);

	expt = res.get_resource_handle(SD[thdr._nPrgs - 1].exports,
		DA_TEMPORARY | DA_EVANESCENT);
	res.lock(expt);
	linbuf[i + 2] = '"';
	strcpy(&linbuf[i + 3], (const char *)interp.lookup(expt, "N:OBJECT"));
	strcat(&linbuf[i + 3], "\"");

	res.unlock(expt);

	TF_writeln(TF, linbuf);
	TF_writeln(TF, "{");

	for (p = 0; p < thdr._nPrgs; p++)
	{
		tptr = Resources::addr(thunk);
		SD = (SD_entry *)add_ptr(tptr, thdr._sdList);

		offset = SD[p].static_base;

		expt = res.get_resource_handle(SD[p].exports,
			DA_TEMPORARY | DA_EVANESCENT);
		res.lock(expt);

		def = (const char *)interp.lookup(expt, "N:OBJECT");

		if (p) TF_writeln(TF, "");
		linbuf[0] = '[';
		strcpy(&linbuf[1], def);
		strcat(linbuf, "]");
		TF_writeln(TF, linbuf);

		inst = add_ptr(Resources::addr(instance), offset);

		dict = interp.first(Resources::addr(expt));
		while ((dict = interp.iterate(Resources::addr(expt), dict, &tag, &def)) != 0)
		{
			type = tag[0];
			if ((type != 'B') && (type != 'W') && (type != 'L'))
				continue;

			strcpy(linbuf, "   ");
			strcpy(&linbuf[3], (const char *)tag);
			strcat(linbuf, " = ");

			d = add_ptr(inst, ascnum(def));
			if ((size = strchr(def, ',')) != NULL)
				asize = (UWORD)ascnum(size + 1);
			else
				asize = 1;

			for (n = 0; n < asize; n++)
			{
				switch (type)
				{
				case 'B': sprintf(val, "%d", *(BYTE *)d); d = add_ptr(d, 1L); break;
				case 'W': sprintf(val, "%d", READ_LE_UINT16(d)); d = add_ptr(d, 2L); break;
				case 'L': sprintf(val, "%d", (int32)READ_LE_UINT32(d)); d = add_ptr(d, 4L); break;
				}

				strcat(linbuf, val);

				if (n != asize - 1)
				{
					strcat(linbuf, ",");

					if (!((n + 1) % 10))
					{
						TF_writeln(TF, linbuf);
						strcpy(linbuf, "      ");
					}
				}

			}

			TF_writeln(TF, linbuf);
		}

		res.unlock(expt);
	}

	TF_writeln(TF, "}");
}

/***************************************************/
//
// Read line from context file, skipping comment lines
//
/***************************************************/


LONG readln(TF_class *TF, BYTE *buffer, LONG maxlen)

{
   LONG status;

   do
      status = TF_readln(TF,linbuf,sizeof(linbuf));
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

CDESC *read_context_descriptor(TF_class *TF)
{
	Interpreter &interp = *_vm->_interpreter;
	Resources &res = *_vm->_resources;
	static CDESC c;
	HRES HROED;
	char *num;
	char *name;
	const char *def;

	if (!readln(TF, (BYTE *)linbuf, sizeof(linbuf)))
		return NULL;

	num = &linbuf[6];

	c.size = (UWORD)-1;
	c.slot = (UWORD)ascnum(num);

	name = strchr(num, '"');
	if (name == NULL)
	{
		c.name = (ULONG)-1L;
		return &c;
	}

	name++;
	name[strlen(name) - 1] = 0;

	HROED = res.get_resource_handle(ROED, DA_TEMPORARY | DA_EVANESCENT);
	res.lock(HROED);

	def = (const char *)interp.lookup(HROED, name);

	if (def == NULL)
		abend(MSG_OMCR, name, c.slot);

	c.name = ascnum(def);

	res.unlock(HROED);

	return &c;
}

/***************************************************/
//
// Restore object's static context from text file
//
/***************************************************/

void restore_static_context(HRES instance, TF_class *TF)
{
	Interpreter &interp = *_vm->_interpreter;
	Resources &res = *_vm->_resources;
	ULONG n, i, p, offset = 0, asize;
	HRES thunk, expt = 0;
	void *d, *tptr;
	SD_entry *SD;
	THDR thdr;
	const char *def, *size;
	char *tag, *chrpnt;
	static char name[256];

	thunk = ((IHDR *)Resources::addr(instance))->_thunk;
	thdr = *((THDR *)Resources::addr(thunk));

	while (readln(TF, (BYTE *)linbuf, sizeof(linbuf)))
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

			for (p = 0; p < thdr._nPrgs; p++)
			{
				tptr = Resources::addr(thunk);
				SD = (SD_entry *)add_ptr(tptr, thdr._sdList);

				offset = SD[p].static_base;

				expt = res.get_resource_handle(SD[p].exports,
					DA_TEMPORARY | DA_EVANESCENT);

				res.lock(expt);
				def = (const char *)interp.lookup(expt, "N:OBJECT");

				res.unlock(expt);

				if (!strcmp(def, tag))
					break;
			}

			if (p == thdr._nPrgs)
				abend(MSG_CMCR, tag);       //"Class '%s' missing; cannot restore"
		}
		else
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

			res.lock(expt);
			def = (const char *)interp.lookup(expt, name);

			res.unlock(expt);

			if (def == NULL)
				abend(MSG_UVR, name);       //"Unresolved variable reference '%s'"

			d = (void *)((ULONG)Resources::addr(instance) + ascnum(def) + offset);

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
					readln(TF, (BYTE *)linbuf, sizeof(linbuf));
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

LONG save_range(const char *filename, ULONG filetype, ULONG first, ULONG last)
{
	LONG good;
	ULONG index;
   UBYTE typetest;
   TF_class *TF;
   HRES instance,thunk;
   HD_entry *hd_inst;
   void *tptr;
   THDR thdr;
   CDESC CD;

   good = 1;

   if (filetype == SF_TXT)
      {
      TF = TF_construct(filename,TF_WRITE);
      if (TF == NULL)
         return 0;

      for (index=first;index<=last;index++)
         {
         dump_static_context(index,TF);

         if (!TF_writeln(TF,""))
            {
            good = 0;
            break;
            };
         }

      TF_destroy(TF);
      }
   else
      {
		  Common::DumpFile f;
		  if (!f.open(filename))
			  return 0;

      typetest = 26;
	  f.writeByte(typetest);

      for (index=first;index<=last;index++)
         {
         CD.slot = index;

         instance = objlist[index];
         if (instance == HRES_NULL)
            {
            CD.name = (ULONG)-1;
            CD.size = 0;
            }
         else
            {
            thunk = ((IHDR *) Resources::addr(instance))->_thunk;

            tptr = Resources::addr(thunk);
            thdr = *((THDR *) tptr);

            hd_inst = (HD_entry *) instance;
            CD.name = hd_inst->_user;
            CD.size = thdr._iSize - sizeof(IHDR);
            }

		 error("TODO: CDESC::save & IHDR::save");
         //f.write(&CD,sizeof(CDESC));

         if (CD.size)
            if (f.write(add_ptr(Resources::addr(objlist[index]),sizeof(IHDR)),
                      CD.size) != CD.size)
               {
               good = 0;
               break;
               }
         }

      f.close();
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

void restore_range(const char *filename, ULONG first, ULONG last, ULONG restoring)
{
	Interpreter &interp = *_vm->_interpreter;
	ULONG bad, txttype;
	ULONG index;
	UBYTE typetest;
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

	Common::File f;
	if (!f.open(filename))
		bad = 1;
	else
	{
		typetest = 0;
		typetest = f.readByte();

		if (typetest != 26)
		{
			f.close();
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
		}
		else
		{
			CD = &stat_C;
			error("TODO: CDESC::load");
			bad = f.read(CD, sizeof(CDESC)) != sizeof(CDESC);
		}

		if ((bad) || (CD->slot != index))
			abend(MSG_CFCAE, index);          //"Context file corrupt at entry %u"

		CDslot = CD->slot;
		CDname = CD->name;
		CDsize = CD->size;

		if (CD->name == (ULONG)-1L)
		{
			if (cur != HRES_NULL)
			{
				destroy_object(0, index);
			}

			continue;
		}

		if (cur != HRES_NULL)
		{
			sel = (HD_entry *)cur;

			if (sel->_user != CD->name)
			{
				destroy_object(0, index);
			}
			else
			{
				cancel_entity_requests(index);

				release_owned_windows(index);
			}
		}

		if (objlist[index] == HRES_NULL)
		{
			create_SOP_instance(CD->name, index);
		}

		if (txttype)
		{
			restore_static_context(objlist[index], TF);
		}
		else
			if (CD->size)
			{
				f.read((BYTE *)Resources::addr(objlist[index]) + sizeof(IHDR), CD->size);
			}

		if (restoring)
		{
			interp.execute(index, MSG_RESTORE, HRES_NULL);
		}
	}

	if (txttype)
		TF_destroy(TF);
	else
		f.close();
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

void translate_file(const char *TXT_filename, const char *BIN_filename, ULONG first,
	ULONG last)
{
	Resources &res = *_vm->_resources;
	TF_class *TF;
	CDESC *CD;
	CDESC CD_out;
	LONG index;
	HRES instance, thunk;
	void *tptr;
	THDR thdr;
	UBYTE typetest;

	Common::DumpFile f;
	if (!f.open(BIN_filename))
		abend(MSG_COOFFT);      //"Couldn't open output file for translation"

	typetest = 26;
	f.writeByte(typetest);

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

		if (CD->name != (ULONG)-1L)
		{
			instance = res.create_instance(CD->name);

			thunk = ((IHDR *)Resources::addr(instance))->_thunk;

			tptr = Resources::addr(thunk);
			thdr = *((THDR *)tptr);

			CD_out.size = thdr._iSize - sizeof(IHDR);

			error("TODO: CDESC::save");
			//write(handle,&CD_out,sizeof(CDESC));

			restore_static_context(instance, TF);

			if (CD_out.size)
			{
				if (f.write(add_ptr(Resources::addr(instance), sizeof(IHDR)), CD_out.size)
					!= CD_out.size)
				{
					abend(MSG_CWTE, index);  //"Couldn't write translated entry %u"
				}
			}

			res.destroy_instance(instance);
		}
		else
		{
			CD_out.size = 0;
			error("TODO: CDESC::save");
			//write(handle,&CD_out,sizeof(CDESC));
		}
	}

	TF_destroy(TF);
	f.close();
}

} // End of namespace Aesop

