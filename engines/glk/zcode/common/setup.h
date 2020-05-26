/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software{} you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation{} either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY{} without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program{} if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

/*
 * Various status thingies for the interpreter and interface.
 *
 */

#ifndef GLK_ZCODE_COMMON_SETUP_H
#define GLK_ZCODE_COMMON_SETUP_H

#include "glk/zcode/common/defs.h"

namespace Glk {
namespace ZCode {

typedef enum {
	FORMAT_NORMAL,
	FORMAT_UNKNOWN,
	FORMAT_DISABLED,
	FORMAT_IRC,
	FORMAT_ANSI
} format_t;

typedef struct frotz_setup_struct {
	int attribute_assignment;
	int attribute_testing;
	int context_lines;
	int object_locating;
	int object_movement;
	int left_margin;
	int right_margin;
	int ignore_errors;
	int interpreter_number;
	int piracy;
	int undo_slots;
	int expand_abbreviations;
	int script_cols;
	int sound;
	int bleep;
	int err_report_mode;

	char *story_file;
	const char *blorb_file;
	char *story_name;
	char *story_base;
	char *script_name;
	char *command_name;
	char *save_name;
	char *tmp_save_name;
	char *aux_name;
	char *story_path;
	char *zcode_path;
	char *restricted_path;

	format_t format;  /* type of format codes for dumb interface */
	bool restore_mode; /* for a save file passed from command line */
	bool use_blorb;
	bool exec_in_blorb;
} f_setup_t;
extern f_setup_t f_setup;

/*** Story file header data ***/
typedef struct zcode_header_struct {
	zbyte version;
	zbyte config;
	zword release;
	zword resident_size;
	zword start_pc;
	zword dictionary;
	zword objects;
	zword globals;
	zword dynamic_size;
	zword flags;
	zbyte serial[6];
	zword abbreviations;
	zword file_size;
	zword checksum;
	zbyte interpreter_number;
	zbyte interpreter_version;
	zbyte screen_rows;
	zbyte screen_cols;
	zword screen_width;
	zword screen_height;
	zbyte font_height;
	zbyte font_width;
	zword functions_offset;
	zword strings_offset;
	zbyte default_background;
	zbyte default_foreground;
	zword terminating_keys;
	zword line_width;
	zbyte standard_high;
	zbyte standard_low;
	zword alphabet;
	zword extension_table;
	zbyte user_name[8];

	zword x_table_size;
	zword x_mouse_x;
	zword x_mouse_y;
	zword x_unicode_table;
	zword x_flags;
	zword x_fore_colour;
	zword x_back_colour;
} z_header_t;
extern z_header_t z_header;

} // End of namespace ZCode
} // End of namespace Glk

#endif
