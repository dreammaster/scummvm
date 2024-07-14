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

#ifndef WASTELAND_WASTELAND1_CORE_TEXT_DECODER_H
#define WASTELAND_WASTELAND1_CORE_TEXT_DECODER_H

#include "common/str-array.h"

namespace Wasteland {
namespace Wasteland1 {

/**
 * Text data in Wasteland 1 is encoded using a lookup
 * table of characters, and 5 bit sequences giving indexes.
 * It currently suffers from a problem that I don't know when
 * the data for the final page ends, so I'm only using it
 * internally and manually stopping it at the right time to
 * get the text for the last page, and then copying and
 * pasting the decoded text directly into the source code.
 */
class TextDecoder {
private:
	Common::Array<Common::StringArray> _text;
	const byte *_lookup;
	const byte *_textPtr;
	byte _capitalize = 0;
	int _ctr = -1;
	byte _mask = 0;

	Common::String getString();
	int getIndex();
	char getChar();

public:
	TextDecoder(const byte *data);
};

} // namespace Wasteland1
} // namespace Wasteland

#endif
