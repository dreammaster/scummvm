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

#ifndef DUNGEONCRAFT_CDX_CDXIMAGE_H
#define DUNGEONCRAFT_CDX_CDXIMAGE_H

#include "cdxfile.h"
#include "cdximagebase.h"

namespace DungeonCraft {
namespace CDX {

const int headsize = 256;
const int footsize = 64;

class CDXImage  
{
public:
	CDXImage(void);
	virtual ~CDXImage(void);

	CDX_LPDIRECTDRAWSURFACE GetImage(CDX_LPDIRECTDRAW lpDD, const char* filename, BYTE memType = CDXMEM_VIDTHENSYS, DWORD type = CDXIMAGE_UNKNOWN );
    CDX_LPDIRECTDRAWSURFACE GetImage(CDX_LPDIRECTDRAW lpDD, LONG lSize, CHAR* lpCache, BYTE memType = CDXMEM_VIDTHENSYS, DWORD type = CDXIMAGE_UNKNOWN );
    CDX_LPDIRECTDRAWSURFACE GetImage(CDX_LPDIRECTDRAW lpDD, LONG lSize, CDXFile* fsptr, BYTE memType = CDXMEM_VIDTHENSYS, DWORD type = CDXIMAGE_UNKNOWN);
    CDX_LPDIRECTDRAWSURFACE GetImage(CDX_LPDIRECTDRAW lpDD, LONG lSize, FILE* fh, BYTE memType = CDXMEM_VIDTHENSYS, DWORD type = CDXIMAGE_UNKNOWN);
	CDX_LPDIRECTDRAWSURFACE CreateDDSurface(CDX_LPDIRECTDRAW lpDD, LONG lSize, CHAR* lpCache, BYTE memType, DWORD type );

private:
	DWORD GetFileFormat(LONG lSize, CHAR* lpCache);
  char m_filename[_MAX_PATH+1];

};

} // End of namespace CDX
} // End of namespace DungeonCraft

#endif
