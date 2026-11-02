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
#include "ultima/ultima6/core/game_clock.h"
#include "ultima/ultima6/core/map.h"
#include "ultima/ultima6/core/obj_manager.h"
#include "ultima/ultima6/core/party.h"
#include "ultima/ultima6/core/player.h"
#include "ultima/ultima6/script/common/script_random.h"
#include "ultima/ultima6/script/md/md_scripts.h"
#include "ultima/ultima6/views/view_manager.h"

namespace Ultima {
namespace Ultima6 {
namespace Scripts {
namespace MD {

namespace {

Actor *getActor(uint8 actorNum) {
	return Game::get_game()->get_actor_manager()->get_actor(actorNum);
}

// Positions around a drill, starting at the top left and going clockwise
const int AROUND_DRILL_X[8] = { -1, 0, 1, 1, 1, 0, -1, -1 };
const int AROUND_DRILL_Y[8] = { -1, -1, -1, 0, 1, 1, 1, 0 };

bool addToWorld(Obj *obj, const MapCoord &loc) {
	if (Game::get_game()->get_obj_manager()->moveto_map(obj, loc))
		return true;

	delete_obj(obj);
	return false;
}

} // End of anonymous namespace

int MDScripts::actorGetMaxHp(Actor *actor) {
	if (actor->get_actor_num() == 6)
		return 0xf0;

	if (actor->is_in_party())
		return actor->get_strength() * 2 + actor->get_level() * 24;

	// FIXME return the actor's max hp from the stats table
	return 1;
}

void MDScripts::useHammerOnOxiumGeode(Obj *obj, const Target &target, Actor *actor, int targetX, int targetY, int targetZ) {
	const uint16 OBJ_BLOB_OF_OXIUM = 131;
	Obj *geode = target._obj;
	if (!geode)
		return;

	if (geode->frame_n != 0) {
		printl("IT_HAS_NO_EFFECT");
		return;
	}

	playMdSfx(0x20);
	geode->frame_n = 1; // Break the geode open

	int qty = randomRange(1, 50) + randomRange(1, 50);

	if (geode->is_on_map()) {
		Obj *oxium = newObj(OBJ_BLOB_OF_OXIUM);
		oxium->qty = qty;
		addToWorld(oxium, MapCoord(geode->x, geode->y, geode->z));
		return;
	}

	// FIXME add to the actor's inventory
	Obj *oxium = actor->inventory_get_object(OBJ_BLOB_OF_OXIUM, 0, false, 0, false);
	if (oxium) {
		oxium->qty += qty;
	} else {
		oxium = newObj(OBJ_BLOB_OF_OXIUM);
		oxium->qty = qty;
		actor->inventory_add_object(oxium, nullptr, false);
	}
}

void MDScripts::usePrybarOnHatch(Obj *obj, const Target &target, Actor *actor, int targetX, int targetY, int targetZ) {
	Obj *hatch = target._obj;
	if (!hatch)
		return;

	if (actor->get_actor_num() != 1) {
		printfl("IS_NOT_STRONG_ENOUGH", actor->get_name());
		return;
	}

	Actor *tesla = getActor(16);
	if (!tesla->get_flag(4)) {
		tesla->set_flag(2);
		actorTalk(tesla);
		return;
	}

	playMidgameSequence(1);

	tesla->set_flag(5);
	hatch->obj_n = 428;
	hatch->frame_n = 0;
	setObjLocation(hatch, hatch->x + 1, hatch->y + 1, hatch->z);

	Actor *blood = getActor(18);
	blood->set_flag(3);
	blood->set_flag(6);
	actorTalk(blood);
}

void MDScripts::useSextant(Obj *obj, Actor *actor) {
	if (actor->get_z() != 0) {
		printl("NOT_WHILE_UNDERGROUND");
		return;
	}

	int lat = (int)(((actor->get_y() - 512) * 240) / 1024.0);
	int longitude = (int)(((actor->get_x() - 512) * 360) / 1024.0);
	const char *latStr = "N";
	const char *longStr = "W";

	if (lat > 0)
		latStr = "S";
	else if (lat == 0)
		latStr = " ";

	if (longitude == 180 || longitude == -180 || longitude == 0)
		longStr = " ";
	else if (longitude < 0)
		longStr = "E";

	printl("YOU_ARE_SOMEWHERE_NEAR");
	print(Common::String::format(" \nLat:%d %s\nLong:%d %s\n", ABS(lat), latStr, 180 - ABS(longitude), longStr));
}

void MDScripts::useBerry(Obj *obj, Actor *actor) {
	uint8 actorNum = actor->get_actor_num();
	if (actorNum == 6) {
		printl("A_MECHANICAL_PERSON_CANT_EAT_BERRIES");
		return;
	}

	playMdSfx(0x32);
	int berryType = obj->obj_n - 73; // OBJ_BERRY

	if (berryType >= BERRY_PURPLE && berryType <= BERRY_BROWN && actorIsAffectedByBerries(actorNum, berryType))
		printl("YOU_EAT_A_MARTIAN_BERRY_YOU_FEEL_AN_INCREASE_IN_THE_STRANGE");
	else
		printl("YOU_EAT_A_MARTIAN_BERRY_YOU_FEEL_A_STRANGE");

	if (berryType == 0) {
		printl("RELATIONSHIP_WITH_THINGS_AROUND_YOU");
		actorIncrementBerryCounter(actorNum, BERRY_PURPLE);
	} else if (berryType == 1) {
		printl("SENSITIVITY_TO_THE_FEELINGS_OF_OBJECTS_AROUND_YOU");
		actorIncrementBerryCounter(actorNum, BERRY_GREEN);
	} else if (berryType == 2) {
		printl("SENSE_OF_YOUR_SPATIAL_LOCATION");
		actorIncrementBerryCounter(actorNum, BERRY_BROWN);
	} else if (berryType == 3) {
		printl("SUDDEN_FLUSH_DIZZINESS_AND_NAUSEA");
		actor->set_poisoned(false);
		actor->hit(randomRange(5, 0x14) + randomRange(5, 0x14), true);
		setBlueBerryCounter(getBlueBerryCounter() + randomRange(1, 2));
	}

	if (obj->qty == 1)
		removeObj(obj);
	else
		obj->qty -= 1;
}

void MDScripts::useRedBerry(Obj *obj, Actor *actor) {
	if (!Game::get_game()->get_party()->is_in_combat_mode()) {
		printl("THAT_WOULD_BE_A_WASTE_OUTSIDE_OF_COMBAT");
		return;
	}

	if (!actor->get_obj_flag(ACTOR_MD_OBJ_FLAG_FRENZY)) {
		printfl("ENTERS_A_BATTLE_FRENZY", actor->get_name());
		playMdSfx(0x32);
	}

	actor->set_obj_flag(ACTOR_MD_OBJ_FLAG_FRENZY, true);

	if (obj->qty > 1)
		obj->qty -= 1;
	else
		removeObj(obj);
}

void MDScripts::useSpittoon(Obj *obj, Actor *actor) {
	printl(actor->get_obj_flag(ACTOR_MD_OBJ_FLAG_HYPOXIA) ? "YOUR_MOUTH_IS_TOO_DRY" : "YOU_SPIT_INTO_THE_SPITTOON");
}

void MDScripts::useGong(Obj *obj, const Target &target, Actor *actor, int targetX, int targetY, int targetZ) {
	printl("GONG");
	playMdSfx(0xf);
}

void MDScripts::useMusicalInstrument(Obj *obj, Actor *actor) {
	switch (obj->obj_n) {
	case 280: // OBJ_CYMBALS
		printl("CHING");
		playMdSfx(0x36);
		break;
	case 281: // OBJ_TAMBORINE
		printl("SHING");
		playMdSfx(0x35);
		break;
	case 282: // OBJ_DRUM
		printl("THUMP_THUMP");
		playMdSfx(0x34);
		wait(100);
		playMdSfx(0x34);
		break;
	case 283: // OBJ_ACCORDION
		printl("WHEEEEZE");
		playMdSfx(0x37);
		break;
	default:
		printfl("YOU_PLAY_THE", Target(obj).name().c_str());
		break;
	}
}

void MDScripts::useWrenchOnSwitchbar(Obj *obj, const Target &target, Actor *actor, int targetX, int targetY, int targetZ) {
	const uint16 OBJ_TURNTABLE = 413;
	Obj *switchBar = target._obj;
	if (!switchBar)
		return;

	if (switchBar->quality != 1) {
		printl("THE_SWITCH_IS_LOOSE");
		playMdSfx(0x1f);
		switchBar->quality = 1;
		return;
	}

	if (switchBar->is_on_map()) {
		Obj *turntable = mapGetObj(switchBar->x - 1, switchBar->y - 1, switchBar->z, OBJ_TURNTABLE);
		if (turntable) {
			switchBar->frame_n = turntable->frame_n;
			switchBar->quality = 0;
			printl("THE_SWITCH_IS_FASTENED");
			playMdSfx(0x1f);
			return;
		}
	}

	printl("THIS_SWITCH_CANNOT_BE_FIXED");
	playMdSfx(0x5);
}

void MDScripts::useWrenchOnDrill(Obj *obj, const Target &target, Actor *actor, int targetX, int targetY, int targetZ) {
	const uint16 OBJ_DRILL_CART = 439;
	const uint16 OBJ_ASSEMBLED_DRILL = 441;
	Obj *drill = target._obj;
	Obj *drillCart = nullptr;

	if (drill && drill->is_on_map())
		drillCart = mapGetObj(drill->x, drill->y, drill->z, OBJ_DRILL_CART);

	if (!drillCart) {
		printl("THE_DRILL_MUST_BE_INSTALLED_ONTO_A_DRILL_CART");
		return;
	}

	Obj *assembled = newObj(OBJ_ASSEMBLED_DRILL);
	assembled->frame_n = 1;
	addToWorld(assembled, MapCoord(drill->x, drill->y, drill->z));
	removeObj(drill);
	removeObj(drillCart);
	printl("THE_DRILLS_POWER_IS_CONNECTED");
	playMdSfx(0x1f);
}

void MDScripts::useWrenchOnPanel(Obj *obj, const Target &target, Actor *actor, int targetX, int targetY, int targetZ) {
	const uint16 OBJ_CABINET = 457;
	Obj *panel = target._obj;
	if (!panel || !panel->is_on_map()) {
		printl("IT_HAS_NO_EFFECT");
		return;
	}

	int quality = panel->quality;
	int panelQty = panel->qty;

	if (quality == 0) {
		panel->quality = 1;
		printl("THE_PANEL_IS_LOOSE");
		playMdSfx(0x1f);
		return;
	}

	if (quality & 2) {
		printl("IT_MUST_BE_REPAIRED_FIRST");
		playMdSfx(0x5);
		return;
	}

	Obj *cabinet = mapGetObj(panel->x, panel->y, panel->z, OBJ_CABINET);
	if (!cabinet) {
		printl("PANELS_ARE_ONLY_INSTALLED_ONTO_CABINETS");
		playMdSfx(0x5);
		return;
	}

	quality = cabinet->quality;
	if ((quality == 0 && panelQty != 0) ||
			(quality != 0 && quality <= 3 && panelQty == 0 && panel->frame_n != quality - 1) ||
			(quality != 0 && (quality > 3 || panelQty != 0) && quality != panelQty)) {
		printl("THIS_CABINET_REQUIRES_A_DIFFERENT_TYPE_OF_PANEL");
		playMdSfx(0x5);
		return;
	}

	panel->quality -= 1;
	printl("THE_PANEL_IS_FASTENED");
	playMdSfx(0x1f);

	if (panel->quality == 3) {
		Actor *dreamMachine = getActor(0x74);
		dreamMachine->set_flag(3);
		if (dreamMachine->get_flag(0))
			getActor(0x60)->set_flag(2);
	}
}

void MDScripts::useOxiumBin(Obj *obj, Actor *actor) {
	const uint16 OBJ_BLOB_OF_OXIUM = 131;

	if (!mapCanReachPoint(actor->get_x(), actor->get_y(), obj->x, obj->y, obj->z)) {
		printl("BLOCKED");
		return;
	}

	Obj *oxium = newObj(OBJ_BLOB_OF_OXIUM);
	oxium->qty = 20;

	if (actor->can_carry_object(oxium)) {
		actor->inventory_add_object(oxium, nullptr, true);
		printl("YOU_GET_TWO_HANDFULS_OF_OXIUM_FROM_THE_BIN");
		getActor(0x12)->set_flag(4);
	} else {
		delete_obj(oxium);
		printl("YOU_ARE_CARRYING_TOO_MUCH_ALREADY");
	}
}

void MDScripts::useSledgeHammerOnReplacementTrackToBrokenTrack(Obj *obj, const Target &target, Obj *toObj, Actor *actor) {
	const uint16 OBJ_TRACK = 414;

	playMdSfx(0x20);
	printl("WHICH_SECTION_OF_RAIL_NEEDS_FIXING");
	if (target._obj)
		removeObj(target._obj);
	toObj->obj_n = OBJ_TRACK;
}

void MDScripts::usePliersOnSpoolToTower(Obj *obj, const Target &target, Obj *toObj, Actor *actor) {
	const uint16 OBJ_RUBBER_GLOVES = 38;
	const uint16 OBJ_CHAMBER1 = 315;
	ActorManager *actorManager = Game::get_game()->get_actor_manager();

	if (actorFindMaxWrappedXYDistance(actor, toObj->x, toObj->y) > 1) {
		printl("THE_WORK_IS_TO_PRECISE_TO_PERFORM_TELEKINETICALLY");
		return;
	}

	if (!actorManager->get_actor(0x73)->get_flag(2) || actorManager->get_actor(0x71)->get_flag(3)) {
		printl("THE_CABLE_DOES_NOT_NEED_REPLACEMENT");
		return;
	}

	if (!actorIsHoldingObj(actor, OBJ_RUBBER_GLOVES)) {
		actor->hit(randomRange(0, actor->get_maxhp() / 2), true);

		Actor *spector = actorManager->get_actor(2);
		if (isActorAbleToTalkToPlayer(spector)) {
			spector->set_flag(2);
			actorTalk(spector);
		}
		return;
	}

	playMdSfx(0x1f);
	if (target._obj)
		removeObj(target._obj);
	actorManager->get_actor(0x73)->set_flag(4);
	playMidgameSequence(3);

	Common::Array<Obj *> objs;
	getObjsInArea(0x3d0, 0x1d0, 0, 32, 17, objs);
	for (uint i = 0; i < objs.size(); ++i) {
		if (objs[i]->obj_n == 215) // OBJ_POWER_CABLE1
			objs[i]->obj_n = 214; // OBJ_POWER_CABLE
	}

	objs.clear();
	findObjs(0, OBJ_CHAMBER1, objs);
	for (uint i = 0; i < objs.size(); ++i)
		removeObj(objs[i]);
}

void MDScripts::useSwitchBar(Obj *obj, Actor *actor) {
	const uint16 OBJ_TURNTABLE = 413;
	const uint16 OBJ_RAIL_CAR = 410;

	obj->frame_n = (obj->frame_n == 0) ? 1 : 0;

	Obj *turntable = obj->is_on_map() ? mapGetObj(obj->x - 1, obj->y - 1, obj->z, OBJ_TURNTABLE) : nullptr;
	if (!turntable) {
		printl("IT_HAS_NO_EFFECT");
		return;
	}

	if (obj->quality == 1) {
		printl("IT_TURNS_LOOSELY");
		return;
	}

	turntable->frame_n = obj->frame_n;

	Obj *railcar = mapGetObj(obj->x - 1, obj->y - 1, obj->z, OBJ_RAIL_CAR);
	if (railcar) {
		railcar->frame_n -= railcar->frame_n % 2;
		railcar->frame_n += turntable->frame_n;
	}
}

void MDScripts::useReadingMaterial(Obj *obj, Actor *actor) {
	if (obj->quality == 0) {
		Common::String signatures;

		if (getActor(5)->get_flag(3))
			signatures += " Richard Sherman";
		signatures += "\n";

		if (getActor(0x19)->get_flag(3))
			signatures += "  Capt. Gregory Duprey";
		signatures += "\n";

		if (getActor(0x1a)->get_flag(3))
			signatures += "  Doctor David Yellin";
		signatures += "\n";

		displayTextInScrollGump(Common::String::format(_lang.get("AFFIDAVIT"),
			Game::get_game()->get_player()->get_name(), signatures.c_str()));
		return;
	}

	Common::String text;
	if (loadTextFromLzc("mdtext.lzc", obj->quality - 1, text))
		displayTextInScrollGump(text);
	else
		printl("YOU_CANT_READ_IT");
}

void MDScripts::usePoolTable(Obj *obj, Actor *actor) {
	const uint16 OBJ_POOL_CUE = 401;
	const uint16 OBJ_POOL_TABLE_END = 400;

	if (actor->inventory_get_readied_obj_n(ACTOR_ARM) != OBJ_POOL_CUE) {
		printl("YOU_NEED_A_POOL_CUE");
		return;
	}

	uint16 leftX = wrapCoord(obj->x - 1, obj->z);

	Obj *poolTable1 = mapGetObj(leftX, obj->y, obj->z, OBJ_POOL_TABLE_END);
	if (!poolTable1) {
		poolTable1 = newObj(OBJ_POOL_TABLE_END);
		poolTable1->frame_n = randomRange(0, 6);
		if (!addToWorld(poolTable1, MapCoord(leftX, obj->y, obj->z)))
			return;
	}

	Obj *poolTable2 = mapGetObj(obj->x, obj->y, obj->z, OBJ_POOL_TABLE_END);
	if (!poolTable2) {
		poolTable2 = newObj(OBJ_POOL_TABLE_END);
		poolTable2->frame_n = randomRange(0, 6);
		if (!addToWorld(poolTable2, MapCoord(obj->x, obj->y, obj->z)))
			return;
	}

	for (int i = 1; i <= 10; ++i) {
		if (i != 1)
			wait(randomRange(200, 500));

		playMdSfx(0x1c + randomRange(0, 2));
		wait(randomRange(10, 200));
		playMdSfx(0x1c + randomRange(0, 2));

		poolTable1->frame_n = randomRange(0, 6);
		poolTable2->frame_n = randomRange(0, 6);

		if (randomRange(0, 40) >= actorDexAdjusted(actor))
			break;
	}

	if (randomRange(0, 40) < actorDexAdjusted(actor))
		printl("GOOD_SHOT_OLD_BEAN");
}

void MDScripts::useReadyObj(Obj *obj, Actor *actor) {
	if (!actor->can_carry_object(obj)) {
		printl("YOU_ARE_CARRYING_TOO_MUCH_ALREADY");
		return;
	}

	if (obj->is_readied())
		return;

	Game::get_game()->get_obj_manager()->unlink_from_engine(obj);
	Game::get_game()->get_obj_manager()->moveto_inventory(obj, actor);
	actor->add_readied_object(obj);
}

void MDScripts::useHeartstoneOnMetalWoman(Obj *obj, const Target &target, Actor *actor, int targetX, int targetY, int targetZ) {
	Obj *metalWoman = target._obj;
	if (!metalWoman)
		return;

	if (metalWoman->quality == 1) {
		printl("THE_ROBOT_ALREADY_HAS_A_HEARTSTONE");
		return;
	}

	metalWoman->quality = 1;
	removeObj(obj);
	playMdSfx(0x1f);
	printl("THE_HEARTSTONE_IS_INSTALLED");
}

void MDScripts::useManuscriptOnMailbox(Obj *obj, const Target &target, Actor *actor, int targetX, int targetY, int targetZ) {
	Actor *twain = getActor(0x57);

	finishDreamQuest(twain);
	twain->set_flag(6);
	actorTalk(twain);
	wakeFromDream();
}

void MDScripts::useOilOnDoor(Obj *obj, const Target &target, Actor *actor, int targetX, int targetY, int targetZ) {
	if (obj->obj_n == 235 && obj->frame_n != 4) { // OBJ_POTASH is only oil on frame 4
		printl("IT_HAS_NO_EFFECT");
		return;
	}

	Obj *door = target._obj;
	if (!door)
		return;

	if ((door->quality & 0x80) == 0) {
		printl("THIS_DOOR_IS_NOT_RUSTED");
		return;
	}

	door->quality &= 0x7f; // Unset the rusted bit
	printl("THE_DOOR_IS_UNSTUCK");
	playMdSfx(4);

	if (Game::get_game()->get_obj_manager()->is_stackable(obj) && obj->qty > 1)
		obj->qty -= 1;
	else
		removeObj(obj);
}

void MDScripts::useOilOnDreamDoor(Obj *obj, const Target &target, Actor *actor, int targetX, int targetY, int targetZ) {
	const Tile *tile = Game::get_game()->get_game_map()->get_tile(wrap_signed_coord(targetX, targetZ),
		wrap_signed_coord(targetY, targetZ), targetZ, false);

	if (!tile || tile->tile_num != 8) {
		printl("IT_HAS_NO_EFFECT");
		return;
	}

	Actor *melies = getActor(0x51);
	melies->set_flag(6);
	actorTalk(melies);
	finishDreamQuest(melies);
	wakeFromDream();
}

void MDScripts::useHandMirror(Obj *obj, Actor *actor) {
	if (actor->get_x() >= 0x76 && actor->get_x() <= 0x78 && actor->get_y() >= 0xca && actor->get_y() <= 0xcc &&
			actor->get_z() == 2) {
		Actor *lowell = getActor(0x50);

		lowell->set_flag(6);
		actorTalk(lowell);
		finishDreamQuest(lowell);
		wakeFromDream();
	} else {
		printl("YOU_SEE_YOURSELF");
	}
}

void MDScripts::useRadium(Obj *obj, const Target &target, Actor *actor, int targetX, int targetY, int targetZ) {
	const uint16 OBJ_POWER_UNIT = 290;
	const uint16 OBJ_BLOCK_OF_RADIUM = 448;
	const uint16 OBJ_CHIP_OF_RADIUM = 449;
	Obj *targetObj = target._obj;
	if (!targetObj)
		return;

	actorRadiationCheck(actor, obj);

	if (obj->obj_n == OBJ_BLOCK_OF_RADIUM) {
		if (targetObj->obj_n == OBJ_BLOCK_OF_RADIUM && targetObj->is_on_map()) {
			Obj *powerUnit = mapGetObj(targetObj->x, targetObj->y, targetObj->z, OBJ_POWER_UNIT);
			if (powerUnit)
				targetObj = powerUnit;
		}

		if (targetObj->obj_n != OBJ_POWER_UNIT) {
			printl("IT_HAS_NO_EFFECT");
			return;
		}

		if (targetObj->frame_n == 1) {
			printl("RADIUM_HAS_ALREADY_BEEN_INSTALLED");
			return;
		}

		removeObj(obj);
		playMdSfx(4);
		printl("THE_RADIUM_HAS_BEEN_INSTALLED");
		targetObj->frame_n = 1;

		Actor *dreamMachine = getActor(0x74);
		dreamMachine->set_flag(2);
		if (dreamMachine->get_flag(0))
			getActor(0x60)->set_flag(2);
	} else if (obj->obj_n == OBJ_CHIP_OF_RADIUM) {
		if (targetObj->qty >= 0xf0) {
			printfl("THE_OBJ_IS_FULLY_CHARGED", Target(targetObj).name().c_str());
			return;
		}

		int qty = inputSelectObjQty(obj);
		if (qty == 0)
			return;

		if (targetObj->qty + qty * 30 > 0xf0) {
			qty = (0xf0 - targetObj->qty + 29) / 30;
			printfl("THE_OBJ_ONLY_NEEDED_N_RADIUM_BLOCKS", Target(targetObj).name().c_str(), qty);
			printfl("THE_OBJ_IS_FULLY_CHARGED", Target(targetObj).name().c_str());
			targetObj->qty = 0xf0;
		} else {
			targetObj->qty += qty * 30;
		}

		if (obj->qty == qty)
			removeObj(obj);
		else
			obj->qty -= qty;
	}
}

void MDScripts::useHeadGear(Obj *obj, const Target &target, Actor *actor, int targetX, int targetY, int targetZ) {
	const uint16 OBJ_DREAM_MACHINE1 = 288;
	const uint16 OBJ_DREAM_MACHINE2 = 289;
	Obj *machine = target._obj;
	if (!machine)
		return;

	if (machine->obj_n == OBJ_DREAM_MACHINE2) {
		machine = mapGetObj(machine->x, machine->y, machine->z, OBJ_DREAM_MACHINE1);
		if (!machine) {
			printl("IT_HAS_NO_EFFECT");
			return;
		}
	}

	Obj *headgear = newObj(100);
	headgear->frame_n = 1;
	addToWorld(headgear, MapCoord(machine->x, machine->y, machine->z));

	printl("THE_HEADGEAR_IS_INSTALLED");

	Actor *dreamMachine = getActor(0x74);
	dreamMachine->set_flag(4);
	if (dreamMachine->get_flag(0))
		getActor(0x60)->set_flag(2);
}

void MDScripts::useDreamMachine(Obj *panel) {
	const uint16 OBJ_DREAM_MACHINE = 100;
	const uint16 OBJ_DREAM_MACHINE1 = 288;
	const uint16 OBJ_DREAM_MACHINE2 = 289;
	const uint16 OBJ_POWER_UNIT = 290;
	const uint16 OBJ_UNCONSCIOUS_MARTIAN = 291;
	const uint16 OBJ_METAL_WOMAN = 287;

	if (getActor(0x46)->get_flag(3)) {
		printl("THE_DREAM_MACHINES_SEEM_TO_HAVE_CEASED_FUNCTIONING");
		return;
	}

	uint8 z = panel->z;
	bool havePowerUnit = false;
	bool haveDreamQuality = false;
	bool headgearInstalled = false;
	bool haveSeat = false;
	int dreamQuality = 0;
	int seatX = 0;
	int seatY = 0;

	Common::Array<Obj *> objs;
	getObjsInArea(panel->x - 5, panel->y - 5, z, 11, 11, objs);

	for (uint i = 0; i < objs.size(); ++i) {
		Obj *obj = objs[i];

		if (obj->obj_n == OBJ_POWER_UNIT && obj->frame_n == 1) {
			havePowerUnit = true;
		} else if (obj->obj_n == OBJ_DREAM_MACHINE) {
			headgearInstalled = true;
		} else if (obj->obj_n == OBJ_DREAM_MACHINE2) {
			haveSeat = true;
			seatX = obj->x;
			seatY = obj->y;
		} else if (obj->obj_n == OBJ_DREAM_MACHINE1) {
			haveDreamQuality = true;
			dreamQuality = obj->quality;
		}
	}

	if (!havePowerUnit || !haveDreamQuality || !headgearInstalled || !haveSeat) {
		printl("THE_MACHINE_DOES_NOT_WORK");
		return;
	}

	Actor *actorInSeat = mapGetActor(seatX, seatY, z);

	Obj *martianObj = mapGetObj(seatX, seatY, z, OBJ_UNCONSCIOUS_MARTIAN);
	if (martianObj) {
		if (getActor(0x21)->get_flag(2) || !getActor(0x61)->get_flag(4)) {
			printl("IT_HAS_NO_EFFECT");
		} else {
			playMidgameSequence(5);

			Actor *martian = getActor(0x21);
			actorTalk(martian);
			martian->set_flag(2);
			// FIXME move the martian to the seat and actually kill it
			martian->set_hp(0);
			removeObj(martianObj);
		}

		return;
	}

	Obj *metalWomanObj = mapGetObj(seatX, seatY, z, OBJ_METAL_WOMAN);
	if (metalWomanObj) {
		// FIXME implement the metal woman activation logic
		if (metalWomanObj->quality == 0)
			printl("IT_HAS_NO_EFFECT");
	} else if (actorInSeat) {
		if (z != 0) {
			wakeFromDream();
			return;
		}

		if (dreamQuality < 2)
			getActor(0x60)->set_flag(3);

		if (dreamQuality == 3) {
			// FIXME call sub_3F624 when flag 0 of actor 0x46 is set
			if (!getActor(0x46)->get_flag(0))
				printl("THE_MACHINE_DOES_NOT_WORK");
		} else {
			actorUseDreamMachine(actorInSeat, dreamQuality);
		}
	} else {
		printl("THERE_IS_NOBODY_SITTING_IN_THE_MACHINE");
	}
}

void MDScripts::usePanel(Obj *obj, Actor *actor) {
	const uint16 OBJ_CABINET = 457;

	if (obj->quality & 2) {
		printl("THE_PANEL_IS_BROKEN");
		return;
	}

	if (obj->quality & 1) {
		printl("THE_PANEL_IS_NOT_INSTALLED");
		return;
	}

	Obj *cabinet = mapGetObj(obj->x, obj->y, obj->z, OBJ_CABINET);
	if (!cabinet) {
		printl("PANELS_ARE_ONLY_INSTALLED_ONTO_CABINETS");
		return;
	}

	switch (cabinet->quality) {
	case 0:
		printl("YOU_ARE_COMPLETELY_UNSURE_WHAT_YOU_JUST_DID");
		break;
	case 1:
		printl("YOU_ACTUATE_THE_MECHANISM_TO_NO_APPARENT_EFFECT");
		break;
	case 2:
		printl("STATUS_LIGHTS_CHANGE_BUT_YOU_SEE_NO_OTHER_EFFECT");
		break;
	case 3:
		printl("LIGHTS_FLASH_AND_CHANGE_COLOR_BUT_NOTHING_ELSE_HAPPENS");
		break;
	case 4:
		useDreamMachine(obj);
		break;
	case 5: // The sprayer system, lens controls and pump controls aren't implemented in the original
	case 6:
	case 7:
		break;
	default:
		printl("IT_HAS_NO_EFFECT");
		break;
	}
}

bool MDScripts::hasMinotaurLeftTheShop(uint8 curZ) {
	for (int i = 1; i < 256; ++i) {
		Actor *actor = getActor(i);

		if (actor->get_obj_n() == 398 && actor->get_z() == curZ && !getActor(0x54)->get_flag(6) &&
				actor->get_x() >= 0x37 && actor->get_y() <= 0x37) // OBJ_MINOTAUR
			return true;
	}

	return false;
}

void MDScripts::useSwitch(Obj *obj, Actor *actor) {
	int switchQty = obj->qty;
	int switchQuality = obj->quality;
	Obj *targetObj = nullptr;
	Obj *switches[4];
	int numSwitches = 0;

	Common::Array<Obj *> objs;
	getObjsInArea(obj->x - 32, obj->y - 32, obj->z, 64, 64, objs);

	for (uint i = 0; i < objs.size(); ++i) {
		Obj *item = objs[i];

		if (item->obj_n == 179 || item->obj_n == 227) { // OBJ_CLOSED_DOOR, OBJ_DOOR3
			if ((item->quality & 0x7f) == switchQty)
				targetObj = item;
		} else if (item->obj_n == 311 || item->obj_n == 312 || item->obj_n == 196) {
			if (numSwitches < 4)
				switches[numSwitches++] = item;
		}
	}

	if (!targetObj) {
		printl("STRANGELY_NOTHING_HAPPENS");
		return;
	}

	if (numSwitches == 0) {
		printl("STRANGELY_IT_DOESNT_MOVE");
		return;
	}

	int frameN = 0;
	if (switchQuality == 1) {
		if ((targetObj->quality & 0x80) == 0) {
			targetObj->quality |= 0x80;
		} else {
			targetObj->quality &= 0x7f;
			frameN = 1;
		}
	} else {
		int oldFrameN = targetObj->frame_n;
		targetObj->frame_n = (oldFrameN & 2) + 5;
		playDoorSfx();

		if (oldFrameN < 4) {
			targetObj->frame_n = (oldFrameN & 2) + 9;
			frameN = 1;
		} else {
			targetObj->frame_n = (oldFrameN & 2) + 1;

			if (_inDreamMode && _currentDreamStage == 0x44 && hasMinotaurLeftTheShop(targetObj->z)) {
				completeTiffanyStage();
				return;
			}
		}

		playDoorSfx();
	}

	for (int i = 0; i < numSwitches; ++i)
		switches[i]->frame_n = frameN;

	playMdSfx(0x11);
}

void MDScripts::useSwitchDevice(Obj *obj, Actor *actor) {
	switch (obj->quality) {
	case 1:
	case 2:
		useSwitch(obj, actor);
		break;
	case 10: // FIXME the drawbridge lever and the cheat levers
	case 20:
	case 21:
		break;
	default:
		printl("WHAT_AN_ODD_LEVER");
		break;
	}
}

void MDScripts::useDreamstuff(Obj *obj, Actor *actor) {
	printl("YOU_IMAGINE");

	Common::Array<Obj *> items;
	getContainerObjs(obj, items);

	if (items.empty()) {
		printl("NOTHING");
		return;
	}

	Obj *item = items[0];
	ObjManager *objManager = Game::get_game()->get_obj_manager();

	if (item->obj_n < 342) { // OBJ_POOR_MONK
		print(Common::String(objManager->look_obj(item, true)) + "\n");

		if (obj->is_on_map()) {
			objManager->moveto_map(item, MapCoord(obj->x, obj->y, obj->z));
		} else if (Actor *holder = obj->get_actor_holding_obj()) {
			objManager->moveto_inventory(item, holder);
		} else if (Obj *parent = obj->get_container_obj()) {
			objManager->moveto_container(item, parent);
		}
	} else {
		print(Target(item).name() + "\n");

		Actor *spawned;
		if (Game::get_game()->get_actor_manager()->create_temp_actor(item->obj_n, NO_OBJ_STATUS, actor->get_x(),
				actor->get_y(), actor->get_z(), ACTOR_ALIGNMENT_NEUTRAL, ACTOR_WT_ASSAULT, &spawned)) {
			actorInit(spawned, (ActorAlignment)(obj->quality + 1));
			tossActor(spawned, actor->get_x(), actor->get_y(), actor->get_z(), 1);
			spawned->set_worktype(item->quality);
		}
	}

	removeObj(obj);
}

void MDScripts::useObjOnSprayGun(Obj *obj, const Target &target, Actor *actor, int targetX, int targetY, int targetZ) {
	Obj *sprayGun = target._obj;
	if (!sprayGun)
		return;

	if (obj->obj_n == 119 && sprayGun->quality == 0) { // OBJ_BOTTLE_OF_GREEN_PAINT
		sprayGun->quality = 1;
		sprayGun->qty = 20;
		printl("SPRAY_GUN_GREEN_PAINT");
	} else if (obj->obj_n == 128 && sprayGun->quality != 0) { // OBJ_WEED_KILLER
		sprayGun->quality = 0;
		sprayGun->qty = 10;
		printl("SPRAY_GUN_WEED_KILLER");
	} else {
		sprayGun->qty = MIN<int>(sprayGun->qty + 10, 0x1e);
		printl("SPRAY_GUN_10_MORE_CHARGES");
	}

	removeObj(obj);
}

void MDScripts::useSprayGun(Obj *obj, const Target &target, Actor *actor, int targetX, int targetY, int targetZ) {
	if (obj->qty == 0) {
		printl("THERE_IS_NOTHING_IN_THE_GUN");
		return;
	}

	if (target.isNull()) {
		printl("IT_HAS_NO_EFFECT");
		return;
	}

	if (obj->quality != 0) {
		obj->qty -= 1;

		if (target.isActor() && target.objN() == 145) { // OBJ_MONSTER_FOOTPRINTS
			target._actor->set_obj_n(364); // OBJ_PROTO_MARTIAN
			printfl("BECOMES_VISIBLE", target.name().c_str());
		} else if (target.isActor() && target._actor->is_invisible()) {
			target._actor->set_invisible(false);
			printfl("BECOMES_VISIBLE", target.name().c_str());
		} else if (!target.isActor() && target._obj->is_invisible()) {
			target._obj->set_invisible(false);
			printfl("BECOMES_VISIBLE", target.name().c_str());
		} else {
			printl("IT_HAS_NO_EFFECT");
		}
	} else if (target.isActor()) {
		attackTargetWithWeapon(actor, target.x(), target.y(), Target(obj));
	} else if (isPlantObj(target._obj)) {
		hitTarget(target, RED_HIT_TILE);
		printl("YOU_KILLED_THE_PLANT");

		if (target._obj->obj_n == 205) { // OBJ_VINE
			getActor(0x30)->set_flag(7);
			removeObj(target._obj);
		} else if (target._obj->obj_n == 408) { // OBJ_TREE
			target._obj->obj_n = 166; // OBJ_DEAD_TREE
		} else {
			removeObj(target._obj);
		}
	} else {
		obj->qty -= 1;
		printl("IT_HAS_NO_EFFECT");
	}
}

void MDScripts::useAssembledDrill(Obj *obj, Actor *actor) {
	const uint16 OBJ_IRON_ORE = 445;
	const uint16 OBJ_VEIN_OF_COAL = 446;
	const uint16 OBJ_CAVE_IN = 213;

	playMdSfx(0x10);

	int x = obj->x;
	int y = obj->y;
	uint8 z = obj->z;

	if (obj->frame_n == 1)
		--x;
	else if (obj->frame_n == 3)
		--y;
	else if (obj->frame_n == 4)
		++y;
	else
		++x;

	Obj *targetObj = nullptr;
	Common::Array<Obj *> objs;
	getObjsAt(wrap_signed_coord(x, z), wrap_signed_coord(y, z), z, objs);
	for (uint i = 0; i < objs.size() && !targetObj; ++i) {
		if (objs[i]->obj_n == OBJ_IRON_ORE || objs[i]->obj_n == OBJ_VEIN_OF_COAL || objs[i]->obj_n == OBJ_CAVE_IN)
			targetObj = objs[i];
	}

	if (!targetObj)
		targetObj = mapGetObj(x, y, z, OBJ_CAVE_IN, true);

	uint16 drilledMaterial = 0;

	if (!targetObj) {
		if (canDrillAtLoc(x, y, z)) {
			drilledMaterial = 442; // OBJ_PILE_OF_ROCKS
		} else {
			// FIXME attack an actor in the way with 40 points of damage
			if (!mapGetActor(x, y, z))
				printl("THERE_IS_NOTHING_TO_DRILL_INTO");
			return;
		}
	} else if (targetObj->obj_n == OBJ_IRON_ORE) {
		drilledMaterial = 443; // OBJ_PILE_OF_IRON_ORE
	} else if (targetObj->obj_n == OBJ_VEIN_OF_COAL) {
		drilledMaterial = 444; // OBJ_PILE_OF_COAL
	}

	if (drilledMaterial != 0) {
		MapCoord spoilLocation;

		if (getFreeLocationAroundDrill(obj, spoilLocation)) {
			Obj *spoil = newObj(drilledMaterial);
			addToWorld(spoil, spoilLocation);
		} else {
			printl("THERE_IS_NO_ROOM_LEFT_FOR_THE_ORE");
		}
	}

	if (targetObj) {
		if (targetObj->quality > 1)
			targetObj->quality -= 1;
		else
			removeObj(targetObj);
	}
}

bool MDScripts::getFreeLocationAroundDrill(Obj *drill, MapCoord &pos) {
	Map *map = Game::get_game()->get_game_map();

	for (int i = 0; i < 8; ++i) {
		pos = MapCoord(wrap_signed_coord(drill->x + AROUND_DRILL_X[i], drill->z),
			wrap_signed_coord(drill->y + AROUND_DRILL_Y[i], drill->z), drill->z);

		Obj *obj = mapGetObj(pos.x, pos.y, pos.z);
		if (!obj || isBlood(obj->obj_n)) {
			const Tile *tile = map->get_tile(pos.x, pos.y, pos.z, false);
			if (tile && !tileGetFlag(tile->tile_num, 1, 1)) // Not blocked
				return true;
		}
	}

	return false;
}

bool MDScripts::canDrillAtLoc(int x, int y, uint8 z) {
	const Tile *tile = Game::get_game()->get_game_map()->get_tile(wrap_signed_coord(x, z), wrap_signed_coord(y, z), z, false);

	return tile && tile->tile_num >= 0xf0 && tile->tile_num <= 0xfb;
}

void MDScripts::restHealParty(int hoursToRest) {
	Common::Array<Actor *> members;
	getPartyMembers(members);

	for (uint i = 0; i < members.size(); ++i) {
		Actor *actor = members[i];
		int hp = randomRange(3, 8) + randomRange(3, 8) * (hoursToRest / 2);
		int maxHp = actorGetMaxHp(actor);

		actor->set_hp(MIN(actor->get_hp() + hp, maxHp));
	}
}

void MDScripts::restLevelUpActor(Actor *actor) {
	static const int EXP_LEVELS[9] = { 0, 100, 200, 400, 800, 1600, 3200, 6400, 32767 };

	if (actor->get_actor_num() > 15 || actor->get_level() > 8)
		return;

	if (actor->get_exp() <= EXP_LEVELS[actor->get_level()])
		return;

	actor->set_level(actor->get_level() + 1);

	int maxHp = actorGetMaxHp(actor);
	actor->set_hp(MIN(actor->get_hp() + 30, maxHp));

	Game::get_game()->get_view_manager()->set_portrait_mode(actor, actor->get_name());

	uint16 objN = actor->get_obj_n();
	int gender = randomRange(0, 1);
	if (objN == 342 || objN == 343 || objN == 345 || (objN >= 347 && objN <= 353))
		gender = 0;
	else if (objN == 344 || objN == 346 || (objN >= 354 && objN <= 357))
		gender = 1;

	const char *pronoun = (gender == 1) ? "She" : "He";

	printfl("HAS_A_DREAM", actor->get_name());
	printfl("SEES_THREE_STONE_OBELISKS", pronoun);
	printfl("FEELS_DRAWN_TO_ONE_OF_THE_OBELISKS", pronoun);
	printfl("DOES_TOUCH_THE_OBELISK", actor->get_name());
	printl("WHICH_BHS");

	Common::String answer = inputSelect("bhs", false);

	if (answer == "B") {
		if (actor->get_intelligence() < 30)
			actor->set_intelligence(actor->get_intelligence() + 1);
	} else if (answer == "H") {
		if (actor->get_dexterity() < 30)
			actor->set_dexterity(actor->get_dexterity() + 1);
	} else if (answer == "S") {
		if (actor->get_strength() < 30)
			actor->set_strength(actor->get_strength() + 1);
	}
}

void MDScripts::useTent(Obj *obj, Actor *actor) {
	const uint16 OBJ_FOLDED_TENT = 106;
	const uint16 OBJ_TENT = 134;
	Game *game = Game::get_game();
	Player *player = game->get_player();
	Party *party = game->get_party();
	ObjManager *objManager = game->get_obj_manager();

	if (!player->in_party_mode()) {
		printl("NOT_WHILE_IN_SOLO_MODE");
		playMdSfx(5);
		return;
	}

	if (_inDreamMode) {
		printl("YOU_CANT_SLEEP_IN_A_DREAM");
		playMdSfx(5);
		return;
	}

	MapCoord playerLoc = player->get_actor()->get_location();
	MapCoord tentLoc = obj->is_on_map() ? MapCoord(obj->x, obj->y, obj->z) : playerLoc;

	for (int y = tentLoc.y - 2; y <= tentLoc.y; ++y) {
		for (int x = tentLoc.x - 1; x <= tentLoc.x + 1; ++x) {
			Obj *mapObj = mapGetObj(x, y, tentLoc.z);

			if (mapObj && mapObj->obj_n != OBJ_FOLDED_TENT && !tileGetFlag(objTileNum(mapObj), 3, 4)) {
				printfl("TENT_OBJ_IN_THE_WAY", Target(mapObj).name().c_str());
				playMdSfx(5);
				return;
			}
		}
	}

	for (int y = tentLoc.y - 2; y <= tentLoc.y; ++y) {
		for (int x = tentLoc.x - 1; x <= tentLoc.x + 1; ++x) {
			const Tile *tile = game->get_game_map()->get_tile(wrap_signed_coord(x, tentLoc.z),
				wrap_signed_coord(y, tentLoc.z), tentLoc.z, false);

			// The map tile is impassable
			if (tile && tileGetFlag(tile->tile_num, 1, 1)) {
				printl("THE_GROUND_IS_NOT_FLAT_ENOUGH");
				playMdSfx(5);
				return;
			}
		}
	}

	printl("REST");

	if (party->is_in_combat_mode()) {
		print(" - ");
		printl("NOT_WHILE_IN_COMBAT_MODE");
		playMdSfx(5);
		return;
	}

	if (foesAreNearby()) {
		printl("NOT_WHILE_FOES_ARE_NEAR");
		playMdSfx(5);
		return;
	}

	Actor *npc = npcsAreNearby();
	if (npc) {
		printfl("IS_TOO_NEAR_TO_SETUP_CAMP", npc->get_name());
		playMdSfx(5);
		return;
	}

	Common::Array<Actor *> members;
	getPartyMembers(members);

	bool poisoned = false;
	for (uint i = 0; i < members.size(); ++i) {
		if (members[i]->is_poisoned()) {
			poisoned = true;
			printfl("IS_POISONED", members[i]->get_name());
		}
	}

	if (poisoned) {
		printl("DO_YOU_REALLY_WANT_TO_SLEEP");
		Common::String answer = inputSelect("yn", false);
		if (answer == "N" || answer == "n")
			return;
	}

	bool partyIsUsingBerries = false;
	for (uint i = 0; i < members.size(); ++i) {
		uint8 actorNum = members[i]->get_actor_num();
		bool green = actorIsAffectedByBerries(actorNum, BERRY_GREEN);
		bool brown = actorIsAffectedByBerries(actorNum, BERRY_BROWN);

		if (!brown && !green)
			continue;

		partyIsUsingBerries = true;
		if (brown && green)
			printfl("COMPLAINS_OF_TOO_MUCH_LIGHT_AND_INANIMATE", members[i]->get_name());
		else if (brown)
			printfl("COMPLAINS_OF_TOO_MUCH_LIGHT", members[i]->get_name());
		else
			printfl("COMPLAINS_OF_INANIMATE_THINGS_TALKING", members[i]->get_name());
	}

	if (partyIsUsingBerries) {
		printl(party->get_party_size() == 1 ? "YOU_CANT_SLEEP" : "NOBODY_CAN_SLEEP");
		return;
	}

	// The player's location can't be changed from a script, so the tent goes where the player is
	int px = playerLoc.x;
	int py = playerLoc.y;
	uint8 pz = playerLoc.z;

	static const struct {
		int _offsetX, _offsetY, _frameN;
	} TENT_PARTS[] = {
		{ 0, -1, 3 }, { 1, -1, 5 }, { -1, 0, 6 }, { 1, 0, 9 }, { 0, 0, 8 }
	};

	Obj *tent = nullptr;
	for (uint i = 0; i < ARRAYSIZE(TENT_PARTS); ++i) {
		tent = newObj(OBJ_TENT);
		tent->frame_n = TENT_PARTS[i]._frameN;
		if (!objManager->moveto_map(tent, MapCoord(px + TENT_PARTS[i]._offsetX, py + TENT_PARTS[i]._offsetY, pz))) {
			delete_obj(tent);
			tent = nullptr;
		}
	}

	party->move(px, py, pz);
	wait(500);
	party->hide();

	if (tent)
		tent->frame_n = 7;

	int hour = game->get_clock()->get_hour();
	const char *time;
	int hoursToRest;

	if (hour < 7 || hour > 16) {
		time = _lang.get("SUNRISE");
		hoursToRest = (hour < 7) ? 7 - hour : 24 - hour + 7;
	} else {
		time = _lang.get("SUNSET");
		hoursToRest = 18 - hour;
	}

	printfl("REST_UNTIL", time);
	Common::String answer = inputSelect("yn", false);

	if (answer == "N" || answer == "n") {
		printl("HOW_MANY_HOURS");
		hoursToRest = (int)strtol(inputSelect("0123456789", true).c_str(), nullptr, 10);
	}

	_partyIsWarm = true;

	if (_hoursTillNextHealing == 0 && hoursToRest > 4) {
		restHealParty(hoursToRest);
		_hoursTillNextHealing = 6;
	}

	for (int i = 0; i < hoursToRest * 3; ++i) {
		advanceTime(20);
		wait(100);

		if (i < party->get_party_size())
			restLevelUpActor(party->get_actor(i));
	}

	for (uint i = 0; i < members.size(); ++i) {
		Actor *member = members[i];
		if (!member->is_poisoned())
			continue;

		if (randomRange(1, 8) + randomRange(1, 8) >= 15) {
			member->set_poisoned(false);
			printfl("FEELS_BETTER", member->get_name());
		} else if (member->get_hp() < hoursToRest * 2 + 5) {
			member->set_hp(5);
		} else {
			member->set_hp(member->get_hp() - (hoursToRest * 2 + 5));
		}
	}

	// Open the tent flap
	if (tent)
		tent->frame_n = 8;
	party->show();

	party->move(px, py + 1, pz);
	wait(500);

	// Remove the tent from the map
	Common::Array<Obj *> tents;
	findObjs(pz, OBJ_TENT, tents);
	for (uint i = 0; i < tents.size(); ++i) {
		Obj *part = tents[i];
		uint16 rightX = wrapCoord(tentLoc.x + 1, pz);
		uint16 leftX = wrapCoord(tentLoc.x - 1, pz);

		if ((part->x == tentLoc.x && part->y == tentLoc.y - 1) ||
				(part->x == rightX && part->y == tentLoc.y - 1) ||
				(part->x == leftX && part->y == tentLoc.y) ||
				(part->x == rightX && part->y == tentLoc.y) ||
				(part->x == tentLoc.x && part->y == tentLoc.y))
			removeObj(part);
	}

	_partyIsWarm = false;
}

} // End of namespace MD
} // End of namespace Scripts
} // End of namespace Ultima6
} // End of namespace Ultima
