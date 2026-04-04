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

#include "common/algorithm.h"
#include "common/str.h"
#include "common/textconsole.h"
#include "mads/madsv2/core/conv.h"

namespace MADS {
namespace MADSV2 {

Conv *conv[CONV_MAX_DATA];
ConvData *conv_data[CONV_MAX_DATA];
Conv *active_conv;
ConvData *active_conv_data;
int conv_restore_running;
ConvControl conv_control;
int *conv_my_next_start;

static int conv_slot_indexes[CONV_MAX_SLOTS];
static int conv_slots[CONV_MAX_DATA];

static const char *conv_get_filename(int convNum, int extType, char *name) {
	*name = '\0';

	if (extType != 2)
		Common::strcat_s(name, 40, "*");
	Common::strcat_s(name, 40, "conv");
	Common::strcat_s(name, 40, Common::String::format("%d", convNum).c_str());
	if (extType == 2)
		Common::strcat_s(name, 40, ".dat");

	return name;
}

static ConvData *conv_load_data(const char *fname) {
	// TODO
	return nullptr;
}

void conv_system_init() {
	Common::fill((byte *)&conv_control, (byte *)&conv_control + sizeof(ConvControl), 0);
	conv_control.running = -1;

	Common::fill(conv_slot_indexes, conv_slot_indexes + CONV_MAX_SLOTS, 0);
	Common::fill(conv_slots, conv_slots + CONV_MAX_DATA, 0);
	Common::fill(conv, conv + CONV_MAX_DATA, (Conv *)nullptr);
	Common::fill(conv_data, conv_data + CONV_MAX_DATA, (ConvData *)nullptr);
	conv_system_cleanup();
}

void conv_system_cleanup() {
	// Removes any files with the format 'conv%d.dat'
}

void conv_get(int convNum) {
	int free_slot = -1;
	char fname[40];

	for (int i = 0; i < CONV_MAX_DATA && free_slot == -1; ++i) {
		if (!conv_slots[i])
			free_slot = i;
	}

	if (free_slot >= 0) {
		conv_slots[free_slot] = -1;
		(void)conv_load_data(conv_get_filename(convNum, 0, fname));
		// TODO: More stuff
	}
}

void conv_run(int convNum) {
}

void conv_update(bool) {
}

void conv_regenerate_last_message() {
}

void conv_export_pointer(int *ptr) {}

void conv_abort() {}

void conv_me_trigger(int trigger) {}

void conv_you_trigger(int trigger) {}

int *conv_get_variable(int varNum) {
	assert(varNum >= 0 && varNum < active_conv_data->variablesCount);

	// TODO
	return nullptr;
}

void conv_export_value(int varNum) {
	// TODO
}

void conv_hold() {
	// TODO
}

void conv_release() {
	// TODO
}

void conv_flush() {
	// TODO
}

int conv_append(Common::WriteStream *handle) {
	error("TODO: conv_append");
	return 0;
}

int conv_expand(Common::SeekableReadStream *handle) {
	error("TODO: conv_expand");
	return 0;
}


} // namespace MADSV2
} // namespace MADS
