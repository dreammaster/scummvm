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

#ifndef DUNGEONCRAFT_CDX_CDX_H
#define DUNGEONCRAFT_CDX_CDX_H

namespace DungeonCraft {
namespace CDX {

//////////////////////////////////////////////////////////////////////////////////
//
//   The following section if for building CDX as a DLL
//   Begin CDX DLL section 
//
//      NOTE: _CDX_BUILDDLL & _CDX_USEDLL should NOT be used together
//
#if defined(_CDX_BUILDDLL) && defined(_CDX_USEDLL)
#error Sorry, you cannot use _CDX_BUILDDLL & _CDX_USEDLL together
#endif

// this is initially defined as nothing
#define CDXAPI

// this should be specified to make CDX as a DLL
#ifdef _CDX_BUILDDLL
#undef CDXAPI
#define CDXAPI __declspec(dllexport)
#endif

// the following should be specified if the user wants to
// use cdx.dll.  Otherwise chaos ensues when you try to delete objects
#ifdef _CDX_USEDLL
#undef CDXAPI
#define CDXAPI __declspec(dllimport)
#endif
//
//   End CDX DLL section 
//
//////////////////////////////////////////////////////////////////////////////////

// If you want to support DX running under NT 4.0 (currently only 
// supporting DX3) then define NTDX3 in your program
#ifdef NTDX3

#define DIRECTDRAW_VERSION	      0x0300
#define DIRECTINPUT_VERSION	      0x0300
#define DIRECTSOUND_VERSION	      0x0300
#define CDX_LPDIRECTDRAWSURFACE   LPDIRECTDRAWSURFACE
#define CDX_LPDIRECTDRAW          LPDIRECTDRAW
#define CDX_DDSURFACEDESC         DDSURFACEDESC
#define CDX_LPDDSURFACEDESC       LPDDSURFACEDESC
#define CDX_DDSCAPS               DDSCAPS
#define CDX_LPDIRECTDRAWPALETTE   LPDIRECTDRAWPALETTE

#else

// CDX currently supports DX7
#define DIRECTDRAW_VERSION	      0x0700
#define DIRECTINPUT_VERSION	      0x0700
#define DIRECTSOUND_VERSION	      0x0700
#define CDX_LPDIRECTDRAWSURFACE   LPDIRECTDRAWSURFACE7
#define CDX_LPDIRECTDRAW          LPDIRECTDRAW7
#define CDX_DDSURFACEDESC         DDSURFACEDESC2
#define CDX_LPDDSURFACEDESC       LPDDSURFACEDESC2
#define CDX_DDSCAPS               DDSCAPS2
#define CDX_LPDIRECTDRAWPALETTE   LPDIRECTDRAWPALETTE

#endif

// CDX Currently support DirectX 7
#define CDX_DDVER		0x0700
#define CDX_DIVER		0x0700
#define CDX_DSVER		0x0700

// System includes
#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>
#include <stdio.h>

// DirectX includes
#include <ddraw.h>
#include <dinput.h>
#include <dsound.h>
#include <dplay.h>
#include <dplobby.h>

// CDX Includes
#include "cdxtypes.h"
#include "cdxlog.h"
#include "cdxstructs.h"
#include "cdxsurface.h"
#include "cdxscreen.h"


//////////////////////////////////////////////////////////////////////////////////
// For those lazy coders, #define CDXINCLUDEALL and all cdx headers will get 
// included. Otherwise, you need to #include all header files you will need 
// (depending on modules you are going to use)
//////////////////////////////////////////////////////////////////////////////////
#ifdef CDXINCLUDEALL
	#include "cdxfile.h"
	#include "cdxsurface.h"
	#include "cdxlayer.h"
	#include "cdxtile.h"
	#include "cdxmap.h"
	#include "cdxsprite.h"
	#include "cdxsound.h"
	#include "cdxmusic.h"
	#include "cdxinput.h"
	#include "cdxlink.h"
	#include "cdxresource.h"
	#include "cdxpacker.h"
    #include "cdxmidi.h"
    #include "cdximage.h"
#endif


//////////////////////////////////////////////////////////////////////////////////
// Globals
//////////////////////////////////////////////////////////////////////////////////
extern "C"
{
	// CDX Functions
	//extern CDXAPI void Clip(int *DestX, int *DestY, RECT *SrcRect, RECT *DestRect);

	// DDUTILS Functions
	extern CDXAPI HRESULT DDLoadPalette(CDX_LPDIRECTDRAW pdd, CDX_LPDIRECTDRAWPALETTE* ppPalette, LPCSTR szBitmap);
	extern CDXAPI CDX_LPDIRECTDRAWSURFACE DDLoadBitmap(CDX_LPDIRECTDRAW pdd, LPCSTR szBitmap, int dx, int dy, BOOL memoryType);
	extern CDXAPI CDX_LPDIRECTDRAWSURFACE DDLoadSizeBitmap(CDX_LPDIRECTDRAW pdd, LPCSTR szBitmap, int *dx, int *dy, BOOL memoryType);

	// Why no extern CDXAPI on this one???
	CDX_LPDIRECTDRAWSURFACE DDCreateSurface(CDX_LPDIRECTDRAW pdd, DWORD width, DWORD height, CDX_LPDDSURFACEDESC ddsd);

	// Why no CDXAPI on these ???
	extern HRESULT       DDReLoadBitmap(CDX_LPDIRECTDRAWSURFACE pdds, LPCSTR szBitmap);
	extern HRESULT       DDCopyBitmap(CDX_LPDIRECTDRAWSURFACE pdds, HBITMAP hbm, int x, int y, int dx, int dy);
	extern DWORD         DDColorMatch(CDX_LPDIRECTDRAWSURFACE pdds, COLORREF rgb);
	extern HRESULT       DDSetColorKey(CDX_LPDIRECTDRAWSURFACE pdds, COLORREF rgb);
    extern BOOL          GetRGBFormat(CDX_LPDIRECTDRAWSURFACE Surface, RGBFORMAT* rgb);
	extern BOOL __cdecl  IsMMX(void);

	// DirectX Errors
	extern CDXAPI void DDError(HRESULT hErr, void* hWnd, LPCSTR szFile, int iLine);
	extern CDXAPI void CDXError( CDXScreen *Screen, char *str, ...);
	extern CDXAPI void DSError(HRESULT hErr, void* hWnd);
	extern CDXAPI char* DXErrorToString( HRESULT hErr );
}

//////////////////////////////////////////////////////////////////////////////////
// MACROS
//////////////////////////////////////////////////////////////////////////////////
#define SAFEDELETE(x) if(x != NULL) { delete x; x = NULL; }
#define DELETEARRAY(x) if(x != NULL) { delete [] x; x = NULL; }
#define RELEASE(x) if(x != NULL) { x->Release(); x = NULL; }
#define ODS(a) OutputDebugString(a)

} // End of namespace CDX
} // End of namespace DungeonCraft

#endif
