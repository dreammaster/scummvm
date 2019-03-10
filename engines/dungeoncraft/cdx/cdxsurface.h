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

#ifndef CDXSURFACE_H
#define CDXSURFACE_H

#include "cdxscreen.h"
#include "cdxresource.h"
#include "cdxfile.h"

// CDX BLT types
#define CDXBLT_BLK				0
#define CDXBLT_BLKALPHA			1
#define CDXBLT_BLKALPHAFAST		2
#define CDXBLT_BLKSCALED		3
#define CDXBLT_BLKSHADOW		4
#define CDXBLT_BLKSHADOWFAST	5
#define CDXBLT_BLKROTATED		6
#define CDXBLT_BLKROTOZOOM		7
#define CDXBLT_BLKHFLIP			8
#define CDXBLT_BLKVFLIP			9
#define CDXBLT_BLKSTRETCHED     10

#define CDXBLT_TRANS			100
#define CDXBLT_TRANSALPHA		101
#define CDXBLT_TRANSALPHAFAST	102
#define CDXBLT_TRANSSCALED		103
#define CDXBLT_TRANSSHADOW		104
#define CDXBLT_TRANSSHADOWFAST	105
#define CDXBLT_TRANSROTATED		106
#define CDXBLT_TRANSROTOZOOM	107
#define CDXBLT_TRANSHFLIP		108
#define CDXBLT_TRANSVFLIP		109
#define CDXBLT_TRANSSTRETCHED   110

#define CDXBLT_TRANSALPHAMASK	120

// CDX memory types
#define CDXMEM_VIDEOONLY		0
#define CDXMEM_SYSTEMONLY		1
#define CDXMEM_VIDTHENSYS		2

// CDX SIMD instuction set types
#define CDXSIMD_NONE			0
#define CDXSIMD_MMX		        2
#define CDXSIMD_3DNOW		    4






struct TexMapTable
	{
		int x;
		int px;
		int py;
	};





class CDXAPI CDXSurface
{
public:
	
	CDXSurface();
	CDXSurface(CDXSurface& aSurface);
	virtual ~CDXSurface();

	HRESULT Create(CDXScreen* pScreen, int Width, int Height, BYTE memoryType = CDXMEM_VIDTHENSYS);
	HRESULT Create(CDXScreen* pScreen, const char *szFilename, BYTE memoryType = CDXMEM_VIDTHENSYS);
	HRESULT Create(CDXScreen* pScreen, LONG size, FILE* fh, BYTE memoryType = CDXMEM_VIDTHENSYS);
	//HRESULT Create(CDXScreen* pScreen, CDXResource* r, RESHANDLE i, BYTE memoryType = CDXMEM_VIDTHENSYS);
    HRESULT Create(CDXScreen* pScreen, LONG size, CDXFile* fs, BYTE memoryType = CDXMEM_VIDTHENSYS);
    HRESULT Create(CDXScreen* pScreen, LONG size, CHAR* lpCache, BYTE memoryType = CDXMEM_VIDTHENSYS);
	
    inline HRESULT PutPixel(int X, int Y, DWORD Col) { return (*this.*m_fpPutPixel)(X, Y, Col); }
    inline HRESULT PutAAPixel(int X, int Y, DWORD Col, WORD ALPHA) { return (*this.*m_fpPutAAPixel)(X, Y, Col, ALPHA); }
    inline DWORD   GetPixel(int X, int Y) { return (*this.*m_fpGetPixel)(X, Y); }
    inline HRESULT VLine(int Y1, int Y2, int X, DWORD Col) { return (*this.*m_fpVLine)(Y1, Y2, X, Col); }
    inline HRESULT HLine(int X1, int X2, int Y, DWORD Col) { return (*this.*m_fpHLine)(X1, X2, Y, Col); }
	HRESULT Rect(int X1,int Y1,int X2,int Y2,DWORD Col);
	HRESULT FillRect(int X1, int Y1, int X2, int Y2, DWORD Col);
	HRESULT Line(int X1, int Y1, int X2, int Y2, DWORD Col);
	HRESULT AALine(int X1, int Y1, int X2, int Y2, DWORD Col);
	HRESULT Circle(int X, int Y, int Radius, DWORD Col);
	HRESULT FillCircle(int X, int Y, int Radius, DWORD Col);
	HRESULT RoundedRect(int X1, int Y1, int X2, int Y2, int Radius, DWORD Col);
    HRESULT Fill(DWORD FillColor);
	HRESULT InvertColors( int x1, int y1, int x2, int y2 );
    HRESULT TextXY(int X, int Y, COLORREF Col, LPCTSTR pString);
	HRESULT DrawText(LPCSTR pString, COLORREF col, LPRECT pRect);
    HRESULT SaveAsBMP(const char * szFilename);
    BOOL ValidateBlt(CDXSurface* lpCDXS, LONG *lDestX, LONG *lDestY, RECT *srcRect);
	BOOL ClipRect(RECT *Rect);
    BOOL LoadAlphaMask(const char* szFilename);

	HRESULT DrawBlk(CDXSurface* lpCDXS, LONG lDestX, LONG lDestY, RECT* srcRect = NULL);
	HRESULT DrawBlkSW(CDXSurface* lpCDXS, LONG lDestX, LONG lDestY, RECT* srcRect = NULL);
	HRESULT DrawTrans(CDXSurface* lpCDXS, LONG lDestX, LONG lDestY, RECT* srcRect = NULL);
	HRESULT DrawTransSW(CDXSurface* lpCDXS, LONG lDestX, LONG lDestY, RECT* srcRect = NULL);
	HRESULT DrawBlkHFlip(CDXSurface* lpCDXS, LONG lDestX, LONG lDestY, RECT* srcRect = NULL);
	HRESULT DrawTransHFlip(CDXSurface* lpCDXS, LONG lDestX, LONG lDestY, RECT* srcRect = NULL);
	HRESULT DrawBlkVFlip(CDXSurface* lpCDXS, LONG lDestX, LONG lDestY, RECT* srcRect = NULL);
	HRESULT DrawTransVFlip(CDXSurface* lpCDXS, LONG lDestX, LONG lDestY, RECT* srcRect = NULL);
	HRESULT DrawBlkAlphaFast(CDXSurface* dest, LONG lDestX, LONG lDestY, RECT* srcRect = NULL);
	HRESULT DrawTransAlphaFast(CDXSurface* dest, LONG lDestX, LONG lDestY, RECT* srcRect = NULL);
	HRESULT DrawBlkAlpha(CDXSurface* dest, LONG lDestX, LONG lDestY, RECT* srcRect = NULL, WORD ALPHA = 0);
	HRESULT DrawTransAlpha(CDXSurface* dest, LONG lDestX, LONG lDestY, RECT* srcRect = NULL, WORD ALPHA = 0);
	HRESULT DrawBlkShadow(CDXSurface* dest, LONG lDestX, LONG lDestY, RECT* srcRect = NULL, WORD SHADOW = 0);
	HRESULT DrawTransShadow(CDXSurface* dest, LONG lDestX, LONG lDestY, RECT* srcRect = NULL, WORD SHADOW = 0);
	HRESULT DrawBlkShadowFast(CDXSurface* dest, LONG lDestX, LONG lDestY, RECT* srcRect = NULL);
	HRESULT DrawTransShadowFast(CDXSurface* dest, LONG lDestX, LONG lDestY, RECT* srcRect = NULL);
	HRESULT DrawTransAlphaMask(CDXSurface* dest, LONG lDestX, LONG lDestY, RECT* srcRect = NULL);
	HRESULT DrawTransScaled(CDXSurface* dest, LONG lDestX, LONG lDestY, RECT* srcRect = NULL, float scaleFactor = 1);
	HRESULT DrawBlkScaled(CDXSurface* dest, LONG lDestX, LONG lDestY, RECT* srcRect = NULL, float scaleFactor = 1);
	HRESULT DrawBlkStretched(CDXSurface* lpCDXS, LONG lDestX, LONG lDestY, RECT* srcRect = NULL, LONG width = 0, LONG height = 0);
	HRESULT DrawTransStretched(CDXSurface* lpCDXS, LONG lDestX, LONG lDestY, RECT* srcRect = NULL, LONG width = 0, LONG height = 0);
	HRESULT Fade(CDXSurface* dest, LONG lDestX, LONG lDestY, RECT* srcRect = NULL, WORD ALPHA = 0);
    HRESULT DrawBlkRotoZoom(CDXSurface* dest, int midX, int midY, RECT* area, double angle, double scale);
    HRESULT DrawTransRotoZoom(CDXSurface* dest, int midX, int midY, RECT* area, double angle, double scale);
    




    HRESULT     SetColorKey(DWORD col);
	HRESULT     SetColorKey(void);
    DWORD       GetColorKey(void) { return m_ColorKey; }
	HRESULT     Restore(void);
	HRESULT     ChangeFont(const char* FontName, int Width, int Height, int Attributes = FW_NORMAL,int Italic=FALSE);
  HRESULT     ChangeFont(LOGFONT &lf);
	HRESULT     SetFont(void);
	HRESULT     Lock(void);
	HRESULT     UnLock(void);
	HDC         GetDC();
	HRESULT     ReleaseDC();
    void        SetTextureEnabled( BOOL onOff ) { m_TextureEnabled = onOff; }
    BOOL        GetTextureEnabled( void ) { return m_TextureEnabled; }
    int         GetWidth( void ) { return m_PixelWidth;  }
    int         GetHeight( void ) { return m_PixelHeight; }
    DWORD       GetPitch( void ) { return m_DDSD.lPitch; }
    void*       GetSurfacePointer( void ) { return m_DDSD.lpSurface; }
    RECT*       GetClipRect(void) { return &m_ClipRect; }
	void        SetClipRect(RECT *clipRect);
    char*       GetFileName(char* name);
    CDXScreen*  GetScreen(void) { return m_Screen; }
    DWORD       GetSIMDType(void) { return m_SIMDInstrctionSet; }
    char*       GetFontName(char* name);
    BYTE*       GetAlphaMaskPointer(void) { return m_AlphaMask; }
    HRESULT     SetAlphaMaskPointer(BYTE* lpAlpha);
    void        SetNext(CDXSurface* lpCDXS) { m_Next = lpCDXS; }
    void        SetPrev(CDXSurface* lpCDXS) { m_Prev = lpCDXS; }
    CDXSurface* GetNext(void) { return m_Next; }
    CDXSurface* GetPrev(void) { return m_Prev; }
    DWORD       GetSIMDInstrctionSet(void) { return m_SIMDInstrctionSet; }
	void        GetPixelFormat(RGBFORMAT* format);
    HRESULT     ReLoadBitmap(const char* filename = NULL);
    void        FunctionMapper(void);
    void        SetCaptureFileName(char* name) { strncpy(m_CaptureFileName, name, 256); }
    char*       GetCaptureFileName(void) { return m_CaptureFileName; }
    LONG        GetSurfaceId(void) { return m_SurfaceId; }
	void GetSurfaceDescriptor(CDX_LPDDSURFACEDESC lpddsd);
	CDX_LPDIRECTDRAWSURFACE GetDDS( void ) { return m_lpDDS; }  


protected:  // protected member functions
    void InitCDXSurface( void );
    friend void CDXScreen::SetSurfaceScreen(CDXSurface* surface);
    friend void CDXScreen::SetSurfaceWidth(CDXSurface* surface, int w);
    friend void CDXScreen::SetSurfaceHeight(CDXSurface* surface, int h);
    friend void CDXScreen::SetSurfacePixelFormat(CDXSurface* surface);
	friend void CDXScreen::SetSurfaceDDSPointer(CDXSurface* pCDXS, CDX_LPDIRECTDRAWSURFACE lpdds);
    void TextureMap(CDXSurface *dest, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, RECT* area, BOOL bTrans);
    void Scanleftside(int x1,int x2,int ytop,int lineheight,char side,int TexWidth,int TexHeight,RECT* dClip);
    void Scanrightside(int x1,int x2,int ytop,int lineheight,char side,int TexWidth,int TexHeight,RECT* dClip);
	
private:  // Private member data (all data should be private)

	CDX_DDSURFACEDESC       m_DDSD;		// A DDSURFACEDESC object, stores surface information
	CDX_LPDIRECTDRAWSURFACE m_lpDDS;		// The LPDIRECTDRAWSURFACE object
	int         m_PixelWidth;			// The surface width in pixels
	int         m_PixelHeight;			// The surface height in pixels
	char        m_FileName[256];		// The name of the bitmap file loaded, used in Restore
    char        m_CaptureFileName[256];
	HDC         m_DC;					// A HDC object, used for drawing text to the surface
	DWORD       m_ColorKey;				// The specified color key for this surface
	HFONT       m_Font;					// A HFONT object describing the currently selected font
	CDXScreen*  m_Screen;				// A pointer to a CDXScreen object
	DWORD       m_SIMDInstrctionSet;    // Type of SIMD instuction set available
	RECT        m_ClipRect;             // The rectangle to which drawing operations are clipped
	RGBFORMAT   m_PixelFormat;          // Structure that holds the information about the color depths, etc.
	char        m_FontName[256];        // Current font associated to the surface
	BOOL        m_TextureEnabled;       // Tells if this surface was enabled to be a texure
	BYTE*       m_AlphaMask;            // Pointer to the surfaces alpha mask
    CDXSurface* m_Next;                 // Used in linked list of surfaces
    CDXSurface* m_Prev;                 // Used in linked list of surfaces
    DWORD       m_LockCount;            // Reference count to avoid mulitple lock/Unlocks
    DWORD       m_DeviceContextCount;   // Reference count to avoid mulitple GetDC/ReleaseDC
    DWORD       m_MemoryLocation;       // What type of memory the surface is stored in
    LONG        m_SurfaceId;

    TexMapTable* m_leftTable;
    TexMapTable* m_rightTable;

    //Function  Pointer declarations
    HRESULT (CDXSurface::* m_fpPutPixel)(int X, int Y, DWORD Col);
	HRESULT (CDXSurface::* m_fpPutAAPixel)(int X, int Y, DWORD Col, WORD ALPHA);
	DWORD   (CDXSurface::* m_fpGetPixel)(int X, int Y);
	HRESULT (CDXSurface::* m_fpVLine)(int Y1, int Y2, int X, DWORD Col);
	HRESULT (CDXSurface::* m_fpHLine)(int X1, int X2, int Y, DWORD Col);
    HRESULT (CDXSurface::* fpDrawBlk)(CDXSurface* lpCDXS, LONG lDestX, LONG lDestY, RECT srcRect);
	HRESULT (CDXSurface::* fpDrawDDBlk)(CDXSurface* lpCDXS, LONG lDestX, LONG lDestY, RECT srcRect);
	HRESULT (CDXSurface::* fpDrawTrans)(CDXSurface* lpCDXS, LONG lDestX, LONG lDestY, RECT srcRect);
	HRESULT (CDXSurface::* fpDrawDDTrans)(CDXSurface* lpCDXS, LONG lDestX, LONG lDestY, RECT srcRect);
	HRESULT (CDXSurface::* fpDrawBlkHFlip)(CDXSurface* lpCDXS, LONG lDestX, LONG lDestY, RECT srcRect);
	HRESULT (CDXSurface::* fpDrawTransHFlip)(CDXSurface* lpCDXS, LONG lDestX, LONG lDestY, RECT srcRect);
	HRESULT (CDXSurface::* fpDrawBlkVFlip)(CDXSurface* lpCDXS, LONG lDestX, LONG lDestY, RECT srcRect);
	HRESULT (CDXSurface::* fpDrawTransVFlip)(CDXSurface* lpCDXS, LONG lDestX, LONG lDestY, RECT srcRect);
	HRESULT (CDXSurface::* fpDrawBlkAlphaFast)(CDXSurface* dest, LONG lDestX, LONG lDestY, RECT srcRect);
	HRESULT (CDXSurface::* fpDrawTransAlphaFast)(CDXSurface* dest, LONG lDestX, LONG lDestY, RECT srcRect);
	HRESULT (CDXSurface::* fpDrawBlkAlpha)(CDXSurface* dest, LONG lDestX, LONG lDestY, RECT srcRect, WORD ALPHA);
	HRESULT (CDXSurface::* fpDrawTransAlpha)(CDXSurface* dest, LONG lDestX, LONG lDestY, RECT srcRect, WORD ALPHA);
	HRESULT (CDXSurface::* fpDrawBlkShadow)(CDXSurface* dest, LONG lDestX, LONG lDestY, RECT srcRect, WORD SHADOW);
	HRESULT (CDXSurface::* fpDrawTransShadow)(CDXSurface* dest, LONG lDestX, LONG lDestY, RECT srcRect, WORD SHADOW);
	HRESULT (CDXSurface::* fpDrawBlkShadowFast)(CDXSurface* dest, LONG lDestX, LONG lDestY, RECT srcRect);
	HRESULT (CDXSurface::* fpDrawTransShadowFast)(CDXSurface* dest, LONG lDestX, LONG lDestY, RECT srcRect);
	HRESULT (CDXSurface::* fpDrawTransAlphaMask)(CDXSurface* dest, LONG lDestX, LONG lDestY, RECT srcRect);
	HRESULT (CDXSurface::* fpDrawTransScaled)(CDXSurface* dest, LONG lDestX, LONG lDestY, RECT srcRect, float scaleFactor);
	HRESULT (CDXSurface::* fpDrawBlkScaled)(CDXSurface* dest, LONG lDestX, LONG lDestY, RECT srcRect, float scaleFactor);
	HRESULT (CDXSurface::* fpDrawBlkStretched)(CDXSurface* lpCDXS, LONG lDestX, LONG lDestY, RECT srcRect, LONG width, LONG height);
	HRESULT (CDXSurface::* fpDrawTransStretched)(CDXSurface* lpCDXS, LONG lDestX, LONG lDestY, RECT srcRect, LONG width, LONG height);
	HRESULT (CDXSurface::* fpFade)(CDXSurface* dest, LONG lDestX, LONG lDestY, RECT srcRect, WORD ALPHA);

    // Specific video mode function declarations
    HRESULT PutPixel8(int X, int Y, DWORD Col);
    HRESULT PutPixel16(int X, int Y, DWORD Col);
    HRESULT PutPixel24(int X, int Y, DWORD Col);
    HRESULT PutPixel32(int X, int Y, DWORD Col);

    HRESULT PutAAPixel8(int X, int Y, DWORD Col, WORD ALPHA);
    HRESULT PutAAPixel15(int X, int Y, DWORD Col, WORD ALPHA);
    HRESULT PutAAPixel16(int X, int Y, DWORD Col, WORD ALPHA);
    HRESULT PutAAPixel24(int X, int Y, DWORD Col, WORD ALPHA);
    HRESULT PutAAPixel32(int X, int Y, DWORD Col, WORD ALPHA);

    DWORD   GetPixel8(int X, int Y);
    DWORD   GetPixel16(int X, int Y);
    DWORD   GetPixel24(int X, int Y);
    DWORD   GetPixel32(int X, int Y);

    HRESULT VLine8(int Y1, int Y2, int X, DWORD Col);
    HRESULT VLine16(int Y1, int Y2, int X, DWORD Col);
    HRESULT VLine24(int Y1, int Y2, int X, DWORD Col);
    HRESULT VLine32(int Y1, int Y2, int X, DWORD Col);

    HRESULT HLine8(int Y1, int Y2, int X, DWORD Col);
    HRESULT HLine16(int Y1, int Y2, int X, DWORD Col);
    HRESULT HLine24(int Y1, int Y2, int X, DWORD Col);
    HRESULT HLine32(int Y1, int Y2, int X, DWORD Col);

};

#endif
