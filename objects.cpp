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
#include "aesop/aesop.h"
#include "aesop/objects.h"
#include "aesop/files.h"
#include "aesop/rtmsg.h"
#include "aesop/utils.h"

namespace Aesop {

Objects::Objects(AesopEngine *vm) : _vm(vm) {
	Common::fill(&_objList[0], &_objList[NUM_OBJECTS], HRES_NULL);
}

int Objects::findFreeEntry(int min, int end) {
	for (int i = min; i < end; ++i)
		if (_objList[i] == HRES_NULL)
			return i;

	return -1;
}

void Objects::createSOPinstance(uint name, int index) {
	_objList[index] = _vm->_resources->create_instance(name);

	_vm->_interpreter->execute(index, MSG_CREATE);
}

int Objects::createObject(uint name) {
	int index = findFreeEntry(0, NUM_ENTITIES);

	if (index != -1)
		createSOPinstance(name, index);

	return index;
}

int Objects::createProgram(int index, uint name) {
	if (index == -1)
		index = findFreeEntry(NUM_ENTITIES, NUM_OBJECTS);

	if (index != -1)
		createSOPinstance(name, index);

	return index;
}

int Objects::destroyObject(int index) {
	int rtn = _vm->_interpreter->execute(index, MSG_DESTROY);

	_vm->_events->cancelEntityRequests(index);
	_vm->_screen->releaseOwnedWindows(index);

	_vm->_resources->destroy_instance(_objList[index]);
	_objList[index] = HRES_NULL;

	return rtn;
}

void Objects::thrashCache() {
	HRES handles[50];

	for (int i = 0; i < 3; ++i) {
		handles[i] = _vm->_resources->alloc(i * 20000, DA_MOVEABLE | DA_DISCARDABLE);
	}

	for (int i = 0; i < 3; ++i) {
		_vm->_resources->free(handles[i]);
	}
}

uint Objects::flushCache(uint goal) {
	return _vm->_resources->force_discard(goal);
}


/***************************************************/
//
// Dump object's static context to text file
//
/***************************************************/

void Objects::dumpStaticContext(uint index, TextFile *TF) {
	Interpreter &interp = *_vm->_interpreter;
	Resources &res = *_vm->_resources;
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
	Common::String line;

	line = Common::String::format("Entry %d: ", (int)index);

	instance = _objList[index];
	if (instance == HRES_NULL) {
		line += "Available";
		TF->writeln(line);
		return;
	}

	thunk = ((IHDR *)Resources::addr(instance))->_thunk;

	tptr = Resources::addr(thunk);
	thdr = *((THDR *)tptr);

	SD = (SD_entry *)add_ptr(tptr, thdr._sdList);

	expt = res.get_resource_handle(SD[thdr._nPrgs - 1].exports,
		DA_TEMPORARY | DA_EVANESCENT);
	res.lock(expt);

	line += '"';
	line += (const char *)interp.lookup(expt, "N:OBJECT");
	line += '"';

	res.unlock(expt);

	TF->writeln(line);
	TF->writeln("{");

	for (p = 0; p < thdr._nPrgs; p++) {
		tptr = Resources::addr(thunk);
		SD = (SD_entry *)add_ptr(tptr, thdr._sdList);

		offset = SD[p].static_base;

		expt = res.get_resource_handle(SD[p].exports,
			DA_TEMPORARY | DA_EVANESCENT);
		res.lock(expt);

		def = (const char *)interp.lookup(expt, "N:OBJECT");

		if (p)
			TF->writeln("");
		line = Common::String::format("[%s]", def);
		TF->writeln(line);

		inst = add_ptr(Resources::addr(instance), offset);

		dict = interp.first(Resources::addr(expt));
		while ((dict = interp.iterate(Resources::addr(expt), dict, &tag, &def)) != 0) {
			type = tag[0];
			if ((type != 'B') && (type != 'W') && (type != 'L'))
				continue;

			line = Common::String::format("   %s = ", tag);

			d = add_ptr(inst, ascnum(def));
			if ((size = strchr(def, ',')) != NULL)
				asize = (UWORD)ascnum(size + 1);
			else
				asize = 1;

			for (n = 0; n < asize; n++) {
				switch (type) {
				case 'B':
					sprintf(val, "%d", *(BYTE *)d);
					d = add_ptr(d, 1L);
					break;
				case 'W':
					sprintf(val, "%d", READ_LE_UINT16(d));
					d = add_ptr(d, 2L);
					break;
				case 'L':
					sprintf(val, "%d", (int32)READ_LE_UINT32(d));
					d = add_ptr(d, 4L);
					break;
				}

				line += val;

				if (n != asize - 1) {
					line += ",";

					if (!((n + 1) % 10)) {
						TF->writeln(line);
						line = "      ";
					}
				}

			}

			TF->writeln(line);
		}

		res.unlock(expt);
	}

	TF->writeln("}");
}

} // End of namespace Aesop

