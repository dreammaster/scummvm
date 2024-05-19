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

#ifndef KRONDOR_SOUND_H
#define KRONDOR_SOUND_H

#include "audio/midiplayer.h"
#include "krondor/sound.h"

namespace Krondor {

enum SoundFormat {
	SF_MIDI,
	SF_WAVE,
	SF_UNKNOWN
};

class Sound {
private:
	uint _type;
	uint _channel = 255;
	SoundFormat _format = SF_UNKNOWN;
	byte *_data = nullptr;
	size_t _size = 0;

public:
	Sound(uint type) : _type(type) {}
	virtual ~Sound();

	uint getType() const {
		return _type;
	}
	SoundFormat GetFormat() const {
		return _format;
	}
	byte *getData() const {
		return _data;
	}
	size_t getSize() const {
		return _size;
	}

	void load(Common::SeekableReadStream *src);
};

} // namespace Krondor

#endif
