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

} // namespace MADSV2
} // namespace MADS
