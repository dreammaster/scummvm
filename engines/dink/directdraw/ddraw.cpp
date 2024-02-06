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

#include "dink/directdraw/ddraw.h"
#include "dink/dink.h"
#include "dink/globals.h"
#include "common/algorithm.h"
#include "graphics/palette.h"

#include "common/events.h"

namespace Dink {

HRESULT IDirectDraw::CreateSurface(LPDDSURFACEDESC ddsd, LPDIRECTDRAWSURFACE *surf, IUnknown *) {
	error("TODO: IDirectDraw::CreateSurface");
}


HRESULT IDirectDrawPalette::GetEntries(uint unused, uint start, uint count, LPPALETTEENTRY dest) {
	assert((start + count) <= PALETTE_SIZE);
	Common::copy(&_palette[start], &_palette[start] + count, dest);
	return DD_OK;
}

HRESULT IDirectDrawPalette::SetEntries(uint unused, uint start, uint count, const LPPALETTEENTRY src) {
	assert((start + count) <= PALETTE_SIZE);
	Common::copy(src, src + count, &_palette[start]);
	return DD_OK;
}

void IDirectDrawPalette::Release() {
}


IDirectDrawSurface::IDirectDrawSurface() {
	create(g_system->getWidth(), g_system->getHeight(),
		g_system->getScreenFormat());
}

IDirectDrawSurface::IDirectDrawSurface(int w, int h, const Graphics::PixelFormat &pformat) {
	if (w != 0 && h != 0)
		create(w, h, pformat);
}

HRESULT IDirectDrawSurface::Lock(const LPRECT rect, LPDDSURFACEDESC desc, uint32 flags, HANDLE handle) {
	return DD_OK;
}

HRESULT IDirectDrawSurface::Unlock(const LPRECT rect) {
	return DD_OK;
}

void IDirectDrawSurface::Release() {
}

HRESULT IDirectDrawSurface::Restore() {
	return DD_OK;
}

HRESULT IDirectDrawSurface::BltFast(int16 x, int16 y,
		const IDirectDrawSurface *src, LPRECT rect, uint32 flags) {
	assert(flags == DDBLTFAST_NOCOLORKEY);
	byte r, g, b;

	assert(src->format.bytesPerPixel == 1);
	byte srcPal[PALETTE_SIZE];
	src->grabPalette(srcPal, 0, PALETTE_COUNT);

	addDirtyRect(Common::Rect(x, y, x + rect->width(), y + rect->height()));

	for (int yp = rect->top; yp < rect->bottom; ++yp, ++y) {
		const byte *srcP = (const byte *)src->getBasePtr(rect->left, yp);
		byte *dest8 = (byte *)getBasePtr(x, y);
		uint32 *dest32 = (uint32 *)getBasePtr(x, y);

		for (int xp = 0; xp < rect->width(); ++xp, ++srcP) {
			r = srcPal[*srcP * 3];
			g = srcPal[*srcP * 3 + 1];
			b = srcPal[*srcP * 3 + 2];
			uint32 destPixel = format.RGBToColor(r, g, b);

			if (format.bytesPerPixel == 1)
				*dest8++ = destPixel;
			else
				*dest32++ = destPixel;
		}
	}

	return DD_OK;
}

HRESULT IDirectDrawSurface::Blt(const LPRECT dstRect, const IDirectDrawSurface *src,
		const LPRECT srcRect, uint32 flags, LPDDBLTFX lpDDBltFx) {
	assert(srcRect->width() == dstRect->width() && srcRect->height() == dstRect->height());

	if (format.bytesPerPixel != 1 && src->format.bytesPerPixel != 1) {
		rawBlitFrom(*src, *srcRect, Common::Point(dstRect->left, dstRect->top));
	} else {
		BltFast(dstRect->left, dstRect->top, src, srcRect, 0);
	}

	if (flags & DDBLT_WAIT) {
		assert(g_system->getScreenFormat() == this->format);
		g_system->copyRectToScreen(getPixels(), this->pitch, 0, 0, this->w, this->h);
		g_system->updateScreen();
	}

	return DD_OK;
}

HRESULT IDirectDrawSurface::Flip(IDirectDrawSurface *, uint32 flags) {
	SWAP(lpDDSPrimary, lpDDSBack);

	if (lpDDSPrimary->hasPalette() && lpDDSPrimary->format.bytesPerPixel == 1)
		lpDDSPrimary->setScreenPalette();

	g_system->copyRectToScreen(lpDDSPrimary->getPixels(),
		lpDDSPrimary->pitch, 0, 0, lpDDSPrimary->w, lpDDSPrimary->h);
	g_system->updateScreen();

	return DD_OK;
}

void IDirectDrawSurface::setScreenPalette() {
	byte pal[PALETTE_SIZE];
	grabPalette(pal, 0, PALETTE_COUNT);
	g_system->getPaletteManager()->setPalette(pal, 0, PALETTE_COUNT);
}

HRESULT IDirectDrawSurface::GetDC(HDC *hdc) {
	*hdc = this;
	return DD_OK;
}

HRESULT IDirectDrawSurface::ReleaseDC(HDC &hdc) {
	hdc = nullptr;
	return DD_OK;
}

HRESULT IDirectDrawSurface::IsLost() const {
	return empty() ? DDERR_SURFACELOST : DD_OK;
}

HRESULT IDirectDrawSurface::SetPalette(LPDIRECTDRAWPALETTE pal) {
	Graphics::ManagedSurface *surf = this;

	byte palette[PALETTE_SIZE];
	const PALETTEENTRY *srcP = pal->_palette;
	byte *destP = palette;
	for (int i = 0; i < PALETTE_COUNT; ++i, ++srcP) {
		*destP++ = srcP->peRed;
		*destP++ = srcP->peGreen;
		*destP++ = srcP->peBlue;
	}

	surf->setPalette(palette, 0, PALETTE_COUNT);
	return DD_OK;
}

HRESULT IDirectDrawSurface::SetColorKey(uint32 flags, LPDDCOLORKEY rgb) {
	return DD_OK;
}

} // namespace Dink
