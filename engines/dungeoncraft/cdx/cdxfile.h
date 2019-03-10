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

#ifndef DUNGEONCRAFT_CDX_CDXFILE_H
#define DUNGEONCRAFT_CDX_CDXFILE_H

#ifndef CDX_H
class CDXFile;
#include "cdx.h"
#endif

namespace DungeonCraft {
namespace CDX {

#ifndef QWORD
#define QWORD unsigned __int64
#endif

#ifndef INT64
#define INT64 __int64
#endif

//////////////////////////////////////////////////////////////////////////////////
// File handling wrapper
//////////////////////////////////////////////////////////////////////////////////
class CDXAPI CDXFile
{
public:
	CDXFile();
	virtual ~CDXFile();

	int		Open(const char* FileName);
	int 	OpenRead(const char* FileName);
	int		OpenWrite(const char* FileName);

	void	Close();

	// reading functions
	DWORD	Read(void *whence, UINT howmuch);
	
	UCHAR	ReadUCHAR();
	BYTE	ReadBYTE();

	USHORT	ReadUSHORT();
	WORD	ReadWORD();

	UINT	ReadUINT();
	DWORD	ReadDWORD();

	INT64	ReadINT64();
	QWORD	ReadQWORD();

	// writing functions
	DWORD	Write(const void *whence, UINT howmuch);

	DWORD	WriteUCHAR(const UCHAR data);
	DWORD	WriteBYTE(const BYTE data);

	DWORD	WriteUSHORT(const USHORT data);
	DWORD	WriteWORD(const WORD data);

	DWORD	WriteUINT(const UINT data);
	DWORD	WriteDWORD(const DWORD data);

	DWORD	WriteINT64(const INT64 data);
	DWORD	WriteQWORD(const QWORD data);

	// File pointer functions
	DWORD	Position();
	DWORD	Position(LONG lDistanceToMove, DWORD dwMoveMethod);
	
	void	SeekToStart();
	void	SeekToEnd();

	// information functions
	DWORD	FileSize();

protected:
	int		OpenCommon(const char *FileName, DWORD Access, DWORD Sharing, DWORD CreateDisposition);

private:
	HANDLE	m_fh;
};

} // End of namespace CDX
} // End of namespace DungeonCraft

#endif
