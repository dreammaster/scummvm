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
#include "ultima/ultima6/core/obj_manager.h"
#include "ultima/ultima6/core/tile_manager.h"
#include "ultima/ultima6/script/se/se_scripts.h"

namespace Ultima {
namespace Ultima6 {
namespace Scripts {
namespace SE {

// Keyed by tile number rather than object number
const SEScripts::ObjValue SEScripts::READIABLE_LOCATIONS[] = {
	// Neck
	{ 630, 1 }, { 631, 1 }, { 696, 1 },
	// One handed
	{ 512, 3 }, { 541, 3 }, { 545, 3 }, { 546, 3 }, { 547, 3 }, { 548, 3 }, { 549, 3 },
	{ 550, 3 }, { 554, 3 }, { 558, 3 }, { 560, 3 }, { 565, 3 }, { 574, 3 }, { 677, 3 },
	{ 700, 3 }, { 701, 3 }, { 900, 3 }, { 901, 3 }, { 1028, 3 }, { 1029, 3 },
	// Torso
	{ 518, 2 }, { 519, 2 }, { 520, 2 },
	// Shield hand
	{ 513, 3 }, { 524, 3 }, { 525, 3 }, { 526, 3 }, { 606, 3 },
	// Two handed. The guns (561, 571, 572) can't be equipped by natives in the original
	{ 552, 4 }, { 553, 4 }, { 561, 4 }, { 571, 4 }, { 572, 4 }, { 640, 4 }, { 676, 4 },
	{ 689, 4 }, { 692, 4 }, { 904, 4 },
	// Finger
	{ 600, 9 }
};

const SEScripts::ObjValue SEScripts::WEAPON_RANGES[] = {
	{ 1, 5 },   // Spear of Shamap
	{ 3, 4 },   // Boomerang
	{ 21, 4 },  // Black staff, only ranged when Myrmidex are near in the original
	{ 26, 6 },  // Spear
	{ 27, 4 },  // Throwing axe
	{ 31, 5 },  // Bow
	{ 32, 4 },  // Blowgun
	{ 37, 7 },  // Atl atl
	{ 40, 7 },  // Modern rifle
	{ 44, 4 },  // Knife
	{ 50, 7 },  // Bamboo flintlock
	{ 138, 6 }, // Grenade
	{ 139, 6 }, // Lit grenade
	{ 240, 6 }, // Device
	{ 241, 6 }  // Activated device
};

SEScripts::SEScripts(Configuration *config) : GameScripts(config) {
	initLang();
}

const GameScripts::ObjValue *SEScripts::weaponRanges(uint &count) const {
	count = ARRAYSIZE(WEAPON_RANGES);
	return WEAPON_RANGES;
}

uint16 SEScripts::getTileToObjectMapping(uint16 tileN) {
	switch (tileN) {
	// Trees
	case 50: case 51: case 64: case 65: case 66: case 67: case 68: case 69: case 70:
	case 71: case 72: case 130: case 179: case 180: case 192: case 193: case 194:
	case 195: case 198:
		return 5000;
	// Oven fire
	case 16: case 17: case 18: case 19: case 20: case 21: case 22: case 23: case 24:
	case 25: case 26: case 27: case 170: case 171:
		return 5001;
	// Yucca plant
	case 52:
		return 5002;
	default:
		return 0;
	}
}

bool SEScripts::isTileObject(uint16 objN) {
	return objN >= 5000 && objN <= 5002;
}

bool SEScripts::isContainerObj(uint16 objN) {
	switch (objN) {
	case 59: case 60: case 97: case 182: case 183: case 184:
		return true;
	default:
		return false;
	}
}

int SEScripts::objGetReadiableLocation(Obj *obj) {
	ObjManager *objManager = Game::get_game()->get_obj_manager();
	uint16 tileNum = objManager->get_obj_tile(obj->obj_n, obj->frame_n)->tile_num;

	return findObjValue(READIABLE_LOCATIONS, ARRAYSIZE(READIABLE_LOCATIONS), tileNum, -1);
}

bool SEScripts::talkToObj(Obj *obj) {
	printl("NOTHING");
	return false;
}

void SEScripts::search(Obj *obj) {
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

} // End of namespace SE
} // End of namespace Scripts
} // End of namespace Ultima6
} // End of namespace Ultima
