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
#include "ultima/ultima6/script/script_cutscene.h"

namespace Ultima {
namespace Ultima6 {
namespace Scripts {
namespace MD {

namespace {

bool hasHypoxia(const Actor *actor) {
	return actor->get_obj_flag(ACTOR_MD_OBJ_FLAG_HYPOXIA);
}

void setHypoxia(Actor *actor, bool value) {
	actor->set_obj_flag(ACTOR_MD_OBJ_FLAG_HYPOXIA, value);
}

bool hasFrenzy(const Actor *actor) {
	return actor->get_obj_flag(ACTOR_MD_OBJ_FLAG_FRENZY);
}

bool isCold(const Actor *actor) {
	return actor->get_status_flag(ACTOR_MD_STATUS_FLAG_COLD);
}

void setCold(Actor *actor, bool value) {
	actor->set_status_flag(ACTOR_MD_STATUS_FLAG_COLD, value);
}

Actor *getActor(uint8 actorNum) {
	return Game::get_game()->get_actor_manager()->get_actor(actorNum);
}

} // End of anonymous namespace

const MDScripts::ActorStats *MDScripts::getActorStats(uint16 objN) {
	for (uint i = 0; i < ACTOR_STATS_COUNT; ++i) {
		if (ACTOR_STATS[i]._objN == objN)
			return &ACTOR_STATS[i];
	}

	return nullptr;
}

bool MDScripts::isActorStatBitSet(uint16 objN, int bit) {
	const ActorStats *stats = getActorStats(objN);
	return stats && stats->_v[22 - bit] == 1;
}

int MDScripts::actorGetDamage(uint16 objN) {
	const ActorStats *stats = getActorStats(objN);
	return stats ? stats->_v[6] : -1;
}

bool MDScripts::actorIsAffectedByBerries(uint8 actorNum, int berryType) {
	return actorGetBerryCount(actorNum, berryType) > 0;
}

int MDScripts::actorGetBerryCount(uint8 actorNum, int berryType) {
	if (actorNum >= 16)
		return 0;

	return Game::get_game()->get_clock()->get_timer(actorNum * 3 + berryType);
}

void MDScripts::actorDecrementBerryCounter(Actor *actor, int berryType) {
	GameClock *clock = Game::get_game()->get_clock();
	uint8 actorNum = actor->get_actor_num();
	int count = clock->get_timer(actorNum * 3 + berryType);

	if (count > 0 && randomRange(1, actorIntAdjusted(actor) * 4) == 1) {
		clock->set_timer(actorNum * 3 + berryType, count - 1);

		if ((actorNum == 0 && _inDreamMode) || (actorNum != 0 && !_inDreamMode)) {
			playMdSfx(0x32);
			printl("A_PSYCHIC_POWER_FADES");
		}
	}
}

void MDScripts::actorDecrementBerryCounters(Actor *actor) {
	actorDecrementBerryCounter(actor, BERRY_PURPLE);
	actorDecrementBerryCounter(actor, BERRY_GREEN);
	actorDecrementBerryCounter(actor, BERRY_BROWN);
}

void MDScripts::actorIncrementBerryCounter(uint8 actorNum, int berryType) {
	GameClock *clock = Game::get_game()->get_clock();
	int count = clock->get_timer(actorNum * 3 + berryType);

	if (actorNum < 16 && count < 10)
		clock->set_timer(actorNum * 3 + berryType, count + 1);
}

void MDScripts::actorClearBerryCounters(uint8 actorNum) {
	GameClock *clock = Game::get_game()->get_clock();

	for (int berryType = BERRY_PURPLE; berryType <= BERRY_BROWN; ++berryType)
		clock->set_timer(actorNum * 3 + berryType, 0);
}

int MDScripts::getBlueBerryCounter() {
	return Game::get_game()->get_clock()->get_timer(16 * 3);
}

void MDScripts::setBlueBerryCounter(int value) {
	Game::get_game()->get_clock()->set_timer(16 * 3, value);
}

bool MDScripts::actorInit(Actor *actor, ActorAlignment alignment) {
	const ActorStats *stats = getActorStats(actor->get_obj_n());

	if (stats) {
		actor->set_strength(actorRandomiseStat(stats->_v[0]));
		actor->set_dexterity(actorRandomiseStat(stats->_v[1]));
		actor->set_intelligence(actorRandomiseStat(stats->_v[2]));
		actor->set_hp(actorRandomiseStat(stats->_v[3]));
		actor->set_level(0);
		actor->set_alignment((ActorAlignment)stats->_v[4]);
	} else {
		actor->set_strength(15);
		actor->set_dexterity(15);
		actor->set_intelligence(15);
		actor->set_hp(30);
		actor->set_level(1);
		actor->set_alignment(ACTOR_ALIGNMENT_CHAOTIC);
	}

	if (alignment != ACTOR_ALIGNMENT_DEFAULT)
		actor->set_alignment(alignment);

	actor->set_worktype(8);
	actor->set_combat_mode(8);
	actor->set_moves_left((sint8)actor->get_dexterity());
	actor->set_exp(0);

	return true;
}

void MDScripts::actorDivide(Actor *actor) {
	MapCoord loc = actor->get_location();
	ActorManager *actorManager = Game::get_game()->get_actor_manager();

	for (int i = 0; i < 8; ++i) {
		int newX = wrapCoord(randomRange(1, 2) + loc.x - 1, loc.z);
		int newY = wrapCoord(randomRange(1, 2) + loc.y - 1, loc.z);
		MapCoord newLoc(newX, newY, loc.z);

		if (actorManager->can_put_actor(newLoc)) {
			Actor *newActor;
			actorManager->clone_actor(actor, &newActor, newLoc);
			printfl("ACTOR_DIVIDES", actor->get_name());
			return;
		}
	}
}

void MDScripts::defenderUpdateAlignment(Actor *attacker, const Target &defender, int damage) {
	Actor *actor = defender._actor;
	if (!actor)
		return;

	if (actor->get_alignment() != ACTOR_ALIGNMENT_NEUTRAL || actor->is_in_party() ||
			(actor->get_worktype() > 0x10 && actor->get_worktype() < 0x80))
		return;

	if (attacker->get_worktype() == WT_PLAYER && isActorStatBitSet(actor->get_obj_n(), 14) && damage > 0)
		printfl("ACTOR_ATTACKS", actor->get_name());

	if (actor->get_actor_num() >= 0xc0 || actor->get_worktype() < 0x80) {
		if (attacker->get_alignment() == ACTOR_ALIGNMENT_GOOD)
			actor->set_alignment(ACTOR_ALIGNMENT_EVIL);

		if (actor->get_worktype() >= 0xc0)
			actor->set_worktype(8);
	} else if (attacker->get_worktype() == WT_PLAYER) {
		actor->set_worktype(0xa0);
	}
}

void MDScripts::actorHandleDamage(Actor *defender) {
	if (defender->get_obj_n() == 370 || defender->get_hp() == 0)
		return;

	if (defender->get_maxhp() == 0)
		return;

	int hpLevel = (defender->get_hp() * 4) / defender->get_maxhp();

	if (hpLevel == 0) {
		printfl("ACTOR_CRITICAL", defender->get_name());

		int wt = defender->get_worktype();
		if (wt == 0xd) {
			defender->set_worktype(0x11);
		} else if (wt != 6 && wt != 0xe && wt != 2 && wt != 1 && wt != 0 && wt != 0x11 && wt != 0x19 &&
				(isActorStatBitSet(defender->get_obj_n(), 7) || isActorStatBitSet(defender->get_obj_n(), 14))) {
			// OBJ_MONSTER_FOOTPRINTS never flee
			if (actorIntAdjusted(defender) >= 5 && defender->get_obj_n() != 145)
				defender->set_worktype(7);
		}
	} else if (hpLevel < 4) {
		const char *suffix = _lang.get(isActorStatBitSet(defender->get_obj_n(), 14) &&
			isActorStatBitSet(defender->get_obj_n(), 7) ? "DAMAGED" : "WOUNDED");
		const char *damageType = _lang.get(hpLevel == 1 ? "HEAVILY" : (hpLevel == 2 ? "LIGHTLY" : "BARELY"));

		printfl("ACTOR_HIT_MESSAGE", defender->get_name(), damageType, suffix);
	}
}

void MDScripts::actorTakeHit(Actor *attacker, const Target &defender, int maxDmg, int damageMode) {
	if (maxDmg == -1)
		maxDmg = 1;
	else if (maxDmg > 1 && maxDmg < 255)
		maxDmg = randomRange(1, maxDmg);

	int armourValue = 0;
	if (damageMode != DAMAGE_ACID) {
		if (defender.isActor())
			armourValue = actorGetAc(defender._actor);
		else if (defender.objN() == 388 && defender.frameN() == 0) // OBJ_GLOW_WORM
			armourValue = 2;
	}

	if (armourValue > 0 && maxDmg < 255)
		maxDmg -= randomRange(1, armourValue);

	if (maxDmg > 0) {
		Party *party = Game::get_game()->get_party();

		if ((attacker->get_alignment() == ACTOR_ALIGNMENT_EVIL || attacker->get_alignment() == ACTOR_ALIGNMENT_CHAOTIC) &&
				defender.isActor() && defender._actor->is_in_party() && !party->is_in_combat_mode() &&
				Game::get_game()->get_player()->in_party_mode())
			party->set_in_combat_mode(true);

		uint16 exp = attacker->get_exp();
		attacker->set_exp(exp + actorHitTarget(defender, maxDmg, damageMode));
	} else {
		printfl("ACTOR_GRAZED", defender.name().c_str());
	}

	if (!defender.isActor())
		return;

	Actor *defendingActor = defender._actor;
	defenderUpdateAlignment(attacker, defender, maxDmg);

	if (defendingActor->get_hp() != 0) {
		if (isActorStatBitSet(attacker->get_obj_n(), 2) && randomRange(0, 3) == 0 && !actorImmuneToDmg(defendingActor)) {
			printfl("ACTOR_PARALYZED", defendingActor->get_name());
			defendingActor->set_paralyzed(true);
		}

		if (defendingActor->is_alive() && maxDmg > 0)
			actorHandleDamage(defendingActor);
	}
}

bool MDScripts::actorImmuneToDmg(const Actor *actor) {
	uint16 objN = actor->get_obj_n();
	if (objN == 381 || objN == 390 || objN == 391 || objN == 358 || objN == 382)
		return true;

	switch (actor->get_actor_num()) {
	case 0x5c: case 0x5d: case 0x59: case 0x5a: case 0x54: case 0x52: case 0x6d:
	case 0x68: case 0x67: case 0x69: case 0x77: case 0x78: case 0x40: case 0x3c:
		return true;
	default:
		return false;
	}
}

void MDScripts::actorPrintCustomHitMessage(Actor *actor) {
	switch (actor->get_actor_num()) {
	case 0x69:
		printfl("ACTOR_CRITICAL", getActor(0x19)->get_name());
		break;
	case 0x3c:
		printfl("ACTOR_CRITICAL", actor->get_name());
		break;
	case 0x52:
		printfl("ACTOR_HIT_MESSAGE", actor->get_name(), _lang.get("HEAVILY"), _lang.get("WOUNDED"));
		break;
	case 0x68:
		if (actor->get_flag(2))
			printfl("ACTOR_CRITICAL", actor->get_name());
		else
			printfl("ACTOR_HIT_MESSAGE", actor->get_name(), _lang.get("HEAVILY"), _lang.get("WOUNDED"));

		actor->set_flag(5);
		actorTalk(actor);
		break;
	default:
		printl("IT_HAS_NO_EFFECT");
		break;
	}
}

void MDScripts::hitTarget(const Target &target, uint16 hitTile) {
	if (mapIsOnScreen(target.x(), target.y(), target.z())) {
		if (target.isActor() && target._actor->is_in_party())
			playMdSfx(1, true);
		else
			playMdSfx(2, true);

		hitAnim(target.x(), target.y());
	}

	if (target.isActor() && target._actor->is_sleeping())
		target._actor->set_asleep(false);
}

bool MDScripts::actorHit(Actor *actor, uint8 dmg, bool displayHitMsg) {
	actorHitTarget(Target(actor), dmg, DAMAGE_UNSPECIFIED);
	return true;
}

int MDScripts::actorHitTarget(const Target &defender, int maxDmg, int damageMode) {
	int expGained = 0;
	Game *game = Game::get_game();

	if (defender.z() != game->get_player()->get_actor()->get_z() || maxDmg < 0)
		return 0;

	if (!defender.isActor()) {
		actorHitObj(defender._obj, maxDmg, damageMode);
		return 0;
	}

	Actor *actor = defender._actor;
	uint16 defenderObjN = actor->get_obj_n();

	if (actor->get_actor_num() == 0 && actor->get_hp() <= maxDmg && _currentDreamStage == 0xe0)
		maxDmg = actor->get_hp() - 1;

	hitTarget(defender, damageMode == DAMAGE_COLD ? BLUE_HIT_TILE : RED_HIT_TILE);

	if (actorImmuneToDmg(actor)) {
		actorPrintCustomHitMessage(actor);
		actor->set_hp(0xff);

		if (defenderObjN == 391) { // OBJ_YOUR_MOTHER
			printfl("HOW_DARE_YOU_YOUNG_PERSON", _lang.get(game->get_player()->get_gender() == 0 ? "MAN" : "WOMAN"));
			for (int i = 0; i < 3; ++i)
				actorTakeHit(actor, Target(getActor(0)), 45, DAMAGE_NORMAL);
		} else if (actor->get_actor_num() == 0x6d || actor->get_actor_num() == 0x67) {
			getActor(0x6b)->set_flag(3);
			Actor *dreamActor = getActor(0);
			dreamActor->move(_prevPlayerX, _prevPlayerY, dreamActor->get_z());
		}

		return expGained;
	}

	if (damageMode == DAMAGE_HEAT && isActorStatBitSet(defenderObjN, 3)) {
		printl("IT_HAS_NO_EFFECT");
		return 0;
	}

	if (isActorStatBitSet(defenderObjN, 4))
		maxDmg *= 2;

	if (damageMode == DAMAGE_NORMAL || _currentDreamStage != 0xe0) {
		int hp = actor->get_hp();

		// FIXME kill the actor when its hit points run out
		if (hp > maxDmg) {
			actor->set_hp(hp - maxDmg);

			uint8 wt = actor->get_worktype();
			if (wt == 9 || wt == 11)
				actor->set_worktype(8);
			else if (wt == 10 || wt == 12)
				actor->set_worktype(0x1b);

			if (isActorStatBitSet(defenderObjN, 6))
				actorDivide(actor);
		}
	}

	return expGained;
}

void MDScripts::attackBucket(Obj *bucket, int damageMode) {
	if (damageMode == DAMAGE_HEAT && bucket->frame_n == 2) {
		printl("THE_ICE_THAWS");
		bucket->frame_n = 1;
	} else if (damageMode == DAMAGE_COLD && bucket->frame_n == 1) {
		printl("THE_WATER_FREEZES");
		bucket->frame_n = 2;
	}
}

void MDScripts::actorHitObj(Obj *obj, int dmg, int damageMode) {
	if (obj->qty == 0 || !isObjAttackable(obj)) {
		if (obj->obj_n == 160) // OBJ_BUCKET
			attackBucket(obj, damageMode);
		return;
	}

	if (damageMode != DAMAGE_NORMAL && damageMode != DAMAGE_HEAT && !isPlantObj(obj))
		return;

	hitTarget(Target(obj), damageMode == DAMAGE_COLD ? BLUE_HIT_TILE : RED_HIT_TILE);

	if (damageMode == DAMAGE_HEAT && isObjBurnable(obj))
		dmg = obj->qty;

	if (obj->qty > dmg) {
		if (obj->obj_n == 307) { // OBJ_DEVIL_POD
			printl("THE_POD_SPLITS_OPEN");

			Actor *podDevil;
			if (Game::get_game()->get_actor_manager()->create_temp_actor(384, NO_OBJ_STATUS, obj->x, obj->y, obj->z,
					ACTOR_ALIGNMENT_NEUTRAL, ACTOR_WT_ASSAULT, &podDevil)) {
				actorInit(podDevil, ACTOR_ALIGNMENT_DEFAULT);
				podDevil->move(obj->x, obj->y, obj->z);
			}
			removeObj(obj);
		} else {
			if (obj->obj_n == 388) { // OBJ_GLOW_WORM
				obj->frame_n = 0;
				obj->quality = randomRange(0xb, 0x12);
				playMdSfx(0x22);
			}
			obj->qty -= dmg;
		}
		return;
	}

	if (obj->obj_n == 217) { // OBJ_GLASS_PITCHER
		playMdSfx(0x1a);
		obj->obj_n = 218; // OBJ_BROKEN_CRYSTAL
		obj->frame_n = 0;

		if (_inDreamMode && _currentDreamStage == 0x44) {
			bool pitcherLeft = false;
			Common::Array<Obj *> objs;
			getObjsInArea(0x21, 0x33, 2, 0x1c, 0x10, objs);

			for (uint i = 0; i < objs.size(); ++i)
				pitcherLeft |= objs[i]->obj_n == 217;

			if (!pitcherLeft)
				getActor(0x54)->set_flag(6);
		}
	} else {
		playMdSfx(0);
		removeObj(obj);
	}
}

bool MDScripts::actorMove(Actor *actor, NuvieDir direction, int flag) {
	MapCoord loc = actor->get_location();
	int x = loc.x;
	int y = loc.y;

	if (direction == NUVIE_DIR_N)
		--y;
	if (direction == NUVIE_DIR_S)
		++y;
	if (direction == NUVIE_DIR_E)
		++x;
	if (direction == NUVIE_DIR_W)
		--x;

	if (actor->get_obj_n() != 382) // COKER FIXME need to have an update frame function here
		actor->set_direction(direction);

	bool didMove = actor->move(wrap_signed_coord(x, loc.z), wrap_signed_coord(y, loc.z), loc.z);

	// FIXME need more logic here, footprints, bots etc.
	if (didMove)
		subtractMapMovementPts(actor);

	return didMove;
}

int MDScripts::actorMoveDiagonal(Actor *actor, NuvieDir xDirection, NuvieDir yDirection) {
	MapCoord loc = actor->get_location();
	int x = loc.x;
	int y = loc.y;

	if (yDirection == NUVIE_DIR_N)
		--y;
	if (yDirection == NUVIE_DIR_S)
		++y;
	if (xDirection == NUVIE_DIR_E)
		++x;
	if (xDirection == NUVIE_DIR_W)
		--x;

	actor->set_direction(yDirection);
	bool didMove = actor->move(wrap_signed_coord(x, loc.z), wrap_signed_coord(y, loc.z), loc.z);

	if (didMove)
		subtractMapMovementPts(actor);

	return didMove ? 1 : 0;
}

void MDScripts::actorUpdateFrame(Actor *actor, NuvieDir direction) {
	uint16 objN = actor->get_obj_n();

	// Only human actors sit down
	if (objN < 342 || objN > 358)
		return;

	MapCoord loc = actor->get_location();
	Common::Array<Obj *> objs;
	getObjsAt(loc.x, loc.y, loc.z, objs);

	for (uint i = 0; i < objs.size(); ++i) {
		switch (objs[i]->obj_n) {
		case 406: // OBJ_BENCH
			actor->set_frame_n(3);
			return;
		case 407: // OBJ_COUCH
			actor->set_frame_n(4 + 3);
			return;
		case 289: // OBJ_DREAM_MACHINE2
			actor->set_frame_n(8 + 3);
			return;
		case 216:
			actor->set_frame_n((direction / 2) * 4 + 3);
			return;
		default:
			break;
		}
	}
}

void MDScripts::reviveAvatar() {
	// FIXME dying in the dream world should respawn into the dream world
	if (!_inDreamMode)
		avatarFallsUnconscious();
}

bool MDScripts::actorResurrect(Actor *actor) {
	return true;
}

void MDScripts::avatarFallsUnconscious() {
	Game *game = Game::get_game();

	printl("OVERCOME_BY_YOUR_WOUNDS_YOU_FALL_UNCONSCIOUS");
	fadeOutEffect();

	while (get_cutscene()->input_poll() == INPUT_NONE) {
	}

	const char *location;
	MapCoord target;
	if (getActor(0x46)->get_flag(3)) {
		location = _lang.get("THE_OLYMPUS_MINE");
		target = MapCoord(0xa4, 0xc3, 4);
	} else {
		location = _lang.get("THE_SPACE_CAPSULE");
		target = MapCoord(0x19d, 0x278, 0);
	}

	printfl("YOU_AWAKEN_BACK_AT_FEELING_RESTORED", location);
	inputSelect(nullptr, true);

	Party *party = game->get_party();
	party->resurrect_dead_members();

	Common::Array<Actor *> members;
	getPartyMembers(members);
	for (uint i = 0; i < members.size(); ++i) {
		members[i]->set_hp(members[i]->get_maxhp());
		members[i]->set_moves_left((sint8)actorDexAdjusted(members[i]));
	}

	party->move(target.x, target.y, target.z);

	GameClock *clock = game->get_clock();
	int hour = clock->get_hour();
	int minutes = 60 - clock->get_minute();
	if (hour < 7)
		hour = 6 - hour;
	else
		hour = 24 - (hour + 1) + 7;

	// Advances time to between 7am and 8am on the next day
	clock->inc_minute(hour * 60 + minutes + randomRange(0, 59));
	updateWatchTile();
	advanceTime(0);

	Actor *leader = party->get_leader_actor();
	if (leader)
		game->get_player()->update_player(leader);
	party->set_in_combat_mode(false);
	game->get_player()->set_party_mode(party->get_actor(0));

	Actor *blood = getActor(0x12);
	if (blood->is_alive()) {
		blood->set_flag(5);
		actorTalk(blood);
	}

	fadeInEffect();
}

void MDScripts::partyUpdate() {
	Actor *avatar = getActor(1);
	Actor *dreamActor = getActor(0);
	Game *game = Game::get_game();

	if (avatar->get_hp() != 0 && !(_inDreamMode && !dreamActor->is_alive()))
		return;

	if (!_inDreamMode) {
		reviveAvatar();
		return;
	}

	_objlist1d22Unk = 0;
	if (_prevPlayerX == 0) {
		printl("YOU_SHAKE_YOURSELF_AWAKE_FROM_THE_NIGHTMARE");
		wakeFromDream();
		return;
	}

	dreamActor->set_hp(avatar->get_maxhp());
	dreamActor->set_poisoned(false);
	dreamActor->move(_prevPlayerX, _prevPlayerY, dreamActor->get_z());
	game->get_party()->set_in_combat_mode(false);
	printl("YOU_FEEL_YOUR_DREAM_CONSCIOUSNESS_RETURNING");

	if (_currentDreamStage != 0xc0)
		return;

	Actor *raxachk = getActor(0x66);
	if (!raxachk->get_flag(2) && raxachk->get_flag(3) && raxachk->get_flag(4) && !raxachk->get_flag(5)) {
		for (int i = 0; i < 256; ++i) {
			Actor *maw = getActor(i);
			if (maw->get_obj_n() == 373 && maw->get_worktype() == 0x12 && maw->get_x() == 0x87 && maw->get_y() == 0x17)
				maw->die(false);
		}

		const uint16 OBJ_BRIDGE = 224;
		if (!mapGetObj(0x7f, 0x18, dreamActor->get_z(), OBJ_BRIDGE)) {
			Obj *bridge = newObj(OBJ_BRIDGE);
			bridge->frame_n = 3;
			if (!game->get_obj_manager()->moveto_map(bridge, MapCoord(0x7f, 0x18, dreamActor->get_z())))
				delete_obj(bridge);
		}
	}

	raxachk->set_flag(7);
	actorTalk(raxachk);
}

bool MDScripts::actorUpdateAll() {
	Game *game = Game::get_game();
	Player *player = game->get_player();
	Actor *selectedActor;

	partyUpdate();

	do {
		selectedActor = nullptr;
		int di = 0;
		int dex6 = 1;

		do {
			MapCoord playerLoc = player->get_actor()->get_location();
			int var_C = (playerLoc.x - 16) - floorMod(playerLoc.x - 16, 8);
			int var_A = (playerLoc.y - 16) - floorMod(playerLoc.y - 16, 8);

			Common::Array<Actor *> members;
			getPartyMembers(members);
			for (uint i = 0; i < members.size(); ++i) {
				if (members[i]->get_worktype() == WT_FOLLOW && members[i]->get_moves_left() < 0)
					members[i]->set_moves_left(0);
			}

			for (int i = 0; i < 256; ++i) {
				Actor *actor = getActor(i);

				// FIXME need to check 11000b not zero on obj_flags
				if (actor->get_obj_n() == 0 || actor->get_z() != playerLoc.z || actor->get_moves_left() <= 0 ||
						actor->is_paralyzed() || (actor->is_sleeping() && actor->get_worktype() != 0x80) ||
						actor->get_worktype() == WT_NOTHING || !actor->is_alive())
					continue;

				uint8 wt = actor->get_worktype();

				// FIXME need to check map wrapping here
				if (ABS((int)actor->get_x() - var_C) > 0x27 || ABS((int)actor->get_y() - var_A) > 0x27) {
					if (wt >= 0x83 && wt <= 0x86) {
						// Move the actor to its schedule location if that isn't on screen
						MapCoord schedLoc;
						if (actor->get_schedule_location(&schedLoc) && !mapIsOnScreen(schedLoc.x, schedLoc.y, schedLoc.z)) {
							actor->move(schedLoc.x, schedLoc.y, schedLoc.z);
							// FIXME the worktype and pathfinder aren't reset for the new location
							actorSubtractMovementPoints(actor, 10);
						}
					}
				} else if (wt != WT_FOLLOW) {
					if (wt == 0x80)
						actor->set_worktype(actor->get_sched_worktype());

					int dexAdjusted = actorDexAdjusted(actor);
					int mpts = actor->get_moves_left();
					int dx = (mpts * dex6) - dexAdjusted * di;

					if (mpts >= dexAdjusted || dx > 0 || (dx == 0 && dexAdjusted > dex6)) {
						selectedActor = actor;
						di = mpts;
						dex6 = dexAdjusted;
					}

					if (dexAdjusted <= mpts)
						break;
				}
			}

			if (di <= 0) {
				for (int i = 0; i < 256; ++i) {
					Actor *actor = getActor(i);
					int dexAdjusted = actorDexAdjusted(actor);

					if (actor->get_moves_left() >= 0)
						actor->set_moves_left((sint8)dexAdjusted);
					else
						actor->set_moves_left((sint8)(actor->get_moves_left() + dexAdjusted));
				}

				advanceTime(1);
			}
		} while (di <= 0);

		if (selectedActor->get_worktype() != WT_PLAYER && selectedActor->get_worktype() != WT_FOLLOW) {
			performWorktype(selectedActor);
			partyUpdate();
		}
	} while (selectedActor->get_obj_n() == 0 || selectedActor->get_worktype() != WT_PLAYER);

	// Swap the player to the next party member with the 'command' combat worktype
	Actor *oldPlayer = player->get_actor();
	if (selectedActor != oldPlayer)
		player->update_player(selectedActor);
	// The player's worktype is changed to follow by update_player()
	oldPlayer->set_worktype(WT_PLAYER);

	displayPrompt(true);
	return true;
}

int MDScripts::actorGetAc(Actor *actor) {
	int ac = 0;
	Common::Array<Obj *> inventory;
	getInventoryObjs(actor, false, inventory);

	for (uint i = 0; i < inventory.size(); ++i) {
		if (inventory[i]->is_readied())
			ac += findObjValue(ARMOUR, ARMOUR_COUNT, inventory[i]->obj_n, 0);
	}

	return ac;
}

int MDScripts::clothingGetWarmthRating(const Obj *obj) {
	return findObjValue(CLOTHING_WARMTH, CLOTHING_WARMTH_COUNT, obj->obj_n, 0);
}

bool MDScripts::isLitLightsource(const Obj *obj) {
	switch (obj->obj_n) {
	case 110: case 112: case 114: case 116: case 118:
		return true;
	default:
		return false;
	}
}

int MDScripts::actorStrAdjusted(Actor *actor) {
	int str = actor->get_strength();

	if (hasHypoxia(actor))
		str -= 3;

	if (actorIsAffectedByBerries(actor->get_actor_num(), BERRY_PURPLE))
		str -= 3;

	if (hasFrenzy(actor))
		str = MIN(str + 3, 30);

	return (str <= 3) ? 1 : str;
}

int MDScripts::actorDexAdjusted(Actor *actor) {
	int dex = actor->get_dexterity();

	if (hasHypoxia(actor))
		dex = (dex <= 3) ? 1 : dex - 3;

	if (hasFrenzy(actor))
		dex = MIN(dex + 3, 30);

	if (actor->is_sleeping())
		dex = 1;

	return dex;
}

int MDScripts::actorIntAdjusted(Actor *actor) {
	int intelligence = actor->get_intelligence();

	if (hasHypoxia(actor) || (hasFrenzy(actor) && actor->get_actor_num() != 1))
		intelligence -= 3;

	return MAX(intelligence, 1);
}

uint8 MDScripts::actorStrAdj(Actor *actor) {
	return actorStrAdjusted(actor);
}

uint8 MDScripts::actorDexAdj(Actor *actor) {
	return actorDexAdjusted(actor);
}

uint8 MDScripts::actorIntAdj(Actor *actor) {
	return actorIntAdjusted(actor);
}

bool MDScripts::actorMapDmg(Actor *actor, MapCoord location) {
	uint16 objN = actor->get_obj_n();
	MapCoord playerLoc = Game::get_game()->get_player()->get_actor()->get_location();

	if (isActorStatBitSet(objN, 8) || isActorStatBitSet(objN, 10) || actor->get_z() != playerLoc.z ||
			actorFindMaxWrappedXYDistance(actor, playerLoc.x, playerLoc.y) > 40)
		return true;

	if (!actor->is_alive() || actor->is_hit())
		return true;

	Common::Array<Obj *> objs;
	getObjsAt(actor->get_x(), actor->get_y(), actor->get_z(), objs);

	for (uint i = 0; i < objs.size(); ++i) {
		uint16 tileNum = objOriginalTileNum(objs[i]);

		// Damaging objects that can't be walked through
		if (!tileGetFlag(tileNum, 3, 2) && tileGetFlag(tileNum, 1, 3))
			actorTakeDamageFromObj(actor, objs[i]);
	}

	actorUpdateFrame(actor, actor->get_direction());
	return true;
}

void MDScripts::actorTakeDamageFromObj(Actor *actor, Obj *obj) {
	if (actorImmuneToDmg(actor) || !actor->is_alive())
		return;

	int damage = 0;
	int damageMode = DAMAGE_NORMAL;

	switch (obj->obj_n) {
	case 168: // OBJ_PORCUPOD
		damage = randomRange(1, 0x14);
		break;
	case 209: // OBJ_STEAM_LEAK
		damageMode = DAMAGE_HEAT;
		if (actor->get_actor_num() != 6) {
			int low = actor->get_hp() / 2;
			int high = actor->get_maxhp() / 2;
			damage = (low <= high) ? randomRange(low, high) : low;
		}
		break;
	case 215: // OBJ_POWER_CABLE1
		playMdSfx(0x15);
		damage = randomRange(1, 0x14) + randomRange(1, 0x14);
		break;
	case 381: // OBJ_DUST_DEVIL
		damage = randomRange(1, 0xa) + randomRange(1, 0xa);
		break;
	default:
		break;
	}

	actorHitTarget(Target(actor), damage, damageMode);
}

void MDScripts::actorRemoveCharm(Actor *actor) {
	Game *game = Game::get_game();

	actor->set_charmed(false);
	actor->set_alignment((ActorAlignment)actor->get_old_alignment());

	if (actor->is_in_party())
		actor->set_alignment(ACTOR_ALIGNMENT_GOOD);

	if (game->get_party()->is_in_combat_mode())
		actor->set_worktype(actor->get_combat_mode());
	else if (!game->get_player()->in_party_mode())
		actor->set_worktype(WT_NOTHING);
	else
		actor->set_worktype(WT_FOLLOW);

	Actor *leader = game->get_party()->get_leader_actor();
	if (leader)
		game->get_player()->update_player(leader);
}

bool MDScripts::advanceTime(uint16 numTurns) {
	Game *game = Game::get_game();
	GameClock *clock = game->get_clock();
	int hour = clock->get_hour();

	// Rasputin's earthquakes
	if (getActor(0x46)->get_flag(3) && randomRange(0, 4) == 0)
		quakeStart(1, 200);

	for (int actorNum = 0; actorNum < 256; ++actorNum) {
		Actor *actor = getActor(actorNum);
		if (!actor->is_alive())
			continue;

		if (!_inDreamMode && actor->is_in_party() && actorNum != 0 && numTurns > 0 && !actor->is_sleeping())
			advanceTimeForPartyMember(actor, numTurns, hour);

		for (int turn = 0; turn < numTurns; ++turn)
			advanceTimeTurn(actor, actorNum);

		actorMapDmg(actor, actor->get_location());
		actor->set_hit_flag(false);
	}

	int minute = clock->get_minute();
	clock->inc_minute(numTurns);

	if (minute + numTurns < 60)
		return true;

	updateWatchTile();
	game->get_actor_manager()->updateSchedules(false);

	if (_hoursTillNextHealing > 0)
		--_hoursTillNextHealing;

	updateLampPosts();

	int blueBerryCounter = getBlueBerryCounter();
	if (blueBerryCounter > 0)
		setBlueBerryCounter(blueBerryCounter - 1);

	if (_partyIsWarm || _inDreamMode || !getActor(0x10)->get_flag(5))
		return true;

	// Oxium is used up as the party breathes
	const uint16 OBJ_BLOB_OF_OXIUM = 131;
	Common::Array<Actor *> members;
	getPartyMembers(members);

	for (uint i = 0; i < members.size(); ++i) {
		Actor *actor = members[i];
		if (actor->get_actor_num() == 6 || actor->is_sleeping())
			continue;

		Obj *oxium = actor->inventory_get_object(OBJ_BLOB_OF_OXIUM, 0, false, 0, false);
		if (oxium && oxium->qty > 1) {
			oxium->qty -= 1;
			if (hasHypoxia(actor)) {
				setHypoxia(actor, false);
				printfl("BREATHES_EASIER", actor->get_name());
			}
		} else {
			if (oxium)
				removeObj(oxium);

			if (actor->inventory_get_object(OBJ_BLOB_OF_OXIUM, 0, false, 0, false)) {
				if (hasHypoxia(actor)) {
					setHypoxia(actor, false);
					printfl("BREATHES_EASIER", actor->get_name());
				}
			} else if (!hasHypoxia(actor)) {
				setHypoxia(actor, true);
				printfl("GASPS_FOR_AIR", actor->get_name());
			}
		}
	}

	return true;
}

void MDScripts::advanceTimeForPartyMember(Actor *actor, int numTurns, int hour) {
	const uint16 OBJ_BLOB_OF_OXIUM = 131;
	uint8 actorNum = actor->get_actor_num();

	if (actorNum == 6 || !getActor(0x10)->get_flag(5) || actor->inventory_has_object(OBJ_BLOB_OF_OXIUM, 0, false)) {
		if (hasHypoxia(actor)) {
			setHypoxia(actor, false);
			printfl("BREATHES_EASIER", actor->get_name());
		}
	} else if (!hasHypoxia(actor)) {
		setHypoxia(actor, true);
		printfl("GASPS_FOR_AIR", actor->get_name());
	}

	int warmthRating = 0;
	Common::Array<Obj *> inventory;
	getInventoryObjs(actor, false, inventory);

	for (uint i = 0; i < inventory.size(); ++i) {
		Obj *obj = inventory[i];
		if (!obj->is_readied())
			continue;

		warmthRating += clothingGetWarmthRating(obj);

		if (isLitLightsource(obj) && randomRange(0, 1) == 1) {
			if (obj->quality <= numTurns) {
				if (obj->obj_n == 116 || obj->obj_n == 118) { // OBJ_LIT_OIL_LAMP, OBJ_LIT_LANTERN
					obj->obj_n = (obj->obj_n == 116) ? 115 : 117;
					obj->quality = 0;
					printfl("THE_IS_OUT_OF_FUEL", Target(obj).name().c_str());
				} else {
					printfl("WENT_OUT", Game::get_game()->get_obj_manager()->look_obj(obj, true));
					removeObj(obj);
				}
			} else {
				obj->quality -= numTurns;
			}
		}
	}

	if (_partyIsWarm || actor->get_z() != 0) {
		if (isCold(actor)) {
			setCold(actor, false);
			printfl("FEELS_WARMER", actor->get_name());
		}
		return;
	}

	int coldStatus = 0;
	if (hour <= 3 || hour >= 22)
		coldStatus = (warmthRating >= 10) ? 1 : 2;
	else if ((hour <= 6 || hour >= 18) && warmthRating < 10)
		coldStatus = 1;

	if (actorNum == 6)
		coldStatus = 0;

	if (coldStatus == 0) {
		if (isCold(actor)) {
			setCold(actor, false);
			printfl("FEELS_WARMER", actor->get_name());
		}
		return;
	}

	if (!isCold(actor)) {
		setCold(actor, true);
		printfl("IS_FREEZING", actor->get_name());
	}

	for (int i = 0; i < numTurns; ++i) {
		if (randomRange(0, 1) == 0) {
			if (coldStatus == 2) {
				printfl("IS_FREEZING", actor->get_name());
				actorHitTarget(Target(actor), randomRange(1, 2), DAMAGE_UNSPECIFIED);
			}
		} else {
			printfl("IS_FREEZING", actor->get_name());
			actorHitTarget(Target(actor), randomRange(1, 2), DAMAGE_UNSPECIFIED);
			if (coldStatus == 2)
				actorHitTarget(Target(actor), randomRange(1, 2), DAMAGE_UNSPECIFIED);
		}
	}
}

void MDScripts::advanceTimeTurn(Actor *actor, int actorNum) {
	Party *party = Game::get_game()->get_party();

	// Battle frenzy ends when the party leaves combat mode
	if (hasFrenzy(actor) && !party->is_in_combat_mode())
		actor->set_obj_flag(ACTOR_MD_OBJ_FLAG_FRENZY, false);

	if (actor->is_poisoned() && randomRange(0, 25) == 0)
		actor->set_poisoned(false);

	if (actor->is_charmed() && randomRange(0, 0x19) == 0)
		actorRemoveCharm(actor);

	if (actor->is_paralyzed()) {
		// FIXME should use the adjusted strength
		if (actorNum == 6 || (randomRange(0, 3) == 0 && actor->get_strength() >= randomRange(1, 0x1e)))
			actor->set_paralyzed(false);
	}

	if (actor->is_sleeping() && actor->get_worktype() != WT_SLEEP && (!_partyIsWarm || !actor->is_in_party())) {
		// FIXME check sub_2B0EC(actor.x,actor.y,actor.z)
		if (randomRange(0, 0x14) == 0)
			actor->set_asleep(false);
	}

	if (actor->is_poisoned() && actorNum != 6 && randomRange(0, 7) == 0)
		actorHitTarget(Target(actor), 1, DAMAGE_UNSPECIFIED);

	if (actorNum >= 8)
		return;

	actorDecrementBerryCounters(actor);

	Common::Array<Obj *> inventory;
	getInventoryObjs(actor, true, inventory);

	for (uint i = 0; i < inventory.size(); ++i) {
		Obj *obj = inventory[i];

		if (obj->obj_n == 160 && obj->frame_n > 1) { // OBJ_EMPTY_BUCKET with ice
			if (randomRange(0, 100) < 10) {
				printl("SOME_ICE_HAS_MELTED");
				obj->frame_n = 1;
			}
		} else if (obj->obj_n == 256) { // OBJ_CHUNK_OF_ICE
			if (randomRange(0, 100) < 10) {
				printl("SOME_ICE_HAS_MELTED");
				removeObj(obj);
			}
		} else if (obj->obj_n == 448 || (obj->obj_n == 449 && actorNum != 6 && !actor->is_poisoned())) {
			// OBJ_BLOCK_OF_RADIUM, OBJ_CHIP_OF_RADIUM unless in a lead box
			Obj *container = obj->get_container_obj();
			if (!obj->is_in_container() || !container || container->obj_n != 139) {
				printfl("IS_POISONED", actor->get_name());
				actor->set_poisoned(true);
			}
		}
	}
}

bool MDScripts::actorSubtractMovementPoints(Actor *actor, uint8 points) {
	int pts = points;

	if (actor->get_actor_num() < 16 && !Game::get_game()->get_party()->is_in_combat_mode())
		pts -= 2;

	if (pts < 1)
		pts = 1;

	actor->set_moves_left((sint8)(actor->get_moves_left() - pts));
	return true;
}

void MDScripts::actorRadiationCheck(Actor *actor, Obj *obj) {
	if (obj->obj_n != 448 && obj->obj_n != 449) // OBJ_BLOCK_OF_RADIUM, OBJ_CHIP_OF_RADIUM
		return;

	const uint16 OBJ_TONGS = 136;
	uint8 actorNum = actor->get_actor_num();

	// Purple berries protect against radiation
	if (actorNum == 6 || actorIsAffectedByBerries(actorNum, BERRY_PURPLE) ||
			actor->inventory_get_readied_obj_n(ACTOR_ARM) == OBJ_TONGS ||
			actor->inventory_get_readied_obj_n(ACTOR_ARM_2) == OBJ_TONGS)
		return;

	actor->set_poisoned(true);
	printl("OUCH_IT_IS_VERY_HOT");
}

bool MDScripts::actorGetObj(Actor *actor, Obj *obj, Obj *container) {
	Game *game = Game::get_game();

	// FIXME handle getting into a container
	if (!game->get_obj_manager()->can_get_obj(obj)) {
		print(Common::String("\n") + _lang.get("THAT_IS_NOT_POSSIBLE"));
		return false;
	}

	if (actorFindMaxWrappedXYDistance(actor, obj->x, obj->y) > 1) {
		playMdSfx(0x32);
		projectileAnim(objTileNum(obj), obj->x, obj->y, actor->get_x(), actor->get_y(), 4, false, 0);
	}

	if (!game->using_hackmove() && !actor->can_carry_weight(obj)) {
		printl("THE_TOTAL_IS_TOO_HEAVY");
		return false;
	}

	// FIXME need to limit inventory slots
	if (!actor->can_carry_object(obj)) {
		print(Common::String("\n") + _lang.get("YOU_ARE_CARRYING_TOO_MUCH_ALREADY"));
		return false;
	}

	actorSubtractMovementPoints(actor, 3);
	actorRadiationCheck(actor, obj);

	if (obj->obj_n == 256) // OBJ_CHUNK_OF_ICE
		print(Common::String("\n") + _lang.get("THE_ICE_IS_MELTING"));

	switch (obj->obj_n) {
	case 110: case 112: case 114: case 116: case 118: // Lit light sources
		if (!actorHasFreeArm(actor)) {
			printl("YOUR_HANDS_ARE_FULL");
			return false;
		}

		game->get_obj_manager()->moveto_inventory(obj, actor);
		actor->add_readied_object(obj);
		if (obj->obj_n == 110) // OBJ_LIT_TORCH
			obj->quality = 0xb4;

		advanceTime(0);
		return true;

	case 411: case 458: case 314: case 206: // OBJ_SWITCH_BAR, OBJ_PANEL, OBJ_TRACKING_MOTOR, OBJ_TIFFANY_LENS
		if (obj->quality % 2 == 0) {
			print(Common::String("\n") + _lang.get("WONT_BUDGE"));
			return false;
		}
		break;

	default:
		break;
	}

	game->get_obj_manager()->moveto_inventory(obj, actor);
	return true;
}

uint8 MDScripts::getPortraitNumber(Actor *actor) {
	const uint8 NO_PORTRAIT = 255;
	int idx = actor->get_actor_num();
	uint8 gender = Game::get_game()->get_player()->get_gender();

	if (idx >= 0xa && idx <= 0xf)
		idx = 0x51;

	if (idx >= 0xbf)
		return NO_PORTRAIT;

	if (idx <= 1)
		idx = (gender == 1) ? 0 : 1;
	else if (idx == 6 && actor->get_flag(1))
		idx = 0x16;
	else if (actor->get_obj_flag(0))
		idx = (gender == 1) ? 0x73 : 0x72;
	else if (idx == 0x21 && !actor->get_flag(2))
		idx = 0x61;
	else if (idx == 0x15 || (idx >= 0x47 && idx <= 0x4a) || idx == 0x4e)
		idx = 0x14;
	else if (idx >= 0x4b && idx <= 0x4d)
		idx = 0x16;
	else if (idx == 0x1e)
		idx = 0x18;
	else if (idx == 0x46)
		idx = 0x40;
	else if (idx >= 0x50 && idx <= 0x57)
		idx -= 0x26;
	else if (idx == 0x5e)
		idx = 0x6;
	else if (idx == 0x68 || idx == 0x77 || idx == 0x78)
		idx = 0x67;
	else if (idx == 0x69)
		idx = 0x19;
	else if (idx == 0x6d)
		idx = 0x2;

	return idx;
}

Target MDScripts::findRockwormActor(const Target &target) {
	if (target.isActor() || target.isNull())
		return target;

	if (target._obj->obj_n == 371 || target._obj->obj_n == 370) { // OBJ_ROCKWORM1, OBJ_ROCKWORM_BASE
		Actor *actor = getActor(target._obj->quality);
		if (actor->is_alive())
			return Target(actor);
	}

	return target;
}

void MDScripts::removeObj(Obj *obj) {
	if (_selectedTarget._obj == obj)
		_selectedTarget = Target();
	if (_attackTarget._obj == obj)
		_attackTarget = Target();

	for (uint i = 0; i < _playerReadiedWeapons.size(); ++i) {
		if (_playerReadiedWeapons[i] == obj) {
			_playerReadiedWeapons.remove_at(i);
			break;
		}
	}

	removeFromEngine(obj);
}

} // End of namespace MD
} // End of namespace Scripts
} // End of namespace Ultima6
} // End of namespace Ultima
