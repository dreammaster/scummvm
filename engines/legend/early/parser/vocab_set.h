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

#ifndef LEGEND_EARLY_PARSER_VOCAB_SET_H
#define LEGEND_EARLY_PARSER_VOCAB_SET_H

#include "legend/utils/string.h"

namespace Legend {
namespace Early {
namespace Parser {

struct VocabSet {
	int _prefixVocabId;
	int _val2;
	int _vocabCount;
	uint16 _vocabIds[16];
	int _suffixVocabId;

	VocabSet() {
		clear();
	}
	void clear();

	/**
	 * Add to text window
	 */
	void display() const;

	/**
	 * Asks the player to clarify which item they mean
	 */
	void whichDoYouMean(const void *items) const;
};

} // namespace Parser
} // namespace Early
} // namespace Legend

#endif
