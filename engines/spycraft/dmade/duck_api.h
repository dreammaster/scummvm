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

#ifndef SPYCRAFT_DMADE_DUK_API_H
#define SPYCRAFT_DMADE_DUK_API_H

#include "spycraft/dmade/duck_hfb.h"
#include "spycraft/dmade/duck_dxl.h"
//include <setjmp.h>

namespace Spycraft {

// TODO: Refactor out
struct jmp_buf {
};

enum spkrst8 {
	SPEAKEROFF = 0, SPEAKERON = 1, FEEDSPEAKER = 2, MONOSPEAKER = 3
};
enum syncst8 {
	NOSYNC = 0, SYNCSPEAKER = 1, SYNCSYSTEM = 2
};

void DumpDelta();

void 	BlackBox(unsigned char *addr, int x, int y, int w, int h, int pitch);
void 	BlackDuck(DXL_VSCREEN_HANDLE vsp);   // clear screen
void 	Announcement(const char *lpszString);			// make errors less Windows-dependent
void 	AnnDone();	// flush announce buufer
void 	longjmpOnAudioErrors(jmp_buf *pErrorContext);
void 	ErrorBuffer(char *errorbuf);
int		RestoreDirty(DXL_XIMAGE_HANDLE pXImageint, DXL_VSCREEN_HANDLE dst);
int 	doVGASetup(DXL_VSCREEN_HANDLE dst, char *bgstring);			  // VGA palette stuff not working, yet in W95
void 	resetDisplay(DXL_VSCREEN_HANDLE dst);					  // give back objects
void 	flip_screens(DXL_VSCREEN_HANDLE dst);        // triple buffer 640 X 480 X 16bits
void 	MarkSkips(DXL_VSCREEN_HANDLE dst, int skipnum);
int GetAudioProfile();
// dignostics
int 	Get_show_dots();			// look at diagnostic flag
void	Set_show_dots(int fred);	// set flag
int		DLLVersion();
// demo stuff -- ignore
int 	Get_ZBuffer();
void 	NewTrigger(int val);		// trigger from the UI
// audio API's unique to Windows version -- may change
int FillAudio(HFB_BUFFER_HANDLE HFBuffer, MFP_STREAM_HANDLE AStream, int *pAudioChunkIndex,
	void **audaddr, long *audlen, int buffPreload, int MultiBlock);
int SetupAudio(HFB_BUFFER_HANDLE HFBuffer, MFP_STREAM_HANDLE AStream, int *pAudioChunkIndex,
	void **audaddr, long *audlen, int buffPreload);
void 	StartPlaying();			// Start the audio playing
void	StopPlaying();			// Stop Audio drivers from playing
void 	EndAudio();				// 
void	TakeBack();
void	PauseSpeaker();			// Pause the speaker -- suspend in mid-flight
void	ResumeSpeaker();		// Resume the speaker
int		TimeSpeaker();
int		IsPlaying();		// returns whether speaker is actually playing			// 
void 	longjmpOnAudioErrors(jmp_buf *pErrorContext);

// sync objects in both EXE and DLL ugly -- will change for sure
void SetStates(spkrst8 speaker, int S3style);
void SetTraceNow();
void ClearTraceNow();

// extra def's for UI objects
#define VTR_PAUSE 1		// set if VTR Paused, else playing
#define VTR_FF 2		// set if VTR doing Fast FORWARD
#define VTR_REW 4		// set VTR rewinding
#define VTR_SEEK 8		// set VTR for seek
#define VTR_VIDEO 16
#define VTR_AUDIO 32

#define ALTKEY 8
#define CTRLKEY 4
#define SHIFTKEY 2
#define RIGHTSHIFT 1

#define MOUSE_LEFT 1
#define MOUSE_RIGHT 2

} // namespace Spycraft

#endif
