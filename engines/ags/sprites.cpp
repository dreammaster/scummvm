/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

/* Based on the Adventure Game Studio source code, copyright 1999-2011 Chris Jones,
 * which is licensed under the Artistic License 2.0.
 */

#include "common/debug.h"

#include "ags/ags.h"
#include "ags/sprites.h"

#include "graphics/surface.h"

namespace AGS {

Sprite::~Sprite() {
	_surface->free();
	delete _surface;
}

const char *kSpriteFileSignature = " Sprite File ";
const char *kSpriteIndexFilename = "sprindex.dat";
const char *kSpriteIndexSignature = "SPRINDEX";

SpriteSet::SpriteSet(AGSEngine *vm, Common::SeekableReadStream *stream) : _vm(vm), _stream(stream) {
	uint16 version = _stream->readUint16LE();

	char signature[13 + 1];
	_stream->read(signature, 13);
	signature[13] = '\0';
	if (memcmp(kSpriteFileSignature, signature, 13) != 0)
		error("bad sprite file signature ('%s')", signature);

	uint32 spriteFileID = 0;
	switch (version) {
	case 4:
		_spritesAreCompressed = false;
		break;
	case 5:
		_spritesAreCompressed = true;
		break;
	case 6:
		_spritesAreCompressed = (bool)_stream->readByte();
		spriteFileID = _stream->readUint32LE();
		break;
	default:
		error("unsupported sprite file version %d", version);
	}

	if (version < 5) {
		_stream->skip(256 * 3); // palette
	}

	uint16 spriteCount = _stream->readUint16LE();
	if (version < 4)
		spriteCount = 200;
	_spriteInfo.resize(spriteCount + 1);

	debug(2, "sprite set has %d sprites (version %d, %s)", spriteCount, version, _spritesAreCompressed ? "compressed" : "uncompressed");

	// try and load the sprite index file first
	if (loadSpriteIndexFile(spriteFileID))
		return;

	// no sprite index file, manually index the sprites
	for (uint i = 0; i < spriteCount; ++i) {
		SpriteInfo &info = _spriteInfo[i];
		info._offset = _stream->pos();
		uint16 colorDepth = _stream->readUint16LE();
		if (!colorDepth) {
			// FIXME: invalid sprite
			continue;
		}
		info._width = _stream->readUint16LE();
		info._height = _stream->readUint16LE();

		uint32 dataSize = info._width * info._height * colorDepth;
		if (version == 5)
			dataSize = _stream->readUint32LE();
		_stream->skip(dataSize);
	}

	if (_stream->eos())
		error("failed to read sprite file");
}

SpriteSet::~SpriteSet() {
	delete _stream;

	for (Common::HashMap<uint, Sprite *>::iterator i = _sprites.begin(); i != _sprites.end(); ++i)
		delete i->_value;
}

bool SpriteSet::loadSpriteIndexFile(uint32 spriteFileID) {
	Common::SeekableReadStream *idxStream = _vm->getFile(kSpriteIndexFilename);
	if (!idxStream)
		return false;

	char signature[8 + 1];
	idxStream->read(signature, 8);
	signature[8] = '\0';
	if (memcmp(kSpriteIndexSignature, signature, 8) != 0) {
		warning("bad sprite index file signature ('%s')", signature);
		delete idxStream;
		return false;
	}

	uint32 version = idxStream->readUint32LE();
	if (version < 1 || version > 2) {
		warning("unsupported sprite index file version %d", version);
		delete idxStream;
		return false;
	}

	if (version >= 2) {
		uint32 fileID = idxStream->readUint32LE();
		if (fileID != spriteFileID) {
			delete idxStream;
			return false;
		}
	}

	uint32 spriteCount = idxStream->readUint32LE() + 1;
	uint32 lastIndex = idxStream->readUint32LE();

	// sprite count should match, and last index should be one beyond number of sprites
	if ((spriteCount != _spriteInfo.size()) || (lastIndex != spriteCount)) {
		warning("inconsistent sprite index file (%d sprites, should be %d, last index %d)",
			spriteCount, _spriteInfo.size(), lastIndex);
		delete idxStream;
		return false;
	}

	debug(3, "reading sprite information from index file");

	for (uint i = 0; i < spriteCount; ++i)
		_spriteInfo[i]._width = idxStream->readUint16LE();
	for (uint i = 0; i < spriteCount; ++i)
		_spriteInfo[i]._height = idxStream->readUint16LE();
	for (uint i = 0; i < spriteCount; ++i) {
		_spriteInfo[i]._offset = idxStream->readUint32LE();
		if (_spriteInfo[i]._offset >= (uint32)_stream->size())
			error("corrupt sprite index file (item %d has offset %x of %x", i, _spriteInfo[i]._offset, _stream->size());
	}

	delete idxStream;
	return true;
}

Sprite *SpriteSet::getSprite(uint32 spriteId) {
	if (spriteId >= _spriteInfo.size())
		error("SpriteSet::getSprite: sprite id %d is too high", spriteId);

	// convert non-existant sprites to the big blue cup
	if (_spriteInfo[spriteId]._offset == 0)
		spriteId = 0;

	if (_sprites.contains(spriteId))
		return _sprites[spriteId];

	_stream->seek(_spriteInfo[spriteId]._offset);
	uint16 colorDepth = _stream->readUint16LE();

	Graphics::PixelFormat format;
	switch (colorDepth) {
	case 0:
		return NULL; // FIXME
	case 1:
		format = Graphics::PixelFormat::createFormatCLUT8();
		break;
	case 2:
		format = Graphics::PixelFormat(2, 5, 6, 5, 0, 11, 5, 0, 0);
		break;
	case 4:
		format = Graphics::PixelFormat(4, 8, 8, 8, 8, 24, 16, 8, 0);
		break;
	default:
		error("SpriteSet::getSprite: invalid sprite depth %d", colorDepth);
	}

	_spriteInfo[spriteId]._width = _stream->readUint16LE();
	_spriteInfo[spriteId]._height = _stream->readUint16LE();

	Graphics::Surface *surface = new Graphics::Surface;
	surface->create(_spriteInfo[spriteId]._width, _spriteInfo[spriteId]._height, format);

	if (_spritesAreCompressed) {
		switch (colorDepth) {
		case 1:
			_stream->skip(2); // data size, FIXME: use this
			unpackSpriteBits(_stream, (byte *)surface->getPixels(), surface->w * surface->h);
			break;
		case 2:
			unpackSpriteBits16(_stream, (uint16 *)surface->getPixels(), surface->w * surface->h);
			break;
		case 4:
			unpackSpriteBits32(_stream, (uint32 *)surface->getPixels(), surface->w * surface->h);
			break;
		}
	} else {
		_stream->read((byte *)surface->getPixels(), surface->w * surface->h * colorDepth);
	}

	// FIXME

	Sprite *sprite = new Sprite(spriteId, surface);
	_sprites[spriteId] = sprite;
	return sprite;
}

void unpackSpriteBits(Common::SeekableReadStream *stream, byte *dest, uint32 size) {
	uint32 offset = 0;

	while (!stream->eos() && (offset < size)) {
		signed char n = (signed char)stream->readByte();

		if (n == -128)
			n = 0;

		if (n < 0) {
			// run of a single byte
			uint32 count = 1 - n;
			byte data = stream->readByte();
			while (count-- && (offset < size)) {
				dest[offset++] = data;
			}
		} else {
			// run of non-encoded bytes
			uint32 count = 1 + n;
			while (count-- && (offset < size)) {
				dest[offset++] = stream->readByte();
			}
		}
	}
}

void unpackSpriteBits16(Common::SeekableReadStream *stream, uint16 *dest, uint32 size) {
	uint32 offset = 0;

	while (!stream->eos() && (offset < size)) {
		signed char n = (signed char)stream->readByte();

		if (n == -128)
			n = 0;

		if (n < 0) {
			// run of a single pixel
			uint32 count = 1 - n;
			uint16 data = stream->readUint16LE();
			while (count-- && (offset < size)) {
				dest[offset++] = data;
			}
		} else {
			// run of non-encoded pixels
			uint32 count = 1 + n;
			while (count-- && (offset < size)) {
				dest[offset++] = stream->readUint16LE();
			}
		}
	}
}

void unpackSpriteBits32(Common::SeekableReadStream *stream, uint32 *dest, uint32 size) {
	uint32 offset = 0;

	while (!stream->eos() && (offset < size)) {
		signed char n = (signed char)stream->readByte();

		if (n == -128)
			n = 0;

		if (n < 0) {
			// run of a single pixel
			uint32 count = 1 - n;
			uint32 data = stream->readUint32LE();
			while (count-- && (offset < size)) {
				dest[offset++] = data;
			}
		} else {
			// run of non-encoded pixels
			uint32 count = 1 + n;
			while (count-- && (offset < size)) {
				dest[offset++] = stream->readUint32LE();
			}
		}
	}
}



} // End of namespace AGS
