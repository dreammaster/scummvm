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
#include "ultima/ultima6/core/tile_manager.h"
#include "ultima/ultima6/files/nuvie_io.h"
#include "ultima/ultima6/script/md/md_scripts.h"
#include "ultima/ultima6/views/inventory_view.h"
#include "ultima/ultima6/views/view_manager.h"

namespace Ultima {
namespace Ultima6 {
namespace Scripts {
namespace MD {

static const uint16 OBJLIST_OFFSET_HOURS_TILL_NEXT_HEALING = 0x1cf2;
static const uint16 OBJLIST_OFFSET_PREV_PLAYER_X = 0x1d03;
static const uint16 OBJLIST_OFFSET_PREV_PLAYER_Y = 0x1d04;
static const uint16 OBJLIST_OFFSET_1D22_UNK = 0x1d22;
static const uint16 OBJLIST_OFFSET_DREAM_MODE_FLAG = 0x1d29;
static const uint16 OBJLIST_OFFSET_DREAM_STAGE = 0x1d53;

static const uint16 DREAM_MODE_BIT = 0x10;

uint16 Target::x() const {
	return _actor ? _actor->get_x() : _obj->x;
}

uint16 Target::y() const {
	return _actor ? _actor->get_y() : _obj->y;
}

uint8 Target::z() const {
	return _actor ? _actor->get_z() : _obj->z;
}

uint16 Target::objN() const {
	return _actor ? _actor->get_obj_n() : _obj->obj_n;
}

uint16 Target::frameN() const {
	return _actor ? _actor->get_frame_n() : _obj->frame_n;
}

Common::String Target::name() const {
	if (_actor)
		return _actor->get_name();

	return Game::get_game()->get_obj_manager()->get_obj_name(_obj->obj_n, _obj->frame_n);
}

MDScripts::MDScripts(Configuration *config) : GameScripts(config) {
	initLang();
}

const GameScripts::ObjValue *MDScripts::weaponRanges(uint &count) const {
	count = WEAPON_RANGES_COUNT;
	return WEAPON_RANGES;
}

bool MDScripts::loadGame(NuvieIO *objlist) {
	_selectedTarget = Target();
	_attackTarget = Target();

	objlist->seek(OBJLIST_OFFSET_HOURS_TILL_NEXT_HEALING);
	_hoursTillNextHealing = objlist->read1();

	objlist->seek(OBJLIST_OFFSET_DREAM_MODE_FLAG);
	_inDreamMode = (objlist->read2() & DREAM_MODE_BIT) != 0;

	Game *game = Game::get_game();
	game->get_actor_manager()->enable_temp_actor_cleaning(!_inDreamMode);

	if (_inDreamMode) {
		InventoryView *view = game->get_view_manager()->get_inventory_view();
		view->set_actor(game->get_actor_manager()->get_actor(0), true);
		view->lock_to_actor(true);
		game->get_view_manager()->set_inventory_mode();
	}

	objlist->seek(OBJLIST_OFFSET_DREAM_STAGE);
	_currentDreamStage = objlist->read2();

	objlist->seek(OBJLIST_OFFSET_PREV_PLAYER_X);
	_prevPlayerX = objlist->read1();

	objlist->seek(OBJLIST_OFFSET_PREV_PLAYER_Y);
	_prevPlayerY = objlist->read1();

	objlist->seek(OBJLIST_OFFSET_1D22_UNK);
	_objlist1d22Unk = objlist->read1();

	updateWatchTile();
	return true;
}

bool MDScripts::saveGame(NuvieIO *objlist) {
	objlist->seek(OBJLIST_OFFSET_HOURS_TILL_NEXT_HEALING);
	objlist->write1(_hoursTillNextHealing);

	objlist->seek(OBJLIST_OFFSET_DREAM_MODE_FLAG);
	uint16 flags = objlist->read2();
	if (_inDreamMode)
		flags |= DREAM_MODE_BIT;
	else
		flags &= 0xFFEF;
	objlist->seek(OBJLIST_OFFSET_DREAM_MODE_FLAG);
	objlist->write2(flags);

	objlist->seek(OBJLIST_OFFSET_DREAM_STAGE);
	objlist->write2(_currentDreamStage);

	objlist->seek(OBJLIST_OFFSET_PREV_PLAYER_X);
	objlist->write1(_prevPlayerX);

	objlist->seek(OBJLIST_OFFSET_PREV_PLAYER_Y);
	objlist->write1(_prevPlayerY);

	objlist->seek(OBJLIST_OFFSET_1D22_UNK);
	objlist->write1(_objlist1d22Unk);

	return true;
}

void MDScripts::updateWatchTile() {
	const uint16 WATCH_TILE = 616;
	TileManager *tileManager = Game::get_game()->get_tile_manager();

	for (uint i = 0; i < tileManager->get_number_of_animations(); ++i) {
		if (tileManager->get_anim_tile(i) == WATCH_TILE) {
			tileManager->set_anim_first_frame(i, 416 + Game::get_game()->get_clock()->get_hour() % 12);
			return;
		}
	}
}

bool MDScripts::isBlood(uint16 objN) {
	// OBJ_BLOOD, OBJ_ICHOR, OBJ_SAP
	return objN == 334 || objN == 335 || objN == 336;
}

bool MDScripts::isContainerObj(uint16 objN) {
	switch (objN) {
	case 80: case 81: case 82: case 83: case 85: case 86: case 87: case 89:
	case 104: case 139: case 257: case 284: case 285: case 304: case 341:
		return true;
	default:
		return false;
	}
}

bool MDScripts::isObjAttackable(const Obj *obj) {
	// OBJ_CLOSED_DOOR, OBJ_GLOW_WORM, OBJ_DEVIL_POD, OBJ_DEVIL_PLANT, OBJ_GLASS_PITCHER
	switch (obj->obj_n) {
	case 0xB3: case 0x184: case 0x133: case 0x12C: case 0xD9:
		return true;
	default:
		return false;
	}
}

bool MDScripts::isPlantObj(const Obj *obj) {
	// OBJ_VINE, OBJ_TREE, OBJ_PORCUPOD, OBJ_FLOWER, OBJ_HARD_SHELLED_PLANT,
	// OBJ_DEVIL_PLANT, OBJ_DEVIL_POD, OBJ_HEDGE
	switch (obj->obj_n) {
	case 0xCD: case 0x198: case 0xA8: case 0xAA: case 0xAB: case 0x12C: case 0x133: case 0x12B:
		return true;
	default:
		return false;
	}
}

bool MDScripts::isObjBurnable(const Obj *obj) {
	switch (obj->obj_n) {
	case 0x18: case 0x84: case 0xB9: case 0x3E: case 0x65: case 0x6B: case 0x94: case 0x96:
	case 0x97: case 0x9E: case 0xF3: case 0xFC: case 0x1BC: case 0x1BF: case 0x100:
		return true;
	default:
		return false;
	}
}

bool MDScripts::isOpenWaterAtLoc(uint16 x, uint16 y, uint8 z) {
	const Tile *tile = Game::get_game()->get_game_map()->get_tile(x, y, z, false);
	if (!tile)
		return false;

	uint16 tileNum = tile->tile_num;
	if (!tileGetFlag(tileNum, 1, 0) || !tileGetFlag(tileNum, 1, 1))
		return false;

	Common::Array<Obj *> objs;
	getObjsAt(x, y, z, objs);

	for (uint i = 0; i < objs.size(); ++i) {
		tileNum = objTileNum(objs[i]);
		// Support objects and force passable objects
		if (tileGetFlag(tileNum, 3, 1) || tileGetFlag(tileNum, 3, 2))
			return false;
	}

	return true;
}

void MDScripts::search(Obj *obj) {
	if (!obj->is_on_map())
		return;

	Common::Array<Obj *> children;
	getContainerObjs(obj, children);

	bool foundObj = false;
	Obj *prevObj = nullptr;
	ObjManager *objManager = Game::get_game()->get_obj_manager();

	for (uint i = 0; i < children.size(); ++i) {
		Obj *child = children[i];

		if (prevObj) {
			printfl("SEARCH_NEXT_OBJ", objManager->look_obj(prevObj, true));
			objManager->moveto_map(prevObj, MapCoord(obj->x, obj->y, obj->z));
		}

		if (i == 0) {
			foundObj = true;
			printfl("SEARCHING_HERE_YOU_FIND", objManager->look_obj(child, true));
			objManager->moveto_map(child, MapCoord(obj->x, obj->y, obj->z));
		} else {
			prevObj = child;
		}

		wait(50);
	}

	if (prevObj) {
		printfl("SEARCH_LAST_OBJ", objManager->look_obj(prevObj, true));
		objManager->moveto_map(prevObj, MapCoord(obj->x, obj->y, obj->z));
	}

	if (!foundObj)
		printl("SEARCHING_HERE_YOU_FIND_NOTHING");
	else
		print(".\n");
}

int MDScripts::objGetReadiableLocation(Obj *obj) {
	return findObjValue(READIABLE_LOCATIONS, READIABLE_LOCATIONS_COUNT, objTileNum(obj), -1);
}

uint16 MDScripts::wrapCoord(int coord, uint8 level) {
	int mapStride = (level != 0) ? 256 : 1024;

	if (coord < 0)
		return mapStride + coord;

	return coord % mapStride;
}

bool MDScripts::canMoveObj(const Obj *obj, int relX, int relY) {
	uint8 z = obj->z;

	return Game::get_game()->get_game_map()->can_put_obj(wrapCoord(obj->x + relX, z), wrapCoord(obj->y + relY, z), z);
}

void MDScripts::updateLampPosts() {
	// Turn lamps on or off if there's power and it's dark
	const uint16 OBJ_LAMP_POST = 228;
	Game *game = Game::get_game();
	uint8 frameN = 3;
	uint8 hour = game->get_clock()->get_hour();

	if (game->get_actor_manager()->get_actor(0x73)->get_flag(4) && (hour < 6 || hour > 17))
		frameN = 7;

	uint8 level = game->get_player()->get_actor()->get_z();
	for (Obj *obj = game->get_obj_manager()->find_obj(level, OBJ_LAMP_POST, 0, OBJ_NOMATCH_QUALITY); obj;
			obj = game->get_obj_manager()->find_next_obj(level, obj, OBJ_NOMATCH_FRAME_N, OBJ_NOMATCH_QUALITY))
		obj->frame_n = frameN;
}

void MDScripts::playMdSfx(uint16 sfxId, bool async) {
}

void MDScripts::playDoorSfx() {
}

bool MDScripts::createObjectNeedsQuan(uint16 objN) {
	// OBJ_LEVER, OBJ_SWITCH, OBJ_SWITCH1
	return objN == 196 || objN == 311 || objN == 312;
}

int MDScripts::inputSelectObjQty(Obj *obj) {
	if (!Game::get_game()->get_obj_manager()->is_stackable(obj))
		return 1;

	printl("HOW_MANY");
	int qty = (int)strtol(inputSelect("0123456789", true).c_str(), nullptr, 10);

	return MIN<int>(qty, obj->qty);
}

} // End of namespace MD
} // End of namespace Scripts
} // End of namespace Ultima6
} // End of namespace Ultima
