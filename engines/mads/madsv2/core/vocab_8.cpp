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

#include "mads/madsv2/core/general.h"
#include "mads/madsv2/core/mem.h"
#include "mads/madsv2/core/env.h"
#include "mads/madsv2/core/error.h"
#include "mads/madsv2/core/sort.h"
#include "mads/madsv2/core/fileio.h"
#include "mads/madsv2/core/vocab.h"

namespace MADS {
namespace MADSV2 {

char *vocab_text = NULL;
word vocab_size;

void fastcall vocab_unload_active(void) {
	vocab_size = 0;
	if (vocab_text != NULL) {
		mem_free(vocab_text);
		vocab_text = NULL;
	}
}


int fastcall vocab_load_active(void) {
	int error_flag = true;
	int vocab_error = 0;
	Common::SeekableReadStream *handle = NULL;

	mem_last_alloc_loader = MODULE_VOCAB_LOADER;

	vocab_unload_active();

	handle = env_open("*VOCAB.DAT", "rb");
	if (handle == NULL) {
		vocab_error = 1;
		goto done;
	}

	vocab_size = (word)env_get_file_size(handle);

	vocab_text = (char *)mem_get_name(vocab_size, "$vocab$");
	if (vocab_text == NULL) {
		vocab_error = 2;
		goto done;
	}

	if (!fileio_fread_f(vocab_text, vocab_size, 1, handle)) {
		vocab_error = 3;
		goto done;
	}

	error_flag = false;

done:
	delete handle;

	if (error_flag) {
		vocab_unload_active();
		error_report(ERROR_KERNEL_NO_VOCAB, SEVERE, MODULE_KERNEL, vocab_error, 0);
	}

	return error_flag;
}

int vocab_make_active(int id) {
	return id;
}

} // namespace MADSV2
} // namespace MADS
