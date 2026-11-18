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
#include "common/endian.h"
#include "ultima/ultima5/data/lzw.h"

namespace Ultima {
namespace Ultima5 {
namespace Data {

constexpr int LZW_CLEAR = 256;
constexpr int LZW_EOI = 257;
constexpr int LZW_FIRST = 258;
constexpr int LZW_MAX = 4096;
constexpr int LZW_MAX_BITS = 12;

// Expands a code onto the stack, last byte first, and returns its first byte
static byte expandCode(int code, const uint16 *prefix, const byte *suffix, byte *stack, int &sp) {
	while (code >= 256) {
		stack[sp++] = suffix[code];
		code = prefix[code];
	}

	stack[sp++] = (byte)code;
	return (byte)code;
}

bool decompressLzw(const byte *src, size_t srcSize, Common::Array<byte> &dest) {
	dest.clear();
	if (srcSize < 4)
		return false;

	const uint32 outLen = READ_LE_UINT32(src);
	src += 4;
	srcSize -= 4;
	dest.reserve(outLen);

	uint16 prefix[LZW_MAX];
	byte suffix[LZW_MAX];
	byte stack[LZW_MAX];
	uint32 bitBuffer = 0;
	int bits = 0;
	size_t pos = 0;

	int codeSize = 9;
	int nextCode = LZW_FIRST;
	int prevCode = -1;

	while (dest.size() < outLen) {
		while (bits < codeSize && pos < srcSize) {
			bitBuffer |= (uint32)src[pos++] << bits;
			bits += 8;
		}
		if (bits < codeSize)
			break;

		const int code = bitBuffer & ((1 << codeSize) - 1);
		bitBuffer >>= codeSize;
		bits -= codeSize;

		if (code == LZW_CLEAR) {
			codeSize = 9;
			nextCode = LZW_FIRST;
			prevCode = -1;
			continue;
		}
		if (code == LZW_EOI)
			break;

		int sp = 0;
		byte first;
		if (code < nextCode) {
			first = expandCode(code, prefix, suffix, stack, sp);
			while (sp > 0 && dest.size() < outLen)
				dest.push_back(stack[--sp]);
		} else if (code == nextCode && prevCode >= 0) {
			first = expandCode(prevCode, prefix, suffix, stack, sp);
			while (sp > 0 && dest.size() < outLen)
				dest.push_back(stack[--sp]);
			if (dest.size() < outLen)
				dest.push_back(first);
		} else {
			return false;
		}

		if (prevCode >= 0 && nextCode < LZW_MAX) {
			prefix[nextCode] = prevCode;
			suffix[nextCode] = first;
			++nextCode;

			if (codeSize < LZW_MAX_BITS && nextCode == (1 << codeSize))
				++codeSize;
		}

		prevCode = code;
	}

	return dest.size() == outLen;
}

bool loadLzwFile(const char *name, Common::Array<byte> &dest) {
	Common::File f;
	if (!f.open(name))
		return false;

	Common::Array<byte> data;
	data.resize(f.size());
	if (f.read(&data[0], data.size()) != data.size())
		return false;

	return decompressLzw(&data[0], data.size(), dest);
}

} // namespace Data
} // namespace Ultima5
} // namespace Ultima
