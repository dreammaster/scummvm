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
#include "engines/ags/gamestate.h"
#include "engines/ags/graphics.h"
#include "engines/ags/pathfinder.h"
#include "engines/ags/room.h"
#include "engines/ags/sprites.h"

#include "common/random.h"

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

	// FIXME: _walkWait = (uint)-1;
	_walkWait = 0;

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
	case 92:
		return _indexId;
	default:
		error("Character::readUint32: offset %d is invalid", offset);
	}
}

bool Character::writeUint32(uint offset, uint value) {
	switch (offset) {
	case 0:
		_defView = value;
		break;
	case 4:
		_talkView = value;
		break;
	case 8:
		_view = value;
		break;
	case 12:
		_room = value;
		break;
	case 16:
		_prevRoom = value;
		break;
	case 20:
		_x = value;
		break;
	case 24:
		_y = value;
		break;
	case 28:
		_wait = value;
		break;
	case 32:
		_flags = value;
		break;
	case 40:
		_idleView = value;
		break;
	case 52:
		_activeInv = value;
		break;
	case 56:
		_talkColor = value;
		break;
	case 60:
		_thinkView = value;
		break;
	case 76:
		_z = value;
		break;
	case 80:
		_walkWait = value;
		break;
	case 92:
		_indexId = value;
		break;
	default:
		return false;
	}

	return true;
}

uint16 Character::readUint16(uint offset) {
	if (offset >= 112 && offset <= 112 + (MAX_INV*2)) {
		if (offset % 2 != 0)
			error("Character::readUint16: offset %d is invalid", offset);
		offset = (offset - 112) / 2;
		return _inventory[offset];
	}

	switch (offset) {
	case 36:
		return _following;
	case 38:
		return _followInfo;
	case 44:
		return _idleTime;
	case 46:
		return _idleLeft;
	case 48:
		return _transparency;
	case 50:
		return _baseline;
	case 64:
		return _blinkView;
	case 66:
		return _blinkInterval;
	case 68:
		return _blinkTimer;
	case 70:
		return _blinkFrame;
	case 72:
		return _walkSpeedY;
	case 74:
		return _picYOffs;
	case 84:
		return _speechAnimSpeed;
	// (86 is reserved)
	case 88:
		return _blockingWidth;
	case 90:
		return _blockingHeight;
	case 96:
		return _picXOffs;
	case 98:
		return _walkWaitCounter;
	case 100:
		return _loop;
	case 102:
		return _frame;
	case 104:
		return _walking;
	case 106:
		return _animating;
	case 108:
		return _walkSpeed;
	case 110:
		return _animSpeed;
	case 714: // 112 + MAX_INV*2
		return _actX;
	case 716:
		return _actY;
	default:
		error("Character::readUint16: offset %d is invalid", offset);
	}
}

bool Character::writeUint16(uint offset, uint16 value) {
	if (offset >= 112 && offset <= 112 + (MAX_INV*2)) {
		if (offset % 2 != 0)
			error("Character::writeUint16: offset %d is invalid", offset);
		offset = (offset - 112) / 2;
		_inventory[offset] = value;
		return true;
	}

	switch (offset) {
	case 36:
		_following = value;
		break;
	case 38:
		_followInfo = value;
		break;
	case 44:
		_idleTime = value;
		break;
	case 46:
		_idleLeft = value;
		break;
	case 48:
		_transparency = value;
		break;
	case 50:
		_baseline = value;
		break;
	case 64:
		_blinkView = value;
		break;
	case 66:
		_blinkInterval = value;
		break;
	case 68:
		_blinkTimer = value;
		break;
	case 70:
		_blinkFrame = value;
		break;
	case 72:
		_walkSpeedY = value;
		break;
	case 74:
		_picYOffs = value;
		break;
	case 84:
		_speechAnimSpeed = value;
		break;
	case 88:
		_blockingWidth = value;
		break;
	case 90:
		_blockingHeight = value;
		break;
	case 96:
		_picXOffs = value;
		break;
	case 98:
		_walkWaitCounter = value;
		break;
	case 100:
		_loop = value;
		break;
	case 102:
		_frame = value;
		break;
	case 104:
		_walking = value;
		break;
	case 106:
		_animating = value;
		break;
	case 108:
		_walkSpeed = value;
		break;
	case 110:
		_animSpeed = value;
		break;
	case 714:
		_actX = value;
		break;
	case 716:
		_actY = value;
		break;
	default:
		return false;
	}

	return true;
}

byte Character::readByte(uint offset) {
	if (offset >= 718 && offset <= 718 + 40) {
		// TODO: do we need this?
		offset = offset - 718;
		if (offset < _name.size())
			return _name[offset];
		return 0;
	}
	if (offset >= 758 && offset <= 758 + 20) {
		// TODO: do we need this?
		offset = offset - 758;
		if (offset < _scriptName.size())
			return _scriptName[offset];
		return 0;
	}

	switch (offset) {
	case 778:
		return _on;
	default:
		error("Character::readByte: offset %d is invalid", offset);
	}
}

bool Character::writeByte(uint offset, byte value) {
	// FIXME

	return false;
}

// order of loops to turn character in circle from down to down
const uint turnLoopOrder[8] = { 0, 6, 1, 7, 3, 5, 2, 4 };

uint findTurnLoopOrderIndex(uint loopId) {
	for (uint i = 0; i < 8; ++i)
		if (turnLoopOrder[i] == loopId)
			return i;

	return 0;
}

// returns true if is a following_exactly sheep
bool Character::update() {
	if (_on != 1)
		return false;

	// walking
	if (_walking >= TURNING_AROUND) {
		if (_walkWait > 0) {
			// Currently rotating to correct direction
			_walkWait--;
			return false;
		}

		// Work out which direction is next
		int wantLoop = findTurnLoopOrderIndex(_loop);
		// going anti-clockwise, take one before; otherwise, one after
		if (_walking >= TURNING_BACKWARDS)
			wantLoop -= 1;
		else
			wantLoop += 1;

		// TODO: make sure we don't loop forever!
		while (true) {
			// wrap around
			if (wantLoop >= 8)
				wantLoop = 0;
			else if (wantLoop < 0)
				wantLoop = 7;

			// check that we have a useful loop
			if ((turnLoopOrder[wantLoop] < _vm->_gameFile->_views[_view]._loops.size()) &&
				(!_vm->getViewLoop(_view, turnLoopOrder[wantLoop])->_frames.empty())) {
				// if this is moving diagonally, make sure that's okay

				if (!(_flags & CHF_NODIAGONAL))
					break;
				if (turnLoopOrder[wantLoop] < 4)
					break;
			}

			// continue to the next loop
			if (_walking >= TURNING_BACKWARDS)
				wantLoop--;
			else
				wantLoop++;
		}

		_loop = turnLoopOrder[wantLoop];
		_walking -= TURNING_AROUND;

		// if still turning, wait for next frame
		if (_walking % TURNING_BACKWARDS >= TURNING_AROUND)
			_walkWait = _animSpeed;
		else
			_walking = _walking % TURNING_BACKWARDS;

		_animWait = 0;

		return false;
	}

	// Make sure it doesn't flash up a blue cup
	if (_view >= 0 && _loop >= _vm->_gameFile->_views[_view]._loops.size())
		_loop = 0;

	bool doingNothing = true;

	if (_room == _vm->getCurrentRoomId() && _walking > 0) {
		if (_walkWait > 0) {
			_walkWait--;
		} else {
			_flags &= ~CHF_AWAITINGMOVE;

			int numSteps = needMoveSteps();

			if (numSteps && _xWas != INVALID_X) {
				// if the zoom level changed mid-move, the walkcounter
				// might not have come round properly - so sort it out
				_x = _xWas;
				_y = _yWas;
				_xWas = INVALID_X;
			}

			int oldX = _x, oldY = _y;
			for (int i = 0; i < abs(numSteps); ++i) {
				if (doNextMoveStep())
					break;
				if (_walking == 0 || _walking >= TURNING_AROUND)
					break;
			}

			if (numSteps < 0) {
				// very small scaling, intersperse the movement
				// to stop it being jumpy
				_xWas = _x;
				_yWas = _y;
				_x = (_x - oldX) / 2 + oldX;
				_y = (_y - oldY) / 2 + oldY;
			} else if (numSteps > 0) {
				_xWas = INVALID_X;
			}

			if (!(_flags & CHF_ANTIGLIDE))
				_walkWaitCounter++;
		}

		if (_loop >= _vm->_gameFile->_views[_view]._loops.size())
			error("can't render character '%s' (id %d) because loop %d doesn't exist in view %d",
				_scriptName.c_str(), _indexId, _loop, _view + 1);

		uint framesInLoop = _vm->_gameFile->_views[_view]._loops[_loop]._frames.size();
		if (_frame >= framesInLoop) {
			// force back to a valid frame
			if (framesInLoop == 0)
				error("can't render character '%s' (id %d) because loop %d has no frames in view %d",
					_scriptName.c_str(), _indexId, _loop, _view + 1);
			else if (framesInLoop == 1)
				_frame = 0;
			else
				_frame = 1;
		}

		if (!_walking) {
			// done!
			_processIdleThisTime = true;
			doingNothing = true; // TODO: this is stomped over below
			_walkWait = 0;
			_animWait = 0;

			// use standing pic
			stopMoving();
			_frame = 0;
			checkViewFrame();
		} else if (_animWait > 0) {
			// waiting for an animation..
			_animWait--;
		} else {
			// moved
			if (_flags & CHF_ANTIGLIDE)
				_walkWaitCounter++;

			if (!(_flags & CHF_MOVENOTWALK)) {
				// walk animation
				_frame++;
				if (_frame >= framesInLoop) {
					// end of loop, so loop back round skipping the standing frame (if possible)
					if (framesInLoop == 1)
						_frame = 0;
					else
						_frame = 1;
				}

				_animWait = _vm->_gameFile->_views[_view]._loops[_loop]._frames[_frame]._speed + _animSpeed;

				if (_flags & CHF_ANTIGLIDE)
					_walkWait = _animWait;
				else
					_walkWait = 0;

				checkViewFrame();
			}
		}

		doingNothing = false;
	}

	if (_room == _vm->getCurrentRoomId() &&
		(_animating || (_idleLeft < 0)) &&
		(!_walking || (_flags & CHF_MOVENOTWALK))) {
		// not moving, but animating (or playing idle anim)

		// idle anim doesn't count as doing something
		doingNothing = (_idleLeft < 0);

		if (_wait > 0)
			_wait--;
		else if (_vm->getGameOption(OPT_LIPSYNCTEXT) && false /* FIXME: check _charSpeaking */) {
			// FIXME
			return false;
		} else {
			// TODO: some of this is similar to RoomObject::update
			uint oldFrame = _frame;
			if (_animating & CHANIM_BACKWARDS) {
				if (_frame == 0) {
					// at the start of the loop, what now?
					if (_loop > 0 && _vm->getViewLoop(_view, _loop - 1)->shouldRunNextLoop()) {
						// If it's a Go-to-next-loop on the previous one, then go back
						_loop--;
						_frame = _vm->getViewLoop(_view, _loop)->_frames.size() - 1;
					} else if (_animating & CHANIM_REPEAT) {
						// repeating animation
						_frame = _vm->getViewLoop(_view, _loop)->_frames.size() - 1;
						while (_vm->getViewLoop(_view, _loop)->shouldRunNextLoop()) {
							_loop++;
							_frame = _vm->getViewLoop(_view, _loop)->_frames.size() - 1;
						}
					} else {
						// leave it on the first frame
						_animating = 0;
					}
				} else
					_frame--;
			} else
				_frame++;

			// FIXME: stop talking if we were _charSpeaking and the speech is done

			ViewLoopNew *loop = _vm->getViewLoop(_view, _loop);
			if (_frame >= loop->_frames.size()) {
				// at the end of the loop, what now?
				if (loop->shouldRunNextLoop()) {
					// go to next loop thing
					if ((uint)_loop + 1 >= _vm->_gameFile->_views[_view]._loops.size())
						error("Character::update: last loop %d in view %d requested to move to next loop",
							_loop, _view);
					_loop++;
					_frame = 0;
				} else if (!(_animating & CHANIM_REPEAT)) {
					// leave it on the last frame
					_animating = 0;
					_frame--;
					if (_idleLeft < 0) {
						// end of idle anim
						if (_idleTime == 0) {
							// constant anim, reset (need this cos animating==0)
							_frame = 0;
						} else {
							// one-off anim, stop
							unlockView();
							_idleLeft = _idleTime;
						}
					}
				} else {
					_frame = 0;
					if (!_vm->_state->_noMultiLoopRepeat) {
						// multi-loop animation, go back to the start
						while (_loop > 0 && _vm->getViewLoop(_view, _loop - 1)->shouldRunNextLoop())
							_loop--;
					}
				}
			}

			ViewFrame *frame = _vm->getViewFrame(_view, _loop, _frame);
			_wait = frame->_speed;

			// idle anim doesn't have speed stored cos animating==0
			if (_idleLeft < 0)
				_wait += _animSpeed + 5;
			else
				_wait += (_animating >> 8) & 0xff;

			if (_frame != oldFrame)
				checkViewFrame();
		}
	}

	bool isSheep = false;
	if (_following >= 0) {
		if (_followInfo == FOLLOW_ALWAYSONTOP) {
			// an always-on-top follow

			isSheep = true;
		} else if (doingNothing) {
			// not moving, but should be following another character

			// FIXME
		}
	}

	if (_idleView >= 1 && _idleLeft >= 0 && _room == _vm->getCurrentRoomId()) {
		// char is in current room, and has an idle anim which is not currently playing

		if (!doingNothing || (_flags & CHF_FIXVIEW)) {
			// they are moving or animating, or the view is locked, so reset idle timeout
			_idleLeft = _idleTime;
		} else if (_processIdleThisTime || (_vm->getLoopCounter() % 40 == 0)) {
			_idleLeft--;
			if (_idleLeft == -1) {
				debugC(kDebugLevelGame, "character '%s' (id %d) now idle: view %d",
					_scriptName.c_str(), _indexId, _idleView + 1);
				uint useLoop = _loop;

				lockView(_idleView + 1);
				// setView resets this to 0
				_idleLeft = -2;

				const ViewStruct &view = _vm->_gameFile->_views[_idleView];
				uint maxLoops = view._loops.size();

				// don't try using diagonal loops if useDiagonal() doesn't return 0
				// (either there aren't any, or they're standing frame only)
				if (maxLoops > 4 && useDiagonal())
					maxLoops = 4;

				if (_idleTime > 0 && useLoop >= maxLoops) {
					// If it's not a "swimming"-type idleanim, choose a random loop
					// if there arent enough loops to do the current one.

					do {
						useLoop = _vm->getRandomSource()->getRandomNumber(maxLoops - 1);
					// don't select a loop which is a continuation of a previous one
					} while (useLoop > 0 && view._loops[useLoop - 1].shouldRunNextLoop());
				} else if (useLoop >= maxLoops) {
					// Normal idle anim - just reset to loop 0 if not enough to
					// use the current one

					useLoop = 0;
				}

				animate(useLoop, _animSpeed + 5, (_idleTime == 0) ? 1 : 0, true);

				// don't set animating while the idle anim plays
				_animating = 0;
			}
		}
	}

	_processIdleThisTime = false;

	return isSheep;
}

// return 0 to use diagonal, 1 to not
// or 2 if there are only standing frames for the diagonal loops (to allow providing smoother turning)
uint Character::useDiagonal() {
	// don't use if the flags don't allow it
	if (_flags & CHF_NODIAGONAL)
		return 1;
	// don't use if we don't have diagonal loops
	if (_vm->_gameFile->_views[_view]._loops.size() < 8)
		return 1;
	// only standing frames for loop 4?
	if (_vm->_gameFile->_views[_view]._loops[4]._frames.size() < 2)
		return 2;
	return 0;
}

// returns false if the character only has horizontal animations
bool Character::hasUpDownLoops() {
	const ViewStruct &view = _vm->_gameFile->_views[_view];

	// no frames in the Down animation?
	if (view._loops[0]._frames.empty())
		return false;
	// no Up animation, or no frames in it?
	if (view._loops.size() < 4 || view._loops[3]._frames.empty())
		return false;

	return true;
}

void Character::walk(int x, int y, bool ignoreWalkable, bool autoWalkAnims) {
	if (_room != _vm->getCurrentRoomId())
		error("Character::walk: character '%s' (id %d) in room %d, not in current room (%d)",
			_scriptName.c_str(), _indexId, _room, _vm->getCurrentRoomId());

	_flags &= ~CHF_MOVENOTWALK;

	Common::Point from(_vm->convertToLowRes(_x), _vm->convertToLowRes(_y));
	Common::Point to(_vm->convertToLowRes(x), _vm->convertToLowRes(y));

	// if we're already there, don't bother
	if (to == from) {
		stopMoving();
		debugC(kDebugLevelGame, "character '%s' (id %d) already at dest, not moving",
			_scriptName.c_str(), _indexId);
		return;
	}

	// if we're going to override the animation, stop the existing one
	if (_animating && autoWalkAnims)
		_animating = 0;

	// if we're playing an idle animation, stop it
	if (_idleLeft < 0) {
		unlockView();
		_idleLeft = _idleTime;
	}

	uint32 oldWait = 0;
	uint16 oldAnimWait = 0;
	if (_walking) {
		// if they are currently walking, save the current wait
		oldWait = _wait;
		oldAnimWait = _animWait;
	}

	// stop them to make sure they're on a walkable area
	// but save their frame first so that if they're already
	// moving it looks smoother
	uint16 oldFrame = _frame;
	stopMoving();
	_frame = oldFrame;

	debugC(kDebugLevelGame, "character '%s' (id %d) now moving to %d,%d",
		_scriptName.c_str(), _indexId, x, y);

	int moveSpeedX = _walkSpeed;
	int moveSpeedY = _walkSpeed;
	if (_walkSpeedY != UNIFORM_WALK_SPEED)
		moveSpeedY = _walkSpeedY;

	if (moveSpeedX == 0 && moveSpeedY == 0)
		warning("character '%s' (id %d) moving with walk speed 0", _scriptName.c_str(), _indexId);

	Graphics::Surface *walkableMask = _vm->getWalkableMaskFor(_indexId);

	if (findPath(_vm, from, to, walkableMask, &_moveList, moveSpeedX, moveSpeedY, true, ignoreWalkable)) {
		_walking = 1;
		_moveList._direct = ignoreWalkable;

		if (autoWalkAnims) {
			// cancel any pending waits on current animations
			// or if they were already moving, keep the current wait -
			// this prevents a glitch if MoveCharacter is called when they
			// are already moving

			_walkWait = oldWait;
			_animWait = oldAnimWait;

			if (_moveList._stages[0].pos != _moveList._stages[1].pos)
				fixPlayerSprite();
		} else
			_flags |= CHF_MOVENOTWALK;
	} else if (autoWalkAnims) {
		// pathfinder couldn't get a route, stand them still
		_frame = 0;
	}

	delete walkableMask;
}

void Character::followCharacter(Character *chr, int distance, uint eagerness) {
	if (eagerness > 250)
		error("followCharacter: invalid eagerness %d (must be 0-250)", eagerness);

	warning("Character::followChararacter unimplemented");
	// FIXME
}

void Character::stopMoving() {
	if (_vm->_state->_skipUntilCharStops == _indexId)
		_vm->endSkippingUntilCharStops();

	if (_xWas != INVALID_X) {
		_x = _xWas;
		_y = _yWas;
		_xWas = INVALID_X;
	}

	if (_walking > 0 && _walking < TURNING_AROUND) {
		// if it's not a MoveCharDirect, make sure they end up on a walkable area
		if (!_moveList._direct && _room == _vm->getCurrentRoomId())
			moveToNearestWalkableArea();

		debugC(kDebugLevelGame, "character '%s' (id %d) stop walking",
			_scriptName.c_str(), _indexId);

		_idleLeft = _idleTime;
		// restart the idle animation straight away
		_processIdleThisTime = true;
	}

	if (_walking) {
		// If the character is currently moving, stop them and reset their frame
		_walking = 0;
		if (!(_flags & CHF_MOVENOTWALK))
			_frame = 0;
	}
}

// replaces the 'CHECK_DIAGONAL' macro in original
void adjustDiagonalMoveFrame(uint &useLoop, int mainDir, int otherDir, uint loop1, uint loop2) {
	if (abs(mainDir) <= abs(otherDir) / 2)
		return;

	if (mainDir < 0)
		useLoop = loop1;
	else
		useLoop = loop2;
}

// changes to the appropriate animation loop for our current walk
void Character::fixPlayerSprite() {
	// TODO: some of this is copied right from faceLocation
	uint useLoop = 1;

	frac_t xDiff = _moveList._stages[_moveList._curStage].xPerMove;
	frac_t yDiff = _moveList._stages[_moveList._curStage].yPerMove;

	uint diagonalState = useDiagonal();

	if (_vm->getGameFileVersion() <= kAGSVer272) {
		// Use a different logic on 2.x. This fixes some edge cases where
		// FaceLocation() is used to select a specific loop.
		// "This fixes edge cases where the function was used to select specific loops, e.g. in Murder in a Wheel."

		bool canRight = _vm->_gameFile->_views[_view]._loops.size() >= 3 && !_vm->_gameFile->_views[_view]._loops[2]._frames.empty();
		bool canLeft = _vm->_gameFile->_views[_view]._loops.size() >= 2 && !_vm->_gameFile->_views[_view]._loops[1]._frames.empty();

		if (abs(yDiff) < abs(xDiff)) {
			// primary movement in X direction
			if (!canLeft && !canRight) {
				useLoop = 0;
			} else if (canRight && xDiff >= 0) {
				useLoop = 2;
				if (diagonalState == 0)
					adjustDiagonalMoveFrame(useLoop, yDiff, xDiff, 5, 4);
			} else if (canLeft && yDiff < 0) {
				useLoop = 1;
				if (diagonalState == 0)
					adjustDiagonalMoveFrame(useLoop, yDiff, xDiff, 7, 6);
			}
		} else {
			if (yDiff >= 0) {
				useLoop = 0;
				if (diagonalState == 0)
					adjustDiagonalMoveFrame(useLoop, yDiff, xDiff, 6, 4);
			} else {
				useLoop = 3;
				if (diagonalState == 0)
					adjustDiagonalMoveFrame(useLoop, yDiff, xDiff, 7, 5);
			}
		}
	} else {
		// modern (3.x) logic

		if (!hasUpDownLoops() || abs(yDiff) < abs(xDiff)) {
			// wantHoriz
			if (xDiff > 0) {
				useLoop = 2;
				if (diagonalState == 0)
					adjustDiagonalMoveFrame(useLoop, yDiff, xDiff, 5, 4);
			} else {
				useLoop = 1;
				if (diagonalState == 0)
					adjustDiagonalMoveFrame(useLoop, yDiff, xDiff, 7, 6);
			}
		} else {
			// !wantHoriz
			if (yDiff > 0) {
				useLoop = 0;
				if (diagonalState == 0)
					adjustDiagonalMoveFrame(useLoop, xDiff, yDiff, 6, 4);
			} else if (yDiff < 0) {
				useLoop = 3;
				if (diagonalState == 0)
					adjustDiagonalMoveFrame(useLoop, xDiff, yDiff, 7, 5);
			}
		}
	}

	if (!_vm->getGameOption(OPT_ROTATECHARS) || (_flags & CHF_NOTURNING)) {
		_loop = useLoop;
		return;
	}

	if (_loop > 3 && (_flags & CHF_NODIAGONAL)) {
		// They've just been playing an animation with an extended loop number,
		// so don't try and rotate using it
		_loop = useLoop;
		return;
	}

	if (_loop >= _vm->_gameFile->_views[_view]._loops.size() ||
		_vm->_gameFile->_views[_view]._loops[_loop]._frames.empty() || !hasUpDownLoops()) {
		// Character is not currently on a valid loop, so don't try to rotate
		// eg. left/right only view, but current loop 0
		_loop = useLoop;
		return;
	}

	startTurning(useLoop, diagonalState);
}

// returns the number of move steps needed per frame
// (-ve for only partial steps) based on the walkWaitCounter
int Character::needMoveSteps() {
	// check the most likely cases first
	if (_zoom == 100)
		return 1;
	if (!(_flags & CHF_SCALEMOVESPEED))
		return 1;

	if (_zoom >= 170) {
		// scaling 170-200%, move 175% speed
		if ((_walkWaitCounter % 4) >= 1)
			return 2;
		else
			return 1;
	} else if (_zoom >= 140) {
		// scaling 140-170%, move 150% speed
		if ((_walkWaitCounter % 2) == 1)
			return 2;
		else
			return 1;
	} else if (_zoom >= 115) {
		// scaling 115-140%, move 125% speed
		if ((_walkWaitCounter % 4) >= 3)
			return 2;
		else
			return 1;
	} else if (_zoom >= 80) {
		// scaling 80-120%, normal speed
		return 1;
	} else if (_zoom >= 60) {
		// scaling 60-80%, move 75% speed
		if ((_walkWaitCounter % 4) >= 1)
			return 1;
		else
			return 0;
	} else if (_zoom >= 30) {
		// scaling 30-60%, move 50% speed
		if ((_walkWaitCounter % 2) == 1)
			return -1;
		else if (_xWas != INVALID_X) {
			// move the second half of the movement to make it smoother
			_x = _xWas;
			_y = _yWas;
			_xWas = INVALID_X;
		}
		return 0;
	} else {
		// scaling 0-30%, move 25% speed
		if ((_walkWaitCounter % 4) >= 3)
			return -1;
		else if ((_walkWaitCounter % 4) == 1) {
			if (_xWas != INVALID_X) {
				// move the second half of the movement to make it smoother
				_x = _xWas;
				_y = _yWas;
				_xWas = INVALID_X;
			}
		}
		return 0;
	}
}

// returns true if waiting for another char to move
bool Character::doNextMoveStep() {
	int xWas = _x;
	int yWas = _y;

	Common::Point pos(_x, _y);
	if (_moveList.doStep(pos)) {
		if (!(_flags & CHF_MOVENOTWALK))
			fixPlayerSprite();
	}
	if (_moveList._stages.empty())
		_walking = 0;
	_x = pos.x;
	_y = pos.y;

	// FIXME
	warning("Character::doNextMoveStep unimplemented");

	return false;
}

// returns true if it actually started turning
bool Character::faceLocation(int x, int y) {
	int xDiff = x - _x;
	int yDiff = y - _y;

	debugC(kDebugLevelGame, "character '%s' (id %d) turning to face location: %d, %d",
		_scriptName.c_str(), _indexId, x, y);

	// called on their current position - do nothing
	if (xDiff == 0 && yDiff == 0)
		return false;

	uint useLoop = 1;
	uint highestLoopForTurning = 3;

	// Allow use of any available diagonal frames, even if they're standing-only.
	uint diagonalState = useDiagonal();
	if (diagonalState != 1)
		highestLoopForTurning = 7;

	if (_vm->getGameFileVersion() <= kAGSVer272) {
		// Use a different logic on 2.x. This fixes some edge cases where
		// FaceLocation() is used to select a specific loop.
		// "This fixes edge cases where the function was used to select specific loops, e.g. in Murder in a Wheel."

		bool canRight = _vm->_gameFile->_views[_view]._loops.size() >= 3 && !_vm->_gameFile->_views[_view]._loops[2]._frames.empty();
		bool canLeft = _vm->_gameFile->_views[_view]._loops.size() >= 2 && !_vm->_gameFile->_views[_view]._loops[1]._frames.empty();

		if (abs(yDiff) < abs(xDiff)) {
			// primary movement in X direction
			if (!canLeft && !canRight) {
				useLoop = 0;
			} else if (canRight && xDiff >= 0) {
				useLoop = 2;
				if (diagonalState == 0)
					adjustDiagonalMoveFrame(useLoop, yDiff, xDiff, 5, 4);
			} else if (canLeft && yDiff < 0) {
				useLoop = 1;
				if (diagonalState == 0)
					adjustDiagonalMoveFrame(useLoop, yDiff, xDiff, 7, 6);
			}
		} else {
			if (yDiff >= 0) {
				useLoop = 0;
				if (diagonalState == 0)
					adjustDiagonalMoveFrame(useLoop, yDiff, xDiff, 6, 4);
			} else {
				useLoop = 3;
				if (diagonalState == 0)
					adjustDiagonalMoveFrame(useLoop, yDiff, xDiff, 7, 5);
			}
		}
	} else {
		// modern (3.x) logic

		if (!hasUpDownLoops() || abs(yDiff) < abs(xDiff)) {
			// wantHoriz
			if (xDiff > 0) {
				useLoop = 2;
				if (diagonalState == 0)
					adjustDiagonalMoveFrame(useLoop, yDiff, xDiff, 5, 4);
			} else {
				useLoop = 1;
				if (diagonalState == 0)
					adjustDiagonalMoveFrame(useLoop, yDiff, xDiff, 7, 6);
			}
		} else {
			// !wantHoriz
			if (yDiff > 0) {
				useLoop = 0;
				if (diagonalState == 0)
					adjustDiagonalMoveFrame(useLoop, xDiff, yDiff, 6, 4);
			} else if (yDiff < 0) {
				useLoop = 3;
				if (diagonalState == 0)
					adjustDiagonalMoveFrame(useLoop, xDiff, yDiff, 7, 5);
			}
		}
	}

	if (_vm->getGameOption(OPT_TURNTOFACELOC) && useLoop != _loop && _loop <= highestLoopForTurning) {
		if (!_vm->inEntersScreen()) {
			// Turn to face new direction.
			stopMoving();

			if (_on == 1) {
				// only do the turning if the character is not hidden

				startTurning(useLoop, diagonalState);
				// beware: the caller is reponsible for blocking and setting _frame to 0,
				// unlike in the original code
				return true;
			}
		}
	}

	_loop = useLoop;
	_frame = 0;
	return false;
}

void Character::startTurning(uint useLoop, uint diagonalState) {
	// work out how far round they have to turn
	uint fromIndex = findTurnLoopOrderIndex(_loop);
	uint toIndex = findTurnLoopOrderIndex(useLoop);

	int turnDirection = 1;
	// work out whether anticlockwise is quicker or not
	if ((toIndex > fromIndex) && ((toIndex - fromIndex) > 4))
		turnDirection = -1;
	if ((toIndex < fromIndex) && ((fromIndex - toIndex) < 4))
		turnDirection = -1;

	debugC(1, kDebugLevelGame, "character '%s' (id %d) starting turn from %d (loop %d) to %d (loop %d), direction %d",
		_scriptName.c_str(), _indexId, fromIndex, _loop, toIndex, useLoop, turnDirection);

	// strip any current turning_around stages
	_walking = _walking % TURNING_AROUND;
	if (turnDirection == -1)
		_walking += TURNING_BACKWARDS;

	// Iterate over the turning loops from the fromIndex to the toIndex.
	for (int i = (int)fromIndex; i != (int)toIndex; i += turnDirection) {
		// wrap the index
		if (i < 0)
			i = 7;
		if (i >= 8)
			i = 0;
		if ((uint)i == toIndex)
			break;

		// If we shouldn't use ANY diagonals, don't.
		if (turnLoopOrder[i] >= 4 && diagonalState == 1)
			continue;
		// If this frame is empty, it's useless.
		if (_vm->_gameFile->_views[_view]._loops[turnLoopOrder[i]]._frames.empty())
			continue;

		// If this loop is present, then add it to the walking queue.
		if (turnLoopOrder[i] < _vm->_gameFile->_views[_view]._loops.size())
			_walking += TURNING_AROUND;
	}
}

void Character::animate(uint loopId, uint speed, uint repeat, bool noIdleOverride, uint direction) {
	if (_view < 0)
		error("Character::animate: character '%s (id %d) has no view set",
			_scriptName.c_str(), _indexId);
	if ((uint)_view >= _vm->_gameFile->_views.size())
		error("Character::animate: character '%s' (id %d) has invalid view %d (only have %d)",
			_scriptName.c_str(), _indexId, _view, _vm->_gameFile->_views.size());

	debugC(kDebugLevelGame, "character '%s' (id %d) starting animation: view %d, loop %d, speed %d, repeat %d",
		_scriptName.c_str(), _indexId, _view, _loop, speed, repeat);

	if (_idleLeft < 0 && !noIdleOverride) {
		// if idle view in progress for the character (and this is not the
		// "start idle animation" animate_character call), stop the idle anim
		unlockView();
		_idleLeft = _idleTime;
	}

	if (loopId >= _vm->_gameFile->_views[_view]._loops.size())
		error("Character::animate: character '%s' (id %d) using invalid loop %d for view %d (only have %d)",
			_scriptName.c_str(), _indexId, loopId, _view, _vm->_gameFile->_views[_view]._loops.size());

	stopMoving();
	_animating = 1;
	if (repeat)
		_animating |= CHANIM_REPEAT;
	if (direction)
		_animating |= CHANIM_BACKWARDS;

	_animating |= ((speed << 8) & 0xff00);
	_loop = loopId;

	if (direction)
		_frame = _vm->_gameFile->_views[_view]._loops[_loop]._frames.size() - 1;
	else
		_frame = 0;

	_wait = speed + _vm->_gameFile->_views[_view]._loops[_loop]._frames[_frame]._speed;
	checkViewFrame();
}

void Character::findReasonableLoop() {
	const ViewStruct &view = _vm->_gameFile->_views[_view];
	if (view._loops.empty())
		error("Character::findReasonableLoop: character '%s' (id %d) using empty view %d (no loops)",
			_scriptName.c_str(), _indexId, _view);
	if (_loop >= view._loops.size())
		_loop = 0;

	if (!view._loops[_loop]._frames.empty())
		return;

	// if the current loop has no frames, find one that does
	for (uint i = 0; i < view._loops.size(); ++i) {
		if (view._loops[i]._frames.empty())
			continue;

		_loop = i;
		return;
	}

	error("Character::findReasonableLoop: character '%s' (id %d) using empty view %d (all loops are empty)",
		_scriptName.c_str(), _indexId, _view);
}

void Character::lockView(uint viewId) {
	if (viewId < 1 || viewId > _vm->_gameFile->_views.size())
		error("Character::lockView: invalid view number %d (max is %d)", viewId, _vm->_gameFile->_views.size());
	viewId--;

	debugC(kDebugLevelGame, "character '%s' (id %d) locked to view %d",
		_scriptName.c_str(), _indexId, viewId + 1);

	if (_idleLeft < 0) {
		unlockView();
		_idleLeft = _idleTime;
	}

	stopMoving();
	_view = viewId;
	_animating = 0;
	findReasonableLoop();
	_frame = 0;
	_wait = 0;
	_flags |= CHF_FIXVIEW;
	_picXOffs = 0;
	_picYOffs = 0;
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
		/* TODO: unused: (presumably the intention was to limit the reasonable
		 * loops to only the non-diagonal ones, if the flags want that)
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
	// restart the idle animation straight away
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

void Character::setSpeechView(int view) {
	if (view == -1) {
		_talkView = -1;
		return;
	}

	if (view < 1 || (uint)view > _vm->_gameFile->_views.size())
		error("Character::setSpeechView: invalid view number %d (max is %d)", view, _vm->_gameFile->_views.size());

	_talkView = view - 1;
}

void Character::checkViewFrame() {
	// FIXME: volume stuff

	_vm->checkViewFrame(_view, _loop, _frame);
}

void Character::changeRoom(int room, int x, int y) {
	if (_indexId != _vm->_gameFile->_playerChar) {
		// NewRoomNPC
		if (x != SCR_NO_VALUE && y != SCR_NO_VALUE) {
			_x = x;
			_y = y;
		}
		_prevRoom = _room;
		_room = room;

		debugC(kDebugLevelGame, "character '%s' moved to room %d, location %d,%d",
			_scriptName.c_str(), room, _x, _y);

		return;
	}

	if (x != SCR_NO_VALUE && y != SCR_NO_VALUE) {
		_vm->_newRoomPos = 0;

		if (_vm->getGameFileVersion() <= kAGSVer272) {
			// Set position immediately on 2.x.
			// "Fixed the player looking the wrong way after entering a room in the Ben Jordan games."
			_x = x;
			_y = y;
		} else {
			// don't check X or Y bounds, so that they can do a
			// walk-in animation if they want
			_vm->_newRoomX = x;
			_vm->_newRoomY = y;
		}
	}

	_vm->scheduleNewRoom(room);
}

void Character::addInventory(uint itemId, uint addIndex) {
	assert(itemId < _vm->_gameFile->_invItemInfo.size());

	debugC(kDebugLevelGame, "adding inv %d (at %d) to character '%s' (id %d)",
		itemId, addIndex, _scriptName.c_str(), _indexId);

	if (_inventory[itemId] >= 32000)
		error("Character::addInventory: can't carry more than 32000 of one inventory item");
	_inventory[itemId]++;

	if (!_vm->getGameOption(OPT_DUPLICATEINV)) {
		// Ensure it is only in the list once
		for (uint i = 0; i < _invOrder.size(); ++i) {
			if (_invOrder[i] == itemId) {
				// They already have the item, so don't add it to the list
				if (_vm->getPlayerChar() == this)
					_vm->runOnEvent(GE_ADD_INV, itemId);
				return;
			}
		}
	}

	if (_invOrder.size() >= MAX_INVORDER)
		error("Character::addInventory: too many inventory items added, can only have 500 at one time");

	if (addIndex >= _invOrder.size()) {
		// add new item at end of list
		_invOrder.push_back(itemId);
	} else {
		// insert new item at index
		_invOrder.insert_at(addIndex, itemId);
	}

	_vm->invalidateGUI();

	if (_vm->getPlayerChar() == this)
		_vm->runOnEvent(GE_ADD_INV, itemId);
}

void Character::loseInventory(uint itemId) {
	assert(itemId < _vm->_gameFile->_invItemInfo.size());

	debugC(kDebugLevelGame, "removing inv %d from character '%s' (id %d)",
		itemId, _scriptName.c_str(), _indexId);

	if (_inventory[itemId] > 0)
		_inventory[itemId]--;

	if (_activeInv == itemId && _inventory[itemId] < 1) {
		_activeInv = (uint)-1;
		if (_vm->getPlayerChar() == this && _vm->getCursorMode() == MODE_USE)
			_vm->setCursorMode(0);
	}

	if (!_inventory[itemId] || _vm->getGameOption(OPT_DUPLICATEINV)) {
		// remove the invOrder entry, if any
		for (uint i = 0; i < _invOrder.size(); ++i) {
			if (_invOrder[i] != itemId)
				continue;

			_invOrder.remove_at(i);
			break;
		}
	}

	_vm->invalidateGUI();

	if (_vm->getPlayerChar() == this)
		_vm->runOnEvent(GE_LOSE_INV, itemId);
}

void Character::setActiveInventory(uint itemId) {
	_vm->invalidateGUI();

	if (itemId == 0) {
		_activeInv = itemId;

		if (_vm->getPlayerChar() == this && _vm->getCursorMode() == MODE_USE)
			_vm->setCursorMode(0);

		return;
	}

	assert(itemId < _vm->_gameFile->_invItemInfo.size());

	if (_inventory[itemId] < 1)
		error("setActiveInventory: character doesn't have any items with id %d", itemId);

	_activeInv = itemId;

	if (_vm->getPlayerChar() == this) {
		// if it's the player character, update mouse cursor
		// FIXME: _vm->updateInvCursor(itemId);
		_vm->setCursorMode(MODE_USE);
	}
}

byte Character::getSpeechAnimationDelay() {
	if (_vm->getGameOption(OPT_OLDTALKANIMSPD)) {
		// The talkanim property only applies to Lucasarts style speech.
		// Sierra style speech has a fixed delay of 5.
		if (_vm->getGameOption(OPT_SPEECHTYPE) == 0)
			return _vm->_state->_talkAnimSpeed;
		else
			return 5;
	}

	return _speechAnimSpeed;
}

bool Character::moveToNearestWalkableAreaWithin(int range, int step) {
	int x = _vm->convertToLowRes(_x), y = _vm->convertToLowRes(_y);
	int startX = 0, startY = 14;

	int width = _vm->convertToLowRes(_vm->getCurrentRoom()->_width);
	int height = _vm->convertToLowRes(_vm->getCurrentRoom()->_height);

	Common::Rect boundary = _vm->getCurrentRoom()->_boundary;
	boundary.left = _vm->convertToLowRes(boundary.left);
	boundary.right = _vm->convertToLowRes(boundary.right);
	boundary.top = _vm->convertToLowRes(boundary.top);
	boundary.bottom = _vm->convertToLowRes(boundary.bottom);

	// tweak because people forget to move the edges sometimes
	// if the player is already over the edge, ignore it
	if (x >= boundary.right)
		boundary.right = width;
	if (x <= boundary.left)
		boundary.left = 0;
	if (y >= boundary.bottom)
		boundary.bottom = height;
	if (y <= boundary.top)
		boundary.top = 0;

	if (range > 0) {
		startX = x - range;
		width = MIN<int>(width, startX + (range * 2));
		if (startX < 0)
			startX = 0;
		startY = y - range;
		height = MIN<int>(height, startY + (range * 2));
		if (startY < 0)
			startY = 0;
	}

	const Graphics::Surface &mask = _vm->getCurrentRoom()->_walkableMask;

	uint nearest = 99999;
	int nearestX, nearestY;

	// TODO: This could be a lot more efficient than it is, if it ever shows up in a profile enough to care.
	for (int newX = startX; newX < width; newX += step) {
		for (int newY = startY; newY < height; newY += step) {
			// if it's non-walkable, don't go there
			if (*(byte *)mask.getBasePtr(newX, newY) == 0)
				continue;

			// off a screen edge, don't move them there
			if (newX <= boundary.left || newX >= boundary.right || newY <= boundary.top || newY >= boundary.bottom)
				continue;

			// otherwise, calculate distance from target
			uint thisIs = (uint)sqrt((float)((newX - x) * (newX - x) + (newY - y) * (newY - y)));
			if (thisIs < nearest) {
				nearest = thisIs;
				nearestX = newX;
				nearestY = newY;
			}
		}
	}

	if (nearest < 99999) {
		_x = _vm->convertBackToHighRes(nearestX);
		_y = _vm->convertBackToHighRes(nearestY);
		return true;
	}

	return false;
}

void Character::moveToNearestWalkableArea() {
	int x = _vm->convertToLowRes(_x);
	int y = _vm->convertToLowRes(_y);

	byte pixValue = 0;
	if (x >= 0 && y >= 0 && x < _vm->getCurrentRoom()->_width && y < _vm->getCurrentRoom()->_height)
		pixValue = *(byte *)_vm->getCurrentRoom()->_walkableMask.getBasePtr(x, y);
	else if (_vm->getGameFileVersion() < kAGSVer261)
		return; // "only fix this code if the game was built with 2.61 or above"

	if (pixValue != 0)
		return;

	// First, check every 2 pixels within immediate area
	if (moveToNearestWalkableAreaWithin(20, 2))
		return;

	// If not, check whole screen at 5 pixel intervals
	moveToNearestWalkableAreaWithin(-1, 5);
}

Common::Point Character::getDrawPos() {
	uint spriteId = _vm->getViewFrame(_view, _loop, _frame)->_pic;

	return Common::Point(_vm->multiplyUpCoordinate(_x) - getDrawWidth()/2 + _picXOffs,
		_vm->multiplyUpCoordinate(_y) - _vm->getSprites()->getSpriteHeight(spriteId) -
		_vm->multiplyUpCoordinate(_z) + _picYOffs);
}

uint Character::getBaseline() const {
	return _baseline ? _baseline : _y;
}

int Character::getDrawOrder() const {
	return getBaseline() + ((_flags & CHF_NOWALKBEHINDS) ? _vm->getCurrentRoom()->_height : 0);
}

const Graphics::Surface *Character::getDrawSurface() {
	uint spriteId = _vm->getViewFrame(_view, _loop, _frame)->_pic;

	return _vm->getSprites()->getSprite(spriteId)->_surface; // FIXME
}

uint Character::getDrawWidth() {
	return getDrawSurface()->w; // FIXME
}

uint Character::getDrawHeight() {
	return getDrawSurface()->h; // FIXME
}

uint Character::getDrawTransparency() {
	return _transparency;
}

bool Character::isDrawMirrored() {
	return _vm->getViewFrame(_view, _loop, _frame)->_flags & VFLG_FLIPSPRITE;
}

int Character::getDrawLightLevel() {
	return 0; // FIXME
}

void Character::getDrawTint(int &lightLevel, int &luminance, byte &red, byte &green, byte &blue) {
	// FIXME
}

} // End of namespace AGS
