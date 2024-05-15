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

#include "krondor/res/movie_resource.h"
#include "krondor/res/resource_tag.h"

namespace Krondor {

void MovieResource::clear() {
	_version.clear();
	_pages = 0;

	for (uint i = 0; i < _movieTags.size(); i++) {
		_movieTags[i]->_data.clear();
		delete _movieTags[i];
	}

	_movieTags.clear();
}

void MovieResource::load(const Common::String &resName) {
	clear();
	File f(resName);
	loadIndex(&f);

	Common::SeekableReadStream *verbuf = getTag(&f, TAG_VER);
	Common::SeekableReadStream *pagbuf = getTag(&f, TAG_PAG);
	Common::SeekableReadStream *tt3buf = getTag(&f, TAG_TT3);
	Common::SeekableReadStream *tagbuf = getTag(&f, TAG_TAG);
	if (!verbuf || !pagbuf || !tt3buf || !tagbuf)
		error("Movie resource data corruption");

	_version = verbuf->readString();
	_pages = pagbuf->readUint16LE();
	tt3buf->skip(1);

	Common::SeekableReadStream *tmpbuf = DecompressRLE(tt3buf).decompress();

	ResourceTag tags;
	tags.load(tagbuf);

	while (!tmpbuf->eos()) {
		MovieTag *mt = new MovieTag;
		uint code = tmpbuf->readUint16LE();
		uint size = code & 0x000f;
		code &= 0xfff0;
		mt->_code = code;

		if ((code == 0x1110) && (size == 1)) {
			uint id = tmpbuf->readUint16LE();
			mt->_data.push_back(id);

			Common::String name;
			if (tags.find(id, name)) {
				mt->_name = name;
			}
		} else if (size == 15) {
			mt->_name = tmpbuf->readString();
			mt->_name.toUppercase();

			if (tmpbuf->pos() & 1) {
				tmpbuf->skip(1);
			}
		} else {
			for (uint i = 0; i < size; i++) {
				mt->_data.push_back(tmpbuf->readSint16LE());
			}
		}

		_movieTags.push_back(mt);
	}

	delete tmpbuf;

	delete verbuf;
	delete pagbuf;
	delete tt3buf;
	delete tagbuf;
}

} // namespace Krondor
