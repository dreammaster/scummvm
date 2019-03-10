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

#ifndef DUNGEONCRAFT_RUNTIME_CDX_BITMAPFONT
#define DUNGEONCRAFT_RUNTIME_CDX_BITMAPFONT

//#define NTDX3
#define CDXINCLUDEALL
#include "dungeoncraft/cdx/cdx.h"

namespace DungeonCraft {

#ifndef CDX_CENTERJUSTIFY
#define CDX_CENTERJUSTIFY 1
#endif
#ifndef CDX_RIGHTJUSTIFY
#define CDX_RIGHTJUSTIFY 3
#endif

extern CDXAPI void Clip(int *DestX, int *DestY, RECT *SrcRect, RECT *DestRect);

class CDXBitmapFont
{
  CDXBitmapFont() { } // default ctor not used
public:
  
  enum { MAX_CHAR=128 };
  typedef struct 
  {
    RECT    Rectangle;
    int   Width , Height;
  } CDXBITMAPFONT;
  
  CDXBITMAPFONT CDXBitmapFontArray[ MAX_CHAR ];
  LOGFONT m_lf;
  bool m_uself;
  int       FontColor;
  int       FontBackgroundColor;
  int       FontColorKey;
  int       FontHeight;
  int       FontAttributes;
  const char * FontName;
  int       TextSurfaceType;

  int m_MaxCharHeight;
  int m_MaxCharWidth;
  
  CDXSurface *  TextSurface;
  CDXScreen  *  Screen;
  
  CDXBitmapFont( CDXScreen * Scr );
  CDXBitmapFont(CDXScreen *Scr,LOGFONT *lf,int Color,int ColKey,int BackColor,int MemSurfaceType=CDXMEM_VIDTHENSYS);
  CDXBitmapFont( CDXScreen * Scr , const char * FontNam , int Height , int Color = RGB( 255 , 255 , 255 ), int ColKey = RGB(0,0,0),
     int BackColor = RGB(0,0,0) , int Attributes = FW_NORMAL , int SurfaceType = CDXMEM_VIDTHENSYS );
  ~CDXBitmapFont();
  
  void Create(CDXScreen * Scr, LOGFONT *lf, int Color, int ColKey, int BackColor, int SurfaceType=CDXMEM_VIDTHENSYS);
  void Create( CDXScreen * Scr , const char * FontNam , int Height , int Color , int ColKey, 
    int BackColor , int Attributes = FW_NORMAL , int SurfaceType = CDXMEM_VIDTHENSYS );
  void PaintCharactersInSurface( void );
  
  HRESULT Draw(int X, int Y, const char * Text , CDXSurface* lpDDest , int Length = -1 );
  HRESULT DrawClipped(int X, int Y, char * Text , CDXSurface* lpDDest, LPRECT ClipRect , int Length = -1);
  HRESULT DrawTrans(int X, int Y, const char * Text , CDXSurface* lpDDest , int Length = -1);
  HRESULT DrawTransClipped(int X, int Y, char * Text , CDXSurface* lpDDest, LPRECT ClipRect , int Length = -1);
  HRESULT DrawAligned( int X , int Y , int Width , char * Text , CDXSurface * lpDDest , int Align );
  HRESULT DrawAlignedTrans( int X , int Y , int Width , char * Text , CDXSurface * lpDDest , int Align );
  
  void SetFont( char * FontNam , int Height , int Color , int Attributes = FW_NORMAL );
  void SetColor( int Color );
  void SetColorKey( int ColorKey );
  void SetBackgroundColor( int Color );
  
  int GetCharacterWidth( char ch );
  int GetCharacterHeight( char ch );
  int GetTextWidth(const char * Text, int length=-1 );
  
  void Restore( );
};

} // End of namespace DungeonCraft

#endif