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

#include "audio/mixer.h"
#include "common/textconsole.h"
#include "mads/madsv2/core/speech.h"
#include "mads/madsv2/core/env.h"
#include "mads/madsv2/core/mem.h"
#include "mads/madsv2/core/pack.h"
#include "mads/madsv2/engine.h"

namespace MADS {
namespace MADSV2 {

bool speech_system_active = false;
bool speech_on = false;
int speech_ems_handle;
SpeechBuffer speech_main_buffer;
char global_speech_resource[16] = "*PHAN009.DSR";
int  global_speech_ready = -1;


void SpeechDir::load(Common::SeekableReadStream *src) {
	src->readMultipleLE(field0, compression, field4, field6, field8, size, offset);
}


void speech_init() {
	speech_system_active = true;
}

void speech_shutdown() {
	speech_system_active = false;
}

SpeechDirPtr speech_load(const char *resName, int id, bool useMainMemory) {
	SpeechDirPtr result = nullptr;
	SpeechDirPtr speechPtr = nullptr;
	uint filePos;
	int count;
	SpeechDir speechDir;
	int headerSize, totalSize;
	byte *load_buf;
	int packing_flag;

	// Always use main memory in ScummVM
	useMainMemory = true;

	// Open the sound resource for access
	Common::SeekableReadStream *handle = env_open(resName);
	if (!handle) goto done;
	filePos = handle->pos();

	// Get the number of voice samples in the file
	count = handle->readUint16LE();

	// Validate the speech Id specified is within range
	--id;
	if (id < 0 || id >= count) goto done;

	// Seek to the correct offset and read the index entry
	if (id > 0)
		handle->seek(id * SpeechDir::SIZE, SEEK_CUR);
	speechDir.load(handle);

	// Seek to the start of the voice content
	filePos += speechDir.offset;
	handle->seek(filePos);

	// Get the buffer space
	headerSize = ((sizeof(SpeechDir) + 15) / 16) * 16;
	totalSize = headerSize + speechDir.size;
	speechPtr = (SpeechDirPtr)mem_get_name(totalSize, "$SPEECH");

	// Copy the the index entry into memory block
	*speechPtr = speechDir;
	load_buf = (byte *)speechPtr + headerSize;

	// Decompress the data
	pack_strategy = speechDir.compression;
	packing_flag = (speechDir.compression != PACK_NONE) ? PACK_EXPLODE : PACK_RAW_COPY;

	if (pack_data(packing_flag, speechDir.size, FROM_DISK, handle, TO_MEMORY, load_buf) != speechDir.size) goto done;

	// At this point we have valid data
	result = speechPtr;

done:
	if (useMainMemory && !result && speechPtr)
		mem_free(speechPtr);
	delete handle;

	return result;
}

void speech_play(const char *resName, int id) {
	SpeechDirPtr speech = speech_load(resName, id);
	assert(speech);

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
		speech_play(global_speech_resource, id);
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
