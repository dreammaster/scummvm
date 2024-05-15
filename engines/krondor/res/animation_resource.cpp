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

#include "krondor/res/animation_resource.h"
#include "krondor/res/resource_tag.h"

namespace Krondor {

void AnimationResource::clear() {
	_version.clear();
	_animationMap.clear();
	delete _script;
	_script = nullptr;
}

void AnimationResource::load(const Common::String &resName) {
	clear();
	File f(resName);
	loadIndex(&f);

	Common::SeekableReadStream *verbuf = getTag(&f, TAG_VER);
	Common::SeekableReadStream *resbuf = getTag(&f, TAG_RES);
	Common::SeekableReadStream *scrbuf = getTag(&f, TAG_SCR);
	Common::SeekableReadStream *tagbuf = getTag(&f, TAG_TAG);
	if (!verbuf || !resbuf || !scrbuf || !tagbuf)
		error("Animation resource data corruption");

	_version = verbuf->readString();

	if (scrbuf->readByte() != 2)
		error("Animation resource data corruption");

	_script = DecompressLZW(scrbuf).decompress();

	ResourceTag tags;
	tags.load(tagbuf);

	uint n = resbuf->readUint16LE();
	for (uint i = 0; i < n; i++) {
		uint id = resbuf->readUint16LE();
		Common::String resource = resbuf->readString();
		Common::String name;
		if (tags.find(id, name)) {
			AnimationData data;
			data._name = name;
			data._resource = resource;
			_animationMap[id] = data;

		} else {
			error("Data corruption loading animation resources");
		}
	}

	delete verbuf;
	delete resbuf;
	delete scrbuf;
	delete tagbuf;
}

} // namespace Krondor
