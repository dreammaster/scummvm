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

#ifndef SPYCRAFT_DMADE_ADVMAIN_H
#define SPYCRAFT_DMADE_ADVMAIN_H

#include "common/events.h"
#include "spycraft/dmade/advlib.h"

namespace Spycraft {

#define MADE_VERS			0x0120		// version 1.2.0 11/30/94 djh

// the following #defines are used exclusively for sfxGetEvent()
enum {
	EVENT_LBUTTONUP		= Common::EVENT_LBUTTONUP,
	EVENT_LBUTTONDOWN	= Common::EVENT_LBUTTONDOWN,
	EVENT_MBUTTONUP		= Common::EVENT_MBUTTONUP,
	EVENT_MBUTTONDOWN	= Common::EVENT_MBUTTONDOWN,
	EVENT_RBUTTONUP		= Common::EVENT_RBUTTONUP,
	EVENT_RBUTTONDOWN	= Common::EVENT_RBUTTONDOWN,
	EVENT_MOUSE			= Common::EVENT_MOUSEMOVE,
	EVENT_KEYDOWN		= Common::EVENT_KEYDOWN,        // jcd: eliminated EVENT_KEYUP on 6/21/95
	EVENT_SYSKEYDOWN	= 9001,
	EVENT_SYSCHAR		= 9002,
	EVENT_DISKINSERT	= 9003
};

typedef struct MADEEventStamp {
	BitField modifiers; 	// Control keys depressed during event
	uint32 clock_lo;		// Lo 32 bits of system clock at event
	uint32 message;			// Event specific message
	MADEPt cursor_pt;		// location of the cursor in dic
	uint16 get_event_message;  // Id of windows event 
} MADEEventStamp;

typedef struct MADEEnvirons {
	bool slow_system;		// if non-zero, system may have trouble playing movies
	MADERect screen_rect;	// rectangle the main screen occupies
	uint32 cpu_load;		// a number giving the current load of the CPU
} MADEEnvirons;

/* These are the required calls that all games must implement */
extern void StartScript(void);						// game initialization
extern void EndScript(void);
extern void OnNotify(int, void *, MADEEventStamp *);
extern short OnRightButtonDown(MADEEventStamp *);	// Right mouse button down
extern short OnRightButtonUp(MADEEventStamp *);		// Right mouse button up
extern short OnMiddleButtonDown(MADEEventStamp *);	// Middle mouse button down
extern short OnMiddleButtonUp(MADEEventStamp *);	// Middle mouse button up
extern short OnLeftButtonDown(MADEEventStamp *);	// Left mouse button down
extern short OnLeftButtonUp(MADEEventStamp *);		// Left mouse button up
extern short OnKeyDown(MADEEventStamp *);			// Key down
extern short OnIdle(MADEEventStamp *);				// No event; Idle

//	Function:	sfxInit
//
//	Purpose:	Allows MADE to do any needed setup
//
//	Parameters:	none
//
//	Returns:	none

//void sfxInit(void);


//	Function:	sfxGetEvent
//
//	Purpose:	Gives access to user events while game is in loop
//
//	Parameters:	MADEEventStamp *
//
//	Returns:	none
//

extern void sfxGetEvent(MADEEventStamp *event);


//	Function:	sfxQuit
//
//	Purpose:	Notifies MADE we are about to shut down
//
//	Parameters:	none
//
//	Returns:	none
//
//						NOTE
//
//	This routine DOES NOT terminate the game. As soon as you return
//	from the event handler that calls this function, MADE will shut
//	the game down. Therefore, it is safe to call sfxQuit, do your
//	cleanup and then return. We will take care of the rest.

extern void sfxQuit(void);

//	Function:	sfxEnvirons
//
//	Purpose:	returns various information about the machine
//
//	Parameters:	version - pass in MADE_VERS, this will tell us which structure you understand
//
//	Returns:	all the information in stuff

extern MADEEnvirons *sfxEnvirons(uint16 version);

} // namespace Spycraft

#endif
