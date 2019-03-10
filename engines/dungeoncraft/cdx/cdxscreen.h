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

#ifndef CDXSCREEN_H
#define CDXSCREEN_H

#include "cdxclist.h"

// structure for videomodes
typedef struct 
{
    DWORD Width;
    DWORD Height;
    DWORD BPP;
    void* NextMode;
} CDX_VIDEOMODESSTRUCT;

// fix for those strange DDRAW header files where it is missing
#ifndef MAX_DDDEVICEID_STRING
	#define MAX_DDDEVICEID_STRING 512
#endif

// structure for direct draw drivers
struct CDX_DRIVERSTRUCT
{
	LPGUID	lpGUID;
	GUID	guid;
	char	lpName[MAX_DDDEVICEID_STRING];
	char	lpDesc[MAX_DDDEVICEID_STRING];
	DDCAPS	DDcaps;
    DDCAPS  HELcaps;

#if DIRECTDRAW_VERSION >= CDX_DDVER
	HMONITOR hMonitor;
#endif

	CDX_DRIVERSTRUCT *NextDriver;
};
		
// callback function for video mode enumeration, implemented in ddutils.cpp
HRESULT CALLBACK EnumDisplayModesCallback( CDX_LPDDSURFACEDESC lpDDSurfaceDesc, LPVOID lpContext  );

// callback function for devices enumeration, implemented in ddutils.cpp
#if DIRECTDRAW_VERSION >= CDX_DDVER
BOOL WINAPI DDEnumCallback( GUID FAR *lpGUID, LPSTR lpDesc, LPSTR lpName, LPVOID lpContext, HMONITOR hm );
#else
BOOL WINAPI DDEnumCallback( GUID FAR *lpGUID, LPSTR lpDesc, LPSTR lpName, LPVOID lpContext );
#endif

// Forward declaration of CDXSurface needed by the CDXScreen class
class CDXAPI CDXSurface;

//////////////////////////////////////////////////////////////////////////////////
// CDXScreen is the primary object of the library and every program that
// uses CDX must include a CDXScreen object.  As the name implies CDXScreen
// contains all the functions necessary to control the video display,
// currently this also includes functions to control the palette.
//////////////////////////////////////////////////////////////////////////////////
class CDXAPI CDXScreen
{
public:
	CDXScreen();
	virtual ~CDXScreen();

  HRESULT SetClipperWindow(HWND hWnd);
  HRESULT RestoreClipper();

	HRESULT CreateFullScreen(void* hWnd, DWORD Width, DWORD Height, DWORD BPP, BOOL bVGA = FALSE);
	HRESULT CreateWindowed(void* hWnd, int Width, int Height, BOOL UseClipper);
	HRESULT LoadBitmap(const char* szFilename);
	HRESULT LoadPalette(const char* szFilename);
	HRESULT Fill(DWORD FillColor);
	HRESULT Restore(void);
	HRESULT Flip(BOOL VSync=TRUE, BOOL FlipWithStretch=TRUE, BOOL displayFPS = FALSE);
	HRESULT SetColor(int col, int r, int g, int b);
	HRESULT GetColor(int col, int* r, int* g, int* b);
	HRESULT SetPalette(int Start, int Count, LPPALETTEENTRY lpPE);
	HRESULT GetPalette(int Start, int Count, LPPALETTEENTRY lpPE);
	HRESULT FillPalette(int r, int g, int b);
	HRESULT GreyScale(void);
	HRESULT FadeIn(int delay, LPPALETTEENTRY lpPE);
	HRESULT FadeOut(int delay);
	HRESULT FadeTo(int r, int g, int b, int delay);
	int     GetWidth(void)  { return m_dwPixelWidth; }
	int     GetHeight(void) { return m_dwPixelHeight; }
	int     GetBPP(void)    { return m_BPP; }
	CDXSurface* GetFront(void) { return m_lpDDSFront; }
	CDXSurface* GetBack(void) { return m_lpDDSBack; }
	LPDIRECTDRAWPALETTE GetPalette(void) { return m_lpDDPalette; }
    DWORD   GetNumberOfVideoModes( void );
    BOOL    GetVideoModeInformation( DWORD Index, DWORD* Width , DWORD* Height , DWORD* BPP );
    BOOL    CheckIfVideoModeExists( DWORD Width, DWORD Height , DWORD BPP );
	HRESULT WaitForVerticalBlank( void );
	void    CreateBackBufferInVRAM( BOOL Flag ) { m_BackBufferInVRAM = Flag; };
	void    SetTripleBuffering( BOOL Flag ) { m_TripleBuffering = Flag; }
	BOOL    GetTripleBuffering( void ) { return m_TripleBuffering; }
    HRESULT ChangeVideoMode(DWORD Width, DWORD Height,DWORD BPP, BOOL bVGA = FALSE);
    DWORD   MakeColor( DWORD Color , CDXSurface* Surface = NULL );
	void    AddSurfaceToList( CDXSurface* Surface );
	void    ReleaseAllSurfaces( void );
	void    ReleaseSurfaceFromList( CDXSurface* Surface );
	HRESULT SelectDriver( DWORD Index );
	HRESULT FadeToBlack(WORD duration);
	HRESULT FadeToSource(WORD duration);
    BOOL    IsFullScreen(void) {return m_bFullScreen;}
    void*   GetWindowHandle(void) {return m_hWnd;}
	CDX_LPDIRECTDRAW GetDD(void) { return m_lpDD; }
    HRESULT SaveAllSurfaces(void);
    HRESULT ReloadAllSurfaces(void);
    LONG    GetNextId(void);
	LONG    GetFPS(void) {return m_fps;}

protected:
    void    InitCDXScreen();
	HRESULT EnumVideoModes( void );
	HRESULT EnumDrivers( void );
    void    SetSurfaceScreen(CDXSurface* surface);
    void    SetSurfacePixelFormat(CDXSurface* surface);
    void    SetSurfaceWidth(CDXSurface* surface, int w);
    void    SetSurfaceHeight(CDXSurface* surface, int h);
	void    SetSurfaceDDSPointer(CDXSurface* pCDXS, CDX_LPDIRECTDRAWSURFACE lpdds);

private:

	CDX_LPDIRECTDRAW          m_lpDD;              // the DirectDraw object
    CDX_LPDIRECTDRAWSURFACE   m_ZBuffer;           // Zbuffer surface pointer
	CDXSurface*            m_lpDDSFront;           // a CDXSurface object for the front buffer
	CDXSurface*            m_lpDDSBack;            // a CDXSurface object for the back buffer
	LPDIRECTDRAWPALETTE    m_lpDDPalette;          // the DirectDraw palette object
	LPDIRECTDRAWCLIPPER    m_lpClipper;            // the DirectDraw clipper, used for window mode only
	DWORD                  m_dwPixelWidth;         // the screen width, in pixels
	DWORD                  m_dwPixelHeight;        // the screen height, in pixels
	DWORD                  m_BPP;                  // bits per pixel (colour depth)
	void*                  m_hWnd;                 // pointer to the HWND handle of the window
	BOOL                   m_bFullScreen;          // is the program running in fullscreen mode?
    DWORD                  m_NumberOfVideoModes;   // number of video modes the video adapter supports
    CDX_VIDEOMODESSTRUCT*  m_VideoModes;           // linked list with all video modes informations
	BOOL                   m_TripleBuffering;      // is triple buffering enabled ?
	BOOL                   m_BackBufferInVRAM;     // Should the back buffer be placed in video memory?
    DWORD                  m_NumberOfDrivers;      // number of grafikcards installed
    CDX_DRIVERSTRUCT*      m_Drivers;              // linked list with grafikcard informations
    DWORD                  m_SelectedDriver;       // Which driver will be used for creating DirectDraw
	DWORD                  m_frameCount;           // A count of how many times you have called Flip()
	DWORD                  m_accumulatedFrameTime; // How long since the last time you called Flip()
	DWORD                  m_frameStartTime;       // The time when the frame started
	LONG                   m_fps;                  // The frames per second rate
    CDXCList<CDXSurface>   m_SurfaceList;          // Keeps track of all CDXSurfaces created
    static LONG            m_lastId;
    BOOL m_UseClipper;    
};

#endif
