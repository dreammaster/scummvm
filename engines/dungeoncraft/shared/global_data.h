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

#ifndef DUNGEONCRAFT_SHARED_GLOBAL_DATA
#define DUNGEONCRAFT_SHARED_GLOBAL_DATA

#include "dungeoncraft/shared/items.h"
#include "dungeoncraft/shared/char.h"
#include "dungeoncraft/shared/party.h"
#include "dungeoncraft/shared/GameEvent.h"
#include "dungeoncraft/shared/SoundMgr.h"

//#include "dungeoncraft/shared/SoundMgr.h"
//#include "dungeoncraft/shared/SharedQueue.h"
//lint -e1411

namespace DungeonCraft {

class TITLE_SCREEN
{
public:
  enum { tsSplash=0, tsFadeIn=1, tsMovie=2 };
  TITLE_SCREEN() { Clear(); }
  virtual ~TITLE_SCREEN() { Clear(); }
  void Clear()
  {
    TitleBgArt="";
    UseTrans=FALSE;
    UseBlend=FALSE;
    DisplayBy=tsFadeIn;
  }
  void Serialize(CArchive &ar);
  void Serialize(CAR &ar);
  TITLE_SCREEN& operator =(const TITLE_SCREEN& src);
  TITLE_SCREEN(const TITLE_SCREEN &src) { *this = src; }

  CString TitleBgArt;
  BOOL UseTrans;
  BOOL UseBlend;
  DWORD DisplayBy;
};

class TITLE_SCREEN_DATA
{
public:
  TITLE_SCREEN_DATA() { Clear(); }
  virtual ~TITLE_SCREEN_DATA() { Clear(); }
  void Clear() { Titles.RemoveAll();  }
  void Serialize(CArchive &ar);
  void Serialize(CAR &ar);
  TITLE_SCREEN_DATA& operator =(const TITLE_SCREEN_DATA& src);
  TITLE_SCREEN_DATA(const TITLE_SCREEN_DATA &src) { *this = src; }
  POSITION AddTitle(TITLE_SCREEN &data);
  void SetDefault(CString deftitle);

  DWORD Timeout; // zero means none
  CList<TITLE_SCREEN, TITLE_SCREEN&> Titles;
};


// lists of spells that have special
// meaning to the user interface
//
// **** Not used anymore, except when loading
// **** older designs that still have this data
// **** in the archive.
//
class SPELL_SPECIAL_DATA
{
public:
  SPELL_SPECIAL_DATA() { Clear(); }
  virtual ~SPELL_SPECIAL_DATA() { Clear(); }
  void Clear()
  {
    m_DetectMagicSpells.RemoveAll();
    m_IdentifySpells.RemoveAll();
  }
  void Serialize(CArchive &ar);
  void Serialize(CAR &ar);
  SPELL_SPECIAL_DATA& operator =(const SPELL_SPECIAL_DATA& src);
  SPELL_SPECIAL_DATA(const SPELL_SPECIAL_DATA &src) { *this = src; }

  DWORD HasDetectMagicSpell(CHARACTER &data); // return spellData key, or NO_SPELL
  DWORD HasIdentifySpell(CHARACTER &data);

  CList<DWORD, DWORD> m_DetectMagicSpells;
  CList<DWORD, DWORD> m_IdentifySpells;
};

class DIFFICULTY_DATA
{
public:
  DIFFICULTY_DATA() { Clear(); }
  virtual ~DIFFICULTY_DATA() { Clear(); }
  void Clear() 
  { 
    m_Name="";
    m_ModifyHD=m_ModifyQty=m_ModifyMonsterExp=m_ModifyAllExp=FALSE;
    m_HDAmount=m_QtyAmount=m_MExpAmount=m_AExpAmount=0;
  }
  void Serialize(CArchive &ar);
  void Serialize(CAR &ar);
  DIFFICULTY_DATA& operator =(const DIFFICULTY_DATA& src);
  DIFFICULTY_DATA(const DIFFICULTY_DATA &src) { *this = src; }

public:
  CString m_Name;
  BOOL m_ModifyHD;
  BOOL m_ModifyQty;
  BOOL m_ModifyMonsterExp;
  BOOL m_ModifyAllExp;
  char m_HDAmount;
  char m_QtyAmount;
  char m_MExpAmount;
  char m_AExpAmount;
};

class DIFFICULTY_LEVEL_DATA
{
public:
  enum { NUM_DIFF_LVLS=5 };

  DIFFICULTY_LEVEL_DATA() { Clear(); }
  virtual ~DIFFICULTY_LEVEL_DATA() { Clear(); }
  void Clear() { for (int i=0;i<NUM_DIFF_LVLS;i++) m_data[i].Clear(); m_defaultLvl=2; }
  void Serialize(CArchive &ar);
  void Serialize(CAR &ar);
  DIFFICULTY_LEVEL_DATA& operator =(const DIFFICULTY_LEVEL_DATA& src);
  DIFFICULTY_LEVEL_DATA(const DIFFICULTY_LEVEL_DATA &src) { *this = src; }
  void SetUADefaults();
  BYTE m_defaultLvl;
  DIFFICULTY_DATA m_data[NUM_DIFF_LVLS];
};


class GLOBAL_SOUND_DATA : public CObject
{
public:
  DECLARE_SERIAL( GLOBAL_SOUND_DATA )
  GLOBAL_SOUND_DATA();
  virtual ~GLOBAL_SOUND_DATA() { ClearSounds(); }
  void Clear(BOOL ctor=FALSE);
  void ClearSounds();
  void Serialize(CArchive &ar);
  void Serialize(CAR &ar);
  GLOBAL_SOUND_DATA& operator =(const GLOBAL_SOUND_DATA& src);
  GLOBAL_SOUND_DATA(const GLOBAL_SOUND_DATA &src) { *this = src; }

  int hCharHit;
  int hCharMiss;
  int hPartyBump;
  int hPartyStep;
  int hDeathMusic;
  //int hIntroMusic;
  CString CharHit;
  CString CharMiss;
  CString PartyBump;
  CString PartyStep;
  CString DeathMusic;
  BACKGROUND_SOUNDS IntroMusic;
  BACKGROUND_SOUNDS CampMusic;
};

const int MAX_QUEST_STAGE = 0xFDE9;
const int QUEST_FAILED_STAGE = 0xFDE9;
const int QUEST_COMPLETED_STAGE = 0xFDE8;

enum QuestStateType { QS_NotStarted, QS_InProg, QS_Complete, QS_Failed };

class QUEST_DATA : public CObject
{
public:
   DECLARE_SERIAL( QUEST_DATA )
   QUEST_DATA() : quest_asl("QUEST_DATA_ATTRIBUTES")
		{ Clear(); }
   virtual ~QUEST_DATA() { Clear(); }
   void Clear() { name=""; stage=0; state=QS_NotStarted; id=0;}
   void Reset() { state=QS_NotStarted; stage=0; }
   void Serialize(CArchive &ar, double ver);
   void Serialize(CAR &ar, double ver);
   QUEST_DATA& operator =(const QUEST_DATA& src);
   QUEST_DATA(const QUEST_DATA &src) 
	   :quest_asl("QUEST_DATA_ATTRIBUTES")	   
		{ *this = src; }

   CString name;
   QuestStateType state;
   WORD stage;
   int id;   
   ASL quest_asl;
};

class SPECIAL_OBJECT_DATA : public CObject
{
public:
   DECLARE_SERIAL( SPECIAL_OBJECT_DATA )
   SPECIAL_OBJECT_DATA() 
		  :obj_asl("SPECIAL_OBJECT_DATA_ATTRIBUTES"),
		  temp_asl("SPECIAL_OBJECT_DATA_ATTRIBUTES"),
      ExamineEvent(0)
		{ Clear(); }
   virtual ~SPECIAL_OBJECT_DATA() { Clear(); }
   void Clear() { CanBeDropped=FALSE;name="";id=0;stage=0;ExamineEvent=0;ExamineLabel="EXAMINE"; }
   void Serialize(CArchive &ar, double ver);
   void Serialize(CAR &ar, double ver);
	void Save(CArchive& ar);
	void PrepareForRestore(void);
	void Restore(CArchive& ar);
	void CommitRestore(void);
   SPECIAL_OBJECT_DATA& operator =(const SPECIAL_OBJECT_DATA& src);
   SPECIAL_OBJECT_DATA(const SPECIAL_OBJECT_DATA &src) 
		:obj_asl("SPECIAL_OBJECT_DATA_ATTRIBUTES")	,
		temp_asl("SPECIAL_OBJECT_DATA_ATTRIBUTES")
	   { *this = src; }
#ifdef UAFEDITOR
  CString GetSpecialObjectText();
#endif

   CString name;
   WORD stage;
   int id; 
   BOOL CanBeDropped;
   DWORD ExamineEvent;
   CString ExamineLabel;   
   ASL obj_asl;
   ASL temp_asl;
};

const int MAX_SPECIAL_OBJECTS = 0x7FFFFFFF;

class SPECIAL_OBJECT_LIST : public CObject
{
  int m_Max;
  //POSITION pos;
  CCriticalSection m_CS;
  OrderedQueue<SPECIAL_OBJECT_DATA, MAX_SPECIAL_OBJECTS> items;
  int GetNextKey();

public:

  DECLARE_SERIAL( SPECIAL_OBJECT_LIST )
  SPECIAL_OBJECT_LIST()  { Clear(); m_Max = MAX_SPECIAL_OBJECTS; }  
  SPECIAL_OBJECT_LIST(const int MaxObjects)  { Clear(); m_Max = MaxObjects; }
  ~SPECIAL_OBJECT_LIST() { Clear(); }

  void Clear() { CSingleLock sLock(&m_CS, TRUE); items.RemoveAll(); }
  int GetCount() { CSingleLock sLock(&m_CS, TRUE); return items.GetCount(); }

  POSITION GetHeadPosition() { return items.GetHeadPosition(); }
  SPECIAL_OBJECT_DATA& GetAt(POSITION p) { return items.GetAt(p); }
  SPECIAL_OBJECT_DATA& GetNext(POSITION &p) { return items.GetNext(p); }

  void Serialize(CArchive &ar, double ver);
  void Serialize(CAR &ar, double ver);
  void Save(CArchive& ar);
  void Restore(CArchive& ar);
  void CommitRestore(void);

  SPECIAL_OBJECT_LIST(const SPECIAL_OBJECT_LIST& src) { *this = src; }


  SPECIAL_OBJECT_LIST& operator=(const SPECIAL_OBJECT_LIST& src);
  BOOL HaveItem(int index);
  const CString GetName(const int index);
  void SetName(const int index, const CString &name);
  // called when loading from scripts, or
  // when adding from editor GUI
  int AddItem(SPECIAL_OBJECT_DATA &data);
  // called when loading binary monster data that
  // was saved with key info
  int AddItemWithCurrKey(SPECIAL_OBJECT_DATA &data);
  BOOL DeleteItem(const int index);
  BOOL GetItem(const int index, SPECIAL_OBJECT_DATA &item); 
  BOOL SetItem(const int index, const SPECIAL_OBJECT_DATA &item);
  WORD GetStage(const int index);
  void SetStage(const int index, const WORD stage);
  int GetItemKeyUsingSequence(int SeqNum);
  BOOL DeleteSpecialObjectEvents(int index);
  CString GetExamineLabel(int index);
  DWORD GetExamineEvent(int index);
  void SetExamineLabel(int index, CString label);
  void SetExamineEvent(int index, DWORD event);
  BOOL CanBeDropped(int index);
  void SetCanBeDropped(int index, BOOL val);
};

class QUEST_LIST : public CObject
{
  //POSITION pos;
  CCriticalSection m_CS;
  OrderedQueue<QUEST_DATA, MAX_QUESTS> quests;

  int GetNextKey();

public:
  DECLARE_SERIAL( QUEST_LIST )
  QUEST_LIST() { Clear(); }
  ~QUEST_LIST() { Clear(); }

  void Clear() { CSingleLock sLock(&m_CS, TRUE); quests.RemoveAll(); }
  int GetCount() { CSingleLock sLock(&m_CS, TRUE); return quests.GetCount(); }

  QUEST_LIST(const QUEST_LIST& src) { *this = src; }
  QUEST_LIST& operator=(const QUEST_LIST& src);

  POSITION GetHeadPosition() { return quests.GetHeadPosition(); }
  QUEST_DATA& GetAt(POSITION p) { return quests.GetAt(p); }
  QUEST_DATA& GetNext(POSITION &p) { return quests.GetNext(p); }

  void Reset(); 
  void Serialize(CArchive &ar, double ver);
  void Serialize(CAR &ar, double ver);
  BOOL HaveQuest(int index);
  const CString GetName(const int id);
  void SetName(const int id, const CString &name);
  int  FindQuest(const CString& name);
  WORD GetStage(const int id);
  WORD GetStage(const CString& questName);
  void SetStage(const int id, const WORD stage);
  void SetStage(const CString& questName, const WORD stage);
  void IncStage(const int id, const WORD value);
  BOOL IsPresent(const int id);
  BOOL IsInProgress(const int id); 
  BOOL IsComplete(const int id); 
  BOOL IsFailed(const int id); 
  void SetNotStarted(const int id);
  void SetInProgress(const int id); 
  void SetComplete(const int id); 
  void SetFailed(const int id); 
  // called when loading from scripts, or
  // when adding from editor GUI
  int AddQuest(QUEST_DATA &data);
  // called when loading binary monster data that
  // was saved with key info
  int AddQuestWithCurrKey(QUEST_DATA &data);
  BOOL DeleteQuest(const int index);
  BOOL GetQuest(const int id, QUEST_DATA &quest); 
  BOOL SetQuest(const int id, const QUEST_DATA &quest);
};

const int MAX_GLOBAL_VAULTS = 15;

class GLOBAL_VAULT_DATA : public CObject
{
public:
  DECLARE_SERIAL( GLOBAL_VAULT_DATA )
  GLOBAL_VAULT_DATA() { Clear(); }
  ~GLOBAL_VAULT_DATA() { Clear(); }

  GLOBAL_VAULT_DATA& operator =(const GLOBAL_VAULT_DATA& src);
  void Clear();
  void Serialize(CArchive &ar);
  void Serialize(CAR &ar);

  MONEY_SACK money;
  ITEM_LIST items;
};

struct ENTRY_POINT_TYPE
{
  ENTRY_POINT_TYPE() { facing=0;x=0;y=0; }
  BOOL operator ==(ENTRY_POINT_TYPE& src)
  {
    if (&src == this) return TRUE;
    if (facing != src.facing) return FALSE;
    if (x != src.x) return FALSE;
    if (y != src.y) return FALSE;
    return TRUE;
  }
  BYTE facing;
  int x;
  int y;  
};

// these settins override the area view config in each zone, and
// will be applied to the entire level
const int MAX_GLOBAL_AVSTYLES = 3;
enum GlobalAreaViewStyle { AnyView, OnlyAreaView, Only3DView };

class LEVEL_STATS : public CObject
{
public:
  DECLARE_SERIAL( LEVEL_STATS )
  LEVEL_STATS() 
	:level_asl("LEVEL_STATS_ATTRIBUTES"),
	temp_asl("LEVEL_STATS_ATTRIBUTES")
	{ Clear(); }
  ~LEVEL_STATS() { Clear(); }
  void Serialize(CArchive &ar);
  void Serialize(CAR &ar);

  void Save(CArchive& ar);
  void Restore(CArchive& ar);
  void CommitRestore(void);
  void PreSerialize(BOOL IsStoring);
  void PostSerialize(BOOL IsStoring);

  LEVEL_STATS& operator =(const LEVEL_STATS& src);
  BOOL operator ==(LEVEL_STATS& src);
  void Clear();
  void SetDefaults();

	BYTE area_height;
	BYTE area_width;
  ENTRY_POINT_TYPE entryPoints[MAX_ENTRY_POINTS];
  BOOL used;
  BOOL overland;
  GlobalAreaViewStyle AVStyle;
  CString level_name;
  CString StepSound;
  CString BumpSound;
  BACKGROUND_SOUND_DATA bgSounds;

  ASL level_asl;
  ASL temp_asl;
};

class LEVEL_INFO : public CObject
{
public:
   DECLARE_SERIAL( LEVEL_INFO )
   LEVEL_INFO() { Clear(); }
   ~LEVEL_INFO() { Clear(); }

   LEVEL_INFO& operator =(const LEVEL_INFO& src);
   void Clear(); 
   void Clear(int level); 

   void Serialize(CArchive &ar);
   void Serialize(CAR &ar);

   void Save(CArchive& ar);
   void Restore(CArchive& ar);
   void CommitRestore(void);
   void SaveSounds(void);

   int GetEPX(int ep);
   int GetEPY(int ep);
   BYTE GetEPFacing(int ep);
  
   int numLevels;
   LEVEL_STATS stats[MAX_LEVELS];   
};

enum { START_EXP_VALUE=0, START_LVL_VALUE=1 };

class GLOBAL_STATS : public CObject
{
public:
  DECLARE_SERIAL( GLOBAL_STATS )
  GLOBAL_STATS();
  ~GLOBAL_STATS() { Clear(TRUE); }
  void Clear(BOOL ctor=FALSE, BOOL npcclear=TRUE);
  void Serialize(CArchive &ar);
  void Serialize(CAR &ar);
  void Restore(CArchive& ar);
  void Save(CArchive& ar);
  void CommitRestore(void);

  GLOBAL_STATS& operator =(const GLOBAL_STATS& src);

  BOOL AddItemToVault(int which, ITEM item);
  BOOL RemoveItemFromVault(int which, int index, int qty);
  void UpdateMoneyInVault(int which, itemClassType type, int qty);
  void AddMoney(int which, MONEY_SACK &src);
  void TransferMoney(int which, MONEY_SACK &src);
  BOOL VaultHasMoney(int which=-1);
  double VaultTotal(int which=-1);

  void SaveArt();
  void SaveSounds();
  BOOL LoadSounds();
  BOOL ConfirmUsedLevels();
  BOOL LoadFonts(COLORREF green, COLORREF yellow, COLORREF red, COLORREF blue, COLORREF keyb, COLORREF BackColor, COLORREF TransColor, COLORREF Custom);
  void ReleaseFonts();
  void ReleaseSurfaces();

   double version;
   double SaveGameVersion; // gets set when saved game is loaded
   CString designName;
   int startLevel; // starting map level for new game
   int currLevel;
   BYTE startX;
   BYTE startY;
   BYTE startFacing;
   BOOL useAreaView;
   int startTime;
   int startExp;
   int startExpType; // indicates if startExp is exp or lvl value
   int UNUSED_startEquip;
   int startPlatinum;
   int startGem;
   int startJewelry;
   int minPartySize;
   int maxPartySize;
   BOOL allowCharCreate;
   int DungeonTimeDelta;
   int DungeonSearchTimeDelta;
   int WildernessTimeDelta;
   int WildernessSearchTimeDelta;
   BOOL AutoDarkenViewport;
   BOOL AutoDarkenAmount;
   int StartDarken;
   int EndDarken;
   CString MapArt;  // various area/outdoor map images
   CString IconBgArt;  // background when selecting icons
   CString BackgroundArt; // fills screen
   CString CreditsBgArt; // displayed at game exit
   LOGFONT logfont; // font used during gameplay
   long int WhiteFont;    // white characters
   long int YellowFont;   // yellow characters
   long int RedFont;      // red characters 
   long int GreenFont;    // green characters
   long int BlueFont;     // blue characters
   long int KeyboardFont; // font for keyboard shortcut characters
   long int KeyboardHighlightFont; // font for shortcuts in highlighted text
   long int HighlightFont; // highlighted characters
   long int Custom1Font;
   long int MagicAdjFont; // for stats being modified by magic
   long MapArtSurf;
   long BackgroundSurf;
   BOOL UseAVZones;
   ITEM_LIST startEquip;
   ITEM_LIST ClericEquip;
   ITEM_LIST FighterEquip;
   ITEM_LIST MagicUserEquip;
   ITEM_LIST ThiefEquip;
   ITEM_LIST RangerEquip;
   ITEM_LIST PaladinEquip;
   ITEM_LIST DruidEquip;
   GLOBAL_SOUND_DATA sounds;
   SPECIAL_OBJECT_LIST keyData;
   SPECIAL_OBJECT_LIST specialItemData;
   QUEST_LIST questData;
	 CHAR_LIST charData; // pre-gen characters are shared across all levels
   GLOBAL_VAULT_DATA vault[MAX_GLOBAL_VAULTS];
   LEVEL_INFO levelInfo;
   MONEY_DATA_TYPE moneyData;
   GameEventList eventData; // global events, npc talk, item use, etc
   DIFFICULTY_LEVEL_DATA m_diffLvlData;
   JOURNAL_DATA journalData;
   TITLE_SCREEN_DATA titleData;
   spellBookType fixSpellBook; // spells used when 'FIX' menu pressed to heal party
   ASL global_asl;
   ASL temp_asl;
};

BYTE CurrLevelWidth();
BYTE CurrLevelHeight();


void PlayCharHit(BOOL play=TRUE); 
void PlayCharMiss(BOOL play=TRUE); 
void PlayPartyStep(BOOL play=TRUE); 
void PlayPartyBump(BOOL play=TRUE); 
void PlayIntro(BOOL play=TRUE); 
void PlayDeath(BOOL play=TRUE); 
void PlayCampMusic(BOOL play=TRUE);
void CheckAndPlayBackgroundMusic(int &UsingDayMusic);

// global attribute flags for RunAs versions
enum {
  RunAsVersion=0,
  GuidedTourVersion=1,
  ItemUseEventVersion=2,
  SpecialItemKeyQtyVersion=3
};

//lint +e1411

} // End of namespace DungeonCraft

#endif