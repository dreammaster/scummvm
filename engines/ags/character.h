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

#ifndef AGS_CHARACTER_H
#define AGS_CHARACTER_H

#include "common/array.h"
#include "common/hash-str.h"

#include "engines/ags/scriptobj.h"
#include "engines/ags/drawable.h"

namespace AGS {

class AGSEngine;

class Character : public ScriptObject, public Drawable {
public:
	Character(AGSEngine *vm);

	bool isOfType(ScriptObjectType objectType) { return (objectType == sotCharacter); }
	const char *getObjectTypeName() { return "Character"; }
	uint32 readUint32(uint offset);
	bool writeUint32(uint offset, uint value);

	void walk(int x, int y, bool ignoreWalkable, bool autoWalkAnims);
	void followCharacter(Character *chr, int distance, uint eagerness);
	void stopMoving();

	void animate(uint loopId, uint speed, uint repeat, bool noIdleOverride, uint direction);

	void findReasonableLoop();

	void lockView(uint viewId);
	void lockViewOffset(uint viewId, int xOffs, int yOffs);
	void unlockView();

	void setIdleView(int view, uint time);
	void setSpeechView(int view);

	int32 _defView, _talkView, _view;

	uint32 _room, _prevRoom;

	int32 _x, _y;

	uint32 _wait;
	uint32 _flags;

	int16 _following;
	uint16 _followInfo;

	int32 _idleView; // the loop will be randomly picked
	uint16 _idleTime; // num seconds idle before playing anim
	int32 _idleLeft; // num seconds left, or -2 for playing idle anim

	uint16 _transparency; // if character is transparent
	uint16 _baseline;

	uint32 _activeInv;
	uint32 _talkColor;
	uint32 _thinkView;

	uint16 _blinkView, _blinkInterval; // design time
	uint16 _blinkTimer, _blinkFrame; // run time

	uint16 _walkSpeedY;
	int16 _picYOffs;

	int32 _z;

	uint32 _walkWait;

	uint16 _speechAnimSpeed;
	uint16 _blockingWidth, _blockingHeight;

	uint32 _indexId; // used for object functions to know the id

	int16 _picXOffs;
	uint16 _walkWaitCounter;

	uint16 _loop;
	uint16 _frame;

	uint16 _walking;
	uint16 _animating;

	uint16 _walkSpeed, _animSpeed;

	Common::Array<uint16> _inventory;

	int16 _actX, _actY;

	Common::String _name;
	Common::String _scriptName;

	byte _on;

	Common::StringMap _properties;

	// CharacterExtras
	Common::Array<uint16> _invOrder;
	uint16 _width, _height;
	uint16 _zoom;
	uint16 _xWas, _yWas;
	uint16 _tintR, _tintG, _tintB, _tintLevel, _tintLight;
	bool _processIdleThisTime;
	byte _slowMoveCounter;
	uint16 _animWait;

	uint getBaseline() const;

	virtual Common::Point getDrawPos();
	virtual int getDrawOrder() const;
	virtual const Graphics::Surface *getDrawSurface();
	virtual uint getDrawWidth();
	virtual uint getDrawHeight();
	virtual uint getDrawTransparency();
	virtual bool isDrawVerticallyMirrored();
	virtual int getDrawLightLevel();
	virtual void getDrawTint(int &lightLevel, int &luminance, byte &red, byte &green, byte &blue);

protected:
	AGSEngine *_vm;
};

} // End of namespace AGS

#endif // AGS_CHARACTER_H
