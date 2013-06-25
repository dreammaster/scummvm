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

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

/* Based on the Adventure Game Studio source code, copyright 1999-2011 Chris Jones,
 * which is licensed under the Artistic License 2.0.
 * You may also modify/distribute the code in this file under that license.
 */

#include "engines/ags/dynamicarray.h"

namespace AGS {

ScriptDynamicArray::ScriptDynamicArray(uint32 elementSize, uint32 elementCount, bool isManaged) {
	_elementCount = elementCount;
	_elementSize = elementSize;
	_isManaged = isManaged;

	_array.resize(elementCount);
}

ScriptDynamicArray::~ScriptDynamicArray() {
}

uint32 ScriptDynamicArray::readUint32(uint offset) {
	assert(!_isManaged);
	assert(_elementSize == 4);
	assert(offset % _elementSize == 0);
	uint32 objectId = offset / _elementSize;
	if (objectId >= _array.size())
		error("readUint32: offset %d is beyond end of dynamic array (%d) (size %d)", offset, _elementSize, _array.size());
	return _array[objectId]._value;
}

bool ScriptDynamicArray::writeUint32(uint offset, uint value) {
	assert(!_isManaged);
	assert(_elementSize == 4);
	assert(offset % _elementSize == 0);
	uint32 objectId = offset / _elementSize;
	if (objectId >= _array.size())
		error("writeUint32: offset %d is beyond end of dynamic array (%d) (size %d)", offset, _elementSize, _array.size());
	_array[objectId] = value;
	return true;
}

uint16 ScriptDynamicArray::readUint16(uint offset) {
	assert(!_isManaged);
	assert(_elementSize == 2);
	assert(offset % _elementSize == 0);
	uint32 objectId = offset / _elementSize;
	if (objectId >= _array.size())
		error("readUint16: offset %d is beyond end of dynamic array (%d) (size %d)", offset, _elementSize, _array.size());
	return _array[objectId]._value;
}

bool ScriptDynamicArray::writeUint16(uint offset, uint16 value) {
	assert(!_isManaged);
	assert(_elementSize == 2);
	assert(offset % _elementSize == 0);
	uint32 objectId = offset / _elementSize;
	if (objectId >= _array.size())
		error("writeUint16: offset %d is beyond end of dynamic array (%d) (size %d)", offset, _elementSize, _array.size());
	_array[objectId] = value;
	return true;
}

byte ScriptDynamicArray::readByte(uint offset) {
	assert(!_isManaged);
	assert(_elementSize == 1);
	uint32 objectId = offset;
	if (objectId >= _array.size())
		error("readByte: offset %d is beyond end of dynamic array (%d) (size %d)", offset, _elementSize, _array.size());
	return _array[objectId]._value;
}

bool ScriptDynamicArray::writeByte(uint offset, byte value) {
	assert(!_isManaged);
	assert(_elementSize == 1);
	uint32 objectId = offset;
	if (objectId >= _array.size())
		error("writeByte: offset %d is beyond end of dynamic array (%d) (size %d)", offset, _elementSize, _array.size());
	_array[objectId] = value;
	return true;
}

} // End of namespace AGS
