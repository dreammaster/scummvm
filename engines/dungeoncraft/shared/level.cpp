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

#include "dungeoncraft/shared/..\Shared\stdafx.h"


#ifdef UAFEDITOR
#include "dungeoncraft/shared/..\UAFWinEd\UAFWinEd.h"
#include "dungeoncraft/shared/..\UAFWinEd\ShellLink.h"
#include <sys\stat.h>
#include "dungeoncraft/shared/..\UAFWinEd\itemdb.h"
#else
#include "dungeoncraft/shared/externs.h"
#include "dungeoncraft/shared/..\UAFWin\Dungeon.h"
#endif

#include "dungeoncraft/shared/class.h"
#include "dungeoncraft/shared/Char.h"
#include "dungeoncraft/shared/level.h"
#include "dungeoncraft/shared/GameEvent.h"
#include "dungeoncraft/shared/spell.h"
#include "dungeoncraft/shared/Graphics.h"
#include "dungeoncraft/shared/party.h"
#include "dungeoncraft/shared/GlobalData.h"
#include "dungeoncraft/shared/FileParse.h"
#include "dungeoncraft/shared/Viewport.h"
#include "dungeoncraft/shared/SoundMgr.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_SERIAL( ZONE, CObject, 1 )
IMPLEMENT_SERIAL( ZONE_DATA, CObject, 1 )
IMPLEMENT_SERIAL( BLOCKAGE_KEYS, CObject, 1 )
IMPLEMENT_SERIAL( LEVEL, CObject, 1 )

// When saving a design the design's version is updated
// to be the most current version. Some decisions rely
// on knowing what the design version was before we updated it.
DWORD PreSaveDesignVer;

LEVEL levelData;

ZONE::ZONE() :zone_asl("ZONE_ATTRIBUTES")
{ 
  Clear(TRUE);
}

void ZONE::Serialize(CArchive &ar, double version)
{
  CObject::Serialize(ar);

  if (ar.IsStoring())
  {
    ar << summonedMonster;
    ar << addedTurningDifficulty;
    ar << (int)allowMap;
    ar << AllowMagic;
    ar << AllowAutoDarken;

    AS(ar,zoneMsg);
    AS(ar,zoneName);

    StripFilenamePath(indoorCombatArt);
    StripFilenamePath(outdoorCombatArt);
    AS(ar,indoorCombatArt);
    AS(ar,outdoorCombatArt);
    AddDesignFolderToPath(indoorCombatArt);
    AddDesignFolderToPath(outdoorCombatArt);

    bgSounds.Serialize(ar);
  
  }
  else
  {
    int temp;
    ar >> summonedMonster;
    ar >> addedTurningDifficulty;
    ar >> temp;
    allowMap = (MappingType)temp;
    if (version >= _VERSION_0660_)
      ar >> AllowMagic;
    if (version >= _VERSION_0730_)
      ar >> AllowAutoDarken;

    DAS(ar,zoneMsg);
    DAS(ar,zoneName);

    DAS(ar,indoorCombatArt);
    DAS(ar,outdoorCombatArt);
    AddDesignFolderToPath(indoorCombatArt);
    AddDesignFolderToPath(outdoorCombatArt);

    if (version >= _VERSION_0720_)
      bgSounds.Serialize(ar);
  }

  campArt.Serialize(ar, version);
  treasurePicture.Serialize(ar, version);
  restEvent.Serialize(ar);
  zone_asl.Serialize(ar);
}

void ZONE::Serialize(CAR &ar, double version)
{
  //CObject::Serialize(ar);

  if (ar.IsStoring())
  {
    ar << summonedMonster;
    ar << addedTurningDifficulty;
    ar << (int)allowMap;
    ar << AllowMagic;
    ar << AllowAutoDarken;

    AS(ar,zoneMsg);
    AS(ar,zoneName);

    StripFilenamePath(indoorCombatArt);
    StripFilenamePath(outdoorCombatArt);
    AS(ar,indoorCombatArt);
    AS(ar,outdoorCombatArt);    
    AddDesignFolderToPath(indoorCombatArt);
    AddDesignFolderToPath(outdoorCombatArt);

    bgSounds.Serialize(ar);
  }
  else
  {
    int temp;
    ar >> summonedMonster;
    ar >> addedTurningDifficulty;
    ar >> temp;
    allowMap = (MappingType)temp;
    if (version >= _VERSION_0660_)
      ar >> AllowMagic;
    if (version >= _VERSION_0730_)
      ar >> AllowAutoDarken;

    DAS(ar,zoneMsg);
    DAS(ar,zoneName);
    DAS(ar,indoorCombatArt);
    DAS(ar,outdoorCombatArt);

    if (version >= _VERSION_0720_)
      bgSounds.Serialize(ar);

    AddDesignFolderToPath(indoorCombatArt);
    AddDesignFolderToPath(outdoorCombatArt);
  }

  campArt.Serialize(ar, version);
  treasurePicture.Serialize(ar, version);
  restEvent.Serialize(ar);
  if (ar.IsStoring())
    ar << zone_asl;
  else
    ar >> zone_asl;
}

ZONE::~ZONE() 
{      
 campArt.Clear();
 treasurePicture.Clear();
 restEvent.Clear();
}
#ifdef UAFEDITOR
void ZONE::GetEventText(char *ptext) 
{ 
 sprintf(ptext, "%s: %s", 
         (LPCSTR)zoneName, 
         (LPCSTR)GetEventIdDescription(restEvent.restEvent, LevelEventSrc));
}
#endif
ZONE& ZONE::operator =(const ZONE& src)
{
 if (&src == this) return *this;
 Clear();
 summonedMonster=src.summonedMonster;
 addedTurningDifficulty=src.addedTurningDifficulty;
 allowMap=src.allowMap;
 AllowMagic=src.AllowMagic;
 AllowAutoDarken=src.AllowAutoDarken;
 zoneMsg=src.zoneMsg;
 zoneName=src.zoneName;
 indoorCombatArt=src.indoorCombatArt;
 outdoorCombatArt=src.outdoorCombatArt;
 campArt=src.campArt;
 treasurePicture=src.treasurePicture;
 restEvent=src.restEvent;
 zone_asl=src.zone_asl;
 bgSounds=src.bgSounds;
 return *this;
}

void ZONE::Clear(BOOL ctor)
{
  summonedMonster=DEFAULT_SUMMONED_MONSTER;
  addedTurningDifficulty=0;
  allowMap=AreaMapping;
  AllowMagic=TRUE;
  AllowAutoDarken=TRUE;
  zoneMsg="";
  zoneName="";
  outdoorCombatArt="";
  indoorCombatArt="";
  campArt.Clear();
  treasurePicture.Clear();
  restEvent.Clear();
  zone_asl.Clear();
  bgSounds.Clear();

#if (defined UAFEDITOR)
 if (!ctor)
 {
    indoorCombatArt.Format("%s%s", EditorArt, DEFAULT_CWD);
    outdoorCombatArt.Format("%s%s", EditorArt, DEFAULT_CWW);
    campArt.filename.Format("%s%s", EditorArt, DEFAULT_CA);
    campArt.picType = SmallPicDib;
    campArt.SetDefaults();
    treasurePicture.filename.Format("%s%s", EditorArt, DEFAULT_TR);
    treasurePicture.picType = SmallPicDib;
    treasurePicture.SetDefaults();
 }
#endif
}

///////////////////////////////////////////////////////////////////////

ZONE_DATA& ZONE_DATA::operator =(const ZONE_DATA& src)
{
  if (&src == this)
    return *this;
  Clear();
  for (int i=0;i<MAX_ZONES;i++) 
    zones[i] = src.zones[i];
  AVArt=src.AVArt;
  AVArtSurf=src.AVArtSurf;
  AVArtTried=src.AVArtTried;
  if (!GraphicsMgr.ValidSurface(AVArtSurf))
    ReleaseSurfaces();
  return *this;
}

void ZONE_DATA::Clear(BOOL ctor) 
{ 
  ReleaseSurfaces();
  AVArt="";  
  for (int i=0;i<MAX_ZONES;i++) 
  {
    zones[i].Clear(ctor);
    zones[i].zoneName.Format("Zone %i", i+1);
  }
#if (defined UAFEDITOR)
 if (!ctor)
    AVArt.Format("%s%s", EditorArt, DEFAULT_ZONEAVART);
#else
 if (!ctor)
    AVArt = DEFAULT_ZONEAVART;
#endif

}

void ZONE_DATA::ReleaseSurfaces()
{
  if (AVArtSurf >= 0)
    GraphicsMgr.ReleaseSurface(AVArtSurf);
  AVArtSurf=-1;
  AVArtTried=FALSE;
}

long ZONE_DATA::GetAreaViewArt()
{
  if ((AVArtTried) || (AVArt.IsEmpty()))
    return -1;

  if ((AVArtSurf <= 0)||(!GraphicsMgr.ValidSurface(AVArtSurf)))
  {
    AVArtSurf = GraphicsMgr.AddSurface(AVArt, TransBufferDib);

    if (!GraphicsMgr.ValidSurface(AVArtSurf))
    {
      AVArtTried=TRUE;
      WriteDebugString("Failed to load area view art \'%s\' in ZONE_DATA::GetAreaViewArt\n", AVArt);
    }
  }
  return AVArtSurf;
}

void ZONE_DATA::Serialize(CArchive &ar, double version)
{
  CObject::Serialize(ar);
  int i;
  if (ar.IsStoring())
  {
    ar << MAX_ZONES;
    for (i=0;i<MAX_ZONES;i++)
      zones[i].Serialize(ar, version);
    AS(ar, AVArt);
  }
  else
  {
    Clear();
    if (version >= _VERSION_05661_)
    {
      int count;
      ar >> count;
      for (i=0;i<count;i++)
      {
        zones[i].Clear();
        zones[i].Serialize(ar, version);
      }
    }
    else
    {
      for (i=0;i<8;i++) // load old quantity of zones (= 8)
      {
        zones[i].Clear();
        zones[i].Serialize(ar, version);
      }
    }
    if (version >= _VERSION_0731_)
      DAS(ar, AVArt);
  }
}
void ZONE_DATA::Serialize(CAR &ar, double version)
{
  //CObject::Serialize((CArchive&)ar);
  int i;
  if (ar.IsStoring())
  {
    ar << MAX_ZONES;
    for (i=0;i<MAX_ZONES;i++)
      zones[i].Serialize(ar, version);
    AS(ar, AVArt);
  }
  else
  {
    Clear();
    int count;
    ar >> count;
    for (i=0;i<count;i++)
    {
      zones[i].Clear();
      zones[i].Serialize(ar, version);
    }
    if (version >= _VERSION_0731_)
      DAS(ar, AVArt);
  }
}

void ZONE_DATA::SaveArt(int level)
{
  saveArt(AVArt, CombatDib, level);

  for (int i=0;i<MAX_ZONES; i++)
  {    
    saveArt(zones[i].indoorCombatArt, CombatDib, level);
    saveArt(zones[i].outdoorCombatArt, CombatDib, level);
    zones[i].campArt.SavePicData(level);  
    zones[i].treasurePicture.SavePicData(level);
    zones[i].bgSounds.backgroundSounds.SaveSounds(level);
    zones[i].bgSounds.nightSounds.SaveSounds(level);
  }
}

///////////////////////////////////////////////////////////////////////
void AREA_MAP_DATA::Serialize(CAR &ar, double ver)
{
  if (ar.GetCompression()==0)
  {
    Serialize((CArchive&)ar, ver);
    return;
  }

  if (ar.IsStoring())
  {
    if (ShowDistantBG) bkgrnd |= 0x80;
    if (DistantBGInBands) bkgrnd |= 0x40;
    ar << bkgrnd;
    bkgrnd &= 0x3F;
    ar << northBG;
    ar << eastBG;
    ar << southBG;
    ar << westBG;
    ar << zone;
    ar << (BYTE)event;
    ar << northWall;
    ar << southWall;
    ar << eastWall;
    ar << westWall;
    ar << (BYTE)northBlockage;
    ar << (BYTE)southBlockage;
    ar << (BYTE)eastBlockage;
    ar << (BYTE)westBlockage;
  }
  else
  {
    BYTE tmp;
    ar >> bkgrnd;
    if (bkgrnd & 0x80) ShowDistantBG = TRUE;
    if (bkgrnd & 0x40) DistantBGInBands = TRUE;
    bkgrnd &= 0x3F;

    if ((ver < _VERSION_0695_) && (ShowDistantBG))
      DistantBGInBands=TRUE;

    if (ver >= _VERSION_05771_)
    {
      ar >> northBG;
      ar >> eastBG;
      ar >> southBG;
      ar >> westBG;
    }
    else
    {
      northBG = bkgrnd;
      eastBG = bkgrnd;
      southBG = bkgrnd;
      westBG = bkgrnd;
    }

    ar >> zone;
    ar >> tmp;
    event = tmp;
    ar >> northWall;
    ar >> southWall;
    ar >> eastWall;
    ar >> westWall;
    ar >> tmp; 
    northBlockage = (BlockageType)tmp;
    ar >> tmp; 
    southBlockage = (BlockageType)tmp;
    ar >> tmp; 
    eastBlockage = (BlockageType)tmp;
    ar >> tmp; 
    westBlockage = (BlockageType)tmp;
  }
}

// the compression archive falls back to this if compression is turned off
void AREA_MAP_DATA::Serialize(CArchive &ar, double ver)
{
  if (ar.IsStoring())
  {
    if (ShowDistantBG) bkgrnd |= 0x80; // set high bit
    if (DistantBGInBands) bkgrnd |= 0x40;
    ar << bkgrnd;
    bkgrnd &= 0x3F;

    ar << northBG;
    ar << eastBG;
    ar << southBG;
    ar << westBG;
    ar << zone;
    ar << (BYTE)event;
    ar << northWall;
    ar << southWall;
    ar << eastWall;
    ar << westWall;
    ar << (BYTE)northBlockage;
    ar << (BYTE)southBlockage;
    ar << (BYTE)eastBlockage;
    ar << (BYTE)westBlockage;
  }
  else
  {
    BYTE tmp;
    ar >> bkgrnd;
    if (bkgrnd & 0x80) ShowDistantBG = TRUE;
    if (bkgrnd & 0x40) DistantBGInBands = TRUE;
    bkgrnd &= 0x3F;
    if ((ver < _VERSION_0695_) && (ShowDistantBG))
      DistantBGInBands=TRUE;

    if (ver >= _VERSION_05771_)
    {
      ar >> northBG;
      ar >> eastBG;
      ar >> southBG;
      ar >> westBG;
    }
    else
    {
      northBG=bkgrnd;
      eastBG=bkgrnd;
      southBG=bkgrnd;
      westBG=bkgrnd;
    }

    ar >> zone;
    ar >> tmp;
    event = tmp;
    ar >> northWall;
    ar >> southWall;
    ar >> eastWall;
    ar >> westWall;
    ar >> tmp; 
    northBlockage = (BlockageType)tmp;
    ar >> tmp; 
    southBlockage = (BlockageType)tmp;
    ar >> tmp; 
    eastBlockage = (BlockageType)tmp;
    ar >> tmp; 
    westBlockage = (BlockageType)tmp; 
  }
}

void AREA_MAP_DATA::Clear()
{
  ShowDistantBG = FALSE;
  DistantBGInBands=FALSE;
  bkgrnd = 0; zone = 0; event = FALSE;
  for (int i=0; i<4; i++)
  {
    backgrounds(i)=0;
    blockages(i)=OpenBlk;
    walls(i)=0;
  };
}

void AREA_MAP_DATA::operator=(const PRE_VERSION_0573_AREA_MAP_DATA &rhs)
{
  // convert old map data into new format
  bkgrnd = rhs.bkgrnd;  
  zone = rhs.zone;
  event = rhs.event;
  northWall = rhs.northWall;
  southWall = rhs.southWall;
  eastWall = rhs.eastWall;
  westWall = rhs.westWall;
  northBlockage = rhs.northBlockage;
  southBlockage = rhs.southBlockage;
  eastBlockage = rhs.eastBlockage;
  westBlockage = rhs.westBlockage;

  // this data is not present in pre-0573 AREA_MAP_DATA
  northBG = rhs.bkgrnd;
  eastBG = rhs.bkgrnd;
  southBG = rhs.bkgrnd;
  westBG = rhs.bkgrnd;
  ShowDistantBG = FALSE;
  DistantBGInBands=TRUE;
}

AREA_MAP_DATA &AREA_MAP_DATA::operator=(const AREA_MAP_DATA &rhs)
{
  if (&rhs==this) return *this;
  bkgrnd=rhs.bkgrnd; 
  ShowDistantBG=rhs.ShowDistantBG;
  DistantBGInBands=rhs.DistantBGInBands;
  northBG=rhs.northBG;
  eastBG=rhs.eastBG;
  southBG=rhs.southBG;
  westBG=rhs.westBG;
  zone=rhs.zone;
  event=rhs.event;
  northWall=rhs.northWall;
  southWall=rhs.southWall;
  eastWall=rhs.eastWall;
  westWall=rhs.westWall;
  northBlockage=rhs.northBlockage;
  southBlockage=rhs.southBlockage;
  eastBlockage=rhs.eastBlockage;
  westBlockage=rhs.westBlockage;
  return *this;
}

BYTE& AREA_MAP_DATA::walls(int dir)
{
  static int room_rearranger[4]= 
  { 
    (&northWall)-(&northWall),
    (&eastWall) -(&northWall),
    (&southWall)-(&northWall),
    (&westWall) -(&northWall)
  };
  // Adding 0x10000000 would make it work on 1's-complement machines
  return *((&northWall)+room_rearranger[dir&3]);
}

BlockageType& AREA_MAP_DATA::blockages(int dir)
{
  static int room_rearranger[4]= 
  { 
    (&northBlockage)-(&northBlockage),
    (&eastBlockage) -(&northBlockage),
    (&southBlockage)-(&northBlockage),
    (&westBlockage) -(&northBlockage)
  };
  // Adding 0x10000000 would make it work on 1's-complement machines
  return *((&northBlockage)+room_rearranger[dir&3]);
}

BYTE& AREA_MAP_DATA::backgrounds(int dir)
{
  static int room_rearranger[4]= 
  { 
    (&northBG)-(&northBG),
    (&eastBG) -(&northBG),
    (&southBG)-(&northBG),
    (&westBG) -(&northBG)
  };
  // Adding 0x10000000 would make it work on 1's-complement machines
  return *((&northBG)+room_rearranger[dir&3]);
}

///////////////////////////////////////////////////////////////////////

BLOCKAGE_KEYS::BLOCKAGE_KEYS() 
{ 
  Clear(); 
}

BLOCKAGE_KEYS::~BLOCKAGE_KEYS() { }

void BLOCKAGE_KEYS::Clear() 
{ 
  memset(SpecialKeys, 0, sizeof(SpecialKeys)); 
}

BLOCKAGE_KEYS& BLOCKAGE_KEYS::operator=(const BLOCKAGE_KEYS& src)
{
  if (this==&src) return *this;
  Clear();
  for (int i=0;i<MAX_SPECIAL_KEYS;i++)
    SpecialKeys[i]=src.SpecialKeys[i];
  return *this;
}  

void BLOCKAGE_KEYS::Serialize(CArchive &ar, double version)
{
  if (ar.IsStoring())
  {
    ar << MAX_SPECIAL_KEYS;
    for (int i=0;i<MAX_SPECIAL_KEYS;i++)
      ar << SpecialKeys[i];
  }
  else
  {
    Clear();
    int count;
    ar >> count;
    for (int i=0;i<count;i++)
      ar >> SpecialKeys[i];
  }
}

void BLOCKAGE_KEYS::Serialize(CAR &ar, double version)
{
  if (ar.IsStoring())
  {
    ar << MAX_SPECIAL_KEYS;
    for (int i=0;i<MAX_SPECIAL_KEYS;i++)
      ar << SpecialKeys[i];
  }
  else
  {
    Clear();
    int count;
    ar >> count;
    for (int i=0;i<count;i++)
      ar >> SpecialKeys[i];
  }
}

///////////////////////////////////////////////////////////////////////

void LEVEL::Serialize(CArchive &ar, double ver)
{
  CObject::Serialize(ar);
  int x;  

  if (ar.IsStoring())
  {  
    ASSERT(FALSE); // shouldn't get here, use CAR instead
  }
  else
  {
    PRE_VERSION_0573_AREA_MAP_DATA tempAREA[50][50]; // old size
    ar.Read(tempAREA, sizeof(tempAREA)); // old size
    // copy to new format
    for (int h=0;h<50;h++)
      for (int w=0;w<50;w++)
        area[h][w] = tempAREA[h][w]; // operator=() performs translation
  }

  eventData.m_level = globalData.currLevel;
  eventData.SetEventSource(LevelEventSrc);
  eventData.Serialize(ar, ver);
  eventData.SetEventSource(LevelEventSrc);
  zoneData.Serialize(ar, ver);
  level_asl.Serialize(ar);

  for (x=0;x<MAX_STEP_EVENTS;x++)
     stepEvents[x].Serialize(ar, ver);

  // won't be here unless loading pre-0.5772 data
  // older level files always have 16 wall slots
  for (x=0; x<16; x++)
    m_WallSets[x].Serialize(ar, ver);

  for (x=0; x<16; x++)
     m_BackgroundSets[x].Serialize(ar, ver);
}

void LEVEL::Serialize(CAR &ar, double ver)
{
  //CObject::Serialize((CArchive&)ar);
  int x;

  if (ar.IsStoring())
  {  
    ar << area_width;
    ar << area_height;
    for (int h=0;h<area_height;h++)
    {
      for (int w=0;w<area_width;w++)
      {
        //ar << area[h][w]; // operator<< defined in class.cpp
        area[h][w].Serialize(ar, ver);
      }
    }
  }
  else
  {
    ar >> area_width;
    ar >> area_height;
    for (int h=0;h<area_height;h++)
    {
      for (int w=0;w<area_width;w++)
      {
        //ar >> area[h][w];
        area[h][w].Serialize(ar, ver);
      }
    }
  }

  eventData.m_level = globalData.currLevel;
  eventData.SetEventSource(LevelEventSrc);
  ASSERT( (globalData.currLevel >= 0) && (globalData.currLevel < MAX_LEVELS) );
  eventData.Serialize(ar, ver);
  eventData.SetEventSource(LevelEventSrc);
  zoneData.Serialize(ar, ver);
  if (ar.IsStoring())
    ar << level_asl;
  else
    ar >> level_asl;

  for (x=0;x<MAX_STEP_EVENTS;x++)
     stepEvents[x].Serialize(ar, ver);

  int wall_count;
  if (ar.IsStoring())
  {
    // always store all wall slots
    wall_count = MAX_WALLSETS;
    ar << wall_count;
    for (x=0; x<wall_count; x++)
      m_WallSets[x].Serialize(ar, ver);

    wall_count = MAX_BACKGROUNDS;
    ar << wall_count;
    for (x=0; x<wall_count; x++)
      m_BackgroundSets[x].Serialize(ar, ver);

    // set to default special keys if index slot is empty
    for (int i=0;i<MAX_SPECIAL_KEYS;i++)
    {
      if (blockageKeys.SpecialKeys[i]<=0)
        blockageKeys.SpecialKeys[i] = globalData.keyData.GetItemKeyUsingSequence(i+1);
    }

    blockageKeys.Serialize(ar, ver);
  }
  else
  {
    // older levels just stored 16 wall slots
    if (ver < _VERSION_0600_)
    {
      for (x=0; x<16; x++)
        m_WallSets[x].Serialize(ar, ver);
    }
    else
    {
      ar >> wall_count;
      ASSERT( wall_count <= MAX_WALLSETS );
      wall_count = min(wall_count, MAX_WALLSETS);
      for (x=0; x<wall_count; x++)
        m_WallSets[x].Serialize(ar, ver);
    }

    // older levels just stored 16 background slots
    if (ver < _VERSION_0660_)
    {
      for (x=0; x<16; x++)
        m_BackgroundSets[x].Serialize(ar, ver);
    }
    else
    {
      ar >> wall_count;
      ASSERT( wall_count <= MAX_BACKGROUNDS );
      wall_count = min(wall_count, MAX_BACKGROUNDS);
      for (x=0; x<wall_count; x++)
        m_BackgroundSets[x].Serialize(ar, ver);
    }

    if (ver >= _VERSION_0842_)
    {
      blockageKeys.Serialize(ar, ver);
    }
    else
    {   
      // global data is loaded prior to level data
      for (int i=0;i<MAX_SPECIAL_KEYS;i++)
        blockageKeys.SpecialKeys[i] = globalData.keyData.GetItemKeyUsingSequence(i+1);
    }
  }
}


void LEVEL::Clear(BOOL ctor)
{
   int i;   
   area_height = 0;
   area_width = 0;
   version = 0.0;
   memset(area, 0, sizeof(area));
   eventData.Clear();
   eventData.SetEventSource(LevelEventSrc);
   for (i=0;i<MAX_ZONES;i++)
    eventData.DeleteEvents(zoneData.zones[i].restEvent.restEvent);
   zoneData.Clear(ctor);
   blockageKeys.Clear();
   for (i=0;i<MAX_STEP_EVENTS;i++)
   {
     eventData.DeleteEvents(stepEvents[i].stepEvent);
     stepEvents[i].Clear();
   }

   level_asl.Clear();
   SetPrevZone(-1);

   int x;
   for (x=0; x<MAX_WALLSETS; x++)
     m_WallSets[x].Clear();

   for (x=0; x<MAX_BACKGROUNDS; x++)
     m_BackgroundSets[x].Clear();
}

void LEVEL::SaveArt(int level)
{
  eventData.saveUsedEventArt(level);
  zoneData.SaveArt(level);
  eventData.saveUsedEventSounds(level);

  int x;
  for (x=0; x<MAX_WALLSETS; x++)
  {
    m_WallSets[x].SaveArt(level);
    m_WallSets[x].SaveSound(level);
  }

  for (x=0; x<MAX_BACKGROUNDS; x++)
  {
     m_BackgroundSets[x].SaveArt(level);
     m_BackgroundSets[x].SaveSound(level);
  }
}

// load global slots into level slots
void LEVEL::GetSlots()
{
  int x;
  for (x=0; x<MAX_WALLSETS; x++)
    m_WallSets[x]=WallSets[x];

  for (x=0; x<MAX_BACKGROUNDS; x++)
     m_BackgroundSets[x]=BackgroundSets[x];
}

// load level slots into global slots
void LEVEL::SetSlots()
{
  ASSERT( this == &levelData );
  int x;
  for (x=0; x<MAX_WALLSETS; x++)
    WallSets[x]=m_WallSets[x];

  for (x=0; x<MAX_BACKGROUNDS; x++)
    BackgroundSets[x]=m_BackgroundSets[x];
}

LEVEL::LEVEL() :level_asl("LEVEL_ATTRIBUTES")
{ 
 area_height = 0;
 area_width = 0;
 memset(area, 0, sizeof(area));
 Clear(TRUE);
}
LEVEL::~LEVEL() 
{ 
  Clear(TRUE); 
}
int  LEVEL::GetCurrZone(int x, int y) 
{ 
  ASSERT(ValidAreaMapCoords(x,y));
  ASSERT((area[y][x].zone >=0)&&(area[y][x].zone<MAX_ZONES));
  return area[y][x].zone; 
}

void LEVEL::IncRestEventTime(int zone, int mins)
{
  if (eventData.IsValidEvent(zoneData.zones[zone].restEvent.restEvent))
    zoneData.zones[zone].restEvent.prevMinChecked += mins;
}
/*
void LEVEL::IncTimeEventTime()
{
  for (int i=0; i<MAX_TIME_EVENTS; i++)
  {
    if (eventData.IsValidEvent(timeEvents[i].timeEvent))
    {
      int mn, hr, day;
      
      day = 0;
      hr = 0;
      mn = 1;
       
      timeEvents[i].minuteCounter += mn;
      
      if (timeEvents[i].minuteCounter >= 60)
      { 
        timeEvents[i].minuteCounter -= 60;
        hr++;
      }
      
      timeEvents[i].hourCounter += hr;
      
      if (timeEvents[i].hourCounter >= 24)
      {
        timeEvents[i].hourCounter -= 24;
        timeEvents[i].dayCounter++;
      }
    }
  }
}
*/

BOOL LEVEL::haveZoneMsg(int x, int y)
{ 
  return (strlen(zoneData.zones[GetCurrZone(x,y)].zoneMsg) > 0);
}

BOOL LEVEL::haveEvent(int x, int y)
{
  ASSERT(ValidAreaMapCoords(x,y));
  return (area[y][x].event);
}

BOOL LEVEL::haveSecretDoor(int x, int y)
{
  //AREA_MAP_DATA data = area[y][x];
  for (int i=0;i<4;i++)
  {
    if (haveSecretDoor(x,y,i))
      return TRUE;
    //BOOL isSecret=party.blockageData.IsSecret(GetCurrentLevel(),x,y,i);
    //if ((data.blockages(i)==OpenSecretBlk) && (isSecret)) return TRUE;
    //if ((data.blockages(i)==LockedSecretBlk) && (isSecret)) return TRUE;
    //if ((data.blockages(i)==LockedWizardSecretBlk) && (isSecret)) return TRUE;
  }
  return FALSE;
}

BOOL LEVEL::haveSecretDoor(int x, int y, int facing)
{
  ASSERT(ValidAreaMapCoords(x,y));  
  AREA_MAP_DATA data = area[y][x];
  BOOL isSecret=party.blockageData.IsSecret(GetCurrentLevel(),x,y,facing);
  if (!isSecret) return FALSE;
  if (data.blockages(facing)==OpenSecretBlk) return TRUE;
  if (data.blockages(facing)==LockedSecretBlk) return TRUE;
  if (data.blockages(facing)==LockedWizardSecretBlk) return TRUE;
  return FALSE;
}

BOOL LEVEL::haveLockedDoor(int x, int y, int facing)
{
  // only care about locked doors that need (or can be) bashed, so
  // ignore LockedKey1, etc.
  //
  ASSERT(ValidAreaMapCoords(x,y));
  AREA_MAP_DATA data = area[y][x];
  if (!party.blockageData.IsLocked(GetCurrentLevel(),x,y,facing)) return FALSE;
  BOOL isSecret=party.blockageData.IsSecret(GetCurrentLevel(),x,y,facing);
  BOOL isSpelled=party.blockageData.IsSpelled(GetCurrentLevel(),x,y,facing);
  if (data.blockages(facing)==LockedBlk) return TRUE;
  if ((!isSecret) && (data.blockages(facing)==LockedSecretBlk)) return TRUE;
  if ((!isSpelled) && (data.blockages(facing)==LockedWizardBlk)) return TRUE;
  if ((!isSecret) && (!isSpelled) && (data.blockages(facing)==LockedWizardSecretBlk)) return TRUE;
  return FALSE;
}

BOOL LEVEL::haveSpelledDoor(int x, int y, int facing)
{
  ASSERT(ValidAreaMapCoords(x,y));
  AREA_MAP_DATA data = area[y][x];
  
  if (!party.blockageData.IsSpelled(GetCurrentLevel(),x,y,facing)) return FALSE;
  BOOL isSecret=party.blockageData.IsSecret(GetCurrentLevel(),x,y,facing);
  if (data.blockages(facing)==LockedWizardBlk) return TRUE;
  if ((!isSecret) && (data.blockages(facing)==LockedWizardSecretBlk)) return TRUE;
  return FALSE;
}

int LEVEL::GetUnlockSpell(int x, int y, int facing)
{
  ASSERT(ValidAreaMapCoords(x,y));
  AREA_MAP_DATA data = area[y][x];
  int spell = -1;
  switch (facing)
  {
  case FACE_NORTH:
    spell = WallSets[data.northWall].UnlockSpell;
    break;
  case FACE_EAST:
    spell = WallSets[data.eastWall].UnlockSpell;
    break;
  case FACE_SOUTH:
    spell = WallSets[data.southWall].UnlockSpell;
    break;
  case FACE_WEST:
    spell = WallSets[data.westWall].UnlockSpell;
    break;
  }
  return spell;
}

/*
BOOL LEVEL::haveTimeEvent(int x, int y, int day, int hour, int minute, GameEvent **timeEvent)
{
  BOOL TE_Found = FALSE;
  if (timeEvent == NULL) return FALSE;
  
  // scan for first time event that should trigger
  for (int i=0; (i<MAX_TIME_EVENTS) && (!TE_Found); i++)
  {
    switch (timeEvents[i].triggerType)
    {
    case TIME_EVENT::ABSOLUTE_TIME:
      {
        if (   (day == timeEvents[i].day)
            && (hour == timeEvents[i].hour)
            && (minute == timeEvents[i].minute))
        {
          // if event data is present...
          if (eventData.IsValidEvent(timeEvents[i].timeEvent))
          {
            *timeEvent = eventData.GetEvent(timeEvents[i].timeEvent);
            TE_Found=TRUE;
          }
        }
      }
      break;
    case TIME_EVENT::REPEAT_TIME:
      {        
        if (   (timeEvents[i].day > 0)
            || (timeEvents[i].hour > 0)
            || (timeEvents[i].minute > 0))
        {
          // at least one of them is configured with a trigger
          BOOL dayTrigger = TRUE;
          BOOL hourTrigger = TRUE;
          BOOL minuteTrigger = TRUE;

          if (timeEvents[i].day > 0)
            dayTrigger = ((timeEvents[i].day % day) == 0);

          if (timeEvents[i].hour > 0) // change hour to 1-24 (from 0-23)
            hourTrigger = ((timeEvents[i].hour % (hour+1)) == 0);

          if (timeEvents[i].minute > 0) // change minute to 1-60 (from 0-59)
            minuteTrigger = ((timeEvents[i].minute % (minute+1)) == 0);

          if ( dayTrigger && hourTrigger && minuteTrigger )
          {
            // if event data is present...
            if (eventData.IsValidEvent(timeEvents[i].timeEvent))
            {
              *timeEvent = eventData.GetEvent(timeEvents[i].timeEvent);
              TE_Found=TRUE;
            }
          }
        }
      }
      break;
    }
  }
  
  return TE_Found;
}
*/

BOOL LEVEL::haveStepEvent(int x, int y, int stepCount, GameEvent **stepEvent)
{
  BOOL SE_Found = FALSE;
  int zone = GetCurrZone(x,y);
  if (stepEvent == NULL) return FALSE;
  // scan for first step event that should trigger
  for (int i=0; (i<MAX_STEP_EVENTS) && (!SE_Found); i++)
  {
    // if this zone can trigger and count is multiple of step count
    if (   (stepEvents[i].stepTrigger[zone] > 0) // does this zone trigger?
        && (stepEvents[i].stepCount > 0)         // must have non-zero trigger count
        && ((stepCount % stepEvents[i].stepCount) == 0) )
    {
      // if event data is present...
      if (eventData.IsValidEvent(stepEvents[i].stepEvent))
      {
        *stepEvent = eventData.GetEvent(stepEvents[i].stepEvent);
        SE_Found=TRUE;
      }
    }
  }

  return SE_Found;
}

BOOL LEVEL::haveRestEvent(int x, int y, GameEvent **restEvent)
{
  BOOL RE_Found = FALSE;

  int zone = GetCurrZone(x, y);
  if (restEvent == NULL) return FALSE;

  if (   (eventData.IsValidEvent(zoneData.zones[zone].restEvent.restEvent))
      && (zoneData.zones[zone].restEvent.everyMin > 0)
      && (zoneData.zones[zone].restEvent.chance > 0))
  {
    if (zoneData.zones[zone].restEvent.prevMinChecked >= zoneData.zones[zone].restEvent.everyMin)
    {
      zoneData.zones[zone].restEvent.prevMinChecked = 0;
      int result = rollDice(100, 1, 0);
    
      WriteDebugString("Rest event chance %i, result %i\n",
                       zoneData.zones[zone].restEvent.chance, result);
    
      if (result <= zoneData.zones[zone].restEvent.chance)
      {
        *restEvent = eventData.GetEvent(zoneData.zones[zone].restEvent.restEvent);
        RE_Found=TRUE;
      }
    }
  }

  return RE_Found;
}
#ifdef UAFEDITOR
// event descriptive text for editor
/*
void LEVEL::GetTimeEventText(int num, char *ptext) 
{ 
  if ((num >= 0) && (num < MAX_TIME_EVENTS))
  {
    sprintf(ptext, "Time Event %u: %s", 
      num+1, 
      (LPCSTR)GetEventIdDescription(timeEvents[num].timeEvent, LevelEventSrc));
  }
}
*/
void LEVEL::GetStepEventText(int num, char *ptext) 
{ 
  if ((num >= 0) && (num < MAX_STEP_EVENTS))
  {
    sprintf(ptext, "Step Event %u: %s", 
            num+1, 
            (LPCSTR)GetEventIdDescription(stepEvents[num].stepEvent, LevelEventSrc));
  }
}
void LEVEL::GetRestEventText(int zone, char *ptext) 
{ 
  if ((zone >= 0) && (zone < MAX_ZONES))
  {
    zoneData.zones[zone].GetEventText(ptext);
  }
}
#endif
BOOL LEVEL::ValidAreaMapCoords(int x, int y)
{
  return ((x >= 0) && (x < area_width) && (y >= 0) && (y < area_height));
}

///////////////////////////////////////////////////////////////////////


BOOL saveLevel(LEVEL &data, int LevelIndex)
{
   BOOL success = FALSE;  

   CFile myFile;
   char fullPath[_MAX_PATH+1];
   char temp[4];
   CFileException e;
   
   GetDesignPath(fullPath);
   strcat(fullPath, "Level");
   sprintf(temp, "%.3i", LevelIndex);
   strcat(fullPath, temp);
   strcat(fullPath, ".lvl");

   SetFileAttributes(fullPath, FILE_ATTRIBUTE_NORMAL);
   success = myFile.Open(fullPath, CFile::modeCreate | CFile::modeWrite | CFile::shareExclusive, &e);

   if (!success)
      WriteDebugString("Unable to open level file %s, error %i\n", fullPath, e.m_cause);

   if (success)
   {
     // put signal value and version at start of file
     __int64 hdr=0xFABCDEFABCDEFABF;
     double ver = PRODUCT_VER;
     myFile.Write(&hdr, sizeof(hdr));
     myFile.Write(&ver, sizeof(double));

     CAR ar(&myFile, CArchive::store);
     //ar.Compress(true); // qqqqq
   
     try 
     {
        data.Serialize(ar, ver);
        success = TRUE;
     }
     catch (...) 
     {
        WriteDebugString("Unable to save level file %s\n", ar.GetFile()->GetFileName());
        success = FALSE;
     }

     ar.Close();
   }

   return success;
}

BOOL saveLevel(int LevelIndex)
{
   //ASSERT( LevelIndex == globalData.currLevel ); // won't hold true when importing UA levels
   EditorStatusMsg("Saving level data...");

   levelData.GetSlots();

   BOOL success = saveLevel(levelData, LevelIndex);

   if (success)
      EditorStatusMsg("Level data saved.");
   else
      EditorStatusMsg("Unable to save level data.");

#ifdef UAFEDITOR
   EditorOptionsData.m_LastSaveSuccess=success;
   EditorOptionsData.m_LastDesign = GetDesignFolderRoot();
   EditorOptionsData.m_LastDesignLevel = globalData.currLevel;
#endif

   return success;
}

BOOL loadLevel(LEVEL &data, int LevelIndex)
{
   BOOL success = FALSE;
   CFile myFile;
   double ver=0.0;
   char fullPath[_MAX_PATH+1];
   char temp[4];

   sprintf(temp, "%.3i", LevelIndex);
   
   GetDesignPath(fullPath);
   strcat(fullPath, "Level");
   strcat(fullPath, temp);
   strcat(fullPath, ".lvl");   

   success = myFile.Open(fullPath, CFile::modeRead);

   if (!success)
   {
     WriteDebugString("Unable to open level file %s\n", fullPath);
     return FALSE;
   }

   data.Clear();

   /*
   int x;
   for (x=0; x<MAX_WALLSETS; x++)
     data.m_WallSets[x].Clear();

   for (x=0; x<MAX_BACKGROUNDS; x++)
     data.m_BackgroundSets[x].Clear();
    */

  // check for version
  __int64 hdr=0;
  myFile.Read(&hdr, sizeof(hdr));
  if (hdr == 0xFABCDEFABCDEFABF)
  {
    myFile.Read(&ver, sizeof(double));
  }
  else
  {
    // special hdr not present, so reset archive to beginning
    myFile.SeekToBegin();
    // set version to last build that didn't save version
    ver = _VERSION_0572_;
  }

  data.version = ver;

  // if loading older level data, don't use compression archive
  if (ver < _VERSION_0573_)
  {
    CArchive ar(&myFile, CArchive::load);
    try 
    {  
        data.Serialize(ar, ver);
        success = TRUE;
     }
     catch (...) 
     {
        success = FALSE;
        WriteDebugString("Unable to load level data file %s\n", fullPath);
     }

    ar.Close();
  }
  else
  {
    CAR ar(&myFile, CArchive::load);
    //ar.Compress(true); // qqqqq
    try 
    {
      data.Serialize(ar, ver);
      success = TRUE;
    }
    catch (...) 
    {
       success = FALSE;
       WriteDebugString("Unable to load level data file %s\n", fullPath);
    }
    ar.Close();
  } 
  
   if (success)
   {
     data.area_height = globalData.levelInfo.stats[LevelIndex].area_height;
     data.area_width = globalData.levelInfo.stats[LevelIndex].area_width;
     data.version = ver;
   }

   return success;
}

BOOL loadLevel(int LevelIndex, BOOL loadArt)
{
   EditorStatusMsg("Loading level data...");

   BOOL success = loadLevel(levelData, LevelIndex);

   if (success)
   {   
     globalData.currLevel = LevelIndex;
     StopAllSounds();
     deleteAllLevelArt(LevelIndex);
     deleteAllLevelSound(LevelIndex);

     levelData.SetSlots();
     if (loadArt)
     {
        WriteDebugString("Start loading WallSetSurfaces\n");
        if (!loadWallSetSurfaces())
          WriteDebugString("Failed to load WallSetSurfaces\n");
     }
     EditorStatusMsg("Level data loaded.");
     WriteDebugString("Level %u loaded, %ix%i, ver %f\n",
                      LevelIndex, 
                      levelData.area_width,levelData.area_height,
                      levelData.version);
   }
   else
     EditorStatusMsg("Unable to load level data.");

#ifdef UAFEDITOR
   EditorOptionsData.m_LastLoadSuccess=success;
   EditorOptionsData.m_LastDesign = GetDesignFolderRoot();
   EditorOptionsData.m_LastDesignLevel = globalData.currLevel;
   UndoData.RemoveAll();
#endif
   return success;
}


#if (defined UAFEDITOR)

BOOL FirstTimeFileAction = TRUE;

BOOL saveDesign(CArchive& ar)
{
   BOOL success = TRUE;
   int i;
   
   EditorStatusMsg("Saving design data...");
   SetCurrentDirectory(m_fileDir);

   try 
   {
      for (i=0;i<MAX_LEVELS;i++)
      {
        deleteAllLevelArt(i);
        deleteAllLevelSound(i);
        if (!globalData.levelInfo.stats[i].used)
          deleteLevel(i);
      }

      deleteAllLevelArt(GLOBAL_ART);
      deleteAllLevelSound(GLOBAL_SOUND);
      
     if (success)
     {
       EditorStatusMsg("Writing art files...");

       saveArt(globalData);
       saveArt(itemData);
       saveArt(monsterData);
       saveArt(spellData);
       // need to guarantee default monster icon is in design folder
       CString MI_Temp;
       MI_Temp.Format("%s%s", EditorArt,DEFAULT_MONSTER_ICON);
       saveArt(MI_Temp, IconDib, GLOBAL_ART);

       levelData.GetSlots();

       if (   (globalData.currLevel >= 0)
           && (globalData.levelInfo.stats[globalData.currLevel].used))
       {
         levelData.SaveArt(globalData.currLevel);
       }

       WriteRequiredArtList();

       EditorStatusMsg("Writing sound files...");
       
       saveSounds(globalData);
       saveSounds(itemData);
       saveSounds(monsterData);
       saveSounds(spellData);

       WriteRequiredSoundList();
     }

     if (success)
       globalData.Serialize(ar);   

     // save current level data
     if (   (success)
         && (globalData.currLevel >= 0)
         && (globalData.levelInfo.stats[globalData.currLevel].used))
     {
       success = saveLevel(globalData.currLevel);
     }

     if (success)
       globalData.ConfirmUsedLevels();

     if (success)
      success = saveData(itemData);

     if (success)
       success = saveData(monsterData);

     if (success)
        success = saveData(spellData);

     if (success)
        success = saveData(spellgroupData);

     if (success)
       success = saveData(raceData);

     if (success)
       success = saveData(traitData);

     if (success)
       success = saveData(abilityData);

     if (success)
       success = saveData(baseclassData);

     if (success)
       success = saveData(classData);

     // make sure GameExec exists
     BOOL PreSaveExecStatus = success;
     CString temp;
     if (success)
     {
       success = ConfigFile.FindToken("Game_Exec", temp, false);
       
       if (success)
       {
         if (!FileExists(temp))
         {
            success = FALSE;
            temp += " - game exec file does not exist\n";
            MsgBoxError(temp, "Save Design Error");
         }
       }
       else
         MsgBoxError("Game_Exec token not found in config.txt", "Save Design Error");
     }

     CString saveExec;
     CString file;
     char path[_MAX_PATH+1];
     if (success)
     {
        EditorStatusMsg("Saving game shortcut...");              
        int index = temp.ReverseFind('\\');

        if (index > 2)
          saveExec = temp.Right(temp.GetLength()-(index+1));
        else
          saveExec = temp;
        
        GetDesignPath(path);
        file.Format("%sStart.lnk", path);
        char *pCh;
        if ((pCh = strrchr(path, '\\')) != NULL)
          *pCh = '\0';

        CShellLinkInfo sli1;
        sli1.m_sTarget = _T(temp);
        file = "\"";
        file += path;
        file += "\"";
        sli1.m_sArguments = _T(file);
        sli1.m_sWorkingDirectory = _T(path);
        sli1.m_sDescription = _T("Play Design");
        sli1.m_sIconLocation = _T(temp);
        sli1.m_nIconIndex = 0;
        sli1.m_nShowCmd = SW_MAXIMIZE;

        CShellLink sl1;
        if (sl1.Create(sli1))
        {
          strcat(path, "\\");
          file.Format("%sStart.lnk", path);          
          SetFileAttributes(file, FILE_ATTRIBUTE_NORMAL);
          DeleteFile(file);
          if (!sl1.Save(_T(file)))
            success = FALSE;
        }
        else
          success = FALSE;

        if (!success)
        {
           CString error;
           error.Format("Could not create game exec shortcut for %s", temp);
           MsgBoxError(error, "Save Design Error");
        }
     }

     if (success)
     {
       saveExec = ConfigFile.GetFilename();

       if (!FileExists(saveExec))
       {
         temp.Format("Could not locate config.txt file to add to design");
         MsgBoxError(temp, "Save Design Error");
       }
       else
       {
         GetDesignPath(path);
         file.Format("%s%s", path, "config.txt");

         // Only update config.txt if editor version is more
         // recent than the design version. This will ensure that
         // that the design gets the latest config.txt file after 
         // the user upgrades DC.

          BOOL copy = ((!FileExists(file)) || (PreSaveDesignVer != PRODUCT_VER));

          if (copy)
          {
            SetFileAttributes(file, FILE_ATTRIBUTE_NORMAL);

            success = CopyFile(saveExec, file, FALSE);

            SetFileAttributes(file, FILE_ATTRIBUTE_NORMAL);

            if (!success)
            {
               temp.Format("Could not copy config file %s to %s", saveExec, file);
               MsgBoxError(temp, "Save Design Error");
            }
          }
       }
     }

     // ignore config file copy errors after letting the user know about it
     success = PreSaveExecStatus;
   }
   catch (...) 
   {
      WriteDebugString("Unable to save design %s\n", ar.GetFile()->GetFileName());
      success = FALSE;
   }
      
   if (success)
   {
     EditorStatusMsg("Validating event data...");
     if (!levelData.eventData.Validate())
       EditorStatusMsg("Event validation failed");

     EditorStatusMsg("Design data saved.");
   }
   else
      EditorStatusMsg("Unable to save design data.");

   FirstTimeFileAction=FALSE;
   return success;
}

BOOL saveDesign()
{
  HANDLE EngineEvent = CreateEvent(NULL, FALSE, FALSE, ENGINE_APP_EVENT);
  if (ERROR_ALREADY_EXISTS == GetLastError())
  {    
    CloseHandle(EngineEvent);

    // engine is running, but check to see if it is using
    // the same design files were are about to overwrite
    char dest[_MAX_PATH+1];      
    GetDesignPath(dest);
    strcat(dest,"game.dat");

    if (IsFileAlreadyOpen(dest))
    {
      MsgBoxError("You cannot save the design while the engine is using the same files");
      FirstTimeFileAction=FALSE;
      return FALSE;
    }
  }
  else
    CloseHandle(EngineEvent);

   if (EditorOptionsData.m_LastDesign.CompareNoCase(GetDesignName())!=0)
     EditorOptionsData.m_LastSaveSuccess=FALSE; // not the same design

   BOOL success = FALSE;
   
   CFile myFile;
   char fullPath[_MAX_PATH+1];

   try
   {
     if (!CreateDesignDirectory())
     {
       WriteDebugString("Failed to create design directory\n");
       EditorOptionsData.m_LastSaveSuccess=FALSE;
       FirstTimeFileAction=FALSE;
       return FALSE;
     }

     PreSaveDesignVer = globalData.version;

     if (globalData.version != PRODUCT_VER)
     {       
       WriteDebugString("Changing design ver from %f to %f\n",
                        globalData.version, PRODUCT_VER);
       globalData.version = PRODUCT_VER;                 
     }

     // backup the current files if we know that
     // they have been validated by loading/saving
     if ((FirstTimeFileAction)||(EditorOptionsData.m_LastSaveSuccess))
       BackupDesignFiles();

     char dest[_MAX_PATH+1];
     CString src;
#ifdef USE_BASS      
      GetDesignPath(dest);
      strcat(dest,"bass.dll");
      src.Format("%s%s", m_installDir, "bass.dll");
      SetFileAttributes(dest, FILE_ATTRIBUTE_NORMAL);

      // if engine is running, bass.dll will be in use and this
      // function will fail.
      //
      // try this operation before opening game.dat so we don't
      // lose any design data.
      if (!CopyFile(src, dest, FALSE))
      {
        WriteDebugString("Error %u: Failed to copy BASS.DLL to design folder\n", GetLastError());
        MsgBoxError("You cannot save the design while the engine is using the same files");        
        FirstTimeFileAction=FALSE;
        return FALSE;
      }
#endif

      GetDesignPath(dest);
      strcat(dest,"ReadMe.txt");
      DeleteFile(dest);
      src.Format("%s%s", m_installDir, "ReadMe.txt");
      CopyFile(src, dest, FALSE);

     GetDesignPath(fullPath);
     strcat(fullPath, "game.dat");

     CFileException e;
     SetFileAttributes(fullPath, FILE_ATTRIBUTE_NORMAL);
     if (!myFile.Open(fullPath, (CFile::modeCreate|CFile::modeWrite|CFile::shareExclusive), &e))
     {
        WriteDebugString("Unable to open design file %s, error %i\n", fullPath, e.m_cause);
        EditorOptionsData.m_LastSaveSuccess=FALSE;
        FirstTimeFileAction=FALSE;
        return FALSE;
     }   
  
     CArchive ar(&myFile, CArchive::store);
     success = saveDesign(ar);
     ar.Close();
   }
   catch(...)
   {
     WriteDebugString("Unable to save design %s\n", fullPath);
     success = FALSE;
   }

   EditorOptionsData.m_LastSaveSuccess=success;
   FirstTimeFileAction=FALSE;
   return success;
}

void ForceReloadOfConfigFiles()
{
  CUAFWinEdApp *pApp = reinterpret_cast<CUAFWinEdApp*>(AfxGetApp());
  pApp->LoadDefaultDBFiles();     	
}
#endif // UAFEDITOR


BOOL loadDesign(CArchive& ar)
{
   BOOL success = TRUE;
   int dataCount;

   EditorStatusMsg("Loading design data...");

   //clearDesign(FALSE);

   m_SmallPicImport.RemoveAll();
   m_IconPicImport.RemoveAll();

   WriteDebugString("Finished clearing old design\n");
   try 
   {
     char fullPath[_MAX_PATH+1];    

      GetDesignPath(fullPath);
      strcat(fullPath, ITEM_DB_NAME);

      if (loadData(itemData, fullPath) <= 0)
         success = FALSE;
      WriteDebugString("Finished loading ItemDB\n");

     if (success)
     {
        GetDesignPath(fullPath);
        strcat(fullPath, MONSTER_DB_NAME);

        if (loadData(monsterData, fullPath) <= 0)
           success = FALSE;
        WriteDebugString("Finished loading MonsterDB\n");
     }

     if (success)
     {
        GetDesignPath(fullPath);
        strcat(fullPath, SPELL_DB_NAME);

        if (loadData(spellData, fullPath) <= 0)
           success = FALSE;
        WriteDebugString("Finished loading SpellDB\n");
     }

     if (success)
     {
        GetDesignPath(fullPath);
        strcat(fullPath, RACE_DB_NAME);

        dataCount=loadData(raceData, fullPath);
        if (dataCount <= 0)
        {
          WriteDebugString("Error loading %s.  Using defaults\n",fullPath);
          loadUADefaults(raceData);
        }
        else
        {
          WriteDebugString("Loaded %d entries from %s\n", dataCount, fullPath);
        };
     }

     if (success)
     {
        GetDesignPath(fullPath);
        strcat(fullPath, CLASS_DB_NAME);

        dataCount=loadData(classData, fullPath);
        if ( dataCount<= 0)
        {
          WriteDebugString("Error loading %s.  Using Defaults\n",fullPath);
          loadUADefaults(classData);
        }
        else
        {
          WriteDebugString("Loaded %d entries from %s\n", dataCount, fullPath);
        };
     }

     if (success)
     {
        GetDesignPath(fullPath);
        strcat(fullPath, BASE_CLASS_DB_NAME);

        dataCount=loadData(baseclassData, fullPath);
        if ( dataCount <= 0)          
        {
          WriteDebugString("Error loading %s.  Using Defaults\n",fullPath);
          loadUADefaults(baseclassData);
        }
        else
        {
          WriteDebugString("Loaded %d entries from %s\n",dataCount,fullPath);
        };
     }

     if (success)
     {
        GetDesignPath(fullPath);
        strcat(fullPath, ABILITY_DB_NAME);

        dataCount=loadData(abilityData, fullPath);
        if ( dataCount <= 0)
        {
          WriteDebugString("Error loading %s.  Using Defaults\n",fullPath);
          loadUADefaults(abilityData);
        }
        else
        {
          WriteDebugString("Loaded %d entries from %s\n",dataCount,fullPath);
        };
     }

     if (success)
     {
        GetDesignPath(fullPath);
        strcat(fullPath, SPELLGROUP_DB_NAME);

        dataCount=loadData(spellgroupData, fullPath);
        if ( dataCount <= 0)
        {
          WriteDebugString("Error loading %s.  Using Defaults\n",fullPath);
          loadUADefaults(spellgroupData);
        }
        else
        {
          WriteDebugString("Loaded %d entries from %s\n",dataCount,fullPath);
        };
     }

     if (success)
     {
        GetDesignPath(fullPath);
        strcat(fullPath, TRAIT_DB_NAME);

        dataCount=loadData(traitData, fullPath);
        if ( dataCount <= 0)
        {
          WriteDebugString("Error loading %s.  Using Defaults\n",fullPath);
          loadUADefaults(traitData);
        }
        else
        {
          WriteDebugString("Loaded %d entries from %s\n",dataCount,fullPath);
        };
     }

#ifdef UAFEDITOR
     if ((success)&&(globalData.version < _VERSION_0882_))
     {
       // Recent changes to the config files require the latest
       // files to be reloaded, rather than relying on the older
       // data stored in the DAT files (just loaded prior to this).
       // 
       // globalData.version is initialized prior to this even though
       // globalData.serialize() is called after this.
       ForceReloadOfConfigFiles();
     }
#endif

     globalData.Serialize(ar);
     WriteDebugString("Finished loading globalData\n");

     if (success)
     {      
        party.Posx = globalData.startX;
        party.Posy = globalData.startY;
        party.facing = globalData.startFacing;
     }
     else 
     {
        party.Posx = 0;
        party.Posy = 0;
        party.facing = FACE_EAST;
     }
   }
   catch (...)
   {
     success = FALSE;
   }
  
   if (success)
   {
      EditorStatusMsg("Design data loaded.");
      WriteDebugString("Design %s loaded, version %f\n", 
                       globalData.designName, globalData.version);         
   }
   else
      EditorStatusMsg("Unable to load design data.");

#if (defined UAFEDITOR)
   EditorOptionsData.m_LastLoadSuccess=success;
   UndoData.RemoveAll();
   FirstTimeFileAction=FALSE;
#endif

   if (success)
   {
     for (int i=0;i<MAX_LEVELS;i++)
     {
       if (!globalData.levelInfo.stats[i].used)
         deleteLevel(i);
     }
   }

   return success;
}

BOOL loadDesign(LPCSTR name)
{
   BOOL success = TRUE;
   CFile myFile;
   char fullPath[_MAX_PATH+1];
   memset(fullPath, 0, sizeof(fullPath));

   clearDesign(FALSE);
  
   if (name != NULL)
   {
     if (strlen(name) == 0)
     {
        WriteDebugString("Empty design name in loadDesign()\n");
        EditorStatusMsg("Unable to load design data."); 
        success = FALSE;
     }
     else
     {
       strcpy(fullPath, m_installDir);
       strcat(fullPath, name);   
       strcat(fullPath, ".dsn");     
       strcpy(m_designFolder, fullPath);
       strcat(m_designFolder, "\\");
       strcat(fullPath, "\\game.dat");
     }
   }
   else
   {
     strcpy(fullPath, m_installDir);
     strcpy(m_designFolder, fullPath);
     strcat(m_designFolder, "\\");
     strcat(fullPath, "game.dat");
   }

   double DesignVersion = GetDesignVersion(fullPath);
   BOOL VersionError = FALSE;

   if ((success) && (DesignVersion > PRODUCT_VER))
   {
     // version in design file is greater than the
     // version of this build
     VersionError = TRUE;
     WriteDebugString("Version Check: design name %s, curr ver %f, design ver %f\n",
                      fullPath, PRODUCT_VER, DesignVersion);
   }
   else if (DesignVersion < 0.5)
   {
     // something is wrong with game.dat file
     MsgBoxError("The Game.dat file appears to be missing or invalid - Load failed", "Load Error");
     success = FALSE;
   }
   else
     WriteDebugString("Loading design %s, version %f...\n", fullPath, DesignVersion);

   try
   {
     if (success)
     {
       globalData.version = DesignVersion;
       if (!myFile.Open(fullPath, CFile::modeRead))
       {
          WriteDebugString("Unable to open design file %s\n", fullPath);
          CString errorMsg;
          errorMsg.Format("Unable to open design file %s, load failed", fullPath);                       
          MsgBoxError(errorMsg, "Design Load Failed");
          success = FALSE;
       }   
       else
       {  
         CArchive ar(&myFile, CArchive::load);  
         success = loadDesign(ar);
         ar.Close();
       }
     }
   }
   catch (...)
   {
      WriteDebugString("Unable to load design %s\n", fullPath);
      success = FALSE;
   }

   if ((!success) && (VersionError))
   {
#ifdef UAFEDITOR
     EditorStatusMsg("Design version incompatible");
     CString errorMsg;
     errorMsg.Format("You are trying to load a design saved with UAF version %f,\n"
                     "which is not compatible with your UAF version %f.\n"
                     "You must upgrade to version %f or higher to load this design",
                     DesignVersion, PRODUCT_VER, DesignVersion);
     MsgBoxError(errorMsg, "Design Version Incompatible");
#else
     miscError = DesignVersionError;
#endif
   }

#ifdef UAFEDITOR
   EditorOptionsData.m_LastLoadSuccess=success;
   FirstTimeFileAction=FALSE;
#endif        
   return success;
}

void clearDesign(BOOL FillDefaults)
{
   int i;  
   globalData.Clear();
   levelData.Clear();

   for (i=0;i<MAX_LEVELS;i++)
   {
      deleteAllLevelArt(i);
      deleteAllLevelSound(i);
   }

   deleteAllLevelArt(GLOBAL_ART);
   deleteAllLevelSound(GLOBAL_SOUND);

   clearArtSlots();

   m_SmallPicImport.RemoveAll();
   m_IconPicImport.RemoveAll();

#if (defined UAFEDITOR)

   CopyEventData.Clear();
   UndoData.RemoveAll();

   HBarVPArt.name.Format("%s%s", EditorArt, DEFAULT_HB);
   VBarVPArt.name.Format("%s%s", EditorArt, DEFAULT_VB);
   FrameVPArt.name.Format("%s%s", EditorArt, DEFAULT_FR);
   VBarCombArt.name.Format("%s%s", EditorArt, DEFAULT_VB);
   HBarCombArt.name.Format("%s%s", EditorArt, DEFAULT_HB);
   CombatWallArt.name.Format("%s%s", EditorArt, DEFAULT_CWD);
   CombatCursorArt.name.Format("%s%s", EditorArt, DEFAULT_CC);
   CombatDeathIconArt.name.Format("%s%s", EditorArt, DEFAULT_CDI);
   CombatDeathArt.name.Format("%s%s", EditorArt, DEFAULT_CD);
   CursorArt.filename.Format("%s%s", EditorArt, DEFAULT_CURSOR);
   CursorArt.FrameHeight = 32;
   CursorArt.FrameWidth = 32;
   CursorArt.NumFrames = 15;
   CursorArt.picType = TransBufferDib;
   CursorArt.style = PIC_DATA::AS_None;
   CursorArt.timeDelay = 0;
   
   // load up the default icons/sprites that
   // are in the editor/DefaultArt folder.
   //
   // Loads all icons named Icon1.pcx..Icon50.pcx
   // Loads all icons named SPic1.pcx..SPic50.pcx

   i=1;
   CString temp;
   CString path;
   PIC_DATA pic;
   pic.picType = SmallPicDib;
   pic.SetDefaults();

   while (i <= 50)
   {
     temp.Format("prt_SPic%u.png", i);
     // uses root name to search for any matches
     // using valid image extensions (bmp, pcx, etc)
     if (FindImageWithValidExt(temp))
     {
       pic.filename = temp;
       m_SmallPicImport.AddTail(pic);
     }
     i++;
   }

   i=1;
   pic.Clear();
   pic.picType = IconDib;
   pic.SetDefaults();
   while (i <= 50)
   {
     temp.Format("cn_Icon%u.png", i);
     if (FindImageWithValidExt(temp))
     {
       pic.filename = temp;
       m_IconPicImport.AddTail(pic);
     }
     i++;
   }
   currX = globalData.startX;
   currY = globalData.startY;
   if ((currX < 0) || (currX >= globalData.levelInfo.stats[globalData.startLevel].area_width))
   {
     currX  = max(0, currX);
     currX  = min(globalData.levelInfo.stats[globalData.startLevel].area_width-1, currX);
   }
   if ((currY < 0) || (currY >= globalData.levelInfo.stats[globalData.startLevel].area_height))
   {
     currY  = max(0, currY);
     currY  = min(globalData.levelInfo.stats[globalData.startLevel].area_height-1, currY);
   }
   currFacing = globalData.startFacing;
   party.facing = currFacing;
   party.Posx = currX;
   party.Posy = currY;

   globalData.levelInfo.stats[globalData.startLevel].SetDefaults();

   if (FillDefaults)
   {
     fillDefaultArtSlots(levelData,globalData.levelInfo.stats[globalData.startLevel].overland);
     levelData.SetSlots();
     if (loadWallSetSurfaces())
     {
       currBlockage = BlockedBlk;
       currWallSlot = FindFirstLoadedWall();
       currBkGnd = FindFirstLoadedBackground();
     }

     fillDefaultLevelData(levelData,
                          globalData.startLevel, 
                          globalData.levelInfo.stats[globalData.startLevel].area_width, 
                          globalData.levelInfo.stats[globalData.startLevel].area_height, 
                          globalData.levelInfo.stats[globalData.startLevel].overland, 
                          TRUE);
     levelData.GetSlots();

   }

   m_mapType = (globalData.levelInfo.stats[globalData.startLevel].overland ? OVERLAND_TYPE : DUNGEON_TYPE);

   if (m_mapType == OVERLAND_TYPE)
   {
     if (m_mode == WALL_MODE)
       m_mode = BLOCKAGE_MODE;
   }
   else
   {
     if (m_mode == BLOCKAGE_MODE)
       m_mode = WALL_MODE;
   }
#endif

   /*
  for (i=0;i<MAX_LEVELS;i++)
  {
    if (!globalData.levelInfo.stats[i].used)
      deleteLevel(i);
  }
  */

  m_designFolder[0]='\0';
}


BOOL CheckLevelForWallSlot(int slot)
{
  if (!globalData.levelInfo.stats[slot].used)
    return FALSE;

  for (int i=0;i < globalData.levelInfo.stats[slot].area_height;i++)
  {
    for (int j=0;j < globalData.levelInfo.stats[slot].area_width;j++)
    {
       if (   (levelData.area[i][j].northWall == slot)
           || (levelData.area[i][j].southWall == slot)
           || (levelData.area[i][j].eastWall == slot)
           || (levelData.area[i][j].westWall == slot))
       {
          return TRUE;
       }
    }
  }
  return FALSE;
}

BOOL CheckLevelForBackgroundSlot(int slot)
{
  if (!globalData.levelInfo.stats[slot].used)
    return FALSE;

  for (int i=0;i < globalData.levelInfo.stats[slot].area_height;i++)
  {
    for (int j=0;j < globalData.levelInfo.stats[slot].area_width;j++)
    {
      if (levelData.area[i][j].northBG == slot)
        return TRUE;
      if (levelData.area[i][j].eastBG == slot)
        return TRUE;
      if (levelData.area[i][j].southBG == slot)
        return TRUE;
      if (levelData.area[i][j].westBG == slot)
        return TRUE;
    }
  }
  return FALSE;
}

int GetAreaWidth(LEVEL& data)
{
  return data.area_width;
}

int GetAreaHeight(LEVEL& data)
{
  return data.area_height;
}

int GetAreaBlockages(LEVEL& data, int x, int y, int dir)
{
  return data.area[y][x].blockages(dir);
}

BOOL deleteLevel(int LevelIndex)
{   
   char fullPath[_MAX_PATH+1];
   char temp[4];
   sprintf(temp, "%.3i", LevelIndex);
  
   GetDesignPath(fullPath);
   strcat(fullPath, "Level");
   strcat(fullPath, temp);
   strcat(fullPath, ".lvl");
   
   globalData.levelInfo.Clear(LevelIndex);
   if (LevelIndex == globalData.currLevel)
     levelData.Clear();

   // remove any art this level may have been using
   deleteAllLevelArt(LevelIndex);

   if (!FileExists(fullPath))
     return TRUE;

   // remove read-only flags
   SetFileAttributes(fullPath, FILE_ATTRIBUTE_NORMAL);

   return remove(fullPath);
}

BOOL levelExists(int i)
{
  BOOL success = TRUE;
  char fullPath[_MAX_PATH+1];
  char temp[9];
  sprintf(temp, "%.3i", i);

  GetDesignPath(fullPath);
  strcat(fullPath, "Level");
  strcat(fullPath, temp);
  strcat(fullPath, ".lvl");

  if (!FileExists(fullPath))
    success = FALSE;

  return success;
}
