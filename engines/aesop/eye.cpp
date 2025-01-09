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

#include "aesop/eye.h"

namespace Aesop {

//========================================
//==                                    ==
//== AESOP/C code resource declarations ==
//==                                    ==
//========================================

code_resources {
	//
	// Miscellaneous functions
	//

	PROCDEF load_string,
	PROCDEF load_resource,
	PROCDEF copy_string,
	PROCDEF string_force_lower,
	PROCDEF string_force_upper,
	PROCDEF string_len,
	PROCDEF string_compare,
	PROCDEF strval,
	PROCDEF envval,
	PROCDEF beep,
	PROCDEF pokemem,
	PROCDEF peekmem,
	PROCDEF rnd,
	PROCDEF dice,
	PROCDEF absv,
	PROCDEF minv,
	PROCDEF maxv,
	PROCDEF diagnose,
	PROCDEF heapfree,

	//
	// Event functions
	// 

	PROCDEF notify,
	PROCDEF cancel,
	PROCDEF drain_event_queue,
	PROCDEF post_event,
	PROCDEF send_event,
	PROCDEF peek_event,
	PROCDEF dispatch_event,
	PROCDEF flush_event_queue,
	PROCDEF flush_input_events,

	//
	// Interface functions
	//

	PROCDEF init_interface,
	PROCDEF shutdown_interface,
	PROCDEF set_mouse_pointer,
	PROCDEF set_wait_pointer,
	PROCDEF standby_cursor,
	PROCDEF resume_cursor,
	PROCDEF show_mouse,
	PROCDEF hide_mouse,
	PROCDEF mouse_XY,
	PROCDEF mouse_in_window,
	PROCDEF lock_mouse,
	PROCDEF unlock_mouse,
	PROCDEF getkey,

	//
	// Graphics-related functions
	//

	PROCDEF init_graphics,
	PROCDEF draw_dot,
	PROCDEF draw_line,
	PROCDEF line_to,
	PROCDEF draw_rectangle,
	PROCDEF fill_rectangle,
	PROCDEF hash_rectangle,
	PROCDEF get_bitmap_height,
	PROCDEF draw_bitmap,
	PROCDEF visible_bitmap_rect,
	PROCDEF set_palette,
	PROCDEF refresh_window,
	PROCDEF wipe_window,
	PROCDEF shutdown_graphics,
	PROCDEF wait_vertical_retrace,
	PROCDEF read_palette,
	PROCDEF write_palette,
	PROCDEF pixel_fade,
	PROCDEF color_fade,
	PROCDEF light_fade,

	PROCDEF assign_window,
	PROCDEF assign_subwindow,
	PROCDEF release_window,
	PROCDEF get_x1,
	PROCDEF get_x2,
	PROCDEF get_y1,
	PROCDEF get_y2,
	PROCDEF set_x1,
	PROCDEF set_x2,
	PROCDEF set_y1,
	PROCDEF set_y2,

	PROCDEF text_window,
	PROCDEF text_style,
	PROCDEF text_xy,
	PROCDEF text_color,
	PROCDEF text_refresh_window,
	PROCDEF get_text_x,
	PROCDEF get_text_y,
	PROCDEF home,
	PROCDEF print,
	PROCDEF sprint,
	PROCDEF dprint,
	PROCDEF aprint,
	PROCDEF crout,
	PROCDEF char_width,
	PROCDEF font_height,

	PROCDEF solid_bar_graph,

	//   PROCDEF mono_on,
	//   PROCDEF mono_off,

	//
	// Sound-related functions
	//

	PROCDEF init_sound,
	PROCDEF shutdown_sound,
	PROCDEF load_sound_block,
	PROCDEF sound_effect,
	PROCDEF play_sequence,
	PROCDEF load_music,
	PROCDEF unload_music,
	PROCDEF set_sound_status,

	//
	// Eye III object management
	//

	PROCDEF create_object,
	PROCDEF create_program,
	PROCDEF destroy_object,
	PROCDEF flush_cache,
	PROCDEF thrash_cache,

	//
	// Eye III support functions
	//

	PROCDEF step_X,
	PROCDEF step_Y,
	PROCDEF step_FDIR,

	PROCDEF step_square_X,
	PROCDEF step_square_Y,
	PROCDEF step_region,

	PROCDEF distance,
	PROCDEF seek_direction,

	PROCDEF spell_request,
	PROCDEF spell_list,
	PROCDEF magic_field,
	PROCDEF do_dots,
	PROCDEF do_ice,

	PROCDEF read_save_directory,
	PROCDEF savegame_title,
	PROCDEF write_save_directory,

	PROCDEF save_game,
	PROCDEF suspend_game,
	PROCDEF resume_items,
	PROCDEF resume_level,
	PROCDEF change_level,
	PROCDEF restore_items,
	PROCDEF restore_level_objects,
	PROCDEF read_initial_items,
	PROCDEF write_initial_tempfiles,
	PROCDEF create_initial_binary_files,
	PROCDEF launch,

	//
	// Eye II savegame file access
	//

	PROCDEF open_transfer_file,
	PROCDEF close_transfer_file,
	PROCDEF player_attrib,
	PROCDEF item_attrib,
	PROCDEF arrow_count,
};

} // namespace Aesop
