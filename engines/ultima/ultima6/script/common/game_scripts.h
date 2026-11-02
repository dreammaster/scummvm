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

#ifndef NUVIE_SCRIPT_COMMON_GAME_SCRIPTS_H
#define NUVIE_SCRIPT_COMMON_GAME_SCRIPTS_H

#include "common/array.h"
#include "common/str.h"
#include "ultima/ultima6/core/map.h"
#include "ultima/ultima6/actors/actor.h"
#include "ultima/ultima6/misc/u6_misc.h"
#include "ultima/ultima6/script/common/continuation.h"
#include "ultima/ultima6/script/common/lang.h"
#include "ultima/ultima6/usecode/usecode.h"

namespace Ultima {
namespace Ultima6 {

class Configuration;
class NuvieIO;
class Obj;
class ScriptThread;
class Spell;

namespace Scripts {

/**
 * Per-game script logic. Each virtual is the C++ equivalent of a Lua global
 * the engine used to call by name. The defaults match what the engine saw
 * when that global didn't exist for a game, so a game only overrides what
 * its Lua scripts actually defined.
 */
class GameScripts {
protected:
	Configuration *_config;
	Lang _lang;

	/** Table of per-object values, searched linearly by object number */
	struct ObjValue {
		uint16 objN;
		uint8 value;
	};

	void print(const Common::String &str) const;
	void printl(const char *code) const;
	void printfl(const char *code, const Common::String &arg) const;

	/** Blocks for a number of milliseconds while the game keeps updating */
	void wait(uint32 duration) const;

	Common::String inputSelect(const char *allowedChars, bool canEscape) const;
	int inputSelectInteger(bool canEscape) const;
	bool inputShouldProceed() const;

	void altCodeSetActorTalkFlag();
	void altCodeCreateObject();
	void altCodeExportTmxMapFiles();
	void altCodeExportTileset();
	void altCodeFindObjsOnMap();

	/** Collects the direct children of a container so they can be moved while iterating */
	static void getContainerObjs(Obj *container, Common::Array<Obj *> &objs);

	/** Returns the party members, in party order */
	static void getPartyMembers(Common::Array<Actor *> &members);

	/** Creates a loose object; the caller must place it in the game or delete it */
	static Obj *newObj(uint16 objN);

	static int findObjValue(const ObjValue *table, uint count, uint16 objN, int defaultValue);

	/** Weapon ranges for this game, or nullptr if it doesn't define any */
	virtual const ObjValue *weaponRanges(uint &count) const {
		count = 0;
		return nullptr;
	}

public:
	explicit GameScripts(Configuration *config);
	virtual ~GameScripts() {}

	virtual bool loadGame(NuvieIO *objlist) {
		return false;
	}
	virtual bool saveGame(NuvieIO *objlist) {
		return false;
	}

	virtual MovementStatus playerBeforeMoveAction(sint16 *relX, sint16 *relY) {
		return CAN_MOVE;
	}
	virtual bool playerPostMoveAction(bool didMove) {
		return false;
	}
	virtual bool playerPass() {
		return false;
	}
	virtual bool playerAttack() {
		return false;
	}

	/** The player's attack command, which can ask for a target or direction */
	virtual ScriptThread *playerAttackThread() {
		return new FinishedThread();
	}

	virtual bool actorUpdateAll() {
		return false;
	}
	virtual bool actorInit(Actor *actor, ActorAlignment alignment) {
		return false;
	}
	virtual bool actorAttack(Actor *actor, MapCoord location, Obj *weapon, Actor *foe) {
		return false;
	}
	virtual bool actorMapDmg(Actor *actor, MapCoord location) {
		return false;
	}
	virtual bool actorTileDmg(Actor *actor, uint16 tileNum) {
		return false;
	}
	virtual bool actorHit(Actor *actor, uint8 dmg, bool displayHitMsg) {
		return false;
	}
	virtual uint8 actorStrAdj(Actor *actor) {
		return 0;
	}
	virtual uint8 actorDexAdj(Actor *actor) {
		return 0;
	}
	virtual uint8 actorIntAdj(Actor *actor) {
		return 0;
	}
	virtual uint8 actorGetMaxMagicPoints(const Actor *actor) {
		return 0;
	}
	virtual bool actorGetObj(Actor *actor, Obj *obj, Obj *container) {
		return false;
	}
	virtual bool actorSubtractMovementPoints(Actor *actor, uint8 points) {
		return false;
	}
	virtual bool actorResurrect(Actor *actor) {
		return false;
	}
	virtual bool actorUseEffect(Obj *effectObj, Actor *actor) {
		return false;
	}
	virtual uint8 getPortraitNumber(Actor *actor) {
		return 1;
	}

	virtual bool lookObj(Obj *obj) {
		return false;
	}
	virtual int objGetReadiableLocation(Obj *obj) {
		return -1;
	}
	virtual bool useKeg(Obj *obj) {
		return false;
	}
	virtual bool hasUsecode(Obj *obj, UseCodeEvent usecodeType) {
		return false;
	}

	/**
	 * Runs the use code for an object. Games whose use code never has to wait
	 * for player input run it to completion here and return nullptr.
	 */
	virtual ScriptThread *useObj(Obj *obj, Actor *actor) {
		return nullptr;
	}
	virtual bool readyObj(Obj *obj, Actor *actor) {
		return false;
	}
	virtual bool moveObj(Obj *obj, sint16 relX, sint16 relY) {
		return false;
	}
	virtual bool canGetObjOverride(Obj *obj) {
		return false;
	}
	virtual bool outOfAmmo(Actor *attacker, Obj *weapon, bool printMessage) {
		return false;
	}
	virtual bool isRangedSelect(UseCodeType operation) {
		return false;
	}

	virtual bool isAvatarDead() {
		return false;
	}
	virtual bool magicGetSpellList(Spell **spellList) {
		return false;
	}

	virtual bool moonstoneSetLoc(uint8 phase, MapCoord location) {
		return false;
	}
	virtual MapCoord moonstoneGetLoc(uint8 phase) {
		return MapCoord(0, 0, 0);
	}
	virtual bool updateMoongates(bool visible) {
		return false;
	}
	virtual bool advanceTime(uint16 minutes) {
		return false;
	}
	virtual bool setGShowStealing(bool stealing) {
		return false;
	}

	virtual bool talkScript(uint8 scriptNumber) {
		return false;
	}
	virtual bool talkToObj(Obj *obj) {
		return false;
	}
	virtual bool talkToActor(Actor *actor) {
		return false;
	}

	/** Whether the alt code create object command asks for a quantity of this object */
	virtual bool createObjectNeedsQuan(uint16 objN) {
		return false;
	}

	/** Intro and ending cutscenes. Return false if the game has none. */
	virtual bool playIntro() {
		return false;
	}
	virtual bool playEnding() {
		return false;
	}

	/** Debug commands, run by number */
	bool handleAltCode(uint16 altCode);
	uint8 playMidgameSequence(uint16 seqNum);

	virtual bool isContainerObj(uint16 objN) {
		return false;
	}
	virtual uint16 getTileToObjectMapping(uint16 tileN) {
		return 0;
	}
	virtual bool isTileObject(uint16 objN) {
		return false;
	}

	/** Combat range for an x/y offset in tiles, or 9 if out of range */
	uint8 getCombatRange(uint16 absX, uint16 absY) const;
	uint8 getWeaponRange(uint16 objN) const;
};

} // End of namespace Scripts
} // End of namespace Ultima6
} // End of namespace Ultima

#endif
