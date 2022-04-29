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

#include "legend/shannara/flix.h"
#include "legend/memory.h"
#include "legend/shannara/globals.h"

namespace Legend {
namespace Shannara {

void QHeader::load(Stream *src) {
	_id = src->readUint16BE();
	_field2 = src->readUint16LE();
	_field4 = src->readUint16LE();
	_field6 = src->readUint16LE();
	_field8 = src->readByte();
	_field9 = src->readByte();
	_fieldA = src->readUint16LE();
	_fieldC = src->readUint32LE();
	_field10 = src->readByte();
	_field16 = 0;
	src->skip(5);
	_field16 = src->readUint32LE();
}


void q_init(int y, PalettePtr pal) {
	if (!_GS(qbuffer) && pal) {
		_GS(qpalette) = pal;
		_GS(qbuffer) = (byte *)g_mem->new_pointer(0x13C06);

		if (_GS(qbuffer)) {
			_GS(qbuffer_orig) = _GS(qbuffer);
			_GS(video_buffer) = gxVideoAddr(0, y);
			_GS(q_y1) = y;
			_GS(q_y2) = y + 287;
			memset(_GS(qpalette), 0, PALETTE_SIZE);
		}
	}
}

void q_open(const char *filename, size_t bufSize) {
	if (_GS(qbuffer) && (_GS(q_fp) = fp_open(filename)) != nullptr) {
		if (bufSize)
			fp_set_bufsiz(_GS(q_fp), bufSize);

		_GS(qheader)->load(_GS(q_fp));
		if (_GS(qheader)->_id != MKTAG16('9', 'h')) {
			// Invalid header Id
			fp_close(_GS(q_fp));
			_GS(q_fp) = nullptr;

		} else {
			if (!_GS(qheader)->_field10)
				_GS(qheader)->_field10 = 5;

			_GS(q_y2) = _GS(q_y1) + _GS(qheader)->_field9 *
				_GS(qheader)->_field6 - 1;
			_GS(new_palette) = nullptr;
			_GS(qframe) = nullptr;
			_GS(next_qframe) = 0;
			_GS(qinfo)->_field4 = _GS(qheader)->_field4 - 1;
			_GS(qinfo)->_field6 = _GS(qheader)->_field6;

			// TODO: Stuff
		}
	}
}

void q_close() {
	_GS(qaudio_buffer_size) = 0;

	if (_GS(qbuffer) && _GS(q_fp)) {
		fp_close(_GS(q_fp));
		_GS(q_fp) = nullptr;
	}
}

void q_kill() {
	if (_GS(composite_addr_tbl))
		g_mem->kill_pointer(_GS(composite_addr_tbl));

	if (_GS(composite_buffer))
		g_mem->kill_pointer(_GS(composite_buffer));

	if (_GS(qbuffer)) {
		_GS(qbuffer) = _GS(qbuffer_orig);
		q_close();
		g_mem->kill_pointer(_GS(qbuffer));
		_GS(qbuffer) = nullptr;
		_GS(video_buffer) = nullptr;
	}
}

} // namespace Shannara
} // namespace Legend
