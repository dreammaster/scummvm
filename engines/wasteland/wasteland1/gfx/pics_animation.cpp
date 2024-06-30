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

#include "wasteland/wasteland1/gfx/pics_animation.h"
#include "wasteland/wasteland1/data/msq_header.h"
#include "wasteland/core/file.h"
#include "wasteland/wasteland1/files/huffman_stream.h"

namespace Wasteland {
namespace Wasteland1 {
namespace Gfx {

bool PicsAnimation::read(Common::SeekableReadStream *src, int width) {
#ifdef TODO
	Data::MsqHeader header;
	int height;
	HuffmanStream huffmanStream;
	Pic baseFrame;

	// Read the next MSQ header and validate it
	header = MsqHeader.read(stream);
	if (header == null)
	{
		// No more animations, abort.
		return null;
	}
	if (header.getType() != MsqType.Compressed)
	{
		throw new IOException(
			"Expected base frame block of PICS stream to be compressed");
	}

	// Calculate the height
	height = header.getSize() * 2 / width;

	// Read the base frame
	huffmanStream = new HuffmanInputStream(stream);
	baseFrame = Pic.read(huffmanStream, width, height);

	// Read the second MSQ header (The animation frames) and validate it
	header = MsqHeader.read(stream);
	if (header == null)
	{
		throw new EOFException(
			"Unexpected end of stream while reading PICS animation block");
	}
	if (header.getType() != MsqType.Compressed)
	{
		throw new IOException(
			"Expected animation block of PICS stream to be compressed");
	}

	// Start a huffman input stream
	huffmanStream = new HuffmanInputStream(stream);

	return new PicsAnimation(baseFrame, readAnimationData(huffmanStream,
		baseFrame));
#endif
	return false;
}

} // namespace Gfx
} // namespace Wasteland1
} // namespace Wasteland
