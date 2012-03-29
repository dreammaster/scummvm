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

#ifndef AGS_SCRIPTING_H
#define AGS_SCRIPTING_H

#include "engines/ags/ags.h"
#include "engines/ags/script.h"

#define UNUSED(x) (void)(x)

namespace AGS {

void addSystemScripting(AGSEngine *vm);

RuntimeValue Script_UnimplementedStub(AGSEngine *vm, ScriptObject *self, const Common::Array<RuntimeValue> &params);

class AudioChannel;
class AudioClip;
class DateTime;
class Dialog;
class DialogOptionsRenderingInfo;
class DrawingSurface;
class DynamicSprite;
class ScriptFile;
class Overlay;
class ViewFrame;

typedef RuntimeValue ScriptAPIFunction(AGSEngine *vm, ScriptObject *self, const Common::Array<RuntimeValue> &params);

struct ScriptSystemFunctionInfo {
	const char *name;
	ScriptAPIFunction *function;
	const char *signature;
	ScriptObjectType objectType;
};

class GlobalScriptState {
public:
	Common::HashMap<Common::String, ScriptImport, Common::CaseSensitiveString_Hash, Common::CaseSensitiveString_EqualTo> _imports;

	void addImport(const Common::String &name, const ScriptImport &import, bool forceReplace = false);
	void removeImport(const Common::String &name);
	void addSystemFunctionImport(const ScriptSystemFunctionInfo *function);
	void addSystemObjectImport(const Common::String &name, ScriptObject *object);
	void addSystemFunctionImportList(const ScriptSystemFunctionInfo *list, uint32 count);
};

} // End of namespace AGS

#endif // AGS_SCRIPTING_H
