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

#include "audio/mixer.h"
#include "common/file.h"
#include "common/textconsole.h"
#include "spycraft/dmade/advsound.h"
#include "spycraft/spycraft.h"

namespace Spycraft {

void sfxSoundVolume(int hSample, int dwVolume) {
	warning("TODO: sfxSoundVolume");
}

void sfxSoundPan(int hSample, int dwPanPosition) {
	warning("TODO: sfxSoundPan");
}

void sfxStopSound(int hSample) {
	// TODO: play sound
}

void sfxSoundRate(int hSample, int dwAdjust) {
	warning("TODO: sfxSoundRate");
}

void sfxSoundLoop(int hSample, int dwLoopCount) {
	warning("TODO: sfxSoundLoop");
}

int sfxCalculateRate(int dwRate, int dwAdjust) {
	return dwRate;
}

int sfxPlaySoundBuffer(int dwID, int dwVolume, int dwRate, int dwPanLocation,
		void (*pfnCallback)(int)) {
	warning("TODO: sfxPlaySoundBuffer");
	return 0;
}

int sfxPlaySound(int dwID, int dwVolume, int dwRate, int dwPanLocation,
		void (*pfnCallback)(int)) {
	warning("TODO: sfxPlaySound");
	return 0;
}

bool sfxIsSoundPlaying(int hSample) {
	return false;
}

void *OpenWaveFile(const char *filename, int *size) {
	Common::File f;

	if (f.open(filename)) {
		size_t filesize = f.size();

		if (size)
			*size = filesize;

		byte *buf = (byte *)malloc(filesize);
		f.read(buf, filesize);
		return buf;
	}

	return nullptr;
}

void sfxPauseSound() {
	g_engine->_mixer->pauseAll(true);
}

void sfxResumeSound() {
	g_engine->_mixer->pauseAll(false);
}

} // namespace Spycraft
