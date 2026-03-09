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
#include "mads/madsv2/core/error.h"
#include "mads/madsv2/core/loader.h"
#include "mads/madsv2/core/font.h"

namespace MADS {
namespace MADSV2 {

FontPtr font_inter = NULL;
FontPtr font_main = NULL;              /* Interface & main font handles */
FontPtr font_conv = NULL;
FontPtr font_menu = NULL;
FontPtr font_misc = NULL;


FontPtr font_load(const char *name) {
	char temp_buf_2[80];
	char *mark;
	char block_name[20];
	long size;
	FontPtr new_font = NULL;
	FontPtr result = NULL;
	Load load_handle;

	mem_last_alloc_loader = MODULE_FONT_LOADER;

	load_handle.open = false;

	Common::strcpy_s(temp_buf_2, name);
	mark = strchr(temp_buf_2, '.');
	if (mark == NULL) {
		Common::strcat_s(temp_buf_2, ".FF");
	}

	mark = temp_buf_2;
	if (*mark == '*') mark++;
	strncpy(block_name, mark, 8);

	if (loader_open(&load_handle, temp_buf_2, "rb", true)) goto done;

	size = load_handle.pack.strategy[0].size;

	new_font = (FontPtr)mem_get_name(size, block_name);
	if (new_font == NULL) goto done;

	if (!loader_read(new_font, size, 1, &load_handle)) goto done;

	result = new_font;

done:
	if (new_font != NULL) {
		if (result == NULL)
			mem_free(new_font);
	}
	if (load_handle.open)
		loader_close(&load_handle);

	return result;
}

} // namespace MADSV2
} // namespace MADS
