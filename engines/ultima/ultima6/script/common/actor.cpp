/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "ultima/ultima6/core/game.h"
#include "ultima/ultima6/actors/actor_manager.h"
#include "ultima/ultima6/core/map.h"
#include "ultima/ultima6/core/obj_manager.h"
#include "ultima/ultima6/core/tile_manager.h"
#include "ultima/ultima6/gui/widgets/map_window.h"
#include "ultima/ultima6/misc/u6_llist.h"
#include "ultima/ultima6/core/player.h"
#include "ultima/ultima6/script/common/game_scripts.h"
#include "ultima/ultima6/script/common/script_random.h"

namespace Ultima {
namespace Ultima6 {
namespace Scripts {

void GameScripts::getObjsAt(int x, int y, int z, Common::Array<Obj *> &objs) {
	if (x < 0 || x >= 1024 || y < 0 || y >= 1024 || z < 0 || z > 5)
		return;

	U6LList *list = Game::get_game()->get_obj_manager()->get_obj_list(x, y, z);
	if (!list)
		return;

	for (U6Link *link = list->start(); link; link = link->next)
		objs.push_back((Obj *)link->data);
}

void GameScripts::getObjsInArea(int x, int y, int z, int width, int height, Common::Array<Obj *> &objs) {
	for (int yOffset = 0; yOffset < height; ++yOffset) {
		for (int xOffset = 0; xOffset < width; ++xOffset)
			getObjsAt(wrap_signed_coord(x + xOffset, z), wrap_signed_coord(y + yOffset, z), z, objs);
	}
}

Obj *GameScripts::mapGetObj(int x, int y, int z, int objN, bool includeMultiTile) {
	ObjManager *objManager = Game::get_game()->get_obj_manager();
	uint16 wx = wrap_signed_coord(x, z);
	uint16 wy = wrap_signed_coord(y, z);

	if (objN < 0)
		return objManager->get_obj(wx, wy, z);

	if (includeMultiTile)
		return objManager->get_obj_of_type_from_location_inc_multi_tile(objN, wx, wy, z);

	return objManager->get_obj_of_type_from_location(objN, wx, wy, z);
}

Actor *GameScripts::mapGetActor(int x, int y, int z, Actor *excluded) {
	return Game::get_game()->get_actor_manager()->get_actor(wrap_signed_coord(x, z), wrap_signed_coord(y, z), z, true, excluded);
}

bool GameScripts::mapIsOnScreen(int x, int y, int z) {
	return Game::get_game()->get_map_window()->is_on_screen(wrap_signed_coord(x, z), wrap_signed_coord(y, z), z);
}

const char *GameScripts::tileDescription(uint16 tileNum) {
	return Game::get_game()->get_tile_manager()->lookAtTile(tileNum, 1, false);
}

void GameScripts::setObjLocation(Obj *obj, uint16 x, uint16 y, uint8 z) {
	if (obj->is_on_map()) {
		Game::get_game()->get_obj_manager()->move(obj, x, y, z);
	} else {
		obj->x = x;
		obj->y = y;
		obj->z = z;
	}
}

bool GameScripts::tileGetFlag(uint16 tileNum, uint8 flagSet, uint8 bit) {
	Tile *tile = Game::get_game()->get_tile_manager()->get_original_tile(tileNum);
	if (!tile || flagSet < 1 || flagSet > 3 || bit > 7)
		return false;

	uint8 flags = (flagSet == 1) ? tile->flags1 : (flagSet == 2) ? tile->flags2 : tile->flags3;
	return (flags & (1 << bit)) != 0;
}

uint16 GameScripts::objTileNum(const Obj *obj) {
	return Game::get_game()->get_obj_manager()->get_obj_tile(obj->obj_n, obj->frame_n)->tile_num;
}

uint16 GameScripts::getWrappedDist(uint16 pt1, uint16 pt2) {
	uint16 diff = (pt2 >= pt1) ? pt2 - pt1 : pt1 - pt2;

	if (diff > 512)
		diff = 1024 - diff;

	return diff;
}

int GameScripts::actorRandomiseStat(int baseStat) {
	int half = baseStat / 2;
	if (half == 0)
		return baseStat;

	return randomRange(0, half) + randomRange(0, half) + baseStat - half;
}

bool GameScripts::actorHasBadAlignment(const Actor *actor) {
	ActorAlignment alignment = actor->get_alignment();
	return alignment == ACTOR_ALIGNMENT_EVIL || alignment == ACTOR_ALIGNMENT_CHAOTIC;
}

bool GameScripts::actorHasFreeArm(Actor *actor) {
	return actor->inventory_get_readied_obj_n(ACTOR_ARM) == -1 ||
		actor->inventory_get_readied_obj_n(ACTOR_ARM_2) == -1;
}

bool GameScripts::actorIsHoldingObj(Actor *actor, uint16 objN) {
	return actor->inventory_get_readied_obj_n(ACTOR_HAND) == objN ||
		actor->inventory_get_readied_obj_n(ACTOR_HAND_2) == objN;
}

int GameScripts::actorFindMaxWrappedXYDistance(const Actor *actor, uint16 x, uint16 y) {
	int dx = getWrappedDist(actor->get_x(), x);
	int dy = getWrappedDist(actor->get_y(), y);

	return MAX(dx, dy);
}

uint8 GameScripts::actorGetCombatRange(const Actor *actor, uint16 targetX, uint16 targetY) const {
	return getCombatRange(getWrappedDist(actor->get_x(), targetX), getWrappedDist(actor->get_y(), targetY));
}

void GameScripts::subtractMapMovementPts(Actor *actor) {
	Map *map = Game::get_game()->get_game_map();
	int points = map->get_impedance(actor->get_x(), actor->get_y(), actor->get_z(), false) + 5;

	actorSubtractMovementPoints(actor, points);
}

int GameScripts::actorMoveTowardsLoc(Actor *actor, int mapX, int mapY) {
	const int flag = 1;
	int result = 1;
	int diffX = mapX - actor->get_x();
	int diffY = mapY - actor->get_y();

	if (diffX == 0 && diffY == 0) {
		actorSubtractMovementPoints(actor, 5);
		return 0;
	}

	NuvieDir xDirection, yDirection;

	if (diffX != 0)
		xDirection = (diffX >= 0) ? NUVIE_DIR_E : NUVIE_DIR_W;
	else
		xDirection = (randomRange(0, 1) == 0) ? NUVIE_DIR_W : NUVIE_DIR_E;

	if (diffY != 0)
		yDirection = (diffY >= 0) ? NUVIE_DIR_S : NUVIE_DIR_N;
	else
		yDirection = (randomRange(0, 1) == 0) ? NUVIE_DIR_S : NUVIE_DIR_N;

	int absX = ABS(diffX);
	int absY = ABS(diffY);
	bool moveVertically;

	if (absX >= 4 || absY >= 4)
		moveVertically = randomRange(1, absX + absY) > absX;
	else if (absX > absY)
		moveVertically = false;
	else if (absX < absY)
		moveVertically = true;
	else
		moveVertically = randomRange(0, 1) == 1;

	if (!moveVertically) {
		if (!actorMove(actor, xDirection, flag)) {
			if (actorMoveDiagonal(actor, xDirection, yDirection) == 0) {
				if (!actorMove(actor, yDirection, flag)) {
					if (randomRange(0, 1) != 0 || !actorMove(actor, (yDirection == NUVIE_DIR_N) ? NUVIE_DIR_S : NUVIE_DIR_N, 1)) {
						subtractMapMovementPts(actor);
						result = 0;
					}
				}
			}
		}
	} else {
		if (!actorMove(actor, yDirection, flag)) {
			if (actorMoveDiagonal(actor, xDirection, yDirection) == 0) {
				if (!actorMove(actor, xDirection, flag)) {
					if (randomRange(0, 1) != 0 || !actorMove(actor, (xDirection == NUVIE_DIR_E) ? NUVIE_DIR_W : NUVIE_DIR_E, 1)) {
						subtractMapMovementPts(actor);
						result = 0;
					}
				}
			}
		}
	}

	return result;
}

bool GameScripts::tossActor(Actor *actor, int fromX, int fromY, uint8 fromZ, int arg0) {
	MapCoord playerLoc = Game::get_game()->get_player()->get_actor()->get_location();

	for (int i = 0; i < 8; ++i) {
		int newX = randomRange(1, 4) + randomRange(1, 4) + fromX - 5;
		int newY = randomRange(1, 4) + randomRange(1, 4) + fromY - 5;

		if (arg0 == 0 || playerLoc.x - 5 > newX || playerLoc.x + 5 < newX ||
				playerLoc.y - 5 > newY || playerLoc.y + 5 < newY) {
			if (actor->move(wrap_signed_coord(newX, fromZ), wrap_signed_coord(newY, fromZ), fromZ))
				return true;
		}
	}

	return false;
}

} // End of namespace Scripts
} // End of namespace Ultima6
} // End of namespace Ultima
