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

//include <cmath>
#include "ags/engine/media/audio/clip_openal.h"
#include "ags/engine/media/audio/audio_core.h"
#include "ags/engine/media/audio/audio_defines.h"
#include "ags/shared/util/path.h"
#include "ags/shared/util/stream.h"

namespace AGS3 {

OPENAL_SOUNDCLIP::OPENAL_SOUNDCLIP() : SOUNDCLIP(), slot_(-1) {
}
OPENAL_SOUNDCLIP::~OPENAL_SOUNDCLIP() {
	destroy();
}

void OPENAL_SOUNDCLIP::destroy() {
	if (slot_ < 0) {
		return;
	}
	audio_core_slot_stop(slot_);
	slot_ = -1;
}

int OPENAL_SOUNDCLIP::play() {
	return play_from(0);
}

int OPENAL_SOUNDCLIP::play_from(int position) {
	if (slot_ < 0) {
		return 0;
	}

	configure_slot(); // volume, speed, panning, repeat
	audio_core_slot_seek_ms(slot_, (float)position);
	audio_core_slot_play(slot_);
	return 1;
}

void OPENAL_SOUNDCLIP::pause() {
	if (slot_ < 0) {
		return;
	}
	audio_core_slot_pause(slot_);
}
void OPENAL_SOUNDCLIP::resume() {
	if (slot_ < 0) {
		return;
	}
	audio_core_slot_play(slot_);
}

bool OPENAL_SOUNDCLIP::is_playing() const {
	if (slot_ < 0) {
		return false;
	}
	auto status = audio_core_slot_get_play_state(slot_);
	switch (status) {
	case PlayStateInitial:
	case PlayStatePlaying:
	case PlayStatePaused:
		return true;
	}
	return false;
}

void OPENAL_SOUNDCLIP::seek(int pos_ms) {
	if (slot_ < 0) {
		return;
	}
	audio_core_slot_seek_ms(slot_, (float)pos_ms);
}

void OPENAL_SOUNDCLIP::poll() {
}


void OPENAL_SOUNDCLIP::configure_slot() {
	if (slot_ < 0) {
		return;
	}

	auto vol_f = static_cast<float>(get_final_volume()) / 255.0f;
	if (vol_f < 0.0f) {
		vol_f = 0.0f;
	}
	if (vol_f > 1.0f) {
		vol_f = 1.0f;
	}

	auto speed_f = static_cast<float>(speed) / 1000.0f;
	if (speed_f <= 0.0) {
		speed_f = 1.0f;
	}

	/* Sets the pan position, ranging from 0 (left) to 255 (right). 128 is considered centre */
	auto panning_f = (static_cast<float>(panning - 128) / 255.0f) * 2.0f;
	if (panning_f < -1.0f) {
		panning_f = -1.0f;
	}
	if (panning_f > 1.0f) {
		panning_f = 1.0f;
	}

	audio_core_slot_configure(slot_, vol_f, speed_f, panning_f);
}

void OPENAL_SOUNDCLIP::set_volume(int newvol) {
	vol = newvol;
	adjust_volume();
}

void OPENAL_SOUNDCLIP::set_speed(int new_speed) {
	speed = new_speed;
	configure_slot();
}

void OPENAL_SOUNDCLIP::set_panning(int newPanning) {
	panning = newPanning;
	configure_slot();
}

void OPENAL_SOUNDCLIP::adjust_volume() {
	configure_slot();
}

int OPENAL_SOUNDCLIP::get_pos() {
	// until we can figure out other details, pos will always be in milliseconds
	return get_pos_ms();
}

int OPENAL_SOUNDCLIP::get_pos_ms() {
	if (slot_ < 0) {
		return -1;
	}
	// given how unaccurate mp3 length is, maybe we should do differently here
	// on the other hand, if on repeat, maybe better to just return an infinitely increasing position?
	// but on the other other hand, then we can't always seek to that position.
	if (lengthMs <= 0.0f) {
		return (int)std::round(audio_core_slot_get_pos_ms(slot_));
	}
	return (int)std::round(fmod(audio_core_slot_get_pos_ms(slot_), lengthMs));
}

int OPENAL_SOUNDCLIP::get_length_ms() {
	return lengthMs;
}


// -------------------------------------------------------------------------------------------------
// FACTORY METHODS
// -------------------------------------------------------------------------------------------------

int GuessSoundTypeFromExt(const String &extension) {
	if (extension.CompareNoCase("mp3") == 0) {
		return MUS_MP3;
	} else if (extension.CompareNoCase("ogg") == 0) {
		return MUS_OGG;
	} else if (extension.CompareNoCase("mid") == 0) {
		return MUS_MIDI;
	} else if (extension.CompareNoCase("wav") == 0) {
		return MUS_WAVE;
	} else if (extension.CompareNoCase("voc") == 0) {
		return MUS_WAVE;
	} else if (extension.CompareNoCase("mod") == 0) {
		return MUS_MOD;
	} else if (extension.CompareNoCase("s3m") == 0) {
		return MUS_MOD;
	} else if (extension.CompareNoCase("it") == 0) {
		return MUS_MOD;
	} else if (extension.CompareNoCase("xm") == 0) {
		return MUS_MOD;
	}
	return 0;
}

SOUNDCLIP *my_load_openal(const AssetPath &asset_name, const char *extension_hint, int voll, bool loop) {
	size_t asset_size;
	auto *s = LocateAsset(asset_name, asset_size);
	if (!s)
		return nullptr;

	std::vector<char> data(asset_size);
	s->Read(data.data(), asset_size);
	delete s;

	const auto asset_ext = AGS::Shared::Path::GetFileExtension(asset_name.Name);
	const auto ext_hint = asset_ext.IsEmpty() ? String(extension_hint) : asset_ext;
	const auto sound_type = GuessSoundTypeFromExt(asset_ext);
	const auto lengthMs = (int)std::round(audio_core_get_sound_length_ms(data, asset_ext));

	auto slot = audio_core_slot_init(data, asset_ext, loop);
	if (slot < 0) {
		return nullptr;
	}

	auto clip = new OPENAL_SOUNDCLIP();
	clip->slot_ = slot;
	clip->vol = voll;
	clip->repeat = loop;
	clip->soundType = sound_type;
	clip->lengthMs = lengthMs;
	return clip;
}

} // namespace AGS3