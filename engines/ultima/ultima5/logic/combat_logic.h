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

#ifndef ULTIMA5_LOGIC_COMBAT_LOGIC_H
#define ULTIMA5_LOGIC_COMBAT_LOGIC_H

#include "common/scummsys.h"
#include "ultima/ultima5/data/savegame.h"
#include "ultima/ultima5/views/message_log.h"

namespace Ultima {
namespace Ultima5 {

constexpr int COMBAT_SIZE = 11;
constexpr int COMBAT_ENTITIES = 32;
constexpr int COMBAT_MAP_STRIDE = 32;
constexpr int COMBAT_MAP_BYTES = 0x160;

/**
 * A fight on a map of 11x11 tiles, between the party and a group of enemies. The
 * fighters take their turns in order of how quick they are. The turns of the party
 * wait here for the player's commands
 */
class PartyLogic;

class CombatLogic {
	friend class PartyLogic;

public:
	// What a fighter is, and is going through
	enum Flags {
		FLAG_PLAYER = 0x80,
		FLAG_MONSTER = 0x40,
		FLAG_DEAD = 0x20,
		FLAG_INVISIBLE = 0x10,
		FLAG_ASLEEP = 0x08,
		FLAG_SWALLOWED = 0x04,
		FLAG_FLEEING = 0x02,
		FLAG_CHARMED = 0x01
	};

	struct Entity {
		byte _hp = 0;
		byte _dex = 0;
		byte _flags = 0;
		byte _entity = 0;	// Which member of the party, or which kind of enemy
		byte _actor = 0;	// The object on the map for it
		byte _timer = 0;	// Turns until it acts
		byte _x = 0;
		byte _y = 0;
	};

	// Where the fight stands
	enum State {
		STATE_COMMAND,		// A member of the party is to be told what to do
		STATE_FINISHED
	};

	// What came of a key pressed for a member of the party
	enum Outcome {
		OUTCOME_INVALID,	// Not a command
		OUTCOME_DONE,		// The turn is over
		OUTCOME_AGAIN,		// The command didn't use up the turn
		OUTCOME_AIM,		// A target is to be picked
		OUTCOME_SUBCOMMAND,	// A list or a picture is to be shown by the caller
		OUTCOME_CHOICE,		// Up or down is to be chosen
		OUTCOME_DIRECTION	// A direction is to be chosen
	};

	// The ways of leaving a fight
	enum Exit {
		EXIT_NONE = 0,
		EXIT_UP = 5,
		EXIT_DOWN = 6
	};

	// The keys, as the original gives them
	enum Key {
		KEY_LEFT = 1,
		KEY_RIGHT = 2,
		KEY_UP = 3,
		KEY_DOWN = 4
	};

	// What the fights of the game are won or lost to
	// The way out of a fight that ends the game
	static constexpr int EXIT_ENDGAME = 0x4D;

	enum Kind {
		KIND_NORMAL = 1,
		KIND_DUNGEON_ROOM = 0x82,
		KIND_CAMP = 4
	};

private:
	// What is asked of the stats of a fighter
	enum Stat {
		STAT_INT = -1,
		STAT_DEX = -2,
		STAT_STR = -3,
		STAT_DEF = -4
	};

	// Where in a turn the fight is
	enum Stage {
		STAGE_SCAN,
		STAGE_COMMAND,
		STAGE_AFTER,
		STAGE_DONE
	};

	Views::MessageLog &_log;

	byte _map[0x400] = {};
	Entity _entities[COMBAT_ENTITIES];
	Data::ActorRecord _actors[COMBAT_ENTITIES];
	byte _partyX[6] = {};
	byte _partyY[6] = {};
	byte _monsterX[16] = {};
	byte _monsterY[16] = {};

	byte _origin = 0;			// The map the party came from
	int _flags = KIND_NORMAL;	// The kind of fight
	int _exitCode = 0;			// Which way the party left
	bool _won = false;
	int _result = 1;
	byte _savedActive = Data::NO_ACTIVE_CHARACTER;

	Stage _stage = STAGE_SCAN;
	int _current = 0;			// Whose turn it is
	int _scratchX = 0;
	int _scratchY = 0;
	int _item = 0;				// The weapon in use
	int _outcome = 0;			// What a blow did, for its report
	bool _spell = false;
	bool _special = false;
	int _interferer[COMBAT_ENTITIES];
	byte _lastKey = 0xFF;
	int _armed = 0;
	bool _bothLadders = false;	// The room has a ladder to go either way
	int _lastTargetX = 0;
	int _lastTargetY = 0;

	// Aiming
	int _aimX = 0;
	int _aimY = 0;
	int _aimRange = 0;
	int _aimDistance = 0;
	int _queue[3] = {};
	int _queueSize = 0;
	int _queuePos = 0;
	int _queueItem = 0;

	void say(const char *text);
	void newline();
	void sayName(int idx);

	// Looking at a fighter
	bool isEnemySide(int idx) const;
	int stat(int idx, int which);
	int armour(int who) const;
	bool canStand(int tile, int x, int y);
	bool isBlocked(int x, int y, int idx);
	bool boxedIn(int idx);
	bool inside(int x, int y) const {
		return x >= 0 && y >= 0 && x < COMBAT_SIZE && y < COMBAT_SIZE;
	}
	int distance(int a, int b);
	static int distancePoints(int ax, int ay, int bx, int by);
	int findFighter(int x, int y);
	void countSides(int &enemies, int &party);
	int uncharm();

	// Setting up and taking out fighters
	void clearFighters();
	int place(int what, int type, int x, int y);
	void placeParty();
	void placeEnemies(int kind, bool lone, bool shuffle = false);
	static void regenerate();
	void removeFighter(int idx);
	void fallAsleep(int idx);
	void wakeUp(int idx);
	void hideIfInvisible(int idx);
	bool unequip(int who, int item);
	int unequipAll(int item);

	// The turns of the enemies
	void monsterTurn();
	int monsterAttack(int idx);
	int monsterShoot(int idx, int target, bool protect);
	int pickTarget(int idx);
	bool monsterMove(int idx);
	bool randomSpot();
	bool monsterSpecial(int idx);
	void hazards(int idx);

	/**
	 * Takes away a fighter who stands at the mirror in the last room, which is the way out of the game
	 */
	void checkMirror();
	void regurgitate(int idx);
	void upkeep();
	void burnOut();

	// How blows land
	bool hits(int target, int attacker, int item);
	int blowDamage(int attacker, int target);
	int takeDamage(int idx, int amount);
	void poison(int target, int attacker);
	void strike(int target, int attacker);
	int woundLevel(int idx);
	void report(int idx, int attacker);
	void missed(int idx);
	void spreadNear(int x, int y);
	int project(int attacker, int x, int y, bool landed, int kind);
	bool pathClear(int fromX, int fromY, int toX, int toY, int kind);
	bool triggers(int x, int y);
	void meleeBlow(int attacker, int target, int item);
	void useAmmunition(int item, int range);
	void giveExperience(int idx, int amount);

	// Commands of the party
	bool prompt(bool header);
	bool beginTurn();
	void endTurn(int key);
	Outcome attack();
	Outcome nextWeapon();
	Outcome movePlayer(int key);
	Outcome klimb();
	Outcome klimbAt(int tile);
	int objectTileAt(int x, int y) const;
	bool leaveBattle(int idx, int code);
	bool setActive(int who);
	int armedWith(int item, bool first, Common::String &list);
	void aimStart(int attacker, int range);

public:
	CombatLogic(Views::MessageLog &log);

	/**
	 * Picks the map for a fight
	 * @param ground		The tile of the map the enemy is on
	 * @param partyTile		The tile the party is shown as
	 * @param enemyTile		The tile of the enemy
	 * @param mapId			The map the party is in
	 * @returns				The number of the map, in the file of the maps of the fights
	 */
	static int mapFor(byte ground, byte partyTile, byte enemyTile, byte mapId);

	/**
	 * Follows a shot from one place to another over a map of tiles with 32 to a row, as the
	 * fights and the open country do
	 * @param lastX,lastY	Where it got to, going in as where the last one got to
	 * @returns				False if something was in the way
	 */
	static bool traceProjectile(int fromX, int fromY, int toX, int toY, int kind, const byte *tiles, int &lastX, int &lastY);

	/**
	 * Returns which kind of enemy a tile of the objects on the map is
	 */
	static int enemyKind(byte tile);

	/**
	 * Loads a map of the fights from a file of them
	 * @returns		False if it couldn't be read
	 */
	bool loadMap(const char *filename, int offset);

	/**
	 * Sets up a fight with a group of a kind of enemy on the map that's been loaded
	 * @param kind		The kind of enemy
	 * @param lone		Whether a lone enemy is met in places where they come one at a time
	 * @param flags		The kind of fight
	 * @param origin	The map the party is in
	 */
	void begin(int kind, bool lone, int flags, byte origin);

	// What came of a night's rest
	enum CampResult {
		CAMP_NO_EFFECT,
		CAMP_RESTED,
		CAMP_AMBUSHED
	};

	/**
	 * Has the party sleep for a number of hours on the loaded map, which can be broken by an ambush
	 * that leads straight into the fight
	 * @param guard			Which of the party keeps watch, or -1
	 * @param apparition	Set if the party is visited as it rested
	 */
	CampResult camp(int guard, int hours, byte origin, bool &apparition);

	/**
	 * Sets up a fight in a dungeon on the map that has been put in place: where the party and the
	 * enemies start, and what stands about in a room
	 * @param facing		Which way the party faces
	 * @param cell			What the cell of the dungeon is, for rooms
	 * @param level			The level of the dungeon
	 * @param withEnemies	Whether the map's enemies are put in, as they are for rooms that hold some
	 * @param ladders		Whether the room has ladders both ways
	 */
	void beginDungeon(byte origin, int flags, int facing, int cell, int level, bool withEnemies, bool ladders);

	/**
	 * Carries on with a Klimb of the party once up or down is chosen, or on a ladder
	 */
	Outcome klimbChosen(bool down);

	/**
	 * Carries on with a Klimb that asked for a direction
	 */
	Outcome klimbDirection(int dx, int dy, bool cancelled);

	/**
	 * Carries on until a member of the party is to be told what to do, or the fight is over
	 */
	State advance();

	/**
	 * Starts a spell that is aimed, which is shot as the fight's things that are, with a target to pick
	 */
	Outcome castAimed(int item);

	/**
	 * Wakes the fighter that a member of the party is, if they are asleep
	 */
	void wakeMember(int member);

	/**
	 * Tells if an enemy next to the one whose turn it is stops them from casting a spell, and says so
	 */
	bool spellInterference();

	/**
	 * Tells the member of the party whose turn it is what to do
	 * @param key		The key, as the original gives it
	 */
	Outcome command(int key);

	/**
	 * Carries on after what a command asked of the caller has been done
	 * @param key		The key that began the command
	 */
	void commandDone(int key);

	// Picking a target
	void aimMove(int dx, int dy);
	int aimX() const {
		return _aimX;
	}
	int aimY() const {
		return _aimY;
	}
	bool aimOnSelf() const;
	int aimRange() const {
		return _aimRange;
	}

	/**
	 * Accepts the target aimed at
	 * @returns		False if nothing is aimed at, which can't be accepted
	 */
	bool aimAccept();

	/**
	 * Fires at what's aimed at, and goes on to the next weapon
	 */
	Outcome aimFire();

	/**
	 * Gives up on aiming, which still uses the turn
	 */
	Outcome aimCancel();

	// What the fight looks like
	int current() const {
		return _current;
	}
	const Entity &entity(int idx) const {
		return _entities[idx];
	}
	const Data::ActorRecord &actor(int idx) const {
		return _actors[idx];
	}
	byte tile(int x, int y) const {
		return _map[y * COMBAT_MAP_STRIDE + x];
	}
	int exitCode() const {
		return _exitCode;
	}
	bool won() const {
		return _won;
	}

	/**
	 * Returns 0 if the party left having won, and 1 otherwise
	 */
	int result() const {
		return _result;
	}

	/**
	 * Puts the savegame back as it was outside the fight
	 */
	void finish();

	// For testing
	Entity &entityRef(int idx) {
		return _entities[idx];
	}
	Data::ActorRecord &actorRef(int idx) {
		return _actors[idx];
	}
	byte *mapData() {
		return _map;
	}
};

} // namespace Ultima5
} // namespace Ultima

#endif
