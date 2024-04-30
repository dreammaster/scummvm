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
#include "common/memstream.h"
#include "common/textconsole.h"
#include "krondor/res/resource.h"

namespace Krondor {

void Resource::load(const Common::String &path) {
	Common::File f;
	if (!f.open(Common::Path(path)))
		error("Opening file - %s", path.c_str());

	readIndex(&f);
	read(&f);
}

void TaggedResource::readIndex(Common::SeekableReadStream *src) {
	if (!_file)
		_file = src;

	for (;;) {
		uint32 label = src->readUint32LE();
		if (src->eos())
			break;

		switch (label) {
		case TAG_ADS:
		case TAG_APP:
		case TAG_BIN:
		case TAG_BMP:
		case TAG_DAT:
		case TAG_FNT:
		case TAG_GID:
		case TAG_INF:
		case TAG_MAP:
		case TAG_PAG:
		case TAG_PAL:
		case TAG_RES:
		case TAG_SCR:
		case TAG_SND:
		case TAG_TAG:
		case TAG_TT3:
		case TAG_TTI:
		case TAG_VER:
		case TAG_VGA: {
			uint size = src->readUint32LE();
			_tags[label] = TagEntry(src->pos(), size & 0x7fffffff);

			if (size & 0x80000000) {
				// Sub-container
				SubStream sub(src, src->pos(), size & 0x7fffffff);
				readIndex(&sub);

			} else {
				// Move to next entry
				src->skip(size);
			}
			break;
		}

		default:
			error("Unknown tag encountered - %x", label);
			break;
		}
	}
}

Common::SeekableReadStream *TaggedResource::getTag(uint32 tag) {
	if (_tags.contains(tag)) {
		_file->seek(_tags[tag]._offset);
		return _file->readStream(_tags[tag]._size);
	}

	return nullptr;
}

} // namespace Krondor
