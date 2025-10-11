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

#ifndef SPYCRAFT_GAME_SOUND_H
#define SPYCRAFT_GAME_SOUND_H

#include "spycraft/game/object.h"
#include "spycraft/game/list.h"

namespace Spycraft {

#define SND_LOOP_INFINITE	0

class SoundObject : public Object {
public:
	int handler;
	long number;
	int loop;
	int pan;
	int rate;
	int type;
	int canInterupt;
	uint16 volume;
	Object *caller;

	SoundObject();
	void cue(void);
	void dispose(void);
	int isPlaying(void);
	virtual void stop(void) {
	};
	virtual void setVolume(uint16 theLevel) {
	};
};

class Sound : public SoundObject {
public:
	Sound();
	~Sound();
	int play(void);
	int play(Object *whoToCue);
	int play(int theNumber, Object *whoToCue);
	int playBuffered(int theNumber, Object *whoToCue);
	void stop(void);
	void setVolume(uint16 theLevel);
};

class Music : public SoundObject {
public:
	Music();
	~Music();

	int play(int theNumber);
	int play(void);
	void stop(void);
	void setVolume(uint16 theLevel);
};

class SoundList : public List {
public:
	SoundList();
	void setVolume(uint16 theLevel);
};

extern Music *theMusic;
extern SoundList *sounds;
extern Sound *sound1;
extern Sound *sound2;

} // namespace Spycraft

#endif
