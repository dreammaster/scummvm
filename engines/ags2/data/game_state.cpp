
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

#include "ags2/data/game_state.h"

namespace AGS2 {

void GameState::synchronize(Common::Serializer &s) {
	int i;

	s.syncAsSint32LE(score);
	s.syncAsSint32LE(usedmode);
	s.syncAsSint32LE(disabled_user_interface);
	s.syncAsSint32LE(gscript_timer);
	s.syncAsSint32LE(debug_mode);
	s.syncAsSint32LE(globalvars[MAXGLOBALVARS]);
	s.syncAsSint32LE(messagetime);
	s.syncAsSint32LE(usedinv);
	s.syncAsSint32LE(inv_top);
	s.syncAsSint32LE(inv_numdisp);
	s.syncAsSint32LE(obsolete_inv_numorder);
	s.syncAsSint32LE(inv_numinline);
	s.syncAsSint32LE(text_speed);
	s.syncAsSint32LE(sierra_inv_color);
	s.syncAsSint32LE(talkanim_speed);
	s.syncAsSint32LE(inv_item_wid);
	s.syncAsSint32LE(inv_item_hit);
	s.syncAsSint32LE(speech_text_shadow);
	s.syncAsSint32LE(swap_portrait_side);
	s.syncAsSint32LE(speech_textwindow_gui);
	s.syncAsSint32LE(follow_change_room_timer);
	s.syncAsSint32LE(totalscore);
	s.syncAsSint32LE(skip_display);
	s.syncAsSint32LE(no_multiloop_repeat);
	s.syncAsSint32LE(roomscript_finished);
	s.syncAsSint32LE(used_inv_on);
	s.syncAsSint32LE(no_textbg_when_voice);
	s.syncAsSint32LE(max_dialogoption_width);
	s.syncAsSint32LE(no_hicolor_fadein);
	s.syncAsSint32LE(bgspeech_game_speed);
	s.syncAsSint32LE(bgspeech_stay_on_display);
	s.syncAsSint32LE(unfactor_speech_from_textlength);
	s.syncAsSint32LE(mp3_loop_before_end);
	s.syncAsSint32LE(speech_music_drop);
	s.syncAsSint32LE(in_cutscene);
	s.syncAsSint32LE(fast_forward);
	s.syncAsSint32LE(room_width);
	s.syncAsSint32LE(room_height);
	s.syncAsSint32LE(game_speed_modifier);
	s.syncAsSint32LE(score_sound);
	s.syncAsSint32LE(takeover_data);
	s.syncAsSint32LE(replay_hotkey);
	s.syncAsSint32LE(dialog_options_x);
	s.syncAsSint32LE(dialog_options_y);
	s.syncAsSint32LE(narrator_speech);
	s.syncAsSint32LE(ambient_sounds_persist);
	s.syncAsSint32LE(lipsync_speed);
	s.syncAsSint32LE(close_mouth_speech_time);
	s.syncAsSint32LE(disable_antialiasing);
	s.syncAsSint32LE(text_speed_modifier);
	s.syncAsSint32LE(text_align);
	s.syncAsSint32LE(speech_bubble_width);
	s.syncAsSint32LE(min_dialogoption_width);
	s.syncAsSint32LE(disable_dialog_parser);
	s.syncAsSint32LE(anim_background_speed);
	s.syncAsSint32LE(top_bar_backcolor);
	s.syncAsSint32LE(top_bar_textcolor);
	s.syncAsSint32LE(top_bar_bordercolor);
	s.syncAsSint32LE(top_bar_borderwidth);
	s.syncAsSint32LE(top_bar_ypos);
	s.syncAsSint32LE(screenshot_width);
	s.syncAsSint32LE(screenshot_height);
	s.syncAsSint32LE(top_bar_font);
	s.syncAsSint32LE(speech_text_align);
	s.syncAsSint32LE(auto_use_walkto_points);
	s.syncAsSint32LE(inventory_greys_out);
	s.syncAsSint32LE(skip_speech_specific_key);
	s.syncAsSint32LE(abort_key);
	s.syncAsSint32LE(fade_to_red);
	s.syncAsSint32LE(fade_to_green);
	s.syncAsSint32LE(fade_to_blue);
	s.syncAsSint32LE(show_single_dialog_option);
	s.syncAsSint32LE(keep_screen_during_instant_transition);
	s.syncAsSint32LE(read_dialog_option_colour);
	s.syncAsSint32LE(stop_dialog_at_end);
	s.syncAsSint32LE(reserved[10]);
	s.syncAsSint32LE(recording);
	s.syncAsSint32LE(playback);
	s.syncAsSint16LE(gamestep);
	s.syncAsSint32LE(randseed);
	s.syncAsSint32LE(player_on_region);
	s.syncAsSint32LE(screen_is_faded_out);
	s.syncAsSint32LE(check_interaction_only);
	s.syncAsSint32LE(bg_frame);
	s.syncAsSint32LE(bg_anim_delay);
	s.syncAsSint32LE(music_vol_was);
	s.syncAsSint16LE(wait_counter);
	s.syncAsSint16LE(mboundx1);
	s.syncAsSint16LE(mboundx2);
	s.syncAsSint16LE(mboundy1);
	s.syncAsSint16LE(mboundy2);
	s.syncAsSint32LE(fade_effect);
	s.syncAsSint32LE(bg_frame_locked);
	for (i = 0; i < MAXGSVALUES; ++i)
		s.syncAsSint32LE(globalscriptvars[i]);
	s.syncAsSint32LE(cur_music_number, music_repeat);
	s.syncAsSint32LE(music_master_volume);
	s.syncAsSint32LE(digital_master_volume);
	s.syncBytes((byte *)walkable_areas_on, MAX_WALK_AREAS + 1);
	s.syncAsSint16LE(screen_flipped);
	s.syncAsSint16LE(offsets_locked);
	s.syncAsSint32LE(entered_at_x);
	s.syncAsSint32LE(entered_at_y);
	s.syncAsSint32LE(entered_edge);
	s.syncAsSint32LE(want_speech);
	s.syncAsSint32LE(want_music);
	s.syncAsSint32LE(cant_skip_speech);
	for (i = 0; i < MAX_TIMERS; ++i)
		s.syncAsSint32LE(script_timers[i]);
	s.syncAsSint32LE(sound_volume);
	s.syncAsSint32LE(speech_volume);
	s.syncAsSint32LE(normal_font);
	s.syncAsSint32LE(speech_font);
	s.syncAsByte(key_skip_wait);
	s.syncAsSint32LE(swap_portrait_lastchar);
	s.syncAsSint32LE(seperate_music_lib);
	s.syncAsSint32LE(in_conversation);
	s.syncAsSint32LE(screen_tint);
	s.syncAsSint32LE(num_parsed_words);
	for (i = 0; i < MAX_PARSED_WORDS; ++i)
		s.syncAsSint16LE(parsed_words[i]);
	s.syncBytes((byte *)bad_parsed_word, 100);
	s.syncAsSint32LE(raw_color);
	s.syncAsSint32LE(raw_modified[MAX_BSCENE]);
	s.syncAsSint16LE(filenumbers[MAXSAVEGAMES]);
	s.syncAsSint32LE(room_changes);
	s.syncAsSint32LE(mouse_cursor_hidden);
	s.syncAsSint32LE(silent_midi);
	s.syncAsSint32LE(silent_midi_channel);
	s.syncAsSint32LE(current_music_repeating);
	s.syncAsUint32LE(shakesc_delay);
	s.syncAsSint32LE(shakesc_amount);
	s.syncAsSint32LE(shakesc_length);
	s.syncAsSint32LE(rtint_red);
	s.syncAsSint32LE(rtint_green);
	s.syncAsSint32LE(rtint_blue);
	s.syncAsSint32LE(rtint_level);
	s.syncAsSint32LE(rtint_light);
	s.syncAsSint32LE(end_cutscene_music);
	s.syncAsSint32LE(skip_until_char_stops);
	s.syncAsSint32LE(get_loc_name_last_time);
	s.syncAsSint32LE(get_loc_name_save_cursor);
	s.syncAsSint32LE(restore_cursor_mode_to);
	s.syncAsSint32LE(restore_cursor_image_to);
	s.syncAsSint16LE(music_queue_size);
	for (i = 0; i < MAX_QUEUED_MUSIC; ++i)
		s.syncAsSint16LE(music_queue[i]);
	s.syncAsSint16LE(new_music_queue_size);
	s.syncAsSint16LE(crossfading_out_channel);
	s.syncAsSint16LE(crossfade_step);
	s.syncAsSint16LE(crossfade_out_volume_per_step);
	s.syncAsSint16LE(crossfade_initial_volume_out);
	s.syncAsSint16LE(crossfading_in_channel);
	s.syncAsSint16LE(crossfade_in_volume_per_step);
	s.syncAsSint16LE(crossfade_final_volume_in);

	for (i = 0; i < MAX_QUEUED_MUSIC; ++i)
		new_music_queue[i].synchronize(s);
	s.syncBytes((byte *)takeover_from, 50);
	s.syncBytes((byte *)playmp3file_name, PLAYMP3FILE_MAX_FILENAME_LEN);
	s.syncBytes((byte *)&globalstrings[0][0], MAXGLOBALSTRINGS * MAX_MAXSTRLEN);
	s.syncBytes((byte *)lastParserEntry, MAX_MAXSTRLEN);
	s.syncBytes((byte *)game_name, 100);
	s.syncAsSint32LE(ground_level_areas_disabled);
	s.syncAsSint32LE(next_screen_transition);
	s.syncAsSint32LE(gamma_adjustment);
	s.syncAsSint16LE(temporarily_turned_off_character);
	s.syncAsSint16LE(inv_backwards_compatibility);

	s.skip(4);	// gui_draw_order
	s.skip(4);	// do_once_tokens
	s.syncAsSint32LE(num_do_once_tokens);
	s.syncAsSint32LE(text_min_display_time_ms);
	s.syncAsSint32LE(ignore_user_input_after_text_timeout_ms);
	s.syncAsUint32LE(ignore_user_input_until_time);
	for (i = 0; i < MAX_AUDIO_TYPES; ++i)
		s.syncAsSint32LE(default_audio_type_volumes[i]);
}

} // namespace AGS2
