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

#ifndef DINK_TYPES_H
#define DINK_TYPES_H

#include "common/scummsys.h"
#include "graphics/managed_surface.h"

namespace Dink {

typedef void *LPDIRECTSOUNDBUFFER;
typedef void *LPDIRECTSOUND;
typedef void *HFONT;
typedef void *HINSTANCE;
typedef void *LPDIRECTDRAW;
typedef uint32 HRESULT;

typedef uint32 PALETTEENTRY;
typedef uint32 *LPDIRECTDRAWPALETTE;

typedef void *LPDIRECTINPUT;
typedef void *LPDIRECTINPUTDEVICE;
typedef void *HANDLE;
typedef void *HWND;

typedef Graphics::ManagedSurface *LPDIRECTDRAWSURFACE;

struct DDBLTFX {
};

struct JOYINFOEX {
};

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

} // namespace Dink

#endif
