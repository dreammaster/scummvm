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
#include "ultima/ultima6/core/map.h"
#include "ultima/ultima6/core/obj_manager.h"
#include "ultima/ultima6/core/player.h"
#include "ultima/ultima6/core/tile_manager.h"
#include "ultima/ultima6/script/common/script_random.h"
#include "ultima/ultima6/script/md/md_scripts.h"

namespace Ultima {
namespace Ultima6 {
namespace Scripts {
namespace MD {

namespace {

enum {
	OBJ_DIRT_PILE = 258,
	OBJ_CHUNK_OF_ICE = 256,
	OBJ_HOLE = 257,
	OBJ_HOLE_IN_ICE = 255,
	OBJ_PILE_OF_ROCKS = 442,
	OBJ_PILE_OF_IRON_ORE = 443,
	OBJ_PILE_OF_COAL = 444,
	OBJ_MARTIAN_WHEEL_BARROW = 268,
	OBJ_RAIL_CAR = 410,
	OBJ_FURNACE = 233,
	OBJ_CONVEYOR_BELT = 188,
	OBJ_CONVEYOR_BELT1 = 189,
	OBJ_HUGE_LUMP_OF_COAL = 447
};

// Positions around a point, starting at the top left and going clockwise
const int AROUND_X[8] = { -1, 0, 1, 1, 1, 0, -1, -1 };
const int AROUND_Y[8] = { -1, -1, -1, 0, 1, 1, 1, 0 };

} // End of anonymous namespace

void MDScripts::searchContainer(Obj *obj) {
	Common::Array<Obj *> children;
	getContainerObjs(obj, children);

	for (uint i = 0; i < children.size(); ++i)
		Game::get_game()->get_obj_manager()->moveto_map(children[i], MapCoord(obj->x, obj->y, obj->z));
}

void MDScripts::useDoor(Obj *obj, Actor *actor) {
	if (obj->quality & 0x80) {
		printl("IT_IS_LOCKED");
		return;
	}

	if (mapGetActor(obj->x, obj->y, obj->z)) {
		printl("BLOCKED");
		return;
	}

	obj->frame_n ^= 2;
}

void MDScripts::useCrate(Obj *obj, const Target &target, Actor *actor, int targetX, int targetY, int targetZ) {
	Obj *crate = target._obj;
	if (!crate)
		return;

	if (crate->frame_n == 2) {
		crate->frame_n = 1;
		printl("YOU_PRIED_THE_NAILS_OUT_OF_THE_CRATE");
	} else if (crate->frame_n == 1 && obj->obj_n == 273) {
		crate->frame_n = 2;
		printl("YOU_NAIL_THE_CRATE_SHUT");
	} else {
		printl("IT_HAS_NO_EFFECT");
	}
}

void MDScripts::useContainer(Obj *obj, Actor *actor) {
	if (obj->frame_n == 2) {
		printl(obj->obj_n == 86 || obj->obj_n == 284 ? "IT_IS_NAILED_SHUT" : "IT_IS_LOCKED");
	} else if (obj->frame_n == 1) {
		obj->frame_n = 0;
		searchContainer(obj);
	} else {
		obj->frame_n = 1;
	}
}

void MDScripts::useGate(Obj *obj, Actor *actor) {
	if (obj->quality & 128) {
		printl("IT_IS_APPARENTLY_LOCKED");
		return;
	}

	switch (obj->frame_n) {
	case 0: case 1:
		obj->frame_n = 3;
		setObjLocation(obj, obj->x - 1, obj->y, obj->z);
		break;
	case 2: case 3:
		obj->frame_n = 1;
		setObjLocation(obj, obj->x + 1, obj->y, obj->z);
		break;
	case 4: case 5:
		obj->frame_n = 7;
		break;
	case 6: case 7:
		obj->frame_n = 5;
		break;
	default:
		break;
	}
}

void MDScripts::useMiscTextOnly(Obj *obj, Actor *actor) {
	switch (obj->obj_n) {
	case 65: case 66: case 67: case 263: case 267: case 327:
		printl("YOU_CANT_DIG_ANY_DEEPER");
		break;
	case 427:
		printl("IT_IS_STUCK");
		break;
	case 288:
		printl("THE_CONTROL_PANEL_OPERATES_THE_DREAM_MACHINE");
		break;
	case 199:
		printl("YOU_NEED_TO_USE_PLIERS_TO_ATTACH_THE_CABLE");
		break;
	case 131:
		printl("YOU_WILL_CHEW_IT_INSTINCTIVELY");
		break;
	case 184:
		printl("THE_PLATE_IN_THE_CAMERA_HAS_ALREADY_BEEN_EXPOSED");
		break;
	case 442: case 443: case 444:
		printl("YOU_MUST_USE_A_SHOVEL_TO_MOVE_THAT");
		break;
	case 293:
		printl("YOU_CANT_DETERMINE_HOW_TO_READ_THE_TIME");
		break;
	case 77: case 78:
		printl("THE_BERRIES_ARE_NOT_EDIBLE");
		break;
	case 323:
		printl("YOU_DONT_KNOW_HOW_IT_WORKS");
		break;
	default:
		printl("IT_HAS_NO_EFFECT");
		break;
	}
}

void MDScripts::useMiscText(Obj *obj, const Target &target, Actor *actor, int targetX, int targetY, int targetZ) {
	useMiscTextOnly(obj, actor);
}

uint16 MDScripts::getPileObjNum(uint16 mapTile) {
	if ((mapTile >= 32 && mapTile <= 36) || mapTile == 12 || mapTile == 13 || (mapTile >= 40 && mapTile <= 63))
		return OBJ_DIRT_PILE;

	if (mapTile == 124 || mapTile == 125 || mapTile == 127)
		return OBJ_CHUNK_OF_ICE;

	return 0; // Can't dig here
}

bool MDScripts::getFreeLocationAroundActor(Actor *actor, MapCoord &pos) {
	for (int i = 0; i < 8; ++i) {
		pos = MapCoord(wrap_signed_coord(actor->get_x() + AROUND_X[i], actor->get_z()),
			wrap_signed_coord(actor->get_y() + AROUND_Y[i], actor->get_z()), actor->get_z());

		if (Game::get_game()->get_actor_manager()->can_put_actor(pos) && !mapGetObj(pos.x, pos.y, pos.z))
			return true;
	}

	return false;
}

void MDScripts::useToolOnGround(Obj *obj, const Target &target, Actor *actor, int targetX, int targetY, int targetZ) {
	if (!target.isNull()) {
		// FIXME check the tile flags here
		printl("THE_GROUND_IS_NOT_CLEAR_FOR_DIGGING");
		return;
	}

	const Tile *tile = Game::get_game()->get_game_map()->get_tile(wrap_signed_coord(targetX, targetZ),
		wrap_signed_coord(targetY, targetZ), targetZ, false);
	uint16 pileObjN = tile ? getPileObjNum(tile->tile_num) : 0;

	if (pileObjN == 0) {
		printl("IT_HAS_NO_EFFECT");
		return;
	}

	ObjManager *objManager = Game::get_game()->get_obj_manager();
	MapCoord holeLoc(wrap_signed_coord(targetX, targetZ), wrap_signed_coord(targetY, targetZ), targetZ);

	Obj *hole = newObj(pileObjN == OBJ_CHUNK_OF_ICE ? OBJ_HOLE_IN_ICE : OBJ_HOLE);
	hole->set_temporary(true);
	if (!objManager->moveto_map(hole, holeLoc)) {
		delete_obj(hole);
		return;
	}

	MapCoord pileLoc;
	if (getFreeLocationAroundActor(actor, pileLoc)) {
		Obj *pile = newObj(pileObjN);
		pile->set_temporary(true);
		if (!objManager->moveto_map(pile, pileLoc))
			delete_obj(pile);
	} else {
		removeObj(hole);
		printl("YOU_CANT_DIG_HERE");
	}
}

void MDScripts::useShovelOnPileToHole(Obj *obj, const Target &target, Obj *toObj, Actor *actor) {
	if (target._obj)
		removeObj(target._obj);
	removeObj(toObj);

	playMdSfx(0x1b);
	playMdSfx(0x1b);
	printl("YOU_FILLED_IN_THE_HOLE");
}

int MDScripts::getOreContainerQuality(uint16 oreObjN) {
	switch (oreObjN) {
	case OBJ_DIRT_PILE:
		return 1;
	case OBJ_PILE_OF_ROCKS:
		return 2;
	case OBJ_PILE_OF_IRON_ORE:
		return 3;
	case OBJ_PILE_OF_COAL:
		return 4;
	default:
		return 1;
	}
}

uint16 MDScripts::getObjNumFromOreQuality(int oreQuality) {
	switch (oreQuality) {
	case 2:
		return OBJ_PILE_OF_ROCKS;
	case 3:
		return OBJ_PILE_OF_IRON_ORE;
	case 4:
		return OBJ_PILE_OF_COAL;
	default:
		return OBJ_DIRT_PILE;
	}
}

bool MDScripts::shovelOreToContainer(Obj *ore, Obj *toObj) {
	print("\n");
	int oreQuality = getOreContainerQuality(ore->obj_n);

	if (toObj->obj_n == OBJ_MARTIAN_WHEEL_BARROW) {
		if (toObj->qty == 1) {
			printl("THERE_IS_NO_MORE_ROOM");
			playMdSfx(5);
			return false;
		}

		playMdSfx(0x1b);
		toObj->qty = 1;
		toObj->quality = oreQuality;
		printfl("YOU_PUT_THE_ORE_INTO_THE_WHEELBARROW", Target(ore).name().c_str());
		removeObj(ore);
	} else if (toObj->obj_n == OBJ_RAIL_CAR) {
		if (toObj->qty >= 7) {
			printl("THERE_IS_NO_MORE_ROOM");
			playMdSfx(5);
			return false;
		}

		if (toObj->qty > 0 && toObj->quality != oreQuality) {
			printl("YOU_CANT_MIX_DIFFERENT_THINGS_IN_THE_SAME_LOAD");
			playMdSfx(5);
			return false;
		}

		toObj->quality = oreQuality;
		toObj->qty += 1;
		if (toObj->qty == 1 || toObj->qty == 7)
			toObj->frame_n += 2;

		printfl("YOU_PUT_THE_ORE_INTO_THE_RAIL_CAR", Target(ore).name().c_str());
		removeObj(ore);
	}

	return true;
}

void MDScripts::useShovelOnOreToContainer(Obj *obj, const Target &target, Obj *toObj, Actor *actor) {
	if (target._obj)
		shovelOreToContainer(target._obj, toObj);
}

bool MDScripts::shovelOreToFurnace(Obj *ore, Obj *furnace) {
	uint16 objN = ore->obj_n;

	playMdSfx(0x1b);
	removeObj(ore);

	if (objN == OBJ_PILE_OF_COAL) {
		if (furnace->frame_n < 4)
			activatePowerSystem();
	} else {
		printl("IT_HAS_NO_EFFECT");
	}

	return true;
}

void MDScripts::useShovelToUnloadContainer(Obj *obj, const Target &target, Obj *toObj, Actor *actor) {
	Obj *container = target._obj;
	if (!container)
		return;

	if (container->qty == 0) {
		printl("THERE_IS_NOTHING_TO_UNLOAD");
		playMdSfx(5);
		return;
	}

	Obj *ore = newObj(getObjNumFromOreQuality(container->quality));
	bool success = false;

	// FIXME unloading into a hole and burying aren't implemented
	if (toObj->obj_n == OBJ_MARTIAN_WHEEL_BARROW || toObj->obj_n == OBJ_RAIL_CAR) {
		success = shovelOreToContainer(ore, toObj);
		if (!success)
			delete_obj(ore);
	} else if (toObj->obj_n == OBJ_FURNACE) {
		success = shovelOreToFurnace(ore, toObj);
	} else {
		delete_obj(ore);
	}

	if (success) {
		container->qty -= 1;

		if (container->obj_n == OBJ_RAIL_CAR && (container->qty == 6 || container->qty == 0))
			container->frame_n -= 2;
	}
}

void MDScripts::activatePowerSystem() {
	ActorManager *actorManager = Game::get_game()->get_actor_manager();

	printl("THE_COAL_BEGINS_MOVING_DOWN_THE_BELT");
	actorManager->get_actor(0x73)->set_flag(2);
	actorManager->get_actor(0x71)->set_flag(3);
	activatePowerUpdateTiles();
	updateConveyorBelt(false);
}

void MDScripts::activatePowerUpdateTiles() {
	const uint16 OBJ_CRACK = 208;
	const uint16 OBJ_STEAM_LEAK = 209;
	const uint16 OBJ_LAMP = 154;
	TileManager *tileManager = Game::get_game()->get_tile_manager();
	uint8 level = Game::get_game()->get_player()->get_actor()->get_z();

	for (int anim = 9; anim <= 12; ++anim)
		tileManager->anim_play_repeated(anim);

	Common::Array<Obj *> objs;
	findObjs(level, OBJ_CRACK, objs);
	for (uint i = 0; i < objs.size(); ++i) {
		Obj *obj = objs[i];

		if (!mapGetObj(obj->x, obj->y, obj->z, OBJ_STEAM_LEAK)) {
			Obj *steam = newObj(OBJ_STEAM_LEAK);
			steam->frame_n = 1;
			if (!Game::get_game()->get_obj_manager()->moveto_map(steam, MapCoord(obj->x, obj->y + 1, obj->z)))
				delete_obj(steam);
		}
	}

	objs.clear();
	findObjs(level, OBJ_FURNACE, objs);
	for (uint i = 0; i < objs.size(); ++i) {
		if (objs[i]->frame_n == 1 || objs[i]->frame_n == 3)
			objs[i]->frame_n += 4;
	}

	objs.clear();
	findObjs(level, OBJ_LAMP, objs);
	findObjs(4, OBJ_LAMP, objs);
	for (uint i = 0; i < objs.size(); ++i)
		objs[i]->frame_n = 1;
}

void MDScripts::shutdownPowerUpdateTiles() {
	const uint16 OBJ_STEAM_LEAK = 209;
	const uint16 OBJ_LAMP = 154;
	TileManager *tileManager = Game::get_game()->get_tile_manager();
	uint8 level = Game::get_game()->get_player()->get_actor()->get_z();

	for (int anim = 9; anim <= 12; ++anim)
		tileManager->anim_stop_playing(anim);

	Common::Array<Obj *> objs;
	findObjs(level, OBJ_STEAM_LEAK, objs);
	for (uint i = 0; i < objs.size(); ++i)
		removeObj(objs[i]);

	objs.clear();
	findObjs(level, OBJ_FURNACE, objs);
	for (uint i = 0; i < objs.size(); ++i) {
		if (objs[i]->frame_n == 5 || objs[i]->frame_n == 7)
			objs[i]->frame_n -= 4;
	}

	objs.clear();
	findObjs(level, OBJ_LAMP, objs);
	findObjs(4, OBJ_LAMP, objs);
	for (uint i = 0; i < objs.size(); ++i)
		objs[i]->frame_n = 0;
}

void MDScripts::updateConveyorBelt(bool canStop) {
	ActorManager *actorManager = Game::get_game()->get_actor_manager();

	if (!actorManager->get_actor(0x73)->get_flag(2))
		return;

	if (Game::get_game()->get_player()->get_actor()->get_z() != 5)
		return;

	// There's only one conveyor in the game, so this doesn't search for conveyors like the original
	int x = 0x3c;
	const int y = 0x63;
	const uint8 z = 5;

	Obj *conveyor = mapGetObj(x, y, z, OBJ_CONVEYOR_BELT);

	while (conveyor) {
		if (conveyor->frame_n == 2) {
			Obj *seam = mapGetObj(x, y, z, OBJ_CONVEYOR_BELT1);
			if (seam)
				removeObj(seam);
		} else if (conveyor->frame_n == 0) {
			if (conveyor->qty == 0) {
				Obj *seam = newObj(OBJ_CONVEYOR_BELT1);
				if (!Game::get_game()->get_obj_manager()->moveto_map(seam, MapCoord(x, y, z)))
					delete_obj(seam);
				conveyor->qty = 4;
			}
			conveyor->qty -= 1;
		}

		Obj *seam = mapGetObj(x, y, z, OBJ_CONVEYOR_BELT1);
		if (seam)
			setObjLocation(seam, seam->x + 1, seam->y, seam->z);

		Common::Array<Obj *> objs;
		getObjsAt(x, y, z, objs);
		for (uint i = 0; i < objs.size(); ++i) {
			Obj *obj = objs[i];
			if (obj->obj_n == OBJ_HUGE_LUMP_OF_COAL || objectWeight(obj) > 0)
				setObjLocation(obj, obj->x + 1, obj->y, obj->z);
		}

		Actor *actor = mapGetActor(x, y, z);
		if (actor)
			actor->move(x + 1, y, z);

		--x;
		conveyor = mapGetObj(x, y, z, OBJ_CONVEYOR_BELT);
	}

	if (canStop && actorManager->get_actor(0x71)->get_flag(3) && randomRange(0, 6) == 0) {
		printl("THE_CONVEYOR_BELT_STOPS");
		actorManager->get_actor(0x73)->clear_flag(2);
		actorManager->get_actor(0x71)->clear_flag(3);
		shutdownPowerUpdateTiles();
	}
}

void MDScripts::midgameCutscene2() {
	const uint16 OBJ_TOWER_TOP = 201;
	const uint16 OBJ_POWER_CABLE = 214;
	Common::Array<Obj *> objs;

	playMidgameSequence(2);

	findObjs(0, OBJ_TOWER_TOP, objs);
	for (uint i = 0; i < objs.size(); ++i) {
		Obj *tower = objs[i];
		if (tower->x >= 0x3d0 && tower->x <= 0x3f0 && tower->y >= 0x1d0 && tower->y <= 0x1e7)
			tower->frame_n = 4 + (tower->frame_n % 4);
	}

	objs.clear();
	findObjs(0, OBJ_POWER_CABLE, objs);
	for (uint i = 0; i < objs.size(); ++i) {
		Obj *cable = objs[i];
		if (cable->x >= 0x3d0 && cable->x <= 0x3f0 && cable->y >= 0x1d0 && cable->y <= 0x1e7)
			cable->obj_n = 215;
	}
}

void MDScripts::useFixedBeltOnBareRollers(Obj *obj, const Target &target, Actor *actor, int targetX, int targetY, int targetZ) {
	const uint16 OBJ_BARE_ROLLERS = 192;
	Obj *startObj = nullptr;
	Obj *rollers = target._obj;

	// Find the left end of the rollers
	while (rollers) {
		if (rollers->frame_n == 0) {
			startObj = rollers;
			break;
		}

		rollers = mapGetObj(rollers->x - 1, rollers->y, rollers->z, rollers->obj_n);
	}

	if (!startObj) {
		printl("OOOPS_THESE_ROLLERS_CAN_NEVER_BE_FIXED");
		return;
	}

	rollers = startObj;
	int i = 4;
	while (rollers) {
		rollers->obj_n = OBJ_CONVEYOR_BELT;

		if (i == 0) {
			i = 4;
			Obj *beltJoin = newObj(OBJ_CONVEYOR_BELT1);
			if (!Game::get_game()->get_obj_manager()->moveto_map(beltJoin, MapCoord(rollers->x, rollers->y, rollers->z)))
				delete_obj(beltJoin);
		} else {
			--i;
		}

		rollers = mapGetObj(rollers->x + 1, rollers->y, rollers->z, OBJ_BARE_ROLLERS);
	}

	removeObj(obj);
	Game::get_game()->get_actor_manager()->get_actor(0x72)->set_flag(2);
}

void MDScripts::useRubySlippers(Obj *obj, Actor *actor) {
	if (!obj->is_readied()) {
		// FIXME check that we can ready this object
		Game::get_game()->get_obj_manager()->unlink_from_engine(obj);
		actor->add_readied_object(obj);
		return;
	}

	if (obj->quality == 2) {
		printl("YOU_MAY_USE_THE_RUBY_SLIPPERS_TO_GO_HOME");

		if (inputSelect("yn", false) == "Y")
			playEndSequence();
		else
			playMidgameSequence(13);
	} else {
		printl("CLICK");
		obj->quality += 1;
	}
}

bool MDScripts::foesAreNearby() {
	MapCoord loc = Game::get_game()->get_player()->get_actor()->get_location();

	for (int i = 1; i < 256; ++i) {
		Actor *actor = Game::get_game()->get_actor_manager()->get_actor(i);

		if (actor->is_alive() && (actor->get_alignment() == ACTOR_ALIGNMENT_EVIL || actor->get_alignment() == ACTOR_ALIGNMENT_CHAOTIC) &&
				actor->get_z() == loc.z) {
			if (getWrappedDist(actor->get_x(), loc.x) < 40 && getWrappedDist(actor->get_y(), loc.y) < 40)
				return true;
		}
	}

	return false;
}

Actor *MDScripts::npcsAreNearby() {
	MapCoord loc = Game::get_game()->get_player()->get_actor()->get_location();

	for (int i = 1; i < 256; ++i) {
		Actor *actor = Game::get_game()->get_actor_manager()->get_actor(i);

		if (actor->is_alive() && !actor->is_sleeping() && !actor->is_in_party() && actor->get_z() == loc.z) {
			// FIXME check the actor is visible to the player
			if (getWrappedDist(actor->get_x(), loc.x) < 6 && getWrappedDist(actor->get_y(), loc.y) < 6)
				return actor;
		}
	}

	return nullptr;
}

bool MDScripts::isActorAbleToTalkToPlayer(Actor *actor) {
	MapCoord loc = Game::get_game()->get_player()->get_actor()->get_location();

	// FIXME check the actor is visible to the player
	return actor->is_alive() && !actor->is_sleeping() && actor->get_z() == loc.z &&
		getWrappedDist(actor->get_x(), loc.x) < 6 && getWrappedDist(actor->get_y(), loc.y) < 6;
}

} // End of namespace MD
} // End of namespace Scripts
} // End of namespace Ultima6
} // End of namespace Ultima
