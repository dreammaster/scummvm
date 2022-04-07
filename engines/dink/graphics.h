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

#ifndef DINK_LIB_GRAPHICS_H
#define DINK_LIB_GRAPHICS_H

#include "graphics/surface.h"
#include "dink/lib/wintypes.h"
#include "dink/lib/rect.h"

namespace Dink {

// SetBkMode flags
enum BkMode {
	BKMODE_NONE = 0,
	TRANSPARENT = 1,
	OPAQUE = 2
};

typedef void *HGDIOBJ;
typedef Graphics::Surface *HBITMAP;

typedef uint32 COLORREF;
#define RGB(r,g,b) ((COLORREF)(((byte)(r)|((uint16)((byte)(g))<<8))|(((uint32)(byte)(b))<<16)))

extern HRESULT SelectObject(HDC hdc, HGDIOBJ h);
extern HRESULT SetBkMode(HDC hdc, int mode);
extern COLORREF SetBkColor(HDC hdc, COLORREF color);
extern int GetTextFace(HDC hdc, int c, const char *lpName);
extern COLORREF SetTextColor(HDC hdc, COLORREF color);
extern int DrawText(HDC hdc, const char *lpchText,
	int cchText, RECT *lprc, uint format);


inline bool ClientToScreen(HWND hWnd, LPPOINT lpPoint) {
	return true;
}

extern void initfonts(const Common::String &fontName);
extern void kill_fonts();
extern void flip_it_second();
extern void copy_bmp(const char *name);
extern void show_bmp(char name[80], int showdot, int reserved, int script);

} // namespace Dink

#endif
