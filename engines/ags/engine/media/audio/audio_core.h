/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

//=============================================================================
//
// Audio Core: an audio backend interface.
//
//=============================================================================

#ifndef AGS_ENGINE_MEDIA_AUDIO_AUDIO_CORE_H
#define AGS_ENGINE_MEDIA_AUDIO_AUDIO_CORE_H

#include "ags/lib/std/vector.h"
#include "ags/engine/media/audio/audio_core_defs.h"
#include "ags/shared/util/string.h"

namespace AGS3 {

// Initializes audio core system;
// starts polling on a background thread.
void audio_core_init(/*config, soundlib*/);
// Shut downs audio core system;
// stops any associated threads.
void audio_core_shutdown();

// Audio slot controls: slots are abstract holders for a playback.
//
// Initializes playback on a free playback slot (reuses spare one or allocates new if there's none).
// Data array must contain full wave data to _GP(play).
// TODO: this method requires having full sound in memory;
// should we add a streaming method later? is this of any priority for regular builds?
int  audio_core_slot_init(const std::vector<char> &data, const AGS::Shared::String &extension_hint, bool repeat);
// Start playback on a slot
void audio_core_slot_play(int slot_handle);
// Pause playback on a slot, resume with 'audio_core_slot_play'
void audio_core_slot_pause(int slot_handle);
// Stop playback on a slot, disposes sound data, frees a slot
void audio_core_slot_stop(int slot_handle);
// Seek on a slot, new position in milliseconds
void audio_core_slot_seek_ms(int slot_handle, float pos_ms);

// Audio core config
// Set new master volume, affects all slots
void audio_core_set_master_volume(float newvol);
// Sets up single playback parameters
void audio_core_slot_configure(int slot_handle, float volume, float speed, float panning);

AudioCorePlayState audio_core_slot_get_play_state(int slot_handle);
float audio_core_slot_get_pos_ms(int slot_handle);

// Utility methods
// Calculate sound clip length in milliseconds
float audio_core_get_sound_length_ms(const std::vector<char> &data, const char *extension_hint);

} // namespace AGS3

#endif
