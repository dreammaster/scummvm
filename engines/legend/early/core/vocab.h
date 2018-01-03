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

#ifndef LEGEND_EARLY_CORE_VOCAB_H
#define LEGEND_EARLY_CORE_VOCAB_H

#include "common/array.h"
#include "common/str.h"

namespace Legend {
namespace Early {

class VocabEntry : public Common::String {
public:
	byte _flags;
	int _field6;
	byte _field8;
public:
	VocabEntry() : Common::String(), _flags(0), _field6(0), _field8(0) {}
	VocabEntry(const char *text, byte flags = 0) : Common::String(text),
		_flags(flags), _field6(0), _field8(0) {}
	VocabEntry(const Common::String &text, byte flags = 0) : Common::String(text),
		_flags(flags), _field6(0), _field8(0) {}
};

class Vocab : public Common::Array<VocabEntry> {
public:
	Vocab();
};

} // End of namespace Early
} // End of namespace Legend

#endif
