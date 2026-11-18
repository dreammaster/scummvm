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

#ifndef ULTIMA5_DATA_LZW_H
#define ULTIMA5_DATA_LZW_H

#include "common/array.h"
#include "common/scummsys.h"

namespace Ultima {
namespace Ultima5 {
namespace Data {

/**
 * Decompresses a resource: a 32-bit little endian size followed by LZW codes
 * of 9 to 12 bits, packed from the low bit upwards
 * @returns		False if the data is invalid or ends early
 */
extern bool decompressLzw(const byte *src, size_t srcSize, Common::Array<byte> &dest);

/**
 * Loads and decompresses a resource file, such as the .16 graphics files
 * @returns		False if the file doesn't exist or is invalid
 */
extern bool loadLzwFile(const char *name, Common::Array<byte> &dest);

} // namespace Data
} // namespace Ultima5
} // namespace Ultima

#endif
