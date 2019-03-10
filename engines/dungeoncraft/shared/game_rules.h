/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
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

#ifndef DUNGEONCRAFT_SHARED_AMERULES
#define DUNGEONCRAFT_SHARED_GAMERULES

//#include "dungeoncraft/shared/char.h"
#include "dungeoncraft/shared/class.h"
#include "dungeoncraft/shared/gameevent.h"

namespace DungeonCraft {

/*
Valid Classes
  Humans cannot be multi-classed
     Cleric
     Fighter
     MU
     Thief
     Ranger
     Druid
     Paladin
  Dwarf	
    Cleric
    Fighter
    Thief
    Fighter/Thief	
    Fighter/Cleric
  Halfling
    Cleric
    Fighter
    Thief
    Fighter/Thief
  Elf	
    Cleric
    Fighter
    MU
    Thief
    Ranger
    Fighter/MU
    Fighter/Thief
    Fighter/MU/Thief
    MU/Thief
  Half-elf
    Cleric
    Druid
    Fighter
    Ranger
    MU
    Thief
    Cleric/Fighter
    Fighter/Druid
    Fighter/MU/Cleric
    Fighter/MU/Druid
    Cleric/Ranger
**->    Druid/Ranger ** not implemented anywhere!
    Cleric/MU
    Druid/MU	
    Fighter/MU   
    Fighter/Thief
    Fighter/MU/Thief   
    Thief/MU    
  Gnome	
    Fighter
    Thief
    Cleric
    MU
    Fighter/Thief
    Cleric/MU
*/

const int NUM_RACE_TYPES = 6;
const int NUM_GENDER_TYPES = 2;
const int NUM_ALIGNMENT_TYPES = 9;
const int NUM_CLASS_TYPES = 19;
//const int NUM_UNDEAD_TYPES = 14;
const int NUM_CHAR_STATUS_TYPES = 10;

enum raceType 
{ 
  Human=0, 
  Elf=1, 
  Dwarf=2, 
  Gnome=3, 
  HalfElf=4, 
  Halfling=5, 
  raceUnknown=6 
};

enum genderType 
{ 
  Male=0, 
  Female=1 
};

enum alignmentType 
{ 
  LawfulGood=0,    
  NeutralGood=1, 
  ChaoticGood=2, 
  LawfulNeutral=3, 
  TrueNeutral=4, 
  ChaoticNeutral=5,  
  LawfulEvil=6,    
  NeutralEvil=7, 
  ChaoticEvil=8 
};

enum classType 
{ 
  Fighter=0, 
  Cleric=1, 
  Ranger=2, 
  Paladin=3, 
  MagicUser=4, 
  Thief=5, 
  Druid=6,
	C_F=7, 
  C_F_MU=8, 
  C_R=9, 
  C_MU=10, 
  C_T=11, 
  F_MU=12, 
  F_T=13, 
  F_MU_T=14, 
  MU_T=15, 
  F_D=16, 
  D_MU=17, 
  F_MU_D=18, 
  BogusClass=19, 
  classUnknown=20 
};

//enum undeadType { NotUndead, Skeleton, Zombie, Ghoul, Shadow, Wight, Ghast, Wraith, Mummy,
//						Spectre, Vampire, Ghost, Lich, Special };
enum charStatusType 
{ 
  Okay,        // 0
  Unconscious, // 1  less than one hp, but not losing hp (dying)
  Dead,        // 2  <= -10 hp
  Fled,        // 3 
  Petrified,   // 4
  Gone,        // 5
  Animated,    // 6
  TempGone,    // 7
	Running,     // 8
  Dying        // 9 less than one hp, and losing 1 hp per round (combat only)
};

//enum spellSaveEffectType;
//enum spellSaveVsType;
enum creatureSizeType;

extern int START_AGE;
extern int AGE_RANGE;

const int MAX_MOVE = 12;

const int FIGHTER_HIT_DIE = 10;
const int CLERIC_HIT_DIE  = 6;
const int THIEF_HIT_DIE = 6;
const int MAGIC_USER_HIT_DIE = 4;
const int RANGER_HIT_DIE = 8;
const int DRUID_HIT_DIE = CLERIC_HIT_DIE;
const int PALADIN_HIT_DIE = FIGHTER_HIT_DIE;

const int SKILL_DIE = 6;

const int START_PLATINUM  = 200;
const int START_GOLD      =   0;
const int START_ELECTRUM  =   0;
const int START_SILVER    =   0;
const int START_COPPER    =   0;

inline int GetMaxCharMovement() { return MAX_MOVE; }
inline int GetCharStartingAge() { return START_AGE; }
inline int GetCharStartingAgeRanger() { return AGE_RANGE; }


void determineStrengthProperties(int strength, int strengthMod,
                                        int &hitBonus, int &dmgBonus, BYTE &openDoor, 
                                        BYTE &openMagicDoor, BYTE &BB_LG);

int getFighterLevel(int exp); // determine level for given exp pts
int getPaladinLevel(int exp);
int getRangerLevel(int exp);
int getClericLevel(int exp);
int getDruidLevel(int exp);
int getMagicUserLevel(int exp);
int getThiefLevel(int exp);

int GetMaxClericExpForLevel(int level);
int GetMaxDruidExpForLevel(int level);
int GetMaxFighterExpForLevel(int level);
int GetMaxRangerExpForLevel(int level);
int GetMaxPaladinExpForLevel(int level);
int GetMaxMagicUserExpForLevel(int level);
int GetMaxThiefExpForLevel(int level);

int GetMinClericExpForLevel(int level);
int GetMinDruidExpForLevel(int level);
int GetMinFighterExpForLevel(int level);
int GetMinRangerExpForLevel(int level);
int GetMinPaladinExpForLevel(int level);
int GetMinMagicUserExpForLevel(int level);
int GetMinThiefExpForLevel(int level);

void getFighterHitDice(int level, int& dice, int& nbr, int& constant);
void getPaladinHitDice(int level, int& dice, int& nbr, int& constant);
void getRangerHitDice(int level, int& dice, int& nbr, int& constant);
void getMagicUserHitDice(int level, int& dice, int& nbr, int& constant);
void getThiefHitDice(int level, int& dice, int& nbr, int& constant);
void getClericHitDice(int level, int& dice, int& nbr, int& constant);
void getDruidHitDice(int level, int& dice, int& nbr, int& constant);
void getCharClassHitDice(WORD ClassFlag, int level, int& dice, int& nbr, int& constant);

BOOL isDruid(classType type);
BOOL isCleric(classType type);
BOOL isFighter(classType type);
BOOL isRanger(classType type);
BOOL isPaladin(classType type);
BOOL isMagicUser(classType type);
BOOL isThief(classType type);
BOOL isMultiClass(classType type);
int numClasses(classType type);
BOOL CanCastMagicUserSpells(classType CharClass);
BOOL CanCastClericSpells(classType CharClass);

int determineHitDiceBonus(BYTE Score, BOOL IsFighter);
BYTE rollSkillDie(abilityType type);
void CheckNewCharClassScores(classType charClass, CHARSKILLDATA &scores);
void CheckNewCharRaceScores(raceType race, CHARSKILLDATA &scores);
int determineCharStartAge(raceType race);
int determineCharMaxAge(raceType race);
int DetermineNormalEncumbrance(int str, int strMod);

CString GetGenderName(genderType type);
CString GetAlignmentName(alignmentType align);

#define UNDEAD_NOAFFECT (0)
#define UNDEAD_TURNED   (-1)
#define UNDEAD_DESTROY  (-2)
// See #defines above for return values.
// If value in range 1-20 then need that roll or higher
// on d20 to succeed in turning.
enum eventTurnUndeadModType;
int GetUndeadTurnValueByHD(int level, int undeadHD, eventTurnUndeadModType modifier);
int GetUndeadTurnValue(int level, undeadType type, eventTurnUndeadModType modifier);


enum spellTargetingType { Self, SelectedByCount, WholeParty, TouchedTargets, AreaCircle, SelectByHitDice,
                          AreaLinePickStart, AreaLinePickEnd, AreaSquare, AreaCone };
const int NUM_SPELL_TARGETING_TYPES = 10;

enum spellSaveEffectType { NoSave, SaveNegates, SaveForHalf, UseTHACO };
const int NUM_SAVE_EFFECT_TYPES = 4;

enum spellSaveVsType { ParPoiDM, PetPoly, RodStaffWand, Sp, BreathWeapon };
const int  NUM_SAVE_VS_TYPES = 5;

enum spellDurationType { inRounds, byDamageTaken, inHours, inDays, Instantaneous, byNbrAttacks };
const int NUM_SPELL_DURATION_TYPES = 6;

enum spellCastingTimeType { sctImmediate, sctInitiative, sctRounds, sctTurns };
const int NUM_SPELL_CASTING_TIME_TYPES = 4;

const int MAX_SPELL_LEVEL = 9;

struct spellLimitsType
{
  WORD spellClasses; // which spell class can this dude use? (only one!)
  BOOL UseLimits; // some NPC types get unlimited spells
  BYTE PrimeScore[NumClassFlags];
  BYTE  numSpellsKnown[NumClassFlags][MAX_SPELL_LEVEL];

  spellLimitsType() { Clear(); }
  ~spellLimitsType() { Clear(); }
  spellLimitsType &operator=(const spellLimitsType &src);
  BOOL operator==(const spellLimitsType &src);
  void Clear();
  void Serialize(CArchive &ar, double version);
  void Serialize(CAR &ar, double version);

  void SetToMaxLimits();
  BOOL ValidSpellClass(WORD ClassFlag);
  BOOL AddSpell(WORD CasterClassFlag, WORD SpellClassFlag, WORD CastMask, int SpellLevel);
  void RemoveSpell(WORD CasterClassFlag, int SpellLevel);
  void RemoveSpellLevel(int SpellLevel);
  // ie: how many cleric spells of any level?
  BYTE numKnown(WORD ClassFlag, int SpellLevel);
  BYTE MaxSpellsForLevel(WORD CharClass, int CharLevel, int SpellLevel, BYTE prime);
  BOOL CanKnowSpell(WORD CasterClassFlag, WORD SpellClassFlag, WORD CastMask, int SpellLevel);
};

void SetSpellLimits(WORD CharClass, int prime, spellLimitsType &limits);
int CurrSpellLevel(WORD CharClass, int CharLevel, BYTE Prime, WORD CastingClass);
WORD GetSpellCastingClassForCharClass(WORD CharClassFlag);
void GetSpellClasses(WORD charClass, WORD &c1, WORD &c2, WORD &c3);
WORD CharClassToClassFlag(classType ctype);
WORD SetClassBitsFromCharClass(classType ctype);
classType ClassFlagToCharClass(WORD cflag);
WORD GetSpellCastingClassForCharClass(WORD CharClassFlag);
BOOL NumClassBits(WORD ClassFlag);
int GetSpellMemorizeTime(int level);
int GetSpellPrepTime(int level);
BOOL doClassesOverlap(classType type1, classType type2);

} // End of namespace DungeonCraft

#endif
