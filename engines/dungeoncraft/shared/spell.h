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

#ifndef DUNGEONCRAFT_SHARED_SPELL
#define DUNGEONCRAFT_SHARED_SPELL

//#include "dungeoncraft/shared/externs.h"
#include "dungeoncraft/shared/SpecAb.h"
#include "dungeoncraft/shared/SharedQueue.h"
#include "dungeoncraft/shared/PicData.h"
#include "dungeoncraft/shared/class.h"
#include "dungeoncraft/shared/gamerules.h"

namespace DungeonCraft {

const int COST_FLAG                   = 0x0001;
const int LEVEL_FLAG                  = 0x0002;
const int QTY_FLAG                    = 0x0004;
const int MEMORIZE_FORMAT_FLAG        = 0x0008;
const int SHOW_SELECTED_FLAG          = 0x0010;
const int SHOW_MEMORIZED_FLAG         = 0x0020;
const int SHOW_NOT_MEMORIZED_FLAG     = 0x0040;
const int SHOW_NOT_SELECTED_FLAG      = 0x0080;
const int CAST_FORMAT_FLAG            = 0x0100;
const int SHOW_DESIGN_FLAG            = 0x0200;
const int NEW_SPELL_LEVEL_FORMAT_FLAG = 0x0400;
const int TEMPLE_CAST_FORMAT_FLAG     = 0x0800;

const int MAX_SPELL_NAME = 50;
const int MAX_SPELLS = 0x000FFFFF;
const int NO_SPELL = MAX_SPELLS+1;

const int MAX_ACTIVE_SPELLS = INT_MAX;
const int MAX_PENDING_SPELLS = INT_MAX;

struct CHARACTER_SPELL 
{
  CHARACTER_SPELL() { Clear(); }
  ~CHARACTER_SPELL() { Clear(); }
  void Clear();
  CHARACTER_SPELL &operator=(const CHARACTER_SPELL &cSpell);
  BOOL operator==(const CHARACTER_SPELL &src);
  void Serialize(CArchive &ar, double version);
  void Serialize(CAR &ar, double version);

  BOOL IsSelected() { return (selected > 0); }
  BOOL IsMemorized() { return (memorized > 0); }
  void ResetMemorizedTime() { memTime=0; }
  BOOL IncMemorizedTime(int minuteInc);
  BOOL MemTimeSufficient();
  void SetMemorized();
  void SetUnMemorized();
  BOOL HaveUnmemorized();

  int selected;  // TRUE if dude will memorize this spell again
	int memorized; // 
	int spell;      // SpellDbKey to retrieve data from spellData table
  int level;      // spell level, here for convenience
  int memTime;    // total time spent on memorizing this spell
  BOOL JustMemorized;
};

struct SPELL_LIST 
{
  SPELL_LIST() { Clear(); }
  ~SPELL_LIST() { Clear(); }
  void Clear();
  int GetCount() const;

  SPELL_LIST &operator=(const SPELL_LIST &list);
  BOOL operator==(const SPELL_LIST &src);
  void Serialize(CArchive &ar, double version);
  void Serialize(CAR &ar, double version);

  // these access the sorted list and will return
  // spells is sorted order regardless of spell book
  // ordering.
  POSITION GetHeadPosition();
  CHARACTER_SPELL& GetAt(POSITION pos);
  CHARACTER_SPELL& GetNext(POSITION &pos);
  int GetKeyAt(POSITION pos);

  BOOL IsMemorized(int index);
  BOOL DecMemorized(int index, int memorized_count);
  BOOL SetSelected(int index, int selected);
  BOOL IsSelected(int index);
  BOOL HaveSpell(int index) const;
  BOOL AddSpell(CHARACTER_SPELL &data);
  void SortSpells(void);
  BOOL RemoveSpell(int index);  
  void RemoveSpellLevel(int level);
  int SpellsMemorized();
  BOOL GetSpell(int index, CHARACTER_SPELL &spell);
  BOOL SetSpell(int index, CHARACTER_SPELL &spell);
  DWORD GetRestTime(int index);
  int getSelectedCount(int index);
  int getMemorizedCount(int index);
  int getMemorizedTime(int index);
  int resetMemorizedTime(int index);
  int incMemorizedTime(int index, int minuteInc);

  OrderedQueue <CHARACTER_SPELL, MAX_SPELLS> spells;
  CList<int, int> sortedAccess;
};


// The keys used to access spell data are the
// spell db keys. Some code appears to be using
// an actual spell book key instead, but in
// reality the lookup of such a key only
// returns the spell db key. 
//
// When added, each new spell uses
// the spell db key as the SPELL_LIST key.
//
struct spellBookType 
{
  spellLimitsType spellLimits;
	SPELL_LIST list;
  DWORD spellPrepTimeNeeded;
  DWORD spellPrepTimeUsed;

  spellBookType() { Clear(); }
  ~spellBookType() { Clear(); }

  int SpellsKnown();
  int SpellsMemorized();

  void Clear();
  spellBookType &operator=(const spellBookType &book);
  BOOL operator==(const spellBookType &book);

  void Serialize(CArchive &ar, double version);
  void Serialize(CAR &ar, double version);

  // index is spell book key, not spell database key
  BOOL isKnown(int index) const;
  BOOL isMemorized(int index);
  BOOL isSelected(int index);
  int  getSelectedCount(int index);
  int  getMemorizedCount(int index);
  int getMemorizedTime(int index);
  int resetMemorizedTime(int index);
  int incMemorizedTime(int index, int minuteInc);
  void resetAllMemorizedTime();
  BOOL incAllMemorizedTime(int minuteInc);
  void RemoveSpellLevel(int level); 
  BOOL CanKnowSpell(WORD CasterClassFlag, WORD SpellClassFlag, WORD CastMask, int SpellLevel);
  BOOL knowSpell(WORD CasterClassFlag, WORD SpellClassFlag, int SpellDbKey, int level, BOOL known);
  BOOL selectSpell(int index, int selected=1);
  void UnselectIfNotMemorized(void);
  void UnselectAll(void);
  DWORD CalcRestTime(void);
  DWORD CalcSpellPrepTime(void);
};

class SPELL_TARGETING_DATA
{
public:
  SPELL_TARGETING_DATA();
  SPELL_TARGETING_DATA(const SPELL_TARGETING_DATA &src);
  SPELL_TARGETING_DATA &operator=(const SPELL_TARGETING_DATA &src);
  bool operator==(const SPELL_TARGETING_DATA& src) const;
  void Clear();

  int NumTargets();
  BOOL AddTarget(int target, double hd=0.0, int range=0);
  BOOL AddMapTarget(int mapx, int mapy);

  BOOL CanAddTarget(int hd=0, int range=0);
  BOOL AllTargetsChosen();
  BOOL ValidNumTargets();
  BOOL HDLimitReached();
  CString FormatRemainingTargsText();

  spellTargetingType m_type;
  BOOL m_SelectingUnits; // vs selecting map location
  BOOL m_FriendlyFireOk; // can party member be targeted?
  BOOL m_IsPartyAligned;
  double m_MaxHitDice;
  double m_HDTotal;
  int m_MaxTargets;
  int m_MaxRange;
  int m_MapTargetX;
  int m_MapTargetY;
  CArray<int, int> m_targets;
  MAPLOCATIONLIST m_maptarglist;
};

class SPELL_DATA : public CObject
{
public:
  DECLARE_SERIAL( SPELL_DATA )
  SPELL_DATA();
  SPELL_DATA(const SPELL_DATA &src) : 
	  spell_asl ("SPELL_DATA_ATTRIBUTES"),
	  temp_asl  ("SPELL_DATA_ATTRIBUTES"),
    hCastSound(-1),hMissileSound(-1),hCoverageSound(-1),hHitSound(-1),hLingerSound(-1)
      { Clear(); *this = src; }
  ~SPELL_DATA() { ClearSounds(); Clear(TRUE); }
  SPELL_DATA &operator=(const SPELL_DATA &src);
  bool operator ==(const SPELL_DATA& src) const;

  void Clear(BOOL ctor=FALSE);

  void SetSpellAnimDefaults(PIC_DATA &art, const char *file);
  
  void Serialize(CArchive &ar, double version);
  void PreSerialize(BOOL IsStoring);
  void PostSerialize(BOOL IsStoring);
  void Serialize(CAR &ar, double version);
  void PreCARSerialize(BOOL IsStoring);
  void PostCARSerialize(BOOL IsStoring);

  void Save(CArchive& ar);
  void PrepareForRestore(void);
  void Restore(CArchive& ar);
  void CommitRestore(void);

  void PlayCast();
  void PlayMissile();
  void PlayCoverage();
  void PlayHit();
  void PlayLinger();

  void ClearSounds();
  void LoadArt();
  void ClearArt();
  void FreeArt();

  DWORD key;
	CString Name;
  CString CastSound;
  int hCastSound;
  int hMissileSound;
  int hCoverageSound;
  int hHitSound;
  int hLingerSound;

  // what school this spell belongs to
  // Must not indicate more than one class!
	WORD ClassMask; 
  WORD CastMask; // which classes are allowed to cast this spell
  BOOL IsCumulative; // can cast multiple times?
	BOOL Friendly_Fire_Ok;
  BOOL Combat_Only;
  BOOL UsableByParty;
  BOOL CanScribeAtLevelChange; // always true for clerics, affects MU spells only
  BOOL AutoScribeAtLevelChange; // always true for clerics, affects MU spells only
  BOOL CanBeDispelled;
  BOOL Lingers;
  BOOL LingerOnceOnly; // affects target once only, else once per round
	spellSaveVsType Save_Vs;
	spellSaveEffectType Save_Result;
	spellTargetingType Targeting;
	spellDurationType Duration_Rate;
  spellCastingTimeType Casting_Time_Type;
	long Level;
	long Casting_Time;
  long Cast_Cost;
  long Cast_Priority;
	SPECIAL_ABILITIES specAbs;
  DICEPLUS Duration; // duration for spell
  DICEPLUS EffectDuration; // duration for spell's effects on target (if different than spell's duration)
  DICEPLUS NumTargets;
  DICEPLUS TargetRange;
  CList< SPELL_EFFECTS_DATA, SPELL_EFFECTS_DATA& > m_EffectedAttributes;
  CString MissileSound;
  CString CoverageSound;
  CString HitSound;
  CString LingerSound;
  CString CastMsg; // what msg is displayed as spell affects the target?
  PIC_DATA CastArt;          // used when spell is cast, along with CastSound
  PIC_DATA MissileArt;       // used in-route to target
  PIC_DATA TargCoverageArt;  // displays over target area when target reached
  PIC_DATA TargHitArt;       // used on each succesfully hit target (each in turn)
  PIC_DATA TargLingerArt;    // used if graphic is displayed for spell duration

  bool CompileScripts();
  bool ExecuteSpellBeginScript() const;
  void ExecuteSpellEndScript() const;
    
  CString SpellBeginSource;
  CString SpellBeginBinary;
  CString SpellEndSource;
  CString SpellEndBinary;
  
  POSITION AddEffect(SPELL_EFFECTS_DATA &src);
  void DeleteEffect(SPELL_EFFECTS_DATA &src);
  bool HaveEffectAttr(const SPELL_EFFECTS_DATA &src) const;
  bool CompileScript(SPELL_EFFECTS_DATA &src);
  bool ExecuteActivationScript(const SPELL_EFFECTS_DATA &src);
  int ExecuteModificationScript(const SPELL_EFFECTS_DATA &src);
  int ExecuteSavingThrowScript(const SPELL_EFFECTS_DATA &src);    
  void ExecuteSavingThrowFailedScript(const SPELL_EFFECTS_DATA &src);    
  void ExecuteSavingThrowSucceededScript(const SPELL_EFFECTS_DATA &src);    

  BOOL CanPerformSpecAb(specialAbilitiesType sa);
  bool CanEnableAffectAttr(const CString &attr) const;

	ASL spell_asl;
	ASL temp_asl;
};

class SPELL_DATA_TYPE : public CObject
{
  CCriticalSection m_CS;
  OrderedQueue<SPELL_DATA, MAX_SPELLS> SpellData;

public:
  double version;
  DECLARE_SERIAL( SPELL_DATA_TYPE )
  SPELL_DATA_TYPE() { Clear(); }
  ~SPELL_DATA_TYPE() { Clear(); }
  SPELL_DATA_TYPE& operator =(SPELL_DATA_TYPE& src);
  bool operator ==(SPELL_DATA_TYPE& src);

  void Clear();

  void Serialize(CArchive &ar, double version);
  void PreSerialize(BOOL IsStoring);
  void PostSerialize(BOOL IsStoring);

  void Serialize(CAR &ar, double version);
  void PreCARSerialize(BOOL IsStoring);
  void PostCARSerialize(BOOL IsStoring);

  void Save(CArchive& ar);
  void Restore(CArchive& ar); // Restore to temporary location
  void CommitRestore(void);

  int GetCount() { CSingleLock sLock(&m_CS, TRUE); return SpellData.GetCount(); }
  void SaveSounds();
  void SaveArt();

  POSITION GetHeadPosition() { CSingleLock sLock(&m_CS, TRUE); return SpellData.GetHeadPosition(); }
  SPELL_DATA& GetAt(POSITION pos) { CSingleLock sLock(&m_CS, TRUE); return SpellData.GetAt(pos); }
  SPELL_DATA& GetNext(POSITION &pos) { CSingleLock sLock(&m_CS, TRUE); return SpellData.GetNext(pos); }
  int GetKeyAt(POSITION pos) { CSingleLock sLock(&m_CS, TRUE); return SpellData.GetKeyAt(pos); }

  void ExpandDefaultFilenames();
#ifdef UAFEDITOR
  void CollapseDefaultFilenames();
#endif

  DWORD GetNextKey();
  BOOL Validate(SPELL_DATA &data, FILE *pFile);
  BOOL Validate(int index, FILE *pFile); // specific spell
  BOOL Validate(FILE *pFile); // all spells

  void PlayCast(DWORD index);
  void ClearSounds();
  void FreeArt();
  WORD GetSpellClass(DWORD index);
  WORD GetSpellCastMask(DWORD index);
  int GetSpellLevel(DWORD index);
  int GetSpellCastCost(DWORD index);
  CString GetSpellName(DWORD index);
  BOOL HaveSpell(DWORD index);
  // called when loading from scripts, or
  // when adding from editor GUI
  DWORD AddSpell(SPELL_DATA &data);
  // called when loading binary monster data that
  // was saved with key info
  DWORD AddSpellWithCurrKey(SPELL_DATA &data);
  BOOL SetSpell(DWORD index, SPELL_DATA &spell);
  BOOL RemoveSpell(DWORD index);
  SPELL_DATA *GetSpellData(DWORD index);

private:
  // No locking... for internal use
  int m_FindName (const CString& name);

public:
  // With locking...for public use
  int FindName(const CString& name);
};

extern SPELL_DATA_TYPE spellData;

struct CHARACTER_SCROLL 
{
	BYTE qty;
	int scroll;
};

#ifdef UAFEngine

struct SPELL_TEXT_LIST 
{
  SPELL_TEXT_LIST() { Clear(); }
  ~SPELL_TEXT_LIST() { Clear(); }

  void Clear();
  int GetCount() const { return spells.GetSize(); }
  int GetSpellIndex(int index) const { return spells[index]; }
  int GetSpellMemorizeCount(int index);
  int GetSpellSelectedCount(int index);
  int GetSpellDatabaseKey(int index);
  int GetSpellBookKey(int index);
  int GetSpellSchool(int index);
  void GetSpellLevelAndSchoolIndex(int index, int &lvl, int &sindex);

  void IncSpellSelectedCount(int index);
  void DecSpellSelectedCount(int index);
  void DecSpellMemorizedCount(int index);
  void ToggleSelected(int index);
  BOOL IsSelected(int index);
  //int  NumSelected();
  BOOL CanSelect(int index);
  BOOL CanSelectAnything(); // return TRUE if MaxSelect[] is non-zero in any slot

  void FillTempleCastSpellListText(CHARACTER &data, costFactorType type);
  void FillCastSpellListText(CHARACTER &data);
  void FillNewSpellLevelListText(CHARACTER &data);
  void FillMemorizeSpellListText(CHARACTER &data);
  void FillSpellEffectsListText(CHARACTER &data);
  void FillSpellListText(CHARACTER &cdata, spellBookType &list, UINT flags, costFactorType type=Normal);

  void Add(DWORD index) { spells.Add(index); isSelected.Add(FALSE); }
  BOOL Have(DWORD index);

  char *GetSpellLevelText(int index);
  char *GetSpellSelectedText(int index);
  char *GetSpellMemorizedText(int index);
  char *GetSpellCostText(int index);
  char *GetSpellNameText(int index);
  char *GetSpellText(int index);
  void FormatSpellListTitle(const char *EmptySpellTitle=NULL);

  BOOL UseTempleCastLayout() { return (m_flags & TEMPLE_CAST_FORMAT_FLAG); }
  BOOL UseCastLayout() { return (m_flags & CAST_FORMAT_FLAG); }
  BOOL UseMemorizeLayout() { return (m_flags & MEMORIZE_FORMAT_FLAG); }
  BOOL UseNewSpellLevelLayout() { return (m_flags & NEW_SPELL_LEVEL_FORMAT_FLAG); }
  
  BOOL UseDesignLayout() { return (m_flags & SHOW_DESIGN_FLAG); }
  BOOL UseLevel() { return ((m_flags & LEVEL_FLAG) && (!UseDesignLayout()) && (!UseMemorizeLayout()) && (!UseNewSpellLevelLayout()) ); }
  BOOL UseCost()  { return ((m_flags & COST_FLAG) && (!UseDesignLayout()) && (!UseMemorizeLayout()) && (!UseNewSpellLevelLayout()) ); }
  
  BOOL UsesSpellBookPos() { return ((UseTempleCastLayout())||(UseCastLayout())||(UseMemorizeLayout())); }
  BOOL UsesSpellDbKey() { return ((!UsesSpellBookPos())||(UseNewSpellLevelLayout())); }
  BOOL UsesSelectionArrays() { return ((UseNewSpellLevelLayout())||(UseMemorizeLayout())); }

  costFactorType CostFactor;
  DWORD m_flags;

	char temp[MAX_SPELL_NAME+50];
  char text[300];

  CHARACTER *m_CharData;
  //int m_SpellLevelFilter;
  //int m_school;
  WORD spellClasses;
  CString title;  

  // this will keep track of how many spells are
  // currently selected and how many can be selected,
  // and is used by:
  //
  // FillNewSpellLevelListText:  SpellDbKey
  // FillMemorizeSpellListText:  character's spell book POS
  //
  BYTE MaxLevel[NumClassFlags];
  BYTE CurrSelect[MAX_SPELL_LEVEL][NumClassFlags];
  BYTE MaxSelect[MAX_SPELL_LEVEL][NumClassFlags];

  CArray<BOOL, BOOL> isSelected;

  // this will contain either the SpellDbKey,
  // or a POSITION for access to character's spell book
  //
  // FillNewSpellLevelListText:  SpellDbKey
  // FillCastSpellListText:      character's spell book POS
  // FillTempleCastSpellListText character's spell book POS
  // FillMemorizeSpellListText:  character's spell book POS
  // FillSpellEffectsListText:   SpellDbKey
  // FillSpellListText:          SpellDbKey
  CArray<DWORD, DWORD> spells;
};

struct PENDING_SPELL
{
  PENDING_SPELL() { Clear(); }
  ~PENDING_SPELL() { Clear(); }
  void Clear() { key=-1;waitUntil=0;caster.Clear(); }
  PENDING_SPELL &operator=(const PENDING_SPELL &src)
  {
    key=src.key;
    caster=src.caster;
    waitUntil=src.waitUntil;
    flags=src.flags;
    return *this;
  }

  int key; // key that id's this spell
  ActorType caster; // context info to id caster
  DWORD waitUntil; // clock time when spell is activated
  DWORD flags;
};

struct SPELL_LINGER_DATA
{
  SPELL_LINGER_DATA() { Clear(); }
  
  void Clear()
  {
    OnceOnly=FALSE;
    mapdata.RemoveAll();
    affectedtargs.RemoveAll();
  }

  void Serialize(CAR &ar, double ver)
  {
    if (ar.IsStoring())
    {
      ar << OnceOnly;
      ar << mapdata.GetCount();
      POSITION pos = mapdata.GetHeadPosition();
      while (pos != NULL)
      {
        POINT pt = mapdata.GetAt(pos);
        
        ar << pt.x;
        ar << pt.y;
        
        mapdata.GetNext(pos);
      }
      
      ar << affectedtargs.GetCount();
      pos = affectedtargs.GetHeadPosition();
      while (pos != NULL)
      {
        int dude = affectedtargs.GetAt(pos);        
        ar << dude;        
        affectedtargs.GetNext(pos);
      }
    }
    else
    {
      ar >> OnceOnly;
      int count;
      int i,dude;
      POINT pt;
      ar >> count;
      for (i=0;i<count;i++)
      {
        ar >> pt.x;
        ar >> pt.y;
        mapdata.AddTail(pt);
      }      
      ar >> count;
      for (i=0;i<count;i++)
      {
        ar >> dude;
        affectedtargs.AddTail(dude);
      }      
    }
  }
  void Serialize(CArchive &ar, double ver)
  {
    if (ar.IsStoring())
    {
      ar << OnceOnly;
      ar << mapdata.GetCount();
      POSITION pos = mapdata.GetHeadPosition();
      while (pos != NULL)
      {
        POINT pt = mapdata.GetAt(pos);
        
        ar << pt.x;
        ar << pt.y;

        mapdata.GetNext(pos);
      }

      ar << affectedtargs.GetCount();
      pos = affectedtargs.GetHeadPosition();
      while (pos != NULL)
      {
        int dude = affectedtargs.GetAt(pos);        
        ar << dude;        
        affectedtargs.GetNext(pos);
      }
    }
    else
    {
      ar >> OnceOnly;
      int count;
      int i,dude;
      POINT pt;
      ar >> count;
      for (i=0;i<count;i++)
      {
        ar >> pt.x;
        ar >> pt.y;
        mapdata.AddTail(pt);
      }      
      ar >> count;
      for (i=0;i<count;i++)
      {
        ar >> dude;
        affectedtargs.AddTail(dude);
      }      
    }
  }
  SPELL_LINGER_DATA &operator=(const SPELL_LINGER_DATA &src)
  {
    if (this==&src) return *this;
    
    OnceOnly=src.OnceOnly;
    
    mapdata.RemoveAll();
    POSITION pos = src.mapdata.GetHeadPosition();
    while (pos!=NULL)
    {
      POINT pt = src.mapdata.GetNext(pos);
      mapdata.AddTail( pt );
    }

    affectedtargs.RemoveAll();
    pos = src.affectedtargs.GetHeadPosition();
    while (pos!=NULL)
    {
      int dude = src.affectedtargs.GetNext(pos);
      affectedtargs.AddTail( dude ); 
    }
    
    return *this;
  }

  void AddTarget(int targ)
  {
    POSITION pos = affectedtargs.GetHeadPosition();
    while (pos != NULL)
    {
      if (affectedtargs.GetNext(pos) == targ)
        return;
    }
    affectedtargs.AddTail(targ);
    return;    
  }

  BOOL EligibleTarget(int targ)
  {
    POSITION pos = affectedtargs.GetHeadPosition();
    while (pos != NULL)
    {
      if (affectedtargs.GetAt(pos) == targ)
        return (!OnceOnly);
      affectedtargs.GetNext(pos);
    }
    //affectedtargs.AddTail(targ);
    return TRUE;
  }

  BOOL AffectsTarget(int targ, int mapx, int mapy)
  {
    BOOL found=FALSE;
    POSITION pos = mapdata.GetHeadPosition();
    while ((pos != NULL) && (!found))
    {
      POINT pt = mapdata.GetAt(pos);
      if ((pt.x == mapx) && (pt.y == mapy))
        found = TRUE;
      mapdata.GetNext(pos);
    }

    // spell not affecting specified map location
    if (!found)
      return FALSE;

    // spell is affecting specified map location,
    // so check for target eligibility.
    return (EligibleTarget(targ));
  }

  BOOL OnceOnly;
  MAPLOCATIONLIST mapdata;
  MAPTARGETLIST affectedtargs;
};

struct ACTIVE_SPELL
{
  ACTIVE_SPELL() { Clear(); }
  ~ACTIVE_SPELL() { Clear(); }
  void Clear() { key=-1;spellKey=-1;caster.Clear();StopTime=0;CountTime=0;Lingers=FALSE;lingerData.Clear(); }
  void Serialize(CAR &ar, double ver)
  {
    if (ar.IsStoring())
    {
      ar << key;
      caster.Serialize(ar, ver);
      ar << spellKey;
      ar << StopTime;
      ar << CountTime;
      ar << Lingers;

      lingerData.Serialize(ar, ver);
    }
    else
    {
      ar >> key;
      caster.Serialize(ar, ver);
      ar >> spellKey;
      ar >> StopTime;
      ar >> CountTime;

      if (ver >= _VERSION_0906_)
      {
        ar >> Lingers;
        lingerData.Serialize(ar, ver);      
      }
    }
  }
  void Serialize(CArchive &ar, double ver)
  {
    if (ar.IsStoring())
    {
      ar << key;
      caster.Serialize(ar, ver);
      ar << spellKey;
      ar << StopTime;
      ar << CountTime;
      ar << Lingers;

      lingerData.Serialize(ar, ver);
    }
    else
    {
      ar >> key;
      caster.Serialize(ar, ver);
      ar >> spellKey;
      ar >> StopTime;
      ar >> CountTime;

      if (ver >= _VERSION_0906_)
      {
        ar >> Lingers;
        lingerData.Serialize(ar, ver);
      }
    }    
  }
  ACTIVE_SPELL &operator=(const ACTIVE_SPELL &src)
  {
    if (this==&src) return *this;
    key=src.key;
    caster=src.caster;
    spellKey=src.spellKey;
    StopTime = src.StopTime;
    CountTime = src.CountTime;
    lingerData = src.lingerData;
    Lingers=src.Lingers;
    return *this;
  }
  BOOL LingerSpellAffectsTarget(int targ, int mapx, int mapy)
  {
    if (!Lingers) return FALSE;
    return (lingerData.AffectsTarget(targ, mapx, mapy));
  }
    
  int key; // key that id's this active spell
  int spellKey; // spell db key
  ActorType caster; // context info that id's caster
  DWORD StopTime;
  DWORD CountTime;
  BOOL Lingers;
  SPELL_LINGER_DATA lingerData;
};

class ActiveSpellList
{
public:
  ActiveSpellList() { Clear(); }
  ~ActiveSpellList() { Clear(); }

  void Clear();
  int GetCount() { CSingleLock sLock(&m_CS, TRUE); return m_spells.GetCount(); }
  POSITION GetHeadPosition() { CSingleLock sLock(&m_CS, TRUE); return m_spells.GetHeadPosition(); }
  ACTIVE_SPELL& GetAt(POSITION pos) { CSingleLock sLock(&m_CS, TRUE); return m_spells.GetAt(pos); }
  ACTIVE_SPELL& GetNext(POSITION &pos) { CSingleLock sLock(&m_CS, TRUE); return m_spells.GetNext(pos); }
  int GetKeyAt(POSITION pos) { CSingleLock sLock(&m_CS, TRUE); return m_spells.GetKeyAt(pos); }
  ActiveSpellList &operator=(ActiveSpellList &src);

  // called when loading from scripts, or
  // when adding from editor GUI
  int Add(ACTIVE_SPELL &data);
  // called when loading binary data that
  // was saved with key info
  int AddWithCurrKey(ACTIVE_SPELL &data);
  BOOL Set(int index, ACTIVE_SPELL &spell);
  BOOL Remove(int index);
  ACTIVE_SPELL *Get(int index);
  int GetNextKey();

  void Serialize(CAR &ar, double ver);
  void Serialize(CArchive &ar, double ver);
  void PreSerialize(BOOL IsStoring) { }
  void PostSerialize(BOOL IsStoring) { }

  void ProcessTimeSensitiveData(int roundInc);
  void DeactivateActiveSpell(int index, SPELL_DATA *pSpell);

  BOOL LingerSpellAffectsTarget(int targ, int mapx, int mapy)
  {
    CSingleLock sLock(&m_CS, TRUE);
    POSITION pos = GetHeadPosition();
    while (pos != NULL)
    {
      if (GetAt(pos).LingerSpellAffectsTarget(targ,mapx,mapy))
        return TRUE;
      GetNext(pos);
    }
    return FALSE;
  }    

  BOOL ActivateLingerSpellsOnTarget(int targ, int mapx, int mapy);
  void RemoveLingerSpells();

private:
  CCriticalSection m_CS;
  OrderedQueue<ACTIVE_SPELL, MAX_ACTIVE_SPELLS> m_spells;

  ActiveSpellList(ActiveSpellList &src); // not enabled
};

class PendingSpellList
{
public:
  PendingSpellList() { Clear(); }
  ~PendingSpellList() { Clear(); }

  void Clear() { CSingleLock sLock(&m_CS, TRUE); m_spells.RemoveAll(); }
  int GetCount() { CSingleLock sLock(&m_CS, TRUE); return m_spells.GetCount(); }
  POSITION GetHeadPosition() { CSingleLock sLock(&m_CS, TRUE); return m_spells.GetHeadPosition(); }
  PENDING_SPELL& GetAt(POSITION pos) { CSingleLock sLock(&m_CS, TRUE); return m_spells.GetAt(pos); }
  void RemoveAt(POSITION pos) { CSingleLock sLock(&m_CS, TRUE); m_spells.RemoveAt(pos); }
  PENDING_SPELL& GetNext(POSITION &pos) { CSingleLock sLock(&m_CS, TRUE); return m_spells.GetNext(pos); }
  int GetKeyAt(POSITION pos) { CSingleLock sLock(&m_CS, TRUE); return m_spells.GetKeyAt(pos); }

  int Add(PENDING_SPELL &data);
  BOOL Set(int index, PENDING_SPELL &spell);
  BOOL Remove(int index);
  PENDING_SPELL *Get(int index);

  // no need to save pending spells
  //void Serialize(CAR &ar);
  //void Serialize(CArchive &ar);
  //void PreSerialize(BOOL IsStoring) { }
  //void PostSerialize(BOOL IsStoring) { }

  // initiative is for combat only
  void ProcessTimeSensitiveData(int roundInc, int currInitiative=-1);

private:
  CCriticalSection m_CS;
  OrderedQueue<PENDING_SPELL, MAX_PENDING_SPELLS> m_spells;

  int GetNextKey();

  PendingSpellList(PendingSpellList &src); // not enabled
  PendingSpellList &operator=(PendingSpellList &src); // not enabled
};

extern PendingSpellList PendingSpellData;
extern ActiveSpellList ActiveSpellData;

#endif // UAFEngine

} // End of namespace DungeonCraft

#endif
