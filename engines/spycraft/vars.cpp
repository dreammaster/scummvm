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

#include "spycraft/vars.h"

namespace Spycraft {

// dmade/diremade.cpp
const char *szAppName = APP_NAME;
HWND hGameWnd;
HINSTANCE hInst;
bool UserWantsToQuit;
int offsetX;
int offsetY;
int window_right;
int window_bottom;
bool surfaceOK;
MADEEventStamp event;

void init_vars() {
	// dmade/diremade.cpp
	UserWantsToQuit = false;
	offsetX = offsetY = 0;
	window_right = window_bottom = 0;
	surfaceOK = false;
}

} // End of namespace Spycraft
