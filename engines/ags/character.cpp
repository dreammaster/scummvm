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

#include "engines/ags/character.h"
#include "engines/ags/ags.h"
#include "engines/ags/constants.h"
#include "engines/ags/gamefile.h"
#include "engines/ags/graphics.h"

namespace AGS {

Character::Character(AGSEngine *vm) : _vm(vm) {
	_walking = 0;
	_animating = 0;
	_picXOffs = 0;
	_picYOffs = 0;
	_blinkInterval = 140;
	_blinkTimer = _blinkInterval;
	_blockingWidth = 0;
	_blockingHeight = 0;
	_prevRoom = (uint)-1;
	_loop = 0;
	_frame = 0;
	_walkWait = (uint)-1;

	// CharacterExtras
	_width = 0;
	_height = 0;
	_xWas = INVALID_X;
	_yWas = 0;
	_zoom = 100;
	_tintR = 0;
	_tintG = 0;
	_tintB = 0;
	_tintLevel = 0;
	_tintLight = 0;
	_processIdleThisTime = false;
	_slowMoveCounter = 0; // unused?
	_animWait = 0;
}

uint32 Character::readUint32(uint offset) {
	switch (offset) {
	case 0:
		return _defView;
	case 4:
		return _talkView;
	case 8:
		return _view;
	case 12:
		return _room;
	case 16:
		return _prevRoom;
	case 20:
		return _x;
	case 24:
		return _y;
	case 28:
		return _wait;
	case 32:
		return _flags;
	case 40:
		return _idleView;
	case 52:
		return _activeInv;
	case 56:
		return _talkColor;
	case 60:
		return _thinkView;
	case 76:
		return _z;
	case 80:
		return _walkWait;
	case 96:
		return _indexId;
	default:
		error("Character::readUint32: offset %d is invalid", offset);
	}
}

bool Character::writeUint32(uint offset, uint value) {
	// FIXME

	return false;
}

void Character::followCharacter(Character *chr, int distance, uint eagerness) {
	if (eagerness > 250)
		error("followCharacter: invalid eagerness %d (must be 0-250)", eagerness);

	// FIXME
}

void Character::stopMoving() {
	// FIXME
}

void Character::findReasonableLoop() {
	// FIXME
}

void Character::lockView(uint viewId) {
	if (viewId < 1 || viewId > _vm->_gameFile->_views.size())
		error("Character::lockView: invalid view number %d (max is %d)", viewId, _vm->_gameFile->_views.size());
	viewId--;

	// FIXME
}

void Character::lockViewOffset(uint viewId, int xOffs, int yOffs) {
	lockView(viewId);

	if (_vm->_graphics->_screenResolutionMultiplier == 1 && _vm->_gameFile->_defaultResolution >= 3) {
		// running a 640x400 game at 320x200, adjust
		xOffs /= 2;
		yOffs /= 2;
	} else if (_vm->_graphics->_screenResolutionMultiplier > 1 && _vm->_gameFile->_defaultResolution <= 2) {
		// running a 320x200 game at 640x400, adjust
		xOffs *= 2;
		yOffs *= 2;
	}

	_picXOffs = xOffs;
	_picYOffs = yOffs;
}

void Character::unlockView() {
	if (_flags & CHF_FIXVIEW)
		debugC(kDebugLevelGame, "released view of character '%s' back to default",
			_scriptName.c_str());

	_flags &= ~CHF_FIXVIEW;
	_view = _defView;
	_frame = 0;
	stopMoving();
	if (_view >= 0) {
		/* unused:
		uint maxLoop = _vm->_gameFile->_views[_view].size();
		if ((_flags & CHF_NODIAGONAL) && (maxLoop > 4)
			maxLoop = 4;
		*/
		findReasonableLoop();
	}
	_animating = 0;
	_idleLeft = _idleTime;
	_picXOffs = 0;
	_picYOffs = 0;
	_processIdleThisTime = true;
}

void Character::setIdleView(int view, uint time) {
	if (view == 1)
		error("Character::setIdleView: view 1 may not be used as an idle view");
	else if (view > 0 && (uint)view > _vm->_gameFile->_views.size())
		error("Character::setIdleView: invalid view number %d (max is %d)", view, _vm->_gameFile->_views.size());

	// if an idle anim is currently playing, release it
	if (_idleLeft < 0)
		unlockView();

	if (view < 0) {
		// make sure they don't appear idle while idle anim is disabled
		_idleTime = 10;
	}
	_idleView = view - 1;
	_idleLeft = (uint32)_idleTime;

	// // if not currently animating, reset the wait counter
	if (_animating == 0 && _walking == 0)
		_wait = 0;

	if (_idleView >= 0)
		debugC(kDebugLevelGame, "set idle view of character '%s' to %d (time %d)",
			_scriptName.c_str(), _idleView, _idleTime);
	else
		debugC(kDebugLevelGame, "disabled idle view of character '%s'",
			_scriptName.c_str());

	if (_flags & CHF_FIXVIEW)
		debugC(kDebugLevelGame, "view of character '%s' is locked, idle view will not kick in until released",
			_scriptName.c_str());

	if (_idleTime == 0)
		_processIdleThisTime = true;
}

} // End of namespace AGS
