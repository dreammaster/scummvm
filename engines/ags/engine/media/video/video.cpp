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

#include "ags/engine/media/video/video.h"

#ifndef AGS_NO_VIDEO_PLAYER
//include <SDL.h>
#include "ags/shared/apeg.h"
#include "ags/shared/core/platform.h"
#include "ags/engine/debugging/debug_log.h"
#include "ags/shared/debugging/out.h"
#include "ags/shared/ac/asset_helper.h"
#include "ags/shared/ac/common.h"
#include "ags/engine/ac/draw.h"
#include "ags/shared/ac/game_version.h"
#include "ags/shared/ac/game_setup_struct.h"
#include "ags/engine/ac/game_state.h"
#include "ags/shared/ac/global_display.h"
#include "ags/shared/ac/keycode.h"
#include "ags/shared/ac/mouse.h"
#include "ags/shared/ac/sys_events.h"
#include "ags/engine/ac/runtime_defines.h"
#include "ags/shared/ac/system.h"
#include "ags/shared/core/asset_manager.h"
#include "ags/shared/gfx/bitmap.h"
#include "ags/shared/gfx/ddb.h"
#include "ags/shared/gfx/graphicsdriver.h"
#include "ags/shared/main/game_run.h"
#include "ags/shared/util/stream.h"
#include "ags/engine/media/audio/audio_system.h"

namespace AGS3 {

using namespace AGS::Shared;
using namespace AGS::Engine;




extern int _G(psp_video_framedrop);

enum VideoPlaybackType {
	kVideoNone,
	kVideoFlic,
	kVideoTheora
};

VideoPlaybackType video_type = kVideoNone;

// FLIC player start
Bitmap *fli_buffer = nullptr;
short fliwidth, fliheight;
int canabort = 0, stretch_flc = 1;
Bitmap *hicol_buf = nullptr;
IDriverDependantBitmap *fli_ddb = nullptr;
Bitmap *fli_target = nullptr;
int fliTargetWidth, fliTargetHeight;
volatile int fli_timer = 0; // TODO: use SDL thread conditions instead?
int check_if_user_input_should_cancel_video() {
	int key, mbut, mwheelz;
	if (run_service_key_controls(key)) {
		if ((key == eAGSKeyCodeEscape) && (canabort == 1))
			return 1;
		if (canabort >= 2)
			return 1;  // skip on any key
	}
	if (run_service_mb_controls(mbut, mwheelz) && mbut >= 0 && canabort == 3) {
		return 1; // skip on mouse click
	}
	return 0;
}

Uint32 fli_timer_callback(Uint32 interval, void *param) {
	fli_timer++;
	return interval;
}

int fli_callback() {
	Bitmap *usebuf = fli_buffer;

	update_audio_system_on_game_loop();

	if (_GP(game).color_depth > 1) {
		hicol_buf->Blit(fli_buffer, 0, 0, 0, 0, fliwidth, fliheight);
		usebuf = hicol_buf;
	}

	const Rect &view = _GP(play).GetMainViewport();
	if (stretch_flc == 0)
		fli_target->Blit(usebuf, 0, 0, view.GetWidth() / 2 - fliwidth / 2, view.GetHeight() / 2 - fliheight / 2, view.GetWidth(), view.GetHeight());
	else
		fli_target->StretchBlt(usebuf, RectWH(0, 0, fliwidth, fliheight), RectWH(0, 0, view.GetWidth(), view.GetHeight()));

	_G(gfxDriver)->UpdateDDBFromBitmap(fli_ddb, fli_target, false);
	_G(gfxDriver)->DrawSprite(0, 0, fli_ddb);
	render_to_screen();

	return check_if_user_input_should_cancel_video();
}

void play_flc_file(int numb, int playflags) {
	RGB oldpal[256];

	// AGS 2.x: If the screen is faded out, fade in again when playing a movie.
	if (_G(loaded_game_file_version) <= kGameVersion_272)
		_GP(play).screen_is_faded_out = 0;

	if (_GP(play).fast_forward)
		return;

	get_palette_range(oldpal, 0, 255);

	int clearScreenAtStart = 1;
	canabort = playflags % 10;
	playflags -= canabort;

	if (canabort == 2) // convert to PlayVideo-compatible setting
		canabort = 3;

	if (playflags % 100 == 0)
		stretch_flc = 1;
	else
		stretch_flc = 0;

	if (playflags / 100)
		clearScreenAtStart = 0;

	String flicname = String::FromFormat("flic%d.flc", numb);
	Stream *in = AssetMgr->OpenAsset(flicname);
	if (!in) {
		flicname.Format("flic%d.fli", numb);
		in = AssetMgr->OpenAsset(flicname);
	}
	if (!in) {
		debug_script_warn("FLIC animation flic%d.flc nor flic%d.fli not found", numb, numb);
		return;
	}

	in->Seek(8);
	fliwidth = in->ReadInt16();
	fliheight = in->ReadInt16();
	delete in;

	if (_GP(game).color_depth > 1) {
		hicol_buf = BitmapHelper::CreateBitmap(fliwidth, fliheight, _GP(game).GetColorDepth());
		hicol_buf->Clear();
	}
	// override the stretch option if necessary
	const Rect &view = _GP(play).GetMainViewport();
	if ((fliwidth == view.GetWidth()) && (fliheight == view.GetHeight()))
		stretch_flc = 0;
	else if ((fliwidth > view.GetWidth()) || (fliheight > view.GetHeight()))
		stretch_flc = 1;
	fli_buffer = BitmapHelper::CreateBitmap(fliwidth, fliheight, 8);
	if (fli_buffer == nullptr) quit("Not enough memory to play animation");
	fli_buffer->Clear();

	if (clearScreenAtStart) {
		if (_G(gfxDriver)->UsesMemoryBackBuffer()) {
			Bitmap *screen_bmp = _G(gfxDriver)->GetMemoryBackBuffer();
			screen_bmp->Clear();
		}
		render_to_screen();
	}

	video_type = kVideoFlic;
	fli_target = BitmapHelper::CreateBitmap(view.GetWidth(), view.GetHeight(), _GP(game).GetColorDepth());
	fli_ddb = _G(gfxDriver)->CreateDDBFromBitmap(fli_target, false, true);

	size_t asset_size;
	PACKFILE *pf = PackfileFromAsset(AssetPath("", flicname), asset_size);
	if (open_fli_pf(pf) == FLI_OK) {
		// TODO: refactor all this later!!!
		SDL_AddTimer(fli_speed, fli_timer_callback, nullptr);
		const int loop = 0; // TODO: add looping FLIC support to API?

		// actual FLI playback state, base on original Allegro 4's do_play_fli
		fli_timer = 1;
		int ret = next_fli_frame(loop);
		while (ret == FLI_OK) {
			/* update the palette */
			if (fli_pal_dirty_from <= fli_pal_dirty_to)
				set_palette_range(fli_palette, fli_pal_dirty_from, fli_pal_dirty_to, TRUE);

			/* update the screen */
			if (fli_bmp_dirty_from <= fli_bmp_dirty_to) {
				blit(fli_bitmap, fli_buffer->GetAllegroBitmap(), 0, fli_bmp_dirty_from, 0, fli_bmp_dirty_from,
					fli_bitmap->w, 1 + fli_bmp_dirty_to - fli_bmp_dirty_from);
			}

			reset_fli_variables();

			ret = fli_callback();
			if (ret != FLI_OK)
				break;

			ret = next_fli_frame(loop);
			fli_timer--;

			while (fli_timer <= 0) {
				/* wait a bit */
				SDL_Delay(1);
			}
		}
	} else {
		// This is not a fatal error that should prevent the game from continuing
		Debug::Printf("FLI/FLC animation play error");
	}
	pack_fclose(pf);


	video_type = kVideoNone;
	delete fli_buffer;
	fli_buffer = nullptr;
	// NOTE: the screen bitmap could change in the meanwhile, if the display mode has changed
	if (_G(gfxDriver)->UsesMemoryBackBuffer()) {
		Bitmap *screen_bmp = _G(gfxDriver)->GetMemoryBackBuffer();
		screen_bmp->Clear();
	}
	set_palette_range(oldpal, 0, 255, 0);
	render_to_screen();

	delete fli_target;
	_G(gfxDriver)->DestroyDDB(fli_ddb);
	fli_target = nullptr;
	fli_ddb = nullptr;


	delete hicol_buf;
	hicol_buf = nullptr;
	//  SetVirtualScreen(screen); wputblock(0,0,backbuffer,0);
	while (ags_mgetbutton() != MouseNone) {
	} // clear any queued mouse events.
	invalidate_screen();
}

// FLIC player end

// Theora player begin
// TODO: find a way to take Bitmap here?
Bitmap gl_TheoraBuffer;
int theora_playing_callback(BITMAP *theoraBuffer) {
	sys_evt_process_pending();
	if (theoraBuffer == nullptr) {
		// No video, only sound
		return check_if_user_input_should_cancel_video();
	}

	gl_TheoraBuffer.WrapAllegroBitmap(theoraBuffer, true);

	int drawAtX = 0, drawAtY = 0;
	const Rect &viewport = _GP(play).GetMainViewport();
	if (fli_ddb == nullptr) {
		fli_ddb = _G(gfxDriver)->CreateDDBFromBitmap(&gl_TheoraBuffer, false, true);
	}
	if (stretch_flc) {
		drawAtX = viewport.GetWidth() / 2 - fliTargetWidth / 2;
		drawAtY = viewport.GetHeight() / 2 - fliTargetHeight / 2;
		if (!_G(gfxDriver)->HasAcceleratedTransform()) {
			fli_target->StretchBlt(&gl_TheoraBuffer, RectWH(0, 0, gl_TheoraBuffer.GetWidth(), gl_TheoraBuffer.GetHeight()),
				RectWH(drawAtX, drawAtY, fliTargetWidth, fliTargetHeight));
			_G(gfxDriver)->UpdateDDBFromBitmap(fli_ddb, fli_target, false);
			drawAtX = 0;
			drawAtY = 0;
		} else {
			_G(gfxDriver)->UpdateDDBFromBitmap(fli_ddb, &gl_TheoraBuffer, false);
			fli_ddb->SetStretch(fliTargetWidth, fliTargetHeight, false);
		}
	} else {
		_G(gfxDriver)->UpdateDDBFromBitmap(fli_ddb, &gl_TheoraBuffer, false);
		drawAtX = viewport.GetWidth() / 2 - gl_TheoraBuffer.GetWidth() / 2;
		drawAtY = viewport.GetHeight() / 2 - gl_TheoraBuffer.GetHeight() / 2;
	}

	_G(gfxDriver)->DrawSprite(drawAtX, drawAtY, fli_ddb);
	update_audio_system_on_game_loop();
	render_to_screen();

	return check_if_user_input_should_cancel_video();
}

//
// Theora stream reader callbacks. We need these because APEG library does not
// provide means to supply user's PACKFILE directly.
//
// Open stream for reading (return suggested cache buffer size).
int apeg_stream_init(void *ptr) {
	return ptr != nullptr ? F_BUF_SIZE : 0;
}
// Read requested number of bytes into provided buffer,
// return actual number of bytes managed to read.
int apeg_stream_read(void *buffer, int bytes, void *ptr) {
	return ((Stream *)ptr)->Read(buffer, bytes);
}
// Skip requested number of bytes
void apeg_stream_skip(int bytes, void *ptr) {
	((Stream *)ptr)->Seek(bytes);
}
//

APEG_STREAM *get_theora_size(Stream *video_stream, int *width, int *height) {
	APEG_STREAM *oggVid = apeg_open_stream_ex(video_stream);
	if (oggVid != nullptr) {
		apeg_get_video_size(oggVid, width, height);
	} else {
		*width = 0;
		*height = 0;
	}
	return oggVid;
}

// TODO: use shared utility function for placing rect in rect
void calculate_destination_size_maintain_aspect_ratio(int vidWidth, int vidHeight, int *targetWidth, int *targetHeight) {
	const Rect &viewport = _GP(play).GetMainViewport();
	float aspectRatioVideo = (float)vidWidth / (float)vidHeight;
	float aspectRatioScreen = (float)viewport.GetWidth() / (float)viewport.GetHeight();

	if (aspectRatioVideo == aspectRatioScreen) {
		*targetWidth = viewport.GetWidth();
		*targetHeight = viewport.GetHeight();
	} else if (aspectRatioVideo > aspectRatioScreen) {
		*targetWidth = viewport.GetWidth();
		*targetHeight = (int)(((float)viewport.GetWidth() / aspectRatioVideo) + 0.5f);
	} else {
		*targetHeight = viewport.GetHeight();
		*targetWidth = (float)viewport.GetHeight() * aspectRatioVideo;
	}

}

void play_theora_video(const char *name, int skip, int flags) {
	std::unique_ptr<Stream> video_stream(AssetMgr->OpenAsset(name));
	apeg_set_stream_reader(apeg_stream_init, apeg_stream_read, apeg_stream_skip);
	apeg_set_display_depth(_GP(game).GetColorDepth());
	// we must disable length detection, otherwise it takes ages to start
	// playing if the file is large because it seeks through the whole thing
	apeg_disable_length_detection(TRUE);
	// Disable framedrop because it can lead to the PSP not playing the video at all.
	apeg_enable_framedrop(_G(psp_video_framedrop));
	update_polled_stuff_if_runtime();

	stretch_flc = (flags % 10);
	canabort = skip;
	apeg_ignore_audio((flags >= 10) ? 1 : 0);

	int videoWidth, videoHeight;
	APEG_STREAM *oggVid = get_theora_size(video_stream.get(), &videoWidth, &videoHeight);

	if (videoWidth == 0) {
		Display("Unable to load theora video '%s'", name);
		return;
	}

	if (flags < 10) {
		stop_all_sound_and_music();
	}

	calculate_destination_size_maintain_aspect_ratio(videoWidth, videoHeight, &fliTargetWidth, &fliTargetHeight);

	if ((fliTargetWidth == videoWidth) && (fliTargetHeight == videoHeight) && (stretch_flc)) {
		// don't need to stretch after all
		stretch_flc = 0;
	}

	if ((stretch_flc) && (!_G(gfxDriver)->HasAcceleratedTransform())) {
		fli_target = BitmapHelper::CreateBitmap(_GP(play).GetMainViewport().GetWidth(), _GP(play).GetMainViewport().GetHeight(), _GP(game).GetColorDepth());
		fli_target->Clear();
		fli_ddb = _G(gfxDriver)->CreateDDBFromBitmap(fli_target, false, true);
	} else {
		fli_ddb = nullptr;
	}

	update_polled_stuff_if_runtime();

	if (_G(gfxDriver)->UsesMemoryBackBuffer())
		_G(gfxDriver)->GetMemoryBackBuffer()->Clear();

	video_type = kVideoTheora;
	if (apeg_play_apeg_stream(oggVid, nullptr, 0, theora_playing_callback) == APEG_ERROR) {
		Display("Error playing theora video '%s'", name);
	}
	apeg_close_stream(oggVid);
	video_type = kVideoNone;

	//destroy_bitmap(fli_buffer);
	delete fli_target;
	_G(gfxDriver)->DestroyDDB(fli_ddb);
	fli_target = nullptr;
	fli_ddb = nullptr;
	invalidate_screen();
}
// Theora player end

void video_on_gfxmode_changed() {
	if (video_type == kVideoFlic) {
		// If the FLIC video is playing, restore its palette
		set_palette_range(fli_palette, 0, 255, 0);
	}
}

#else

void play_theora_video(const char *name, int skip, int flags) {
}
void play_flc_file(int numb, int playflags) {
}
void video_on_gfxmode_changed() {
}

#endif

} // namespace AGS3
