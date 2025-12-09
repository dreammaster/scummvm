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

#include "spycraft/dmade/advprime.h"
#include "spycraft/afxwin.h"

namespace Spycraft {

extern HDC hGameDC;
extern HDC hSrcDC;
extern int scene_width;
extern int scene_height;
extern int offsetX;
extern int offsetY;

void sfxMDBitBlt() {
	BitBlt(hGameDC, offsetX, offsetY, scene_width, scene_height, hSrcDC, 0, 0, SRCCOPY);
}

void sfxMDDrawRECT(int x, int y, int w, int h, int c) {
	HPEN hPen;
	HGDIOBJ oldObj;
	HDC hdc = hGameDC;

	x += offsetX;
	y += offsetY;

	hPen = CreatePen(0, 1, c);
	SetMapMode(hdc, MM_ANISOTROPIC);
	SetROP2(hdc, R2_XORPEN);
	oldObj = SelectObject(hdc, hPen);

	MoveToEx(hdc, x, y, NULL);
	LineTo(hdc, x + w, y);
	LineTo(hdc, x + w, y + h);
	LineTo(hdc, x, y + h);
	LineTo(hdc, x, y);

	SelectObject(hdc, oldObj);
	DeleteObject(hPen);
}

} // namespace Spycraft
