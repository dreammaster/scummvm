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

#include "spycraft/game/movie.h"
#include "spycraft/dmade/advmovie.h"
#include "spycraft/dmade/advsound.h"
#include "spycraft/dmade/aviread.h"
#include "spycraft/game/game.h"
#include "spycraft/dmade/advcursor.h"
#include "spycraft/game/sound.h"
#include "spycraft/game/interface.h"
#include "spycraft/dmade/winscreen.h"
#include "spycraft/game/flag.h"

namespace Spycraft {

Movie *theMovie;
extern Intrface *intrface;

int soundGone, isBuffered;
bool intrfaceState;
int redrawBack;
int interrupted;

extern Room *curRoom;

void cueFun() {
	theMovie->cue();
}

Movie::Movie() {
	movieNum = 0;
	movieName = NULL;
	paused = false;
	lastPosn = 0;
	name = "Movie";
	caller = NULL;
	x = 0;
	y = 0;
	isOpen = false;
	closeWhenDone = true;
	isPlaying = false;
	soundNum = 0;
}

void Movie::play(const char *moviename, int theX, int theY, int width, int height, Object *whoToCue) {
	SRect movieRect;
	char theName[64];
	sfxGetDir(theName, atoi(moviename), RES_MOVIE);
	x = theX;
	y = theY;
	closeWhenDone = true;
	if (sounds->size) {
		if (sound1->isPlaying()) {
			soundNum = sound1->number;
			if (sound1->flag)
				isBuffered = true;
			sound1->stop();
		}
		Node *n = sounds->head;
		SoundObject *obj;
		while (n) {
			obj = (SoundObject *)n->data;
			n = n->next;
			if (obj->type == RES_WAVE) {
				obj->stop();
				//sfxPrintf("Trying to play movie while WAVE playing");
				//return;
			}

		}
	}
	if (!soundGone) {
		soundGone = true;
		sfxDisableSound();
	}
	sfxSystemCursor(SYS_ARROW);
	movieName = moviename;
	movieRect.top = theY;
	movieRect.left = theX;
	movieRect.bottom = y + height - 1;
	movieRect.right = x + width - 1;
	caller = whoToCue;

	if (!sfxIsMovieOpen()) {
		sfxOpenMovie(theName, theX, theY, 1);
		isOpen = true;
		intrfaceState = intrface->enabled;
		intrface->disable();
	}
	redrawBack = true;

	paused = false;
	sfxPlayMovie(true, true, &cueFun);
	flushMessageCue();
	isPlaying = true;

}

void Movie::play(const char *moviename, Object *whoToCue, int close) {
	SRect movieRect;
	char theName[64];
	sfxGetDir(theName, atoi(moviename), RES_MOVIE);
	x = 84;
	y = 112;
	closeWhenDone = close;
	if (sounds->size) {
		if (sound1->isPlaying()) {
			soundNum = sound1->number;
			if (sound1->flag)
				isBuffered = true;
			sound1->stop();
		}
		Node *n = sounds->head;
		SoundObject *obj;
		while (n) {
			obj = (SoundObject *)n->data;
			n = n->next;
			if (obj->type == RES_WAVE) {
				obj->stop();
				//	sfxPrintf("Trying to play movie while WAVE playing");
					//return;
			}

		}
	}
	if (!soundGone) {
		soundGone = true;
		sfxDisableSound();
	}
	sfxSystemCursor(SYS_ARROW);
	movieName = moviename;
	movieRect.top = y;
	movieRect.left = x;
	movieRect.bottom = y + 192 - 1;
	movieRect.right = x + 256 - 1;
	caller = whoToCue;

	if (!sfxIsMovieOpen()) {
		sfxOpenMovie(theName, x, y, 1);
		isOpen = true;
		intrfaceState = intrface->enabled;
		intrface->disable();
	}

	int erase = close;
	redrawBack = close;

	paused = false;
	sfxPlayMovie(close, erase, &cueFun);

	flushMessageCue();
	isPlaying = true;
	//flushMessageCue();
}

void Movie::play(const char *moviename, Object *whoToCue) {
	SRect movieRect;
	char theName[64];
	sfxGetDir(theName, atoi(moviename), RES_MOVIE);
	closeWhenDone = true;
	//DEBUG
	if (sounds->size) {
		if (sound1->isPlaying()) {
			soundNum = sound1->number;
			if (sound1->flag)
				isBuffered = true;
			sound1->stop();
		}
		Node *n = sounds->head;
		SoundObject *obj;
		while (n) {
			obj = (SoundObject *)n->data;
			n = n->next;
			if (obj->type == RES_WAVE) {
				obj->stop();
				//	sfxPrintf("Playing movie while WAVE playing");
			}

		}
	}
	//DEBUG
	sfxSystemCursor(SYS_ARROW);
	if (!soundGone) {
		soundGone = true;
		sfxDisableSound();
	}
	movieName = moviename;
	movieRect.top = MOVIE_Y;
	movieRect.left = MOVIE_X;
	movieRect.bottom = MOVIE_Y + MOVIE_HEIGHT - 1;
	movieRect.right = MOVIE_X + MOVIE_WIDTH - 1;
	caller = whoToCue;

	if (!sfxIsMovieOpen()) {
		sfxOpenMovie(theName, MOVIE_X, MOVIE_Y, 2);
		isOpen = true;
		intrfaceState = intrface->enabled;
		intrface->disable();
	}
	redrawBack = true;
	//isPlaying = true;
	paused = false;
	sfxPlayMovie(true, true, &cueFun);
	flushMessageCue();
	isPlaying = true;
	//flushMessageCue();
}


void Movie::play(const char *moviename, Object *whoToCue, int wait, int close, int erase) {
	SRect movieRect;
	char theName[64];
	sfxGetDir(theName, atoi(moviename), RES_MOVIE);
	closeWhenDone = close;
	if (sounds->size) {
		if (sound1->isPlaying()) {
			soundNum = sound1->number;
			if (sound1->flag)
				isBuffered = true;
			sound1->stop();
		}
		Node *n = sounds->head;
		SoundObject *obj;
		while (n) {
			obj = (SoundObject *)n->data;
			n = n->next;
			if (obj->type == RES_WAVE) {
				obj->stop();
				//	sfxPrintf("Trying to play movie while WAVE playing");
			}

		}
	}
	sfxSystemCursor(SYS_ARROW);
	if (!soundGone) {
		soundGone = true;
		sfxDisableSound();
	}

	movieName = moviename;
	movieRect.top = MOVIE_Y;
	movieRect.left = MOVIE_X;
	movieRect.bottom = MOVIE_Y + MOVIE_HEIGHT - 1;
	movieRect.right = MOVIE_X + MOVIE_WIDTH - 1;
	caller = whoToCue;

	if (!sfxIsMovieOpen()) {
		if ((close == 2) || (close == -1)) {
			sfxOpenMovie(theName, MOVIE_X, MOVIE_Y, 1);
		} else {
			sfxOpenMovie(theName, MOVIE_X, MOVIE_Y, 2);
		}
		isOpen = true;
		intrfaceState = intrface->enabled;
		intrface->disable();
	}
	//isPlaying = true;
	redrawBack = erase;
	paused = false;
	sfxPlayMovie(close, erase, &cueFun);
	flushMessageCue();
	isPlaying = true;
	//flushMessageCue();
}

void Movie::play(const char *moviename, int theX, int theY, int width, int height, Object *whoToCue, int wait, int close,
	int erase) {
	SRect movieRect;
	char theName[64];
	sfxGetDir(theName, atoi(moviename), RES_MOVIE);
	x = theX;
	y = theY;
	closeWhenDone = close;
	if (sounds->size) {
		if (sound1->isPlaying()) {
			soundNum = sound1->number;
			if (sound1->flag)
				isBuffered = true;
			sound1->stop();
		}
		Node *n = sounds->head;
		SoundObject *obj;
		while (n) {
			obj = (SoundObject *)n->data;
			n = n->next;
			if (obj->type == RES_WAVE) {
				obj->stop();
				//	sfxPrintf("Trying to play movie while WAVE playing");
					//return;
			}

		}
	}
	if (!soundGone) {
		soundGone = true;
		sfxDisableSound();
	}
	sfxSystemCursor(SYS_ARROW);
	movieName = moviename;
	movieRect.top = theY;
	movieRect.left = theX;
	movieRect.bottom = y + height - 1;
	movieRect.right = x + width - 1;
	caller = whoToCue;

	if (!sfxIsMovieOpen()) {
		if ((close == 2) || (close == -1)) {
			sfxOpenMovie(theName, theX, theY, 1);
		} else {
			sfxOpenMovie(theName, theX, theY, 2);
		}
		isOpen = true;
		intrfaceState = intrface->enabled;
		intrface->disable();
	}
	//isPlaying = true;
	redrawBack = erase;
	paused = false;
	sfxPlayMovie(close, erase, &cueFun);
	flushMessageCue();
	isPlaying = true;

}


void Movie::stop() {
	interrupted = true;
	isPlaying = false;
	if (sfxIsMovieOpen()) {
		sfxCloseMovie();
		isOpen = false;
	}
}

void Movie::cue() {
	SRect gameRect;
	SRect_Init(&gameRect, 0, 0, 511, 479);
	if (!GameFlag.test(fDCK10) && (soundGone) && (!sfxIsMovieOpen())) {
		if (sfxIsMovieOpen())
			sfxPrintf("Enabling sound when movie open");
		soundGone = false;
		sfxEnableSound();
	}
	if ((!sfxIsMovieOpen()) && closeWhenDone) {

		if (soundNum) {
			sound1->loop = -1;
			if (isBuffered)
				sound1->playBuffered(soundNum, NULL);
			else
				sound1->play(soundNum, NULL);
			soundNum = 0;
			isBuffered = false;
		}
		if (intrfaceState == true)
			intrface->enable();
		intrfaceState = false;
		if (redrawBack)
			sfxMadeCursor();
	}
	isPlaying = false;


	if (interrupted && redrawBack)
		UpdateScreenRect(backgrounds[curBack]->animPort, &gameRect);
	interrupted = false;
	redrawBack = false;
	if (caller) {
		caller->cue();
		//caller = NULL;
	}
}

// modes:
// MCI_SEEK_TO_END
// MCI_SEEK_TO_START
// MCI_TO

void Movie::seek(int offset, int mode) {
	ASSERT(0, 0);
}

void Movie::pause(bool repaint, bool cueMe) //if repaint is true then backround is drawn over movie
{
	if (isPlaying) {
		paused = true;
		sfxPauseMovie(repaint, cueMe);
	}
}

void Movie::pause(bool repaint) //if repaint is true then backround is drawn over movie
{
	if (isPlaying) {
		paused = true;
		sfxPauseMovie(repaint, false);
	}
}

void Movie::resume(void) {
	if (paused) {
		sfxResumeMovie();
		paused = false;
	}
}

int Movie::getPosn(void) {
	return (sfxGetMoviePosn());
}

bool Movie::fromTo(int from, int to) //after sfxPlayMCI is called, we go out of fromTo mode
{
	return (sfxSetMovieRange(from, to));
}

void Movie::step(int numSteps)// we must be paused for this to work
{
	ASSERT(0, 0);
}

void Movie::open(const char *filename) {
	char theName[64];

	if (sounds->size) {
		if (sound1->isPlaying()) {
			soundNum = sound1->number;
			if (sound1->flag)
				isBuffered = true;
			sound1->stop();
		}
		Node *n = sounds->head;
		SoundObject *obj;
		while (n) {
			obj = (SoundObject *)n->data;
			n = n->next;
			if (obj->type == RES_WAVE) {
				obj->stop();
				//sfxPrintf("Trying to play movie while WAVE playing");
				//return;
			}

		}
	}
	if (!soundGone) {
		soundGone = true;
		sfxDisableSound();
	}

	sfxGetDir(theName, atoi(filename), RES_MOVIE);
	filename = filename;
	sfxOpenMovie(theName, 0, 0, 1);
	isOpen = true;
}

} // namespace Spycraft
