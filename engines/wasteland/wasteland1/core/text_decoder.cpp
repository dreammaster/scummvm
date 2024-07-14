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

#include "common/debug.h"
#include "common/endian.h"
#include "wasteland/wasteland1/core/text_decoder.h"

namespace Wasteland {
namespace Wasteland1 {

TextDecoder::TextDecoder(const byte *data) : _lookup(data) {
	int count = READ_LE_UINT16(_lookup + 60) / 2;
	const byte *nextPtr = nullptr;

	for (int i = 0; i < count; ++i) {
		_textPtr = data + 60 + READ_LE_UINT16(data + 60 + i * 2);
		nextPtr = data + 60 + ((i == (count - 1)) ? 999 :
			READ_LE_UINT16(data + 60 + (i + 1) * 2));
		_ctr = -1;
		_mask = 0;

		_text.push_back(Common::StringArray());
		while (_textPtr < (nextPtr - 1)) {
			Common::String s = getString();

			debugN("\"");
			for (uint idx = 0; idx < s.size(); ++idx) {
				if (s[idx] == '\r')
					debugN("\\r");
				else
					debugN("%c", s[idx]);
			}
			debugN("\"\n");

			_text.back().push_back(s);
		}

		debug("----");
	}
}

Common::String TextDecoder::getString() {
	Common::String result;
	char c;

	while ((c = getChar()) != '\0')
		result += c;

	return result;
}

int TextDecoder::getIndex() {
	byte result = 0;

	for (int bitCtr = 0; bitCtr < 5; ++bitCtr) {
		if (_ctr < 0) {
			// Move to next source byte
			_ctr = 7;
			_mask = *_textPtr++;
		}

		result = (result >> 1) | ((_mask & 1) ? 0x80 : 0);
		_mask >>= 1;

		--_ctr;
	}

	result >>= 3;
	return result;
}

char TextDecoder::getChar() {
	int index;
	bool capitalize = false;

	do {
		_capitalize = (_capitalize >> 1) | (capitalize ? 0x80 : 0);
		index = getIndex();
		capitalize = true;
	} while (index == 30);

	if (index == 31)
		index = 30 + getIndex();

	char c = (char)*(_lookup + index);

	_capitalize >>= 1;
	if (_capitalize & 0x40)
		c = toupper(c);

	return c;
}

} // namespace Wasteland1
} // namespace Wasteland
