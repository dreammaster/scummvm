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

// Parser: import static int FindWordID(const string wordToFind)
// Returns the parser dictionary word ID for the specified word
RuntimeValue Script_Parser_FindWordID(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	ScriptString *wordToFind = (ScriptString *)params[0]._object;
	UNUSED(wordToFind);

	// FIXME
	error("Parser::FindWordID unimplemented");

	return RuntimeValue();
}

// Parser: import static void ParseText(const string text)
// Stores the supplied user text for later use with Said
RuntimeValue Script_Parser_ParseText(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	ScriptString *text = (ScriptString *)params[0]._object;
	UNUSED(text);

	// FIXME
	error("Parser::ParseText unimplemented");

	return RuntimeValue();
}

// Parser: import static bool Said(const string text)
// Checks whether the player's input matched this text.
RuntimeValue Script_Parser_Said(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	ScriptString *text = (ScriptString *)params[0]._object;
	UNUSED(text);

	// FIXME
	error("Parser::Said unimplemented");

	return RuntimeValue();
}

// Parser: import static String SaidUnknownWord()
// Gets any word that the player typed that was not in the game dictionary.
RuntimeValue Script_Parser_SaidUnknownWord(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("Parser::SaidUnknownWord unimplemented");

	return RuntimeValue();
}

// import void ParseText (const string)
// Undocumented.
RuntimeValue Script_ParseText(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	ScriptString *string = (ScriptString *)params[0]._object;
	UNUSED(string);

	// FIXME
	error("ParseText unimplemented");

	return RuntimeValue();
}

// import int SaidUnknownWord (string buffer)
// Undocumented.
RuntimeValue Script_SaidUnknownWord(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	ScriptString *buffer = (ScriptString *)params[0]._object;
	UNUSED(buffer);

	// FIXME
	error("SaidUnknownWord unimplemented");

	return RuntimeValue();
}

// import int Said (const string text)
// Undocumented.
RuntimeValue Script_Said(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	ScriptString *text = (ScriptString *)params[0]._object;
	UNUSED(text);

	// FIXME
	error("Said unimplemented");

	return RuntimeValue();
}

static const ScriptSystemFunctionInfo ourFunctionList[] = {
	{ "Parser::FindWordID^1", (ScriptAPIFunction *)&Script_Parser_FindWordID, "s", sotNone },
	{ "Parser::ParseText^1", (ScriptAPIFunction *)&Script_Parser_ParseText, "s", sotNone },
	{ "Parser::Said^1", (ScriptAPIFunction *)&Script_Parser_Said, "s", sotNone },
	{ "Parser::SaidUnknownWord^0", (ScriptAPIFunction *)&Script_Parser_SaidUnknownWord, "", sotNone },
	{ "ParseText", (ScriptAPIFunction *)&Script_ParseText, "s", sotNone },
	{ "SaidUnknownWord", (ScriptAPIFunction *)&Script_SaidUnknownWord, "s", sotNone },
	{ "Said", (ScriptAPIFunction *)&Script_Said, "s", sotNone },
};

void addParserSystemScripting(AGSEngine *vm) {
	GlobalScriptState *state = vm->getScriptState();

	state->addSystemFunctionImportList(ourFunctionList, ARRAYSIZE(ourFunctionList));
}

} // End of namespace AGS
