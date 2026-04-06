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
#include "common/memstream.h"
#include "common/textconsole.h"
#include "mads/madsv2/core/conv.h"
#include "mads/madsv2/core/general.h"
#include "mads/madsv2/core/fileio.h"
#include "mads/madsv2/core/game.h"
#include "mads/madsv2/core/inter.h"
#include "mads/madsv2/core/kernel.h"
#include "mads/madsv2/core/matte.h"
#include "mads/madsv2/core/mem.h"
#include "mads/madsv2/core/popup.h"

namespace MADS {
namespace MADSV2 {

Conv *conv[CONV_MAX_DATA];
ConvData *conv_data[CONV_MAX_DATA];
Conv *active_conv;
ConvData *active_conv_data;
int conv_restore_running;
ConvControl conv_control;
Box conv_box;
int *conv_my_next_start;
int conv_error_code;

static int conv_slot_indexes[CONV_MAX_SLOTS];
static int conv_slots[CONV_MAX_DATA];



void Conv::load(Common::SeekableReadStream *src) {
	src->readMultipleLE(node_count, dialog_count, message_count, text_line_count,
		num_variables, max_imports, speaker_count);
	for (int i = 0; i < 5; ++i)
		src->read(speaker_portraits[i], 16);
	for (int i = 0; i < 5; ++i)
		speaker_frame[i] = src->readSint16LE();
	src->read(speech_file, 14);
	src->readMultipleLE(text_length, commands_length);

	// In the original these are far pointers patched after load; read as
	// raw offsets and store temporarily - conv_load_data resolves them.
	uint32 text_off, scripts_off, nodes_off, dialogs_off, messages_off, text_lines_off;
	src->readMultipleLE(text_off, scripts_off, nodes_off, dialogs_off, messages_off, text_lines_off);
	text_ptr = (void *)(uintptr_t)text_off;
	scripts_ptr = (void *)(uintptr_t)scripts_off;
	nodes_ptr = (void *)(uintptr_t)nodes_off;
	dialogs_ptr = (void *)(uintptr_t)dialogs_off;
	messages_ptr = (void *)(uintptr_t)messages_off;
	text_lines_ptr = (void *)(uintptr_t)text_lines_off;
}

void ConvNode::load(Common::SeekableReadStream *src) {
	src->readMultipleLE(index, dialog_count, active, field_6, field_8);
}

void ConvDialog::load(Common::SeekableReadStream *src) {
	src->readMultipleLE(text_line_index, speech_index, script_offset, script_size);
}

void ConvScriptParams::load(Common::SeekableReadStream *src) {
	operation = src->readSint16LE();
	param1IsVar = src->readByte();
	param2IsVar = src->readByte();
	src->readMultipleLE(param1, param2);
}

void ConvVariable::load(Common::SeekableReadStream *src) {
	src->readMultipleLE(isPtr, val, unused);
}


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

static Conv *conv_load_data(const char *fname) {
	Load file;
	Conv convHeader;
	Conv *dataPtr = nullptr;
	Conv *result = nullptr;
	char filename[80];
	char name_buf[9];

	file.open = false;

	Common::strcpy_s(filename, fname);
	fileio_add_ext(filename, "cnv");

	// Strip leading '*' (wildcard prefix used for himem search) to get bare name
	const char *fn = (filename[0] == '*') ? filename + 1 : filename;
	Common::strlcpy(name_buf, fn, sizeof(name_buf));

	if (loader_open(&file, filename, "rb", true)) {
		conv_error_code = 1;
		goto done;
	}

	{
		// Read the fixed-size Conv header through a MemoryReadStream so that
		// the load() function handles field sizes and endianness correctly.
		byte hdrBuf[Conv::SIZE];
		if (!loader_read(hdrBuf, Conv::SIZE, 1, &file)) {
			conv_error_code = 2;
			goto done;
		}
		Common::MemoryReadStream hdrStream(hdrBuf, Conv::SIZE);
		convHeader.load(&hdrStream);
	}

	{
		// Calculate total allocation size.  The +15 on node_count matches the
		// original: the DOS allocator rounded up to paragraph (16-byte) boundaries
		// for the nodes sub-array so that far-pointer normalisation never overflows.
		long nodeBytes = (long)(convHeader.node_count + 15) * ConvNode::SIZE;
		long dialogBytes = (long)convHeader.dialog_count * ConvDialog::SIZE;
		long messageBytes = (long)convHeader.message_count * 4;   // 2 x int16 per entry
		long textLineBytes = (long)convHeader.text_line_count * 2;   // 1 x int16 per entry
		long total = Conv::SIZE + nodeBytes + dialogBytes + messageBytes
			+ textLineBytes + (long)convHeader.text_length
			+ (long)convHeader.commands_length;

		dataPtr = (Conv *)mem_get_name(total, name_buf);
		if (!dataPtr) {
			conv_error_code = 3;
			goto done;
		}

		// Copy the loaded header to the front of the block, then fix up all
		// sub-array pointers to point into the memory immediately following.
		*dataPtr = convHeader;
		byte *p = (byte *)dataPtr + Conv::SIZE;

		dataPtr->nodes_ptr = p;  p += nodeBytes;
		dataPtr->dialogs_ptr = p;  p += dialogBytes;
		dataPtr->messages_ptr = p;  p += messageBytes;
		dataPtr->text_lines_ptr = p;  p += textLineBytes;
		dataPtr->text_ptr = p;  p += convHeader.text_length;
		dataPtr->scripts_ptr = p;

		// Read each section from the file.  Error codes deliberately match the
		// originals (note: 6 is skipped, matching the disassembly).
		if (!loader_read(dataPtr->nodes_ptr,
			(long)convHeader.node_count * ConvNode::SIZE, 1, &file)) {
			conv_error_code = 4;
			goto done;
		}

		if (!loader_read(dataPtr->dialogs_ptr,
			(long)convHeader.dialog_count * ConvDialog::SIZE, 1, &file)) {
			conv_error_code = 5;
			goto done;
		}

		if (!loader_read(dataPtr->messages_ptr,
			(long)convHeader.message_count * 4, 1, &file)) {
			conv_error_code = 7;
			goto done;
		}

		if (!loader_read(dataPtr->text_lines_ptr,
			(long)convHeader.text_line_count * 2, 1, &file)) {
			conv_error_code = 8;
			goto done;
		}

		if (!loader_read(dataPtr->text_ptr, convHeader.text_length, 1, &file)) {
			conv_error_code = 9;
			goto done;
		}

		if (!loader_read(dataPtr->scripts_ptr, convHeader.commands_length, 1, &file)) {
			conv_error_code = 10;
			goto done;
		}

		result = dataPtr;
	}

done:
	if (file.open)
		loader_close(&file);

	// Free the block if we bailed out before completing the load
	if (dataPtr && dataPtr != result)
		mem_free(dataPtr);

	return result;
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

static void conv_purge_any_popup() {
	if (conv_control.popup_is_up) {
		Box *my_box = box;
		box = &conv_box;
		popup_destroy();
		matte_frame(0, 0);

		if ((box->y + box->ys) >= 156)
			matte_inter_frame(-1, -1);

		box = my_box;
		conv_control.popup_is_up = 0;
		conv_control.popup_clock = kernel.clock;
	}
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

void conv_abort() {
	if (conv_control.running >= 0) {
		if (kernel_mode == KERNEL_ACTIVE_CODE)
			player.commands_allowed = conv_control.commands_allowed;

		conv_control.running = -1;
		conv_purge_any_popup();

		for (int i = conv[conv_control.index]->speaker_count - 1; i >= 0; --i) {
			if (conv_control.speaker_active[i])
				matte_deallocate_series(conv_control.speaker_series[i], -1);
		}

		if (conv_control.input_mode == INTER_CONVERSATION)
			kernel_init_dialog();

		kernel_set_interface_mode(conv_control.input_mode);
	}
}

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
