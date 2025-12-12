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

#ifndef AESOP_MOD_H
#define AESOP_MOD_H

namespace Aesop {

#ifdef __cplusplus
extern "C" {
#endif

// For each channel passed to the mixer.

typedef struct
{
	unsigned char *sound;      // Address of sound mod, Must be on paragraph.
	unsigned char *sndloc;     // Current output location.
	unsigned long sndlen;	   // Sound length.
	unsigned long silence;		// Amount of silence, between samples.
	unsigned long silout; 	 	// Number of silence samples, left over.
	unsigned long oneshot;		// Sample is played as a one shot.
	unsigned long trigger;		// Trigger is high for sample to be played.
	char *buff; 				   // Build buffer. (output is interleaved)
	unsigned long bufflen;		// Length of build buffer.
	unsigned long emshandle;	// EMS handle associated with mod.

} MODSPEC;

extern void cdecl BuildModStream(MODSPEC *mod);

// Build mixer data stream for this mod channel.
// Output buffer has 8 channels of interleaved data to be merged.

extern void cdecl ModSilence(MODSPEC *mod1,char *merge);
extern void cdecl MergeMod1 (MODSPEC *mod1,char *merge);
extern void cdecl MergeMod2 (MODSPEC *mod1,char *merge);
extern void cdecl MergeMod3 (MODSPEC *mod1,char *merge);
extern void cdecl MergeMod4 (MODSPEC *mod1,char *merge);

#ifdef __cplusplus
}
#endif

} // namespace Aesop

#endif
