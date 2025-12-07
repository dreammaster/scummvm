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

#include "spycraft/dmade/advlib.h"
#include "spycraft/dmade/advres.h"
#include "spycraft/dmade/advcompat.h"
#include "spycraft/dmade/adverror.h"
#include "spycraft/dmade/advmem.h"
#include "spycraft/dmade/advdebug.h"
#include "spycraft/dmade/advmain.h"
#include "spycraft/dmade/advtext.h"
#include "spycraft/dmade/advsprite.h"
#include "spycraft/dmade/wintext.h"
#include "spycraft/dmade/advsound.h"
#include "spycraft/dmade/advcursor.h"
#include "spycraft/dmade/winscreen.h"
#include "spycraft/dmade/advmisc.h"
#include "spycraft/dmade/advmovie.h"
#include "spycraft/dmade/advtime.h"
#include "spycraft/dmade/advmusic.h"
#include "spycraft/afxwin.h"
#include "common/mfc/keyboard.h"

namespace Spycraft {

#define APP_NAME        "Spycraft"


#define GAME_WIDTH      512
#define GAME_HEIGHT     480

//	Instance, Window handles, and DC's used throughout program
HWND hGameWnd;
HINSTANCE hInst;

// variables for responding to user's request to quit
short UserWantsToQuit = false;
const char *szAppName = APP_NAME;
int offsetX = 0;
int offsetY = 0;
int window_right;
int window_bottom;
int surfaceOK = false;
MADEEventStamp event;

static int posted = false;
static int appActive = false;
static unsigned yct = 0;
static SRect screenRect = { 0, 0, 511, 479, 512, 480 };

extern int cursorHidden;

// external function definitions for dispatch.cpp 
extern void StartScript(void);
extern short OnRightButtonDown(MADEEventStamp *);	// Right mouse button down
extern short OnRightButtonUp(MADEEventStamp *);	   // Right mouse button up
extern short OnMiddleButtonDown(MADEEventStamp *);  // Middle mouse button down
extern short OnMiddleButtonUp(MADEEventStamp *);	   // Middle mouse button up
extern short OnLeftButtonDown(MADEEventStamp *);	   // Left mouse button down
extern short OnLeftButtonUp(MADEEventStamp *);	   // Left mouse button up
extern short OnKeyDown(MADEEventStamp *);		      // Key down
extern short OnIdle(MADEEventStamp *);		         // No event; Idle
//extern short OnDiskInsert ( MADEEventStamp* );	      // Disk has been inserted

void StopStreams(void);
void StopAllMusic(void);
void ReStartStreams(void);
void ReStartAllMusic(void);

/*
extern snServiceRef  gAOLServiceRef = NULL;

static bool netHandler(snMsg theMsg)
{
	char body[200], title[200];
	wsprintf(body, "Msg: type %d data %1x", theMsg->mMsgType, theMsg->mMsgData);
	wsprintf(title, "Msg id %1d service %1x", theMsg->mRequestID, theMsg->mServiceRef);
	MessageBox(NULL, body, title, MB_OK);

	snBrowserGotoURL(gAOLServiceRef, "http://www.activision.com");
	return true;
}
*/
extern int volumeBoxUp;
LRESULT CALLBACK GameWndProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT ps;

	switch (message) {
	case WM_CREATE:
		break;

	case WM_DESTROY:
		//sfxCleanMADE();
		PostQuitMessage(0);
		break;

	case WM_LBUTTONUP:
		event.cursor_pt.x = LOWORD(lParam) - offsetX;
		event.cursor_pt.y = HIWORD(lParam) - offsetY;
		OnLeftButtonUp((MADEEventStamp *)&event);
		break;

	case WM_LBUTTONDOWN:
		event.cursor_pt.x = LOWORD(lParam) - offsetX;
		event.cursor_pt.y = HIWORD(lParam) - offsetY;
		OnLeftButtonDown((MADEEventStamp *)&event);
		break;

	case WM_MBUTTONUP:
		event.cursor_pt.x = LOWORD(lParam) - offsetX;
		event.cursor_pt.y = HIWORD(lParam) - offsetY;
		OnMiddleButtonUp((MADEEventStamp *)&event);
		break;

	case WM_RBUTTONUP:
		event.cursor_pt.x = LOWORD(lParam) - offsetX;
		event.cursor_pt.y = HIWORD(lParam) - offsetY;
		OnRightButtonUp((MADEEventStamp *)&event);
		break;

	case WM_RBUTTONDOWN:
		event.cursor_pt.x = LOWORD(lParam) - offsetX;
		event.cursor_pt.y = HIWORD(lParam) - offsetY;
		OnRightButtonDown((MADEEventStamp *)&event);
		break;

	case WM_KEYDOWN:
		if (GetKeyState(VK_SHIFT) < 0)
			event.modifiers = 1L;
		else if (GetKeyState(VK_CONTROL) < 0)
			event.modifiers = 2L;
		else
			event.modifiers = 0L;
		event.message = wParam;
		OnKeyDown((MADEEventStamp *)&event);
		break;

	case WM_MOUSEMOVE:
	{
		int x = (int)LOWORD(lParam);
		int y = (int)HIWORD(lParam);

		event.cursor_pt.x = x - offsetX;
		event.cursor_pt.y = y - offsetY;
		UpdateCursor(event.cursor_pt.x, event.cursor_pt.y);

		/* NO GAME CURSOR SET YET */
		if (!haveSetGameCursor)
			break;

		if ((x < offsetX) || (x > window_right) ||
			(y < offsetY) || (y > window_bottom)) {
			/* OUTSIDE THE GAME WINDOW */
			if (turnedOffWindowsCursor) {
				sfxHideSprite(cursor);
				ShowCursor(true);
				turnedOffWindowsCursor = false;
			}
		}
		/* INSIDE THE GAME WINDOW */
		else {
			if (!turnedOffWindowsCursor && !cursorHidden) {
				sfxShowSprite(cursor);
				ShowCursor(false);
				turnedOffWindowsCursor = true;
			}
		}
	}
	break;

	case WM_PAINT:
		BeginPaint(hWnd, &ps);

		if (!sfxIsMovieOpen() && surfaceOK && curBack != -1)
			UpdateScreenRect(backgrounds[curBack]->animPort, &screenRect);

		EndPaint(hWnd, &ps);
		break;

	case WM_ACTIVATEAPP:
		appActive = wParam;
		if (appActive) {
			if (sfxIsMoviePlaying()) {
				sfxResumeMovie();
			} else {
				MSG msg;
				if (surfaceOK && curBack != -1)
					UpdateScreenRect(backgrounds[curBack]->animPort, &screenRect);
				sfxEnableSound();
				sfxEnableMusic();
				while (PeekMessage(&msg, NULL, WM_MOUSEFIRST, WM_MOUSELAST, PM_REMOVE));

			}
		} else {
			if (sfxIsMoviePlaying()) {
				sfxPauseMovie(false, false);
			} else {
				if (volumeBoxUp)
					volumeBoxUp = false;
				else {
					StopStreams();
					StopAllMusic();
					sfxDisableSound();
					sfxDisableMusic();
				}
			}
		}
		break;

	case WM_SYSCOMMAND:
		if (wParam == SC_SCREENSAVE)
			return true;
		break;


	default:
#ifdef TODO
		return DefWindowProc(hWnd, message, wParam, lParam);
#else
		warning("TODO: DefWindowProc");
#endif
	}

	return false;

}

bool InitFirstInstance(HINSTANCE hInstance) {
	WNDCLASS wc;

	wc.lpszClassName = szAppName;
	wc.style = 0;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = nullptr; // LoadIcon(hInstance, "Icon");
	wc.lpszMenuName = NULL;
	wc.hbrBackground = GetStockObject(BLACK_BRUSH);
	wc.hInstance = hInstance;
	wc.lpfnWndProc = GameWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
#ifdef TODO
	if (!RegisterClass(&wc))
		return (false);
#else
	warning("TODO: RegisterClass");
#endif

	return (true);

}

extern	int screen_width;
extern	int screen_height;

#ifdef TODO
int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpszCmdLine, int nShow) {
	MSG msg;

	hInst = hInstance;

	if (!hPrevInst) {
		if (!InitFirstInstance(hInstance)) {
			ErrMsg("Fatal error initializing program instance.");
			return(1);
		}
	}

	/* CHECK BITS-PER-PIXEL */
	if (GetDeviceCaps(GetDC(NULL), BITSPIXEL) != 16) {
		ErrMsg("This program requires 16-bit High-color mode.");
		return (1);
	}

	/* FIXED SIZE FOR FULL SCREEN MODE */
	screen_width = GetSystemMetrics(SM_CXSCREEN);
	screen_height = GetSystemMetrics(SM_CYSCREEN);

	/*if ( screen_width != 640 ) {
		int r =	MessageBox ( NULL, "This program performs best at 640 x 480 resolution.\nDo you want to keep the current resolution?",
							 "Change resolution", MB_ICONQUESTION | MB_YESNO );
		if ( r == IDNO ) {
			screen_width = 640;
			screen_height = 480 ;
		}
	}*/

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
		NULL,            // parent
		NULL,            // child window id
		hInstance,       // process instance
		NULL);
	if (hGameWnd == 0)
		return (1);

	ShowWindow(hGameWnd, SW_SHOWNORMAL);
	UpdateWindow(hGameWnd);

	/* SETUP THE GAME */
	hGameDC = GetDC(hGameWnd);
	sfxSetCacheSize(RES_ATS, 24);

	sfxInitMADE();

	/* instantiates C++ class object for script */
	StartScript();

	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
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
	if (!FreeLibrary(GetModuleHandle("actsock.dll")))
		ASSERT(0, 0);

	return (msg.wParam);
}
#endif

void sfxGetEvent(MADEEventStamp *ev) {
	POINT pCursor;
	MSG msg;

	GetCursorPos(&pCursor);
	ScreenToClient(hGameWnd, &pCursor);
	ev->cursor_pt.x = (SHORT)pCursor.x;
	ev->cursor_pt.y = (SHORT)pCursor.y;
	ev->clock_lo = sfxGetTime();

	if (GetKeyState(VK_SHIFT) < 0)
		ev->modifiers = 1L;
	else if (GetKeyState(VK_CONTROL) < 0)
		ev->modifiers = 2L;
	else
		ev->modifiers = 0L;

	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT || msg.message == WM_PAINT) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		} else
		{
			switch (msg.message)
			{
			case WM_LBUTTONUP:
				ev->get_event_message = (uint16)EVENT_LBUTTONUP;
				break;

			case WM_LBUTTONDOWN:
				ev->get_event_message = (uint16)EVENT_LBUTTONDOWN;
				break;

			case WM_MBUTTONUP:
				ev->get_event_message = (uint16)EVENT_MBUTTONUP;
				break;

			case WM_MBUTTONDOWN:
				ev->get_event_message = (uint16)EVENT_MBUTTONDOWN;
				break;

			case WM_RBUTTONUP:
				ev->get_event_message = (uint16)EVENT_RBUTTONUP;
				break;

			case WM_RBUTTONDOWN:
				ev->get_event_message = (uint16)EVENT_RBUTTONDOWN;
				break;

			case WM_KEYDOWN:
				ev->get_event_message = (uint16)EVENT_KEYDOWN;
				ev->message = msg.wParam;
				break;

			default:
				break;
			}
		}
	}

	return;
}

void sfxQuit(void) {
	UserWantsToQuit = true;
}

MADEEnvirons environment;

MADEEnvirons *sfxEnvirons(uint16 version) {
	environment.slow_system = 0;
	environment.screen_rect.top = 0;
	environment.screen_rect.left = 0;
	environment.screen_rect.bottom = (int16)GetSystemMetrics(SM_CYSCREEN);
	environment.screen_rect.right = (int16)GetSystemMetrics(SM_CXSCREEN);
	environment.cpu_load = 0;

	return &environment;
}

} // namespace Spycraft
