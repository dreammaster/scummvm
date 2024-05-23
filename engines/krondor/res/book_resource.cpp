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

#include "krondor/res/book_resource.h"

namespace Krondor {

BookResource::BookResource() : _paragraphs() {
}

BookResource::~BookResource() {
	clear();
}

uint BookResource::getNumParagraphs() const {
	return _paragraphs.size();
}

Common::String &BookResource::getParagraph(const uint i) {
	return _paragraphs[i];
}

void BookResource::clear() {
	_paragraphs.clear();
}

void BookResource::load(Common::SeekableReadStream *src) {
	clear();
	src->skip(4);

	// TODO: read is being called every loop?
	for (uint i = 0; i < src->readUint16LE(); i++) {
		src->skip(4);
	}

	for (uint i = 0; i < 40; i++) {
		src->skip(2);
	}

	while (!src->eos()) {
		uint code = src->readByte();
		Common::String s = "";

		while ((code & 0xf0) != 0xf0) {
			s += (char)code;
			code = src->readByte();
		}
		if (!s.empty()) {
			_paragraphs.push_back(s);
		}

		switch (code) {
		case 0xf0:
			do {
				src->skip(2);
				if (!src->eos()) {
					code = src->readByte();
					src->seek(-1, SEEK_CUR);
				}
			} while ((code != 0xf0) && (!src->eos()));
			break;

		case 0xf1:
			for (uint i = 0; i < 8; i++)
				src->skip(2);
			break;

		case 0xf4:
			for (uint i = 0; i < 5; i++)
				src->skip(2);
			break;

		default:
			break;
		}
	}
}

} // namespace Krondor
