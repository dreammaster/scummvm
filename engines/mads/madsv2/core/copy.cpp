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

#include "common/file.h"
#include "mads/madsv2/core/copy.h"
#include "mads/madsv2/core/imath.h"
#include "mads/madsv2/core/env.h"
#include "mads/madsv2/core/popup.h"
#include "mads/madsv2/core/font.h"
#include "mads/madsv2/core/fileio.h"
#include "mads/madsv2/core/mem.h"
#include "mads/madsv2/core/error.h"
#include "mads/madsv2/core/timer.h"
#include "mads/madsv2/core/font.h"
#include "mads/madsv2/core/popup.h"

namespace MADS {
namespace MADSV2 {

extern char global_release_version[10];

char happy_file_name[12] = "XSOUND.000";
bool popup_vomitation_flag = false;

void CopyProt::load(Common::SeekableReadStream *src) {
	manual = src->readByte();
	page = src->readUint16LE();
	line = src->readUint16LE();
	word_number = src->readUint16LE();
	src->read(say, 20);
}

void copy_mangle(CopyProt *copy_prot) {
	int count;
	byte *dog;

	dog = (byte *)copy_prot;

	for (count = 0; count < sizeof(CopyProt); count++) {
		*dog ^= 0xff;
		dog++;
	}
}

int copy_load(CopyProt *copy_prot) {
	int error_flag = true;
	int num_items;
	int item;
	int seek_dog;
	Common::SeekableReadStream *handle = NULL;

	handle = env_open("*HOGANUS.DAT", "rb");
	if (handle == NULL) goto done;

	num_items = handle->readUint16LE();
	item = imath_random(1, num_items);
	if (item < 1) item = 1;
	if (item > num_items) item = num_items;

	seek_dog = (item - 1) * sizeof(CopyProt);
	if (seek_dog) {
		handle->seek(seek_dog, SEEK_CUR);
	}

	copy_prot->load(handle);

	copy_mangle(copy_prot);

	error_flag = false;

done:
	delete handle;
	return error_flag;
}

int copy_pop_and_ask(void) {
	int error_flag = COPY_FAIL;
	int result;
	int count;
	char work_buf[80];
	char page_buf[10];
	char line_buf[10];
	char word_buf[10];
	char entry_buf[80];
	CopyProt copy_prot;

	popup_vomitation_flag = false; /* Allow keep of first letter */

	if (copy_load(&copy_prot)) goto finish;

	mads_itoa(copy_prot.page, page_buf, 10);
	mads_itoa(copy_prot.line, line_buf, 10);
	mads_itoa(copy_prot.word_number, word_buf, 10);

	for (count = 0; (count < COPY_TRIES_ALLOWED); count++) {
		if (popup_create(font_inter, 32, POPUP_CENTER, POPUP_CENTER))
			goto finish;

		if (!count) {
			Common::strcpy_s(work_buf, "REX NEBULAR version ");
			Common::strcat_s(work_buf, global_release_version);
			popup_center_string(work_buf, true);
			popup_write_string("\n");
			popup_center_string("(Copy Protection, for your convenience)", false);
		} else {
			popup_center_string("ANSWER INCORRECT!", true);
			popup_write_string("\n");
			popup_center_string("(But we'll give you another chance!)", false);
		}
		popup_write_string("\n");

		Common::strcpy_s(work_buf, "Now comes the part that everybody hates.  But if we don't");
		popup_write_string(work_buf);
		Common::strcpy_s(work_buf, "do this, nasty rodent-like people will pirate this game,");
		popup_write_string(work_buf);
		Common::strcpy_s(work_buf, "and a whole generation of talented designers, programmers,");
		popup_write_string(work_buf);
		Common::strcpy_s(work_buf, "artists, and playtesters will go hungry, and will wander");
		popup_write_string(work_buf);
		Common::strcpy_s(work_buf, "aimlessly through the land at night searching for peace.");
		popup_write_string(work_buf);
		Common::strcpy_s(work_buf, "So let's grit our teeth and get it over with.  Just get");
		popup_write_string(work_buf);

		Common::strcpy_s(work_buf, "out your copy of ");
		if (copy_prot.manual == 'g') {
			Common::strcat_s(work_buf, "the GAME MANUAL");
		} else {
			Common::strcat_s(work_buf, "REX'S LOGBOOK");
		}
		Common::strcat_s(work_buf, ".  See!  That was easy.  ");
		popup_write_string(work_buf);

		Common::strcpy_s(work_buf, "Next, just turn to page ");
		Common::strcat_s(work_buf, page_buf);
		Common::strcat_s(work_buf, ".  On line ");
		Common::strcat_s(work_buf, line_buf);
		Common::strcat_s(work_buf, ", find word number ");
		Common::strcat_s(work_buf, word_buf);
		Common::strcat_s(work_buf, ", ");
		popup_write_string(work_buf);

		Common::strcpy_s(work_buf, "and type it on the line below (we've even given you");
		popup_write_string(work_buf);
		Common::strcpy_s(work_buf, "first letter as a hint).  As soon as you do that, we can get");
		popup_write_string(work_buf);
		popup_write_string("right into this really COOL adventure game!\n");

		popup_write_string("\n");
		popup_write_string("                    ");
		popup_set_ask();
		popup_write_string("\n");

		entry_buf[0] = copy_prot.say[0];
		entry_buf[1] = 0;

		result = popup_ask_string(entry_buf, 12, true);
		if (result < 0) goto finish;
		if (result > 0) {
			error_flag = COPY_ESCAPE;
			goto done;
		}

		mads_strlwr(entry_buf);
		if (strcmp(entry_buf, copy_prot.say) == 0) goto finish;
	}

	goto done;

finish:
	error_flag = COPY_SUCCEED;

done:
	popup_vomitation_flag = true; /* Reset */

	return (error_flag);
}

static int copy_read_boot_sector(byte *buffer) {
	error("TODO: copy_read_boot_sector");
}

static int copy_read_happy_file(byte *buffer) {
	int error_flag = true;
	Common::File handle;

	if (handle.open(happy_file_name)) {
		if (!fileio_fread_f(buffer, COPY_LENGTH, 1, &handle)) goto done;
	}

	error_flag = false;

done:
	handle.close();
	return error_flag;
}

static int copy_write_happy_file(byte *buffer) {
	error("TODO: copy_write_happy_file");
}

int copy_verify(void) {
	int error_flag = COPY_FAIL;
	int dog_master;
	byte *work1 = NULL;
	byte *work2 = NULL;

	work1 = (byte *)mem_get(COPY_LENGTH);
	work2 = (byte *)mem_get(COPY_LENGTH);

	if ((work1 != NULL) && (work2 != NULL)) {
		//srand((word)timer_read_dos());
		dog_master = imath_random(1, 1000);

		if (dog_master <= 50) {
			memset(work1, 0, COPY_LENGTH);
			copy_write_happy_file(work1);
		}

		if (!copy_read_boot_sector(work1)) {
			if (!copy_read_happy_file(work2)) {
				if (memcmp(work1, work2, COPY_LENGTH) == 0) {
					goto finish;
				}
			}
		}
	}

	error_flag = copy_pop_and_ask();
	if (error_flag != COPY_SUCCEED) goto done;

	if ((work1 != NULL) && (work2 != NULL)) {
		copy_write_happy_file(work1);
	}

finish:
	error_flag = COPY_SUCCEED;

done:
	if (work2 != NULL) mem_free(work2);
	if (work1 != NULL) mem_free(work1);
	return error_flag;
}

} // namespace MADSV2
} // namespace MADS
