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


#ifdef DIRECTX
#include "spycraft/dmade/advmovie.h"
#include "spycraft/dmade/advsound.h"
#endif

#include "spycraft/game/knee.h"
#include "spycraft/game/image.h"
#include "spycraft/game/web.h"

namespace Spycraft {

Knee *knee = NULL;
extern int soundGone;
#ifdef DIRECTX
extern void EraseAltLine();
extern void AddScreen2PicEx(int dx, int dy, int w, int h);
#endif										  
KneeBack::KneeBack() {
	name = "backgrnd";
	view = 675;
	loop = 0;
	cel = 0;
	init(0, 15);
	noSkip();
	addToPic();
}

Knee::Knee() {
	name = "knee";
	currentFocus = PUZZLE_FOCUS;
	sfxSystemCursor(SYS_ARROW);
	user->input = false;
	webreport->hide();
	knee = this;
	new (KneeBack);
	playing = false;
	pos = 0;
	out = 380;
	report = new CapsSendBut;
	new KneePlayButt;
	new KneeStopButt;
	new KneeIAButt;

	init(WEB_KNEE);
	helppage = 92700;
	GameFlag.set(fDCK10);
	intrpuzz->setScript(new(KneeScript));
}
Knee::~Knee() {
}

void Knee::dispose() {
	theMovie->stop();
	knee = NULL;
	if (soundGone)
	{
#ifdef DIRECTX
		sfxEnableSound();
#endif
		soundGone = false;
	}
	sfxMadeCursor();
	if (web_intrface)
	{
		webreport->show();
		report->dispose();
	}
	GameFlag.clear(fDCK10);
	IntrPuzzPage::dispose();
}

void KneeScript::changeState(int newState) {
#ifdef DIRECTX
	char tempPath[256];
	memset(&tempPath, 0, 256);
#endif
	switchTo
		ticks = 2;
	END
		BEG
#ifdef DIRECTX
		sfxGetDir(tempPath, atoi("677"), RES_MOVIE);

	if (sounds->size) {
		if (sound1->isPlaying()) {
			sound1->stop();
		}
		Node *n = sounds->head;
		SoundObject *obj;
		while (n) {
			obj = (SoundObject *)n->data;
			n = n->next;
			if (obj->type == RES_WAVE) {
				obj->stop();
			}

		}
	}
	if (!soundGone) {
		soundGone = true;
		sfxDisableSound();
	}
	//	    SetMovieErase (false);
	sfxOpenMovie(tempPath, 18, 80, 2);
	EraseAltLine();
	sfxStepMovie(0);
	sfxCloseMovie();
	/*
				theMovie->caller = NULL;
				theMovie->stop();
	*/
	AddScreen2PicEx(18, 80, 368, 272);
#else
		theMovie->fromTo(0, 0);
	theMovie->play("677.avi", 18, 80, 368, 272, NULL, false, true, false);
#endif
	seconds = 2;
	END
		BEG
		flushMessageCue();
	user->input = true;
	intrpuzz->setScript(NULL);
	END
}
void KneeDrawScript::changeState(int newState) {
#ifdef DIRECTX
	char tempPath[256];
	memset(&tempPath, 0, 256);

	intrpuzz->setScript(NULL);
	return;
#endif
	switchTo
		ticks = 2;
	END
		BEG
#ifdef DIRECTX
		sfxGetDir(tempPath, atoi("677"), RES_MOVIE);
	//	    SetMovieErase (false);
	sfxOpenMovie(tempPath, 18, 80, 2);
	EraseAltLine();
	sfxStepMovie(0);
	sfxCloseMovie();
	/*
			theMovie->caller = NULL;
			theMovie->stop();
	*/
	AddScreen2PicEx(18, 80, 368, 272);
#else
		theMovie->fromTo(knee->pos, knee->pos);
	theMovie->play("677.avi", 18, 80, 368, 272, NULL, false, true, false);
#endif
	intrpuzz->setScript(NULL);
	END
}
/*********************
*
*		BUTTONS
*
**********************/
KneeIAButt::KneeIAButt() {
	name = "kneeiabutt";
	view = 676;
	loop = 0;
	cel = 0;
	init(397, 307);
	noSkip();
	new CommandLine(left, top, width, height, "Import data into Image analysis.");
}

void KneeIAButt::activate(void) {
	intrpuzz->dispose();
	new ImageAnalCaps;
}


KneePlayButt::KneePlayButt() {
	name = "kneeplaybutt";
	view = 676;
	loop = 1;
	cel = 0;
	init(387, 92);
	noSkip();
	new CommandLine(left, top, width, height, "Play footage.");
}

void KneePlayButt::activate(void) {
	if (!knee->playing)
	{
		if (knee->pos >= knee->out)
		{
			knee->pos = 0;
		}
		knee->playing = true;
		knee->report->setCel(2);
		theMovie->fromTo(knee->pos, knee->out);
		theMovie->play("677.avi", 18, 80, 368, 272, this, false, true, false);
	}
}
void KneePlayButt::cue(void) {
#ifdef DIRECTX
	AddScreen2PicEx(18, 80, 368, 272);
#endif
	knee->playing = false;
	knee->pos = knee->out;
	knee->report->setCel(0);
}

KneeStopButt::KneeStopButt() {
	name = "kneestopbutt";
	view = 676;
	loop = 2;
	cel = 0;
	init(387, 166);
	noSkip();
	new CommandLine(left, top, width, height, "Stop footage.");
}

void KneeStopButt::activate(void) {
	if (knee->playing)
	{
		theMovie->pause(false);
		knee->pos = theMovie->getPosn();
		theMovie->resume();
		theMovie->caller = NULL;
		theMovie->stop();
		knee->playing = false;
		knee->report->setCel(0);
#ifdef DIRECTX
		AddScreen2PicEx(18, 80, 368, 272);
#endif
	}
}

} // namespace Spycraft
