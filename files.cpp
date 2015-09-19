/* RogueVM - RPG Engines
 *
 * RogueVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include "common/scummsys.h"
#include "common/system.h"
#include "common/config-manager.h"
#include "aesop/aesop.h"
#include "aesop/files.h"
#include "aesop/rtmsg.h"
#include "aesop/rtsystem.h"
#include "aesop/stubs.h"
#include "aesop/utils.h"

namespace Aesop {

#define FILE_BUFFER_SIZE 4096

BinaryFile::BinaryFile() : _inSave(nullptr), _outSave(nullptr) {
}

BinaryFile::BinaryFile(const Common::String &filename, FileMode oFlag) {
	if (!open(filename, oFlag))
		error("File not found");
}

BinaryFile::~BinaryFile() {
	close();
}

bool BinaryFile::open(const Common::String &filename, FileMode oFlag) {
	Common::String fname = filename;
	bool isSavegame = fname.hasPrefix("SAVEGAME\\");
	if (isSavegame)
		fname = Common::String(fname.c_str() + 9);
	_mode = oFlag;

	if (oFlag == FILE_WRITE) {
		// For writing files, we only allow writing savegame files
		assert(isSavegame);

		_outSave = g_system->getSavefileManager()->openForSaving(fname, false);
		return _outSave != nullptr;
	} else {
		// If it's a savegame folder file, first try to load it from the save path
		if (isSavegame) {
			_inSave = g_system->getSavefileManager()->openForLoading(fname);
			if (_inSave)
				return true;
		}

		// Fallback on getting a local file
		if (isSavegame) {
			Common::FSNode dirNode(ConfMan.get("path"));
			Common::FSNode fileNode = dirNode.getChild("SAVEGAME").getChild(fname);
			return _inFile.open(fileNode);
		} else {
			return _inFile.open(fname);
		}
	}
}

void BinaryFile::close() {
	_inFile.close();
	delete _inSave;
	delete _outSave;
	_inSave = nullptr;
	_outSave = nullptr;
}

uint BinaryFile::read(void *buffer, uint size) {
	if (_inSave)
		return _inSave->read(buffer, size);
	else
		return _inFile.read(buffer, size);
}

byte BinaryFile::readByte() {
	byte v;
	if (pos() >= size())
		return 0;

	read(&v, 1);
	return v;
}

uint BinaryFile::write(const void *buffer, uint size) {
	assert(_outSave);
	return _outSave->write(buffer, size);
}

void BinaryFile::writeByte(byte v) {
	write(&v, 1);
}

uint BinaryFile::pos() const {
	assert(_mode == FILE_READ);
	return _inSave ? _inSave->pos() : _inFile.pos();
}

uint BinaryFile::size() const {
	return _inSave ? _inSave->size() : _inFile.size();
}

BinaryFile::operator Common::SeekableReadStream &() {
	if (_inSave)
		return *_inSave;
	return _inFile;
}

BinaryFile::operator Common::WriteStream &() {
	return *_outSave;
}

/*----------------------------------------------------------------*/

void TextFile::wchar(char ch) {
	write(&ch, 1);
}

char TextFile::rchar() {
	if (pos() >= size())
		return 0;

	return (char)readByte();
}

bool TextFile::readln(Common::String &buffer, uint maxlen) {
	char c;
	buffer.clear();

	do {
		while (buffer.size() != (maxlen - 1)) {
			c = rchar();

			if (c == '\n') break;
			if (c == '\r') continue;

			buffer += c;

			if (!c) return false;
		}

		if (buffer.size() == (maxlen - 1))
			while ((c = rchar()) != '\n')
				if (!c) return false;

		buffer.deleteLastChar();
	} while (buffer.empty());

	return true;
}

bool TextFile::writeln(const Common::String &line) {
	write(line.c_str(), line.size());
	write("\r\n", 2);
	return true;
}

/*----------------------------------------------------------------*/

bool Files::deleteFile(const Common::String &filename) {
	error("TODO");
}

int Files::copyFile(const Common::String &srcFilename, const Common::String &destFilename) {
	Common::File inFile;
	Common::DumpFile outFile;
	byte buffer[FILE_BUFFER_SIZE];
	uint size;

	if (!inFile.open(srcFilename) || !outFile.open(destFilename))
		return -1;

	while ((size = inFile.read(buffer, FILE_BUFFER_SIZE)) != 0) {
		outFile.write(buffer, size);
	}

	inFile.close();
	outFile.close();

	return 1;
}

uint Files::fileSize(const Common::String &filename) {
	Common::File f;
	if (!f.open(filename))
		return 0;

	uint filesize = f.size();
	f.close();

	return filesize;
}

byte *Files::readFile(const Common::String &filename, byte *dest) {
	Common::File f;
	uint len;
	byte *buf;

	_diskError = NO_ERROR;

	// Open the file for access
	if (!f.open(filename)) {
		_diskError = FILE_NOT_FOUND;
		return nullptr;
	}

	// Get memory for the file
	len = f.size();
	buf = (dest == NULL) ? (byte *)malloc(len) : dest;
	if (buf == nullptr) {
		_diskError = OUT_OF_MEMORY;
		return nullptr;
	}

	// Read in the file's contents
	f.read(buf, len);
	f.close();

	return buf;
}

uint Files::writeFile(const Common::String &filename, void *buf, uint len) {
	Common::DumpFile f;

	_diskError = NO_ERROR;

	if (!f.open(filename)) {
		_diskError = CANT_WRITE_FILE;
		return 0;
	}

	f.write(buf, len);
	f.close();

	return 1;
}

uint Files::appendFile(const Common::String &filename, void *buf, uint len) {
	Common::File inFile;
	Common::DumpFile outFile;

	_diskError = NO_ERROR;

	// Unfortunately, DumpFile doesn't have a way to append to existing files,
	// so we'll have to read in the existing contents
	if (!inFile.open(filename)) {
		_diskError = CANT_WRITE_FILE;
		return 0;
	}

	uint oldLen = inFile.size();
	byte *buffer = new byte[len];
	inFile.read(buffer, len);
	inFile.close();

	// Write out the new file, which has the old data, and then the new data at the end
	if (!outFile.open(filename)) {
		_diskError = CANT_WRITE_FILE;
		return 0;
	}

	outFile.write(buffer, oldLen);
	outFile.write(buf, len);
	outFile.close();

	delete[] buffer;
	return 1;
}

uint Files::fileTime(const Common::String &filename) {
	// ScummVM doesn't support returning file times
	return 0L;
}

int Files::saveRange(const Common::String &filename, FileType fileType, uint first, uint last) {
	Objects &objects = *_vm->_objects;
	LONG good;
	ULONG index;
	UBYTE typetest;
	TextFile *TF;
	HRES instance, thunk;
	HD_entry *hd_inst;
	void *tptr;
	THDR thdr;
	CDESC CD;

	good = 1;

	if (fileType == SF_TXT) {
		TF = new TextFile(filename, FILE_WRITE);
		if (TF == nullptr)
			return 0;

		for (index = first; index <= last; index++) {
			objects.dumpStaticContext(index, TF);

			if (!TF->writeln("")) {
				good = 0;
				break;
			};
		}

		delete TF;
	} else {
		BinaryFile f;
		if (!f.open(filename, FILE_WRITE))
			return 0;

		typetest = 26;
		f.writeByte(typetest);

		for (index = first; index <= last; index++) {
			CD.slot = index;

			instance = objects[index];
			if (instance == HRES_NULL) {
				CD.name = (ULONG)-1;
				CD.size = 0;
			} else {
				thunk = ((IHDR *)Resources::addr(instance))->_thunk;

				tptr = Resources::addr(thunk);
				thdr = *((THDR *)tptr);

				hd_inst = (HD_entry *)instance;
				CD.name = hd_inst->_user;
				CD.size = thdr._iSize - sizeof(IHDR);
			}

			CD.save(f);

			if (CD.size)
				if (f.write(add_ptr(Resources::addr(objects[index]), sizeof(IHDR)),
					CD.size) != CD.size) {
					good = 0;
					break;
				}
		}

		f.close();
	}

	return good;
}

void Files::restoreRange(const Common::String &filename, uint first, uint last, uint restoring) {
	Objects &objects = *_vm->_objects;
	ULONG bad, txttype;
	ULONG index;
	UBYTE typetest;
	TextFile *TF = nullptr;
	HRES cur;
	HD_entry *sel;
	CDESC stat_C;
	CDESC *CD;

	UWORD CDslot;       // object list index
	ULONG CDname;       // code object name
	UWORD CDsize;       // size of instance data (unused in text files)

	txttype = 0;
	bad = 0;

	BinaryFile f;
	if (!f.open(filename, FILE_READ))
		bad = 1;
	else {
		typetest = f.readByte();

		if (typetest != 26) {
			f.close();
			txttype = 1;
		}
	}

	if (txttype) {
		TF = new TextFile(filename, FILE_READ);
		bad = (TF == NULL);
	}

	if (bad)
		error(MSG_CNOCF, filename);          //"Could not open context file '%s'"

	for (index = first; index <= last; index++) {
		cur = objects[index];

		if (txttype) {
			CD = readContextDescriptor(TF);

			bad = (CD == NULL);
		} else {
			CD = &stat_C;
			CD->load(f);
		}

		if ((bad) || (CD->slot != index))
			error(MSG_CFCAE, index);          //"Context file corrupt at entry %u"

		CDslot = CD->slot;
		CDname = CD->name;
		CDsize = CD->size;

		if (CD->name == (ULONG)-1L) {
			if (cur != HRES_NULL) {
				objects.destroyObject(index);
			}

			continue;
		}

		if (cur != HRES_NULL) {
			sel = (HD_entry *)cur;

			if (sel->_user != CD->name) {
				objects.destroyObject(index);
			} else {
				_vm->_events->cancelEntityRequests(index);
				_vm->_screen->releaseOwnedWindows(index);
			}
		}

		if (objects[index] == HRES_NULL) {
			objects.createSOPinstance(CD->name, index);
		}

		if (txttype) {
			restoreStaticContext(objects[index], TF);
		} else if (CD->size) {
			f.read((byte *)Resources::addr(objects[index]) + sizeof(IHDR), CD->size);
		}

		if (restoring) {
			Interpreter *interp = new Interpreter(_vm);
			interp->execute(index, MSG_RESTORE);
			delete interp;
		}
	}

	if (txttype)
		delete TF;
	else
		f.close();
}

CDESC *Files::readContextDescriptor(TextFile *TF) {
	Interpreter &interp = *_vm->_interpreter;
	Resources &res = *_vm->_resources;
	static CDESC c;
	HRES HROED;
	const char *num;
	const char *name;
	const char *def;
	Common::String line;

	if (!TF->readln(line, 128))
		return nullptr;

	num = line.c_str() + 6;
	c.size = (UWORD)-1;
	c.slot = (UWORD)ascnum(num);

	name = strchr(num, '"');
	if (name == NULL) {
		c.name = (ULONG)-1L;
		return &c;
	}

	Common::String contextName(name + 1);
	contextName.deleteLastChar();

	HROED = res.get_resource_handle(ROED, DA_TEMPORARY | DA_EVANESCENT);
	res.lock(HROED);

	def = (const char *)interp.lookup(HROED, contextName);

	if (def == NULL)
		abend(MSG_OMCR, contextName, c.slot);

	c.name = ascnum(def);
	res.unlock(HROED);

	return &c;
}

void Files::restoreStaticContext(HRES instance, TextFile *TF) {
	Interpreter &interp = *_vm->_interpreter;
	Resources &res = *_vm->_resources;
	ULONG n, i, p, offset = 0, asize;
	HRES thunk, expt = 0;
	void *d, *tptr;
	SD_entry *SD;
	THDR thdr;
	const char *def, *size, *tagP;
	const char *chrpnt;
	Common::String name;
	Common::String line, tag;

	thunk = ((IHDR *)Resources::addr(instance))->_thunk;
	thdr = *((THDR *)Resources::addr(thunk));

	while (TF->readln(line, 128)) {
		if (line[0] == '{')
			continue;
		else if (line[0] == '}')
			break;
		else if (line[0] == '[') {
			Common::String tag(strchr(line.c_str(), '[') + 1);
			if ((tagP = strchr(tag.c_str(), ']')) != nullptr)
				tag = Common::String(tag.c_str(), tagP);

			for (p = 0; p < thdr._nPrgs; p++) {
				tptr = Resources::addr(thunk);
				SD = (SD_entry *)add_ptr(tptr, thdr._sdList);

				offset = SD[p].static_base;

				expt = res.get_resource_handle(SD[p].exports,
					DA_TEMPORARY | DA_EVANESCENT);

				res.lock(expt);
				def = (const char *)interp.lookup(expt, "N:OBJECT");

				res.unlock(expt);

				if (!strcmp(def, tag.c_str()))
					break;
			}

			if (p == thdr._nPrgs)
				abend(MSG_CMCR, tag);       //"Class '%s' missing; cannot restore"
		} else {
			tag = line;
		
			// Skip leading white-space
			while (tag.hasPrefix(" "))
				tag.deleteChar(0);

			// Get Variable Type and Name
			for (i = 0; (tag[i] != ' ') && tag[i]; ++i)
				name += tag[i];

			if (!i)
				continue;

			res.lock(expt);
			def = (const char *)interp.lookup(expt, name);

			res.unlock(expt);

			if (def == NULL)
				abend(MSG_UVR, name);       //"Unresolved variable reference '%s'"

			d = (void *)((ULONG)Resources::addr(instance) + ascnum(def) + offset);

			if ((size = strchr(def, ',')) != NULL)
				asize = ascnum(size + 1);
			else
				asize = 1;

			chrpnt = strchr(line.c_str(), '=');
			if (chrpnt == NULL)
				abend(MSG_BDIE);           //"Bad data item entry"

			for (n = 0; n < asize; n++) {
				//Skip to data
				while ((!aesop_isnumprefix(*chrpnt)) && (*chrpnt != '\'')) {
					if (!(*chrpnt))
						break;
					else
						++chrpnt;
				}

				//Get Next Line if needed

				if ((!(*chrpnt)) && (n != asize - 1)) {
					TF->readln(line, 128);
					chrpnt = line.c_str();
				}

				// Store data from text file in Instance
				switch (name[0]) {
				case 'B':
					*(BYTE *)d = (BYTE)ascnum(chrpnt);
					break;
				case 'W':
					*(WORD *)d = (WORD)ascnum(chrpnt);
					break;
				case 'L':
					*(LONG *)d = (LONG)ascnum(chrpnt);
					break;
				}

				if (n != asize - 1) {
					switch (name[0]) {
					case 'B':
						d = ((BYTE *)d + 1);
						break;
					case 'W':
						d = ((BYTE *)d + 2);
						break;
					case 'L':
						d = ((BYTE *)d + 4);
						break;
					}

					while (aesop_isnumprefix(*chrpnt) || (*chrpnt == ' ') || (*chrpnt == '\''))
						chrpnt++;
				}
			} // for
		} // else
	} // while
}

void Files::translateFile(const Common::String &txtFilename, const Common::String &binFilename, uint first, uint last) {
	Resources &res = *_vm->_resources;
	TextFile *TF;
	CDESC *CD;
	CDESC CD_out;
	LONG index;
	HRES instance, thunk;
	void *tptr;
	THDR thdr;
	UBYTE typetest;

	Common::DumpFile f;
	if (!f.open(binFilename))
		abend(MSG_COOFFT);      //"Couldn't open output file for translation"

	typetest = 26;
	f.writeByte(typetest);

	TF = new TextFile(txtFilename, FILE_READ);
	if (TF == NULL)
		abend(MSG_COIFFT);      //"Couldn't open input file for translation"

	for (index = first; index <= (LONG)last; index++) {
		CD = readContextDescriptor(TF);
		if ((CD == NULL) || (CD->slot != index))
			abend(MSG_CTCFE, index); //"Couldn't translate context file entry %u"

		CD_out.name = CD->name;
		CD_out.slot = index;

		if (CD->name != (ULONG)-1L) {
			instance = res.create_instance(CD->name);

			thunk = ((IHDR *)Resources::addr(instance))->_thunk;

			tptr = Resources::addr(thunk);
			thdr = *((THDR *)tptr);

			CD_out.size = thdr._iSize - sizeof(IHDR);

			error("TODO: CDESC::save");
			//write(handle,&CD_out,sizeof(CDESC));

			restoreStaticContext(instance, TF);

			if (CD_out.size) {
				if (f.write(add_ptr(Resources::addr(instance), sizeof(IHDR)), CD_out.size)
					!= CD_out.size) {
					abend(MSG_CWTE, index);  //"Couldn't write translated entry %u"
				}
			}

			res.destroy_instance(instance);
		} else {
			CD_out.size = 0;
			error("TODO: CDESC::save");
			//write(handle,&CD_out,sizeof(CDESC));
		}
	}

	delete TF;
	f.close();
}

} // End of namespace Aesop

