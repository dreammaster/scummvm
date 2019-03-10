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

#include "dungeoncraft/shared/..\shared\stdafx.h"

#ifdef UAFEDITOR
#include "dungeoncraft/shared/..\UAFWinEd\UAFWinEd.h"
#else
//#include "dungeoncraft/shared/externs.h"
#include "dungeoncraft/shared/..\UAFWin\Dungeon.h"
#endif

#include "dungeoncraft/shared/GlobalData.h"
#include "dungeoncraft/shared/GameRules.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int START_AGE = 17;
int AGE_RANGE = 5;

#define HIGHEST_LEVEL_ALLOWED 30

/*
struct DICEDATA
{
  DICEDATA(n,s,b)
  int nbr;
  int sides;
  int bonus;
};
*/

// For hit dice, the bonus represents a fixed hp addition
// and if non-zero the dice are not rolled. This fixed
// value is cumulative. Actual hit dice bonuses are
// determined elsewhere. Do not put a non-zero bonus
// with non-zero dice sides/nbr. It's one or the other.
//
#define DEFFIGHTERHD(n)   DICEDATA(n,FIGHTER_HIT_DIE,0)
#define DEFFIGHTERHD2()   DICEDATA(0,0,3)

#define DEFPALADINHD(n)   DICEDATA(n,PALADIN_HIT_DIE,0)
#define DEFPALADINHD2()   DICEDATA(0,0,3)

#define DEFRANGERHD(n)    DICEDATA(n,RANGER_HIT_DIE,0)
#define DEFRANGERHD2()    DICEDATA(0,0,2)

#define DEFMAGICUSERHD(n) DICEDATA(n,MAGIC_USER_HIT_DIE,0)
#define DEFMAGICUSERHD2() DICEDATA(0,0,1)

#define DEFTHIEFHD(n)     DICEDATA(n,THIEF_HIT_DIE,0)
#define DEFTHIEFHD2()     DICEDATA(0,0,2)

#define DEFCLERICHD(n)    DICEDATA(n,CLERIC_HIT_DIE,0)
#define DEFCLERICHD2()    DICEDATA(0,0,2)

#define DEFDRUIDHD(n)     DICEDATA(n,DRUID_HIT_DIE,0)
#define DEFDRUIDHD2()     DICEDATA(0,0,0)

DICEDATA FighterHitDiceTable[HIGHEST_LEVEL_ALLOWED] =
{
  DEFFIGHTERHD(1),
  DEFFIGHTERHD(1),
  DEFFIGHTERHD(1),
  DEFFIGHTERHD(1),
  DEFFIGHTERHD(1),
  DEFFIGHTERHD(1),
  DEFFIGHTERHD(1),
  DEFFIGHTERHD(1),
  DEFFIGHTERHD(1),
  DEFFIGHTERHD2(),
  DEFFIGHTERHD2(),
  DEFFIGHTERHD2(),
  DEFFIGHTERHD2(),
  DEFFIGHTERHD2(),
  DEFFIGHTERHD2(),
  DEFFIGHTERHD2(),
  DEFFIGHTERHD2(),
  DEFFIGHTERHD2(),
  DEFFIGHTERHD2(),
  DEFFIGHTERHD2(),
  DEFFIGHTERHD2(),
  DEFFIGHTERHD2(),
  DEFFIGHTERHD2(),
  DEFFIGHTERHD2(),
  DEFFIGHTERHD2(),
  DEFFIGHTERHD2(),
  DEFFIGHTERHD2(),
  DEFFIGHTERHD2(),
  DEFFIGHTERHD2(),
  DEFFIGHTERHD2()
};
DICEDATA PaladinHitDiceTable[HIGHEST_LEVEL_ALLOWED] =
{
  DEFPALADINHD(1),
  DEFPALADINHD(1),
  DEFPALADINHD(1),
  DEFPALADINHD(1),
  DEFPALADINHD(1),
  DEFPALADINHD(1),
  DEFPALADINHD(1),
  DEFPALADINHD(1),
  DEFPALADINHD(1),
  DEFPALADINHD2(),
  DEFPALADINHD2(),
  DEFPALADINHD2(),
  DEFPALADINHD2(),
  DEFPALADINHD2(),
  DEFPALADINHD2(),
  DEFPALADINHD2(),
  DEFPALADINHD2(),
  DEFPALADINHD2(),
  DEFPALADINHD2(),
  DEFPALADINHD2(),
  DEFPALADINHD2(),
  DEFPALADINHD2(),
  DEFPALADINHD2(),
  DEFPALADINHD2(),
  DEFPALADINHD2(),
  DEFPALADINHD2(),
  DEFPALADINHD2(),
  DEFPALADINHD2(),
  DEFPALADINHD2(),
  DEFPALADINHD2()
};

DICEDATA RangerHitDiceTable[HIGHEST_LEVEL_ALLOWED] =
{
  DEFRANGERHD(2),
  DEFRANGERHD(1),
  DEFRANGERHD(1),
  DEFRANGERHD(1),
  DEFRANGERHD(1),
  DEFRANGERHD(1),
  DEFRANGERHD(1),
  DEFRANGERHD(1),
  DEFRANGERHD(1),
  DEFRANGERHD(1),
  DEFRANGERHD2(),
  DEFRANGERHD2(),
  DEFRANGERHD2(),
  DEFRANGERHD2(),
  DEFRANGERHD2(),
  DEFRANGERHD2(),
  DEFRANGERHD2(),
  DEFRANGERHD2(),
  DEFRANGERHD2(),
  DEFRANGERHD2(),
  DEFRANGERHD2(),
  DEFRANGERHD2(),
  DEFRANGERHD2(),
  DEFRANGERHD2(),
  DEFRANGERHD2(),
  DEFRANGERHD2(),
  DEFRANGERHD2(),
  DEFRANGERHD2(),
  DEFRANGERHD2(),
  DEFRANGERHD2()
};

DICEDATA MagicUserHitDiceTable[HIGHEST_LEVEL_ALLOWED] =
{
  DEFMAGICUSERHD(1),
  DEFMAGICUSERHD(1),
  DEFMAGICUSERHD(1),
  DEFMAGICUSERHD(1),
  DEFMAGICUSERHD(1),
  DEFMAGICUSERHD(1),
  DEFMAGICUSERHD(1),
  DEFMAGICUSERHD(1),
  DEFMAGICUSERHD(1),
  DEFMAGICUSERHD(1),
  DEFMAGICUSERHD(1),
  DEFMAGICUSERHD2(),
  DEFMAGICUSERHD2(),
  DEFMAGICUSERHD2(),
  DEFMAGICUSERHD2(),
  DEFMAGICUSERHD2(),
  DEFMAGICUSERHD2(),
  DEFMAGICUSERHD2(),
  DEFMAGICUSERHD2(),
  DEFMAGICUSERHD2(),
  DEFMAGICUSERHD2(),
  DEFMAGICUSERHD2(),
  DEFMAGICUSERHD2(),
  DEFMAGICUSERHD2(),
  DEFMAGICUSERHD2(),
  DEFMAGICUSERHD2(),
  DEFMAGICUSERHD2(),
  DEFMAGICUSERHD2(),
  DEFMAGICUSERHD2(),
  DEFMAGICUSERHD2()
};

DICEDATA ThiefHitDiceTable[HIGHEST_LEVEL_ALLOWED] =
{
  DEFTHIEFHD(1),
  DEFTHIEFHD(1),
  DEFTHIEFHD(1),
  DEFTHIEFHD(1),
  DEFTHIEFHD(1),
  DEFTHIEFHD(1),
  DEFTHIEFHD(1),
  DEFTHIEFHD(1),
  DEFTHIEFHD(1),
  DEFTHIEFHD(1),
  DEFTHIEFHD2(),
  DEFTHIEFHD2(),
  DEFTHIEFHD2(),
  DEFTHIEFHD2(),
  DEFTHIEFHD2(),
  DEFTHIEFHD2(),
  DEFTHIEFHD2(),
  DEFTHIEFHD2(),
  DEFTHIEFHD2(),
  DEFTHIEFHD2(),
  DEFTHIEFHD2(),
  DEFTHIEFHD2(),
  DEFTHIEFHD2(),
  DEFTHIEFHD2(),
  DEFTHIEFHD2(),
  DEFTHIEFHD2(),
  DEFTHIEFHD2(),
  DEFTHIEFHD2(),
  DEFTHIEFHD2(),
  DEFTHIEFHD2()
};

DICEDATA ClericHitDiceTable[HIGHEST_LEVEL_ALLOWED] =
{
  DEFCLERICHD(1),
  DEFCLERICHD(1),
  DEFCLERICHD(1),
  DEFCLERICHD(1),
  DEFCLERICHD(1),
  DEFCLERICHD(1),
  DEFCLERICHD(1),
  DEFCLERICHD(1),
  DEFCLERICHD(1),
  DEFCLERICHD2(),
  DEFCLERICHD2(),
  DEFCLERICHD2(),
  DEFCLERICHD2(),
  DEFCLERICHD2(),
  DEFCLERICHD2(),
  DEFCLERICHD2(),
  DEFCLERICHD2(),
  DEFCLERICHD2(),
  DEFCLERICHD2(),
  DEFCLERICHD2(),
  DEFCLERICHD2(),
  DEFCLERICHD2(),
  DEFCLERICHD2(),
  DEFCLERICHD2(),
  DEFCLERICHD2(),
  DEFCLERICHD2(),
  DEFCLERICHD2(),
  DEFCLERICHD2(),
  DEFCLERICHD2(),
  DEFCLERICHD2()
};

DICEDATA DruidHitDiceTable[HIGHEST_LEVEL_ALLOWED] =
{
  DEFDRUIDHD(1),
  DEFDRUIDHD(1),
  DEFDRUIDHD(1),
  DEFDRUIDHD(1),
  DEFDRUIDHD(1),
  DEFDRUIDHD(1),
  DEFDRUIDHD(1),
  DEFDRUIDHD(1),
  DEFDRUIDHD(1),
  DEFDRUIDHD(1),
  DEFDRUIDHD(1),
  DEFDRUIDHD(1),
  DEFDRUIDHD(1),
  DEFDRUIDHD(1),
  DEFDRUIDHD2(),
  DEFDRUIDHD2(),
  DEFDRUIDHD2(),
  DEFDRUIDHD2(),
  DEFDRUIDHD2(),
  DEFDRUIDHD2(),
  DEFDRUIDHD2(),
  DEFDRUIDHD2(),
  DEFDRUIDHD2(),
  DEFDRUIDHD2(),
  DEFDRUIDHD2(),
  DEFDRUIDHD2(),
  DEFDRUIDHD2(),
  DEFDRUIDHD2(),
  DEFDRUIDHD2(),
  DEFDRUIDHD2()
};

CString GetGenderName(genderType type)
{
  CString temp("??");
  switch (type)
  {
  case Male:   temp = "Male";   break;
  case Female: temp = "Female"; break;
  }
  return temp;
}

CString GetAlignmentName(alignmentType align)
{
  CString temp("??");
  switch (align) 
  {
  case LawfulGood:     temp = "Lawful Good";     break;
  case NeutralGood:    temp = "Neutral Good";    break;
  case ChaoticGood:    temp = "Chaotic Good";    break;
  case LawfulNeutral:  temp = "Lawful Neutral";  break;
  case TrueNeutral:    temp = "True Neutral";    break;
  case ChaoticNeutral: temp = "Chaotic Neutral"; break;
  case LawfulEvil:     temp = "Lawful Evil";     break;
  case NeutralEvil:    temp = "Neutral Evil";    break;
  case ChaoticEvil:    temp = "Chaotic Evil";    break;
  }
  return temp;
}

// Temporary defines for table initialization only.
// 
// from file char.h
//#define UNDEAD_NOAFFECT (0)
//#define UNDEAD_TURNED   (-1)
//#define UNDEAD_DESTROY  (-2)

#define NONE  UNDEAD_NOAFFECT
#define TURN  UNDEAD_TURNED
#define DEST  UNDEAD_DESTROY
#define DE_X  UNDEAD_DESTROY

// TODO: High-level Clerics can turn with a D* result,
//       I've put DE_X in the Table to act as placeholders,
//       but for now it just evaluates to UNDEAD_DESTROY.


#ifdef _DEBUG
// these next two tables are for debugging only
// only define one at a time, and don't define any
// for a release build
//
//#define DESTROY_ALL_UNDEAD
//#define TURN_ALL_UNDEAD

#ifdef DESTROY_ALL_UNDEAD
static const int UndeadTurnTable[13][14] =
{
  { DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST },
  { DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST },
  { DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST },
  { DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST },
  { DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST },
  { DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST },
  { DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST },
  { DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST },
  { DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST },
  { DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST },
  { DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST },
  { DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST },
  { DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST, DEST }
};
#endif

#ifdef TURN_ALL_UNDEAD
static const int UndeadTurnTable[13][14] =
{
  { TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN },
  { TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN },
  { TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN },
  { TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN },
  { TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN },
  { TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN },
  { TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN },
  { TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN },
  { TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN },
  { TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN },
  { TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN },
  { TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN },
  { TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN, TURN }
};
#endif

#endif // ifdef _DEBUG

#if (!defined TURN_ALL_UNDEAD) && (!defined DESTROY_ALL_UNDEAD)

// Table for Clerics Turning Undead - used as a lookup
//
// Obs. The order of Undead Types on the rows of this array
//      must match the order in which they are declared within
//      the UndeadType enumeration (NotUndead is accounted for
//      by special code.)
//
static const int UndeadTurnTable[13][14] =
{
// Cleric Lvl       1     2     3     4     5     6     7     8     9    10    11    12    13    14+

/* Skeletn */  {   10,    7,    4, TURN, TURN, DEST, DEST, DE_X, DE_X, DE_X, DE_X, DE_X, DE_X, DE_X },
/* Zombie  */  {   13,   10,    7,    4, TURN, TURN, DEST, DEST, DE_X, DE_X, DE_X, DE_X, DE_X, DE_X },
/* Ghoul   */  {   16,   13,   10,    7,    4, TURN, TURN, DEST, DEST, DE_X, DE_X, DE_X, DE_X, DE_X },
/* Shadow  */  {   19,   16,   13,   10,    7,    4, TURN, TURN, DEST, DEST, DEST, DE_X, DE_X, DE_X },
/* Wight   */  {   20,   19,   16,   13,   10,    7,    4, TURN, TURN, DEST, DEST, DEST, DEST, DE_X },
/* Ghast   */  { NONE,   20,   19,   16,   13,   10,    7,    4, TURN, TURN, TURN, DEST, DEST, DEST },
/* Wraith  */  { NONE, NONE,   20,   19,   16,   13,   10,    7,    4, TURN, TURN, TURN, TURN, DEST },
/* Mummy   */  { NONE, NONE, NONE,   20,   19,   16,   13,   10,    7,    4,    4, TURN, TURN, TURN },
/* Spectre */  { NONE, NONE, NONE, NONE,   20,   19,   16,   13,   10,    7,    7,    4,    4, TURN },
/* Vampire */  { NONE, NONE, NONE, NONE, NONE,   20,   19,   16,   13,   10,   10,    7,    7,    4 },
/* Ghost   */  { NONE, NONE, NONE, NONE, NONE, NONE,   20,   19,   16,   13,   13,   10,   10,    7 },
/* Lich    */  { NONE, NONE, NONE, NONE, NONE, NONE, NONE,   20,   19,   16,   16,   13,   13,   10 },
/* Special */  { NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,   20,   19,   19,   16,   16,   13 }
};

#endif


// Undefine temporary defines for Undead Turning table.
// 
#undef NONE
#undef TURN
#undef DEST
#undef DE_X

// GetUndeadTurnValueByHD is used internally by the
// exported GetUndeadTurnValue method.
//
// modifier is one of [ None, Hard, Difficult, Impossible ].
//
int GetUndeadTurnValueByHD(int level, int undeadHD, eventTurnUndeadModType modifier)
{
  if ( ( undeadHD <= 0 ) ||       // this condition should be taken care of by caller
       ( modifier == Impossible ) ||
       ( level <= 0 ) )
  {
    return UNDEAD_NOAFFECT;
  }

  // Cap cleric level, UndeadTurnTable can only handle
  // Cleric levels of 1 to 15 (the column for 14 can be
  // used for 14-15th level Clerics).
  //
  if ( level > 14 )
  {
    level = 14;
  }

  // Assertion: UndeadTurnTable cannot handle more
  // than 12 hit dice of Undead.
  //
  ASSERT( undeadHD <= 12 );
  // just in case, so we don't crash
  if ( undeadHD > 12 )
  {
    undeadHD = 12;
  }

  // Array is zero-based indexing; so subtract one from
  // one-based level and hit dice to perform look-up.
  //
  // result will be 1-20=roll dice, 0=no affect, -1=turned, -2=destroyed
  int result = UndeadTurnTable[ undeadHD - 1][ level - 1];

  // OBS. We only adjust d20 Undead turn results for
  // Difficulty -- why not T and D results?
  //
  if ( ( result >= 1 ) && ( result <= 20 ) )
  {
    // Adjust for Difficulty
    //
    switch (modifier)
    {
      case Hard:      result += 2; break; // I only guessed for these modifier values,
      case Difficult: result += 4; break; // not sure what UA actually uses.
    }
  }

  // may have to cap result if the above condition was false,
  // or if +modifier pushed it above 20
  if ( result > 20 )
  {
    result = 20;
  }
  return result;
}


// GetUndeadTurnValue returns the number necessary on a
// d20 turning roll for Undead to be turned.
//
// GetUndeadTurnValue may also return three special values:
//
//   UNDEAD_NOAFFECT - cleric has no effect
//   UNDEAD_TURNED - 2d6 HD of undead should be turned
//   UNDEAD_DESTROYED - 2d6 HD of undead should be destroyed
//
// modifier parameter specifies game difficulty.
//
// See Char.h for the definitions of these constants.
//
int GetUndeadTurnValue(int level, undeadType type, eventTurnUndeadModType modifier)
{
  int hd;  // not 'really' hit dice, but row of table.

  // Assertion: Bad undeadType, perhaps undeadType was extended
  // w/o a corresponding change made to UndeadTurnTable.
  //
  ASSERT( (int)type <= Special );

  hd = (int)type;

  // This is equivalent to testing type == NotUndead
  //
  if ( !hd )
  {
    return UNDEAD_NOAFFECT;
  }

  return GetUndeadTurnValueByHD( level, hd, modifier);
}


struct EXPERIENCE_POINT_DATA
{
  EXPERIENCE_POINT_DATA() {min=max=0;}
  EXPERIENCE_POINT_DATA(int mn,int mx) {min=mn;max=mx;}
  int min;
  int max;
};

#define DEFEXPPTS(a,b) EXPERIENCE_POINT_DATA(a,b)

EXPERIENCE_POINT_DATA FighterExpPointTable[HIGHEST_LEVEL_ALLOWED] =
{
  DEFEXPPTS(      0,   1999),
  DEFEXPPTS(   2000,   3999),
  DEFEXPPTS(   4000,   7999),
  DEFEXPPTS(   8000,  15999),
  DEFEXPPTS(  16000,  31999),
  DEFEXPPTS(  32000,  63999),
  DEFEXPPTS(  64000, 124999),
  DEFEXPPTS( 125000, 249999),
  DEFEXPPTS( 250000, 499999),
  DEFEXPPTS( 500000, 749999),
  DEFEXPPTS( 750000, 999999),
  DEFEXPPTS(1000000,1249999),
  DEFEXPPTS(1250000,1499999),
  DEFEXPPTS(1500000,1749999),
  DEFEXPPTS(1750000,1999999),
  DEFEXPPTS(2000000,2249999),
  DEFEXPPTS(2250000,2499999),
  DEFEXPPTS(2500000,2749999),
  DEFEXPPTS(2750000,2999999),
  DEFEXPPTS(3000000,3249999),
  DEFEXPPTS(3250000,3499999),
  DEFEXPPTS(3500000,3749999),
  DEFEXPPTS(3750000,3999999),
  DEFEXPPTS(4000000,4249999),
  DEFEXPPTS(4250000,4499999),
  DEFEXPPTS(4500000,4749999),
  DEFEXPPTS(4750000,4999999),
  DEFEXPPTS(5000000,5249999),
  DEFEXPPTS(5250000,5499999),
  DEFEXPPTS(5500000,5749999)
};
EXPERIENCE_POINT_DATA RangerExpPointTable[HIGHEST_LEVEL_ALLOWED] =
{
  DEFEXPPTS(      0,   2249),
  DEFEXPPTS(   2250,   4499),
  DEFEXPPTS(   4500,   8999),
  DEFEXPPTS(   9000,  17999),
  DEFEXPPTS(  18000,  35999),
  DEFEXPPTS(  36000,  74999),
  DEFEXPPTS(  75000, 149999),
  DEFEXPPTS( 150000, 299999),
  DEFEXPPTS( 300000, 599999),
  DEFEXPPTS( 600000, 899999),
  DEFEXPPTS( 900000,1199999),
  DEFEXPPTS(1200000,1499999),
  DEFEXPPTS(1500000,1799999),
  DEFEXPPTS(1800000,2099999),
  DEFEXPPTS(2100000,2399999),
  DEFEXPPTS(2400000,2699999),
  DEFEXPPTS(2700000,2999999),
  DEFEXPPTS(3000000,3299999),
  DEFEXPPTS(3300000,3599999),
  DEFEXPPTS(3600000,3899999),
  DEFEXPPTS(3900000,4199999),
  DEFEXPPTS(4200000,4499999),
  DEFEXPPTS(4500000,4799999),
  DEFEXPPTS(4800000,5099999),
  DEFEXPPTS(5100000,5399999),
  DEFEXPPTS(5400000,5699999),
  DEFEXPPTS(5700000,5999999),
  DEFEXPPTS(6000000,6299999),
  DEFEXPPTS(6300000,6899999),
  DEFEXPPTS(6900000,7199999)
};
EXPERIENCE_POINT_DATA PaladinExpPointTable[HIGHEST_LEVEL_ALLOWED] =
{
  DEFEXPPTS(      0,   2249),
  DEFEXPPTS(   2250,   4499),
  DEFEXPPTS(   4500,   8999),
  DEFEXPPTS(   9000,  17999),
  DEFEXPPTS(  18000,  35999),
  DEFEXPPTS(  36000,  74999),
  DEFEXPPTS(  75000, 149999),
  DEFEXPPTS( 150000, 299999),
  DEFEXPPTS( 300000, 599999),
  DEFEXPPTS( 600000, 899999),
  DEFEXPPTS( 900000,1199999),
  DEFEXPPTS(1200000,1499999),
  DEFEXPPTS(1500000,1799999),
  DEFEXPPTS(1800000,2099999),
  DEFEXPPTS(2100000,2399999),
  DEFEXPPTS(2400000,2699999),
  DEFEXPPTS(2700000,2999999),
  DEFEXPPTS(3000000,3299999),
  DEFEXPPTS(3300000,3599999),
  DEFEXPPTS(3600000,3899999),
  DEFEXPPTS(3900000,4199999),
  DEFEXPPTS(4200000,4499999),
  DEFEXPPTS(4500000,4799999),
  DEFEXPPTS(4800000,5099999),
  DEFEXPPTS(5100000,5399999),
  DEFEXPPTS(5400000,5699999),
  DEFEXPPTS(5700000,5999999),
  DEFEXPPTS(6000000,6299999),
  DEFEXPPTS(6300000,6899999),
  DEFEXPPTS(6900000,7199999)
};
EXPERIENCE_POINT_DATA MagicUserExpPointTable[HIGHEST_LEVEL_ALLOWED] =
{
  DEFEXPPTS(      0,   2499),
  DEFEXPPTS(   2500,   4999),
  DEFEXPPTS(   5000,   9999),
  DEFEXPPTS(  10000,  19999),
  DEFEXPPTS(  20000,  39999),
  DEFEXPPTS(  40000,  59999),
  DEFEXPPTS(  60000,  89999),
  DEFEXPPTS(  90000, 134999),
  DEFEXPPTS( 135000, 249999),
  DEFEXPPTS( 250000, 374999),
  DEFEXPPTS( 375000, 749999),
  DEFEXPPTS( 750000,1124999),
  DEFEXPPTS(1125000,1499999),
  DEFEXPPTS(1500000,1874999),
  DEFEXPPTS(1875000,2249999),
  DEFEXPPTS(2250000,2624999),
  DEFEXPPTS(2625000,2999999),
  DEFEXPPTS(3000000,3374999),
  DEFEXPPTS(3375000,3749999),
  DEFEXPPTS(3750000,3249999),
  DEFEXPPTS(4125000,4499999),
  DEFEXPPTS(4500000,4874999),
  DEFEXPPTS(4875000,5249999),
  DEFEXPPTS(5250000,5624999),
  DEFEXPPTS(5625000,5999999),
  DEFEXPPTS(6000000,6374999),
  DEFEXPPTS(6375000,6749999),
  DEFEXPPTS(6750000,7124999),
  DEFEXPPTS(7125000,7499999),
  DEFEXPPTS(7500000,7874999)
};
EXPERIENCE_POINT_DATA ThiefExpPointTable[HIGHEST_LEVEL_ALLOWED] =
{
  DEFEXPPTS(      0,   1249),
  DEFEXPPTS(   1250,   2499),
  DEFEXPPTS(   2500,   4999),
  DEFEXPPTS(   5000,   9999),
  DEFEXPPTS(  10000,  19999),
  DEFEXPPTS(  20000,  39999),
  DEFEXPPTS(  40000,  69999),
  DEFEXPPTS(  70000, 109999),
  DEFEXPPTS( 110000, 159999),
  DEFEXPPTS( 160000, 219999),
  DEFEXPPTS( 220000, 439999),
  DEFEXPPTS( 440000, 659999),
  DEFEXPPTS( 660000, 879999),
  DEFEXPPTS( 880000,1099999),
  DEFEXPPTS(1100000,1319999),
  DEFEXPPTS(1320000,1539999),
  DEFEXPPTS(1540000,1759999),
  DEFEXPPTS(1760000,1979999),
  DEFEXPPTS(1980000,2199999),
  DEFEXPPTS(2200000,2419999),
  DEFEXPPTS(2420000,2639999),
  DEFEXPPTS(2640000,2859999),
  DEFEXPPTS(2860000,3079999),
  DEFEXPPTS(3080000,3299999),
  DEFEXPPTS(3300000,3519999),
  DEFEXPPTS(3520000,3739999),
  DEFEXPPTS(3740000,3959999),
  DEFEXPPTS(3960000,4179999),
  DEFEXPPTS(4180000,4399999),
  DEFEXPPTS(4400000,4619999)
};
EXPERIENCE_POINT_DATA DruidExpPointTable[HIGHEST_LEVEL_ALLOWED] =
{
  DEFEXPPTS(       0,    1999),
  DEFEXPPTS(    2000,    3999),
  DEFEXPPTS(    4000,    7499),
  DEFEXPPTS(    7500,   12499),
  DEFEXPPTS(   12500,   19999),
  DEFEXPPTS(   20000,   34999),
  DEFEXPPTS(   35000,   59999),
  DEFEXPPTS(   60000,   89999),
  DEFEXPPTS(   90000,  124999),
  DEFEXPPTS(  125000,  199999),
  DEFEXPPTS(  200000,  299999),
  DEFEXPPTS(  300000,  749999),
  DEFEXPPTS(  750000, 1499999),
  DEFEXPPTS( 1500000, 2999999),
  DEFEXPPTS( 3000000, 3499999),
  DEFEXPPTS( 3500000, 3999999),
  DEFEXPPTS( 4000000, 4499999),
  DEFEXPPTS( 4500000, 4999999),
  DEFEXPPTS( 5000000, 5499999),
  DEFEXPPTS( 5500000, 5999999),
  DEFEXPPTS( 6000000, 6499999),
  DEFEXPPTS( 6500000, 6999999),
  DEFEXPPTS( 7000000, 7499999),
  DEFEXPPTS( 7500000, 7999999),
  DEFEXPPTS( 8000000, 8499999),
  DEFEXPPTS( 8500000, 8999999),
  DEFEXPPTS( 9000000, 9499999),
  DEFEXPPTS( 9500000, 9999999),
  DEFEXPPTS(10000000,10499999),
  DEFEXPPTS(10500000,10999999)
};
EXPERIENCE_POINT_DATA ClericExpPointTable[HIGHEST_LEVEL_ALLOWED] =
{
  DEFEXPPTS(      0,   1499),
  DEFEXPPTS(   1500,   2999),
  DEFEXPPTS(   3000,   5999),
  DEFEXPPTS(   6000,  12999),
  DEFEXPPTS(  13000,  27499),
  DEFEXPPTS(  27500,  54999),
  DEFEXPPTS(  55000, 109999),
  DEFEXPPTS( 110000, 224999),
  DEFEXPPTS( 225000, 449999),
  DEFEXPPTS( 450000, 674999),
  DEFEXPPTS( 675000, 899999),
  DEFEXPPTS( 900000,1124999),
  DEFEXPPTS(1125000,1349999),
  DEFEXPPTS(1350000,1574999),
  DEFEXPPTS(1575000,1799999),
  DEFEXPPTS(1800000,2024999),
  DEFEXPPTS(2025000,2249999),
  DEFEXPPTS(2250000,2474999),
  DEFEXPPTS(2475000,2699999),
  DEFEXPPTS(2700000,2924999),
  DEFEXPPTS(2925000,3149999),
  DEFEXPPTS(3150000,3374999),
  DEFEXPPTS(3375000,3599999),
  DEFEXPPTS(3600000,3824999),
  DEFEXPPTS(3825000,4049999),
  DEFEXPPTS(4050000,4274999),
  DEFEXPPTS(4275000,4499999),
  DEFEXPPTS(4500000,4724999),
  DEFEXPPTS(4725000,4949999),
  DEFEXPPTS(4950000,5174999)
};
int GetMaxFighterExpForLevel(int level)
{
  level++; // max is one less than maximum for next level up

  if (level > HIGHEST_LEVEL_ALLOWED) level = HIGHEST_LEVEL_ALLOWED;
  if (level < 1) level = 1;
  level--;
  return FighterExpPointTable[level].max;
}

int GetMinFighterExpForLevel(int level)
{
  if (level > HIGHEST_LEVEL_ALLOWED) level = HIGHEST_LEVEL_ALLOWED;
  if (level < 1) level = 1;
  level--;
  return FighterExpPointTable[level].min;
}

int GetMaxPaladinExpForLevel(int level)
{
  level++; // max is one less than maximum for next level up
  if (level > HIGHEST_LEVEL_ALLOWED) level = HIGHEST_LEVEL_ALLOWED;
  if (level < 1) level = 1;
  level--;
  return PaladinExpPointTable[level].max;
}

int GetMinPaladinExpForLevel(int level)
{
  if (level > HIGHEST_LEVEL_ALLOWED) level = HIGHEST_LEVEL_ALLOWED;
  if (level < 1) level = 1;
  level--;
  return PaladinExpPointTable[level].min;
}
int GetMaxRangerExpForLevel(int level)
{
  level++; // max is one less than maximum for next level up
  if (level > HIGHEST_LEVEL_ALLOWED) level = HIGHEST_LEVEL_ALLOWED;
  if (level < 1) level = 1;
  level--;
  return RangerExpPointTable[level].max;
}

int GetMinRangerExpForLevel(int level)
{
  if (level > HIGHEST_LEVEL_ALLOWED) level = HIGHEST_LEVEL_ALLOWED;
  if (level < 1) level = 1;
  level--;
  return RangerExpPointTable[level].min;
}

int GetMaxMagicUserExpForLevel(int level)
{
  level++; // max is one less than maximum for next level up
  if (level > HIGHEST_LEVEL_ALLOWED) level = HIGHEST_LEVEL_ALLOWED;
  if (level < 1) level = 1;
  level--;
  return MagicUserExpPointTable[level].max;
}

int GetMinMagicUserExpForLevel(int level)
{
  if (level > HIGHEST_LEVEL_ALLOWED) level = HIGHEST_LEVEL_ALLOWED;
  if (level < 1) level = 1;
  level--;
  return MagicUserExpPointTable[level].min;
}

int GetMaxThiefExpForLevel(int level)
{
  level++; // max is one less than maximum for next level up
  if (level > HIGHEST_LEVEL_ALLOWED) level = HIGHEST_LEVEL_ALLOWED;
  if (level < 1) level = 1;
  level--;
  return ThiefExpPointTable[level].max;
}

int GetMinThiefExpForLevel(int level)
{
  if (level > HIGHEST_LEVEL_ALLOWED) level = HIGHEST_LEVEL_ALLOWED;
  if (level < 1) level = 1;
  level--;
  return ThiefExpPointTable[level].min;
}

int GetMaxDruidExpForLevel(int level)
{
  level++; // max is one less than maximum for next level up
  if (level > HIGHEST_LEVEL_ALLOWED) level = HIGHEST_LEVEL_ALLOWED;
  if (level < 1) level = 1;
  level--;
  return DruidExpPointTable[level].max;
}

int GetMinDruidExpForLevel(int level)
{
  if (level > HIGHEST_LEVEL_ALLOWED) level = HIGHEST_LEVEL_ALLOWED;
  if (level < 1) level = 1;
  level--;
  return DruidExpPointTable[level].min;
}


int GetMaxClericExpForLevel(int level)
{
  level++; // max is one less than maximum for next level up
  if (level > HIGHEST_LEVEL_ALLOWED) level = HIGHEST_LEVEL_ALLOWED;
  if (level < 1) level = 1;
  level--;
  return ClericExpPointTable[level].max;
}

int GetMinClericExpForLevel(int level)
{
  if (level > HIGHEST_LEVEL_ALLOWED) level = HIGHEST_LEVEL_ALLOWED;
  if (level < 1) level = 1;
  level--;
  return ClericExpPointTable[level].min;
}

void getFighterHitDice(int level, int& dice, int& nbr, int& constant)
{
  if (level > HIGHEST_LEVEL_ALLOWED) level = HIGHEST_LEVEL_ALLOWED;
  if (level < 1) level = 1;
  level--;

  dice = FighterHitDiceTable[level].sides;
  nbr = FighterHitDiceTable[level].nbr;
  constant = FighterHitDiceTable[level].bonus;    
}
void getPaladinHitDice(int level, int& dice, int& nbr, int& constant)
{
  if (level > HIGHEST_LEVEL_ALLOWED) level = HIGHEST_LEVEL_ALLOWED;
  if (level < 1) level = 1;
  level--;

  dice = PaladinHitDiceTable[level].sides;
  nbr = PaladinHitDiceTable[level].nbr;
  constant = PaladinHitDiceTable[level].bonus;   
}
void getRangerHitDice(int level, int& dice, int& nbr, int& constant)
{
  if (level > HIGHEST_LEVEL_ALLOWED) level = HIGHEST_LEVEL_ALLOWED;
  if (level < 1) level = 1;
  level--;

  dice = RangerHitDiceTable[level].sides;
  nbr = RangerHitDiceTable[level].nbr;
  constant = RangerHitDiceTable[level].bonus;   
}
void getMagicUserHitDice(int level, int& dice, int& nbr, int& constant)
{
  if (level > HIGHEST_LEVEL_ALLOWED) level = HIGHEST_LEVEL_ALLOWED;
  if (level < 1) level = 1;
  level--;

  dice = MagicUserHitDiceTable[level].sides;
  nbr = MagicUserHitDiceTable[level].nbr;
  constant = MagicUserHitDiceTable[level].bonus;   
}
void getThiefHitDice(int level, int& dice, int& nbr, int& constant)
{
  if (level > HIGHEST_LEVEL_ALLOWED) level = HIGHEST_LEVEL_ALLOWED;
  if (level < 1) level = 1;
  level--;

  dice = ThiefHitDiceTable[level].sides;
  nbr = ThiefHitDiceTable[level].nbr;
  constant = ThiefHitDiceTable[level].bonus;   
}
void getClericHitDice(int level, int& dice, int& nbr, int& constant)
{
  if (level > HIGHEST_LEVEL_ALLOWED) level = HIGHEST_LEVEL_ALLOWED;
  if (level < 1) level = 1;
  level--;

  dice = ClericHitDiceTable[level].sides;
  nbr = ClericHitDiceTable[level].nbr;
  constant = ClericHitDiceTable[level].bonus;    
}
void getDruidHitDice(int level, int& dice, int& nbr, int& constant)
{
  if (level > HIGHEST_LEVEL_ALLOWED) level = HIGHEST_LEVEL_ALLOWED;
  if (level < 1) level = 1;
  level--;

  dice = DruidHitDiceTable[level].sides;
  nbr = DruidHitDiceTable[level].nbr;
  constant = DruidHitDiceTable[level].bonus;    
}
void getCharClassHitDice(WORD ClassFlag, int level, int& dice, int& nbr, int& constant)
{
  ASSERT( NumClassBits(ClassFlag) == 1 );

  switch (ClassFlag)
  {
    case MagicUserFlag: getMagicUserHitDice(level,dice,nbr,constant); break;
    case ClericFlag:    getClericHitDice(level,dice,nbr,constant); break;
    case ThiefFlag:     getThiefHitDice(level,dice,nbr,constant); break;
    case FighterFlag:   getFighterHitDice(level,dice,nbr,constant); break;
    case PaladinFlag:   getPaladinHitDice(level,dice,nbr,constant); break;
    case RangerFlag:    getRangerHitDice(level,dice,nbr,constant); break;
    case DruidFlag:     getDruidHitDice(level,dice,nbr,constant); break;
    default: ASSERT(FALSE); return;
  }
}

int getFighterLevel(int exp)
{
  if (exp==0) return 1;
  int count=0;
  while (count<HIGHEST_LEVEL_ALLOWED)
  {
    if (   (exp >= FighterExpPointTable[count].min)
        && (exp <= FighterExpPointTable[count].max))
      return (count+1);
    count++;
  }
  return HIGHEST_LEVEL_ALLOWED;
}

int getPaladinLevel(int exp)
{
  if (exp==0) return 1;
  int count=0;
  while (count<HIGHEST_LEVEL_ALLOWED)
  {
    if (   (exp >= PaladinExpPointTable[count].min)
        && (exp <= PaladinExpPointTable[count].max))
      return (count+1);
    count++;
  }
  return HIGHEST_LEVEL_ALLOWED;
}

int getRangerLevel(int exp)
{
  if (exp==0) return 1;
  int count=0;
  while (count<HIGHEST_LEVEL_ALLOWED)
  {
    if (   (exp >= RangerExpPointTable[count].min)
        && (exp <= RangerExpPointTable[count].max))
      return (count+1);
    count++;
  }
  return HIGHEST_LEVEL_ALLOWED;
}

int getClericLevel(int exp)
{
  if (exp==0) return 1;
  int count=0;
  while (count<HIGHEST_LEVEL_ALLOWED)
  {
    if (   (exp >= ClericExpPointTable[count].min)
        && (exp <= ClericExpPointTable[count].max))
      return (count+1);
    count++;
  }
  return HIGHEST_LEVEL_ALLOWED;
}

int getDruidLevel(int exp)
{
  if (exp==0) return 1;
  int count=0;
  while (count<HIGHEST_LEVEL_ALLOWED)
  {
    if (   (exp >= DruidExpPointTable[count].min)
        && (exp <= DruidExpPointTable[count].max))
      return (count+1);
    count++;
  }
  return HIGHEST_LEVEL_ALLOWED;
}

int getMagicUserLevel(int exp)
{
  if (exp==0) return 1;
  int count=0;
  while (count<HIGHEST_LEVEL_ALLOWED)
  {
    if (   (exp >= MagicUserExpPointTable[count].min)
        && (exp <= MagicUserExpPointTable[count].max))
      return (count+1);
    count++;
  }
  return HIGHEST_LEVEL_ALLOWED;
}

int getThiefLevel(int exp)
{
  if (exp==0) return 1;
  int count=0;
  while (count<HIGHEST_LEVEL_ALLOWED)
  {
    if (   (exp >= ThiefExpPointTable[count].min)
        && (exp <= ThiefExpPointTable[count].max))
      return (count+1);
    count++;
  }
  return HIGHEST_LEVEL_ALLOWED;
}

WORD SetClassBitsFromCharClass(classType ctype)
{
  WORD mask=0;
  if (isFighter(ctype)) mask |= FighterFlag;
  if (isCleric(ctype)) mask |= ClericFlag;
  if (isMagicUser(ctype)) mask |= MagicUserFlag;
  if (isThief(ctype)) mask |= ThiefFlag;
  if (isRanger(ctype)) mask |= RangerFlag;
  if (isPaladin(ctype)) mask |= PaladinFlag;
  if (isDruid(ctype)) mask |= DruidFlag;
  return mask;
}
// don't pass a multi-class type
WORD CharClassToClassFlag(classType ctype)
{
  ASSERT(!isMultiClass(ctype));
  switch (ctype)
  {
  case Fighter: return FighterFlag;
  case Cleric: return ClericFlag;
  case Ranger: return RangerFlag;
  case Paladin: return PaladinFlag;
  case MagicUser: return MagicUserFlag;
  case Thief: return ThiefFlag;
  case Druid: return DruidFlag;
  default:
      ASSERT(FALSE);
      return 0;
  }
  return 0;
}
// don't pass a multi-class flag
classType ClassFlagToCharClass(WORD cflag)
{
  ASSERT( NumClassBits(cflag) == 1 );

  switch (cflag)
  {
  case FighterFlag: return Fighter;
  case ClericFlag: return Cleric;
  case RangerFlag: return Ranger;
  case PaladinFlag: return Paladin;
  case MagicUserFlag: return MagicUser;
  case ThiefFlag: return Thief;
  case DruidFlag: return Druid;
  default:ASSERT(FALSE);return BogusClass;
  }
  return BogusClass;
}

int numClasses(classType type)
{
  return (NumClassBits(SetClassBitsFromCharClass(type)));
}

BOOL isMultiClass(classType type)
{
  return (numClasses(type)>1);
}

BOOL doClassesOverlap(classType type1, classType type2)
{
  DWORD bits1 = SetClassBitsFromCharClass(type1);
  DWORD bits2 = SetClassBitsFromCharClass(type2);
  return ((bits1 & bits2) ? TRUE : FALSE);
}
/*
enum classType { Fighter, Cleric, Ranger, Paladin, MagicUser, Thief, Druid,
						C_F, C_F_MU, C_R, C_MU, C_T, F_MU, F_T, 
            F_MU_T, MU_T, F_D, D_MU, F_MU_D };

*/
BOOL isDruid(classType type)
{
  switch (type)
  {
  case Druid:
  case F_D:
  case D_MU:
  case F_MU_D:
    return TRUE;
  }
  return FALSE;
}

BOOL isPaladin(classType type)
{
  switch (type)
  {
  case Paladin:
    return TRUE;
  }
  return FALSE;
}

BOOL isRanger(classType type)
{
  switch (type)
  {
  case Ranger:
  case C_R:
    return TRUE;
  }
  return FALSE;
}

BOOL isThief(classType type)
{
  switch (type)
  {
  case Thief:
  case C_T:
  case F_T:
  case F_MU_T:
  case MU_T:
    return TRUE;
  }
  return FALSE;
}

BOOL isMagicUser(classType type)
{
  switch (type)
  {
  case MagicUser:
  case C_F_MU:
  case C_MU:
  case F_MU:
  case F_MU_T:
  case MU_T:
  case D_MU:
  case F_MU_D:
    return TRUE;
  }
  return FALSE;
}

//*****************************************************************************
//    NAME: CanCastMagicUserSpells
//
// PURPOSE: 
//
//*****************************************************************************
BOOL CanCastMagicUserSpells(classType CharClass)
{
  WORD ClassBits = SetClassBitsFromCharClass(CharClass);
  for (int i=0;i<NumClassFlags;i++)
  {
    WORD flag = (ClassBits & (0x0001 << i));
    if (flag)
    {
      if (GetSpellCastingClassForCharClass(flag) == MagicUserFlag)
        return TRUE;
    }
  }
  return FALSE;
}

//*****************************************************************************
//    NAME: CanCastClericSpells
//
// PURPOSE: 
//
//*****************************************************************************
BOOL CanCastClericSpells(classType CharClass)
{
  WORD ClassBits = SetClassBitsFromCharClass(CharClass);
  for (int i=0;i<NumClassFlags;i++)
  {
    WORD flag = (ClassBits & (0x0001 << i));
    if (flag)
    {
      if (GetSpellCastingClassForCharClass(flag) == ClericFlag)
        return TRUE;
    }
  }
  return FALSE;
}

BOOL isCleric(classType type)
{
  switch (type)
  {
  case Cleric:
  case C_F:
  case C_F_MU:
  case C_R:
  case C_MU:
  case C_T:
    return TRUE;
  }
  return FALSE;
}
BOOL isFighter(classType type)
{
  switch (type)
  {
  case Fighter:
  case C_F:
  case C_F_MU:
  case F_MU:
  case F_T:
  case F_MU_T:
  case F_D:
  case F_MU_D:
    return TRUE;
  }
  return FALSE;
}

void determineStrengthProperties(int strength, int strengthMod,
                                 int &hitBonus, int &dmgBon, BYTE &openDoor, 
                                 BYTE &openMagicDoor, BYTE &BB_LG)
{
  switch (strength) 
  {
  case 1:
    hitBonus = -5;
    dmgBon = -4;
    openDoor = 1;      // locked door
    openMagicDoor = 0; // locked needing spell to open
    BB_LG = 0;
    break;
  case 2:
    hitBonus = -3;
    dmgBon = -2;
    openDoor = 1;
    openMagicDoor = 0;
    BB_LG = 0;
    break;
  case 3:
    hitBonus = -3;
    dmgBon = -1;
    openDoor = 2;
    openMagicDoor = 0;
    BB_LG = 0;
    break;
  case 4:
  case 5:
    hitBonus = -2;
    dmgBon = -1;
    openDoor = 3;
    openMagicDoor = 0;
    BB_LG = 0;
    break;
  case 6:
  case 7:
    hitBonus = -1;
    dmgBon = 0;
    openDoor = 4;
    openMagicDoor = 0;
    BB_LG = 0;
    break;
  case 8:
  case 9:
    hitBonus = 0;
    dmgBon = 0;
    openDoor = 5;
    openMagicDoor = 0;
    BB_LG = 1;
    break;
  case 10:
  case 11:
    hitBonus = 0;
    dmgBon = 0;
    openDoor = 6;
    openMagicDoor = 0;
    BB_LG = 2;
    break;
  case 12:
  case 13:
    hitBonus = 0;
    dmgBon = 0;
    openDoor = 7;
    openMagicDoor = 0;
    BB_LG = 4;
    break;
  case 14:
  case 15:
    hitBonus = 0;
    dmgBon = 0;
    openDoor = 8;
    openMagicDoor = 0;
    BB_LG = 7;
    break;
  case 16:
    hitBonus = 0;
    dmgBon = 1;
    openDoor = 9;
    openMagicDoor = 0;
    BB_LG = 10;
    break;
  case 17:
    hitBonus = 1;
    dmgBon = 1;
    openDoor = 10;
    openMagicDoor = 0;
    BB_LG = 13;
    break;
  case 18:
    if (strengthMod == 0)
    {
      hitBonus = 1;
      dmgBon = 2;
      openDoor = 11;
      openMagicDoor = 0;
      BB_LG = 16;
    }
    else if (strengthMod < 51)
    {
      hitBonus = 1;
      dmgBon = 3;
      openDoor = 12;
      openMagicDoor = 0;
      BB_LG = 20;
    }
    else if (strengthMod < 76)
    {
      hitBonus = 2;
      dmgBon = 3;
      openDoor = 13;
      openMagicDoor = 0;
      BB_LG = 25;
    }
    else if (strengthMod < 91)
    {
      hitBonus = 2;
      dmgBon = 4;
      openDoor = 14;
      openMagicDoor = 0;
      BB_LG = 30;
    }
    else if (strengthMod < 100)
    {
      hitBonus = 2;
      dmgBon = 5;
      openDoor = 15;
      openMagicDoor = 3;
      BB_LG = 35;
    }
    else {
      hitBonus = 3;
      dmgBon = 6;
      openDoor = 16;
      openMagicDoor = 6;
      BB_LG = 40;
    }
    break;
  case 19:
    hitBonus = 3;
    dmgBon = 7;
    openDoor = 16;
    openMagicDoor = 8;
    BB_LG = 50;
    break;
  case 20:
    hitBonus = 3;
    dmgBon = 8;
    openDoor = 17;
    openMagicDoor = 10;
    BB_LG = 60;
    break;
  case 21:
    hitBonus = 4;
    dmgBon = 9;
    openDoor = 17;
    openMagicDoor = 12;
    BB_LG = 70;
    break;
  case 22:
    hitBonus = 4;
    dmgBon = 10;
    openDoor = 18;
    openMagicDoor = 14;
    BB_LG = 80;
    break;
  case 23:
    hitBonus = 5;
    dmgBon = 11;
    openDoor = 18;
    openMagicDoor = 16;
    BB_LG = 90;
    break;
  case 24:
    hitBonus = 6;
    dmgBon = 12;
    openDoor = 19;
    openMagicDoor = 17;
    BB_LG = 95;
    break;
  case 25:
    hitBonus = 7;
    dmgBon = 16;
    openDoor = 19;
    openMagicDoor = 18;
    BB_LG = 99;
    break;
  default:
    hitBonus = 0;
    dmgBon = 0;
    openDoor = 0;
    openMagicDoor = 0;
    BB_LG = 0;
    WriteDebugString("Unhandled strength %i in determineStrengthProperties()\n", strength);
    break;
  }
}


int determineHitDiceBonus(BYTE Score, BOOL IsFighter)
{
   int bonus = 0;
   switch (Score)
   {
   case 3:
      bonus = -2;
      break;
   case 4:
   case 5:
   case 6:
      bonus = -1;
      break;
   case 15:
      bonus = 1;
      break;
   case 16:
      bonus = 2;
      break;
   case 17:
      if (IsFighter)
         bonus = 3;
      else
         bonus = 2;
      break;
   case 18:
      if (IsFighter)
         bonus = 4;
      else
         bonus = 2;
      break;
   default:
      bonus = 0;
      break;
   }
   return bonus;
}

BYTE rollSkillDie(abilityType type)
{
  if (globalData.version < _VERSION_0870_)
  {
    BYTE tmp1 = (BYTE)rollDice(SKILL_DIE,3);
    BYTE tmp2 = (BYTE)rollDice(SKILL_DIE,3);
    BYTE tmp3 = (BYTE)rollDice(SKILL_DIE,3);
    return ( max(tmp1, max(tmp2,tmp3)) );
  }
  else
  {
    double result;  
    BYTE tmp1=0,tmp2=0,tmp3=0;  
    if (abilityData.RollAbility(type, result))
      tmp1=(BYTE)result;
    if (abilityData.RollAbility(type, result))
      tmp2=(BYTE)result;
    if (abilityData.RollAbility(type, result))
      tmp3=(BYTE)result;

    return ( max(tmp1, max(tmp2,tmp3)) );
  }
}


void CheckNewCharClassScores(classType charClass, CHARSKILLDATA &scores)
{
  if (globalData.version < _VERSION_0870_)
  {
    // old method
    if (isFighter(charClass))
    {
       if (scores.str < 9) scores.str = 9;
       if (scores.con < 7) scores.con = 7;
    }
    if (isCleric(charClass))
    {         
       if (scores.wis < 9) scores.wis = 9;
    }
    if (isRanger(charClass))
    {
       if (scores.str < 13) scores.str = 13;
       if (scores.intel < 13) scores.intel = 13;
       if (scores.wis < 14) scores.wis = 14;
       if (scores.con < 14) scores.con = 14;
    }
    if (isPaladin(charClass))
    {         
       if (scores.str < 12) scores.str = 12;
       if (scores.con < 9) scores.con = 9;
       if (scores.wis < 13) scores.wis = 13;
       if (scores.cha < 17) scores.cha = 17;
    }
    if (isMagicUser(charClass))
    {
       if (scores.intel < 9) scores.intel = 9;
       if (scores.dex < 6) scores.dex = 6;
    }
    if (isThief(charClass))
    {
       if (scores.dex < 9) scores.dex = 9;
    }
    if (isDruid(charClass))
    {
       if (scores.wis < 12) scores.wis = 12;
       if (scores.cha < 15) scores.cha = 15;
    }
  }
  else
  {
    // new method, based on config files
    int minstr = ABILITYMINBASE(classData.GetAbilityLimits(charClass, scores.strKey));
    int minint = ABILITYMINBASE(classData.GetAbilityLimits(charClass, scores.intelKey));
    int minwis = ABILITYMINBASE(classData.GetAbilityLimits(charClass, scores.wisKey));
    int mindex = ABILITYMINBASE(classData.GetAbilityLimits(charClass, scores.dexKey));
    int mincon = ABILITYMINBASE(classData.GetAbilityLimits(charClass, scores.conKey));
    int mincha = ABILITYMINBASE(classData.GetAbilityLimits(charClass, scores.chaKey));

    int maxstr = ABILITYMAXBASE(classData.GetAbilityLimits(charClass, scores.strKey));
    int maxint = ABILITYMAXBASE(classData.GetAbilityLimits(charClass, scores.intelKey));
    int maxwis = ABILITYMAXBASE(classData.GetAbilityLimits(charClass, scores.wisKey));
    int maxdex = ABILITYMAXBASE(classData.GetAbilityLimits(charClass, scores.dexKey));
    int maxcon = ABILITYMAXBASE(classData.GetAbilityLimits(charClass, scores.conKey));
    int maxcha = ABILITYMAXBASE(classData.GetAbilityLimits(charClass, scores.chaKey));

    if (scores.str < minstr) scores.str = minstr;
    if (scores.intel < minint) scores.intel = minint;
    if (scores.wis < minwis) scores.wis = minwis;
    if (scores.dex < mindex) scores.dex = mindex;
    if (scores.con < mincon) scores.con = mincon;
    if (scores.cha < mincha) scores.cha = mincha;

    if (scores.str > maxstr) scores.str = maxstr;
    if (scores.intel > maxint) scores.intel = maxint;
    if (scores.wis > maxwis) scores.wis = maxwis;
    if (scores.dex > maxdex) scores.dex = maxdex;
    if (scores.con > maxcon) scores.con = maxcon;
    if (scores.cha > maxcha) scores.cha = maxcha;
  }
}

void CheckNewCharRaceScores(raceType race, CHARSKILLDATA &scores)
{
  if (globalData.version < _VERSION_0870_)
  {
    // old method
	  switch (race) 
    {
	  case Elf:
		  scores.dex++;
		  scores.con--;
		  break;
	  case Dwarf:
		  scores.con++;
		  scores.cha--;
		  break;
	  case Gnome:
		  scores.intel++;
		  scores.wis--;
		  break;
	  case Halfling:
		  scores.dex++;
		  scores.str--;
		  scores.strMod = 0;
		  break;
	  default:
      // no changes
		  break;
	  }

	  switch (race) 
    {
	  case Elf:
		  if (scores.dex < 6) scores.dex = 6;
		  if (scores.con < 7) scores.con = 7;
		  if (scores.intel < 8)	scores.intel = 8;
		  if (scores.cha < 8) scores.cha = 8;
		  break;
	  case Dwarf:
		  if (scores.str < 8)	scores.str = 8;
		  if (scores.con < 11) scores.con = 11;
		  if (scores.dex > 17)	scores.dex = 17;
		  if (scores.cha > 17) scores.cha = 17;
		  break;
	  case Gnome:
		  if (scores.str < 6) scores.str = 6;
		  if (scores.con < 8)	scores.con = 8;
		  if (scores.intel < 6)	scores.intel = 6;
		  break;
	  case HalfElf:
		  if (scores.dex < 6) scores.dex = 6;
		  if (scores.con < 6)	scores.con = 6;
		  if (scores.intel < 4)	scores.intel = 4;
		  break;
	  case Halfling:
		  if (scores.str < 7)	scores.str = 7;
      if (scores.dex < 7) scores.dex = 7;
		  if (scores.con < 10) scores.con = 10;
		  if (scores.intel < 6) scores.intel = 6;
		  if (scores.wis > 17) scores.wis = 17;
		  break;
	  default: // human
      // no changes
		  break;
	  }
  }
  else
  {
    // new method, based on config files
    int minstr = raceData.GetMinAbility(race, scores.strKey);
    int minint = raceData.GetMinAbility(race, scores.intelKey);
    int minwis = raceData.GetMinAbility(race, scores.wisKey);
    int mindex = raceData.GetMinAbility(race, scores.dexKey);
    int mincon = raceData.GetMinAbility(race, scores.conKey);
    int mincha = raceData.GetMinAbility(race, scores.chaKey);

    int maxstr = raceData.GetMaxAbility(race, scores.strKey);
    int maxint = raceData.GetMaxAbility(race, scores.intelKey);
    int maxwis = raceData.GetMaxAbility(race, scores.wisKey);
    int maxdex = raceData.GetMaxAbility(race, scores.dexKey);
    int maxcon = raceData.GetMaxAbility(race, scores.conKey);
    int maxcha = raceData.GetMaxAbility(race, scores.chaKey);

    if (scores.str < minstr) scores.str = minstr;
    if (scores.intel < minint) scores.intel = minint;
    if (scores.wis < minwis) scores.wis = minwis;
    if (scores.dex < mindex) scores.dex = mindex;
    if (scores.con < mincon) scores.con = mincon;
    if (scores.cha < mincha) scores.cha = mincha;

    if (scores.str > maxstr) scores.str = maxstr;
    if (scores.intel > maxint) scores.intel = maxint;
    if (scores.wis > maxwis) scores.wis = maxwis;
    if (scores.dex > maxdex) scores.dex = maxdex;
    if (scores.con > maxcon) scores.con = maxcon;
    if (scores.cha > maxcha) scores.cha = maxcha;
  }
}

int determineCharStartAge(raceType race)
{
  int age = raceData.GetStartAge(race);
  if ((START_AGE > 0)&&(age < START_AGE)) 
    age = START_AGE;
  return age;
}

int determineCharMaxAge(raceType race)
{
  if (globalData.version < _VERSION_0870_)
  {
	  switch (race) 
    {
	  case Elf:   return 1200;
	  case Dwarf: return 350;
	  case Gnome:	return 350;
	  case HalfElf:	return 160;
	  case Halfling: return 150;
	  default: return 100;
    }
  }
  else
  {
    return (raceData.GetMaxAge(race));
  }
}

int DetermineNormalEncumbrance(int str, int strMod)
{
  int result=0;
  int BaseAllowance = 350;
  // normal is based on str 8-11 able to carry 350gp
	switch (str)
	{
  case 1:
	case 2:
	case 3: // -350
		result = BaseAllowance-350;
		break;
	case 4:
	case 5: // -250
		result = BaseAllowance-250;
		break;
	case 6:
	case 7: // -150
		result = BaseAllowance-150;
		break;
	case 8:
	case 9: // normal
	case 10:
	case 11:
		result = BaseAllowance;
		break;
	case 12:
	case 13:
		result = BaseAllowance+100;
		break;
	case 14:
	case 15:
		result = BaseAllowance+200;
		break;
	case 16:
		result = BaseAllowance+350;
		break;
	case 17:
		result = BaseAllowance+500;
		break;
	case 18:
    {
    BYTE mod = strMod;
    if (mod == 0)
      result = BaseAllowance+750;
		else if (mod < 51)
			result = BaseAllowance+1000;
		else if (mod < 76)
			result = BaseAllowance+1250;
		else if (mod < 91)
			result = BaseAllowance+1500;
		else if (mod < 100)
			result = BaseAllowance+2000;
    else
		  result = BaseAllowance+3000;
    }
		break;
	default:
		result = BaseAllowance;
	}
  if (result <= 0) result = 1;
  return result;
}


struct SPLVLENTRY
{
  SPLVLENTRY() { ClLvl=MuLvl=0; }
  SPLVLENTRY(BYTE cl, BYTE mu) { ClLvl=cl;MuLvl=mu; }
  BYTE ClLvl;
  BYTE MuLvl;
};

struct SPBONUSENTRY
{
  SPBONUSENTRY() { Lvl=Qty=0; }
  SPBONUSENTRY(BYTE qty, BYTE lvl) { Qty=qty;Lvl=lvl; }
  BYTE Lvl;
  BYTE Qty;
};


/*
enum classType { Fighter, Cleric, Ranger, Paladin, MagicUser, Thief, Druid,
						     C_F, C_F_MU, C_R, C_MU, C_T, F_MU, F_T, 
                 F_MU_T, MU_T, F_D, D_MU, F_MU_D };

// class flags
const int MagicUserFlag = 1;
const int ClericFlag    = 2;
const int ThiefFlag     = 4;
const int FighterFlag   = 8;
const int PaladinFlag   = 16;
const int RangerFlag    = 32;
const int DruidFlag     = 64;
const int AllClasses    = 255;
const int NumClassFlags = 7;

int SpellClassToCharClass(WORD SpellClassFlag);

inline BYTE IndexToFlag(BYTE index)
{
  switch (index)
  {
  case 0: return MagicUserFlag;
  case 1: return ClericFlag;
  case 2: return ThiefFlag;
  case 3: return FighterFlag;
  case 4: return PaladinFlag;
  case 5: return RangerFlag;
  case 6: return DruidFlag;
  case 7: return AllClasses;
  }
  ASSERT(FALSE);
  return 0;
}

inline BYTE FlagToIndex(BYTE flag)
{ 
  switch (flag)
  {
  case MagicUserFlag: return 0;
  case ClericFlag:    return 1;
  case ThiefFlag:     return 2;
  case FighterFlag:   return 3;
  case PaladinFlag:   return 4;
  case RangerFlag:    return 5;
  case DruidFlag:     return 6;
  case AllClasses:    return 7;
  }
  ASSERT(FALSE);
  return 0;
}
extern char *ClassFlagText[8];
*/

// max spells by level tables

#define DEFCLLVL(a)   SPLVLENTRY(a,0)
#define DEFMULVL(a)   SPLVLENTRY(0,a)
#define DEFSPLVL(a,b) SPLVLENTRY(a,b)

#define DEFCLERICROW(a,b,c,d,e,f,g) { DEFCLLVL(a),DEFCLLVL(b),DEFCLLVL(c),DEFCLLVL(d),DEFCLLVL(e),DEFCLLVL(f),DEFCLLVL(g),DEFCLLVL(0),DEFCLLVL(0) }
#define DEFDRUIDROW(a,b,c,d,e,f,g) { DEFCLLVL(a),DEFCLLVL(b),DEFCLLVL(c),DEFCLLVL(d),DEFCLLVL(e),DEFCLLVL(f),DEFCLLVL(g),DEFCLLVL(0),DEFCLLVL(0) }
#define DEFPALADINROW(a,b,c,d) { DEFCLLVL(a),DEFCLLVL(b),DEFCLLVL(c),DEFCLLVL(d),DEFCLLVL(0),DEFCLLVL(0),DEFCLLVL(0),DEFCLLVL(0),DEFCLLVL(0) }
#define DEFRANGERROW(a,b,c, d,e) { DEFSPLVL(a,d),DEFSPLVL(b,e),DEFSPLVL(c,0),DEFCLLVL(0),DEFCLLVL(0),DEFCLLVL(0),DEFCLLVL(0),DEFCLLVL(0),DEFCLLVL(0) }
#define DEFMUROW(a,b,c,d,e,f,g,h,i) { DEFMULVL(a),DEFMULVL(b),DEFMULVL(c),DEFMULVL(d),DEFMULVL(e),DEFMULVL(f),DEFMULVL(g),DEFMULVL(h),DEFMULVL(i) }
#define DEFFIGHTERROW { DEFSPLVL(0,0),DEFSPLVL(0,0),DEFSPLVL(0,0),DEFSPLVL(0,0),DEFSPLVL(0,0),DEFSPLVL(0,0),DEFSPLVL(0,0),DEFSPLVL(0,0),DEFSPLVL(0,0) }
#define DEFTHIEFROW { DEFSPLVL(0,0),DEFSPLVL(0,0),DEFSPLVL(0,0),DEFSPLVL(0,0),DEFSPLVL(0,0),DEFSPLVL(0,0),DEFSPLVL(0,0),DEFSPLVL(0,0),DEFSPLVL(0,0) }

// before modifications, the maximum spells per spell lvl 
// knowable for each character level.
// 
SPLVLENTRY BaseFighterSpellLvls[HIGHEST_LEVEL_ALLOWED][MAX_SPELL_LEVEL] = 
{
  DEFFIGHTERROW,
  DEFFIGHTERROW,
  DEFFIGHTERROW,
  DEFFIGHTERROW,
  DEFFIGHTERROW,
  DEFFIGHTERROW,
  DEFFIGHTERROW,
  DEFFIGHTERROW,
  DEFFIGHTERROW,
  DEFFIGHTERROW,
  DEFFIGHTERROW,
  DEFFIGHTERROW,
  DEFFIGHTERROW,
  DEFFIGHTERROW,
  DEFFIGHTERROW,
  DEFFIGHTERROW,
  DEFFIGHTERROW,
  DEFFIGHTERROW,
  DEFFIGHTERROW,
  DEFFIGHTERROW,
  DEFFIGHTERROW,
  DEFFIGHTERROW,
  DEFFIGHTERROW,
  DEFFIGHTERROW,
  DEFFIGHTERROW,
  DEFFIGHTERROW,
  DEFFIGHTERROW,
  DEFFIGHTERROW,
  DEFFIGHTERROW,
  DEFFIGHTERROW
};
SPLVLENTRY BaseThiefSpellLvls[HIGHEST_LEVEL_ALLOWED][MAX_SPELL_LEVEL] = 
{
  DEFTHIEFROW,
  DEFTHIEFROW,
  DEFTHIEFROW,
  DEFTHIEFROW,
  DEFTHIEFROW,
  DEFTHIEFROW,
  DEFTHIEFROW,
  DEFTHIEFROW,
  DEFTHIEFROW,
  DEFTHIEFROW,
  DEFTHIEFROW,
  DEFTHIEFROW,
  DEFTHIEFROW,
  DEFTHIEFROW,
  DEFTHIEFROW,
  DEFTHIEFROW,
  DEFTHIEFROW,
  DEFTHIEFROW,
  DEFTHIEFROW,
  DEFTHIEFROW,
  DEFTHIEFROW,
  DEFTHIEFROW,
  DEFTHIEFROW,
  DEFTHIEFROW,
  DEFTHIEFROW,
  DEFTHIEFROW,
  DEFTHIEFROW,
  DEFTHIEFROW,
  DEFTHIEFROW,
  DEFTHIEFROW
};

SPLVLENTRY BaseClericSpellLvls[HIGHEST_LEVEL_ALLOWED][MAX_SPELL_LEVEL] = 
{
  DEFCLERICROW(1,0,0,0,0,0,0),
  DEFCLERICROW(2,0,0,0,0,0,0),
  DEFCLERICROW(2,1,0,0,0,0,0),

  DEFCLERICROW(3,2,0,0,0,0,0),
  DEFCLERICROW(3,3,1,0,0,0,0),
  DEFCLERICROW(3,3,2,0,0,0,0),

  DEFCLERICROW(3,3,2,1,0,0,0),
  DEFCLERICROW(3,3,3,2,0,0,0),
  DEFCLERICROW(4,4,3,2,1,0,0),
  
  DEFCLERICROW(4,4,3,3,2,0,0),
  DEFCLERICROW(5,4,4,3,2,1,0),
  DEFCLERICROW(6,5,5,3,2,2,0),
  
  DEFCLERICROW(6,6,6,4,2,2,0),
  DEFCLERICROW(6,6,6,5,3,2,1),
  DEFCLERICROW(6,6,6,6,4,2,1),
  
  DEFCLERICROW(7,7,7,6,4,3,1),
  DEFCLERICROW(7,7,7,7,5,3,2),
  DEFCLERICROW(8,8,8,8,6,4,2),
  
  DEFCLERICROW(9,9,8,8,6,4,2),
  DEFCLERICROW(9,9,9,8,7,5,2),
  DEFCLERICROW(9,9,9,8,7,5,2),
  
  DEFCLERICROW(9,9,9,8,7,5,2),
  DEFCLERICROW(9,9,9,8,7,5,2),
  DEFCLERICROW(9,9,9,8,7,5,2),
  
  DEFCLERICROW(9,9,9,8,7,5,2),
  DEFCLERICROW(9,9,9,8,7,5,2),
  DEFCLERICROW(9,9,9,8,7,5,2),
  
  DEFCLERICROW(9,9,9,8,7,5,2),
  DEFCLERICROW(9,9,9,8,7,5,2),
  DEFCLERICROW(9,9,9,8,7,5,2)
};

SPLVLENTRY BaseDruidSpellLvls[HIGHEST_LEVEL_ALLOWED][MAX_SPELL_LEVEL] = 
{
  DEFDRUIDROW(1,0,0,0,0,0,0),
  DEFDRUIDROW(2,0,0,0,0,0,0),
  DEFDRUIDROW(2,1,0,0,0,0,0),

  DEFDRUIDROW(3,2,0,0,0,0,0),
  DEFDRUIDROW(3,3,1,0,0,0,0),
  DEFDRUIDROW(3,3,2,0,0,0,0),

  DEFDRUIDROW(3,3,2,1,0,0,0),
  DEFDRUIDROW(3,3,3,2,0,0,0),
  DEFDRUIDROW(4,4,3,2,1,0,0),
  
  DEFDRUIDROW(4,4,3,3,2,0,0),
  DEFDRUIDROW(5,4,4,3,2,1,0),
  DEFDRUIDROW(6,5,5,3,2,2,0),
  
  DEFDRUIDROW(6,6,6,4,2,2,0),
  DEFDRUIDROW(6,6,6,5,3,2,1),
  DEFDRUIDROW(6,6,6,6,4,2,1),
  
  DEFDRUIDROW(7,7,7,6,4,3,1),
  DEFDRUIDROW(7,7,7,7,5,3,2),
  DEFDRUIDROW(8,8,8,8,6,4,2),
  
  DEFDRUIDROW(9,9,8,8,6,4,2),
  DEFDRUIDROW(9,9,9,8,7,5,2),
  DEFDRUIDROW(9,9,9,8,7,5,2),
  
  DEFDRUIDROW(9,9,9,8,7,5,2),
  DEFDRUIDROW(9,9,9,8,7,5,2),
  DEFDRUIDROW(9,9,9,8,7,5,2),
  
  DEFDRUIDROW(9,9,9,8,7,5,2),
  DEFDRUIDROW(9,9,9,8,7,5,2),
  DEFDRUIDROW(9,9,9,8,7,5,2),
  
  DEFDRUIDROW(9,9,9,8,7,5,2),
  DEFDRUIDROW(9,9,9,8,7,5,2),
  DEFDRUIDROW(9,9,9,8,7,5,2)
};

SPLVLENTRY BasePaladinSpellLvls[HIGHEST_LEVEL_ALLOWED][MAX_SPELL_LEVEL] = 
{
  DEFPALADINROW(0,0,0,0),
  DEFPALADINROW(0,0,0,0),
  DEFPALADINROW(0,0,0,0),
  
  DEFPALADINROW(0,0,0,0),
  DEFPALADINROW(0,0,0,0),
  DEFPALADINROW(0,0,0,0),
  
  DEFPALADINROW(0,0,0,0),
  DEFPALADINROW(0,0,0,0),
  DEFPALADINROW(1,0,0,0),
  
  DEFPALADINROW(2,0,0,0),
  DEFPALADINROW(2,1,0,0),
  DEFPALADINROW(2,2,0,0),
  
  DEFPALADINROW(2,2,1,0),
  DEFPALADINROW(3,2,1,0),
  DEFPALADINROW(3,2,1,1),
  
  DEFPALADINROW(3,3,1,1),
  DEFPALADINROW(3,3,2,1),
  DEFPALADINROW(3,3,3,1),
  
  DEFPALADINROW(3,3,3,2),
  DEFPALADINROW(3,3,3,3),
  DEFPALADINROW(3,3,3,3),
  
  DEFPALADINROW(3,3,3,3),
  DEFPALADINROW(3,3,3,3),
  DEFPALADINROW(3,3,3,3),
  
  DEFPALADINROW(3,3,3,3),
  DEFPALADINROW(3,3,3,3),
  DEFPALADINROW(3,3,3,3),
  
  DEFPALADINROW(3,3,3,3),
  DEFPALADINROW(3,3,3,3),
  DEFPALADINROW(3,3,3,3)
};

SPLVLENTRY BaseRangerSpellLvls[HIGHEST_LEVEL_ALLOWED][MAX_SPELL_LEVEL] = 
{
  DEFRANGERROW(0,0,0, 0,0),
  DEFRANGERROW(0,0,0, 0,0),
  DEFRANGERROW(0,0,0, 0,0),
  
  DEFRANGERROW(0,0,0, 0,0),
  DEFRANGERROW(0,0,0, 0,0),
  DEFRANGERROW(0,0,0, 0,0),
  
  DEFRANGERROW(0,0,0, 0,0),
  DEFRANGERROW(1,0,0, 0,0),
  DEFRANGERROW(2,0,0, 0,0),
  
  DEFRANGERROW(2,1,0, 0,0),
  DEFRANGERROW(2,2,0, 0,0),
  DEFRANGERROW(2,2,1, 0,0),
  
  DEFRANGERROW(3,2,1, 0,0),
  DEFRANGERROW(3,2,2, 0,0),
  DEFRANGERROW(3,3,2, 0,0),
  
  DEFRANGERROW(3,3,3, 0,0),
  DEFRANGERROW(3,3,3, 0,0),
  DEFRANGERROW(3,3,3, 0,0),
  
  DEFRANGERROW(3,3,3, 0,0),
  DEFRANGERROW(3,3,3, 0,0),
  DEFRANGERROW(3,3,3, 0,0),
  
  DEFRANGERROW(3,3,3, 0,0),
  DEFRANGERROW(3,3,3, 0,0),
  DEFRANGERROW(3,3,3, 0,0),
  
  DEFRANGERROW(3,3,3, 0,0),
  DEFRANGERROW(3,3,3, 0,0),
  DEFRANGERROW(3,3,3, 0,0),
  
  DEFRANGERROW(3,3,3, 0,0),
  DEFRANGERROW(3,3,3, 0,0),
  DEFRANGERROW(3,3,3, 0,0)
};

SPLVLENTRY BaseMuSpellLvls[HIGHEST_LEVEL_ALLOWED][MAX_SPELL_LEVEL] = 
{
  DEFMUROW(1,0,0,0,0,0,0,0,0),
  DEFMUROW(2,0,0,0,0,0,0,0,0),
  DEFMUROW(2,1,0,0,0,0,0,0,0),
  
  DEFMUROW(3,2,0,0,0,0,0,0,0),
  DEFMUROW(4,2,1,0,0,0,0,0,0),
  DEFMUROW(4,2,2,0,0,0,0,0,0),
  
  DEFMUROW(4,3,2,1,0,0,0,0,0),
  DEFMUROW(4,3,3,2,0,0,0,0,0),
  DEFMUROW(4,3,3,2,1,0,0,0,0),
  
  DEFMUROW(4,4,3,2,2,0,0,0,0),
  DEFMUROW(4,4,4,3,3,0,0,0,0),
  DEFMUROW(4,4,4,4,4,1,0,0,0),
  
  DEFMUROW(5,5,5,4,4,2,0,0,0),
  DEFMUROW(5,5,5,4,4,2,1,0,0),
  DEFMUROW(5,5,5,5,5,2,1,0,0),
  
  DEFMUROW(5,5,5,5,5,3,2,1,0),
  DEFMUROW(5,5,5,5,5,3,3,2,0),
  DEFMUROW(5,5,5,5,5,3,3,2,1),
  
  DEFMUROW(5,5,5,5,5,3,3,3,1),
  DEFMUROW(5,5,5,5,5,4,3,3,2),
  DEFMUROW(5,5,5,5,5,4,3,3,2),
  
  DEFMUROW(5,5,5,5,5,4,3,3,2),
  DEFMUROW(5,5,5,5,5,4,3,3,2),
  DEFMUROW(5,5,5,5,5,4,3,3,2),
  
  DEFMUROW(5,5,5,5,5,4,3,3,2),
  DEFMUROW(5,5,5,5,5,4,3,3,2),
  DEFMUROW(5,5,5,5,5,4,3,3,2),
  
  DEFMUROW(5,5,5,5,5,4,3,3,2),
  DEFMUROW(5,5,5,5,5,4,3,3,2),
  DEFMUROW(5,5,5,5,5,4,3,3,2)
};

// based on char intelligence, the maximum
// spells of each level that can be known.
// 0 = none, 255 = unlimited
//
// for example, a mu with intel 9 can only
// know a max of 6 first lvl spells, 6 2nd lvl, etc
// and MaxMuSpellsPerSpellLvl[9-1] = 6
//
BYTE MaxFighterSpellsForEachSpellLvlByPrime[19] =
{
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};
BYTE MaxThiefSpellsForEachSpellLvlByPrime[19] =
{
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

BYTE MaxClericSpellsForEachSpellLvlByPrime[19] =
{
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255
};

BYTE MaxDruidSpellsForEachSpellLvlByPrime[19] =
{
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255
};

BYTE MaxPaladinSpellsForEachSpellLvlByPrime[19] =
{
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255
};

BYTE MaxRangerSpellsForEachSpellLvlByPrime[19] =
{
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255
};

BYTE MaxMuSpellsForEachSpellLvlByPrime[19] =
{
  0,0,0,0,0,0,0,0,6,7,7,7,9,9,11,11,14,18,255
};

// the maximum spell level allowed by the
// character's prime requisite score
// 
BYTE MaxFighterSpellLvlByPrime[19] = 
{
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};
BYTE MaxThiefSpellLvlByPrime[19] = 
{
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};
BYTE MaxClericSpellLvlByPrime[19] = 
{
  0,0,0,0,0,0,0,0,5,5,5,5,5,5,5,5,6,7,7
};
BYTE MaxDruidSpellLvlByPrime[19] = 
{
  0,0,0,0,0,0,0,0,5,5,5,5,5,5,5,5,6,7,7
};
BYTE MaxPaladinSpellLvlByPrime[19] = 
{
  0,0,0,0,0,0,0,0,5,5,5,5,5,5,5,5,6,7,7
};
BYTE MaxRangerSpellLvlByPrime[19] = 
{
  0,0,0,0,0,0,0,0,4,5,5,6,6,7,7,8,8,9,9
};
BYTE MaxMuSpellLvlByPrime[19] = 
{
  0,0,0,0,0,0,0,0,4,5,5,6,6,7,7,8,8,9,9
};

// the bonus spells allowed per spell lvl by the
// character's prime requisite score
// 

#define DEFSPELLBONUS(q,l)   SPBONUSENTRY(q,l)
#define DEFSPELLBONUSNONE    SPBONUSENTRY(0,0)

SPBONUSENTRY BonusFighterSpellsByPrime[19] =
{
  DEFSPELLBONUSNONE,
  DEFSPELLBONUSNONE,
  DEFSPELLBONUSNONE,
  DEFSPELLBONUSNONE,
  DEFSPELLBONUSNONE,
  DEFSPELLBONUSNONE,
  DEFSPELLBONUSNONE,
  DEFSPELLBONUSNONE,
  DEFSPELLBONUSNONE,
  DEFSPELLBONUSNONE,
  DEFSPELLBONUSNONE,
  DEFSPELLBONUSNONE,
  DEFSPELLBONUSNONE,
  DEFSPELLBONUSNONE,
  DEFSPELLBONUSNONE,
  DEFSPELLBONUSNONE,
  DEFSPELLBONUSNONE,
  DEFSPELLBONUSNONE,
  DEFSPELLBONUSNONE
};
SPBONUSENTRY BonusThiefSpellsByPrime[19] =
{
  DEFSPELLBONUSNONE,
  DEFSPELLBONUSNONE,
  DEFSPELLBONUSNONE,
  DEFSPELLBONUSNONE,
  DEFSPELLBONUSNONE,
  DEFSPELLBONUSNONE,
  DEFSPELLBONUSNONE,
  DEFSPELLBONUSNONE,
  DEFSPELLBONUSNONE,
  DEFSPELLBONUSNONE,
  DEFSPELLBONUSNONE,
  DEFSPELLBONUSNONE,
  DEFSPELLBONUSNONE,
  DEFSPELLBONUSNONE,
  DEFSPELLBONUSNONE,
  DEFSPELLBONUSNONE,
  DEFSPELLBONUSNONE,
  DEFSPELLBONUSNONE,
  DEFSPELLBONUSNONE
};
SPBONUSENTRY BonusClericSpellsByPrime[19] =
{
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(1,1),
  DEFSPELLBONUS(1,1),
  DEFSPELLBONUS(1,2),
  DEFSPELLBONUS(1,2),
  DEFSPELLBONUS(1,3),
  DEFSPELLBONUS(1,4),
  DEFSPELLBONUS(1,4)
};
SPBONUSENTRY BonusDruidSpellsByPrime[19] =
{
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(1,1),
  DEFSPELLBONUS(1,1),
  DEFSPELLBONUS(1,2),
  DEFSPELLBONUS(1,2),
  DEFSPELLBONUS(1,3),
  DEFSPELLBONUS(1,4),
  DEFSPELLBONUS(1,4)
};
SPBONUSENTRY BonusPaladinSpellsByPrime[19] =
{
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(1,1),
  DEFSPELLBONUS(1,1),
  DEFSPELLBONUS(1,2),
  DEFSPELLBONUS(1,2),
  DEFSPELLBONUS(1,3),
  DEFSPELLBONUS(1,4),
  DEFSPELLBONUS(1,4)
};
SPBONUSENTRY BonusRangerSpellsByPrime[19] =
{
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(1,1),
  DEFSPELLBONUS(1,1),
  DEFSPELLBONUS(1,2),
  DEFSPELLBONUS(1,2),
  DEFSPELLBONUS(1,3),
  DEFSPELLBONUS(1,4),
  DEFSPELLBONUS(1,4)
};
SPBONUSENTRY BonusMuSpellsByPrime[19] =
{
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0),
  DEFSPELLBONUS(0,0)
};

///////////////////////////////////////////////////////////////////////
//
// Global Spell Functions
//
///////////////////////////////////////////////////////////////////////

BOOL NumClassBits(WORD ClassFlag)
{
  int count=0;
  for (int i=0;i<NumClassFlags;i++)
  {
    if (ClassFlag & (0x0001 << i)) 
      count++;
  }  
  return count;
}

BYTE GetMaxSpellsForSpellLvlPrime(WORD CharClassFlag, BYTE prime)
{
  ASSERT( NumClassBits(CharClassFlag) == 1 );

  if (prime > 19) prime = 19;
  if (prime < 1) prime = 1;
  prime--;

  switch (CharClassFlag)
  {
  case ClericFlag:    return MaxClericSpellsForEachSpellLvlByPrime[prime];
  case MagicUserFlag: return MaxMuSpellsForEachSpellLvlByPrime[prime];
  case ThiefFlag:     return MaxThiefSpellsForEachSpellLvlByPrime[prime];
  case FighterFlag:   return MaxFighterSpellsForEachSpellLvlByPrime[prime];
  case PaladinFlag:   return MaxPaladinSpellsForEachSpellLvlByPrime[prime];
  case RangerFlag:    return MaxRangerSpellsForEachSpellLvlByPrime[prime];
  case DruidFlag:     return MaxDruidSpellsForEachSpellLvlByPrime[prime];
  default: ASSERT(FALSE);
  }
  return 0;
}

BYTE GetMaxSpellLvlByPrime(WORD CharClassFlag, BYTE prime)
{
  ASSERT( NumClassBits(CharClassFlag) == 1 );

  if (prime > 19) prime = 19;
  if (prime < 1) prime = 1;
  prime--;
  switch (CharClassFlag)
  {
  case ClericFlag:    return MaxClericSpellLvlByPrime[prime];
  case MagicUserFlag: return MaxMuSpellLvlByPrime[prime];
  case ThiefFlag:     return MaxThiefSpellLvlByPrime[prime];
  case FighterFlag:   return MaxFighterSpellLvlByPrime[prime];
  case PaladinFlag:   return MaxPaladinSpellLvlByPrime[prime];
  case RangerFlag:    return MaxRangerSpellLvlByPrime[prime];
  case DruidFlag:     return MaxDruidSpellLvlByPrime[prime];
  default: ASSERT(FALSE);
  }
  return 0;
}

BYTE GetBaseSpellLvlBySpellClass( SPLVLENTRY data, WORD SpellClassFlag )
{
  ASSERT( NumClassBits(SpellClassFlag) == 1 );

  switch (SpellClassFlag)
  {
  case ClericFlag: return data.ClLvl;
  case MagicUserFlag: return data.MuLvl;
  default: ASSERT(FALSE);
  }
  return 0;
}

BYTE GetBonusSpellQtyBySpellLevel( int prime, int SpellLevel, WORD CharClassFlag )
{
  ASSERT( NumClassBits(CharClassFlag) == 1 );

  if (prime > 19) prime = 19;
  if (prime < 1) prime = 1;
  prime--;
  switch (CharClassFlag)
  {
  case ClericFlag:    { if (BonusClericSpellsByPrime[prime].Lvl==SpellLevel) return BonusClericSpellsByPrime[prime].Qty; } break;
  case MagicUserFlag: { if (BonusMuSpellsByPrime[prime].Lvl==SpellLevel) return BonusClericSpellsByPrime[prime].Qty; } break;
  case ThiefFlag:     { if (BonusThiefSpellsByPrime[prime].Lvl==SpellLevel) return BonusClericSpellsByPrime[prime].Qty; } break;
  case FighterFlag:   { if (BonusFighterSpellsByPrime[prime].Lvl==SpellLevel) return BonusClericSpellsByPrime[prime].Qty; } break;
  case PaladinFlag:   { if (BonusPaladinSpellsByPrime[prime].Lvl==SpellLevel) return BonusClericSpellsByPrime[prime].Qty; } break;
  case RangerFlag:    { if (BonusRangerSpellsByPrime[prime].Lvl==SpellLevel) return BonusClericSpellsByPrime[prime].Qty; } break;
  case DruidFlag:     { if (BonusDruidSpellsByPrime[prime].Lvl==SpellLevel) return BonusClericSpellsByPrime[prime].Qty; } break;
  default: ASSERT(FALSE);
  }
  return 0;
}

BYTE GetBaseSpellLvl(WORD CharClassFlag, WORD CastingClassFlag, int CharLevel, int SpellLevel)
{
  ASSERT( NumClassBits(CharClassFlag) == 1 );
  ASSERT( NumClassBits(CastingClassFlag) == 1 );

  if (CharLevel > HIGHEST_LEVEL_ALLOWED) CharLevel = HIGHEST_LEVEL_ALLOWED;
  if (CharLevel < 1) CharLevel = 1;
  CharLevel--;

  if (SpellLevel > MAX_SPELL_LEVEL) SpellLevel = MAX_SPELL_LEVEL;
  if (SpellLevel < 1) SpellLevel = 1;
  SpellLevel--;

  switch (CharClassFlag)
  {
  case MagicUserFlag: return GetBaseSpellLvlBySpellClass( BaseMuSpellLvls[CharLevel][SpellLevel],      CastingClassFlag);
  case ClericFlag:    return GetBaseSpellLvlBySpellClass( BaseClericSpellLvls[CharLevel][SpellLevel],  CastingClassFlag);
  case ThiefFlag:     return GetBaseSpellLvlBySpellClass( BaseThiefSpellLvls[CharLevel][SpellLevel],   CastingClassFlag);
  case FighterFlag:   return GetBaseSpellLvlBySpellClass( BaseFighterSpellLvls[CharLevel][SpellLevel], CastingClassFlag);
  case PaladinFlag:   return GetBaseSpellLvlBySpellClass( BasePaladinSpellLvls[CharLevel][SpellLevel], CastingClassFlag);
  case RangerFlag:    return GetBaseSpellLvlBySpellClass( BaseRangerSpellLvls[CharLevel][SpellLevel],  CastingClassFlag);
  case DruidFlag:     return GetBaseSpellLvlBySpellClass( BaseDruidSpellLvls[CharLevel][SpellLevel],   CastingClassFlag);
  default: ASSERT(FALSE);
  }
  return 0;
}

BYTE GetBaseSpells(int CharLevel, int prime, WORD CharClass, WORD CastingClass, int SpellLevel )
{  
  int maxlvl = GetMaxSpellLvlByPrime(CharClass, prime);
  if (SpellLevel > maxlvl) return 0;
  BYTE result = 0;
  //for (int i=1;i<=prime;i++)
  //  result += GetBonusSpellQtyBySpellLevel(i, SpellLevel, CharClass);
  result += GetBaseSpellLvl(CharClass, CastingClass, CharLevel, SpellLevel);  
  int MaxPerSpellLvl = GetMaxSpellsForSpellLvlPrime(CharClass, prime);
  if (result > MaxPerSpellLvl) result = MaxPerSpellLvl;
  return result;
}

void SetSpellLimits(WORD CharClass, int prime, spellLimitsType &limits)
{ 
  if (GetMaxSpellsForSpellLvlPrime(CharClass, prime) == 0) return; // cannot cast any spells

  int index = FlagToIndex(CharClass);
  if (limits.ValidSpellClass(CharClass)==0)
  {
    // if adding new spell casting class, zero out
    for (int i=0;i<MAX_SPELL_LEVEL;i++)
      limits.numSpellsKnown[index][i] = 0;
  }

  limits.spellClasses |= CharClass;
  limits.PrimeScore[index]=prime;
}

// max spell level that a character can cast at the current character level
// ie: a second level cleric can cast only first level cleric spells
int CurrSpellLevel(WORD CharClass, int CharLevel, BYTE Prime, WORD CastingClass)
{
  // type is character class mask
  // level is character level

   for (int i=MAX_SPELL_LEVEL;i>0;i--)
   {
     if (GetBaseSpells(CharLevel, Prime, CharClass, CastingClass, i) > 0) 
       return i;
   }
   return 0;
}

// Spells fall into two schools, MU and Cleric.
// Convert spell class into a school
//

WORD GetSpellCastingClassForCharClass(WORD CharClassFlag)
{
  ASSERT( NumClassBits(CharClassFlag) == 1 );

  // I have double-checked this - all classes except MU use
  // cleric spells.
  switch (CharClassFlag)
  {
  case ClericFlag:    return ClericFlag;
  case MagicUserFlag: return MagicUserFlag;
  case ThiefFlag:     return ClericFlag;
  case FighterFlag:   return ClericFlag;
  case PaladinFlag:   return ClericFlag;
  case RangerFlag:    return ClericFlag;
  case DruidFlag:     return ClericFlag;
  default: ASSERT(FALSE);
  }
  return ClericFlag;
}

// Split character class into two possible spell classes
// Multi-Class chars never have more than three spell classes available to them
//
void GetSpellClasses(classType CharClass, WORD &c1, WORD &c2, WORD &c3)
{
  c1=c2=c3=0;
  WORD mask = CharClassToClassFlag(CharClass);

  for (int i=0;i<NumClassFlags;i++)
  {
    WORD flag = (mask & (0x0001 << i));
    if (flag)
    {
      if (c1==0) 
        c1=flag;
      else if (c2==0)
        c2=flag;
      else if (c3==0)
        c3=flag;
      else ASSERT(FALSE); // shouldn't be more than 3 classes in CharClass!
    }
  }
  // at least one must be valid
  ASSERT( c1!=0 );
}

///////////////////////////////////////////////////////////////////////
//
// Spell Limits Functions
//
///////////////////////////////////////////////////////////////////////

void spellLimitsType::Serialize(CArchive &ar, double version)
{
  int i;
  if (ar.IsStoring())
  {
    ar << spellClasses;    
    ar << UseLimits;
    for (i=0;i<NumClassFlags;i++)
      ar << PrimeScore[i];
  }
  else
  {   
    ar >> spellClasses;
   
    if (version < _VERSION_0780_)
    {
  	  BYTE  maxSpellLevel[NumClassFlags];
      BYTE  maxSpells[NumClassFlags];
	    BYTE  maxSpellsPerLevel[NumClassFlags][MAX_SPELL_LEVEL];
      for (i=0;i<NumClassFlags;i++)
      {
        ar >> maxSpellLevel[i];
        ar >> maxSpells[i];
        for (int j=0;j<MAX_SPELL_LEVEL;j++)
          ar >> maxSpellsPerLevel[i][j];
      }
    }

    if (version >= _VERSION_0780_)
    {
      ar >> UseLimits;
      for (i=0;i<NumClassFlags;i++)
        ar >> PrimeScore[i];
    }
    else
    {
      UseLimits=TRUE;
      for (i=0;i<NumClassFlags;i++)
        PrimeScore[i]=18;
    }
  }
}

void spellLimitsType::Serialize(CAR &ar, double version)
{
  int i;
  if (ar.IsStoring())
  {
    ar << spellClasses;    
    ar << UseLimits;
    for (i=0;i<NumClassFlags;i++)
      ar << PrimeScore[i];
  }
  else
  {
    ar >> spellClasses;
   
    if (version < _VERSION_0780_)
    {
  	  BYTE  maxSpellLevel[NumClassFlags];
      BYTE  maxSpells[NumClassFlags];
	    BYTE  maxSpellsPerLevel[NumClassFlags][MAX_SPELL_LEVEL];
      for (i=0;i<NumClassFlags;i++)
      {
        ar >> maxSpellLevel[i];
        ar >> maxSpells[i];
        for (int j=0;j<MAX_SPELL_LEVEL;j++)
          ar >> maxSpellsPerLevel[i][j];
      }
    }

    if (version >= _VERSION_0780_)
    {
      ar >> UseLimits;
      for (i=0;i<NumClassFlags;i++)
        ar >> PrimeScore[i];
    }
    else
    {
      UseLimits=TRUE;
      for (i=0;i<NumClassFlags;i++)
        PrimeScore[i]=18;
    }
  }
}

spellLimitsType& spellLimitsType::operator=(const spellLimitsType &src)
{
  if (&src == this) return *this;
  spellClasses = src.spellClasses;
  UseLimits=src.UseLimits;
  for (int i=0;i<NumClassFlags;i++)
  {
    PrimeScore[i] = src.PrimeScore[i];
    for (int j=0;j<MAX_SPELL_LEVEL;j++)
      numSpellsKnown[i][j] = src.numSpellsKnown[i][j];
  }    
  return *this;
}

BOOL spellLimitsType::operator==(const spellLimitsType &src)
{
  if (&src == this) return TRUE;
  if (spellClasses != src.spellClasses) return FALSE;
  if (UseLimits != src.UseLimits) return FALSE;
  for (int i=0;i<NumClassFlags;i++)
  {
    if (PrimeScore[i] != src.PrimeScore[i]) return FALSE;
    for (int j=0;j<MAX_SPELL_LEVEL;j++)
    {
      if (numSpellsKnown[i][j] != src.numSpellsKnown[i][j]) return FALSE;
    }
  }    
  return TRUE;
}

void spellLimitsType::Clear()
{
  spellClasses = 0;
  UseLimits=TRUE;
  for (int i=0;i<NumClassFlags;i++)
  {
    PrimeScore[i]=0;
    for (int j=0;j<MAX_SPELL_LEVEL;j++)
      numSpellsKnown[i][j] = 0;
  }    
}

void spellLimitsType::SetToMaxLimits()
{
  spellClasses = AllClasses;
  UseLimits=FALSE;

  for (int i=0;i<NumClassFlags;i++)
  {
    PrimeScore[i]=18;
    for (int j=0;j<MAX_SPELL_LEVEL;j++)
      numSpellsKnown[i][j] = 0;
  }    
}

// Do the class bits overlap with the spell book's class bits?
BOOL spellLimitsType::ValidSpellClass(WORD CastMask) 
{ 
  if (!UseLimits) return TRUE; 
  return ((CastMask & spellClasses) ? TRUE : FALSE); 
}

BOOL spellLimitsType::AddSpell(WORD CasterClassFlag, WORD SpellClassFlag, WORD CastMask, int SpellLevel)
{
  ASSERT( (SpellLevel>=1)&&(SpellLevel<=MAX_SPELL_LEVEL) );
  ASSERT( NumClassBits(CasterClassFlag) == 1 );
  
  BOOL OkToAdd = CanKnowSpell(CasterClassFlag, SpellClassFlag, CastMask, SpellLevel);

  if (!UseLimits) 
    OkToAdd = TRUE;

  if (OkToAdd)
  {
    numSpellsKnown[FlagToIndex(CasterClassFlag)][SpellLevel-1]++;
    return TRUE;
  }
  else
    return FALSE;
}

// remove one spell of this level, for this class
void spellLimitsType::RemoveSpell(WORD CasterClassFlag, int SpellLevel)
{ 
  ASSERT( (SpellLevel>=1)&&(SpellLevel<=MAX_SPELL_LEVEL) );
  if (numSpellsKnown[FlagToIndex(CasterClassFlag)][SpellLevel-1] > 0)
    numSpellsKnown[FlagToIndex(CasterClassFlag)][SpellLevel-1]--;    
}

// remove all spells of this level, for all classes
void spellLimitsType::RemoveSpellLevel(int SpellLevel)
{
  ASSERT( (SpellLevel>=1)&&(SpellLevel<=MAX_SPELL_LEVEL) );
  for (int i=0;i<NumClassFlags;i++)
    numSpellsKnown[i][SpellLevel-1] = 0;
}

// ie: how many cleric spells of any level?
BYTE spellLimitsType::numKnown(WORD ClassFlag, int SpellLevel)
{ 
  ASSERT( (SpellLevel>=1)&&(SpellLevel<=MAX_SPELL_LEVEL) );
  return numSpellsKnown[FlagToIndex(ClassFlag)][SpellLevel-1];
  /*
  BYTE count=0;
  for (int i=0;i<MAX_SPELL_LEVEL;i++)
    count += numSpellsKnown[FlagToIndex(ClassFlag)][i];
  return count; 
  */
}

// max spells of SpellLevel that can be memorized by char at current CharLevel
BYTE spellLimitsType::MaxSpellsForLevel(WORD CharClass, int CharLevel, int SpellLevel, BYTE prime)
{ 
  ASSERT( (SpellLevel>=1)&&(SpellLevel<=MAX_SPELL_LEVEL) );
  if (!UseLimits) return 255;


  if (!ValidSpellClass(CharClass))
    return 0;

  int i;
  int max = GetBaseSpells(CharLevel, prime, CharClass, GetSpellCastingClassForCharClass(CharClass), SpellLevel);

  if (max > 0)
  {
    BYTE bonus = 0;
    for ( i=1;i<=prime;i++)
      bonus += GetBonusSpellQtyBySpellLevel(i, SpellLevel, CharClass);
    max += bonus;

    int MaxPerSpellLvl = GetMaxSpellsForSpellLvlPrime(CharClass, prime);
    if (max > MaxPerSpellLvl)
      max = MaxPerSpellLvl;
  }

  return max;
}

// this one checks the spell limits without regard to current char level,
// since chars can add spells to the book without actually being at a
// char level that can cast the spell
BOOL spellLimitsType::CanKnowSpell(WORD CasterClassFlag, WORD SpellClassFlag, WORD CastMask, int SpellLevel)
{
  ASSERT( (SpellLevel>=1)&&(SpellLevel<=MAX_SPELL_LEVEL) );
  ASSERT( NumClassBits(CasterClassFlag) == 1 );

  if (!UseLimits) return TRUE;
  
  if (!ValidSpellClass(CastMask))
    return FALSE;

  BYTE prime = PrimeScore[FlagToIndex(CasterClassFlag)];  
  
  // what is the highest spell level usable by this char class
  BYTE MaxSpellCastingLevel = GetMaxSpellLvlByPrime(CasterClassFlag, prime);
  if (SpellLevel > MaxSpellCastingLevel) return FALSE;

  // no matter what the char level is, what is the maximum number of spells
  // that can be known at this spell level
  int MaxPerSpellLvl = GetMaxSpellsForSpellLvlPrime(CasterClassFlag, prime);

  if (MaxPerSpellLvl < 255)
  {
    // accumulate spell bonuses based upon prime score
    BYTE bonus = 0;
    for (int i=1;i<=prime;i++)
      bonus += GetBonusSpellQtyBySpellLevel(i, SpellLevel, CasterClassFlag);

    MaxPerSpellLvl += bonus;
  }

  if (MaxPerSpellLvl > 255) MaxPerSpellLvl = 255;

  return ( numKnown(CasterClassFlag, SpellLevel) < MaxPerSpellLvl );
}


/*
Memorizing a spell takes 15 minutes of games time per spell level, plus a
preparation period of four hours plus an additional two hours for 3rd level
spells, etc.  For example, 1st and 2nd level spells take a minimum preparation of
four hours, while 3rd level spells take six hours.

Example: To memorize two 1st level spells, one 2nd level spell, and one 3rd
level spell would take: (6 hours preparation) + (2 * 15 min) + (1 * 30 min) +
(1 * 45 min) = 7 hours 45 min
*/
// in minutes
int GetSpellMemorizeTime(int level)
{
  return (level*15);
}

// in minutes
int GetSpellPrepTime(int level)
{
  switch (level)
  {
  case -1:
  case 0:  return 0;

  case 1:
  case 2:  return 4*60; // 4 hours

  case 3:
  case 4:  return 6*60; // 6 hours

  case 5:
  case 6:  return 8*60; // 8 hours

  case 7:
  case 8:  return 10*60; // 10 hours

  default: return 12*60; // 12 hours
  }
}