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

#include "wasteland/wasteland1/gfx/raw_animation_frame_part.h"

namespace Wasteland {
namespace Wasteland1 {
namespace Gfx {

void RawAnimationFramePart::apply(Pic *image) {
	int x, y;
	int xorVal;

	x = (_offset * 2) % image->w;
	y = (_offset * 2) / image->w;

	for (int xorVals : _diff) {
		xorVal = (xorVals >> 4) & 0x0f;
		image->setPixel(x, y, image->getPixel(x, y) ^ xorVal);
		x++;

		if (x >= image->w) {
			x = 0;
			y++;
		}

		xorVal = xorVals & 0x0f;
		image->setPixel(x, y, image->getPixel(x, y) ^ xorVal);
		x++;

		if (x >= image->w) {
			x = 0;
			y++;
		}
	}
}

RawAnimationFramePart *RawAnimationFramePart::read(
		Common::SeekableReadStream *stream) {
	int address;
	int bytes;
	int h, l;
	int offset;
	int size;
	Common::Array<int> diff;

	// Read address from stream-> Abort if hit the end of the frame part
	l = stream->readByte();
	h = stream->readByte();

	address = l | (h << 8);
	if (address == 0xffff) {
		return nullptr;
	}

	// Extract number of diff bytes from address
	bytes = ((address >> 12) & 0xf) + 1;
	address = address & 0xfff;

	size = 2;
	offset = address;
	diff.resize(bytes);

	for (int i = 0; i < bytes; i++) {
		diff[i] = stream->readByte();
		size++;
	}

	return new RawAnimationFramePart(offset, diff, size);
}

bool RawAnimationFramePart::isMergable(const RawAnimationFramePart &other) const {
	int size;

	size = other._offset - _offset + other._diff.size();
	if (size > 16)
		return false;

	if ((size + 2) >= (int)(other._diff.size() + _diff.size() + 4))
		return false;

	return true;
}

void RawAnimationFramePart::merge(const RawAnimationFramePart &other) {
	int nulls;
	Common::Array<int> newDiff;

	nulls = other._offset - _offset - _diff.size();
	newDiff.resize(nulls + _diff.size() + other._diff.size());

	for (uint i = 0; i < _diff.size(); i++)
		newDiff[i] = _diff[i];

	for (int i = 0; i < nulls; i++)
		newDiff[_diff.size() + i] = 0;

	for (uint i = 0; i < other._diff.size(); i++)
		newDiff[_diff.size() + nulls + i] = other._diff[i];

	_diff = newDiff;
}

} // namespace Gfx
} // namespace Wasteland1
} // namespace Wasteland
