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

#include "common/str.h"
#include "common/textconsole.h"
#include "spycraft/dmade/advlib.h"
#include "spycraft/dmade/advmain.h"
#include "spycraft/dmade/advres.h"
#include "spycraft/dmade/advscreen.h"
#include "spycraft/dmade/advmsg.h"
#include "spycraft/dmade/tgapic.h"
#include "spycraft/dmade/ats.h"
#include "spycraft/dmade/advsound.h"
#include "spycraft/dmade/advmusic.h"
#include "spycraft/dmade/advmem.h"
#include "spycraft/dmade/advvols.h"
#include "spycraft/dmade/advini.h"
#include "spycraft/dmade/advtime.h"
#include "spycraft/dmade/pkware.h"
#include "spycraft/dmade/advdcmp.h"
#include "spycraft/game/dispatch.h"

namespace Spycraft {

#define MAX_BUFFER_SIZE 		1024
#define MAX_VOLS_FILE   		4096
#define MAX_VOL_NAME_SIZE 		128
#define MAX_VOL_PATH_SIZE 		128

int sizeHTM = 0;

ResInfo resInfo;
static char resourceDir[MAX_VOL_PATH_SIZE];
char sysDir[MAX_VOL_PATH_SIZE];
char viewDir[MAX_VOL_PATH_SIZE];
char picDir[MAX_VOL_PATH_SIZE];
char soundDir[MAX_VOL_PATH_SIZE];
char fontDir[MAX_VOL_PATH_SIZE];
char movieDir[MAX_VOL_PATH_SIZE];
char htmDir[MAX_VOL_PATH_SIZE];
char faceDir[MAX_VOL_PATH_SIZE];
static char patchDir[] = "\\resource";

char ATSEXT[] = ".ats";
char WAVEXT[] = ".wav";
char HTMEXT[] = ".htm";
char MIDEXT[] = ".mid";
char PICEXT[] = ".tga";
char FONEXT[] = ".msg";
char FACEEXT[] = ".fnt";
static char MAP_FILE[] = "resource.map";
static int curDisc = 0;
bool cacheValide = false;
ResCache *atsCache = NULL;
ResCache *picCache = NULL;
ResCache *wavCache = NULL;
ResCache *midCache = NULL;
ResCache *fonCache = NULL;
ResCache *htmCache = NULL;
ResCache *faceCache = NULL;
static ArrayList *mapList = NULL;
static ArrayList *volList = NULL;
static ArrayList *streamList = NULL;
static HANDLE discHandler = nullptr;
static char INI_FILE[] = "resource.ini";
static char KEYWORD_DRIVE[] = "[RESOURCE_DRIVE]";
static char KEYWORD_CD[] = "[CD";

extern int __mem_index;

typedef struct {
	int picSize;
	int atsSize;
	int wavSize;
	int midSize;
	int fonSize;
	int faceSize;
} ResourceSize;

static ResourceSize resSize = { 2, 48, 8, 2, 2, 4 };

#define GetLogicalDrives() (4)
#define DRIVE_CDROM 1
#define GetDriveType(DRIVE) (DRIVE_CDROM)
#define GetCurrentDirectory(SIZE, BUFF) *BUFF = '\0'

static char *sc_strupr(char *str) {
	for (char *s = str; *s; ++s)
		*s = toupper(*s);
	return str;
}

static int strncmpl(char *str1, char *str2, int n) {
	return (strncmp(sc_strupr(str1), sc_strupr(str2), n));
}


void GetCDROM(char *path) {
	int i;
	char drive[8];
	int mask = GetLogicalDrives();

	for (i = 3; i < 26; i++) {
		if (mask & (0x1 << i)) {
			drive[0] = 'A' + i;
			drive[1] = ':';
			drive[2] = '\0';
			if (GetDriveType(drive) == DRIVE_CDROM) {
				Common::strcpy_s(path, 256, drive);
#ifdef _FROMD
				/***************FOR TEST FROM D ONLY!!!**************************/
#pragma message("_FROMD: LINE 99 -- MUST BE REMOVED IN GTJ VERSION!!!" )
				Common::strcpy_s(path, "D:");
#ifdef _DEBUG
#pragma message ("_DEBUG\n")
				DebugBreak();
#endif
				/***************FOR TEST FROM D ONLY!!!**************************/
#endif
				return;
			}
			/*
			if ( GetDriveType ( drive ) == DRIVE_CDROM )	{
				int l;
				char label[128];
				if ( GetVolumeInformation ( drive,	label, 128, NULL, &l, 0, NULL, 0 ) );
					dbgmsg ( "volume label is %s", label );
			}
			*/
		}
	}
}

void sfxSetCacheSize(int type, int value) {
	switch (type) {
	case RES_PIC:
		resSize.picSize = value;
		break;

	case RES_ATS:
		resSize.atsSize = value;
		break;

	case RES_WAVE:
		resSize.wavSize = value;
		break;

	case RES_MIDI:
		resSize.midSize = value;
		break;

	case RES_FONT:
		resSize.fonSize = value;
		break;

	case RES_FACE:
		resSize.faceSize = value;
		break;
	}
}

static void GetResPath(char *dest, char *src) {
	int i = 0;
	char *p = src;

	while (*p) {
		if (*p == '=') {
			p++;
			break;
		}
		p++;
	}
	while (*p) {
		if (*p != ' ')
			break;
		else
			p++;
	}

	while (*p) {
		if (Common::isAlpha(*p) || (*p == '.') ||
			(*p == '\\') || (*p == ':')
			)
		{
			dest[i] = *p;
			i++;
		}
		p++;

	}
	dest[i] = '\0';
}

#ifndef _DVD
#pragma message ("not _DVD:  Line 190.\n")
void PromptDisk(int location) {
	MADEEventStamp evt;

	evt.get_event_message = EVENT_DISKINSERT;
	evt.message = location + 1;		// for user ( 0 to 1, 1 to 2, etc. )

	/* CLOSE PREVIOUS FILE */
	if (discHandler)
		sfxCloseFile(discHandler);

	OnDiskInsert(&evt);
}
#endif

static int ReadMapFile() {
	int i, n;
	HANDLE hf;
	FileChunk *fk;
	char *buffer, *p;
	int size;

	size = sfxFileSize(MAP_FILE);

	if (!size)
		return false;

	buffer = (char *)AllocPtr(size);
	ADV_ASSERT(buffer, __ERR_MEM_ALLOC_FAIL);

	hf = sfxOpenFile(MAP_FILE, MADE_FILE_READ);

	if (hf) {
		sfxReadFile(hf, buffer, size);

		p = buffer;
		n = *(int *)p;
		p += sizeof(int);
		if (mapList == NULL) {
			mapList = ArrayList_Calloc(n);
			ADV_ASSERT(mapList, __ERR_MEM_ALLOC_FAIL);
		}

		for (i = 0; i < n; i++) {
			fk = (FileChunk *)AllocPtr(sizeof(FileChunk));
			ADV_ASSERT(fk, __ERR_MEM_ALLOC_FAIL);
			if (fk) {
				/* USE 4 READS INSTEAD OF CHANGE STRUCTURE PACK */
				fk->vid = *(int *)p;
				p += sizeof(int);
				fk->offset = *(int *)p;
				p += sizeof(int);
				fk->compressor = *(unsigned char *)p;
				p += sizeof(unsigned char);
				fk->location = *(unsigned char *)p;
				p += sizeof(unsigned char);
				ArrayList_Add(mapList, fk, NULL);
			}
		}

		FreePtr(buffer);
		sfxCloseFile(hf);
		return true;
	}

	return false;
}

static char *StrDup(char *str) {
	char *ret = (char *)AllocPtr(strlen(str) + 1);
	if (ret == NULL)
		return nullptr;

	Common::strcpy_s(ret, 256, str);
	return (ret);
}

char *GetResourceDir(char *resourceLine) {
	char *p = resourceLine;

	while (*p) {
		if (*p == '=') {
			p++;
			break;
		}
		p++;
	}

	while (*p) {
		if (*p != ' ')
			return (p);
		else
			p++;
	}

	return NULL;
}

char *StrDupPath(char *volName) {
	int len;
	char *buffer;

	len = strlen(volName) + strlen(resourceDir) + 4;
	buffer = (char *)AllocPtr(len);
	ADV_ASSERT(buffer, __ERR_MEM_ALLOC_FAIL);
	Common::strcpy_s(buffer, 256, resourceDir);
	Common::strcat_s(buffer, 256, "\\");
	Common::strcat_s(buffer, 256, volName);
	return buffer;
}

static void ReadVols() {
	int i, n;
	char *ptr;
	int isItVols;
	char volName[MAX_VOL_NAME_SIZE];
#ifdef _NOTROOT
	char CDROMname[3];
#endif
	HANDLE hi = sfxOpenFile(INI_FILE, MADE_FILE_READ);

	/* Get resource directory from RESOURCE.INI */
	if ( ( n = sfxGetString ( hi, volName ) ) == -1 )	{
		ErrMsg ( "Resource.ini is corrupt please reinstall Spycraft.");
		ADV_ASSERT ( false, __ERR_RES_NOT_FOUND );
	}

	if (scumm_stricmp( KEYWORD_DRIVE, volName ) )
		ADV_ASSERT ( false, __ERR_CODING );

	if ( ( n = sfxGetString ( hi, volName ) ) == -1 )	{
		ErrMsg ( "Resource.ini is corrupt please reinstall Spycraft.");
		ADV_ASSERT ( false, __ERR_RES_NOT_FOUND );
	}

	if ( ( ptr = (char *)GetResourceDir ( volName ) ) ) {
		Common::strcpy_s ( resourceDir, ptr );
	}
	
	if ( GetDriveType ( resourceDir ) != DRIVE_CDROM )
#ifdef _NOTROOT
#pragma message ("_NOTROOT\n")
	 {
	  GetCDROM ( CDROMname );
	  resourceDir[0]=CDROMname[0];
	 }
#else
#pragma message ("not _NOTROOT\n")
	  GetCDROM ( resourceDir );
#endif
#ifdef _TERRY
/*****************debug*******************/
#pragma message ("_TERRY:  Lines 355-365 are debug code.\n")
	fiel=fopen("c:\\junk.jnk","w");
/*****************debug*******************/
	fprintf(fiel,"volName||%s||\n",volName);
	fprintf(fiel,"resourceDir||%s||\n",resourceDir);
	fclose (fiel);
#ifdef _DEBUG
#pragma message ("_DEBUG\n")
	DebugBreak();
#endif
/*****************debug*******************/
#endif
	volList = ArrayList_Calloc ( MAX_DISC_LIMIT ) ;
	ADV_ASSERT ( volList, __ERR_MEM_ALLOC_FAIL );

	streamList = ArrayList_Calloc(MAX_STREAM_LIMIT);
	ADV_ASSERT ( streamList, __ERR_MEM_ALLOC_FAIL );

	i = -1;
	isItVols = false;
	while ( true ) {
		if ( ( i >= MAX_DISC_LIMIT) || ( n = sfxGetString ( hi, volName ) ) == -1 ) {
			sfxCloseFile(hi);
			break;
		}
		else {
			/* CD Keyword */
			if ( !strncmp ( sc_strupr ( volName ), KEYWORD_CD, 3 ) ) {
				isItVols = true;
				i ++;
			}
			/* FILE OR EXTRA FILE ON CD */
			else {
				if ( isItVols ) {
					isItVols = false;
					ArrayList_Add ( volList, StrDupPath ( volName ), NULL );
				}
				else {
					VolInfo *info = (VolInfo *)AllocPtr ( sizeof ( VolInfo ) );

					if ( info == NULL )
						ADV_ASSERT ( false, __ERR_MEM_ALLOC_FAIL );

					info->filename = StrDupPath ( volName );
					info->location = i;
					ArrayList_Add ( streamList, info, NULL );
				}
			}
		}
	}
}

int OpenVols ( int dn )
{
	unsigned char uc;

	discHandler = sfxOpenFile ( (char *)volList->elements[dn], MADE_FILE_READ );

	/* VALIDATE THE DISK */
	if (!discHandler)
		return false;

	sfxReadFile ( discHandler, &uc, sizeof ( unsigned char ) );
	if ( uc != dn )
		return false;

	return true;
}

void ReadWhere() {
	int n;
	HANDLE hf;
	char buffer[128];

	Common::strcpy_s(buffer, sysDir);
	Common::strcat_s(buffer, "\\where");
	hf = sfxOpenFile(buffer, MADE_FILE_READ);

	/* FIND DEFAULT WHERE */
	if (!hf) {
		Common::strcpy_s(picDir, sysDir);
		Common::strcpy_s(viewDir, sysDir);
		Common::strcpy_s(soundDir, sysDir);
		Common::strcpy_s(fontDir, sysDir);
		Common::strcpy_s(movieDir, sysDir);
		Common::strcpy_s(htmDir, sysDir);
		Common::strcpy_s(faceDir, sysDir);
		return;
	}

	do {
		n = sfxGetString(hf, buffer);

		if (n != -1) {
			sc_strupr(buffer);

			if (strstr(buffer, "PIC"))
				GetResPath(picDir, buffer);
			else if (strstr(buffer, "VIEW"))
				GetResPath(viewDir, buffer);
			else if (strstr(buffer, "SOUND"))
				GetResPath(soundDir, buffer);
			else if (strstr(buffer, "TEXT"))
				GetResPath(fontDir, buffer);
			else if (strstr(buffer, "AVI"))
				GetResPath(movieDir, buffer);
			else if (strstr(buffer, "HTM"))
				GetResPath(htmDir, buffer);
			else if (strstr(buffer, "FACE"))
				GetResPath(faceDir, buffer);
		}
	} while (n != -1);

	sfxCloseFile(hf);
}

int sfxGetDir(char *dest, int id, int type) {
	char typeString[8];

	switch (type) {
	case RES_WAVE:
		Common::strcpy_s(typeString, ".WAV");
		if (mapList) {
			Common::strcpy_s(dest, 256, resourceDir);
		} else {
			Common::strcpy_s(dest, 256, soundDir);
		}
		break;

	case RES_MOVIE:
		Common::strcpy_s(typeString, ".AVI");
		if (mapList) {
			Common::strcpy_s(dest, 256, resourceDir);
		} else {
			Common::strcpy_s(dest, 256, movieDir);
		}
		break;

	case RES_HTM:
		Common::strcpy_s(typeString, ".HTM");
		if (mapList) {
			Common::strcpy_s(dest, 256, resourceDir);
		} else {
			Common::strcpy_s(dest, 256, htmDir);
		}
		break;

	}

	Common::String suffix = Common::String::format("\\%d%s", id, typeString);
	Common::strcat_s(dest, 256, suffix.c_str());

	if (mapList) {
		int i;
		VolInfo *info = nullptr, *tinfo;
		for (i = 0, tinfo = NULL; i < streamList->size; i++) {
			info = (VolInfo *)streamList->elements[i];
			if (strcmp(dest, info->filename) == 0) {
				if (info->location == curDisc)
					break;
				else
					tinfo = info;
			}
		}
		if (i == streamList->size) {
			if (tinfo == NULL) {
				ErrMsg("Can't find %s, please reinstall Spycraft.", dest);
				ADV_ASSERT(false, __ERR_RES_NOT_FOUND);
			}
		}
		// found only on another disc
		if ((tinfo != NULL) && (strcmp(dest, info->filename) != 0))
			info = tinfo;

#ifndef _DVD
#pragma message ("not _DVD:  Line 532.\n")
		/* CHECK VALID DISC LOCATION */
		if (info->location != curDisc) {
			do {
				PromptDisk(info->location);
			} while (!OpenVols(info->location));
			curDisc = info->location;
			return true;
		} else {
			return true;
		}
#else
#pragma message ("_DVD:  Line 546.\n")
		return true;
#endif
	} else {
		return true;
	}
}

void SetRESCache()
{
   int i;

	/* GET THE CURDIR */
	GetCurrentDirectory ( MAX_VOL_PATH_SIZE, sysDir ); 

	/* CHECK WITH INI FILE */
	if ( ReadMapFile() ) {
		ReadVols();
      curDisc = -1;
      for ( i=0; i<3; i++ ) {
         if ( OpenVols ( i ) ) {
            curDisc = i;
            break;
         }
      }
      if ( curDisc == -1 ) {
         curDisc = 0;
#ifndef _DVD
#pragma message ("not _DVD:  Line 567.\n")
		 while ( !OpenVols ( 0 ) ) {
		   PromptDisk ( 0 );
         }
#endif
		}
	}
	else
		ReadWhere();

	/* SET GLOBAL CACHE */
	cacheValide = true;

	/* SET INITIAL RESINFO */
	resInfo.res_pic_size = resSize.picSize * __mem_index;
	resInfo.res_ats_size = resSize.atsSize + __mem_index * 2;
	resInfo.res_wav_size = resSize.wavSize + __mem_index;
	resInfo.res_mid_size = resSize.midSize * __mem_index;
	resInfo.res_fon_size = resSize.fonSize * __mem_index;
	resInfo.res_face_size = resSize.faceSize * __mem_index;

	/* RES_ATS */
	atsCache = (ResCache *)AllocPtr ( sizeof ( ResCache ) );
	ADV_ASSERT ( atsCache, __ERR_MEM_ALLOC_FAIL );
	atsCache->list = ArrayList_Calloc ( resInfo.res_ats_size );
	ADV_ASSERT ( atsCache->list, __ERR_MEM_ALLOC_FAIL );
	atsCache->size = 0;

	/* RES_PIC */
	picCache = (ResCache *)AllocPtr ( sizeof ( ResCache ) );
	ADV_ASSERT ( picCache, __ERR_MEM_ALLOC_FAIL );
	picCache->list = ArrayList_Calloc ( resInfo.res_pic_size );
	ADV_ASSERT ( picCache->list, __ERR_MEM_ALLOC_FAIL );
	picCache->size = 0;

	/* RES_WAVE */
	wavCache = (ResCache *)AllocPtr ( sizeof ( ResCache ) );
	ADV_ASSERT ( wavCache, __ERR_MEM_ALLOC_FAIL );
	wavCache->list = ArrayList_Calloc ( resInfo.res_wav_size );
	ADV_ASSERT ( wavCache->list, __ERR_MEM_ALLOC_FAIL );
	wavCache->size = 0;

	/* RES_HTM */
	htmCache = (ResCache *)AllocPtr ( sizeof ( ResCache ) );
	ADV_ASSERT ( htmCache, __ERR_MEM_ALLOC_FAIL );
	htmCache->list = ArrayList_Alloc();
	ADV_ASSERT ( htmCache->list, __ERR_MEM_ALLOC_FAIL );
	htmCache->size = 0;

	/* RES_MID */
	midCache = (ResCache *)AllocPtr ( sizeof ( ResCache ) );
	ADV_ASSERT ( midCache, __ERR_MEM_ALLOC_FAIL );
	midCache->list = ArrayList_Calloc ( resInfo.res_mid_size );
	ADV_ASSERT ( midCache->list, __ERR_MEM_ALLOC_FAIL );
	midCache->size = 0;

	/* RES_FONT */
	fonCache = (ResCache *)AllocPtr ( sizeof ( ResCache ) );
	ADV_ASSERT ( fonCache, __ERR_MEM_ALLOC_FAIL );
	fonCache->list = ArrayList_Calloc ( resInfo.res_fon_size );
	ADV_ASSERT ( fonCache->list, __ERR_MEM_ALLOC_FAIL );
	fonCache->size = 0;

	/* RES_FACE */
	faceCache = (ResCache *)AllocPtr ( sizeof ( ResCache ) );
	ADV_ASSERT ( faceCache, __ERR_MEM_ALLOC_FAIL );
	faceCache->list = ArrayList_Calloc ( resInfo.res_face_size );
	ADV_ASSERT ( faceCache->list, __ERR_MEM_ALLOC_FAIL );
	faceCache->size = 0;
}

static void FreePICNode ( GenericData *pNode )
{
	FreePort ( ( (Viewport*)pNode->data ) );
	FreePtr ( pNode );
}

static void FreeATSNode ( GenericData *pNode )
{
	DestroyATS ( ( (ATS *)pNode->data ) );
	FreePtr ( pNode );
}

static void FreeMSGNode ( GenericData *pNode )
{
	FreeMSG ( (( MSGTag *)pNode->data ) );
	FreePtr ( pNode );
}

static void FreeWaveNode ( GenericData *pNode )
{
	FreeWave ( pNode->data );
	FreePtr ( pNode );
}

static void FreeMidiNode ( GenericData *pNode )
{
	FreeMidi ( pNode->data );
	FreePtr ( pNode );
}

static void FreeHTMNode ( GenericData *pNode )
{
	FreePtr ( pNode->data );
	FreePtr ( pNode );
}

static void FreeFaceNode ( GenericData *pNode )
{
	FreePtr ( pNode->data );
	FreePtr ( pNode );
}

static void FreeVolInfo ( VolInfo *data )
{
	FreePtr ( data->filename );
	FreePtr ( data );
}

void DestroyRESCache() {
	/* SET GLOBAL CACHE */
	cacheValide = false;

	/* CLOSE VOL FILE */
	if (discHandler)
		sfxCloseFile(discHandler);

	/* FREE THE MAP ARRAY */
	if (mapList) {
		ArrayList_Free(mapList, NULL);
		mapList = NULL;
		if (discHandler) {
			sfxCloseFile(discHandler);
			discHandler = nullptr;
		}
	}

	/* FREE THE VOL ARRAY */
	if (volList) {
		ArrayList_Free(volList, NULL);
		volList = NULL;
	}

	if (streamList) {
		ArrayList_Free(streamList, (FreeFnPtr)FreeVolInfo);
		streamList = NULL;
	}

	/* RES_PIC */
	if (picCache) {
		if (picCache->list) {
			ArrayList_Free(picCache->list, (FreeFnPtr)FreePICNode);
			picCache->list = NULL;
		}
		FreePtr(picCache);
		picCache = NULL;
	}

	/* RES_ATS */
	if (atsCache) {
		if (atsCache->list) {
			ArrayList_Free(atsCache->list, (FreeFnPtr)FreeATSNode);
			atsCache->list = NULL;
		}
		FreePtr(atsCache);
		atsCache = NULL;
	}

	/* RES_WAVE */
	if (wavCache) {
		if (wavCache->list) {
			ArrayList_Free(wavCache->list, (FreeFnPtr)FreeWaveNode);
			wavCache->list = NULL;
		}
		FreePtr(wavCache);
		wavCache = NULL;
	}

	/* RES_HTM */
	if (htmCache) {
		if (htmCache->list) {
			ArrayList_Free(htmCache->list, (FreeFnPtr)FreeHTMNode);
			htmCache->list = NULL;
		}
		FreePtr(htmCache);
		htmCache = NULL;
	}

	/* RES_MIDI */
	if (midCache) {
		if (midCache->list) {
			ArrayList_Free(midCache->list, (FreeFnPtr)FreeMidiNode);
			midCache->list = NULL;
		}
		FreePtr(midCache);
		midCache = NULL;
	}

	/* RES_FONT */
	if (fonCache) {
		if (fonCache->list) {
			ArrayList_Free(fonCache->list, (FreeFnPtr)FreeMSGNode);
			fonCache->list = NULL;
		}
		FreePtr(fonCache);
		fonCache = NULL;
	}

	/* RES_FACE */
	if (faceCache) {
		if (faceCache->list) {
			ArrayList_Free(faceCache->list, (FreeFnPtr)FreeFaceNode);
			faceCache->list = NULL;
		}
		FreePtr(faceCache);
		faceCache = NULL;
	}
}

GenericData *SearchRESData(int type, int id) {
	int i;
	ArrayList *list = NULL;
	GenericData *data;

	switch (type) {
	case RES_ATS:
		list = atsCache->list;
		break;

	case RES_PIC:
		list = picCache->list;
		break;

	case RES_WAVE:
		list = wavCache->list;
		break;

	case RES_MIDI:
		list = midCache->list;
		break;

	case RES_TEXT:
		list = fonCache->list;
		break;

	case RES_HTM:
		list = htmCache->list;
		break;

	case RES_FACE:
		list = faceCache->list;
		break;

	}

	ADV_ASSERT(list, __ERR_CODING);
	for (i = 0; i < list->size; i++) {
		data = (GenericData *)list->elements[i];
		if (data->id == id)
			return (data);
	}
	return nullptr;
}

void MakeFilename(char *dest, int src, char *theDir, char *ext) {
	Common::String fname = Common::String::format("%s\\%d%s", theDir, src, ext);
	Common::strcpy_s(dest, 256, fname.c_str());
}

void MakePatchFilename ( char *dest, int src, char *theDir, char *ext ) {
	Common::String fname = Common::String::format("%s%s\\%d%s", theDir, patchDir, src, ext);
	Common::strcpy_s(dest, 256, fname.c_str());
}

void ID2File(char *dest, int src, int type) {
	switch (type) {
	case RES_ATS:
		MakeFilename(dest, src, viewDir, ATSEXT);
		break;

	case RES_PIC:
		MakeFilename(dest, src, picDir, PICEXT);
		break;

	case RES_WAVE:
		MakeFilename(dest, src, soundDir, WAVEXT);
		break;

	case RES_MIDI:
		MakeFilename(dest, src, soundDir, MIDEXT);
		break;

	case RES_TEXT:
		MakeFilename(dest, src, fontDir, FONEXT);
		break;

	case RES_HTM:
		MakeFilename(dest, src, htmDir, HTMEXT);
		break;

	case RES_FACE:
		MakeFilename(dest, src, faceDir, FACEEXT);
		break;

	default:
		ADV_ASSERT(false, __ERR_CODING);
	}
}

void ID2PatchFile(char *dest, int src, int type) {
	switch (type) {
	case RES_ATS:
		MakePatchFilename(dest, src, sysDir, ATSEXT);
		break;

	case RES_PIC:
		MakePatchFilename(dest, src, sysDir, PICEXT);
		break;

	case RES_WAVE:
		MakePatchFilename(dest, src, sysDir, WAVEXT);
		break;

	case RES_MIDI:
		MakePatchFilename(dest, src, sysDir, MIDEXT);
		break;

	case RES_TEXT:
		MakePatchFilename(dest, src, sysDir, FONEXT);
		break;

	case RES_HTM:
		MakePatchFilename(dest, src, sysDir, HTMEXT);
		break;

	case RES_FACE:
		MakePatchFilename(dest, src, sysDir, FACEEXT);
		break;

	default:
		ADV_ASSERT(false, __ERR_CODING);
	}
}

void *OpenFace(void *src) {
	return src;
}

void *OpenFaceFile(char *filename, int *size) {
	HANDLE hf;
	void *ret = NULL;

	*size = sfxFileSize(filename);
	hf = sfxOpenFile(filename, MADE_FILE_READ);

	if (!hf)
		ADV_ASSERT(false, __ERR_FILE_OPEN_FAIL);

	ret = AllocPtr(*size);
	if (ret == NULL) {
		sfxCloseFile(hf);
		return nullptr;
	}

	sfxReadFile(hf, ret, *size);
	sfxCloseFile(hf);

	return (ret);
}

void *Decompress_NONE(int st, int *size) {
	int c_size;
	void *buffer;

	sfxSeekFile(discHandler, st, MADE_SEEK_BEG);

	sfxReadFile(discHandler, size, sizeof(int));
	sfxReadFile(discHandler, &c_size, sizeof(int));
	if (c_size != *size)
		ADV_ASSERT(false, __ERR_DECOMPRESS_SIZE_MISMATCH);

	buffer = AllocPtr(*size);
	if (buffer) {
		sfxReadFile(discHandler, buffer, *size);
	}

	return (buffer);
}

void *Decompress_PKWARE(int st, int *size) {
	int compSize;
	void *buffer, *src;

	sfxSeekFile(discHandler, st, MADE_SEEK_BEG);
	sfxReadFile(discHandler, &compSize, sizeof(int));
	sfxReadFile(discHandler, size, sizeof(int));

	src = AllocPtr(compSize);
	buffer = AllocPtr(*size);
	if (src && buffer) {
		sfxReadFile(discHandler, src, compSize);
		error("TODO: Uncompress_PKWARE(buffer, src, compSize, *size)");
		FreePtr(src);
	}

	return (buffer);
}

int ZDecompress ( char *in, char *out, int inLen, int origLen );

void *Decompress_ZLIB(int st, int *size) {
	int compSize;
	void *buffer, *src;

	sfxSeekFile(discHandler, st, MADE_SEEK_BEG);
	sfxReadFile(discHandler, &compSize, sizeof(int));
	sfxReadFile(discHandler, size, sizeof(int));

	src = AllocPtr(compSize);
	buffer = AllocPtr(*size);
	if (src && buffer) {
		sfxReadFile(discHandler, src, compSize);
		error("TODO: ZDecompress(src, buffer, compSize, *size)");
	}
	if (src != NULL)
		FreePtr(src);

	return (buffer);
}

static void *OpenHTM(void *src) {
	return (src);
}

static int IsFileExist(char *filename) {
	HANDLE hf = sfxOpenFile(filename, MADE_FILE_READ);
	if (hf) {
		sfxCloseFile(hf);
		return true;
	}
	return false;
}

static void *ReadRawFile(char *filename, int *size) {
	HANDLE hf;
	char *buffer;

	*size = sfxFileSize(filename);
	hf = sfxOpenFile(filename, MADE_FILE_READ);

	if (!hf) {
		ErrMsg("Can't open %s, please reinstall Spycraft.", filename);
		ADV_ASSERT(false, __ERR_FILE_OPEN_FAIL);
	}

	buffer = (char *)AllocPtr(*size);
	if (buffer == NULL) {
		sfxCloseFile(hf);
		return NULL;
	}
	sfxReadFile(hf, buffer, *size);

	sfxCloseFile(hf);

	return (buffer);
}

static void *VolLoad(int id, int type) {
	int vid, i, size = 0, compSize;
	FileChunk *fk = nullptr, *ffk;
	void *decomp = nullptr, *data = nullptr;
	char filename[MAX_VOL_NAME_SIZE];
	DcmpStream dStream;
	HANDLE hFile = 0;

	vid = type << 24 | id;

	dStream = NULL;
	/* CHECK THE PATCH DIRECTORY FIRST */
	ID2PatchFile(filename, id, type);
	if (IsFileExist(filename)) {
		if ((type != RES_ATS) && (type != RES_PIC)) {
			decomp = ReadRawFile(filename, &size);
			if (decomp == NULL)
				return NULL;
		} else {
			decomp = NULL;
			size = sfxFileSize(filename);
			hFile = sfxOpenFile(filename, MADE_FILE_READ);
			dStream = OpenDcmpStream(hFile, size, 0, COMPRESS_NONE);
			if (dStream == NULL) {
				sfxCloseFile(hFile);
				return NULL;
			}
		}
	} else {
		/* SEARCH FROM THE MAP */
		for (i = 0, ffk = NULL; i < mapList->size; i++) {
			fk = (FileChunk *)mapList->elements[i];
			if (fk->vid == vid) {
				if (fk->location == curDisc)
					break;
				else
					ffk = fk;
			}
		}
		if (i == mapList->size) {
			if (ffk == NULL) {
				ErrMsg("id = %d type = %d not found, please reinstall Spycraft.", (vid & 0xfffffff), (vid >> 24));
				ADV_ASSERT(false, __ERR_RES_NOT_FOUND);
			}
		}
		// found only on another disc
		if ((ffk != NULL) && (fk->vid != vid))
			fk = ffk;

		/* CHECK VALID DISC LOCATION */
		if (fk->location != curDisc) {
			do {
#ifndef _DVD
#pragma message ("not _DVD:  Line 1113.\n")
				PromptDisk(fk->location);
#endif
			} while (!OpenVols(fk->location));
			curDisc = fk->location;
		}
		if ((type != RES_ATS) && (type != RES_PIC)) {
			/* DECOMPRESS THE SOURCE IF NECESSARY */
			switch (fk->compressor) {
			case COMPRESS_NONE:
				decomp = Decompress_NONE(fk->offset, &size);
				break;

			case COMPRESS_PKWARE:
				decomp = Decompress_PKWARE(fk->offset, &size);
				break;

			case COMPRESS_ZLIB:
				decomp = Decompress_ZLIB(fk->offset, &size);
				break;

			default:
				ADV_ASSERT(false, __ERR_CODING);
				break;
			}
			if (decomp == NULL)
				return NULL;
		} else {
			hFile = nullptr;
			sfxSeekFile(discHandler, fk->offset, MADE_SEEK_BEG);
			sfxReadFile(discHandler, &compSize, sizeof(int));
			sfxReadFile(discHandler, &size, sizeof(int));
			dStream = OpenDcmpStream(discHandler, size, compSize, fk->compressor);
			if (dStream == NULL)
				return NULL;
		}
	}
	/* DECODE TO DATA AND FREE BUFFER */
	switch (type) {
	case RES_PIC:
		data = OpenPic(dStream);
		break;

	case RES_ATS:
		data = OpenATS(dStream);
		break;

	case RES_WAVE:
		data = OpenWave(decomp);
		break;

	case RES_MIDI:
		data = OpenMidi(decomp);
		break;

	case RES_TEXT:
		data = OpenMSG(decomp, size);
		break;

	case RES_HTM:
		sizeHTM = size;
		data = OpenHTM(decomp);
		break;

	case RES_FACE:
		data = OpenFace(decomp);
		break;

	default:
		ADV_ASSERT(false, __ERR_CODING);
		break;
	}
	if (dStream != NULL) {
		CloseDcmpStream(dStream);
		if (hFile)
			sfxCloseFile(hFile);
	}

	return data;
}

static void *FileLoad(int id, int type) {
	char filename[MAX_VOL_NAME_SIZE];
	void *data;
	int size;
	DcmpStream dStream;
	HANDLE hFile;

	data = NULL;

	/* TRY THE PATCH DIRECTORY FIRST */
	ID2PatchFile(filename, id, type);
	if (!IsFileExist(filename))
		ID2File(filename, id, type);
	switch (type) {
	case RES_ATS:
	case RES_PIC:
		size = sfxFileSize(filename);
		hFile = sfxOpenFile(filename, MADE_FILE_READ);
		dStream = OpenDcmpStream(hFile, size, 0, COMPRESS_NONE);
		if (dStream == NULL) {
			sfxCloseFile(hFile);
			return NULL;
		}

		if (type == RES_ATS)
			data = OpenATS(dStream);
		else
			data = OpenPic(dStream);	// used to have , scene_width, scene_height
		CloseDcmpStream(dStream);
		sfxCloseFile(hFile);
		break;

	case RES_TEXT:
		data = OpenMSGFile(filename);
		break;

	case RES_WAVE:
		data = OpenWaveFile(filename, &size);
		break;

	case RES_MIDI:
		data = OpenMidiFile(filename, &size);
		break;

	case RES_HTM:
		data = ReadRawFile(filename, &size);
		sizeHTM = size;
		break;

	case RES_FACE:
		data = OpenFaceFile(filename, &size);
		break;

	default:
		ADV_ASSERT(false, __ERR_CODING);
	}
	return data;
}

void *sfxLoadRes(int id, int type) {
	ArrayList *list = nullptr;
	void *data = nullptr;
	GenericData *node = nullptr;
	int tries;

	if (!cacheValide)
		return nullptr;

	/* SEE IF ALREADY LOADED IN CACHE */
	node = SearchRESData(type, id);
	if (node) {
		node->time = sfxGetTime();
		return (node->data);
	}

	/* SELECT CACHE LIST */
	switch (type) {
	case RES_ATS:
		list = atsCache->list;
		break;

	case RES_PIC:
		list = picCache->list;
		break;

	case RES_WAVE:
		list = wavCache->list;
		break;

	case RES_MIDI:
		list = midCache->list;
		break;

	case RES_TEXT:
		list = fonCache->list;
		break;

	case RES_HTM:
		list = htmCache->list;
		break;

	case RES_FACE:
		list = faceCache->list;
		break;

	}

	/* MAKE ROOM FOR RESOURCE */
	if (list->size == list->limit) {
		if (!sfxPurgeRes(-1, type)) {
			sfxPrintf("Reached cache list type: %d, limit: %d", type, list->limit);
			ADV_ASSERT(false, __ERR_CANNOT_PURGE_RES);
		}
	}
	while (GetMemFree() < 0) {
		if (!sfxPurgeRes(-1, -1))
			break;
	}
	tries = 100;
	while (tries-- > 0) {
		/* TRY TO LOAD RESOURCE */
		if (mapList != NULL)
			data = VolLoad(id, type);
		else
			data = FileLoad(id, type);

		/* BREAK FROM LOOP ON SUCCESSFUL LOAD */
		if (data != NULL)
			break;

		/* MAKE MORE ROOM FOR RESOURCE */
		if (!sfxPurgeRes(-1, -1))
			ADV_ASSERT(false, __ERR_CANNOT_PURGE_RES);
	}
	if (tries == 0)
		ADV_ASSERT(false, __ERR_MEM_ALLOC_FAIL);

	/* PUT RESOURCE IN CACHE */
	node = (GenericData *)AllocPtr(sizeof(GenericData));
	ADV_ASSERT(node, __ERR_MEM_ALLOC_FAIL);
	node->locked = false;
	node->id = id;
	node->location = RES_IN_MEM;
	node->time = sfxGetTime();
	node->data = data;
	ArrayList_Add(list, node, NULL);

	/* RETURN POINTER TO RESOURCE */
	return (data);
}

static void FindOldRes(int *typePtr, ArrayList **listPtr, GenericData **dataPtr) {
	int i;
	ArrayList *list;
	unsigned int min = 0xffffffff;
	GenericData *data = NULL;

	*dataPtr = NULL;

	/* SEARCH ATS */
	list = atsCache->list;
	for (i = 0; i < list->size; i++) {
		data = (GenericData *)list->elements[i];
		if ((!data->locked) && (data->time < min)) {
			min = data->time;
			*dataPtr = data;
			*typePtr = RES_ATS;
			*listPtr = list;
		}
	}

	/* SEARCH PIC */
	list = picCache->list;
	for (i = 0; i < list->size; i++) {
		data = (GenericData *)list->elements[i];
		if ((!data->locked) && (data->time < min)) {
			min = data->time;
			*dataPtr = data;
			*typePtr = RES_PIC;
			*listPtr = list;
		}
	}

	/* SEARCH WAVE */
	list = wavCache->list;
	for (i = 0; i < list->size; i++) {
		data = (GenericData *)list->elements[i];
		if ((!data->locked) && (data->time < min)) {
			min = data->time;
			*dataPtr = data;
			*typePtr = RES_WAVE;
			*listPtr = list;
		}
	}

	/* SEARCH MIDI */
	list = midCache->list;
	for (i = 0; i < list->size; i++) {
		data = (GenericData *)list->elements[i];
		if ((!data->locked) && (data->time < min)) {
			min = data->time;
			*dataPtr = data;
			*typePtr = RES_MIDI;
			*listPtr = list;
		}
	}

	/* SEARCH TEXT */
	list = fonCache->list;
	for (i = 0; i < list->size; i++) {
		data = (GenericData *)list->elements[i];
		if ((!data->locked) && (data->time < min)) {
			min = data->time;
			*dataPtr = data;
			*typePtr = RES_TEXT;
			*listPtr = list;
		}
	}

	/* SEARCH HTM */
	list = htmCache->list;
	for (i = 0; i < list->size; i++) {
		data = (GenericData *)list->elements[i];
		if ((!data->locked) && (data->time < min)) {
			min = data->time;
			*dataPtr = data;
			*typePtr = RES_HTM;
			*listPtr = list;
		}
	}

	/* SEARCH FACE */
	list = faceCache->list;
	for (i = 0; i < list->size; i++) {
		data = (GenericData *)list->elements[i];
		if ((!data->locked) && (data->time < min)) {
			min = data->time;
			*dataPtr = data;
			*typePtr = RES_FACE;
			*listPtr = list;
		}
	}
}

int sfxPurgeRes(int id, int type) {
	int i;
	ArrayList *list = nullptr;
	unsigned min = 0xffffffff;
	GenericData *data = NULL, *pData = NULL;
	void (*freeFn) (GenericData * pNode) = nullptr;

	if (!cacheValide)
		return false;

	/* SELECT RESOURCE TO PURGE */
	if (type == -1) {
		FindOldRes(&type, &list, &data);
	} else {
		/* SELECT CACHE */
		switch (type) {
		case RES_ATS:
			list = atsCache->list;
			break;

		case RES_PIC:
			list = picCache->list;
			break;

		case RES_WAVE:
			list = wavCache->list;
			break;

		case RES_MIDI:
			list = midCache->list;
			break;

		case RES_TEXT:
			list = fonCache->list;
			break;

		case RES_HTM:
			list = htmCache->list;
			break;

		case RES_FACE:
			list = faceCache->list;
			break;

		}
		if (id < 0) {
			/* AUTO PURGE */
			pData = NULL;
			for (i = 0; i < list->size; i++) {
				data = (GenericData *)list->elements[i];
				if ((!data->locked) && (data->time < min)) {
					min = data->time;
					pData = data;
				}
			}
			data = pData;
		} else {
			/* MAKE THE RESOURCE IS IN CACHE */
			data = SearchRESData(type, id);
		}
	}

	if ((data == NULL) || (data->locked))
		return false;

	/* PURGE RESOURCE */
	switch (type) {
	case RES_PIC:
		freeFn = FreePICNode;
		break;

	case RES_ATS:
		freeFn = FreeATSNode;
		break;

	case RES_TEXT:
		freeFn = FreeMSGNode;
		break;

	case RES_HTM:
		freeFn = FreeHTMNode;
		break;

	case RES_WAVE:
		freeFn = FreeWaveNode;
		break;

	case RES_MIDI:
		freeFn = FreeMidiNode;
		break;

	case RES_FACE:
		freeFn = FreeFaceNode;
		break;

	default:
		ADV_ASSERT(false, __ERR_CODING);
	}
	ArrayList_Del(list, data, (FreeFnPtr)freeFn);

	return true;
}

int sfxLockRes(int id, int type) {
	GenericData *data;

	if (!cacheValide)
		return false;

	data = SearchRESData(type, id);

	if (data) {
		data->locked++;
		return true;
	} else {
		return false;
	}
}

int sfxUnlockRes(int id, int type) {
	GenericData *data;

	if (!cacheValide)
		return false;

	data = SearchRESData(type, id);

	if (data) {
		data->locked--;
		return true;
	} else {
		return false;
	}
}

int sfxCheckRes(int id, int type) {
	int i, vid;
	HANDLE hf;
	FileChunk *fk;
	char filename[256];

	if (mapList) {
		vid = type << 24 | id;
		/* SEARCH FROM THE MAP */
		for (i = 0; i < mapList->size; i++) {
			fk = (FileChunk *)mapList->elements[i];
			if (fk->vid == vid)
				return true;
		}
		return false;
	} else {
		ID2File(filename, id, type);
		hf = sfxOpenFile(filename, MADE_FILE_READ);

		if (!hf)
			return false;
		else {
			sfxCloseFile(hf);
			return true;
		}
	}
}

} // namespace Spycraft
