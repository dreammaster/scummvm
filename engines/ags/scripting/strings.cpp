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

void addStringsSystemScripting(GlobalScriptState *state) {
	// String functions
	state->addSystemFunctionImport("String::IsNullOrEmpty^1", &Script_UnimplementedStub);
	state->addSystemFunctionImport("String::Append^1", &Script_UnimplementedStub);
	state->addSystemFunctionImport("String::AppendChar^1", &Script_UnimplementedStub);
	state->addSystemFunctionImport("String::CompareTo^2", &Script_UnimplementedStub);
	state->addSystemFunctionImport("String::Contains^1", &Script_UnimplementedStub);
	state->addSystemFunctionImport("String::Copy^0", &Script_UnimplementedStub);
	state->addSystemFunctionImport("String::EndsWith^2", &Script_UnimplementedStub);
	state->addSystemFunctionImport("String::Format^101", &Script_UnimplementedStub);
	state->addSystemFunctionImport("String::IndexOf^1", &Script_UnimplementedStub);
	state->addSystemFunctionImport("String::LowerCase^0", &Script_UnimplementedStub);
	state->addSystemFunctionImport("String::Replace^3", &Script_UnimplementedStub);
	state->addSystemFunctionImport("String::ReplaceCharAt^2", &Script_UnimplementedStub);
	state->addSystemFunctionImport("String::StartsWith^2", &Script_UnimplementedStub);
	state->addSystemFunctionImport("String::Substring^2", &Script_UnimplementedStub);
	state->addSystemFunctionImport("String::Truncate^1", &Script_UnimplementedStub);
	state->addSystemFunctionImport("String::UpperCase^0", &Script_UnimplementedStub);
	state->addSystemFunctionImport("String::get_AsFloat", &Script_UnimplementedStub);
	state->addSystemFunctionImport("String::get_AsInt", &Script_UnimplementedStub);
	state->addSystemFunctionImport("String::geti_Chars", &Script_UnimplementedStub);
	state->addSystemFunctionImport("String::get_Length", &Script_UnimplementedStub);

	// static string functions
	state->addSystemFunctionImport("StrCat", &Script_UnimplementedStub);
	state->addSystemFunctionImport("StrCaseComp", &Script_UnimplementedStub);
	state->addSystemFunctionImport("StrComp", &Script_UnimplementedStub);
	state->addSystemFunctionImport("StrContains", &Script_UnimplementedStub);
	state->addSystemFunctionImport("StrCopy", &Script_UnimplementedStub);
	state->addSystemFunctionImport("StrFormat", &Script_UnimplementedStub);
	state->addSystemFunctionImport("StrGetCharAt", &Script_UnimplementedStub);
	state->addSystemFunctionImport("StringToInt", &Script_UnimplementedStub);
	state->addSystemFunctionImport("StrLen", &Script_UnimplementedStub);
	state->addSystemFunctionImport("StrSetCharAt", &Script_UnimplementedStub);
	state->addSystemFunctionImport("StrToLowerCase", &Script_UnimplementedStub);
	state->addSystemFunctionImport("StrToUpperCase", &Script_UnimplementedStub);
}

} // End of namespace AGS
