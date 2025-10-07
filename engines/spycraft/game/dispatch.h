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

#ifndef SPYCRAFT_GAME_DISPATCH_H
#define SPYCRAFT_GAME_DISPATCH_H

#include "spycraft/dmade/advmain.h"

namespace Spycraft {

//DEBUG
extern int scratchGlobal;

extern void StartScript();
extern short OnRightButtonDown(MADEEventStamp *event);
extern short OnRightButtonUp(MADEEventStamp *event);
extern short OnLeftDouble(MADEEventStamp *event);
extern short OnMiddleButtonDown(MADEEventStamp *event);
extern short OnMiddleButtonUp(MADEEventStamp *event);
extern short OnLeftButtonDown(MADEEventStamp *event);
extern short OnLeftButtonUp(MADEEventStamp *event);
extern short OnKeyDown(MADEEventStamp *event);
extern short OnKeyUp(MADEEventStamp *event);
extern short OnIdle(MADEEventStamp *event);
extern short OnDiskInsert(MADEEventStamp *event);

} // namespace Spycraft

#endif
