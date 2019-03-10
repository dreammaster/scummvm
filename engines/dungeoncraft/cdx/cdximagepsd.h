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

//////////////////////////////////////////////////////////////////////////////////
// CDXImagePSD.h: interface for the CDXImagePSD class.
//
//////////////////////////////////////////////////////////////////////////////////

#ifndef DUNGEONCRAFT_CDX_CDXIMAGEPSD_H
#define DUNGEONCRAFT_CDX_CDXIMAGEPSD_H

#include "cdximagebase.h"

#pragma pack(1)
struct _psdHeader 
{
	char	magic[4];	//magic identifier (8BPS)
	WORD	version;	//must be 1
	WORD	pad[3];		//unneeded data (always 0)
	WORD	channels;	//number of channels
	DWORD	height;
	DWORD	width;
	WORD	bpc;		//bits per channel. must be 8
	WORD	colormode;	//colormode.  must be 3
	DWORD   ModeDataCount;
};
#pragma pack()

#define MAX_PSD_CHANNELS	24

class CDXImagePSD : public  CDXImageBase
{
public:
	CDXImagePSD(void);
	virtual ~CDXImagePSD(void);

	CDX_LPDIRECTDRAWSURFACE GetImage(CDX_LPDIRECTDRAW lpDD, LONG lSize, CHAR* lpCache, BYTE memType);
	BOOL Validate(const CHAR *head, const int headsize, const CHAR *foot, const int footsize);

private:
	void		UnPackPSD( CHAR* lpCache, BYTE* pixels, WORD channelCnt, WORD compression );
	_psdHeader	m_psdInfo;
};

} // End of namespace CDX
} // End of namespace DungeonCraft

#endif
