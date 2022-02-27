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


HRESULT DIRECTDRAWSURFACE::Lock(const LPRECT rect, LPDDSURFACEDESC desc, uint32 flags, HANDLE handle) {
	return DD_OK;
}

HRESULT DIRECTDRAWSURFACE::Unlock(const LPRECT rect) {
	return DD_OK;
}

void DIRECTDRAWSURFACE::Release() {
}

HRESULT DIRECTDRAWSURFACE::BltFast(int16 x, int16 y, const DIRECTDRAWSURFACE *src,
	LPRECT rect, uint32 flags) {
	return DD_OK;
}

HRESULT DIRECTDRAWSURFACE::Blt(const LPRECT dstRect, const DIRECTDRAWSURFACE *src,
	const LPRECT srcRect, uint32 flags, LPDDBLTFX lpDDBltFx) {
	return DD_OK;
}

HRESULT DIRECTDRAWSURFACE::Flip(DIRECTDRAWSURFACE *surface, uint32 flags) {
	return DD_OK;
}

HRESULT DIRECTDRAWSURFACE::GetDC(HDC *hdc) {
	return DD_OK;
}

HRESULT DIRECTDRAWSURFACE::ReleaseDC(HDC hdc) {
	return DD_OK;
}

} // namespace Dink
