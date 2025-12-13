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

#include "spycraft/dmade/advlib.h"
#include "spycraft/dmade/advport.h"
#include "spycraft/afxwin.h"

namespace Spycraft {

Viewport *AllocPort(int width, int height, int colors) {
	Viewport *port;
	int rowBytes;

	port = (Viewport *)AllocPtr(sizeof(Viewport));
	if (port == NULL)
		return NULL;

	rowBytes = width;
	if (colors == 16)
		rowBytes *= 2;
	port->width = width;
	port->height = height;
	port->colors = colors;
	port->rowBytes = (uint16)rowBytes;
	port->origX = 0;
	port->origY = 0;
	port->ptr = AllocPtr(rowBytes * height);
	if (port->ptr == NULL) {
		FreePtr(port);
		return NULL;
	}

	return port;
}

void FreePort(Viewport *port) {
	if (port->ptr != NULL)
		FreePtr(port->ptr);

	FreePtr(port);
}

Viewport *AllocMDPort(int width, int height, int colors) {
	int i;
	WinViewport *port;

	struct {
		BITMAPINFOHEADER bmiHeader;
		RGBQUAD ctab[256];
	} bmi;

	if (colors != 8 && colors != 16) {
		ASSERT(false, __ERR_CODING);
	}

	port = (WinViewport *)AllocPtr(sizeof(WinViewport));
	ASSERT(port, __ERR_PORT_ALLOC_FAIL);

	port->vp.width = width;
	port->vp.height = height;
	port->vp.colors = colors;

	bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
	bmi.bmiHeader.biWidth = width;
	bmi.bmiHeader.biHeight = (-1) * height;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = colors;
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biSizeImage = 0;
	bmi.bmiHeader.biXPelsPerMeter = 0;
	bmi.bmiHeader.biYPelsPerMeter = 0;
	bmi.bmiHeader.biClrUsed = 0;
	bmi.bmiHeader.biClrImportant = 0;

	for (i = 0; i < 256; i++) {
		bmi.ctab[i].rgbRed = 0;
		bmi.ctab[i].rgbGreen = 0;
		bmi.ctab[i].rgbBlue = 0;
	}
#ifdef TODO
	hBitmap = CreateDIBSection(hSrcDC, (BITMAPINFO *)&bmi, DIB_RGB_COLORS,
		&ptr, NULL, 0);

	ASSERT(hBitmap, __ERR_WIN_API_FAIL);
	ASSERT(ptr, __ERR_WIN_API_FAIL);

	port->vp.ptr = ptr;
	port->vp.rowBytes = width * (port->vp.colors == 8 ? 1 : 2);
	port->hBitmap = hBitmap;
#else
	error("TODO: AllocMDPort");
#endif
	return (Viewport *)port;
}

void FreeMDPort(Viewport *port) {
	WinViewport *winport = (WinViewport *)port;
	if (winport != NULL) {
		ASSERT((void *)(DeleteObject(winport->hBitmap)), __ERR_WIN_API_FAIL);
		FreePtr(winport);
	}
}

} // namespace Spycraft
