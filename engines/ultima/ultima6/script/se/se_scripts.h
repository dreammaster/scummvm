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

#ifndef NUVIE_SCRIPT_SE_SE_SCRIPTS_H
#define NUVIE_SCRIPT_SE_SE_SCRIPTS_H

#include "ultima/ultima6/script/common/game_scripts.h"

namespace Ultima {
namespace Ultima6 {
namespace Scripts {
namespace SE {

class SEScripts : public GameScripts {
private:
	typedef void (SEScripts::*UseFunc)(Obj *obj, Actor *actor);

	struct UseEntry {
		uint16 _objN;
		UseFunc _func;
	};

	static const UseEntry USE_TABLE[];
	static const ObjValue READIABLE_LOCATIONS[];
	static const ObjValue WEAPON_RANGES[];

	static UseFunc getUseFunc(uint16 objN);

	void initLang();
	void search(Obj *obj);
	bool actorHasItem(Actor *actor, Obj *obj);
	Actor *checkCanReach(Obj *obj, Actor *actor, bool printMsg);
	void deleteOrRemoveObject(Actor *actor, Obj *obj);
	bool addItemToActor(uint16 objN, Actor *actor, bool stackQty, bool printMsg);

	void useCornStalk(Obj *obj, Actor *actor);
	void useTree(Obj *obj, Actor *actor);
	void useYuccaPlant(Obj *obj, Actor *actor);
	void useClay(Obj *obj, Actor *actor);
	void useFishingPole(Obj *obj, Actor *actor);
	void useDiggingStick(Obj *obj, Actor *actor);

protected:
	const ObjValue *weaponRanges(uint &count) const override;

public:
	explicit SEScripts(Configuration *config);
	~SEScripts() override {}

	bool loadGame(NuvieIO *objlist) override {
		return true;
	}
	bool saveGame(NuvieIO *objlist) override {
		return true;
	}

	MovementStatus playerBeforeMoveAction(sint16 *relX, sint16 *relY) override {
		return CAN_MOVE;
	}
	bool playerPostMoveAction(bool didMove) override {
		return true;
	}
	bool playerPass() override;
	bool playerAttack() override {
		return true;
	}

	bool actorUpdateAll() override {
		return true;
	}
	bool actorMapDmg(Actor *actor, MapCoord location) override {
		return true;
	}
	bool actorGetObj(Actor *actor, Obj *obj, Obj *container) override;

	bool lookObj(Obj *obj) override;
	int objGetReadiableLocation(Obj *obj) override;
	bool hasUsecode(Obj *obj, UseCodeEvent usecodeType) override;
	ScriptThread *useObj(Obj *obj, Actor *actor) override;
	bool moveObj(Obj *obj, sint16 relX, sint16 relY) override {
		return false;
	}
	bool canGetObjOverride(Obj *obj) override {
		return false;
	}
	bool outOfAmmo(Actor *attacker, Obj *weapon, bool printMessage) override;
	bool isRangedSelect(UseCodeType operation) override {
		return false;
	}

	bool advanceTime(uint16 minutes) override;
	bool talkToObj(Obj *obj) override;

	bool playIntro() override;
	bool playEnding() override;

	bool isContainerObj(uint16 objN) override;
	uint16 getTileToObjectMapping(uint16 tileN) override;
	bool isTileObject(uint16 objN) override;
};

} // End of namespace SE
} // End of namespace Scripts
} // End of namespace Ultima6
} // End of namespace Ultima

#endif
