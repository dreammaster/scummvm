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

#ifndef LEGEND_RESOURCES_H
#define LEGEND_RESOURCES_H

#include "common/str.h"
#include "common/str-array.h"
#include "legend/file.h"

namespace Legend {

class LegendEngine;

class TextMessage {
	friend class Resources;
private:
	static LegendEngine *_vm;
	uint _id;
	const char *_msg;
public:
	TextMessage(uint id);
	TextMessage(const char *msg);
	operator const char *() const;
};

struct TextIndexEntry {
public:
	int _count;
	size_t _size;
	size_t _indexOffset;
	size_t _dataOffset;
public:
	TextIndexEntry() : _count(0), _size(0), _indexOffset(0), _dataOffset(0) {}

	/**
	 * Load the data for an entry
	 */
	void load(Common::SeekableReadStream &f);
};

class Resources {
	friend class TextMessage;
	struct TextEntry {
		uint _id;
		Common::String _text;
	};
private:
	LegendEngine *_vm;
	Common::String _prefix;
	File _textFile;
	Common::Array<TextIndexEntry> _textList;
	Common::Array<int> _decryptionTable;
	Common::StringArray _wordList;
	Common::Array<TextEntry> _textCache;
	int _currentTextIndexNum;
	Common::Array<int> _currentTextIndexVals;
private:
	/**
	 * Load text for the game
	 */
	void loadText();

	/**
	 * Get the text message for a given Id
	 */
	const char *getMessage(uint id);

	/**
	 * Decompresses a single text message from a passed stream
	 */
	Common::String decompressText(Common::SeekableReadStream *stream);
public:
	Resources(LegendEngine *vm);
};

} // End of namespace Legend

#endif
