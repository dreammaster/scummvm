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

#ifndef AGS2_AC_SOUND_H
#define AGS2_AC_SOUND_H

#include "common/serializer.h"

namespace AGS2 {

#define MUS_MIDI 1
#define MUS_MP3  2
#define MUS_WAVE 3
#define MUS_MOD  4
#define MUS_OGG  5

#define SCHAN_SPEECH  0
#define SCHAN_AMBIENT 1
#define SCHAN_MUSIC   2
#define SCHAN_NORMAL  3
#define AUDIOTYPE_LEGACY_AMBIENT_SOUND 1
#define AUDIOTYPE_LEGACY_MUSIC 2
#define AUDIOTYPE_LEGACY_SOUND 3

struct SOUNDCLIP {
	int done = 0;
	int priority = 50;
	int soundType = -1;
	int vol = 255;
	int volAsPercentage = 100;
	int originalVolAsPercentage = 100;
	int volModifier = 0;
	int paused = 0;
	int panning = 128;
	int panningAsPercentage = 0;
	int xSource = -1, ySource = -1;
	int maximumPossibleDistanceAway = 0;
	int directionalVolModifier = 0;
	bool repeat = false;
	void *sourceClip = nullptr;

	virtual ~SOUNDCLIP() {
	}

	virtual int poll() = 0;
	virtual void destroy() = 0;
	virtual void set_volume(int) = 0;
	virtual void restart() = 0;
	virtual void seek(int) = 0;
	virtual int get_pos() = 0;    // return 0 to indicate seek not supported
	virtual int get_pos_ms() = 0; // this must always return valid value if poss
	virtual int get_length_ms() = 0; // return total track length in ms (or 0)
	virtual int get_voice() = 0;  // return the allegro voice number (or -1 if none)
	virtual int get_sound_type() = 0;
	virtual int play() = 0;

	virtual int play_from(int position);
	virtual void set_panning(int newPanning);
	virtual void pause();
	virtual void resume();
};

SOUNDCLIP *my_load_wave(const char *filename, int voll, int loop);
SOUNDCLIP *my_load_mp3(const char *filname, int voll);
SOUNDCLIP *my_load_static_mp3(const char *filname, int voll, bool loop);
SOUNDCLIP *my_load_static_ogg(const char *filname, int voll, bool loop);
SOUNDCLIP *my_load_ogg(const char *filname, int voll);
SOUNDCLIP *my_load_midi(const char *filname, int repet);
SOUNDCLIP *my_load_mod(const char *filname, int repet);

int  init_mod_player(int numVoices);
void remove_mod_player();

extern void CheckViewFrame(int view, int loop, int frame);

} // namespace AGS2

#endif
