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

#ifndef __CLASS_H__
#define __CLASS_H__

#include "dungeoncraft/shared/SharedQueue.h"

#define MAX_RACES 99
#define MAX_BASE_CLASSES 99
#define MAX_CLASSES 99
#define MAX_ABILITIES 6
#define MAX_TRAITS 500
#define MAX_SPELLGROUPS 500

struct CONFIG_PKT;
class  CLASS_DATA;
class  ALLOWED_CLASSES;
class  CAR;
struct AREA_MAP_DATA;

typedef DWORD abilityType;
#define abilityUnknown 0xffffffff

typedef DWORD spellType;
#define spellUnknown 0xffffffff

typedef DWORD spellgroupType;
#define spellgroupUnknown 0xffffffff

typedef DWORD traitType;
#define traitUnknown 0xffffffff

typedef DWORD baseclassType;
#define baseclassUnknown 0xffffffff


enum SYS_SKILLS // Skills referenced by the runtime engine
{
  skill_THAC0 = 1,
  skill_END  // User-defined skills can start here
};

enum DBTYPE {
  UNKNOWN_DB = 1,
  LEVEL_DB, // Not a database really.  The character's level.
  SPELL_DB,
  ITEM_DB,
  RACE_DB,
  BASECLASS_DB,
  CLASS_DB,
  ABILITY_DB,
  TRAIT_DB,
  SPELLGROUP_DB,
  LITERAL_DB, // Also not a database. refKey is literal value
  DICE_DB, // Not a db, indicates dice roll info for compiled RDR data
  GENDER_DB // Not a db
};

// order of precedence for ajo operators (highest to lowest)
//
//  *,/
//  +, -
//  >, >=, <, <=
//  ==
//  &
//  |
//  &&
//  ||

enum ADJOP 
{
  AJO_none=0,
  AJO_gear=1,
  AJO_plus,
  AJO_equality,
  AJO_greater,
  AJO_greaterEqual,
  AJO_andBit,
  AJO_slash,
  AJO_andLogical,
  AJO_continue,
  AJO_minus,
  AJO_less,
  AJO_lessEqual,
  AJO_orBit,
  AJO_orLogical
};

class GENERIC_REFERENCE
{
public:
  char    m_refType;  // Which database?
  CString m_refName;  // For editor..Which item in databse?
  DWORD   m_refKey;      // For runtime..Which item in database?
public:
  GENERIC_REFERENCE(void);
  ~GENERIC_REFERENCE(void);
  GENERIC_REFERENCE& operator =(const GENERIC_REFERENCE& src);
  GENERIC_REFERENCE& operator =(const CString& name);
  BOOL operator ==(const GENERIC_REFERENCE& src) const;
  void Clear(void);
  int GetDataSize(void);
  void PreSerialize(BOOL IsStoring);
  int Serialize(CAR& car);
  int Serialize(CArchive& car);
  void PostSerialize(BOOL IsStoring);
  BOOL LookupRefKey(void);
  BOOL LookupReferenceData(double &result);
  BOOL Evaluate(double &result);
};

class ADJUSTMENT
{
  // Adjustments take the form:
  //
  //   [[[[ reference [op1 par1]] op2 par2] op3 [par3]]
  // opn between -32768 and 32767 inclusive.  For example.
  //
  //     strength > -3  * 4 / 2
  //
  // I don't know if that makes much sense but it means that if
  // strength is greater than -3 then multiply by 4 and divide by 2
  // Else zero.
  //
  // All the following are legal and equivalent
  //   Height Adjust  =  Level - 0 * +1 / 1  ; Means (+1*(level-0))/1
  //   Height Adjust  =  Level - 0 * 1 / 1
  //   Height Adjust  =  level * 1 / 1
  //   Height Adjust  =  level / 1
  //   Height Adjust  =  level * 1
  //   Height Adjust  =  level
  // 
public:
  enum { MAX_ADJ=3 };

  short   m_parameter[MAX_ADJ]; // Three parameters A, B, and C
  char    m_operator[MAX_ADJ];
  GENERIC_REFERENCE m_reference;
public:
  ADJUSTMENT (void);
  ADJUSTMENT (const ADJUSTMENT& src);
  ADJUSTMENT(char value, CString name);
  ~ADJUSTMENT(void);
  void Clear(void);
  ADJUSTMENT& operator = (const ADJUSTMENT& src);
  BOOL operator ==(const ADJUSTMENT& src) const;
  void PreSerialize(BOOL IsStoring);
  int Serialize(CAR& car);
  int Serialize(CArchive& car);
  void PostSerialize(BOOL IsStoring);
  int GetDataSize(void);
  double ApplyOperatorsToInput(double &input);
  BOOL Evaluate(double &result);
  BOOL EvaluateBinary(double &result);
  CString EncodeOperator(int op);
  CString EncodeText();
  BOOL Add(int param, char oper);
  BOOL HasTwoOperands();
};

typedef CArray<ADJUSTMENT, ADJUSTMENT&> DICEPLUSADJLIST;

class DICEPLUS
{ // To record things like:
  //     2d9 + 12 + 3*level
public:
  /*
  char              m_numDice;  // Yes.  It can be negative.
  BYTE              m_numSides; // Must be positive
  char              m_bonus;    // Yes...It can be negative
  int               m_min;      // 
  int               m_max;      // 
  char              m_sign;     // +/-, +1/-1
  CArray<ADJUSTMENT, ADJUSTMENT&>  m_adjust;
  */
  CString m_Text;
  CString m_Bin;

public:
  DICEPLUS(void);
  ~DICEPLUS(void);
  DICEPLUS &operator =(const DICEPLUS& src);
  bool operator!=(const DICEPLUS &rhs) const { return (!(*this == rhs)); }
  bool operator==(const DICEPLUS &rhs) const
  {
    if (m_Text.CompareNoCase(rhs.m_Text)!=0) return false;
    /*
    if (m_numDice != rhs.m_numDice) return false;
    if (m_numSides != rhs.m_numSides) return false;
    if (m_bonus != rhs.m_bonus) return false;
    if (m_min != rhs.m_min) return false;
    if (m_max != rhs.m_max) return false;
    if (m_adjust.GetSize() != rhs.m_adjust.GetSize()) return false;
    if (m_sign != rhs.m_sign) return false;
    for (int i=0;i<m_adjust.GetSize();i++)
      if (!(m_adjust[i] == rhs.m_adjust[i])) return false;
    */
    return true;
  }
  void Clear(void);
  void PreSerialize(BOOL IsStoring);
  int Serialize(CAR& car);
  int Serialize(CArchive& car);
  //void AddAdjust(int value, const CString& name);
  //void AddAdjust(ADJUSTMENT& adjust);
  void AddAdjust(CString &name);
  int GetDataSize(void);
  CString EncodeText();
  BOOL DecodeText(CString data, CString& err);
  BOOL DecodeText(CString data);
  BOOL Roll(double &result);
  BOOL MaxRoll(double &result); // return max possible result

  BOOL Compile();
  //int compileRDR(CString& name, int *array);
  //int interpretRDR(int *array);
};


extern "C" int compileDicePlusRDR(CString& name, int *array);
extern "C" int interpretDicePlusRDR(int *array);

class BASECLASS_REFERENCE
{
public:
  baseclassType m_baseclassKey;
  CString       m_baseclassName;
public:
  BASECLASS_REFERENCE(void);
  ~BASECLASS_REFERENCE(void);
  BASECLASS_REFERENCE& operator =(const BASECLASS_REFERENCE& src);
  void Clear(void);
  int GetDataSize(void);
  void PreSerialize(BOOL IsStoring);
  int Serialize(CAR& car);
  void PostSerialize(BOOL IsStoring);
};

// default number of ability references (str,int,wis,dex,con,cha)
#define ABREFKEY_COUNT 6

class ABILITY_REFERENCE 
{
public:
  abilityType m_abilityKey; // one of the ABREFKEY_* values, for str/int/wis, etc
  CString     m_abilityName;
public:
  ABILITY_REFERENCE(void);
  ~ABILITY_REFERENCE(void);
  ABILITY_REFERENCE& operator =(const ABILITY_REFERENCE& src);
  void Clear(void);
  int GetDataSize(void);
  void PreSerialize(BOOL IsStoring);
  int Serialize(CAR& car);
  void PostSerialize(BOOL IsStoring);
};

class SPELL_REFERENCE
{
public:
  spellType m_spellKey;
  CString   m_spellName;
public:
  SPELL_REFERENCE(void);
  ~SPELL_REFERENCE(void);
  SPELL_REFERENCE& operator =(const SPELL_REFERENCE& src);
  void Clear(void);
  int GetDataSize(void);
  void PreSerialize(BOOL IsStoring);
  int Serialize(CAR& car);
  void PostSerialize(BOOL IsStoring);
};

class SPELLGROUP_REFERENCE
{
public:
  spellgroupType m_spellgroupKey;
  CString        m_spellgroupName;
public:
  SPELLGROUP_REFERENCE(void);
  ~SPELLGROUP_REFERENCE(void);
  SPELLGROUP_REFERENCE& operator =(const SPELLGROUP_REFERENCE& src);
  void Clear(void);
  int GetDataSize(void);
  void PreSerialize(BOOL IsStoring);
  void Serialize(CAR& car);
  void PostSerialize(BOOL IsStoring);
};



class RACE_REFERENCE
{
public:
  raceType  m_raceKey;
  CString   m_raceName;
public:
  RACE_REFERENCE(void);
  ~RACE_REFERENCE(void);
  RACE_REFERENCE& operator =(const RACE_REFERENCE& src);
  void Clear(void);
  int GetDataSize();
  void PreSerialize(BOOL IsStoring);
  int Serialize(CAR& car);
  void PostSerialize(BOOL IsStoring);
};


// used if no entry for an ability in the config file
#define DEFAULT_MIN_ABILITY_LIMIT 3
#define DEFAULT_MAX_ABILITY_LIMIT 18

class ABILITY_LIMITS
{
public:
  ABILITY_REFERENCE m_ability;
  short   m_min; // Positive
  short   m_minMod;
  short   m_max; // Positive
  short   m_maxMod;
public:
  ABILITY_LIMITS(void);
  ABILITY_LIMITS(int min, int minmod, int max, int maxmod);
  ~ABILITY_LIMITS(void);
  ABILITY_LIMITS& operator =(const ABILITY_LIMITS& src);
  void Clear(void);
  void PreSerialize(BOOL IsStoring);
  int Serialize(CAR& car);

  abilityType GetKey(void) const { return m_ability.m_abilityKey; }
  CString Name(void) const { return m_ability.m_abilityName; }
  short GetMin(void) const { return m_min; }
  short GetMinMod(void) const { return m_minMod; }
  short GetMax(void) const { return m_max; }
  short GetMaxMod(void) const { return m_maxMod; }

  int GetDataSize(void);
};

/*
  ** SAMPLE RACE **

\(BEGIN) Human
name = Human
Height        =  2d10  +  59 + 1*Male
Weight        =  6d10  + 100 + 40*Male
Age           = 1d4   +  15
MaxAge        = 2d20  +  90
BaseMovement  = 12
AbilityRequirement = Strength       3/18
AbilityRequirement = Dexterity      3/18
AbilityRequirement = Constitution   3/18
AbilityRequirement = Intelligence   3/18
AbilityRequirement = Wisdom         3/18
AbilityRequirement = Charisma       3/18
\(END)
*/
class RACE_DATA
{
private:

  friend class RACE_DATA_TYPE;
  friend static void raceObject(CONFIG_PKT *pkt);

  mutable     DWORD m_raceKey;
	ASL         m_race_asl;
	ASL         m_temp_asl;

public:
  CString     m_name;
  DICEPLUS    m_weight;
  DICEPLUS    m_height;
  DICEPLUS    m_age;
  DICEPLUS    m_maxAge;
  DICEPLUS    m_baseMovement; // Positive
  CArray<ABILITY_LIMITS, ABILITY_LIMITS&> m_abilityRequired;

public:
  RACE_DATA();
  RACE_DATA(const RACE_DATA &src);
  ~RACE_DATA();
  RACE_DATA *operator=(const RACE_DATA &src);
  int GetDataSize(void);

  void Clear(void);
  void LogUndefinedMultiClassReferences(void);
  int Serialize(CAR &car);
  void PreSerialize(BOOL IsStoring);
  void PostSerialize(BOOL IsStoring);

  void AddAbilityRequirement(ABILITY_LIMITS& abReq);
  void AddAllowedClass(const CString& className);

  CString Name(void) const { return m_name; }
  DWORD GetWeight(void);
  DWORD GetHeight(void);
  DWORD GetStartAge(void);
  DWORD GetMaxAge(void);
  DWORD GetBaseMovement(void);
  short GetMinAbility(abilityType type);
  short GetMaxAbility(abilityType type);

  //Check to see if a particular class is allowed.
  bool IsAllowedClass(const CString& className) const;
};

class RACE_DATA_TYPE
{
private:
  //POSITION         m_pos;
  CCriticalSection m_CS;
  OrderedQueue<RACE_DATA, MAX_RACES> m_RaceData;
  // No locking... for internal use
  // Return key associated with the named object, or zero
  raceType m_FindName (const CString& name);
public:
  RACE_DATA_TYPE();
  ~RACE_DATA_TYPE();
  void Clear() { CSingleLock sLock(&m_CS, TRUE); m_RaceData.RemoveAll(); }
  void LoadUADefaults(void);
  int Serialize(CAR &car);
  void PreSerialize(BOOL IsStoring);
  void PostSerialize(BOOL IsStoring);

  void Save(CArchive& ar);
  void Restore(CArchive& ar); // Restore to temporary location
  void CommitRestore(void);

  unsigned int GetCount() { CSingleLock sLock(&m_CS, TRUE); return m_RaceData.GetCount(); }

  POSITION GetHeadPosition() { return m_RaceData.GetHeadPosition(); }
  RACE_DATA& GetAt(POSITION pos) { return m_RaceData.GetAt(pos); }
  RACE_DATA& GetNext(POSITION &pos) { return m_RaceData.GetNext(pos); }
  int GetKeyAt(POSITION pos) { return m_RaceData.GetKeyAt(pos); }

  raceType GetNextRace();

  // With locking...for public use
  // Return key associated with the named object, or zero
  raceType FindName(const CString& name);
  CString GetRaceName(raceType race);
  BOOL HaveSpell(DWORD index);
  int GetDataSize(void);

  // Called when loading from scripts, or
  // when adding from editor GUI
  raceType AddRace(const RACE_DATA &data);

  // Called when loading binary race data that
  // was saved with key info
  raceType AddRaceWithCurrKey(const RACE_DATA &data);

  BOOL SetRace(DWORD index, const RACE_DATA &race);
  BOOL RemoveRace(raceType race);

  DWORD GetWeight(raceType race);
  DWORD GetHeight(raceType race);
  DWORD GetStartAge(raceType race);
  DWORD GetMaxAge(raceType race);
  DWORD GetBaseMovement(raceType race);
  short GetMinAbility(raceType race,abilityType type);
  short GetMaxAbility(raceType race,abilityType type);

  BOOL GetRaceData(raceType race, RACE_DATA *raceData);
  bool IsAllowedClass(raceType race, const CString& className);

#ifdef UAFEDITOR
  void ExpandDefaultFilenames();
  void CollapseDefaultFilenames();
#endif
};



/*
  ** SAMPLE ABILITY **

\(BEGIN)
name = Strength
abbreviation = STR
roll = 3d6 min 3 max 18 
modify = 18
adjust = -1 * Halfling
\(END)
*/
class ABILITY_DATA
{
  friend class ABILITY_DATA_TYPE;
public:
  mutable DWORD m_abilityKey;
  CString  m_name;
  CString  m_abbreviation;
  DICEPLUS m_roll;
  BYTE     m_modify;  // Ability score to modify.  Like Strength = 18(34)
public:
  ABILITY_DATA(void);
  ABILITY_DATA(ABILITY_DATA& src);
  ~ABILITY_DATA(void);
  void Clear(void);
  int GetDataSize(void);
  void PreSerialize(BOOL IsStoring);
  int Serialize(CAR& car);
  void PostSerialize(BOOL IsStoring);
  //void AddAdjust(ADJUSTMENT& adjust);
  void AddAdjust(CString &name);
  BOOL RollAbility(double &result);
};

class ABILITY_DATA_TYPE
{
private:
  // No locking... for internal use
  // Return key associated with the named object, or zero
  abilityType m_FindName (const CString& name);

public:
  //POSITION         m_pos;
  OrderedQueue<ABILITY_DATA, MAX_ABILITIES> m_AbilityData;
  CCriticalSection m_CS;

public:
  ABILITY_DATA_TYPE(void);
  ~ABILITY_DATA_TYPE(void);
  void Clear(void);
  void LoadUADefaults(void);
  void PreSerialize (BOOL IsStoring);
  int Serialize (CAR& car);
  void PostSerialize (BOOL IsStoring);
  unsigned int GetCount() { CSingleLock sLock(&m_CS, TRUE); return m_AbilityData.GetCount(); }
  int GetDataSize(void);  

  abilityType GetKeyByIndex(int index);
  CString GetName(abilityType type);
  CString GetShortName(abilityType type);

  abilityType GetNextAbility();

  POSITION GetHeadPosition() { return m_AbilityData.GetHeadPosition(); }
  ABILITY_DATA& GetAt(POSITION pos) { return m_AbilityData.GetAt(pos); }
  ABILITY_DATA& GetNext(POSITION &pos) { return m_AbilityData.GetNext(pos); }
  abilityType GetKeyAt(POSITION pos) { return m_AbilityData.GetKeyAt(pos); }

  abilityType FindName(const CString& name);
  // Called when loading from scripts, or
  // when adding from editor GUI
  abilityType AddAbility(const ABILITY_DATA &data);

  // Called when loading binary ability data that
  // was saved with key info
  abilityType AddAbilityWithCurrKey(const ABILITY_DATA &data);

  BOOL RollAbility(abilityType ability, double &result);
};

/*
    ** SAMPLE CLASS **

\(BEGIN)
name = Cleric/Fighter/Magic User
baseclass = cleric
baseclass = fighter
baseclass = magicUser
\(END)
*/
class CLASS_DATA
{
  friend class CLASS_DATA_TYPE;
private:
  CArray<BASECLASS_REFERENCE, BASECLASS_REFERENCE> m_baseclasses;
// See BASE_CLASS_DATA  CArray<ABILITY_LIMITS, ABILITY_LIMITS&> m_abilityLimits;
public:
  mutable DWORD m_classKey;
  CString m_name;
public:
  CLASS_DATA(void);
  ~CLASS_DATA(void);
  CLASS_DATA& operator =(const CLASS_DATA& src);
  void Clear(void);
  //The following should someday become unnecessary.  When
  //classType is defined as a DWORD like spellType, abilityType, etc.
  //For now, classType is an enum.
  classType WhatIsYourClassType(void) const {return (classType)m_classKey;};
  int GetDataSize(void);
  void AddBaseclass(baseclassType bclass);
       // If not already present.
  void AddBaseclass(BASECLASS_REFERENCE& basclass);
  void AddAbilityLimit(abilityType ab, 
                       int min, int minmod, 
                       int max, int maxmod);
       // Or replace the current one.
  int Serialize(CAR &car);
  void PreSerialize(BOOL IsStoring);
  void PostSerialize(BOOL IsStoring);
  ABILITYLIMITS GetAbilityLimits(abilityType abtype); // Min/Max allowed for this class
  WORD GetAllowedAlignments(void);
  bool IsRaceAllowed(raceType race) const;
};

/*
   ** SAMPLE BASECLASS **

\(BEGIN)
name = cleric
Prime Requisite = Wisdom
AbilityRequirement = Wisdom 9/99
Allowed Alignment = any
Allowed race = Human
Allowed race = Halfling
Allowed race = Half-Elf
Allowed race = Gnome
Allowed race = Elf
Allowed race = Dwarf
ExpLevel = 0
ExpLevel = 1500
ExpLevel = 3000
ExpLevel = 6000
ExpLevel = 13000
ExpLevel = 27500
ExpLevel = 55000
ExpLevel = 110000
ExpLevel = 225000
ExpLevel = 450000
\(END)
*/
class BASE_CLASS_DATA
{
  friend class BASE_CLASS_DATA_TYPE;
public:
  mutable DWORD m_baseclassKey;
  CString m_name;
  CArray<ABILITY_REFERENCE, ABILITY_REFERENCE&> m_primeReq;
  CArray<ABILITY_LIMITS, ABILITY_LIMITS&> m_abilityReq;
  CArray<RACE_REFERENCE, RACE_REFERENCE&> m_allowedRaces;
  CArray<WORD, WORD> m_expLevels;
  WORD m_allowedAlignments; // Bitmap of 1 << alignmentType
public:
  BASE_CLASS_DATA(void);
  ~BASE_CLASS_DATA(void);
  BASE_CLASS_DATA& operator =(const BASE_CLASS_DATA& src);
  void Clear(void);
  int GetDataSize(void);
  void PreSerialize (BOOL IsStoring);
  int Serialize (CAR& car);
  void PostSerialize (BOOL IsStoring);
  void AddPrimeReq(const CString& reqName);
  void AddAbilityRequirement(ABILITY_LIMITS& abReq);
  void AddAllowedRace(RACE_REFERENCE& race);
  void AddExpLevel(WORD exp);
  void SetUAAbilityLimits(void); 
  void SetUAAllowedAlignments(void);
  ABILITYLIMITS GetAbilityLimits(abilityType abType);
  bool IsAllowedRace(raceType race);
  WORD GetMaxExpForLevel(int level);
  WORD GetMinExpForLevel(int level);
};

class BASE_CLASS_DATA_TYPE
{
private:
  //POSITION         m_pos;
  CCriticalSection m_CS;
  OrderedQueue<BASE_CLASS_DATA, MAX_BASE_CLASSES> m_BaseclassData;
  // No locking... for internal use
  // Return key associated with the named object, or zero
  baseclassType m_FindName (const CString& name);
public:
  BASE_CLASS_DATA_TYPE(void);
  ~BASE_CLASS_DATA_TYPE(void);
  CLASS_DATA& operator [](classType type);
  void Clear(void);
  void LoadUADefaults(void);
  int GetDataSize(void);
  void PreSerialize (BOOL IsStoring);
  int Serialize (CAR& car);
  void PostSerialize (BOOL IsStoring);
  unsigned int GetCount() { CSingleLock sLock(&m_CS, TRUE); return m_BaseclassData.GetCount(); }

  POSITION GetHeadPosition() { return m_BaseclassData.GetHeadPosition(); }
  BASE_CLASS_DATA& GetAt(POSITION pos) { return m_BaseclassData.GetAt(pos); }
  BASE_CLASS_DATA& GetNext(POSITION &pos) { return m_BaseclassData.GetNext(pos); }
  baseclassType GetKeyAt(POSITION pos) { return m_BaseclassData.GetKeyAt(pos); }

  baseclassType GetNextBaseclass();
  
  baseclassType FindName(const CString& name);
  // Called when loading from scripts, or
  // when adding from editor GUI
  baseclassType AddBaseclass(const BASE_CLASS_DATA &data);

  // Called when loading binary base class data that
  // was saved with key info
  baseclassType AddBaseclassWithCurrKey(const BASE_CLASS_DATA &data);
  ABILITYLIMITS GetAbilityLimits(baseclassType bType, abilityType abType);
  WORD GetAllowedAlignments(baseclassType bType);
  bool IsAllowedRace(baseclassType baseclass, raceType race);
  WORD GetMaxExpForLevel(baseclassType bType,int level);
  WORD GetMinExpForLevel(baseclassType bType,int level);
};


class CLASS_DATA_TYPE
{
private:
  //POSITION         m_pos;
  CCriticalSection m_CS;
  OrderedQueue<CLASS_DATA, MAX_CLASSES> m_ClassData;
  // No locking... for internal use
  // Return key associated with the named object, or zero
  classType m_FindName (const CString& name);
public:
  CLASS_DATA_TYPE(void);
  ~CLASS_DATA_TYPE(void);
  CLASS_DATA& operator [](classType Class);
  void Clear(void);
  void LoadUADefaults(void);
  int GetDataSize(void);
  void PreSerialize (BOOL IsStoring);
  int Serialize (CAR& car);
  void PostSerialize (BOOL IsStoring);
  unsigned int GetCount() { CSingleLock sLock(&m_CS, TRUE); return m_ClassData.GetCount(); }

//  CString Name(classType Class); // can't say "class"
//  int StatMax(classType Class, int statistic);
//   int StatMin(classType Class, int statistic);


  POSITION GetHeadPosition() { return m_ClassData.GetHeadPosition(); }
  CLASS_DATA& GetAt(POSITION pos) { return m_ClassData.GetAt(pos); }
  CLASS_DATA& GetNext(POSITION &pos) { return m_ClassData.GetNext(pos); }
  classType GetKeyAt(POSITION pos) { return (classType)m_ClassData.GetKeyAt(pos); }

  classType GetNextClass();
  
  classType FindName(const CString& className);
  // Called when loading from scripts, or
  // when adding from editor GUI
  classType AddClass(const CLASS_DATA &data);

  // Called when loading binary class data that
  // was saved with key info
  classType AddClassWithCurrKey(const CLASS_DATA &data);
  ABILITYLIMITS GetAbilityLimits(classType ctype, abilityType abtype); 
  WORD GetAllowedAlignments(classType Class);
  bool IsAllowedRace(classType Class, raceType race);
};


//*********************************************************
// Right now (Feb 2002) a character is allowed a class if
// a) The Class has a single Base Class and that Base Class
//    names the Race of the character.
//            --or--
// b) The character's Race names the Class.
//
class ALLOWED_CLASSES
{
private:
  classType   *m_allowedClasses;
  int          m_numAllowedClasses;
public:
  ALLOWED_CLASSES(void);
  ~ALLOWED_CLASSES(void);
  void Initialize(void); //Initialize to ALL classes!
  int NumAllowedClasses(void) const;
  classType GetAt(int i) const;
  void RemoveAt(int i);
  void RestrictByRace(raceType race);
};

//*********************************************************

/*
  ** SAMPLE TRAIT **

\(BEGIN)
name = Pick Pockets
adjust = "thief"         *  15
adjust = Dexterity == 9  * -15
adjust = Dexterity == 10 * -10
adjust = Dexterity == 11 *  -5
adjust = Dexterity == 17 *  +5
adjust = Dexterity == 18 * +10
adjust = Dexterity == 19 * +10
adjust = "Dwarf"         *  +5
adjust = "Half-Elf"      * +10
adjust = "Halfling"      *  +5
\(END)
*/

class TRAIT_DATA
{
  friend class TRAIT_DATA_TYPE;
public:
  mutable DWORD m_traitKey;
  CString  m_name;
  CString  m_abbreviation;
  //CArray<ADJUSTMENT, ADJUSTMENT&> m_adjustments;
  DICEPLUS m_roll;
public:
  TRAIT_DATA(void);
  TRAIT_DATA(TRAIT_DATA& src);
  ~TRAIT_DATA(void);
  TRAIT_DATA& operator =(const TRAIT_DATA& src);
   void Clear(void);
  void PreSerialize(BOOL IsStoring);
  int Serialize(CAR& car);
  void PostSerialize(BOOL IsStoring);
  //void AddAdjust(ADJUSTMENT& adjust);
  void AddAdjust(CString &name);
  BOOL EvaluateTrait(double &result);
};

class TRAIT_DATA_TYPE
{
private:
  // No locking... for internal use
  // Return key associated with the named object, or zero
  traitType m_FindName (const CString& name);
public:
  //POSITION         m_pos;
  OrderedQueue<TRAIT_DATA, MAX_TRAITS> m_TraitData;
  CCriticalSection m_CS;
  int m_numAbilities;
public:
  TRAIT_DATA_TYPE(void);
  ~TRAIT_DATA_TYPE(void);
  void Clear(void);
  void LoadUADefaults(void);
  void PreSerialize (BOOL IsStoring);
  int Serialize (CAR& car);
  void PostSerialize (BOOL IsStoring);
  unsigned int GetCount() { CSingleLock sLock(&m_CS, TRUE); return m_TraitData.GetCount(); }

  traitType GetNextTrait();

  POSITION GetHeadPosition() { return m_TraitData.GetHeadPosition(); }
  TRAIT_DATA& GetAt(POSITION pos) { return m_TraitData.GetAt(pos); }
  TRAIT_DATA& GetNext(POSITION &pos) { return m_TraitData.GetNext(pos); }
  traitType GetKeyAt(POSITION pos) { return m_TraitData.GetKeyAt(pos); }

  traitType FindName(const CString& name);
  // Called when loading from scripts, or
  // when adding from editor GUI
  traitType AddTrait(const TRAIT_DATA &data);

  // Called when loading binary trait data that
  // was saved with key info
  traitType AddTraitWithCurrKey(const TRAIT_DATA &data);

  BOOL EvaluteTrait(traitType trait, double &result);
};

/*
Attribute modified
Set or Delta +/-,  points or %,  cumulative yes/no
affects target/targeter
duration permanent/temporal (rds/turns + x rds/turns per lvl)
*/

struct SPELL_EFFECTS_DATA
{
  SPELL_EFFECTS_DATA();
  SPELL_EFFECTS_DATA(const SPELL_EFFECTS_DATA &src);
  virtual ~SPELL_EFFECTS_DATA();
  void Serialize(CAR &ar, double version);
  void Serialize(CArchive &ar, double version);
  void PreSerialize(BOOL IsStoring);
  bool operator==(const SPELL_EFFECTS_DATA &rhs) const;
  void Clear();

/*
  Effects:
  32-bits, default is all zeros

    bit     meaning
     2      0=literal delta,  1=percent
     3      0=not target      1=affects target
     4      0=not targeter    1=affects targeter
     5      0=Not cumulative, 1=is cumulative
     6      0=literal delta,  1=change current to amount
     7      1=item special ability source
     8      1=spell special ability source
     9      1=char/monster special ability source
     10     1=spell source
     11-32

  Target is equivalent to Self
*/
  enum 
  { 
    EFFECT_PCNT       =0x00000002, // set to percentage of original value
    EFFECT_TARGET     =0x00000004,
    EFFECT_TARGETER   =0x00000008,
    EFFECT_CUMULATIVE =0x00000010,
    EFFECT_ABS        =0x00000020, // set to amount. If not 2 or 32, amount is delta to original
    EFFECT_ITEMSPECAB =0x00000040, // source of effect is an item special ability
    EFFECT_SPELLSPECAB=0x00000080, // source of effect is a spell special ability
    EFFECT_CHARSPECAB =0x00000100, // source of effect is a char/monster special ability
    EFFECT_SPELL      =0x00000200, // source of effect is a spell
    EFFECT_REMOVEALL  =0x00000400, // remove all effects of this type from character
                                   // with the exception of CHARSPECAB effects (inherent ability)

    ALLSOURCE_MASK = EFFECT_ITEMSPECAB|EFFECT_SPELLSPECAB|EFFECT_CHARSPECAB|EFFECT_SPELL,
    SPECAB_MASK = EFFECT_ITEMSPECAB|EFFECT_SPELLSPECAB|EFFECT_CHARSPECAB,
    ALL_TARG_TYPES = EFFECT_TARGET|EFFECT_TARGETER
  };

  double ApplyChange(double src);
  double GetChange(void);
  void ClearChangeResult(void);
  bool ExecuteActivationScript(void) const;
  double ExecuteModificationScript(void) const;
  double ExecuteSavingThrowScript(void) const;
  void ExecuteSavingThrowFailedScript(void) const;
  void ExecuteSavingThrowSucceededScript(void) const;
  bool CompileScripts(const CString &basename);
  CString GetEffectActivationFuncName(void);
  CString GetEffectModificationFuncName(void);
  CString GetEffectSavingThrowFuncName(void);    
  CString GetEffectSavingThrowFailedFuncName(void);    
  CString GetEffectSavingThrowSucceededFuncName(void);    
  BOOL IsReadyToExpire();    
  DWORD GetData() const;
  void SetData(DWORD val);
    
  DICEPLUS changeData;
  double changeResult;
  CString affectedAttr;
  CString activationScript;
  CString activationBinary; // compiled GPDL script
  CString modificationScript;
  CString modificationBinary; // compiled GPDL script  
  CString savingThrowScript;
  CString savingThrowBinary; // compiled GPDL script    
  CString savingThrowFailedScript;
  CString savingThrowFailedBinary; // compiled GPDL script    
  CString savingThrowSucceededScript;
  CString savingThrowSucceededBinary; // compiled GPDL script    
  DWORD flags;
  DWORD key;    // POSITION in editor, spell db key in engine
  DWORD parent; // key from global list of active spells
  DWORD StopTime; // stop time (elapsed game minutes) if not same
                  // as parent spell (Stinking Cloud).
  DWORD data;  // extra data available to an effect if needed to track
               // something (MirrorImage)
};

class SPELLGROUP_DATA
{
  friend class SPELLGROUP_DATA_TYPE;
public:
  mutable DWORD m_spellgroupKey;
  CString  m_name;
  CArray <SPELL_REFERENCE, SPELL_REFERENCE&> m_spells;
  CArray <SPELLGROUP_REFERENCE, SPELLGROUP_REFERENCE> m_spellgroups;
public:
  SPELLGROUP_DATA(void);
  SPELLGROUP_DATA(SPELLGROUP_DATA& src);
  SPELLGROUP_DATA& SPELLGROUP_DATA::operator =(const SPELLGROUP_DATA& src);
  ~SPELLGROUP_DATA(void);
  void Clear(void);
  int GetDataSize(void);
  void PreSerialize(BOOL IsStoring);
  int Serialize(CAR& car);
  void PostSerialize(BOOL IsStoring);
  void AddSpell(const CString& name);
  void AddSpellgroup(const CString& name);
};

class SPELLGROUP_DATA_TYPE
{
private:
  // No locking... for internal use
  // Return key associated with the named object, or zero
  spellgroupType m_FindName (const CString& name);
public:
  //POSITION         m_pos;
  OrderedQueue<SPELLGROUP_DATA, MAX_SPELLGROUPS> m_SpellgroupData;
  CCriticalSection m_CS;
public:
  SPELLGROUP_DATA_TYPE(void);
  ~SPELLGROUP_DATA_TYPE(void);
  void Clear(void);
  void LoadUADefaults(void);
  int GetDataSize(void);
  void PreSerialize (BOOL IsStoring);
  int Serialize (CAR& car);
  void PostSerialize (BOOL IsStoring);
  unsigned int GetCount() { CSingleLock sLock(&m_CS, TRUE); return m_SpellgroupData.GetCount(); }

  spellgroupType GetNextSpellgroup();

  POSITION GetHeadPosition() { return m_SpellgroupData.GetHeadPosition(); }
  SPELLGROUP_DATA& GetAt(POSITION pos) { return m_SpellgroupData.GetAt(pos); }
  SPELLGROUP_DATA& GetNext(POSITION &pos) { return m_SpellgroupData.GetNext(pos); }
  spellgroupType GetKeyAt(POSITION pos) { return m_SpellgroupData.GetKeyAt(pos); }

  spellgroupType FindName(const CString& name);
  // Called when loading from scripts, or
  // when adding from editor GUI
  spellgroupType AddSpellgroup(const SPELLGROUP_DATA &data);

  // Called when loading binary spellgroup data that
  // was saved with key info
  spellgroupType AddSpellgroupWithCurrKey(const SPELLGROUP_DATA &data);
};
/*
class STRINGINDEX 
{
  // Provides 16-bit replacements for CStrings in archives
private:
  CString *m_strings;
  int      m_used;
  int      m_allocated;
  bool     m_operational;
  int m_errorcount;
public:
  STRINGINDEX(void);
  ~STRINGINDEX(void);
  void Clear(bool operational); // To turn it off or on.
  void AddString(const CString& str);
//  CString& GetString(WORD n);
//  WORD GetIndex(const CString& str);
  void Serialize(CArchive& ar, CString& string);

};
*/
class CAR : public CArchive
{
  class CODES
  {
  private:
    unsigned __int16 m_hashTable[9973];
    unsigned int m_codes[8192];
    unsigned __int16 m_nextCode;
    int m_numCodes;
    int m_numMisses;
  public:
    CODES(void);
    ~CODES(void);
    void Clear(void);
    unsigned __int16 AddCode(unsigned int code);
    void PrintStats(void);
    void HashDump(FILE *f);
    bool Full(void) { return m_nextCode == 8190;};
    unsigned __int16 FindCode(unsigned int code); // zero if no find
  };
  class DDATA
  {
    friend class CAR;
    char m_stack[1000];
    unsigned __int16 m_prefix[8192];
    unsigned char   m_postfix[8192];
  };
  class CDATA
  {
    friend class CAR;
  };
private:
  char m_compressType;
  CMap <CString, LPCSTR, unsigned int, unsigned int> stringIndex;
  CArray <CString, CString&> m_stringArray;
  unsigned int m_nextIndex;
  DDATA *m_dData; // Allocated at run time on heap.  We like
  CDATA *m_cData; // to keep these large items off the stack.
  CODES *m_codes;
  void compress (const char *chars, int num);
  void decompress (char *chars, int num);
  unsigned __int16 m_w;
  unsigned int m_bufferIndex;
  unsigned int m_buffer[13]; // 52 bytes
  unsigned int m_stkLen;
  unsigned __int16 m_OC;
  unsigned int m_numCode;
  unsigned char m_C;
public:
  CAR(CFile* pFile, UINT nMode);
  ~CAR(void);
  void Close(void);
  void Flush(void);
  void Compress(bool compress);
  char GetCompression() const { return m_compressType; }
  void WriteCount(unsigned int n);
  void WriteCount(DWORD n);
  void WriteCount(int n);
  DWORD ReadCount(void);

  CAR& operator << (int Int);
  CAR& operator >> (int& Int);

  CAR& operator << (unsigned int UInt);
  CAR& operator >> (unsigned int& UInt);

  CAR& operator << (unsigned long ULong);
  CAR& operator >> (unsigned long& ULong);

  CAR& operator << (long ULong);
  CAR& operator >> (long& ULong);

  CAR& operator << (BYTE byte);
  CAR& operator >> (BYTE& byte);

  CAR& operator << (char Char);
  CAR& operator >> (char& Char);

  CAR& operator << (short Short);
  CAR& operator >> (short& Short);

  CAR& operator << (double Double);
  CAR& operator >> (double& Double);

  CAR& operator << (float Float);
  CAR& operator >> (float& Float);

  CAR& operator << (unsigned short UShort);
  CAR& operator >> (unsigned short& UShort);

  CAR& operator << (CString& str);
  CAR& operator >> (CString& str);

  CAR& operator << (const char *);
  CAR& operator >> (const char *);

  CAR& operator << (ASL& attr);
  CAR& operator >> (ASL& attr);

  //CAR& operator << (AREA_MAP_DATA& attr);
  //CAR& operator >> (AREA_MAP_DATA& attr);

  CAR& operator << ( CArray<WORD, WORD> &warray);
  CAR& operator >> ( CArray<WORD, WORD> &warray);
};

#endif // #ifndef __CLASS_H__