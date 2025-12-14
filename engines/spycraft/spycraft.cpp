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
#include "spycraft/vars.h"
#include "common/config-manager.h"
#include "common/debug-channels.h"
#include "common/system.h"
#include "engines/util.h"

namespace Spycraft {

SpycraftEngine *g_engine;
extern	int screen_width;
extern	int screen_height;
extern HDC hGameDC;

constexpr HINSTANCE hInstance = nullptr;
static bool posted = false;
static bool appActive = false;
static unsigned yct = 0;
static SRect screenRect = { 0, 0, 511, 479, 512, 480 };

class CTheApp : public CWinApp {
private:
	bool _posted = false;

public:
	bool InitInstance() override;
	bool OnIdle(long lCount) override;
};

bool CTheApp::InitInstance() {
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
	return true;
}

bool CTheApp::OnIdle(long lCount) {
	if (UserWantsToQuit && !_posted) {
		PostMessage(hGameWnd, WM_CLOSE, 0, 0);
		_posted = true;
	} else if (appActive) {
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
		Spycraft::OnIdle((MADEEventStamp *)&event);
	}

	return true;
}

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
	// Initialize graphics mode
	const Graphics::PixelFormat rgb565(2, 5, 6, 5, 0, 11, 5, 0, 0);
	initGraphics(GAME_WIDTH, GAME_HEIGHT, &rgb565);
	_screen = new Graphics::Screen();

	// Set the engine's debugger console
	setDebugger(new Console());

	// Set up data folder for file access
	SearchMan.addDirectory("data");

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

	CTheApp app;
	app.InitApplication();
	app.InitInstance();
	app.Run();

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

void SpycraftEngine::guiMessage(const Common::String &str) {
	GUIErrorMessage(str);
}

} // End of namespace Spycraft
