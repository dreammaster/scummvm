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
#include "ultima/ultima6/core/game_clock.h"
#include "ultima/ultima6/core/obj_manager.h"
#include "ultima/ultima6/core/player.h"
#include "ultima/ultima6/gui/widgets/map_window.h"
#include "ultima/ultima6/script/common/script_random.h"
#include "ultima/ultima6/script/u6/u6_scripts.h"

namespace Ultima {
namespace Ultima6 {
namespace Scripts {
namespace U6 {

namespace {

enum {
	OBJ_CHARGE = 0x150,
	OBJ_SPELLBOOK = 0x39,
	OBJ_WISP = 0x175,
	OBJ_GLASS_SWORD = 0x30,
	OBJ_BOOMERANG = 0x31,
	OBJ_TRIPLE_CROSSBOW = 0x32,
	OBJ_MAGIC_BOW = 0x36,
	OBJ_ZU_YLEM = 0x5b,
	OBJ_MORNING_STAR = 40,
	OBJ_HALBERD = 47,
	OBJ_BOLT = 0x38,
	OBJ_ARROW = 0x37,
	OBJ_FIRE_FIELD = 317
};

} // End of anonymous namespace

Target U6Scripts::actorGetWeapon(Actor *attacker, const Target &foe) {
	if (foe.isNull())
		return Target();

	bool inParty = attacker->is_in_party();
	int range = actorGetCombatRange(attacker, foe.x(), foe.y());
	int maxDmg = 0;
	Obj *weapon = nullptr;

	Common::Array<Obj *> inventory;
	getInventoryObjs(attacker, false, inventory);

	for (uint i = 0; i < inventory.size(); ++i) {
		Obj *obj = inventory[i];

		if (obj->obj_n == OBJ_CHARGE && Game::get_game()->get_clock()->get_timer(TIMER_STORM) == 0 &&
				randomRange(0, 3) == 0 && randomRange(0, obj->quality) < 16) {
			return Target(obj);
		}

		if (!inParty || obj->is_readied()) {
			int dmg = getWeaponDmg(obj->obj_n);

			if (dmg != -1 && dmg > maxDmg && getWeaponRange(obj->obj_n) >= range) {
				maxDmg = dmg;
				weapon = obj;
			}
		}
	}

	// Attack with bare hands
	return weapon ? Target(weapon) : Target(attacker);
}

bool U6Scripts::actorAttack(Actor *actor, MapCoord location, Obj *weapon, Actor *foe) {
	actorAttackTarget(actor, location.x, location.y, location.z, weapon ? Target(weapon) : Target(actor), Target(foe));
	Game::get_game()->get_map_window()->updateBlacking(); // The script might have broken a door
	return true;
}

void U6Scripts::actorAttackTarget(Actor *attacker, int targetX, int targetY, int targetZ, const Target &weapon,
		Target foe) {
	uint16 weaponObjN = weapon.objN();
	int weaponQuality = weapon.isActor() ? -1 : weapon._obj->quality;

	if (foe.isActor() && foe._actor->get_actor_num() == attacker->get_actor_num()) {
		print(Common::String::format("\n%s is try to attack itself. Report me!\n", attacker->get_name()));
		return;
	}

	if (foe.isNull()) {
		Obj *foeObj = mapGetObj(targetX, targetY, targetZ);
		if (foeObj)
			foe = Target(foeObj);
	}

	if (actorGetCombatRange(attacker, targetX, targetY) > getWeaponRange(weaponObjN))
		return;

	// Check for arrows or bolts if attacking with a bow or crossbow
	if (!weapon.isActor() && outOfAmmo(attacker, weapon._obj, false))
		return;

	int xDiff = targetX - attacker->get_x();
	int yDiff = targetY - attacker->get_y();
	NuvieDir attackerDirection;

	if (ABS(xDiff) <= ABS(yDiff))
		attackerDirection = (yDiff >= 0) ? NUVIE_DIR_S : NUVIE_DIR_N;
	else
		attackerDirection = (xDiff >= 0) ? NUVIE_DIR_E : NUVIE_DIR_W;

	uint16 actorObjN = attacker->get_obj_n();

	// Don't change direction for the ship and silver serpent
	if (actorObjN != 412 && actorObjN != 413)
		attacker->set_direction(attackerDirection);

	if (actorObjN >= 0x170 && actorObjN <= 0x174) { // skeleton, mongbat
		int frameN = attacker->get_frame_n();
		attacker->set_frame_n(frameN - (frameN % 4) + 2);
	}

	bool isRangeWeapon = false;
	const ProjectileInfo *projectileInfo = getProjectileInfo(weaponObjN);

	if (weaponObjN != OBJ_MORNING_STAR && weaponObjN != OBJ_HALBERD) {
		if (ABS(xDiff) > 1 || ABS(yDiff) > 1 || projectileInfo)
			isRangeWeapon = true;
	}

	int dmg = getWeaponDmg(weaponObjN);
	if (dmg == -1)
		dmg = 1;

	MapCoord playerLoc = Game::get_game()->get_player()->get_actor()->get_location();

	if (weaponObjN == OBJ_CHARGE || weaponObjN == OBJ_SPELLBOOK) {
		if (weaponQuality == 0x81 && actorObjN == OBJ_WISP) {
			// Special wisp magic: the wisp teleports next to the player
			if (tossActor(attacker, playerLoc.x, playerLoc.y, playerLoc.z, 0))
				print("Wisp teleports!\n");

			return;
		}

		MapCoord target(targetX, targetY, targetZ);
		magicCastSpell(weaponQuality, attacker, &target);

		if (weaponQuality == 0x45 && !foe.isNull())
			print(Common::String::format("`%s is paralyzed.\n", foe.name().c_str()));

		return;
	}

	bool hitActor = actorCombatHitCheck(attacker, foe, weapon);
	int numBolts = 1;
	Common::Array<MapCoord> collTiles;
	Common::Array<Target> collFoes;

	if (isRangeWeapon) {
		// Get new target coordinates if the attack roll failed. This is done before collision
		// checking, which means even blocked targets can cause a miss. Empty tiles always hit
		if (!hitActor && !foe.isNull()) {
			targetX += randomRange(0, 2) - 1;
			targetY += randomRange(0, 2) - 1;
		}

		if (weaponObjN == OBJ_TRIPLE_CROSSBOW)
			numBolts = MIN<int>(attacker->inventory_count_object(OBJ_BOLT), 3);

		// Handles collision checking, the animation, ammo removal and placing thrown weapons
		combatRangeWeapon(attacker, targetX, targetY, targetZ, weapon, numBolts, collTiles, collFoes);
		foe = collFoes[0];

		// If the first attack was a miss, do another attack roll
		if (!hitActor)
			hitActor = actorCombatHitCheck(attacker, foe, weapon);
	} else if (actorFindMaxXYDistance(attacker, playerLoc.x, playerLoc.y) < 6) {
		playSfx(SFX_ATTACK_SWING, true);
	}

	if (weaponObjN == OBJ_TRIPLE_CROSSBOW) {
		// Do an attack roll for the extra projectiles and deal damage to each target hit
		for (int i = 0; i < numBolts; ++i) {
			if (i > 0) {
				foe = collFoes[i];
				hitActor = actorCombatHitCheck(attacker, foe, weapon);
			}

			if (hitActor) {
				if (!foe.isNull() && foe.objN() != attacker->get_obj_n())
					actorTakeHit(attacker, foe, dmg);

				if (_avatarDied)
					return;
			}
		}

		return;
	}

	if (hitActor && !foe.isNull() && attacker->get_obj_n() != foe.objN()) {
		// Takes half damage
		if (statFlag(foe.objN(), FLAG_REPEL_UNDEAD) && weaponObjN != OBJ_GLASS_SWORD &&
				weaponObjN != OBJ_TRIPLE_CROSSBOW && weaponObjN != OBJ_MAGIC_BOW)
			dmg = (dmg + 1) / 2;

		if (weaponObjN != OBJ_ZU_YLEM)
			actorTakeHit(attacker, foe, dmg);

		if (weaponObjN == OBJ_GLASS_SWORD) {
			print("Thy sword hath shattered!\n");
			if (!weapon.isActor())
				removeInventoryObj(attacker, weapon._obj);
		}
	}

	if (weaponObjN == OBJ_BOOMERANG && projectileInfo) {
		// Stops the return projectile if the avatar has died
		if (attacker->get_x() != 0 && attacker->get_y() != 0) {
			projectile(projectileInfo->_tileNum, collTiles[0].x, collTiles[0].y, attacker->get_x(), attacker->get_y(),
				projectileInfo->_speed, projectileInfo->_spin);
		}
	}
}

void U6Scripts::combatRangeWeapon(Actor *attacker, int targetX, int targetY, int targetZ, const Target &weapon,
		int numBolts, Common::Array<MapCoord> &collTiles, Common::Array<Target> &collFoes) {
	// Handles the projectile collision checking, animation, ammo removal, the placement of
	// thrown weapons and the Zu Ylem sleep effect. The original used globals for the new targets
	uint16 weaponObjN = weapon.objN();
	Common::Array<MapCoord> targets;
	Common::Array<MapCoord> preCollTiles;
	const ProjectileInfo *info = getProjectileInfo(weaponObjN);
	Game *game = Game::get_game();

	targets.push_back(MapCoord(targetX, targetY, targetZ));

	if (weaponObjN == OBJ_TRIPLE_CROSSBOW) {
		int index = ((targetY - (int)attacker->get_y() + 5) * 11) + (targetX - (int)attacker->get_x() + 5);

		for (int i = 0; i < 2; ++i) {
			NuvieDir offsetDir = NUVIE_DIR_N;
			int offsetX = 0, offsetY = 0;

			if (index >= 0 && index < 121) {
				// The offset direction is numbered from north going clockwise through the diagonals
				static const int DIAGONAL_X[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };
				static const int DIAGONAL_Y[8] = { -1, -1, 0, 1, 1, 1, 0, -1 };
				int dir = PROJECTILE_OFFSETS[i][index];
				offsetX = DIAGONAL_X[dir];
				offsetY = DIAGONAL_Y[dir];
			}

			(void)offsetDir;
			targets.push_back(MapCoord(targetX + offsetX, targetY + offsetY, targetZ));
		}
	}

	// Collision checking gives the new target and the tile before the collision for each projectile
	for (int i = 0; i < numBolts; ++i) {
		int hitX, hitY, preHitX, preHitY;
		mapLineHitCheck(attacker->get_x(), attacker->get_y(), targets[i].x, targets[i].y, targets[i].z,
			hitX, hitY, preHitX, preHitY);

		Target foe;
		Actor *foeActor = mapGetActor(hitX, hitY, targets[i].z);
		if (foeActor) {
			foe = Target(foeActor);
		} else {
			Obj *foeObj = mapGetObj(hitX, hitY, targets[i].z);
			if (foeObj)
				foe = Target(foeObj);
		}

		collTiles.push_back(MapCoord(hitX, hitY, targets[i].z));
		collFoes.push_back(foe);
		preCollTiles.push_back(MapCoord(preHitX, preHitY, targets[i].z));
	}

	// The animation does its own collision checking as well
	if (info) {
		if (weaponObjN == OBJ_TRIPLE_CROSSBOW) {
			Common::Array<MapEntity> hitEntities;
			projectileAnimMulti(info->_tileNum, attacker->get_x(), attacker->get_y(), collTiles, info->_speed, false,
				info->_rotation, hitEntities);
		} else {
			projectile(info->_tileNum, attacker->get_x(), attacker->get_y(), collTiles[0].x, collTiles[0].y,
				info->_speed, info->_spin);
		}
	}

	Target foe = collFoes[0];
	const MapCoord &preColl = preCollTiles[0];

	if (weaponObjN == OBJ_ZU_YLEM) {
		if (foe.isActor()) {
			attacker->inventory_del_object(OBJ_ZU_YLEM, 1, 0);
			spellPutActorToSleep(attacker, foe._actor);
		}
	} else if (weaponObjN == 0x4f || weaponObjN == 0x50) {
		// Lightning wand, fire wand
		if (randomRange(1, 100) == 37) {
			removeInventoryObj(attacker, weapon._obj);
			print("A wand has vanished!\n");
		}
	} else if (weaponObjN == 0x53) {
		// Flask of oil
		attacker->inventory_del_object(0x53, 1, 0);

		if (!game->get_game_map()->is_water(preColl.x, preColl.y, preColl.z, true)) {
			Obj *fire = newObj(OBJ_FIRE_FIELD);
			fire->set_temporary(true);
			if (!game->get_obj_manager()->moveto_map(fire, preColl))
				delete_obj(fire);
		}
	} else if (weaponObjN == 0x24 || weaponObjN == 0x25 || weaponObjN == 0x26) {
		// Spear, throwing axe, dagger. Only remove the thrown weapon if the target is more than one tile away
		if (ABS(collTiles[0].x - (int)attacker->get_x()) > 1 || ABS(collTiles[0].y - (int)attacker->get_y()) > 1) {
			// Remove unreadied throwing weapons first so the weapon stays readied
			Common::Array<Obj *> inventory;
			getInventoryObjs(attacker, false, inventory);
			Obj *removalCandidate = nullptr;

			for (uint i = 0; i < inventory.size(); ++i) {
				if (inventory[i]->obj_n == weaponObjN) {
					removalCandidate = inventory[i];
					if (!removalCandidate->is_readied())
						break;
				}
			}

			if (removalCandidate) {
				removeInventoryObj(attacker, removalCandidate);

				// Don't place the thrown weapon if the target is an actor, or on impassable water
				// or the ethereal void unless there's already an object there
				uint16 destTile = Game::get_game()->get_game_map()->get_tile(preColl.x, preColl.y, preColl.z, true)->tile_num;
				bool impassableWater = tileGetFlag(destTile, 1, 0) && tileGetFlag(destTile, 1, 1);
				bool etherealVoid = destTile >= 252 && destTile <= 255;
				bool canPlace = !((impassableWater || etherealVoid) && !mapGetObj(preColl.x, preColl.y, preColl.z));

				if (!foe.isActor() && canPlace) {
					Obj *thrown = newObj(weaponObjN);
					thrown->set_ok_to_take(true);
					thrown->set_temporary(true);
					if (!game->get_obj_manager()->moveto_map(thrown, preColl))
						delete_obj(thrown);
				}
			}
		}
	} else if (weaponObjN == 0x29 || weaponObjN == 0x2a || weaponObjN == OBJ_TRIPLE_CROSSBOW || weaponObjN == OBJ_MAGIC_BOW) {
		// Bow, crossbow, triple crossbow, magic bow
		uint16 ammoObjN = (weaponObjN == 0x29 || weaponObjN == OBJ_MAGIC_BOW) ? OBJ_ARROW : OBJ_BOLT;
		int qty = (weaponObjN == OBJ_TRIPLE_CROSSBOW) ? 3 : 1;

		attacker->inventory_del_object(ammoObjN, qty, 0);
	}
}

} // End of namespace U6
} // End of namespace Scripts
} // End of namespace Ultima6
} // End of namespace Ultima
