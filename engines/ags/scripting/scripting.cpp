/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

/* Based on the Adventure Game Studio source code, copyright 1999-2011 Chris Jones,
 * which is licensed under the Artistic License 2.0.
 * You may also modify/distribute the code in this file under that license.
 */

#include "engines/ags/scripting/scripting.h"
#include "common/debug.h"

namespace AGS {

RuntimeValue Script_UnimplementedStub(AGSEngine *vm, ScriptObject *self, const Common::Array<RuntimeValue> &params) {
	error("call to unimplemented system scripting function");
}

void GlobalScriptState::addImport(const Common::String &name, const ScriptImport &import, bool forceReplace) {
	if (_imports.contains(name)) {
		if (forceReplace) {
			ScriptImport oldImport = _imports[name];
			if (oldImport._type == sitSystemObject) {
				// TODO: Should this ever happen?
				if (oldImport._object->getRefCount() != 1)
					error("replacing referenced (%d) import '%s'", oldImport._object->getRefCount(), name.c_str());
				oldImport._object->DecRef();
			}
		} else {
			// this happens a lot (e.g. 'on_event'/'repeatedly_execute')
			debug(2, "duplicate exported '%s'", name.c_str());
			return;
		}
	}

	if (import._type == sitSystemObject)
		import._object->IncRef();
	_imports[name] = import;
}

void GlobalScriptState::removeImport(const Common::String &name) {
	if (!_imports.contains(name)) {
		warning("tried to remove non-existent import '%s'", name.c_str());
		return;
	}

	ScriptImport oldImport = _imports[name];
	if (oldImport._type == sitSystemObject)
		oldImport._object->DecRef();
	_imports.erase(name);
}

void GlobalScriptState::addSystemFunctionImport(const ScriptSystemFunctionInfo *function) {
	ScriptImport import;

	import._type = sitSystemFunction;
	import._function = function;

	addImport(function->name, import, true);
}

void GlobalScriptState::addSystemObjectImport(const Common::String &name, ScriptObject *object) {
	ScriptImport import;

	import._type = sitSystemObject;
	import._object = object;

	addImport(name, import, true);
}

void GlobalScriptState::addSystemFunctionImportList(const ScriptSystemFunctionInfo *list, uint32 count) {
	for (uint i = 0; i < count; ++i)
		addSystemFunctionImport(&list[i]);
}

extern void addAudioSystemScripting(AGSEngine *vm);
extern void addCharacterSystemScripting(AGSEngine *vm);
extern void addDialogSystemScripting(AGSEngine *vm);
extern void addFileSystemScripting(AGSEngine *vm);
extern void addFlashlightSystemScripting(AGSEngine *vm);
extern void addGameSystemScripting(AGSEngine *vm);
extern void addGraphicsSystemScripting(AGSEngine *vm);
extern void addGuiSystemScripting(AGSEngine *vm);
extern void addInputSystemScripting(AGSEngine *vm);
extern void addInventorySystemScripting(AGSEngine *vm);
extern void addMiscSystemScripting(AGSEngine *vm);
extern void addObjectSystemScripting(AGSEngine *vm);
extern void addParserSystemScripting(AGSEngine *vm);
extern void addRoomSystemScripting(AGSEngine *vm);
extern void addSnowRainSystemScripting(AGSEngine *vm);
extern void addStringSystemScripting(AGSEngine *vm);
extern void addUtilsSystemScripting(AGSEngine *vm);

void addSystemScripting(AGSEngine *vm) {
	addAudioSystemScripting(vm);
	addDialogSystemScripting(vm);
	addCharacterSystemScripting(vm);
	addFileSystemScripting(vm);
	addFlashlightSystemScripting(vm);
	addGameSystemScripting(vm);
	addGraphicsSystemScripting(vm);
	addGuiSystemScripting(vm);
	addInputSystemScripting(vm);
	addInventorySystemScripting(vm);
	addMiscSystemScripting(vm);
	addObjectSystemScripting(vm);
	addParserSystemScripting(vm);
	addRoomSystemScripting(vm);
	addSnowRainSystemScripting(vm);
	addStringSystemScripting(vm);
	addUtilsSystemScripting(vm);
}

} // End of namespace AGS
