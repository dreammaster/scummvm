/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef DINK_DIRECTDRAW_DDRAW_H
#define DINK_DIRECTDRAW_DDRAW_H

#include "common/scummsys.h"
#include "common/textconsole.h"
#include "common/rect.h"
#include "graphics/managed_surface.h"
#include "graphics/screen.h"
#include "dink/lib/wintypes.h"
#include "dink/lib/rect.h"

namespace Dink {

#define DD_OK 0
#define DD_FALSE 1

// Errors
#define MAKE_DDHRESULT(X) (HRESULT)X
#define DDERR_SURFACELOST                       MAKE_DDHRESULT( 450 )
#define DDERR_WASSTILLDRAWING                   MAKE_DDHRESULT( 540 )
#define DIERR_INPUTLOST MAKE_DDHRESULT( 1 )

/*
 * DrawText() Format Flags
 */
#define DT_TOP                      0x00000000
#define DT_LEFT                     0x00000000
#define DT_CENTER                   0x00000001
#define DT_RIGHT                    0x00000002
#define DT_VCENTER                  0x00000004
#define DT_BOTTOM                   0x00000008
#define DT_WORDBREAK                0x00000010
#define DT_SINGLELINE               0x00000020
#define DT_EXPANDTABS               0x00000040
#define DT_TABSTOP                  0x00000080
#define DT_NOCLIP                   0x00000100
#define DT_EXTERNALLEADING          0x00000200
#define DT_CALCRECT                 0x00000400
#define DT_NOPREFIX                 0x00000800
#define DT_INTERNAL                 0x00001000

// DirectDraw return values
#define DD_OK 0

// Blit flags
#define DDBLT_DDFX                              0x00000800l
#define DDBLT_COLORFILL                         0x00000400l
#define DDBLT_WAIT                              0x01000000l
#define DDBLT_KEYSRC                            0x00008000l

// BltFast flags
#define DDBLTFAST_NOCOLORKEY                    0x00000000
#define DDBLTFAST_SRCCOLORKEY                   0x00000001
#define DDBLTFAST_DESTCOLORKEY                  0x00000002
#define DDBLTFAST_WAIT                          0x00000010
#define DDBLTFAST_DONOTWAIT                     0x00000020

// Lock modes
#define DDLOCK_DONOTWAIT                        0x00004000L

// Flip flags
#define DDFLIP_WAIT                          0x00000001L
#define DDFLIP_EVEN                          0x00000002L
#define DDFLIP_ODD                           0x00000004L
#define DDFLIP_NOVSYNC                       0x00000008L


enum BlankFlag {
	DDWAITVB_BLOCKBEGIN,
	DDWAITVB_BLOCKBEGINEVENT,
	DDWAITVB_BLOCKEND
};

#define DDBLTFX_NOTEARING 1

// ddsCaps flags
#define DDSD_CAPS               0x00000001l
#define DDSD_HEIGHT             0x00000002l
#define DDSD_WIDTH              0x00000004l
#define DDSCAPS_OFFSCREENPLAIN  0x00000040l

#define IDirectDraw_CreateSurface(p, a, b, c)       (p)->CreateSurface(a, b, c)

#include "common/pack-start.h"	// START STRUCT PACKING
struct PALETTEENTRY {
	byte peRed = 0;
	byte peGreen = 0;
	byte peBlue = 0;
	byte peFlags = 0;
} PACKED_STRUCT;
#include "common/pack-end.h"	// END STRUCT PACKING
typedef PALETTEENTRY *LPPALETTEENTRY;

struct IDirectDrawPalette {
	PALETTEENTRY _palette[PALETTE_SIZE];

	HRESULT GetEntries(uint unused, uint start, uint count, LPPALETTEENTRY dest);
	HRESULT SetEntries(uint unused, uint start, uint count, const LPPALETTEENTRY src);
	void Release();
};
typedef IDirectDrawPalette *LPDIRECTDRAWPALETTE;

struct DIRECTDRAWCLIPPER {
};
typedef DIRECTDRAWCLIPPER *LPDIRECTDRAWCLIPPER;

struct DDSCAPS {
	DWORD dwCaps;
};

struct DDSURFACEDESC {
	DWORD dwSize;
	DWORD dwFlags;
	DWORD dwHeight;
	DWORD dwWidth;
	DWORD lPitch;
	void *lpSurface;
	DDSCAPS ddsCaps;
};
typedef DDSURFACEDESC *LPDDSURFACEDESC;

struct DDBLTFX {
	uint32 dwSize;
	uint dwFillColor;
	int dwDDFX;
};
typedef DDBLTFX *LPDDBLTFX;

struct IDirectDrawSurface : public Graphics::ManagedSurface {
public:
	HRESULT Lock(const LPRECT rect, LPDDSURFACEDESC desc, uint32 flags, HANDLE handle);
	HRESULT Unlock(const LPRECT rect);
	void Release();
	HRESULT Restore();
	HRESULT BltFast(int16 x, int16 y, const IDirectDrawSurface *src,
		LPRECT rect, uint32 flags);
	HRESULT Blt(const LPRECT dstRect, const IDirectDrawSurface *src,
		const LPRECT srcRect, uint32 flags, LPDDBLTFX lpDDBltFx);
	HRESULT Flip(IDirectDrawSurface *surface, uint32 flags);
	HRESULT GetDC(HDC *hdc);
	HRESULT ReleaseDC(HDC hdc);
	HRESULT IsLost() const;
	HRESULT SetPalette(LPDIRECTDRAWPALETTE pal);
};
typedef IDirectDrawSurface *LPDIRECTDRAWSURFACE;


struct IDirectDraw {
	HRESULT WaitForVerticalBlank(BlankFlag flags, void *unused) {
		assert(!unused);
		return DD_OK;
	}
	HRESULT RestoreDisplayMode() {
		return DD_OK;
	}
	HRESULT Release() {
		return DD_OK;
	}
	HRESULT CreateSurface(LPDDSURFACEDESC ddsd, LPDIRECTDRAWSURFACE *surf, IUnknown *);
};
typedef IDirectDraw *LPDIRECTDRAW;

} // namespace Dink

#endif
