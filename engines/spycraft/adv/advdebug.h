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

#ifndef SPYCRAFT_ADVDEBUG_H
#define SPYCRAFT_ADVDEBUG_H

#include "spycraft/adv/adverror.h"

namespace Spycraft {

//	Function:	sfxPrintf
//
//	Purpose:	outputs formatted information
//
//	Parameters:	same as printf
//
//	Returns:	same as printf

int sfxPrintf(const char *format, ...);

/* MADE Internal */

extern void pAssert(int, const char *, int);
extern void dbgmsg(const char *sz, ...);
extern void ErrMsg(const char *sz, ...);
extern void ShowMon(void);
extern void MonoOut(const char *format, ...);
extern void KillMono(void);
extern int AlertMsg(const char *title, char *sz, ...);

} // namespace Spycraft

#endif
