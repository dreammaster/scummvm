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
 */

#include "engines/ags/constants.h"
#include "engines/ags/room.h"
#include "engines/ags/util.h"

namespace AGS {

#define ROOM_FILE_VERSION kAGSRoomVer303x // 29

#define MAXOBJNAMELEN 30

#define BLOCKTYPE_MAIN        1
#define BLOCKTYPE_SCRIPT      2
#define BLOCKTYPE_COMPSCRIPT  3
#define BLOCKTYPE_COMPSCRIPT2 4
#define BLOCKTYPE_OBJECTNAMES 5
#define BLOCKTYPE_ANIMBKGRND  6
#define BLOCKTYPE_COMPSCRIPT3 7     // new CSCOMP script instead of SeeR
#define BLOCKTYPE_PROPERTIES  8
#define BLOCKTYPE_OBJECTSCRIPTNAMES 9
#define BLOCKTYPE_EOF         0xff

Room::Room(Common::SeekableReadStream *dta) {
	// FIXME: num_bscenes = 1
	// FIXME: bscene_anim_speed = 5

	_version = dta->readUint16LE();

	if (_version < kAGSRoomVer241)
		error("room version %d is too old", _version);
	else if (_version > kAGSRoomVer303x)
		error("room version %d is too new", _version);

	bool seenMain = false;

	while (true) {
		if (dta->eos())
			error("ran out of data while reading room");

		byte blockType = dta->readByte();
		if (blockType == BLOCKTYPE_EOF)
			break;

		uint32 blockSize = dta->readUint32LE();
		uint32 blockEnd = dta->pos() + blockSize;

		if (blockType != BLOCKTYPE_MAIN && !seenMain)
			error("Room: expected main block, but got block type %d", blockType);

		byte count;
		switch (blockType) {
		case BLOCKTYPE_MAIN:
			if (seenMain)
				error("Room: second main block encountered");
			seenMain = true;
			readMainBlock(dta);
			break;
		case BLOCKTYPE_SCRIPT:
			error("SCRIPT");
			// FIXME
			break;
		case BLOCKTYPE_COMPSCRIPT:
			// TODO
			warning("Room: ignoring old (COMPSCRIPT) compiled script");
			dta->skip(blockSize);
			break;
		case BLOCKTYPE_COMPSCRIPT2:
			// TODO
			warning("Room: ignoring old (COMPSCRIPT2) compiled script");
			dta->skip(blockSize);
			break;
		case BLOCKTYPE_COMPSCRIPT3:
			error("COMPSCRIPT3");
			// FIXME
			break;
		case BLOCKTYPE_OBJECTNAMES:
			count = dta->readByte();
			if (count != _objects.size())
				error("Room: got %d object names for %d objects", count, _objects.size());
			for (uint i = 0; i < count; ++i) {
				char objectName[MAXOBJNAMELEN + 1];
				dta->read(objectName, MAXOBJNAMELEN);
				objectName[MAXOBJNAMELEN] = '\0';
				_objects[i]._name = objectName;
			}
			break;
		case BLOCKTYPE_OBJECTSCRIPTNAMES:
			count = dta->readByte();
			if (count != _objects.size())
				error("Room: got %d object script names for %d objects", count, _objects.size());
			for (uint i = 0; i < count; ++i) {
				char objectName[MAX_SCRIPT_NAME_LEN + 1];
				dta->read(objectName, MAX_SCRIPT_NAME_LEN);
				objectName[MAX_SCRIPT_NAME_LEN] = '\0';
				_objects[i]._scriptName = objectName;
			}
			break;
		case BLOCKTYPE_ANIMBKGRND:
			error("ANIMBKGRND");
			// FIXME
			break;
		case BLOCKTYPE_PROPERTIES:
			error("PROPERTIES");
			// FIXME
			break;
		default:
			error("Room: unknown block type %d", blockType);
		}

		if ((uint32)dta->pos() != blockEnd)
			error("Room: didn't read correct amount from block type %d (at %d, block end at %d)",
				blockType, dta->pos(), blockEnd);
	}

	// FIXME
}

void Room::readMainBlock(Common::SeekableReadStream *dta) {
	_bytesPerPixel = 1;
	if (_version >= kAGSRoomVer208)
		_bytesPerPixel = dta->readUint32LE();
	if (_bytesPerPixel < 1)
		_bytesPerPixel = 1;

	// walk-behind base lines
	uint16 walkBehindsCount = dta->readUint16LE();
	if (walkBehindsCount > 16)
		error("Room: too many walk-behinds (%d)", walkBehindsCount);
	_walkBehindBaselines.resize(walkBehindsCount);
	for (uint i = 0; i < _walkBehindBaselines.size(); ++i)
		_walkBehindBaselines[i] = dta->readUint16LE();

	// hotspots
	uint32 hotspotCount = dta->readUint32LE();
	if (!hotspotCount)
		hotspotCount = 20;
	if (hotspotCount > 50) // MAX_HOTSPOTS: v2.62 increased from 20 to 30; v2.8 to 50
		error("Room: too many hotspots (%d)", hotspotCount);
	_hotspots.resize(hotspotCount);
	for (uint i = 0; i < _hotspots.size(); ++i) {
		_hotspots[i]._walkToPos.x = dta->readSint16LE();
		_hotspots[i]._walkToPos.y = dta->readSint16LE();
	}
	for (uint i = 0; i < _hotspots.size(); ++i) {
		if (_version >= kAGSRoomVer303) {
			_hotspots[i]._name = readString(dta);
		} else {
			char hotspotName[30 + 1];
			dta->read(hotspotName, 30);
			hotspotName[30] = '\0';
			_hotspots[i]._name = hotspotName;
		}
	}
	if (_version >= kAGSRoomVer270) {
		for (uint i = 0; i < _hotspots.size(); ++i) {
			char hotspotName[MAX_SCRIPT_NAME_LEN + 1];
			dta->read(hotspotName, MAX_SCRIPT_NAME_LEN);
			hotspotName[MAX_SCRIPT_NAME_LEN] = '\0';
			_hotspots[i]._scriptName = hotspotName;
		}
	}

	// walk area wall points
	uint32 walkAreaCount = dta->readUint32LE();
	_walkAreas.resize(walkAreaCount);
	for (uint i = 0; i < _walkAreas.size(); ++i) {
		_walkAreas[i]._wallPoint.x = dta->readUint32LE();
		_walkAreas[i]._wallPoint.y = dta->readUint32LE();
	}

	// room boundary
	_boundary.top = dta->readUint16LE();
	_boundary.bottom = dta->readUint16LE();
	_boundary.left = dta->readUint16LE();
	_boundary.right = dta->readUint16LE();

	// objects
	uint32 objectCount = dta->readUint16LE();
	if (objectCount > 40)
		error("Room: too many objects (%d)", objectCount);
	_objects.resize(objectCount);
	for (uint i = 0; i < _objects.size(); ++i) {
		SpriteStruct &sprite = _objects[i]._sprite;

		sprite._spriteId = dta->readUint16LE();
		sprite._x = dta->readUint16LE();
		sprite._y = dta->readUint16LE();
		sprite._roomId = dta->readUint16LE();
		sprite._on = dta->readUint16LE();
	}

	if (_version >= kAGSRoomVer253) {
		// local variables
		uint32 localVarCount = dta->readUint32LE();
		_localVars.resize(localVarCount);
		for (uint i = 0; i < _localVars.size(); ++i)
			_localVars[i].readFrom(dta);
	}

	if (_version >= kAGSRoomVer241) {
		// FIXME
	}

	// FIXME
}

} // End of namespace AGS
