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

#ifndef AGS_DYNAMICARRAY_H
#define AGS_DYNAMICARRAY_H

#include "scriptobj.h"
#include "script.h"

namespace AGS {

class ScriptDynamicArray : public ScriptObject {
public:
	virtual const char *getObjectTypeName() { return "ScriptDynamicArray"; }
	virtual bool isOfType(ScriptObjectType objectType) { return (objectType == sotDynamicArray); }
	ScriptDynamicArray(uint32 elementSize, uint32 elementCount, bool isManaged);
	virtual ~ScriptDynamicArray();

	virtual bool writeUint32(uint offset, uint value);
	virtual uint32 readUint32(uint offset);
	virtual uint16 readUint16(uint offset);
	virtual bool writeUint16(uint offset, uint16 value);
	virtual byte readByte(uint offset);
	virtual bool writeByte(uint offset, byte value);

	uint getMaxOffset() { return _elementSize * _elementCount; }

protected:
	bool _isManaged;
	uint32 _elementSize;
	uint32 _elementCount;
	Common::Array<RuntimeValue> _array;
};

} // End of namespace AGS

#endif // AGS_DYNAMICARRAY_H
