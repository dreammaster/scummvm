/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#ifndef LEGEND_EARLY_CORE_HUFFMAN_H
#define LEGEND_EARLY_CORE_HUFFMAN_H

#include "common/stream.h"
#include "common/memstream.h"

namespace Legend {
namespace Early {

/**
 * Huffman decoder
 * TODO: See if engine can be refactored to use the common Huffman class
 */
class Huffman {
public:
	/**
	 * Decompress Huffman encoded stream
	 */
	static Common::MemoryWriteStreamDynamic *decompress(Common::SeekableReadStream &stream,
		const int16 *huffmanTable, size_t nodeCount, byte **symbols = nullptr, int symbolsCount = 0);
};

} // End of namespace Early
} // End of namespace Legend

#endif
