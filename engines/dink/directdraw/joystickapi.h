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

#ifndef DINK_DIRECTDRAW_JOYSTICKAPI_H
#define DINK_DIRECTDRAW_JOYSTICKAPI_H

#include "common/scummsys.h"

namespace Dink {

enum {
	JOYERR_UNPLUGGED = -1,
	JOY_RETURNALL = 0,
	JOYSTICKID1 = 1
};

struct JOYINFOEX {
	uint32 dwSize = 0;
	uint32 dwFlags = 0;
	uint32 dwXpos = 0;
	uint32 dwYpos = 0;
	uint32 dwZpos = 0;
	uint32 dwRpos = 0;
	uint32 dwUpos = 0;
	uint32 dwVpos = 0;
	uint32 dwButtons = 0;
	uint32 dwButtonNumber = 0;
	uint32 dwPOV = 0;
	uint32 dwReserved1 = 0;
	uint32 dwReserved2 = 0;
};
typedef JOYINFOEX *LPJOYINFOEX;
typedef int MMRESULT;

inline MMRESULT joyGetPosEx(uint uJoyID, LPJOYINFOEX pji) {
	return 0;
}

} // namespace Dink

#endif
