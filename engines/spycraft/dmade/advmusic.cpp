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
#include "spycraft/dmade/advmusic.h"
#include "spycraft/dmade/advsound.h"

namespace Spycraft {

int sfxPlayMusic(int id, int volume, int volume_delay, int tempo, int tempo_delay,
		int loop_count, void (*CallBackFn)(int)) {
	error("TODO: sfxPlayMusic");
}

int sfxStopMusic(int midi_id) {
	warning("TODO: sfxStopMusic");
	return 0;
}

int sfxPauseMusic(int midi_id) {
	warning("TODO: sfxPauseMusic");
	return 0;
}

int sfxResumeMusic(int midi_id) {
	warning("TODO: sfxResumeMusic");
	return 0;
}

int sfxSetMusicVolume(int midi_id, int new_volume, int delay) {
	warning("TODO: sfxSetMusicVolume");
	return 0;
}

int sfxSetMusicTempo(int midi_id, int new_tempo, int delay) {
	warning("TODO: sfxSetMusicTempo");
	return 0;
}

int sfxSetMusicLoops(int midi_id, int loop_count) {
	warning("TODO: sfxSetMusicLoops");
	return 0;
}

int sfxGetMusicVolume(int midi_id) {
	warning("TODO: sfxGetMusicVolume");
	return 0;
}

int sfxGetMusicTempo(int midi_id) {
	warning("TODO: sfxGetMusicTempo");
	return 0;
}

int sfxGetRemainingMusicLoops(int midi_id) {
	warning("TODO: sfxGetRemainingMusicLoops");
	return 0;
}

bool sfxIsMusicPlaying(int midi_id) {
	warning("TODO: sfxIsMusicPlaying");
	return false;
}

int sfxEnableMusic() {
	warning("TODO: sfxEnableMusic");
	return 0;
}

int sfxDisableMusic() {
	warning("TODO: sfxDisableMusic");
	return 0;
}

void *OpenMidiFile(const char *filename, int *size) {
	return OpenWaveFile(filename, size);
}

void StopAllMusic() {
	warning("TODO: StopAllMusic");
}

void RestartAllMusic() {
	warning("TODO: RestartAllMusic");
}

} // namespace Spycraft
