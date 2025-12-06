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

#include "spycraft/dmade/gamebox.h"
#include "spycraft/dmade/advlib.h"
#include "spycraft/dmade/advmain.h"
#include "spycraft/dmade/advfile.h"
#include "spycraft/dmade/advscreen.h"
#include "spycraft/dmade/advcursor.h"
#include "spycraft/dmade/advsound.h"
#include "spycraft/dmade/advmusic.h"
#include "spycraft/dmade/advmovie.h"
#include "spycraft/afxwin.h"

namespace Spycraft {

#define WIN_EXEC_SUCCESS 31

#define WANTS_TO_SAVE                    2
#define WANTS_TO_RESTORE	             4
#define WANTS_TO_CHANGE_VOL	             8
#define THINKING_ABOUT_QUITTING         10
#define WANTS_TO_CANCEL		            12
#define DOESNT_WANT_TO_QUIT		        -1

HWND hVolumeControl;

int gameBoxChoice;
int userChoice = -1;
int volumeBoxUp = false;


// flags controlling movie replay, save game, and restore game
int enableSaveGame = true;
int enableRestoreGame = true;

extern HINSTANCE hInst;
extern HWND hGameWnd;

bool CALLBACK FindRunningPrograms(HWND, LPARAM);
intptr GameBoxProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);

int sfxRunProgram(const char *programName) {
#ifdef TODO
	int ret;
	STARTUPINFO startInfo;
	PROCESS_INFORMATION processInfo;

	startInfo.cb = sizeof(STARTUPINFO);
	startInfo.lpReserved = NULL;
	startInfo.lpDesktop = NULL;
	startInfo.lpTitle = NULL;
	startInfo.dwX = 0;
	startInfo.dwY = 0;
	startInfo.dwXSize = 0;
	startInfo.dwYSize = 0;
	startInfo.dwXCountChars = 0;
	startInfo.dwYCountChars = 0;
	startInfo.dwFillAttribute = 0;
	startInfo.dwFlags = 0;
	startInfo.wShowWindow = 0;
	startInfo.cbReserved2 = 0;
	startInfo.lpReserved2 = NULL;
	startInfo.hStdInput = NULL;
	startInfo.hStdOutput = NULL;
	startInfo.hStdError = NULL;

	ret = CreateProcess(

		NULL,	// address of module name 
		programName,	// address of command line 
		NULL,	// address of process security attributes 
		NULL,	// address of thread security attributes 
		false,	// new process inherits handles 
		CREATE_DEFAULT_ERROR_MODE,	// creation flags 
		NULL,	// address of new environment block 
		NULL,	// address of current directory name 
		&startInfo,	// address of STARTUPINFO 
		&processInfo 	// address of PROCESS_INFORMATION  
	);

	if (ret)
		return true;
	else
		return false;
#else
	error("TODO: CreateProcess %s", programName);
#endif
}

HWND hCurDlg;

intptr CALLBACK GameBoxProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam) {
	POINT pCursor;
	RECT r;
	int x, y, width, height;
	int scrWidth, scrHeight;
	int rightEdge, bottomLine;

	switch (msg) {
	case WM_INITDIALOG:
		hCurDlg = hDlg;

		// set enable for movie replay, save game, and restor game

		if (!enableSaveGame)
			EnableWindow(GetDlgItem(hDlg, IDSAVE), false);

		if (!enableRestoreGame)
			EnableWindow(GetDlgItem(hDlg, IDRESTORE), false);

		scrWidth = GetSystemMetrics(SM_CXSCREEN);
		scrHeight = GetSystemMetrics(SM_CYSCREEN);
		rightEdge = (scrWidth - scene_width) / 2 + scene_width;
		bottomLine = (scrHeight - scene_height) / 2 + scene_height;
		GetWindowRect(hDlg, &r);
		width = r.right - r.left + 1;
		height = r.bottom - r.top + 1;
		GetCursorPos(&pCursor);
		x = pCursor.x;
		y = pCursor.y;
		while ((x + width) >= rightEdge)
			x--;
		while ((y + height) >= bottomLine)
			y--;
		MoveWindow(hDlg, x, y, width, height, false);
		return true;

	case WM_COMMAND:
		gameBoxChoice = (int)wParam;
		switch (wParam) {

		case IDSAVE:
			userChoice = WANTS_TO_SAVE;
			EndDialog(hDlg, true);
			return true;

		case IDRESTORE:
			userChoice = WANTS_TO_RESTORE;
			EndDialog(hDlg, true);
			return true;

		case IDVOL:
			userChoice = WANTS_TO_CHANGE_VOL;
			EndDialog(hDlg, true);
			return true;

		case IDQUIT:
			userChoice = THINKING_ABOUT_QUITTING;
			EndDialog(hDlg, true);
			return true;

		case IDCANCEL:
			userChoice = WANTS_TO_CANCEL;
			EndDialog(hDlg, true);
			return true;
		}
		return true;

	case WM_CLOSE:
		EndDialog(hDlg, true);
		break;

	case WM_DESTROY:
		EndDialog(hDlg, true);
		return true;
	}
	return false;
}

int sfxGameBox() {
	MSG msg;
	int numChars;
	char title[256];
	int giveBackMadeCursor = 0;

	if (turnedOffWindowsCursor)
		giveBackMadeCursor = true;

	sfxSystemCursor(SYS_ARROW);
	DialogBox(hInst, MAKEINTRESOURCE(GAMEBOX), hGameWnd, GameBoxProc);

	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	switch (userChoice) {
	case WANTS_TO_SAVE:
		SaveDlg("sgm");
		break;

	case WANTS_TO_RESTORE:
		OpenDlg("sgm");
		break;

	case WANTS_TO_CHANGE_VOL:
#ifdef TODO
	{
		char fullDir[128];
		GetWindowsDirectory(fullDir, sizeof(fullDir));
		strcat(fullDir, "\\sndvol32.exe");
		volumeBoxUp = true;
		EnumWindows(FindRunningPrograms, 0L);
		if (hVolumeControl != NULL) {
			if (IsIconic(hVolumeControl))
				ShowWindow(hVolumeControl, SW_RESTORE);
			else
				BringWindowToTop(hVolumeControl);
		} else
			sfxRunProgram(fullDir);
	}
#else
		error("TODO: WANTS_TO_CHANGE_VOL");
#endif
		break;

	case THINKING_ABOUT_QUITTING:
		numChars = LoadString(hInst, EXIT_TITLE, title, sizeof(title));
		if (numChars == 0)
			Common::strcpy_s(title, "Spy-Lingo Talking, Washington D.C. Type Person...");
		if (MessageBox(hGameWnd, "Do you really want to quit?", title, MB_SETFOREGROUND | MB_YESNO)
			!= IDYES)
			gameBoxChoice = DOESNT_WANT_TO_QUIT;

		break;

	case WANTS_TO_CANCEL:
		break;

	default:
		break;
	}

	if (giveBackMadeCursor)
		sfxMadeCursor();

	return (gameBoxChoice);
}

void sfxEnableSaveGame(int saveGame) {
	if (!saveGame)
		enableSaveGame = false;
	else
		enableSaveGame = true;
}

void sfxEnableRestoreGame(int restoreGame) {
	if (!restoreGame)
		enableRestoreGame = false;
	else
		enableRestoreGame = true;
}

bool CALLBACK FindRunningPrograms(HWND hWnd, LPARAM lParam) {
	char title[100];
	char *ptr;

	GetWindowText(hWnd, title, sizeof(title));
	strUpper(title);

	hVolumeControl = NULL;
	if ((ptr = strstr(title, "VOLUME"))) {
		hVolumeControl = hWnd;
		return (false);
	}

	return (true);
}

} // namespace Spycraft
