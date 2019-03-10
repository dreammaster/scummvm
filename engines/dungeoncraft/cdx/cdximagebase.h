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
// CDXImageBase.h: interface for the CDXImageBase class.
//
//////////////////////////////////////////////////////////////////////////////////

#ifndef DUNGEONCRAFT_CDX_CDXIMAGEBASE_H
#define DUNGEONCRAFT_CDX_CDXIMAGEBASE_H

//#define VC_EXTRALEAN

#include "cdx.h"
#include "cdxfile.h"

namespace DungeonCraft {
namespace CDX {

#define CDXIMAGE_UNKNOWN  0
#define CDXIMAGE_BMP      1
#define CDXIMAGE_PSD      2
#define CDXIMAGE_TGA      3
#define CDXIMAGE_PCX      4
#define CDXIMAGE_JPG      5
#define CDXIMAGE_PNG      6
#define CDXIMAGE_LAST     7

#define BIG2LIT(a)			(a>>8 & 0xff) | ((a & 0x00ff)<<8)
#define DWORDBIG2LIT(b)     MAKELONG( BIG2LIT(HIWORD(b)), BIG2LIT(LOWORD(b)) );


class CDXImageBase  
{
public:
	CDXImageBase();
	virtual ~CDXImageBase();

	//must implement in derived classes
	virtual BOOL Validate(const CHAR *head, const int headsize, const CHAR *foot, const int footsize) = 0;
	virtual CDX_LPDIRECTDRAWSURFACE GetImage(CDX_LPDIRECTDRAW lpDD, LONG lSize, CHAR* lpCache, BYTE memType ) = 0;

protected:
	void			SetupBitmapInfo();
    BOOL			AllocateStorage( int w, int h, unsigned char bpp );
	BOOL			AllocateStorage( unsigned long size);
	BOOL			DeAllocateStorage(void);
    BOOL			OpenFile( const char *filename );
    BOOL			CloseFile(void);
    BOOL			CreateSurface(CDXSurface* pSurface, BYTE memType);
    CHAR*           GetStoragePtr(void)				{ return m_pStorage; }
	BOOL			IsInverted(void)				{ return m_IsInverted; }
	BOOL			IsMirrored(void)				{ return m_IsMirrored; }
    void			SetInverted(BOOL b)				{ m_IsInverted = b; }
	void			SetMirrored(BOOL b)				{ m_IsMirrored = b; }
    void			SetImageWidth(int w)			{ m_iWidth = w; }
    void			SetImageHeight(int h)			{ m_iHeight = h; }
    void			SetImageBPP(unsigned char bpp)	{ m_cBpp = bpp; }
    int				GetImageWidth(void)				{ return m_iWidth; }
    int				GetImageHeight(void)			{ return m_iHeight; }
    unsigned char	GetImageBPP(void)               { return m_cBpp; }
    unsigned long	GetSizeInBytes(void)			{ return m_iWidth * m_iHeight * (m_cBpp>>3); }
	CDX_LPDIRECTDRAWSURFACE CopyBits(void);
    BOOL			CreateSurface(CDX_LPDIRECTDRAW lpDD, BYTE memType);	

protected:
	BITMAPINFO*     m_pBitmapInfo;  // 
	BITMAPINFO		m_BitmapInfo;   // 
	CDXFile			m_file;         // file handler
    int				m_iWidth;		// signed because if -, that means image is backwards.
    int				m_iHeight;		// signed because if -, that means image is upside down.
    unsigned char   m_cBpp;			// char, valids are only 1,2,4,8,16,24,32
	CHAR*           m_pStorage;     // char, 8bit or less is accessed by byte
    BOOL            m_IsInverted;   // 
	BOOL			m_IsMirrored;   // 
	PALETTEENTRY*   m_pPalette;     // 
	unsigned int	m_iUsage;		// DIB_RGB_COLORS or DIB_PAL_COLORS
	CDX_LPDIRECTDRAWSURFACE m_lpdds;	
};

} // End of namespace CDX
} // End of namespace DungeonCraft

#endif
