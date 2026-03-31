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

#ifndef MADS_CORE_CONV_H
#define MADS_CORE_CONV_H

#include "common/scummsys.h"

namespace MADS {
namespace MADSV2 {

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

enum ConvStatus {
	CONV_NONE,
	CONV_STATUS_WAIT_AUTO,
	CONV_STATUS_WAIT_ENTRY
};

struct ConvControl {
	int running;
	int slot;
	ConvStatus status;
	ConvStatus prior_status;

	uint32 popup_clock;
};

extern int conv_restore_running;
extern ConvControl conv_control;
extern int *conv_my_next_start;

extern void conv_system_init();
extern void conv_system_cleanup();

extern void conv_get(int convNum);
extern void conv_run(int convNum);
extern void conv_update(bool);
extern void conv_export_pointer(int *ptr);
extern void conv_abort();
extern void conv_me_trigger(int trigger);
extern void conv_you_trigger(int trigger);
extern int *conv_get_variable(int varNum);
extern void conv_export_value(int varNum);
extern void conv_hold();
extern void conv_release();

} // namespace MADSV2
} // namespace MADS

#endif
