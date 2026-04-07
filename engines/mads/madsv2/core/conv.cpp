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
#include "mads/madsv2/core/env.h"
#include "mads/madsv2/core/error.h"
#include "mads/madsv2/core/fileio.h"
#include "mads/madsv2/core/game.h"
#include "mads/madsv2/core/inter.h"
#include "mads/madsv2/core/kernel.h"
#include "mads/madsv2/core/matte.h"
#include "mads/madsv2/core/mem.h"
#include "mads/madsv2/core/popup.h"
#include "mads/madsv2/core/speech.h"

namespace MADS {
namespace MADSV2 {

Conv *conv[CONV_MAX_DATA];
ConvData *conv_data[CONV_MAX_DATA];
Conv *active_conv;
ConvData *active_conv_data;
int16 *conv_imports;
uint16 *conv_entry_flags;
ConvVariable *conv_varsDataPtr;
int16 *conv_vars0ValPtr;
int conv_restore_running;
ConvControl conv_control;
Box conv_box;
int16 *conv_my_next_start;
int conv_error_code;

static int conv_indexes[CONV_MAX_SLOTS];
static int conv_slots[CONV_MAX_DATA];


void ConvHeader::load(Common::SeekableReadStream *src) {
	src->readMultipleLE(node_count, dialog_count, message_count, text_line_count,
		num_variables, max_imports, speaker_count);
	for (int i = 0; i < 5; ++i)
		src->read(speaker_portraits[i], 16);
	for (int i = 0; i < 5; ++i)
		speaker_frame[i] = src->readSint16LE();
	src->read(speech_file, 14);
	src->readMultipleLE(text_length, commands_length);

	// Read in the offsets
	src->readMultipleLE(textOffset, scriptsOffset, nodesOffset,
		dialogsOffset, messagesOffset, textLinesOffset);
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

void ConvDataHeader::load(Common::SeekableReadStream *src) {
	src->readMultipleLE(currentNode, entryFlagsCount, variablesCount,
		importsCount, numImports, array1_size,
		messageList1_size, messageList2_size, speechListSize, messageList4_size);
	src->readMultipleLE(array1);
	src->readMultipleLE(messageList1);
	src->readMultipleLE(messageList2);
	src->readMultipleLE(speechList);
	src->readMultipleLE(messageList4);
	src->readMultipleLE(importsOffset, entryFlagsOffset, variablesOffset);
}

//====================================================================

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

static Common::SeekableReadStream *conv_open(int convNum) {
	char name[40];
	return env_open(conv_get_filename(convNum, 2, name));
}

static Conv *load_conv(const char *fname) {
	Load file;
	ConvHeader convHeader;
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
		byte hdrBuf[ConvHeader::SIZE];
		if (!loader_read(hdrBuf, ConvHeader::SIZE, 1, &file)) {
			conv_error_code = 2;
			goto done;
		}
		Common::MemoryReadStream hdrStream(hdrBuf, ConvHeader::SIZE);
		convHeader.load(&hdrStream);
	}


	dataPtr = new Conv();
	if (!dataPtr) {
		conv_error_code = 3;
		goto done;
	}

	// Copy the loaded header to the front of the block
	*dataPtr = convHeader;

	// Read each section from the file.  Error codes deliberately match the
	// originals (note: 6 is skipped, matching the disassembly).

	// Nodes
	{
		byte *buffer = (byte *)malloc(dataPtr->node_count * ConvNode::SIZE);
		if (!loader_read(buffer, convHeader.node_count * ConvNode::SIZE, 1, &file)) {
			conv_error_code = 4;
			free(buffer);
			goto done;
		}

		Common::MemoryReadStream src(buffer, convHeader.node_count * ConvNode::SIZE);
		dataPtr->nodes.resize(convHeader.node_count);
		for (int i = 0; i < convHeader.node_count; ++i)
			dataPtr->nodes[i].load(&src);

		free(buffer);
	}

	// Dialogs
	{
		byte *buffer = (byte *)malloc(dataPtr->dialog_count * ConvDialog::SIZE);
		if (!loader_read(buffer, convHeader.dialog_count * ConvDialog::SIZE, 1, &file)) {
			conv_error_code = 5;
			free(buffer);
			goto done;
		}

		Common::MemoryReadStream src(buffer, convHeader.dialog_count * ConvDialog::SIZE);
		dataPtr->dialogs.resize(convHeader.dialog_count);
		for (int i = 0; i < convHeader.dialog_count; ++i)
			dataPtr->dialogs[i].load(&src);

		free(buffer);
	}

	// Messages
	{
		byte *buffer = (byte *)malloc(dataPtr->message_count * 4);
		if (!loader_read(buffer, convHeader.message_count * 4, 1, &file)) {
			conv_error_code = 7;
			free(buffer);
			goto done;
		}

		Common::MemoryReadStream src(buffer, convHeader.message_count * 4);
		dataPtr->messages.resize(convHeader.message_count);
		for (int i = 0; i < convHeader.message_count; ++i)
			dataPtr->messages[i] = src.readSint32LE();

		free(buffer);
	}

	// Text lines
	{
		byte *buffer = (byte *)malloc(dataPtr->text_line_count * 2);
		if (!loader_read(buffer, convHeader.text_line_count * 2, 1, &file)) {
			conv_error_code = 8;
			free(buffer);
			goto done;
		}

		Common::MemoryReadStream src(buffer, convHeader.text_line_count * 2);
		dataPtr->textLines.resize(convHeader.text_line_count);
		for (int i = 0; i < convHeader.text_line_count; ++i)
			dataPtr->textLines[i] = src.readUint16LE();

		free(buffer);
	}

	// Text block
	{
		dataPtr->text.resize(convHeader.text_length);
		if (!loader_read(&dataPtr->text[0], convHeader.text_length, 1, &file)) {
			conv_error_code = 9;
			goto done;
		}
	}

	//  Scripts
	{
		dataPtr->scripts.resize(convHeader.commands_length);
		if (!loader_read(&dataPtr->scripts[0], convHeader.commands_length, 1, &file)) {
			conv_error_code = 10;
			goto done;
		}
	}

	result = dataPtr;

done:
	if (file.open)
		loader_close(&file);

	// Free the block if we bailed out before completing the load
	if (dataPtr && dataPtr != result)
		delete dataPtr;

	return result;
}

static ConvData *load_conv_data(const char *fname) {
	Load file;
	ConvDataHeader header;
	ConvData *dataPtr = nullptr;
	ConvData *result  = nullptr;
	char filename[80];
	char name_buf[9];

	file.open = false;

	Common::strcpy_s(filename, fname);
	fileio_add_ext(filename, "cnv");

	const char *fn = (filename[0] == '*') ? filename + 1 : filename;
	Common::strlcpy(name_buf, fn, sizeof(name_buf));

	if (loader_open(&file, filename, "rb", true))
		goto done;

	{
		byte hdrBuf[ConvDataHeader::SIZE];
		if (!loader_read(hdrBuf, ConvDataHeader::SIZE, 1, &file))
			goto done;

		Common::MemoryReadStream hdrStream(hdrBuf, ConvDataHeader::SIZE);
		header.load(&hdrStream);
	}

	dataPtr = new ConvData();
	if (!dataPtr)
		goto done;

	*dataPtr = header;

	// Imports: conditional — the original skips the loader_read when count <= 0
	if (header.importsCount > 0) {
		int16 *buffer = (int16 *)malloc(header.importsCount * 2);
		if (!loader_read(buffer, (long)header.importsCount * 2, 1, &file)) {
			free(buffer);
			goto done;
		}

		dataPtr->imports.resize(header.importsCount);
		for (int i = 0; i < header.importsCount; ++i)
			dataPtr->imports[i] = FROM_LE_16(buffer[i]);
		free(buffer);
	}

	// Entry flags: always read (no count guard in the original)
	{
		int16 *buffer = (int16 *)malloc(header.entryFlagsCount * 2);
		if (!loader_read(buffer, (long)header.entryFlagsCount * 2, 1, &file)) {
			free(buffer);
			goto done;
		}

		dataPtr->entryFlags.resize(header.entryFlagsCount);
		for (int i = 0; i < header.entryFlagsCount; ++i)
			dataPtr->entryFlags[i] = FROM_LE_16(buffer[i]);
		free(buffer);
	}

	// Variables
	{
		byte *buffer = (byte *)malloc(header.variablesCount * ConvVariable::SIZE);
		if (!loader_read(buffer, (long)header.variablesCount * ConvVariable::SIZE, 1, &file)) {
			free(buffer);
			goto done;
		}

		Common::MemoryReadStream src(buffer, header.variablesCount * ConvVariable::SIZE);
		dataPtr->variables.resize(header.entryFlagsCount);
		for (int i = 0; i < header.entryFlagsCount; ++i) {
			dataPtr->variables[i].load(&src);

			// Zero the runtime isPtr flag for every variable; it is not meaningful
			// as stored on disk
			dataPtr->variables[i].isPtr = false;
		}
		free(buffer);
	}

	result = dataPtr;

done:
	if (file.open)
		loader_close(&file);

	if (dataPtr && dataPtr != result)
		delete dataPtr;

	return result;
}

static ConvData *read_conv_data(Common::SeekableReadStream *src) {
	ConvDataHeader header;
	ConvData *dataPtr = nullptr;
	ConvData *result = nullptr;

	// Load the header
	header.load(src);

	dataPtr = new ConvData();
	if (!dataPtr)
		goto done;

	*dataPtr = header;

	// Imports: conditional — the original skips the loader_read when count <= 0
	if (header.importsCount > 0) {
		dataPtr->imports.resize(header.importsCount);
		for (int i = 0; i < header.importsCount; ++i)
			dataPtr->imports[i] = src->readSint16LE();
	}

	if (header.entryFlagsCount > 0) {
		dataPtr->entryFlags.resize(header.entryFlagsCount);
		for (int i = 0; i < header.entryFlagsCount; ++i)
			dataPtr->entryFlags[i] = src->readUint16LE();
	}

	if (header.variablesCount > 0) {
		dataPtr->variables.resize(header.entryFlagsCount);
		for (int i = 0; i < header.entryFlagsCount; ++i) {
			dataPtr->variables[i].load(src);
		}
	}

	result = dataPtr;

done:
	if (dataPtr && dataPtr != result)
		delete dataPtr;

	return result;
}

static void conv_init(ConvData *convData, int val) {
	conv_start(convData, nullptr);

	for (uint i = 0; i < convData->entryFlags.size(); ++i) {
		uint16 &flag = convData->entryFlags[i];
		flag &= 0x3fff;

		if ((flag & 1) || ((flag & 4) && val))
			flag |= 0x8000;
	}
}

static ConvData *conv_get_data(int convNum) {
	ConvData *convData = nullptr;
	char name[80];

	if (conv_indexes[convNum]) {
		Common::SeekableReadStream *handle = conv_open(convNum);
		if (handle) {
			convData = read_conv_data(handle);
			delete handle;
		}
	} else {
		convData = load_conv_data(conv_get_filename(convNum, 1, name));
		conv_init(convData, 0);
	}

	return convData;
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

static void conv_set_variable(int idx, int16 v1, int16 *valPtr) {
	// TODO
}

static int conv_next_node() {
	active_conv_data->currentNode = *conv_vars0ValPtr;
	return active_conv->nodes[active_conv_data->currentNode].active;
}

void conv_system_init() {
	Common::fill((byte *)&conv_control, (byte *)&conv_control + sizeof(ConvControl), 0);
	conv_control.running = -1;

	Common::fill(conv_indexes, conv_indexes + CONV_MAX_SLOTS, 0);
	Common::fill(conv_slots, conv_slots + CONV_MAX_DATA, 0);
	Common::fill(conv, conv + CONV_MAX_DATA, (Conv *)nullptr);
	Common::fill(conv_data, conv_data + CONV_MAX_DATA, (ConvData *)nullptr);
	conv_system_cleanup();
}

void conv_system_cleanup() {
	// Removes any files with the format 'conv%d.dat'
}


void conv_start(ConvData *convData, Conv *convIn) {
	active_conv      = convIn;
	active_conv_data = convData;

	// Resolve the byte-offset sub-array pointers stored in the ConvData block
	conv_imports = &convData->imports[0];
	conv_entry_flags = &convData->entryFlags[0];
	conv_varsDataPtr = &convData->variables[0];

	// conv_vars0ValPtr -> variables[0].val (skips the isPtr field)
	conv_vars0ValPtr   = &conv_varsDataPtr[0].val;

	// conv_my_next_start -> variables[1].val
	// (offset = offsetof(ConvVariable, val) + sizeof(ConvVariable) from base)
	conv_my_next_start = (int16 *)&conv_varsDataPtr[1].val;

	convData->currentNode = -1;
	convData->numImports  = 0;

	// Initialise variables[0].val from variables[1].val
	*conv_vars0ValPtr = (int16)*conv_my_next_start;
}

void conv_get(int convNum) {
	char fname[40];
	int free_slot = -1;
	int error_occurred = -1;   // matches original si: -1 = error, 0 = success
	int stage_error   = 0;     // which stage failed (1/2/3); used as data2 in error_report

	// Find first free slot (stops as soon as one is found, matching original loop)
	for (int i = 0; i < CONV_MAX_DATA; ++i) {
		if (!conv_slots[i]) {
			free_slot = i;
			break;
		}
	}

	if (free_slot < 0) {
		stage_error = 1;
		goto report;
	}

	conv_slots[free_slot] = 0xFF;

	conv[free_slot] = load_conv(conv_get_filename(convNum, 0, fname));
	if (!conv[free_slot]) {
		stage_error = 2;
		goto report;
	}

	conv_data[free_slot] = conv_get_data(convNum);
	if (!conv_data[free_slot]) {
		stage_error = 3;
		goto report;
	}

	// Encode slot as (free_slot + 2) so that 0 means "not loaded"
	conv_indexes[convNum] = free_slot + 2;
	error_occurred = 0;

report:
	if (error_occurred)
		error_report(ERROR_CONV_GET, ERROR, MODULE_CONV, convNum, stage_error);
}

void conv_run(int convId) {
	char name[80];
	int idx;

	// Validate convId is loaded (non-fatal: report error but continue, matching original)
	if (conv_indexes[convId] < 2)
		error_report(ERROR_CONV_RUN, ERROR, MODULE_CONV, convId, 0);

	// Stop any conversation already in progress
	if (conv_control.running >= 0)
		conv_abort();

	conv_control.running          = convId;
	conv_control.index            = conv_indexes[convId] - 2;
	conv_control.status           = CONV_STATUS_NEXT_NODE;
	conv_control.mask             = 0x7FFF;
	conv_control.popup_clock      = kernel.clock;
	conv_control.entry            = -1;
	conv_control.commands_allowed = player.commands_allowed;
	conv_control.input_mode       = inter_input_mode;
	conv_control.popup_is_up      = 0;
	conv_control.me_trigger       = 0;
	conv_control.you_trigger      = 0;

	// Initialise per-speaker slots.
	// speaker_val and person_speaking are written inside the loop in the original
	// (compiler artefact from an unrolled version); preserved here for fidelity.
	for (idx = 0; idx < CONV_MAX_DATA; ++idx) {
		conv_control.speaker_active[idx] = 0;
		conv_control.speaker_series[idx] = -1;
		conv_control.speaker_frame[idx]  = 1;
		conv_control.x[idx]              = (int16)0x8000;
		conv_control.y[idx]              = (int16)0x8000;
		conv_control.width[idx]          = 30;
		conv_control.speaker_val         = 1;
		conv_control.person_speaking     = 1;
	}

	int slot = conv_control.index;
	conv_start(conv_data[slot], conv[slot]);

	// Bind conv variables 2–22 to live ConvControl fields.
	// Variable 2 maps to speaker_val (not contiguous with the arrays below).
	// Variables 3–22 map to speaker_frame[5], x[5], y[5], width[5] — 20
	// consecutive int16s — so a single pointer walk replaces 20 separate calls.
	conv_set_variable(2, -1, &conv_control.speaker_val);
	int16 *p = conv_control.speaker_frame;
	for (int i = 3; i <= 22; ++i, ++p)
		conv_set_variable(i, -1, p);

	// Load speaker portrait series for each declared speaker
	for (idx = 0; idx < conv[slot]->speaker_count; ++idx) {
		Common::strcpy_s(name, conv[slot]->speaker_portraits[idx]);
		int series = kernel_load_series(name, 0x4000);
		conv_control.speaker_series[idx] = series;
		if (series > 0) {
			conv_control.speaker_active[idx] = -1;
			conv_control.speaker_frame[idx]  = conv[slot]->speaker_frame[idx];
		}
	}

	if (kernel_mode == KERNEL_ACTIVE_CODE)
		kernel_new_palette();

	player.commands_allowed = 0;
}

// ---------------------------------------------------------------------------
// conv_string
//
// Returns a pointer to the null-terminated text string whose index within the
// flat text buffer is stored in conv->textLines[textIdx].  The text blob is
// held in conv->text (stored as uint16 units but treated as a byte stream);
// textLines values are byte offsets into that blob.
// ---------------------------------------------------------------------------
static const char *conv_string(int textIdx, Conv *convIn) {
	uint16 byteOffset = convIn->textLines[textIdx];
	return reinterpret_cast<const char *>(convIn->text.begin()) + byteOffset;
}

// ---------------------------------------------------------------------------
// string_trim
//
// Strips trailing whitespace (any byte value <= ' ') from a mutable C string
// in-place.  Mirrors the original DOS helper of the same name.
// ---------------------------------------------------------------------------
static void string_trim(char *str) {
	if (!str) return;
	size_t len = strlen(str);
	while (len > 0 && (unsigned char)str[len - 1] <= ' ')
		str[--len] = '\0';
}

// ---------------------------------------------------------------------------
// conv_generate_text
//
// Builds and displays a conversation-line popup for the current speaker, then
// triggers the associated speech audio (if the speech system is active and at
// least one speech index was supplied).
//
// Parameters
//   speechList  — array of speech-audio indices; speechList[0] is played
//   convData    — active ConvData (unused here; callers already route through
//                 conv_control)
//   convIn      — active Conv (provides the text pool and speech filename)
//   textIdx     — index into convIn->textLines that selects the dialog string
//   speechCount — number of valid entries in speechList (0 = no speech)
// ---------------------------------------------------------------------------
static void conv_generate_text(int16 *speechList, ConvData * /*convData*/,
                               Conv *convIn, int textIdx, int speechCount) {
	int  person = conv_control.person_speaking;
	char textBuf[512];
	Box *savedBox;

	// Redirect popup operations to the conversation-private box so that the
	// bounds recorded by popup_draw can be used later by conv_purge_any_popup.
	savedBox = box;
	box = &conv_box;

	// Size and position the popup from the current speaker's slot data.
	int horiz_pieces = popup_estimate_pieces(conv_control.width[person]);
	popup_create(horiz_pieces,
	             conv_control.x[person],
	             conv_control.y[person]);

	// Attach the speaker portrait icon if a series was loaded for this slot.
	// The icon_id is the 1-based frame stored in speaker_frame.
	// center=0: left-aligned within the popup box.
	if (conv_control.speaker_series[person] >= 0) {
		popup_add_icon(series_list[conv_control.speaker_series[person]],
		               conv_control.speaker_frame[person],
		               0);
	}

	// Copy the dialog string to a local mutable buffer, strip trailing
	// whitespace (the raw text pool can have padding bytes), then write it.
	Common::strlcpy(textBuf, conv_string(textIdx, convIn), sizeof(textBuf));
	string_trim(textBuf);
	popup_write_string(textBuf);

	// Render the popup, saving the underlying screen region.
	popup_draw(true, false);

	// Restore the caller's box and record that a conversation popup is live.
	box = savedBox;
	conv_control.popup_is_up  = -1;
	conv_control.popup_clock  = kernel.clock;

	// Play the associated speech audio when the speech system is on.
	if (speech_system_active && speech_on && speechCount > 0) {
		speech_ems_play(convIn->speech_file, speechList[0]);
	}
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
