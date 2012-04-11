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

#ifndef AGS_ROOM_H
#define AGS_ROOM_H

#include "common/array.h"
#include "common/rect.h"
#include "graphics/surface.h"

#include "engines/ags/drawable.h"
#include "engines/ags/gamefile.h"

namespace Common {
	class SeekableReadStream;
}

namespace AGS {

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

struct WalkBehind : public Drawable {
	uint _left, _top, _right, _bottom;
	int16 _baseline; // was objyval: baseline of walkbehind area

	Graphics::Surface _surface;

	virtual Common::Point getDrawPos() { return Common::Point(_left, _top); }
	virtual int getDrawOrder() const { return _baseline; }
	virtual bool priorityIfEqual() const { return false; }
	virtual const Graphics::Surface *getDrawSurface() { return &_surface; }
	virtual uint getDrawWidth() { return _surface.w; }
	virtual uint getDrawHeight() { return _surface.h; }
	virtual uint getDrawTransparency() { return 0; }
	virtual bool isDrawMirrored() { return false; }
	virtual int getDrawLightLevel() { return 0; }
	virtual void getDrawTint(int &lightLevel, int &luminance, byte &red, byte &green, byte &blue) { }
};

struct RoomRegion : public ScriptObject {
	RoomRegion() : _interaction(NULL), _lightLevel(0), _tintLevel(0), _enabled(true) { }
	bool isOfType(ScriptObjectType objectType) { return (objectType == sotRegion); }
	const char *getObjectTypeName() { return "RoomRegion"; }

	NewInteraction *_interaction;
	InteractionScript _interactionScripts;
	uint16 _lightLevel;
	uint32 _tintLevel;
	bool _enabled;
};

// object flags
#define OBJF_NOINTERACT        1  // not clickable
#define OBJF_NOWALKBEHINDS     2  // ignore walk-behinds
#define OBJF_HASTINT           4  // the tint_* members are valid
#define OBJF_USEREGIONTINTS    8  // obey region tints/light areas
#define OBJF_USEROOMSCALING 0x10  // obey room scaling areas
#define OBJF_SOLID          0x20  // blocks characters from moving
#define OBJF_DELETED        0x40  // object has been deleted

struct RoomObject : public ScriptObject, public Drawable {
	RoomObject(AGSEngine *vm, uint id) : _vm(vm), _interaction(NULL), _flags(0), _id(id),
		_view(-1), _loop(0), _frame(0), _wait(0), _cycling(0), _transparency(0), _moving((uint16)-1),
		_blockingWidth(0), _blockingHeight(0), _baseline(-1) { }
	bool isOfType(ScriptObjectType objectType) { return (objectType == sotRoomObject); }
	const char *getObjectTypeName() { return "RoomObject"; }

	bool isVisible() const { return _visible; }
	void setVisible(bool visible);
	void setObjectView(uint viewId);
	void setObjectFrame(uint viewId, int loopId, int frameId);

	uint getTransparency() {
		if (_transparency == 0)
			return 0;
		if (_transparency == 255)
			return 100;
		return 100 - (_transparency * 10) / 25;
	}

	void setTransparency(uint value) {
		if (value == 0)
			_transparency = 0;
		else if (value == 100)
			_transparency = 255;
		else
			_transparency = ((100 - value) * 25) / 10;
	}

	void animate(uint loopId, uint speed, uint repeat, uint direction);
	void update();
	void stopMoving();

	int getBaseline() const;

	uint _id;

	// originally from room, immutable
	NewInteraction *_interaction;
	InteractionScript _interactionScripts;
	Common::String _name;
	Common::String _scriptName;
	Common::StringMap _properties;

	// originally from room, mutable
	int32 _baseline;
	uint16 _flags;
	// below originally from sprite
	Common::Point _pos;
	uint16 _spriteId;

	// these replace _on
	bool _visible;
	bool _merged;

	// constructed at runtime
	uint16 _view, _loop, _frame;
	uint16 _wait, _moving;
	uint _transparency;
	byte _cycling; // see ANIM_BACKWARDS etc
	byte _overallSpeed;
	uint16 _tintRed, _tintGreen, _tintBlue, _tintLevel, _tintLight;
	uint16 _blockingWidth, _blockingHeight;

	virtual Common::Point getDrawPos();
	virtual int getDrawOrder() const;
	virtual const Graphics::Surface *getDrawSurface();
	virtual uint getDrawWidth();
	virtual uint getDrawHeight();
	virtual uint getDrawTransparency();
	virtual bool isDrawMirrored();
	virtual int getDrawLightLevel();
	virtual void getDrawTint(int &lightLevel, int &luminance, byte &red, byte &green, byte &blue);

protected:
	AGSEngine *_vm;
};

#define MSG_DISPLAYNEXT 1 // supercedes using alt-200 at end of message
#define MSG_TIMELIMIT   2
struct MessageInfo {
	MessageInfo() : _displayAs(0), _flags(0) { }

	Common::String _text;
	byte _displayAs; // 0 = normal window, 1 = as speech
	byte _flags;
};

#define NOT_VECTOR_SCALED ((uint16)-10000)
struct RoomWalkArea {
	RoomWalkArea() : _light(0), _top(0xffff), _bottom(0xffff), _zoom(0), _zoom2(NOT_VECTOR_SCALED) { }

	int16 _zoom; // 0 = 100%, 1 = 101%, -1 = 99%
	int16 _zoom2; // for vector scaled areas
	int16 _light; // 0 = normal, + lighter, - darker
	uint16 _top, _bottom; // YP of area
};

struct RoomHotspot : public ScriptObject {
	RoomHotspot() : _interaction(NULL), _enabled(true) { }
	bool isOfType(ScriptObjectType objectType) { return (objectType == sotHotspot); }
	const char *getObjectTypeName() { return "RoomHotspot"; }

	Common::Point _walkToPos;
	Common::String _name;
	Common::String _scriptName;
	NewInteraction *_interaction;
	InteractionScript _interactionScripts;
	Common::StringMap _properties;
	bool _enabled;
};

struct BackgroundScene {
	BackgroundScene() : _sharedPalette(false) { }

	Graphics::Surface _scene;
	bool _sharedPalette;
	byte _palette[256 * 4];
};

class AGSEngine;

class Room : public Drawable {
public:
	Room(AGSEngine *vm, Common::SeekableReadStream *dta);
	~Room();

	virtual Common::Point getDrawPos() { return Common::Point(0, 0); }
	virtual int getDrawOrder() const { return 0; }
	virtual const Graphics::Surface *getDrawSurface();
	virtual uint getDrawWidth();
	virtual uint getDrawHeight();
	virtual uint getDrawTransparency() { return 0; }
	virtual bool isDrawMirrored() { return false; }
	virtual int getDrawLightLevel() { return 0; }
	virtual void getDrawTint(int &lightLevel, int &luminance, byte &red, byte &green, byte &blue) { }

	bool isLoaded() { return _loaded; }

	void loadFrom(Common::SeekableReadStream *dta);
	void unload();

	void initWalkBehinds();
	void updateWalkBehinds();

	void redoWalkableAreas();

	uint getHotspotAt(int x, int y);
	uint getObjectAt(int x, int y);
	uint getObjectAt(int x, int y, int &id);
	uint getRegionAt(int x, int y);

protected:
	AGSEngine *_vm;
	bool _loaded;

	void readData(Common::SeekableReadStream *dta);
	void readMainBlock(Common::SeekableReadStream *dta);

public:
	// TODO: obsolete 1.x(?) script conditions

	Graphics::Surface _originalWalkableMask; // walkareabackup
	Graphics::Surface _walkableMask; // walls - as updated (scripts, characters)
	Graphics::Surface _walkBehindMask; // object
	Graphics::Surface _hotspotMask; // lookat
	Graphics::Surface _regionsMask; // regions

	Common::Array<WalkBehind> _walkBehinds;

	Common::Rect _boundary; // to walk off screen

	Common::Array<RoomRegion> _regions;
	Common::Array<RoomObject *> _objects;

	Common::String _password;
	Common::Array<byte> _options;

	Common::Array<MessageInfo> _messages;

	uint16 _version; // wasversion: when loaded from file
	uint32 _gameId;
	uint16 _flagStates;
	Common::Array<FullAnimation> _anims;

	Common::Array<uint16> _shadingInfo; // walkable area-specific view number

	// v2.x
	Common::Array<PolyPoint> _wallPoints;

	Common::Array<RoomWalkArea> _walkAreas;
	Common::Array<RoomHotspot> _hotspots;

	NewInteraction *_interaction;
	InteractionScript _interactionScripts;
	Common::Array<InteractionVariable> _localVars;

	Common::String _script;
	ccScript *_compiledScript;

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
