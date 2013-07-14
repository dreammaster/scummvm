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
#include "common/queue.h"
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

void MoveList::setRouteMoveSpeed(int x, int y) {
	// negative move speeds like -2 get converted to 1/2

	if (x < 0)
		_moveSpeedX = intToFrac(1) / (-x);
	else
		_moveSpeedX = intToFrac(x);
	if (y < 0)
		_moveSpeedY = intToFrac(1) / (-y);
	else
		_moveSpeedY = intToFrac(y);
}

struct PathFinder {
	// the walkable mask to use
	const Graphics::Surface *_mask;
	// the list to fill
	MoveList *_moveList;
	// the destination (possibly adjusted)
	Common::Point _dest;

	// internal pathfinding state
	Graphics::Surface _beenHere;
	Common::Array<int> _walkAreaGranularity;

	bool _hasFinalStep;
	Common::Point _finalStep;

	// state needed to construct the final path
	Common::Array<Common::Point> _pathBackPositions;

	bool findPath(bool onlyIfDestAllowed);

	bool constructPath();

protected:
	Common::Point roundDownCoordinates(Common::Point pos);
	bool isRoutePossible(bool &foundNewCandidate);
	bool findRouteDijkstra();
	bool tryThisSquare(const Common::Point &from, const Common::Point &to, bool goingRight);

	bool dijkstraCheckNeighbor(uint x, uint y, uint nX, uint nY, int modifier, int &min, Common::Array<uint> &found, Common::Array<uint> &cheapest);
	bool findNearestWalkableArea(const Graphics::Surface &tempMask, int startX, int startY, int endX, int endY, uint step);
};

bool PathFinder::findPath(bool onlyIfDestAllowed) {
	// FIXME: bad name + unused
	if (!onlyIfDestAllowed) {
		if (*(const byte *)_mask->getBasePtr(_dest.x, _dest.y) == 0)
			return false;
	}

	const Common::Point &from = _moveList->_from;

	// vertical, horizontal or exactly diagonal?
	bool isStraight = ((from.x == _dest.x) || (from.y == _dest.y) || (abs(from.x - _dest.x) == abs(from.y - _dest.y)));

	if (from == _dest)
		return true;

	bool foundNewCandidate;
	if (!isRoutePossible(foundNewCandidate) && (!foundNewCandidate))
		return false;

	if (from == _dest)
		return true;

	// TODO: this is annotated "don't use new algo on arrow key presses", maybe be smarter for that use case..
	//if (!isStraight && findRouteDijkstra())
	if (findRouteDijkstra())
		return true;

	// if the new pathfinder failed, try the old one..
	_pathBackPositions.clear();
	memset(_beenHere.getPixels(), 0, _beenHere.w * _beenHere.h * 2);
	if (tryThisSquare(from, _dest, true))
		return true;
	// .. and again, in the other direction.
	_pathBackPositions.clear();
	memset(_beenHere.getPixels(), 0, _beenHere.w * _beenHere.h * 2);
	if (tryThisSquare(from, _dest, false))
		return true;

	// No possible path found.
	return false;
}

bool PathFinder::findNearestWalkableArea(const Graphics::Surface &tempMask, int startX, int startY, int endX, int endY, uint step) {
	startX = MAX<int>(0, startX);
	startY = MAX<int>(0, startY);
	endX = MIN<int>(endX, tempMask.w - 1);
	endY = MIN<int>(endY, tempMask.h - 1);

	uint nearest = 99999;
	Common::Point best;
	for (int x = startX; x < endX; x += step) {
		for (int y = startY; y < endY; y += step) {
			if (*(const byte *)tempMask.getBasePtr(x, y) != 232)
				continue;

			uint distance = (uint)sqrt((x - _dest.x) * (x - _dest.x) + (y - _dest.y) * (y - _dest.y));
			if (distance < nearest) {
				nearest = distance;
				best = Common::Point(x, y);
			}
		}
	}

	if (nearest == 99999)
		return false;

	_dest = best;
	return true;
}

static void floodFill(Graphics::Surface &surf, uint x, uint y, byte color) {
	// TODO: Make this more efficient.

	byte replace = *(byte *)surf.getBasePtr(x, y);
	assert(replace != color);

	Common::Queue<Common::Point> _queue;
	_queue.push(Common::Point(x, y));

	while (!_queue.empty()) {
		Common::Point point = _queue.pop();
		x = point.x;
		y = point.y;
		if (*(byte *)surf.getBasePtr(x, y) == color)
			continue;
		*(byte *)surf.getBasePtr(x, y) = color;

		if (x > 0 && *(byte *)surf.getBasePtr(x - 1, y) == replace)
			_queue.push(Common::Point(x - 1, y));
		if (x + 1 < surf.w && *(byte *)surf.getBasePtr(x + 1, y) == replace)
			_queue.push(Common::Point(x + 1, y));
		if (y > 0 && *(byte *)surf.getBasePtr(x, y - 1) == replace)
			_queue.push(Common::Point(x, y - 1));
		if (y + 1 < surf.h && *(byte *)surf.getBasePtr(x, y + 1) == replace)
			_queue.push(Common::Point(x, y + 1));
	}
}

const int MAX_GRANULARITY = 3;

// First, prepare our internal temporary walkable mask. Then, check if there's
// a possible path, by flood-filling from the source and making sure the
// destination gets filled. If it doesn't, try finding a nearby point which was.
bool PathFinder::isRoutePossible(bool &foundNewCandidate) {
	foundNewCandidate = false;

	// If we're not *starting* from a walkable position, this will never work.
	if (*(const byte *)_mask->getBasePtr(_moveList->_from.x, _moveList->_from.y) == 0) {
		warning("refusing to route from unwalkable point %d,%d", _moveList->_from.x, _moveList->_from.y);
		return false;
	}

	Graphics::Surface tempMask;
	tempMask.copyFrom(*_mask);

	_walkAreaGranularity.resize(MAX_WALK_AREAS + 1);
	uint walkAreaTimes[MAX_WALK_AREAS + 1];
	for (uint i = 0; i < MAX_WALK_AREAS + 1; ++i) {
		_walkAreaGranularity[i] = 0;
		walkAreaTimes[i] = 0;
	}

	// calculate the maximum 'size' of each area
	// TODO: shouldn't this really stop at the end of each row/column? and reset between orientations?
	uint prevAreaType = 0;
	uint inARow = 0;
	for (uint y = 0; y < tempMask.h; ++y) {
		for (uint x = 0; x < tempMask.w; ++x) {
			uint areaType = *(byte *)tempMask.getBasePtr(x, y);
			// TODO: verify the walkable mask before we get here, error check here would be silly
			if (areaType == prevAreaType && inARow > 0)
				inARow++;
			else if (prevAreaType != 0) {
				_walkAreaGranularity[prevAreaType] += inARow;
				walkAreaTimes[prevAreaType]++;
				inARow = 0;
			}
			prevAreaType = areaType;
		}
	}
	for (uint x = 0; x < tempMask.w; ++x) {
		for (uint y = 0; y < tempMask.h; ++y) {
			uint areaType = *(byte *)tempMask.getBasePtr(x, y);
			// overwrite the walkable areas with a uniform color
			if (areaType)
				*(byte *)tempMask.getBasePtr(x, y) = 1;
			if (areaType == prevAreaType && inARow > 0)
				inARow++;
			else if (prevAreaType != 0) {
				_walkAreaGranularity[prevAreaType] += inARow;
				walkAreaTimes[prevAreaType]++;
				inARow = 0;
			}
			prevAreaType = areaType;
		}
	}

	// find the average "width" of a path in this walkable area
	_walkAreaGranularity[0] = MAX_GRANULARITY;
	for (uint i = 1; i <= MAX_WALK_AREAS; ++i) {
		if (!walkAreaTimes[i]) {
			// We didn't encounter *any* (useful) walkable areas of this type.
			_walkAreaGranularity[i] = MAX_GRANULARITY;
			continue;
		}

		_walkAreaGranularity[i] /= walkAreaTimes[i];
		if (_walkAreaGranularity[i] <= 4)
			_walkAreaGranularity[i] = 2;
		else if (_walkAreaGranularity[i] <= 15)
			_walkAreaGranularity[i] = 3;
		else
			_walkAreaGranularity[i] = MAX_GRANULARITY;
	}

	bool found = true;

	floodFill(tempMask, _moveList->_from.x, _moveList->_from.y, 232);
	if (*(byte *)tempMask.getBasePtr(_dest.x, _dest.y) != 232) {
		// destination pixel is not reachable
		found = false;
		foundNewCandidate = true;

		warning("%d, %d not reachable", _dest.x, _dest.y);

		// try 100x100 square around the target, at 3-pixel granularity
		if (!findNearestWalkableArea(tempMask, _dest.x - 50, _dest.y - 50, _dest.x + 50, _dest.y + 50, 3)) {
			// then sweep the whole room at 5-pixel granularity
			if (!findNearestWalkableArea(tempMask, 0, 0, tempMask.w, tempMask.h, 5))
				foundNewCandidate = false;
		}

		if (foundNewCandidate)
			warning("now using %d, %d", _dest.x, _dest.y);
	}

	tempMask.free();
	return found;
}

// Round down the supplied co-ordinates to the area granularity,
// and move a bit if this causes them to become non-walkable
Common::Point PathFinder::roundDownCoordinates(Common::Point pos) {
	int startGranularity = _walkAreaGranularity[*(const byte *)_mask->getBasePtr(pos.x, pos.y)];

	pos.y = pos.y - pos.y % startGranularity;
	if (pos.y < 0)
		pos.y = 0;
	pos.x = pos.x - pos.x % startGranularity;
	if (pos.x < 0)
		pos.x = 0;

	// We try one step to the right, one step down, then one step back left.
	if (*(const byte *)_mask->getBasePtr(pos.x, pos.y) == 0) {
		pos.x += startGranularity;
		if ((pos.y < _mask->h - startGranularity) && *(const byte *)_mask->getBasePtr(pos.x, pos.y) == 0) {
			pos.y += startGranularity;
			if (*(const byte *)_mask->getBasePtr(pos.x, pos.y) == 0)
				pos.x -= startGranularity;
		}
	}

	return pos;
}

// replaces the 'CHECK_MIN' macro in the original:
// Update our state to account for the specified cell and adjacent neighbor,
// and return true if we bothered making the check.
bool PathFinder::dijkstraCheckNeighbor(uint x, uint y, uint nX, uint nY, int modifier, int &min, Common::Array<uint> &found, Common::Array<uint> &cheapest) {
	// Neighbor finished already?
	if (*(const int16 *)_beenHere.getBasePtr(nX, nY) != -1)
		return false;

	// Neighbor not walkable?
	if (*(const byte *)_mask->getBasePtr(nX, nY) == 0)
		return true;

	// Cost of the cell?
	int ourValue = *(int16 *)_beenHere.getBasePtr(x, y) + modifier;

	// More expensive than what we already saw?
	if (ourValue > min)
		return true;

	if (ourValue < min) {
		// New minimum path length found. Clear any found nodes.
		min = ourValue;
		found.clear();
		cheapest.clear();
	}

	// TODO: Agh, this is actually necessary, otherwise it spends forever checking the same cells.
	if (found.size() >= 40)
		return true;

	// Add the neighbor and cell to the lists, if we're the cheapest path to it so far.
	found.push_back(nX + (nY * _beenHere.w));
	cheapest.push_back(x + (y * _beenHere.w));

	return true;
}

// Try to find a route using A*.
bool PathFinder::findRouteDijkstra() {
	Common::Point from = roundDownCoordinates(_moveList->_from);

	// already at destination, once adjusted?
	if (from == roundDownCoordinates(_dest))
		return true;

	memset(_beenHere.getPixels(), 0xff, _beenHere.w * _beenHere.h * 2);
	*(int16 *)_beenHere.getBasePtr(from.x, from.y) = 0;

	// We store the previously-visited 'parent' cells here, for later tracing back.
	Graphics::Surface parent;
	parent.create(_beenHere.w, _beenHere.h, Graphics::PixelFormat(4, 0, 0, 0, 0, 0, 0, 0, 0));
	uint *parentVals = (uint *)parent.getPixels();

	// This is a list of the visited cell positions, in order.
	uint visited[5000];
	visited[0] = (from.y * parent.w) + from.x;
	parentVals[visited[0]] = (uint)-1;

	uint iteration = 1;
	uint totalFound = 0;
	int directionBonus = 0;

	uint foundAnswer = (uint)-1;
	while (iteration < 5000 && foundAnswer == (uint)-1) {
		// TODO: Make these a single array?
		Common::Array<uint> found;
		Common::Array<uint> cheapest;

		Common::Array<uint> replace;

		int min = 29999;
		uint changeIter = iteration;

		// Update each visited cell, to take into account newly-visited neighbours.
		for (uint i = 0; i < iteration; ++i) {
			// Skip already-exhausted cells.
			if (visited[i] == (uint)-1)
				continue;

			int x = visited[i] % parent.w;
			int y = visited[i] / parent.w;
			int granularity = _walkAreaGranularity[*(const byte *)_mask->getBasePtr(x, y)];

			bool updated = false;

			if (x >= granularity)
				updated |= dijkstraCheckNeighbor(x, y, x - granularity, y, (_dest.x < x) ? directionBonus : 0,
					min, found, cheapest);

			if (y >= granularity)
				updated |= dijkstraCheckNeighbor(x, y, x, y - granularity, (_dest.y < y) ? directionBonus : 0,
					min, found, cheapest);

			if (x < _mask->w - granularity)
				updated |= dijkstraCheckNeighbor(x, y, x + granularity, y, (_dest.x > x) ? directionBonus : 0,
					min, found, cheapest);

			if (y < _mask->h - granularity)
				updated |= dijkstraCheckNeighbor(x, y, x, y + granularity, (_dest.y > y) ? directionBonus : 0,
					min, found, cheapest);

			if (!updated) {
				// If all the adjacent cells have been done, stop checking this one
				visited[replace.size()] = (uint)-1;
				assert(i < 5000);
				replace.push_back(i);
			}
		}

		if (found.empty()) {
			// No further candidates, give up.
			parent.free();
			return false;
		}

		totalFound += found.size();

		for (uint i = 0; i < found.size(); ++i) {
			int newX = found[i] % _mask->w;
			int newY = found[i] / _mask->w;

			assert(found[i] != cheapest[i]);

			// Cost of this cell = cost of cheapest adjacent neighbor + 1.
			uint cheapestX = cheapest[i] % _mask->w;
			uint cheapestY = cheapest[i] / _mask->w;
			*(int16 *)_beenHere.getBasePtr(newX, newY) = *(int16 *)_beenHere.getBasePtr(cheapestX, cheapestY) + 1;

			// And that neighbor is our 'parent'.
			parentVals[found[i]] = cheapest[i];

			// The edges of the screen pose a problem, so if the current position
			// and the destination are both within a certain distance of the edge,
			// just snap to the destination.
			if ((newX >= _mask->w - MAX_GRANULARITY) && (_dest.x >= _mask->w - MAX_GRANULARITY))
				newX = _dest.x;
			if ((newY >= _mask->h - MAX_GRANULARITY) && (_dest.y >= _mask->h - MAX_GRANULARITY))
				newY = _dest.y;

			if ((newX >= _dest.x - MAX_GRANULARITY) && (newX <= _dest.x + MAX_GRANULARITY) &&
				(newY >= _dest.y - MAX_GRANULARITY) && (newY <= _dest.y + MAX_GRANULARITY)) {
				// We're close enough to the destination, hoorah, done!
				foundAnswer = found[i];
				break;
			}

			if (totalFound >= 1000) {
				// Ever so often, check if we can see the destination.
				// TODO: This seems silly, and original says "Doesn't work cos it
				// can see the destination from the point that's not nearest".
				if (canSee(Common::Point(newX, newY), _dest, _mask)) {
					directionBonus -= 50;
					totalFound = 0;
				}
			}

			if (!replace.empty()) {
				changeIter = replace.back();
				replace.pop_back();
			} else
				changeIter = iteration;

			visited[changeIter] = found[i];
			if (changeIter == iteration)
				iteration++;

			changeIter = iteration;

			// Make sure we don't violate the outer loop condition.
			if (iteration >= 5000)
				break;
		}

		if (totalFound >= 1000)
			totalFound = 0;
	}

	if (iteration >= 5000) {
		// Too many iterations, give up.
		parent.free();
		return false;
	}

	_pathBackPositions.push_back(_dest);

	for (uint on = parentVals[foundAnswer]; on != (uint)-1; on = parentVals[on]) {
		assert(parentVals[on] != on);

		int newX = on % _mask->w;
		int newY = on / _mask->w;

		// Done?
		if ((newX >= _dest.x - MAX_GRANULARITY) && (newX <= _dest.x + MAX_GRANULARITY) &&
			(newY >= _dest.y - MAX_GRANULARITY) && (newY <= _dest.y + MAX_GRANULARITY))
			break;

		_pathBackPositions.push_back(Common::Point(newX, newY));
	}

	parent.free();
	return true;
}

bool PathFinder::tryThisSquare(const Common::Point &from, const Common::Point &to, bool goingRight) {
	return false; // FIXME
}

bool PathFinder::constructPath() {
	_moveList->_stages.clear();

	Common::Point pos = _moveList->_from;

	// Add the start stage.
	_moveList->_stages.push_back(MoveStage());
	_moveList->_stages.back().pos = pos;

	uint lastRelevantPosIndex = _pathBackPositions.size();
	while (lastRelevantPosIndex != 0) {
		bool foundPos = false;
		uint nearestPosIndex;

		// find the furthest point that can be seen from this stage,
		// by walking backwards through the position list
		for (int i = lastRelevantPosIndex - 1; i >= 0; --i) {
			if (canSee(pos, _pathBackPositions[i], _mask)) {
				foundPos = true;
				nearestPosIndex = i;
			}
		}

		if (!foundPos && lastRelevantPosIndex != 0) {
			// If we have a path but we didn't find any next stage we could see..
			if (!canSee(pos, _dest, _mask)) {
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
		lastRelevantPosIndex = nearestPosIndex;
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

	// If we have a single step and the starting point is destination, done.
	// (The alternative for a single step is that the destination was modified.)
	if (_moveList->_stages.size() == 1 && _moveList->_from == _dest)
		return false;

	for (uint i = 0; i < _moveList->_stages.size() - 1; ++i)
		_moveList->calculateMoveStage(i);

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
void MoveList::calculateMoveStage(uint stageId) {
	MoveStage &stage = _stages[stageId];
	MoveStage &nextStage = _stages[stageId + 1];

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

struct CanSeeInfo {
	const Graphics::Surface *mask;
	bool lineFailed;
	Common::Point *lastGoodPos;
};
#define BITMAP CanSeeInfo
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
void lineCallback(CanSeeInfo *info, int x, int y, int) {
	if (x < 0 || y < 0 || x >= info->mask->w || y >= info->mask->h)
		info->lineFailed = true;
	else if (*(const byte *)info->mask->getBasePtr(x, y) == 0)
		info->lineFailed = true;
	else if (!info->lineFailed && info->lastGoodPos) {
		info->lastGoodPos->x = x;
		info->lastGoodPos->y = y;
	}
}

bool canSee(const Common::Point &from, const Common::Point &to, const Graphics::Surface *mask, Common::Point *lastGoodPos) {
	if (from == to)
		return true;

	CanSeeInfo info;
	info.mask = mask;
	info.lineFailed = false;
	info.lastGoodPos = lastGoodPos;

	do_line(&info, from.x, from.y, to.x, to.y, 0, lineCallback);

	return !info.lineFailed;
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
	moveList->setRouteMoveSpeed(speedX, speedY);

	// Construct a pathfinder.
	PathFinder pathfinder;
	pathfinder._mask = mask;
	pathfinder._moveList = moveList;
	pathfinder._dest = to;
	pathfinder._beenHere.create(mask->w, mask->h, Graphics::PixelFormat(2, 0, 0, 0, 0, 0, 0, 0, 0));
	pathfinder._hasFinalStep = false;

	// Find a path.
	bool foundPath = true;
	if (!ignoreWalls && !canSee(from, to, mask)) {
		if (!pathfinder.findPath(onlyIfDestAllowed)) {
			// give up
			foundPath = false;
		}
	}

	// Construct the path.
	if (foundPath)
		foundPath = pathfinder.constructPath();

	if (vm->getGameOption(OPT_NATIVECOORDINATES) && vm->_gameFile->_defaultResolution > 2)
		moveList->convertToHighRes(vm->_graphics->_screenResolutionMultiplier);

	// Done!
	pathfinder._beenHere.free();
	return foundPath;
}

} // End of namespace AGS
