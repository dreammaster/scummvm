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

#ifndef SPYCRAFT_ADVSOUND_H
#define SPYCRAFT_ADVSOUND_H

#include "spycraft/adv/advlib.h"

namespace Spycraft {

// defines for the InitSound function
#define  _SFX_USE_CLOSEST_SETTINGS  0x0001
#define  _SFX_USE_PASSED_SETTINGS   0x0002

// defines for flagging a sample as streaming or memory resident
#define  _SFX_SOUND_STREAMING       0x0001
#define  _SFX_SOUND_RESIDENT        0x0002

#define MIN_VOLUME						0
#define MAX_VOLUME						127
#define MAX_PITCH		   				0
#define CENTER_PITCH	   				64
#define MIN_PITCH		   				127

#define _SFX_8_BIT						0x0001
#define _SFX_16_BIT						0x0002
#define _SFX_MONO							0x0004
#define _SFX_STEREO						0x0008
#define _SFX_11025                  0x0010
#define _SFX_22050                  0x0020
#define _SFX_44100                  0x0040

/* MIN (LEFT) 0 <--> 127 MAX (RIGHT) */
extern void sfxSoundVolume(int, int);
extern void sfxSoundPan(int, int);

extern void sfxStopSound(int);
extern void sfxSoundRate(int, int);

/* 0 LOOPS INFINITELY */
extern void sfxSoundLoop(int, int);

extern int sfxCalculateRate(int, int);

/* BUFFER SOUND */
extern int sfxPlaySoundBuffer(int, int, int, int, void (*)(int));
extern int sfxPlaySound(int, int, int, int, void (*)(int));
extern int sfxIsSoundPlaying(int hSample);

/* MADE Internal */

extern int InitSound(int dwRate, int dwChannels, int dwBits, int dwFlags);
extern void UpdateSound();
extern int sfxEnableSound();
extern int sfxDisableSound();
extern int CleanSound();
extern int SoundIsDisabled();

extern void *OpenWaveFile(const char *filename, int *size);
extern void *OpenWave(void *src);
extern void FreeWave(void *data);

extern void sfxPauseSound();
extern void sfxResumeSound();

} // namespace Spycraft

#endif
