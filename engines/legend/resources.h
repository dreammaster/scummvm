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

#include "common/file.h"
#include "common/str.h"
#include "common/str-array.h"

namespace Legend {

enum FileType {
	FILETYPE_PIC = 0,
	FILETYPE_RGN = 1,
	FILETYPE_FNT = 2,
	FILETYPE_MUS = 3,
	FILETYPE_SAV = 4,
	FILETYPE_SAV2 = 5,
	FILETYPE_VOC = 6,
	FILETYPE_RS = 7
};

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
	Common::File _textFile;
	Common::Array<TextIndexEntry> _textList;
	Common::Array<uint> _textData2;
	Common::StringArray _wordList;
	size_t _textData5Offset;
	Common::Array<byte> _textData5;
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

	/**
	 * Form a filename for a specified file type and number
	 */
	Common::String getFilename(FileType fileType, int fileNumber);
};

} // End of namespace Legend

#endif
