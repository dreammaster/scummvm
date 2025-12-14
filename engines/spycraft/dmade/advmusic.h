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

#ifndef SPYCRAFT_DMADE_ADVMUSIC_H
#define SPYCRAFT_DMADE_ADVMUSIC_H

#include "spycraft/dmade/advlib.h"

namespace Spycraft {

/* MADE External */

#define NUM_MUSIC_VOICES	8


/**
 * Initialize the SFX MIDI system
 */
inline int InitMusic() {
	return true;
}

/**
 * De-Initialize the SFX MIDI system
 */
inline int CleanMusic() {
	return true;
}

/**
 * Start a song playing
 * @param id				Resource ID
 * @param volume			Volume to play song at
 * @param volume_delay		Period till volume is reached
 * @param tempo				Tempo to play song at
 * @param tempo_delay		Tempo change period
 * @param loop_count		Number of times to loop song
 * @param CallBackFn		Callback function pointer
 * @return Handle of song
 */
extern int sfxPlayMusic(int id, int volume, int volume_delay,
	int tempo, int tempo_delay,
	int loop_count, void (*CallBackFn)(int));

/**
 * Stop a song from playing
 * @param midi_id		Handle of the song
 */
extern int sfxStopMusic(int midi_id);

/**
 * Pause a song from playing
 * @param midi_id		Handle of the song
 */
extern int sfxPauseMusic(int midi_id);

/**
 * Resume a song playing
 * @param midi_id		Handle of the song
 */
extern int sfxResumeMusic(int midi_id);

/**
 * Set the music volume
 * @param midi_id		Handle of the song
 * @param new_volume	Volume to set song to
 * @param delay			Period to fade over
 * @return 
*/
extern int sfxSetMusicVolume(int midi_id, int new_volume, int delay);

/**
 * Alter the tempo of a song
 * @param midi_id		Handle of the song
 * @param new_tempo		Tempo to set song to
 * @param delay			Period to alter tempo over
 */
extern int sfxSetMusicTempo(int midi_id, int new_tempo, int delay);

/**
 * Set the loop count for a song
 * @param midi_id		Handle of the song
 * @param loop_count	Loop count
 */
extern int sfxSetMusicLoops(int midi_id, int loop_count);

/**
 * Return the music volume 
 * @param midi_id		Handle of the song
 */
extern int sfxGetMusicVolume(int midi_id);

/**
 * Get the tempo for a song
 * @param midi_id		Handle of the song
 * @return The tempo
 */
extern int sfxGetMusicTempo(int midi_id);

/**
 * Get the loop count for a song
 * @param midi_id		Handle of the song
 * @return Loop count
 */
extern int sfxGetRemainingMusicLoops(int midi_id);

/**
 * Check to see if a song is playing
 * @param midi_id		Handle of the song
 * @return Is playing
 */
extern bool sfxIsMusicPlaying(int midi_id);

/**
 * Enable the MIDI system 
 */
extern int sfxEnableMusic();

/**
 * Disable the MIDI system
 */
extern int sfxDisableMusic();

/**
 * Update the playing music.
 * Not needed in ScummVM, which has audio playback in a separate thread.
 */
inline void UpdateMusic() {
	// No implementation
}

/**
 * Reads the content of a MIDI file.
 * @param filename		Filename
 * @param size			Pointer to store the file size to
 * @return Pointer to buffer for the loaded data
 */
extern void *OpenMidiFile(const char *filename, int *size);

/**
 * Reads a MIDI from a source buffer
 * @param src		Source buffer
 * @return Pointer to buffer
 */
inline void *OpenMidi(void *src) {
	return src;
}

inline void FreeMidi(void *data) {
	free(data);
}

} // namespace Spycraft

#endif
