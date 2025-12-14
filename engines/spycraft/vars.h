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

#ifndef SPYCRAFT_VARS_H
#define SPYCRAFT_VARS_H

#include "spycraft/afxwin.h"
#include "spycraft/dmade/advmain.h"

namespace Spycraft {

#define APP_NAME        "Spycraft"

#define GAME_WIDTH      512
#define GAME_HEIGHT     480

// dmade/advini.cpp
extern int releaseVersion;
extern const char *debugText;

// dmade/diremade.cpp
extern HWND hGameWnd;
extern HINSTANCE hInst;
extern bool UserWantsToQuit;
extern const char *szAppName;
extern int offsetX;
extern int offsetY;
extern int window_right;
extern int window_bottom;
extern bool surfaceOK;
extern MADEEventStamp event;

// dmade/dirscreen.cpp
extern int screen_width;
extern int screen_height;
extern int screen_colors;
extern int scene_width;
extern int scene_height;
extern int spritelist_size;
extern HDC hGameDC;
extern HDC hSrcDC;

// dmade/winmemx.cpp
constexpr int __mem_index = 999;
extern long memAllocLimit;
extern int allow_heapCheck;

// game/dispatch.cpp
extern unsigned long absoluteTime;
extern unsigned long oldTime;
extern unsigned long elapsedTime;
extern bool suspended;


extern void init_vars();

} // End of namespace Spycraft

#endif // SPYCRAFT_H
