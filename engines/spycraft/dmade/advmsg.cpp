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

#include "spycraft/dmade/advlib.h"
#include "spycraft/dmade/advmsg.h"
#include "spycraft/dmade/advres.h"

namespace Spycraft {

void FreeMSG(MSGTag *tag) {
	int i;

	for (i = 0; i < tag->size; i++)
		FreePtr(tag->data[i]);
	FreePtr(tag->data);

	FreePtr(tag);
}

static char *StrDup(char *str) {
	return scumm_strdup(str);
}

static char *ReadFileString(HANDLE hf) {
	int i;
	char buffer;
	char dest[2048];

	i = 0;
	while (1) {
		if (i == 2048 - 1)
			break;
		if (!sfxReadFile(hf, &buffer, 1))
			break;
		if ((buffer == '\0') || (buffer == '\n'))
			break;
		if ((buffer != '\r') && (buffer != 0x09))
			dest[i++] = buffer;
	}
	if (i == 0)
		return (NULL);

	dest[i] = '\0';
	return (StrDup(dest));
}

MSGTag *OpenMSGFile(char *filename) {
	int i;
	char *str;
	MSGTag *ret;
	char *strList[2048];
	HANDLE hf = sfxOpenFile(filename, MADE_FILE_READ);

	if (hf == (HANDLE)-1) {
		ErrMsg("Can't find %s, please reinstall Spycraft.", filename);
		ASSERT(false, __ERR_FILE_OPEN_FAIL);
	}

	i = 0;
	do {
		str = ReadFileString(hf);
		if (str)
			strList[i++] = str;
	} while (str);

	sfxCloseFile(hf);

	ret = (MSGTag *)AllocPtr(sizeof(MSGTag));
	if (ret == NULL)
		return (NULL);

	ret->size = i;
	ret->data = (char **)AllocPtr(sizeof(char *) * i);
	if (ret->data == NULL) {
		FreePtr(ret);
		return (NULL);
	}

	for (i = 0; i < ret->size; i++)
		ret->data[i] = strList[i];

	return (ret);
}

static byte *bufPtr;
static int bufSize, bufLoc;

static char *ReadMemString() {
	int i;
	char buffer;
	char dest[2048];

	i = 0;
	while (1) {
		if (i == 2048 - 1)
			break;
		if (bufLoc == bufSize)
			break;
		buffer = bufPtr[bufLoc++];
		if ((buffer == '\0') || (buffer == '\n'))
			break;
		if ((buffer != '\r') && (buffer != 0x09))
			dest[i++] = buffer;
	}
	if (i == 0)
		return (NULL);

	dest[i] = '\0';
	return (StrDup(dest));
}

MSGTag *OpenMSG(void *buffer, int size) {
	int i;
	MSGTag *ret;
	char *strList[2048];

	if ((buffer == NULL) || (size <= 0))
		return NULL;

	bufPtr = (byte *)buffer;
	bufSize = size;
	bufLoc = 0;

	for (i = 0; i < 2048; i++) {
		strList[i] = ReadMemString();
		if (strList[i] == NULL)
			break;
	}

	ret = (MSGTag *)AllocPtr(sizeof(MSGTag));
	if (ret == NULL)
		return (NULL);

	ret->size = i;
	ret->data = (char **)AllocPtr(sizeof(char *) * i);
	if (ret->data == NULL) {
		FreePtr(ret);
		return (NULL);
	}

	for (i = 0; i < ret->size; i++)
		ret->data[i] = strList[i];

	FreePtr(buffer);
	return (ret);
}

char *sfxGetMsg(int id, int index, char *dest) {
	MSGTag *tag = (MSGTag *)sfxLoadRes(id, RES_TEXT);

	if (tag == NULL)
		return (NULL);

	if (index > (tag->size - 1))
		return (NULL);

	if (dest)
		Common::strcpy_s(dest, 65536, tag->data[index]);

	return (tag->data[index]);
}

} // namespace Spycraft
