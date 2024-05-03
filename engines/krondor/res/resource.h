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

#ifndef KRONDOR_RES_RESOURCE_H
#define KRONDOR_RES_RESOURCE_H

#include "common/hashmap.h"
#include "common/serializer.h"

namespace Krondor {

enum {
	TAG_ADS = 0x3a534441,
	TAG_APP = 0x3a505041,
	TAG_BIN = 0x3a4e4942,
	TAG_BMP = 0x3a504d42,
	TAG_DAT = 0x3a544144,
	TAG_FNT = 0x3a544e46,
	TAG_GID = 0x3a444947,
	TAG_INF = 0x3a464e49,
	TAG_MAP = 0x3a50414d,
	TAG_PAG = 0x3a474150,
	TAG_PAL = 0x3a4c4150,
	TAG_RES = 0x3a534552,
	TAG_SCR = 0x3a524353,
	TAG_SND = 0x3a444e53,
	TAG_TAG = 0x3a474154,
	TAG_TT3 = 0x3a335454,
	TAG_TTI = 0x3a495454,
	TAG_VER = 0x3a524556,
	TAG_VGA = 0x3a414756
};


class Resource {
protected:
	virtual void readIndex(Common::SeekableReadStream *src) {}
	virtual void read(Common::SeekableReadStream *src) = 0;

public:
	Resource() {}
	virtual ~Resource() {}
	void load(const Common::String &name);

	virtual void clear() {}
};

class TaggedResource : public Resource {
	/**
	 * Wrapper for a sub-section of the source file
	 */
	class SubStream : public Common::SeekableReadStream {
	private:
		Common::SeekableReadStream *_src;
		uint _startPos, _endPos;
	public:
		SubStream(Common::SeekableReadStream *src, uint startPos, uint endPos) :
			_src(src), _startPos(startPos), _endPos(endPos) {
		}

		int64 pos() const override {
			return _src->pos();
		}
		int64 size() const override {
			return _endPos;
		}
		bool seek(int64 offset, int whence = SEEK_SET) override {
			return _src->seek(offset, whence);
		}
		bool eos() const override {
			return pos() >= _endPos;
		}
		uint32 read(void *dataPtr, uint32 dataSize) override {
			return _src->read(dataPtr, dataSize);
		}
	};

	struct TagEntry {
		uint _offset = 0, _size = 0;
		TagEntry() {}
		TagEntry(uint offset, uint size) : _offset(offset), _size(size) {}
	};
private:
	Common::SeekableReadStream *_file = nullptr;
	Common::HashMap<uint32, TagEntry> _tags;

protected:
	/**
	 * Returns a sub-stream for the tag we're interested in
	 */
	Common::SeekableReadStream *getTag(uint32 tag);

	void readIndex(Common::SeekableReadStream *src) override;

public:
	TaggedResource() : Resource() {}
	~TaggedResource() override {}
};

} // namespace Krondor

#endif
