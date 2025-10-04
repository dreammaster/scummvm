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

#ifndef SPYCRAFT_ADVSFX_H
#define SPYCRAFT_ADVSFX_H

#include "spycraft/advback.h"

namespace Spycraft {

/*
 *
 *  Header for Special Effects Functions
 *
 */

extern void DoFlip(Background *from, Background *to);
extern void WipeRight(Background *from, Background *to,
	uint16 steps, uint32 stepTime);
extern void WipeLeft(Background *from, Background *to,
	uint16 steps, uint32 stepTime);
extern void WipeDown(Background *from, Background *to,
	uint16 steps, uint32 stepTime);
extern void WipeUp(Background *from, Background *to,
	uint16 steps, uint32 stepTime);
extern void WipeCenterVert(Background *from, Background *to,
	uint16 steps, uint32 stepTime);
extern void WipeCenterHorz(Background *from, Background *to,
	uint16 steps, uint32 stepTime);
extern void WipeOutHorz(Background *from, Background *to,
	uint16 steps, uint32 stepTime);
//extern void DoDissolve(Viewport *from_port, Viewport *to_port,
  //              uint16 steps, uint32 stepTime);
extern void DoDissolve(Background *from, Background *to,
	uint16 steps, uint32 stepTime);
extern void DoDissolveRight(Viewport *from_port, Viewport *to_port,
	uint16 steps, uint32 stepTime);
extern void DoDissolveLeft(Viewport *from_port, Viewport *to_port,
	uint16 steps, uint32 stepTime);
extern void DoDissolveDown(Viewport *from_port, Viewport *to_port,
	uint16 steps, uint32 stepTime);
extern void DoDissolveUp(Viewport *from_port, Viewport *to_port,
	uint16 steps, uint32 stepTime);
extern void DoDissolveCenter(Viewport *from_port, Viewport *to_port,
	uint16 steps, uint32 stepTime);
extern void DoDissolveHorz(Viewport *from_port, Viewport *to_port,
	uint16 steps, uint32 stepTime);
extern void DoDissolveVert(Viewport *from_port, Viewport *to_port,
	uint16 steps, uint32 stepTime);
//extern void DoFade(Viewport *from_port, Viewport *to_port,
  //          uint16 steps, uint32 stepTime);
extern void DoFade(Background *from, Background *to,
	uint16 steps, uint32 stepTime);
extern void DoWhip(uint16 direction, Viewport *from_port, Viewport *to_port,
	uint16 steps, uint32 stepTime);

} // namespace Spycraft

#endif
