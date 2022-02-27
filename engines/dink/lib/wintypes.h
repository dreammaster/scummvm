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

#ifndef DINK_LIB_WINTYPES_H
#define DINK_LIB_WINTYPES_H

#include "common/scummsys.h"
#include "dink/lib/rect.h"

namespace Dink {

typedef void *HFONT;
typedef void *HINSTANCE;
typedef uint32 HDC;
typedef uint32 DWORD;
typedef uint16 WORD;
typedef uint32 *LPDWORD;
typedef int32 *LONG_PTR;
typedef uint *UINT_PTR;
typedef void *HANDLE;
typedef void *HWND;

typedef uint32 HRESULT;
#define S_OK 0

#define lstrlen strlen
#define FAILED(hr) (((HRESULT)(hr)) < 0)

#define MCI_CLOSE 1
inline void mciSendCommand(int v1, int v2, int v3, void *v4) {}
inline int mciSendString(const char *lpstrCommand,
	const char *lpstrReturnString, uint uReturnLength, HWND hwndCallback) {
	return 0;
}
inline bool GetClientRect(HWND hWnd, LPRECT lpRect) { return false; }
inline void CloseHandle(HANDLE) {}

} // namespace Dink

#endif
