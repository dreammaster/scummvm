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
#include "mads/madsv2/core/general.h"

namespace MADS {
namespace MADSV2 {

void *mem_get_name(long size, const char *) {
	void *memory_block = nullptr;
	if (size > 0)
		memory_block = malloc(size);

	return memory_block;
}

void *mem_get(long size) {
	return (mem_get_name(size, "$sys$"));
}

void mem_get_block_name(byte *block, char *block_name) {
	// TODO: See if the block_name is needed. If so, we'll need to simulate the
	// original by allocating extra space to store it
	error("TODO: mem_get_block_name");
}


bool mem_free(void *memory_block) {
	free(memory_block);
	return false;
}

int mem_adjust_impl(void *&target, long size) {
	void *p = realloc(target, size);
	if (p == NULL)
		return -1;
	target = p;
	return 0;
}

void mem_save_free(void) {
	// No implementation
}

void mem_restore_free(void) {
	// No implementation
}

} // namespace MADSV2
} // namespace MADS
