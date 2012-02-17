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

void addGameSystemScripting(GlobalScriptState *state) {
	// text parser functions
	state->addSystemFunctionImport("Parser::FindWordID^1", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Parser::ParseText^1", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Parser::SaidUnknownWord^0", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Parser::Said^1", &Script_UnimplementedStub);

	// text parser static functions
	state->addSystemFunctionImport("ParseText", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Said", &Script_UnimplementedStub);
	state->addSystemFunctionImport("SaidUnknownWord", &Script_UnimplementedStub);

	// misc game-related functions
	state->addSystemFunctionImport("AbortGame", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Debug", &Script_UnimplementedStub);
	state->addSystemFunctionImport("DeleteSaveSlot", &Script_UnimplementedStub);
	state->addSystemFunctionImport("EndCutscene", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GetGameOption", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GetGameParameter", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GetGameSpeed", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GetGlobalInt", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GetGlobalString", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GetGraphicalVariable", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GiveScore", &Script_UnimplementedStub);
	state->addSystemFunctionImport("IsGamePaused", &Script_UnimplementedStub);
	state->addSystemFunctionImport("PauseGame", &Script_UnimplementedStub);
	state->addSystemFunctionImport("QuitGame", &Script_UnimplementedStub);
	state->addSystemFunctionImport("RestartGame", &Script_UnimplementedStub);
	state->addSystemFunctionImport("RestoreGameDialog", &Script_UnimplementedStub);
	state->addSystemFunctionImport("RestoreGameSlot", &Script_UnimplementedStub);
	state->addSystemFunctionImport("SaveGameDialog", &Script_UnimplementedStub);
	state->addSystemFunctionImport("SaveGameSlot", &Script_UnimplementedStub);
	state->addSystemFunctionImport("SetGameOption", &Script_UnimplementedStub);
	state->addSystemFunctionImport("SetGameSpeed", &Script_UnimplementedStub);
	state->addSystemFunctionImport("SetGlobalInt", &Script_UnimplementedStub);
	state->addSystemFunctionImport("SetGlobalString", &Script_UnimplementedStub);
	state->addSystemFunctionImport("SetGraphicalVariable", &Script_UnimplementedStub);
	state->addSystemFunctionImport("SetRestartPoint", &Script_UnimplementedStub);
	state->addSystemFunctionImport("StartCutscene", &Script_UnimplementedStub);
	state->addSystemFunctionImport("UnPauseGame", &Script_UnimplementedStub);
}

} // End of namespace AGS
