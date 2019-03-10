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

#ifndef __CHAR_H__
#define __CHAR_H__

#include "dungeoncraft/shared/class.h"
#include "dungeoncraft/shared/items.h"
#include "dungeoncraft/shared/spell.h"
#include "dungeoncraft/shared/Property.h"

const int MAX_CHARACTERS = INT_MAX; // Max pre-gen NPC's 
const int MAX_CHAR_NAME = 30; // should match MAX_MONSTER_NAME!

#define DEFAULT_SPELL_EFFECT_FLAGS (SPELL_EFFECTS_DATA::ALL_TARG_TYPES)


// Keep track of which blockages have been removed through party action.
// These flags need to be saved when the game is saved so the engine can
// remember if a particular lock has been picked, secret door found, or
// wizard lock spell removed.

union BlockageStats
{
  BlockageStats() { StatsFull = 0xFFFF; }
  WORD StatsFull; // access to all 16 flags at once
  
  struct // access to all flags for a particular facing
  {
    WORD NorthFlags : 4;
    WORD SouthFlags : 4;
    WORD EastFlags  : 4;
    WORD WestFlags  : 4;
  } directionStats;

  // each bit starts as 1, then gets set to 0 when party
  // clears the blockage. 
  struct
  {
    WORD NorthLocked  : 1;
    WORD NorthSecret  : 1;
    WORD NorthSpelled : 1;
    WORD NorthUnused  : 1;
    WORD SouthLocked  : 1;
    WORD SouthSecret  : 1;
    WORD SouthSpelled : 1;
    WORD SouthUnused  : 1;
    WORD EastLocked   : 1;
    WORD EastSecret   : 1;
    WORD EastSpelled  : 1;
    WORD EastUnused   : 1;
    WORD WestLocked   : 1;
    WORD WestSecret   : 1;
    WORD WestSpelled  : 1;
    WORD WestUnused   : 1;
  } flags;
};

// there will 1 of these saved for each blockage removed by party.
struct BlockageDataType
{
  void Serialize(CArchive &ar);
  void Serialize(CAR &ar);

  int level;
  int x;
  int y;
  BlockageStats Stats;
};

class BLOCKAGE_STATUS : public CObject
{
public:
  DECLARE_SERIAL( BLOCKAGE_STATUS )
  BLOCKAGE_STATUS() { Clear(); }
  ~BLOCKAGE_STATUS() { Clear(); }
  BLOCKAGE_STATUS& operator =(const BLOCKAGE_STATUS& src);
  bool operator==(const BLOCKAGE_STATUS& src);
  void Clear() { data.RemoveAll(); }
  void Serialize(CArchive &ar); 
  void Serialize(CAR &ar); 

  CList <BlockageDataType, BlockageDataType&> data;

  POSITION FindData(int level, int x, int y);
  BOOL IsSecret(int level, int x, int y, int facing);
  void ClearSecret(int level, int x, int y, int facing);
  BOOL IsLocked(int level, int x, int y, int facing);
  void ClearLocked(int level, int x, int y, int facing);
  BOOL IsSpelled(int level, int x, int y, int facing);
  void ClearSpelled(int level, int x, int y, int facing);

  BOOL IsLockedSecret(int level, int x, int y, int facing);
  BOOL IsSpelledSecret(int level, int x, int y, int facing);
};


/*
  **** Defined in externs.h ****

#define CHAR_TYPE    1
#define NPC_TYPE     2
#define MONSTER_TYPE 3
#define IN_PARTY_TYPE 128

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

inline BYTE IndexToFlag(BYTE index)
{
  switch (index)
  {
  case 0: return MagicUserFlag;break;
  case 1: return ClericFlag;break;
  case 2: return ThiefFlag;break;
  case 3: return FighterFlag;break;
  case 4: return PaladinFlag;break;
  case 5: return RangerFlag;break;
  case 6: return DruidFlag;break;
  case 7: return AllClasses;break;
  }
  ASSERT(FALSE);
  return 0;
}

inline BYTE FlagToIndex(BYTE flag)
{ 
  switch (flag)
  {
  case MagicUserFlag: return 0;break;
  case ClericFlag:    return 1;break;
  case ThiefFlag:     return 2;break;
  case FighterFlag:   return 3;break;
  case PaladinFlag:   return 4;break;
  case RangerFlag:    return 5;break;
  case DruidFlag:     return 6;break;
  case AllClasses:    return 7;break;
  }
  ASSERT(FALSE);
  return 0;
}

extern char *ClassFlagText[8];
const int NO_DUDE = -1;

*/

class CHARACTER : public CObject
{
public:
  DECLARE_SERIAL( CHARACTER )

  CHARACTER(void);
  CHARACTER(const CHARACTER& src);
  virtual ~CHARACTER(void);
  CHARACTER& operator=(const CHARACTER& src);
  BOOL operator==(const CHARACTER& src);
  BOOL IsSameCharacter(const CHARACTER& dude); // doesn't compare all member vars

  void Clear(BOOL IsConstructor=FALSE);
  void Serialize(CArchive &ar, double version);
  void Serialize(CAR &ar, double version);
  BOOL serializeCharacter(BOOL save, LPCSTR fname); // stores member vars and image data
  void SaveArt();
  
  void getCharWeaponText(CString &wpn, CString &dmg);
  void getCharArmorText(CString &armor);
  CString getInfo(void);
  CString getItemList(int id);
  virtual int GetUniqueId() { return marchOrder; }
  
  BOOL addCharacterItem(int item, int qty, int charges, BOOL id, int cost);
  virtual BOOL delCharacterItem(int index, int qty);
  BOOL HasItem(int item);
  void buyItem(int item, costFactorType type);
  void payForItem(int moneyCost, itemClassType moneyType, int gems=0, int jewelry=0);
  virtual BOOL toggleReadyItem(int item);
  void ReadyBestWpn(int dist, BOOL isLargeTarget);
  void ReadyBestShield();
  void ReadyBestArmor();
  void ReadyBestAmmo(BOOL isLargeTarget);
  void ReadyItemByType(itemLocationType type, int index);
  void UnreadyItemByType(itemLocationType type);
  void ReadyWeapon(int index);
  void ReadyShield(int index);
  void ReadyArmor(int index);
  void ReadyGauntlets(int index);
  void ReadyHelm(int index);
  void ReadyBelt(int index);
  void ReadyRobe(int index);
  void ReadyCloak(int index);
  void ReadyBoots(int index);
  void ReadyRing(int index);
  void ReadyAmmo(int index);

  int poolCharacterGold();
  BOOL enoughMoney(int moneyCost, int gemCost=0, int jewelryCost=0, itemClassType moneyType=PlatinumType);
  void addCharMoneyToItemList(ITEM_TEXT_LIST &list);
  void removeCharMoney(itemClassType type, int qty);  
  
  BOOL canCastSpells();
  BOOL CanCastAsRanger() const { return ((GetClass() == Ranger) && (RangerLevel >= 8)); }
  BOOL CanCastAsPaladin() const { return ((GetClass() == Paladin) && (PaladinLevel >= 9)); }
  DWORD GetBestMemorizedHealingSpell(int& index);
  int GetHealingPointsNeeded();
 
  void giveCharacterDamage(spellSaveEffectType eventSave, int attackTHACO, int dmgDice, int dmgDiceQty, int dmgBonus, spellSaveVsType spellSave, int saveBonus);
  void giveCharacterDamage(int damage); // non-combat damage
  virtual void TakeDamage(int dmg, BOOL IsNonLethal) { giveCharacterDamage(dmg); }

  void rollStats();
  void UpdateStats(BOOL IsNewChar);
  void UpdateLevelBasedStats(void);
  void UpdateSkillBasedStats(void);
  void UpdateSpellAbility(void);

  BOOL CanBeModified();
  void generateNewCharacter(DWORD StartExperience, int StartExpType);
  void getNewCharLevel();
  void checkNewCharClassScores();
  void checkNewCharRaceScores(BOOL ApplyModifiers);
  void getNewCharStartingMoney();
  void getNewCharStartingEquip();
  WORD getAllowedAlignments(void);    

  void getThiefSkillDexAdjustments();
  void getThiefSkillRaceAdjustments();
  void getThiefSkills();
  int getThiefBackstabDamageMultiplier();
  void getThiefSkillArmorAdustments();

  BYTE DetermineCharItemMask();
  BYTE GetItemMask() const { return ItemMask; }  
  
  void TrainCharacter();  
  BOOL CanChangeClassHuman();  
  void HumanChangeClass(classType newClass);
  classType GetPreviousClass();
  BYTE GetPreviousClassLevel();
  BOOL CanUsePrevClass();
  BOOL IsDualClass();
  BOOL CanDualClassCleric();
  BOOL CanDualClassFighter();
  BOOL CanDualClassMagicUser();
  BOOL CanDualClassThief();
  BOOL CanDualClassDruid();
  BOOL CanDualClassRanger();
  BOOL CanDualClassPaladin();

  BOOL IsUsingClass(WORD ClassFlag);  
  
  BOOL IsAlive()
  {
    charStatusType stype = GetAdjStatus();
    return ((stype==Okay)||(stype==Unconscious)||(stype==Running)||(stype==Dying));
  }  

  virtual ActorType GetContext(WORD ClassFlag=0);
  double EvalDiceAsClass(DICEPLUS &dice, WORD ClassFlag);

  void DisplayCurrSpellEffects(const CString &SrcFunc);
  
#ifdef UAFEngine
  void ClearAllItemCursedFlags();
  int RemoveAllSpellEffectsByLevel(int lvl); // removes all that match lvl
  int DispelSpellEffectsByLevel(int lvl); // checks dispel flag, removes all <= lvl
  virtual BOOL CastSpell(int spell);
  virtual void SpellActivate(PENDING_SPELL &data);
  virtual int InvokeSpellOnTarget(CHARACTER& targ, int activeSpellKey, int spellDbKey);

  void ApplySpellEffectAdjustments(DWORD flags, const char *key, double &val);
  BOOL AddSpellEffect(SPELL_EFFECTS_DATA &data);
  BOOL AddSpecAb(SPELL_EFFECTS_DATA &data);
  void CalcSpellEffectChangeValue(SPELL_DATA *spData, SPELL_EFFECTS_DATA &data, BOOL friendlyFire);
  BOOL RemoveSpellByName(const CString &sname);
  virtual BOOL RemoveSpellEffect(DWORD parent, SPELL_DATA *pSpell);  
  virtual void CheckForExpiredSpellEffects();
  
  void RemoveAllEffectsFor(const CString &AttrName);
  void RemoveSpecAb(SPELL_EFFECTS_DATA &data);
  DWORD CalcRestTime(void);
  void RemoveAllTargets();
  void InitTargeting(spellTargetingType type, BOOL friendfireok, BOOL partyfriend, DWORD targs, DWORD range);
  virtual BOOL AddTarget(CHARACTER &targ, int range=0);
  virtual BOOL AddTargetSelf();
  BOOL IsAffectedBySpell(const CString& SpellName);
  BOOL IsAffectedBySpellAttribute(const CString& SpellAttribName);
  void UpdateSpellForDamage(int DamageTaken);
  void UpdateSpellForAttacks(int AttacksTaken);
  SPELL_TARGETING_DATA targets;
#endif

#ifdef UAFEDITOR
  CString GetNPCEventText();
#endif
  
  // functions that access the attributes

  CString GetName() const { return name; }
  void SetName(const char *val);

  int GetPickPockets() const { return pickPockets; }
  int GetOpenLocks() const { return openLocks; }
  int GetFindTraps() const { return findRemoveTrap; }
  int GetMoveSilent() const { return moveSilent; }
  int GetHideInShadows() const { return hideInShadows; }
  int GetHearNoise() const { return hearNoise; }
  int GetClimbWalls() const { return climbWalls; }
  int GetReadLanguages() const { return readLanguages; }
  int GetAdjPickPockets(DWORD flags=DEFAULT_SPELL_EFFECT_FLAGS);
  int GetAdjOpenLocks(DWORD flags=DEFAULT_SPELL_EFFECT_FLAGS);
  int GetAdjFindTraps(DWORD flags=DEFAULT_SPELL_EFFECT_FLAGS);
  int GetAdjMoveSilent(DWORD flags=DEFAULT_SPELL_EFFECT_FLAGS);
  int GetAdjHideInShadows(DWORD flags=DEFAULT_SPELL_EFFECT_FLAGS);
  int GetAdjHearNoise(DWORD flags=DEFAULT_SPELL_EFFECT_FLAGS);
  int GetAdjClimbWalls(DWORD flags=DEFAULT_SPELL_EFFECT_FLAGS);
  int GetAdjReadLanguages(DWORD flags=DEFAULT_SPELL_EFFECT_FLAGS);
  void SetPickPockets(int val) { val=min(100,val);val=max(0,val);pickPockets=val; }
  void SetOpenLocks(int val) { val=min(100,val);val=max(0,val);openLocks=val; }
  void SetFindTraps(int val) { val=min(100,val);val=max(0,val); findRemoveTrap=val; }
  void SetMoveSilent(int val) { val=min(100,val);val=max(0,val);moveSilent=val; }
  void SetHideInShadows(int val) { val=min(100,val);val=max(0,val);hideInShadows=val; }
  void SetHearNoise(int val) { val=min(100,val);val=max(0,val);hearNoise=val; }
  void SetClimbWalls(int val) { val=min(100,val);val=max(0,val);climbWalls=val; }
  void SetReadLanguages(int val) { val=min(100,val);val=max(0,val);readLanguages=val; }

  BYTE GetOpenDoors() const { return openDoors; }
  BYTE GetOpenMagicDoors() const { return openMagicDoors; }
  BYTE GetBendBarsLiftGates() const { return BB_LG; }
  BYTE GetAdjOpenDoors(DWORD flags=DEFAULT_SPELL_EFFECT_FLAGS);
  BYTE GetAdjOpenMagicDoors(DWORD flags=DEFAULT_SPELL_EFFECT_FLAGS);
  BYTE GetAdjBendBarsLiftGates(DWORD flags=DEFAULT_SPELL_EFFECT_FLAGS);
  void SetOpenDoors(BYTE val) { val=min(100,val);openDoors=val; }
  void SetOpenMagicDoors(BYTE val) { val=min(100,val);openMagicDoors=val; }
  void SetBendBarsLiftGates(BYTE val) { val=min(100,val);BB_LG=val; }

  int DetermineMaxCureDisease() { maxCureDisease=0; return maxCureDisease;/* not used yet */ }
  int GetMaxCureDisease() const { return maxCureDisease; }
  void SetMaxCureDisease(int val) { val=max(0,val);maxCureDisease=val; }
  int GetCureDisease() const { return usedCureDisease; }
  void SetCureDisease(int val) { val=max(0,val);val=min(maxCureDisease,val);usedCureDisease=val; }
  int IncCureDisease() { usedCureDisease++;usedCureDisease=min(maxCureDisease,usedCureDisease);return usedCureDisease; }
  BOOL HaveCureDisease() const { return ((maxCureDisease>0) && (usedCureDisease<maxCureDisease)); }

  int GetMorale() const { return morale; }
  int GetAdjMorale(DWORD flags=DEFAULT_SPELL_EFFECT_FLAGS);
  void SetMorale(int val) { val=max(0,val);val=min(100,val); morale=val; }

  void DetermineUnarmedHitDice(); // for attacks without weapons
  void DetermineCharHitDice();
  int DetermineHitDiceBonus(); // not stored as member var
  double GetNbrHD() const { return nbrHitDice; }
  void SetNbrHD(double val) { val=max(0,val);nbrHitDice=val; }

  float determineNbrAttacks();
  float GetNbrAttacks() const { return NbrAttacks; }
  void SetNbrAttacks(float val) { val=max(0,val);NbrAttacks=val; }
  
  BYTE GetCurrentLevel(WORD ClassFlag=0);// 0 means return max level for all class types
  BYTE GetPrevLevel(WORD ClassFlag);
  BYTE GetPreDrainLevel(WORD ClassFlag);
  void SetCurrentLevel(WORD ClassFlag, int lvl);
  void SetPrevLevel(WORD ClassFlag, int lvl);
  void SetPreDrainLevel(WORD ClassFlag, int lvl);
  int IncCurrentLevel(WORD ClassFlag, int inc=1);
  int IncPrevLevel(WORD ClassFlag, int inc=1);
  int IncPreDrainLevel(WORD ClassFlag, int inc=1);

  int GetHitBonus() const { return hitBonus; }
  int GetAdjHitBonus(DWORD flags=DEFAULT_SPELL_EFFECT_FLAGS);
  void SetHitBonus(int val);
  int GetDmgBonus() const { return dmgBonus; }
  int GetAdjDmgBonus(DWORD flags=DEFAULT_SPELL_EFFECT_FLAGS);
  void SetDmgBonus(int val);

  void getCharSavingThrows();
  BOOL didCharacterSaveVs(spellSaveVsType type, int bonus);
  int GetMagicResistance() const { return magicResistance; }
  int GetAdjMagicResistance(DWORD flags=DEFAULT_SPELL_EFFECT_FLAGS);
  void SetMagicResistance(int val);
  int GetSaveVsPPDM() const { return saveVsParPsnDMag; }
  int GetAdjSaveVsPPDM(DWORD flags=DEFAULT_SPELL_EFFECT_FLAGS);
  void SetSaveVsPPDM(int val);
  int GetSaveVsPP() const { return saveVsPetPoly; }
  int GetAdjSaveVsPP(DWORD flags=DEFAULT_SPELL_EFFECT_FLAGS);
  void SetSaveVsPP(int val);
  int GetSaveVsRSW() const { return saveVsRdStWnd; }
  int GetAdjSaveVsRSW(DWORD flags=DEFAULT_SPELL_EFFECT_FLAGS);
  void SetSaveVsRSW(int val);
  int GetSaveVsBr() const { return saveVsBreath; }
  int GetAdjSaveVsBr(DWORD flags=DEFAULT_SPELL_EFFECT_FLAGS);
  void SetSaveVsBr(int val);
  int GetSaveVsSp() const { return saveVsSpell; }
  int GetAdjSaveVsSp(DWORD flags=DEFAULT_SPELL_EFFECT_FLAGS);
  void SetSaveVsSp(int val);

  genderType GetGender() const { return gender; }
  classType GetClass() const { return charClass; }
  alignmentType GetAlignment() const { return alignment; }
  charStatusType GetStatus() const { return status; }
  undeadType GetUndeadType() const { return undead; }
  creatureSizeType GetSize() const { return creatureSize; }
  BOOL GetAllowInCombat() const { return allowInCombat; }

  genderType GetAdjGender(DWORD flags=DEFAULT_SPELL_EFFECT_FLAGS);
  classType GetAdjClass(DWORD flags=DEFAULT_SPELL_EFFECT_FLAGS);
  alignmentType GetAdjAlignment(DWORD flags=DEFAULT_SPELL_EFFECT_FLAGS);
  charStatusType GetAdjStatus(DWORD flags=DEFAULT_SPELL_EFFECT_FLAGS);
  undeadType GetAdjUndeadType(DWORD flags=DEFAULT_SPELL_EFFECT_FLAGS);
  creatureSizeType GetAdjSize(DWORD flags=DEFAULT_SPELL_EFFECT_FLAGS);
  virtual BOOL GetIsFriendly() { return TRUE; }

  void SetGender(genderType val) { gender=val; }
  void SetClass(classType val) { charClass=val; }
  void SetAlignment(alignmentType val) { alignment=val; }
  void SetStatus(charStatusType val) { status=val; if (status==Dead) spellEffects.RemoveAll(); }
  void SetUndead(undeadType val) { undead=val; }
  void SetSize(creatureSizeType val) { creatureSize=val; }
  void SetAllowInCombat(BOOL allow) { allowInCombat=allow; }

  BOOL IsAlignmentEvil();
  BOOL IsAlignmentGood();

  int GetEncumbrance() const { return encumbrance; }
  void SetEncumbrance(int val) { val=max(0,val); encumbrance=val; }
  int determineEffectiveEncumbrance(); // compute enc for all non-magical inventory
  int determineEncumbrance();       // compute enc for all inventory
  int determineNormalEncumbrance(); // compute standard enc based on str (no penalty enc)
  int determineMaxEncumbrance();    // compute max enc allowed
  int GetMaxEncumbrance() const { return maxEncumbrance; }
  void SetMaxEncumbrace(int val) { val=max(0,val); maxEncumbrance=val; }
  int GetAdjMaxEncumbrance(DWORD flags=DEFAULT_SPELL_EFFECT_FLAGS);

  BYTE GetStr() const { return strength; }
  BYTE GetStrMod() const { return strengthMod; }
  BYTE GetInt() const { return intelligence; }
  BYTE GetWis() const { return wisdom; }
  BYTE GetDex() const { return dexterity; }
  BYTE GetCon() const { return constitution; }
  BYTE GetCha() const { return charisma; }
  BYTE GetAdjStr(DWORD flags=DEFAULT_SPELL_EFFECT_FLAGS);
  BYTE GetAdjStrMod(DWORD flags=DEFAULT_SPELL_EFFECT_FLAGS);
  BYTE GetAdjInt(DWORD flags=DEFAULT_SPELL_EFFECT_FLAGS);
  BYTE GetAdjWis(DWORD flags=DEFAULT_SPELL_EFFECT_FLAGS);
  BYTE GetAdjDex(DWORD flags=DEFAULT_SPELL_EFFECT_FLAGS);
  BYTE GetAdjCon(DWORD flags=DEFAULT_SPELL_EFFECT_FLAGS);
  BYTE GetAdjCha(DWORD flags=DEFAULT_SPELL_EFFECT_FLAGS);
  void SetStr(BYTE val);
  void SetStrMod(BYTE val);
  void SetInt(BYTE val);
  void SetWis(BYTE val);
  void SetDex(BYTE val);
  void SetCon(BYTE val);
  void SetCha(BYTE val);

  BYTE GetAbility(abilityType type);

  BYTE GetPrimeSpellCastingScore(WORD ClassFlag);
  BOOL HaveSpell(int spelldbkey, BOOL checkmemorized); // check for spell db key in spell book
  int GetSpellBookKey(int spelldbkey, BOOL checkmemorized); // return spell book key for given spell db key

  ABILITYLIMITS GetAbilityLimits(abilityType abType); 
           // Min/Max for this character
           // Four bytes packed into DWORD

  BOOL IsPartyMember() const { return ((type&IN_PARTY_TYPE)>0); }
  void SetPartyMember(BOOL flag=TRUE) { if (flag) type|=IN_PARTY_TYPE; else type &=(~IN_PARTY_TYPE); }

  BYTE GetType() const { return (type&(~IN_PARTY_TYPE)); }
  void SetType(BYTE flag) 
  { 
    /*remove high-bit first*/
    BOOL inparty = IsPartyMember();
    flag &=(~IN_PARTY_TYPE); 
    type=flag;
    if (inparty) SetPartyMember(TRUE);
  }
#ifdef UAFEngine
  // These ModifyXXX functions dynamically alter character
  // values based on spell effects or special abilities.
  virtual BOOL ModifyAttackRollDice(CHARACTER *pTarget,int &num, int &sides, int &bonus);
  virtual BOOL ModifyAttackRollDiceForItem(CHARACTER *pTarget,int itemdbkey,int &num, int &sides, int &bonus);
  virtual BOOL ModifyAttackRollDiceForItemAsTarget(CHARACTER *pAttacker,int itemdbkey,int &num, int &sides, int &bonus);
  virtual BOOL ModifyAttackDamageDiceForItem(CHARACTER *pTarget,int itemdbkey,int &num, int &sides, int &bonus, BOOL &NonLethal);
  virtual BOOL ModifyAttackDamageDiceForItemAsTarget(CHARACTER *pAttacker,int itemdbkey,int &num, int &sides, int &bonus, BOOL &NonLethal);
  virtual BOOL ModifyAttackDamageDice(CHARACTER *pTarget,int &num, int &sides, int &bonus, BOOL &NonLethal);
  virtual BOOL ModifyACAsTarget(CHARACTER *pAttacker, int &ac, int itemkey);
  virtual BOOL ModifyAttackThaco(CHARACTER *pTarget,int &val);
  virtual BOOL ModifyAttackRollDiceAsTarget(CHARACTER *pAttacker,int &num, int &sides, int &bonus);
  virtual BOOL ModifyAttackDamageDiceAsTarget(CHARACTER *pAttacker,int &num, int &sides, int &bonus, BOOL &NonLethal);
  virtual BOOL ModifyAttackThacoAsTarget(CHARACTER *pAttacker,int &val);
  virtual BOOL ModifySaveRoll(CHARACTER *pTarget, int &roll);
  virtual BOOL ModifySaveRollAsTarget(CHARACTER *pAttacker, int &roll);
  CString FormatSpecAbMsg(DWORD sa_state);
#endif

  BOOL GetAutomatic() const { return automatic; }
  void SetAutomatic(BOOL flag) { automatic=flag; }
  BOOL GetAdjAutomatic(DWORD flags=DEFAULT_SPELL_EFFECT_FLAGS);
  
  BOOL GetAllowPlayerControl() const { return allowPlayerControl; }
  void SetAllowPlayerControl(BOOL flag) { allowPlayerControl=flag; SetAutomatic(!allowPlayerControl); }
  BOOL GetAdjAllowPlayerControl(DWORD flags=DEFAULT_SPELL_EFFECT_FLAGS);

  // These properties are only TRUE for a spell's
  // duration. The property will always be FALSE if a spell which
  // grants them is not active.
  //
  BOOL GetDetectingInvisible() const { return detectingInvisible; }
  void SetDetectingInvisible(BOOL flag) { detectingInvisible=flag; }
  BOOL GetAdjDetectingInvisible(DWORD flags=DEFAULT_SPELL_EFFECT_FLAGS);
  BOOL GetDetectingTraps() const { return detectingTraps; }
  void SetDetectingTraps(BOOL flag) { detectingTraps=flag; }
  BOOL GetAdjDetectingTraps(DWORD flags=DEFAULT_SPELL_EFFECT_FLAGS);

   // if such a spell is memorized, return char's spell book key
  DWORD CanCastDetectMagic();
  // if such a spell is memorized, return char's spell book key
  DWORD CanCastIdentify();

  raceType race(void) const {return m_race;};
  void race(raceType race);

  void getCharTHACO();
  int GetTHACO(void) const;
  int GetAdjTHACO(DWORD flags=DEFAULT_SPELL_EFFECT_FLAGS);
  void SetTHACO(int val);
  
  int GetAC() const { return AC; }
  void getCharBaseAC();
  int GetAdjAC(DWORD flags=DEFAULT_SPELL_EFFECT_FLAGS);
  void SetAC(int val);
  int SetCharAC();

  void DetermineCharMaxHitPoints();
  void DetermineNewCharMaxHitPoints();   
  int GetHitPoints() const { return hitPoints; }
  int GetAdjHitPoints(DWORD flags=DEFAULT_SPELL_EFFECT_FLAGS);
  void SetHitPoints(int val);
  void RestoreMaxHitPoints();
  int GetMaxHitPoints();
  void SetMaxHitPoints(int val);
  void RestoreToBestCondition();
    
  int GetAge() const { return age; }
  int GetAdjAge(DWORD flags=DEFAULT_SPELL_EFFECT_FLAGS);
  void SetAge(int val);
  int GetBirthday() const { return birthday; }
  void SetBirthday(int bday); // day 1-365
  int GetMaxAge() const { return maxAge; }
  int GetAdjMaxAge(DWORD flags=DEFAULT_SPELL_EFFECT_FLAGS);
  void DetermineCharStartAge();
  void DetermineCharMaxAge();
  void SetMaxAge(int val) { maxAge = val; }

  int determineMaxMovement();
  BYTE GetMaxMovement() const { return maxMovement; }
  BYTE GetAdjMaxMovement(DWORD flags=DEFAULT_SPELL_EFFECT_FLAGS);
  void SetMaxMovement(BYTE val);

  BOOL IsReadyToTrain(); // checks exp pts and sets readyToTrain flag
  BOOL GetReadyToTrain() { return readyToTrain; }
  BOOL GetAdjReadyToTrain(DWORD flags=DEFAULT_SPELL_EFFECT_FLAGS);
  void SetReadyToTrain(BOOL enable) { readyToTrain = enable; } 
  BOOL IsAbleToTrade() const { return canTradeItems; }
  void SetAbleToTrade(BOOL enable) { canTradeItems=enable; }

  int getCharExpWorth(); // total value, exp + items
  void giveCharacterExperience(int exp, BOOL UseLimits=TRUE); // split among classes
  int GetCurrExp(int classflag);
  void SetCurrExp(int classflag, int exp);
  int IncCurrExp(int classflag, int exp); // +/- adjustment

  int GetAdjClericExp(DWORD flags=DEFAULT_SPELL_EFFECT_FLAGS);
  int GetAdjFighterExp(DWORD flags=DEFAULT_SPELL_EFFECT_FLAGS);
  int GetAdjMagicUserExp(DWORD flags=DEFAULT_SPELL_EFFECT_FLAGS);
  int GetAdjThiefExp(DWORD flags=DEFAULT_SPELL_EFFECT_FLAGS);
  int GetAdjRangerExp(DWORD flags=DEFAULT_SPELL_EFFECT_FLAGS);
  int GetAdjPaladinExp(DWORD flags=DEFAULT_SPELL_EFFECT_FLAGS);
  int GetAdjDruidExp(DWORD flags=DEFAULT_SPELL_EFFECT_FLAGS);
  int GetClericExp() const { return ClericExperience; }
  int GetFighterExp() const { return FighterExperience; }
  int GetMagicUserExp() const { return MUExperience; }
  int GetThiefExp() const { return ThiefExperience; }
  int GetRangerExp() const { return RangerExperience; }
  int GetPaladinExp() const { return PaladinExperience; }
  int GetDruidExp() const { return DruidExperience; }

  int GetAdjSpecAb(int sa, DWORD *pSource=NULL, DWORD *pSrcKey=NULL);
  void SetSpecAb(int sa, BYTE enable, DWORD flags);
  void QueueUsedSpecAb(int sa, WORD src, DWORD key);
  void ClearQueuedSpecAb();

  void AdjustEffectData(const char *akey, int val);

  BOOL IsMammal();
  BOOL IsAnimal();
  BOOL IsSnake();
  BOOL IsGiant();
  BOOL IsAlwaysLarge();

  BOOL HasDwarfACPenalty();
  BOOL HasGnomeACPenalty();
  BOOL HasDwarfTHACOPenalty();
  BOOL HasGnomeTHACOPenalty();
  BOOL HasRangerDmgPenalty();

  BOOL HasPoisonImmunity();
  BOOL HasDeathImmunity();
  BOOL HasConfusionImmunity();
  BOOL HasVorpalImmunity();

  BOOL CanBeHeldOrCharmed();
  BOOL AffectedByDispelEvil();

  int key;	     // unique id for NPC in NPC data table
  int origIndex; // id for npc or monster if added to party or combatData tables
    
  BYTE marchOrder; // 0-7 party id, never changes, even when party order changes
  BOOL IsPreGen;   // means NPC is available to add to party at start
  BOOL CanBeSaved; // allow saving NPC for later restore?
	
  BOOL HasLayedOnHandsToday; // Paladins allowed to heal self/others once per day

	int unarmedDieS;   // if not using a weapon
	int unarmedNbrDieS;
	int unarmedBonus;
	int unarmedDieL;
	int unarmedNbrDieL;

  BOOL DisableTalkIfDead;
  DWORD TalkEvent; // event chain used for 'Talk' menu 
  CString TalkLabel; // text used in menu to trigger talk event
  DWORD ExamineEvent;
  CString ExamineLabel;
  BYTE ItemMask;  // determines which items can be readied
  
  long unused; // was iconSurface;
  int iconIndex; // which icon to use - default is 1
  PIC_DATA icon;
  PIC_DATA smallPic;
  CList<SPELL_EFFECTS_DATA, SPELL_EFFECTS_DATA&> spellEffects;
  SPECIAL_ABILITIES specAbs;
  spellBookType spells;
  ITEM_LIST myItems;
  MONEY_SACK money;  
  TRAITS *m_traits; // Allocated/deallocated in constructor/destructor
  BLOCKAGE_STATUS blockageData; // which locked doors have been attempted?
  ASL char_asl;

  struct SQueuedSpecAb
  {
    SQueuedSpecAb() { sa=0;src=0;key=0xFFFFFFFF; }
    int sa;
    WORD src;
    DWORD key;
  };

protected: // attributes of a character

  CString name;
  raceType m_race;
	int THACO;
	int AC;
  int hitPoints;
  int maxHitPoints;
	BOOL readyToTrain;
  BOOL canTradeItems; // can restrict player from taking NPC items
  BOOL automatic;  // combat only, true means control with AI instead of player control
  BOOL allowPlayerControl; // combat only, if true plyr can control actions
  //BOOL detectingMagic;
  BOOL detectingInvisible;
  BOOL detectingTraps;

  // current exp in each class	
  int FighterExperience;
	int ClericExperience;
	int RangerExperience;
	int PaladinExperience;
	int MUExperience;
	int ThiefExperience;
  int DruidExperience;

	int age;
	int maxAge;	
  int birthday;
  BYTE maxMovement;

  // low 7 bits indicate pc, npc, or even monster during combat
  // high-bit indicates member of party: 1=inparty,0=notinparty
  BYTE type;

  // regular attributes
	BYTE strength;
	BYTE strengthMod;
	BYTE intelligence;
	BYTE wisdom;
	BYTE dexterity;
	BYTE constitution;
	BYTE charisma;

  // keys into abilityData database
  // no need to serialize these, they are initialized
  // at runtime
  abilityType strengthKey;
  abilityType intelligenceKey;
  abilityType wisdomKey;
  abilityType dexterityKey;
  abilityType constitutionKey;
  abilityType charismaKey;

  int encumbrance;
	int maxEncumbrance;

  int hitBonus;
  int dmgBonus;
  double nbrHitDice;
	float NbrAttacks; // per round
  
  // saving throw values
	int magicResistance;  // only monsters have non-zero mr
	int saveVsParPsnDMag; // paralysis, poison, death magic
	int saveVsPetPoly;    // petrify, polymorph
	int saveVsRdStWnd;    // rod, staff, wand
	int saveVsBreath;     // breath weapon
	int saveVsSpell;      // spell

  int morale;	 // 0-100, higher is better
  int maxCureDisease; // uses per day, 0=none
  int usedCureDisease; // times used this day (not serialized or used yet!)

  // thief attributes, 0-100 percentage
	int pickPockets;
	int openLocks;
	int findRemoveTrap;
	int moveSilent;
	int hideInShadows;
	int hearNoise;
	int climbWalls;
	int readLanguages;

  // attributes based on strength
  BYTE openDoors;      // locked doors
  BYTE openMagicDoors; // wizard locked doors
  BYTE BB_LG;	         // bend bars - lift gates

   // current levels (more than one set for Multi-Class)
	BYTE FighterLevel;
	BYTE ClericLevel;
	BYTE RangerLevel;
	BYTE PaladinLevel;
	BYTE MULevel;
	BYTE ThiefLevel;
  BYTE DruidLevel;

	// levels attained before changing class (Dual-Class)
	BYTE prevFighterLevel;
	BYTE prevClericLevel;
	BYTE prevRangerLevel;
	BYTE prevPaladinLevel;
	BYTE prevMULevel;
	BYTE prevThiefLevel;
  BYTE prevDruidLevel;

	// pre-drain levels
	BYTE pdFighterLevel;
	BYTE pdClericLevel;
	BYTE pdRangerLevel;
	BYTE pdPaladinLevel;
	BYTE pdMULevel;
	BYTE pdThiefLevel;
  BYTE pdDruidLevel;

	genderType       gender;
	classType        charClass;
	alignmentType    alignment;
	charStatusType   status;
	undeadType       undead; // monsters only
	creatureSizeType creatureSize;

  BOOL allowInCombat; // for NPC's, if false it won't appear in combat events

  CList<SQueuedSpecAb, SQueuedSpecAb&> specAbQueue; // special abilities used recently

  //CPropertyContainer m_Properties;
  
  BYTE GetClassLevel(WORD ClassFlag); // curr level for specific class
  BYTE GetClassPrevLevel(WORD ClassFlag); // prev level for specific class
  BYTE GetClassPreDrainLevel(WORD ClassFlag); // pre-drain level for specific class
  bool HaveEffectAttr(const SPELL_EFFECTS_DATA &src) const; 
  POSITION GetFirstEffectAttributeMod(CString attr);
  POSITION GetNextEffectAttributeMod(CString attr, POSITION start);

};


#ifdef UAFEngine
extern CHARACTER character;
#endif

class CHAR_LIST : public CObject
{
  CCriticalSection m_CS;
  OrderedQueue<CHARACTER, MAX_CHARACTERS> chars;

  int GetNextKey();
  int m_FindNamei (const CString& name); // case-insensitive
  BOOL m_IsTempCharList;

public:
  DECLARE_SERIAL( CHAR_LIST )
  CHAR_LIST();
  CHAR_LIST(const CHAR_LIST& src);
  ~CHAR_LIST(); 
  void Clear();
  int GetCount();
  int numChars();

  void SetAsTempCharList() { m_IsTempCharList=TRUE; }
  BOOL IsTempCharList() { return m_IsTempCharList; }

  POSITION GetHeadPosition();
  CHARACTER& GetAt(POSITION pos);
  CHARACTER& GetNext(POSITION &pos);

  CHAR_LIST& operator=(const CHAR_LIST& src);
  
  void Serialize(CArchive &ar, double version);
  void Serialize(CAR &ar, double version);
  BOOL Validate(int index, FILE *pFile, WORD flags=VAL_ALL); // specific char
  BOOL Validate(FILE *pFile, WORD flags=VAL_ALL); // all chars
  BOOL HaveKey(int index);
  
  int FindNamei(const CString& name); // case-insensitive

#ifdef UAFEngine
  void CastSpell(int index, int spell);
  void SpellActivate(int index, PENDING_SPELL &data);
#endif
  // called when loading from scripts, or
  // when adding from editor GUI
  int AddCharacter(CHARACTER &data);

  // called when loading binary monster data that
  // was saved with key info
  int AddCharacterWithCurrKey(CHARACTER &data);

  CString GetCharacterName(int index);
  BOOL SetCharacter(int index, CHARACTER &data);
  BOOL RemoveCharacter(int index);
  BOOL DeleteCharacterEvents(int index);
  BOOL GetCharacterData(int index, CHARACTER &data);  
  CHARACTER *GetCharacterData(int index);  
};


void tradeItemMoney(itemClassType type, int qty, CHARACTER& giver, CHARACTER& taker);
void tradeItem(int item, int qty, int charges, int id, int g_index, CHARACTER& giver, CHARACTER& taker);
void addSpecialItemsToItemList(ITEM_TEXT_LIST &list);

#endif