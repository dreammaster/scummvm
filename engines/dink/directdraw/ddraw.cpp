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
	blitFrom(*src, *rect, Common::Point(x, y));

	if (src->getPalette() != nullptr)
		setPalette(src->getPalette(), 0, PALETTE_COUNT);

	return DD_OK;
}

HRESULT IDirectDrawSurface::Blt(const LPRECT dstRect, const IDirectDrawSurface *src,
	const LPRECT srcRect, uint32 flags, LPDDBLTFX lpDDBltFx) {
	return DD_OK;
}

HRESULT IDirectDrawSurface::Flip(IDirectDrawSurface *, uint32 flags) {
	SWAP(lpDDSPrimary, lpDDSBack);

	if (lpDDSPrimary->getPalette() != nullptr)
		lpDDSPrimary->setScreenPalette();

	g_system->copyRectToScreen(lpDDSPrimary->getPixels(),
		lpDDSPrimary->w, 0, 0, lpDDSPrimary->w, lpDDSPrimary->h);
	g_system->updateScreen();

	return DD_OK;
}

void IDirectDrawSurface::setScreenPalette() {
	byte pal[PALETTE_SIZE];
	const uint32 *src = getPalette();
	byte *dest = pal;

	for (int i = 0; i < PALETTE_COUNT; ++i, ++src, dest += 3) {
		dest[0] = *src & 0xff;
		dest[1] = (*src >> 8) & 0xff;
		dest[2] = (*src >> 16) & 0xff;
	}

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
	surf->setPalette(reinterpret_cast<const uint32 *>(&pal->_palette[0]), 0, PALETTE_COUNT);
	return DD_OK;
}

} // namespace Dink
