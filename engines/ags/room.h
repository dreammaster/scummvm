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

#ifndef AGS_ROOM_H
#define AGS_ROOM_H

#include "common/array.h"
#include "common/rect.h"
#include "graphics/surface.h"

#include "engines/ags/gamefile.h"

namespace Common {
	class SeekableReadStream;
}

namespace AGS {

struct SpriteStruct {
	uint16 _spriteId;
	uint16 _x, _y;
	uint16 _roomId;
	uint16 _on;
};

struct PolyPoint {
	uint32 x, y;
};

struct AnimationStruct {
	uint32 _x, _y;
	uint32 _data;
	uint32 _object;
	uint32 _speed;
	byte _action;
	byte _wait;
};

struct FullAnimation {
	Common::Array<AnimationStruct> _stages;
};

struct RoomRegion {
	NewInteraction *_interaction;
	Common::Array<Common::String> *_scripts;
	uint16 _lightLevel;
	uint32 _tintLevel;
};

struct RoomObject {
	SpriteStruct _sprite;
	NewInteraction *_interaction;
	Common::Array<Common::String> *_scripts;
	uint32 _baseLine;
	uint16 _flags;
	Common::String _name;
	Common::String _scriptName;
	Common::StringMap _properties;
};

#define MSG_DISPLAYNEXT 1 // supercedes using alt-200 at end of message
#define MSG_TIMELIMIT   2
struct RoomMessage {
	Common::String _text;
	byte _displayAs; // 0 = normal window, 1 = as speech
	byte _flags;
};

struct RoomWalkArea {
	PolyPoint _wallPoint;
	int16 _zoom; // 0 = 100%, 1 = 101%, -1 = 99%
	int16 _zoom2; // for vector scaled areas
	int16 _light; // 0 = normal, + lighter, - darker
	uint16 _top, _bottom; // YP of area
};

struct RoomHotspot {
	Common::Point _walkToPos;
	Common::String _name;
	Common::String _scriptName;
	NewInteraction *_interaction;
	Common::Array<Common::String> *_scripts;
	Common::StringMap _properties;
};

struct BackgroundScene {
	Graphics::Surface _scene;
	bool _sharedPalette;
	byte _palette[256 * 3];
};

class Room {
public:
	Room(Common::SeekableReadStream *dta);

protected:
	void readMainBlock(Common::SeekableReadStream *dta);

public:
	// TODO: obsolete 1.x(?) script conditions

	Graphics::Surface _walkableMask; // walls
	Graphics::Surface _walkBehindMask; // object
	Graphics::Surface _hotspotMask; // lookat
	Graphics::Surface _regionsMask; // regions

	byte *_palette[256 * 3];

	Common::Array<uint16> _walkBehindBaselines; // objyval: baselines of walkbehind areas

	Common::Rect _boundary; // to walk off screen

	Common::Array<RoomRegion> _regions;
	Common::Array<RoomObject> _objects;

	Common::String _password;
	Common::Array<byte> _options;

	Common::Array<Common::String> _messages;

	uint16 _version; // wasversion: when loaded from file
	uint16 _flagStates;
	Common::Array<FullAnimation> _anims;

	uint16 _shadingInfo; // walkable area-specific view number

	// v2.x
	Common::Array<RoomWalkArea> _walkAreas;
	Common::Array<RoomHotspot> _hotspots;

	NewInteraction *_interaction;
	Common::String *_scripts;
	Common::Array<InteractionVariable> _localVars;

	Common::String _script;

	uint16 _width, _height; // in 320x200 terms (scrolling room size)
	uint16 _resolution; // 1 = 320x200, 2 = 640x400

	// FIXME: scripts, compiled_script, cscriptsize

	byte _backgroundSceneAnimSpeed;
	Common::Array<BackgroundScene> _backgroundScenes;

	uint32 _bytesPerPixel;

	Common::StringMap _properties;
};

} // End of namespace AGS

#endif // AGS_ROOM_H
