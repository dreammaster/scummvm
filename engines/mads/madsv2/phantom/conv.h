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

#ifndef MADS_PHANTOM_CONV_H
#define MADS_PHANTOM_CONV_H

#include "common/scummsys.h"

namespace MADS {
namespace MADSV2 {
namespace Phantom {


enum {
	conv001_speech_talk = 0,
	conv001_continue_abc = 1,
	conv002_questions_one = 2,
	conv001_what_one = 4,
	conv001_yesno_yes = 8,
	conv001_everything_byebye = 10,
	conv001_everything_copycat = 12,
	conv001_speak_byebye = 18,
	conv001_saytwo_1 = 22,
	conv001_instructions_three = 24,
	conv001_point_two_abc = 27,
};

enum {
	conv002_sayone_abc = 1,
	conv002_answers_job = 5,
	conv002_answers_please = 6,
	conv002_answers_house = 8,
	conv002_answers_go_on = 9,
	conv002_answers_prison = 10,
	conv002_answers_building = 11,
	conv002_answers_catacombs = 12,
	conv002_interrogate_mishap = 15,
	conv002_interrogate_chandelier = 16,
	conv002_interrogate_phantom = 19,
	conv002_interrogate_giry = 22,
	conv002_nomore_first = 25,
	conv002_saytwo_abc = 26,

	conv007_richard_intro_b = 2,
	conv007_daaeb_intro_c = 3,
	conv007_where_pushed = 8,
	conv007_where_killed = 10,
	conv007_badfall_abc = 11,
	conv007_youraoul_abc = 12,
	conv007_kiss_abc = 13,
	conv007_afterkiss_abc = 14,
	conv007_delirious_abc = 16,
	conv007_long_abc = 20,
	conv007_worry_abc = 21,
	conv007_dashing_tuxedo = 22,
	conv007_dashing_london = 23,
	conv007_answers_abc = 24,
	conv007_final_goaway = 25,
	conv007_office_abc = 28,
	conv007_solo_alone = 30,
	conv007_pinch_wait_b_nothing_b = 32,

	conv012_hello_one = 1,
	conv012_hello_four = 4,
	conv012_byebye_first = 6,
	conv012_questions_three = 7,
	conv012_questions_one = 8,
	conv012_seen_byebye = 10,
	conv012_seen_mask = 12,
	conv012_tell_byebye = 19,
	conv012_var_questions_done = 26,
	conv012_tell_knewhim = 29,
	conv012_nomore_first = 30
};

struct ConvData {
	int16 node_count;
	int16 dialog_count;
	int16 message_count;
	int16 text_line_count;
	int16 num_variables;

	int16 max_imports;
	int16 speaker_count;
	char speaker_portraits[5][16];
	int16 speaker_frame[5];
	char speech_file[14];
	uint32 text_length;

	uint32 commands_length;
	void *text_ptr;
	void *scripts_ptr;
	void *nodes_ptr;
	void *dialogs_ptr;
	void *messages_ptr;
	void *text_lines_ptr;
};

struct ConvControl {
	int running;
};

extern int conv_restore_running;
extern ConvControl conv_control;

extern void conv_system_init();
extern void conv_system_cleanup();

extern void conv_get(int convNum);
extern void conv_run(int convNum);
extern void conv_export_pointer(int *ptr);
extern void conv_abort();
extern void conv_me_trigger(int trigger);
extern void conv_you_trigger(int trigger);
extern int *conv_get_variable(int varNum);
extern void conv_export_value(int varNum);
extern void conv_hold();
extern void conv_release();

} // namespace Phantom
} // namespace MADSV2
} // namespace MADS

#endif
