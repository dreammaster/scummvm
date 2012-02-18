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
 */

#include "engines/ags/scripting/scripting.h"

namespace AGS {

RuntimeValue Script_UnimplementedStub(AGSEngine *vm, const Common::Array<RuntimeValue> &params) {
	error("call to unimplemented system scripting function");
}

void GlobalScriptState::addImport(const Common::String &name, const ScriptImport &import, bool forceReplace) {
	if (_imports.contains(name)) {
		if (forceReplace) {
			ScriptImport oldImport = _imports[name];
			if (oldImport._type == sitSystemObject)
				delete oldImport._object;
		}  else {
			warning("duplicate exported '%s'", name.c_str());
			return;
		}
	}

	_imports[name] = import;
}

void GlobalScriptState::addSystemFunctionImport(const Common::String &name, ScriptAPIFunction *function) {
	ScriptImport import;

	import._type = sitSystemFunction;
	import._function = function;

	addImport(name, import);
}

void GlobalScriptState::addSystemObjectImport(const Common::String &name, ScriptObject *object, bool forceReplace) {
	ScriptImport import;

	import._type = sitSystemObject;
	import._object = object;

	addImport(name, import, forceReplace);
}

extern void addAudioSystemScripting(AGSEngine *vm);
extern void addCharacterSystemScripting(AGSEngine *vm);
extern void addGameSystemScripting(AGSEngine *vm);
extern void addGraphicsSystemScripting(AGSEngine *vm);
extern void addGUISystemScripting(AGSEngine *vm);
extern void addInputSystemScripting(AGSEngine *vm);
extern void addInventorySystemScripting(AGSEngine *vm);
extern void addMiscSystemScripting(AGSEngine *vm);
extern void addRoomSystemScripting(AGSEngine *vm);
extern void addStringsSystemScripting(AGSEngine *vm);

void addSystemScripting(AGSEngine *vm) {
	GlobalScriptState *state = vm->getScriptState();

	addAudioSystemScripting(vm);
	addCharacterSystemScripting(vm);
	addGameSystemScripting(vm);
	addGraphicsSystemScripting(vm);
	addGUISystemScripting(vm);
	addInputSystemScripting(vm);
	addInventorySystemScripting(vm);
	addMiscSystemScripting(vm);
	addRoomSystemScripting(vm);
	addStringsSystemScripting(vm);
}

} // End of namespace AGS
