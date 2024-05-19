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

#include "krondor/res/sound_resource.h"
#include "krondor/res/resource_tag.h"

namespace Krondor {

void SoundResource::clear() {
	_soundMap.clear();
}

void SoundResource::load(const Common::String &resName) {
	clear();
	File f(resName);
	loadIndex(&f);

	Common::SeekableReadStream *infBuff = getTag(&f, TAG_INF);
	Common::SeekableReadStream *tagBuff = getTag(&f, TAG_TAG);
	if (!infBuff || !tagBuff)
		error("Data corruption in sound resource");

	infBuff->skip(2);
	uint n = infBuff->readUint16LE();
	infBuff->skip(1);
	ResourceTag tags;
	tags.load(tagBuff);

	for (uint i = 0; i < n; i++) {
		uint id = infBuff->readUint16LE();
		uint offset = infBuff->readUint32LE();
		Common::String name;

		if (!tags.find(id, name))
			error("Tag not found - %d", id);

		f.seek(offset + 8);
		if (id != f.readUint16LE())
			error("Data corruption in sound resource");

		SoundData data;
		data._name = name;
		data._type = f.readByte();
		f.skip(2);

		uint sndSize = f.readUint32LE() - 2;
		f.skip(2);
		Common::SeekableReadStream *soundBuff = f.readStream(sndSize);

		f.skip(-(int)soundBuff->size());
		int code = f.readByte();

		while (code != 0xff) {
			Sound *sound = new Sound(code);
			Common::Array<uint> offsetVec;
			Common::Array<uint> sizeVec;
			code = f.readByte();

			while (code != 0xff) {
				f.skip(1);
				offsetVec.push_back(f.readUint16LE());
				sizeVec.push_back(f.readUint16LE());
				code = f.readByte();
			}

			for (uint j = 0; j < offsetVec.size(); j++) {
				soundBuff->seek(offsetVec[j]);
				Common::SeekableReadStream *sampleBuff =
					soundBuff->readStream(sizeVec[j]);
				sound->load(sampleBuff);
				delete sampleBuff;
			}

			data._sounds.push_back(sound);
			code = f.readByte();
		}

		_soundMap[id] = data;
		delete soundBuff;
	}

	delete infBuff;
	delete tagBuff;
}


SoundData::~SoundData() {
	for (auto *snd : _sounds)
		delete snd;
}


} // namespace Krondor
