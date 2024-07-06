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

#include "common/memstream.h"
#include "wasteland/wasteland1/gfx/pics_animation.h"
#include "wasteland/wasteland1/gfx/pic.h"
#include "wasteland/wasteland1/data/msq_header.h"
#include "wasteland/core/file.h"
#include "wasteland/wasteland1/files/huffman_stream.h"

namespace Wasteland {
namespace Wasteland1 {
namespace Gfx {

PicsAnimation::~PicsAnimation() {
	delete _baseFrame;
}

bool PicsAnimation::read(Common::SeekableReadStream *src, int width) {
	Data::MsqHeader header;
	int height;

	// Read the next MSQ header and validate it
	header.synchronize(src);

	if (header._type != Data::MsqType::Compressed)
		error("Expected base frame block of PICS stream to be compressed");

	// Calculate the height
	height = header._size * 2 / width;
	assert((header._size * 2) % width == 0);

	// Read the base frame
	HuffmanStream huffmanStream1(src);
	_baseFrame = Pic::read(&huffmanStream1, width, height);

	// Read the second MSQ header (The animation frames) and validate it
	header.synchronize(src);
	if (header._type != Data::MsqType::Compressed)
		error("Expected animation block of PICS stream to be compressed");

	// Read in the animation frames data
	HuffmanStream huffmanStream2(src);
	readAnimationData(&huffmanStream2);

	return true;
}

void PicsAnimation::readAnimationData(BitStream *src) {
	PicsAnimationFrameSet *frameSet = nullptr;
	Common::Array<RawAnimationFrame> rawFrames;
	int dataSize;
	int pos;
	Common::Array<byte> instructions;
	Pic *workingFrame = nullptr;
	uint headerSize;
	int delay;
	int frameNo;

	// Read the header size from the MSQ block
	headerSize = src->readSint16LE();

	// Read the raw animation instructions
	instructions.resize(headerSize);
	if (src->read(&instructions[0], headerSize) != headerSize)
		error("Unexpected end of stream while reading animation header");
	Common::MemoryReadStream byteStream(&instructions[0], headerSize);

	// Read the raw animation frames
	dataSize = src->readUint16LE();

	pos = 0;
	while (pos < dataSize) {
		rawFrames.push_back(RawAnimationFrame());

		RawAnimationFrame &rawFrame = rawFrames.back();
		rawFrame.read(src);
		pos += rawFrame.getSize();
	}

	// Cycle through the animation instructions and build the frame sets
	while (!byteStream.eos()) {
		delay = byteStream.readByte();

		if (delay == 255) {
			// End of frames for current frame set
			frameSet = nullptr;
			continue;
		}

		frameNo = byteStream.readByte();

		if (!frameSet) {
			_frameSets.push_back(PicsAnimationFrameSet());
			frameSet = &_frameSets.back();

			workingFrame = _baseFrame->clone();
		}

		frameSet->addFrame(delay, frameNo, _baseFrame, workingFrame,
			rawFrames);
	}
}

} // namespace Gfx
} // namespace Wasteland1
} // namespace Wasteland
