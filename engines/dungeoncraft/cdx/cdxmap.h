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

#ifndef CDXMAP_H
#define CDXMAP_H

#include "cdxtile.h"

//////////////////////////////////////////////////////////////////////////////////
// The CDXMapCell is a helper class for all CDXMap classes. It allows you to 
// define override a custom class to define your data for each map tile. This 
// way you can create your own CDXMapCell class and store as much information
// as you want and save it the way you want.
//////////////////////////////////////////////////////////////////////////////////
class CDXAPI CDXMapCell
{
public:
	CDXMapCell();
	virtual ~CDXMapCell();

	virtual void Save(FILE *fp);
	virtual void Load(FILE *fp);
	virtual int GetTile();
	virtual void SetTile(int Tile);
private:
	int TileNum;
};

//////////////////////////////////////////////////////////////////////////////////
// This is the main object which controls tile based scrolling in CDX. Screen, 
// tile and map information all come together in this class to produce a scrolling 
// output.  Map information is stored (using the default CDXMapCell class) in 
// binary files in the simplest possible form:
//
// Map width (4 bytes)
// Map height (4 bytes)
// Map data (Map width * Map height * 4 bytes)
//////////////////////////////////////////////////////////////////////////////////
class CDXAPI CDXMap
{
public:
	CDXMap(CDXScreen *pScreen);
	CDXMap(CDXTile *pTiles, CDXScreen *pScreen);
	virtual ~CDXMap();

	// creation functions
	virtual BOOL Create(int Width, int Height, int Fill);

	virtual BOOL Load( FILE *fptr );
	virtual BOOL Load(const char *szFilename);
	virtual BOOL Save( FILE *fptr );
	virtual BOOL Save(const char *szFilename);
	virtual void Clear(void);
	virtual void Fill(int TileNum);

	// drawing functions
	virtual void Draw(CDXSurface* lpDDS, int BlitStyle = 0, BOOL Wrap=TRUE );
	virtual void DrawTrans(CDXSurface* lpDDS);
	virtual void DrawClipped(CDXSurface* lpDDS, LPRECT ClipRect);

	// animation functions
	virtual void NextFrame();

	// location functions
	virtual void MoveTo(int PosX, int PosY);
	virtual int GetPosX();
	virtual int GetPosY();
	virtual void ScrollUp(int Offset);
	virtual void ScrollDown(int Offset);
	virtual void ScrollLeft(int Offset);
	virtual void ScrollRight(int Offset);
	void EnableScrollWrapping(BOOL bEnable);
	virtual void ScreenTileSize(int Width, int Height);
	virtual void SetViewPort(LPRECT View);
	virtual RECT GetTileCoord(int MapX, int MapY);
	virtual int  GetTile(int MapX, int MapY);
	virtual CDXMapCell *GetCell(int MapX, int MapY);
	virtual void SetTile(int MapX, int MapY, int Tile);
	virtual void LoadTiles(CDXTile *pTiles);
	virtual BOOL InitData(int iSIZE, int iFill);
	virtual int GetMapWidth();
	virtual int GetMapHeight();
	virtual int GetMapSize() { return m_SIZE; };
	virtual int GetPixelWidth() { return m_PixelWidth; }
	virtual int GetPixelHeight() { return m_PixelHeight; }
	virtual int GetTileWidth();
	virtual int GetTileHeight();
	virtual int GetScreenWidth();
	virtual int GetScreenHeight();
	virtual int GetScreenBlocksWidth();
	virtual int GetScreenBlocksHeight();

protected:
	void Init();

protected:
	int m_PosX;			// The current map X position in pixels
	int m_PosY;			// The current map Y position in pixels
	int m_Width;		// The map width in map units
	int m_Height;		// The map height in map units
	int m_TileWidth;	// Tile width in pixels, from the CDXTile pointer
	int m_TileHeight;	// Tile height in pixels, from the CDXTile pointer
	int m_PixelWidth;	// The width of the map in pixels
	int m_PixelHeight;	// The height of the map in pixels
	int m_ViewportX;	// The starting X position of the Viewport
	int m_ViewportY;	// The starting Y position of the Viewport
	int m_SCREEN_TW;		// Number of tiles wide on screen
	int m_SCREEN_TH;		// Number of tiles high on screen
	int m_SCREEN_W;		// Screen pixel width, from CDXScreen pointer
	int m_SCREEN_H;		// Screen pixel height, from CDXScreen pointer
	int m_SIZE;			// Map size, width * height

protected:
	CDXMapCell *m_DATA;		// Pointer to the CDXMapCell object for tiles
	CDXScreen *m_Screen;	// Pointer to the CDXScreen object
	CDXTile *m_Tiles;		// Pointer to the CDXTile object

	BOOL m_bScrollWrapping;	// turn this on to wrap while you scroll
};

//////////////////////////////////////////////////////////////////////////////////
// This is the main object which controls tile based ISO scrolling in CDX. Screen, 
// tile and map information all come together in this class to produce a scrolling 
// output.
//////////////////////////////////////////////////////////////////////////////////
class CDXAPI CDXIsoMap : public CDXMap
{
public:
	CDXIsoMap(CDXTile *pTiles, CDXScreen *pScreen):CDXMap(pTiles, pScreen) {};
    virtual void ScrollUp(int);
    virtual void ScrollDown(int);
    virtual void ScrollLeft(int);
    virtual void ScrollRight(int);
    virtual void BltTileTrans(CDXSurface*, int, int, int, int, int, int, int, int);
    virtual void DrawTrans(CDXSurface*);
	virtual POINT ProcessClicks(int, int);
};

#endif
