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

#include "engines/ags/pathfinder.h"
#include "engines/ags/ags.h"
#include "engines/ags/constants.h"
#include "engines/ags/gamefile.h"
#include "engines/ags/graphics.h"
#include "graphics/surface.h"

namespace AGS {

void MoveList::convertToHighRes(uint multiplier) {
	_from.x *= multiplier;
	_from.y *= multiplier;
	// (lastx/lasty unused)

	for (uint i = 0; i < _stages.size(); ++i) {
		_stages[i].pos.x *= multiplier;
		_stages[i].pos.y *= multiplier;

		_stages[i].xPerMove *= multiplier;
		_stages[i].yPerMove *= multiplier;
	}
}

bool MoveList::doStep(Common::Point &pos) {
	assert(_curStage + 1 < _stages.size());
	const MoveStage &stage = _stages[_curStage];
	const MoveStage &nextStage = _stages[_curStage + 1];

	int targetX = nextStage.pos.x;
	int targetY = nextStage.pos.y;

	// TODO: this changed in 2.70, 2.61 RC1, etc
	if (_doneX) {
		// if the X movement has finished, and the Y per move is <1, then
		// allow it to finish more easily by adjusting the target

		int adjAmnt = 3;

		// 2.70: if the X permove is also <=1, don't do the skipping
		if ((stage.xPerMove & 0xffff0000) == 0xffff0000 || (stage.xPerMove & 0xffff0000) == 0)
			adjAmnt = 2;

		// 2.61 RC1: correct this to work with > -1 as well as < 1
		if (stage.yPerMove != 0) {
			if ((stage.yPerMove & 0xffff0000) == 0) {
				// Y per move is <1, so finish the move
				targetY -= adjAmnt;
			} else if ((uint)stage.yPerMove != 0xffff0000 && (stage.yPerMove & 0xffff0000) == 0xffff0000) {
				// Y per move is > -1, so finish the move
				targetY += adjAmnt;
			}
		}
	} else
		pos.x = _from.x + (int)(fracToDouble(stage.xPerMove)*(double)_curPart);

	// same as above, for Y
	if (_doneY) {
		int adjAmnt = 3;

		if ((stage.yPerMove & 0xffff0000) == 0xffff0000 || (stage.yPerMove & 0xffff0000) == 0)
			adjAmnt = 2;

		if (stage.xPerMove != 0) {
			if ((stage.xPerMove & 0xffff0000) == 0) {
				targetX -= adjAmnt;
			} else if ((uint)stage.xPerMove != 0xffff0000 && (stage.xPerMove & 0xffff0000) == 0xffff0000) {
				targetX += adjAmnt;
			}
		}
	} else
		pos.y = _from.y + (int)(fracToDouble(stage.yPerMove)*(double)_curPart);

	warning("move stage: now %d (target %d), %d (target %d)", pos.x, targetX, pos.y, targetY);

	// did we finish horizontal movement?
	if ((stage.xPerMove > 0 && pos.x >= targetX) || (stage.xPerMove < 0 && pos.x <= targetX)) {
		_doneX = true;
		pos.x = targetX;
	} else if (stage.xPerMove == 0)
		_doneX = true;

	// did we finish vertical movement?
	if ((stage.yPerMove > 0 && pos.y >= targetY) || (stage.yPerMove < 0 && pos.y <= targetY)) {
		_doneY = true;
		pos.y = targetY;
	} else if (stage.yPerMove == 0)
		_doneY = true;

	bool ret = false;
	if (_doneX && _doneY) {
		// this stage is done, go on to the next stage
		_from = nextStage.pos;

		_curStage++;
		_curPart = -1;
		_doneX = false;
		_doneY = false;
		pos = _from;
		if (_curStage + 1 < _stages.size()) {
			ret = true;
		} else {
			// done; last stage is just the dest pos
			_stages.clear();
		}
	}

	_curPart++;
	return ret;
}

struct PathFinder {
	// the walkable mask to use
	const Graphics::Surface *_mask;
	// the list to fill
	MoveList *_moveList;
	// the destination (possibly adjusted)
	Common::Point _dest;

	frac_t _moveSpeedX, _moveSpeedY;

	// internal pathfinding state
	bool _goingLeft;
	int16 *_beenHere;

	bool _hasFinalStep;
	Common::Point _finalStep;

	// state for the canSee callback
	bool _lineFailed;

	// state needed to construct the final path
	Common::Array<Common::Point> _pathBackPositions;

	void setRouteMoveSpeed(int x, int y);

	bool findPath(const Common::Point &to, bool onlyIfDestAllowed);

	bool constructPath();
	void calculateMoveStage(uint stageId);

	bool canSee(const Common::Point &from, const Common::Point &to);
};

void PathFinder::setRouteMoveSpeed(int x, int y) {
	// // negative move speeds like -2 get converted to 1/2

	if (x < 0)
		_moveSpeedX = intToFrac(1) / (-x);
	else
		_moveSpeedX = intToFrac(x);
	if (y < 0)
		_moveSpeedY = intToFrac(1) / (-y);
	else
		_moveSpeedY = intToFrac(y);
}

bool PathFinder::findPath(const Common::Point &to, bool onlyIfDestAllowed) {
	// FIXME
	error("findPath unimplemented");
}

bool PathFinder::constructPath() {
	_moveList->_stages.clear();

	Common::Point pos = _moveList->_from;

	// Add the start stage.
	_moveList->_stages.push_back(MoveStage());
	_moveList->_stages.back().pos = pos;

	uint lastRelevantPosIndex = _pathBackPositions.size();
	while (true) {
		bool foundPos = false;
		uint nearestPosIndex;

		// find the furthest point that can be seen from this stage,
		// by walking backwards through the position list
		for (int i = lastRelevantPosIndex - 1; i >= 0; --i) {
			if (canSee(pos, _pathBackPositions[i])) {
				foundPos = true;
				nearestPosIndex = i;
			}
		}

		if (!_pathBackPositions.empty() && !foundPos) {
			// If we have a path but we didn't find any next stage we could see..
			if (!canSee(pos, _dest)) {
				// We're stuck in a corner so advance to the next square anyway.
				if (pos.x >= 0 && pos.y >= 0 && pos.x < _mask->w && pos.y < _mask->h) {
					// (but only if they're on the screen)
					foundPos = true;
					nearestPosIndex = lastRelevantPosIndex - 1;
				}
			}
		}

		if (!foundPos)
			break;

		pos = _pathBackPositions[nearestPosIndex];
		_moveList->_stages.push_back(MoveStage());
		_moveList->_stages.back().pos = pos;
	}

	if (_hasFinalStep) {
		_moveList->_stages.push_back(MoveStage());
		_moveList->_stages.back().pos = _finalStep;
	}

	// Make sure the end co-ord is in there
	if (_moveList->_stages.back().pos != _dest) {
		_moveList->_stages.push_back(MoveStage());
		_moveList->_stages.back().pos = _dest;
	}

	if (_moveList->_stages.size() == 1)
		return false;

	for (uint i = 0; i < _moveList->_stages.size() - 1; ++i)
		calculateMoveStage(i);

	return true;
}

static frac_t fracMul(frac_t a, frac_t b) {
	long long res = (long long)a * (long long)b;
	return (int)(res >> 16);
}

static frac_t fracDiv(frac_t a, frac_t b) {
	return doubleToFrac(fracToDouble(a) / fracToDouble(b));
}

// Calculate the x/y movement per frame, for this stage (moving to the one after it) of the movelist.
void PathFinder::calculateMoveStage(uint stageId) {
	MoveStage &stage = _moveList->_stages[stageId];
	MoveStage &nextStage = _moveList->_stages[stageId + 1];

	if (stage.pos == nextStage.pos) {
		stage.xPerMove = 0;
		stage.yPerMove = 0;
		return;
	}

	if (stage.pos.x == nextStage.pos.x) {
		// special case for vertical movement
		stage.xPerMove = 0;
		if (nextStage.pos.y < stage.pos.y)
			stage.yPerMove = -_moveSpeedY;
		else
			stage.yPerMove = _moveSpeedY;
		return;
	} else if (stage.pos.y == nextStage.pos.y) {
		// special case for horizontal movement
		if (nextStage.pos.x < stage.pos.x)
			stage.xPerMove = -_moveSpeedX;
		else
			stage.xPerMove = _moveSpeedX;
		stage.yPerMove = 0;
		return;
	}

	int xDist = abs(stage.pos.x - nextStage.pos.x);
	int yDist = abs(stage.pos.y - nextStage.pos.y);

	frac_t useMoveSpeed = _moveSpeedX;
	if (_moveSpeedX != _moveSpeedY) {
		// different X and Y move speeds
		// the X proportion of the movement is (x / (x + y))
		frac_t xProportion = fracDiv(xDist, xDist + yDist);

		if (_moveSpeedX > _moveSpeedY) {
			// speed = y + ((1 - xproportion) * (x - y))
			useMoveSpeed = _moveSpeedY + fracMul(xProportion, _moveSpeedX - _moveSpeedY);
		} else {
			// speed = x + (xproportion * (y - x))
			useMoveSpeed = _moveSpeedX + fracMul(intToFrac(1) - xProportion, _moveSpeedY - _moveSpeedX);
		}
	}

	// First, opp/adj=tan, so work out the angle
	// angle = atan(yDist / xDist)
	double angle = atan(fracToDouble(fracDiv(yDist, xDist)));

	// now, since new opp=hyp*sin, work out the Y step size
	frac_t newYMove = fracMul(useMoveSpeed, doubleToFrac(sin(angle)));
	if (nextStage.pos.y < stage.pos.y)
		newYMove = -newYMove;
	// since adj=hyp*cos, work out X step size
	frac_t newXMove = fracMul(useMoveSpeed, doubleToFrac(cos(angle)));
	if (nextStage.pos.x < stage.pos.x)
		newXMove = -newXMove;

	stage.xPerMove = newXMove;
	stage.yPerMove = newYMove;
}

#define BITMAP PathFinder
// The following function is copied (without change) from Allegro 4.4.2.

/* do_line:
 *  Calculates all the points along a line between x1, y1 and x2, y2, 
 *  calling the supplied function for each one. This will be passed a 
 *  copy of the bmp parameter, the x and y position, and a copy of the 
 *  d parameter (so do_line() can be used with putpixel()).
 */
void do_line(BITMAP *bmp, int x1, int y1, int x2, int y2, int d, void (*proc)(BITMAP *, int, int, int))
{
   int dx = x2-x1;
   int dy = y2-y1;
   int i1, i2;
   int x, y;
   int dd;

   /* worker macro */
   #define DO_LINE(pri_sign, pri_c, pri_cond, sec_sign, sec_c, sec_cond)     \
   {                                                                         \
      if (d##pri_c == 0) {                                                   \
	 proc(bmp, x1, y1, d);                                               \
	 return;                                                             \
      }                                                                      \
									     \
      i1 = 2 * d##sec_c;                                                     \
      dd = i1 - (sec_sign (pri_sign d##pri_c));                              \
      i2 = dd - (sec_sign (pri_sign d##pri_c));                              \
									     \
      x = x1;                                                                \
      y = y1;                                                                \
									     \
      while (pri_c pri_cond pri_c##2) {                                      \
	 proc(bmp, x, y, d);                                                 \
									     \
	 if (dd sec_cond 0) {                                                \
	    sec_c = sec_c sec_sign 1;                                        \
	    dd += i2;                                                        \
	 }                                                                   \
	 else                                                                \
	    dd += i1;                                                        \
									     \
	 pri_c = pri_c pri_sign 1;                                           \
      }                                                                      \
   }

   if (dx >= 0) {
      if (dy >= 0) {
	 if (dx >= dy) {
	    /* (x1 <= x2) && (y1 <= y2) && (dx >= dy) */
	    DO_LINE(+, x, <=, +, y, >=);
	 }
	 else {
	    /* (x1 <= x2) && (y1 <= y2) && (dx < dy) */
	    DO_LINE(+, y, <=, +, x, >=);
	 }
      }
      else {
	 if (dx >= -dy) {
	    /* (x1 <= x2) && (y1 > y2) && (dx >= dy) */
	    DO_LINE(+, x, <=, -, y, <=);
	 }
	 else {
	    /* (x1 <= x2) && (y1 > y2) && (dx < dy) */
	    DO_LINE(-, y, >=, +, x, >=);
	 }
      }
   }
   else {
      if (dy >= 0) {
	 if (-dx >= dy) {
	    /* (x1 > x2) && (y1 <= y2) && (dx >= dy) */
	    DO_LINE(-, x, >=, +, y, >=);
	 }
	 else {
	    /* (x1 > x2) && (y1 <= y2) && (dx < dy) */
	    DO_LINE(+, y, <=, -, x, <=);
	 }
      }
      else {
	 if (-dx >= -dy) {
	    /* (x1 > x2) && (y1 > y2) && (dx >= dy) */
	    DO_LINE(-, x, >=, -, y, <=);
	 }
	 else {
	    /* (x1 > x2) && (y1 > y2) && (dx < dy) */
	    DO_LINE(-, y, >=, -, x, <=);
	 }
      }
   }

   #undef DO_LINE
}

// Mark a line as failed if it goes outside the mask boundaries,
// or if the mask is zero (blocked/unwalkable).
void lineCallback(PathFinder *pathfinder, int x, int y, int) {
	if (x < 0 || y < 0 || x >= pathfinder->_mask->w || y >= pathfinder->_mask->h)
		pathfinder->_lineFailed = true;
	else if (*(byte *)pathfinder->_mask->getBasePtr(x, y) == 0)
		pathfinder->_lineFailed = true;
}

bool PathFinder::canSee(const Common::Point &from, const Common::Point &to) {
	_lineFailed = false;

	if (from == to)
		return true;

	do_line(this, from.x, from.y, to.x, to.y, 0, lineCallback);

	return !_lineFailed;
}

bool findPath(AGSEngine *vm, const Common::Point &from, const Common::Point &to, const Graphics::Surface *mask,
	MoveList *moveList, int speedX, int speedY, bool onlyIfDestAllowed, bool ignoreWalls) {

	// Reset the state of the move list.
	moveList->_stages.clear();
	moveList->_from = from;
	moveList->_curStage = 0;
	moveList->_curPart = 0;
	moveList->_doneX = false;
	moveList->_doneY = false;

	// Construct a pathfinder.
	PathFinder pathfinder;
	pathfinder._mask = mask;
	pathfinder._moveList = moveList;
	pathfinder._dest = to;
	pathfinder._beenHere = new int16[mask->w * mask->h];
	pathfinder._goingLeft = false;
	pathfinder._hasFinalStep = false;

	pathfinder.setRouteMoveSpeed(speedX, speedY);

	// Find a path.
	bool foundPath = true;
	/* FIXME if (!ignoreWalls && !pathfinder.canSee(from, to)) {
		if (!pathfinder.findPath(to, onlyIfDestAllowed)) {
			pathfinder._goingLeft = true;
			if (!pathfinder.findPath(to, onlyIfDestAllowed)) {
				// give up
				foundPath = false;
			}
		}
	} */

	// Construct the path.
	if (foundPath)
		foundPath = pathfinder.constructPath();

	if (vm->getGameOption(OPT_NATIVECOORDINATES) && vm->_gameFile->_defaultResolution > 2)
		moveList->convertToHighRes(vm->_graphics->_screenResolutionMultiplier);

	// Done!
	delete[] pathfinder._beenHere;
	return foundPath;
}

} // End of namespace AGS
