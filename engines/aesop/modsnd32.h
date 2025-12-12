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

#ifndef AESOP_MODSND_H
#define AESOP_MODSND_H

#include "aesop/ail32.h"

namespace Aesop {

#ifdef __cplusplus
extern "C" {
#endif

#define BUFFSIZE 1024           // Size of build buffer in bytes

#define CHANNELS 4              // # of logical channels

#define PHYSICAL 4	           // # of physical channels

//
// Return number of bytes needed by mixer for
// the specified number of channels.
//

int ModSizeNeeded(void);        

//
// Report address of pending flag.
//

char * ReportPending(void); 

//
// Bootstrap the mod player object.  MOD player requires a fixed address
// buffer for workspace.	Return code of zero if no sound driver is available.
//

int StartMod(HDRIVER driver, char *soundbase, char *emsloc);

//
// Stop mixer.
//

void StopMod(void);             

//
// Give the mod player a timeslice
//

void PollMod(void);

//
// If channel passed is -1 then it means to establish on the first available
// channel found.  A return code of -1 means there was no channel free.
//

int  SetChannel(int emshandle,			  // address of sound file.
					 unsigned int emsoffset,  // Offset into this page.
					 unsigned int length,	  // length of sound effect.
					 int channel,				  // channel to establish.
					 int mode);					  // mode 0=continuous loop, 1=one-shot

//
// Set physical channel active to this logical channel.
//

void SetActive(int channel,int mod);

//
// Make this physical channel inactive.
//

void InActive(int channel); 

//
// Trigger a channel to play.
//

void ChannelOn(int channel); 

//
// Turn channel off.
//

void ChannelOff(int channel); 

void ChannelSilence(int channel,int silence);

//
// Report channel status on/off.
//

int ChannelState(int channel); 

//
// Report if physical channel is free.
//

int PhysicalState(int channel); 

#ifdef __cplusplus
}
#endif

} // namespace Aesop

#endif
