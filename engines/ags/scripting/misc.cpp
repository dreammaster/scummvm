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

void addMiscSystemScripting(GlobalScriptState *state) {
	// File functions
	state->addSystemFunctionImport("File::Delete^1", &Script_UnimplementedStub);
	state->addSystemFunctionImport("File::Exists^1", &Script_UnimplementedStub);
	state->addSystemFunctionImport("File::Open^2", &Script_UnimplementedStub);
	state->addSystemFunctionImport("File::Close^0", &Script_UnimplementedStub);
	state->addSystemFunctionImport("File::ReadInt^0", &Script_UnimplementedStub);
	state->addSystemFunctionImport("File::ReadRawChar^0", &Script_UnimplementedStub);
	state->addSystemFunctionImport("File::ReadRawInt^0", &Script_UnimplementedStub);
	state->addSystemFunctionImport("File::ReadRawLine^1", &Script_UnimplementedStub);
	state->addSystemFunctionImport("File::ReadRawLineBack^0", &Script_UnimplementedStub);
	state->addSystemFunctionImport("File::ReadString^1", &Script_UnimplementedStub);
	state->addSystemFunctionImport("File::ReadStringBack^0", &Script_UnimplementedStub);
	state->addSystemFunctionImport("File::WriteInt^1", &Script_UnimplementedStub);
	state->addSystemFunctionImport("File::WriteRawChar^1", &Script_UnimplementedStub);
	state->addSystemFunctionImport("File::WriteRawLine^1", &Script_UnimplementedStub);
	state->addSystemFunctionImport("File::WriteString^1", &Script_UnimplementedStub);
	state->addSystemFunctionImport("File::get_EOF", &Script_UnimplementedStub);
	state->addSystemFunctionImport("File::get_Error", &Script_UnimplementedStub);

	// timer functions
	state->addSystemFunctionImport("IsTimerExpired", &Script_UnimplementedStub);
	state->addSystemFunctionImport("SetTimer", &Script_UnimplementedStub);

	// misc functions
	state->addSystemFunctionImport("FloatToInt", &Script_UnimplementedStub);
	state->addSystemFunctionImport("IntToFloat", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Random", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Wait", &Script_UnimplementedStub);
}

} // End of namespace AGS
