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

#ifndef DUNGEONCRAFT_SHARED_LEVEL_
#define DUNGEONCRAFT_SHARED_LEVEL

#include "dungeoncraft/shared/items.h"
#include "dungeoncraft/shared/PicData.h"
#include "dungeoncraft/shared/PicSlot.h"
#include "dungeoncraft/shared/GameEvent.h"
#include "dungeoncraft/shared/SoundMgr.h"

namespace DungeonCraft {

enum BlockageType { OpenBlk, 
                    OpenSecretBlk,
                    BlockedBlk,
                    FalseDoorBlk, //( secret + blocked )
                    LockedBlk, 
                    LockedSecretBlk, // ( secret + locked )
                    LockedWizardBlk, 
                    LockedWizardSecretBlk, // ( secret + wizard )
                    LockedKey1, LockedKey2,
                    LockedKey3, LockedKey4,
                    LockedKey5, LockedKey6,
                    LockedKey7, LockedKey8 };


// This struct is used when loading Pre-0.573 designs.
// Don't change it!
//
struct PRE_VERSION_0573_AREA_MAP_DATA 
{
  BYTE bkgrnd; // indicates blockage for overland maps
  BYTE zone;   // what zone is this spot in?
  BOOL event;  // is there a root event linked to this spot?
  BYTE northWall; // index into wall slots
  BYTE southWall;
  BYTE eastWall;
  BYTE westWall;
  BlockageType northBlockage; // blockages for all 4 sides
  BlockageType southBlockage; 
  BlockageType eastBlockage;
  BlockageType westBlockage;
};

// Used for viewport map
struct AREA_MAP_DATA 
{
  AREA_MAP_DATA() { Clear(); }
  ~AREA_MAP_DATA() { /*Clear();*/ }
  void Clear();

  // TRUE/FALSE, indicates blockage present for overland maps
  // Using high bit of this var for dungeon maps as flag
  // to indicate showdistant on/off
  BYTE bkgrnd; 

  // this flag is not serialized, instead it is stored using
  // bkgrnd above.
  BYTE ShowDistantBG; // render backgrounds in front of player
                      // or just the one where player is standing
  BYTE DistantBGInBands;

  BYTE northBG; // index into background slots
  BYTE eastBG;
  BYTE southBG;
  BYTE westBG;

  BYTE zone;   // what zone is this spot in?
  BOOL event;  // is there a root event linked to this spot?

  BYTE northWall; // index into wall slots
  BYTE southWall; // Obsolete...see below.
  BYTE eastWall;
  BYTE westWall;

  BlockageType northBlockage; // blockages for all 4 sides
  BlockageType southBlockage; // Obsolete.  See below.
  BlockageType eastBlockage;
  BlockageType westBlockage;

  BlockageType& blockages(int dir);
  BYTE& backgrounds(int dir);
  BYTE& walls(int dir);
  void SetAllBG(int slot) { northBG=eastBG=southBG=westBG=slot; }
  void Serialize(CArchive &ar, double version);
  void Serialize(CAR &ar, double version);
  void operator=(const PRE_VERSION_0573_AREA_MAP_DATA &rhs);
  AREA_MAP_DATA &operator=(const AREA_MAP_DATA &rhs);
};


enum MappingType { AreaMapping, AutoMapping, NoMapping };


class ZONE : public CObject
{
public:
   DECLARE_SERIAL( ZONE )
   ZONE();
   ~ZONE();

   void Serialize(CArchive &ar, double version);
   void Serialize(CAR &ar, double version);

   void GetEventText(char *text); 
   ZONE& operator =(const ZONE& src);
   void Clear(BOOL ctor=FALSE);

   MappingType allowMap;
   CString zoneMsg;
   CString zoneName;
   CString indoorCombatArt;
   CString outdoorCombatArt;
   PIC_DATA campArt;
   PIC_DATA treasurePicture;
   REST_EVENT restEvent;
   int summonedMonster;
   int addedTurningDifficulty;
   BOOL AllowMagic;
   BOOL AllowAutoDarken;
   BACKGROUND_SOUND_DATA bgSounds;
   ASL zone_asl;
};

class ZONE_DATA : public CObject
{
public:
  DECLARE_SERIAL( ZONE_DATA )
  ZONE_DATA() : AVArtSurf(-1) { }
  ~ZONE_DATA() {  Clear(TRUE); }
  void Serialize(CArchive &ar, double version);
  void Serialize(CAR &ar, double version);
  void SaveArt(int level);

  ZONE_DATA& operator =(const ZONE_DATA& src);
  void Clear(BOOL ctor=FALSE); 
  void ReleaseSurfaces();

  long GetAreaViewArt();
  CString AVArt;
  long AVArtSurf;
  BOOL AVArtTried;

  ZONE zones[MAX_ZONES];
};

// configures which special keys will be used for
// blockage types LockedKey1 through LockedKey8
//
class BLOCKAGE_KEYS : public CObject
{
public:
  DECLARE_SERIAL( BLOCKAGE_KEYS )
  BLOCKAGE_KEYS();
  ~BLOCKAGE_KEYS();

  void Clear();
  BLOCKAGE_KEYS& operator=(const BLOCKAGE_KEYS& src);
  void Serialize(CArchive &ar, double version);
  void Serialize(CAR &ar, double version);

  int SpecialKeys[MAX_SPECIAL_KEYS];
};

class LEVEL : public CObject
{
public:
  DECLARE_SERIAL( LEVEL )
  LEVEL();
  ~LEVEL();
  void Clear(BOOL ctor=FALSE);
  void Serialize(CArchive &ar, double version);
  void Serialize(CAR &ar, double version);
  void SaveArt(int level);
  
  int  GetCurrZone(int x, int y);
  int  GetPrevZone() { return PrevZone; }
  void SetPrevZone(int z) { PrevZone = z; }

  void IncRestEventTime(int zone, int mins);
  //void IncTimeEventTime(); // by one minute each time
  BOOL haveZoneMsg(int x, int y);
  BOOL haveEvent(int x, int y);
  BOOL haveSecretDoor(int x, int y); // in any of the 4 sides of this map square
  BOOL haveSecretDoor(int x, int y, int facing); // on particular side of map square
  BOOL haveLockedDoor(int x, int y, int facing);
  BOOL haveSpelledDoor(int x, int y, int facing);
  //BOOL haveTimeEvent(int x, int y, int day, int hour, int minute, GameEvent **stepEvent);
  BOOL haveStepEvent(int x, int y, int stepCount, GameEvent **stepEvent);
  BOOL haveRestEvent(int x, int y, GameEvent **restEvent);
  // event descriptive text for editor
  //void GetTimeEventText(int num, char *text); 
  void GetStepEventText(int num, char *text); 
  void GetRestEventText(int zone, char *text); 
  BOOL ValidAreaMapCoords(int x, int y);
  int GetUnlockSpell(int x, int y, int facing);
    
  BYTE area_height;
  BYTE area_width;
  double version;
  AREA_MAP_DATA area[MAX_AREA_HEIGHT][MAX_AREA_WIDTH]; // viewport map
  GameEventList eventData;
  ZONE_DATA zoneData;
  STEP_EVENT stepEvents[MAX_STEP_EVENTS];
  //TIME_EVENT timeEvents[MAX_TIME_EVENTS];
  BLOCKAGE_KEYS blockageKeys;
  ASL level_asl;
  int PrevZone;

  // These are used just to load/save the images used
  // for this level. There are global equivalents
  // that this data is copied to after loading, and
  // they are used for viewport rendering
  void GetSlots();
  void SetSlots();
  BackgroundSlotMemType m_BackgroundSets[MAX_BACKGROUNDS];
  WallSetSlotMemType m_WallSets[MAX_WALLSETS];
};

//extern LEVEL levelData;

BOOL loadDesign(LPCSTR name);
BOOL loadDesign(CArchive& ar);
BOOL saveDesign();
BOOL saveDesign(CArchive& ar);
void clearDesign(BOOL FillDefaults=TRUE);
BOOL CopyDesignFiles(CString newFolder);

BOOL loadLevel(LEVEL &data, int LevelIndex);
BOOL loadLevel(int levelNum, BOOL loadArt=TRUE);
BOOL saveLevel(LEVEL &data, int LevelIndex);
BOOL saveLevel(int LevelIndex);
BOOL deleteLevel(int num);

BOOL levelExists(int i);
BOOL CheckLevelForWallSlot(int slot);
BOOL CheckLevelForBackgroundSlot(int slot);

} // End of namespace DungeonCraft

#endif 
