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

#ifndef CDXRESOURCE_H
#define CDXRESOURCE_H

//////////////////////////////////////////////////////////////////////////////////
// Structure used for each file entry in a resource file.
//////////////////////////////////////////////////////////////////////////////////
typedef struct
{
	long offset;
	char name[13];
} FILEENTRY, *LPFILEENTRY;

//////////////////////////////////////////////////////////////////////////////////
//version 2 resource files
//////////////////////////////////////////////////////////////////////////////////

typedef struct
{
	UINT	Signature;			//signature of the resource file, 'CDXR'
	UINT	Version;			//currently version 2
	UINT	NumOfEntries;		//number of resource file entries
} CDXRESOURCEFILEHEADER;

typedef UINT RESHANDLE;

class CDXAPI CDXResDirEntry
{
public:
	CDXResDirEntry()	{pos=0; size=0; type=0; id=0;}		//constructor - so new will be cleared

	UINT		pos;					//position in RF of particular file
	UINT		size;					//size of particular file
	UCHAR		type;					//are 256 different types enough?
	UINT		id;						//id of the file, see IDHFILE keyword of ResourceCompiler
};



//////////////////////////////////////////////////////////////////////////////////
// The CDXResource class allows you to clump all types of files together for 
// your game. Files can be retrieved at runtime with the GetFile() method. There 
// is very little implied structure with the file so any type of file can be
// used as a resource.
//////////////////////////////////////////////////////////////////////////////////
class CDXAPI CDXResource
{
public:
	CDXResource(void);
	virtual ~CDXResource(void);

	BOOL Open(const char *Filename);
	BOOL OpenV1(const char *Filename);
	void Close(void);
	char* GetFile(const char *Filename);

public:
	LPFILEENTRY m_pFE;			// file entry structure for each file in a resource file
	int m_Infile;				// handle to the resource file
	int m_Outfile;				// handle to the output file
	int m_nFiles;				// number of files in a resource file

//version 2 resource files
	char	*fName;							//name of Resource File
	FILE	*fHandle;						//handle of Resource File
	UINT	version;						//version of resource file
	UINT	NumOfEntries;
	UINT	fSize;							//Resource File Size
	CDXResDirEntry	*Directory;

	FILE*	GetfHandle();
	RESHANDLE GetHandle(UINT id);
	FILE*	Seek(RESHANDLE i);
	UINT GetPos(RESHANDLE i);
	UINT GetSize(RESHANDLE i);
	UCHAR GetType(RESHANDLE i);
};

#endif
