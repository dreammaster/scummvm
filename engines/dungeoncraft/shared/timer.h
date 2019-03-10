/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#ifndef DUNGEONCRAFT_SHARED_TIMER
#define DUNGEONCRAFT_SHARED_TIMER

//#include <afxwin.h>
//#include <mmsystem.h>

namespace DungeonCraft {

class CTimer
{
public:
  const enum { DEFAULT_RES = 1 /* milliseconds */};

  CTimer(UINT TargetRes = DEFAULT_RES);
  virtual ~CTimer();

  BOOL StartOneTimeTimer(UINT MsInterval);
  BOOL StartPeriodicTimer(UINT MsInterval);
  VOID StopTimer();
  UINT Id() const { return m_TimerId; }
  BOOL Active() { return m_Active; }
  virtual VOID OnTimerEvent(UINT TimerId) = 0;

  friend void CALLBACK TimerCallback(UINT TimerId, UINT msg, 
                                     DWORD UserData, DWORD dw1, DWORD dw2);
private:
  UINT  m_TimerId;
  UINT  m_Resolution;
  BOOL  m_Periodic;
  BOOL  m_Active;
};

} // End of namespace DungeonCraft

#endif
