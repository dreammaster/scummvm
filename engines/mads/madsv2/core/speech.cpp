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

#include "common/textconsole.h"
#include "mads/madsv2/core/speech.h"

namespace MADS {
namespace MADSV2 {

bool speech_system_active = false;
bool speech_on = false;
int speech_ems_handle;
SpeechBuffer speech_main_buffer;
char global_speech_resource[16] = "*PHAN009.DSR";
int  global_speech_ready = -1;

SpeechDirPtr speech_load(const char *resName, int id, bool) {
	warning("TODO: speech_load");
	return nullptr;
}

void speech_ems_play(const char *resName, int id) {
	warning("TODO: global_speech_resource");
}

void speech_all_off() {
	warning("TODO: speech_all_off");
}

void speech_sample_rate(int rate) {
	warning("TODO: speech_sample_rate");
}

void speech_ems_go(int handle, int size) {
	warning("TODO: speech_ems_go");
}

void global_speech(int id) {
	if (speech_system_active && speech_on) {
		speech_ems_play(global_speech_resource, id);
	}
}

void global_speech_load(int id) {
	SpeechDirPtr chunk;

	if (speech_system_active && speech_on) {
		speech_all_off();
		chunk = speech_load(global_speech_resource, id, false);
		if (chunk != NULL) {
			global_speech_ready = id;
		} else {
			global_speech_ready = -1;
		}
	} else {
		global_speech_ready = -1;
	}
}

void global_speech_go(int id) {
	if (speech_system_active && speech_on) {
		if (global_speech_ready == id) {
			speech_all_off();
			speech_sample_rate(speech_main_buffer.sample_rate);
			speech_ems_go(speech_ems_handle, speech_main_buffer.decompress_size);
		} else {
			global_speech(id);
		}
	}
}

} // namespace MADSV2
} // namespace MADS
