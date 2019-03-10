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

#ifndef DUNGEONCRAFT_CDX_CDXLAYER_H
#define DUNGEONCRAFT_CDX_CDXLAYER_H

#include "cdxsurface.h"

namespace DungeonCraft {
namespace CDX {

//////////////////////////////////////////////////////////////////////////////////
// CDXLayer is derived from CDXSurface and is very similar in most respects. 
// However, a CDXLayer has the added ability to smoothly scroll the surface in 
// all directions. This effect is often seen in platform games where the background 
// is a scrolling bitmap and the foreground a scrolling tile map, giving a good 
// feeling of depth. A CDXLayer would also be perfect for a pinball game since 
// DirectX places no restrictions on the height of bitmaps in video memory.
//////////////////////////////////////////////////////////////////////////////////
class CDXAPI CDXLayer
{
public:
    CDXLayer(void);
    virtual ~CDXLayer(void);

	HRESULT Create(CDXScreen *pScreen, char *szFilename, BOOL memoryType = CDXMEM_VIDTHENSYS );
	HRESULT Create(CDXSurface* pSurface);

	void ScrollUp(int Offset);
	void ScrollDown(int Offset);
	void ScrollLeft(int Offset);
	void ScrollRight(int Offset);
	void MoveTo(int XOffset, int YOffset);

	virtual HRESULT Draw(CDXSurface* lpDDS);
	virtual HRESULT Draw(int X, int Y, CDXSurface* lpDDS);
	virtual HRESULT DrawTrans(int X, int Y, CDXSurface* lpDDS);

public:
	int         m_XOffset;   // The current X position in pixels
	int         m_YOffset;   // The current Y position in pixels
	CDXSurface* m_pSurface;
	BOOL        m_bSurfaceCreated;
};

} // End of namespace CDX
} // End of namespace DungeonCraft

#endif
