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

#ifndef SPYCRAFT_DMADE_ADVMEM_H
#define SPYCRAFT_DMADE_ADVMEM_H

namespace Spycraft {

//	Function:	sfxKillBuffer
//
//	Purpose:	Release memory allocated by MADE.
//
//	Parameters:	buffer - a pointer to the buffer to be killed
//
//	Returns:	void

extern void sfxKillBuffer(void *buffer);
extern int sfxGetClientMem(void);
extern int sfxGetFreeMem(void);

extern void InitMem(void);
extern void CleanMem(void);

extern void *AllocPtrEx(int size);
extern void FreePtrEx(void *ptr);
extern void FreePtr(void *buffer);

#define AllocPtr(size)  AllocPtrEx(size)
#define FreePtr(ptr) FreePtrEx(ptr)

extern long GetMemFree(void);
extern void RegisterMem(long size);
extern void UnregisterMem(long size);

} // namespace Spycraft

#endif

