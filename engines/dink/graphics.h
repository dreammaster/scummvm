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
#define TRANSPARENT 1
#define OPAQUE 2

typedef uint32 COLORREF;
#define RGB(r,g,b) ((COLORREF)(((byte)(r)|((uint16)((byte)(g))<<8))|(((uint32)(byte)(b))<<16)))

inline COLORREF SetTextColor(HDC hdc, COLORREF color) {
	return color;
}

inline int DrawText(HDC hdc, const char *lpchText, int cchText,
	RECT *lprc, uint format) {
	return 0;
}

/*
struct LOGFONT {
	int32 lfHeight;
	int32 lfWidth;
	int32 lfEscapement;
	int32 lfOrientation;
	int32 lfWeight;
	byte lfItalic;
	byte lfUnderline;
	byte lfStrikeOut;
	byte lfCharSet;
	byte lfOutPrecision;
	byte lfClipPrecision;
	byte lfQuality;
	byte lfPitchAndFamily;
	char lfFaceName[32];
};
*/
typedef void *HGDIOBJ;
typedef Graphics::Surface *HBITMAP;

extern HRESULT SelectObject(HDC hdc, HGDIOBJ h);
extern HRESULT SetBkMode(HDC hdc, int mode);
extern COLORREF SetBkColor(HDC hdc, COLORREF color);
extern int GetTextFace(HDC hdc, int c, const char *lpName);
inline bool ClientToScreen(HWND hWnd, LPPOINT lpPoint) {
	return true;
}

extern void initfonts(const Common::String &fontName);
extern void kill_fonts();

} // namespace Dink

#endif
