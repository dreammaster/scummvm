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

#ifndef AGS_PATHFINDER_H
#define AGS_PATHFINDER_H

#include "common/array.h"
#include "common/frac.h"
#include "common/rect.h"

namespace Graphics {
struct Surface;
}

namespace AGS {

struct MoveStage {
	Common::Point pos;
	frac_t xPerMove, yPerMove;
};

struct MoveList {
	Common::Array<MoveStage> _stages;
	uint _curStage;
	int _curPart;

	Common::Point _from;
	Common::Point _curPos;

	// is the move direct (i.e. ignoring walls)?
	// (if not, characters need to make sure they end up on a walkable area when done)
	bool _direct;

	// doneflag&1 == _doneX, doneflag&2 == _doneY
	bool _doneX, _doneY;

	frac_t _moveSpeedX, _moveSpeedY;

	void setRouteMoveSpeed(int x, int y);
	void convertToHighRes(uint multiplier);
	bool doStep(Common::Point &pos);
	void calculateMoveStage(uint stageId);
};

bool canSee(const Common::Point &from, const Common::Point &to, const Graphics::Surface *mask, Common::Point *lastGoodPos = NULL);
bool findPath(class AGSEngine *vm, const Common::Point &from, const Common::Point &to, const Graphics::Surface *mask,
	MoveList *moveList, int speedX, int speedY, bool onlyIfDestAllowed, bool ignoreWalls);

} // End of namespace AGS

#endif // AGS_PATHFINDER_H
