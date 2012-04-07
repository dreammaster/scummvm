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

#include <string.h>
#include "engines/ags/scripting/scripting.h"

namespace AGS {

// String: import static String Format(const string format, ...)
// Creates a formatted string using the supplied parameters.
RuntimeValue Script_String_Format(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	ScriptString *format = (ScriptString *)params[0]._object;

	Common::Array<RuntimeValue> values = params;
	values.remove_at(0);
	Common::String string = vm->formatString(format->getString(), values);

	RuntimeValue ret = new ScriptMutableString(string);
	ret._object->DecRef();
	return ret;
}

// String: import static bool IsNullOrEmpty(String stringToCheck)
// Checks whether the supplied string is null or empty.
RuntimeValue Script_String_IsNullOrEmpty(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	if (params[0]._value == 0)
		return 1;

	ScriptString *stringToCheck = (ScriptString *)params[0]._object;
	return stringToCheck->getString().empty();
}

// String: import String Append(const string appendText)
// Returns a new string with the specified string appended to this string.
RuntimeValue Script_String_Append(AGSEngine *vm, ScriptString *self, const Common::Array<RuntimeValue> &params) {
	ScriptString *appendText = (ScriptString *)params[0]._object;

	RuntimeValue ret = new ScriptMutableString(self->getString() + appendText->getString());
	ret._object->DecRef();
	return ret;
}

// String: import String AppendChar(char extraChar)
// Returns a new string that has the extra character appended.
RuntimeValue Script_String_AppendChar(AGSEngine *vm, ScriptString *self, const Common::Array<RuntimeValue> &params) {
	char extraChar = (char)params[0]._value;

	RuntimeValue ret = new ScriptMutableString(self->getString() + extraChar);
	ret._object->DecRef();
	return ret;
}

// String: import int CompareTo(const string otherString, bool caseSensitive = false)
// Compares this string to the other string.
RuntimeValue Script_String_CompareTo(AGSEngine *vm, ScriptString *self, const Common::Array<RuntimeValue> &params) {
	ScriptString *otherString = (ScriptString *)params[0]._object;
	uint32 caseSensitive = params[1]._value;

	if (caseSensitive)
		return self->getString().compareToIgnoreCase(otherString->getString());
	else
		return self->getString().compareTo(otherString->getString());
}

// String: import int Contains(const string needle)
RuntimeValue Script_String_Contains(AGSEngine *vm, ScriptString *self, const Common::Array<RuntimeValue> &params) {
	ScriptString *needle = (ScriptString *)params[0]._object;

	Common::String needleStr = needle->getString();
	Common::String selfStr = self->getString();
	needleStr.toLowercase();
	selfStr.toLowercase();

	const char *offset = strstr(selfStr.c_str(), needleStr.c_str());
	if (offset == NULL)
		return -1;

	return (uint)(offset - selfStr.c_str());
}

// String: import String Copy()
// Creates a copy of the string.
RuntimeValue Script_String_Copy(AGSEngine *vm, ScriptString *self, const Common::Array<RuntimeValue> &params) {
	RuntimeValue ret = new ScriptMutableString(self->getString());
	ret._object->DecRef();
	return ret;
}

// String: import bool EndsWith(const string endsWithText, bool caseSensitive = false)
// Checks whether this string ends with the specified text.
RuntimeValue Script_String_EndsWith(AGSEngine *vm, ScriptString *self, const Common::Array<RuntimeValue> &params) {
	ScriptString *endsWithText = (ScriptString *)params[0]._object;
	UNUSED(endsWithText);
	uint32 caseSensitive = params[1]._value;
	UNUSED(caseSensitive);

	// FIXME
	error("String::EndsWith unimplemented");

	return RuntimeValue();
}

// String: import int IndexOf(const string needle)
// Returns the index of the first occurrence of the needle in this string.
RuntimeValue Script_String_IndexOf(AGSEngine *vm, ScriptString *self, const Common::Array<RuntimeValue> &params) {
	ScriptString *needle = (ScriptString *)params[0]._object;
	UNUSED(needle);

	// FIXME
	error("String::IndexOf unimplemented");

	return RuntimeValue();
}

// String: import String LowerCase()
// Returns a lower-cased version of this string.
RuntimeValue Script_String_LowerCase(AGSEngine *vm, ScriptString *self, const Common::Array<RuntimeValue> &params) {
	Common::String string = self->getString();
	string.toLowercase();
	RuntimeValue ret = new ScriptMutableString(string);
	ret._object->DecRef();
	return ret;
}

// String: import String Replace(const string lookForText, const string replaceWithText, bool caseSensitive = false)
// Returns a copy of this string with all occurrences of LookForText replaced with ReplaceWithText
RuntimeValue Script_String_Replace(AGSEngine *vm, ScriptString *self, const Common::Array<RuntimeValue> &params) {
	ScriptString *lookForText = (ScriptString *)params[0]._object;
	UNUSED(lookForText);
	ScriptString *replaceWithText = (ScriptString *)params[1]._object;
	UNUSED(replaceWithText);
	uint32 caseSensitive = params[2]._value;
	UNUSED(caseSensitive);

	// FIXME
	error("String::Replace unimplemented");

	return RuntimeValue();
}

// String: import String ReplaceCharAt(int index, char newChar)
// Returns a new string, with the specified character changed.
RuntimeValue Script_String_ReplaceCharAt(AGSEngine *vm, ScriptString *self, const Common::Array<RuntimeValue> &params) {
	int index = params[0]._signedValue;
	UNUSED(index);
	char newChar = (char)params[1]._value;
	UNUSED(newChar);

	// FIXME
	error("String::ReplaceCharAt unimplemented");

	return RuntimeValue();
}

// String: import bool StartsWith(const string startsWithText, bool caseSensitive = false)
// Checks whether this string starts with the specified text.
RuntimeValue Script_String_StartsWith(AGSEngine *vm, ScriptString *self, const Common::Array<RuntimeValue> &params) {
	ScriptString *startsWithText = (ScriptString *)params[0]._object;
	UNUSED(startsWithText);
	uint32 caseSensitive = params[1]._value;
	UNUSED(caseSensitive);

	// FIXME
	error("String::StartsWith unimplemented");

	return RuntimeValue();
}

// String: import String Substring(int index, int length)
// Returns a portion of the string.
RuntimeValue Script_String_Substring(AGSEngine *vm, ScriptString *self, const Common::Array<RuntimeValue> &params) {
	uint index = params[0]._value;
	uint length = params[1]._value;

	Common::String string = self->getString();
	if (index > string.size())
		error("String::Substring: invalid index (%d, on string of length %d)",
			index, string.size());
	if (index + length > string.size())
		error("String::Substring: invalid length (%d, from index %d on string of length %d)",
			length, index, string.size());

	Common::String result(string.c_str() + index, length);
	RuntimeValue ret = new ScriptMutableString(result);
	ret._object->DecRef();
	return ret;
}

// String: import String Truncate(int length)
// Truncates the string down to the specified length by removing characters from the end.
RuntimeValue Script_String_Truncate(AGSEngine *vm, ScriptString *self, const Common::Array<RuntimeValue> &params) {
	int length = params[0]._signedValue;
	UNUSED(length);

	// FIXME
	error("String::Truncate unimplemented");

	return RuntimeValue();
}

// String: import String UpperCase()
// Returns an upper-cased version of this string.
RuntimeValue Script_String_UpperCase(AGSEngine *vm, ScriptString *self, const Common::Array<RuntimeValue> &params) {
	Common::String string = self->getString();
	string.toUppercase();
	RuntimeValue ret = new ScriptMutableString(string);
	ret._object->DecRef();
	return ret;
}

// String: readonly import attribute float AsFloat
// Converts the string to a float.
RuntimeValue Script_String_get_AsFloat(AGSEngine *vm, ScriptString *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("String::get_AsFloat unimplemented");

	return RuntimeValue();
}

// String: readonly import attribute int AsInt
// Converts the string to an integer.
RuntimeValue Script_String_get_AsInt(AGSEngine *vm, ScriptString *self, const Common::Array<RuntimeValue> &params) {
	return atoi(self->getString().c_str());
}

// String: readonly import attribute char Chars[]
// Accesses individual characters of the string.
RuntimeValue Script_String_geti_Chars(AGSEngine *vm, ScriptString *self, const Common::Array<RuntimeValue> &params) {
	uint index = params[0]._value;

	Common::String string = self->getString();
	if (index >= string.size())
		error("String::geti_Chars: char %d is too high (only have %d chars)", index, string.size());

	return (uint)(byte)string[index];
}

// String: readonly import attribute int Length
// Returns the length of the string.
RuntimeValue Script_String_get_Length(AGSEngine *vm, ScriptString *self, const Common::Array<RuntimeValue> &params) {
	return self->getString().size();
}

// import void StrCat(string main, const string newbit)
// String function.
RuntimeValue Script_StrCat(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	ScriptString *main = (ScriptString *)params[0]._object;
	ScriptString *newbit = (ScriptString *)params[1]._object;

	main->setString(main->getString() + newbit->getString());

	return RuntimeValue();
}

// import int StrCaseComp(const string str1, const string str2)
// String function.
RuntimeValue Script_StrCaseComp(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	ScriptString *str1 = (ScriptString *)params[0]._object;
	ScriptString *str2 = (ScriptString *)params[1]._object;

	return str1->getString().compareToIgnoreCase(str2->getString());
}

// import int StrComp(const string str1, const string str2)
// String function.
RuntimeValue Script_StrComp(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	ScriptString *str1 = (ScriptString *)params[0]._object;
	ScriptString *str2 = (ScriptString *)params[1]._object;

	return str1->getString().compareTo(str2->getString());
}

// import void StrCopy(string dest, const string source)
// String function.
RuntimeValue Script_StrCopy(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	ScriptString *dest = (ScriptString *)params[0]._object;
	ScriptString *source = (ScriptString *)params[1]._object;

	dest->setString(source->getString());

	return RuntimeValue();
}

// import void StrFormat(string dest, const string format, ...)
// String function.
RuntimeValue Script_StrFormat(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	ScriptString *dest = (ScriptString *)params[0]._object;
	ScriptString *format = (ScriptString *)params[1]._object;

	Common::Array<RuntimeValue> values = params;
	values.remove_at(0);
	values.remove_at(0);
	Common::String string = vm->formatString(format->getString(), values);
	dest->setString(string);

	return RuntimeValue();
}

// import int StrLen(const string)
// String function.
RuntimeValue Script_StrLen(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	ScriptString *string = (ScriptString *)params[0]._object;

	return string->getString().size();
}

// import int StrGetCharAt (const string, int position)
// String function.
RuntimeValue Script_StrGetCharAt(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	ScriptString *string = (ScriptString *)params[0]._object;
	uint position = params[1]._value;

	const Common::String realString = string->getString();
	if (position >= realString.size())
		return 0;

	return (uint)(byte)realString[position];
}

// import void StrSetCharAt (string, int position, int newChar)
// String function.
RuntimeValue Script_StrSetCharAt(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	ScriptString *string = (ScriptString *)params[0]._object;
	uint position = params[1]._value;
	uint newChar = params[2]._value;

	Common::String realString = string->getString();
	if (position > realString.size())
		error("StrSetCharAt: position %d is beyond string length %d", position, realString.size());
	else if (position == realString.size())
		realString = realString + (char)newChar;
	else
		realString.setChar((char)newChar, position);
	string->setString(realString);

	return RuntimeValue();
}

// import void StrToLowerCase (string)
// String function.
RuntimeValue Script_StrToLowerCase(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	ScriptString *string = (ScriptString *)params[0]._object;

	Common::String realString = string->getString();
	realString.toLowercase();
	string->setString(realString);

	return RuntimeValue();
}

// import void StrToUpperCase (string)
// String function.
RuntimeValue Script_StrToUpperCase(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	ScriptString *string = (ScriptString *)params[0]._object;

	Common::String realString = string->getString();
	realString.toUppercase();
	string->setString(realString);

	return RuntimeValue();
}

// import int StrContains (const string haystack, const string needle)
// String function.
RuntimeValue Script_StrContains(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	ScriptString *haystack = (ScriptString *)params[0]._object;
	ScriptString *needle = (ScriptString *)params[1]._object;

	Common::String haystackString = haystack->getString();
	Common::String needleString = needle->getString();
	haystackString.toLowercase();
	needleString.toLowercase();

	const char *haystackBuf = haystackString.c_str();
	const char *offset = strstr(haystackBuf, needleString.c_str());
	if (offset == NULL)
		return RuntimeValue(-1);
	else
		return (uint)(offset - haystackBuf);
}

// import int StringToInt(const string)
// Undocumented.
RuntimeValue Script_StringToInt(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	ScriptString *string = (ScriptString *)params[0]._object;

	return atoi(string->getString().c_str());
}

static const ScriptSystemFunctionInfo ourFunctionList[] = {
	{ "String::Format^101", (ScriptAPIFunction *)&Script_String_Format, "s.", sotNone },
	{ "String::IsNullOrEmpty^1", (ScriptAPIFunction *)&Script_String_IsNullOrEmpty, "t", sotNone },
	{ "String::Append^1", (ScriptAPIFunction *)&Script_String_Append, "s", sotString },
	{ "String::AppendChar^1", (ScriptAPIFunction *)&Script_String_AppendChar, "c", sotString },
	{ "String::CompareTo^2", (ScriptAPIFunction *)&Script_String_CompareTo, "si", sotString },
	{ "String::Contains^1", (ScriptAPIFunction *)&Script_String_Contains, "s", sotString },
	{ "String::Copy^0", (ScriptAPIFunction *)&Script_String_Copy, "", sotString },
	{ "String::EndsWith^2", (ScriptAPIFunction *)&Script_String_EndsWith, "si", sotString },
	{ "String::IndexOf^1", (ScriptAPIFunction *)&Script_String_IndexOf, "s", sotString },
	{ "String::LowerCase^0", (ScriptAPIFunction *)&Script_String_LowerCase, "", sotString },
	{ "String::Replace^3", (ScriptAPIFunction *)&Script_String_Replace, "ssi", sotString },
	{ "String::ReplaceCharAt^2", (ScriptAPIFunction *)&Script_String_ReplaceCharAt, "ic", sotString },
	{ "String::StartsWith^2", (ScriptAPIFunction *)&Script_String_StartsWith, "si", sotString },
	{ "String::Substring^2", (ScriptAPIFunction *)&Script_String_Substring, "ii", sotString },
	{ "String::Truncate^1", (ScriptAPIFunction *)&Script_String_Truncate, "i", sotString },
	{ "String::UpperCase^0", (ScriptAPIFunction *)&Script_String_UpperCase, "", sotString },
	{ "String::get_AsFloat", (ScriptAPIFunction *)&Script_String_get_AsFloat, "", sotString },
	{ "String::get_AsInt", (ScriptAPIFunction *)&Script_String_get_AsInt, "", sotString },
	{ "String::geti_Chars", (ScriptAPIFunction *)&Script_String_geti_Chars, "i", sotString },
	{ "String::get_Length", (ScriptAPIFunction *)&Script_String_get_Length, "", sotString },
	{ "StrCat", (ScriptAPIFunction *)&Script_StrCat, "ss", sotNone },
	{ "StrCaseComp", (ScriptAPIFunction *)&Script_StrCaseComp, "ss", sotNone },
	{ "StrComp", (ScriptAPIFunction *)&Script_StrComp, "ss", sotNone },
	{ "StrCopy", (ScriptAPIFunction *)&Script_StrCopy, "ss", sotNone },
	{ "StrFormat", (ScriptAPIFunction *)&Script_StrFormat, "ss.", sotNone },
	{ "StrLen", (ScriptAPIFunction *)&Script_StrLen, "s", sotNone },
	{ "StrGetCharAt", (ScriptAPIFunction *)&Script_StrGetCharAt, "si", sotNone },
	{ "StrSetCharAt", (ScriptAPIFunction *)&Script_StrSetCharAt, "sii", sotNone },
	{ "StrToLowerCase", (ScriptAPIFunction *)&Script_StrToLowerCase, "s", sotNone },
	{ "StrToUpperCase", (ScriptAPIFunction *)&Script_StrToUpperCase, "s", sotNone },
	{ "StrContains", (ScriptAPIFunction *)&Script_StrContains, "ss", sotNone },
	{ "StringToInt", (ScriptAPIFunction *)&Script_StringToInt, "s", sotNone },
};

void addStringSystemScripting(AGSEngine *vm) {
	GlobalScriptState *state = vm->getScriptState();

	state->addSystemFunctionImportList(ourFunctionList, ARRAYSIZE(ourFunctionList));
}

} // End of namespace AGS
