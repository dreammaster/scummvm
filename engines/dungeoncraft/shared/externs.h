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

#ifndef DUNGEONCRAFT_SHARED_EXTERNS
#define DUNGEONCRAFT_SHARED_EXTERNS

namespace DungeonCraft {

//
// This file has become the place to declarations that are
// global to the entire application, both engine and editor.
//

/////////////////////////////////////////////////////////
//#include "dungeoncraft/shared/SurfaceMgr.h"
//
// I replaced this #include with a declaration  PRS
//
enum SurfaceType;
//
/////////////////////////////////////////////////////////

// Each release gets it's own version number. In addition
// to tagging data files with the current release version,
// the code must check loaded file versions against the 
// current release version to provide backward compatibility.

// Each new version is used in at least one place, directly
// below where the PRODUCT_VER is defined. But older version 
// defines are not always used directly in the code anywhere. 
// The unused defines can be commented out after moving beyond
// the initial release which used it.
//
#define _VERSION_0500_ 0.500
#define _VERSION_0505_ 0.505
//#define _VERSION_0530_ 0.530
#define _VERSION_0560_ 0.560
//#define _VERSION_0562_ 0.562
#define _VERSION_0563_ 0.563
#define _VERSION_0564_ 0.564
//#define _VERSION_0565_ 0.565
#define _VERSION_0566_ 0.566
#define _VERSION_05661_ 0.5661
#define _VERSION_05662_ 0.5662
//#define _VERSION_05663_ 0.5663
//#define _VERSION_0567_ 0.567
//#define _VERSION_05671_ 0.5671
//#define _VERSION_0568_ 0.568
#define _VERSION_0569_ 0.569
#define _VERSION_05691_ 0.5691
//#define _VERSION_0570_ 0.570
//#define _VERSION_0571_ 0.571
#define _VERSION_0572_ 0.572
#define _VERSION_0573_ 0.573
//#define _VERSION_05731_ 0.5731
#define _VERSION_0574_ 0.574
#define _VERSION_0575_ 0.575
//#define _VERSION_05751_ 0.5751
#define _VERSION_0576_ 0.576
//#define _VERSION_0577_ 0.577
#define _VERSION_05771_ 0.5771
#define _VERSION_0600_ 0.6000
//#define _VERSION_0610_ 0.6100
#define _VERSION_0620_ 0.6200
#define _VERSION_0630_ 0.6300
#define _VERSION_0640_ 0.6400
//#define _VERSION_0641_ 0.6410
#define _VERSION_0642_ 0.6420
//#define _VERSION_0650_ 0.6500
//#define _VERSION_0651_ 0.6510
//#define _VERSION_0653_ 0.6530
//#define _VERSION_0654_ 0.6540
//#define _VERSION_0655_ 0.6550
//#define _VERSION_0656_ 0.6560
#define _VERSION_0660_ 0.6600
#define _VERSION_0661_ 0.6610
#define _VERSION_0662_ 0.6620
#define _VERSION_0670_ 0.6700
//#define _VERSION_0671_ 0.6710
//#define _VERSION_0672_ 0.6720
//#define _VERSION_0680_ 0.6800
#define _VERSION_0681_ 0.6810
#define _VERSION_0682_ 0.6820
#define _VERSION_0690_ 0.6900
//#define _VERSION_0691_ 0.6910
#define _VERSION_0692_ 0.6920
#define _VERSION_0693_ 0.6930
#define _VERSION_0694_ 0.6940
//#define _VERSION_06941_ 0.6941
#define _VERSION_0695_ 0.6950
#define _VERSION_0696_ 0.6960
//#define _VERSION_06961_ 0.6961
#define _VERSION_0697_ 0.6970
//#define _VERSION_06971_ 0.6971
#define _VERSION_0698_ 0.6980
#define _VERSION_0699_ 0.6990
#define _VERSION_06991_ 0.6991
#define _VERSION_06992_ 0.6992
//#define _VERSION_0700_ 0.7000
#define _VERSION_0710_ 0.7100
#define _VERSION_0720_ 0.7200
#define _VERSION_0730_ 0.7300
#define _VERSION_0731_ 0.7310
#define _VERSION_0740_ 0.7400
#define _VERSION_0750_ 0.7500
//#define _VERSION_0760_ 0.7600
#define _VERSION_0770_ 0.7700
//#define _VERSION_0771_ 0.7710
#define _VERSION_0780_ 0.7800
#define _VERSION_0790_ 0.7900
//#define _VERSION_0791_ 0.7910
//#define _VERSION_0792_ 0.7920
#define _VERSION_0800_ 0.8000
#define _VERSION_0810_ 0.8100
#define _VERSION_0820_ 0.8200
#define _VERSION_0830_ 0.8300
//#define _VERSION_0831_ 0.8310
#define _VERSION_0840_ 0.8400
#define _VERSION_0841_ 0.8410
#define _VERSION_0842_ 0.8420
#define _VERSION_0850_ 0.8500
#define _VERSION_0851_ 0.8510
#define _VERSION_0852_ 0.8520
#define _VERSION_0860_ 0.8600
#define _VERSION_0870_ 0.8700
//#define _VERSION_0871_ 0.8710
#define _VERSION_0880_ 0.8800
#define _VERSION_0881_ 0.8810
#define _VERSION_0882_ 0.8820
#define _VERSION_0890_ 0.8900
//#define _VERSION_0891_ 0.8910
//#define _VERSION_0892_ 0.8920
#define _VERSION_0900_ 0.9000
//#define _VERSION_0902_ 0.9020
#define _VERSION_0904_ 0.9040
#define _VERSION_0906_ 0.9060
//#define _VERSION_0907_ 0.9070
#define _VERSION_0908_ 0.9080
#define _VERSION_0909_ 0.9090
//#define _VERSION_09091_ 0.9091
#define _VERSION_0910_ 0.910
#define _VERSION_0911_ 0.911
#define _VERSION_0912_ 0.912
#define _VERSION_0913_ 0.913

// The current editor and engine versions
//
// Editor version
#define PRODUCT_VER                   _VERSION_0913_
// Engine version
#define ENGINE_VER                    _VERSION_0913_

// Some special versions
#define _ASL_LEVEL_                   _VERSION_0505_
#define _EXTENDED_ASL_LEVEL_          _VERSION_0560_  // Include flags at this version
#define _ITEM_DB_VERSION_             _VERSION_0560_
#define _SPELL_DB_VERSION_            _VERSION_0560_
#define _MONSTER_DB_VERSION_          _VERSION_0500_

// graphics
#define USE_DIRECTX
//#define USE_OPENGL  1

enum raceType;
enum genderType;
enum classType;
enum EventSourceType;
enum eventType;
extern CString GenderNames[2];

class ASL;
class CHARACTER;
struct COMBATANT;
struct QueuedCombatantData;
enum PATH_DIR;
struct COMBAT_MSGS;
class TRAITS;
class PIC_DATA;
class PASSWORD_DATA;
class FileParse;
class CAR;
extern FileParse ConfigFile;
struct PENDING_SPELL;
class SPELL_DATA;
extern int SCREEN_WIDTH;  // these change to reflect the current
extern int SCREEN_HEIGHT; // screen resolution for the editor,
extern int COLOR_BITS;

enum screenRes { res640x480=0, res800x600=1, res1024x768=2, res1152x864=3, res1280x1024=4 };
extern screenRes m_GameScreenRes;

// background colors
extern COLORREF BACKGROUND_FILL;
extern COLORREF BACKGROUND_COMBAT_FILL;

 // used by editor to display map
extern CString MAPART;
extern CString OVERLANDART;
extern CString EditorArt; // path to art used by editor


// used by engine to display area view of map
extern CString AREAVIEWART;

class PARTY;
extern PARTY party;
int GetPartySize(PARTY& party);
CString GetCharacterName(PARTY& data, int memberNumber);
genderType GetCharacterGender(PARTY& data, int memberNumber);
genderType GetCombatantGender(int memberNumber);
void InsertCharacterASL(
           PARTY& data, 
           int memberNumber,
           const CString& key,
           const CString& value,
           int flags=0);
CString LookupCharacterASL(
           PARTY& data,
           int memberNumber,
           const CString& key);
BOOL ExistsPartyASL(PARTY& data, const CString& key);
CString LookupPartyASL(
           PARTY& data,
           const CString& key);
void InsertPartyASL(
           PARTY& data, 
           const CString& key,
           const CString& value,
           int flags=0);
void DeletePartyASL(PARTY& data, const CString& key);

class LEVEL;
extern LEVEL levelData;
int GetAreaWidth(LEVEL& data);
int GetAreaHeight(LEVEL& data);
int GetAreaBlockages(LEVEL& data, int x, int y, int dir);

class GLOBAL_STATS;
extern GLOBAL_STATS globalData;
// Names of the keys used rto open doors and such.  "" if unnamed
void GetKeyNames(GLOBAL_STATS& data, CString (*names)[8]);
long int GetKeyboardFont(GLOBAL_STATS& data);
long int GetKeyboardHighlightFont(GLOBAL_STATS& data);
long int GetHighlightFont(GLOBAL_STATS& data);
double GetGameVersion(GLOBAL_STATS& data);
void SetGameVersion(GLOBAL_STATS& data, double version);
CString GetDesignName(GLOBAL_STATS& data);
void DeleteLevelASL(GLOBAL_STATS& data, int level, const CString& key);
CString LookupGlobalASL(GLOBAL_STATS& data, const CString& key);
void InsertGlobalASL(
          GLOBAL_STATS& data,
          const CString& key,
          const CString& value,
          int flags=0);
void InsertLevelASL(
          GLOBAL_STATS& data,
          int level,
          const CString& key,
          const CString& value,
          int flags=0);

void saveSounds(GLOBAL_STATS& data);
void saveArt(GLOBAL_STATS& data);

// item database filename
class ITEM_DATA_TYPE;
#define ITEM_DB_NAME "items.dat"
#define ITEM_CONFIG_NAME "items.txt"
extern ITEM_DATA_TYPE itemData;
void clearData(ITEM_DATA_TYPE& data);
int loadData(ITEM_DATA_TYPE& data, LPCSTR fullPath);
BOOL saveData(ITEM_DATA_TYPE& data);
BOOL saveData(ITEM_DATA_TYPE& data, LPCSTR fullPath);
void saveArt(ITEM_DATA_TYPE& data);
void saveSounds(ITEM_DATA_TYPE& data);

// monster database filename
class MONSTER_DATA_TYPE;
#define MONSTER_DB_NAME "monsters.dat"
#define MONSTER_CONFIG_NAME "monsters.txt"
extern MONSTER_DATA_TYPE monsterData;
void clearData(MONSTER_DATA_TYPE& data);
int loadData(MONSTER_DATA_TYPE& data, LPCSTR fullPath);
BOOL saveData(MONSTER_DATA_TYPE& data);
BOOL saveData(MONSTER_DATA_TYPE& data, LPCSTR fullPath);
void SaveSounds(MONSTER_DATA_TYPE& data);
void SaveArt(MONSTER_DATA_TYPE& data);
void saveArt(MONSTER_DATA_TYPE& data);
void saveSounds(MONSTER_DATA_TYPE& data);

// spell database filename
class SPELL_DATA_TYPE;
#define SPELL_DB_NAME "spells.dat"
#define SPELL_CONFIG_NAME "spells.txt"
int loadData(SPELL_DATA_TYPE& data, LPCSTR fullPath);
BOOL saveData(SPELL_DATA_TYPE& data);
BOOL saveData(SPELL_DATA_TYPE& data, LPCSTR fullPath);
void saveSounds(SPELL_DATA_TYPE& data);
void saveArt(SPELL_DATA_TYPE& data);
void clearData(SPELL_DATA_TYPE& data);

#ifdef UAFEngine
struct SPELL_TEXT_LIST;
extern SPELL_TEXT_LIST spellListText;
#endif

// races data filename
class RACE_DATA_TYPE;
#define RACE_DB_NAME "races.dat"
#define RACE_CONFIG_NAME "races.txt"
#define RACE_ALLOWED_CLASS_ATTRIBUTE "AllowedClass"
extern  RACE_DATA_TYPE raceData;
int  GetCount(RACE_DATA_TYPE& raceData);
int  loadData(RACE_DATA_TYPE& raceData, LPCSTR fullpath);
void loadUADefaults(RACE_DATA_TYPE& data);
BOOL saveData(RACE_DATA_TYPE& data);
BOOL saveData(RACE_DATA_TYPE& data, LPCSTR fullPath);
int  GetDataSize(RACE_DATA_TYPE& data);
void Clear(RACE_DATA_TYPE& data);

// class data filename
class CLASS_DATA_TYPE;
#define CLASS_DB_NAME "classes.dat"
#define CLASS_CONFIG_NAME "classes.txt"
extern CLASS_DATA_TYPE classData;
int  GetCount(CLASS_DATA_TYPE& classData);
BOOL saveData(CLASS_DATA_TYPE& data);
BOOL saveData(CLASS_DATA_TYPE& data, LPCSTR fullpath);
int  loadData(CLASS_DATA_TYPE& data, LPCSTR fullpath);
void loadUADefaults(CLASS_DATA_TYPE& data);
int  GetDataSize(CLASS_DATA_TYPE& data);
void Clear(CLASS_DATA_TYPE& data);

// base classes data filename
class BASE_CLASS_DATA_TYPE;
#define BASE_CLASS_DB_NAME "baseclass.dat"
#define BASE_CLASS_CONFIG_NAME "baseclass.txt"
extern BASE_CLASS_DATA_TYPE baseclassData;
int  GetCount(BASE_CLASS_DATA_TYPE& baseclassData);
BOOL saveData(BASE_CLASS_DATA_TYPE& data);
BOOL saveData(BASE_CLASS_DATA_TYPE& data, LPCSTR fullpath);
int  loadData(BASE_CLASS_DATA_TYPE& data, LPCSTR fullpath);
void loadUADefaults(BASE_CLASS_DATA_TYPE& data);
int  GetDataSize(BASE_CLASS_DATA_TYPE& data);
void Clear(BASE_CLASS_DATA_TYPE& data);

// ability data filename
class ABILITY_DATA_TYPE;
#define ABILITY_DB_NAME "ability.dat"
#define ABILITY_CONFIG_NAME "ability.txt"
typedef DWORD abilityType;
extern ABILITY_DATA_TYPE abilityData;
int  GetCount(ABILITY_DATA_TYPE& abilityData);
BOOL saveData(ABILITY_DATA_TYPE& data);
BOOL saveData(ABILITY_DATA_TYPE& data, LPCSTR fullpath);
int  loadData(ABILITY_DATA_TYPE& data, LPCSTR fullpath);
void loadUADefaults(ABILITY_DATA_TYPE& data);
int  GetDataSize(ABILITY_DATA_TYPE& data);
void Clear(ABILITY_DATA_TYPE& data);

// spellgroups data filename
class SPELLGROUP_DATA_TYPE;
#define SPELLGROUP_DB_NAME "spellgroups.dat"
#define SPELLGROUP_CONFIG_NAME "spellgroups.txt"
extern SPELLGROUP_DATA_TYPE spellgroupData;
int  GetCount(SPELLGROUP_DATA_TYPE& spellgroupData);
BOOL saveData(SPELLGROUP_DATA_TYPE& data);
BOOL saveData(SPELLGROUP_DATA_TYPE& data, LPCSTR fullpath);
int  loadData(SPELLGROUP_DATA_TYPE& data, LPCSTR fullpath);
void loadUADefaults(SPELLGROUP_DATA_TYPE& data);
int  GetDataSize(SPELLGROUP_DATA_TYPE& data);
void Clear(SPELLGROUP_DATA_TYPE& data);
 
// trait data filename
class TRAIT_DATA_TYPE;
#define TRAIT_DB_NAME "traits.dat"
#define TRAIT_CONFIG_NAME "traits.txt"
extern TRAIT_DATA_TYPE traitData;
int  GetCount(TRAIT_DATA_TYPE& traitData);
BOOL saveData(TRAIT_DATA_TYPE& data);
BOOL saveData(TRAIT_DATA_TYPE& data, LPCSTR fullpath);
int  loadData(TRAIT_DATA_TYPE& data, LPCSTR fullpath);
void loadUADefaults(TRAIT_DATA_TYPE& data);
int  GetDataSize(TRAIT_DATA_TYPE& data);
void Clear(TRAIT_DATA_TYPE& data);

// Limits imposed on a character or race or whatever.
// Packaged as a DWORD to allow easy transportation between functions.
// Inline functions to extract the pieces.
// Inline function to assemble the pieces.
//lint -e701
typedef DWORD ABILITYLIMITS;
inline int ABILITYMINBASE (ABILITYLIMITS x) { return ((x)>>24)&0xff; };
inline int ABILITYMINMOD  (ABILITYLIMITS x) { return ((x)>>16)&0xff; };
inline int ABILITYMAXBASE (ABILITYLIMITS x) { return ((x)>> 8)&0xff; };
inline int ABILITYMAXMOD  (ABILITYLIMITS x) { return ((x)>> 0)&0xff; };
inline ABILITYLIMITS ASSEMBLEABILITYLIMITS (int n, int nm, int x, int xm)
{  return (((((((n)&0xff)<<8)|((nm)&0xff))<<8)|((x)&0xff))<<8)|((xm)&0xff); };
//lint +e701


extern char m_fileDir[_MAX_PATH+1];
extern char m_installDir[_MAX_PATH+1];
extern char m_designFolder[_MAX_PATH+1];

extern BOOL bGameServer;
extern int ExitSignaled;
extern BOOL DisableScreenSaver;
extern LONGLONG virtualGameTime;

extern int ForceSysMemUsage;

// used to report errors
enum miscErrorType {
   NoError,
   UnknownError, 
   DeadChar, 
   UnconsciousChar, 
   FledChar, 
   PetrifiedChar, 
   GoneChar,
   AnimatedChar,
   TempGoneChar,
   RunningChar,
   DyingChar,
   NoMoney, 
   NotEnoughMoney,
   MaxItemsReached, 
   TooMuchWeight,
   TakesTwoHands,
   WrongClass,
   ItemAlreadyReadied,
   NoIcons,
   NoSmallPics,
   NoDesigns,
   LevelLoadError,
   InvalidGameState,
   InvalidEventType,
   InvalidMenuState,
   InvalidTaskState,
   PartyIsFull,
   NPCPartyLimitReached,
   ItemIsReadied,
   MissingMonsterCombatIcons,
   GraphicEngineError,
   DesignVersionError,
   SaveGameVersionError,
   SaveCharVersionError,
   FileLoadError,
   FileSaveError,
   DupCharName,
   NoFreeHands,
   LastMiscError 
};

extern miscErrorType miscError;
const int MAX_MISC_ERROR_MSGS = LastMiscError+1;
extern char *miscErrorText[MAX_MISC_ERROR_MSGS];

enum costFactorType { Free, 
                      Div100,Div50,Div20,Div10,Div5,Div4,Div3,Div2,Div1_5,
                      Normal,
                      Mult1_5,Mult2,Mult3,Mult4,Mult5,Mult10,Mult20,Mult50,Mult100 };
const int NUM_COST_FACTOR_TYPES = 20;

// upper left corner of viewable area, just inside the border
enum { ViewportScreenX = 14, ViewportScreenY = 16 };

// Size of one combat terrain tile
enum { COMBAT_TILE_WIDTH=48, COMBAT_TILE_HEIGHT=48 };

const int MAX_EVENTS = 0x00FFFFFF; // per level
const int MAX_SPECIAL_KEYS = 8;
const int MAX_SPECIAL_ITEMS = 0x0FFFFFFF;
const int MAX_QUESTS = 0x0FFFFFFF;
const int MAX_ZONES = 16;
const int MAX_STEP_EVENTS = 8; // per level
const int MAX_TIME_EVENTS = 8; // per level

// must be a multiple of 16!
const int MAX_WALLSETS = 96;
const int MAX_COLOR_SLOTS = MAX_WALLSETS; // should be as large as max_wallsets

const int MAX_BACKGROUNDS = 96;

#define MAXTASK 30

// object types
enum ObjectTypes { ObjectNone      = 0x00, 
                   ObjectItem      = 0x01, 
                   ObjectSpell     = 0x02, 
                   ObjectMonster   = 0x04, 
                   ObjectCharacter = 0x08,
                   ObjectAll       = 0xFF };

// special item, key, quest flags
const WORD ITEM_FLAG = 0x01;
const WORD KEY_FLAG = 0x02;
const WORD QUEST_FLAG = 0x04;

enum startEquipType { seNone, sePoor, seModest, seAverage, seProsperous, seP1, seP2, seP3, seP4 };
const int NUM_START_EQUIP_TYPES = 9;

enum undeadType { NotUndead, Skeleton, Zombie, Ghoul, Shadow, Wight, Ghast, Wraith, Mummy,
						Spectre, Vampire, Ghost, Lich, Special };
const int NUM_UNDEAD_TYPES = 14;

const int MAX_ENTRY_POINTS = 8;
const int MAX_LEVELS = 255;

// Max size for an area map
const int MAX_AREA_WIDTH = 100;
const int MAX_AREA_HEIGHT = 100;

// Max qty of non-appraised gem/jewelry
const int MAX_NON_APPRAISED_GEM_JEWELRY = INT_MAX-1;

// Size of the combat map
extern int MAX_TERRAIN_WIDTH;
extern int MAX_TERRAIN_HEIGHT;

// width of stats box on combat screen
extern int COMBAT_STAT_BOX_WIDTH;

// max player-created characters in party
const int MAX_PARTY_CHARACTERS = 6;
// max characters + 2 NPC's added using ADD_NPC event
const int MAX_PARTY_MEMBERS = 8;

int GetMaxPartyCharacters();
int GetMaxPartyMembers();
int GetMinPartyCharacters();

// Max allocated sound buffers.
// Includes all global sounds - always allocated, 
// but TS_WALK and TS_COMBAT sounds are never in memory
// at the same time.
// Also includes dynamic buffers for events.
//
const int MAX_SOUND_BUFFERS = 50;

const int DEFAULT_SUMMONED_MONSTER = 1;

// used to determine encumbrance of carried coins
//extern int COINS_PER_POUND;

// max steps the path mgr will examine before giving
// up on pathing to target
extern int MAX_STEPS;

#define EDITOR_APP_EVENT "UAFWINED_EVENT"
#define ENGINE_APP_EVENT "UAFWINENG_EVENT"

// These were moved to externs.h because so many
// files needed just these four symbols from char.h
// and monster.h.
#define CHAR_TYPE     ((BYTE)1)
#define NPC_TYPE      ((BYTE)2)
#define MONSTER_TYPE  ((BYTE)3)
#define IN_PARTY_TYPE ((BYTE)128)

const int NO_MONSTER = -1;

extern BOOL UseCustomCursor;

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
//lint -e527
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
  default: ASSERT(FALSE); return 0;
  }
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
  default: ASSERT(FALSE); return 0;
  }
}
CString ClassFlagToText(WORD types);

//lint +e527
extern char *ClassFlagText[8];

const int NO_DUDE = -1;

// must match order of eventFacingType
enum { FACE_NORTH, FACE_EAST, FACE_SOUTH, FACE_WEST,
       FACE_NW,    FACE_NE,   FACE_SW,    FACE_SE };

// starting coords of textbox
extern int textboxX;
extern int textboxY;
extern int dsgnVerX;
extern int dsgnVerY;

extern int INTER_CHARACTER_DELAY; // in millisecs

extern COLORREF White;
extern COLORREF Black;

extern COLORREF Green;
extern COLORREF Red;
extern COLORREF Blue;
extern COLORREF Yellow;

extern COLORREF CombatGreen;
extern COLORREF CombatRed;
extern COLORREF CombatYellow;
extern COLORREF CombatBlue;

extern COLORREF CustomColor;

extern COLORREF KeyboardCharColor;
extern BOOL UseKeyboardShortcuts;

extern int deltaX[4];
extern int deltaY[4];

extern HANDLE hShutdown;

extern "C" /* disable C++ name decorations */
{ 
  #define EA_NULL 0
  #define EA_ENEMY 1
  #define EA_ALLY 2
  #define EA_LOCATION 4
    
  #define FLAG_COMBAT  1
  #define FLAG_NONCOMBAT 2
  #define FLAG_XY 4
  #define FLAG_PARTY_MEMBER 8
  #define FLAG_ITEM 16
  #define FLAG_MONSTER 32
  #define FLAG_NPC 64

  struct ActorType
  {
    void Clear();
    BOOL operator==(const ActorType &src) const;
    BOOL operator!=(const ActorType &src) const;
    void Serialize(CAR &ar, double ver);
    void Serialize(CArchive &ar, double ver);
    void XYSet(unsigned short x, unsigned short y);
    void XYGet(unsigned short &x, unsigned short &y);

    void SetCombatPartySrc(BOOL friendly=TRUE);
    void SetCombatNPCSrc(BOOL friendly=FALSE);
    void SetCombatMonsterSrc(BOOL friendly=FALSE);
    
    void SetPartySrc(BOOL friendly=TRUE);
    void SetItemSrc();
    void SetNPCSrc(BOOL friendly=FALSE);
    void SetMonsterSrc(BOOL friendly=FALSE);

    BOOL IsCombatSrc();
    BOOL IsPartySrc();
    BOOL IsNPCSrc();
    BOOL IsMonsterSrc();
    BOOL IsItemSrc();

    unsigned long GetInstance();
    unsigned long GetSrc(); 
    BOOL IsValidSrc();

    unsigned long EnemyAlly;
    unsigned long Race;
    unsigned long Class;
    unsigned long Flags;
    unsigned long Instance;
    unsigned long Level;
  };
  
  extern const ActorType NULL_ACTOR;

  // de-reference context data and return indicated CHARACTER data
  CHARACTER &GetCharacterContext(ActorType X);
  // de-reference context data and return indicated COMBATANT data
  COMBATANT &GetCombatCharacterContext(ActorType X);
  // de-reference context data and return indicated target CHARACTER data
  CHARACTER &GetTargetCharacterContext(/*ActorType X*/);
  CHARACTER *GetCurrentlyActiveContext(ActorType *pActor);

  // access to global character context data
  extern void ClearCharContextStack(void);           // pop all
  extern int HaveCharContext(void);

  extern void SetCharContext(ActorType data); // push context
  extern void ClearCharContext(void);         // pop  context
  extern ActorType GetCharContext(void);      // get  context head

  // access to global target context data
  extern int HaveTargetContext(void);
  extern void SetTargetContext(ActorType data);
  extern void ClearTargetContext(void);
  extern ActorType GetTargetContext(void);

} // extern 'C'

CString GetActorDebugText(ActorType &actor);

void SpellActivate(PENDING_SPELL &data);
BOOL NeedSpellTargeting(SPELL_DATA *pData);


CHARACTER &GetPartyMember(int index);
COMBATANT *GetCombatantPtr(int index);
int InvokeSpellOnCombatant(int caster, int target, int activespellkey);

extern int net;

extern BOOL PlaySoundEffects;
extern BOOL PlayEventSounds;
extern BOOL PlayEffectsDuringEvents;

extern BOOL bActive;     // is application active?
extern BOOL UseErrorLog;
extern int LogDatabaseErrors;
extern int LogCompileErrors; // In databases
extern BOOL UseOldAVStyle;
//extern BOOL UseAVZoneStyle;

#if (defined UAFEDITOR) || (defined UAFEngine)
  void WriteDebugString(const char *text, ...);
#else
#define WriteDebugString TRACE
#endif

void seedMT(DWORD seed);
float rollDice( int sides, int numTimes, float bonus=0.0);
BOOL IsFileAlreadyOpen(const char *filename);
BOOL FileExists(const char *file);
int FileSize(const char *file);
CString GetDesignName();
int GetDesignCharStartExp();
int GetDesignCharStartExpType();
CString GetSpecialItemName(int i);
CString GetSpecialKeyName(int i);
int GetCurrentLevel();
int GetDesignStartPlatinum();
int GetDesignStartElectrum();
int GetDesignStartGold();
int GetDesignStartSilver();
int GetDesignStartCopper();
int GetDesignStartGems();
int GetDesignStartJewelry();
double GetRunAsVersion(int flag);
int GetCurrSkillLevel();
int GetAllExpMod();
int GetMonsterExpMod();
int GetMonsterQtyMod();
int GetMonsterHDMod();
void DoTaskStackTrace();
BOOL IsCombatActive();

void StripFilenamePath(CString &filename);
BOOL StripOneFolderFromFilenamePath(CString &filename, CString &path);
CString GetFilenameDrive(CString &filename);
BOOL GetFilenamePath(CString &filename, CString &path);
void GetFilenameExt(CString &filename, CString &ext);
void AddDesignFolderToPath(CString &filename);
BOOL SearchForFileInPath(CString &file, const char *FileExt, const CString &path, BOOL SearchSubfolders);
BOOL SearchForFile(CString &file);
BOOL FindImageWithValidExt(CString &filename);
BOOL FindImageWithValidExtInFolder(CString &filename, CString &folder, BOOL recurse);

BOOL CreateSaveDirectory();
BOOL CreateDesignDirectory();
BOOL CreateCommonCharDirectory();
CString GetDesignPath(char *path=NULL); // full path: 'c:\UAF\my_design.dsn'
CString GetDesignFolder(char *path=NULL); // design folder only: 'my_design.dsn'
CString GetDesignFolderRoot(char *path=NULL); // design folder root only: 'my_design'
CString GetSavePath(char *path=NULL); // saved game path: 'c:\uaf\my_design\save'
CString GetCommonCharPath(void);
CString GetEventDescription(DWORD type);
CString GetEventIdDescription(DWORD EventId, EventSourceType type);
CString GetGlobalEventIdDescription(DWORD EventId);
CString GetCopyEventIdDescription(DWORD EventId);

void EditorStatusMsg(LPCSTR text);
void SetMiscError(int error);
unsigned int GetSystemTextCharWidth();
void displayMiscError(miscErrorType error);
void displayText(int x, int y, LPCSTR text, 
                 COLORREF color=White, 
                 BOOL highlight=FALSE, 
                 BOOL slowText=FALSE, 
                 BOOL UseBackBuffer=TRUE);
int GetEntryPtX(int ep);
int GetEntryPtY(int ep);
DWORD ApplyCostFactor(costFactorType type, DWORD price);

void CreateShutdownEvent();
BOOL IsShutdownSignaled();
void CloseShutdownEvent();
void SignalShutdown();

BOOL LoadConfigFile(const char *path);
void DeleteAllFilesInFolder(const char *path);
void CloseErrorLog();
void WriteStackTrace();
void WriteValidateLog(FILE *pFile, const char *text, ... );
int backup(const CString& filename, CString bak="bak");
void BackupDesignFiles();
BOOL CopyFileToFolder(CString srcFilename, CString dstFolder, CString bak);
double GetDesignVersion(const char *fname);
BOOL SaveGameExists(int num);
BOOL LoadAreaViewArt(const char *file, long &surf, BOOL &UseAVZones);
unsigned long HashString(const char *s, const unsigned long hashsize);
BOOL IsTriggerTimeInCurrentTime(int start, int stop);

BOOL CheckDxInstall();

void MsgBoxError(const char *pMsg, const char *pTitle=NULL);
void MsgBoxInfo(const char *pMsg, const char *pTitle=NULL);
int  MsgBoxYesNo(const char *pMsg, const char *pTitle=NULL);

// used to specify which data needs to be validated by editor
const WORD VAL_ICON = 1;
const WORD VAL_SOUNDS = 2;
const WORD VAL_SPIC = 4;   // small pic 1, 2, 3
const WORD VAL_SPIC2 = 8;
const WORD VAL_SPIC3 = 16;
const WORD VAL_ALL = 0xFFFF;

struct DICEDATA
{
  DICEDATA(int n,int s,int b) {nbr=n;sides=s;bonus=b;}
  DICEDATA() {nbr=sides=bonus=0;}
  int nbr;
  int sides;
  int bonus;
};

struct CHARSKILLDATA
{
  CHARSKILLDATA() 
  { 
    str=strMod=intel=wis=dex=con=cha=0; 
    strKey=intelKey=wisKey=dexKey=conKey=chaKey=0xffffffff; 
  }
	BYTE str;
	BYTE strMod;
	BYTE intel;
	BYTE wis;
	BYTE dex;
	BYTE con;
	BYTE cha;
  abilityType strKey;
  abilityType intelKey;
  abilityType wisKey;
  abilityType dexKey;
  abilityType conKey;
  abilityType chaKey;
};

typedef CList<int,int> MAPTARGETLIST;
typedef CList<POINT,POINT&> MAPLOCATIONLIST;

#if (defined UAFEDITOR)
CString BrowseForFolder(HWND hWnd, const char *pTitle);
BOOL GetFilename(CString& defaultFilename, CString Ext="EXE", BOOL OpenDlg=TRUE);
BOOL GetFilenameWithPreview(SurfaceType type, CString& defaultFilename);
BOOL GetSoundFilename(CString& defaultFilename, CString Ext="WAV");
void OpenTextFileForEdit(const char *path);
extern int MaxDefaultArtSlots;
#else // UAFEngine only vars
extern BOOL EditorRunning;
#endif

class CUAFCommandLineInfo : public CCommandLineInfo
{
public:
  CUAFCommandLineInfo() : CCommandLineInfo() 
  { 
    m_forceAV=FALSE;
    m_ConfigFilename=""; 
    m_level=-1;
    m_sx=-1;m_sy=-1;m_facing=-1; 
    m_Party.RemoveAll();
  }
  CString m_ConfigFilename;
  int m_level;
  int m_sx;
  int m_sy;
  int m_facing;
  BOOL m_forceAV;
  CList<CString, CString&> m_Party;
  // virtual callback
  void ParseParam( LPCTSTR lpszParam, BOOL bFlag, BOOL bLast );
  
};

extern CUAFCommandLineInfo cmdLine;

// ArchiveString and DeArchiveString
//
// Empty CStrings are stored as a single
// character '*'. When loaded, the '*'
// is replaced with "" (empty string) again.
//
// The actual replacement char can be changed
// by altering ArchiveBlank. Default is '*'.
// 
// Many released versions of DC have
// used '*' so this must be supported
// in DAS regardless of what is used
// in ArchiveBlank.
//
extern CString ArchiveBlank;
#define AS(archive,cstring) \
{                           \
  if (cstring.IsEmpty())    \
    archive << ArchiveBlank;\
  else                      \
    archive << cstring;     \
}

#define DAS(archive,cstring)    \
{                               \
  archive >> cstring;         \
  if ((cstring==ArchiveBlank)||(cstring=="*"))\
    cstring="";               \
}

#ifdef UAFEngine
eventType GetCurrEventType();
void EnableUserInput(BOOL enable);
int CurrCombatTurn();
int CurrCombatRound();
int CurrCombatInitiative();
BOOL IsCurrentCombatant(int dude);
int GetUserGameSpeed();
int GetMsgTimeDelay();
void IncNumFriendFlee();
int GetNumFriendFlee();
void IncNumFriendSlain();
int GetNumFriendSlain();
void IncNumMonsterFlee();
int GetNumMonsterFlee();
void IncNumMonsterSlain();
int GetNumMonsterSlain();
int GetNumCombatants();
void GetNumCombatants(int &party, int &monsters);
BOOL GetConfigMonsterNoFlee();
BOOL GetConfigPartyAlwaysMiss();
BOOL GetConfigPartyAlwaysHit();
BOOL GetConfigMonsterAlwaysMiss();
BOOL GetConfigMonsterAlwaysHit();
BOOL GetConfigNoMagic();
BOOL GetConfigMonsterNoCast();
BOOL GetConfigMonsterNoDeath();
BOOL GetConfigPartyNeverDies();
BOOL GetConfigMonsterPlyrControl();
BOOL GetConfigMonsterNoMove();
BOOL GetConfigPartyNoExperience();
void PlayCombatMove();
void PlayCombatTurnUndead();
void PlayCombatDeath();
void PlaceCursorOnCurrentDude();
int GetCurrentRound();
QueuedCombatantData &GetQueuedCombatants();
void GetStartTerrain(int &x, int &y);
RECT &GetCombatRect();
RECT &GetFullRect();
extern COMBAT_MSGS combatMsgs;

COMBATANT *GetLastAttackerOf(COMBATANT *pDude);
COMBATANT *GetLeastDamagedFriendly();
COMBATANT *GetLeastDamagedEnemy();
COMBATANT *GetMostDamagedFriendly();
COMBATANT *GetMostDamagedEnemy();

COMBATANT *GetNearestTo(COMBATANT *pDude);
COMBATANT *GetNearestEnemyTo(COMBATANT *pDude);

#endif // ifdef UAFEngine

#if _MSC_VER == 1100     // Need CRegKey implementation explicitly in VC5
/////////////////////////////////////////////////////////////////////////////
// CRegKey
// 
// Copied from "c:\Program Files\DevStudio\VC\atl\include\atlimpl.cpp"  
//
//   PRS
LONG CRegKey::Close();
LONG CRegKey::Create(HKEY hKeyParent, LPCTSTR lpszKeyName,
	LPTSTR lpszClass, DWORD dwOptions, REGSAM samDesired,
	LPSECURITY_ATTRIBUTES lpSecAttr, LPDWORD lpdwDisposition);
LONG CRegKey::Open(HKEY hKeyParent, LPCTSTR lpszKeyName, REGSAM samDesired);
LONG CRegKey::QueryValue(DWORD& dwValue, LPCTSTR lpszValueName);
LONG CRegKey::QueryValue(LPTSTR szValue, LPCTSTR lpszValueName, DWORD* pdwCount);
LONG WINAPI CRegKey::SetValue(HKEY hKeyParent, LPCTSTR lpszKeyName, LPCTSTR lpszValue, LPCTSTR lpszValueName);
LONG CRegKey::SetKeyValue(LPCTSTR lpszKeyName, LPCTSTR lpszValue, LPCTSTR lpszValueName);
HRESULT CRegKey::SetValue(LPCTSTR lpszValue, LPCTSTR lpszValueName);

#endif // CRegKey implementaion for VC5

} // End of namespace DungeonCraft

#endif
