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

#include "aesop/defs.h"
#include "aesop/ail32.h"
#include "aesop/modsnd32.h"   // Common C header for MODSND code.
#include "aesop/mod32.h"      // Assembly prototypes for mixer code.

namespace Aesop {

/******* Private data region used by the MIXER  *******/

static int Active[PHYSICAL] = { 0,0,0,0 };  // Active flags.
static int ActiveCount = 0;              // Current number of channels active.
static MODSPEC MOD[CHANNELS];          // Array of all logical channel spec's.
static char *buildbuf1;                // Output buffer BUFFSIZE*CHANNELS
static char *buildbuf2;                // Output buffer BUFFSIZE*CHANNELS
static char *buff1;                            // Double buffered built sound buffer.
static char *buff2;                            // Second of double buffered sound.
static unsigned seg1;                  // Real-mode segs of buffers
static unsigned seg2;
static char *basePCM;                  // Base sound storage address
static HDRIVER hPCM;

extern WORD PCM_active;

//
// Returns the number of bytes needed by the mixer code for work buffers.
//

int ModSizeNeeded(void) {
	return (BUFFSIZE * PHYSICAL * 2 /*+BUFFSIZE*4*/);
}

//
// Base address of fixed buffer for sound processing.
//

int StartMod(HDRIVER driver, const char *soundbase, const char *emsloc) {
#ifdef TODO
	int i;
	union REGS inregs, outregs;

	hPCM = driver;
	basePCM = emsloc;

	buildbuf1 = soundbase;                   // Build buffer.
	buildbuf2 = soundbase + BUFFSIZE * PHYSICAL; // Second build stream.

	//
	// Allocate two buffers from real-mode (lower 1MB) memory
	//
	// *buf1, *buf2 -> protected-mode pointers to buffers (sel:0000)
	// *seg1, *seg2 -> real-mode (physical) pointers to buffers (seg:0000)
	//
	// Note: DPMI calculations assume flat model near pointer offset 0 = 
	// segment 0, offset 0 (Rational DOS4GW).
	//

	inregs.x.eax = 0x100;
	inregs.x.ebx = ((BUFFSIZE + 15) / 16);
	int386(0x31, &inregs, &outregs);

	seg1 = outregs.x.eax << 16;
	buff1 = (char *)(outregs.x.eax * 16);

	inregs.x.eax = 0x100;
	inregs.x.ebx = ((BUFFSIZE + 15) / 16);
	int386(0x31, &inregs, &outregs);

	seg2 = outregs.x.eax << 16;
	buff2 = (char *)(outregs.x.eax * 16);

	//
	// Init MOD structures
	//

	for (i = 0; i < CHANNELS; i++)
	{
		MOD[i].buff = buildbuf1;
		MOD[i].bufflen = BUFFSIZE;
		MOD[i].sound = 0;
		MOD[i].silence = 0;
		MOD[i].sndlen = 0;
		MOD[i].oneshot = 0;
		MOD[i].trigger = 0;                 // Sample is inactive.
	}
#endif
	return 0;                           // Mod player installed.
}

void PollMod(void) {
#ifdef TODO
	int a1, j, i;
	int bufnum;
	sound_buff AILbuf;
	char *outbuf;

	if (!PCM_active) return;

	if (AIL_sound_buffer_status(hPCM, 0) == DAC_DONE)
	{
		AILbuf.seg_data = seg1;
		AILbuf.sel_data = buff1;
		outbuf = buff1;

		bufnum = 0;
	} else if (AIL_sound_buffer_status(hPCM, 1) == DAC_DONE)
	{
		AILbuf.seg_data = seg2;
		AILbuf.sel_data = buff2;
		outbuf = buff2;

		bufnum = 1;
	} else
		return;

	for (j = 0, i = 0; j < PHYSICAL; j++)
	{
		a1 = Active[j];
		if (a1)
		{
			a1--;                  // Less one to get actual channel number

			if (MOD[a1].trigger == 0)
			{
				Active[j] = 0;
				ActiveCount--;        // one less active channel.
			} else
			{
				MOD[a1].buff = buildbuf1 + i;

				BuildModStream(&MOD[a1]);

				i++;
			}
		}
	}

	MOD[a1].buff = buildbuf1;

	AILbuf.pack_type = 0;
	AILbuf.sample_rate = 131;     // (Eye III sound samples at 8.000 kHz.)
	AILbuf.len = MOD[a1].bufflen;

	switch (ActiveCount)
	{
	case 1:
		MergeMod1(&MOD[a1], outbuf);  // Merge into output buffer.
		break;
	case 2:
		MergeMod2(&MOD[a1], outbuf);  // Merge into output buffer.
		break;
	case 3:
		MergeMod3(&MOD[a1], outbuf);  // Merge into output buffer.
		break;
	case 4:
		MergeMod4(&MOD[a1], outbuf);  // Merge into output buffer.
		break;
	default:                        // (Nothing to merge)
		return;
	}

	AIL_format_sound_buffer(hPCM, &AILbuf);
	AIL_register_sound_buffer(hPCM, bufnum, &AILbuf);
	AIL_start_digital_playback(hPCM);
#endif
}

//
// Set active channel.
//

void SetActive(int channel, int logical) {
#ifdef TODO
	if (!Active[channel]) ActiveCount++;   // Increase total active.
	Active[channel] = logical + 1;               // Set physical channel to this logical channel.
#endif
}

void InActive(int channel) {
#ifdef TODO
	if (Active[channel]) ActiveCount--;
	Active[channel] = 0;
#endif
}

//
// Set this audio channel up.
//

int SetChannel(int ems, unsigned int emsoffset, unsigned int length,
	int channel, int mode) {
#ifdef TODO
	unsigned char *temp;

	temp = (unsigned char *)(basePCM + (ems * 65536) + emsoffset);

	MOD[channel].sound = temp;
	MOD[channel].sndloc = temp;
	MOD[channel].silence = 0;
	MOD[channel].sndlen = length;
	MOD[channel].bufflen = BUFFSIZE;
	MOD[channel].oneshot = mode;
	MOD[channel].trigger = 1;                     // Sample is inactive.
#endif
	return(channel);
}

void ChannelOn(int logical) {
	MOD[logical].trigger = 1;
}

int ChannelState(int logical) {
	return(MOD[logical].trigger);
}

void ChannelOff(int logical) {
	MOD[logical].trigger = 0;
}

void ChannelSilence(int logical, int silence) {
	MOD[logical].silence = silence;
}

void StopMod(void) {
	AIL_stop_digital_playback(hPCM);
}

//
// Reports if this channel physical channel is free for use or not.
// This is true if the channel is not active, or the active channel assigned
// has had its trigger go low.
//

int PhysicalState(int channel) {
	return(Active[channel]);
}

} // namespace Aesop
