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
// SOUNDCLIP implementation using OpenAL.
// Supports following formats:
//   - MIDI (?)
//   - MOD (?)
//   - MP3
//   - OGG
//   - WAV
//
//=============================================================================
#ifndef AGS_ENGINE_MEDIA_AUDIO_CLIP_OPENAL_H
#define AGS_ENGINE_MEDIA_AUDIO_CLIP_OPENAL_H

#include "media/audio/soundclip.h"
#include "ac/asset_helper.h"

namespace AGS3 {

struct OPENAL_SOUNDCLIP final : public SOUNDCLIP {
public:
	OPENAL_SOUNDCLIP();
	~OPENAL_SOUNDCLIP();
	void destroy() override;

	virtual int get_sound_type() override {
		return soundType;
	}

	int play() override;
	virtual int play_from(int position) override;
	void pause() override;
	void resume() override;
	bool is_playing() const override;

	void seek(int pos) override;

	void poll() override;

	void set_volume(int newvol) override;
	void set_speed(int new_speed) override;
	void set_panning(int newPanning) override;

	int get_pos() override;
	int get_pos_ms() override;
	int get_length_ms() override;

	// TODO: make these private
	int slot_ = -1;
	int lengthMs = -1;
	int soundType = 0;

protected:
	void adjust_volume() override;

private:
	void configure_slot();
};


// Factory methods
SOUNDCLIP *my_load_openal(const AssetPath &asset_name, const char *extension_hint, int voll, bool loop);

} // namespace AGS3

#endif
