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
//include "common/compression/unzip.h"
#include "spycraft/dmade/advlib.h"
#include "spycraft/dmade/advdcmp.h"
#include "spycraft/dmade/advfile.h"
#include "spycraft/dmade/advvols.h"

namespace Spycraft {

typedef void *voidpf;

struct _DcmpInfo {
	HANDLE file;
	uint8 code;
	int size, c_size;
	char compressor;
	Common::SeekableReadStream *stream;
};

DcmpStream OpenDcmpStream(HANDLE file, int size, int c_size, uint8 compressor) {
	DcmpStream dStream;

	assert((compressor == COMPRESS_NONE) || (compressor == COMPRESS_ZLIB));
	dStream = (DcmpStream)AllocPtr(sizeof(*dStream));
	assert(dStream != NULL);
	dStream->file = file;
	dStream->size = size;
	dStream->c_size = c_size;
	dStream->compressor = compressor;
	dStream->stream = nullptr; // TODO: How is this achieved?

	return dStream;
}

int ReadDcmpStream(DcmpStream dStream, void *buffer, int size) {
	assert(dStream->stream);
	size_t bytesRead = dStream->stream->read(buffer, size);
	dStream->size -= bytesRead;

	return bytesRead;
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
	delete dStream->stream;
	FreePtr(dStream);
}

} // namespace Spycraft
