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
#include "ultima/ultima6/core/obj_manager.h"
#include "ultima/ultima6/script/se/se_scripts.h"

namespace Ultima {
namespace Ultima6 {
namespace Scripts {
namespace SE {

const SEScripts::UseEntry SEScripts::USE_TABLE[] = {
	{ 61, &SEScripts::useCornStalk },
	{ 108, &SEScripts::useTree },
	{ 131, &SEScripts::useDiggingStick },
	{ 192, &SEScripts::useClay },
	{ 212, &SEScripts::useFishingPole },
	{ 5000, &SEScripts::useTree },
	{ 5002, &SEScripts::useYuccaPlant }
};

SEScripts::UseFunc SEScripts::getUseFunc(uint16 objN) {
	for (uint i = 0; i < ARRAYSIZE(USE_TABLE); ++i) {
		if (USE_TABLE[i]._objN == objN)
			return USE_TABLE[i]._func;
	}

	return nullptr;
}

bool SEScripts::hasUsecode(Obj *obj, UseCodeEvent usecodeType) {
	return usecodeType == USE_EVENT_USE && getUseFunc(obj->obj_n) != nullptr;
}

ScriptThread *SEScripts::useObj(Obj *obj, Actor *actor) {
	UseFunc func = getUseFunc(obj->obj_n);

	if (func) {
		print("\n");
		(this->*func)(obj, actor);
	}

	return nullptr;
}

bool SEScripts::actorHasItem(Actor *actor, Obj *obj) {
	while (obj->is_in_container())
		obj = obj->get_container_obj();

	if (obj->is_on_map() || !obj->is_in_inventory())
		return false;

	Actor *holder = obj->get_actor_holding_obj();
	return holder && holder->get_actor_num() == actor->get_actor_num();
}

Actor *SEScripts::checkCanReach(Obj *obj, Actor *actor, bool printMsg) {
	MapCoord loc = actor->get_location();

	if (ABS(obj->x - loc.x) <= 1 && ABS(obj->y - loc.y) <= 1 && obj->z == loc.z)
		return actor;

	Common::Array<Actor *> members;
	getPartyMembers(members);

	for (uint i = 0; i < members.size(); ++i) {
		if (actorHasItem(members[i], obj))
			return members[i];
	}

	if (printMsg)
		printl("OUT_OF_RANGE");

	return nullptr;
}

void SEScripts::deleteOrRemoveObject(Actor *actor, Obj *obj) {
	if (actor->inventory_has_object(obj->obj_n, 0, false))
		actor->inventory_remove_obj(obj);
	else
		Game::get_game()->get_obj_manager()->remove_obj_from_map(obj);

	if (obj->get_engine_loc() == OBJ_LOC_NONE)
		delete_obj(obj);
}

bool SEScripts::addItemToActor(uint16 objN, Actor *actor, bool stackQty, bool printMsg) {
	Obj *item = newObj(objN);
	item->qty = 1;

	if (!actor->can_carry_object(item)) {
		delete_obj(item);

		if (printMsg)
			printl("CARRY_TOO_MUCH");
		return false;
	}

	actor->inventory_add_object(item, nullptr, stackQty);
	return true;
}

void SEScripts::useCornStalk(Obj *obj, Actor *actor) {
	Actor *useActor = checkCanReach(obj, actor, true);
	if (!useActor)
		return;

	if (addItemToActor(108, useActor, true, true))
		printl("GOT_CORN");
}

void SEScripts::useTree(Obj *obj, Actor *actor) {
	Actor *useActor = checkCanReach(obj, actor, true);
	if (!useActor)
		return;

	if (addItemToActor(206, useActor, true, true))
		printl("USE_TREE");
}

void SEScripts::useYuccaPlant(Obj *obj, Actor *actor) {
	Actor *useActor = checkCanReach(obj, actor, true);
	if (!useActor)
		return;

	if (addItemToActor(210, useActor, true, true))
		printl("USE_YUCCA_PLANT");
}

void SEScripts::useClay(Obj *obj, Actor *actor) {
	Actor *useActor = checkCanReach(obj, actor, true);
	if (!useActor)
		return;

	deleteOrRemoveObject(useActor, obj);

	if (addItemToActor(132, useActor, false, false))
		printl("USE_CLAY");
}

void SEScripts::useFishingPole(Obj *obj, Actor *actor) {
	Actor *useActor = checkCanReach(obj, actor, true);
	if (!useActor)
		return;

	printl("USE_FISHING_POLE_NO_WATER");

	if (addItemToActor(192, useActor, false, false))
		printl("USE_FISHING_POLE_SUCCESS");

	printl("USE_FISHING_POLE_FAIL");
}

void SEScripts::useDiggingStick(Obj *obj, Actor *actor) {
	Actor *useActor = checkCanReach(obj, actor, true);
	if (!useActor)
		return;

	printl("USE_DIGGING_STICK_NO_WATER");

	if (addItemToActor(192, useActor, true, true))
		printl("USE_DIGGING_STICK");
}

} // End of namespace SE
} // End of namespace Scripts
} // End of namespace Ultima6
} // End of namespace Ultima
