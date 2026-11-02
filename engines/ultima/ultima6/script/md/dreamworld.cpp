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
#include "ultima/ultima6/core/obj_manager.h"
#include "ultima/ultima6/core/party.h"
#include "ultima/ultima6/core/player.h"
#include "ultima/ultima6/script/common/script_random.h"
#include "ultima/ultima6/script/md/md_scripts.h"
#include "ultima/ultima6/views/inventory_view.h"
#include "ultima/ultima6/views/view_manager.h"

namespace Ultima {
namespace Ultima6 {
namespace Scripts {
namespace MD {

namespace {

bool allTalkFlagsSet(const uint8 *actorNums, uint count, uint8 flag) {
	ActorManager *actorManager = Game::get_game()->get_actor_manager();

	for (uint i = 0; i < count; ++i) {
		if (!actorManager->get_actor(actorNums[i])->get_flag(flag))
			return false;
	}

	return true;
}

} // End of anonymous namespace

void MDScripts::finishDreamQuest(Actor *actor) {
	Game *game = Game::get_game();
	ActorManager *actorManager = game->get_actor_manager();
	const uint16 OBJ_DREAM_TELEPORTER = 461;
	const uint16 OBJ_OBELISK = 292;

	Schedule *schedule = actor->get_schedule(0);
	actor->move(schedule->x, schedule->y, schedule->z);

	Obj *obj = mapGetObj(schedule->x, schedule->y, schedule->z, OBJ_DREAM_TELEPORTER);
	if (obj)
		removeObj(obj);

	obj = mapGetObj(schedule->x, schedule->y - 1, schedule->z, OBJ_OBELISK);
	if (obj)
		removeObj(obj);

	actor->set_flag(7);

	static const uint8 FOUR_QUESTS[] = { 0x56, 0x54, 0x52, 0x55 };
	if (allTalkFlagsSet(FOUR_QUESTS, ARRAYSIZE(FOUR_QUESTS), 7))
		actorManager->get_actor(0x60)->set_flag(4);

	static const uint8 ALL_QUESTS[] = { 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57 };
	if (allTalkFlagsSet(ALL_QUESTS, ARRAYSIZE(ALL_QUESTS), 7))
		actorManager->get_actor(0x20)->set_flag(4);
}

void MDScripts::wakeFromDream() {
	Game *game = Game::get_game();
	GameClock *clock = game->get_clock();
	Party *party = game->get_party();
	ActorManager *actorManager = game->get_actor_manager();

	// FIXME dreamworld_cleanup_state() is called with actor zero by the original
	_objlist1d22Unk = 0;

	int minutes = 60 - clock->get_minute();
	int hour = clock->get_hour() + 1;
	if (hour >= 8)
		minutes += (24 - hour + 8) * 60;
	else
		minutes += (8 - hour) * 60;
	clock->inc_minute(minutes);

	// FIXME reset the walk direction modifier

	Common::Array<Actor *> members;
	getPartyMembers(members);
	for (uint i = 0; i < members.size(); ++i) {
		Actor *actor = members[i];
		actorClearBerryCounters(actor->get_actor_num());

		if (actor->is_poisoned()) {
			if (actor->get_hp() <= 30) {
				if (actor->get_hp() <= 10)
					actor->set_hp(1);
				else
					actor->set_hp(randomRange(1, 10));
			} else {
				actor->set_hp(randomRange(24, 30));
			}
		}
	}

	Actor *dreamActor = actorManager->get_actor(0);
	party->set_in_vehicle(false);
	dreamActor->set_invisible(true);
	party->show();

	Actor *leader = party->get_leader_actor();
	if (leader)
		game->get_player()->update_player(leader);
	party->set_in_combat_mode(false);

	_inDreamMode = false;
	actorManager->enable_temp_actor_cleaning(true);
	printl("YOU_WAKE_UP");

	InventoryView *inventoryView = game->get_view_manager()->get_inventory_view();
	inventoryView->lock_to_actor(false);
	inventoryView->set_party_member(0);
	game->get_view_manager()->set_inventory_mode();

	// Remove the dream actor's inventory, contents of containers first
	Common::Array<Obj *> inventory;
	getInventoryObjs(dreamActor, true, inventory);
	for (int i = (int)inventory.size() - 1; i >= 0; --i)
		removeObj(inventory[i]);

	Actor *avatar = actorManager->get_actor(1);
	avatar->set_moves_left((sint8)avatar->get_dexterity());
	avatar->set_worktype(WT_PLAYER);
}

void MDScripts::actorUseDreamMachine(Actor *actor, int dreamQuality) {
	Game *game = Game::get_game();
	GameClock *clock = game->get_clock();

	if (actor->get_actor_num() != 1) {
		// FIXME advance time till dawn
		printfl("ACTOR_DREAMS_UNTIL_DAWN_WHILE_THE_PARTY_WAITS", actor->get_name());
		actor->set_asleep(true);
		_partyIsWarm = true;
		advanceTime(60 - clock->get_minute());

		while (clock->get_hour() != 8) {
			advanceTime(20);
			wait(100);
		}

		_partyIsWarm = false;
		return;
	}

	playMidgameSequence(4);

	Party *party = game->get_party();
	game->get_player()->set_party_mode(party->get_actor(0));
	Actor *dreamActor = game->get_actor_manager()->get_actor(0);

	uint16 dreamX = 0x93;
	uint16 dreamY = 0x34;
	if (dreamQuality == 1) {
		dreamX = 0x64;
		dreamY = 0x3b;
	}

	// FIXME need to copy over more data from the avatar actor
	dreamActor->set_obj_n(actor->get_obj_n());
	dreamActor->set_frame_n(actor->get_frame_n());
	dreamActor->change_base_obj_n(actor->get_base_obj_n());
	dreamActor->set_worktype(WT_PLAYER);
	dreamActor->set_invisible(false);
	dreamActor->set_hp(actor->get_maxhp());

	dreamActor->move(dreamX, dreamY, 2);
	if (dreamActor != game->get_player()->get_actor())
		game->get_player()->update_player(dreamActor);
	party->set_in_vehicle(true);
	party->hide();
	_inDreamMode = true;
	game->get_actor_manager()->enable_temp_actor_cleaning(false);
	_prevPlayerX = 0;
	_prevPlayerY = 0;
	_currentDreamStage = 0;

	InventoryView *inventoryView = game->get_view_manager()->get_inventory_view();
	inventoryView->set_actor(dreamActor, true);
	inventoryView->lock_to_actor(true);
	game->get_view_manager()->set_inventory_mode();
}

void MDScripts::cleanupCliffFall() {
	ActorManager *actorManager = Game::get_game()->get_actor_manager();
	Actor *dreamActor = actorManager->get_actor(0);

	dreamActor->set_obj_n(actorManager->get_actor(1)->get_base_obj_n());
	dreamActor->set_frame_n(9);
}

void MDScripts::fallFromCliff() {
	Game *game = Game::get_game();
	Actor *dreamActor = game->get_actor_manager()->get_actor(0);

	printl("YOU_FALL_OFF_THE_CLIFF");
	dreamActor->set_obj_n(game->get_player()->get_gender() == 0 ? 0x126 : 0x127);
	dreamActor->set_frame_n(0);

	for (int y = dreamActor->get_y() - 8; y <= dreamActor->get_y(); ++y) {
		Actor *actor = mapGetActor(dreamActor->get_x(), y, dreamActor->get_z());
		if (actor && actor->get_obj_n() == 391) // OBJ_YOUR_MOTHER
			actor->die(false);
	}
}

void MDScripts::spawnYourMother() {
	MapCoord playerLoc = Game::get_game()->get_player()->get_actor()->get_location();
	Actor *mother;

	if (!Game::get_game()->get_actor_manager()->create_temp_actor(391, NO_OBJ_STATUS, playerLoc.x, playerLoc.y - 1,
			playerLoc.z, ACTOR_ALIGNMENT_NEUTRAL, ACTOR_WT_ASSAULT, &mother))
		return;

	actorInit(mother, ACTOR_ALIGNMENT_DEFAULT);
	mother->set_worktype(0x15);
	mother->set_combat_mode(0x15);
	mother->set_invisible(false);
	mother->move(playerLoc.x, playerLoc.y - 1, playerLoc.z);
}

void MDScripts::createPitcher(uint16 x, uint16 y, uint8 z) {
	const uint16 OBJ_GLASS_PITCHER = 217;

	Obj *obj = newObj(OBJ_GLASS_PITCHER);
	obj->frame_n = randomRange(0, 5) / 2;
	obj->qty = 3;

	if (!Game::get_game()->get_obj_manager()->moveto_map(obj, MapCoord(x, y, z)))
		delete_obj(obj);
}

void MDScripts::setupTiffanyStage() {
	Game *game = Game::get_game();
	Actor *tiffany = game->get_actor_manager()->get_actor(0x54);

	tiffany->clear_flag(0);
	tiffany->clear_flag(6);

	uint8 z = game->get_player()->get_actor()->get_z();

	Common::Array<Obj *> objs;
	getObjsInArea(0x21, 0x33, z, 0x1c, 0x10, objs);
	for (uint i = 0; i < objs.size(); ++i) {
		// OBJ_RED_THROW_RUG, OBJ_RED_CAPE, OBJ_GLASS_PITCHER, OBJ_BROKEN_CRYSTAL, OBJ_MINOTAUR
		uint16 objN = objs[i]->obj_n;
		if (objN == 161 || objN == 162 || objN == 217 || objN == 218 || objN == 398)
			removeObj(objs[i]);
	}

	const uint16 OBJ_RED_THROW_RUG = 161;
	Obj *rug = newObj(OBJ_RED_THROW_RUG);
	if (!game->get_obj_manager()->moveto_map(rug, MapCoord(0x24, 0x36, z)))
		delete_obj(rug);

	for (int i = 57; i <= 65; ++i) {
		if (i != 0x3e)
			createPitcher(0x37, i, z);
		if (i > 0x3a && i != 0x40)
			createPitcher(0x39, i, z);
		createPitcher(0x3b, i, z);
	}

	createPitcher(0x38, 0x39, z);
	createPitcher(0x38, 0x41, z);
	createPitcher(0x3a, 0x39, z);
	createPitcher(0x3a, 0x41, z);

	Actor *minotaur;
	if (game->get_actor_manager()->create_temp_actor(398, NO_OBJ_STATUS, 0x3b, 0x41, z, ACTOR_ALIGNMENT_NEUTRAL,
			ACTOR_WT_ASSAULT, &minotaur)) {
		actorInit(minotaur, ACTOR_ALIGNMENT_DEFAULT);
		minotaur->move(0x3b, 0x41, z);
		minotaur->set_worktype(0x16);
	}
}

void MDScripts::completeTiffanyStage() {
	Actor *tiffany = Game::get_game()->get_actor_manager()->get_actor(0x54);

	finishDreamQuest(tiffany);
	tiffany->set_flag(1);
	actorTalk(tiffany);
	wakeFromDream();
}

void MDScripts::dreamworldCleanupState(Obj *obj) {
	Game *game = Game::get_game();
	Actor *dreamActor = game->get_actor_manager()->get_actor(0);
	uint16 newStage = obj->status & 0xe5;

	if (_currentDreamStage != 0 && newStage != 0xa5 && newStage != 0xe5) {
		if (_currentDreamStage == 0x40)
			cleanupCliffFall();
	}

	if (newStage == 1) {
		_currentDreamStage = 0;
		return;
	}

	if (newStage == 0x24) {
		wakeFromDream();
		return;
	}

	if (_currentDreamStage != 0 && newStage == 0x40)
		fallFromCliff();

	if (newStage != 0xa5) {
		// FIXME clean up temp objects
		_objlist1d22Unk = 0;
	}

	actorClearBerryCounters(dreamActor->get_actor_num());
	game->get_player()->move(obj->quality, obj->qty, game->get_player()->get_actor()->get_z(), false);

	if (newStage == 0xe5) {
		if (!game->get_actor_manager()->get_actor(0x66)->get_flag(3))
			_objlist1d22Unk = 6;
	} else {
		if (newStage == 0x25)
			spawnYourMother();
		else if (newStage == 0x44)
			setupTiffanyStage();

		_currentDreamStage = newStage;
	}
}

} // End of namespace MD
} // End of namespace Scripts
} // End of namespace Ultima6
} // End of namespace Ultima
