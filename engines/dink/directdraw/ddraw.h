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
#include "common/rect.h"
#include "graphics/managed_surface.h"

namespace Dink {

#define DD_OK 0
#define DD_FALSE 1

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

typedef uint32 HDC;
typedef uint32 PALETTEENTRY;
typedef uint32 *LPDIRECTDRAWPALETTE;

typedef void *LPDIRECTSOUNDBUFFER;
typedef void *LPDIRECTSOUND;
typedef void *HFONT;
typedef void *HINSTANCE;
typedef void *LPDIRECTDRAW;
typedef uint32 HRESULT;

typedef void *LPDIRECTINPUT;
typedef void *LPDIRECTINPUTDEVICE;
typedef void *HANDLE;
typedef void *HWND;

struct DIRECTDRAWCLIPPER {
};
typedef DIRECTDRAWCLIPPER *LPDIRECTDRAWCLIPPER;

struct DIRECTDRAWSURFACE : public Graphics::ManagedSurface {
public:
	HRESULT Restore() {
		return DD_OK;
	}
};
typedef DIRECTDRAWSURFACE *LPDIRECTDRAWSURFACE;


inline void SetRect(Common::Rect *r, int16 x1, int16 y1, int16 x2, int16 y2) {
	*r = Common::Rect(x1, y1, x2, y2);
}

inline void OffsetRect(Common::Rect *r, int16 dx, int16 dy) {
	r->translate(dx, dy);
}

typedef uint32 COLORREF;
#define RGB(r,g,b) ((COLORREF)(((byte)(r)|((uint16)((byte)(g))<<8))|(((uint32)(byte)(b))<<16)))

inline COLORREF SetTextColor(HDC hdc, COLORREF color) {
	return color;
}

inline int DrawText(HDC hdc, const char *lpchText, int cchText,
	Common::Rect *lprc, uint format) {
	return 0;
}

} // namespace Dink

#endif
