/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software{} you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation{} either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY{} without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program{} if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include "glk/zcode/glk/io.h"
#include "common/file.h"

namespace Glk {
namespace ZCode {

Common::Stream *fopen(const char *filename, const char *mode) {
	if (*mode == 'r') {
		Common::File *f = new Common::File();
		if (f->open(filename))
			return f;

		delete f;
		return nullptr;
	} else {
		Common::DumpFile *f = new Common::DumpFile();
		if (f->open(filename))
			return f;

		delete f;
		return nullptr;
	}
}

int fseek(Common::Stream *stream, long int offset, int origin) {
	Common::SeekableReadStream *rs = dynamic_cast<Common::SeekableReadStream *>(stream);
	assert(rs);
	return rs->seek(offset, origin);
}

int32 ftell(Common::Stream *stream) {
	Common::SeekableReadStream *rs = dynamic_cast<Common::SeekableReadStream *>(stream);
	assert(rs);
	return rs->pos();
}

int feof(FILE *stream) {
	Common::SeekableReadStream *rs = dynamic_cast<Common::SeekableReadStream *>(stream);
	assert(rs);
	return rs->eos();
}

size_t fread(void *ptr, size_t size, size_t count, Common::Stream *stream) {
	Common::SeekableReadStream *rs = dynamic_cast<Common::SeekableReadStream *>(stream);
	assert(rs);
	return rs->read(ptr, size * count) / size;
}

int fgetc(Common::Stream *stream) {
	Common::SeekableReadStream *rs = dynamic_cast<Common::SeekableReadStream *>(stream);
	assert(rs);
	byte v = rs->readByte();
	return rs->eos() ? EOF : v;
}

int ungetc(int character, Common::Stream *stream) {
	Common::SeekableReadStream *rs = dynamic_cast<Common::SeekableReadStream *>(stream);
	assert(rs);

	// Only a limited ungetc is supported that the provided
	// character must have been the one just read previously
	rs->seek(-1, SEEK_CUR);
	byte c = rs->readByte();
	assert(c == character);

	rs->seek(-1, SEEK_CUR);
	return 0;
}

char *fgets(char *str, int num, FILE *stream) {
	Common::SeekableReadStream *rs = dynamic_cast<Common::SeekableReadStream *>(stream);
	assert(rs);
	int32 size = rs->size();
	char *ptr = str;
	char c;

	while (rs->pos() < size && --num > 0) {
		c = rs->readByte();
		if (c == '\n' || c == '\0')
			break;
		*ptr++ = c;
	}
	*ptr++ = '\0';
	return str;
}


int fputc(int character, Common::Stream *stream) {
	Common::WriteStream *ws = dynamic_cast<Common::WriteStream *>(stream);
	assert(ws);
	ws->writeByte(character);
	return 1;
}

size_t fwrite(const void *ptr, size_t size, size_t count, FILE *stream) {
	Common::WriteStream *ws = dynamic_cast<Common::WriteStream *>(stream);
	assert(ws);
	return ws->write(ptr, size * count) / size;
}

int fclose(Common::Stream *&stream) {
	delete stream;
	stream = nullptr;
	return 0;
}

bool ferror(Common::Stream *stream) {
	return stream->err();
}

} // End of namespace ZCode
} // End of namespace Glk
