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
#include "ultima/ultima6/core/player.h"
#include "ultima/ultima6/script/md/md_scripts.h"
#include "ultima/ultima6/script/script.h"
#include "ultima/ultima6/script/script_cutscene.h"

namespace Ultima {
namespace Ultima6 {
namespace Scripts {
namespace MD {

namespace {

// Worktypes the converse scripts can leave behind to be acted on after the conversation
enum {
	TALK_STATUS_NONE = -1,
	TALK_STATUS_END_GAME = 0x9,
	TALK_STATUS_WAKE_FROM_DREAM_1 = 0x34,
	TALK_STATUS_WAKE_FROM_DREAM_2 = 0x36
};

} // End of anonymous namespace

void MDScripts::openGatesAtOlympusMons() {
	const uint16 OBJ_GATE = 181;

	Obj *gate = mapGetObj(0x2c3, 0x1f3, 0, OBJ_GATE);
	if (gate) {
		setObjLocation(gate, 0x2c2, gate->y, gate->z);
		gate->frame_n = 3;
	} else {
		printl("AARGH");
	}

	gate = mapGetObj(0x2c4, 0x1f3, 0, OBJ_GATE);
	if (gate)
		gate->frame_n = 7;
	else
		printl("AARGH");
}

void MDScripts::openDreamMachineDoor() {
	const uint16 OBJ_DOOR = 152;

	Obj *door = mapGetObj(0x2c7, 0x1dc, 0, OBJ_DOOR);
	if (door)
		door->frame_n = 7;
}

void MDScripts::talkScriptFixPanels() {
	const uint16 OBJ_PANEL = 458;
	ActorManager *actorManager = Game::get_game()->get_actor_manager();
	int numPanels = 0;

	Common::Array<Actor *> members;
	getPartyMembers(members);

	for (uint i = 0; i < members.size(); ++i) {
		Common::Array<Obj *> inventory;
		getInventoryObjs(members[i], false, inventory);

		for (uint j = 0; j < inventory.size(); ++j) {
			if (inventory[j]->obj_n == OBJ_PANEL) {
				++numPanels;
				playMdSfx(4);
				inventory[j]->quality &= 0xfd;
				inventory[j]->qty = 4;
			}
		}
	}

	if (numPanels <= 1)
		actorManager->get_actor(0x39)->clear_flag(3);
	else
		actorManager->get_actor(0x39)->set_flag(3);
}

void MDScripts::talkScriptSpawnMonsterFootprints() {
	const uint16 OBJ_MONSTER_FOOTPRINTS = 145;
	MapCoord playerLoc = Game::get_game()->get_player()->get_actor()->get_location();

	_objlist1d22Unk = 0;

	for (int i = 0; i < 6; ++i) {
		Actor *monster;
		if (!Game::get_game()->get_actor_manager()->create_temp_actor(OBJ_MONSTER_FOOTPRINTS, NO_OBJ_STATUS, playerLoc.x,
				playerLoc.y, playerLoc.z, ACTOR_ALIGNMENT_NEUTRAL, ACTOR_WT_ASSAULT, &monster))
			continue;

		actorInit(monster, ACTOR_ALIGNMENT_DEFAULT);
		tossActor(monster, playerLoc.x, playerLoc.y, playerLoc.z, 0);
		monster->set_worktype(0x8);
		++_objlist1d22Unk;
	}
}

bool MDScripts::talkScript(uint8 scriptNumber) {
	switch (scriptNumber) {
	case 1:
		talkScriptFixPanels();
		break;
	case 6:
		openGatesAtOlympusMons();
		break;
	case 7:
		openDreamMachineDoor();
		break;
	case 9:
		_talkScriptStatus = TALK_STATUS_END_GAME;
		break;
	case 0xA:
		_talkScriptStatus = 0xA;
		break;
	case 0x35:
		talkScriptSpawnMonsterFootprints();
		break;
	default:
		print(Common::String::format("Attempting to run talk script #%d\n", scriptNumber));
		break;
	}

	return true;
}

void MDScripts::playEndSequence() {
	get_cutscene()->moveToFront();
	Script::get_script()->play_ending();
	Game::get_game()->quit();
}

bool MDScripts::talkToActor(Actor *actor) {
	Game *game = Game::get_game();
	uint8 actorNum = actor->get_actor_num();

	if (actorNum < 2) {
		if (_inDreamMode) {
			printl("YOU_TRY_TO_WAKE_YOURSELF_UP");

			if (game->get_player()->get_actor()->get_z() != 3)
				wakeFromDream();
		} else if (!game->get_player()->in_party_mode()) {
			printl("NOT_WHILE_IN_SOLO_MODE");
		} else {
			printl("YOU_ARENT_YET_THAT_INSANE");
		}

		return true;
	}

	if (actor->get_obj_n() == 391) { // Your mother
		printl(game->get_player()->get_gender() == 0 ? "NO_BACKTALK_FROM_YOU_YOUNG_MAN" : "NO_BACKTALK_FROM_YOU_YOUNG_WOMAN");
		return true;
	}

	print(Common::String(actor->get_name()) + "\n");
	actorTalk(actor);
	print("\n");

	int status = _talkScriptStatus;
	_talkScriptStatus = TALK_STATUS_NONE;

	switch (status) {
	case TALK_STATUS_END_GAME:
		playEndSequence();
		break;
	case TALK_STATUS_WAKE_FROM_DREAM_1:
	case TALK_STATUS_WAKE_FROM_DREAM_2:
		wakeFromDream();
		break;
	default:
		break;
	}

	return true;
}

void MDScripts::talkConveyor() {
	talkToActor(Game::get_game()->get_actor_manager()->get_actor(0x72));
}

void MDScripts::talkTower() {
	talkToActor(Game::get_game()->get_actor_manager()->get_actor(0x73));
}

void MDScripts::talkDreamMachine() {
	talkToActor(Game::get_game()->get_actor_manager()->get_actor(0x74));
}

bool MDScripts::talkToObj(Obj *obj) {
	Actor *player = Game::get_game()->get_player()->get_actor();

	if (!actorIsAffectedByBerries(player->get_actor_num(), BERRY_GREEN)) {
		printl("NOTHING");
		return false;
	}

	if ((obj->status & 1) != 1) {
		printl("YOU_RECIEVE_NO_PSYCHIC_IMPRESSIONS");
		return false;
	}

	switch (obj->obj_n) {
	case 0xC:
		printl("YOU_RECEIVE_A_MOMENTARY_IMPRESSION");
		break;
	case 0x5C:
		printl("THIS_WAS_WORN_LAST_BY_A_DYING_MAN");
		break;
	case 0x64:
	case 0x120:
	case 0x121:
	case 0x122:
		talkDreamMachine();
		break;
	case 0xBC:
	case 0xBF:
	case 0xC0:
		talkConveyor();
		break;
	case 0xC8:
	case 0xC9:
	case 0xD6:
	case 0xD7:
		talkTower();
		break;
	case 0xE7:
		printl("THIS_WAS_USED_BY_A_MARTIAN_MEASURING_TIME");
		break;
	case 0xEA:
		printl("THIS_JEWELRY_WAS_WORN_BY");
		break;
	case 0xF6:
		printl("THESE_FOOTBAGS_WERE_WORN_BY");
		break;
	case 0xF7:
		printl("THIS_SAW_WAS_USED_BY");
		break;
	case 0xF9:
		printl("THIS_DEVICE_WAS_USED_BY");
		break;
	case 0x113:
		printl("THIS_SCULPTURE_HAD_GREAT_RITUAL_OR_RELIGIOUS_SIGNIFICANCE");
		break;
	default:
		return false;
	}

	return true;
}

} // End of namespace MD
} // End of namespace Scripts
} // End of namespace Ultima6
} // End of namespace Ultima
