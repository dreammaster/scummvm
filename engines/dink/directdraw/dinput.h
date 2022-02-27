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

#ifndef DINK_DIRECTDRAW_DINPUT_H
#define DINK_DIRECTDRAW_DINPUT_H

#include "dink/lib/wintypes.h"

namespace Dink {

#define DIMOFS_X 1
#define DIMOFS_Y 2
#define DIMOFS_Z 3
#define DIDFT_BUTTON 4

struct DIDEVICEOBJECTDATA {
	DWORD dwOfs;
	DWORD dwData;
	DWORD dwTimeStamp;
	DWORD dwSequence;
	UINT_PTR uAppData;
};
typedef DIDEVICEOBJECTDATA *LPDIDEVICEOBJECTDATA;

struct IDirectInput {
	HRESULT Release();
};
typedef IDirectInput *LPDIRECTINPUT;

struct IDirectInputDevice {
	HRESULT GetDeviceData(DWORD dwSize, LPDIDEVICEOBJECTDATA rgdod,
		LPDWORD pdwInOut, DWORD dwFlags);
	HRESULT Acquire();
	HRESULT Unacquire();
	HRESULT Release();
};
typedef IDirectInputDevice *LPDIRECTINPUTDEVICE;

} // namespace Dink

#endif
