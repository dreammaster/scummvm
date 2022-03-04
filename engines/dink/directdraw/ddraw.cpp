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

HRESULT IDirectDrawSurface::BltFast(int16 x, int16 y, const IDirectDrawSurface *src,
	LPRECT rect, uint32 flags) {
	return DD_OK;
}

HRESULT IDirectDrawSurface::Blt(const LPRECT dstRect, const IDirectDrawSurface *src,
	const LPRECT srcRect, uint32 flags, LPDDBLTFX lpDDBltFx) {
	return DD_OK;
}

HRESULT IDirectDrawSurface::Flip(IDirectDrawSurface *surface, uint32 flags) {
	return DD_OK;
}

HRESULT IDirectDrawSurface::GetDC(HDC *hdc) {
	return DD_OK;
}

HRESULT IDirectDrawSurface::ReleaseDC(HDC hdc) {
	return DD_OK;
}

HRESULT IDirectDrawSurface::IsLost() const {
	return empty() ? DDERR_SURFACELOST : DD_OK;
}

HRESULT IDirectDrawSurface::SetPalette(LPDIRECTDRAWPALETTE pal) {
	error("TODO");
}

} // namespace Dink
