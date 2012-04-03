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

#include "common/savefile.h"
#include "engines/ags/scripting/scripting.h"

namespace AGS {

class ScriptFile : public ScriptObject {
public:
	ScriptFile() : _inFile(NULL), _mode(0) { }
	~ScriptFile() { delete _inFile; }

	virtual bool isOfType(ScriptObjectType objectType) { return (objectType == sotFile); }
	const char *getObjectTypeName() { return "ScriptFile"; }

	int _mode; // 1 - read, 2 - write, 3 - append
	union {
		Common::SeekableReadStream *_inFile;
		Common::WriteStream *_outFile;
	};
};

// import int FileOpen(const string filename, const string mode)
// Obsolete file I/O function.
RuntimeValue Script_FileOpen(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	ScriptString *filename = (ScriptString *)params[0]._object;
	UNUSED(filename);
	ScriptString *mode = (ScriptString *)params[1]._object;
	UNUSED(mode);

	// FIXME
	error("FileOpen unimplemented");

	return RuntimeValue();
}

// import void FileWrite(int fileHandle, const string text)
// Obsolete file I/O function.
RuntimeValue Script_FileWrite(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int fileHandle = params[0]._signedValue;
	UNUSED(fileHandle);
	ScriptString *text = (ScriptString *)params[1]._object;
	UNUSED(text);

	// FIXME
	error("FileWrite unimplemented");

	return RuntimeValue();
}

// import void FileWriteRawLine(int fileHandle, const string text)
// Obsolete file I/O function.
RuntimeValue Script_FileWriteRawLine(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int fileHandle = params[0]._signedValue;
	UNUSED(fileHandle);
	ScriptString *text = (ScriptString *)params[1]._object;
	UNUSED(text);

	// FIXME
	error("FileWriteRawLine unimplemented");

	return RuntimeValue();
}

// import void FileRead(int fileHandle, string buffer)
// Obsolete file I/O function.
RuntimeValue Script_FileRead(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int fileHandle = params[0]._signedValue;
	UNUSED(fileHandle);
	ScriptString *buffer = (ScriptString *)params[1]._object;
	UNUSED(buffer);

	// FIXME
	error("FileRead unimplemented");

	return RuntimeValue();
}

// import void FileClose(int fileHandle)
// Obsolete file I/O function.
RuntimeValue Script_FileClose(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int fileHandle = params[0]._signedValue;
	UNUSED(fileHandle);

	// FIXME
	error("FileClose unimplemented");

	return RuntimeValue();
}

// import void FileWriteInt(int fileHandle, int value)
// Obsolete file I/O function.
RuntimeValue Script_FileWriteInt(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int fileHandle = params[0]._signedValue;
	UNUSED(fileHandle);
	int value = params[1]._signedValue;
	UNUSED(value);

	// FIXME
	error("FileWriteInt unimplemented");

	return RuntimeValue();
}

// import int FileReadInt(int fileHandle)
// Obsolete file I/O function.
RuntimeValue Script_FileReadInt(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int fileHandle = params[0]._signedValue;
	UNUSED(fileHandle);

	// FIXME
	error("FileReadInt unimplemented");

	return RuntimeValue();
}

// import char FileReadRawChar(int fileHandle)
// Obsolete file I/O function.
RuntimeValue Script_FileReadRawChar(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int fileHandle = params[0]._signedValue;
	UNUSED(fileHandle);

	// FIXME
	error("FileReadRawChar unimplemented");

	return RuntimeValue();
}

// import void FileWriteRawChar(int fileHandle, int value)
// Obsolete file I/O function.
RuntimeValue Script_FileWriteRawChar(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int fileHandle = params[0]._signedValue;
	UNUSED(fileHandle);
	int value = params[1]._signedValue;
	UNUSED(value);

	// FIXME
	error("FileWriteRawChar unimplemented");

	return RuntimeValue();
}

// import int FileReadRawInt(int fileHandle)
// Obsolete file I/O function.
RuntimeValue Script_FileReadRawInt(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int fileHandle = params[0]._signedValue;
	UNUSED(fileHandle);

	// FIXME
	error("FileReadRawInt unimplemented");

	return RuntimeValue();
}

// import int FileIsEOF(int fileHandle)
// Obsolete file I/O function.
RuntimeValue Script_FileIsEOF(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int fileHandle = params[0]._signedValue;
	UNUSED(fileHandle);

	// FIXME
	error("FileIsEOF unimplemented");

	return RuntimeValue();
}

// import int FileIsError(int fileHandle)
// Obsolete file I/O function.
RuntimeValue Script_FileIsError(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int fileHandle = params[0]._signedValue;
	UNUSED(fileHandle);

	// FIXME
	error("FileIsError unimplemented");

	return RuntimeValue();
}

// File: import static bool Delete(const string filename)
// Delets the specified file from the disk.
RuntimeValue Script_File_Delete(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	ScriptString *filename = (ScriptString *)params[0]._object;
	UNUSED(filename);

	// FIXME
	error("File::Delete unimplemented");

	return RuntimeValue();
}

// File: import static bool Exists(const string filename)
// Checks if the specified file exists on the disk.
RuntimeValue Script_File_Exists(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	ScriptString *filename = (ScriptString *)params[0]._object;
	UNUSED(filename);

	// FIXME
	error("File::Exists unimplemented");

	return RuntimeValue();
}

// File: import static File *Open(const string filename, FileMode)
// Opens the specified file in order to read from or write to it.
RuntimeValue Script_File_Open(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	ScriptString *filename = (ScriptString *)params[0]._object;
	uint32 filemode = params[1]._value;

	// Modes are:
	// 0 - NULL (error?)
	// 1 - "rb"
	// 2 - "wb"
	// 3 - "ab"

	Common::String wrappedName = vm->wrapFilename(filename->getString());
	Common::SaveFileManager *saveFileMan = vm->getSaveFileManager();

	ScriptFile *f = new ScriptFile();
	if (filemode == 1) {
		f->_inFile = saveFileMan->openForLoading(wrappedName);
	} else if (filemode == 2) {
		f->_outFile = saveFileMan->openForSaving(wrappedName);
	} else if (filemode == 3) {
		error("File::open: unsupported file mode 3");
	} else
		error("File::open: Invalid file mode %u", filemode);

	if (f->_inFile == NULL)
		return 0;

	f->_mode = filemode;

	RuntimeValue ret = f;
	ret._object->DecRef();
	return ret;
}

// File: import void Close()
// Closes the file.
RuntimeValue Script_File_Close(AGSEngine *vm, ScriptFile *self, const Common::Array<RuntimeValue> &params) {
	self->_outFile->finalize();
	delete self->_outFile;
	self->_outFile = NULL;
	self->_mode = 0;

	return RuntimeValue();
}

// File: import int ReadInt()
// Reads an integer value from the file.
RuntimeValue Script_File_ReadInt(AGSEngine *vm, ScriptFile *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("File::ReadInt unimplemented");

	return RuntimeValue();
}

// File: import int ReadRawChar()
// Reads the next raw byte from the file.
RuntimeValue Script_File_ReadRawChar(AGSEngine *vm, ScriptFile *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("File::ReadRawChar unimplemented");

	return RuntimeValue();
}

// File: import int ReadRawInt()
// Reads the next raw 32-bit int from the file.
RuntimeValue Script_File_ReadRawInt(AGSEngine *vm, ScriptFile *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("File::ReadRawInt unimplemented");

	return RuntimeValue();
}

// File: import void ReadRawLine(string buffer)
// Undocumented.
RuntimeValue Script_File_ReadRawLine(AGSEngine *vm, ScriptFile *self, const Common::Array<RuntimeValue> &params) {
	ScriptString *buffer = (ScriptString *)params[0]._object;
	UNUSED(buffer);

	// FIXME
	error("File::ReadRawLine unimplemented");

	return RuntimeValue();
}

// File: import void ReadString(string buffer)
// Undocumented.
RuntimeValue Script_File_ReadString(AGSEngine *vm, ScriptFile *self, const Common::Array<RuntimeValue> &params) {
	ScriptString *buffer = (ScriptString *)params[0]._object;
	UNUSED(buffer);

	// FIXME
	error("File::ReadString unimplemented");

	return RuntimeValue();
}

// File: import String ReadRawLineBack()
// Reads the next raw line of text from the file.
RuntimeValue Script_File_ReadRawLineBack(AGSEngine *vm, ScriptFile *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("File::ReadRawLineBack unimplemented");

	return RuntimeValue();
}

// File: import String ReadStringBack()
// Reads the next string from the file.
RuntimeValue Script_File_ReadStringBack(AGSEngine *vm, ScriptFile *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("File::ReadStringBack unimplemented");

	return RuntimeValue();
}

// File: import void WriteInt(int value)
// Writes an integer to the file.
RuntimeValue Script_File_WriteInt(AGSEngine *vm, ScriptFile *self, const Common::Array<RuntimeValue> &params) {
	int value = params[0]._signedValue;

	if (self->_mode != 2 && self->_mode != 3)
		error("File::WriteString: File not opened in 'write' mode");

	self->_outFile->writeSByte('I');
	self->_outFile->writeSint32LE(value);

	return RuntimeValue();
}

// File: import void WriteRawChar(int value)
// Writes a raw byte to the file.
RuntimeValue Script_File_WriteRawChar(AGSEngine *vm, ScriptFile *self, const Common::Array<RuntimeValue> &params) {
	int value = params[0]._signedValue;
	UNUSED(value);

	// FIXME
	error("File::WriteRawChar unimplemented");

	return RuntimeValue();
}

// File: import void WriteRawLine(const string text)
// Writes a raw line of text to the file.
RuntimeValue Script_File_WriteRawLine(AGSEngine *vm, ScriptFile *self, const Common::Array<RuntimeValue> &params) {
	ScriptString *text = (ScriptString *)params[0]._object;
	UNUSED(text);

	// FIXME
	error("File::WriteRawLine unimplemented");

	return RuntimeValue();
}

// File: import void WriteString(const string text)
// Writes a string to the file.
RuntimeValue Script_File_WriteString(AGSEngine *vm, ScriptFile *self, const Common::Array<RuntimeValue> &params) {
	ScriptString *text = (ScriptString *)params[0]._object;

	if (self->_mode != 2 && self->_mode != 3)
		error("File::WriteString: File not opened in 'write' mode");

	self->_outFile->writeSint32LE(text->getString().size()+1);
	self->_outFile->writeString(text->getString());
	self->_outFile->writeSByte(0);

	return RuntimeValue();
}

// File: readonly import attribute bool EOF
// Gets whether you have reached the end of the file.
RuntimeValue Script_File_get_EOF(AGSEngine *vm, ScriptFile *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("File::get_EOF unimplemented");

	return RuntimeValue();
}

// File: readonly import attribute bool Error
// Gets whether any errors occurred reading or writing the file.
RuntimeValue Script_File_get_Error(AGSEngine *vm, ScriptFile *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("File::get_Error unimplemented");

	return RuntimeValue();
}

static const ScriptSystemFunctionInfo ourFunctionList[] = {
	{ "FileOpen", (ScriptAPIFunction *)&Script_FileOpen, "ss", sotNone },
	{ "FileWrite", (ScriptAPIFunction *)&Script_FileWrite, "is", sotNone },
	{ "FileWriteRawLine", (ScriptAPIFunction *)&Script_FileWriteRawLine, "is", sotNone },
	{ "FileRead", (ScriptAPIFunction *)&Script_FileRead, "is", sotNone },
	{ "FileClose", (ScriptAPIFunction *)&Script_FileClose, "i", sotNone },
	{ "FileWriteInt", (ScriptAPIFunction *)&Script_FileWriteInt, "ii", sotNone },
	{ "FileReadInt", (ScriptAPIFunction *)&Script_FileReadInt, "i", sotNone },
	{ "FileReadRawChar", (ScriptAPIFunction *)&Script_FileReadRawChar, "i", sotNone },
	{ "FileWriteRawChar", (ScriptAPIFunction *)&Script_FileWriteRawChar, "ii", sotNone },
	{ "FileReadRawInt", (ScriptAPIFunction *)&Script_FileReadRawInt, "i", sotNone },
	{ "FileIsEOF", (ScriptAPIFunction *)&Script_FileIsEOF, "i", sotNone },
	{ "FileIsError", (ScriptAPIFunction *)&Script_FileIsError, "i", sotNone },
	{ "File::Delete^1", (ScriptAPIFunction *)&Script_File_Delete, "s", sotNone },
	{ "File::Exists^1", (ScriptAPIFunction *)&Script_File_Exists, "s", sotNone },
	{ "File::Open^2", (ScriptAPIFunction *)&Script_File_Open, "si", sotNone },
	{ "File::Close^0", (ScriptAPIFunction *)&Script_File_Close, "", sotFile },
	{ "File::ReadInt^0", (ScriptAPIFunction *)&Script_File_ReadInt, "", sotFile },
	{ "File::ReadRawChar^0", (ScriptAPIFunction *)&Script_File_ReadRawChar, "", sotFile },
	{ "File::ReadRawInt^0", (ScriptAPIFunction *)&Script_File_ReadRawInt, "", sotFile },
	{ "File::ReadRawLine^1", (ScriptAPIFunction *)&Script_File_ReadRawLine, "s", sotFile },
	{ "File::ReadString^1", (ScriptAPIFunction *)&Script_File_ReadString, "s", sotFile },
	{ "File::ReadRawLineBack^0", (ScriptAPIFunction *)&Script_File_ReadRawLineBack, "", sotFile },
	{ "File::ReadStringBack^0", (ScriptAPIFunction *)&Script_File_ReadStringBack, "", sotFile },
	{ "File::WriteInt^1", (ScriptAPIFunction *)&Script_File_WriteInt, "i", sotFile },
	{ "File::WriteRawChar^1", (ScriptAPIFunction *)&Script_File_WriteRawChar, "i", sotFile },
	{ "File::WriteRawLine^1", (ScriptAPIFunction *)&Script_File_WriteRawLine, "s", sotFile },
	{ "File::WriteString^1", (ScriptAPIFunction *)&Script_File_WriteString, "s", sotFile },
	{ "File::get_EOF", (ScriptAPIFunction *)&Script_File_get_EOF, "", sotFile },
	{ "File::get_Error", (ScriptAPIFunction *)&Script_File_get_Error, "", sotFile },
};

void addFileSystemScripting(AGSEngine *vm) {
	GlobalScriptState *state = vm->getScriptState();

	state->addSystemFunctionImportList(ourFunctionList, ARRAYSIZE(ourFunctionList));
}

} // End of namespace AGS
