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

void GlobalScriptState::addImport(const Common::String &name, const ScriptImport &import) {
	// FIXME: allow overriding of script imports by system
	if (_imports.contains(name)) {
		warning("duplicate exported '%s'", name.c_str());
		return;
	}

	_imports[name] = import;
}

void GlobalScriptState::addSystemFunctionImport(const Common::String &name, ScriptAPIFunction *function) {
	ScriptImport import;

	import._type = sitSystemFunction;
	import._function = function;

	addImport(name, import);
}

void GlobalScriptState::addSystemObjectImport(const Common::String &name, ScriptObject *object) {
	ScriptImport import;

	import._type = sitSystemObject;
	import._object = object;

	addImport(name, import);
}

extern void addAudioSystemScripting(GlobalScriptState *state);
extern void addCharacterSystemScripting(GlobalScriptState *state);
extern void addGameSystemScripting(GlobalScriptState *state);
extern void addGraphicsSystemScripting(GlobalScriptState *state);
extern void addGUISystemScripting(GlobalScriptState *state);
extern void addInputSystemScripting(GlobalScriptState *state);
extern void addInventorySystemScripting(GlobalScriptState *state);
extern void addMiscSystemScripting(GlobalScriptState *state);
extern void addRoomSystemScripting(GlobalScriptState *state);
extern void addStringsSystemScripting(GlobalScriptState *state);

void addSystemScripting(GlobalScriptState *state) {
	addAudioSystemScripting(state);
	addCharacterSystemScripting(state);
	addGameSystemScripting(state);
	addGraphicsSystemScripting(state);
	addGUISystemScripting(state);
	addInputSystemScripting(state);
	addInventorySystemScripting(state);
	addMiscSystemScripting(state);
	addRoomSystemScripting(state);
	addStringsSystemScripting(state);
}

} // End of namespace AGS
