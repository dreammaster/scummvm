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

#ifndef SPYCRAFT_DMADE_ADVSOUND_H
#define SPYCRAFT_DMADE_ADVSOUND_H

#include "spycraft/dmade/advlib.h"

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

/* MADE Internal */

/**
 * Initialize the SFX DIGI system
 */
inline int InitSound(int dwRate, int dwChannels, int dwBits, int dwFlags) {
	return 0;
}

/**
 * Uninitializes the SFX DIGI system
 */
inline int CleanSound() {
	//
	return 0;
}

/**
 * Application function to give the sound system foreground time.
 * Not needed in ScummVM, since it has it's own thread.
 */
inline void UpdateSound() {
}

/**
 * Enable the sound system
 */
inline int sfxEnableSound() {
	return 0;
}

/**
 * Disable the sound system
 */
inline int sfxDisableSound() {
	return 0;
}

/**
 * Returns sound state - Disabled or not
 */
inline int SoundIsDisabled() {
	return false;
}

/**
 * Set the volume of a sample
 * @param hSample		Handle of the sample
 * @param dwVolume		Volume to set the sound to (0 - 127)
 */
extern void sfxSoundVolume(int hSample, int dwVolume);

/**
 * Set the pan positiion of a sample
 * @param hSample		Handle of the sample
 * @param dwPanPosition	Position to set the pan to (0 - 127)
 */
extern void sfxSoundPan(int hSample, int dwPanPosition);

/**
 * Stop a sample from playing
 * @param hSample		Handle of the sample
 */
extern void sfxStopSound(int hSample);

/**
 * Adjust the rate of the sample
 * @param hSample		Handle of the sample
 * @param dwAdjust		Adjust rate (0 - 127)
 */
extern void sfxSoundRate(int hSample, int dwAdjust);

/**
 * Set the number of times to loop a sample
 * @param hSample		Handle of the sample
 * @param dwLoopCount	Number of times to loop (0 = infinite)
  */
extern void sfxSoundLoop(int hSample, int dwLoopCount);

/**
 * Calculate a new rate based on the rate adjust value
 * @param dwRate		Base rate
 * @param dwAdjust		Adjustment for rate (0 - 127)
 * @return Adjusted rate
*/
extern int sfxCalculateRate(int dwRate, int dwAdjust);

/**
 * Play a sound from the disk
 * @param dwID			Resource ID
 * @param dwVolume		Volume of sample (0 - 127)
 * @param dwRate		Rate to play at (0 - 127)
 * @param dwPan			Pan location
 * @param pfnCallback	Pointer to callback function
 * @return Handle of sample
*/
extern int sfxPlaySoundBuffer(int dwID, int dwVolume, int dwRate, int dwPan, void (*pfnCallback)(int));

/**
 * Play a sound from the resource file
 * @param dwID			Resource ID 
 * @param dwVolume		Volume of sample (0 - 127)
 * @param dwRate		Rate to play at (0 - 127)
 * @param dwPan			Pan Location (0 - 127)
 * @param pfnCallback	Pointer to callback function
 * @return Handle of sample
 */
extern int sfxPlaySound(int dwID, int dwVolume, int dwRate, int dwPan, void (*pfnCallback)(int));

/**
 * Test to see if a sample is playing
 * @param hSample		Handle of sample
 * @return True if it's playing
*/
extern bool sfxIsSoundPlaying(int hSample);

/**
 * Read a WAVE file from disk
 * @param filename		Filename
 * @param size			Set with the filesize
 * @return Pointer to allocated data for the WAVE file content
*/
extern void *OpenWaveFile(const char *filename, int *size);

/**
 * Return a WAVE from a passed buffer
 * @param src	Source buffer
 * @return Pointer to buffer
*/
inline void *OpenWave(void *src) {
	return src;
}

/**
 * Frees a WAVE file buffer
 * @param data		Buffer point
 */
inline void FreeWave(void *data) {
	free(data);
}

/**
 * Pauses all streams that are currently playing
 */
extern void sfxPauseSound();

/**
 * Resumes all streams that are currently paused
 */
extern void sfxResumeSound();

} // namespace Spycraft

#endif
