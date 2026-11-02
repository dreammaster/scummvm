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
#include "ultima/ultima6/core/effect.h"
#include "ultima/ultima6/core/game.h"
#include "ultima/ultima6/core/game_clock.h"
#include "ultima/ultima6/core/obj_manager.h"
#include "ultima/ultima6/core/party.h"
#include "ultima/ultima6/core/player.h"
#include "ultima/ultima6/core/u6_objects.h"
#include "ultima/ultima6/script/common/script_random.h"
#include "ultima/ultima6/script/u6/u6_scripts.h"

namespace Ultima {
namespace Ultima6 {
namespace Scripts {
namespace U6 {

namespace {

enum {
	OBJ_BALLOON = 0x1a7,
	OBJ_SKIFF = 0x19e,
	OBJ_RAFT = 0x19f,
	OBJ_SHIP = 0x19c,
	OBJ_HORSE_WITH_RIDER = 431,
	OBJ_HORSE = 430,
	OBJ_SLIME = 0x177,
	OBJ_CORPSER = 0x165,
	OBJ_LORD_BRITISH = 409,
	OBJ_TANGLE_VINE_POD = 0x16d,
	OBJ_TANGLE_VINE = 0x16e
};

const uint16 MONSTER_INV_FOOD[3] = { 129, 133, 128 };

const uint16 ACID_SLUG_ITEMS[] = {
	0x2, 0x3, 0x4, 0x5, 0x7, 0x0A, 0x0B, 0x0C, 0x0D, 0x0F, 0x13, 0x14, 0x15, 0x16, 0x23, 0x26, 0x2B, 0x71, 0x72
};

const uint16 GREMLIN_ITEMS[] = {
	0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x87, 0xD1, 0xD2, 0xD3
};

const uint16 TANGLE_VINE_FRAMES[16] = {
	1, 3, 1, 4,
	5, 0, 4, 0,
	1, 2, 1, 5,
	2, 0, 3, 0
};

bool godMode() {
	return Game::get_game()->is_god_mode_enabled();
}

} // End of anonymous namespace

const U6Scripts::ActorStats *U6Scripts::getActorStats(uint16 objN) {
	for (uint i = 0; i < ACTOR_STATS_COUNT; ++i) {
		if (ACTOR_STATS[i]._objN == objN)
			return &ACTOR_STATS[i];
	}

	return nullptr;
}

bool U6Scripts::statFlag(uint16 objN, int flag) {
	const ActorStats *stats = getActorStats(objN);
	return stats && stats->_flags[flag - FLAG_CAN_TALK] == 1;
}

const U6Scripts::ProjectileInfo *U6Scripts::getProjectileInfo(uint16 objN) {
	for (uint i = 0; i < PROJECTILES_COUNT; ++i) {
		if (PROJECTILES[i]._objN == objN)
			return &PROJECTILES[i];
	}

	return nullptr;
}

const GameScripts::ObjValue *U6Scripts::weaponRanges(uint &count) const {
	count = WEAPON_RANGES_COUNT;
	return WEAPON_RANGES;
}

Actor *U6Scripts::getActor(uint8 actorNum) {
	return Game::get_game()->get_actor_manager()->get_actor(actorNum);
}

Actor *U6Scripts::newTempActor(uint16 objN, int x, int y, int z, ActorAlignment alignment, uint8 worktype) {
	Actor *actor = nullptr;
	if (!Game::get_game()->get_actor_manager()->create_temp_actor(objN, NO_OBJ_STATUS, x, y, z, alignment, worktype, &actor))
		return nullptr;

	return actor;
}

int U6Scripts::dirX(NuvieDir direction) {
	static const int OFFSETS[9] = { 0, 1, 0, -1, 1, 1, -1, -1, 0 };
	return OFFSETS[direction];
}

int U6Scripts::dirY(NuvieDir direction) {
	static const int OFFSETS[9] = { -1, 0, 1, 0, -1, 1, 1, -1, 0 };
	return OFFSETS[direction];
}

int U6Scripts::actorFindMaxXYDistance(const Actor *actor, int x, int y) {
	return MAX(ABS((int)actor->get_x() - x), ABS((int)actor->get_y() - y));
}

bool U6Scripts::alignmentIsEvil(int align) {
	return align == ACTOR_ALIGNMENT_EVIL || align == ACTOR_ALIGNMENT_CHAOTIC;
}

int U6Scripts::getWeaponDmg(uint16 objN) {
	int dmg = findObjValue(WEAPON_DAMAGE, WEAPON_DAMAGE_COUNT, objN, -1);
	if (dmg == -1) {
		const ActorStats *stats = getActorStats(objN);
		if (stats)
			dmg = stats->_dmg;
	}

	return dmg;
}

int U6Scripts::actorGetAc(Actor *actor) {
	int ac = 0;
	Common::Array<Obj *> inventory;
	getInventoryObjs(actor, false, inventory);

	for (uint i = 0; i < inventory.size(); ++i) {
		if (inventory[i]->is_readied())
			ac += findObjValue(ARMOUR, ARMOUR_COUNT, inventory[i]->obj_n, 0);
	}

	if (actor->is_cursed())
		ac -= 3;

	if (actor->is_protected())
		ac += 3;

	return ac;
}

Obj *U6Scripts::actorSelectObjFromList(Actor *actor, const uint16 *objNums, uint count) {
	Common::Array<Obj *> inventory;
	getInventoryObjs(actor, false, inventory);

	Obj *selected = nullptr;
	for (uint i = 0; i < inventory.size(); ++i) {
		bool listed = false;
		for (uint j = 0; j < count; ++j) {
			if (objNums[j] == inventory[i]->obj_n)
				listed = true;
		}

		if (listed && (!selected || randomRange(0, 1) == 0))
			selected = inventory[i];
	}

	return selected;
}

void U6Scripts::acidSlugDissolveItem(Actor *target) {
	Obj *obj = actorSelectObjFromList(target, ACID_SLUG_ITEMS, ARRAYSIZE(ACID_SLUG_ITEMS));

	if (obj) {
		playSfx(SFX_SLUG_DISSOLVE, true);
		print(Common::String::format("A slug dissolves %s's %s!\n", target->get_name(),
			Game::get_game()->get_obj_manager()->get_obj_name(obj)));
		removeInventoryObj(target, obj);
	}
}

void U6Scripts::gremlinStealItem(Actor *target) {
	Obj *obj = actorSelectObjFromList(target, GREMLIN_ITEMS, ARRAYSIZE(GREMLIN_ITEMS));

	if (obj) {
		playSfx(SFX_FAILURE, true);
		print(Common::String::format("`%s  has been robbed!\n", target->get_name()));
		removeInventoryObj(target, obj);
	}
}

void U6Scripts::removeInventoryObj(Actor *actor, Obj *obj) {
	actor->inventory_remove_obj(obj);
	delete_obj(obj);
}

void U6Scripts::actorPutToSleep(Actor *actor) {
	if (actor->get_obj_n() == OBJ_HORSE_WITH_RIDER)
		dismountActor(actor);

	if (actor->get_actor_num() != 0) {
		actor->set_asleep(true);
		if (actor->is_in_party())
			partyUpdateLeader();
	}
}

void U6Scripts::dismountActor(Actor *actor) {
	UseCode *usecode = Game::get_game()->get_usecode();
	Obj *actorObj = actor->make_obj();
	usecode->use_obj(actorObj, actor);
	delete_obj(actorObj);
}

void U6Scripts::unreadyObj(Actor *actor, Obj *obj) {
	// Try to unready via the usecode first
	UseCode *usecode = Game::get_game()->get_usecode();
	if (usecode->has_readycode(obj) && !usecode->ready_obj(obj, actor))
		return;

	actor->remove_readied_object(obj);
}

void U6Scripts::partyUpdateLeader() {
	Party *party = Game::get_game()->get_party();
	Actor *leader = party->get_leader_actor();

	if (leader)
		Game::get_game()->get_player()->update_player(leader);
}

bool U6Scripts::actorCanTurnInvisible(uint16 objN) {
	// Not giant scorpions, dragons, hydras, silver serpents or slimes
	return objN < 0x1aa && objN != 0x19b && objN != 0x176 && objN != 0x19d && objN != OBJ_SLIME;
}

bool U6Scripts::actorIntCheck(Actor *defender, Actor *attacker) {
	if (godMode()) {
		if (defender->is_in_party())
			return true;
		if (attacker->is_in_party())
			return false;
	}

	// Returns true if the defender loses the check
	return (actorIntAdjusted(attacker) / 2 + 15) - actorIntAdjusted(defender) > randomRange(1, 30);
}

void U6Scripts::subtractMovementPts(Actor *actor, int pts) {
	if (actor->get_obj_n() == 0x1af) // horse with rider
		pts /= 2;

	if (pts < 1)
		pts = 1;

	actor->set_moves_left((sint8)(actor->get_moves_left() - pts));

	if (_avatarDied)
		actorAvatarDeath();
}

bool U6Scripts::actorSubtractMovementPoints(Actor *actor, uint8 points) {
	subtractMovementPts(actor, points);
	return true;
}

bool U6Scripts::actorMove(Actor *actor, NuvieDir direction, int flag) {
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

	actor->set_direction(direction);
	bool didMove = actor->move(wrap_signed_coord(x, loc.z), wrap_signed_coord(y, loc.z), loc.z);

	if (didMove) {
		if (actor->get_obj_n() == OBJ_SLIME)
			slimeUpdateFrames();
		subtractMapMovementPts(actor);
	}

	return didMove;
}

int U6Scripts::actorMoveDiagonal(Actor *actor, NuvieDir xDirection, NuvieDir yDirection) {
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

	if (didMove) {
		if (actor->get_obj_n() == OBJ_SLIME)
			slimeUpdateFrames();
		subtractMapMovementPts(actor);
	}

	return didMove ? 1 : 0;
}

bool U6Scripts::actorMapDmg(Actor *actor, MapCoord location) {
	uint16 objN = actor->get_obj_n();

	if (!actor->is_alive() || actor->is_hit())
		return true;

	// Balloons, skiffs, rafts and ships are immune to map damage
	if (objN != OBJ_BALLOON && objN != OBJ_SKIFF && objN != OBJ_RAFT && objN != OBJ_SHIP &&
			!statFlag(objN, FLAG_IMMUNE_TO_TREMOR) &&
			!actor->is_protected() && !actor->is_in_vehicle()) {
		const Tile *tile = Game::get_game()->get_game_map()->get_dmg_tile(location.x, location.y, location.z);

		if (tile) {
			uint16 mapTile = tile->tile_num;
			actorTileDamage(actor, mapTile);

			if (mapTile == 732) {
				Obj *trap = mapGetObj(location.x, location.y, location.z, 0xad);
				if (trap)
					trap->set_invisible(false);
			}
		}
	}

	return true;
}

bool U6Scripts::actorTileDmg(Actor *actor, uint16 tileNum) {
	actorTileDamage(actor, tileNum);
	return true;
}

void U6Scripts::actorTileDamage(Actor *actor, uint16 mapTile) {
	if (godMode() && actor->is_in_party())
		return;

	uint16 objN = actor->get_obj_n();

	actor->set_hit_flag(true);

	if (mapTile == 564) {
		// Web
		if (objN != 0x169) { // giant spider
			if (actorStrAdjusted(actor) < randomRange(1, 0x1e)) {
				actor->set_paralyzed(true);
				print(Common::String::format("`%s is stuck in a web!\n", actor->get_name()));
			}
		}
	} else if ((mapTile >= 2 && mapTile <= 5) || mapTile == 1165) {
		// Swamp tiles or poison field
		Obj *boots = actor->inventory_get_readied_object(ACTOR_FOOT);
		bool swampBoots = boots && boots->obj_n == 0x1c;

		if (!swampBoots || mapTile == 1165) {
			if (!statFlag(objN, FLAG_IMMUNE_TO_POISON) && !actor->is_poisoned()) {
				actor->set_poisoned(true);
				print(Common::String::format("%s poisoned!\n", actor->get_name()));
				hitAnim(actor->get_x(), actor->get_y());
			}
		}
	} else if ((mapTile >= 220 && mapTile <= 223) || mapTile == 890 || mapTile == 1124 || mapTile == 1125 ||
			mapTile == 1130 || mapTile == 1131 || mapTile == 1164 || mapTile == 1193) {
		// Fire
		int dmg = (mapTile == 1164) ? randomRange(1, 0x1e) : randomRange(1, 8);
		if (statFlag(objN, FLAG_DOUBLE_FIRE_DAMAGE))
			dmg *= 2;

		actorHitTarget(Target(actor), dmg, nullptr);
	} else if (mapTile == 732 || mapTile == 1010) {
		// Trap, log saw
		actorHitTarget(Target(actor), randomRange(1, 0x1e), nullptr);
	} else if (mapTile >= 1458 && mapTile <= 1463) {
		if (objN != OBJ_TANGLE_VINE)
			actorHitTarget(Target(actor), randomRange(1, 8), nullptr);
	} else if (mapTile == 562 || mapTile == 731) {
		// Force field, spikes
		actorHitTarget(Target(actor), randomRange(1, 8), nullptr);
	} else if (mapTile == 1167) {
		// Sleep field
		if (!statFlag(objN, FLAG_IMMUNE_TO_SLEEP) && !actor->is_sleeping())
			actorPutToSleep(actor);
	}
}

uint8 U6Scripts::actorGetMaxMagicPoints(const Actor *actor) {
	uint16 objN = actor->get_base_obj_n();

	if (objN == 410) // avatar
		return actor->get_intelligence() * 2;

	if (objN == 378) // mage
		return actor->get_intelligence();

	if (objN == 377 || objN == 386) // swashbuckler, musician
		return actor->get_intelligence() / 2;

	return 0;
}

int U6Scripts::actorStrAdjusted(Actor *actor) {
	int str = actor->get_strength();

	if (!actor->is_cursed())
		return str;

	return (str <= 3) ? 1 : str - 3;
}

int U6Scripts::actorDexAdjusted(Actor *actor) {
	int dex = actor->get_dexterity();

	if (actor->is_cursed())
		dex = (dex <= 3) ? 1 : dex - 3;

	if (_timeStopped || actor->is_sleeping())
		dex = 1;

	return dex;
}

int U6Scripts::actorIntAdjusted(Actor *actor) {
	int intelligence = actor->get_intelligence();

	if (actor->is_cursed())
		intelligence -= 3;

	return MAX(intelligence, 1);
}

uint8 U6Scripts::actorStrAdj(Actor *actor) {
	return actorStrAdjusted(actor);
}

uint8 U6Scripts::actorDexAdj(Actor *actor) {
	return actorDexAdjusted(actor);
}

uint8 U6Scripts::actorIntAdj(Actor *actor) {
	return actorIntAdjusted(actor);
}

bool U6Scripts::actorCombatHitCheck(Actor *attacker, const Target &foe, const Target &weapon) {
	if (foe.isNull())
		return false;

	if (godMode() && attacker->is_in_party())
		return true;

	if (!foe.isActor() || weapon.objN() == 48) // glass sword
		return true;

	bool useStr = false;
	uint16 objN = weapon.objN();

	if (weapon.isActor()) {
		if (statFlag(objN, FLAG_STRENGTH_BASED))
			useStr = true;
	} else if (objN == 4 || objN == 34 || objN == 39 || objN == 44) { // spiked helm, club, mace, two-handed hammer
		useStr = true;
	}

	int attackRoll = useStr ? actorStrAdjusted(attacker) : actorDexAdjusted(attacker);
	int foeDex = actorDexAdjusted(foe._actor);
	int roll = randomRange(1, 30);

	return (foeDex + 30 - attackRoll) / 2 < roll;
}

bool U6Scripts::actorInit(Actor *actor, ActorAlignment alignment) {
	const ActorStats *base = getActorStats(actor->get_obj_n());

	if (base) {
		actor->set_strength(actorRandomiseStat(base->_str));
		actor->set_dexterity(actorRandomiseStat(base->_dex));
		actor->set_intelligence(actorRandomiseStat(base->_int));
		int hp = actorRandomiseStat(base->_hp);
		actor->set_hp(hp);
		actor->set_level((hp + 29) / 30);
		actor->set_alignment((ActorAlignment)base->_align);
	} else {
		actor->set_strength(10);
		actor->set_dexterity(10);
		actor->set_intelligence(10);
		actor->set_hp(30);
		actor->set_level(1);
	}

	if (alignment != ACTOR_ALIGNMENT_DEFAULT)
		actor->set_alignment(alignment);

	actor->set_worktype(8);
	actor->set_combat_mode(8);
	actor->set_magic(actorGetMaxMagicPoints(actor));
	actor->set_moves_left((sint8)actor->get_dexterity());
	actor->set_exp(100);

	if (actor->get_obj_n() == OBJ_TANGLE_VINE_POD) {
		// Add the tangle vines around the pod
		static const int OFFSET_X[4] = { 1, -1, 0, 0 };
		static const int OFFSET_Y[4] = { 0, 0, 1, -1 };
		static const NuvieDir DIRECTIONS[4] = { NUVIE_DIR_E, NUVIE_DIR_W, NUVIE_DIR_S, NUVIE_DIR_N };
		ActorManager *actorManager = Game::get_game()->get_actor_manager();

		for (int i = 0; i < 4; ++i) {
			int x = actor->get_x() + OFFSET_X[i];
			int y = actor->get_y() + OFFSET_Y[i];

			if (actorManager->can_put_actor(MapCoord(x, y, actor->get_z()))) {
				Actor *vine = newTempActor(OBJ_TANGLE_VINE, x, y, actor->get_z(), actor->get_alignment(), WT_TANGLE);
				if (vine)
					vine->set_direction(DIRECTIONS[i]);
			}
		}
	}

	if (base)
		actorInitInventory(actor, base);

	return true;
}

void U6Scripts::actorInitInventory(Actor *actor, const ActorStats *base) {
	ObjManager *objManager = Game::get_game()->get_obj_manager();

	// Spells
	for (uint i = 0; i < ARRAYSIZE(base->_spells) && base->_spells[i]; ++i) {
		Obj *obj = newObj(336); // charge
		obj->quality = base->_spells[i];
		obj->qty = 1;
		obj->status = 56;
		actor->inventory_add_object(obj, nullptr, false);
	}

	// Weapons, each with half the chance of the one before
	int chance = 2;
	for (uint i = 0; i < ARRAYSIZE(base->_weapons) && base->_weapons[i]; ++i) {
		uint16 v = base->_weapons[i];

		if (randomRange(1, chance) == 1) {
			// Spear, throwing axe, dagger
			int qty = (v >= 36 && v <= 38) ? randomRange(6, 12) : 1;

			for (int j = 0; j < qty; ++j) {
				Obj *obj = newObj(v);
				obj->status = 57;
				obj->qty = (v == 90) ? 1 : 0;
				actor->inventory_add_object(obj, nullptr, false);
				actor->add_readied_object(obj);

				if (v == 41 || v == 54) { // bow, magic bow
					obj = newObj(55); // arrow
					obj->status = 49;
					obj->qty = randomRange(12, 24);
					actor->inventory_add_object(obj, nullptr, false);
				} else if (v == 42 || v == 50) { // crossbow, triple crossbow
					obj = newObj(56); // bolt
					obj->status = 49;
					obj->qty = randomRange(12, 24);
					actor->inventory_add_object(obj, nullptr, false);
				}
			}
		}

		chance *= 2;
	}

	// Armour
	chance = 2;
	for (uint i = 0; i < ARRAYSIZE(base->_armour) && base->_armour[i]; ++i) {
		if (randomRange(1, chance) == 1) {
			Obj *obj = newObj(base->_armour[i]);
			obj->qty = 1;
			obj->status = 56;
			actor->inventory_add_object(obj, nullptr, false);
			actor->add_readied_object(obj);
		}

		chance *= 2;
	}

	// Treasure
	chance = 2;
	for (uint i = 0; i < ARRAYSIZE(base->_treasure) && base->_treasure[i]; ++i) {
		uint16 v = base->_treasure[i];

		if (randomRange(1, chance) == 1) {
			if (v == 98) { // chest
				Obj *chest = newObj(v);
				chest->frame_n = 1;
				chest->status = 41;
				chest->qty = randomRange(1, 100);
				actor->inventory_add_object(chest, nullptr, false);

				if (randomRange(0, 1) == 1)
					addChestItem(chest, 337, 0, 1, 22); // effect

				if (randomRange(0, 3) != 0)
					addChestItem(chest, 88, 0, randomRange(1, 100), 0); // gold coin

				if (randomRange(0, 3) != 0)
					addChestItem(chest, MONSTER_INV_FOOD[randomRange(0, 2)], 0, randomRange(1, 10), 0);

				if (randomRange(0, 3) == 0)
					addChestItem(chest, 90, 0, randomRange(1, 6), 0); // torch

				if (randomRange(0, 10) == 0)
					addChestItem(chest, 77, 0, randomRange(1, 4), 0); // gem
			} else if (v == 58) { // spell
				if (randomRange(0, 3) != 0 && actor->get_obj_n() != 362) { // not a gargoyle
					Obj *obj = newObj(88);
					obj->status = 49;
					obj->qty = randomRange(1, 100);
					actor->inventory_add_object(obj, nullptr, false);
				}

				if (randomRange(0, 3) == 0) {
					Obj *obj = newObj(MONSTER_INV_FOOD[randomRange(0, 2)]);
					obj->status = 49;
					obj->qty = randomRange(1, 100);
					actor->inventory_add_object(obj, nullptr, false);
				}

				if (randomRange(0, 3) == 0) {
					Obj *obj = newObj(275); // potion
					obj->frame_n = randomRange(0, 7);
					obj->status = 49;
					obj->qty = 1;
					actor->inventory_add_object(obj, nullptr, false);
				}
			} else if (v == 88) { // gold coin
				Obj *obj = newObj(v);
				obj->status = 49;
				obj->qty = randomRange(1, 30) + randomRange(1, 30);
				actor->inventory_add_object(obj, nullptr, false);
			} else {
				Obj *obj = newObj(v);
				obj->quality = 49;
				obj->qty = 1;
				actor->inventory_add_object(obj, nullptr, false);
			}
		}

		chance *= 2;
	}

	(void)objManager;
}

void U6Scripts::addChestItem(Obj *chest, uint16 objN, uint8 frameN, int qty, int quality) {
	Obj *obj = newObj(objN);
	obj->frame_n = frameN;
	obj->status = 41;
	obj->qty = qty;
	obj->quality = quality;
	Game::get_game()->get_obj_manager()->moveto_container(obj, chest);
}

void U6Scripts::actorTakeHit(Actor *attacker, const Target &defender, int maxDmg) {
	if (attacker->get_obj_n() == OBJ_CORPSER)
		attacker->set_frame_n(1); // Reveal the corpser

	if (maxDmg == -1)
		maxDmg = 1;
	else if (maxDmg > 1 && maxDmg != 255)
		maxDmg = randomRange(1, maxDmg);

	if (defender.isActor()) {
		int ac = actorGetAc(defender._actor);
		if (maxDmg != 255 && ac > 0)
			maxDmg -= randomRange(1, ac);
	}

	if (godMode()) {
		if (defender.isActor() && defender._actor->is_in_party())
			return;
		else if (attacker->is_in_party())
			maxDmg = 255;
	}

	if (maxDmg > 0) {
		int expGained = actorHitTarget(defender, maxDmg, attacker);
		attacker->set_exp(attacker->get_exp() + expGained);
	} else {
		print(Common::String::format("`%s grazed.\n", defender.name().c_str()));
	}

	if (!defender.isActor())
		return;

	Actor *defendingActor = defender._actor;
	actorYellForHelp(attacker, defender, maxDmg);

	uint16 defenderObjN = defendingActor->get_obj_n();

	if (defendingActor->is_alive() || defenderObjN == OBJ_BALLOON || defenderObjN == OBJ_SKIFF ||
			defenderObjN == OBJ_RAFT || defenderObjN == OBJ_SHIP) {
		uint16 attackerObjN = attacker->get_obj_n();

		if (attackerObjN == OBJ_CORPSER) {
			playSfx(SFX_CORPSER_DRAGGED_UNDER, true);
			print(Common::String::format("`%s dragged under!\n", defendingActor->get_name()));
			defendingActor->set_corpser_flag(true);
			if (defendingActor->is_in_party())
				partyUpdateLeader();
		}

		if (attackerObjN == 0x16c) // acid slug
			acidSlugDissolveItem(defendingActor);

		if (attackerObjN == 0x161) // gremlin
			gremlinStealItem(defendingActor);

		// Poisonous actors don't poison vehicles
		if (statFlag(attackerObjN, FLAG_POISONOUS) && randomRange(0, 3) == 0 && defendingActor->get_actor_num() != 0) {
			defendingActor->set_poisoned(true);
			print(Common::String::format("`%s poisoned!\n", defendingActor->get_name()));
		}

		if (maxDmg > 0)
			actorHitMsg(defendingActor);
	} else {
		print(Common::String::format("`%s killed!\n", defendingActor->get_name()));
	}
}

bool U6Scripts::actorHit(Actor *actor, uint8 dmg, bool displayHitMsg) {
	actorHitTarget(Target(actor), dmg, nullptr);

	if (displayHitMsg)
		actorHitMsg(actor);

	return true;
}

int U6Scripts::actorHitTarget(const Target &defender, int maxDmg, Actor *attacker, bool noHitAnim) {
	uint16 defenderObjN = defender.objN();
	int expGained = 0;

	if (defenderObjN == OBJ_BALLOON)
		return 0;

	if (defender.isActor()) {
		Actor *actor = defender._actor;

		if (godMode()) {
			if (actor->is_in_party())
				return 0;
			maxDmg = 255;
		}

		actor->set_hit_flag(true);
		if (!noHitAnim)
			hitAnim(actor->get_x(), actor->get_y());

		if (defenderObjN == OBJ_LORD_BRITISH) {
			actor->set_hp(0xff);
		} else if (defenderObjN == 414 || defenderObjN == 415) { // skiff, raft
			// Hit a random party member
			Party *party = Game::get_game()->get_party();
			int partySize = party->get_party_size();
			int memberNum = (partySize > 1) ? randomRange(0, partySize - 1) : 0;

			Actor *member = party->get_actor(memberNum);
			actorHitTarget(Target(member), maxDmg, attacker, true);
			actorHitMsg(member);
		} else if (maxDmg >= actor->get_hp()) {
			if (actor->get_hp() != 0) {
				if (actor->is_in_party())
					playerSubtractKarma(5); // Allowing your party to die is bad karma

				actor->set_hp(0);

				if (alignmentIsEvil(actor->get_alignment())) {
					const ActorStats *base = getActorStats(defenderObjN);
					if (base)
						expGained = (base->_str + base->_dex + base->_int + base->_hp + base->_dmg + base->_exp) / 4;
				}

				actor->set_invisible(false);

				if (defenderObjN == 412 && actor->get_actor_num() == 0) {
					// Ship destruction: unlink the ship front and back and toss out a raft
					actor->unlink_surrounding_objects(true);
					actor->change_base_obj_n(OBJ_RAFT);

					if (!actorMove(actor, NUVIE_DIR_N, 1)) {
						if (!actorMove(actor, NUVIE_DIR_S, 1)) {
							if (!actorMove(actor, NUVIE_DIR_E, 1))
								actorMove(actor, NUVIE_DIR_W, 1);
						}
					}

					actor->set_hp(10);
					Game::get_game()->get_party()->move(actor->get_x(), actor->get_y(), actor->get_z());
				} else {
					actorDead(actor);

					actor->set_worktype(WT_NOTHING);
					if (actor->is_in_party() && !actor->is_in_vehicle())
						partyUpdateLeader();
				}
			}
		} else {
			actor->set_hp(actor->get_hp() - maxDmg);

			if (actor->get_worktype() == WT_FLEE)
				actor->set_worktype(WT_ASSAULT);

			if (defenderObjN == OBJ_SLIME)
				slimeDivide(actor);
		}
	} else {
		Obj *obj = defender._obj;
		ObjManager *objManager = Game::get_game()->get_obj_manager();

		if (defenderObjN == OBJ_TANGLE_VINE) {
			// The pod doesn't have this text in the original for some reason
			print("\nTangle vine killed!\n");
		} else if (!objManager->is_stackable(obj) && obj->qty != 0) {
			hitAnim(obj->x, obj->y);

			// Check the frame of door objects, and don't attack open chests
			if ((defenderObjN < 0x129 || defenderObjN > 0x12c || obj->frame_n < 0xc) &&
					(defenderObjN != 0x62 || obj->frame_n != 3)) {
				if (obj->qty <= maxDmg) {
					print(Common::String::format("\n`%s broken!\n", defender.name().c_str()));

					// Look through the container for an effect object
					Common::Array<Obj *> children;
					getContainerObjs(obj, children);

					for (uint i = 0; i < children.size(); ++i) {
						if (children[i]->obj_n == 0x151) { // effect
							if (attacker)
								actorUseEffectOnActor(attacker, children[i]);
							break;
						} else {
							objManager->moveto_map(children[i], MapCoord(obj->x, obj->y, obj->z));
						}
					}

					if (defenderObjN == 0x7b) { // mirror
						playSfx(SFX_BROKEN_GLASS);
						obj->frame_n = 2;
						playerSubtractKarma(10);
					} else {
						removeFromEngine(obj);
					}
				} else {
					obj->qty -= maxDmg;
				}
			}
		}
	}

	return expGained;
}

void U6Scripts::actorHitMsg(Actor *actor) {
	if (actor->get_obj_n() == OBJ_BALLOON)
		return;

	int hp = actor->get_hp();
	if (hp == 0 || actor->get_maxhp() == 0)
		return;

	int di = (hp * 4) / actor->get_maxhp();
	if (di >= 4)
		return;

	Common::String s = Common::String::format("\n`%s ", actor->get_name());

	if (di == 0) {
		s += "critical!\n";
		int wt = actor->get_worktype();
		if (actorIntAdjusted(actor) >= 5 && wt != WT_BERSERK && wt != WT_STATIONARY && wt > WT_FRONT)
			actor->set_worktype(WT_RETREAT);
	} else {
		if (di == 1)
			s += "heavily ";
		else if (di == 2)
			s += "lightly ";
		else if (di == 3)
			s += "barely ";
		s += "wounded.\n";
	}

	print(s);
}

void U6Scripts::actorDead(Actor *actor) {
	uint16 objN = actor->get_obj_n();
	const ActorStats *base = getActorStats(objN);
	Game *game = Game::get_game();

	if (base) {
		if (objN == 0x163) // gazer
			newTempActor(0x157, actor->get_x(), actor->get_y(), actor->get_z()); // insect

		// Magical creatures fade away
		if (base->_flags[FLAG_FADES_AWAY - FLAG_CAN_TALK] == 1) {
			AsyncEffect *effect = new AsyncEffect(new TileFadeEffect(actor, 20));
			effect->run();
		}
	}

	bool inVehicle = actor->is_in_vehicle();

	// Blood. Farmers and musicians always bleed
	if ((!inVehicle && base && base->_flags[FLAG_DROPS_BLOOD - FLAG_CAN_TALK] == 1) || objN == 0x187 || objN == 0x188) {
		Obj *blood = newObj(0x152);
		blood->frame_n = randomRange(0, 2);
		blood->set_temporary(true);
		if (!game->get_obj_manager()->moveto_map(blood, actor->get_location()))
			delete_obj(blood);
	}

	if (actor->get_actor_num() != 1) { // not the avatar
		bool createBody = !inVehicle; // Don't drop a body when a party member dies on a raft

		if (isReadiableTile(actor->get_tile()->tile_num)) {
			Obj *animated = newObj(objN);
			if (!game->get_obj_manager()->moveto_map(animated, actor->get_location()))
				delete_obj(animated);
		}

		if (objN == OBJ_HORSE_WITH_RIDER)
			newTempActor(OBJ_HORSE, actor->get_x(), actor->get_y(), actor->get_z(), ACTOR_ALIGNMENT_NEUTRAL, 12);

		actor->die(createBody);
	} else {
		actor->set_hp(0); // Hackish way of stopping the avatar extinguishing his torch
		_avatarDied = true;
	}

	if (objN == OBJ_SLIME)
		slimeUpdateFrames();
}

void U6Scripts::slimeDivide(Actor *slime) {
	MapCoord loc = slime->get_location();
	ActorManager *actorManager = Game::get_game()->get_actor_manager();

	for (int i = 0; i < 8; ++i) {
		int newX = randomRange(1, 2) + loc.x - 1;
		int newY = randomRange(1, 2) + loc.y - 1;
		MapCoord newLoc(newX, newY, loc.z);

		if (actorManager->can_put_actor(newLoc)) {
			Actor *newActor;
			actorManager->clone_actor(slime, &newActor, newLoc);
			slimeUpdateFrames();
			print("Slime divides!\n");
			return;
		}
	}
}

void U6Scripts::slimeUpdateFrames() {
	for (int i = 1; i <= 0xff; ++i) {
		Actor *actor = getActor(i);

		if (actor->get_obj_n() == OBJ_SLIME && actor->is_alive()) {
			int newFrame = 0;

			for (int j = 0; j < 4; ++j) {
				Actor *other = mapGetActor(actor->get_x() + dirX((NuvieDir)j), actor->get_y() + dirY((NuvieDir)j), actor->get_z());
				if (other && other->get_obj_n() == OBJ_SLIME && other->is_alive())
					newFrame += 1 << j;
			}

			actor->set_frame_n(newFrame);
		}
	}
}

bool U6Scripts::outOfAmmo(Actor *attacker, Obj *weapon, bool printMessage) {
	uint16 objN = weapon->obj_n;

	if ((objN == 41 || objN == 54) && !attacker->inventory_has_object(55, 0, false)) { // bow, magic bow, arrows
		if (printMessage)
			print("Out of arrows!\n");
		return true;
	}

	if ((objN == 42 || objN == 50) && !attacker->inventory_has_object(56, 0, false)) { // crossbow, triple crossbow, bolts
		if (printMessage)
			print("Out of bolts!\n");
		return true;
	}

	return false;
}

bool U6Scripts::actorResurrect(Actor *actor) {
	int karma = Game::get_game()->get_player()->get_karma();

	if (karma < 98)
		actor->set_exp(actor->get_exp() * karma / 100);

	return true;
}

bool U6Scripts::actorUseEffect(Obj *effectObj, Actor *actor) {
	actorUseEffectOnActor(actor, effectObj);
	return true;
}

void U6Scripts::actorUseEffectOnActor(Actor *actor, Obj *effect) {
	int effectType = randomRange(0, 3);
	removeFromEngine(effect); // Done first so it won't be seen in the chest

	if (effectType == 0) {
		print("Acid!\n");
		actorHitTarget(Target(actor), randomRange(1, 0x14), nullptr);
	} else if (effectType == 1) {
		print("Poison!\n");
		actor->set_poisoned(true);
		hitAnim(actor->get_x(), actor->get_y());
	} else if (effectType == 2) {
		print("Bomb!\n");
		Common::Array<MapEntity> hits;
		explosion(0x17e, actor->get_x(), actor->get_y(), hits);

		for (uint i = 0; i < hits.size(); ++i) {
			if (hits[i].entity_type == ENT_ACTOR)
				actorHitTarget(Target(hits[i].actor), randomRange(1, 0x14), nullptr);
		}
	} else {
		print("Gas!\n");
		Common::Array<MapEntity> hits;
		explosion(0x17c, actor->get_x(), actor->get_y(), hits);

		for (uint i = 0; i < hits.size(); ++i) {
			if (hits[i].entity_type == ENT_ACTOR) {
				hits[i].actor->set_poisoned(true);
				hitAnim(hits[i].actor->get_x(), hits[i].actor->get_y());
			}
		}
	}
}

bool U6Scripts::canGetObjOverride(Obj *obj) {
	// Meat, ribs and dead animals are top tile objects and need an override check
	return obj->obj_n == 209 || obj->obj_n == 210 || obj->obj_n == 211;
}

bool U6Scripts::actorGetObj(Actor *actor, Obj *obj, Obj *container) {
	Game *game = Game::get_game();

	if (!game->get_obj_manager()->can_get_obj(obj)) {
		print("\n\nNot possible.");
		return false;
	}

	if (!actor->can_carry_weight(obj)) {
		print("\n\nThe total is too heavy.");
		return false;
	}

	MapCoord playerLoc = game->get_player()->get_actor()->get_location();

	// The stealing logic only applies to the surface level
	if (playerLoc.z == 0 && !obj->is_ok_to_take()) {
		bool caughtStealing = false;
		playerSubtractKarma(1);
		playSfx(SFX_FAILURE, true);

		for (int i = 1; i <= 202; ++i) { // permanent actors
			Actor *other = getActor(i);

			if (other->is_alive() && other->get_z() == playerLoc.z && !other->is_in_party() &&
					other->get_alignment() == ACTOR_ALIGNMENT_NEUTRAL && !other->is_sleeping()) {
				if (actorFindMaxXYDistance(other, playerLoc.x, playerLoc.y) < 6) {
					print("\n\n\"Stop Thief!!!\"\n");
					activateCityGuards();
					caughtStealing = true;
					break;
				}
			}
		}

		if (!caughtStealing)
			print("\n\nStealing!!!\n");
	}

	obj->set_ok_to_take(true);

	game->get_obj_manager()->moveto_inventory(obj, actor);
	subtractMovementPts(actor, 3);

	return true;
}

void U6Scripts::actorUpdateFlags(Actor *actor) {
	if (!actor->is_alive())
		return;

	Obj *ring1 = actor->inventory_get_readied_object(ACTOR_HAND);
	Obj *ring2 = actor->inventory_get_readied_object(ACTOR_HAND_2);
	bool invisibilityRing = (ring1 && ring1->obj_n == 0x102) || (ring2 && ring2->obj_n == 0x102);

	if (!actor->is_visible() && randomRange(0, 0x3f) == 0 && !invisibilityRing) {
		if (actorIntAdjusted(actor) <= randomRange(1, 0x1e))
			actor->set_invisible(false);
	}

	if (actor->is_protected() && randomRange(0, 0x3f) == 0) {
		if (actorIntAdjusted(actor) <= randomRange(1, 0x1e))
			actor->set_protected(false);
	}

	if (actor->is_cursed() && randomRange(0, 15) == 0) {
		if (actorIntAdjusted(actor) >= randomRange(1, 0x1e))
			actor->set_cursed(false);
	}

	if (actor->is_charmed() && randomRange(0, 7) == 0) {
		if (actorIntAdjusted(actor) >= randomRange(1, 0x1e))
			actorRemoveCharm(actor);
	}

	if (actor->is_paralyzed() && randomRange(0, 3) == 0) {
		if (actorStrAdjusted(actor) >= randomRange(1, 0x1e)) {
			actor->set_paralyzed(false);
			partyUpdateLeader();
		}
	}

	if (actor->is_sleeping() && actor->get_worktype() != WT_SLEEP && randomRange(0, 15) == 0) {
		actor->set_asleep(false);
		actor->set_hit_flag(true);
		actor->set_frame_n(actor->get_old_frame_n());
		if (actor->is_in_party())
			partyUpdateLeader();
	}

	if (actor->is_poisoned() && !actor->is_protected() && randomRange(0, 7) == 0) {
		// Don't show the animation if the actor is off screen
		actorHitTarget(Target(actor), 1, nullptr, !mapIsOnScreen(actor->get_x(), actor->get_y(), actor->get_z()));
	}
}

void U6Scripts::actorCorpserRegurgitation(Actor *actor) {
	if (!actor->get_corpser_flag())
		return;

	if (actor->get_worktype() == WT_PLAYER)
		print(Common::String::format("\n%s:\nARGH!\n", actor->get_name()));

	if (randomRange(1, 0x1e) < actorStrAdjusted(actor)) {
		playSfx(SFX_CORPSER_REGURGITATE, true);
		print(Common::String::format("`%s regurgitated!\n", actor->get_name()));
		actor->set_corpser_flag(false);
		if (actor->is_in_party())
			partyUpdateLeader();
	} else {
		actorHitTarget(Target(actor), randomRange(1, 0xf), nullptr);
		actor->set_moves_left(0);
	}
}

bool U6Scripts::actorRemoveCharm(Actor *actor) {
	if (!actor->is_charmed())
		return false;

	actor->set_charmed(false);
	actor->set_alignment((ActorAlignment)actor->get_old_alignment());

	if (actor->is_in_party())
		actor->set_alignment(ACTOR_ALIGNMENT_GOOD);

	partyUpdateLeader();
	return true;
}

void U6Scripts::actorYellForHelp(Actor *attacker, const Target &defender, int dmg) {
	if (!defender.isActor())
		return;

	Actor *defendingActor = defender._actor;
	if (defendingActor->get_hp() == 0)
		return;

	if (defendingActor->get_alignment() == ACTOR_ALIGNMENT_NEUTRAL && !defendingActor->is_in_party()) {
		if (attacker->get_worktype() == WT_PLAYER && getActorStats(defendingActor->get_obj_n())) {
			if (statFlag(defendingActor->get_obj_n(), FLAG_CAN_TALK) && dmg > 0) {
				print(Common::String::format("`%s yells for help!\n", defendingActor->get_name()));
				activateCityGuards();
			}
		}

		if (defendingActor->get_worktype() >= 0x80) {
			if (attacker->get_worktype() == WT_PLAYER) {
				if (defendingActor->get_obj_n() == 0x188) // musician
					defendingActor->set_frame_n(defendingActor->get_old_frame_n());
				defendingActor->set_worktype(WT_ATTACK_PARTY);
			}
		} else {
			if (attacker->get_alignment() != ACTOR_ALIGNMENT_GOOD)
				defendingActor->set_alignment(ACTOR_ALIGNMENT_GOOD);
			else
				defendingActor->set_alignment(ACTOR_ALIGNMENT_CHAOTIC);

			if (defendingActor->is_temp())
				defendingActor->set_worktype(WT_ASSAULT);
		}
	}
}

void U6Scripts::activateCityGuards() {
	MapCoord playerLoc = Game::get_game()->get_player()->get_actor()->get_location();

	playerSubtractKarma(5);

	for (int i = 1; i <= 0xff; ++i) {
		Actor *actor = getActor(i);

		if (actor->is_alive() && actor->get_z() == playerLoc.z && !actor->is_in_party() &&
				actor->get_alignment() == ACTOR_ALIGNMENT_NEUTRAL) {
			if (actor->get_obj_n() == 0x17e) { // guard
				if (actorFindMaxXYDistance(actor, playerLoc.x, playerLoc.y) < 0x20)
					actor->set_worktype(WT_GUARD_ARREST_PLAYER);
			} else {
				int wt = actor->get_worktype();

				if (wt != WT_STATIONARY && wt != WT_NOTHING && wt != WT_PLAYER && !actor->is_paralyzed() &&
						!actor->is_sleeping() && !actor->get_corpser_flag()) {
					if (actorFindMaxXYDistance(actor, playerLoc.x, playerLoc.y) < 6) {
						if (wt < 0x80) {
							actor->set_worktype(WT_UNK_13);
						} else if (wt > 0x86) {
							if (actor->get_obj_n() == 0x188)
								actor->set_frame_n(actor->get_old_frame_n());
							actor->set_worktype(WT_ATTACK_PARTY);
						}
					}
				}
			}
		}
	}
}

void U6Scripts::getLBToThrone() {
	GameClock *clock = Game::get_game()->get_clock();

	// Lord British is on his throne from 8 to 12 and 14 to 18
	int hour = clock->get_hour();
	while (hour < 8 || hour > 17 || (hour > 11 && hour < 14)) {
		advanceTime(60);
		hour = clock->get_hour();
	}
}

void U6Scripts::actorAvatarDeath() {
	Game *game = Game::get_game();
	Party *party = game->get_party();
	Player *player = game->get_player();

	_avatarDied = false; // before getLBToThrone()
	Actor *avatar = getActor(1);

	if (avatar->get_obj_n() == OBJ_HORSE_WITH_RIDER)
		dismountActor(avatar);

	avatar->set_asleep(true); // Makes it look like the avatar is dead
	print("\nAn unending darkness engulfs thee...\n\n");
	playSfx(SFX_AVATAR_DEATH, true);
	fadeOutEffect();
	print("A voice in the darkness intones, \"KAL LOR!\"\n");
	playSfx(SFX_KAL_LOR, true);
	avatar->set_asleep(false);
	party->dismount_from_horses();

	if (!_armageddon)
		party->resurrect_dead_members();

	// Unready the inventory, except the amulet of submission
	Common::Array<Obj *> inventory;
	getInventoryObjs(avatar, false, inventory);
	for (uint i = 0; i < inventory.size(); ++i) {
		if (inventory[i]->is_readied() && inventory[i]->obj_n != 76)
			unreadyObj(avatar, inventory[i]);
	}

	partyHeal(); // Needs to heal before the player moves
	partyUpdateLeader();
	player->move(0x133, 0x160, 0, true);
	party->exit_vehicle(0x133, 0x160, 0);
	getLBToThrone();

	for (int i = 0; i <= 0xff; ++i) {
		Actor *actor = getActor(i);
		actor->set_moves_left(0);

		if (!actor->is_temp() && actor->get_worktype() == WT_ATTACK_PARTY)
			actor->set_worktype(WT_WALK_TO_LOCATION);
	}

	avatar->set_moves_left(1);

	actorResurrect(avatar);
	party->set_in_combat_mode(false);
	player->set_party_mode(party->get_actor(0));
	fadeInEffect();
}

bool U6Scripts::isAvatarDead() {
	return _avatarDied;
}

void U6Scripts::actorCalculateAvgCoords() {
	Game *game = Game::get_game();
	Party *party = game->get_party();
	MapCoord playerLoc = game->get_player()->get_actor()->get_location();
	Actor *player = game->get_player()->get_actor();
	NuvieDir playerDir = player->get_direction();

	int n = 0;
	int avgX = 0;
	int avgY = 0;

	Common::Array<Actor *> members;
	getPartyMembers(members);
	for (uint i = 0; i < members.size(); ++i) {
		Actor *actor = members[i];
		if (actor->get_worktype() != WT_FLANK && actor->get_worktype() != WT_BERSERK) {
			++n;
			avgX += actor->get_x();
			avgY += actor->get_y();
		}
	}

	if (n > 0) {
		_partyAvgX = avgX / n;
		_partyAvgY = avgY / n;
	} else {
		_partyAvgX = playerLoc.x;
		_partyAvgY = playerLoc.y;
	}

	n = 0;
	avgX = 0;
	avgY = 0;

	for (int i = 0; i <= 0xff; ++i) {
		Actor *actor = getActor(i);

		if (actor->get_obj_n() != 0 && actor->is_alive()) {
			if (actor->get_alignment() == ACTOR_ALIGNMENT_EVIL || actor->get_alignment() == ACTOR_ALIGNMENT_CHAOTIC) {
				if (actor->get_worktype() != WT_RETREAT ||
						(ABS((int)actor->get_x() - (int)playerLoc.x) <= 5 && ABS((int)actor->get_y() - (int)playerLoc.y) <= 5)) {
					if (ABS((int)actor->get_x() - (int)playerLoc.x) < 0x18 && ABS((int)actor->get_y() - (int)playerLoc.y) < 0x18) {
						++n;
						avgX += actor->get_x();
						avgY += actor->get_y();
					}
				}
			}
		}
	}

	_numMonstersNear = n;

	if (n > 0) {
		_combatAvgX = (2 * avgX + n) / (2 * n);
		_combatAvgY = (2 * avgY + n) / (2 * n);

		_rearMaxMonster = nullptr;
		_rearMinMonster = nullptr;

		int tmpX = _combatAvgX - _partyAvgX;
		int tmpY = _combatAvgY - _partyAvgY;

		_frontTargetActor = nullptr;
		int minMonsterPos = 0;
		int maxMonsterPos = 0;
		int minPartyPos = 0;
		int maxPartyPos = 0;

		for (int i = 0; i <= 0xff; ++i) {
			Actor *actor = getActor(i);

			if (actor->get_obj_n() != 0 && actor->is_alive() &&
					(actor->get_worktype() == WT_FRONT || actor->get_worktype() == WT_PLAYER)) {
				int pos = (((int)actor->get_x() - _partyAvgX) * tmpY) - (((int)actor->get_y() - _partyAvgY) * tmpX);

				if (!actor->is_in_party()) {
					if (!_frontTargetActor)
						_frontTargetActor = actor;
					if (pos > maxMonsterPos) {
						maxMonsterPos = pos;
						_rearMaxMonster = actor;
					}
					if (pos < minMonsterPos) {
						minMonsterPos = pos;
						_rearMinMonster = actor;
					}
				} else {
					if (pos > maxPartyPos) {
						maxPartyPos = pos;
						_rearMaxParty = actor;
					}
					if (pos < minPartyPos) {
						minPartyPos = pos;
						_rearMinParty = actor;
					}
				}
			}
		}

		if (!_rearMaxParty)
			_rearMaxParty = player;
		if (!_rearMinParty)
			_rearMinParty = player;
	} else {
		// Used by party members in the combat front worktype
		_combatAvgX = playerLoc.x + dirX(playerDir);
		_combatAvgY = playerLoc.y + dirY(playerDir);
	}

	if (_combatAvgX == _partyAvgX && _combatAvgY == _partyAvgY) {
		_combatAvgX += dirX(playerDir);
		_combatAvgY += dirY(playerDir);
	}

	(void)party;
}

bool U6Scripts::actorUpdateAll() {
	Game *game = Game::get_game();
	Player *player = game->get_player();

	if (_avatarDied) {
		actorAvatarDeath();
		return true;
	}

	_timeStopped = Game::get_game()->get_clock()->get_timer(TIMER_TIME_STOP) != 0;

	actorCalculateAvgCoords();

	Actor *selectedActor;
	do {
		selectedActor = nullptr;
		int di = 0;
		int dex6 = 1;

		do {
			MapCoord playerLoc = player->get_actor()->get_location();
			int chunkX = (playerLoc.x - 16) - (playerLoc.x - 16) % 8;
			int chunkY = (playerLoc.y - 16) - (playerLoc.y - 16) % 8;

			Common::Array<Actor *> members;
			getPartyMembers(members);
			for (uint i = 0; i < members.size(); ++i) {
				if (members[i]->get_worktype() == WT_FOLLOW && members[i]->get_moves_left() < 0)
					members[i]->set_moves_left(0);
			}

			for (int i = 0; i <= 0xff; ++i) {
				Actor *actor = getActor(i);

				if (actor->get_obj_n() != 0 && actor->get_z() == playerLoc.z && actor->get_moves_left() > 0 &&
						!actor->is_paralyzed() && !actor->is_sleeping() && actor->get_worktype() != WT_NOTHING &&
						actor->is_alive()) {
					if (ABS((int)actor->get_x() - chunkX) > 0x27 || ABS((int)actor->get_y() - chunkY) > 0x27) {
						int wt = actor->get_worktype();

						if (wt >= 0x83 && wt <= 0x86) {
							// Move the actor to its schedule location if that isn't on screen
							MapCoord schedLoc;
							actor->get_schedule_location(&schedLoc);

							if (!mapIsOnScreen(schedLoc.x, schedLoc.y, schedLoc.z)) {
								actor->move(schedLoc.x, schedLoc.y, schedLoc.z);
								wtWalkToLocation(actor); // This will cancel the pathfinder and set the new worktype
								subtractMovementPts(actor, 10);
							}
						}
					} else if (actor->get_worktype() != WT_FOLLOW) {
						if (actor->get_worktype() == 0x80)
							actor->set_worktype(actor->get_sched_worktype());

						int movesLeft = actor->get_moves_left();
						int dex = actor->get_dexterity();
						int dx = (movesLeft * dex6) - dex * di;

						if (movesLeft >= dex || dx > 0 || (dx == 0 && dex > dex6)) {
							selectedActor = actor;
							di = movesLeft;
							dex6 = dex;
						}

						if (movesLeft >= dex)
							break;
					}
				}
			}

			if (di <= 0)
				advanceTime(1);
		} while (di <= 0);

		if (selectedActor->get_corpser_flag())
			actorCorpserRegurgitation(selectedActor);

		if (!selectedActor->get_corpser_flag()) {
			if (selectedActor->get_worktype() != WT_PLAYER && selectedActor->get_worktype() != WT_FOLLOW)
				performWorktype(selectedActor);
		}
	} while (selectedActor->get_obj_n() == 0 || selectedActor->get_worktype() != WT_PLAYER ||
		selectedActor->get_corpser_flag());

	// Swap the player to the next party member with the 'command' combat worktype
	Actor *oldPlayer = player->get_actor();
	if (selectedActor != oldPlayer)
		player->update_player(selectedActor);
	// The worktype is changed to follow by update_player()
	oldPlayer->set_worktype(WT_PLAYER);

	if (_updateVolcano) {
		Obj *obj = findVolcanoNearPlayer();
		if (obj) {
			if (obj->obj_n == 307) { // volcano
				playSfx(SFX_EARTH_QUAKE, false);
				quakeStart(1, 200);
			} else {
				// Duplicate the fumarole so it can be exploded
				Obj *fumarole = new Obj(obj);
				explodeObj(fumarole);
			}
		}

		_updateVolcano = false;
	}

	displayPrompt(true);
	return true;
}

Obj *U6Scripts::findVolcanoNearPlayer() {
	const uint8 range = 5;
	ObjManager *objManager = Game::get_game()->get_obj_manager();
	uint16 px, py;
	uint8 pz;
	Game::get_game()->get_player()->get_location(&px, &py, &pz);

	for (uint16 y = py - range; y < py + range; ++y) {
		for (uint16 x = px - range; x < px + range; ++x) {
			U6LList *objList = objManager->get_obj_list(x, y, pz);

			if (objList) {
				for (U6Link *link = objList->start(); link; link = link->next) {
					Obj *obj = (Obj *)link->data;
					if (obj->obj_n == OBJ_U6_VOLCANO || obj->obj_n == OBJ_U6_FUMAROLE)
						return obj;
				}
			}
		}
	}

	return nullptr;
}

bool U6Scripts::advanceTime(uint16 minutes) {
	Game *game = Game::get_game();
	GameClock *clock = game->get_clock();
	int numTurns = minutes;
	bool teleport = numTurns >= 60;

	int timeStopTimer = clock->get_timer(TIMER_TIME_STOP);
	if (timeStopTimer != 0) {
		if (timeStopTimer > numTurns) {
			clock->set_timer(TIMER_TIME_STOP, timeStopTimer - numTurns);
			return true;
		}

		clock->set_timer(TIMER_TIME_STOP, 0);
	}

	clock->update_timers(numTurns);

	// Update the keg timer and explode kegs
	if (_kegTimer > 0) {
		_kegTimer = MAX(_kegTimer - numTurns, 0);
		if (_kegTimer == 0)
			explodeKeg();
	}

	bool cloakReadied = false;

	for (int i = 0; i <= 0xff; ++i) {
		Actor *actor = getActor(i);

		if (actor->get_obj_n() == 0)
			continue;

		int movesLeft = actor->get_moves_left();
		actor->set_moves_left((movesLeft >= 0) ? actor->get_dexterity() : movesLeft + actor->get_dexterity());

		if (actor->get_worktype() == WT_FOLLOW && actor->get_corpser_flag())
			actorCorpserRegurgitation(actor);

		if (actor->is_in_party()) {
			Common::Array<Obj *> inventory;
			getInventoryObjs(actor, false, inventory);

			for (uint j = 0; j < inventory.size(); ++j) {
				Obj *obj = inventory[j];
				if (!obj->is_readied())
					continue;

				const char *objName = nullptr;

				if (obj->obj_n == 0x5a && obj->frame_n == 1) { // lit torch
					if (randomRange(0, 1) == 0) {
						if (obj->qty > numTurns)
							obj->qty -= numTurns;
						else
							unreadyObj(actor, obj);
					}
				} else if (obj->obj_n == 0x102) { // invisibility ring
					if (randomRange(0, 1000) == 429) {
						Obj *hand1 = actor->inventory_get_readied_object(ACTOR_HAND);
						Obj *hand2 = actor->inventory_get_readied_object(ACTOR_HAND_2);
						bool twoRings = hand1 && hand2 && hand1->obj_n == 0x102 && hand2->obj_n == 0x102;
						if (!twoRings)
							actor->set_invisible(false);
						objName = "ring";
					}
				} else if (obj->obj_n == 0x101) { // regeneration ring
					actor->set_hp(MIN(actor->get_hp() + numTurns, (int)actor->get_maxhp()));

					if (randomRange(0, 1000) == 734)
						objName = "ring";
				} else if (obj->obj_n == 0x51) { // storm cloak
					if (randomRange(0, 1000) == 588) {
						objName = "cloak";
						clock->set_timer(TIMER_STORM, 0);
					} else {
						cloakReadied = true;
					}
				}

				if (objName) {
					removeInventoryObj(actor, obj);
					print(Common::String::format("A %s has vanished!\n", objName));
				}
			}
		}

		for (int j = 0; j < numTurns; ++j)
			actorUpdateFlags(actor);

		actorMapDmg(actor, actor->get_location());
		actor->set_hit_flag(false);
	}

	if (cloakReadied)
		clock->set_timer(TIMER_STORM, 1);

	int minute = clock->get_minute();
	clock->inc_minute(numTurns);

	if (minute + numTurns >= 60) {
		game->get_actor_manager()->updateSchedules(teleport);

		// Update magic points
		Common::Array<Actor *> members;
		getPartyMembers(members);
		for (uint i = 0; i < members.size(); ++i) {
			Actor *member = members[i];
			int maxMagic = actorGetMaxMagicPoints(member);

			if (maxMagic != 0) // Avatar, mage, swashbuckler, musician
				member->set_magic(MIN(member->get_magic() + member->get_level(), maxMagic));
		}

		game->get_player()->dec_alcohol(1);
	}

	if (_avatarDied)
		actorAvatarDeath();

	if (randomRange(0, 7) == 0)
		_updateVolcano = true;

	return true;
}

bool U6Scripts::setGShowStealing(bool stealing) {
	_showStealing = stealing;
	return true;
}

} // End of namespace U6
} // End of namespace Scripts
} // End of namespace Ultima6
} // End of namespace Ultima
