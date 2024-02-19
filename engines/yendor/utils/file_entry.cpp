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

#include "common/file.h"
#include "common/savefile.h"
#include "yendor/utils/file_entry.h"
#include "yendor/yendor.h"

namespace Yendor {

void FileEntry::close() {
	delete _stream;
	_stream = nullptr;

	g_engine->_errorCode = 0;
}

bool FileEntry::create() {
	_stream = g_system->getSavefileManager()->openForSaving(_filename);
	if (_stream) {
		g_engine->_errorCode = 0;
		return true;
	} else {
		return false;
	}
}

bool FileEntry::open() {
	Common::File *f = new Common::File();

	if (f->open(Common::Path(_filename))) {
		_stream = f;
		g_engine->_errorCode = 0;
		return true;
	} else {
		delete f;
		return false;
	}
}

bool FileEntry::read() {
	seek();
	Common::SeekableReadStream *rs = dynamic_cast<Common::SeekableReadStream *>(_stream);
	assert(rs);

	if (rs->read(_buffer, _blockSize) == _blockSize) {
		g_engine->_errorCode = 0;
		return true;
	} else {
		return false;
	}
}

bool FileEntry::write() {
	seek();
	Common::WriteStream *ws = dynamic_cast<Common::WriteStream *>(_stream);
	assert(ws);

	if (ws->write(_buffer, _blockSize) == _blockSize) {
		g_engine->_errorCode = 0;
		return true;
	} else {
		return false;
	}
}

void FileEntry::seek() {
	Common::SeekableReadStream *rs = dynamic_cast<Common::SeekableReadStream *>(_stream);
	Common::SeekableWriteStream *ws = dynamic_cast<Common::SeekableWriteStream *>(_stream);

	if (ws)
		ws->seek(_blockOffset + _blockIndex * _blockSize);
	else if (rs)
		rs->seek(_blockOffset + _blockIndex * _blockSize);
}

} // namespace Yendor
