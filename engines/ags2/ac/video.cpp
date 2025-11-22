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

#include "ags2/ac/video.h"

namespace AGS2 {


void play_theora_video(const char *name, int skip, int flags) {
	apeg_set_display_depth(bitmap_color_depth(screen));
	// we must disable length detection, otherwise it takes ages to start
	// playing if the file is large because it seeks through the whole thing
	apeg_disable_length_detection(TRUE);
	apeg_enable_framedrop(TRUE);
	update_polled_stuff();

	stretch_flc = (flags % 10);
	canabort = skip;
	apeg_ignore_audio((flags >= 10) ? 1 : 0);

	int videoWidth, videoHeight;
	APEG_STREAM *oggVid = get_theora_size(name, &videoWidth, &videoHeight);

	if (videoWidth == 0)
	{
		Display("Unable to load theora video '%s'", name);
		return;
	}

	if (flags < 10)
	{
		stop_all_sound_and_music();
	}

	fli_target = NULL;
	//fli_buffer = create_bitmap_ex(final_col_dep, videoWidth, videoHeight);
	calculate_destination_size_maintain_aspect_ratio(videoWidth, videoHeight, &fliTargetWidth, &fliTargetHeight);

	if ((fliTargetWidth == videoWidth) && (fliTargetHeight == videoHeight) && (stretch_flc))
	{
		// don't need to stretch after all
		stretch_flc = 0;
	}

	if ((stretch_flc) && (!gfxDriver->HasAcceleratedStretchAndFlip()))
	{
		fli_target = create_bitmap_ex(final_col_dep, scrnwid, scrnhit);
		clear(fli_target);
		fli_ddb = gfxDriver->CreateDDBFromBitmap(fli_target, false, true);
	} else
	{
		fli_ddb = NULL;
	}

	update_polled_stuff();

	clear(virtual_screen);

	if (apeg_play_apeg_stream(oggVid, NULL, 0, theora_playing_callback) == APEG_ERROR)
	{
		Display("Error playing theora video '%s'", name);
	}
	apeg_close_stream(oggVid);

	//destroy_bitmap(fli_buffer);
	if (fli_target != NULL)
		destroy_bitmap(fli_target);
	gfxDriver->DestroyDDB(fli_ddb);
	fli_ddb = NULL;
	invalidate_screen();
}

int theora_playing_callback(BITMAP *theoraBuffer) {
	if (theoraBuffer == NULL)
	{
		// No video, only sound
		return check_if_user_input_should_cancel_video();
	}

	int drawAtX = 0, drawAtY = 0;
	if (fli_ddb == NULL)
	{
		fli_ddb = gfxDriver->CreateDDBFromBitmap(theoraBuffer, false, true);
	}
	if (stretch_flc)
	{
		drawAtX = scrnwid / 2 - fliTargetWidth / 2;
		drawAtY = scrnhit / 2 - fliTargetHeight / 2;
		if (!gfxDriver->HasAcceleratedStretchAndFlip())
		{
			stretch_blit(theoraBuffer, fli_target, 0, 0, theoraBuffer->w, theoraBuffer->h,
				drawAtX, drawAtY, fliTargetWidth, fliTargetHeight);
			gfxDriver->UpdateDDBFromBitmap(fli_ddb, fli_target, false);
			drawAtX = 0;
			drawAtY = 0;
		} else
		{
			gfxDriver->UpdateDDBFromBitmap(fli_ddb, theoraBuffer, false);
			fli_ddb->SetStretch(fliTargetWidth, fliTargetHeight);
		}
	} else
	{
		gfxDriver->UpdateDDBFromBitmap(fli_ddb, theoraBuffer, false);
		drawAtX = scrnwid / 2 - theoraBuffer->w / 2;
		drawAtY = scrnhit / 2 - theoraBuffer->h / 2;
	}

	gfxDriver->DrawSprite(drawAtX, drawAtY, fli_ddb);
	render_to_screen(virtual_screen, 0, 0);
	update_polled_stuff_and_crossfade();

	return check_if_user_input_should_cancel_video();
}

APEG_STREAM *get_theora_size(const char *fileName, int *width, int *height) {
	APEG_STREAM *oggVid = apeg_open_stream(fileName);
	if (oggVid != NULL)
	{
		apeg_get_video_size(oggVid, width, height);
	} else
	{
		*width = 0;
		*height = 0;
	}
	return oggVid;
}

} // namespace AGS2

#endif
