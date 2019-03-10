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

#ifndef DUNGEONCRAFT_CDX_CDXTILE
#define DUNGEONCRAFT_CDX_CDXTILE

#include "cdxsurface.h"

namespace DungeonCraft {
namespace CDX {

#define CDXBLT_BLKALPHA25			11
#define CDXBLT_TRANSALPHA25 		111

//////////////////////////////////////////////////////////////////////////////////
// This class name is somewhat deceptive given that a CDXTile object holds the 
// bitmap information for all the tiles of a given map or sprite, rather than just 
// one tile. CDXTile is derived from CDXSurface.
//
// IMPORTANT NOTES: The bitmap file containing the tiles should have an empty tile 
// at the start, this is because CDX treats the first tile to be transparent. Tiles 
// should be stored from left to right down the bitmap and should fit exactly into 
// the width of the bitmap, have a look at the tiles from one of the examples if 
// this is unclear. Remember to set the colour key for the surface if you intend to 
// draw the tiles transparently.
//////////////////////////////////////////////////////////////////////////////////
struct CDX_ANIMSTRUCT
{
	INT32 framerate, pause, rnd;
    CDXBOOL pingpong;
	
	INT32 *anim, num;
};

class CDXAPI CDXTile : public CDXSurface
{
public:
	CDXTile();
	virtual ~CDXTile();

	CDXBOOL Create(CDXScreen *pScreen, const char* szFilename, int w, int h, int num, BYTE memoryType = CDXMEM_VIDTHENSYS);
    CDXBOOL Create(CDXScreen *pScreen, LONG lSize, CDXFile* fs, int w, int h, int num, BYTE memoryType = CDXMEM_VIDTHENSYS);
    CDXBOOL Create(CDXScreen *pScreen, LONG lSize, CHAR* lpCache, int w, int h, int num, BYTE memoryType = CDXMEM_VIDTHENSYS);
	CDXBOOL CreateFromTLE(CDXScreen *pScreen, const char* szFilename , BYTE memoryType = CDXMEM_VIDTHENSYS );

	const RECT *GetBlockRect(int block);

 // Anim stuff

	// adds animation frames to a specific index
//	virtual CDXBOOL AddAnim( int framerate, int pause, int rnd, BOOL pingpong, INT32 *anim, int num );
	virtual INT32 AddAnim( int framerate, int pause, int rnd, BOOL pingpong, INT32 *anim, int num );
//	virtual CDXBOOL AddAnimSection( int framerate, int pause, int rnd, BOOL pingpong, int blockstart, int blockend );
	virtual INT32 AddAnimSection( int framerate, int pause, int rnd, BOOL pingpong, int blockstart, int numblocks );
	virtual CDXBOOL InsertTile( int anim, int pos, int tile );
	virtual CDXBOOL DelAnim( int pos );
	virtual CDXBOOL RemoveTile( int anim, int pos );
	virtual INT32 GetAnimTile( int tile );
	virtual void Draw( int x, int y, CDXSurface *s, int BlitType, int tile );
	virtual void DrawFirst( int x, int y, CDXSurface *s, int BlitType, int tile );
	virtual void NextFrame( );
	virtual CDXBOOL LoadAnim( FILE *fptr );
	virtual CDXBOOL SaveAnim( FILE *fptr );
	virtual inline INT32 GetAnimCount( void )	{	return m_Count;	};
	virtual CDX_ANIMSTRUCT * GetAnimData( int nr );

private:
	CDX_ANIMSTRUCT *m_Data;
	INT32 m_Count, m_FrameCtr;
 // Anim stuff end 

public:
	INT32 GetBlockWidth() { return m_BlockWidth; }
	INT32 GetBlockHeight() { return m_BlockHeight; }
	INT32 GetBlockCount() { return m_BlockNum; }

protected:
	void Init();
	void SetSrc(INT32 block) { CDXSurface::SetClipRect(&m_BlockRects[block]); }

public:
	INT32 m_BlockWidth;		// The width of one tile, in pixels
	INT32 m_BlockHeight;	// The height of one tile, in pixels
	INT32 m_BlockNum;		// The number of tiles in the bitmap file
	INT32 m_BlockX;			// The number of tiles (x)
	INT32 m_BlockY;			// The number of tiles (y)

private:
	RECT *m_BlockRects;    // Pointer to thelist of block rects
};

#endif