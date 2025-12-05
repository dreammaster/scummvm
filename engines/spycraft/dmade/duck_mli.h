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

#ifndef SPYCRAFT_DMADE_DUCK_MLI_H
#define SPYCRAFT_DMADE_DUCK_MLI_H

#include "common/scummsys.h"

namespace Spycraft {

// Mid Level Interface routines.  Video Layers are numbered from Back to front
// Layer 0 is the farthest Back (Tpically, background video)

// Note that API's with default behavior need not be called
// Setup Routines

typedef struct {		// extent of rectangle bounding this object
	int		x;
	int		y;
	int		w;
	int		h;
} Box;

#define MODE15BIT 0
#define MODE16BIT 1

typedef struct {
	Box			ScrnBox;     // height and width of screen                                       
	short		ScrnPitch;		// distnace in bytes from line to line
	short		SizeOfPixel;	// bytes per pixel
	short		Is565;			// Screen Mode 16 or 15 Bit (555, set for 565)
	enum BITDEPTH bd;			// bit depth for VScreen setup -- e.g. DXRGB16
	enum BLITQUALITY bq;		// Quality of Blit to screen (e.g. VGA, same, stretched
	unsigned char *ScrnPtr;		// points to memory for this screen
} ScrnInfo;

typedef struct Layer {
#define LAYER_IN_USE 1		// if this Stream is not in use -- 
	// don't access or use it!!
#define LAYER_HAS_VIDEO 2	// has Video STream Data from AVI File
#define LAYER_HAS_AUDIO	4	// has Audio STream Data from AVI File
#define LAYER_OFF 8			// layer turned off if set
#define LAYER_IS_SPRITE 16	// set if Layer is a sprite and has DIrty Buffers, etc
#define LAYER_VIDEO_OFF 32	// Video turned off, this layer
#define LAYER_AUDIO_OFF 64	// Audio turned off, this layer
#define LAYER_STREAMS  128	// set if Layer streams from disk;  else entirely
#define LAYER_EXISTS  256	// set if exists at all.  Can Exist, yet not be in use
							// memory resident
#define PREALLOCATED_DXL_XIMAGE 512    // set if XImage was pre-allocated and we don't want to destroy it
#define PREALLOCATED_HFB_BUFFER 1024    // set if Buffer was pre-allocated and we don't want to destroy it
	int		LFlags;
	int		LPriority;		// Priority runs from 0 (back) to + number (front)
	Box		LImgBox;				// extent of this Image
	unsigned char *Lcompbuf;    // address of compressed header and data

	short *Laudioaddr;		// address pointing to compressed audio
	long 	Laudiolen;			// length of that data
	int 	LaudChunkIndex;		// index of audio stuff

	int 	LbuffSize;       	// buffer-related lifted from dukdos main
	int		LbuffPreload;
	int 	LFileSize;			// length of whole file (we may care if using in-memory buffer)

	int		LvidChunkIndex;		// index of chunk under scrutiny ??
	int		LFrameIndex;
	int		LFirstFrame;		// for loops
	int		LLastFrame;			// if set to -1, go to end and stop

	long	Lframelen;
	int		LframeRate;			// frame rate for this stream (usually background
	int		LAudioSkew;			// number of skewed audio samples

	MFP_FILE_HANDLE LFile;			// File handle
	MFP_STREAM_HANDLE LVStream;		// Video Stream Handle
	MFP_STREAM_HANDLE LAStream;		// audio buffer (if any)
	HFB_BUFFER_HANDLE LBuffer;		// points at buffer where data for this VStream sits

	DXL_VSCREEN_HANDLE LVScreen;	// points to vScreen  where we play this object
	DXL_XIMAGE_HANDLE  LXImage;	// info about compressed frames
} MLI_LAYER, *MLI_LAYER_HANDLE;

typedef uint32 clock_t;
extern clock_t timer_time;

// **********************These API defintions have been implemented...***************
// describe screen characteristics (x, y, w, h measured in screen space)
int MLI_Screen(unsigned char *ScreenAddr, int x, int y, int w, int h, int pitch,  /*DONE */
	enum BITDEPTH Bitdepth, enum BLITQUALITY BlitQuality);
// pitch is sometime referred to as "stride" or RowBytes"
// distance (in bytes) between two pixels in the same column on adjacent rows

// returns true if worked or false (not enought memory for internal structures)

// Specify Max Number of Layers that will be open at any one time   /*DONE*/
int MLI_Init(int NumberLayers);
// Default number of Layers is one (this API not Called).
// false out of memory for control structures
// or not permitted (previous MLI_Open issued)

// Set up a Layer		     /*DONE*/
MLI_LAYER_HANDLE MLI_CreateLayer(bool Sprite, 		// Set if Sprite
	HFB_BUFFER_HANDLE DefaultBuffer,	// set if want to use a default buffer
	int Size, 		  	// size of streaming buffer
	int LbuffPreload, 	// AMount, -1 for 3/4, -2 for full
	enum HFB_Modes BufferMode, // see HFB_Modes def.
	int bStreams); 		// set if want streaming data
// If SPrite set true, there is an associated Dirty Buffer

// Size is Size of buffer to use
// Default Init for a Layer is Stream, 0.5 Meg (This API Not Called
// false Invalid Layer

// Destroy a layer
int MLI_DestroyLayer(MLI_LAYER_HANDLE pL);	     /*DONE*/
// destroys layer associated with Handle

// set this layer to top priority	   /*DONE*/
int MLI_TopPriority(MLI_LAYER_HANDLE pL);
//returns priority.  -1 for invalid call

// set this layer to Bottom priority     /*DONE*/
int MLI_BottomPriority(MLI_LAYER_HANDLE pL);
//returns priority.  -1 for invalid call

// Close Movie
int MLI_Close(MLI_LAYER_HANDLE pL);	// close movie at this layer  /*DONE*/

// Close All Open Movies;  free Layer Structures    /*DONE*/
void MLI_Exit();

// Open Movie (.avi) file
// Note -- After first Movie has been opened, can't call MaxLayers

MLI_LAYER_HANDLE MLI_Open(MLI_LAYER_HANDLE pL,
	char *name,		  	// open Movie whose FQP (Fully Qualified Path) is in "name"
	DXL_XIMAGE_HANDLE DefaultXImage, // NULL to make one up, else pass in handle to pre-made XImage
	bool DirtyBuffer);  // set Buffer for sprites that will be displayed
// over interframe background video
// set Dirty Buffer to False for background video
// and for sprites that will go over intraframe video
// returns false for failure

// return current frame number
int MLI_GetPosition(MLI_LAYER_HANDLE pL);

// Audio On
void MLI_EnableAudio(MLI_LAYER_HANDLE pL, bool flag);
// True to turn on, False to turn off
// Note.  Audio comes from One Layer Only!! Enabling one Layer turns off the other

// Video On
void MLI_EnableVideo(MLI_LAYER_HANDLE pL, bool flag);
// True to turn on, False to turn off

void MLI_EnableLayer(MLI_LAYER_HANDLE pL, bool flag);
// True to turn whole Layer on, False to turn off

// move layer location (x, y measured in screen space)
void MLI_LocateLayer(MLI_LAYER_HANDLE pL, enum OFFSETXY offsetmode, int x, int y);
// NB enum OFFSETXY { DXL_ABSOLUTE = 0, DXL_RELATIVE
// Default Location is 0,0 (this API not Called)

void MLI_PlayRange(MLI_LAYER_HANDLE pL, int from, int to);	// Set play range 
//for background (lowest numbered layer)

void MLI_AlterScreenPointer(unsigned char *ptrScreen);
// Change pointer to Screen memory

// point at First (lowest priority) active layer
MLI_LAYER_HANDLE MLI_FirstActiveLayer();
// returns pointer to first active layer (Video or audio in use)
// returns NULL if no active layer

// points at next highest pritority layer
MLI_LAYER_HANDLE MLI_NextActiveLayer();
// returns point to next active layer (Video or audio in use)
// returns NULL if no more active layers

// point at First (lowest priority) active layer
MLI_LAYER_HANDLE MLI_LastActiveLayer();
// returns pointer to last active layer (Video or audio in use)
// returns NULL if no active layer

// points at next highest pritority layer
MLI_LAYER_HANDLE MLI_PrevActiveLayer();
// returns point to previous active layer (Video or audio in use)
// returns NULL if no more active layers

// Play video from here to there.  Return when done
int MLI_Play(int from, int to);    // from and to refer to background video (lowest non-sprite layer)
// if from < 0, plays from current pos'n).
// returns ending frame

// This API call goes away and doesn't return until
// the video is done or a Stop Character (see below) is pressed

int MLI_ProcessFrame(int WantedFrame);  // Put next frame up, if appropriate
// This API should be called in a game loop
// returns True if a frame was composited and displayed

// set done = true at end of play range

// Wanted Frame is normally 0, and sync is maintained by the audio sub-system
// If wanted frame is non-0, 

/* Code fragment example of use of MLI_ProcessFrame():

	MLI_PlayRange(from, to);
	while (true)	{		// game loop
		ddrval = lpDDSPrimary->Lock(NULL,&DDSPriDesc,0,NULL); // lock screen
		unsigned char *ScrnPtr = (unsigned char *)DDSPriDesc.lpSurface;

		MLI_AlterScreenPointer(ScrnPtr);
		MLI_ProcessFrame();
		if (done)	break;		// true at end of movie segment

		ddrval = lpDDSPrimary->Unlock(NULL);		// unlock again so we can debug
		.
		.
		.
		// other game loop things...
	}
*/

// Identify Layer from which we get audio
int MLI_SetAudioLayer(MLI_LAYER_HANDLE pL);
// Default (API not Called) is Background (lowest) layer

// returns false if layer does not exist or has no audio

// COmpletely done with audio...
void MLI_EndAudio();

// Access Layer from which we get audio
MLI_LAYER_HANDLE MLI_GetAudioLayer();
// Default (API not Called) is Background (lowest) layer

// returns false if layer does not exist or has no audio

// Stop Audio.  Set flag so we know layer is off
int MLI_StartPlaying();
// returns True if stopped, false if no audio

// Stop Audio.  Set flag so we know layer is off
int MLI_StopPlaying();
// returns True if stopped, false if no audio

// Returns true if audio is playing.  False if non-existent or off
int MLI_IsAudioPlaying();

// Returns speaker Time in msec. false if not running
int MLI_TimeSpeaker();

// returns true if took samples back out of buffer.
int MLI_TakeBack();


// Pause a layer (other Layers (e.g. sprites) continue to play)
// If Audio Enabled for this layer, audio pauses
bool MLI_Pause(MLI_LAYER_HANDLE pL);

// returns true if valid layer

// Resume a layer
bool MLI_Resume(MLI_LAYER_HANDLE pL);

// returns true if valid layer

// Resume plays to the end of the selected frame range

// Sets View to Black
int MLI_Erase();
// false No Screen Pointer

// set view	(video top left at top left of view.(x, y, w, h measured in screen space)
int MLI_View(MLI_LAYER_HANDLE pL, int x, int y, int w, int h);

// Default Dimensions are full Screen (this API not Called)

// set clipping rectangle (within View) (Clip is Clipped to View automatically)
int MLI_Clip(MLI_LAYER_HANDLE pL, int x, int y, int w, int h);

// Default Dimensions are full Screen (this API not Called)

// return frames in entire Movie
int MLI_GetFrames(MLI_LAYER_HANDLE pL);
// NULL if no useful info
// else number of frames

// Seek to Frame.  This actually causes the CD to do Something
// It's really a "cue", as it loads whichever buffers are needed

bool MLI_Seek(MLI_LAYER_HANDLE pL, int frame);
// NB Seek positions to non-keyframes by decompressing with no Blit

// decompress one frame to screen -- unbuffered read
int MLI_ShowFrame(MLI_LAYER_HANDLE pL, int frame);
// returns False on bad layer or off end of movie

void MLI_Prefill(MLI_LAYER_HANDLE pL);

/****************These functions are not implemented ******************/
// sugestions:
//
// Extra Utilities, mostly for debugging

int MLI_EnableProfiling(bool Flag);
// Enable Diagnostic skipping and profiling

void MLI_StopCharacter(char StopChar, bool MouseClick);
// Keyboard Character that stops (aborts) video
// NULL means no stopping from keyboard
// MouseCLick true if any Click also stops Video

// Default (API not called) no way to stop Video (Null, false)
int MLI_SwapLayers(int Layer1, int Layer2);
// Layer 1 and Layer 2 (AND ALL THEIR REFERENCES!!)are swapped
// E.G. After open, Layer1, Swap to 2, CLose using Layer 2.

// returns False on bad layer

void MLI_SetSync(enum syncst8 Sync);
// Set Clock Sync Type:  NOSYNC, SYNCSYS, SYNCSPEAKER
// Default is SYNCSPEAKER (API not Called)

void MLI_SetSpeaker(enum spkrst8 Speaker);
// Set Clock Sync Type:  SPEAKEROFF, SPEAKERON, FEEDSPEAKER
// Default is FEEDSPEAKER (API not Called)
//////////////////////////////////////////////////////////////////////////////////
// special area for single allocation of lots of Global memory buffers
//
extern unsigned char *pFFREWFrame;		// frame for FF, REW frames
extern HFB_BUFFER_HANDLE pDefaultBuffer;		// Buffer for CD
extern DXL_XIMAGE_HANDLE pGenericXImage;		// Decompression area

#define SIZEOFFFREW 65535	// max size of frame expected for Rev-style video (This is Huge, no?)
#define DEFAULTBUFFERSIZE (512) * 1024 
int MLI_DuckInit();		// allocate all this stuff
void MLI_DuckFree();	// give it back

} // namespace Spycraft

#endif
