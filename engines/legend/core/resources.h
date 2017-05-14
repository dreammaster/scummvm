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

#include "common/hashmap.h"
#include "legend/core/file.h"
#include "legend/core/string.h"

namespace Legend {

class LegendEngine;

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
	struct ResourceEntry {
		uint _offset;
		uint _size;

		ResourceEntry() : _offset(0), _size(0) {}
		ResourceEntry(uint offset, uint size) : _offset(offset), _size(size) {}
	};
	typedef Common::HashMap<Common::String, ResourceEntry> ResourceHash;

	struct TextEntry {
		uint _id;
		String _text;
	};
private:
	ResourceHash _resources;
	Common::File _datFile;
	File _textFile;
	Common::Array<TextIndexEntry> _textList;
	Common::Array<int> _decryptionTable;
	StringArray _wordList;
	Common::Array<TextEntry> _textCache;
	int _currentTextIndexNum;
	Common::Array<int> _currentTextIndexVals;
private:
	/**
	 * Load the index of the legend.dat file
	 */
	void loadResourceIndex();

	/**
	 * Load text for the game
	 */
	void loadText();

	/**
	 * Decompresses a single text message from a passed stream
	 */
	String decompressText(Common::SeekableReadStream *stream);
public:
	Resources(LegendEngine *vm);

	/**
	* Get a resource from the legend.dat
	*/
	Common::SeekableReadStream *getResource(const String &name);

	/**
	 * Get the text message for a given Id
	 */
	Common::String getMessage(uint id);
};

} // End of namespace Legend

#endif
