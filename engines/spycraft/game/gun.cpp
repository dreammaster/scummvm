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

#include "spycraft/game/gun.h"

namespace Spycraft {

Gun *theGun = NULL;
Sound *gunSound;

extern Colby colby;

Clip::Clip() {
	name = "clip";
	view = 622;
	loop = 0;
	cel = 0;
	init(21, 26);
	setPri(100);
}
void Clip::dispose(void) {
	cast->release(this);
	deathList->add(this);
}

Mags::Mags() {
	name = "mags";
	view = 621;
	loop = 0;
	cel = 0;
	init(21, 56);
	setPri(100);
}
void Mags::dispose(void) {
	cast->release(this);
	deathList->add(this);
}

Gun::Gun() {
	name = "gun";
	view = vGUNOUT;
	loop = 5;
	cel = 0;
	shots = 16;
	clips = 2;
	mt = false;
	sfxLoadRes(95000, RES_WAVE);
	sfxLockRes(95000, RES_WAVE);
	sfxLoadRes(95002, RES_WAVE);
	sfxLockRes(95002, RES_WAVE);
	theGame->setCursor(gunCursor);
	sfxCursorPriority(99);
	init(254, 402);
	setPri(100);
	clip = new Clip;
	mags = new Mags;
	setScript(NULL);
}
void Gun::dispose(void) {
	clip->dispose();
	cast->release(this);
	deathList->add(this);
	theGun = NULL;
}
Gun::~Gun() {
	theGun = NULL;
	sfxUnlockRes(95000, RES_WAVE);
	sfxUnlockRes(95002, RES_WAVE);
}
int Gun::handleEvent(MADEEventStamp *event) {
	if (event_type == USER_LEFT_DOWN && user->input)
	{
		if (!script)
		{
			if (mouse_y < 400 && colby.gunning)
			{
				setScript(new GunScript);
			}
		}
	}
	return false;
}

void Gun::doit() {
	//	View::doit();
	//	return;
	  /*if (mouse_x < 70)	{
		if (theGun->loop != 1)	{
		  theGun->setLoop(1);
		}
	  }*/
	if (mouse_x < 26) {
		if (theGun->loop != 0) {
			theGun->setLoop(0);
		}
	} else if (mouse_x < 116) {
		if (theGun->loop != 1) {
			theGun->setLoop(1);
		}
	} else if (mouse_x < 206) {
		if (theGun->loop != 2) {
			theGun->setLoop(2);
		}
	} else if (mouse_x < 296) {
		if (theGun->loop != 3) {
			theGun->setLoop(3);
		}
	} else if (mouse_x < 386) {
		if (theGun->loop != 4) {
			theGun->setLoop(4);
		}
	} else if (mouse_x < 474) {
		if (theGun->loop != 5) {
			theGun->setLoop(5);
		}
	}
	/*else if (mouse_x < 630)	{
	  if (theGun->loop != 7)	{
		  theGun->setLoop(7);
	  }
	}*/
	else if (theGun->loop != 6) {
		theGun->setLoop(6);
	}
	View::doit();
}

void GunScript::changeState(int newState) {
	switchTo
		if (!theGun->mt)
		{
			theGun->setCel(1);
			flag = sfxPlaySound(95002, 100, 64, 64, NULL);
			theGun->shots--;
			if (theGun->shots)
				theGun->clip->setCel(theGun->shots - 1);
			else
				theGun->clip->hide();
		} else
		{
			flag = sfxPlaySound(95000, 100, 64, 64, NULL);
		}
		ticks = 10;
		END

			BEG
			if (!theGun->mt)
			{
				theGun->setCel(0);
				if (theGun->shots)
					((Gun *)client)->setScript(NULL);
				else
					((Gun *)client)->setScript(new ReloadScript);
			} else
				((Gun *)client)->setScript(NULL);
			END
}
void ReloadScript::changeState(int newState) {
	switchTo
		if (--theGun->clips >= 0)
		{
			user->input = false;
			sfxSystemCursor(SYS_WAIT);
			theGun->shots = 15;
			theGun->clip->hide();
			theGun->mags->setCel(theGun->clips);
			seconds = 2;
		} else
		{
			theGun->clip->hide();
			theGun->mt = true;
			((Gun *)client)->setScript(NULL);
		}
		END

			BEG
			sfxMadeCursor();
		theGun->clip->setCel(theGun->shots - 1);
		theGun->clip->show();
		((Gun *)client)->setScript(NULL);
		user->input = true;
		END
}

} // namespace Spycraft
