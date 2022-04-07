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
	Graphics::ManagedSurface *destSurf = (Graphics::ManagedSurface *)hdc;
	Graphics::Surface destArea = destSurf->getSubArea(*lprc);
	currFont->drawString(&destArea, lpchText, 0, 0, destArea.w, textColor);

	return currFont->getFontHeight();
}

void text_draw(int h, HDC hdc) {
	char crap[200];
	char *cr;
	RECT rcRect;
	int color = 0;

	if (spr[h].damage == -1) {
		//redink1 fix for : and '%deee bugs?
		strcpy(crap, spr[h].text);
		//sprintf(crap, "%s", spr[h].text);
		cr = &crap[0];
		color = 14;
		while (cr[0] == '`') {
			//color code at top
			if (cr[1] == '#') color = 13;
			if (cr[1] == '1') color = 1;
			if (cr[1] == '2') color = 2;
			if (cr[1] == '3') color = 3;
			if (cr[1] == '5') color = 5;
			if (cr[1] == '6') color = 6;
			if (cr[1] == '7') color = 7;
			if (cr[1] == '8') color = 8;
			if (cr[1] == '9') color = 9;
			if (cr[1] == '0') color = 10;
			if (cr[1] == '$') color = 14;
			if (cr[1] == '%') color = 15;
			//redink1 support for additional colors
			if (cr[1] == '@') color = 12;
			if (cr[1] == '!') color = 11;

			if (cr[1] == '4') color = 4;
			cr = &cr[2];
		}

		//Msg("Final is %s.",cr);
		if (spr[h].owner == 1000) {
			SetRect(&rcRect, spr[h].x, spr[h].y, spr[h].x + 620, spr[h].y + 400);
		} else {
			SetRect(&rcRect, spr[h].x, spr[h].y, spr[h].x + 150, spr[h].y + 150);

			if (spr[h].x + 150 > 620)
				OffsetRect(&rcRect, ((spr[h].x + 150) - 620) - (((spr[h].x + 150) - 620) * 2), 0);
		}
	} else {
		sprintf(crap, "%d", spr[h].damage);
		cr = &crap[0];
		if (spr[h].brain_parm == 5000)
			color = 14;

		if (spr[h].y < 0) spr[h].y = 0;
		SetRect(&rcRect, spr[h].x, spr[h].y, spr[h].x + 50, spr[h].y + 50);
	}

	if (truecolor && (bFadedDown || process_downcycle))
		color = 15;

	SetTextColor(hdc, RGB(8, 14, 21));
	if (spr[h].owner == 1200) {
		//this text has no sprite, and doesn't want to be centered.
		DrawText(hdc, cr, strlen(cr), &rcRect, DT_WORDBREAK);

		OffsetRect(&rcRect, -2, 0);
		DrawText(hdc, cr, strlen(cr), &rcRect, DT_WORDBREAK);

		OffsetRect(&rcRect, 1, 1);
		DrawText(hdc, cr, strlen(cr), &rcRect, DT_WORDBREAK);
		OffsetRect(&rcRect, 0, -2);
		DrawText(hdc, cr, strlen(cr), &rcRect, DT_WORDBREAK);

	} else {
		DrawText(hdc, cr, strlen(cr), &rcRect, DT_CENTER | DT_WORDBREAK);
		OffsetRect(&rcRect, -2, 0);
		DrawText(hdc, cr, strlen(cr), &rcRect, DT_CENTER | DT_WORDBREAK);

		OffsetRect(&rcRect, 1, 1);
		DrawText(hdc, cr, strlen(cr), &rcRect, DT_CENTER | DT_WORDBREAK);
		OffsetRect(&rcRect, 0, -2);
		DrawText(hdc, cr, strlen(cr), &rcRect, DT_CENTER | DT_WORDBREAK);
	}

	OffsetRect(&rcRect, 0, 1);

	//redink1 support for custom colors
	if (color >= 1 && color <= 15) {
		SetTextColor(hdc, RGB(font_colors[color].red, font_colors[color].green, font_colors[color].blue));
	} else {
		SetTextColor(hdc, RGB(255, 255, 255));
	}

	if (spr[h].owner == 1200)
		DrawText(hdc, cr, strlen(cr), &rcRect, DT_WORDBREAK);
	else
		DrawText(hdc, cr, strlen(cr), &rcRect, DT_CENTER | DT_WORDBREAK);
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
