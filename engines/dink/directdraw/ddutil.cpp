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

#include "common/file.h"
#include "image/bmp.h"
#include "dink/directdraw/ddutil.h"

namespace Dink {

static const Graphics::Surface *loadImage(Image::ImageDecoder &decoder,
		Common::SeekableReadStream &src) {
	if (decoder.loadStream(src))
		return decoder.getSurface();

	return nullptr;
}

IDirectDrawSurface *DDLoadBitmap(IDirectDraw * pdd, LPCSTR szBitmap, int dx, int dy) {
	IDirectDrawSurface *surf = new IDirectDrawSurface();
	assert(dx == 0 && dy == 0);

	if (DDReLoadBitmap(surf, szBitmap) == DD_OK) {
		return surf;
	} else {
		delete surf;
		return nullptr;
	}
}

HRESULT DDReLoadBitmap(IDirectDrawSurface *dds, LPCSTR szBitmap) {
	Common::File f;
	Common::String name(szBitmap);
	const Graphics::Surface *surf;
	Image::BitmapDecoder bmp;

	if (!f.open(name)) {
		warning("Could not open image - %s", szBitmap);
		return DD_FALSE;
	}

	if (name.hasSuffixIgnoreCase(".bmp") &&
		(surf = loadImage(bmp, f)) != nullptr) {
		// Do nothing
	} else {
		warning("Unknown image type - %s", szBitmap);
		return DD_FALSE;
	}

	dds->copyFrom(surf);

	return DD_OK;
}

HRESULT DDCopyBitmap(IDirectDrawSurface *surf, HBITMAP hbm, int x, int y, int dx, int dy) {
	if (dx == 0 && dy == 0) {
		surf->blitFrom(*hbm, Common::Point(0, 0));
	} else {
		surf->blitFrom(*hbm,
			Common::Rect(0, 0, hbm->w, hbm->h),
			Common::Rect(x, y, x + dx, y + dy)
		);
	}

	return S_OK;
}

IDirectDrawPalette *DDLoadPalette(IDirectDraw *pdd, LPCSTR szBitmap) {
#ifdef TODO
	IDirectDrawPalette *ddpal;
	int                 i;
	int                 n;
	int                 fh;
	HRSRC               h;
	LPBITMAPINFOHEADER  lpbi;
	RGBQUAD *prgb;

	//
	// get a pointer to the bitmap resource.
	//
	if (szBitmap && (h = FindResource(NULL, szBitmap, RT_BITMAP))) {
		lpbi = (LPBITMAPINFOHEADER)LockResource(LoadResource(NULL, h));
		if (!lpbi)
			OutputDebugString("lock resource failed\n");
		prgb = (RGBQUAD *)((byte *)lpbi + lpbi->biSize);

		if (lpbi == NULL || lpbi->biSize < sizeof(BITMAPINFOHEADER))
			n = 0;
		else if (lpbi->biBitCount > 8)
			n = 0;
		else if (lpbi->biClrUsed == 0)
			n = 1 << lpbi->biBitCount;
		else
			n = lpbi->biClrUsed;

		//
		//  a DIB color table has its colors stored BGR not RGB
		//  so flip them around.
		//
		for (i = 0; i < n; i++) {
			ape[i].peRed = prgb[i].rgbRed;
			ape[i].peGreen = prgb[i].rgbGreen;
			ape[i].peBlue = prgb[i].rgbBlue;
			ape[i].peFlags = 0;
		}
	} else if (szBitmap && (fh = _lopen(szBitmap, OF_READ)) != -1) {
		BITMAPFILEHEADER bf;
		BITMAPINFOHEADER bi;

		_lread(fh, &bf, sizeof(bf));
		_lread(fh, &bi, sizeof(bi));
		_lread(fh, ape, sizeof(ape));
		_lclose(fh);

		if (bi.biSize != sizeof(BITMAPINFOHEADER))
			n = 0;
		else if (bi.biBitCount > 8)
			n = 0;
		else if (bi.biClrUsed == 0)
			n = 1 << bi.biBitCount;
		else
			n = bi.biClrUsed;

		//
		//  a DIB color table has its colors stored BGR not RGB
		//  so flip them around.
		//
		for (i = 0; i < n; i++) {
			byte r = ape[i].peRed;
			ape[i].peRed = ape[i].peBlue;
			ape[i].peBlue = r;
		}
	}

	pdd->CreatePalette(DDPCAPS_8BIT, ape, &ddpal, NULL);

	return ddpal;
#endif
	//
	// build a 332 palette as the default.
	//
	IDirectDrawPalette *pal = new IDirectDrawPalette();
	PALETTEENTRY *ape = pal->_palette;

	for (int i = 0; i < PALETTE_SIZE; i++) {
		ape[i].peRed = (byte)(((i >> 5) & 0x07) * 255 / 7);
		ape[i].peGreen = (byte)(((i >> 2) & 0x07) * 255 / 7);
		ape[i].peBlue = (byte)(((i >> 0) & 0x03) * 255 / 3);
		ape[i].peFlags = (byte)0;
	}

	Common::File f;
	Image::BitmapDecoder bmp;

	if (f.open(szBitmap) && bmp.loadStream(f)) {
		const byte *srcP = bmp.getPalette();
		int palCount = bmp.getPaletteColorCount();
		PALETTEENTRY *destP = ape;

		for (int i = 0; i < palCount; ++i, srcP += 3, ++destP) {
			destP->peRed = srcP[0];
			destP->peGreen = srcP[1];
			destP->peBlue = srcP[2];
		}
	}

	return pal;
}

DWORD DDColorMatch(IDirectDrawSurface *pdds, COLORREF rgb) {
#ifdef TODO
	COLORREF rgbT;
	HDC hdc;
	DWORD dw = CLR_INVALID;
	DDSURFACEDESC ddsd;
	HRESULT hres;

	//
	//  use GDI SetPixel to color match for us
	//
	if (rgb != CLR_INVALID && pdds->GetDC(&hdc) == DD_OK) {
		rgbT = GetPixel(hdc, 0, 0);             // save current pixel value
		SetPixel(hdc, 0, 0, rgb);               // set our value
		pdds->ReleaseDC(hdc);
	}

	//
	// now lock the surface so we can read back the converted color
	//
	ddsd.dwSize = sizeof(ddsd);
	while ((hres = pdds->Lock(NULL, &ddsd, 0, NULL)) == DDERR_WASSTILLDRAWING)
		;

	if (hres == DD_OK) {
		dw = *(DWORD *)ddsd.lpSurface;                     // get DWORD
		if (ddsd.ddpfPixelFormat.dwRGBBitCount < 32)
			dw &= (1 << ddsd.ddpfPixelFormat.dwRGBBitCount) - 1;  // mask it to bpp
		pdds->Unlock(NULL);
	}

	//
	//  now put the color that was there back.
	//
	if (rgb != CLR_INVALID && pdds->GetDC(&hdc) == DD_OK) {
		SetPixel(hdc, 0, 0, rgbT);
		pdds->ReleaseDC(hdc);
	}

	return dw;
#else
	error("TODO");
#endif
}

HRESULT DDSetColorKey(IDirectDrawSurface * pdds, COLORREF rgb) {
#ifdef TODO
	DDCOLORKEY          ddck;

	ddck.dwColorSpaceLowValue = DDColorMatch(pdds, rgb);
	ddck.dwColorSpaceHighValue = ddck.dwColorSpaceLowValue;
	return pdds->SetColorKey(DDCKEY_SRCBLT, &ddck);
#else
	error("TODO");
#endif
}

} // namespace Dink
