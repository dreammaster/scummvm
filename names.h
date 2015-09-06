/* RogueVM - RPG Engines
 *
 * RogueVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#ifndef AESOP_NAMES_H
#define AESOP_NAMES_H

#include "aesop/shared.h"

namespace Aesop {

#define MAXBYTE 127
#define MAXWORD 32767
#define MAXLONG 2147483647


	//ллллллллллллллллллллллллллллллллллллллллллл
	//лл                                       лл
	//лл Application event types               лл
	//лл                                       лл
	//ллллллллллллллллллллллллллллллллллллллллллл

	//
	// AESOP event usage:
	//
	//  post_event(owner,event,param);
	//
	//  notify(object,"message",event,param);
	//  cancel(object,"message",event,param);
	//
	//  message "message",param,owner
	//

#define YX <<16|                             // [paramXY] access macro

#define EV_ENTER_SQUARE           32         // [owner] entered [paramXY]
#define EV_EXIT_SQUARE            33         // [owner] exited [paramXY]
#define EV_MONSTER_COMMAND        34         // order issued to all monsters
#define EV_FEATURE_CLICKED        35         // [owner] clicked by user
#define EV_LEVER                  36         // [owner] state became [param]
#define EV_UNLOCKED               37         // [owner] unlocked by user
#define EV_BUTTON                 38         // [owner] state became [param]
#define EV_PRESSURE_PLATE         39         // [owner] state became [param]
#define EV_FORCED_MOVE            40         // [owner] moved by [param]
#define EV_ITEM_TAKEN             41         // [param] taken from floor
#define EV_ITEM_THROWN            42         // [param] thrown
#define EV_ITEM_EXPLODED          43         // [owner] damaged [param]
#define EV_OPEN_DOOR              44         // [owner] forces door [paramXY]
#define EV_FEATURE_HIT            45         // [owner] hit by weapon [param]
#define EV_PARTY_TURN             46         // party turns in [paramXY]
#define EV_ITEM_USED              47         // [owner] was used by [param]
#define EV_NICHE_REMOVED          48         // [owner] removed from [param]
#define EV_NICHE_PLACED           49         // [owner] placed in [param]
#define EV_GLOBAL_EXPIRE          50         // global spell expiration event
#define EV_REQUEST_DESTRUCTION    51         // [owner] requests destruction
#define EV_PLAYER_HIT             52         // [owner] hit [param]
#define EV_PLAYER_DMG             53         // [owner] suffered [param] dmg
#define EV_PLR_AGGRESSION         54         // [param] committed act of agg.
#define EV_SPELL_CAST             55         // [param] cast by [owner]
#define EV_PLR_DMG_NPC            56         // [param] hit for [owner] dmg
#define EV_DEFEAT                 57         // party incapacitated or killed
#define EV_TELEPORT               58         // [param] teleported by [owner]
#define EV_PARTY_ENTER_SQUARE     59         // party enters square [param]
#define EV_PARTY_EXIT_SQUARE      60         // party exits square [param]
#define EV_PARTY_RESTED           61         // party rested in camp
#define EV_RESTORED               62         // game context restored
#define EV_TREES_CUT              63         // [owner] cut down with [param]
#define EV_TELEPORT_TRIGGERED     64         // Teleport Trigger

	//ллллллллллллллллллллллллллллллллллллллллллл
	//лл                                       лл
	//лл Application-specific definitions      лл
	//лл                                       лл
	//ллллллллллллллллллллллллллллллллллллллллллл

#define MAXSTRLEN     64  // Max # of chars supported by "input" class

#define CURSOR_RATE   10  // Input cursor blink rate in 1/30 second intervals

#define SNUM_FLOOR    18  // Shape # of floor/ceiling backdrop in wallset

#define NFEATURES     12  // # of simultaneous clickable features

	//
	// Item draw flags
	//

#define DRAW_THROWN_ONLY  1
#define DRAW_FLOOR_ONLY   2
#define DRAW_ALL          3

	//
	// Kernel & program object indices
	//

#define kernel   0

#define startup  2000
#define dungeon  2001
#define camp     2002
#define tables   2003
#define utils    2004
#define magic    2005

	//
	// Global spell expiration event (EV_GLOBAL_EXPIRE) types
	//

#define SEX_CAMP           1     // Camp resting caused spell expiration
#define SEX_NEWLVL         2     // New PC level: expire if necessary
#define SEX_DISPEL         3     // Dispel Magic cast at [ownerXY]
#define SEX_END_LIFE       4     // Object created by spell no longer exists

	//
	// Time intervals
	//

#define HEARTBEAT_INTERVAL 6     // (6  * 33.333 ms. = 200 ms.)
#define SCRIPT_INTERVAL    6     // (6  * 33.333 ms. = 200 ms.)
#define SPELL_INTERVAL     30    // (30 * 33.333 ms. = 1000 ms.)

	//
	// Note: PCs and event scripts receive service at 200 ms. intervals
	//
	//       Kernel receives service at AESOP system rate of 33 1/3 ms.
	//
	//       NPCs receive service at 33.333 * SPEED ms. intervals
	//
	//       Spells receive service at 1000 ms. intervals
	//

#define TIME_SECOND        5     // # of 200 msec. ticks/second
#define TIME_MINUTE        300   // # of 200 msec. ticks/minute

#define TIME_BREATH        (TIME_SECOND*3)

#define SPELL_ROUND        60    // 1 round = 1 minute
#define SPELL_TURN         600   // 1 turn = 10 minutes
#define SPELL_HOUR         3600  // 1 hour = 60 minutes
#define SPELL_SECOND       1     // 1 second = 1 spell interval

	//
	// Character selection equates
	//

#define NEED_MEMORIZE      0
#define NEED_PRAY          1
#define NEED_DROP          2
#define NEED_SCRIBE        3
#define NEED_RECIPIENT     4
#define NEED_READ          5

	//
	// Index of special "hands" object -- used to implement the characteristics
	// of unarmed attacks
	//

#define hands    15

	//
	// Text window names
	//

#define TW_MAIN    0    // Main text window at bottom of screen
#define TW_PARTY   1    // Party/character window
#define TW_CAMP    2    // Main camp menu area
#define TW_CDIALOG 3    // Camp menu dialog area
#define TW_MAGIC   4    // Magic menu area
#define TW_OUT     5    // Outtake dialog area

#define TW_1       24   // Auxiliary windows for temporary assignment
#define TW_2       25
#define TW_3       26
#define TW_4       27
#define TW_5       28
#define TW_6       29
#define TW_7       30
#define TW_8       31

	//
	// Other application-specific equates
	//

#define MAX      0
#define MIN      1

	//
	// Shortcut macro to get object attributes from derived classes
	//

#define GET )send_message(this,"report",

	//
	// Shortcut macro to get a player #'s instance handle if qualified
	//

#define QUALIFY )send_message(kernel,"qualify",

	//
	// Class names & weapon usage bits
	//

#define NOCLASS  -1
#define FIGHTER   0
#define MAGE      1
#define CLERIC    2
#define THIEF     3
#define PALADIN   4
#define RANGER    5

#define USABLE_FIGHTER  1
#define USABLE_MAGE     2
#define USABLE_CLERIC   4
#define USABLE_THIEF    8
#define USABLE_PALADIN  16
#define USABLE_RANGER   32
#define USABLE_WARRIOR  (USABLE_FIGHTER + USABLE_PALADIN + USABLE_RANGER)
#define USABLE_HOLY_MAN (USABLE_PALADIN + USABLE_CLERIC)
#define USABLE_ANY      -1

#define ST_MAGE   0     // spell types
#define ST_CLERIC 1

	//
	// Status equates:
	//
	// SP_DEAD   = Spell is inactive, and can be destroyed.
	//
	// SP_ACTIVE = Spell is active, and must not be externally destroyed
	//

#define SP_DEAD   0
#define SP_ACTIVE 2

	//
	// Flags used to select qualified players based on class, race, sex, etc.
	//

#define SPEC_ONLY  1
#define ANYONE_OK  2

	//
	// Saving throw type & result equates
	//

#define ST_DEATH         0    // these are table indices -- don't reorder!
#define ST_POISON        0
#define ST_WAND          1    
#define ST_PETRIFICATION 2
#define ST_BREATH        3
#define ST_SPELL         4
#define ST_NONE          5

#define SR_HALF_DAM      1
#define SR_QUARTER_DAM   2
#define SR_NO_DAM        32
#define SR_NONE          32

	//
	// Damage & effect types
	//

#define DAM_MAGIC          0x00000001   // magic related damage
#define DAM_ELECTRICAL     0x00000002   // electrical damage 
#define DAM_FALLING        0x00000004   // falling damage  
#define DAM_COMBAT         0x00000008   // close combat damage
#define DAM_MISSILE        0x00000010   // missile damage  
#define DAM_FIRE           0x00000020   // fire damage   
#define DAM_COLD           0x00000040   // cold damage   
#define DAM_ACID           0x00000080   // acid damage   
#define DAM_WEAPON         0x00000100   // weapon damage  
#define DAM_ABSOLUTE       0x00000200   // absolute damage  
#define DAM_POISON         0x00000400   // poison damage  
#define DAM_EXPERIENCE     0x00000800   // give PC experience for damage
#define DAM_DROWNING       0x00001000   // drowning damage
#define DAM_MISSILE_WEAPON 0x00000110   // missile weapon damage

#define EFFECT_HOLD_PERSON   0
#define EFFECT_HOLD_MONSTER  1
#define EFFECT_HOLD_UNDEAD   2
#define EFFECT_PARALYZATION  3
#define EFFECT_DEATH         4
#define EFFECT_DISINTEGRATE  5
#define EFFECT_PETRIFICATION 6
#define EFFECT_FEAR          7
#define EFFECT_POISON        8
#define EFFECT_SLOW          9
#define EFFECT_STUN          10

	//
	// Player status and qualification flags
	// 

#define POISONED      2       // (used by "inflict effect")
#define PARALYZED     4       // (used by "inflict effect")
#define PETRIFIED     8
#define GASSED        16
#define INELIGIBLE    32
#define SELECTED      64

#define DRAINED       128     // (used only by "inflict effect"; flag ignored)
#define WEAKENED      256     // (used only by "inflict effect"; flag ignored)

#define IN_PARTY      0       // check for handle validity only
#define ALIVE         2       // check for hp>-10 true
#define AWAKE         4       // check for hp>0 true
#define UNPARALYZED   8       // check for paralyzed false
#define UNPOISONED    16      // check for poisoned false
#define UNPETRIFIED   32      // check for poisoned false
#define FED				 64      // check for food>0 true
#define ELIGIBLE      128     // check for ineligibility flag = 0

	//
	// Global monster commands
	//

#define MC_CHARGE     0       // assume MS_HOSTILE,MM_PURSUE attitude
#define MC_RETREAT    1       // set MS_RETREATING,MM_PURSUE attitude
#define MC_ALARM      2       // all MM_GUARDs CHARGE
#define MC_CRYHELP    3       // all MM_PEACEFULs CHARGE
#define MC_DIE        4       // all NPCs "die" (for test purposes)

	//
	// Monster status flags
	// 

#define MS_HOSTILE    0x00000001  // Monster will attack party on sight
#define MS_ATTACKED   0x00000002  // Monster has been attacked by party
#define MS_RETREATING 0x00000004  // Monster is attempting to flee party
#define MS_HELD       0x00000008  // Monster is magically "held"
#define MS_TURNED     0x00000010  // Monster has been turned by a cleric
#define MS_INVISIBLE  0x00000020  // Monster cannot be seen by the unaided eye
#define MS_PETRIFIED  0x00000040  // Monster has been turned to stone
#define MS_POISONED   0x00000080  // Monster is dying of poison
#define MS_PAIN       0x00000100  // Monster has taken damage -- flash white
#define MS_ATKTURN    0x00000200  // Monster can attack this turn
#define MS_SLOWED     0x00000400  // Monster is magically "slowed"
#define MS_WILTED     0x00000800  // Monster was hit by dessication attack
#define MS_CASTONCE   0x00001000  // Monster cast spell once

	//
	// Monster mission parameters
	//

#define MM_PURSUE     0           // Seek party's square
#define MM_GUARD      1           // Set MS_HOSTILE, MM_PURSUE on sight
#define MM_STATIONARY 2           // Remain at post awaiting orders
#define MM_PEACEFUL   3           // Ignore party unless attacked

	//
	// Monster profile flags
	//

#define MP_FACEATK    0x00000001  // Monster must face opponent to attack
#define MP_FACEMOVE   0x00000002  // Monster must face direction of movement
#define MP_EXPLODE    0x00000008  // Monster explodes when hit
#define MP_HOLDP      0x00000010  // Monster affected by HOLD PERSON
#define MP_HOLDM      0x00000020  // Monster affected by HOLD MONSTER
#define MP_HOLDU      0x00000040  // Monster affected by HOLD UNDEAD
#define MP_TURNABLE   0x00000080  // Monster affected by TURN UNDEAD
#define MP_FRONTFLIP  0x00000100  // Monster's front view may be flipped
#define MP_RANGE      0x00000200  // Monster can make range attacks
#define MP_OPENDOOR   0x00000400  // Monster can open doors
#define MP_RNDMOVE    0x00000800  // Monster does God only knows what
#define MP_MULTHIT    0x00001000  // Monster can hit multiple players at once
#define MP_FLYING     0x00002000  // Monster is buoyant/airborne type
#define MP_UNDEAD     0x00004000  // Monster is undead
#define MP_SEEKRING   0x00008000  // Monster controlled by Ring of Trobriand
#define MP_POISON     0x00100000  // Monster poisons victims
#define MP_DESTROYWPN 0x00200000  // Monster consumes metal weapons
#define MP_DRAIN      0x00400000  // Monster drains levels
#define MP_PARALYZE   0x00800000  // Monster paralyzes
#define MP_WEAKEN     0x01000000  // Monster drains strength
#define MP_INVISIBLE  0x02000000  // Monster invisible except when attacking

	//
	// Monster defense (immunity) flags:
	//

#define IMMUNITY_HOLD_P           0x00000001
#define IMMUNITY_HOLD_M           0x00000002
#define IMMUNITY_PARALYZATION     0x00000004
#define IMMUNITY_FEAR             0x00000008
#define IMMUNITY_DEATH_MAGIC      0x00000010
#define IMMUNITY_MAGIC            0x00000020
#define IMMUNITY_ELECTRICAL       0x00000040
#define IMMUNITY_POISON           0x00000080
#define IMMUNITY_COLD             0x00000100
#define IMMUNITY_BLADED_WEAPON    0x00000200
#define IMMUNITY_NORMAL_WEAPON    0x00000400
#define IMMUNITY_ACID             0x00000800
#define IMMUNITY_FIRE             0x00001000
#define IMMUNITY_WEAK_MAGIC_WPN   0x00002000
#define IMMUNITY_STRONG_MAGIC_WPN 0x00004000
#define IMMUNITY_RESIST_COLD      0x00008000
#define IMMUNITY_TIME_STOP        0x00010000
#define IMMUNITY_DISINTEGRATE     0x00020000
#define IMMUNITY_BLUDGEON         0x00040000

	//
	// Monster size values
	//

#define SMALL         1           // Small monster
#define MEDIUM        2           // Medium monster
#define LARGE         3           // Large monster

	//
	// Feature profile flags (constant):
	//

#define FP_FRAGILE    0x00000001  // Feature is vulnerable to physical attacks
#define FP_XFLIP      0x00000002  // Feature is drawn horizontally flipped
#define FP_YFLIP      0x00000004  // Feature is drawn vertically flipped
#define FP_ALTSET     0x00000008  // Feature has secondary wallset frames
#define FP_FLOOR      0x00000010  // Floor feature if 1, else wall feature
#define FP_NOFVIEW    0x00000020  // Floor feature has no "floor" view
#define FP_CLICK      0x00000040  // Feature responds to mouse clicks
#define FP_NOITEMS    0x00000100  // No items droppable in feature's square 
#define FP_FCLICK     0x00000200  // Feature responds to clicks when faced

	//
	// Feature subflags for members of class "teleporters"
	//

#define TP_GROUND     0x00000001  // Do not teleport airborne entities
#define TP_FALL       0x00000002  // Inflict falling damage on party
#define TP_DOWN       0x00000004  // Print "Going down ..." when triggered
#define TP_UP         0x00000008  // Print "Going up ..." when triggered
#define TP_ENTER      0x00000010  // Print "Entering..." when triggered
#define TP_LEAVE      0x00000020  // Print "Leaving..." when triggered
#define TP_CLRSCRN    0x00000040  // Clear screen when triggered (new palette)

	//
	// Weapon effects/attack results
	//

#define AR_MISSED     -1      // Target missed/no target acquired
#define AR_CANT_REACH -2      // Target out of reach from user's position
#define AR_FIRED      -3      // Missile weapon thrown/fired
#define AR_NO_AMMO    -4      // Missile weapon out of ammo
#define AR_HACKED     -5      // Inanimate target "hacked"
#define AR_BASHED     -6      // Inanimate target "bashed"

	//
	// Entity types
	//

#define T_FEA 0               // Feature (decoration, furnishing, etc.)
#define T_ITM 1               // Item (portable)
#define T_NPC 2               // Non-player character (including monsters)
#define T_PLR 3               // Player character (or kernel)
#define T_SPL 4               // Spell supervisor instance

	//
	// Item profile flags
	//

#define IP_WEAPON    0x00000001  // Weapon subclass
#define IP_POLEARM   0x00000002  // Polearm (can attack from 2nd rank)
#define IP_MISSILE   0x00000004  // Missile (inherently thrown)
#define IP_BLUDGEON  0x00000008  // Blunt weapon (as opposed to edged)
#define IP_KEY       0x00000010  // Key subclass
#define IP_AIRSHP    0x00000020  // Draw alternate shape if airborne
#define IP_METAL     0x00000040  // Item is metallic in nature
#define IP_ARMOR     0x00000080  // Armor subclass
#define IP_QUIVER    0x00000100  // Item is carried in quiver
#define IP_RETURNING 0x00000200  // Missile weapon returns after collision
#define IP_SPELLBOOK 0x00000400  // Spellbook subtype (for verification)
#define IP_SCROLL    0x00000800  // Scroll subtype
#define IP_MAGHAND   0x00001000  // Magic hand weapon (attacks from 3rd rank)
#define IP_HOLYSYM   0x00002000  // Holy symbol subtype (for verification)
#define IP_RING      0x00004000  // Ring
#define IP_MAGIC     0x00008000  // Magical properties (glows under detection)
#define IP_SPELL     0x00010000  // Spell manifestation (special coll checks)
#define IP_SPELLWEAP 0x00020000  // Magically created hand weapon (can't drop)
#define IP_CENTER    0x00040000  // Item is drawn centered between L-R regions
#define IP_GLOW      0x00080000  // Item glows, and should not be faded
#define IP_2H        0x00100000  // Item requires both hands to use
#define IP_FOOD      0x00200000  // Item can be eaten (NUTRITION attribute)
#define IP_FLAMING   0x00400000  // Weapon does fire damage 
#define IP_GRENADE   0x01000000  // Item explodes upon collision
#define IP_AUTOHIT   0x02000000  // Wielder of weapon never misses
#define IP_SECHAND   0x04000000  // Weapon can be used in secondary hand
#define IP_SHIELD    0x08000000  // Armor is used in hand
#define IP_AUTOLOAD  0x10000000  // Missile weapon reloaded from inventory

	//
	// Item flags
	//
	// DA __ __ __ ID CU ST RT HL ER ET PL PL PL MD MD
	//
	// MD = missile direction 0-3 (if airborne)
	//
	// PL = player # who launched missile; 0 or 1-6 (if airborne)
	//
	// ET = explodes when picked up; glows red under True Seeing
	//
	// ER = "returning" item currently en route to target if 1, returning if 0
	//
	// HL = "returning" item launched from primary (0) / secondary (1) hand
	//
	// RT = "returning" item is reversing direction
	//
	// ST = item has not yet left its originating square
	//
	// CU = item is cursed and cannot be dropped once placed in a hand
	//
	// ID = item has not been identified; name and bonuses are not apparent
	//
	// DA = item is "disabled" (broken weapons, rotten food, etc.)
	//

#define ITM_MD 0b0000000000000011
#define ITM_PL 0b0000000000011100
#define ITM_ET 0b0000000000100000      
#define ITM_ER 0b0000000001000000
#define ITM_HL 0b0000000010000000
#define ITM_RT 0b0000000100000000
#define ITM_ST 0b0000001000000000
#define ITM_CU 0b0000010000000000
#define ITM_ID 0b0000100000000000
#define ITM_DA 0b1000000000000000

	//
	// Polymorphic attribute names (for use w/GET macro)
	//

#define TYPE      0       // Entity type
#define PROFILE   1       // Constant profile flag word
#define SUBFLAGS  2       // Optional flag word for derived classes
#define NAME      3       // Name resource
#define SHPTABLE  4       // Shape table resource name (features/NPCs)

	//
	// NPC-specific attribute names
	//

#define SPEED     1000    // Turn rate
#define PALETTE   1001    // Palette # (PAL_M1,PAL_M2)
#define SIZE      1002    // Size (S,M,L)
#define DEFENSE   1003    // Immunity/resistance flags
#define AC        1004    // Armor class
#define HITDICE   1005    // Hit dice
#define M_RESIST  1006    // Magic resistance
#define NUM_ATKS  1007    // # of attacks
#define ATK_NUM   1008    // # of dice to determine damage
#define ATK_DIE   1009    // # of sides for damage rolls
#define ATK_ADD   1010    // Bonus for damage rolls
#define THAC0     1011    // Minimum roll on d20 needed to hit AC 0
#define XP_VALUE  1012    // Experience point value
#define MOV_SND   1013    // Sound # made when moving
#define ATK_SND   1014    // Sound # made when attacking
#define RMOV_SND  1015    // 25% chance of sound when moving

	//
	// Item-specific attribute names
	//

#define ICON_SHP  2000    // Shape number for icon/mouse cursor (P_IN_HAND)
#define FLOOR_SHP 2001    // Shape number when lying on floor (P_ON_FLOOR)
#define AIR_SHP   2002    // Shape number when flying through air (P_AIRBORNE)
#define WEIGHT    2003    // Weight in grams
#define INVLOCS   2004    // Where it can be placed in inventory
#define CLASS     2005    // Which PC classes can use it (USABLE_ flags)

#define KEY_CODE  2006    // key code, needed only for IP_KEYs

	//
	// Weapon-specific attribute names
	//

#define LARGE_NUM 2100    // n x-sided die against large monsters
#define LARGE_DIE 2101    // n-sided die against large monsters
#define LARGE_ADD 2102    // x y-sided die + n against large monsters
#define SMALL_NUM 2103    // n x-sided die against small/medium monsters
#define SMALL_DIE 2104    // n-sided die against small/medium monsters
#define SMALL_ADD 2105    // x y-sided die + n against small/medium monsters
#define TRUENAME  2106    // Magical ("ego") name resource

	//
	// Armor-specific attribute names
	// 

#define AC_RATING 2200    // Base AC rating for armor
#define AC_MODIFY 2201    // AC modifier for shields, bracers, etc.

	//
	// Food-specific attribute names
	//

#define NUTRITION 2300    // (present only if IP_FOOD flag is set)

	//
	// Spell missile attribute names
	//

#define FLAGS     2400    // Spell attributes vis-a-vis missile flight
#define DAMTYPE   2401    // Type of damage to cause
#define COLOR     2402    // Dot pattern color (if applicable)
#define DMG_NUM   2403    // # of damage dice to roll
#define DMG_DIE   2404    // # of sides/damage die
#define DMG_ADD   2405    // Additional damage beyond rolled damage
#define DMG_MUL   2406    // Damage roll multiplier

	//
	// Feature-specific attribute names
	//

#define IMPEDANCE 3000    // 0=free, -1=impassable (parm1 = requestor)
#define TIMER     3001    // 0=no timer service, else timer rate * 1/30 sec.
#define STATES    3002    // # of possible valid states (state = 0...n-1)

	//
	// Lever-specific attribute names
	//

#define SHPSET    3101    // Shape set # used for given state

	//
	// Spell missile FLAGS bits
	//

#define AUTOHIT      0x01
#define GROUPHIT     0x02
#define ROLLHIT      0x04
#define SAVE         0x08
#define CANHIT       0x10
#define DOTPATTERN   0x20
#define MAGMISS      0x40
#define CONTINUE     0x80

	//
	// Explosion/vortex colors
	//

#define REDPATTERN   0
#define BLUEPATTERN  1
#define GREENPATTERN 2

#define ICE          0
#define DARKNESS     1
#define ACID         2

	//
	// Place definitions for "display" handlers
	//
	// Places 0-...+ = "owned" by entity
	//
	// 'place' variables for all non-item entities should be P_ON_FLOOR
	//

#define P_ON_FLOOR  -1  // Item, NPC, or feature associated with "floor"
#define P_IN_HAND   -2  // Item taken from floor and placed in "hand" (cursor)
#define P_AIRBORNE  -3  // Item thrown or cast, while in flight

	//
	// Valid item location flags (from Eye II)
	//

#define	LOCPACK		0x01		/* these are bitfields which allow the  	*/
#define	LOCCHEST		0x02		/*	program to determine in what inventory	*/
#define	LOCARMS		0x04		/*	locations a given item may be placed	*/
#define	LOCHANDS		0x08
#define	LOCFEET		0x10
#define	LOCHEAD		0x20
#define	LOCNECK		0x40
#define	LOCBELT		0x80
#define	LOCRING		0x100
#define	LOCANY		0xFFFF

	//
	// Character flags/indices
	//

#define SCR_ADVENT 0    // Current display = "adventure screen"
#define SCR_EQUIP  1    // Current display = "equipment screen"
#define SCR_CHAR   2    // Current display = "character screen"

#define INV_BACKPACK 0  // PC object data equates
#define INV_CHEST    14
#define INV_WRISTS   15
#define INV_PRIMHAND 16
#define INV_LFINGER  17
#define INV_RFINGER  18
#define INV_FEET     19
#define INV_SECHAND  20
#define INV_BELT1    21
#define INV_BELT2    22
#define INV_BELT3    23
#define INV_NECK     24
#define INV_HEAD     25

#define RACE_HUMAN      0
#define RACE_ELF        1
#define RACE_HALFELF    2
#define RACE_DWARF      3
#define RACE_GNOME      4
#define RACE_HALFLING   5
#define RACE_SPRITE     6
#define RACE_SAURIAL    7

#define RACE_NPC        8

} // ENd of namespace Aesop

#endif
