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

#include "ags2/ac/acsound.h"
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

} // namespace AGS2
