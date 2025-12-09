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
#include "spycraft/dmade/duck_api.h"	  // must include prototypes b4 defining DLL fcn's
#include "spycraft/dmade/duck_mli.h"

namespace Spycraft {

/* the following mid-level routines implement defined functionality using lower
	level API calls from the DXL and HFB libararies
*/

MLI_LAYER_HANDLE *pLayers;
MLI_LAYER_HANDLE LayerObjects;
int maxLayers, LayerIndex;
int TopPriority;	// -1 for no layers;  else highest valid (can be 0)
ScrnInfo Screen;						// structure dexribes screen
DXL_VSCREEN_HANDLE VSScreen;			// points to Screen itself for displaying
MLI_LAYER_HANDLE AudioLayer;			// the layer now playing audio

extern syncst8 syncstate;
extern DXL_VSCREEN_HANDLE VSInternal;	// points to Internal Control Structure for decompression
extern bool didMalloc;				// set if had to malloc internal buffer and so have to free it
extern uchar *pInternalBuffer;		// points to internal buffer that was malloced
extern spkrst8 speakerstate;

#define MAX_DATA_RATE (16*1024*15)		// CD rate variables
extern int GlobalMaxToRead;
extern int GlobalMinToRead;
extern int FillAdjustFreq;
extern int CurrentReadSize;

MLI_LAYER_HANDLE Default_pL;			// If not using Layer ID's, this is the one...

enum proftype {
	PROFSYS, PROFAUDIO, PROFWDZAUDIO, // profiling defeinitions
	PROFDX, PROFBLIT, PROFBLITINTER, PROFDXINTER,
	PROFREAD, PROFTOT, PROFTIME, NUM_PROFILES
};

int ProfBuf[NUM_PROFILES];
int prof_time, prof_start, ProfTotal;
char errorbuf[2048];
static uchar *SingleFrameBuffer;			// hold frame for FF or REW
#define SINGLESIZE 65535			// max size of frame expected for 320 * 240 * 16-bit video

/* utility prototypes */
int	SMP_DeletePriority(MLI_LAYER_HANDLE pL);		// deletes pL from priority list, if there
void CloseAll(MLI_LAYER_HANDLE pL);		// close all open files in all Layers
int 	ProfLine(char *bufp, const char *string, int value);  // outputs line of profile info
int 	GetProf(proftype ptype);		// get a profiling value
void StartTrace();
void EndTrace();
int  SMP_GetElapsedTime();
extern int tracenow;
/* external stuff, mostly for profiling and statistics */

extern char buf[];						// to stick text into...
extern int synchFrame, droppedFrames, LastFillFrame, DecodedFrames;  // statistics
extern int RePaint;
extern clock_t timer_time;
extern int framesPlayed;						// other flags (like speakerstate or syncstate) are untouched
extern int key, inter;

// describe screen characteristics (x, y, w, h measured in screen space)
int MLI_Screen(uchar *ScreenAddr, int x, int y, int w, int h, int pitch,
	enum BITDEPTH BitDepth, enum BLITQUALITY BlitQuality)
	// pitch is sometime referred to as "stride" or RowBytes"
	// distance (in bytes) between two pixels in the same column on adjacent rows
{
	Screen.ScrnBox.x = x;
	Screen.ScrnBox.y = y;
	Screen.ScrnBox.w = w;
	Screen.ScrnBox.h = h;
	Screen.bd = BitDepth;
	Screen.bq = BlitQuality;
	Screen.ScrnPtr = ScreenAddr;		// location 0,0
	Screen.ScrnPitch = pitch;
	Screen.SizeOfPixel = (Screen.bd == DXRGB8) ? 1 :
		(Screen.bd == DXRGB16) ? 2 :
		(Screen.bd == DXRGB24) ? 3 : 0;
	// Set up a virtual screen for output
	if ((VSScreen = DXL_CreateVScreen(Screen.ScrnPtr, Screen.bd,
		(short)Screen.ScrnPitch, (short)Screen.ScrnBox.h)) == NULL) {
		Announcement("Can't allocate memory for Vscreen");
		return false;
	}
	DXL_SetBlitQuality(VSScreen, Screen.bq);
	return true;
}

void MLI_AlterScreenPointer(unsigned char *ptrScreen) {
	Screen.ScrnPtr = ptrScreen;
	DXL_AlterVScreen(VSScreen, ptrScreen, DXRGBNULL, -1, -1);
}

// set view	(video top left at top left of view.(x, y, w, h measured in screen space)
int MLI_View(MLI_LAYER_HANDLE pL, int x, int y, int w, int h) {
	if (!pL) pL = Default_pL;

	if (pL->LFlags & LAYER_IN_USE) {
		DXL_AlterVScreenView(pL->LVScreen, x, y, w, h);
		return true;
	}
	return false;
}

// Default Dimensions are full Screen (this API not Called)

// set clipping rectangle (within View) (Clip is Clipped to View automatically)
int MLI_Clip(MLI_LAYER_HANDLE pL, int x, int y, int w, int h) {
	if (!pL) pL = Default_pL;

	if (pL->LFlags & LAYER_IN_USE) {
		DXL_AlterVScreenClip(pL->LVScreen, x, y, w, h);
		return true;
	}
	return false;
}
// Default Dimensions are full Screen (this API not Called)

// Specify Max Number of Layers that will be open at any one time
int MLI_Init(int NumberLayers)
// Default number of Layers is one (this API not Called).
// false out of memory for control structures
// or not permitted (previous MLI_Open issued)
{
	ErrorBuffer(errorbuf);			// tell DLL where to store errors (in the EXE address space)

	if (NumberLayers <= 0)	return false;	// bad parameter
	// allocate pointers to Layers.  Used for priority
	pLayers = (MLI_LAYER_HANDLE *)(calloc((NumberLayers + 1), sizeof(MLI_LAYER_HANDLE)));
	if (pLayers == NULL) return -1;
	// note calloc set arrays to 0;
	LayerObjects = (MLI_LAYER_HANDLE)calloc(maxLayers = NumberLayers, sizeof(MLI_LAYER));
	if (LayerObjects == NULL) return -1;

	TopPriority = -1;
	// rewind Buffer used for Single-Frame reads

	AudioLayer = NULL;	// unspecified
	return true;		// worked
}

int MLI_Erase() {
	if (!VSScreen) return false;
	BlackDuck(VSScreen);
	RePaint = true;			// force this to full repaint
	return true;			  // actually erase
}

// Close All Open Movies;  free Layer Structures
void MLI_Exit() {

	// Everything else that EndDuck Does to give all back
	clock_t fred, end_time;
	char *bufp = buf;

	if (Default_pL) {
		MLI_DestroyLayer(Default_pL);
		Default_pL = NULL;
	}
	EndAudio();
	resetDisplay(VSScreen);

	DXL_DestroyVScreen(VSInternal);		// Give back Screens, etc.
	VSInternal = NULL;
	DXL_DestroyVScreen(VSScreen);
	VSScreen = NULL;

	end_time = timeGetTime();
	fred = end_time - timer_time;
	framesPlayed = (long)(DecodedFrames * 1000 + fred / 2) / fred;

	if (framesPlayed) {
		Common::sprintf_s(buf, 256, "Done -- frame rate %d Decoded %d, skipped %d\n", framesPlayed, DecodedFrames, droppedFrames);
		Announcement(buf);
	}

	if (Get_show_dots()) {
		ProfLine(bufp, "Sys          ", GetProf(PROFSYS));
		Announcement(buf);
		ProfLine(bufp, "Audio        ", GetProf(PROFAUDIO));
		Announcement(buf);
		ProfLine(bufp, "Audio Take   ", GetAudioProfile());
		Announcement(buf);
		ProfLine(bufp, "Dx           ", GetProf(PROFDX));
		Announcement(buf);
		ProfLine(bufp, "DxInter      ", GetProf(PROFDXINTER));
		Announcement(buf);

		ProfLine(bufp, "Blit         ", GetProf(PROFBLIT));
		Announcement(buf);
		ProfLine(bufp, "BlitInter    ", GetProf(PROFBLITINTER));
		Announcement(buf);

		ProfLine(bufp, "Disp         ",
			GetProf(PROFDX) + GetProf(PROFDXINTER) + GetProf(PROFBLIT) + GetProf(PROFBLITINTER));
		Announcement(buf);

		ProfLine(bufp, "Read         ", GetProf(PROFREAD));
		Announcement(buf);
		ProfLine(bufp, "Time         ", GetProf(PROFTIME));
		Announcement(buf);
		Common::strcpy_s(bufp, 256, "\n\r");
		Announcement(buf);
		ProfLine(bufp, "Work         ", GetProf(PROFAUDIO) + GetProf(PROFWDZAUDIO) + GetProf(PROFDXINTER) + GetProf(PROFBLITINTER) + GetProf(PROFDX) + GetProf(PROFBLIT) + GetProf(PROFREAD));
		Announcement(buf);
		ProfLine(bufp, "Total        ", GetProf(PROFTOT));
		Announcement(buf);
		Common::sprintf_s(bufp, 256, "\n\r Keyframe %d,  interframes %d", key, inter);

		Announcement(buf);
		Common::sprintf_s(bufp, 256, "\n\r ms / DXKF %f ms / BLITKF %f",
			(float)GetProf(PROFDX) / key, (float)GetProf(PROFBLIT) / key);
		Announcement(buf);
		Common::sprintf_s(bufp, 256, "\n\rms / DXIF %f, ms / BLITIF %f\n\r",
			(float)GetProf(PROFDXINTER) / inter, (float)GetProf(PROFBLITINTER) / inter);
		Announcement(buf);
	}
}

// Set up a Layer
MLI_LAYER_HANDLE MLI_CreateLayer(bool Sprite, HFB_BUFFER_HANDLE DefaultBuffer, int Size, int LBuffPreload, enum HFB_Modes BufferMode, int bStream)
// Sprite true for Sprite (in memory)
// Sprite false for Streaming AVI

// Size is Size of buffer to use

// Default Init for a Layer is Stream, 0.3 Meg (This API Not Called
// false Invalid Layer

{
	int i;
	MLI_LAYER_HANDLE pL = LayerObjects;

	for (i = 0; i < maxLayers; i++, pL++)		// find a free layer
		if (!(pL->LFlags & LAYER_EXISTS)) break;

	if (i >= maxLayers) return NULL;

	pL->LFlags = LAYER_IN_USE;
	pL->LFlags |= LAYER_EXISTS;
	if (Sprite)	pL->LFlags |= LAYER_IS_SPRITE;
	if (bStream) pL->LFlags |= LAYER_STREAMS;
	pL->LbuffSize = Size;
	pL->LbuffPreload = LBuffPreload;
	pL->LvidChunkIndex = pL->LaudChunkIndex = -1;	// no valid chunks to release
	// make a buffer into which we read the CD Stream
	if (DefaultBuffer) {
		pL->LBuffer = DefaultBuffer;	// pre-allocated, use that
		// Question!! Should check size requested against MAX
		HFB_ConditionBuffer(DefaultBuffer, Size, 0);
		pL->LFlags |= PREALLOCATED_HFB_BUFFER;
	} else {
		if ((pL->LBuffer = HFB_CreateBuffer(Size, 0)) == NULL)	return NULL;
		pL->LFlags &= ~PREALLOCATED_HFB_BUFFER;
	}
	if (BufferMode) {
		HFB_SetBufferMode(pL->LBuffer, BufferMode);
	}
	pL->LPriority = MLI_TopPriority(pL);
	return pL;			// Handle to brand-new Layer
}

// return frames in entire Movie
int MLI_GetFrames(MLI_LAYER_HANDLE pL)
// else number of frames
{
	MFP_STREAM_INFO *pInfo;

	if (!pL)	pL = Default_pL;
	if (!(pL->LVStream))	return false;	// no stream
	if (!(pInfo = MFP_GetStreamInfo(pL->LVStream)))	return false;
	return pInfo->lLength;
}

MLI_LAYER_HANDLE MLI_Open(MLI_LAYER_HANDLE pL, const char *name, DXL_XIMAGE_HANDLE DefaultXImage, bool DirtyBuffer)	// open Movie whose FQP (Fully Qualified Path) is in "name"
{
	if (!pL) {
		if (!Default_pL) {
			// just set up the one Layer for Box of Video-type operation
			if ((Default_pL = MLI_CreateLayer(false, pDefaultBuffer, DEFAULTBUFFERSIZE, -1, // default favors fast opening, small files.
				//  Won't work with large index tables!!!!!
				HFBMODE_NORMAL, true)) == NULL)
				return nullptr;
		}
		pL = Default_pL;
	}

	pL->LFile = MFP_OpenFile(name, pL->LBuffer);	// open Desired File

	if (pL->LFile == NULL) {
		Common::sprintf_s(buf, 256, "Can't open File: %s.  Is it a Truemotion .avi file?", name);
		Announcement(buf);
		if (!(pL->LFlags & PREALLOCATED_HFB_BUFFER))	HFB_DestroyBuffer(pL->LBuffer);
		return nullptr;
	}
	HFB_InitBuffer(pL->LBuffer, pL->LFile, 0, pL->LbuffPreload);

	pL->LVStream = MFP_GetStream(pL->LFile, NULL, 1, DUK_VIDSTREAM);
	if (pL->LVStream) {
		pL->LFlags |= LAYER_HAS_VIDEO;
		pL->LFlags &= ~LAYER_VIDEO_OFF;
	}

	pL->LAStream = MFP_GetStream(pL->LFile, NULL, 1, DUK_AUDSTREAM);
	if (pL->LAStream) {
		pL->LFlags |= LAYER_HAS_AUDIO;
		pL->LFlags &= ~LAYER_AUDIO_OFF;
	}

	MLI_TopPriority(pL);		// promote to top of stack

	pL->LFirstFrame = pL->LFrameIndex = 1;
	pL->LLastFrame = -1;


	pL->LvidChunkIndex = HFB_GetStreamingData(pL->LVStream, (void **)&pL->Lcompbuf, &pL->Lframelen, DUK_FORWARD, 1);
	if (DefaultXImage) {
		pL->LXImage = DefaultXImage;
		pL->LFlags |= PREALLOCATED_DXL_XIMAGE;	// set flag so we don't destroy this at the end
	} else {
		if ((pL->LXImage = DXL_CreateXImage(pL->Lcompbuf)) == NULL) {
			Common::sprintf_s(buf, 256, "Can't create internal XImage for : %s\n", name);
			Announcement(buf);
			return nullptr;
		}
		pL->LFlags &= ~PREALLOCATED_DXL_XIMAGE;	// set flag so we destroy this at the end
	}
	if (DirtyBuffer) {
		if (!DXL_CreateDirtyBuffer(pL->LXImage)) {
			Common::sprintf_s(buf, 256, "Can't create internal Sprite buffer for : %s\n", name);
			Announcement(buf);
			return nullptr;
		}
	}
	DXL_AlterXImageData(pL->LXImage, pL->Lcompbuf);
	// point to decompressed data (first frame).  Also sets up height, width, etc...

	DXL_GetXImageXYWH(pL->LXImage, (int *)&(pL->LImgBox.x),  // recover video bounds
		(int *)&(pL->LImgBox.y),
		(int *)&(pL->LImgBox.w),
		(int *)&(pL->LImgBox.h));

	// set up CD read rates

	MFP_GetFrameRates(pL->LVStream, pL->LAStream, &(pL->LframeRate), &(pL->LAudioSkew));
	if (!pL->LframeRate) {
		Announcement("No frame rate specified -- using 15 fps");
		pL->LframeRate = 15;
	}
	GlobalMaxToRead = MAX_DATA_RATE / pL->LframeRate;
	GlobalMinToRead = GlobalMaxToRead / 2;
	FillAdjustFreq = pL->LframeRate / 3;
	CurrentReadSize = GlobalMinToRead;

	return pL;
}

int MLI_SetAudioLayer(MLI_LAYER_HANDLE pL) {
	if (!pL)	pL = Default_pL;
	AudioLayer = pL; 		// can be set even if not using audio.  Need place
	// to same constants such as frame rate
	if ((pL->LFlags & (LAYER_IN_USE | LAYER_HAS_AUDIO)) ==
		(LAYER_IN_USE | LAYER_HAS_AUDIO)) {
		return (int)AudioLayer;
	} else {
		// no audio stream.  Set sync and speaker to run without 
		// consequence here is that we can never get back to using speaker unless we specifically do so
#if false	// spec please!!??
		if (syncstate == SYNCSPEAKER) {
			syncstate = SYNCSYSTEM;
			speakerstate = SPEAKEROFF;
			SetStates(speakerstate, Screen.Is565);
			Announcement("Background video with no audio stream -- sync off system clock\n");
		}
#endif
		return NULL;
	}
}

void MLI_EndAudio() {
	EndAudio();				// terminate
	AudioLayer = NULL;		// no longer have reference audio layer
}

MLI_LAYER_HANDLE MLI_GetAudioLayer() {
	return AudioLayer;
}

int MLI_StartPlaying() {
	int ret = false;

	timer_time = timeGetTime();  // time all this started
	droppedFrames = DecodedFrames = 0;  // statistics

	if ((speakerstate == FEEDSPEAKER) || (speakerstate == MONOSPEAKER)) {
		if (AudioLayer) {
			AudioLayer->LFlags &= ~LAYER_AUDIO_OFF;
			StartPlaying();
			ret = true;
		}
	}
	LastFillFrame = 0;		// synch fill buffer frequency with frame rate
	return ret;
}

int MLI_StopPlaying() {
	if (AudioLayer) {
		AudioLayer->LFlags |= LAYER_AUDIO_OFF;
		StopPlaying();
		return true;
	}
	return false;		// no layer to deal with
}

int MLI_IsAudioPlaying() {
	if (AudioLayer) {
		if (AudioLayer->LFlags & LAYER_AUDIO_OFF)	return false;
		return true;
	}
	return false;		// no layer to deal with
}

int MLI_TimeSpeaker() {
	if (AudioLayer && MLI_IsAudioPlaying()) {
		return TimeSpeaker();
	}
	return false;		// no layer to deal with
}

int MLI_TakeBack() {
	if (AudioLayer && MLI_IsAudioPlaying()) {
		TakeBack();
		return true;
	}
	return false;		// no layer to deal with
}

int MLI_DestroyLayer(MLI_LAYER_HANDLE pL) {
	if (!pL)	pL = Default_pL;
	MLI_Close(pL);							// close open files; free space
	pL->LFlags = 0;
	return true;		// worked
}

int MLI_Close(MLI_LAYER_HANDLE pL) {
	if (!pL)	pL = Default_pL;
	if (pL->LFlags & LAYER_EXISTS) {
		CloseAll(pL);
		SMP_DeletePriority(pL);		// note we don't free the layer, so it can be re-used
		pL->LFlags &= ~(LAYER_HAS_VIDEO | LAYER_HAS_AUDIO);
	}
	return true;
}

// set this layer to top priority
int MLI_TopPriority(MLI_LAYER_HANDLE pL)
//returns priority.  -1 for invalid call
{
	int i;
	MLI_LAYER_HANDLE *ppL = pLayers;

	if (!pL)
		return -1;

	SMP_DeletePriority(pL);		// deletes pL from priority list, if there
	// now find end, and insert layer pointer
	for (i = 0; i < maxLayers; i++, ppL++) {
		if (!(*ppL))	break;
	}
	if (i >= maxLayers) return -1;		// failed

	TopPriority = pL->LPriority = i;	// set in structure, and globally
	*ppL = pL;			// fill in list
	return i;
}

// set this layer to Bottom priority
int MLI_BottomPriority(MLI_LAYER_HANDLE pL)
//returns priority.  -1 for invalid call
{
	int i;
	MLI_LAYER_HANDLE *ppL = pLayers;

	if (!pL)
		return -1;

	SMP_DeletePriority(pL);		// deletes pL from priority list, if there

	// now find end, and insert layer pointer
	for (i = TopPriority + 1, ppL = &pLayers[i]; i > 0; i--, ppL--) {
		*ppL = *(ppL - 1);
	}

	TopPriority++;
	pL->LPriority = 0;	// set in structure, and globally
	*ppL = pL;			// fill in list
	return 0;			// return its priority (lowest);
}

// return current frame number
int MLI_GetPosition(MLI_LAYER_HANDLE pL) {
	if (!pL)	pL = Default_pL;
	return pL->LFrameIndex;
}

void MLI_PlayRange(MLI_LAYER_HANDLE pL, int from, int to)	// Set play range 
{
	if (!pL)	pL = Default_pL;
	pL->LFirstFrame = from;
	pL->LLastFrame = to;		// -1 means go to end and stop
	MLI_Seek(pL, pL->LFirstFrame);  // go to the first frame that we can
}

// move layer location (x, y measured in screen space)
void MLI_LocateLayer(MLI_LAYER_HANDLE pL, enum OFFSETXY offsetmode, int x, int y)
// NB enum OFFSETXY { DXL_ABSOLUTE = 0, DXL_RELATIVE
// Default Location is 0,0 (this API not Called)
{
	if (!pL)	pL = Default_pL;
	DXL_MoveXImage(pL->LXImage, offsetmode, x, y);
}
// Audio On
void MLI_EnableAudio(MLI_LAYER_HANDLE pL, bool flag)
// true to turn on, false to turn off
// Note.  Audio comes from One Layer Only!! Enabling one Layer turns off the other
{
	if (!pL)	pL = Default_pL;
	if (flag)	pL->LFlags &= ~LAYER_AUDIO_OFF;	// backward so clear is enabled
	else		pL->LFlags |= LAYER_AUDIO_OFF;
}

// Video On
void MLI_EnableVideo(MLI_LAYER_HANDLE pL, bool flag)
// true to turn on, false to turn off
{
	if (!pL)	pL = Default_pL;
	if (flag) {
		pL->LFlags &= ~LAYER_VIDEO_OFF;	// backward so clear is enabled
		RePaint = true;			// force this to full repaint when turning back on
	} else		pL->LFlags |= LAYER_VIDEO_OFF;
}

void MLI_EnableLayer(MLI_LAYER_HANDLE pL, bool flag)
// true to turn whole Layer on, false to turn off
{
	if (!pL)	pL = Default_pL;
	if (flag)	pL->LFlags &= ~LAYER_OFF;	// backward so clear is enabled
	else		pL->LFlags |= LAYER_OFF;
}

/************************ Utilities ***************************/

// Delete Entry from Priority Table
int	SMP_DeletePriority(MLI_LAYER_HANDLE pL)		// deletes pL from priority list, if there
{
	int i;
	MLI_LAYER_HANDLE *ppL = pLayers;

	if (!pL)
		return -1;

	// now find this one, and move all others up
	for (i = 0; i < maxLayers; i++, ppL++) {
		if (*ppL == pL)	break;
	}
	if (i >= maxLayers) return -2;		// failed to find
	pL->LPriority = -1;			// mark unknown
	for (; i < maxLayers; i++, ppL++) {
		*ppL = *(ppL + 1);
		if (!*ppL)	break;
	}
	TopPriority = i - 1;
	return TopPriority;
}

// routines to traverse the Layers
// returns pointer to first layer, whether or not it's active
MLI_LAYER_HANDLE MLI_FirstLayer() {
	LayerIndex = 0;
	if (pLayers) return pLayers[LayerIndex];
	else return NULL;
}

// returns pointer to last layer

MLI_LAYER_HANDLE MLI_LastLayer() {
	LayerIndex = maxLayers - 1;
	return pLayers[LayerIndex];
}

// returns pointer to next layer -- NULL for end of list
MLI_LAYER_HANDLE MLI_NextLayer() {
	if (++LayerIndex == maxLayers) return NULL;
	return pLayers[LayerIndex];		// which is null for unused Layers
}

MLI_LAYER_HANDLE MLI_PrevLayer() {
	if (--LayerIndex < 0) return NULL;
	return pLayers[LayerIndex];		// which is null for unused Layers
}

// returns point to first active layer (Video or audio in use)
MLI_LAYER_HANDLE MLI_FirstActiveLayer() {
	MLI_LAYER_HANDLE pL;

	if ((pL = MLI_FirstLayer()) == NULL) return pL;
	if (!(pL->LFlags & LAYER_IN_USE)) {
		pL = MLI_NextActiveLayer();
	}
	return pL;
}

// returns point to last active layer (Video or audio in use)
MLI_LAYER_HANDLE MLI_LastActiveLayer() {
	MLI_LAYER_HANDLE pL;

	if ((pL = MLI_LastLayer()) == NULL) {
		if ((pL = MLI_PrevActiveLayer()) == NULL)	return pL;
	}
	if (!(pL->LFlags & LAYER_IN_USE)) {
		pL = MLI_PrevActiveLayer();
	}
	return pL;
}

// returns point to next active layer (Video or audio in use)
MLI_LAYER_HANDLE MLI_NextActiveLayer() {
	MLI_LAYER_HANDLE pL;

	while (true) {
		if ((pL = MLI_NextLayer()) == NULL)  break;
		if (pL->LFlags & LAYER_IN_USE)	break;
	}
	return pL;
}

// returns point to next active layer (Video or audio in use)
MLI_LAYER_HANDLE MLI_PrevActiveLayer() {
	MLI_LAYER_HANDLE pL;

	while (true) {
		if (LayerIndex == 0)	return NULL;
		if ((pL = MLI_PrevLayer()) == NULL)  continue;	// end of list not filled in
		if (pL->LFlags & LAYER_IN_USE)	break;	// got one that's in Use
	}
	return pL;
}

void CloseAll(MLI_LAYER_HANDLE pL) {
	if (pL->LVStream) 	MFP_ReleaseStream(pL->LVStream);
	if (pL->LAStream) 	MFP_ReleaseStream(pL->LAStream);
	if (pL->LFile) 		MFP_CloseFile(pL->LFile);
	if (!(pL->LFlags & PREALLOCATED_HFB_BUFFER) && pL->LBuffer)
		HFB_DestroyBuffer(pL->LBuffer);
}
/********************** Profiling ************************/
int ProfLine(char *bufp, const char *string, int value) {
	int ret;
	float TPF;
	int pct;

	TPF = (DecodedFrames) ? (float)value / DecodedFrames : (float)0.0;
	pct = (GetProf(PROFTOT)) ? (value * 100) / GetProf(PROFTOT) : 0;

	ret = Common::sprintf_s(bufp, 256, "\n\r  %s  \t %d,  \t %ld %% \t Ticks/Frame %f",
		string, value, pct, TPF);
	return ret;
}

int GetProf(proftype ptype) {
	int bucket = (int)ptype;

	if (bucket > NUM_PROFILES)	return NULL;
	if (ptype == PROFTOT) return ProfTotal;
	return ProfBuf[bucket];
}

} // namespace Spycraft
