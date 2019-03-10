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

#ifndef DUNGEONCRAFT_SHARED_MOVIEMONSTER
#define DUNGEONCRAFT_SHARED_MOVIEMONSTER

#include "dungeoncraft/shared/class.h"
#include "dungeoncraft/shared/items.h"
#include "dungeoncraft/shared/money.h"

namespace DungeonCraft {

const int MAX_MONSTERS = 0x00FFFFFF;
const int MAX_MONSTER_NAME = 30; // should match MAX_CHAR_NAME!
const int NUM_SIZE_TYPES = 3;
enum creatureSizeType { Small, Medium, Large };

//*** Monster Special Stuff ***

/*
Form
* MAMMAL
Indicates whether the monster is vulnerable to the second level Druid
Spell "Charm Person or Mammal."

* ANIMAL
Determines whether the monster is susceptible to the first level Druid
spell "Invisibility to Animals."

* SNAKE
Determines whether the monster is vulnerable to the second level
Clerical spell "Snake Charm."

* TRUE GIANT
Indicates whether the monster takes extra damage from a Long Sword vs
Giants.

* LARGE EVEN IF ICON 1 BY 1
Monster receives weapon damage based on the "Large" column on the
damage table.  
*/

enum MonsterFormType { FormMammal=1, FormAnimal=2, FormSnake=4, FormGiant=8, FormLarge=16 /*,FormUndead=32*/ };

/*
Penalties

* DWARF AC
Indicates that creatures with the effect "Dwarf AC bonus" (e.g.
dwarves) get an AC bonus of +4 when attacked by the monster.

* GNOME AC
Indicates that creatures with the effect "Gnome AC bonus" (e.g.
gnomes) get an AC bonus of +4 when attacked by the monster.

* DWARF THAC0
Indicates that creatures with the effect "Dwarf THAC0 +1" (e.g.
dwarves) get a THAC0 bonus of +1 when attacking the monster.

* GNOME THAC0
Indicates that creatures with the effect "Gnome THAC0 +1" (e.g.
gnomes) get a THAC0 bonus of +1 when attacking the monster.

* RANGER DAMAGE
Indicates that creatures with the effect "Ranger Gnt Bonus" (e.g.
rangers) get a damage bonus equal to their ranger levels whenever they
hit the monster.
*/

enum MonsterPenaltyType { PenaltyDwarfAC=1, PenaltyGnomeAC=2, 
                          PenaltyDwarfTHACO=4, PenaltyGnomeTHACO=8,
                          PenaltyRangerDmg=16 };

/*
IMMUNE TO

* POISON
Indicates that the monster is immune to poison, including "stinking
cloud".

* DEATH
Indicates that the monster is immune to all death magic, including
"Cloudkill."

* CONFUSION
Indicates that confusion wands, spells, and gazes have no effect on
the monster.

* VORPAL SWORDS
Indicates that the monster cannot be decapitated by a vorpal blade.
*/

enum MonsterImmunityType { ImmunePoison=1, ImmuneDeath=2, ImmuneConfusion=4, ImmuneVorpal=8 };

/*
MISC OPTIONS

* MAY BE HELD OR CHARMED
Indicates that the monster is vulnerable to charm or hold spells. 
Note that the monster is also invulnerable to "Hold Person" spells if
it is of race "monster"; only humans and similar races are susceptible
to "Hold Person."

* AFFECTED BY DISPEL EVIL
Indicates that the monster is summoned from another plane of existence
and may be affected by the fifth level clerical spell "Dispel Evil".
*/

enum MonsterMiscOptionsType { OptionCanBeHeldCharmed=1, OptionAffectedByDispelEvil=2 };

#define MAX_MONSTER_ATTACK_MSG_LEN 20

struct MONSTER_ATTACK_DETAILS
{
  MONSTER_ATTACK_DETAILS() { Clear(); }
  ~MONSTER_ATTACK_DETAILS() { }
  void Clear() { sides=0;nbr=0;bonus=0;msg="";spelldbkey=-1;spellclass=0;spelllevel=0; }
  MONSTER_ATTACK_DETAILS& operator =(const MONSTER_ATTACK_DETAILS& src)
  {
    if (this==&src) return *this;
    Clear();
    sides=src.sides;
    nbr=src.nbr;
    bonus=src.bonus;
    msg=src.msg;
    spelldbkey=src.spelldbkey;
    spellclass=src.spellclass;
    spelllevel=src.spelllevel;
    return *this;
  }
  bool operator == (const MONSTER_ATTACK_DETAILS& src) const
  {
    if (this==&src) return true;
    if (sides!=src.sides) return false;
    if (nbr!=src.nbr) return false;
    if (bonus!=src.bonus) return false;
    if (msg!=src.msg) return false;
    if (spelldbkey!=src.spelldbkey) return false;
    if (spellclass!=src.spellclass) return false;
    if (spelllevel!=src.spelllevel) return false;
    return true;
  }

  void Serialize(CArchive &ar, double version)
  {
    if (ar.IsStoring())
    {
      ar << sides;
      ar << nbr;
      ar << bonus;
      AS(ar,msg);
      ar << spelldbkey;
      ar << spellclass;
      ar << spelllevel;
    }
    else
    {
      ar >> sides;
      ar >> nbr;
      ar >> bonus;
      DAS(ar,msg);
      if (version >= _VERSION_0904_)
      {
        ar >> spelldbkey;      
        ar >> spellclass;
        ar >> spelllevel;
      }
    }
  }
  void Serialize(CAR &ar, double version)
  {
    if (ar.IsStoring())
    {
      ar << sides;
      ar << nbr;
      ar << bonus;
      AS(ar,msg);
      ar << spelldbkey;
      ar << spellclass;
      ar << spelllevel;
    }
    else
    {
      ar >> sides;
      ar >> nbr;
      ar >> bonus;
      DAS(ar,msg);
      if (version >= _VERSION_0904_)
      {
        ar >> spelldbkey;
        ar >> spellclass;
        ar >> spelllevel;
      }
    }
  }
  int sides; // nbr sides on each dice
  int nbr;   // nbr of dice to roll
  int bonus; // bonus per roll
  int spelldbkey;
  int spellclass; // class/level needed to satisfy using
  int spelllevel; // spellbook class in editor
  CString msg; // attack msg
};

struct MONSTER_ATTACK_DATA
{
  MONSTER_ATTACK_DATA() { Clear(); }
  ~MONSTER_ATTACK_DATA() { }
  void Clear() { data.RemoveAll(); }
  int GetCount() const { return data.GetSize(); }
  MONSTER_ATTACK_DATA& operator =(const MONSTER_ATTACK_DATA& src)
  {
    if (this==&src) return *this;
    Clear();
    for (int i=0;i<src.GetCount();i++)
      Add(src.data[i]);
    return *this;
  }
  bool operator == (const MONSTER_ATTACK_DATA& src) const
  {
    if (this==&src) return true;
    if (GetCount()!=src.GetCount()) return false;
    // order is also important
    for (int i=0;i<GetCount();i++)
    {
      if ( !(data[i]==src.data[i]) )
        return false;
    }
    return true;
  }

  int Add(MONSTER_ATTACK_DETAILS src)
  {
    int index = GetCount();
    data.SetAtGrow(index, src);
    return index;
  }

  void Delete(int index)
  {
    if ((index>=0)&&(index<GetCount()))
      data.RemoveAt(index);
  }
  void Serialize(CArchive &ar, double version)
  {
    if (ar.IsStoring())
    {
      int cnt=GetCount();
      ar << cnt;
      for (int i=0;i<cnt;i++)
        data[i].Serialize(ar,version);
    }
    else
    {
      MONSTER_ATTACK_DETAILS tmp;
      Clear();
      int cnt;
      ar >> cnt;
      for (int i=0;i<cnt;i++)
      {
        tmp.Serialize(ar,version);
        data.Add(tmp);
      }
    }
  }
  void Serialize(CAR &ar, double version)
  {
    if (ar.IsStoring())
    {
      int cnt=GetCount();
      ar << cnt;
      for (int i=0;i<cnt;i++)
        data[i].Serialize(ar,version);
    }
    else
    {
      MONSTER_ATTACK_DETAILS tmp;
      Clear();
      int cnt;
      ar >> cnt;
      for (int i=0;i<cnt;i++)
      {
        tmp.Serialize(ar,version);
        data.Add(tmp);
      }
    }
  }
  MONSTER_ATTACK_DETAILS &GetAtIndex(int index) { return data[index]; }

  CArray<MONSTER_ATTACK_DETAILS, MONSTER_ATTACK_DETAILS&> data;
};

class MONSTER_DATA : public CObject
{
public:
  DECLARE_SERIAL( MONSTER_DATA )
  MONSTER_DATA();
  MONSTER_DATA(const MONSTER_DATA &src) : 
      	mon_asl("MONSTER_DATA_ATTRIBUTES"),
	      temp_asl("MONSTER_DATA_ATTRIBUTES"),
	      hHitSound(-1), hMissSound(-1), hMoveSound(-1), hDeathSound(-1) { Clear(); *this = src; }
  ~MONSTER_DATA() { ClearSounds(); Clear(TRUE); }
  MONSTER_DATA& operator =(const MONSTER_DATA& src);
  bool operator == (const MONSTER_DATA& src) const;

  void PreSerialize(BOOL IsStoring);
  void PostSerialize(BOOL IsStoring);
  void Serialize(CArchive &ar, double version);
  void Serialize(CAR &ar, double version);
  void Save(CArchive& ar);
  void PrepareForRestore(void);
  void Restore(CArchive& ar);
  void CommitRestore(void);
  void Clear(BOOL ctor=FALSE);

  void PlayHit();
  void PlayMiss();
  void PlayMove();
  void PlayDeath();
  void ClearSounds();
  ActorType GetContext();
  void GuessUndeadStatus();

  int key;
	CString Name;
  PIC_DATA Icon;
  
  CString HitSound;
  CString MissSound;
  CString MoveSound;
  CString DeathSound;
  int hHitSound;
  int hMissSound;
  int hMoveSound;
  int hDeathSound;

	long Intelligence;
	long Armor_Class;
	long Movement;
	float Hit_Dice;
  BOOL UseHitDice;
	long Hit_Dice_Bonus;
	long THACO;
	long Magic_Resistance;
	creatureSizeType Size;
	long Morale;
	long XP_Value;
  
	SPECIAL_ABILITIES specAbs;

  BYTE  ItemMask;
  DWORD Form_Type;
  DWORD Penalty_Type;
  DWORD Immunity_Type;
  DWORD MiscOptions_Type;
  ITEM_LIST myItems;
  MONEY_SACK money;
  undeadType undeadStatus;
  MONSTER_ATTACK_DATA attackData;
  ASL mon_asl;
  ASL temp_asl;
};


class MONSTER_DATA_TYPE : public CObject
{
  //POSITION pos;
  CCriticalSection m_CS;
  OrderedQueue<MONSTER_DATA, MAX_MONSTERS> MonsterData;
  int GetNextKey();

public:

  DECLARE_SERIAL( MONSTER_DATA_TYPE )
  MONSTER_DATA_TYPE() { Clear(); }
  ~MONSTER_DATA_TYPE() { Clear(); }

  MONSTER_DATA_TYPE& operator = (MONSTER_DATA_TYPE& src);
  bool operator ==(MONSTER_DATA_TYPE& src) ;
  void Clear() { CSingleLock sLock(&m_CS, TRUE); MonsterData.RemoveAll();version=0.0; }
  void Serialize(CArchive &ar);
  void Serialize(CAR &ar, double version);
  void Save(CArchive& ar);
  void Restore(CArchive& ar);
  void CommitRestore(void);
  int GetCount() { CSingleLock sLock(&m_CS, TRUE); return MonsterData.GetCount(); }
  void SaveSounds();
  void SaveArt();
  void PreSerialize(BOOL IsStoring);
  void PostSerialize(BOOL IsStoring);

  POSITION GetHeadPosition() { return MonsterData.GetHeadPosition(); }
  MONSTER_DATA& GetAt(POSITION pos) { return MonsterData.GetAt(pos); }
  MONSTER_DATA& GetNext(POSITION &pos) { return MonsterData.GetNext(pos); }
  int GetKeyAt(POSITION pos) { return MonsterData.GetKeyAt(pos); }

  void ExpandDefaultFilenames();
#ifdef UAFEDITOR
  void CollapseDefaultFilenames();
#endif

  ActorType GetContext(int index);

  BOOL Validate(MONSTER_DATA data, FILE *pFile);
  BOOL Validate(int index, FILE *pFile); // specific monster
  BOOL Validate(FILE *pFile); // all monsters
  void PlayHit(int index);
  void PlayMiss(int index);
  void PlayMove(int index);
  void PlayDeath(int index);
  void ClearSounds();
  CString GetMonsterName(int index);

  BOOL HaveMonster(int index);
  // called when loading from scripts, or
  // when adding from editor GUI
  int AddMonster(MONSTER_DATA &data);
  // called when loading binary monster data that
  // was saved with key info
  int AddMonsterWithCurrKey(MONSTER_DATA &data);
  BOOL SetMonster(int index, MONSTER_DATA &data);
  BOOL RemoveMonster(int index);
  MONSTER_DATA *GetMonsterData(int index);
  creatureSizeType GetMonsterSize(int index);
  int GetMonsterNbrAttacks(int index);
  int GetMonsterExpValue(int index);
  int GetMonsterTHACO(int index);
  int GetMonsterAC(int index);
  void GetMonsterDamageDice(int index, int attackNum, int &num, int &sides, int &bonus);
  void GetMonsterAttackMsg(int index, int attackNum, CString &msg);
  BYTE GetMonsterItemMask(int index);
  double GetMonsterHitDice(int index);
  int GetMonsterHitDiceBonus(int index);
  int GetMonsterMovement(int index);
  DWORD GetMonsterFormFlags(int index);
  DWORD GetMonsterOptionFlags(int index);
  DWORD GetMonsterImmunityFlags(int index);
  DWORD GetMonsterPenaltyFlags(int index);

  // No locking... for internal use
  int m_FindName (const CString& name);
  int m_FindNamei (const CString& name); // case-insensitive
  double version;

public:
  // With locking...for public use
  int FindName(const CString& name);
  int FindNamei(const CString& name); // case-insensitive
};

} // End of namespace DungeonCraft

#endif