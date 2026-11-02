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
#include "ultima/ultima6/core/obj_manager.h"
#include "ultima/ultima6/script/common/script_random.h"
#include "ultima/ultima6/script/md/md_scripts.h"

namespace Ultima {
namespace Ultima6 {
namespace Scripts {
namespace MD {

namespace {

enum {
	OBJ_SHOTGUN = 43,
	OBJ_BELGIAN_COMBINE = 45,
	OBJ_HEAT_RAY_GUN = 240,
	OBJ_FREEZE_RAY_GUN = 241,
	OBJ_WEED_SPRAYER = 129,
	OBJ_SPRAY_GUN = 261,
	OBJ_CUPIDS_BOW_AND_ARROWS = 40,
	OBJ_PISTOL_ROUND = 57,
	OBJ_SHOTGUN_SHELL = 58,
	OBJ_RIFLE_ROUND = 59
};

// How a ranged weapon looks and sounds, and what it uses up. Ammunition is an
// object number, or -1 for thrown weapons, -2 for charges, -3 for nothing
struct ProjectileInfo {
	uint16 _objN;
	uint16 _tileNum;
	uint16 _sfxId;
	int _ammoObjN;
};

const ProjectileInfo PROJECTILES[] = {
	{ 41, 0x103, 7, 57 },
	{ 42, 0x103, 7, 57 },
	{ 44, 0x103, 8, 59 },
	{ 45, 0x103, 8, 59 },
	{ 46, 0x103, 8, 60 },
	{ 47, 0x23E, 6, 63 },
	{ 48, 0x23F, 6, 64 },
	{ 40, 0x23F, 6, -3 },
	{ 240, 0x16B, 10, -2 },
	{ 241, 0x16A, 10, -2 },
	{ 129, 0x10A, 6, -2 },
	{ 261, 0x10A, 6, -2 },
	{ 313, 0x10B, 50, -3 },
	{ 366, 0x16F, 34, -3 },
	{ 386, 0x16C, 34, -3 },
	{ 364, 0x16D, 6, -3 },
	{ 384, 0x16E, 6, -3 }
};

// Spread weapons: sound, projectile tile and damage
struct SpreadInfo {
	uint16 _objN;
	uint16 _sfxId;
	uint16 _tileNum;
	uint8 _damage;
};

const SpreadInfo SPREAD_WEAPONS[] = {
	{ 0x2b, 8, 0x106, 0x14 },
	{ 0x2d, 8, 0x106, 0x14 },
	{ 0xf0, 0xa, 0x14e, 0x19 },
	{ 0xf1, 0xa, 0x14f, 0xa }
};

const SpreadInfo *getSpreadInfo(uint16 objN) {
	for (uint i = 0; i < ARRAYSIZE(SPREAD_WEAPONS); ++i) {
		if (SPREAD_WEAPONS[i]._objN == objN)
			return &SPREAD_WEAPONS[i];
	}

	return nullptr;
}

bool isSprayer(uint16 objN) {
	return objN == OBJ_WEED_SPRAYER || objN == OBJ_SPRAY_GUN;
}

} // End of anonymous namespace

int MDScripts::getWeaponDamage(const Target &weapon) {
	if (weapon.isActor())
		return actorGetDamage(weapon.objN());

	return findObjValue(WEAPON_DAMAGE, WEAPON_DAMAGE_COUNT, weapon.objN(), -1);
}

bool MDScripts::attackDexSavingThrow(Actor *attacker, const Target &defender, const Target &weapon) {
	if (!defender.isActor())
		return true;

	int attackerValue;
	if (weapon.isActor() && isActorStatBitSet(weapon.objN(), 5))
		attackerValue = actorStrAdjusted(attacker);
	else
		attackerValue = actorDexAdjusted(attacker);

	int defenderValue = actorDexAdjusted(defender._actor);

	return randomRange(1, 30) >= (defenderValue + 30 - attackerValue) / 2;
}

bool MDScripts::outOfAmmo(Actor *attacker, Obj *weapon, bool printMessage) {
	if (!weapon)
		return false;

	uint16 objN = weapon->obj_n;

	if (((objN == 41 || objN == 42) && !attacker->inventory_has_object(OBJ_PISTOL_ROUND, 0, false)) ||
			(objN == OBJ_SHOTGUN && !attacker->inventory_has_object(OBJ_SHOTGUN_SHELL, 0, false)) ||
			(objN == 44 && !attacker->inventory_has_object(OBJ_RIFLE_ROUND, 0, false)) ||
			(objN == OBJ_BELGIAN_COMBINE && weapon->quality == 0 &&
				(!attacker->inventory_has_object(OBJ_SHOTGUN_SHELL, 0, false) || !attacker->inventory_has_object(OBJ_RIFLE_ROUND, 0, false))) ||
			(objN == OBJ_BELGIAN_COMBINE && weapon->quality == 1 && !attacker->inventory_has_object(OBJ_RIFLE_ROUND, 0, false)) ||
			(objN == OBJ_BELGIAN_COMBINE && weapon->quality == 2 && !attacker->inventory_has_object(OBJ_SHOTGUN_SHELL, 0, false)) ||
			(objN == 46 && !attacker->inventory_has_object(60, 0, false)) ||
			(objN == 47 && !attacker->inventory_has_object(63, 0, false)) ||
			((objN == OBJ_HEAT_RAY_GUN || objN == OBJ_FREEZE_RAY_GUN || isSprayer(objN)) && weapon->qty == 0)) {
		if (printMessage) {
			printl("OUT_OF_AMMUNITION");
			playMdSfx(5);
		}
		return true;
	}

	if (objN == 48 && !attacker->inventory_has_object(64, 0, false)) { // bow, arrows
		if (printMessage) {
			printl("OUT_OF_ARROWS");
			playMdSfx(5);
		}
		return true;
	}

	return false;
}

void MDScripts::attackWithFreezeray(Actor *actor, Actor *target, int damage) {
	uint16 objN = target->get_obj_n();

	if (getActorStats(objN) && (isActorStatBitSet(objN, 14) || isActorStatBitSet(objN, 7))) {
		target->set_paralyzed(true);
		printfl("ACTOR_PARALYZED", target->get_name());

		if (!isActorStatBitSet(objN, 7) || isActorStatBitSet(objN, 14))
			hitTarget(Target(target), BLUE_HIT_TILE);
		else
			actorTakeHit(actor, Target(target), damage, DAMAGE_COLD);
	} else {
		printl("IT_HAS_NO_EFFECT");
	}
}

int MDScripts::checkAmmo(Actor *actor, Obj *weapon) {
	uint16 objN = weapon->obj_n;

	if (objN == 47 && !actor->inventory_has_object(63, 0, false))
		return ATTACK_NO_AMMO;

	if ((objN == 41 || objN == 42) && !actor->inventory_has_object(OBJ_PISTOL_ROUND, 0, false))
		return ATTACK_NO_AMMO;

	if (objN == 46 && !actor->inventory_has_object(60, 0, false))
		return ATTACK_NO_AMMO;

	if (objN == 44 && !actor->inventory_has_object(OBJ_RIFLE_ROUND, 0, false))
		return ATTACK_NO_AMMO;

	if (objN == OBJ_SHOTGUN && !actor->inventory_has_object(OBJ_SHOTGUN_SHELL, 0, false))
		return ATTACK_NO_AMMO;

	if (isSprayer(objN) && weapon->qty == 0)
		return ATTACK_NO_AMMO;

	return ATTACK_OK;
}

int MDScripts::attackTargetWithWeapon(Actor *actor, uint16 targetX, uint16 targetY, const Target &weapon) {
	int targetRange = actorGetCombatRange(actor, targetX, targetY);

	if (targetRange > getWeaponRange(weapon.objN()))
		return ATTACK_OUT_OF_RANGE;

	if (!weapon.isActor()) {
		int ret = checkAmmo(actor, weapon._obj);
		if (ret != ATTACK_OK)
			return ret;
	}

	uint16 objN = weapon.objN();
	Obj *weaponObj = weapon._obj;
	bool singleShot = false;
	bool spread = false;

	if (objN == OBJ_HEAT_RAY_GUN || objN == OBJ_FREEZE_RAY_GUN || objN == OBJ_BELGIAN_COMBINE) {
		if (weaponObj->quality < 2)
			singleShot = true;

		if (weaponObj->quality != 1)
			spread = true;

		if (objN == OBJ_BELGIAN_COMBINE) {
			if (singleShot && !actor->inventory_has_object(OBJ_RIFLE_ROUND, 0, false))
				singleShot = false;
			if (spread && !actor->inventory_has_object(OBJ_SHOTGUN_SHELL, 0, false))
				spread = false;
		} else {
			if (weaponObj->qty == 0)
				return ATTACK_NO_AMMO;

			if (weaponObj->qty < (spread ? 4 : 0) + (singleShot ? 1 : 0))
				spread = false;
		}

		if (!singleShot && !spread)
			return ATTACK_NO_AMMO;
	} else if (objN == OBJ_SHOTGUN) {
		spread = true;
	} else {
		singleShot = true;
	}

	int damageMode = DAMAGE_NORMAL;
	if (objN == OBJ_HEAT_RAY_GUN)
		damageMode = DAMAGE_HEAT;
	if (objN == OBJ_FREEZE_RAY_GUN)
		damageMode = DAMAGE_COLD;

	_attackTarget = _selectedTarget;

	if (!singleShot) {
		spreadWeaponDamage(actor, targetX, targetY, weaponObj);
		return ATTACK_OK;
	}

	bool isRangedAttack = targetRange > 1;
	if ((objN >= 40 && objN <= 48) || isSprayer(objN) || objN == OBJ_HEAT_RAY_GUN || objN == OBJ_FREEZE_RAY_GUN)
		isRangedAttack = true;

	int damage;
	if (weapon.isActor()) {
		damage = actorGetDamage(actor->get_obj_n());
		if (damage < 0)
			damage = 1;
	} else {
		damage = getWeaponDamage(weapon);
		if (damage < 0)
			damage = 1;
	}

	bool hasSavingThrow = false;
	bool doesDamage = true;

	Target target = findRockwormActor(_attackTarget);
	if (target.isActor() && !isSprayer(objN)) {
		hasSavingThrow = true;
		doesDamage = attackDexSavingThrow(actor, target, weapon);
	}

	if (isRangedAttack)
		fireRangeBasedWeapon(actor, targetX, targetY, weapon);

	target = findRockwormActor(_attackTarget);

	if (!isRangedAttack && mapIsOnScreen(actor->get_x(), actor->get_y(), actor->get_z()))
		playMdSfx(0);

	if (!hasSavingThrow && target.isActor() && !isSprayer(objN))
		doesDamage = attackDexSavingThrow(actor, target, weapon);

	if (!doesDamage)
		playMdSfx(3);

	if (doesDamage && !target.isNull() && (!target.isActor() || target._actor->get_actor_num() != actor->get_actor_num())) {
		if (objN == OBJ_FREEZE_RAY_GUN) {
			if (target.objN() == 160 && target.frameN() == 1) {
				printl("THE_WATER_FREEZES");
				target._obj->frame_n = 2;
			} else if (target.isActor()) {
				attackWithFreezeray(actor, target._actor, damage);
			}
		} else if (isSprayer(objN)) {
			if (weaponObj->quality != 0) {
				if (target.isActor() && target.objN() == 145) { // OBJ_MONSTER_FOOTPRINTS
					target._actor->set_obj_n(364); // OBJ_PROTO_MARTIAN
					printfl("BECOMES_VISIBLE", target.name().c_str());
				} else if (!target.isActor() && target._obj->is_invisible()) {
					target._obj->set_invisible(false);
					printfl("BECOMES_VISIBLE", target.name().c_str());
				} else {
					printl("IT_HAS_NO_EFFECT");
				}
			} else if (getActorStats(target.objN()) && !isActorStatBitSet(target.objN(), 14) &&
					isActorStatBitSet(target.objN(), 7)) {
				actorTakeHit(actor, target, damage, DAMAGE_ACID);
			} else {
				printl("IT_HAS_NO_EFFECT");
			}
		} else if (objN == OBJ_CUPIDS_BOW_AND_ARROWS) {
			if (target.isActor() && target._actor->get_alignment() != ACTOR_ALIGNMENT_GOOD) {
				Actor *targetActor = target._actor;
				targetActor->set_old_alignment(targetActor->get_alignment());
				targetActor->set_alignment(ACTOR_ALIGNMENT_GOOD);
				targetActor->set_charmed(true);
				printfl("ACTOR_CHARMED", targetActor->get_name());
			}
		} else {
			actorTakeHit(actor, target, damage, damageMode);
		}
	}

	if (spread)
		spreadWeaponDamage(actor, targetX, targetY, weaponObj);

	return ATTACK_OK;
}

void MDScripts::spreadWeaponDamage(Actor *actor, uint16 targetX, uint16 targetY, Obj *weapon) {
	const SpreadInfo *info = getSpreadInfo(weapon->obj_n);
	if (!info)
		return;

	playMdSfx(info->_sfxId);

	// FIXME spread weapon anim here
	Common::Array<MapCoord> targets;
	targets.push_back(MapCoord(targetX, targetY, actor->get_z()));
	targets.push_back(MapCoord(targetX + 1, targetY - 1, actor->get_z()));

	Common::Array<MapEntity> hitItems;
	projectileAnimMulti(info->_tileNum, actor->get_x(), actor->get_y(), targets, 2, true, 0, hitItems);

	for (uint i = 0; i < hitItems.size(); ++i) {
		Target hit;
		if (hitItems[i].entity_type == ENT_OBJ)
			hit = Target(hitItems[i].obj);
		else if (hitItems[i].entity_type == ENT_ACTOR)
			hit = Target(hitItems[i].actor);
		else
			continue;

		if (weapon->obj_n == OBJ_FREEZE_RAY_GUN) {
			if (hit.objN() == 160 && hit.frameN() == 1) { // OBJ_EMPTY_BUCKET
				printl("THE_WATER_FREEZES");
				hit._obj->frame_n = 2;
			} else if (hit.isActor()) {
				if (randomRange(1, 0x2d) > actorDexAdjusted(hit._actor)) {
					attackWithFreezeray(actor, hit._actor, 10);
				} else {
					printfl("ACTOR_DODGES", hit._actor->get_name());
					playMdSfx(3);
				}
			}
		} else if (!hit.isActor() || randomRange(1, 0x2d) > actorDexAdjusted(hit._actor)) {
			int damageMode = (weapon->obj_n == OBJ_HEAT_RAY_GUN) ? DAMAGE_HEAT : DAMAGE_NORMAL;
			actorTakeHit(actor, hit, info->_damage, damageMode);
		} else {
			printfl("ACTOR_DODGES", hit.name().c_str());
			playMdSfx(3);
		}
	}

	if (weapon->obj_n == OBJ_SHOTGUN || weapon->obj_n == OBJ_BELGIAN_COMBINE)
		actor->inventory_del_object(OBJ_SHOTGUN_SHELL, 1, 0);
	else
		weapon->qty = MAX(weapon->qty - 4, 0);
}

void MDScripts::fireRangeBasedWeapon(Actor *attacker, uint16 targetX, uint16 targetY, const Target &weapon) {
	uint16 tileNum = weapon.isActor() ? weapon._actor->get_tile()->tile_num : objTileNum(weapon._obj);
	uint16 sfxId = 0;
	int ammoObjN = -1;

	for (uint i = 0; i < ARRAYSIZE(PROJECTILES); ++i) {
		if (PROJECTILES[i]._objN == weapon.objN()) {
			tileNum = PROJECTILES[i]._tileNum;
			sfxId = PROJECTILES[i]._sfxId;
			ammoObjN = PROJECTILES[i]._ammoObjN;
			break;
		}
	}

	playMdSfx(sfxId);
	projectileAnim(tileNum, attacker->get_x(), attacker->get_y(), targetX, targetY, 4, false, 0);

	if (ammoObjN > 0) {
		attacker->inventory_del_object(ammoObjN, 1, 0);
	} else if (ammoObjN == -2) {
		weapon._obj->qty -= 1;
	} else if (ammoObjN == -1 && !weapon.isActor() && actorGetCombatRange(attacker, targetX, targetY) > 1) {
		if (isOpenWaterAtLoc(targetX, targetY, attacker->get_z()))
			removeObj(weapon._obj);
		else
			Game::get_game()->get_obj_manager()->moveto_map(weapon._obj, MapCoord(targetX, targetY, attacker->get_z()));
		// FIXME the original updates the readied weapons here
	}
}

} // End of namespace MD
} // End of namespace Scripts
} // End of namespace Ultima6
} // End of namespace Ultima
