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

#include "spycraft/afxwin.h"
#include "spycraft/dmade/types.h" 
#include "spycraft/dmade/duck_platform.h"
#include "spycraft/dmade/duck_api.h"
#include "spycraft/dmade/duck_mli.h"

namespace Spycraft {

/* the following mid-level routines implement defined functionality using lower
	level API calls from the DXL and HFB libararies
*/

enum proftype {
	PROFSYS, PROFAUDIO, PROFWDZAUDIO,
	PROFDX, PROFBLIT, PROFBLITINTER, PROFDXINTER,
	PROFREAD, PROFTOT, PROFTIME, NUM_PROFILES
};

/* variables */

DXL_VSCREEN_HANDLE VSInternal;	// points to Internal Control Structure for decompression
bool didMalloc;				// set if had to malloc internal buffer and so have to free it
uchar *pInternalBuffer;		// points to internal buffer that was malloced

extern int ProfBuf[NUM_PROFILES];
extern int prof_time, prof_start, ProfTotal;

#define MAX_DATA_RATE (16*1024*15)
int GlobalMaxToRead;
int GlobalMinToRead;
int FillAdjustFreq;
int CurrentReadSize;

int soff;

extern ScrnInfo Screen;		// info about the screen
extern DXL_VSCREEN_HANDLE VSScreen;
extern MLI_LAYER_HANDLE AudioLayer;			// the layer now playing audio
extern MLI_LAYER_HANDLE Default_pL;			// If not using Layer ID's, this is the one...
extern int VTR_Status;
extern int NoSkip;
int key, inter;
int done;
int RePaint;			// set to force full repaint after erase, video off, or the like
int audio_elapsed_sofar;
int framesPlayed;						// other flags (like speakerstate or syncstate) are untouched
extern int synchFrame, droppedFrames, LastFillFrame, DecodedFrames;
int FirstWantedFrame;
extern syncst8 syncstate;
extern char buf[];
clock_t timer_time;
extern int FastSprite;
extern int FramesUp;
/* prototypes */
int  IsDone();			// see if done flag set
void SetDone();
void NotDone();
int  SMP_DuckFrame(uchar *pScreen);  // just does a frame
int  CreateInternalScreen();

int  SMP_CompositeFrame();		// make a frame, if possible
void SMP_RestoreDirty(MLI_LAYER_HANDLE pBgdL);		// restore dirty rectangles under sprites
void SMP_ReleaseData();			// release data as needed
void SMP_GetData(MLI_LAYER_HANDLE pL);				// get streaming data or determine frame numbers
int  SMP_NextFrame(MLI_LAYER_HANDLE pL);
void SMP_FakeInterrupt();		// fill streaming buffers, if it's time
int  SMP_GetElapsedTime();
int  Profile(proftype bucket);		// get number of ticks since last call to profiler.
void duckAudioInitSW(MLI_LAYER_HANDLE pL, int frame);
uchar *LockDown(ScrnInfo *sip);
HRESULT UnlockScreen();
int getBreakStatus();
void StartProfiling();
void init_GlobalMaxToRead();
void StartTrace();
void EndTrace();
int tracenow;
int fillBuffer(MLI_LAYER_HANDLE pL, int maxToRead, int skip);
/* Routine plays video to last frame Since it doesn't do message loop processing,
	it can't be interrupted */
int MLI_Play(int from, int to)    // from and to refer to background video (lowest non-sprite layer)
{
	int DuckSetup = false;
	MLI_LAYER_HANDLE pL = MLI_FirstActiveLayer();

	if (!VSInternal)	CreateInternalScreen();
	synchFrame = 0;
	NotDone();
	MLI_PlayRange(pL, from, to);
	while (!IsDone()) {
		if (Screen.ScrnPtr) {
			if (!DuckSetup) {

				DuckSetup = true;

				DXL_AlterVScreen(VSScreen, Screen.ScrnPtr, DXRGBNULL, -1, -1); // set screen ptr (may have changed in outer loops)
				MLI_Erase();
				MLI_StartPlaying();
				StartProfiling();	// local routine starts MM timers
			}

			if (SMP_DuckFrame(Screen.ScrnPtr)) {
				SetDone();		// returns true if no more to play, else false
			}
		}
		//  	  		     WaitMessage(); // Will not return until a message is posted.
	}
	MLI_StopPlaying();
	return true;  	// completed successfully
}

int getBreakStatus() {
	if (GetAsyncKeyState(VK_ESCAPE) < 0) {
		SetDone();
	}
	return(done);
}

int CreateInternalScreen() {
	MLI_LAYER_HANDLE pL;
	if (!VSInternal) {

		// for now !! will change with multiple (or no) backgrounds!!

		// find lowest background (non-sprite) video

		if ((pL = MLI_FirstActiveLayer()) == NULL)	return true;	// nothing to do
		while (true) {
			if (!(pL->LFlags & LAYER_IS_SPRITE))	break;  // found background layer
			if ((pL = MLI_NextActiveLayer()) == NULL) { 	// got to end and still no background
				if ((pL = MLI_FirstActiveLayer()) == NULL)	return true;	// nothing to do
				while (pL) {
					pL->LVScreen = VSScreen;		// point to right place	-- make all sprites point at desktop
					pL = MLI_NextActiveLayer();  		// look until done
				}
				return true;  // never found one
			}
		}

		// allocate temp buffer to build image up in that's the size of the
		// background video stream.  We'll decompress 16 bit to that and
		// then copy the resultant image out to the screen.
		if (DXL_GetXImageType(pL->LXImage) == DXL_INTERFRAME) {   // for interframe, use reference buffer
			didMalloc = false;
			if ((VSInternal = DXL_DeriveVScreenFromXImage(pL->LXImage)) == NULL) {
				Announcement("Can't allocate memory for Internal Vscreen");
				return false;
			}
		} else {
			pInternalBuffer = (uchar *)malloc(pL->LImgBox.w * pL->LImgBox.h * sizeof(int));
			didMalloc = true;
			if (!pInternalBuffer) {
				Announcement("Can't allocate memory for internal screen");
				return false;
			}
			if ((VSInternal = DXL_CreateVScreen(pInternalBuffer,
				DXRGB16, 			// always 16-bit
				pL->LImgBox.w,
				pL->LImgBox.h)) == NULL) {
				Announcement("Can't allocate memory for Internal Vscreen");
				return false;
			}
		}
	}
	// now search all active layers and point them to the Internal screen for the background

	if ((pL = MLI_FirstActiveLayer()) == NULL)	return true;	// nothing to do
	while (pL) {
		pL->LVScreen = VSInternal;		// point to right place
		pL = MLI_NextActiveLayer();  		// look until done
	}

	return true;
}

int SMP_DuckFrame(uchar *pScreen) {
	MLI_LAYER_HANDLE pL;

	Screen.ScrnPtr = pScreen;	// addr may have changed in outer loops
	DXL_AlterVScreen(VSScreen, pScreen, DXRGBNULL, -1, -1); // set screen ptr (may have changed in outer loops)
	while (!IsDone()) { 		// go until we stop
		if (MLI_ProcessFrame(0)) {
			if (getBreakStatus())	return true;	// at least allow ESC key
			// advance to next frame for all videos (simplest case)
			// but only if we processed a frame...
			if ((pL = MLI_FirstActiveLayer()) == NULL) continue;
			if (!(pL->LFlags & LAYER_IS_SPRITE)) {
				if (pL->LLastFrame >= 0) {	// ending range set
					if (pL->LFrameIndex >= pL->LLastFrame) {
						Announcement("Finished Play Normally\n");
						SetDone();	// if over ending range, stop the playback
					}
				}
			}
			while (pL && !done) {
				SMP_GetData(pL);		// here we get data for all, but this could
				// do arbitrary Frames...
				pL = MLI_NextActiveLayer();
			}
		}
	}
	return done;
}

/* WantedFrame  <0 Decompress and display now
				=0 Decompress and display based on internal clock
				>0 Decompress and display based on external clock -- this is the frame we want
*/
int skipnum = 0;
int MLI_ProcessFrame(int WantedFrame)  // Put next frame up, if appropriate
// This API should be called in a game loop
// returns true at end of range or file
{
	static int inter_freq, we_behind, fcnt, oldskip = 0;

	MLI_LAYER_HANDLE pBgdL = MLI_FirstActiveLayer(); // BIG ASSUMPTION, but this is just an example
	int iskeyframe, NewChunk;

	if (WantedFrame > 0) synchFrame = WantedFrame;  // apply external synch

	Profile(PROFSYS);		// profiling allows recovery of interesting timing data

	SMP_FakeInterrupt(); 		// Fake Interrupt tries to ensure CD buffers fill at frame rate

#if false
	if ((synchFrame - pBgdL->LFrameIndex) > pBgdL->LframeRate) {
		Common::sprintf_s(buf, "Buffer Starved at frame %d\n", pBgdL->LframeRate);
		Announcement(buf);
		HFB_WalkFlags(pBgdL->LBuffer);
		SetDone();
		return false;
	}
#endif
	if ((iskeyframe = DXL_IsXImageKeyFrame(pBgdL->LXImage)) < 0) iskeyframe = false;  // error return!

	if (syncstate != NOSYNC && (WantedFrame >= 0)) {   	// see if trying to stay in synch
		we_behind = synchFrame - (pBgdL->LFrameIndex - 1);
		if ((pBgdL->LFrameIndex - 3) >= synchFrame) {
			return false;  // not done, nothing to do
		}

		if (iskeyframe) {
			inter_freq = HFB_FramestoNextKeyFrame(pBgdL->LVStream, pBgdL->LvidChunkIndex, &NewChunk);

			fcnt = 0;
			oldskip = skipnum;		// number skipped last time
			if (we_behind >= 2) {
				if (we_behind >= 3) skipnum += 2; 		// for big activision 15 frame KF separation
				else 	skipnum = 1;  // number we will skip between KF's
			} else {
				skipnum = 0;
			}
		} else {
			++fcnt;	// fcnt is frame count in this KF sequence
		}
	}

	Profile(PROFSYS);
	if ((fcnt <= (inter_freq - skipnum)) || iskeyframe || (WantedFrame < 0) || NoSkip) {	///	skipping LOGIC

#if false
		if (DisplayConvert.Buffer) {
			flip_screens(VSScreen);         // set for where next image is going to go
		}
#endif
		int NeedBlit = SMP_CompositeFrame();		// make a frame, if possible

		// more profiling counter maintenance
		if (iskeyframe) {
			Profile(PROFDX);
			key++;
		} else {
			Profile(PROFDXINTER);
			inter++;
		}
		SMP_FakeInterrupt();

		//#if false
		if (oldskip > 2) {	// if skipped last KF interval, blit every other frame in this one
			if (pBgdL->LFrameIndex & 1) {
				RePaint = true;
			} else {
				NeedBlit = false;		// don't do these at all
			}
		}
		//#endif
			// now do the blit, if any layer has video to BLIT
		if (VSInternal && NeedBlit && (!(pBgdL->LFlags & LAYER_VIDEO_OFF))) {
			DXL_DisplayVScreen(pBgdL->LXImage, VSInternal, VSScreen, RePaint);
			RePaint = false;	// must force this to full repaint if wanted
		}


		if (iskeyframe)	Profile(PROFBLIT);
		else			Profile(PROFBLITINTER);
		//		Common::sprintf_s(buf,"displayed frame %d, %d\n", pBgdL->LFrameIndex, synchFrame);
		//		Announcement(buf);

	} else { ///-skipping LOGIC
		//		Common::sprintf_s(buf,"skipped frame %d %d\n", pBgdL->LFrameIndex, synchFrame);
		//		Announcement(buf);
		if (Get_show_dots()) {
			MarkSkips(VSScreen, skipnum);		// mark screen for skipped frames, if diagnostic set
			++droppedFrames;
		}
	}

	SMP_ReleaseData();

	SMP_FakeInterrupt();

	SMP_RestoreDirty(pBgdL);

	return true;		// We decompressed something
}

// composite all frames together

int SMP_CompositeFrame()		// make a frame, if possible
{
	Box TempBox;
	MLI_LAYER_HANDLE pL = MLI_FirstActiveLayer();
	int DecompDone = false;

	while (pL)
	{
		if ((pL->LFlags & (LAYER_IN_USE | LAYER_HAS_VIDEO)) ==
			(LAYER_IN_USE | LAYER_HAS_VIDEO)) {

			DXL_GetXImageXYWH(pL->LXImage, (int *)&TempBox.x,
				(int *)&TempBox.y,
				(int *)&pL->LImgBox.w,
				(int *)&pL->LImgBox.h);
			if (pL->Lframelen) {	// only decompress if something there...
				DXL_dxImageToVScreen(pL->LXImage, pL->LVScreen);
				DecompDone = true;
				// sprite requires full, not interframe blitb
				if (pL->LFlags & LAYER_IS_SPRITE)	RePaint = true;
			}
			// if all in memory, don't give any of it away
			DecodedFrames++;		// fully decoded (unpaused) frames
		}
		pL = MLI_NextActiveLayer();
	}
	return DecompDone;
}


void SMP_GetData(MLI_LAYER_HANDLE pL)				// get streaming data or determine frame numbers
{

	if ((pL->LFlags & (LAYER_IN_USE | LAYER_HAS_VIDEO)) == (LAYER_IN_USE | LAYER_HAS_VIDEO)) {
		if (pL->LFlags & LAYER_STREAMS) {		// really streaming 
			if (!done) {
				SMP_NextFrame(pL);		// go get a frame we're going to use
			}
		} else {		// a sprite -- go for it via random access
			if (FastSprite) {
				pL->LFrameIndex += 6;  // Fast, for action
			} else {
				pL->LFrameIndex++;  // normal speed
			}

			// this is the point where you change Frame Indexes to make the sprite
			// jump around arbitrarily and not just play in a tape loop

			if ((pL->LvidChunkIndex = HFB_GetStreamingData(pL->LVStream, (void **)&pL->Lcompbuf,
				&pL->Lframelen, DUK_ABSOLUTE, pL->LFrameIndex)) < 0) {
				// no more data
				pL->LFrameIndex = 1;	// NB Frames start from 1, not 0
				if ((pL->LvidChunkIndex = HFB_GetStreamingData(pL->LVStream, (void **)&pL->Lcompbuf,
					&pL->Lframelen, DUK_ABSOLUTE, pL->LFrameIndex)) < 0) {
				}
			}
		}
		DXL_AlterXImageData(pL->LXImage, pL->Lcompbuf);
	}
}

// or not
int SMP_NextFrame(MLI_LAYER_HANDLE pL) {
	int newIndex;

	if ((newIndex = HFB_GetStreamingData(pL->LVStream, (void **)&pL->Lcompbuf,
		&pL->Lframelen, DUK_FORWARD, 1)) < 0) {
		/* // comment out to "forgive" buffer starvation
				if (newIndex != -1)	{
					Common::sprintf_s(buf,"Vid Index %d, newest %d, Frame %d.  See flags.txt for details",
						pL->LvidChunkIndex, newIndex, pL->LFrameIndex);
					Announcement(buf);
					HFB_WalkFlags(pL->LBuffer);
					SetDone();
				} else {
					newIndex = -1;
				}
		// end comment out
		*/
		if (newIndex == -1) {
			Announcement("End of File\n");
			SetDone();
			return done;
		}
	} else {
		pL->LFrameIndex++;		// stay synched
	}
	if (syncstate == NOSYNC) {		// if not synched, gotta read data without paying attention to clock
		HFB_FillBuffer(pL->LBuffer, DCK_DEFAULT_READSIZE, HFB_IGNORE_COUNT);
	}
	pL->LvidChunkIndex = newIndex;
	return done;
}
//   also in this file ..... shut down gracefully (?)

void SMP_ReleaseData() {
	// release data as needed
	MLI_LAYER_HANDLE pL = MLI_FirstActiveLayer();

	while (pL) {
		if ((pL->LFlags & (LAYER_IN_USE | LAYER_HAS_VIDEO)) ==	  // keep getting and releasing if video diisabled
			(LAYER_IN_USE | LAYER_HAS_VIDEO)) {
			if (pL->LvidChunkIndex > -1 && (pL->LFlags & LAYER_STREAMS)) { // release now, after using interframe status for blit
				HFB_ReleaseStreamingData(pL->LBuffer, pL->LvidChunkIndex);
			}
		}
		pL = MLI_NextActiveLayer();
	}
}


// restore any stepped on dirty rectangles
// put back in reverse order.  We saved a bit of sprite one when we saved sprite 2

void SMP_RestoreDirty(MLI_LAYER_HANDLE pBgdL) {
	// restore dirty buffers
	MLI_LAYER_HANDLE pL = MLI_LastActiveLayer();

	while (pL) {
		if ((pL->LFlags & (LAYER_IN_USE | LAYER_IS_SPRITE | LAYER_VIDEO_OFF | LAYER_HAS_VIDEO)) ==
			(LAYER_IN_USE | LAYER_IS_SPRITE | LAYER_HAS_VIDEO)) {
			RestoreDirty(pL->LXImage, pBgdL->LVScreen);
		}
		pL = MLI_PrevActiveLayer();
	}
}

// this routine maintains time and calls FillAudio to refill streaming buffers once each
// frame time.  While This routine could handle multi-stream input from Hard drive,
// you'd better be ** very careful ** if you're trying to do multiple streams
// That's left as an exercise.  This version only streams the first data it can find
void SMP_FakeInterrupt() {
	MLI_LAYER_HANDLE pL;

	int fred;
	SMP_GetElapsedTime();
	int ReadSize;

	Profile(PROFTIME);

	if (!(VTR_Status & (VTR_REW | VTR_FF))) {  // no synchronous fill if FF or REW

		FillAudio(AudioLayer->LBuffer, AudioLayer->LAStream, &AudioLayer->LaudChunkIndex,
			(void **)&AudioLayer->Laudioaddr, &AudioLayer->Laudiolen, AudioLayer->LbuffPreload, false);	// stuff audio in one chunk of audio, if possible
		Profile(PROFAUDIO);
		if (!LastFillFrame)	LastFillFrame = synchFrame - 1;
		if (LastFillFrame != synchFrame) {
			// HFB_FillBuffer should be called at approximately the frame rate.
			// It's designed to read less data than is in the cache for the CR-ROM controller.
			// This means that frequent, small reads won't block waiting for the disk
			// The fread will return immediately with data and we can go off and decompress
			// frames while the cahce fills up again.
			// call FillBuffer too often and the cache will empty and it will block. 
			// call it too often and it will attempt to read above the data rate (and also block).

			pL = MLI_FirstActiveLayer();
			while (pL) {
				if (pL->LFlags & LAYER_STREAMS) {
					ReadSize = (GlobalMaxToRead * 15) / pL->LframeRate;
					fred = fillBuffer(pL, ReadSize, skipnum);
					if (fred < 0) {
						Common::sprintf_s(buf, 256, "Fill Buffer Error %d", fred);
						Announcement(buf);
					}
					LastFillFrame++;
					if (LastFillFrame > synchFrame) {
						Announcement("Resyncing");
						LastFillFrame = synchFrame;		// could get endless weirdness
					}
					break;		// only do the one -- don't look for more
				}
				pL = MLI_NextActiveLayer();
			}
		}
	}
	Profile(PROFREAD);		// add in any time to profile counts
}

int percent;

int fillBuffer(MLI_LAYER_HANDLE pL, int maxToRead, int skip) {
	int fred;
	extern int skipnum;
	extern int synchFrame;

	if (!(synchFrame % FillAdjustFreq)) {
		percent = HFB_GetBufferPerCentFull(pL->LBuffer);
		if (percent < 50 && CurrentReadSize < GlobalMaxToRead)
			CurrentReadSize += 1024;
		else if (percent >= 50 && CurrentReadSize > GlobalMinToRead && skipnum != 0)
			CurrentReadSize -= 1024;
	}
	fred = HFB_FillBuffer(pL->LBuffer, CurrentReadSize, HFB_IGNORE_COUNT);
	//	Common::sprintf_s(buf,"ReadSize %ld, actually read %ld percent %ld\n",CurrentReadSize, fred, percent);
	//	Announcement(buf);
	return fred;
}

void MLI_Prefill(MLI_LAYER_HANDLE pL) {
	HFB_FillBuffer(pL->LBuffer, CurrentReadSize, HFB_IGNORE_COUNT);
}

int SMP_GetElapsedTime() {
	int elapsed = 0, fred;

	if (syncstate == SYNCSPEAKER) {
		elapsed = MLI_TimeSpeaker() + audio_elapsed_sofar;
		// the frame can back up as speaker switches from interpolation to real
	} else if (syncstate == SYNCSYSTEM) {
		elapsed = timeGetTime() - timer_time + audio_elapsed_sofar; // read system clock.  This doesn't get reset on pause
	}

	fred = (elapsed * AudioLayer->LframeRate) / 1000;
	if (fred > synchFrame)	synchFrame = fred; // debounce

	return elapsed;
}

// pause and resume background layer (and its audio, if that's the audio layer)
bool MLI_Pause(MLI_LAYER_HANDLE pL) {
	if (pL) {
		MLI_EnableVideo(pL, false);
		if (AudioLayer == pL) {
			MLI_EnableAudio(AudioLayer, false);
			PauseSpeaker();
		}
		return true;
	}
	return false;
}

bool MLI_Resume(MLI_LAYER_HANDLE pL) {
	if (pL) {
		MLI_EnableVideo(pL, true);
		if (AudioLayer == pL) {
			MLI_EnableAudio(AudioLayer, true);
			ResumeSpeaker();
		}

		return true;
	}

	return false;
}

// Seek to Frame.  This actually causes the CD to do Something
// It's not really a "cue", as it loads whichever buffers are needed and then continues playing
// Seek starts decompressing from the keyframe before the frame we ask to seek to

bool MLI_Seek(MLI_LAYER_HANDLE pL, int frame)
// NB Seek positions to keyframes only
{
	int FrameRate, prebuffer, PlayState, amtread, PrevKeyFrame, fred;
	long REWLength = SIZEOFFFREW;

	FirstWantedFrame = frame;		// the first frame this guy wants to see
	// in an interframe file, may have keyframe way before this
	if ((PrevKeyFrame = HFB_PreviousKeyFrame(pL->LVStream, frame)) < 0) return false;

	if (pL->LaudChunkIndex > -1)	HFB_ReleaseStreamingData(pL->LBuffer, pL->LaudChunkIndex);	// throw away associated data
	if (pL->LvidChunkIndex > -1)	HFB_ReleaseStreamingData(pL->LBuffer, pL->LvidChunkIndex);	// throw away associated data

	if (frame != PrevKeyFrame) {
		RePaint = true;	// will need full blit
		pL->LFrameIndex = PrevKeyFrame;		// so go there...

		MFP_ReadData(pL->LVStream, pFFREWFrame, &REWLength, DUK_ABSOLUTE, pL->LFrameIndex);
		DXL_AlterXImageData(pL->LXImage, pFFREWFrame); // ppoints data pointer at buffer
		// decompress from keyframe to desired frame, then Blit it once
		do {
			DXL_dxImageToVScreen(pL->LXImage, pL->LVScreen);  // decompress
			if (frame <= pL->LFrameIndex) {
				break;
			}
			pL->LFrameIndex++;
			REWLength = SIZEOFFFREW;
			if ((fred = MFP_ReadData(pL->LVStream, pFFREWFrame, &REWLength, DUK_ABSOLUTE, pL->LFrameIndex)) < 0) {
				Announcement("Bad Read in MLI_Seek()");
			}
			DXL_AlterXImageData(pL->LXImage, pFFREWFrame); // ppoints data pointer at buffer

		} while (true);
	}
	pL->LFrameIndex = frame;		// so go there...
	// note we get frame before to get info, then throw that frame away
	HFB_InitBuffer(pL->LBuffer, pL->LFile, pL->LFrameIndex - 1, pL->LbuffPreload);
	// shut down audio

	if ((PlayState = IsPlaying()) != false) {
		MLI_StopPlaying();
		TakeBack();
	}
	duckAudioInitSW(pL, pL->LFrameIndex - 1);	// we did keyframe.  get 1st interframe
	amtread = HFB_FillBuffer(pL->LBuffer, 128 * 1024, HFB_IGNORE_COUNT);	// load for audio thrown away
	if ((pL->LvidChunkIndex = HFB_GetStreamingData(pL->LVStream, (void **)&pL->Lcompbuf,
		&pL->Lframelen, DUK_ABSOLUTE, 0)) < 0) {  // 0 relative to where we did the InitBuffer
		// no more data
		SetDone();
	}
	DXL_AlterXImageData(pL->LXImage, pL->Lcompbuf);

	//HFB_WalkFlags(pL->LBuffer);
	// re-sync Audio here!!  

	MFP_GetFrameRates(pL->LVStream, pL->LAStream, &FrameRate, &prebuffer);
	audio_elapsed_sofar = ((pL->LFrameIndex - 1) * 1000) / FrameRate;  // FR in msec
	// this is the time in msec from start of audio stream.
	// we started way down the file.  This is the number of msec from
	// start of file to where we started
	synchFrame = 0;		// to force refill

	if (PlayState) {
		MLI_StartPlaying();	// only start up if was running already
		SMP_GetElapsedTime();
	}

	return true;
}

int MLI_ShowFrame(MLI_LAYER_HANDLE pL, int WantedFrame)
// NB Seek positions to non-keyframes by decompressing with no Blit
{
	int PlayState, frame, fred;
	long REWLength = SIZEOFFFREW;

	if (!pL)	pL = Default_pL;
	if (!VSInternal)	CreateInternalScreen();

	if ((frame = HFB_PreviousKeyFrame(pL->LVStream, WantedFrame)) < 0) return false;
	pL->LFrameIndex = frame;		// so go there...

	if ((PlayState = IsPlaying()) != false) {
		MLI_StopPlaying();			// This turns Audio off
		TakeBack();
	}

	MFP_ReadData(pL->LVStream, pFFREWFrame, &REWLength, DUK_ABSOLUTE, pL->LFrameIndex);
	DXL_AlterXImageData(pL->LXImage, pFFREWFrame); // ppoints data pointer at buffer
	// decompress from keyframe to desired frame, then Blit it once
	do {
		DXL_dxImageToVScreen(pL->LXImage, pL->LVScreen);  // decompress
		if (WantedFrame <= pL->LFrameIndex) {
			break;
		}
		pL->LFrameIndex++;
		REWLength = SIZEOFFFREW;
		if ((fred = MFP_ReadData(pL->LVStream, pFFREWFrame, &REWLength, DUK_ABSOLUTE, pL->LFrameIndex)) < 0) {
			Announcement("Bad Read in MLI_ShowFrame()");
		}
		DXL_AlterXImageData(pL->LXImage, pFFREWFrame); // ppoints data pointer at buffer

	} while (true);

	DXL_DisplayVScreen(pL->LXImage, VSInternal, VSScreen, true);  // full, dumb BLIT

	return true;
}

// match audio up with corresponding video

void duckAudioInitSW(MLI_LAYER_HANDLE pL, int frame) {
	DKWAVEFORM *aWave;
	int NumChannels, SamplesPerSec, BytesPerSec, wFormat, playAudio;
	int GlobalBPF, GlobalFrameRate, prebuffer;

	MFP_GetFrameRates(pL->LVStream, pL->LAStream, &GlobalFrameRate, &prebuffer);
	GlobalBPF = MFP_GetSamplesPerFrame(pL->LVStream, (MFP_STREAM_HANDLE)NULL);

	aWave = MFP_GetAudioInfo(pL->LAStream, &NumChannels, &SamplesPerSec, &BytesPerSec, &wFormat);
	playAudio = (aWave != NULL);

	if (playAudio) {
		unsigned char *aData;
		long aLength;
		// always reset the streambbb
		if (frame > 0) {
			pL->LaudChunkIndex = HFB_GetStreamingData(pL->LAStream, (void **)&aData, &aLength, DUK_ABSOLUTE, 0);
			int skipAudio = 0;

			skipAudio = (frame * SamplesPerSec) / GlobalFrameRate;
			skipAudio -= HFB_getDataPosition(pL->LAStream, -1);

			//	        if (skipAudio > 0){
			//	        	loadSamples(skipAudio);
			//	        	DXL_AlterAudioDst(audioDst,NULL,NULL,-1,0,0,0);
			//	        }
#if false 		// code below doesn't work ??!!??
#define af (vs->audFact)		// stream by stream source factory
#define spkr (as->pSpeaker)

			audioSource *src = af->curr();
			src = af->make((const uchar *)vs->audioaddr, vs->audiolen);

			if (src->canProduce()) {
				spkr->take(*src);
			}
#endif
			HFB_ReleaseStreamingData(pL->LBuffer, pL->LaudChunkIndex);
		} else {
			pL->LaudChunkIndex = HFB_GetStreamingData(pL->LAStream, (void **)&aData, &aLength, DUK_ABSOLUTE, 0);
			HFB_ReleaseStreamingData(pL->LBuffer, pL->LaudChunkIndex);
		}
		FillAudio(pL->LBuffer, pL->LAStream, &pL->LaudChunkIndex,
			(void **)&pL->Laudioaddr, &pL->Laudiolen, pL->LbuffPreload, true);	// stuff audio as much audio, as possible
	}
}

void SetDone() {
	MLI_StopPlaying();
	done = 1;
}

int IsDone() {
	return done;
}

void NotDone() {
	done = false;
}

void StartProfiling() {
	int i;

	for (i = 0; i < NUM_PROFILES; i++) {
		ProfBuf[i] = 0;
	}
	prof_start = timeGetTime();
	ProfTotal = 0;
}

int Profile(proftype ptype)		// get number of ticks since last call to profiler.
// add to specified Bucket.  And to sum
{
	int elapsed;
	int bucket = (int)ptype;

	if (bucket > NUM_PROFILES)	return NULL;
	prof_time = timeGetTime();

	elapsed = prof_time - prof_start;
	ProfBuf[bucket] += elapsed;
	ProfTotal += elapsed;
	prof_start = prof_time;
	return elapsed;
}

unsigned char *pFFREWFrame;		// frame for FF, REW frames
HFB_BUFFER_HANDLE pDefaultBuffer;		// frame for FF, REW frames
DXL_XIMAGE_HANDLE pGenericXImage;
extern MLI_LAYER_HANDLE LayerObjects;
extern int maxLayers, LayerIndex;
extern MLI_LAYER_HANDLE *pLayers;

int MLI_DuckInit() {
	// Set up default Box of Video scenario that won't require
	// extra allocs.  THis should reduce the amount of swapping and thrashing
	// by WIn95

	if (MFP_Init(1, 2, 1) < 0) return false;	// 1 files, 2 streams (1 file has audio), 1 buffers, 
	if (DXL_InitVideo(2, 1) < 0) return false;		// 2 VScreens (may only use 1), 1 Compressed Images for each video stream
	if (MLI_Init(1) <= 0)	return false;		// Max of 1 layers (may not use all of these)
	// Allocate a default CD Streaming Buffer	
	if (!pDefaultBuffer) {
		pDefaultBuffer = HFB_CreateBuffer(DEFAULTBUFFERSIZE, 0);
	}
	if (!pDefaultBuffer) {
		Common::strcpy_s(buf, 256, "Not enough room for Streaming Buffer");
		Announcement(buf);
		return false;
	}

	if (!pGenericXImage) {
		// Generic XImage not pointing to any particular data, yet.
		// FIrst call to AlterXImageData will do this
		pGenericXImage = DXL_CreateGenericXImage(NULL, 384, 512, DXL_INTERFRAME);
	}

	if (!pGenericXImage) {
		Common::strcpy_s(buf, 256, "Not enough room for Decompression Buffer");
		Announcement(buf);
		return false;
	}

	// the rewind buffer is used for ShowFrame to get random frames
	if (!pFFREWFrame) {
		pFFREWFrame = (unsigned char *)malloc(SIZEOFFFREW);  // later do this at start of app
	}
	if (!pFFREWFrame) {
		Common::strcpy_s(buf, 256, "Not enough room for Rewind Buffer");
		Announcement(buf);
		return false;
	}
	GlobalMaxToRead = 16384; // added to goose performance a bit
	//	init_GlobalMaxToRead();
	if (!VSInternal)	CreateInternalScreen();

	return true;			// worked
}
void init_GlobalMaxToRead() {
	Common::File f;
	Common::String string;
	extern int soff;
	static bool iniDone = false;

	if (!iniDone) {
		GlobalMaxToRead = 0;

		if (f.open("duck.ini")) {
			string = f.readLine();
			if (!string.empty())
				GlobalMaxToRead = atoi(string.c_str()) * 1024;

			string = f.readLine();
			if (!string.empty())
				soff = atoi(string.c_str());
		}

		iniDone = true;
	}
}

void MLI_DuckFree() {
	int i;
	if (LayerObjects) {
		for (i = 0; i < maxLayers; i++) MLI_DestroyLayer(&LayerObjects[i]);
		free(LayerObjects);
		LayerObjects = NULL;
	}
	if (pLayers) {
		free(pLayers);
		pLayers = NULL;
	}

	if (didMalloc && pInternalBuffer) {
		free((void *)pInternalBuffer);
		pInternalBuffer = NULL;
	}
	if (pGenericXImage) {
		DXL_DestroyXImage(pGenericXImage);
		pGenericXImage = NULL;
	}

	if (pDefaultBuffer) {
		HFB_DestroyBuffer(pDefaultBuffer);
		pDefaultBuffer = NULL;
	}

	DXL_DestroyVScreen(VSInternal);		// Give back Screens, etc.
	VSInternal = NULL;
	DXL_DestroyVScreen(VSScreen);
	VSScreen = NULL;
	MFP_Exit();			// shut down the driver                     
	DXL_ExitVideo();

	if (pFFREWFrame) {
		free(pFFREWFrame);
		pFFREWFrame = NULL;
	}

	AnnDone();					// flush announcement buffer to disk
}

// start high-bandwidth tracing
void StartTrace() {
	tracenow = true;
	SetTraceNow();	// and in the DLL
}

void EndTrace() {
	tracenow = false;
	ClearTraceNow();	// and in the DLL
}

} // namespace Spycraft
