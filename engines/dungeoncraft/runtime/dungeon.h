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

#ifndef DUNGEONCRAFT_SHARED_DNGN
#define DUNGEONCRAFT_SHARED_DNGN

#include "dungeoncraft/shared/resource.h"       // main symbols

#include "dungeoncraft/shared/items.h"
#include "dungeoncraft/shared/char.h"
#include "dungeoncraft/shared/spell.h"
#include "dungeoncraft/shared/Timer.h"

namespace DungeonCraft {

/////////////////////////////////////////////////////////////////////////////
// CDungeonApp:
// See Dungeon.cpp for the implementation of this class
//

class CDungeonApp : public CWinApp, public CTimer
{
  UINT m_EventTimerId;
  HANDLE m_SleepEvent;
  UINT m_TimerResolution;

public:
	CDungeonApp();
	void SetIntervalTimer(UINT idTimer, int secs);
  VOID OnTimerEvent(UINT TimerId);
  void AppSleep(DWORD timeout);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDungeonApp)
	public:
	virtual BOOL InitInstance();
	virtual int Run();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CDungeonApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

extern CDungeonApp theApp;

} // End of namespace DungeonCraft

#endif
