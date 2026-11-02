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
#include "ultima/ultima6/core/party.h"
#include "ultima/ultima6/core/player.h"
#include "ultima/ultima6/script/md/md_scripts.h"

namespace Ultima {
namespace Ultima6 {
namespace Scripts {
namespace MD {

namespace {

const MapCoord MAP_ENTRANCES[] = {
	MapCoord(0x43, 0x51, 0x1),
	MapCoord(0x80, 0x8D, 0x0),
	MapCoord(0x76, 0x0F2, 0x3),
	MapCoord(0x7A, 0x0C, 0x3),
	MapCoord(0x9A, 0x41, 0x4),
	MapCoord(0x2B2, 0x1CC, 0x0),
	MapCoord(0x73, 0x40, 0x4),
	MapCoord(0x29D, 0x1CE, 0x0),
	MapCoord(0x0B3, 0x0E1, 0x4),
	MapCoord(0x27B, 0x1F1, 0x0),
	MapCoord(0x0C3, 0x70, 0x1),
	MapCoord(0x0CB, 0x1D1, 0x0),
	MapCoord(0x24, 0x0F1, 0x1),
	MapCoord(0x31A, 0x232, 0x0),
	MapCoord(0x5C, 0x0F1, 0x1),
	MapCoord(0x34C, 0x25A, 0x0),
	MapCoord(0x7C, 0x28, 0x4),
	MapCoord(0x13E, 0x118, 0x0),
	MapCoord(0x0A, 0x61, 0x5),
	MapCoord(0x3B7, 0x1C4, 0x0),
	MapCoord(0x5C, 0x0B1, 0x5),
	MapCoord(0x3DC, 0x1DD, 0x0),
	MapCoord(0x5E, 0x29, 0x4),
	MapCoord(0x227, 0x1ab, 0x0)
};

const MapCoord POD_TELEPORTS[] = {
	MapCoord(0xCA, 0x295, 0),
	MapCoord(0x34D, 0x295, 0),
	MapCoord(0x13A, 0x17A, 0),
	MapCoord(0x2CD, 0x1FA, 0)
};

// Offsets for the four directions, indexed by door frame
const int MOVEMENT_OFFSET_X[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };
const int MOVEMENT_OFFSET_Y[8] = { -1, -1, 0, 1, 1, 1, 0, -1 };

} // End of anonymous namespace

/** The player's attack command, which asks for a target for each readied weapon */
class MDScripts::PlayerAttack : public Continuation {
private:
	MDScripts *_scripts;
	Target _weapon;

protected:
	uint8 run() override {
		switch (_step) {
		case 0:
			_weapon = _scripts->weaponSelect();
			return wait(NUVIE_SCRIPT_GET_TARGET, 1);

		default: {
			if (_resumedWithNil) {
				_scripts->printl("WHAT");
				return NUVIE_SCRIPT_FINISHED;
			}

			_scripts->selectAttackTarget(_location);
			_scripts->playerAttackWithWeapon(_weapon, _location);

			Obj *next = _scripts->selectNextWeapon();
			if (!next)
				return NUVIE_SCRIPT_FINISHED;

			_weapon = Target(next);
			return wait(NUVIE_SCRIPT_GET_TARGET, 1);
		}
		}
	}

public:
	explicit PlayerAttack(MDScripts *scripts) : _scripts(scripts) {}
};

ScriptThread *MDScripts::playerAttackThread() {
	return new PlayerAttack(this);
}

bool MDScripts::playerPass() {
	printl("PASS");
	updateConveyorBelt(true);
	return true;
}

MovementStatus MDScripts::playerBeforeMoveAction(sint16 *relX, sint16 *relY) {
	if (*relX != 0 && *relY != 0)
		return CAN_MOVE;

	Game *game = Game::get_game();
	MapCoord playerLoc = game->get_player()->get_actor()->get_location();

	if (*relX == 0 && *relY == 1) {
		// Fall off a cliff
		const Tile *tile = game->get_game_map()->get_tile(playerLoc.x, playerLoc.y, playerLoc.z, false);
		if (tile && tile->tile_num >= 384 && tile->tile_num <= 387)
			return FORCE_MOVE;
	}

	uint8 z = playerLoc.z;
	uint16 x = wrapCoord(playerLoc.x + *relX, z);
	uint16 y = wrapCoord(playerLoc.y + *relY, z);

	Common::Array<Obj *> objs;
	getObjsAt(x, y, z, objs);

	for (uint i = 0; i < objs.size(); ++i) {
		Obj *obj = objs[i];

		if (obj->obj_n == 268) { // Wheelbarrow
			moveWheelbarrow(obj, *relX, *relY);
			if (canMoveObj(obj, *relX, *relY))
				setObjLocation(obj, obj->x + *relX, obj->y + *relY, obj->z);
		} else if (obj->obj_n == 410 && canMoveObj(obj, *relX, *relY)) { // Railcar
			moveRailCart(obj, *relX, *relY);
		} else if (obj->obj_n == 441) { // Assembled drill
			moveDrill(obj, *relX, *relY);
			if (canMoveDrill(obj, *relX, *relY))
				setObjLocation(obj, obj->x + *relX, obj->y + *relY, obj->z);
		}
	}

	return CAN_MOVE;
}

void MDScripts::updateObjectsAroundParty() {
	MapCoord loc = Game::get_game()->get_player()->get_actor()->get_location();
	ActorManager *actorManager = Game::get_game()->get_actor_manager();

	Common::Array<Obj *> objs;
	getObjsInArea(wrapCoord(loc.x - 5, loc.z), wrapCoord(loc.y - 5, loc.z), loc.z, 11, 11, objs);

	for (uint i = 0; i < objs.size(); ++i) {
		Obj *obj = objs[i];

		if ((obj->obj_n == 227 && actorManager->get_actor(0x73)->get_flag(2)) || // OBJ_DOOR3
				(obj->obj_n == 179 && actorManager->get_actor(0x73)->get_flag(4))) { // OBJ_CLOSED_DOOR
			// Doors that are stuck stay as they are
			if (obj->quality & 0x80)
				continue;

			int baseFrame = obj->frame_n & 2;
			Actor *actor = mapGetActor(obj->x, obj->y, obj->z);

			if (actor) {
				if (mapIsOnScreen(obj->x, obj->y, obj->z))
					playDoorSfx();
				obj->frame_n = baseFrame + 9;
				continue;
			}

			actor = mapGetActor(obj->x + MOVEMENT_OFFSET_X[baseFrame], obj->y + MOVEMENT_OFFSET_Y[baseFrame], obj->z);
			if (!actor)
				actor = mapGetActor(obj->x + MOVEMENT_OFFSET_X[baseFrame + 4], obj->y + MOVEMENT_OFFSET_Y[baseFrame + 4], obj->z);

			if (actor && mapIsOnScreen(obj->x, obj->y, obj->z)) {
				playDoorSfx();
				if (obj->frame_n < 4)
					obj->frame_n = baseFrame + 5;
				else if (obj->frame_n < 8)
					obj->frame_n = baseFrame + 1;
				else
					obj->frame_n = baseFrame + 5;
			} else if (obj->frame_n == 5) {
				obj->frame_n = 1;
			} else if (obj->frame_n == 7) {
				obj->frame_n = 3;
			}
		} else if (obj->obj_n == 301) { // OBJ_REFLECTIVE_SURFACE
			if (obj->frame_n >= 3)
				continue;

			Actor *actor = mapGetActor(obj->x, obj->y + 1, obj->z);
			if (!actor) {
				obj->frame_n = 0;
				continue;
			}

			uint8 actorNum = actor->get_actor_num();
			if (actorNum >= 0x70 || actorNum == 0x5d || (actorNum >= 0x6a && actorNum <= 0x6c))
				obj->frame_n = 0;
			else if ((actorNum < 0x20 && actorNum != 6) || (actorNum >= 0x2a && actorNum <= 0x5c) ||
					actorNum == 0x60 || actorNum == 0x69 || actorNum == 0x6d)
				obj->frame_n = 1;
			else
				obj->frame_n = 2;
		}
	}
}

bool MDScripts::playerPostMoveAction(bool didMove) {
	Game *game = Game::get_game();
	ActorManager *actorManager = game->get_actor_manager();
	Player *player = game->get_player();
	MapCoord playerLoc = player->get_actor()->get_location();

	if (!didMove) {
		actorMapDmg(player->get_actor(), playerLoc);
		playMdSfx(0);
		return true;
	}

	const Tile *tile = game->get_game_map()->get_tile(playerLoc.x, playerLoc.y, playerLoc.z, true);
	if (tile && tile->tile_num == 0x6f) {
		printl("FASTER");
		// FIXME update falling brick animation speed here
	}

	updateConveyorBelt(true);
	updateObjectsAroundParty();

	Common::Array<Obj *> objs;
	getObjsAt(playerLoc.x, playerLoc.y, playerLoc.z, objs);

	for (uint i = 0; i < objs.size(); ++i) {
		Obj *obj = objs[i];
		uint16 objN = obj->obj_n;

		if (objN == 175 || objN == 163 || objN == 180 || objN == 178 || (objN == 197 && obj->frame_n == 3) || objN == 210) {
			if (!player->in_party_mode()) {
				printl(objN == 163 || objN == 178 ? "YOU_MUST_BE_IN_PARTY_MODE_TO_LEAVE" : "YOU_MUST_BE_IN_PARTY_MODE_TO_ENTER");
				return true;
			}

			if ((objN == 175 || objN == 163 || objN == 180 || objN == 178 || objN == 210) &&
					(obj->quality < 1 || obj->quality > (int)ARRAYSIZE(MAP_ENTRANCES)))
				continue;

			Common::Array<Obj *> transferObjs;
			if (objN == 175) { // Mine entry
				getObjsAt(playerLoc.x, wrapCoord(playerLoc.y - 1, playerLoc.z), playerLoc.z, transferObjs);

				const MapCoord &entrance = MAP_ENTRANCES[obj->quality - 1];
				for (uint j = 0; j < transferObjs.size(); ++j)
					setObjLocation(transferObjs[j], entrance.x, entrance.y - 1, entrance.z);
			} else if (objN == 163) { // Mine exit
				getObjsAt(playerLoc.x, wrapCoord(playerLoc.y + 1, playerLoc.z), playerLoc.z, transferObjs);

				const MapCoord &entrance = MAP_ENTRANCES[obj->quality - 1];
				for (uint j = 0; j < transferObjs.size(); ++j)
					setObjLocation(transferObjs[j], entrance.x, entrance.y + 2, entrance.z);
			} else if (objN == 197) {
				if (actorManager->get_actor(0x73)->get_flag(4)) {
					if (obj->quality == 0) {
						printl("NOTHING_APPEARS_TO_HAPPEN");
					} else if (obj->quality <= 4) {
						MapCoord podExit = POD_TELEPORTS[obj->quality - 1];
						advanceTime(0);
						MapCoord from = playerLoc;
						game->get_party()->walk(&from, &podExit);
					}
				} else {
					printl("THE_ALIEN_DEVICE_DOESNT_SEEM_TO_BE_FUNCTIONING");
				}
				return true;
			}

			advanceTime(0);
			MapCoord from = playerLoc;
			MapCoord entrance = MAP_ENTRANCES[obj->quality - 1];
			game->get_party()->walk(&from, &entrance);
		} else if (objN == 461) { // OBJ_DREAM_TELEPORTER
			// FIXME add logic here, including fading when there's an obelisk
			Actor *dreamActor = actorManager->get_actor(0);

			if ((obj->status & 0xe5) != 0xa5 || dreamActor->get_hp() > 4) {
				if ((obj->status & 0xe5) == 0) {
					player->move(obj->quality, obj->qty, playerLoc.z, false);
					advanceTime(0);
				} else {
					dreamworldCleanupState(obj);
				}
			}
		} else if (objN == 462) { // OBJ_DREAM_TELEPORTER1, walk on walls
			Actor *dreamActor = actorManager->get_actor(0);

			if (obj->quality == 0) {
				dreamActor->set_frame_n(dreamActor->get_old_frame_n());
				dreamActor->set_obj_n(dreamActor->get_base_obj_n());
			} else {
				dreamActor->set_frame_n(0);
				dreamActor->set_obj_n(player->get_gender() == 0 ? 318 : 319);
			}
		}
		// FIXME OBJ 465, the direction control modifier
	}

	return true;
}

bool MDScripts::canMoveDrill(Obj *drill, int relX, int relY) {
	if (canMoveObj(drill, relX, relY))
		return true;

	const uint16 OBJ_MINE_ENTRANCE = 175;
	uint8 z = drill->z;
	uint16 x = wrapCoord(drill->x + relX, z);
	uint16 y = wrapCoord(drill->y + relY, z);

	if (!mapGetObj(x, y, z, OBJ_MINE_ENTRANCE, true))
		return false;

	Common::Array<Obj *> objs;
	getObjsAt(x, y, z, objs);

	for (uint i = 0; i < objs.size(); ++i) {
		if (!isBlood(objs[i]->obj_n))
			return false;
	}

	return true;
}

Target MDScripts::weaponSelect() {
	Actor *player = Game::get_game()->get_player()->get_actor();
	Common::Array<Obj *> inventory;
	getInventoryObjs(player, false, inventory);

	_playerReadiedWeapons.clear();
	for (uint i = 0; i < inventory.size(); ++i) {
		if (inventory[i]->is_readied() && getWeaponDamage(Target(inventory[i])) > 0)
			_playerReadiedWeapons.push_back(inventory[i]);
	}

	if (_playerReadiedWeapons.empty()) {
		printl("ATTACK_WITH_BARE_HANDS");
		return Target(player);
	}

	Obj *weapon = _playerReadiedWeapons.front();
	_playerReadiedWeapons.remove_at(0);
	printfl("ATTACK_WITH_WEAPON", Target(weapon).name().c_str());
	return Target(weapon);
}

Obj *MDScripts::selectNextWeapon() {
	if (_playerReadiedWeapons.empty())
		return nullptr;

	Obj *weapon = _playerReadiedWeapons.front();
	_playerReadiedWeapons.remove_at(0);
	print("\n");
	printfl("ATTACK_WITH_WEAPON", Target(weapon).name().c_str());
	return weapon;
}

void MDScripts::selectAttackTarget(const MapCoord &targetLoc) {
	Game *game = Game::get_game();

	Actor *actor = mapGetActor(targetLoc.x, targetLoc.y, targetLoc.z);
	if (actor) {
		_selectedTarget = Target(actor);
	} else {
		Obj *obj = mapGetObj(targetLoc.x, targetLoc.y, targetLoc.z);
		_selectedTarget = obj ? Target(obj) : Target();
	}

	Common::String name;
	if (!_selectedTarget.isNull()) {
		name = _selectedTarget.name();
	} else {
		const Tile *tile = game->get_game_map()->get_tile(targetLoc.x, targetLoc.y, targetLoc.z, false);
		if (tile)
			name = tileDescription(tile->tile_num);
	}

	print(name + ".\n");
}

void MDScripts::playerAttackWithWeapon(const Target &weapon, const MapCoord &targetLoc) {
	Actor *player = Game::get_game()->get_player()->get_actor();

	if (!weapon.isActor() && outOfAmmo(player, weapon._obj, true))
		return;

	if (weapon.objN() == 313) // OBJ_M60_MACHINE_GUN
		return; // FIXME machine gun logic

	int result = attackTargetWithWeapon(player, targetLoc.x, targetLoc.y, weapon);
	if (result == ATTACK_OUT_OF_RANGE) {
		printl("OUT_OF_RANGE");
		playMdSfx(5);
	} else if (result == ATTACK_NOT_POSSIBLE) {
		printl("THAT_IS_NOT_POSSIBLE");
		playMdSfx(5);
	}
}

} // End of namespace MD
} // End of namespace Scripts
} // End of namespace Ultima6
} // End of namespace Ultima
