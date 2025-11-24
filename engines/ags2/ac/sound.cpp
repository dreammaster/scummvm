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

#include "common/textconsole.h"
#include "ags2/ac/sound.h"
#include "ags2/lib/allegro/digi.h"

namespace AGS2 {


int SOUNDCLIP::play_from(int position) {
	int retVal = play();
	if ((retVal != 0) && (position > 0))
	{
		seek(position);
	}
	return retVal;
}

void SOUNDCLIP::set_panning(int newPanning) {
	int voice = get_voice();
	if (voice >= 0) {
		voice_set_pan(voice, newPanning);
		panning = newPanning;
	}
}

void SOUNDCLIP::pause() {
	int voice = get_voice();
	if (voice >= 0) {
		voice_stop(voice);
		paused = 1;
	}
}

void SOUNDCLIP::resume() {
	int voice = get_voice();
	if (voice >= 0)
		voice_start(voice);
	paused = 0;
}

/*------------------------------------------------------------------*/

SOUNDCLIP *my_load_wave(const char *filename, int voll, int loop) {
	error("TODO: my_load_wave");
}
SOUNDCLIP *my_load_mp3(const char *filname, int voll) {
	error("TODO: my_load_mp3");
}
SOUNDCLIP *my_load_static_mp3(const char *filname, int voll, bool loop) {
	error("TODO: my_load_static_mp3");
}
SOUNDCLIP *my_load_static_ogg(const char *filname, int voll, bool loop) {
	error("TODO: my_load_static_ogg");
}
SOUNDCLIP *my_load_ogg(const char *filname, int voll) {
	error("TODO: my_load_ogg");
}
SOUNDCLIP *my_load_midi(const char *filname, int repet) {
	error("TODO: my_load_midi");
}
SOUNDCLIP *my_load_mod(const char *filname, int repet) {
	error("TODO: my_load_mod");
}

int  init_mod_player(int numVoices) {
	warning("TODO: init_mod_player");
	return 0;
}
void remove_mod_player() {
	warning("TODO: remove_mod_player");
}

} // namespace AGS2
