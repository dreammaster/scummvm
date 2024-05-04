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

#include "common/error.h"
#include "common/memstream.h"
#include "krondor/files/decompress.h"

namespace Krondor {

#define CODE_TABLE_SIZE 4096

DecompressRLE::DecompressRLE(Common::SeekableReadStream *src, int destSize) {
	_src = src;
	_decompressedSize = (destSize != -1) ? destSize : src->readUint32LE();
}

Common::SeekableReadStream *DecompressRLE::decompress() {
	Common::MemoryWriteStreamDynamic buf(DisposeAfterUse::NO);
	uint control, val, i;

	while (!_src->eos() && buf.size() < _decompressedSize) {
		control = _src->readByte();
		if (control & 0x80) {
			// Repeat following byte number of times
			val = _src->readByte();
			for (i = 0; i < (control & 0x7f); ++i)
				buf.writeByte(val);
		} else {
			// Copy a range of bytes
			for (i = 0; i < control; ++i)
				buf.writeByte(_src->readByte());
		}
	}

	return new Common::MemoryReadStream(buf.getData(), buf.size(), DisposeAfterUse::YES);
}


DecompressLZSS::DecompressLZSS(Common::SeekableReadStream *src, int destSize) {
	_src = src;
	_decompressedSize = (destSize != -1) ? destSize : src->readUint32LE();
}

Common::SeekableReadStream *DecompressLZSS::decompress() {
	uint8 code = 0, mask = 0;
	//Common::MemoryReadStream src(&_srcData[0], _srcData.size());
	Common::MemoryWriteStreamDynamic buf(DisposeAfterUse::NO);

	while (!_src->eos() && buf.size() < _decompressedSize) {
		if (!mask) {
			code = _src->readByte();
			mask = 0x01;
		}

		if (code & mask) {
			buf.writeByte(_src->readByte());

		} else {
			uint off = _src->readUint16LE();
			uint len = _src->readByte() + 5;

			buf.seek(off);
			for (uint i = 0; i < len; ++i)
				buf.writeByte(*(buf.getData() + off + i));
		}

		mask <<= 1;
	}

	return new Common::MemoryReadStream(buf.getData(), buf.size(), DisposeAfterUse::YES);
}


DecompressLZW::DecompressLZW(Common::SeekableReadStream *src, int destSize) {
	_src = src;
	_decompressedSize = (destSize != -1) ? destSize :
		src->readUint32LE();
}

Common::SeekableReadStream *DecompressLZW::decompress() {
	Common::Array<CodeTableEntry> codeTable(CODE_TABLE_SIZE);
	Common::Array<uint8> decodeStack(CODE_TABLE_SIZE);
	uint8 *stackPtr = &decodeStack[0];
	uint n_bits = 9;
	uint freeEntry = 257;
	uint bitPos = 0;

	_nextBit = 0;
	_currentByte = _src->readByte();
	uint oldcode = getBits(n_bits);
	uint lastbyte = oldcode;

	Common::MemoryWriteStreamDynamic buf(DisposeAfterUse::NO);
	buf.writeByte(oldcode);

	while (!_src->eos() && buf.size() < _decompressedSize) {
		uint newcode = getBits(n_bits);
		bitPos += n_bits;
		if (newcode == 256) {
			skipBits();

			int bytesToSkip = (((bitPos - 1) + ((n_bits << 3) - (bitPos - 1 + (n_bits << 3)) % (n_bits << 3))) - bitPos) >> 3;
			_src->skip(bytesToSkip - 1);
			_currentByte = _src->readByte();

			n_bits = 9;
			freeEntry = 256;
			bitPos = 0;
		} else {
			uint code = newcode;
			if (code >= freeEntry) {
				*stackPtr++ = lastbyte;
				code = oldcode;
			}
			while (code >= 256) {
				assert(code < CODE_TABLE_SIZE);
				*stackPtr++ = codeTable[code]._append;
				code = codeTable[code]._prefix;
			}

			*stackPtr++ = code;
			lastbyte = code;

			while (stackPtr > &decodeStack[0]) {
				buf.writeByte(*--stackPtr);
			}

			if (freeEntry < CODE_TABLE_SIZE) {
				codeTable[freeEntry]._prefix = oldcode;
				codeTable[freeEntry]._append = lastbyte;
				freeEntry++;

				if ((freeEntry >= (unsigned int)(1 << n_bits)) && (n_bits < 12)) {
					n_bits++;
					bitPos = 0;
				}
			}

			oldcode = newcode;
		}
	}

	return new Common::MemoryReadStream(buf.getData(), buf.size(), DisposeAfterUse::YES);
}

uint DecompressLZW::getBits(uint numBits) {
	uint x = 0;
	for (uint i = 0; i < numBits; i++) {
		if (_currentByte & (1 << _nextBit)) {
			x += (1 << i);
		}

		_nextBit++;
		if (_nextBit > 7) {
			_currentByte = _src->readByte();
			_nextBit = 0;
		}
	}

	return x;
}

void DecompressLZW::skipBits() {
	if (_nextBit) {
		_currentByte = _src->readByte();
		_nextBit = 0;
	}
}

} // namespace Krondor
