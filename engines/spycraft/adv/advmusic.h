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

#ifndef SPYCRAFT_ADVMUSIC_H
#define SPYCRAFT_ADVMUSIC_H

#include "spycraft/adv/advlib.h"

namespace Spycraft {

/* MADE External */

#define NUM_MUSIC_VOICES	8

/* these could use some descriptions */
extern int sfxPlayMusic(int id, int volume, int volume_delay,
	int tempo, int tempo_delay,
	int loop_count, void (*CallBackFn)(int));
extern int sfxStopMusic(int midi_id);
extern int sfxPauseMusic(int midi_id);
extern int sfxResumeMusic(int midi_id);
extern int sfxSetMusicVolume(int midi_id, int new_volume, int delay);
extern int sfxSetMusicTempo(int midi_id, int new_tempo, int delay);
extern int sfxSetMusicLoops(int midi_id, int loop_count);
extern int sfxGetMusicVolume(int midi_id);
extern int sfxGetMusicTempo(int midi_id);
extern int sfxGetRemainingMusicLoops(int midi_id);
extern int sfxIsMusicPlaying(int midi_id);
extern int sfxEnableMusic();
extern int sfxDisableMusic();
extern void UpdateMusic();

/* MADE Internal */

extern int InitMusic();
extern int CleanMusic();

/* what about clean and update? */

extern void *OpenMidiFile(const char *filename, int *size);
extern void *OpenMidi(void *src);
extern void FreeMidi(void *data);

} // namespace Spycraft

#endif
