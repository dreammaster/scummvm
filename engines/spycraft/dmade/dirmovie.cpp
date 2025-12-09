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
#include "spycraft/dmade/duck_api.h"
#include "spycraft/dmade/duck_mli.h"
#include "spycraft/dmade/advlib.h"
#include "spycraft/dmade/advmovie.h"
#include "spycraft/dmade/advscreen.h"

namespace Spycraft {

extern DXL_VSCREEN_HANDLE VSScreen, VSInternal;
extern MLI_LAYER_HANDLE AudioLayer;

extern char errorbuf[];

extern int offsetX;
extern int offsetY;
extern int dx_rowBytes;
extern int pixFormat;

void *LockPtr(void);
void UnlockPtr(void *p);
void UpdateScreenRectEx(Viewport *port, SRect *rect);


jmp_buf audioErrorContext;		// for getting out of audio errors

/* NEED TO CLEAN UP THESE GLOBALS */
int VTR_Status = false;
int FastSprite = false;
int synchFrame, droppedFrames, LastFillFrame, DecodedFrames;
char buf[1024];
int NoSkip = false;

int  IsDone();
void NotDone();
void SetDone();
void StartProfiling();
int  CreateInternalScreen();
void SMP_GetData(MLI_LAYER_HANDLE pL);

typedef struct {
	int               x;
	int               y;
	bool              opened;
	bool              playing;
	bool              paused;
	bool              audioOff;
	bool              videoOff;
	int               from;
	int               to;
	bool              close_when_done;
	bool              erase_when_done;
	callbackFn        callBack;
	HANDLE            hMovie;
	DWORD             threadID;
	CRITICAL_SECTION  cts;
	MLI_LAYER_HANDLE  videoLayer;
	bool              endAudio;
	bool               clearScreen;
	int               width;
	int               height;
} __MovieInfo;


spkrst8 speakerstate;
syncst8 syncstate;

static SRect movieRect = {
   0, 0, 511, 479, 512, 480
};

static __MovieInfo mvi = {
   0, 0, false, false, false, false, false, 1, -1,
   false, true, nullptr, nullptr, 0, nullptr, nullptr,  false, false, 0, 0
};

void EraseAltLine() {
	int i;
	BYTE *p = (BYTE *)LockPtr();
	BYTE *start = p + mvi.y * dx_rowBytes + (mvi.x << 1);

	for (i = 0; i < mvi.height; i++) {
		if (i & 1)
			memset(start, 0, (mvi.width << 1));
		start += dx_rowBytes;
	}

	UnlockPtr(p);
}

void CleanMVI() {
	mvi.x = 0;
	mvi.y = 0;
	mvi.hMovie = nullptr;
	mvi.threadID = 0;
	mvi.clearScreen = true;
}

int sfxPlayMovie(int closeDone, int eraseDone, callbackFn theCallBack) {
	if (!mvi.opened) {
		dbgmsg("sfxPlayMovie(): no movie is currently open.");
		if (theCallBack)
			(*theCallBack)();
		return (false);
	}

	if (mvi.playing && !mvi.paused) {
		dbgmsg("sfxPlayMovie(): another movie is currently playing. paused=%d playing=%d", mvi.paused, mvi.playing);
		return (false);
	}

	/* INITIALIZATION */
	if (closeDone > 0)
		mvi.close_when_done = true;
	else
		mvi.close_when_done = false;

	if (eraseDone)
		mvi.erase_when_done = true;
	else
		mvi.erase_when_done = false;

	mvi.callBack = theCallBack;
	mvi.audioOff = false;
	mvi.videoOff = false;

	if (mvi.paused) {
		MLI_PlayRange(mvi.videoLayer, mvi.from, mvi.to);

		sfxResumeMovie();
		mvi.playing = true;

		return true;
	}

	mvi.paused = false;

	if (mvi.endAudio) {
		if (MLI_SetAudioLayer(mvi.videoLayer)) {
			if (!SetupAudio(AudioLayer->LBuffer, AudioLayer->LAStream,
				&AudioLayer->LaudChunkIndex, (void **)&AudioLayer->Laudioaddr,
				&AudioLayer->Laudiolen, AudioLayer->LbuffPreload))
			{
				mvi.opened = false;
				return (false);
			}
			mvi.endAudio = false;
		}
		MFP_GetFrameRates(AudioLayer->LVStream, AudioLayer->LAStream,
			&(AudioLayer->LframeRate), &(AudioLayer->LAudioSkew));
	}

	CreateInternalScreen();

	if (mvi.clearScreen)
		EraseAltLine();

	/* FROM-TO FRAME HOOK */
	MLI_LAYER_HANDLE pL = MLI_FirstActiveLayer();
	MLI_PlayRange(pL, mvi.from, mvi.to);

	MLI_Prefill(pL);

	NotDone();

	MLI_StartPlaying();
	StartProfiling();

	mvi.playing = true;
	mvi.clearScreen = false;

	return (true);
}

int sfxOpenMovie(const char *filename, int dx, int dy, int size) {
	if (mvi.opened) {
		dbgmsg("Attempt to open a movie while not closing the current movie!!!");
		return (false);
	}

	if (mvi.playing) {
		dbgmsg("Attempt to open a movie while the current movie is playing!!!");
		return (false);
	}

	longjmpOnAudioErrors(&audioErrorContext);

	CleanMVI();

	// speakerstate = SPEAKEROFF and syncstate = SYNCSYSTEM, for movie with no sound
	speakerstate = FEEDSPEAKER;
	syncstate = SYNCSPEAKER;
	SetStates(speakerstate, pixFormat);

	if ((mvi.videoLayer = MLI_Open(nullptr, filename, pGenericXImage, false)) == 0) {
		dbgmsg("Cannot open %s movie", filename);
		return (false);
	}

	if (mvi.videoLayer->LFlags & LAYER_HAS_AUDIO) {
		MLI_EnableAudio(mvi.videoLayer, true);
		mvi.audioOff = false;
	} else {
		speakerstate = SPEAKEROFF;
		syncstate = SYNCSYSTEM;
		SetStates(speakerstate, pixFormat);
		MLI_EnableAudio(mvi.videoLayer, false);
		mvi.audioOff = true;
	}

	mvi.opened = false;
	mvi.x = dx + offsetX;
	mvi.y = dy + offsetY;

	BYTE *screenPtr = (unsigned char *)LockPtr();

	mvi.width = mvi.videoLayer->LImgBox.w;
	mvi.height = mvi.videoLayer->LImgBox.h;
	if (size == 1) {
		mvi.clearScreen = false;
		MLI_Screen(screenPtr, 0, 0, 640, 480, dx_rowBytes,
			DXRGB16, DXBLIT_SAME);
		DXL_AlterVScreenView(VSScreen, mvi.x, mvi.y, mvi.width, mvi.height);
		DXL_AlterVScreenClip(VSScreen, mvi.x, mvi.y, mvi.width, mvi.height);
	} else {
		MLI_Screen(screenPtr, 0, 0, 640, 480, dx_rowBytes,
			DXRGB16, DXBLIT_STRETCH);
		mvi.width *= 2;
		mvi.height *= 2;
		DXL_AlterVScreenView(VSScreen, mvi.x, mvi.y, mvi.width, mvi.height);
		DXL_AlterVScreenClip(VSScreen, mvi.x, mvi.y, mvi.width, mvi.height);
	}

	UnlockPtr(screenPtr);

	if (MLI_SetAudioLayer(mvi.videoLayer)) {
		if (!SetupAudio(AudioLayer->LBuffer, AudioLayer->LAStream,
			&AudioLayer->LaudChunkIndex, (void **)&AudioLayer->Laudioaddr,
			&AudioLayer->Laudiolen, AudioLayer->LbuffPreload))
		{
			mvi.opened = false;
			dbgmsg("Audio setup failure during movie open");
			return (false);
		}
		mvi.endAudio = false;
	}

	MFP_GetFrameRates(AudioLayer->LVStream, AudioLayer->LAStream,
		&(AudioLayer->LframeRate), &(AudioLayer->LAudioSkew));

	mvi.opened = true;


	return (true);
}

int sfxCloseMovie() {
	if (mvi.opened) {

		/* STOP MOVIE */
		if (mvi.playing) {
			mvi.playing = false;

			if (mvi.erase_when_done)
				UpdateScreenRectEx(backgrounds[curBack]->animPort, &movieRect);

			/* RELEASE DRIVER */
			EndAudio();
			mvi.endAudio = true;
		}

		MLI_Exit();
		mvi.opened = false;
		mvi.from = 1;
		mvi.to = -1;
		CleanMVI();

		if (mvi.callBack) {
			(*(mvi.callBack))();
		}

		return (true);
	}
	return (false);
}

int sfxIsMovieOpen() {
	return (mvi.opened);
}

int sfxSetMovieRange(int from, int to) {
	mvi.from = from + 1;
	mvi.to = to + 1;
	return (true);
}

int sfxPauseMovie(int repaint, int doCallBack) {
	if (mvi.opened) {
		MLI_Pause(mvi.videoLayer);
		mvi.paused = true;
		if (repaint)
			UpdateScreenRectEx(backgrounds[curBack]->animPort, &movieRect);

		if (doCallBack && mvi.callBack) {
			(*(mvi.callBack))();
		}

		return (true);
	}
	return (false);
}

int sfxResumeMovie() {
	if (mvi.opened && mvi.paused) {
		MLI_Resume(mvi.videoLayer);
		mvi.paused = false;
		return (true);
	}
	return (false);
}

int sfxEnableMovieAudio(int tOrF) {
	if (!mvi.opened)
		return (false);

	if (tOrF) {
		if (mvi.audioOff) {
			mvi.audioOff = false;
			MLI_EnableAudio(AudioLayer, true);
		}
	} else {
		if (!mvi.audioOff) {
			mvi.audioOff = true;
			MLI_EnableAudio(AudioLayer, false);
		}
	}
	return (false);
}

int sfxEnableMovieVideo(int tOrF) {
	if (!mvi.opened)
		return (false);

	if (tOrF) {
		if (mvi.videoOff) {
			mvi.videoOff = false;
			MLI_EnableVideo(mvi.videoLayer, true);
		}
	} else {
		if (!mvi.videoOff) {
			mvi.videoOff = true;
			MLI_EnableVideo(mvi.videoLayer, false);
			UpdateScreenRectEx(backgrounds[curBack]->animPort, &movieRect);
		}
	}
	return (false);
}

int sfxGetMoviePosn() {
	if (mvi.opened)
		return (MLI_GetPosition(mvi.videoLayer));

	return (0);
}

int sfxStepMovie(int location) {
	if (mvi.opened) {
		if (!VSInternal)
			CreateInternalScreen();

		BYTE *screenPtr = (BYTE *)LockPtr();
		DXL_AlterVScreen(VSScreen, screenPtr, DXRGBNULL, -1, -1);
		int ret = MLI_ShowFrame(mvi.videoLayer, location);
		UnlockPtr(screenPtr);
		return (ret);
	}

	return (false);
}

void UpdateMovie() {
	BYTE *screenPtr;

	if (!mvi.playing || mvi.paused)
		return;

	if (!IsDone()) {
		screenPtr = (unsigned char *)LockPtr();
		DXL_AlterVScreen(VSScreen, screenPtr, DXRGBNULL, -1, -1);

		if (MLI_ProcessFrame(0)) {
			MLI_LAYER_HANDLE pL = MLI_FirstActiveLayer();
			if (pL->LLastFrame >= 0) {
				if (pL->LFrameIndex >= pL->LLastFrame) {
					SetDone();
				}
			}
			if (!IsDone())
				SMP_GetData(pL);
		}
		UnlockPtr(screenPtr);

	} else {
		/* RELEASE DRIVER */
		EndAudio();
		mvi.endAudio = true;

		if (mvi.close_when_done) {
			MLI_Exit();
			mvi.opened = false;
		}

		if (mvi.erase_when_done) {
			UpdateScreenRectEx(backgrounds[curBack]->animPort, &movieRect);
		}

		mvi.from = 1;
		mvi.to = -1;
		mvi.playing = false;

		/* CALLBACK FUNCTION */
		if (mvi.callBack)
			(*(mvi.callBack))();

	}
}

int InitMovie() {
	SRect_Init(&movieRect, offsetX, offsetY, 511 + offsetX, 479 + offsetY);

	return (MLI_DuckInit());
}

void CleanMovie() {
	MLI_DuckFree();
}

void SetMovieErase(int tOrF) {
	mvi.clearScreen = tOrF;
}

int sfxIsMoviePlaying(void) {
	return (mvi.playing);
}

} // namespace Spycraft
