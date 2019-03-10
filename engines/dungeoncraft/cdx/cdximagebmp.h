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
// CDXImageBMP.h: interface for the CDXImageBMP class.
//
//////////////////////////////////////////////////////////////////////////////////

#ifndef DUNGEONCRAFT_CDX_CDXIMAGEBMP_H
#define DUNGEONCRAFT_CDX_CDXIMAGEBMP_H

#include "CDXImageBase.h"

namespace DungeonCraft {
namespace CDX {

class CDXImageBMP : public CDXImageBase  
{
public:
	CDXImageBMP(void);
	virtual ~CDXImageBMP(void);

	CDX_LPDIRECTDRAWSURFACE GetImage(CDX_LPDIRECTDRAW lpDD, LONG lSize, CHAR* lpCache, BYTE memType);
	BOOL Validate(const CHAR *head, const int headsize, const CHAR *foot, const int footsize);

private:
	BOOL ReadRLE8Format(CHAR* lpCache, DWORD offset);

	LPBITMAPINFO		m_pbi;
	BITMAPINFO			m_bi;
	BITMAPFILEHEADER	m_bif;
	int					m_iColoursUsed;
};

} // End of namespace CDX
} // End of namespace DungeonCraft

#endif
