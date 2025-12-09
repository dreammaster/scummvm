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

#include "spycraft/game/sound.h"
#include "spycraft/game/game.h"
#include "spycraft/dmade/advsound.h"
#include "spycraft/dmade/advres.h"
#include "spycraft/dmade/advmusic.h"

namespace Spycraft {

#ifndef ON
#define OFF	0
#define ON  1
#endif
#define COLBYSOUND	ON
#define COLBYMIDI	ON

extern int quitting;

SoundList *sounds;

int resourceIds[];

void SoundCallBack(int hd) {
	SoundObject *obj;
	Node *n = sounds->head;


	while (n) {
		obj = (SoundObject *)n->data;
		n = n->next;
		if ((obj->handler == hd) && (obj->type != RES_MIDI))
			obj->cue();

	}
}

void MusicCallBack(int hd) {
	if (theMusic->isPlaying())
		theMusic->cue();
	else
		sfxPrintf("Music cue for non-existent music");
}

SoundObject::SoundObject() {
	name = "SoundObject";
	number = -1;
	handler = -1;
	volume = 127;
	pan = 64;
	rate = CENTER_PITCH;
	loop = 1;
	caller = NULL;
	flag = false;
	canInterupt = false;
}

void SoundObject::cue() {
	Object *tmpCaller;
	number = -1;
	handler = -1;
	loop = 1;
	sounds->release(this);
	if (caller) {
		tmpCaller = caller;
		caller = NULL;
		tmpCaller->cue();
	}
}

int SoundObject::isPlaying(void) {
	if (handler != -1) //&& sfxIsMusicPlaying(handler))
		return true;
	else
		return false;
}

void SoundObject::dispose() {
	if (isPlaying())
		stop();
	deathList->add(this);
}

Sound::Sound() {
	name = "Sound";
	type = RES_WAVE;
	canInterupt = false;
}

Sound::~Sound() {
	if (isPlaying())
		stop();
	number = -1;
	handler = -1;
	caller = NULL;
}

void Sound::stop() {
	int i;
	if (isPlaying() && (!quitting)) {
		if (handler != -1) {
			if (!sfxIsSoundPlaying(handler)) {
				i = 1;
				i = 2;
				//	sfxPrintf("Invalid handle for resource: %d", number);
				for (i = 0; i < 32; i++)
					if (number == resourceIds[i])
						handler = i;
				if (i >= 32) {
					handler = -1;
					number = -1;
					//return false;
				}

			}
			if (handler != -1)
				sfxStopSound(handler);
			//return true;
			number = -1;
			handler = -1;


			cue();
		}
	}




	//UpdateSound();
//	cue();	//we are not getting a callback so we need to call the cue
//	}			
}

int Sound::play() {
#if COLBYSOUND == OFF
	return true;	// pretend to be playing
#endif
	if (number != -1) {
		if (isPlaying()) {		//error check if sound playing already
			//	sfxPrintf ("Sound object playing number: %d", number); //DEBUG
			return false;
		}
		flag = false;
		handler = sfxPlaySound(number, volume, pan, rate, SoundCallBack);
		if (handler != -1) {
			sounds->add(this);
			if (loop != 1)
				sfxSoundLoop(handler, loop);
		} else {
			sfxPrintf("Can't find sound %d", number); //DEBUG

			return false;
		}
	}
	return true;
}

int Sound::playBuffered(int theNumber, Object *whoToCue) {
#if COLBYSOUND == OFF
	if (whoToCue != NULL)
		whoToCue->cue();
	return true;		// pretend to be playing
#endif
	if (isPlaying()) {		//error check if sound playing already
		//sfxPrintf ("Sound object playing number: %d", number); //DEBUG
		return false;
	}
	number = theNumber;
	caller = whoToCue;
	flag = true;
	handler = sfxPlaySoundBuffer(number, volume, pan, rate, SoundCallBack);
	if (handler != -1) {
		sounds->add(this);
		if (loop != 1)
			sfxSoundLoop(handler, loop);
	} else {
		sfxPrintf("Can't find sound %d", number); //DEBUG
		number = -1;
		caller = NULL;
		return false;
	}
	return true;
}

int Sound::play(Object *whoToCue) {
#if COLBYSOUND == OFF
	if (whoToCue != NULL)
		whoToCue->cue();
	return true;		// pretend to be playing
#endif
	if (play()) {
		caller = whoToCue;
		return true;
	} else
		return false;
}

int Sound::play(int theNumber, Object *whoToCue) {
#if COLBYSOUND == OFF
	if (whoToCue != NULL)
		whoToCue->cue();
	return true;		// pretend to be playing
#endif
	number = theNumber;
	if (play()) {
		caller = whoToCue;
		return true;
	} else {
		number = -1;
		return false;
	}
}

void Sound::setVolume(uint16 theVolume) {
	//if ( isPlaying() )
		//sfxSoundVolume ( handler, theVolume );
}

Music::Music() {
	name = "Music";
	theMusic = this;
	type = RES_MIDI;
	volume = 50;
	number = -1;
}

Music::~Music() {
	if (isPlaying())
		stop();
	number = -1;
	handler = -1;
	caller = 0;
}

void Music::stop(void) {
	if (isPlaying()) {
		sfxStopMusic(handler);
		cue();	//we are not getting a callback so we need to call the cue
	}
	sounds->release(this);
}

int Music::play() {
#if COLBYMIDI == OFF
	return true;		// pretend to be playing
#endif
	if (number != -1) {
		if (isPlaying()) {
			//	sfxPrintf("This object is playing music: %d", number);
			return false;
		}
		handler = sfxPlayMusic(number, volume, 0, 100, 0, -1, MusicCallBack);
		if (handler != -1)
			sounds->add(this);
		else {
			sfxPrintf("Sound Play Error"); //DEBUG
			return false;
		}
	} else {
		sfxPrintf("No Sound Number");
		return false;
	}
	return true;
}

int Music::play(int theNumber) {
#if COLBYMIDI == OFF
	return true;		// pretend to be playing
#endif
	number = theNumber;
	if (!play()) {
		number = -1;
		return false;
	}
	return true;
}

void Music::setVolume(uint16 theVolume) {
	//if ( number != - 1 )	{
	//	volume = theVolume;
	//	sfxSetMusicVolume ( handler, theVolume, 0 );
	//}
}

SoundList::SoundList() {
	name = "sounds";
}

void SoundList::setVolume(uint16 theVolume) {
	/*Node *n = head;
	while ( n ) {
		((Sound *)n->data)->setVolume ( theVolume );
		n = n->next;
	}*/
}

} // namespace Spycraft
