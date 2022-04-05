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
#include "dink/globals.h"
#include "dink/lib/wintypes.h"
#include "common/unzip.h"
#include "graphics/fonts/ttf.h"

namespace Dink {

HRESULT SelectObject(HDC hdc, HGDIOBJ h) {
	return S_OK;
}

HRESULT SetBkMode(HDC hdc, int bkMode) {
	return S_OK;
}

COLORREF SetBkColor(HDC hdc, COLORREF color) {
	return S_OK;
}

int GetTextFace(HDC hdc, int c, const char *lpName) {
	return 0;
}

void initfonts(const Common::String &fontName) {
/*	Common::Archive *archive;
	if ((archive = Common::makeZipArchive("fonts.dat")) == nullptr)
		error("Could not locate fonts");
*/
	// 
	// TODO: Set up fonts most matching FreeDink

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

} // namespace Dink
