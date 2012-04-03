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

#ifndef AGS_SCRIPTOBJ_H
#define AGS_SCRIPTOBJ_H

#include "common/textconsole.h"
#include "common/array.h"
#include "common/str.h"

namespace AGS {

enum ScriptObjectType {
	sotNone = 0,
	sotAudioChannel,
	sotAudioClip,
	sotCharacter,
	sotDateTime,
	sotDialog,
	sotDialogOptionsRenderingInfo,
	sotDrawingSurface,
	sotDynamicSprite,
	sotFile,
	sotGUI,
	sotGUIButton,
	sotGUIControl,
	sotGUIInvWindow,
	sotGUILabel,
	sotGUIListBox,
	sotGUISlider,
	sotGUITextBox,
	sotHotspot,
	sotInventoryItem,
	sotOverlay,
	sotRegion,
	sotRoomObject,
	sotString,
	sotViewFrame
};

class ScriptObject {
public:
	ScriptObject() : _refCount(1) { }
	virtual ~ScriptObject() { }

	// reference counting
	void IncRef() { _refCount++; }
	void DecRef() {
		_refCount--;
		if (_refCount == 0)
			delete this;
	}
	uint32 getRefCount() { return _refCount; }

	virtual uint32 readUint32(uint offset) { error("tried reading uint32 from offset %d on a %s", offset, getObjectTypeName()); }
	virtual bool writeUint32(uint offset, uint value) { return false; }
	virtual uint16 readUint16(uint offset) { error("tried reading uint16 from offset %d on a %s", offset, getObjectTypeName()); }
	virtual bool writeUint16(uint offset, uint16 value) { return false; }
	virtual byte readByte(uint offset) { error("tried reading byte from offset %d on a %s", offset, getObjectTypeName()); }
	virtual bool writeByte(uint offset, byte value) { return false; }

	// for resolving pointer arithmetic by scripts
	virtual ScriptObject *getObjectAt(uint32 &offset) { return this; }

	virtual bool isOfType(ScriptObjectType objectType) { return false; }
	virtual const char *getObjectTypeName() = 0;

protected:
	uint32 _refCount;
};

class ScriptString : public ScriptObject {
public:
	virtual const Common::String getString() = 0;
	virtual void setString(const Common::String &string) = 0;

	virtual bool isOfType(ScriptObjectType objectType) { return (objectType == sotString); }
	const char *getObjectTypeName() { return "ScriptString"; }
};

class ScriptConstString : public ScriptString {
public:
	ScriptConstString(const Common::String &string) : _string(string) { }

	const Common::String getString() { return _string; }
	void setString(const Common::String &string) { error("tried to set a ScriptConstString"); }

protected:
	Common::String _string;
};

class ScriptMutableString : public ScriptString {
public:
	ScriptMutableString(const Common::String &string) : _string(string) { }

	const Common::String getString() { return _string; }
	void setString(const Common::String &string) { _string = string; }

protected:
	Common::String _string;
};

// array of (system) script objects; for characters[], gui[], etc
template<class T> class ScriptObjectArray : public ScriptObject {
public:
	ScriptObjectArray(Common::Array<T> *array, uint32 elementSize, const char *objName)
		: _array(array), _elementSize(elementSize), _objName(objName) { }
	void setArray(Common::Array<T> *array) { _array = array; }
	virtual ScriptObject *getObjectAt(uint32 &offset) {
		uint32 objectId = offset / _elementSize;
		if (objectId >= _array->size())
			return NULL;
		offset = offset % _elementSize;
		return &(*_array)[objectId];
	}
	virtual uint32 readUint32(uint offset) {
		uint32 objectId = offset / _elementSize;
		if (objectId >= _array->size())
			error("readUint32: offset %d is beyond end of array of %s (size %d)", offset, _objName, _array->size());
		return (*_array)[objectId].readUint32(offset % _elementSize);
	}
	virtual bool writeUint32(uint offset, uint value) {
		uint32 objectId = offset / _elementSize;
		if (objectId >= _array->size())
			error("writeUint32: offset %d is beyond end of array of %s (size %d)", offset, _objName, _array->size());
		return (*_array)[objectId].writeUint32(offset % _elementSize, value);
	}
	virtual uint16 readUint16(uint offset) {
		uint16 objectId = offset / _elementSize;
		if (objectId >= _array->size())
			error("readUint16: offset %d is beyond end of array of %s (size %d)", offset, _objName, _array->size());
		return (*_array)[objectId].readUint16(offset % _elementSize);
	}
	virtual bool writeUint16(uint offset, uint16 value) {
		uint16 objectId = offset / _elementSize;
		if (objectId >= _array->size())
			error("writeUint16: offset %d is beyond end of array of %s (size %d)", offset, _objName, _array->size());
		return (*_array)[objectId].writeUint16(offset % _elementSize, value);
	}
	virtual byte readByte(uint offset) {
		byte objectId = offset / _elementSize;
		if (objectId >= _array->size())
			error("readUint16: offset %d is beyond end of array of %s (size %d)", offset, _objName, _array->size());
		return (*_array)[objectId].readByte(offset % _elementSize);
	}
	virtual bool writeByte(uint offset, byte value) {
		byte objectId = offset / _elementSize;
		if (objectId >= _array->size())
			error("writeByte: offset %d is beyond end of array of %s (size %d)", offset, _objName, _array->size());
		return (*_array)[objectId].writeByte(offset % _elementSize, value);
	}

	const char *getObjectTypeName() { return "ScriptObjectArray"; }

protected:
	uint32 _elementSize;
	const char *_objName;
	Common::Array<T> *_array;
};

// specialization of above for arrays containing pointers
template<class T> class ScriptObjectArray<T *> : public ScriptObject {
public:
	ScriptObjectArray(Common::Array<T *> *array, uint32 elementSize, const char *objName)
		: _array(array), _elementSize(elementSize), _objName(objName) { }
	void setArray(Common::Array<T *> *array) { _array = array; }
	virtual ScriptObject *getObjectAt(uint32 &offset) {
		uint32 objectId = offset / _elementSize;
		if (objectId >= _array->size())
			return NULL;
		offset = offset % _elementSize;
		return (*_array)[objectId];
	}
	virtual uint32 readUint32(uint offset) {
		uint32 objectId = offset / _elementSize;
		if (objectId >= _array->size())
			error("readUint32: offset %d is beyond end of array of %s (size %d)", offset, _objName, _array->size());
		return (*_array)[objectId]->readUint32(offset % _elementSize);
	}
	virtual bool writeUint32(uint offset, uint value) {
		uint32 objectId = offset / _elementSize;
		if (objectId >= _array->size())
			error("writeUint32: offset %d is beyond end of array of %s (size %d)", offset, _objName, _array->size());
		return (*_array)[objectId]->writeUint32(offset % _elementSize, value);
	}
	virtual uint16 readUint16(uint offset) {
		uint16 objectId = offset / _elementSize;
		if (objectId >= _array->size())
			error("readUint16: offset %d is beyond end of array of %s (size %d)", offset, _objName, _array->size());
		return (*_array)[objectId]->readUint16(offset % _elementSize);
	}
	virtual bool writeUint16(uint offset, uint16 value) {
		uint16 objectId = offset / _elementSize;
		if (objectId >= _array->size())
			error("writeUint16: offset %d is beyond end of array of %s (size %d)", offset, _objName, _array->size());
		return (*_array)[objectId]->writeUint16(offset % _elementSize, value);
	}
	virtual byte readByte(uint offset) {
		byte objectId = offset / _elementSize;
		if (objectId >= _array->size())
			error("readUint16: offset %d is beyond end of array of %s (size %d)", offset, _objName, _array->size());
		return (*_array)[objectId]->readByte(offset % _elementSize);
	}
	virtual bool writeByte(uint offset, byte value) {
		byte objectId = offset / _elementSize;
		if (objectId >= _array->size())
			error("writeByte: offset %d is beyond end of array of %s (size %d)", offset, _objName, _array->size());
		return (*_array)[objectId]->writeByte(offset % _elementSize, value);
	}

	const char *getObjectTypeName() { return "ScriptObjectArray<*>"; }

protected:
	uint32 _elementSize;
	const char *_objName;
	Common::Array<T *> *_array;
};

} // End of namespace AGS

#endif // AGS_SCRIPTOBJ_H
