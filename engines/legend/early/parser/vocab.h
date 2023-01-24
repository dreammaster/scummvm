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
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#ifndef LEGEND_EARLY_PARSER_VOCAB_H
#define LEGEND_EARLY_PARSER_VOCAB_H

#include "common/array.h"
#include "legend/utils/string.h"

namespace Legend {
namespace Early {
namespace Parser {

enum VocabFlag {
	VFLAG_1 = 1, VFLAG_2 = 2, VFLAG_4 = 4, VFLAG_8 = 8,
	VFLAG_10 = 0x10, VFLAG_20 = 0x20
};

class VocabEntry : public Common::String {
public:
	byte _flags;
	int _altVocabId;
	int _logicNum;
public:
	VocabEntry() : Common::String(), _flags(0), _altVocabId(0), _logicNum(0) {
	}
	VocabEntry(const char *text, byte flags = 0) : Common::String(text),
		_flags(flags), _altVocabId(0), _logicNum(0) {
	}
	VocabEntry(const Common::String &text, byte flags = 0) : Common::String(text),
		_flags(flags), _altVocabId(0), _logicNum(0) {
	}
};

class Vocab {
private:
	Common::Array<VocabEntry> _items;
public:
	Vocab();

	/**
	 * Get the index of a vocab entry
	 */
	int indexOf(const String &word) const;

	/**
	 * Return the vocab size
	 */
	size_t size() const {
		return _items.size();
	}

	/**
	 * Get a vocab entry
	 */
	const VocabEntry &operator[](uint vocabId) const {
		assert(vocabId != 0 && vocabId <= _items.size());
		return _items[vocabId - 1];
	}
};

} // namespace Parser
} // namespace Early
} // namespace Legend

#endif
