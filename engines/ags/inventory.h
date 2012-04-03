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

#ifndef AGS_INVENTORY_H
#define AGS_INVENTORY_H

#include "common/hash-str.h"
#include "engines/ags/scriptobj.h"

namespace AGS {

struct InventoryItem : public ScriptObject {
	bool isOfType(ScriptObjectType objectType) { return (objectType == sotInventoryItem); }
	const char *getObjectTypeName() { return "InventoryItem"; }

	uint _id;

	Common::String _name;
	uint32 _pic;
	uint32 _cursorPic;
	uint32 _hotspotX, _hotspotY;
	uint8 _flags;

	Common::StringMap _properties;
	Common::String _scriptName;
};

} // End of namespace AGS

#endif // AGS_INVENTORY_H
