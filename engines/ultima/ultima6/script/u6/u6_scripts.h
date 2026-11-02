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

#ifndef NUVIE_SCRIPT_U6_U6_SCRIPTS_H
#define NUVIE_SCRIPT_U6_U6_SCRIPTS_H

#include "ultima/ultima6/script/common/game_scripts.h"

namespace Ultima {
namespace Ultima6 {
namespace Scripts {
namespace U6 {

class U6Scripts : public GameScripts {
public:
	enum Worktype {
		WT_NOTHING = 0x0,
		WT_FOLLOW = 0x1,
		WT_PLAYER = 0x2,
		WT_FRONT = 0x3,
		WT_REAR = 0x4,
		WT_FLANK = 0x5,
		WT_BERSERK = 0x6,
		WT_RETREAT = 0x7,
		WT_ASSAULT = 0x8,
		WT_FLEE = 0x9,
		WT_LIKE = 0xa,
		WT_UNFRIENDLY = 0xb,
		WT_WANDER_NEAR_PLAYER = 0xc,
		WT_TANGLE = 0xd,
		WT_STATIONARY = 0xe,
		WT_GUARD_WALK_EAST_WEST = 0xf,
		WT_GUARD_WALK_NORTH_SOUTH = 0x10,
		WT_GUARD_ARREST_PLAYER = 0x12,
		WT_UNK_13 = 0x13,
		WT_WALK_TO_LOCATION = 0x86,
		WT_WALK_NORTH_SOUTH = 0x8b,
		WT_WALK_EAST_WEST = 0x8c,
		WT_WALK_SOUTH_NORTH = 0x8d,
		WT_WALK_WEST_EAST = 0x8e,
		WT_WANDER_AROUND = 0x8f,
		WT_WORK = 0x90,
		WT_SLEEP = 0x91,
		WT_UNK_94 = 0x94,
		WT_BEG = 0x96,
		WT_BRAWLING = 0x99,
		WT_MOUSE = 0x9a,
		WT_ATTACK_PARTY = 0x9b
	};

	enum Sfx {
		SFX_BLOCKED = 0,
		SFX_HIT = 1,
		SFX_BROKEN_GLASS = 5,
		SFX_FIRE = 7,
		SFX_MISSLE = 11,
		SFX_EXPLOSION = 12,
		SFX_ATTACK_SWING = 13,
		SFX_SUCCESS = 14,
		SFX_FAILURE = 15,
		SFX_CORPSER_DRAGGED_UNDER = 16,
		SFX_CORPSER_REGURGITATE = 17,
		SFX_CASTING_MAGIC_P1 = 18,
		SFX_CASTING_MAGIC_P2 = 26,
		SFX_AVATAR_DEATH = 34,
		SFX_KAL_LOR = 35,
		SFX_SLUG_DISSOLVE = 36,
		SFX_EARTH_QUAKE = 39
	};

	enum FadeColor {
		FADE_COLOR_BLUE = 9,
		FADE_COLOR_RED = 12
	};

	enum Timer {
		TIMER_LIGHT = 0,
		TIMER_INFRAVISION = 1,
		TIMER_STORM = 13,
		TIMER_TIME_STOP = 14,
		TIMER_ECLIPSE = 15
	};

	/** Stat table row. The flags are the Lua table entries from "can talk" onwards, read with statFlag() */
	struct ActorStats {
		uint16 _objN;
		uint8 _str, _dex, _int, _hp, _dmg;
		uint8 _align;
		uint8 _flags[15];
		uint8 _spells[8];
		uint8 _weapons[5];
		uint8 _armour[4];
		uint8 _treasure[3];
		uint8 _exp;
	};

	struct ProjectileInfo {
		uint16 _objN;
		uint16 _tileNum;
		uint16 _rotation;
		uint8 _speed;
		uint8 _spin;
	};

	/** Flag numbers in the same numbering the original Lua tables used */
	enum StatFlag {
		FLAG_CAN_TALK = 7,
		FLAG_DROPS_BLOOD = 8,
		FLAG_FADES_AWAY = 9,
		FLAG_LIVES_IN_WATER = 12,
		FLAG_IMMUNE_TO_TREMOR = 13,
		FLAG_REPEL_UNDEAD = 14,
		FLAG_POISONOUS = 15,
		FLAG_STRENGTH_BASED = 16,
		FLAG_DOUBLE_FIRE_DAMAGE = 17,
		FLAG_IMMUNE_TO_MAGIC = 18,
		FLAG_IMMUNE_TO_POISON = 19,
		FLAG_UNDEAD = 20,
		FLAG_IMMUNE_TO_SLEEP = 21
	};

private:
	class MagicThread;
	class UseThread;
	friend class MagicThread;
	friend class UseThread;

	// Saved in the objlist
	uint16 _vanishObjN = 0;
	uint16 _vanishFrameN = 0;
	MapCoord _moonstones[8];
	uint16 _kegTimer = 0;

	bool _armageddon = false;
	bool _avatarDied = false;
	bool _showStealing = false;
	bool _timeStopped = false;
	bool _updateVolcano = false;

	// Combat positions, worked out each time the actors are updated
	Actor *_rearMaxMonster = nullptr;
	Actor *_rearMinMonster = nullptr;
	Actor *_rearMaxParty = nullptr;
	Actor *_rearMinParty = nullptr;
	Actor *_frontTargetActor = nullptr;
	int _combatAvgX = -1;
	int _combatAvgY = -1;
	int _partyAvgX = -1;
	int _partyAvgY = -1;
	int _numMonstersNear = 0;

	static const ActorStats ACTOR_STATS[];
	static const uint ACTOR_STATS_COUNT;
	static const ObjValue WEAPON_RANGES[];
	static const uint WEAPON_RANGES_COUNT;
	static const ObjValue WEAPON_DAMAGE[];
	static const uint WEAPON_DAMAGE_COUNT;
	static const ObjValue ARMOUR[];
	static const uint ARMOUR_COUNT;
	static const ProjectileInfo PROJECTILES[];
	static const uint PROJECTILES_COUNT;
	static const uint8 PROJECTILE_OFFSETS[2][121];

	static const ActorStats *getActorStats(uint16 objN);
	static bool statFlag(uint16 objN, int flag);
	static const ProjectileInfo *getProjectileInfo(uint16 objN);

	// init.lua
	void playSfx(uint16 sfxId, bool async = false) const;
	static bool alignmentIsEvil(int align);
	static int actorFindMaxXYDistance(const Actor *actor, int x, int y);
	static int dirX(NuvieDir direction);
	static int dirY(NuvieDir direction);
	static Actor *getActor(uint8 actorNum);
	static Actor *newTempActor(uint16 objN, int x, int y, int z, ActorAlignment alignment = ACTOR_ALIGNMENT_NEUTRAL,
		uint8 worktype = WT_ASSAULT);
	static void removeInventoryObj(Actor *actor, Obj *obj);
	static void dismountActor(Actor *actor);
	static void unreadyObj(Actor *actor, Obj *obj);
	static void partyUpdateLeader();
	static void addChestItem(Obj *chest, uint16 objN, uint8 frameN, int qty, int quality);
	static Obj *findVolcanoNearPlayer();
	void playerSubtractKarma(int k);
	void partyHeal();
	void explosion(uint16 tileNum, int x, int y, Common::Array<MapEntity> &hitEntities);
	void projectile(uint16 tileNum, int startX, int startY, int endX, int endY, int speed, int spin = 0);
	void fadeObjBlue(Obj *obj);
	void fadeActorBlue(Actor *actor);
	void fadeCasting(Actor *caster);
	void fadeObjOut(Obj *obj);
	void fadeObjIn(Obj *obj);
	void fadeActorIn(Actor *actor);
	void fadeTile(int x, int y, int z, int fromTile, int toTile);
	static bool isReadiableTile(uint16 tileNum);
	void explodeKeg();
	void explodeObj(Obj *obj, Actor *actor = nullptr);
	void explodeSurroundingObjects(int x, int y, int z);
	void setArmageddon(bool value);

	// actor.lua
	static int getWeaponDmg(uint16 objN);
	int actorGetAc(Actor *actor);
	Obj *actorSelectObjFromList(Actor *actor, const uint16 *objNums, uint count);
	void acidSlugDissolveItem(Actor *target);
	void gremlinStealItem(Actor *target);
	void actorInitInventory(Actor *actor, const ActorStats *base);
	void actorUseEffectOnActor(Actor *actor, Obj *effect);
	void actorTakeHit(Actor *attacker, const Target &defender, int maxDmg);
	int actorHitTarget(const Target &defender, int maxDmg, Actor *attacker, bool noHitAnim = false);
	void actorHitMsg(Actor *actor);
	void actorDead(Actor *actor);
	void slimeDivide(Actor *slime);
	void slimeUpdateFrames();
	void actorPutToSleep(Actor *actor);
	static bool actorCanTurnInvisible(uint16 objN);
	bool actorIntCheck(Actor *defender, Actor *attacker);
	bool actorCombatHitCheck(Actor *attacker, const Target &foe, const Target &weapon);
	int actorStrAdjusted(Actor *actor);
	int actorDexAdjusted(Actor *actor);
	int actorIntAdjusted(Actor *actor);
	void actorUpdateFlags(Actor *actor);
	void actorCorpserRegurgitation(Actor *actor);
	bool actorRemoveCharm(Actor *actor);
	void actorYellForHelp(Actor *attacker, const Target &defender, int dmg);
	void activateCityGuards();
	void getLBToThrone();
	void actorAvatarDeath();
	void actorCalculateAvgCoords();
	void actorTileDamage(Actor *actor, uint16 mapTile);

	// spell helpers shared by the magic scripts
	int spellPutActorToSleep(Actor *attacker, Actor *foe);
	int spellPoisonActor(Actor *attacker, Actor *foe);
	void spellTakeFireDmg(Actor *attacker, Actor *foe);
	bool spellCharmActor(Actor *attacker, Actor *foe);
	int spellKillActor(Actor *attacker, Actor *foe);
	bool spellHitActor(Actor *attacker, Actor *foe, int spellNum);

	// combat
	Target actorGetWeapon(Actor *attacker, const Target &foe);
	void magicCastSpell(uint8 spellNum, Actor *caster, const MapCoord *target);
	void actorAttackTarget(Actor *attacker, int targetX, int targetY, int targetZ, const Target &weapon, Target foe);
	void combatRangeWeapon(Actor *attacker, int targetX, int targetY, int targetZ, const Target &weapon, int numBolts,
		Common::Array<MapCoord> &collTiles, Common::Array<Target> &collFoes);

	// worktypes
	bool moveTanglevine(Actor *actor, NuvieDir newDirection);
	void wtFront(Actor *actor);
	int wtFrontPosition(Actor *actor);
	void wtRear(Actor *actor);
	void wtFlank(Actor *actor);
	void wtBerserk(Actor *actor);
	void wtGuardArrestPlayer(Actor *actor);
	void wtCombatTanglevine(Actor *actor);
	void wtCombatStationary(Actor *actor);
	void wtWalkStraight(Actor *actor);
	void wtWanderAround(Actor *actor);
	void moveTowardsPlayer(Actor *actor);
	void wtAttack(Actor *actor);
	void wtTimid(Actor *actor);
	void wtLike(Actor *actor);
	void wtUnfriendly(Actor *actor);
	void wanderNearby(Actor *actor, int actorX, int actorY);
	static bool hasWorktypeHandler(int worktype);
	void wtWalkToLocation(Actor *actor);
	void wtSleep(Actor *actor);
	void wtBeg(Actor *actor);
	void wtBrawling(Actor *actor);
	bool actorOkToAttack(Actor *actor, Actor *target);
	Actor *actorFindTarget(Actor *actor);
	bool actorCatchUpToParty(Actor *actor);
	void caughtByGuard(Actor *actor);
	void performWorktype(Actor *actor);
	void subtractMovementPts(Actor *actor, int pts);

	// usecode.lua
	void useSilverHorn(Actor *actor);

protected:
	const ObjValue *weaponRanges(uint &count) const override;

public:
	explicit U6Scripts(Configuration *config);
	~U6Scripts() override {}

	bool loadGame(NuvieIO *objlist) override;
	bool saveGame(NuvieIO *objlist) override;

	bool playerPass() override;

	bool actorUpdateAll() override;
	bool actorInit(Actor *actor, ActorAlignment alignment) override;
	bool actorAttack(Actor *actor, MapCoord location, Obj *weapon, Actor *foe) override;
	bool actorMapDmg(Actor *actor, MapCoord location) override;
	bool actorTileDmg(Actor *actor, uint16 tileNum) override;
	bool actorHit(Actor *actor, uint8 dmg, bool displayHitMsg) override;
	uint8 actorStrAdj(Actor *actor) override;
	uint8 actorDexAdj(Actor *actor) override;
	uint8 actorIntAdj(Actor *actor) override;
	uint8 actorGetMaxMagicPoints(const Actor *actor) override;
	bool actorGetObj(Actor *actor, Obj *obj, Obj *container) override;
	bool actorSubtractMovementPoints(Actor *actor, uint8 points) override;
	bool actorResurrect(Actor *actor) override;
	bool actorUseEffect(Obj *effectObj, Actor *actor) override;
	bool actorMove(Actor *actor, NuvieDir direction, int flag) override;
	int actorMoveDiagonal(Actor *actor, NuvieDir xDirection, NuvieDir yDirection) override;

	bool lookObj(Obj *obj) override;
	bool useKeg(Obj *obj) override;
	bool hasUsecode(Obj *obj, UseCodeEvent usecodeType) override;
	ScriptThread *useObj(Obj *obj, Actor *actor) override;
	bool moveObj(Obj *obj, sint16 relX, sint16 relY) override;
	bool canGetObjOverride(Obj *obj) override;
	bool outOfAmmo(Actor *attacker, Obj *weapon, bool printMessage) override;
	bool isAvatarDead() override;
	bool magicGetSpellList(Spell **spellList) override;
	ScriptThread *magicCastThread(const char *invocation) override;

	bool moonstoneSetLoc(uint8 phase, MapCoord location) override;
	MapCoord moonstoneGetLoc(uint8 phase) override;
	bool updateMoongates(bool visible) override;
	bool advanceTime(uint16 minutes) override;
	bool setGShowStealing(bool stealing) override;

	/** The intro and ending are still Lua scripts, so the Lua scripts need loading */
	bool needsLua() const override {
		return true;
	}

	bool playIntro() override;
	bool playEnding() override;
};

} // End of namespace U6
} // End of namespace Scripts
} // End of namespace Ultima6
} // End of namespace Ultima

#endif
