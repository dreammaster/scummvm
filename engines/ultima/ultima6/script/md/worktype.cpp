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

#include "ultima/ultima6/actors/actor_manager.h"
#include "ultima/ultima6/core/game.h"
#include "ultima/ultima6/core/obj_manager.h"
#include "ultima/ultima6/core/player.h"
#include "ultima/ultima6/script/common/script_random.h"
#include "ultima/ultima6/script/md/md_scripts.h"

namespace Ultima {
namespace Ultima6 {
namespace Scripts {
namespace MD {

Actor *MDScripts::selectTargetActor(Actor *srcActor) {
	int closest = 0x7fff;
	Actor *target = nullptr;
	Actor *playerActor = Game::get_game()->get_player()->get_actor();

	ActorAlignment srcAlign = srcActor->get_alignment();
	uint16 srcObjN = srcActor->get_obj_n();

	for (int i = 0; i < 256; ++i) {
		Actor *actor = Game::get_game()->get_actor_manager()->get_actor(i);
		ActorAlignment align = actor->get_alignment();
		uint16 objN = actor->get_obj_n();

		if (objN == 0 || !actor->is_alive() || actor->is_invisible() ||
				actor->get_actor_num() == srcActor->get_actor_num() || objN == 381) // OBJ_DUST_DEVIL
			continue;

		if (srcAlign == ACTOR_ALIGNMENT_NEUTRAL && align == ACTOR_ALIGNMENT_NEUTRAL)
			continue;

		bool sameKind = srcObjN == objN ||
			(srcObjN == 372 && (objN == 370 || objN == 371)) ||
			((srcObjN == 362 || srcObjN == 373 || srcObjN == 254) && (objN == 362 || objN == 373 || objN == 254));

		if (srcAlign == ACTOR_ALIGNMENT_CHAOTIC && sameKind)
			continue;
		if (srcAlign == ACTOR_ALIGNMENT_GOOD && align != ACTOR_ALIGNMENT_EVIL && align != ACTOR_ALIGNMENT_CHAOTIC)
			continue;
		if (srcAlign == ACTOR_ALIGNMENT_EVIL && align != ACTOR_ALIGNMENT_GOOD && align != ACTOR_ALIGNMENT_CHAOTIC)
			continue;
		if (actorFindMaxWrappedXYDistance(srcActor, actor->get_x(), actor->get_y()) > 0xb)
			continue;
		if (actor->get_worktype() == 7 && actorFindMaxWrappedXYDistance(playerActor, actor->get_x(), actor->get_y()) > 5)
			continue;

		int dx = getWrappedDist(actor->get_x(), srcActor->get_x());
		int dy = getWrappedDist(actor->get_y(), srcActor->get_y());
		int distance = dx * dx + dy * dy;

		if (distance < closest ||
				(distance == closest && target && actorGetDamage(objN) > actorGetDamage(target->get_obj_n()))) {
			closest = distance;
			target = actor;
		}
	}

	return target;
}

void MDScripts::worktype15YourMother(Actor *actor) {
	if (!actorMove(actor, NUVIE_DIR_S, 0)) {
		Actor *target = selectTargetActor(actor);

		// FIXME do combat range check. sub_1B305
		if (target)
			actorTakeHit(actor, Target(target), 6, DAMAGE_NORMAL);
	}

	actorSubtractMovementPoints(actor, 3);
}

void MDScripts::worktypeStompAround(Actor *actor) {
	actorSubtractMovementPoints(actor, 5);

	if (actor->get_obj_n() == 398 && _currentDreamStage == 0x44) {
		if (randomRange(0, 1) != 0)
			return;

		playMdSfx(0, true);
		quakeStart(1, 50);
	}

	actorMove(actor, (NuvieDir)randomRange(0, 3), 0);

	Common::Array<Obj *> objs;
	getObjsAt(actor->get_x(), actor->get_y(), actor->get_z(), objs);
	for (uint i = 0; i < objs.size(); ++i)
		actorHitTarget(Target(objs[i]), randomRange(0, 0xa) + randomRange(0, 0xa), DAMAGE_NORMAL);
}

void MDScripts::worktype16Minotaur(Actor *actor) {
	if (_currentDreamStage == 0x60) {
		actor->die();
		return;
	}

	const uint16 OBJ_RED_CAPE = 162;
	Actor *dreamActor = Game::get_game()->get_actor_manager()->get_actor(0);
	Target target;

	if (dreamActor->inventory_has_object(OBJ_RED_CAPE, 0, false)) {
		target = Target(dreamActor);
		_selectedTarget = target;
	} else {
		Common::Array<Obj *> objs;
		getObjsInArea(0x21, 0x33, 2, 0x1c, 0x10, objs);

		for (uint i = 0; i < objs.size(); ++i) {
			if (objs[i]->obj_n == OBJ_RED_CAPE)
				target = Target(objs[i]);
		}
	}

	if (target.isNull()) {
		worktypeStompAround(actor);
		return;
	}

	if (actorGetCombatRange(actor, target.x(), target.y()) <= 1) {
		if (target.isActor()) {
			attackTargetWithWeapon(actor, target.x(), target.y(), Target(actor));
			actorSubtractMovementPoints(actor, 0xa);
			return;
		}

		hitTarget(target, RED_HIT_TILE);
		if (!Game::get_game()->get_actor_manager()->get_actor(0x54)->get_flag(6) && target.x() >= 0x37 && target.y() <= 0x37) {
			completeTiffanyStage();
			return;
		}
	}

	actorMoveTowardsLoc(actor, target.x(), target.y());
	playMdSfx(0, true);
	quakeStart(1, 50);

	Common::Array<Obj *> objs;
	getObjsAt(actor->get_x(), actor->get_y(), actor->get_z(), objs);
	for (uint i = 0; i < objs.size(); ++i)
		actorHitTarget(Target(objs[i]), randomRange(1, 0xa) + randomRange(1, 0xa), DAMAGE_NORMAL);

	actorSubtractMovementPoints(actor, 5);
}

void MDScripts::worktype99CokerMoveToCoalVein(Actor *actor) {
	const uint16 OBJ_VEIN_OF_COAL = 446;

	if (!actorMove(actor, NUVIE_DIR_N, 0)) {
		// FIXME should be -2 not -1, but actorMove needs to be fixed for the coker
		Obj *vein = mapGetObj(actor->get_x(), actor->get_y() - 1, actor->get_z(), OBJ_VEIN_OF_COAL);

		if (vein) {
			if (mapIsOnScreen(actor->get_x(), actor->get_y(), actor->get_z()))
				playMdSfx(0x10);
			actor->set_worktype(0x9A);
		}
	}
}

void MDScripts::worktype9ACokerDropCoal(Actor *actor) {
	const uint16 OBJ_CONVEYOR_BELT = 188;
	const uint16 OBJ_BARE_ROLLERS = 192;
	const uint16 OBJ_HUGE_LUMP_OF_COAL = 447;
	int x = actor->get_x();
	int y = actor->get_y() + 1;
	uint8 z = actor->get_z();

	if (mapGetObj(x, y, z, OBJ_CONVEYOR_BELT)) {
		Obj *coal = newObj(OBJ_HUGE_LUMP_OF_COAL);
		if (!Game::get_game()->get_obj_manager()->moveto_map(coal, MapCoord(x, y, z)))
			delete_obj(coal);
	} else if (!mapGetObj(x, y, z, OBJ_BARE_ROLLERS)) {
		actorMove(actor, NUVIE_DIR_S, 0);
		return;
	}

	actor->set_worktype(0x9B);
}

void MDScripts::worktype9BCokerWaitForCoalToMoveAway(Actor *actor) {
	const uint16 OBJ_HUGE_LUMP_OF_COAL = 447;
	const uint16 OBJ_BARE_ROLLERS = 192;
	int x = actor->get_x();
	int y = actor->get_y() + 1;
	uint8 z = actor->get_z();

	if (!mapGetObj(x, y, z, OBJ_HUGE_LUMP_OF_COAL) && !mapGetObj(x, y, z, OBJ_BARE_ROLLERS))
		actor->set_worktype(0x99);
}

void MDScripts::worktype9DStokerWaitForCoal(Actor *actor) {
	const uint16 OBJ_HUGE_LUMP_OF_COAL = 447;
	int x = actor->get_x();
	int y = actor->get_y() + 1;
	uint8 z = actor->get_z();

	Obj *coal = mapGetObj(x, y, z, OBJ_HUGE_LUMP_OF_COAL);
	if (!coal)
		return;

	while (coal) {
		removeObj(coal);
		coal = mapGetObj(x, y, z, OBJ_HUGE_LUMP_OF_COAL);
	}

	actor->set_worktype(0x9E);
}

void MDScripts::worktype9EStokerWalkToFurnace(Actor *actor) {
	const uint16 OBJ_FURNACE = 233;
	ActorManager *actorManager = Game::get_game()->get_actor_manager();

	if (actorMove(actor, NUVIE_DIR_N, 0))
		return;

	Obj *furnace = mapGetObj(actor->get_x(), actor->get_y() - 1, actor->get_z(), OBJ_FURNACE);
	if (!furnace)
		furnace = mapGetObj(actor->get_x() + 1, actor->get_y() - 1, actor->get_z(), OBJ_FURNACE);

	if (!furnace) {
		stokerBlocked(actor);
		return;
	}

	Actor *conveyor = actorManager->get_actor(0x72);
	Actor *tower = actorManager->get_actor(0x73);
	Actor *engineer = actorManager->get_actor(0x71);

	if (!conveyor->get_flag(2)) {
		activatePowerSystem();
	} else if (!tower->get_flag(2) || engineer->get_flag(3)) {
		if (engineer->get_flag(3)) {
			tower->clear_flag(2);
			engineer->clear_flag(3);
			// FIXME sub_3F740
		}

		tower->set_flag(2);
		activatePowerUpdateTiles();
		activateTowerElectricity();
		midgameCutscene2();
	}

	actor->set_worktype(0x9C);
}

void MDScripts::activateTowerElectricity() {
	const uint16 OBJ_TOWER_TOP = 201;
	const uint16 OBJ_POWER_CABLE = 214;
	const uint16 OBJ_POWER_CABLE1 = 215;
	Common::Array<Obj *> objs;

	findObjs(0, OBJ_TOWER_TOP, objs);
	for (uint i = 0; i < objs.size(); ++i) {
		Obj *obj = objs[i];
		if (obj->x >= 0x3d0 && obj->x <= 0x3f0 && obj->y >= 0x1d0 && obj->y <= 0x1e7)
			obj->frame_n |= 4;
	}

	objs.clear();
	findObjs(0, OBJ_POWER_CABLE, objs);
	for (uint i = 0; i < objs.size(); ++i) {
		Obj *obj = objs[i];
		if (obj->x >= 0x3d0 && obj->x <= 0x3f0 && obj->y >= 0x1d0 && obj->y <= 0x1e7)
			obj->obj_n = OBJ_POWER_CABLE1;
	}
}

void MDScripts::stokerBlocked(Actor *stoker) {
	if (mapIsOnScreen(stoker->get_x(), stoker->get_y(), stoker->get_z())) {
		printl("STOKERS_PATH_IS_BLOCKED");
		playMdSfx(0);
	}
}

void MDScripts::worktype9CStokerReturnToConveyorBelt(Actor *actor) {
	const uint16 OBJ_CONVEYOR_BELT2 = 191;

	if (!mapGetObj(actor->get_x(), actor->get_y() + 2, actor->get_z(), OBJ_CONVEYOR_BELT2)) {
		if (!actorMove(actor, NUVIE_DIR_S, 0))
			stokerBlocked(actor);
	} else {
		actor->set_worktype(0x9D);
	}
}

void MDScripts::performWorktype(Actor *actor) {
	sint8 movesLeft = actor->get_moves_left();

	switch (actor->get_worktype()) {
	case 0x15:
		worktype15YourMother(actor);
		break;
	case 0x16:
		worktype16Minotaur(actor);
		break;
	case 0x99:
		worktype99CokerMoveToCoalVein(actor);
		break;
	case 0x9a:
		worktype9ACokerDropCoal(actor);
		break;
	case 0x9b:
		worktype9BCokerWaitForCoalToMoveAway(actor);
		break;
	case 0x9c:
		worktype9CStokerReturnToConveyorBelt(actor);
		break;
	case 0x9d:
		worktype9DStokerWaitForCoal(actor);
		break;
	case 0x9e:
		worktype9EStokerWalkToFurnace(actor);
		break;
	default:
		break;
	}

	if (movesLeft == actor->get_moves_left())
		actorSubtractMovementPoints(actor, 10);
}

} // End of namespace MD
} // End of namespace Scripts
} // End of namespace Ultima6
} // End of namespace Ultima
