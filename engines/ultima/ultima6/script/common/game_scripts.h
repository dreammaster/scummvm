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
#include "ultima/ultima6/misc/map_entity.h"
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

/** An actor or an object, as scripts often don't care which they are dealing with */
struct Target {
	Actor *_actor = nullptr;
	Obj *_obj = nullptr;

	Target() {}
	Target(Actor *actor) : _actor(actor) {}
	Target(Obj *obj) : _obj(obj) {}

	bool isNull() const {
		return !_actor && !_obj;
	}
	bool isActor() const {
		return _actor != nullptr;
	}
	bool operator==(const Target &other) const {
		return _actor == other._actor && _obj == other._obj;
	}
	bool operator!=(const Target &other) const {
		return !(*this == other);
	}

	uint16 x() const;
	uint16 y() const;
	uint8 z() const;
	uint16 objN() const;
	uint16 frameN() const;
	Common::String name() const;
};

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
	/** Prints the text for a code, which is a printf format, with the arguments filled in */
	void printfl(const char *code, ...) const;

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

	/** Effects and engine actions that scripts commonly use */
	static void hitAnim(int x, int y);
	static void projectileAnim(uint16 tileNum, int startX, int startY, int targetX, int targetY,
		int speed, bool trail, int initialTileRotation, int rotationAmount = 0, int srcTileYOffset = 0);
	static void projectileAnimMulti(uint16 tileNum, int startX, int startY, const Common::Array<MapCoord> &targets,
		int speed, bool trail, int initialTileRotation, Common::Array<MapEntity> &hitEntities);
	/** Runs an explosion and returns the actors and objects it hit */
	static void explosionStart(uint16 tileNum, int x, int y, Common::Array<MapEntity> &hitEntities);
	static void quakeStart(int magnitude, int duration);
	static void fadeOutEffect();
	static void fadeInEffect();
	static void displayPrompt(bool newline);
	static void displayTextInScrollGump(const Common::String &text);
	static bool loadTextFromLzc(const char *filename, int idx, Common::String &text);
	static bool mapCanReachPoint(int x, int y, int x1, int y1, uint8 level);
	/** Traces a line for a missile. Gives where it hit and the tile before that, or the target if it didn't */
	static void mapLineHitCheck(int x, int y, int x1, int y1, uint8 level, int &hitX, int &hitY, int &preHitX, int &preHitY);
	static float objectWeight(Obj *obj);
	static void actorTalk(Actor *actor);

	/** Inventory of an actor in list order, optionally including the contents of containers */
	static void getInventoryObjs(Actor *actor, bool recursive, Common::Array<Obj *> &objs);

	/** Removes an object from the game and frees it unless it ended up somewhere else */
	static void removeFromEngine(Obj *obj);
	static uint16 objOriginalTileNum(const Obj *obj);

	/** Objects at a map location, in list order. Safe to move or remove while iterating the result */
	static void getObjsAt(int x, int y, int z, Common::Array<Obj *> &objs);
	static void getObjsInArea(int x, int y, int z, int width, int height, Common::Array<Obj *> &objs);
	/** Every object of a type on a map level */
	static void findObjs(int level, int objN, Common::Array<Obj *> &objs);
	static Obj *mapGetObj(int x, int y, int z, int objN = -1, bool includeMultiTile = false);
	static Actor *mapGetActor(int x, int y, int z, Actor *excluded = nullptr);
	static bool mapIsOnScreen(int x, int y, int z);
	static bool tileGetFlag(uint16 tileNum, uint8 flagSet, uint8 bit);
	static uint16 objTileNum(const Obj *obj);
	static const char *tileDescription(uint16 tileNum);

	/** Moves an object that's on the map, or just changes its location if it isn't */
	static void setObjLocation(Obj *obj, uint16 x, uint16 y, uint8 z);

	static uint16 getWrappedDist(uint16 pt1, uint16 pt2);
	static int actorRandomiseStat(int baseStat);
	static bool actorHasBadAlignment(const Actor *actor);
	static bool actorHasFreeArm(Actor *actor);
	static bool actorIsHoldingObj(Actor *actor, uint16 objN);
	static int actorFindMaxWrappedXYDistance(const Actor *actor, uint16 x, uint16 y);
	uint8 actorGetCombatRange(const Actor *actor, uint16 targetX, uint16 targetY) const;
	void subtractMapMovementPts(Actor *actor);

	/** Moves one tile in a direction. The flag is passed on to the game's movement logic */
	virtual bool actorMove(Actor *actor, NuvieDir direction, int flag) {
		return false;
	}
	virtual int actorMoveDiagonal(Actor *actor, NuvieDir xDirection, NuvieDir yDirection) {
		return 0;
	}

	/** Returns 0 if the actor couldn't move anywhere, otherwise 1 */
	int actorMoveTowardsLoc(Actor *actor, int mapX, int mapY);
	bool tossActor(Actor *actor, int fromX, int fromY, uint8 fromZ, int arg0);

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

	/** Creates the script that casts the spell with an invocation, or nullptr if there isn't one */
	virtual ScriptThread *magicCastThread(const char *invocation) {
		return nullptr;
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

	/** True while some of the game's scripts still run in Lua, so the Lua scripts need loading */
	virtual bool needsLua() const {
		return false;
	}

	/** Intro and ending cutscenes. Return false if the game has none in C++ yet. */
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
