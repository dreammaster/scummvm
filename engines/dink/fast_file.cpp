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

#include "common/debug.h"
#include "common/textconsole.h"
#include "dink/fast_file.h"
#include "dink/file.h"

namespace Dink {

struct FileEntry {
	int32 offset;
	char name[13];

	void load(byte *&ptr) {
		offset = READ_LE_UINT32(ptr);
		Common::copy(ptr + 4, ptr + 4 + 13, name);
		ptr += 17;
	}
};
typedef FileEntry *PFileEntry;

struct FileHandle {
	bool inuse;
	int32 pos;
	int32 size;
	PFileEntry pfe;
};
typedef FileHandle *PFileHandle;

static int LockCount = 0;
static File *hFile = nullptr;
static byte *pBase = nullptr;
static uint32 indexCount = 0;
static PFileEntry index = nullptr;

static PFileHandle lpFH = nullptr;
static uint32 dwFHCnt = 0;
static int32 lFileEnd = 0;

/**
 * Comparison handler
 */
struct FilenameTest : public Common::UnaryFunction<const FileEntry &, bool> {
	const char *_fname;
	FilenameTest(const char *fname) : _fname(fname) {
	}
	bool operator()(const FileEntry &fe) const {
		return scumm_stricmp(fe.name, _fname);
	}
};


bool FastFileInit(const char *fname, int max_handles) {
	//HRSRC  h;

	LockCount = 0;
	FastFileFini();

	// Get a file handle array
	lpFH = new FileHandle[max_handles];
	if (lpFH == nullptr) {
		return false;
	}
	dwFHCnt = max_handles;

#if 0
	/*
	 * try our resourse file first
	 */
	if (h = FindResource(nullptr, fname, RT_RCDATA)) {
		pBase = LockResource(LoadResource(nullptr, h));

		if (pBase == nullptr) {
			ODS("FastFileInit: unable to lock resource");
			FastFileFini();
			return false;
		}

		ODS("FastFileInit: opened resource: ");
		ODS(fname);
		ODS("");
	} else {
#endif
		/*
		 * create a memory mapped file for the master file
		 */
		hFile = new File();

		if (!hFile->open(fname)) {
			warning("FastFileInit: CreateFile(%s)", fname);
			delete hFile;
			hFile = nullptr;
			FastFileFini();
			return false;
		}

		size_t size = hFile->size();
		pBase = new byte[size];
		if (hFile->read(pBase, size) != size) {
			warning("FastFileInit: MapViewOfFile Failed");
			FastFileFini();
			return false;
		}
#if 0
	}
#endif

	// Get initial data from the memory mapped file
	indexCount = READ_LE_UINT32(pBase);
	index = new FileEntry[indexCount];

	byte *p = pBase + 4;
	for (uint i = 0; i < indexCount; ++i)
		index[i].load(p);

	lFileEnd = index[indexCount - 1].offset;

	return true;
}

void FastFileFini(void) {
	delete[] pBase;
	pBase = nullptr;

	delete hFile;
	hFile = nullptr;

	delete[] index;
	index = nullptr;
	dwFHCnt = 0;
	indexCount = 0;
}

HFASTFILE FastFileOpen(const char *name) {
	if (lpFH == nullptr)
		error("FastFileOpen: not initialized");

	if (name == nullptr || !name[0])
		error("FastFileOpen: invalid name");

	PFileEntry pfe = Common::find_if(&index[0], &index[indexCount - 1],
	                                 FilenameTest(name));

	if (pfe != nullptr) {
		uint32 i;
		for (i = 0; i < dwFHCnt; i++) {
			if (!lpFH[i].inuse) {
				lpFH[i].inuse = true;
				lpFH[i].pos = pfe->offset;
				lpFH[i].size = (pfe + 1)->offset - pfe->offset;
				lpFH[i].pfe = pfe;
				return &lpFH[i];
			}
		}

		error("FastFileOpen: Out of file handles");
	} else {
		debugC(1, "FastFileOpen: File \"%s\"", name);
	}

	return nullptr;
}

bool FastFileClose(HFASTFILE ff) {
	PFileHandle pfh = (PFileHandle)ff;

	if (pfh == nullptr || pfh->inuse != true) {
		error("FastFileClose: invalid handle");
		return false;
	}

	pfh->inuse = false;
	return true;
}

void *FastFileLock(HFASTFILE ff, int pos, int size) {
	PFileHandle pfh = (PFileHandle)ff;

	if (pfh == nullptr || pfh->inuse != true) {
		error("FastFileLock: invalid handle");
		return nullptr;
	}
	if (size < 0) {
		error("FastFileLock: invalid size");
		return nullptr;
	}
	if ((pos + size) > ((pfh->pfe) + 1)->offset) {
		error("FastFileLock: read past end of file");
		return nullptr;
	}

	LockCount++;
	return pBase + pfh->pos + pos;
}

bool FastFileUnlock(HFASTFILE ff, int pos, int size) {
	PFileHandle pfh = (PFileHandle)ff;

	if (pfh == nullptr || pfh->inuse != true) {
		error("FastFileUnlock: invalid handle");
		return false;
	}
	if (size < 0) {
		error("FastFileUnlock: invalid size");
		return false;
	}
	if ((pos + size) > ((pfh->pfe) + 1)->offset) {
		error("FastFileUnlock: read past end of file");
		return false;
	}

	LockCount--;
	return true;
}

bool FastFileRead(HFASTFILE ff, void *ptr, int size) {
	PFileHandle pfh = (PFileHandle)ff;

	if (pfh == nullptr || pfh->inuse != true) {
		error("FastFileRead: invalid handle");
		return false;
	}
	if (size < 0) {
		error("FastFileRead: invalid size");
		return false;
	}
	if ((pfh->pos + size) > ((pfh->pfe) + 1)->offset) {
		error("FastFileRead: read past end of file");
		return false;
	}

	memcpy(ptr, pBase + pfh->pos, size);
	pfh->pos += size;
	return true;
}

bool FastFileSeek(HFASTFILE ff, int off, int how) {
	PFileHandle pfh = (PFileHandle)ff;
	PFileEntry  pfe;

	if (pfh == nullptr || pfh->inuse != true) {
		error("FastFileSeek: invalid handle");
		return false;
	}
	pfe = pfh->pfe;
	if (how == SEEK_SET) {
		if (off < 0 || off >= pfh->size) {
			error("FastFileSeek: Invalid offset");
			return false;
		}
		off += pfe->offset;
	} else if (how == SEEK_END) {
		if (off < 0 || off >= pfh->size) {
			error("FastFileSeek: Invalid offset");
			return false;
		}
		off = (pfe + 1)->offset - off;
	} else if (how == SEEK_CUR) {
		off = pfh->pos + off;
		if (off < pfe->offset || off >= (pfe + 1)->offset) {
			error("FastFileSeek: Invalid offset");
			return false;
		}
	} else {
		error("FastFileSeek: Invalid 'how' field");
		return false;
	}

	pfh->pos = off;
	return true;
}

long FastFileTell(HFASTFILE ff) {
	PFileHandle pfh = (PFileHandle)ff;

	if (pfh == nullptr || pfh->inuse != true) {
		error("FastFileTell: invalid handle");
		return -1;
	}
	return pfh->pos - pfh->pfe->offset;

} /* FastFileTell */

} // namespace Dink
