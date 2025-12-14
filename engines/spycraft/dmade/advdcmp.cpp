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

#include "common/algorithm.h"
#include "common/stream.h"
#include "common/compression/deflate.h"
#include "spycraft/dmade/advlib.h"
#include "spycraft/dmade/advdcmp.h"
#include "spycraft/dmade/advfile.h"
#include "spycraft/dmade/advvols.h"

namespace Spycraft {

typedef void *voidpf;

struct _DcmpInfo {
	Common::SeekableReadStream *_stream;
};

DcmpStream OpenDcmpStream(FHANDLE file, int size, int c_size, uint8 compressor) {
	DcmpStream dStream;

	assert((compressor == COMPRESS_NONE) || (compressor == COMPRESS_ZLIB));
	Common::SeekableReadStream *rs = dynamic_cast<Common::SeekableReadStream *>(file);
	assert(rs && ((c_size != 0) == (compressor == COMPRESS_ZLIB)));

	dStream = (DcmpStream)AllocPtr(sizeof(*dStream));
	assert(dStream != NULL);

	if (compressor == COMPRESS_NONE) {
		// No compression
		Common::SeekableReadStream *subStream = rs->readStream(size);
		dStream->_stream = subStream;
	} else {
		// ZLIB compression
		Common::SeekableReadStream *subStream = rs->readStream(c_size);
		dStream->_stream = Common::wrapCompressedReadStream(subStream, DisposeAfterUse::YES, size);
	}

	return dStream;
}

int ReadDcmpStream(DcmpStream dStream, void *buffer, int size) {
	Common::SeekableReadStream *rs = dynamic_cast<Common::SeekableReadStream *>(dStream->_stream);
	assert(rs);

	return rs->read(buffer, size);
}

uint16 ReadWordDcmpStream(DcmpStream dStream) {
	uint16 result;

	assert(ReadDcmpStream(dStream, &result, sizeof(result)) == sizeof(result));
	return FROM_LE_16(result);
}

uint32 ReadLongDcmpStream(DcmpStream dStream) {
	uint32 result;

	assert(ReadDcmpStream(dStream, &result, sizeof(result)) == sizeof(result));
	return FROM_LE_32(result);
}

void CloseDcmpStream(DcmpStream dStream) {
	delete dStream->_stream;
	FreePtr(dStream);
}

} // namespace Spycraft
