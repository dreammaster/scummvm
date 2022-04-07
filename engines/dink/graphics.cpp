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

#include "dink/graphics.h"
#include "dink/file.h"
#include "dink/globals.h"
#include "dink/text.h"
#include "dink/lib/wintypes.h"
#include "dink/directdraw/ddutil.h"
#include "common/unzip.h"
#include "graphics/fonts/ttf.h"

namespace Dink {

static Graphics::Font *currFont;
static COLORREF textColor;
static BkMode backMode;

void initfonts(const Common::String &fontName) {
	// lfHeight = 20, lfWeight = 400
	hfont = Graphics::loadTTFFontFromArchive("FreeSans.ttf", 20);
	// lfHeight = 18, lfWeight = 600
	hfont_small = Graphics::loadTTFFontFromArchive("FreeSans.ttf", 18);
}

void kill_fonts() {
	if (hfont) {
		delete hfont;
		hfont = nullptr;
	}
	if (hfont_small) {
		delete hfont_small;
		hfont_small = nullptr;
	}
}

HRESULT SelectObject(HDC hdc, HGDIOBJ h) {
	currFont = (Graphics::Font *)h;
	return S_OK;
}

COLORREF SetTextColor(HDC hdc, COLORREF color) {
	COLORREF oldColor = textColor;
	textColor = color;
	return oldColor;
}

HRESULT SetBkMode(HDC hdc, int bkMode) {
	backMode = (BkMode)bkMode;
	return S_OK;
}

COLORREF SetBkColor(HDC hdc, COLORREF color) {
	return S_OK;
}

int GetTextFace(HDC hdc, int c, const char *lpName) {
	return 0;
}

int DrawText(HDC hdc, const char *lpchText,
	int cchText, RECT *lprc, uint format) {
	Graphics::Surface dest = lpDDSPrimary->getSubArea(*lprc);
	currFont->drawString(&dest, lpchText, 0, 0, dest.w, textColor);

	return currFont->getFontHeight();
}


void flip_it_second() {
	DDBLTFX ddBltFx;
	RECT rcRectSrc;
	RECT rcRectDest;
	POINT p;

	if (!windowed) {
		while (1) {
			ddrval = lpDDSPrimary->Flip(NULL, DDFLIP_WAIT);
			if (ddrval == DD_OK) {
				break;
			}
			if (ddrval == DDERR_SURFACELOST) {
				if (ddrval != DD_OK) {
					break;
				}
			}
			if (ddrval != DDERR_WASSTILLDRAWING) {
			}
		}

	} else {
		//windowed mode, no flipping
		p.x = 0;
		p.y = 0;
		ClientToScreen(hWndMain, &p);
		GetClientRect(hWndMain, &rcRectDest);

		//rcRectDest.top += winoffset;
		rcRectDest.bottom = 480;
		rcRectDest.right = 640;

		OffsetRect(&rcRectDest, p.x, p.y);
		SetRect(&rcRectSrc, 0, 0, 640, 480);

		ddBltFx.dwSize = sizeof(ddBltFx);

		ddBltFx.dwDDFX = DDBLTFX_NOTEARING;
		ddrval = lpDDSPrimary->Blt(&rcRectDest, lpDDSBack,
			&rcRectSrc, DDBLT_DDFX | DDBLT_WAIT, &ddBltFx);
	}
}

void copy_bmp(const char *name) {
	if (!File::exists(name)) {
		Msg("Error: Can't find bitmap at %s.", name);
		return;
	}

	lpDDSTrick = DDLoadBitmap(lpDD, name, 0, 0);

	lpDDPal = DDLoadPalette(lpDD, name);
	if (lpDDPal)
		lpDDSPrimary->SetPalette(lpDDPal);

	lpDDSTrick = DDLoadBitmap(lpDD, name, 0, 0);
	abort_this_flip = true;

	RECT rcRect;
	SetRect(&rcRect, 0, 0, 640, 480);
	//HDC hdc;
	//char msg[200];

again:
	ddrval = lpDDSBack->BltFast(0, 0, lpDDSTrick,
		&rcRect, DDBLTFAST_NOCOLORKEY);

	if (ddrval == DDERR_WASSTILLDRAWING)
		goto again;

again1:
	ddrval = lpDDSTwo->BltFast(0, 0, lpDDSTrick,
		&rcRect, DDBLTFAST_NOCOLORKEY);

	if (ddrval == DDERR_WASSTILLDRAWING)
		goto again1;

	flip_it_second();
}

void show_bmp(char name[80], int showdot, int reserved, int script) {
	if (!File::exists(name)) {
		Msg("Error: Can't find bitmap at %s.", name);
		return;
	}

	lpDDSTrick = DDLoadBitmap(lpDD, name, 0, 0);

	lpDDPal = DDLoadPalette(lpDD, name);
	if (lpDDPal)
		lpDDSPrimary->SetPalette(lpDDPal);

	lpDDSTrick = DDLoadBitmap(lpDD, name, 0, 0);
	showb.active = true;
	showb.showdot = showdot;
	showb.script = script;

	abort_this_flip = true;

	RECT rcRect;
	SetRect(&rcRect, 0, 0, 640, 480);
	//HDC hdc;
	//char msg[200];

again:
	ddrval = lpDDSBack->BltFast(0, 0, lpDDSTrick,
		&rcRect, DDBLTFAST_NOCOLORKEY);

	if (ddrval == DDERR_WASSTILLDRAWING)
		goto again;

	flip_it_second();
}

} // namespace Dink
