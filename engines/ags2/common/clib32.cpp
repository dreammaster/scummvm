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

#define NATIVESTATIC

#include "common/stream.h"
#include "common/textconsole.h"
#include "ags2/common/clib32.h"
#include "ags2/common/misc.h"

namespace AGS2 {

#define CLIB_IS_INSTALLED

int32 last_opened_size;
char lib_file_name[255] = " ";
char base_path[255] = ".";
char original_base_filename[255];
char clbuff[20];
const int RAND_SEED_SALT = 9338638;
#define MAX_FILES 10000
#define MAXMULTIFILES 25


struct MultiFileLib {
	char data_filenames[MAXMULTIFILES][20];
	int num_data_files;
	char filenames[MAX_FILES][25];
	int32 offset[MAX_FILES];
	int32 length[MAX_FILES];
	char file_datafile[MAX_FILES];        // number of datafile
	int num_files;
};

struct MultiFileLibNew {
	char data_filenames[MAXMULTIFILES][50];
	int num_data_files;
	char filenames[MAX_FILES][100];
	int32 offset[MAX_FILES];
	int32 length[MAX_FILES];
	char file_datafile[MAX_FILES];        // number of datafile
	int num_files;
};

MultiFileLibNew mflib;
static const char *clibendfilesig = "CLIB\x1\x2\x3\x4SIGE";
static const char *clibpasswencstring = "My\x1\xde\x4Jibzle";
static int _last_rand;

void init_pseudo_rand_gen(int seed) {
	_last_rand = seed;
}

int get_pseudo_rand() {
	return(((_last_rand = _last_rand * 214013L + 2531011L) >> 16) & 0x7fff);
}

void clib_decrypt_text(char *toenc) {
	int adx = 0;

	while (1) {
		toenc[0] -= clibpasswencstring[adx];
		if (toenc[0] == 0)
			break;

		adx++;
		toenc++;

		if (adx > 10)
			adx = 0;
	}
}

void fgetnulltermstring(char *sss, Common::ReadStream *ddd, int bufsize) {
	int b = -1;
	do {
		if (b < bufsize - 1)
			b++;
		sss[b] = (char)ddd->readByte();
		if (ddd->eos())
			return;
	} while (sss[b] != 0);
}

template<typename T>
void fread_data_enc(T *data, int dataCount, Common::ReadStream *ooo) {
	error("Unsupported field size");
}

template<> void fread_data_enc<char>(char *data, int dataCount, Common::ReadStream *ooo) {
	ooo->read(data, sizeof(char) * dataCount);

	// Decrypt the data
	byte *dataChar = (byte *)data;
	for (uint i = 0; i < sizeof(char) * dataCount; i++, dataChar++)
		*dataChar -= get_pseudo_rand();
}

template<> void fread_data_enc<int32>(int32 *data, int dataCount, Common::ReadStream *ooo) {
	// Read the data
	byte *buf = new byte[sizeof(int32) * dataCount];
	ooo->read(buf, sizeof(int32) * dataCount);

	// Decrypt the data
	byte *dataChar = buf;
	for (uint i = 0; i < sizeof(int32) * dataCount; i++) {
		dataChar[i] -= get_pseudo_rand();
	}

	// Now copy to the passed data pointer
	const int32 *src = (const int32 *)src;
	int32 *dest = data;
	for (; dataCount > 0; ++src, ++dest, --dataCount)
		*dest = READ_LE_UINT32(src);

	delete[] buf;
}

void fgetstring_enc(char *sss, Common::ReadStream *ooo, int maxLength) {
	int i = 0;
	while ((i == 0) || (sss[i - 1] != 0)) {
		sss[i] = ooo->readByte() - get_pseudo_rand();

		if (i < maxLength - 1)
			i++;
	}
}

int getw_enc(Common::ReadStream *ooo) {
	return ooo->readUint32LE();
}

int read_new_new_enc_format_clib(MultiFileLibNew *mfl, Common::ReadStream *wout, int libver) {
	int aa;
	int randSeed = getw_enc(wout);

	init_pseudo_rand_gen(randSeed + RAND_SEED_SALT);
	mfl->num_data_files = getw_enc(wout);
	for (aa = 0; aa < mfl->num_data_files; aa++) {
		fgetstring_enc(mfl->data_filenames[aa], wout, 50);
	}

	mfl->num_files = getw_enc(wout);

	if (mfl->num_files > MAX_FILES)
		return -1;

	for (aa = 0; aa < mfl->num_files; aa++) {
		fgetstring_enc(mfl->filenames[aa], wout, 100);
	}

	fread_data_enc<int32>(&mfl->offset[0], mfl->num_files, wout);
	fread_data_enc<int32>(&mfl->length[0], mfl->num_files, wout);
	fread_data_enc<char>(&mfl->file_datafile[0], mfl->num_files, wout);
	return 0;
}

int read_new_new_format_clib(MultiFileLibNew *mfl, Common::ReadStream *wout, int libver) {
	int aa;
	mfl->num_data_files = wout->readUint32LE();
	for (aa = 0; aa < mfl->num_data_files; aa++) {
		fgetnulltermstring(mfl->data_filenames[aa], wout, 50);
	}

	mfl->num_files = wout->readUint32LE();
	if (mfl->num_files > MAX_FILES)
		return -1;

	for (aa = 0; aa < mfl->num_files; aa++) {
		short nameLength = wout->readUint16LE();
		nameLength /= 5;
		wout->read(mfl->filenames[aa], nameLength);
		clib_decrypt_text(mfl->filenames[aa]);
	}

	for (aa = 0; aa < mfl->num_files; ++aa)
		mfl->offset[aa] = wout->readSint32LE();
	for (aa = 0; aa < mfl->num_files; ++aa)
		mfl->length[aa] = wout->readSint32LE();
	wout->read(&mfl->file_datafile[0], mfl->num_files);

	return 0;
}

int read_new_format_clib(MultiFileLib *mfl, Common::ReadStream *wout, int libver) {
	int ee;

	mfl->num_data_files = wout->readUint32LE();
	wout->read(&mfl->data_filenames[0][0], 20 * mfl->num_data_files);
	mfl->num_files = wout->readUint32LE();

	if (mfl->num_files > MAX_FILES)
		return -1;

	wout->read(&mfl->filenames[0][0], 25 * mfl->num_files);
	for (ee = 0; ee < mfl->num_files; ++ee)
		mfl->offset[ee] = wout->readSint32LE();
	for (ee = 0; ee < mfl->num_files; ++ee)
		mfl->length[ee] = wout->readSint32LE();
	wout->read(&mfl->file_datafile[0], mfl->num_files);

	if (libver >= 11) {
		int aa;
		for (aa = 0; aa < mfl->num_files; aa++)
			clib_decrypt_text(mfl->filenames[aa]);
	}

	return 0;
}

int csetlib(char *namm, char *passw) {
	original_base_filename[0] = 0;

	if (namm == nullptr) {
		lib_file_name[0] = ' ';
		lib_file_name[1] = 0;
		return 0;
	}
	Common::strcpy_s(base_path, ".");

	int passwmodifier = 0, cc, aa;
	Common::SeekableReadStream *fff = ci_fopen(namm, "rb");
	if (fff == nullptr)
		return -1;

	int32 absoffs = 0;
	fff->read(&clbuff[0], 5);

	if (strncmp(clbuff, "CLIB", 4) != 0) {
		fff->seek(-12, SEEK_END);
		fff->read(&clbuff[0], 12);

		if (strncmp(clbuff, clibendfilesig, 12) != 0)
			return -2;

		fff->seek(-16, SEEK_END);  // it's an appended-to-end-of-exe thing
		absoffs = fff->readUint32LE();
		fff->seek(absoffs + 5, SEEK_SET);
	}

	int lib_version = fff->readByte();
	if ((lib_version != 6) && (lib_version != 10) &&
		(lib_version != 11) && (lib_version != 15) &&
		(lib_version != 20) && (lib_version != 21))
		return -3;  // unsupported version

	char *nammwas = namm;
	// remove slashes so that the lib name fits in the buffer
	while ((strchr(namm, '\\') != nullptr) || (strchr(namm, '/') != nullptr))
		namm++;

	if (namm != nammwas) {
		// store complete path
		Common::strcpy_s(base_path, nammwas);
		base_path[namm - nammwas] = 0;
		if ((base_path[strlen(base_path) - 1] == '\\') || (base_path[strlen(base_path) - 1] == '/'))
			base_path[strlen(base_path) - 1] = 0;
	}

	if (lib_version >= 10) {
		if (fff->readByte() != 0)
			return -4;  // not first datafile in chain

		if (lib_version >= 21) {
			if (read_new_new_enc_format_clib(&mflib, fff, lib_version))
				return -5;
		} else if (lib_version == 20) {
			if (read_new_new_format_clib(&mflib, fff, lib_version))
				return -5;
		} else {
			MultiFileLib mflibOld;
			if (read_new_format_clib(&mflibOld, fff, lib_version))
				return -5;
			// convert to newer format
			mflib.num_files = mflibOld.num_files;
			mflib.num_data_files = mflibOld.num_data_files;
			memcpy(&mflib.offset[0], &mflibOld.offset[0], sizeof(long) * mflib.num_files);
			memcpy(&mflib.length[0], &mflibOld.length[0], sizeof(long) * mflib.num_files);
			memcpy(&mflib.file_datafile[0], &mflibOld.file_datafile[0], sizeof(char) * mflib.num_files);
			for (aa = 0; aa < mflib.num_data_files; aa++)
				Common::strcpy_s(mflib.data_filenames[aa], mflibOld.data_filenames[aa]);
			for (aa = 0; aa < mflib.num_files; aa++)
				Common::strcpy_s(mflib.filenames[aa], mflibOld.filenames[aa]);
		}

		delete fff;
		Common::strcpy_s(lib_file_name, namm);

		// make a backup of the original file name
		Common::strcpy_s(original_base_filename, mflib.data_filenames[0]);
		ags_strlwr(original_base_filename);

		Common::strcpy_s(mflib.data_filenames[0], namm);
		for (aa = 0; aa < mflib.num_files; aa++) {
			// correct offsetes for EXE file
			if (mflib.file_datafile[aa] == 0)
				mflib.offset[aa] += absoffs;
		}
		return 0;
	}

	passwmodifier = fff->readByte();
	fff->readByte(); // unused byte
	mflib.num_data_files = 1;
	Common::strcpy_s(mflib.data_filenames[0], namm);

	mflib.num_files = fff->readUint16LE();

	if (mflib.num_files > MAX_FILES)
		return -4;

	fff->read(clbuff, 13);  // skip password dooberry
	for (aa = 0; aa < mflib.num_files; aa++) {
		fff->read(&mflib.filenames[aa][0], 13);
		for (cc = 0; cc < (int)strlen(mflib.filenames[aa]); cc++)
			mflib.filenames[aa][cc] -= passwmodifier;
	}

	for (int ee = 0; ee < mflib.num_files; ++ee)
		mflib.length[ee] = fff->readUint32LE();
	fff->seek(2 * mflib.num_files, SEEK_CUR);  // skip flags & ratio

	mflib.offset[0] = fff->pos();
	Common::strcpy_s(lib_file_name, namm);
	delete fff;

	for (aa = 1; aa < mflib.num_files; aa++) {
		mflib.offset[aa] = mflib.offset[aa - 1] + mflib.length[aa - 1];
		mflib.file_datafile[aa] = 0;
	}

	mflib.file_datafile[0] = 0;
	return 0;
}

int clibGetNumFiles() {
	if (lib_file_name[0] == ' ')
		return 0;
	return mflib.num_files;
}

const char *clibGetFileName(int index) {
	if (lib_file_name[0] == ' ')
		return nullptr;

	if ((index < 0) || (index >= mflib.num_files))
		return nullptr;

	return &mflib.filenames[index][0];
}

int clibfindindex(char *fill) {
	if (lib_file_name[0] == ' ')
		return -1;

	int bb;
	for (bb = 0; bb < mflib.num_files; bb++) {
		if (scumm_stricmp(mflib.filenames[bb], fill) == 0)
			return bb;
	}
	return -1;
}

int32 clibfilesize(char *fill) {
	int idxx = clibfindindex(fill);
	if (idxx >= 0)
		return mflib.length[idxx];
	return -1;
}

int32 cliboffset(char *fill) {
	int idxx = clibfindindex(fill);
	if (idxx >= 0)
		return mflib.offset[idxx];
	return -1;
}

const char *clibgetoriginalfilename() {
	return original_base_filename;
}

char actfilename[250];
char *clibgetdatafile(char *fill) {
	int idxx = clibfindindex(fill);
	if (idxx >= 0) {
#if defined(LINUX_VERSION) || defined(MAC_VERSION) 
		Common::sprintf_s(actfilename, "%s/%s", base_path, mflib.data_filenames[mflib.file_datafile[idxx]]);
#else
		Common::sprintf_s(actfilename, "%s\\%s", base_path, mflib.data_filenames[mflib.file_datafile[idxx]]);
#endif
		return &actfilename[0];
	}
	return nullptr;
}

Common::SeekableReadStream *tfil;
Common::SeekableReadStream *clibopenfile(char *filly, char *readmode) {
	int bb;
	for (bb = 0; bb < mflib.num_files; bb++) {
		if (scumm_stricmp(mflib.filenames[bb], filly) == 0) {
			char filename[250];
			Common::sprintf_s(filename, "%s\\%s", base_path, mflib.data_filenames[mflib.file_datafile[bb]]);
			tfil = ci_fopen(filename, readmode);
			if (tfil == nullptr)
				return nullptr;
			tfil->seek(mflib.offset[bb], SEEK_SET);
			return tfil;
		}
	}

	return ci_fopen(filly, readmode);
}

#define PR_DATAFIRST 1
#define PR_FILEFIRST 2
int cfopenpriority = PR_DATAFIRST;

Common::ReadStream *clibfopen(char *filnamm, char *fmt) {
	last_opened_size = -1;
	if (cfopenpriority == PR_FILEFIRST) {
		// check for file, otherwise use datafile
		if (fmt[0] != 'r') {
			tfil = ci_fopen(filnamm, fmt);
		} else {
			tfil = ci_fopen(filnamm, fmt);

			if ((tfil == nullptr) && (lib_file_name[0] != ' ')) {
				tfil = clibopenfile(filnamm, fmt);
				last_opened_size = clibfilesize(filnamm);
			}
		}

	} else {
		// check datafile first, then scan directory
		if ((cliboffset(filnamm) < 1) || (fmt[0] != 'r'))
			tfil = ci_fopen(filnamm, fmt);
		else {
			tfil = clibopenfile(filnamm, fmt);
			last_opened_size = clibfilesize(filnamm);
		}

	}

	if ((last_opened_size < 0) && (tfil != nullptr))
		last_opened_size = tfil->size();

	return tfil;
}

} // namespace AGS2
