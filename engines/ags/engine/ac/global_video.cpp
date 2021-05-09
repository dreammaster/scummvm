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

#include "ags/engine/ac/game_setup.h"
#include "ags/engine/ac/game_state.h"
#include "ags/shared/ac/global_audio.h"
#include "ags/shared/ac/global_game.h"
#include "ags/shared/ac/global_video.h"
#include "ags/shared/ac/path_helper.h"
#include "ags/shared/core/assetmanager.h"
#include "ags/shared/debugging/debugger.h"
#include "ags/engine/debugging/debug_log.h"
#include "ags/engine/media/video/video.h"
#include "ags/engine/media/audio/audio_system.h"
#include "ags/shared/platform/base/agsplatformdriver.h"
#include "ags/shared/util/string_compat.h"

namespace AGS3 {

using namespace AGS::Shared;

void scrPlayVideo(const char *name, int skip, int flags) {
	EndSkippingUntilCharStops();

	if (play.fast_forward)
		return;
	if (debug_flags & DBG_NOVIDEO)
		return;

	if ((flags < 10) && (usetup.audio_backend == 0)) {
		// if game audio is disabled in Setup, then don't
		// play any sound on the video either
		flags += 10;
	}

	pause_sound_if_necessary_and_play_video(name, skip, flags);
}


#ifndef AGS_NO_VIDEO_PLAYER

void pause_sound_if_necessary_and_play_video(const char *name, int skip, int flags) {
	int musplaying = play.cur_music_number, i;
	int ambientWas[MAX_SOUND_CHANNELS];
	for (i = 1; i < MAX_SOUND_CHANNELS; i++)
		ambientWas[i] = ambient[i].channel;

	if ((strlen(name) > 3) && (ags_stricmp(&name[strlen(name) - 3], "ogv") == 0)) {
		play_theora_video(name, skip, flags);
	} else {
		debug_script_warn("PlayVideo: file '%s' is an unsupported format.", name);
		return;
	}

	if (flags < 10) {
		update_music_volume();
		// restart the music
		if (musplaying >= 0)
			newmusic(musplaying);
		for (i = 1; i < MAX_SOUND_CHANNELS; i++) {
			if (ambientWas[i] > 0)
				PlayAmbientSound(ambientWas[i], ambient[i].num, ambient[i].vol, ambient[i].x, ambient[i].y);
		}
	}
}

#else

void pause_sound_if_necessary_and_play_video(const char *name, int skip, int flags) {
}

#endif

} // namespace AGS3
