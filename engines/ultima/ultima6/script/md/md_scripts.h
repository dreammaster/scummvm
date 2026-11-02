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

#ifndef NUVIE_SCRIPT_MD_MD_SCRIPTS_H
#define NUVIE_SCRIPT_MD_MD_SCRIPTS_H

#include "ultima/ultima6/script/common/game_scripts.h"

namespace Ultima {
namespace Ultima6 {
namespace Scripts {
namespace MD {

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

class MDScripts : public GameScripts {
public:
	enum Worktype {
		WT_NOTHING = 0x0,
		WT_FOLLOW = 0x1,
		WT_PLAYER = 0x2,
		WT_SLEEP = 0x91
	};

	enum Berry {
		BERRY_PURPLE = 0,
		BERRY_GREEN = 1,
		BERRY_BROWN = 2
	};

	enum DamageMode {
		DAMAGE_UNSPECIFIED = -1,
		DAMAGE_NORMAL = 0,
		DAMAGE_COLD = 1,
		DAMAGE_ACID = 2,
		DAMAGE_HEAT = 3
	};

	enum AttackResult {
		ATTACK_OK = 0,
		ATTACK_NO_AMMO = 1,
		ATTACK_OUT_OF_RANGE = 2,
		ATTACK_NOT_POSSIBLE = 3
	};

	static const uint16 RED_HIT_TILE = 257;
	static const uint16 BLUE_HIT_TILE = 258;

private:
	class PlayerAttack;

	typedef void (MDScripts::*ObjFunc)(Obj *obj);

	// Saved in the objlist
	uint8 _hoursTillNextHealing = 0;
	bool _inDreamMode = false;
	uint16 _currentDreamStage = 0;
	uint8 _prevPlayerX = 0;
	uint8 _prevPlayerY = 0;
	uint8 _objlist1d22Unk = 0;

	bool _partyIsWarm = false;
	int _talkScriptStatus = -1;
	Target _selectedTarget;
	Target _attackTarget;
	Common::Array<Obj *> _playerReadiedWeapons;

	// Generated from the original tables. Some are keyed by tile number rather than object number
	static const ObjValue READIABLE_LOCATIONS[];
	static const uint READIABLE_LOCATIONS_COUNT;
	static const ObjValue WEAPON_RANGES[];
	static const uint WEAPON_RANGES_COUNT;
	static const ObjValue WEAPON_DAMAGE[];
	static const uint WEAPON_DAMAGE_COUNT;
	static const ObjValue ARMOUR[];
	static const uint ARMOUR_COUNT;
	static const ObjValue CLOTHING_WARMTH[];
	static const uint CLOTHING_WARMTH_COUNT;

	void initLang();

	// init.lua
	void updateWatchTile();
	static bool isBlood(uint16 objN);
	static bool isObjAttackable(const Obj *obj);
	static bool isPlantObj(const Obj *obj);
	static bool isObjBurnable(const Obj *obj);
	bool isOpenWaterAtLoc(uint16 x, uint16 y, uint8 z);
	void search(Obj *obj);
	static uint16 wrapCoord(int coord, uint8 level);
	bool canMoveObj(const Obj *obj, int relX, int relY);
	void updateLampPosts();
	void playMdSfx(uint16 sfxId, bool async = false);
	void playDoorSfx();
	int inputSelectObjQty(Obj *obj);

	// actor.lua
	/**
	 * Per object number: strength, dexterity, intelligence, hit points, alignment,
	 * an unknown value, damage, then the flag bytes read by isActorStatBitSet()
	 */
	struct ActorStats {
		uint16 _objN;
		uint8 _v[23];
	};
	static const ActorStats ACTOR_STATS[];
	static const uint ACTOR_STATS_COUNT;
	static const ActorStats *getActorStats(uint16 objN);
	static bool isActorStatBitSet(uint16 objN, int bit);
	static int actorGetDamage(uint16 objN);

	bool actorIsAffectedByBerries(uint8 actorNum, int berryType);
	int actorGetBerryCount(uint8 actorNum, int berryType);
	void actorDecrementBerryCounter(Actor *actor, int berryType);
	void actorDecrementBerryCounters(Actor *actor);
	void actorIncrementBerryCounter(uint8 actorNum, int berryType);
	void actorClearBerryCounters(uint8 actorNum);
	int getBlueBerryCounter();
	void setBlueBerryCounter(int value);

	int actorStrAdjusted(Actor *actor);
	int actorDexAdjusted(Actor *actor);
	int actorIntAdjusted(Actor *actor);
	void advanceTimeForPartyMember(Actor *actor, int numTurns, int hour);
	void advanceTimeTurn(Actor *actor, int actorNum);
	void removeObj(Obj *obj);
	static int floorMod(int value, int divisor) {
		int result = value % divisor;
		return result < 0 ? result + divisor : result;
	}

	void actorDivide(Actor *actor);
	void defenderUpdateAlignment(Actor *attacker, const Target &defender, int damage);
	void actorHandleDamage(Actor *defender);
	void actorTakeHit(Actor *attacker, const Target &defender, int maxDmg, int damageMode);
	static bool actorImmuneToDmg(const Actor *actor);
	void actorPrintCustomHitMessage(Actor *actor);
	void hitTarget(const Target &target, uint16 hitTile);
	int actorHitTarget(const Target &defender, int maxDmg, int damageMode);
	void attackBucket(Obj *bucket, int damageMode);
	void actorHitObj(Obj *obj, int dmg, int damageMode);
	void actorUpdateFrame(Actor *actor, NuvieDir direction);
	void reviveAvatar();
	void avatarFallsUnconscious();
	void partyUpdate();
	int actorGetAc(Actor *actor);
	int clothingGetWarmthRating(const Obj *obj);
	static bool isLitLightsource(const Obj *obj);
	void actorTakeDamageFromObj(Actor *actor, Obj *obj);
	void actorRemoveCharm(Actor *actor);
	void actorRadiationCheck(Actor *actor, Obj *obj);
	Target findRockwormActor(const Target &target);

	// combat.lua
	int getWeaponDamage(const Target &weapon);
	bool attackDexSavingThrow(Actor *attacker, const Target &defender, const Target &weapon);
	void attackWithFreezeray(Actor *actor, Actor *target, int damage);
	int checkAmmo(Actor *actor, Obj *weapon);
	int attackTargetWithWeapon(Actor *actor, uint16 targetX, uint16 targetY, const Target &weapon);
	void spreadWeaponDamage(Actor *actor, uint16 targetX, uint16 targetY, Obj *weapon);
	void fireRangeBasedWeapon(Actor *attacker, uint16 targetX, uint16 targetY, const Target &weapon);

	// worktype.lua
	Actor *selectTargetActor(Actor *srcActor);
	void worktype15YourMother(Actor *actor);
	void worktypeStompAround(Actor *actor);
	void worktype16Minotaur(Actor *actor);
	void worktype99CokerMoveToCoalVein(Actor *actor);
	void worktype9ACokerDropCoal(Actor *actor);
	void worktype9BCokerWaitForCoalToMoveAway(Actor *actor);
	void worktype9DStokerWaitForCoal(Actor *actor);
	void worktype9EStokerWalkToFurnace(Actor *actor);
	void worktype9CStokerReturnToConveyorBelt(Actor *actor);
	void activateTowerElectricity();
	void stokerBlocked(Actor *stoker);
	void performWorktype(Actor *actor);

	// player.lua
	void updateObjectsAroundParty();
	bool canMoveDrill(Obj *drill, int relX, int relY);
	Target weaponSelect();
	Obj *selectNextWeapon();
	void selectAttackTarget(const MapCoord &targetLoc);
	void playerAttackWithWeapon(const Target &weapon, const MapCoord &targetLoc);

	// look.lua
	void lookPocketwatch(Obj *obj);
	void lookCannon(Obj *obj);
	void lookBarrow(Obj *obj);
	static Common::String getLatLongString(int x, int y);
	void lookMarkerFlag(Obj *obj);
	void lookBrokenStrap(Obj *obj);
	void lookMetalWoman(Obj *obj);
	void lookCoveredMartianSeed(Obj *obj);
	void lookSprayerSystem(Obj *obj);
	void lookCabinet(Obj *obj);
	void lookTrackingMotor(Obj *obj);
	void lookPanel(Obj *obj);
	void printNumberOfCharges(int qty);
	void lookPortableSprayer(Obj *obj);
	const char *getWeaponModeString(const Obj *obj);
	void lookRayGun(Obj *obj);
	void lookBelgianCombine(Obj *obj);
	void lookSwitchBar(Obj *obj);
	void lookLightSource(Obj *obj);
	void lookDoor(Obj *obj);
	void lookObelisk(Obj *obj);
	bool lookUsecode(Obj *obj);

	// talk.lua
	void openGatesAtOlympusMons();
	void openDreamMachineDoor();
	void talkScriptFixPanels();
	void talkScriptSpawnMonsterFootprints();
	void talkConveyor();
	void talkTower();
	void talkDreamMachine();
	void playEndSequence();

	// usecode.lua
	class UseObjThread;

	typedef void (MDScripts::*UseFunc)(Obj *obj, Actor *actor);
	typedef void (MDScripts::*UseOnFunc)(Obj *obj, const Target &target, Actor *actor, int targetX, int targetY, int targetZ);
	typedef void (MDScripts::*UseToFunc)(Obj *obj, const Target &target, Obj *toObj, Actor *actor);

	struct UseToEntry {
		uint16 _objN;
		UseToFunc _func;
	};

	/** What an object does when used on another. Either a function, or a table of objects to then use it to */
	struct UseOnEntry {
		uint16 _objN;
		UseOnFunc _func;
		const UseToEntry *_to;
		uint _toCount;
	};

	struct UseEntry {
		uint16 _objN;
		UseFunc _func;
		const UseOnEntry *_on;
		uint _onCount;
	};

	static const UseToEntry SLEDGE_HAMMER_TO[];
	static const UseToEntry PLIERS_TO[];
	static const UseToEntry SHOVEL_ON_PILE_TO[];
	static const UseToEntry SHOVEL_UNLOAD_TO[];
	static const UseToEntry SHOVEL_ON_ORE_TO[];
	static const UseOnEntry SLEDGE_HAMMER_ON[];
	static const UseOnEntry PLIERS_ON[];
	static const UseOnEntry DIGGING_TOOL_ON[];
	static const UseOnEntry SHOVEL_ON[];
	static const UseOnEntry MANUSCRIPT_ON[];
	static const UseOnEntry PRYBAR_ON[];
	static const UseOnEntry HAMMER_ON[];
	static const UseOnEntry SPRAY_GUN_REFILL_ON[];
	static const UseOnEntry OIL_ON[];
	static const UseOnEntry SPRAY_GUN_ON[];
	static const UseOnEntry WRENCH_ON[];
	static const UseOnEntry REPAIRED_BELT_ON[];
	static const UseOnEntry GONG_HAMMER_ON[];
	static const UseOnEntry HEADGEAR_ON[];
	static const UseOnEntry HEART_STONE_ON[];
	static const UseOnEntry RADIUM_BLOCK_ON[];
	static const UseOnEntry RADIUM_CHIP_ON[];
	static const UseEntry USE_TABLE[];
	static const UseEntry *getUseEntry(uint16 objN);

	bool canInteractWithObj(Actor *actor, Obj *obj);
	int actorGetMaxHp(Actor *actor);

	bool moveWheelbarrow(Obj *obj, int relX, int relY);
	bool moveRailCart(Obj *obj, int relX, int relY);
	bool moveDrill(Obj *obj, int relX, int relY);
	bool checkForTrack(Obj *car, int relX, int relY);

	void updateConveyorBelt(bool canStop);
	void activatePowerSystem();
	void activatePowerUpdateTiles();
	void shutdownPowerUpdateTiles();
	void midgameCutscene2();

	void searchContainer(Obj *obj);
	static uint16 getPileObjNum(uint16 mapTile);
	bool getFreeLocationAroundActor(Actor *actor, MapCoord &pos);
	static int getOreContainerQuality(uint16 oreObjN);
	static uint16 getObjNumFromOreQuality(int oreQuality);
	bool shovelOreToContainer(Obj *ore, Obj *toObj);
	bool shovelOreToFurnace(Obj *ore, Obj *furnace);
	bool getFreeLocationAroundDrill(Obj *drill, MapCoord &pos);
	bool canDrillAtLoc(int x, int y, uint8 z);
	bool foesAreNearby();
	Actor *npcsAreNearby();
	bool isActorAbleToTalkToPlayer(Actor *actor);
	void restHealParty(int hoursToRest);
	void restLevelUpActor(Actor *actor);
	void useDreamMachine(Obj *panel);
	bool hasMinotaurLeftTheShop(uint8 curZ);

	void useRubySlippers(Obj *obj, Actor *actor);
	void useReadyObj(Obj *obj, Actor *actor);
	void useBerry(Obj *obj, Actor *actor);
	void useRedBerry(Obj *obj, Actor *actor);
	void useMiscTextOnly(Obj *obj, Actor *actor);
	void useContainer(Obj *obj, Actor *actor);
	void useSextant(Obj *obj, Actor *actor);
	void useTent(Obj *obj, Actor *actor);
	void useHandMirror(Obj *obj, Actor *actor);
	void useReadingMaterial(Obj *obj, Actor *actor);
	void useDoor(Obj *obj, Actor *actor);
	void useGate(Obj *obj, Actor *actor);
	void useSwitch(Obj *obj, Actor *actor);
	void useSwitchDevice(Obj *obj, Actor *actor);
	void useOxiumBin(Obj *obj, Actor *actor);
	void useMusicalInstrument(Obj *obj, Actor *actor);
	void useSpittoon(Obj *obj, Actor *actor);
	void useDreamstuff(Obj *obj, Actor *actor);
	void usePoolTable(Obj *obj, Actor *actor);
	void useSwitchBar(Obj *obj, Actor *actor);
	void useAssembledDrill(Obj *obj, Actor *actor);
	void usePanel(Obj *obj, Actor *actor);

	void useMiscText(Obj *obj, const Target &target, Actor *actor, int targetX, int targetY, int targetZ);
	void useToolOnGround(Obj *obj, const Target &target, Actor *actor, int targetX, int targetY, int targetZ);
	void useManuscriptOnMailbox(Obj *obj, const Target &target, Actor *actor, int targetX, int targetY, int targetZ);
	void useCrate(Obj *obj, const Target &target, Actor *actor, int targetX, int targetY, int targetZ);
	void usePrybarOnHatch(Obj *obj, const Target &target, Actor *actor, int targetX, int targetY, int targetZ);
	void useObjOnSprayGun(Obj *obj, const Target &target, Actor *actor, int targetX, int targetY, int targetZ);
	void useOilOnDoor(Obj *obj, const Target &target, Actor *actor, int targetX, int targetY, int targetZ);
	void useOilOnDreamDoor(Obj *obj, const Target &target, Actor *actor, int targetX, int targetY, int targetZ);
	void useSprayGun(Obj *obj, const Target &target, Actor *actor, int targetX, int targetY, int targetZ);
	void useWrenchOnSwitchbar(Obj *obj, const Target &target, Actor *actor, int targetX, int targetY, int targetZ);
	void useWrenchOnDrill(Obj *obj, const Target &target, Actor *actor, int targetX, int targetY, int targetZ);
	void useWrenchOnPanel(Obj *obj, const Target &target, Actor *actor, int targetX, int targetY, int targetZ);
	void useFixedBeltOnBareRollers(Obj *obj, const Target &target, Actor *actor, int targetX, int targetY, int targetZ);
	void useHammerOnOxiumGeode(Obj *obj, const Target &target, Actor *actor, int targetX, int targetY, int targetZ);
	void useGong(Obj *obj, const Target &target, Actor *actor, int targetX, int targetY, int targetZ);
	void useHeadGear(Obj *obj, const Target &target, Actor *actor, int targetX, int targetY, int targetZ);
	void useHeartstoneOnMetalWoman(Obj *obj, const Target &target, Actor *actor, int targetX, int targetY, int targetZ);
	void useRadium(Obj *obj, const Target &target, Actor *actor, int targetX, int targetY, int targetZ);

	void useSledgeHammerOnReplacementTrackToBrokenTrack(Obj *obj, const Target &target, Obj *toObj, Actor *actor);
	void usePliersOnSpoolToTower(Obj *obj, const Target &target, Obj *toObj, Actor *actor);
	void useShovelOnPileToHole(Obj *obj, const Target &target, Obj *toObj, Actor *actor);
	void useShovelToUnloadContainer(Obj *obj, const Target &target, Obj *toObj, Actor *actor);
	void useShovelOnOreToContainer(Obj *obj, const Target &target, Obj *toObj, Actor *actor);

	// dreamworld.lua
	void finishDreamQuest(Actor *actor);
	void wakeFromDream();
	void actorUseDreamMachine(Actor *actor, int dreamQuality);
	void cleanupCliffFall();
	void fallFromCliff();
	void spawnYourMother();
	void createPitcher(uint16 x, uint16 y, uint8 z);
	void setupTiffanyStage();
	void completeTiffanyStage();
	void dreamworldCleanupState(Obj *obj);

protected:
	const ObjValue *weaponRanges(uint &count) const override;

public:
	explicit MDScripts(Configuration *config);
	~MDScripts() override {}

	bool loadGame(NuvieIO *objlist) override;
	bool saveGame(NuvieIO *objlist) override;

	// Entry points called by the engine
	MovementStatus playerBeforeMoveAction(sint16 *relX, sint16 *relY) override;
	bool playerPostMoveAction(bool didMove) override;
	bool playerPass() override;
	ScriptThread *playerAttackThread() override;

	bool actorUpdateAll() override;
	bool actorInit(Actor *actor, ActorAlignment alignment) override;
	bool actorMapDmg(Actor *actor, MapCoord location) override;
	bool actorHit(Actor *actor, uint8 dmg, bool displayHitMsg) override;
	uint8 actorStrAdj(Actor *actor) override;
	uint8 actorDexAdj(Actor *actor) override;
	uint8 actorIntAdj(Actor *actor) override;
	bool actorGetObj(Actor *actor, Obj *obj, Obj *container) override;
	bool actorSubtractMovementPoints(Actor *actor, uint8 points) override;
	bool actorResurrect(Actor *actor) override;
	uint8 getPortraitNumber(Actor *actor) override;
	bool outOfAmmo(Actor *attacker, Obj *weapon, bool printMessage) override;
	bool actorMove(Actor *actor, NuvieDir direction, int flag) override;
	int actorMoveDiagonal(Actor *actor, NuvieDir xDirection, NuvieDir yDirection) override;
	bool advanceTime(uint16 minutes) override;

	bool lookObj(Obj *obj) override;
	int objGetReadiableLocation(Obj *obj) override;
	bool isContainerObj(uint16 objN) override;
	bool talkScript(uint8 scriptNumber) override;
	bool talkToActor(Actor *actor) override;
	bool talkToObj(Obj *obj) override;
	bool createObjectNeedsQuan(uint16 objN) override;

	bool playIntro() override;
	bool playEnding() override;

	bool hasUsecode(Obj *obj, UseCodeEvent usecodeType) override;
	ScriptThread *useObj(Obj *obj, Actor *actor) override;
	bool readyObj(Obj *obj, Actor *actor) override;
	bool moveObj(Obj *obj, sint16 relX, sint16 relY) override;
	bool isRangedSelect(UseCodeType operation) override;
};

} // End of namespace MD
} // End of namespace Scripts
} // End of namespace Ultima6
} // End of namespace Ultima

#endif
