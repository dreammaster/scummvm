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
#include "ultima/ultima6/core/obj_manager.h"
#include "ultima/ultima6/core/player.h"
#include "ultima/ultima6/script/md/md_scripts.h"

namespace Ultima {
namespace Ultima6 {
namespace Scripts {
namespace MD {

#define USE_TO(OBJ_N, FUNC) { OBJ_N, &MDScripts::FUNC }
#define USE_ON(OBJ_N, FUNC) { OBJ_N, &MDScripts::FUNC, nullptr, 0 }
#define USE_ON_TO(OBJ_N, TABLE) { OBJ_N, nullptr, TABLE, ARRAYSIZE(TABLE) }

// Use X on Y, to Z: the entries are the objects that can be used on, then to
const MDScripts::UseToEntry MDScripts::SLEDGE_HAMMER_TO[] = {
	USE_TO(405, useSledgeHammerOnReplacementTrackToBrokenTrack)
};

const MDScripts::UseToEntry MDScripts::PLIERS_TO[] = {
	USE_TO(201, usePliersOnSpoolToTower), // OBJ_TOWER_TOP
	USE_TO(215, usePliersOnSpoolToTower)  // OBJ_POWER_CABLE1
};

const MDScripts::UseToEntry MDScripts::SHOVEL_ON_PILE_TO[] = {
	USE_TO(257, useShovelOnPileToHole)
};

const MDScripts::UseToEntry MDScripts::SHOVEL_UNLOAD_TO[] = {
	USE_TO(233, useShovelToUnloadContainer), // OBJ_FURNACE
	USE_TO(257, useShovelToUnloadContainer), // OBJ_HOLE
	USE_TO(268, useShovelToUnloadContainer), // OBJ_MARTIAN_WHEEL_BARROW
	USE_TO(410, useShovelToUnloadContainer)  // OBJ_RAIL_CAR
};

const MDScripts::UseToEntry MDScripts::SHOVEL_ON_ORE_TO[] = {
	USE_TO(268, useShovelOnOreToContainer), // OBJ_MARTIAN_WHEEL_BARROW
	USE_TO(410, useShovelOnOreToContainer)  // OBJ_RAIL_CAR
};

const MDScripts::UseOnEntry MDScripts::SLEDGE_HAMMER_ON[] = {
	USE_ON_TO(404, SLEDGE_HAMMER_TO) // OBJ_REPLACEMENT_TRACK
};

const MDScripts::UseOnEntry MDScripts::PLIERS_ON[] = {
	USE_ON_TO(199, PLIERS_TO) // OBJ_CABLE_SPOOL
};

// Picks and hoes, for a hole in the ice or a hole
const MDScripts::UseOnEntry MDScripts::DIGGING_TOOL_ON[] = {
	USE_ON(255, useMiscText),
	USE_ON(257, useMiscText)
};

const MDScripts::UseOnEntry MDScripts::SHOVEL_ON[] = {
	USE_ON(255, useMiscText),
	USE_ON(257, useMiscText),
	USE_ON_TO(258, SHOVEL_ON_PILE_TO),
	USE_ON_TO(268, SHOVEL_UNLOAD_TO),
	USE_ON_TO(410, SHOVEL_UNLOAD_TO),
	USE_ON_TO(442, SHOVEL_ON_ORE_TO),
	USE_ON_TO(443, SHOVEL_ON_ORE_TO),
	USE_ON_TO(444, SHOVEL_ON_ORE_TO),
	USE_ON(0, useToolOnGround)
};

const MDScripts::UseOnEntry MDScripts::MANUSCRIPT_ON[] = {
	USE_ON(84, useManuscriptOnMailbox) // OBJ_MAILBOX
};

const MDScripts::UseOnEntry MDScripts::PRYBAR_ON[] = {
	USE_ON(86, useCrate),
	USE_ON(427, usePrybarOnHatch)
};

const MDScripts::UseOnEntry MDScripts::HAMMER_ON[] = {
	USE_ON(86, useCrate)
};

const MDScripts::UseOnEntry MDScripts::SPRAY_GUN_REFILL_ON[] = {
	USE_ON(129, useObjOnSprayGun), // OBJ_WEED_SPRAYER
	USE_ON(261, useObjOnSprayGun)  // OBJ_SPRAY_GUN
};

const MDScripts::UseOnEntry MDScripts::OIL_ON[] = {
	USE_ON(179, useOilOnDoor), // OBJ_CLOSED_DOOR
	USE_ON(227, useOilOnDoor), // OBJ_DOOR3
	USE_ON(421, useOilOnDoor), // OBJ_CLOSED_HATCH
	USE_ON(152, useOilOnDoor), // OBJ_DOOR
	USE_ON(219, useOilOnDoor), // OBJ_DOOR1
	USE_ON(222, useOilOnDoor), // OBJ_DOOR2
	USE_ON(0, useOilOnDreamDoor)
};

const MDScripts::UseOnEntry MDScripts::SPRAY_GUN_ON[] = {
	USE_ON(0, useSprayGun)
};

const MDScripts::UseOnEntry MDScripts::WRENCH_ON[] = {
	USE_ON(411, useWrenchOnSwitchbar),
	USE_ON(440, useWrenchOnDrill),
	USE_ON(458, useWrenchOnPanel)
};

const MDScripts::UseOnEntry MDScripts::REPAIRED_BELT_ON[] = {
	USE_ON(192, useFixedBeltOnBareRollers)
};

const MDScripts::UseOnEntry MDScripts::GONG_HAMMER_ON[] = {
	USE_ON(130, useHammerOnOxiumGeode), // OBJ_OXIUM_GEODE
	USE_ON(298, useGong)                // OBJ_GONG
};

const MDScripts::UseOnEntry MDScripts::HEADGEAR_ON[] = {
	USE_ON(288, useHeadGear), // OBJ_DREAM_MACHINE1
	USE_ON(289, useHeadGear)  // OBJ_DREAM_MACHINE2
};

const MDScripts::UseOnEntry MDScripts::HEART_STONE_ON[] = {
	USE_ON(287, useHeartstoneOnMetalWoman) // OBJ_METAL_WOMAN
};

const MDScripts::UseOnEntry MDScripts::RADIUM_BLOCK_ON[] = {
	USE_ON(290, useRadium), // OBJ_POWER_UNIT
	USE_ON(448, useRadium)  // OBJ_BLOCK_OF_RADIUM
};

const MDScripts::UseOnEntry MDScripts::RADIUM_CHIP_ON[] = {
	USE_ON(240, useRadium), // OBJ_HEAT_RAY_GUN
	USE_ON(241, useRadium)  // OBJ_FREEZE_RAY_GUN
};

#define USE_OBJ(OBJ_N, FUNC) { OBJ_N, &MDScripts::FUNC, nullptr, 0 }
#define USE_OBJ_ON(OBJ_N, TABLE) { OBJ_N, nullptr, TABLE, ARRAYSIZE(TABLE) }

const MDScripts::UseEntry MDScripts::USE_TABLE[] = {
	USE_OBJ(12, useRubySlippers),
	USE_OBJ(38, useReadyObj), // OBJ_RUBBER_GLOVES
	USE_OBJ_ON(52, SLEDGE_HAMMER_ON),
	USE_OBJ_ON(53, PLIERS_ON),
	USE_OBJ_ON(65, DIGGING_TOOL_ON), // OBJ_PICK
	USE_OBJ_ON(66, SHOVEL_ON), // OBJ_SHOVEL
	USE_OBJ_ON(67, DIGGING_TOOL_ON), // OBJ_HOE
	USE_OBJ(73, useBerry),
	USE_OBJ(74, useBerry),
	USE_OBJ(75, useBerry),
	USE_OBJ(76, useBerry),
	USE_OBJ(77, useRedBerry),
	USE_OBJ(78, useMiscTextOnly), // OBJ_CLUMP_OF_ROUGE_BERRIES
	USE_OBJ(86, useContainer),
	USE_OBJ(87, useContainer),
	USE_OBJ_ON(88, MANUSCRIPT_ON),
	USE_OBJ(96, useSextant),
	USE_OBJ_ON(102, PRYBAR_ON),
	USE_OBJ(104, useContainer),
	USE_OBJ(106, useTent), // OBJ_FOLDED_TENT
	USE_OBJ_ON(119, SPRAY_GUN_REFILL_ON), // OBJ_BOTTLE_OF_GREEN_PAINT
	USE_OBJ_ON(124, OIL_ON), // OBJ_CAN_OF_LAMP_OIL
	USE_OBJ_ON(128, SPRAY_GUN_REFILL_ON), // OBJ_WEED_KILLER
	USE_OBJ_ON(129, SPRAY_GUN_ON), // OBJ_WEED_SPRAYER
	USE_OBJ(131, useMiscTextOnly), // OBJ_BLOB_OF_OXIUM
	USE_OBJ_ON(135, WRENCH_ON), // OBJ_WRENCH
	USE_OBJ(136, useReadyObj), // OBJ_TONGS
	USE_OBJ_ON(144, REPAIRED_BELT_ON), // OBJ_REPAIRED_BELT
	USE_OBJ(147, useHandMirror),
	USE_OBJ(148, useReadingMaterial), // OBJ_BOOK
	USE_OBJ(151, useReadingMaterial), // OBJ_NOTE
	USE_OBJ(152, useDoor), // OBJ_DOOR
	USE_OBJ(181, useGate),
	USE_OBJ(184, useMiscTextOnly), // OBJ_CAMERA
	USE_OBJ(196, useSwitchDevice), // OBJ_LEVER
	USE_OBJ(199, useMiscTextOnly), // OBJ_CABLE_SPOOL
	USE_OBJ(212, useOxiumBin),
	USE_OBJ(222, useDoor),
	USE_OBJ_ON(231, GONG_HAMMER_ON), // OBJ_GONG_HAMMER
	USE_OBJ_ON(235, OIL_ON), // OBJ_POTASH, which is oil when frame_n == 4
	USE_OBJ(243, useReadingMaterial), // OBJ_SCROLL
	USE_OBJ_ON(261, SPRAY_GUN_ON), // OBJ_SPRAY_GUN
	USE_OBJ_ON(263, DIGGING_TOOL_ON), // OBJ_MARTIAN_HOE
	USE_OBJ_ON(267, SHOVEL_ON), // OBJ_MARTIAN_SHOVEL
	USE_OBJ_ON(273, HAMMER_ON),
	USE_OBJ(280, useMusicalInstrument), // OBJ_CYMBALS
	USE_OBJ(281, useMusicalInstrument), // OBJ_TAMBORINE
	USE_OBJ(282, useMusicalInstrument), // OBJ_DRUM
	USE_OBJ(283, useMusicalInstrument), // OBJ_ACCORDION
	USE_OBJ(284, useContainer),
	USE_OBJ(286, useSpittoon),
	USE_OBJ(288, useMiscTextOnly), // OBJ_DREAM_MACHINE1
	USE_OBJ(293, useMiscTextOnly), // OBJ_MARTIAN_CLOCK
	USE_OBJ_ON(296, HEADGEAR_ON), // OBJ_HEADGEAR
	USE_OBJ(311, useSwitchDevice), // OBJ_SWITCH
	USE_OBJ(312, useSwitchDevice), // OBJ_SWITCH1
	USE_OBJ(323, useMiscTextOnly), // OBJ_OXYGENATED_AIR_MACHINE
	USE_OBJ_ON(327, DIGGING_TOOL_ON), // OBJ_MARTIAN_PICK
	USE_OBJ(331, useDreamstuff),
	USE_OBJ(399, usePoolTable),
	USE_OBJ(401, useReadyObj), // OBJ_POOL_QUE
	USE_OBJ(411, useSwitchBar),
	USE_OBJ(421, useDoor), // OBJ_CLOSED_HATCH
	USE_OBJ_ON(426, HEART_STONE_ON), // OBJ_HEART_STONE
	USE_OBJ(427, useMiscTextOnly),
	USE_OBJ(441, useAssembledDrill),
	USE_OBJ(442, useMiscTextOnly), // OBJ_PILE_OF_ROCKS
	USE_OBJ(443, useMiscTextOnly), // OBJ_PILE_OF_IRON_ORE
	USE_OBJ(444, useMiscTextOnly), // OBJ_PILE_OF_COAL
	USE_OBJ_ON(448, RADIUM_BLOCK_ON), // OBJ_BLOCK_OF_RADIUM
	USE_OBJ_ON(449, RADIUM_CHIP_ON), // OBJ_CHIP_OF_RADIUM
	USE_OBJ(458, usePanel)
};

const MDScripts::UseEntry *MDScripts::getUseEntry(uint16 objN) {
	for (uint i = 0; i < ARRAYSIZE(USE_TABLE); ++i) {
		if (USE_TABLE[i]._objN == objN)
			return &USE_TABLE[i];
	}

	return nullptr;
}

bool MDScripts::hasUsecode(Obj *obj, UseCodeEvent usecodeType) {
	uint16 objN = obj->obj_n;

	if (usecodeType == USE_EVENT_USE)
		return getUseEntry(objN) != nullptr;

	if (usecodeType == USE_EVENT_READY)
		return objN == 15 || objN == 136 || objN == 161;

	if (usecodeType == USE_EVENT_MOVE)
		return objN == 268 || objN == 395 || objN == 410 || objN == 441;

	return false;
}

bool MDScripts::canInteractWithObj(Actor *actor, Obj *obj) {
	if (obj->is_on_map()) {
		// FIXME get_combat_range()
		int distance = actorFindMaxWrappedXYDistance(actor, obj->x, obj->y);

		if (actorIsAffectedByBerries(actor->get_actor_num(), BERRY_PURPLE) && distance <= actor->get_level())
			return true;

		if (distance > 1) {
			printl("OUT_OF_RANGE");
			return false;
		}
	}

	return true;
}

/** Uses an object, which for most objects means choosing another object to use it on */
class MDScripts::UseObjThread : public Continuation {
private:
	MDScripts *_scripts;
	Obj *_obj;
	Actor *_actor;
	const UseEntry *_entry;
	const UseOnEntry *_onEntry = nullptr;
	Target _target;
	int _targetX = 0;
	int _targetY = 0;

	static void directionToLocation(NuvieDir direction, const Actor *actor, int &x, int &y) {
		static const int OFFSET_X[9] = { 0, 1, 1, 1, 0, -1, -1, -1, 0 };
		static const int OFFSET_Y[9] = { -1, -1, 0, 1, 1, 1, 0, -1, 0 };

		x = actor->get_x() + OFFSET_X[direction];
		y = actor->get_y() + OFFSET_Y[direction];
	}

	uint8 finish() {
		_scripts->updateObjectsAroundParty();
		return NUVIE_SCRIPT_FINISHED;
	}

	uint8 useOn() {
		if (_resumedWithNil) {
			_scripts->printl("NOTHING");
			return finish();
		}

		uint8 z = _actor->get_z();
		directionToLocation(_direction, _actor, _targetX, _targetY);

		Actor *targetActor = mapGetActor(_targetX, _targetY, z);
		if (targetActor) {
			_target = Target(targetActor);
		} else {
			Obj *targetObj = mapGetObj(_targetX, _targetY, z);
			_target = targetObj ? Target(targetObj) : Target();
		}

		const UseOnEntry *fallback = nullptr;
		_onEntry = nullptr;
		for (uint i = 0; i < _entry->_onCount; ++i) {
			if (_entry->_on[i]._objN == 0)
				fallback = &_entry->_on[i];
			else if (!_target.isNull() && _entry->_on[i]._objN == _target.objN())
				_onEntry = &_entry->_on[i];
		}

		if (_target.isNull()) {
			if (fallback)
				(_scripts->*fallback->_func)(_obj, _target, _actor, _targetX, _targetY, z);
			else
				_scripts->printl("NOTHING");

			return finish();
		}

		_scripts->print(_target.name() + "\n\n");

		if (!_onEntry) {
			if (fallback)
				(_scripts->*fallback->_func)(_obj, _target, _actor, _targetX, _targetY, z);
			else
				_scripts->printl("NO_EFFECT");

			return finish();
		}

		if (_onEntry->_func) {
			(_scripts->*_onEntry->_func)(_obj, _target, _actor, _targetX, _targetY, z);
			return finish();
		}

		_scripts->print(_scripts->_lang.get("TO"));
		return wait(NUVIE_SCRIPT_GET_DIRECTION, 2);
	}

	uint8 useOnTo() {
		if (_resumedWithNil) {
			_scripts->printl("NOTHING");
			return finish();
		}

		int toX, toY;
		directionToLocation(_direction, _actor, toX, toY);

		Obj *toObj = mapGetObj(toX, toY, _actor->get_z());
		if (!toObj) {
			_scripts->printl("SOMETHING");
			return finish();
		}

		_scripts->print(Target(toObj).name() + "\n\n");

		for (uint i = 0; i < _onEntry->_toCount; ++i) {
			if (_onEntry->_to[i]._objN == toObj->obj_n) {
				(_scripts->*_onEntry->_to[i]._func)(_obj, _target, toObj, _actor);
				break;
			}
		}

		return finish();
	}

protected:
	uint8 run() override {
		switch (_step) {
		case 0:
			if (!_scripts->canInteractWithObj(_actor, _obj))
				return NUVIE_SCRIPT_FINISHED;

			if (_entry->_func) {
				_scripts->print("\n");
				(_scripts->*_entry->_func)(_obj, _actor);
				return finish();
			}

			_scripts->print(_scripts->_lang.get("ON"));
			return wait(NUVIE_SCRIPT_GET_DIRECTION, 1);

		case 1:
			return useOn();

		default:
			return useOnTo();
		}
	}

public:
	UseObjThread(MDScripts *scripts, Obj *obj, Actor *actor, const UseEntry *entry) :
		_scripts(scripts), _obj(obj), _actor(actor), _entry(entry) {}
};

ScriptThread *MDScripts::useObj(Obj *obj, Actor *actor) {
	const UseEntry *entry = getUseEntry(obj->obj_n);
	if (!entry)
		return nullptr;

	return new UseObjThread(this, obj, actor, entry);
}

bool MDScripts::readyObj(Obj *obj, Actor *actor) {
	const uint16 OBJ_WINGED_SHOES = 15;
	const uint16 OBJ_TONGS = 136;
	const uint16 OBJ_RED_THROW_RUG = 161;
	uint16 objN = obj->obj_n;

	if (!obj->is_readied()) {
		if (objN == OBJ_WINGED_SHOES) {
			print("\n");

			if (Game::get_game()->get_player()->get_actor()->get_z() == 2) {
				Obj *bridge = newObj(146);
				bridge->set_temporary(false);
				if (!Game::get_game()->get_obj_manager()->moveto_map(bridge, MapCoord(0xc9, 0x9b, 2)))
					delete_obj(bridge);
			}
			return true;
		}

		if (objN == OBJ_TONGS) {
			print("\n");
			printl("THE_TONGS_WILL_NOW_PROTECT_YOUR_HANDS");
			return true;
		}

		if (objN == OBJ_RED_THROW_RUG) {
			print("\n");
			obj->obj_n = 162; // Change to red cape
			return true;
		}
	} else if (objN == OBJ_WINGED_SHOES) {
		print("\n");
		printl("THEY_WONT_COME_OFF");
		return false;
	}

	return true;
}

bool MDScripts::moveObj(Obj *obj, sint16 relX, sint16 relY) {
	switch (obj->obj_n) {
	case 268:
		return moveWheelbarrow(obj, relX, relY);
	case 395: // Plank
		setObjLocation(obj, obj->x + relX, obj->y + relY, obj->z);
		return true;
	case 410:
		return moveRailCart(obj, relX, relY);
	case 441:
		return moveDrill(obj, relX, relY);
	default:
		return true;
	}
}

bool MDScripts::isRangedSelect(UseCodeType operation) {
	return actorIsAffectedByBerries(Game::get_game()->get_player()->get_actor()->get_actor_num(), BERRY_PURPLE);
}

bool MDScripts::moveDrill(Obj *obj, int relX, int relY) {
	if (relX != 0 && relY != 0)
		return false;

	if (relX < 0)
		obj->frame_n = 1;
	else if (relX > 0)
		obj->frame_n = 4;
	else if (relY < 0)
		obj->frame_n = 3;
	else if (relY > 0)
		obj->frame_n = 5;

	return true;
}

bool MDScripts::moveWheelbarrow(Obj *obj, int relX, int relY) {
	if (relX != 0 && relY != 0)
		return false;

	if (relX < 0)
		obj->frame_n = 3;
	else if (relX > 0)
		obj->frame_n = 1;
	else if (relY < 0)
		obj->frame_n = 0;
	else if (relY > 0)
		obj->frame_n = 2;

	return true;
}

bool MDScripts::checkForTrack(Obj *car, int relX, int relY) {
	int x = car->x + relX;
	int y = car->y + relY;

	Common::Array<Obj *> objs;
	getObjsAt(x, y, car->z, objs);

	for (uint i = 0; i < objs.size(); ++i) {
		Obj *obj = objs[i];
		uint16 objN = obj->obj_n;

		// Track objects
		if ((objN >= 412 && objN <= 414) || objN == 419 || objN == 175 || objN == 163)
			return (car->frame_n % 2) == obj->frame_n || obj->frame_n == 2 || objN == 412 || objN == 175 || objN == 163;

		if (!isBlood(objN) && objN != 417) // OBJ_MARTIAN_BARGE
			return false;
	}

	if (mapGetObj(x, y, car->z, 175, true) || mapGetObj(x, y, car->z, 412, true))
		return true;

	const Tile *tile = Game::get_game()->get_game_map()->get_tile(x, y, car->z, false);
	if (!tile)
		return false;

	uint16 tileNum = tile->tile_num;
	return tileNum == 108 || tileNum == 109 || tileNum == 110 || tileNum == 77 || tileNum == 79;
}

bool MDScripts::moveRailCart(Obj *obj, int relX, int relY) {
	uint8 frameN = obj->frame_n;

	if (relX != 0 && relY != 0) {
		printl("IT_WONT_GO_IN_THAT_DIRECTION");
		return false;
	}

	bool horizontalFrame = frameN == 0 || frameN == 2 || frameN == 4;
	bool verticalFrame = frameN == 1 || frameN == 3 || frameN == 5;

	if ((relX != 0 && verticalFrame) || (relY != 0 && horizontalFrame) || !checkForTrack(obj, relX, relY)) {
		printl("IT_WONT_GO_IN_THAT_DIRECTION");
		return false;
	}

	setObjLocation(obj, obj->x + relX, obj->y + relY, obj->z);
	return false;
}

} // End of namespace MD
} // End of namespace Scripts
} // End of namespace Ultima6
} // End of namespace Ultima
