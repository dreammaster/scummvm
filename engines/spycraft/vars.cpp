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
#include "spycraft/dmade/dirscreen.h"

namespace Spycraft {

// dmade/advini.cpp
int releaseVersion;
const char *debugText;

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

// dmade/dirscreen.cpp
int screen_width;
int screen_height;
int screen_colors;
int scene_width;
int scene_height;
int spritelist_size;
HDC hGameDC;
HDC hSrcDC;

// dmade/winmemx.cpp
int __mem_index;
long memAllocLimit;
int allow_heapCheck;

// game/dispatch.cpp
unsigned long absoluteTime;
unsigned long oldTime;
unsigned long elapsedTime;
bool suspended;


void init_vars() {
	// dmade/advini.cpp
	releaseVersion = 0;
	debugText = nullptr;

	// dmade/diremade.cpp
	UserWantsToQuit = false;
	offsetX = offsetY = 0;
	window_right = window_bottom = 0;
	surfaceOK = false;

	// dmade/winscreen.cpp
	screen_width = screen_height = 0;
	screen_colors = 0;
	scene_width = DEFAULT_WIDTH;
	scene_height = DEFAULT_HEIGHT;
	spritelist_size = DEFAULT_SPRITELIST_SIZE;
	hGameDC = nullptr;
	hSrcDC = nullptr;

	// dmade/winmemx.cpp
	__mem_index = 1;
	memAllocLimit = 4L * 1024 * 1024;
	allow_heapCheck = 1;

	// game/dispatch.cpp
	absoluteTime = oldTime = elapsedTime = 0;
	suspended = false;
}

} // End of namespace Spycraft
