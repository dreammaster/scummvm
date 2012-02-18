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

#ifndef AGS_SCRIPTING_H
#define AGS_SCRIPTING_H

#include "engines/ags/script.h"

namespace AGS {

void addSystemScripting(class GlobalScriptState *state);

RuntimeValue Script_UnimplementedStub(AGSEngine *vm, const Common::Array<RuntimeValue> &params);

class GlobalScriptState {
public:
	Common::HashMap<Common::String, ScriptImport, Common::CaseSensitiveString_Hash, Common::CaseSensitiveString_EqualTo> _imports;

	void addImport(const Common::String &name, const ScriptImport &import);
	void addSystemFunctionImport(const Common::String &name, ScriptAPIFunction *function);
	void addSystemObjectImport(const Common::String &name, ScriptObject *object);
};

} // End of namespace AGS

#endif // AGS_SCRIPTING_H
