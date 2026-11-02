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
#include "ultima/ultima6/core/events.h"
#include "ultima/ultima6/core/game.h"
#include "ultima/ultima6/core/obj_manager.h"
#include "ultima/ultima6/core/party.h"
#include "ultima/ultima6/core/player.h"
#include "ultima/ultima6/core/tile_manager.h"
#include "ultima/ultima6/files/nuvie_io.h"
#include "ultima/ultima6/gui/widgets/map_window.h"
#include "ultima/ultima6/script/common/script_random.h"
#include "ultima/ultima6/script/script.h"
#include "ultima/ultima6/script/u6/u6_scripts.h"
#include "ultima/ultima6/sound/sound_manager.h"

namespace Ultima {
namespace Ultima6 {
namespace Scripts {
namespace U6 {

namespace {

const uint16 OBJLIST_OFFSET_VANISH_OBJ = 0x1c13;
const uint16 OBJLIST_OFFSET_MOONSTONES = 0x1c1b;
const uint16 OBJLIST_OFFSET_KEG_TIMER = 0x1c4b;

const uint16 OBJ_TELESCOPE = 154;
const uint16 OBJ_SILVER_HORN = 313;
const uint16 OBJ_KEG = 223;
const uint16 OBJ_MOONGATE = 0x55;
const uint16 OBJ_SILVER_SERPENT = 413;

const char *const DIRECTION_NAMES[8] = {
	"North", "East", "South", "West", "Northeast", "Southeast", "Southwest", "Northwest"
};

// Tile numbers of objects that can be readied
const uint16 READIABLE_TILES[] = {
	0x200, 0x201, 0x202, 0x203, 0x204, 0x205, 0x206, 0x207,
	0x219, 0x250, 0x251, 0x252, 0x217, 0x101,
	0x220, 0x221, 0x223, 0x224, 0x225, 0x226, 0x227, 0x22A, 0x22F, 0x230, 0x238, 0x254, 0x256, 0x255, 0x259,
	0x262, 0x263, 0x264, 0x270, 0x271, 0x272, 0x273, 0x274, 0x275, 0x279, 0x27D, 0x27E, 0x27F, 0x280, 0x281,
	0x28C, 0x28E, 0x29D, 0x2A2, 0x2A3, 0x2B9,
	0x210, 0x211, 0x212, 0x213, 0x214, 0x215, 0x216, 0x218, 0x257,
	0x208, 0x209, 0x20a, 0x20b, 0x20c, 0x20d, 0x20e, 0x20f, 0x222,
	0x21a, 0x21b,
	0x228, 0x229, 0x231, 0x235, 0x22b, 0x22c, 0x22d, 0x22e,
	0x258, 0x37d, 0x37e, 0x37f
};

} // End of anonymous namespace

/**
 * The telescope is the only use code that has to wait for input
 */
class U6Scripts::UseThread : public Continuation {
private:
	U6Scripts *_scripts;
	Obj *_telescope;
	Actor *_actor;

protected:
	uint8 run() override {
		Game *game = Game::get_game();
		MapWindow *mapWindow = game->get_map_window();

		if (_step == 0) {
			_scripts->print("\n");

			if (!_telescope->is_on_map()) {
				// FIXME this should probably work in the inventory
				_scripts->print("Not usable\n");
				return NUVIE_SCRIPT_FINISHED;
			}

			mapWindow->centerMap(_telescope->x, _telescope->y, _telescope->z);
			_scripts->print("Direction-");
			game->get_event()->do_not_show_target_cursor = true;
			return wait(NUVIE_SCRIPT_GET_DIRECTION, 1);
		}

		if (_resumedWithNil || _direction == NUVIE_DIR_NONE) {
			mapWindow->centerMap(_actor->get_x(), _actor->get_y(), _actor->get_z());
			_scripts->print("nowhere.\n");
			return NUVIE_SCRIPT_FINISHED;
		}

		_scripts->print(Common::String(DIRECTION_NAMES[_direction]) + ".\n");

		int x = mapWindow->get_cur_x();
		int y = mapWindow->get_cur_y();
		uint8 z;
		mapWindow->get_level(&z);

		// FIXME need to fade out the blacking
		mapWindow->set_enable_blacking(false);

		for (int i = 0; i <= 40; ++i) {
			x += dirX(_direction);
			y += dirY(_direction);
			mapWindow->moveMap(x, y, z);
			mapWindow->set_enable_blacking(false);
			_scripts->wait(50);
		}

		_scripts->wait(600);

		mapWindow->set_enable_blacking(true);

		mapWindow->centerMap(_actor->get_x(), _actor->get_y(), _actor->get_z());
		_scripts->print("\nDone\n");
		return NUVIE_SCRIPT_FINISHED;
	}

public:
	UseThread(U6Scripts *scripts, Obj *telescope, Actor *actor) : _scripts(scripts), _telescope(telescope), _actor(actor) {}
};

U6Scripts::U6Scripts(Configuration *config) : GameScripts(config) {
	static const int MOONSTONES[8][3] = {
		{ 0x3A7, 0x106, 0 }, { 0x1F7, 0x166, 0 }, { 0x9F, 0x3AE, 0 }, { 0x127, 0x26, 0 },
		{ 0x33F, 0x0A6, 0 }, { 0x147, 0x336, 0 }, { 0x17, 0x16, 1 }, { 0x397, 0x3A6, 0 }
	};

	for (int i = 0; i < 8; ++i)
		_moonstones[i] = MapCoord(MOONSTONES[i][0], MOONSTONES[i][1], MOONSTONES[i][2]);

	config->value("config/ultima6/show_stealing", _showStealing, false);
}

bool U6Scripts::loadGame(NuvieIO *objlist) {
	objlist->seek(OBJLIST_OFFSET_VANISH_OBJ);
	uint16 objData = objlist->read2();

	_vanishObjN = objData;
	_vanishFrameN = 0;
	if (objData > 1023) {
		_vanishFrameN = objData - 1023;
		_vanishObjN = objData - _vanishFrameN;
	}

	// Load the moonstone locations
	objlist->seek(OBJLIST_OFFSET_MOONSTONES);
	for (int i = 0; i < 8; ++i) {
		uint16 x = objlist->read2();
		uint16 y = objlist->read2();
		uint16 z = objlist->read2();
		_moonstones[i] = MapCoord(x, y, z);
	}

	objlist->seek(OBJLIST_OFFSET_KEG_TIMER);
	_kegTimer = objlist->read2();
	setArmageddon(false);
	_avatarDied = false;

	return true;
}

bool U6Scripts::saveGame(NuvieIO *objlist) {
	objlist->seek(OBJLIST_OFFSET_VANISH_OBJ);

	uint16 objData = _vanishObjN;
	if (_vanishFrameN > 0)
		objData += _vanishFrameN + 1023;

	objlist->write2(objData);

	objlist->seek(OBJLIST_OFFSET_MOONSTONES);
	for (int i = 0; i < 8; ++i) {
		objlist->write2(_moonstones[i].x);
		objlist->write2(_moonstones[i].y);
		objlist->write2(_moonstones[i].z);
	}

	objlist->seek(OBJLIST_OFFSET_KEG_TIMER);
	objlist->write2(_kegTimer);

	return true;
}

void U6Scripts::playSfx(uint16 sfxId, bool async) const {
	Script::get_script()->get_sound_manager()->playSfx(sfxId, async ? SFX_PLAY_ASYNC : SFX_PLAY_SYNC);
}

void U6Scripts::playerSubtractKarma(int k) {
	Player *player = Game::get_game()->get_player();
	player->set_karma(MAX<int>(player->get_karma() - k, 0));
}

void U6Scripts::partyHeal() {
	Common::Array<Actor *> members;
	getPartyMembers(members);

	for (uint i = 0; i < members.size(); ++i) {
		Actor *actor = members[i];
		actor->set_asleep(false);
		actor->set_poisoned(false);
		actor->set_paralyzed(false);
		actorRemoveCharm(actor);
		actor->set_hp(actor->get_maxhp());
	}
}

void U6Scripts::explosion(uint16 tileNum, int x, int y, Common::Array<MapEntity> &hitEntities) {
	playSfx(SFX_EXPLOSION);
	explosionStart(tileNum, x, y, hitEntities);
}

void U6Scripts::projectile(uint16 tileNum, int startX, int startY, int endX, int endY, int speed, int spin) {
	int rotateOffset = 0;
	int srcTileYOffset = 0;

	if (tileNum == 547) { // spear
		rotateOffset = 45;
	} else if (tileNum == 566) { // bow
		rotateOffset = 90;
		srcTileYOffset = 4;
	} else if (tileNum == 567) { // crossbow
		rotateOffset = 90;
		srcTileYOffset = 3;
	}

	playSfx(SFX_MISSLE);
	projectileAnim(tileNum, startX, startY, endX, endY, speed, false, rotateOffset, spin, srcTileYOffset);
}

void U6Scripts::fadeObjBlue(Obj *obj) {
	AsyncEffect *effect = new AsyncEffect(new TileBlackFadeEffect(obj, FADE_COLOR_BLUE, 20));
	effect->run();
}

void U6Scripts::fadeActorBlue(Actor *actor) {
	AsyncEffect *effect = new AsyncEffect(new TileBlackFadeEffect(actor, FADE_COLOR_BLUE, 20));
	effect->run();
}

void U6Scripts::fadeCasting(Actor *caster) {
	AsyncEffect *effect = new AsyncEffect(new TileBlackFadeEffect(caster, FADE_COLOR_RED, 20));
	effect->run();
}

void U6Scripts::fadeTile(int x, int y, int z, int fromTile, int toTile) {
	TileManager *tileManager = Game::get_game()->get_tile_manager();
	Tile *from = (fromTile >= 0) ? tileManager->get_tile(fromTile) : nullptr;
	Tile *to = (toTile >= 0) ? tileManager->get_tile(toTile) : nullptr;

	AsyncEffect *effect = new AsyncEffect(new TileFadeEffect(MapCoord(x, y, z), from, to, FADE_PIXELATED, 10));
	effect->run();
}

void U6Scripts::fadeObjOut(Obj *obj) {
	obj->set_invisible(true);
	fadeTile(obj->x, obj->y, obj->z, objTileNum(obj), -1);
	obj->set_invisible(false);
}

void U6Scripts::fadeObjIn(Obj *obj) {
	obj->set_invisible(true);
	fadeTile(obj->x, obj->y, obj->z, -1, objTileNum(obj));
	obj->set_invisible(false);
}

void U6Scripts::fadeActorIn(Actor *actor) {
	uint16 newTileNum = actor->get_tile()->tile_num;
	actor->set_invisible(true);
	fadeTile(actor->get_x(), actor->get_y(), actor->get_z(), -1, newTileNum);
	actor->set_invisible(false);
}

bool U6Scripts::isReadiableTile(uint16 tileNum) {
	for (uint i = 0; i < ARRAYSIZE(READIABLE_TILES); ++i) {
		if (READIABLE_TILES[i] == tileNum)
			return true;
	}

	return false;
}

void U6Scripts::setArmageddon(bool value) {
	_armageddon = value;
	Game::get_game()->set_armageddon(value);
}

bool U6Scripts::moonstoneSetLoc(uint8 phase, MapCoord location) {
	if (phase < 1 || phase > 8)
		return false;

	_moonstones[phase - 1] = location;
	return true;
}

MapCoord U6Scripts::moonstoneGetLoc(uint8 phase) {
	if (phase < 1 || phase > 8)
		return MapCoord(0, 0, 0);

	return _moonstones[phase - 1];
}

bool U6Scripts::updateMoongates(bool visible) {
	ObjManager *objManager = Game::get_game()->get_obj_manager();

	for (int i = 0; i < 8; ++i) {
		const MapCoord &loc = _moonstones[i];
		Obj *moongate = mapGetObj(loc.x, loc.y, loc.z, OBJ_MOONGATE);

		if (visible) {
			if (!moongate && loc.x != 0) {
				moongate = newObj(OBJ_MOONGATE);
				moongate->frame_n = 1;
				if (!objManager->moveto_map(moongate, loc))
					delete_obj(moongate);
			}
		} else if (moongate) {
			removeFromEngine(moongate);
		}
	}

	return true;
}

bool U6Scripts::useKeg(Obj *obj) {
	if (obj->frame_n != 0) {
		print("\nNo effect\n");
		return true;
	}

	if (_kegTimer > 0) {
		print("\nNot now\n");
	} else {
		obj->frame_n = 1;
		print("\nPowder lit!\n");
		_kegTimer = 3;
	}

	return true;
}

void U6Scripts::explodeKeg() {
	// Try to find a lit keg on the current map
	uint8 level = Game::get_game()->get_player()->get_actor()->get_z();
	Common::Array<Obj *> kegs;
	findObjs(level, OBJ_KEG, kegs);

	for (uint i = 0; i < kegs.size(); ++i) {
		if (kegs[i]->frame_n == 1)
			explodeObj(kegs[i]);
	}

	// Try to explode lit kegs in the party's inventory
	Common::Array<Actor *> members;
	getPartyMembers(members);

	for (uint i = 0; i < members.size(); ++i) {
		Obj *keg = members[i]->inventory_get_object(OBJ_KEG, 0, false, 1, true);
		if (keg)
			explodeObj(keg, members[i]);
	}
}

void U6Scripts::explodeObj(Obj *obj, Actor *actor) {
	int x, y, z;

	if (actor) {
		x = actor->get_x();
		y = actor->get_y();
		z = actor->get_z();
	} else {
		x = obj->x;
		y = obj->y;
		z = obj->z;
	}

	removeFromEngine(obj);

	Common::Array<MapEntity> hitItems;
	explosion(0x189, x, y, hitItems);

	for (uint i = 0; i < hitItems.size(); ++i) {
		if (hitItems[i].entity_type == ENT_ACTOR)
			actorHitTarget(Target(hitItems[i].actor), randomRange(1, 0x3c), nullptr);

		if (_avatarDied) {
			explodeSurroundingObjects(x, y, z);
			actorAvatarDeath();
			return; // Don't keep exploding once the avatar is dead
		}
	}

	explodeSurroundingObjects(x, y, z);
}

void U6Scripts::explodeSurroundingObjects(int x, int y, int z) {
	// Blow up doors and other kegs
	for (int ox = x - 2; ox <= x + 2; ++ox) {
		for (int oy = y - 2; oy <= y + 2; ++oy) {
			Obj *mapObj = mapGetObj(ox, oy, z, OBJ_KEG);
			if (mapObj)
				explodeObj(mapObj);

			mapObj = mapGetObj(ox, oy, z, 0x12c); // steel door
			if (!mapObj || mapObj->frame_n == 0xc) {
				mapObj = mapGetObj(ox, oy, z, 0x129); // oaken door
				if (!mapObj || mapObj->frame_n == 0xc) {
					mapObj = mapGetObj(ox, oy, z, 0x12a); // windowed door
					if (!mapObj || mapObj->frame_n == 0xc)
						mapObj = mapGetObj(ox, oy, z, 0x12b); // cedar door
				}
			}

			if (mapObj && mapObj->frame_n <= 0xc) {
				removeFromEngine(mapObj);
				print("\nThe door is blown up!\n");
			}
		}
	}
}

bool U6Scripts::lookObj(Obj *obj) {
	Game *game = Game::get_game();
	ObjManager *objManager = game->get_obj_manager();

	print(Common::String("Thou dost see ") + objManager->look_obj(obj, true));

	float weight = floorf(objManager->get_obj_weight(obj, OBJ_WEIGHT_INCLUDE_CONTAINER_ITEMS, OBJ_WEIGHT_DONT_SCALE)) / 10;
	if (weight != 0) {
		if (obj->qty > 1 && objManager->is_stackable(obj))
			print(". They weigh");
		else
			print(". It weighs");

		print(Common::String::format(" %.1f stones", weight));
	}

	if (game->get_usecode()->look_obj(obj, game->get_player()->get_actor())) {
		print("\n");
		return false;
	}

	int dmg = findObjValue(WEAPON_DAMAGE, WEAPON_DAMAGE_COUNT, obj->obj_n, -1);
	if (dmg != -1) {
		print(weight != 0 ? " and" : ". It");
		print(Common::String::format(" can do %d point", dmg));
		if (dmg > 1)
			print("s");
		print(" of damage");
	}

	int ac = findObjValue(ARMOUR, ARMOUR_COUNT, obj->obj_n, -1);
	if (ac != -1) {
		// The original only tested the damage against 0, so a missing damage counts as a weapon here
		print((weight != 0 || dmg != 0) ? " and" : ". It");
		print(Common::String::format(" can absorb %d point", ac));
		if (ac > 1)
			print("s");
		print(" of damage");
	}

	print(".\n");

	MapCoord playerLoc = game->get_player()->get_actor()->get_location();
	if (_showStealing && objManager->can_get_obj(obj) && playerLoc.z == 0 && !obj->is_ok_to_take()) {
		if (ABS((int)playerLoc.x - (int)obj->x) > 1 || ABS((int)playerLoc.y - (int)obj->y) > 1)
			print("PRIVATE PROPERTY\n");
		else
			print("PRIVATE PROPERTY");
	}

	return true;
}

bool U6Scripts::hasUsecode(Obj *obj, UseCodeEvent usecodeType) {
	return usecodeType == USE_EVENT_USE && (obj->obj_n == OBJ_TELESCOPE || obj->obj_n == OBJ_SILVER_HORN);
}

ScriptThread *U6Scripts::useObj(Obj *obj, Actor *actor) {
	if (obj->obj_n == OBJ_TELESCOPE)
		return new UseThread(this, obj, actor);

	if (obj->obj_n == OBJ_SILVER_HORN) {
		print("\n");
		useSilverHorn(actor);
	}

	return nullptr;
}

void U6Scripts::useSilverHorn(Actor *actor) {
	for (int i = 0; i <= 0xff; ++i) {
		Actor *other = getActor(i);

		if (other->get_obj_n() == OBJ_SILVER_SERPENT && other->is_alive() &&
				actorFindMaxXYDistance(other, actor->get_x(), actor->get_y()) <= 8) {
			print("Not now!\n");
			return;
		}
	}

	for (int i = 0; i < 3; ++i) {
		int newX = randomRange(1, 7) + randomRange(1, 7) + actor->get_x() - 8;
		int newY = randomRange(1, 7) + randomRange(1, 7) + actor->get_y() - 8;
		newTempActor(OBJ_SILVER_SERPENT, newX, newY, actor->get_z(), ACTOR_ALIGNMENT_CHAOTIC);
	}

	print("Silver snakes are generated!\n");
}

bool U6Scripts::playerPass() {
	print("Pass!\n");
	return true;
}

bool U6Scripts::moveObj(Obj *obj, sint16 relX, sint16 relY) {
	return false;
}

} // End of namespace U6
} // End of namespace Scripts
} // End of namespace Ultima6
} // End of namespace Ultima
