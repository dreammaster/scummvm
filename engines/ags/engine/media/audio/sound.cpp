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

#include "ags/shared/media/audio/sound.h"
#include "ags/shared/media/audio/clip_openal.h"

namespace AGS3 {

SOUNDCLIP *my_load_wave(const AssetPath &asset_name, int voll, int loop) {
	return my_load_openal(asset_name, "WAV", voll, loop);
}

SOUNDCLIP *my_load_mp3(const AssetPath &asset_name, int voll) {
	return my_load_openal(asset_name, "MP3", voll, false);
}

SOUNDCLIP *my_load_static_mp3(const AssetPath &asset_name, int voll, bool loop) {
	return my_load_openal(asset_name, "MP3", voll, loop);
}

SOUNDCLIP *my_load_static_ogg(const AssetPath &asset_name, int voll, bool loop) {
	return my_load_openal(asset_name, "OGG", voll, loop);
}

SOUNDCLIP *my_load_ogg(const AssetPath &asset_name, int voll) {
	return my_load_openal(asset_name, "OGG", voll, false);
}

SOUNDCLIP *my_load_midi(const AssetPath &asset_name, int repet) {
	return my_load_openal(asset_name, "MIDI", 0, repet);
}

SOUNDCLIP *my_load_mod(const AssetPath &asset_name, int repet) {
	return my_load_openal(asset_name, "MOD", 0, repet);
}

} // namespace AGS3
