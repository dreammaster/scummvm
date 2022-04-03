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

#ifndef DINK_DIRECTDRAW_DDUTIL_H
#define DINK_DIRECTDRAW_DDUTIL_H

#include "dink/lib/graphics.h"
#include "dink/directdraw/ddraw.h"

namespace Dink {

/**
 * Create a DirectDrawSurface from a bitmap resource.
 */
extern IDirectDrawSurface *DDLoadBitmap(IDirectDraw *pdd, LPCSTR szBitmap, int dx, int dy);

/**
 * Load a bitmap from a file or resource into a directdraw surface.
 * normaly used to re-load a surface after a restore.
 */
extern HRESULT DDReLoadBitmap(IDirectDrawSurface *surf, LPCSTR szBitmap);

/**
 * Draw a bitmap into a DirectDrawSurface
 */
extern HRESULT DDCopyBitmap(IDirectDrawSurface *surf, HBITMAP hbm, int x, int y, int dx, int dy);

/**
 * Create a DirectDraw palette object from a bitmap resoure
 *
 * if the resource does not exist or NULL is passed create a
 * default 332 palette.
 */
extern IDirectDrawPalette *DDLoadPalette(IDirectDraw *pdd, LPCSTR szBitmap);

/**
 * Convert a RGB color to a pysical color.
 */
extern DWORD DDColorMatch(IDirectDrawSurface *pdds, COLORREF rgb);

/**
 * Set a color key for a surface, given a RGB.
 * If you pass CLR_INVALID as the color key,
 * the pixel in the upper-left corner will be used.
 */
extern HRESULT DDSetColorKey(IDirectDrawSurface *pdds, COLORREF rgb);

} // namespace Dink

#endif
