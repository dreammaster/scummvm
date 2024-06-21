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
#include "wasteland/wasteland1/data/msq_header.h"

namespace Wasteland {
namespace Wasteland1 {
namespace Data {

void MsqHeader::synchronize(Common::SeekableReadStream *src) {
	int b1, b2, b3, b4;
	int size;

	b1 = src->readByte();
	b2 = src->readByte();
	b3 = src->readByte();
	b4 = src->readByte();

	// Check for uncompressed MSQ block type
	if (b1 == 'm' && b2 == 's' && b3 == 'q' && (b4 == '0' || b4 == '1')) {
		_type = MsqType::Uncompressed;
		_disk = b4 - '0';
		_size = 0;

	} else {
		// Assume the first four bytes are size information and read the next
		// four bytes
		size = b1 | (b2 << 8) | (b3 << 16) | (b4 << 24);

		b1 = src->readByte();
		b2 = src->readByte();
		b3 = src->readByte();
		b4 = src->readByte();

		if (b1 == 'm' && b2 == 's' && b3 == 'q' && (b4 == 0 || b4 == 1)) {
			// Compressed MSQ block type
			_type = MsqType::Compressed;
			_disk = b4;
			_size = size;

		} else if (b1 == 0x08 && b2 == 0x67 && b3 == 0x01 && b4 == 0) {
			// CPA Animation block type
			_type = MsqType::CpaAnimation;
			_disk = b4;
			_size = size;

		} else {
			error("Unable to read MSQ header from stream");
		}
	}
}

} // namespace Data
} // namespace Wasteland1
} // namespace Wasteland
