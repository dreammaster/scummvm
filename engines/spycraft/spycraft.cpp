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

#include "spycraft/spycraft.h"
#include "spycraft/afxwin.h"
#include "spycraft/dmade/advmain.h"
#include "spycraft/dmade/advmisc.h"
#include "spycraft/dmade/advmovie.h"
#include "spycraft/dmade/advrect.h"
#include "spycraft/dmade/advscreen.h"
#include "spycraft/dmade/advsound.h"
#include "spycraft/dmade/advsprite.h"
#include "spycraft/dmade/advtime.h"
#include "spycraft/detection.h"
#include "spycraft/console.h"
#include "common/config-manager.h"
#include "common/debug-channels.h"
#include "common/system.h"
#include "engines/util.h"

namespace Spycraft {

SpycraftEngine *g_engine;
extern	int screen_width;
extern	int screen_height;
extern HDC hGameDC;

#define APP_NAME        "Spycraft"

#define GAME_WIDTH      512
#define GAME_HEIGHT     480

//	Instance, Window handles, and DC's used throughout program
HWND hGameWnd;
HINSTANCE hInst;

// variables for responding to user's request to quit
bool UserWantsToQuit = false;
const char *szAppName = APP_NAME;
int offsetX = 0;
int offsetY = 0;
int window_right;
int window_bottom;
int surfaceOK = false;
MADEEventStamp event;

constexpr HINSTANCE hInstance = nullptr;
static bool posted = false;
static bool appActive = false;
static unsigned yct = 0;
static SRect screenRect = { 0, 0, 511, 479, 512, 480 };

SpycraftEngine::SpycraftEngine(OSystem *syst, const ADGameDescription *gameDesc) : Engine(syst),
	_gameDescription(gameDesc), _randomSource("Spycraft") {
	g_engine = this;
}

SpycraftEngine::~SpycraftEngine() {
	delete _screen;
}

uint32 SpycraftEngine::getFeatures() const {
	return _gameDescription->flags;
}

Common::String SpycraftEngine::getGameId() const {
	return _gameDescription->gameId;
}

Common::Error SpycraftEngine::run() {
	// Initialize 320x200 paletted graphics mode
	initGraphics(320, 200);
	_screen = new Graphics::Screen();

	// Set the engine's debugger console
	setDebugger(new Console());

	// If a savegame was selected from the launcher, load it
	int saveSlot = ConfMan.getInt("save_slot");
	if (saveSlot != -1)
		(void)loadGameState(saveSlot);

	screen_width = GAME_WIDTH;
	screen_height = GAME_HEIGHT;

	sfxSetGameDimensions(GAME_WIDTH, GAME_HEIGHT, 356);
	offsetX = screen_width / 2 - GAME_WIDTH / 2;
	offsetY = screen_height / 2 - GAME_HEIGHT / 2;
	window_right = offsetX + GAME_WIDTH;
	window_bottom = offsetY + GAME_HEIGHT;

	hGameWnd = CreateWindow(szAppName,
		szAppName,
		WS_POPUP,
		0,               // x
		0,               // y
		GetSystemMetrics(SM_CXSCREEN),   // width
		GetSystemMetrics(SM_CYSCREEN),   // height
		nullptr,            // parent
		nullptr,            // child window id
		hInstance,       // process instance
		nullptr);
	assert(hGameWnd);

	ShowWindow(hGameWnd, SW_SHOWNORMAL);
	UpdateWindow(hGameWnd);

	/* SETUP THE GAME */
	hGameDC = GetDC(hGameWnd);
	sfxSetCacheSize(RES_ATS, 24);

	sfxInitMADE();

	/* instantiates C++ class object for script */
	StartScript();

	MSG msg;
	while (!shouldQuit()) {
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT)
				break;
			else {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		if (UserWantsToQuit) {
			if (!posted) {
				PostMessage(hGameWnd, WM_CLOSE, 0, 0);
				posted = true;
			}
		} else if (appActive) {
			//snSystemIdle();

			/* UPDATE MADE */
			if (curBack != -1) {
				sfxReleaseSprites(backgrounds[curBack]);
				sfxUpdate();
			}

			UpdateSound();

			if (((yct++) % 8) == 0)
				UpdateMovie();

			/* UPDATE FRAMEWORK */
			event.clock_lo = sfxGetTime();
			OnIdle((MADEEventStamp *)&event);
		}
	}

	sfxCleanMADE();

	return Common::kNoError;
}

Common::Error SpycraftEngine::syncGame(Common::Serializer &s) {
	// The Serializer has methods isLoading() and isSaving()
	// if you need to specific steps; for example setting
	// an array size after reading it's length, whereas
	// for saving it would write the existing array's length
	int dummy = 0;
	s.syncAsUint32LE(dummy);

	return Common::kNoError;
}

} // End of namespace Spycraft
