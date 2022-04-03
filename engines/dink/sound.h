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

#ifndef DINK_SOUND_H
#define DINK_SOUND_H

#include "common/stream.h"
#include "dink/lib/wintypes.h"

namespace Dink {

struct WaveHeader {
	byte        RIFF[4];          // "RIFF"
	uint32       dwSize;           // Size of data to follow
	byte        WAVE[4];          // "WAVE"
	byte        fmt_[4];          // "fmt "
	uint32       dw16;             // 16
	uint16        wOne_0;           // 1
	uint16        wChnls;           // Number of Channels
	uint32       dwSRate;          // Sample Rate
	uint32       BytesPerSec;      // Sample Rate
	uint16        wBlkAlign;        // 1
	uint16        BitsPerSample;    // Sample size
	byte        DATA[4];          // "DATA"
	uint32       dwDSize;          // Number of Samples

	void load(Common::SeekableReadStream *rs);
};


/**
 * Sets up the DirectSound object and loads all sounds into secondary
 * DirectSound buffers.  Returns false on error, or true if successful
 */
extern bool InitSound(HWND);

/**
 *Undoes everything that was done in a InitSound call
 */
extern bool DestroySound();

extern bool DSEnable(HWND);

/**
 * Turn off DirectSound
 */
extern bool DSDisable();

/**
 * Stops the sound effect specified. Returns true if succeeded.
 */
extern bool SoundStopEffect(int sound);

extern bool CreateBufferFromWaveFile(const char *filename, uint32 dwBuf);
extern void kill_repeat_sounds();
extern void kill_repeat_sounds_all();
extern bool SoundDestroyEffect(int sound);
extern int SoundPlayEffect(int sound, int min, int plus, int sound3d, bool repeat);
extern void SoundLoadBanks();
extern bool StopMidi();
extern void check_seq_status(int h);
extern bool PlayMidi(const char *sFileName);
extern void update_sound();

} // namespace Dink

#endif
