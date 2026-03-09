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

#ifndef MADS_CORE_PACK_1_H
#define MADS_CORE_PACK_1_H

#include "common/stream.h"
#include "mads/madsv2/core/general.h"

namespace MADS {
namespace MADSV2 {

extern byte *pack_read_memory_ptr;
extern byte *pack_write_memory_ptr;
extern Common::SeekableReadStream *pack_read_file_handle;
extern Common::WriteStream *pack_write_file_handle;

word pack_read_memory(char *buffer, word *size);
void pack_write_memory(char *buffer, word *size);
word pack_read_file(char *buffer, word *size);
void pack_write_file(char *buffer, word *size);

} // namespace MADSV2
} // namespace MADS

#endif
