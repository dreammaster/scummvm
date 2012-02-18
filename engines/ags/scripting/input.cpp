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

class ScriptMouseObject : public ScriptObject {
public:
};

void addInputSystemScripting(GlobalScriptState *state) {
	// static Mouse functions
	state->addSystemFunctionImport("Mouse::ChangeModeGraphic^2", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Mouse::ChangeModeHotspot^3", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Mouse::ChangeModeView^2", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Mouse::DisableMode^1", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Mouse::EnableMode^1", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Mouse::GetModeGraphic^1", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Mouse::IsButtonDown^1", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Mouse::SaveCursorUntilItLeaves^0", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Mouse::SelectNextMode^0", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Mouse::SetBounds^4", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Mouse::SetPosition^2", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Mouse::Update^0", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Mouse::UseDefaultGraphic^0", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Mouse::UseModeGraphic^1", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Mouse::get_Mode", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Mouse::set_Mode", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Mouse::get_Visible", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Mouse::set_Visible", &Script_UnimplementedStub);

	// input functions
	state->addSystemFunctionImport("ChangeCursorGraphic", &Script_UnimplementedStub);
	state->addSystemFunctionImport("ChangeCursorHotspot", &Script_UnimplementedStub);
	state->addSystemFunctionImport("DisableCursorMode", &Script_UnimplementedStub);
	state->addSystemFunctionImport("EnableCursorMode", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GetCursorMode", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GetLocationName", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GetLocationType", &Script_UnimplementedStub);
	state->addSystemFunctionImport("HideMouseCursor", &Script_UnimplementedStub);
	state->addSystemFunctionImport("IsButtonDown", &Script_UnimplementedStub);
	state->addSystemFunctionImport("IsKeyPressed", &Script_UnimplementedStub);
	state->addSystemFunctionImport("ProcessClick", &Script_UnimplementedStub);
	state->addSystemFunctionImport("RefreshMouse", &Script_UnimplementedStub);
	state->addSystemFunctionImport("SaveCursorForLocationChange", &Script_UnimplementedStub);
	state->addSystemFunctionImport("SetCursorMode", &Script_UnimplementedStub);
	state->addSystemFunctionImport("SetDefaultCursor", &Script_UnimplementedStub);
	state->addSystemFunctionImport("SetMouseBounds", &Script_UnimplementedStub);
	state->addSystemFunctionImport("SetMouseCursor", &Script_UnimplementedStub);
	state->addSystemFunctionImport("SetMousePosition", &Script_UnimplementedStub);
	state->addSystemFunctionImport("SetNextCursorMode", &Script_UnimplementedStub);
	state->addSystemFunctionImport("ShowMouseCursor", &Script_UnimplementedStub);
	state->addSystemFunctionImport("WaitKey", &Script_UnimplementedStub);
	state->addSystemFunctionImport("WaitMouseKey", &Script_UnimplementedStub);

	state->addSystemObjectImport("mouse", new ScriptMouseObject);
}

} // End of namespace AGS
