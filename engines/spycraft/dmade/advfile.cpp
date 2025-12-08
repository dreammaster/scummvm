/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "spycraft/afxwin.h"
#include "spycraft/dmade/advfile.h"

namespace Spycraft {

#define PATHSIZE 256
#define FILTERSIZE 128

extern HWND hGameWnd;

//OPENFILENAME ofn;
char filter[FILTERSIZE];
char fileName[PATHSIZE];

//OPENFILENAME sfn;
char sFilter[FILTERSIZE];
char sFileName[PATHSIZE];

uint16 sfxFileGetWORD(HANDLE hf) {
	return ((Common::SeekableReadStream *)hf)->readUint16LE();
}

uint8 sfxFileGetBYTE(HANDLE hf) {
	return ((Common::SeekableReadStream *)hf)->readByte();
}

HANDLE sfxOpenFile(const char *filename, int mode) {
	assert(mode == MADE_FILE_READ);

	Common::File *f = new Common::File();
	if (f->open(filename))
		return (HANDLE)f;

	return (HANDLE)-1;
}

int sfxGetFileSize(HANDLE hf) {
	Common::SeekableReadStream *rs = (Common::SeekableReadStream *)hf;
	return rs->size();
}

int sfxWriteASCFile(HANDLE hf, const char *format, ...) {
	va_list list;

	va_start(list, format);
	Common::String str = Common::String::vformat(format, list);
	va_end(list);

	Common::DumpFile *df = (Common::DumpFile *)hf;
	df->writeString(str);

	return 0;
}

int sfxSeekFile(HANDLE hf, int offset, int mode) {
	Common::SeekableReadStream *f = (Common::SeekableReadStream *)hf;

	switch (mode) {
	case MADE_SEEK_BEG:
		return f->seek(offset, SEEK_SET);

	case MADE_SEEK_CUR:
		return f->seek(offset, SEEK_CUR);

	case MADE_SEEK_END:
		return f->seek(offset, SEEK_END);
	}

	return 0;
}

int sfxWriteFile(HANDLE hf, void *buffer, int size) {
	Common::WriteStream *ws = (Common::WriteStream *)hf;

	return ws->write(buffer, size);
}

int sfxReadFile(HANDLE hf, void *buffer, int size) {
	Common::SeekableReadStream *f = (Common::SeekableReadStream *)hf;

	return f->read(buffer, size);
}

char *sfxReadFileString(HANDLE hf) {
	Common::SeekableReadStream *f = (Common::SeekableReadStream *)hf;
	int i = 0;
	char buffer;
	bool ret = true;
	char dest[2048];
	char *out = nullptr;

	while (ret) {
		buffer = f->readByte();

		if (ret) {
			if ((buffer == '\0') || (buffer == '\n'))
				break;
			if (buffer != '\r' && buffer != 0x09)
				dest[i++] = buffer;
		} else
			return nullptr;

		if (i == 2048)
			ADV_ASSERT(false, __ERR_BUFFER_OVERFLOW);
	}

	if (i) {
		dest[i] = '\0';
		out = scumm_strdup(dest);
	}

	return out;
}

int sfxGetString(HANDLE hf, char *dest) {
	Common::SeekableReadStream *f = (Common::SeekableReadStream *)hf;
	int i = 0;
	char buffer;
	int ret = true;

	while (ret) {
		buffer = f->readByte();

		if (ret) {
			if ((buffer == '\0') || (buffer == '\n'))
				break;
			if (buffer != '\r' && buffer != 0x09)
				dest[i++] = buffer;
		} else
			return (-1);
	}
	dest[i] = '\0';
	return i;
}

int sfxCloseFile(HANDLE hf) {
	Common::Stream *f = (Common::Stream *)hf;

	delete f;
	return 0;
}

size_t sfxFileSize(HANDLE hf) {
	Common::Stream *s = (Common::Stream *)s;
	Common::SeekableReadStream *rs = dynamic_cast<Common::SeekableReadStream *>(s);
	Common::SeekableWriteStream *ws = dynamic_cast<Common::SeekableWriteStream *>(s);

	if (rs)
		return rs->size();
	if (ws)
		return ws->size();

	error("Invalid file");
}

int sfxFileSize(const char *filename) {
	Common::File f;
	return f.open(filename) ? f.size() : 0;
}

char *sfxGetRestoreFile(void) {
	return fileName;
}

char *sfxGetSaveFile(void) {
	return (sFileName);
}

void OpenDlg(const char *fileMask) {
#ifdef TODO
	char chReplace;
	int i;

	wsprintf(filter, "Saved Game Files (*.%s)|*.%s|", fileMask, fileMask);

	chReplace = filter[strlen(filter) - 1];

	for (i = 0; filter[i] != '\0'; i++) {
		if (filter[i] == chReplace)
			filter[i] = '\0';
	}

	memset(&ofn, 0, sizeof(OPENFILENAME));

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hGameWnd;
	ofn.hInstance = nullptr;
	ofn.lpstrFilter = filter;
	ofn.lpstrCustomFilter = nullptr;
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = nullptr;
	ofn.nMaxFile = 0;
	ofn.lpstrFileTitle = fileName;
	ofn.nMaxFileTitle = sizeof(fileName);
	ofn.lpstrInitialDir = nullptr;
	ofn.lpstrTitle = nullptr;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.nFileOffset = 0;
	ofn.nFileExtension = 0;
	ofn.lpstrDefExt = nullptr;
	ofn.lCustData = 0L;
	ofn.lpfnHook = nullptr;
	ofn.lpTemplateName = nullptr;

	if (!GetOpenFileName(&ofn))
		fileName[0] = 0;
#endif
}

void ParseSaveFileName(char *filename, const char *fileMask) {
	char *ptr;

	ptr = filename;
	while (*ptr) {
		if (*ptr == '.') {
			ptr++;
			if (scumm_stricmp(ptr, fileMask)) {
				Common::strcpy_s(ptr, 256, fileMask);
				ErrMsg("Saving as '%s'", filename);
				return;
			} else
				return;
		}
		ptr++;
	}

	Common::strcat_s(filename, 256, ".");
	Common::strcat_s(filename, 256, fileMask);
}

void SaveDlg(const char *fileMask) {
#ifdef TODO
	char chReplace;
	int i;

	wsprintf(sFilter, "Save Game File (*.%s)|*.%s|", fileMask, fileMask);

	chReplace = sFilter[strlen(sFilter) - 1];

	for (i = 0; sFilter[i] != '\0'; i++) {
		if (sFilter[i] == chReplace)
			sFilter[i] = '\0';
	}

	memset(&sfn, 0, sizeof(OPENFILENAME));

	sfn.lStructSize = sizeof(OPENFILENAME);
	sfn.hwndOwner = hGameWnd;
	sfn.hInstance = nullptr;
	sfn.lpstrFilter = sFilter;
	sfn.lpstrCustomFilter = nullptr;
	sfn.nFilterIndex = 1;
	sfn.lpstrFile = nullptr;
	sfn.nMaxFile = 0;
	sfn.lpstrFileTitle = sFileName;
	sfn.nMaxFileTitle = sizeof(sFileName);
	sfn.lpstrInitialDir = nullptr;
	sfn.lpstrTitle = nullptr;
	sfn.Flags = OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY;
	sfn.nFileOffset = 0;
	sfn.nFileExtension = 0;
	sfn.lpstrDefExt = nullptr;
	sfn.lCustData = 0L;
	sfn.lpfnHook = nullptr;
	sfn.lpTemplateName = nullptr;

	if (!GetSaveFileName(&sfn))
		sFileName[0] = 0;
	else
		ParseSaveFileName(sFileName, fileMask);
#endif
}

bool sfxCopyFile(const char *src, const char *dest) {
	Common::File fIn;
	Common::DumpFile fOut;

	if (!fIn.open(src) || !fOut.open(dest))
		return false;

	fOut.writeStream(&fIn);
	return true;
}


} // namespace Spycraft
