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

#ifdef UAFEDITOR
#include "dungeoncraft/shared/..\UAFWinEd\UAFWinEd.h"
#endif
// #include "dungeoncraft/shared/externs.h"
#include <math.h>
#include "dungeoncraft/shared/Char.h"
#include "dungeoncraft/shared/party.h"
#include "dungeoncraft/shared/GlobalData.h"
#include "dungeoncraft/shared/level.h"

#ifdef UAFEngine
#include "dungeoncraft/shared/..\UAFWin\dgngame.h"
#include "dungeoncraft/shared/..\UAFWin\disptext.h"
#include "dungeoncraft/shared/..\UAFWin\Path.h"
#include "dungeoncraft/shared/..\UAFWin\FormattedText.h"
#endif


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_SERIAL( BLOCKAGE_STATUS, CObject, 1 )
IMPLEMENT_SERIAL( VISIT_DATA, CObject, 1 )
IMPLEMENT_SERIAL( TRIGGER_FLAGS, CObject, 1 )
IMPLEMENT_SERIAL( LEVEL_FLAG_DATA, CObject, 1 )
IMPLEMENT_SERIAL( EVENT_TRIGGER_DATA, CObject, 1 )
IMPLEMENT_SERIAL( PARTY, CObject, 1 )

PARTY party;

#ifdef UAFEngine
TEMP_CHARACTERS tempChars;

TEMP_CHARACTERS::TEMP_CHARACTERS() 
{ 
  Clear(); 
}

TEMP_CHARACTERS::~TEMP_CHARACTERS() 
{ 
  Clear(); 
}
int TEMP_CHARACTERS::numChars() 
{ 
  return temps.numChars(); 
}

void TEMP_CHARACTERS::Clear()
{
  inParty.RemoveAll();
  temps.Clear();
  temps.SetAsTempCharList();
}

CString TEMP_CHARACTERS::GetCharName(int index)
{
  if ((index >= 0) && (index < inParty.GetSize()))
    return temps.GetCharacterName(inParty[index].key);
  else
    return "Unknown";
}

void TEMP_CHARACTERS::RemoveThoseInParty()
{
  BOOL done = FALSE;

  while (!done)
  {
    done = TRUE;
    for (int i=0;i<inParty.GetSize();i++)
    {
      if (inParty[i].inParty)
      {
        done = FALSE;
        temps.RemoveCharacter(inParty[i].key);
        inParty.RemoveAt(i);
        break; // start from begining;
      }
    }
  }
}

void TEMP_CHARACTERS::RemoveNPCIfInParty()
{
  BOOL done = FALSE;
  CHARACTER dude;
  while (!done)
  {
    done = TRUE;
    for (int i=0;i<inParty.GetSize();i++)
    {      
      dude = GetCharacter(i);
      if (dude.GetType() == NPC_TYPE)
      {
        for (int p=0;p<party.numCharacters;p++)
        {
          if (   (party.characters[p].GetType() == NPC_TYPE) 
              && (party.characters[p].key == dude.key))
          {
            done = FALSE;
            deleteTemp(i);
          }
        }
      }
      if (!done)
        break;
    }
  }
}

int TEMP_CHARACTERS::GetInPartyCount()
{
  int count = 0;
  for (int i=0;i<inParty.GetSize();i++)
  {
    if (inParty[i].inParty)
      count++;
  }
  return count;
}

void TEMP_CHARACTERS::SetInParty(int index, BOOL status)
{
  if ((index >= 0) && (index < inParty.GetSize()))
    inParty[index].inParty = status;
}

BOOL TEMP_CHARACTERS::InParty(int index) 
{
  if ((index >= 0) && (index < inParty.GetSize()))
      return inParty[index].inParty;
  else
    return FALSE;
}

int TEMP_CHARACTERS::GetKey(int index)
{
  if ((index >= 0) && (index < inParty.GetSize()))
    return inParty[index].key;
  else
    return -1;
}

CHARACTER TEMP_CHARACTERS::GetCharacter(int index)
{
  CHARACTER dude;
  temps.GetCharacterData(inParty[index].key, dude);
  return dude;
}

void TEMP_CHARACTERS::deleteTemp(int index)
{
  if ((index >= 0) && (index < inParty.GetSize()))
  {
    temps.RemoveCharacter(inParty[index].key);
    inParty.RemoveAt(index);      
  }
}

BOOL TEMP_CHARACTERS::IsCharacterInTemps(CHARACTER &dude)
{
  POSITION pos=temps.GetHeadPosition();
  while (pos!=NULL)
  {
    if (temps.GetAt(pos).IsSameCharacter(dude))
      return TRUE;
    temps.GetNext(pos);
  }
  return FALSE;
}

int TEMP_CHARACTERS::addPreGenCharacter(CHARACTER &dude, DWORD key)
{
  if (numChars() < MAX_TEMP_CHARS)
  {
     BOOL partymember=FALSE;
     for (int i=0;i<party.numCharacters;i++)
     {
       // try to determine if the two characters match,
       // this doesn't check all CHARACTER members for
       // equality since some portions of a CHARACTER
       // will change after it is saved (exp, level, etc)
       if (party.characters[i].IsSameCharacter(dude))
       {
         partymember=TRUE;
         break;
       }
     }

     if (!IsCharacterInTemps(dude))
     {
      dude.origIndex = key; // key for character in globalData.charData
      //dude.type = NPC_TYPE;
      TEMP_IN_PARTY_DATA data;
      data.key = temps.AddCharacter(dude);
      data.inParty = partymember;    
      return (inParty.Add(data));
     }
  }
  return -1;
}

int TEMP_CHARACTERS::addTempCharacter(CHARACTER &dude)
{    
  if (numChars() >= MAX_TEMP_CHARS) return -1;
  if (IsCharacterInTemps(dude)) return -1;

  BOOL partymember=FALSE;
   for (int i=0;i<party.numCharacters;i++)
   {
     // try to determine if the two characters match,
     // this doesn't check all CHARACTER members for
     // equality since some portions of a CHARACTER
     // will change after it is saved (exp, level, etc)
     if (party.characters[i].IsSameCharacter(dude))
     {
       partymember=TRUE;
       break;
     }
   }

  //dude.key = 0;
  //dude.type = CHAR_TYPE;
  TEMP_IN_PARTY_DATA data;
  data.key = temps.AddCharacter(dude);
  data.inParty = partymember;
  return (inParty.Add(data));
}

void TEMP_CHARACTERS::AddSavedCharacters()
{
  if (numChars() >= MAX_TEMP_CHARS) return;

  // search save game folder and common char folder for characters
  // and add them to available temps,

  // save game folder first
  CString searchpath = GetSavePath();
  if (searchpath[searchpath.GetLength()-1] != '\\')
    searchpath += "\\";
  searchpath += "*.chr";
 
  CFileFind finder;
  BOOL found = finder.FindFile(searchpath);
  CHARACTER tempCHARACTER;

  while (found)
  {
     found = finder.FindNextFile();
     if (finder.IsDots())
        continue;
     if (finder.IsDirectory())
       continue;

     tempCHARACTER.Clear();
     if (tempCHARACTER.serializeCharacter(FALSE, finder.GetFileTitle()))
     {
       if ((tempCHARACTER.GetType() == NPC_TYPE) && (tempCHARACTER.IsPreGen))
         continue;

       // previously saved character might be dead, and when
       // starting a new game the original NPC must be restored.
       if (tempCHARACTER.GetType()==NPC_TYPE)
         tempCHARACTER.RestoreToBestCondition();

       // only CHAR_TYPE and non-pregen NPC's are stored
       if (!IsCharacterInTemps(tempCHARACTER))
        addTempCharacter(tempCHARACTER);
     }
     else
     {
       WriteDebugString("Failed to load saved char %s into temps\n",
                        finder.GetFilePath());
       CString tmp(finder.GetFilePath());
       tmp.MakeLower();
       if (tmp.Find(".chr_bad")==-1)
       {
         tmp+="_bad";
         SetFileAttributes(finder.GetFilePath(), FILE_ATTRIBUTE_NORMAL);
         rename(finder.GetFilePath(), tmp);
       }
     }
  }

  finder.Close();

  // now the common char folder
  searchpath = GetCommonCharPath();
  if (searchpath.IsEmpty())
  {
    // no common char folder defined
    return;
  }

  if (searchpath[searchpath.GetLength()-1] != '\\')
    searchpath += "\\";
  searchpath += "*.chr";
 
  found = finder.FindFile(searchpath);

  while (found)
  {
     found = finder.FindNextFile();
     if (finder.IsDots())
        continue;
     if (finder.IsDirectory())
       continue;

     tempCHARACTER.Clear();
     if (tempCHARACTER.serializeCharacter(FALSE, finder.GetFileTitle()))
     {
       if ((tempCHARACTER.GetType() == NPC_TYPE) && (tempCHARACTER.IsPreGen))
         continue;

       // only CHAR_TYPE and non-pregen NPC's are stored
       if (!IsCharacterInTemps(tempCHARACTER))
        addTempCharacter(tempCHARACTER);
     }
     else
     {
       WriteDebugString("Failed to load common saved char %s into temps\n",
                        finder.GetFilePath());
       CString tmp(finder.GetFilePath());
       tmp.MakeLower();
       if (tmp.Find(".chr_bad")==-1)
       {
         tmp+="_bad";
         SetFileAttributes(finder.GetFilePath(), FILE_ATTRIBUTE_NORMAL);
         rename(finder.GetFilePath(), tmp);
       }
     }
  }
}

void TEMP_CHARACTERS::RemoveSavedCharacters()
{
  // remove temps if found in save game folder
  // so they don't get duplicated next time
  // the saved characters get added.

  if (numChars() <= 0) return;
  CString searchpath = GetSavePath();
  if (searchpath[searchpath.GetLength()-1] != '\\')
    searchpath += "\\";
  searchpath += "*.chr";
 
  CFileFind finder;
  BOOL found = finder.FindFile(searchpath);
  CHARACTER tempCHARACTER;

  while (found)
  {
     found = finder.FindNextFile();
     if (finder.IsDots())
        continue;
     if (finder.IsDirectory())
       continue;

     tempCHARACTER.Clear();
     if (tempCHARACTER.serializeCharacter(FALSE, finder.GetFileTitle()))
     {
        for (int i=0;i<inParty.GetSize();i++)
        {      
          if (GetCharacter(i).IsSameCharacter(tempCHARACTER))
          {
            deleteTemp(i);
            break;
          }
        }
     }
     else
     {
       WriteDebugString("Failed to load saved char %s while clearing temps\n",
                        finder.GetFilePath());
     }
  }
}
#endif // UAFEngine

//*****************************************************************************
// NAME:    LEVEL_FLAG_DATA::Serialize
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
void LEVEL_FLAG_DATA::Serialize(CArchive &ar)
{
  CObject::Serialize(ar);

  if (ar.IsStoring())
  {
    ar.Write(&stepData, sizeof(stepData));

    ar << flagData.GetCount();  

    POSITION pos = flagData.GetHeadPosition();
    while (pos != NULL)
    {
      ar << flagData.GetKeyAt(pos);
      flagData.GetNext(pos).Serialize(ar);
    }
  }
  else
  {
    int count;  
    TRIGGER_FLAGS tmpFlags;
    DWORD key;

    ar.Read(&stepData, sizeof(stepData));

    ar >> count;
  
    for (int i=0; i<count; i++)
    {
      ar >> key;
      tmpFlags.Serialize(ar);
      flagData.Insert(tmpFlags, key);
    }  
  }
}


//*****************************************************************************
// NAME: EVENT_TRIGGER_DATA::Serialize
//
// PURPOSE:  
//*****************************************************************************
void EVENT_TRIGGER_DATA::Serialize(CArchive &ar)
{
  CObject::Serialize(ar);
  
  int count,i;  

  if (ar.IsStoring())
  {
    ar << data.GetSize();
    for (i=0;i<data.GetSize(); i++)
      data[i].Serialize(ar);
  }
  else
  {
    ar >> count;
    data.SetSize(count);
    for (i=0;i<count;i++)
      data[i].Serialize(ar);
  }
}

//*****************************************************************************
// NAME:    =
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
PARTY& PARTY::operator =(const PARTY& src)
{
  if (&src == this) return *this;
   memcpy(&stateStack, &src.stateStack, sizeof(stateStack));
   days = src.days;
   hours = src.hours;
   minutes = src.minutes;
   drinkPoints = src.drinkPoints;
   if (!src.name.IsEmpty())
      name = src.name;
   else
      name="";
   adventuring = src.adventuring;
   areaView = src.areaView;
   searching = src.searching;
   looking = src.looking;
   resting = src.resting;
   //detectingTraps = src.detectingTraps;
   //detectingInvisible = src.detectingInvisible;
   //detectingMagic = src.detectingMagic;
   level = src.level;
   speed = src.speed;
   Posx = src.Posx;
   Posy = src.Posy;
   PrevPosx = src.PrevPosx;
   PrevPosy = src.PrevPosy;
   facing = src.facing;
   PrevRelFacing=src.PrevRelFacing;
   activeCharacter = src.activeCharacter;
   activeItem = src.activeItem;
   tradeItem = src.tradeItem;
   tradeGiver = src.tradeGiver;
   tradeQty = src.tradeQty;
   skillLevel = src.skillLevel;
   numCharacters = src.numCharacters;
   moneyPooled = src.moneyPooled;
   poolSack = src.poolSack;   
   ActiveScripter=src.ActiveScripter;
   eventFlags = src.eventFlags;
   visitData  = src.visitData;
   blockageData = src.blockageData;
   journal = src.journal;

   int i;
   for (i=0;i<MAX_PARTY_MEMBERS;i++)
      characters[i] = src.characters[i];
   party_asl = src.party_asl;
   return *this;
}

//*****************************************************************************
// NAME:    PARTY::PARTY
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
PARTY::PARTY(const PARTY& src)
	:party_asl("PARTY_ATTRIBUTES")
{
  *this = src;
}

//*****************************************************************************
// NAME: clearPartyFlags
//
// PURPOSE:  
//*****************************************************************************
void PARTY::clearPartyFlags()
{
   eventFlags.Clear();
   visitData.Clear();
   blockageData.Clear();
}

//*****************************************************************************
// NAME: Clear
//
// PURPOSE:  
//*****************************************************************************
void PARTY::Clear(BOOL IsConstructor)
{
   memset(&stateStack, 0, sizeof(stateStack));
   days = 0;
   hours = 0;
   minutes = 0;
   drinkPoints = 0;
   name="";
   adventuring = FALSE;
   areaView = FALSE;
   searching = FALSE;
   looking = FALSE;
   resting = FALSE;
   //detectingTraps = FALSE;
   //detectingInvisible = FALSE;
   //detectingMagic = FALSE;
   level = 0;
   speed = 3;
   Posx = 0;
   Posy = 0;
   PrevPosx = -1;
   PrevPosy = -1;
   facing = 0;
   PrevRelFacing=-1;
   activeCharacter =  0;
   activeItem = 0;
   tradeItem = 0;
   tradeGiver = 0;
   tradeQty = 0;
   skillLevel = 2;
   numCharacters = 0;
   DayMusic=-1;
   moneyPooled = FALSE;
   poolSack.Clear();

   clearPartyFlags();

   int i;
   for (i=0;i<MAX_PARTY_MEMBERS;i++)
      characters[i].Clear(IsConstructor);
   party_asl.Clear();
}

//*****************************************************************************
// NAME:    PARTY::Serialize
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
#ifdef UAFEngine
void PARTY::Serialize(CArchive &ar)
{
  CObject::Serialize(ar);
  int temp,i;

  if (ar.IsStoring())
  {
    ar << stateStack.count;
    for (i=0;i<(int)stateStack.count;i++)
    {
      ar << stateStack.tdata[i].id;
      ar << stateStack.tdata[i].flags;
      ASSERT( stateStack.tdata[i].datacount <= MAX_TASK_STATE_SAVE_BYTES );
      ar << stateStack.tdata[i].datacount;
      for (int d=0;d<stateStack.tdata[i].datacount;d++)
        ar << stateStack.tdata[i].data[d];
    }

    ar << days;
    ar << hours;
    ar << minutes;
    ar << drinkPoints;

    AS(ar,name);
    ar << adventuring;
    ar << areaView;
    ar << searching;
    ar << level;
    ar << speed;
    ar << Posx;
    ar << Posy;
    ar << PrevPosx;
    ar << PrevPosy;
    ar << facing;
    ar << activeCharacter;
    ar << activeItem;
    ar << tradeItem;
    ar << tradeGiver;
    ar << tradeQty;
    ar << skillLevel;
    ar << numCharacters;
    ar << moneyPooled;
  }
  else
  {
    ar >> temp;
    stateStack.count = temp;
    for (i=0;i<temp;i++)
    {
      ar >> stateStack.tdata[i].id;
      ar >> stateStack.tdata[i].flags;
      ar >> stateStack.tdata[i].datacount;
      ASSERT( stateStack.tdata[i].datacount <= MAX_TASK_STATE_SAVE_BYTES );
      for (int d=0;d<stateStack.tdata[i].datacount;d++)
        ar >> stateStack.tdata[i].data[d];
    }

    ar >> days;
    ar >> hours;
    ar >> minutes;
    ar >> drinkPoints;
    DAS(ar,name);
    ar >> adventuring;
    ar >> areaView;
    ar >> searching;
    if (globalData.SaveGameVersion < _VERSION_0850_)
    {
      BOOL unused;
      ar >> unused; //detectingTraps; // these three unused
      ar >> unused; //detectingInvisible;    
      ar >> unused; //detectingMagic;
    }
    ar >> level;
    ar >> speed;
    ar >> Posx;
    ar >> Posy;
    if (globalData.SaveGameVersion >= _VERSION_0575_)
    {
      ar >> PrevPosx;
      ar >> PrevPosy;
    }
    ar >> facing;
    ar >> activeCharacter;
    ar >> activeItem;
    ar >> tradeItem;
    ar >> tradeGiver;
    ar >> tradeQty;
    ar >> skillLevel;
    ar >> numCharacters;
    ar >> moneyPooled;

    poolSack.Clear();
    if (globalData.SaveGameVersion < _VERSION_0661_)
    {
      ar >> temp; poolSack.Coins[PlatinumType] = temp;
      ar >> temp; poolSack.Coins[ElectrumType] = temp;
      ar >> temp; poolSack.Coins[GoldType] = temp;
      ar >> temp; poolSack.Coins[SilverType] = temp;
      ar >> temp; poolSack.Coins[CopperType] = temp;
      ar >> temp; { for (int g=0;g<temp;g++) poolSack.AddGem(); }
      ar >> temp; { for (int j=0;j<temp;j++) poolSack.AddJewelry(); }
      if (globalData.SaveGameVersion >= _VERSION_0660_)
      {
        ar >> temp; poolSack.Coins[CoinType6] = temp;
        ar >> temp; poolSack.Coins[CoinType7] = temp;
        ar >> temp; poolSack.Coins[CoinType8] = temp;
        ar >> temp; poolSack.Coins[CoinType9] = temp;
        ar >> temp; poolSack.Coins[CoinType10] = temp;
      }
    }
  }

  eventFlags.Serialize(ar);
  visitData.Serialize(ar);
  blockageData.Serialize(ar);  

  // not part of party! //  globalData.questData.Serialize(ar);

  if (ar.IsStoring())
  {
    ar << MAX_PARTY_MEMBERS;
    for (i=0;i<MAX_PARTY_MEMBERS;i++)
      characters[i].Serialize(ar, globalData.SaveGameVersion);
  }
  else
  {
    PrevRelFacing=-1;
    ActiveScripter=-1;

    ar >> temp;
    
    if (temp > MAX_PARTY_MEMBERS)
      WriteDebugString("Bogus value for MAX_PARTY_MEMBERS read from save file: %u\n", temp);

    temp = min( temp, MAX_PARTY_MEMBERS );
    
    for (i=0;i<temp;i++)
    {
      characters[i].Serialize(ar, globalData.SaveGameVersion);

      if (characters[i].GetType()!=NPC_TYPE)
        characters[i].origIndex = -1;
    }
  }  

  if (ar.IsStoring())
  {
    poolSack.Serialize(ar, globalData.SaveGameVersion);
    journal.Serialize(ar);
  }
  else
  {
    if (globalData.SaveGameVersion >= _VERSION_0661_)
      poolSack.Serialize(ar, globalData.SaveGameVersion);
    if (globalData.SaveGameVersion >= _VERSION_0780_)
      journal.Serialize(ar);
  }
    
  party_asl.Serialize(ar);

  // reset internal time keeping flags
  ProcessTimeSensitiveData(0);
}
#endif
//*****************************************************************************
// NAME:    PARTY::PartyHasItem
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
BOOL PARTY::PartyHasItem(int item)
{
  BOOL found = FALSE;
  int i;

  for (i=0;i<numCharacters && !found;i++)
  {
    if (characters[i].HasItem(item))
       found = TRUE;
  }

  return found;
}

//*****************************************************************************
// NAME:    PARTY::PartyInDaytime
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
BOOL PARTY::PartyInDaytime()
{ 
  return ((hours >= 6) && (hours <= 18));
}

//*****************************************************************************
// NAME:    PARTY::PartyIsSearching
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
BOOL PARTY::PartyIsSearching()
{
  return (searching);
}

//*****************************************************************************
// NAME:    PARTY::GetPartyFacing
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
int PARTY::GetPartyFacing()
{
  return (facing);
}

//*****************************************************************************
// NAME:    PARTY::PartyIsDetectingTraps
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
BOOL PARTY::PartyIsDetectingTraps()
{
  for (int i=0;i<numCharacters;i++)
  {
    if (characters[i].GetAdjDetectingTraps()) return TRUE;
  }
  return FALSE;
}

//*****************************************************************************
// NAME:    PARTY::PartyIsDetectingInvisible
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
BOOL PARTY::PartyIsDetectingInvisible()
{
  for (int i=0;i<numCharacters;i++)
  {
    if (characters[i].GetAdjDetectingInvisible()) return TRUE;
  }
  return FALSE;
}

//*****************************************************************************
// NAME:    PARTY::PartyIsDetectingMagic
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
BOOL PARTY::PartyIsDetectingMagic()
{
  for (int i=0;i<numCharacters;i++)
  {
    if (characters[i].GetAdjSpecAb(SA_DetectMagic)) return TRUE;
  }
  return FALSE;
}

//*****************************************************************************
// NAME:    PARTY::PartyHasGender
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
BOOL PARTY::PartyHasGender(genderType type)
{
  for (int i=0;i<numCharacters;i++)
  {
    if (characters[i].GetAdjGender()==type) return TRUE;
  }
  return FALSE;
}

//*****************************************************************************
//    NAME: PARTY::PartyHasNPC
//
// PURPOSE: 
//
//*****************************************************************************
BOOL PARTY::PartyHasNPC(int npckey)
{
  return (isNPCinParty(npckey));
}

//*****************************************************************************
// NAME:    PARTY::PartyHasClass
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
BOOL PARTY::PartyHasClass(classType ctype)
{
  BOOL found = FALSE;
  int i;

  for (i=0;!found && i<numCharacters;i++)
  {
     if (characters[i].GetAdjClass() == ctype)
        found = TRUE;
  }

  return found;
}

//*****************************************************************************
//    NAME: PARTY::PartyHasSpellMemorized
//
// PURPOSE: Determine if at least one party member has a 
//          particular spell memorized.
//
//*****************************************************************************
BOOL PARTY::PartyHasSpellMemorized(DWORD spellDbKey)
{
  for (int i=0;i<numCharacters;i++)
  {
    if (characters[i].HaveSpell(spellDbKey,TRUE))
      return TRUE;
  }

  return FALSE;
}

//*****************************************************************************
// NAME:    PARTY::PartyHasRace
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
BOOL PARTY::PartyHasRace(raceType rtype)
{
  BOOL found = FALSE;
  int i;

  for (i=0;!found && i<numCharacters;i++)
  {
     if (characters[i].race() == rtype)
        found = TRUE;
  }

  return found;
}

//*****************************************************************************
// NAME:    PARTY::GetHealingPointsNeeded
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
int PARTY::GetHealingPointsNeeded()
{
  int total = 0;
  int i;

  for (i=0;i<numCharacters;i++)
     total += characters[i].GetHealingPointsNeeded();

  return total;
}

// time in 24 hr format, does not affect current day
//*****************************************************************************
// NAME:    PARTY::setPartyTime
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
void PARTY::setPartyTime(int ptime)
{  
   int min = ptime % 100;
   int hr = ptime / 100;
   
   hours = hr;
   minutes = min;
}

//*****************************************************************************
// NAME:    PARTY::get24HourClock
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
int PARTY::get24HourClock()
{
#ifdef UAFEngine
   return ((hours*100)+minutes);
#else
   return globalData.startTime;
#endif
}

DWORD PARTY::getElapsedMinutes()
{
  return ( (days*24*60) + (hours*60) + minutes );
}

//*****************************************************************************
// NAME: incrementClock
//
// PURPOSE:  
//*****************************************************************************
void PARTY::incrementClock(int MinuteInc)
{  
   int mn, hr, day;
   
   if (MinuteInc <= 0)
      return;
   
   if (MinuteInc >= 60)
   {
      day = MinuteInc / (1440);
      MinuteInc %= (1440);
      hr  = MinuteInc / 60;
      MinuteInc %= 60;
      mn  = MinuteInc;
   }
   else 
   {
      day = 0;
      hr = 0;
      mn = MinuteInc;
   }
   
   minutes += mn;
   
   if (minutes >= 60)
   { 
      minutes -= 60;
      hr++;
   }
   
   hours += hr;
   
   if (hours >= 24)
   {
      hours -= 24;
      day++;
   }

   //days += day;

   for (int d=0; d < day; d++)
   {
     days += 1;

     int DayOfYear = days % 365;

     // then start checking character birthdates
     for (int i=0;i<numCharacters;i++)
     {
       int bday = characters[i].GetBirthday();
       if (bday <= 0) bday = 1;

       if (DayOfYear == bday)
       {
         characters[i].SetAge( characters[i].GetAge()+1 );
         WriteDebugString("%s has aged one year\n", characters[i].GetName());
       }
     }
   }
}


//*****************************************************************************
// NAME:    PARTY::CanPassBlockage
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
BOOL PARTY::CanPassBlockage(BlockageType type, int currfacing)
{
  int key;
  switch (type)
  {
  case OpenBlk:
    return TRUE; // can always pass

  case OpenSecretBlk:
    return (!blockageData.IsSecret(GetCurrentLevel(), Posx, Posy, currfacing));  // can pass if secret door already discovered

  case LockedBlk:             // can pass if lock was picked successfully
    return (!blockageData.IsLocked(GetCurrentLevel(), Posx, Posy, currfacing));  // can pass if secret door already discovered

  case LockedSecretBlk:       // if found secret door, then picked lock
    return (!blockageData.IsLockedSecret(GetCurrentLevel(), Posx, Posy, currfacing));  // can pass if secret door already discovered

  case LockedWizardBlk:       // if cast unlock spell
    return (!blockageData.IsSpelled(GetCurrentLevel(), Posx, Posy, currfacing));  // can pass if secret door already discovered

  case LockedWizardSecretBlk: // if found secret door, then cast unlock spell
    return (!blockageData.IsSpelledSecret(GetCurrentLevel(), Posx, Posy, currfacing));  // can pass if secret door already discovered

  case BlockedBlk:
  case FalseDoorBlk:
    return FALSE;  // can never pass

  // Can pass if party has special key that allows passage.
  case LockedKey1:
    if (globalData.version < _VERSION_0842_)
      key = globalData.keyData.GetItemKeyUsingSequence(1);
    else
      key = levelData.blockageKeys.SpecialKeys[0];
    return (hasSpecialKey(key));

  case LockedKey2:
    if (globalData.version < _VERSION_0842_)
      key = globalData.keyData.GetItemKeyUsingSequence(2);
    else
      key = levelData.blockageKeys.SpecialKeys[1];
    return (hasSpecialKey(key));

  case LockedKey3:
    if (globalData.version < _VERSION_0842_)
      key = globalData.keyData.GetItemKeyUsingSequence(3);
    else
      key = levelData.blockageKeys.SpecialKeys[2];
    return (hasSpecialKey(key));

  case LockedKey4:
    if (globalData.version < _VERSION_0842_)
      key = globalData.keyData.GetItemKeyUsingSequence(4);
    else
      key = levelData.blockageKeys.SpecialKeys[3];
    return (hasSpecialKey(key));

  case LockedKey5:
    if (globalData.version < _VERSION_0842_)
      key = globalData.keyData.GetItemKeyUsingSequence(5);
    else
      key = levelData.blockageKeys.SpecialKeys[4];
    return (hasSpecialKey(key));

  case LockedKey6:
    if (globalData.version < _VERSION_0842_)
      key = globalData.keyData.GetItemKeyUsingSequence(6);
    else
      key = levelData.blockageKeys.SpecialKeys[5];
    return (hasSpecialKey(key));

  case LockedKey7:
    if (globalData.version < _VERSION_0842_)
      key = globalData.keyData.GetItemKeyUsingSequence(7);
    else
      key = levelData.blockageKeys.SpecialKeys[6];
    return (hasSpecialKey(key));

  case LockedKey8:
    if (globalData.version < _VERSION_0842_)
      key = globalData.keyData.GetItemKeyUsingSequence(8);
    else
      key = levelData.blockageKeys.SpecialKeys[7];
    return (hasSpecialKey(key));
  }
  return FALSE;
}

int PARTY::RelativeToActualFacing(int relativeDirection)
{
  // relative dir works according to:
  //
  // 4  0  5
  // 3     1
  // 6  2  7
  //
  // which follows facing directions in externs.h 

  if ((relativeDirection < 0)||(relativeDirection > 7)) return -1;
  if (!globalData.levelInfo.stats[globalData.currLevel].overland)
    return ((facing+relativeDirection)&3); // no diagonal in dungeons
  else
    return ((relativeDirection)&7);
}

BOOL PARTY::newForwardPosition(int relativeDirection, int &newX, int &newY)
{
   int oldX = Posx;
   int oldY = Posy;
   BlockageType type = BlockedBlk;
   BOOL CanMove = FALSE;

   char temp[30];
   char dirChar = 'N';
   int ActualFacing = RelativeToActualFacing(relativeDirection);

   if (!globalData.levelInfo.stats[globalData.currLevel].overland)
   {
     switch (ActualFacing) 
     {      
     case FACE_NORTH:
        type = levelData.area[Posy][Posx].northBlockage;
	      dirChar='N';
        break;      
     case FACE_EAST:
        type = levelData.area[Posy][Posx].eastBlockage;
	      dirChar='E';
        break;      
     case FACE_SOUTH:
         type = levelData.area[Posy][Posx].southBlockage;
	       dirChar='S';
        break;      
     case FACE_WEST:
        type = levelData.area[Posy][Posx].westBlockage;
	      dirChar='W';
        break;
     }     
	   
     sprintf(temp,"$Block,%d,%d,%c",Posx,Posy,dirChar);
	   if (globalData.levelInfo.stats[globalData.currLevel].level_asl.Exists(temp)) 
	   {
		   CString blkstr;
		   blkstr=globalData.levelInfo.stats[globalData.currLevel].level_asl.Lookup(temp) ;
		   if (blkstr=="OpenBlk") type=OpenBlk;
	   }
     CanMove = CanPassBlockage(type, ActualFacing);
   }
   else
   {
     int tempX = Posx;
     int tempY = Posy;
     // check blockage in dest square for overland
     switch (ActualFacing)
     {
     case FACE_NORTH: tempY--; break;
     case FACE_EAST:  tempX++; break;
     case FACE_SOUTH: tempY++; break;
     case FACE_WEST:  tempX--; break;
     case FACE_NW:    tempY--; tempX--; break;
     case FACE_NE:    tempY--; tempX++; break;
     case FACE_SW:    tempY++; tempX--; break;
     case FACE_SE:    tempY++; tempX++; break;
     }     
     if (tempX < 0) tempX=0;
     if (tempY < 0) tempY=0;
     if (tempX >= levelData.area_width)  tempX=levelData.area_width-1;         
     if (tempY >= levelData.area_height) tempY=levelData.area_height-1;
     // if able to move into dest square
     if ((oldX != tempX) || (oldY != tempY))
       type = (levelData.area[tempY][tempX].bkgrnd > 0) ? BlockedBlk : OpenBlk;
     else
       type = BlockedBlk;
     CanMove = (type == OpenBlk);
   }   

   //if (CanMove)
   {
     if (globalData.levelInfo.stats[globalData.currLevel].overland)
     {
       newX = Posx;
       newY = Posy;
       // don't wrap party to opposite side of map when using overland
       switch (ActualFacing)
       {      
       case FACE_NORTH: newY--; break;
       case FACE_EAST:  newX++; break;
       case FACE_SOUTH: newY++; break;
       case FACE_WEST:  newX--; break;
       case FACE_NW:    newY--; newX--; break;
       case FACE_NE:    newY--; newX++; break;
       case FACE_SW:    newY++; newX--; break;
       case FACE_SE:    newY++; newX++; break;
       }     
       if (newX < 0) newX=0;
       if (newY < 0) newY=0;
       if (newX >= levelData.area_width)  newX=levelData.area_width-1;         
       if (newY >= levelData.area_height) newY=levelData.area_height-1;
       if ((oldX == newX) && (oldY == newY)) CanMove=FALSE;
     }
     else
     {
       ActualFacing &= 3;
       newY=(Posy+deltaY[ActualFacing]+levelData.area_height)%levelData.area_height;
       newX=(Posx+deltaX[ActualFacing]+levelData.area_width)%levelData.area_width;
     }
   }

   return CanMove;
}

BOOL PARTY::movePartyForward(int relativeDirection) // 0=facing, 1=right.etc
{
  int oldX = Posx;
  int oldY = Posy;
  int newX;
  int newY;

  BOOL CanMove = newForwardPosition(relativeDirection, newX, newY);
  if (CanMove)
  {
    Posx = newX;
    Posy = newY;
    PrevPosx=oldX;
    PrevPosy=oldY;
    PrevRelFacing=relativeDirection;
  }
  return CanMove;
}


//*****************************************************************************
// NAME: movePartyBackward
//
// PURPOSE:  
//*****************************************************************************
BOOL PARTY::movePartyBackward(void)
{
   int oldX = Posx;
   int oldY = Posy;
   int currFace = facing;

   BlockageType type = BlockedBlk;
   BOOL CanMove = FALSE;
   BOOL UsePrevPos = TRUE;

   if ( (abs(PrevPosx-Posx) > 1) || (abs(PrevPosy-Posy) > 1) )
     UsePrevPos=FALSE;
   if (!levelData.ValidAreaMapCoords(PrevPosx,PrevPosy))
     UsePrevPos=FALSE;

   if (UsePrevPos)
   {
#ifdef UAFEngine
     switch (GetDir(Posx,Posy, PrevPosx,PrevPosy))
     {
     case PathNorth: currFace=FACE_SOUTH; break;
     case PathSouth: currFace=FACE_NORTH; break;
     case PathEast:  currFace=FACE_WEST;  break;
     case PathWest:  currFace=FACE_EAST;  break;
     }
#endif
   }

   if (!globalData.levelInfo.stats[globalData.currLevel].overland)
   {
     switch (currFace) 
     {      
     case FACE_NORTH: type = levelData.area[Posy][Posx].southBlockage; break;      
     case FACE_EAST:  type = levelData.area[Posy][Posx].westBlockage;  break;      
     case FACE_SOUTH: type = levelData.area[Posy][Posx].northBlockage; break;      
     case FACE_WEST:  type = levelData.area[Posy][Posx].eastBlockage;  break;
     }
     CanMove = CanPassBlockage(type, currFace);
   }
   else
   {
     int newX = Posx;
     int newY = Posy;
     // check blockage in next square for overland
     switch (currFace) 
     {
     case FACE_NORTH: newY++; break;      
     case FACE_EAST:  newX--; break;      
     case FACE_SOUTH: newY--; break;      
     case FACE_WEST:  newX++; break;
     }     
     if (newX < 0) newX=0;
     if (newY < 0) newY=0;
     if (newX >= levelData.area_width)  newX=levelData.area_width-1;         
     if (newY >= levelData.area_height) newY=levelData.area_height-1;
     // if able to move into next square
     if ((oldX != newX) || (oldY != newY))
       type = (levelData.area[newY][newX].bkgrnd > 0) ? BlockedBlk : OpenBlk;
     else
       type = BlockedBlk;
     CanMove = (type == OpenBlk);
   }

   if (CanMove)
   {
     switch (currFace) 
     {      
     case FACE_NORTH: Posy++; break;      
     case FACE_EAST:  Posx--; break;      
     case FACE_SOUTH: Posy--; break;      
     case FACE_WEST:  Posx++; break;
     }

     // don't wrap party to opposite side of map when moving backwards
     if (Posx < 0) Posx=0;
     if (Posy < 0) Posy=0;
     if (Posx >= levelData.area_width)  Posx=levelData.area_width-1;         
     if (Posy >= levelData.area_height) Posy=levelData.area_height-1;
     if ((oldX == Posx) && (oldY == Posy)) CanMove=FALSE;
   }
   else
     PlayPartyBump();

   if (CanMove)
   {
     PrevPosx=oldX;
     PrevPosy=oldY;
   }
   return CanMove;
}


//*****************************************************************************
// NAME: turnPartyLeft
//
// PURPOSE:  
//*****************************************************************************
void PARTY::turnPartyLeft(void)
{
   if (facing == FACE_NORTH)
      facing = FACE_WEST;
   else if (facing == FACE_EAST)
      facing = FACE_NORTH;
   else if (facing == FACE_SOUTH)
      facing = FACE_EAST;
   else if (facing == FACE_WEST)
      facing = FACE_SOUTH;
}


//*****************************************************************************
// NAME: turnPartyRight
//
// PURPOSE:  
//*****************************************************************************
void PARTY::turnPartyRight(void)
{
   if (facing == FACE_NORTH)
      facing = FACE_EAST;
   else if (facing == FACE_EAST)
      facing = FACE_SOUTH;
   else if (facing == FACE_SOUTH)
      facing = FACE_WEST;
   else if (facing == FACE_WEST)
      facing = FACE_NORTH;
}


//*****************************************************************************
// NAME: turnParty180
//
// PURPOSE:  
//*****************************************************************************
void PARTY::turnParty180()
{
   switch (facing) 
   {
   case FACE_NORTH:
      facing = FACE_SOUTH;
      break;
   case FACE_EAST:
      facing = FACE_WEST;
      break;
   case FACE_SOUTH:
      facing = FACE_NORTH;
      break;
   case FACE_WEST:
      facing = FACE_EAST;
      break;
   }
}


//*****************************************************************************
// NAME: removeCharacter
//
// PURPOSE:  
//*****************************************************************************
void PARTY::removeCharacter(int loser, BOOL AddToTemps)
{  
   ASSERT( loser >= 0 );
   if (loser < 0) return;

   if ((loser >= 0) && (loser < numCharacters))
   {
      // not permanently deleted, add dude
      // back into the temp dude list
     CHARACTER tempCHARACTER;
     tempCHARACTER = characters[loser];

     if (tempCHARACTER.GetType() == CHAR_TYPE)
     {
       tempCHARACTER.serializeCharacter(TRUE, tempCHARACTER.GetName());
       AddToTemps=FALSE;
     }
     else
     {
       //if ((tempCHARACTER.GetType() == NPC_TYPE) && (!tempCHARACTER.IsPreGen))
       {
         tempCHARACTER.serializeCharacter(TRUE, tempCHARACTER.GetName());
         AddToTemps=FALSE;
       }
     }

      if (loser == numCharacters-1)
      {
         characters[loser].Clear();
      }
      else 
      {    
         for (WORD i=loser;i<(numCharacters-1);i++)
            characters[i] = characters[i+1];         
         characters[i].Clear();
      }
      
      numCharacters--;
      if (activeCharacter>=numCharacters)
        activeCharacter=0;

#ifdef UAFEngine
      // must remove from party before adding back
      // into temps
      if (AddToTemps)
        tempChars.addTempCharacter(tempCHARACTER);
#endif
   }
}


//*****************************************************************************
// NAME: purgeCharacter
//
// PURPOSE:  
//*****************************************************************************
void PARTY::purgeCharacter(int loser)
{
   char fullPath[_MAX_PATH+1];     

   if ((loser >= 0) && (loser < numCharacters))
       //&& (characters[loser].GetType() != NPC_TYPE))
   {
     if (characters[loser].GetName() == "") return;

     strcpy(fullPath, GetSavePath());
     if (characters[loser].GetType()==NPC_TYPE) strcat(fullPath,"DCNPC_");
     strcat(fullPath, characters[loser].GetName());
     strcat(fullPath, ".chr");
     if (!DeleteFile(fullPath))
       WriteDebugString("Cannot delete character file %s\n", fullPath);

      if (characters[loser].icon.filename != "")
      {
        CString tmpPath;
        CString newName="";
        CString tmp;
        tmpPath = GetSavePath();
        if (characters[loser].GetType()==NPC_TYPE) newName += "DCNPC_";
        newName += characters[loser].GetName();
        newName += "_icon";
        tmp = characters[loser].icon.filename;
        StripFilenamePath(tmp);
        int index = tmp.Find('.', 0);
        if (index > 0) newName += tmp.Right(tmp.GetLength()-(index));
        tmpPath += newName;
        SetFileAttributes(tmpPath, FILE_ATTRIBUTE_NORMAL);
        if (!DeleteFile(tmpPath))
          WriteDebugString("Cannot delete character icon file %s\n", tmpPath);
      }
      if (characters[loser].smallPic.filename != "")
      {
        CString tmpPath;
        CString newName="";
        CString tmp;
        tmpPath = GetSavePath();
        if (characters[loser].GetType()==NPC_TYPE) newName += "DCNPC_";
        newName += characters[loser].GetName();
        newName += "_spic";
        tmp = characters[loser].smallPic.filename;
        StripFilenamePath(tmp);
        int index = tmp.Find('.', 0);
        if (index > 0) newName += tmp.Right(tmp.GetLength()-(index));
        tmpPath += newName;
        SetFileAttributes(tmpPath, FILE_ATTRIBUTE_NORMAL);
        if (!DeleteFile(tmpPath))
          WriteDebugString("Cannot delete character smallPic file %s\n", tmpPath);
      }

      if (loser == numCharacters-1)
      {
         characters[loser].Clear();
      }
      else 
      {    
         for (WORD i=loser;i<(numCharacters-1);i++)
            characters[i] = characters[i+1]; 
         
         characters[i].Clear();
      }
      
      numCharacters--;

      if (activeCharacter>=numCharacters)
        activeCharacter=0;
   }
}

//*****************************************************************************
// NAME: TakePartyItems
//
// PURPOSE:  Remove party goods based on event data
//*****************************************************************************
void PARTY::TakePartyItems(TAKE_PARTY_ITEMS_DATA &data)
{
  int qty = 0;
  int dude=-1;

  switch (data.takeAffects)
  {
  case TakeAffectsParty:
    dude = -1;
    break;
  case TakeAffectsRndChar:
    dude = (rollDice(numCharacters, 1) - 1);
    break;
  case TakeAffectsActiveChar:
    dude = activeCharacter;
    break;
  }

  if (data.takeItems & TakeInventoryAction)
  {
    switch (data.itemSelectFlags)
    {
    case TakeSpecifiedQty:
      {
        POSITION pos = data.items.GetHeadPosition();
        while (pos != NULL)
        {
          ITEM &tempITEM = data.items.GetAt(pos);
          if (dude >= 0)
          {
            int count = 0;
            while (count < data.itemPcnt)
            {
              int key = characters[dude].myItems.GetListKeyByItemKey(tempITEM.item);
              if (key >= 0)
              {
                count++;
                if (characters[dude].delCharacterItem(key, 1))
                {
                  if (data.StoreItems)
                  {
                    tempITEM.qty=1;
                    globalData.vault[data.WhichVault].items.AddItem(tempITEM);
                  }
                }
              }
              else
                break;
            }
          }
          else // whole party
          {
            for (int i=0;i<numCharacters;i++)
            {
              int count = 0;
              while (count < data.itemPcnt)
              {
                int key = characters[i].myItems.GetListKeyByItemKey(tempITEM.item);
                if (key >= 0)
                {
                  count++;
                  if (characters[i].delCharacterItem(key, 1))
                  {
                    if (data.StoreItems)
                    {
                      tempITEM.qty=1;
                      globalData.vault[data.WhichVault].items.AddItem(tempITEM);
                    }
                  }
                }
                else
                  break;
              }
            }
          }
          data.items.GetNext(pos);
        }
      }
      break;

    case TakeRndQty:    
    case TakePcntQty:
      // not used for items
      break;

    case TakeAll:     
      {
        if (dude >= 0)
        {
          POSITION pos = characters[dude].myItems.GetHeadPosition();
          while (pos != NULL)
          {
            ITEM &tempITEM = characters[dude].myItems.GetNext(pos);
            if (characters[dude].myItems.DeleteItem(tempITEM.key))
            {
              if (data.StoreItems)
                globalData.vault[data.WhichVault].items.AddItem(tempITEM);
            }
            // DeleteItem() removes the item from the list, so this
            // next call won't work.
            //characters[dude].myItems.GetNext(pos);
          }
        }
        else
        {
          for (int i=0;i<numCharacters;i++)
          {
            POSITION pos = characters[i].myItems.GetHeadPosition();
            while (pos != NULL)
            {
              ITEM &tempITEM = characters[i].myItems.GetNext(pos);
              if (characters[i].myItems.DeleteItem(tempITEM.key))
              {
                if (data.StoreItems)
                  globalData.vault[data.WhichVault].items.AddItem(tempITEM);
              }
              //characters[i].myItems.GetNext(pos);
            }
          }
        }
      }
      break;
    }
  }
  
  if (data.takeItems & TakeGemsAction)
  {
    if (dude >= 0)
    {
      int availgems = characters[dude].money.NumGems();
      qty = TakePartyItemQty(dude, data.gemsSelectFlags, data.gems, availgems);
      if ((data.StoreItems) && (qty > 0))
      {
        POSITION pos = characters[dude].money.Gems.GetHeadPosition();
        int i=0;
        while ((i < qty) && (pos != NULL))
        {
          GEM_TYPE gem = characters[dude].money.Gems.GetAt(pos);
          globalData.vault[data.WhichVault].money.AddGem(gem);
          characters[dude].money.Gems.GetNext(pos);
          i++;
        }
      }
      characters[dude].money.RemoveMultGems(qty);
    }
    else
    {
      for (int i=0;i<numCharacters;i++)
      {
        int availgems = characters[i].money.NumGems();
        qty = TakePartyItemQty(i, data.gemsSelectFlags, data.gems, availgems);
        if ((data.StoreItems) && (qty > 0))
        {
          POSITION pos = characters[i].money.Gems.GetHeadPosition();
          int i=0;
          while ((i < qty) && (pos != NULL))
          {
            GEM_TYPE gem = characters[i].money.Gems.GetAt(pos);
            globalData.vault[data.WhichVault].money.AddGem(gem);
            characters[i].money.Gems.GetNext(pos);
            i++;
          }
        }
        characters[i].money.RemoveMultGems(qty);
      }
    }
  }

  if (data.takeItems & TakeJewelryAction)
  {
    if (dude >= 0)
    {
      int availgems = characters[dude].money.NumJewelry();
      qty = TakePartyItemQty(dude, data.jewelrySelectFlags, data.jewelry, availgems);
      if ((data.StoreItems) && (qty > 0))
      {
        POSITION pos = characters[dude].money.Jewelry.GetHeadPosition();
        int i=0;
        while ((i < qty) && (pos != NULL))
        {
          GEM_TYPE gem = characters[dude].money.Jewelry.GetAt(pos);
          globalData.vault[data.WhichVault].money.AddJewelry(gem);
          characters[dude].money.Jewelry.GetNext(pos);
          i++;
        }
      }
      characters[dude].money.RemoveMultJewelry(qty);
    }
    else
    {
      for (int i=0;i<numCharacters;i++)
      {
        int availgems = characters[i].money.NumJewelry();
        qty = TakePartyItemQty(i, data.jewelrySelectFlags, data.jewelry, availgems);
        if ((data.StoreItems) && (qty > 0))
        {
          POSITION pos = characters[i].money.Jewelry.GetHeadPosition();
          int i=0;
          while ((i < qty) && (pos != NULL))
          {
            GEM_TYPE gem = characters[i].money.Jewelry.GetAt(pos);
            globalData.vault[data.WhichVault].money.AddJewelry(gem);
            characters[i].money.Jewelry.GetNext(pos);
            i++;
          }
        }
        characters[i].money.RemoveMultJewelry(qty);
      }
    }
  }

  if (data.takeItems & TakeMoneyAction)
  {
    // convert money type to base
    qty = globalData.moneyData.ConvertToBase(data.platinum, data.moneyType);

    if (dude >= 0)
    {
      int avail = characters[dude].money.Total();//globalData.moneyData.ConvertToBase(characters[dude].poolCharacterGold(), globalData.moneyData.GetDefaultType());
      int taken = TakePartyItemQty(dude, data.platinumSelectFlags, qty, avail);
      if (taken > 0)
      {
        if (data.StoreItems)
        {
          int even = 0;
          double remain = 0.0;

          even = globalData.moneyData.Convert(taken, globalData.moneyData.GetBaseType(), data.moneyType, &remain);
          globalData.UpdateMoneyInVault(data.WhichVault, data.moneyType, even);
          globalData.UpdateMoneyInVault(data.WhichVault,globalData.moneyData.GetBaseType(), remain);
        }
        // remove money from character
        characters[dude].payForItem(taken, globalData.moneyData.GetBaseType());
      }
    }
    else // dude = -1, so take equally from all party members
    {
      for (int i=0;i<numCharacters;i++)
      {
        int avail = characters[i].money.Total();//globalData.moneyData.ConvertToBase(characters[i].poolCharacterGold(), globalData.moneyData.GetDefaultType());
        int taken = TakePartyItemQty(i, data.platinumSelectFlags, qty, avail);
        if (taken > 0)
        {
          if (data.StoreItems)
          {
            int even = 0;
            double remain = 0.0;
            even = globalData.moneyData.Convert(taken, globalData.moneyData.GetBaseType(), data.moneyType, &remain);
            globalData.UpdateMoneyInVault(data.WhichVault,data.moneyType, even);
            globalData.UpdateMoneyInVault(data.WhichVault,globalData.moneyData.GetBaseType(), remain);
          }

          // remove money from character
          characters[i].payForItem(taken, globalData.moneyData.GetBaseType());
        }
      }
    }
  }

  if (dude >= 0)
  {
    characters[dude].SetEncumbrance(characters[dude].determineEffectiveEncumbrance());
  	characters[dude].determineMaxMovement();
  }
  else
  {
    for (int i=0;i<numCharacters;i++)
    {
      characters[i].SetEncumbrance(characters[i].determineEffectiveEncumbrance());
  	  characters[i].determineMaxMovement();
    }
  }

}

int PARTY::TakePartyItemQty(int dude, takeItemQtyType type, int amount, int &data)
{
  int qty=0;
  if (data == 0)
    return 0;

  switch (type)
  {
  case TakeSpecifiedQty:
    qty = amount;
    qty = min( qty, data);
    data -= qty;
    break;

  case TakeRndQty:
    qty = rollDice(amount, 1);
    qty = min( qty, data);
    data -= qty;
    break;

  case TakePcntQty:      
    qty = (int) ((double)data * ((double)amount / 100.0) );
    qty = min( qty, data);
    data -= qty;
    break;

  case TakeAll:
    qty = data;
    data -= qty;
    break;
  } 

  return qty;
}

int PARTY::GetNextMarchOrder()
{
  int i;
  int used[MAX_PARTY_MEMBERS];
  memset(used, MAX_PARTY_MEMBERS+1, sizeof(used)); // set to impossible value

  // collect current marchOrder values
  for (i=0;i<numCharacters;i++)
    used[i]=characters[i].marchOrder;

  // Search for value that isn't already used by party member.
  for (i=0;i<MAX_PARTY_MEMBERS;i++)
  {
    bool found=false;
    for (int u=0; (u<MAX_PARTY_MEMBERS) && (!found); u++)
    {
      if (used[u]==i)
        found = true;
    }
    if (!found) return i;
  }
  ASSERT(FALSE); // should not be possible to reach this
  return numCharacters; // old default behavior
}

#ifdef UAFEngine
//*****************************************************************************
// NAME: addNPCToParty
//
// PURPOSE:  
//*****************************************************************************
BOOL PARTY::addNPCToParty(int npcIndex, int hitPointMod, BOOL useOrig)
{ 
   if (!globalData.charData.HaveKey(npcIndex))
   {
     WriteDebugString("Invalid npcIndex %i in PARTY::addNPCToParty()\n", npcIndex);
     return FALSE;
   }

   if (CanAddNPC())
   {
      // adjust base NPC morale using highest charisma
      // score in the party
      int HiCharisma = 0;
      int MoraleMod = 0;
      for (int i=0;i<numCharacters;i++)
      {
        BYTE pc_cha = characters[i].GetAdjCha();
        if (pc_cha > HiCharisma) HiCharisma = pc_cha;
      }

      switch (HiCharisma)
      {
      case 3: MoraleMod = -30;break;
      case 4: MoraleMod = -25;break;
      case 5: MoraleMod = -20;break;
      case 6: MoraleMod = -15;break;
      case 7: MoraleMod = -10;break;
      case 8: MoraleMod = -5;break;
      case 14: MoraleMod = 5;break;
      case 15: MoraleMod = 15;break;
      case 16: MoraleMod = 20;break;
      case 17: MoraleMod = 30;break;
      case 18: MoraleMod = 40;break;
      default: break; // 9..13
      }

      double percent = ((double)hitPointMod / (double)100);

      CHARACTER data;      
      if (globalData.charData.GetCharacterData(npcIndex, data))
      {
        if (!useOrig)
        {
          CHARACTER TEMPchar;
          // check for saved NPC first (will have more recent data)
          TEMPchar=data;
          if (TEMPchar.serializeCharacter(FALSE, TEMPchar.GetName()))
            data = TEMPchar;
        }

        characters[numCharacters] = data;
        characters[numCharacters].marchOrder = GetNextMarchOrder();
        characters[numCharacters].origIndex = npcIndex;
        characters[numCharacters].SetType(NPC_TYPE);
        characters[numCharacters].SetPartyMember();        
        characters[numCharacters].SetHitPoints(
           (((double)characters[numCharacters].GetMaxHitPoints()) * percent));
        if (characters[numCharacters].GetAdjHitPoints() > 0)
          characters[numCharacters].SetStatus(Okay);
        else
          characters[numCharacters].SetStatus(Unconscious);
        characters[numCharacters].SetMorale(characters[numCharacters].GetAdjMorale() + MoraleMod);
        numCharacters++;
        return TRUE;
      }
      else
      {
        WriteDebugString("Failed GetCharacterData(%i) in PARTY::addNPCToParty()\n", npcIndex);
        return FALSE;
      }
   }
   else
     WriteDebugString("Failed CanAddNPC() in PARTY::addNPCToParty()\n");

   return FALSE;
}
#endif
//*****************************************************************************
// NAME: removeNPCFromParty
//
// PURPOSE:  
//*****************************************************************************
void PARTY::removeNPCFromParty(int npcIndex)
{
   int i=0;
   BOOL found = FALSE;

   while ((i<numCharacters) && (!found))
   {
      if (characters[i].GetType()==NPC_TYPE)      
      {
        if (characters[i].origIndex == npcIndex)
        {
          found = TRUE;
          removeCharacter(i, FALSE);
        }
      }  
      i++;
   }

   if (!found)
     WriteDebugString("RemoveNPCFromParty for char %i, no matching npc found\n", npcIndex);
}

//*****************************************************************************
// NAME: isNPCinParty
//
// PURPOSE:  
//*****************************************************************************
BOOL PARTY::isNPCinParty(int npcIndex)
{
  int i=0;
  BOOL found = FALSE;

  while ((i<numCharacters) && (!found))
  {
    if (characters[i].GetType() == NPC_TYPE)
    {
      if (characters[i].origIndex == npcIndex)
         found = TRUE;
    }
    i++;
  }

  return found;
}


void PARTY::deductPoolMoney(itemClassType type, int qty)
{
  if (!itemIsMoney(type)) return;

  if (!moneyPooled)
    return;

  switch (type) 
  {
  case GemType:
    poolSack.RemoveMultGems(qty);
    break;
  case JewelryType:
    poolSack.RemoveMultJewelry(qty);
    break;
  default:
    poolSack.Subtract(type , qty);
    break;
  }  
}


BOOL PARTY::PartyHasMoney()
{
  if (moneyPooled)
    return (!poolSack.IsEmpty());

  for (int i=0; i<numCharacters;i++)
  {
    if (!characters[i].money.IsEmpty()) 
      return TRUE;
  }
  return FALSE;
}

//*****************************************************************************
// NAME: PARTY::GetPoolGoldValue
//
// PURPOSE:  
//*****************************************************************************
int PARTY::GetPoolGoldValue()
{
  return poolSack.ConvertToDefault(poolSack.Total(), poolSack.GetBaseType());
}

//*****************************************************************************
// NAME: poolPartyGold
//
// PURPOSE:  
//*****************************************************************************
void PARTY::poolPartyGold()
{
   int i;   
   for (i=0; i<numCharacters;i++)
   {
     poolSack.Transfer(characters[i].money);
	   characters[i].SetEncumbrance(characters[i].determineEffectiveEncumbrance());
	   characters[i].determineMaxMovement();
   }
   
   moneyPooled = !poolSack.IsEmpty();
}

//*****************************************************************************
//    NAME: PARTY::AutoUpConvertPoolMoney
//
// PURPOSE: 
//
//*****************************************************************************
void PARTY::AutoUpConvertPoolMoney()
{
  if (!moneyPooled) return;  
  poolSack.AutoUpConvert();
}

//*****************************************************************************
// NAME: sharePartyGold
//
// PURPOSE:  
//*****************************************************************************
void PARTY::sharePartyGold()
{
   int tot = 0;
   int temp = 0;
   int i;
   int avail;
   int shared = 0;
   
   if (!moneyPooled)
      return;

   if (numCharacters == 0)
   {
     poolSack.Clear();
     moneyPooled=FALSE;
     return;
   }
   
   int c;
   for (c=0; c<poolSack.NumCoinTypes();c++)
   {
     shared = 0;
     itemClassType type = globalData.moneyData.GetItemClass(c);
     avail = poolSack[type];


     if (avail > 0)
     {
        int moneyweight = globalData.moneyData.GetWeight();
        if (moneyweight <= 0) moneyweight = 1;
        temp = avail % (numCharacters);
        tot = avail / (numCharacters);
      
        for (i=0; i<numCharacters;i++)
        {
          int maxenc = characters[i].GetAdjMaxEncumbrance();
          int curenc = characters[i].GetEncumbrance();

          for (int t=0;t<tot;t++)
          {
            // make sure allowed encumbrance is not exceeded            
            if (curenc <= maxenc)
            {
              characters[i].money.Add(type, 1);              
              shared += 1;
            }
            else
              break;

            if (((t+1) % (moneyweight*10)) == 0) // every 10 units of encumbrance
            {
              characters[i].SetEncumbrance(characters[i].determineEffectiveEncumbrance());
              curenc = characters[i].GetEncumbrance();
            }
          }
          characters[i].SetEncumbrance(characters[i].determineEffectiveEncumbrance());
        }
      
        for (i=0; i<temp;i++, temp--)
        {
          if (characters[i].GetEncumbrance() <= characters[i].GetAdjMaxEncumbrance())
          {
            characters[i].money.Add(type, 1);
            characters[i].SetEncumbrance(characters[i].determineEffectiveEncumbrance());
            shared += 1;
          }
        }
     }
     if (shared > 0)
       poolSack.Subtract(globalData.moneyData.GetItemClass(c), shared);
   }

   shared = 0;
   avail = poolSack.NumGems();
   if (avail > 0)
   {
      temp = avail % (numCharacters);
      tot = avail / (numCharacters);
    
      for (i=0; i<numCharacters;i++)
      {
        int maxenc = characters[i].GetAdjMaxEncumbrance();
        for (int g=0;g<tot;g++)
        {
          if (characters[i].GetEncumbrance() <= maxenc)
          {
            characters[i].money.AddGem();
            shared += 1;
          }
          else
            break;

          if (((g+1) % 10) == 0) // every 10
            characters[i].SetEncumbrance(characters[i].determineEffectiveEncumbrance());
        }
        characters[i].SetEncumbrance(characters[i].determineEffectiveEncumbrance());
      }    
      for (i=0; i<temp;i++, temp--)
      {
        if (characters[i].GetEncumbrance() <= characters[i].GetAdjMaxEncumbrance())
        {
          characters[i].money.AddGem();
          characters[i].SetEncumbrance(characters[i].determineEffectiveEncumbrance());
          shared += 1;
        }
      }
   }
   if (shared > 0)
     poolSack.RemoveMultGems(shared);

   shared = 0;
   avail = poolSack.NumJewelry();
   if (avail > 0)
   {
      temp = avail % (numCharacters);
      tot = avail / (numCharacters);
    
      for (i=0; i<numCharacters;i++)
      {
        int maxenc = characters[i].GetAdjMaxEncumbrance();
        for (int g=0;g<tot;g++)
        {
          if (characters[i].GetEncumbrance() <= maxenc)
          {
            characters[i].money.AddJewelry();
            shared += 1;
          }
          else
            break;

          if (((g+1) % 10) == 0) // every 10
            characters[i].SetEncumbrance(characters[i].determineEffectiveEncumbrance());
        }
        characters[i].SetEncumbrance(characters[i].determineEffectiveEncumbrance());
      }    
      for (i=0; i<temp;i++, temp--)
      {
        if (characters[i].GetEncumbrance() <= characters[i].GetAdjMaxEncumbrance())
        {
          characters[i].money.AddJewelry();
          characters[i].SetEncumbrance(characters[i].determineEffectiveEncumbrance());
          shared += 1;
        }
      }
   }
   if (shared > 0)
     poolSack.RemoveMultJewelry(shared);

   for (i=0; i<numCharacters;i++)
   {
	   //characters[i].SetEncumbrance(characters[i].determineEffectiveEncumbrance());
	   characters[i].determineMaxMovement();
   }

   moneyPooled = (poolSack.Total() > 0);
  //poolSack.Clear();
}


//*****************************************************************************
// NAME: setPartyAdventureState
//
// PURPOSE:  
//*****************************************************************************
void PARTY::setPartyAdventureState()
{
   level = GetCurrentLevel();
   adventuring = TRUE;
   //sharePartyGold();
}


//*****************************************************************************
// NAME: setPartyLevelState
//
// PURPOSE:  
//*****************************************************************************
void PARTY::setPartyLevelState(int plevel, int x, int y, int f)
{
   level = (BYTE) plevel;
   searching = FALSE;
   looking = FALSE;
   resting = FALSE;
   areaView = FALSE;
   activeCharacter = 0;
   activeItem = 0;
   PrevPosx=Posx;
   PrevPosy=Posy;   
   Posx = x;
   Posy = y;
   facing = f;
   SetVisited(level,x,y);  
}

#ifdef UAFEngine

//*****************************************************************************
// NAME:    prevItem
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
void PARTY::prevItem()
{
  if (ItemsOnPage > 0)
  {
    if (activeItem > 0)
      activeItem--;
    else 
      activeItem = (BYTE) (ItemsOnPage - 1);
  }
}

//*****************************************************************************
// NAME:    nextItem
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
void PARTY::nextItem()
{
  if (ItemsOnPage > 0)
  {
    activeItem = 
      (BYTE) ((activeItem+1) % ItemsOnPage);        
  }
}

//*****************************************************************************
// NAME:    nextCharacter
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
void PARTY::nextCharacter()
{
  if (numCharacters > 0)
  {
    activeCharacter = 
      (BYTE) ((activeCharacter+1) % (numCharacters));
  }
}

//*****************************************************************************
// NAME:    prevCharacter
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
void PARTY::prevCharacter()
{
  if (numCharacters > 0)
  {
    if (activeCharacter > 0)
      activeCharacter--;
    else
      activeCharacter = 
      (BYTE) (numCharacters-1);
  }
}  
#endif

//*****************************************************************************
// NAME: distributeExpPoints
//
// PURPOSE:  
//*****************************************************************************
void PARTY::distributeExpPoints(int total)
{
   if (total <= 0)
      return;
   if (numCharacters == 0)
     return;

   int remainder = total % (numCharacters);
   int bonus = total / (numCharacters);
   int lcv;
   
   for (lcv=0;lcv<numCharacters;lcv++)
   {
      if (remainder)
      {
         characters[lcv].giveCharacterExperience(bonus+remainder);
         remainder = 0;
      }
      else
         characters[lcv].giveCharacterExperience(bonus);
   }  
}


//*****************************************************************************
// NAME: getActiveChar
//
// PURPOSE:  
//*****************************************************************************
CHARACTER& PARTY::getActiveChar()
{
   return getCharacter(activeCharacter);
}

//*****************************************************************************
// NAME:    getCharacter
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
CHARACTER& PARTY::getCharacter(int activeChar)
{
  ASSERT( (activeChar >= 0) && (activeChar <= numCharacters));
  return characters[activeChar];
}

//*****************************************************************************
// NAME: addSpecialItem
//
// PURPOSE:  
//*****************************************************************************
void PARTY::addSpecialItem(int item)
{
  if (!globalData.specialItemData.HaveItem(item))
  {
    WriteDebugString("Bogus special item index in addSpecialItem(%u)\n",
                     item);
    return;
  }

  globalData.specialItemData.SetStage(item, 1);
}


//*****************************************************************************
// NAME: addSpecialKey
//
// PURPOSE:  
//*****************************************************************************
void PARTY::addSpecialKey(int key)
{
  if (!globalData.keyData.HaveItem(key))
  {
    WriteDebugString("Bogus special key index in addSpecialKey(%u)\n",
                     key);
    return;
  }

  globalData.keyData.SetStage(key, 1);
}


//*****************************************************************************
// NAME: removeSpecialItem
//
// PURPOSE:  
//*****************************************************************************
void PARTY::removeSpecialItem(int item)
{
  if (!globalData.specialItemData.HaveItem(item))
  {
    WriteDebugString("Bogus special item index in removeSpecialItem(%u)\n",
                     item);
    return;
  }

  globalData.specialItemData.SetStage(item, 0);
}


//*****************************************************************************
// NAME: removeSpecialKey
//
// PURPOSE:  
//*****************************************************************************
void PARTY::removeSpecialKey(int key)
{
   if (!globalData.keyData.HaveItem(key))
  {
    WriteDebugString("Bogus special key int in removeSpecialKey(%u)\n",
                     key);
    return;
  }

  globalData.keyData.SetStage(key, 0);
}


//*****************************************************************************
// NAME: hasSpecialItem
//
// PURPOSE:  
//*****************************************************************************
BOOL PARTY::hasSpecialItem(int item)
{
  if (!globalData.specialItemData.HaveItem(item))
  {
    WriteDebugString("Bogus special item index in hasSpecialItem(%u)\n",
                     item);
    return FALSE;
  }

  return (globalData.specialItemData.GetStage(item) > 0);
}


//*****************************************************************************
// NAME: hasSpecialKey
//
// PURPOSE:  
//*****************************************************************************
BOOL PARTY::hasSpecialKey(int key)
{
  if (!globalData.keyData.HaveItem(key))
  {
    WriteDebugString("Bogus special key index in hasSpecialKey(%u)\n",
                     key);
    return FALSE;
  }

  return (globalData.keyData.GetStage(key) > 0);
}


//*****************************************************************************
// NAME: getPartySpeed
//
// PURPOSE:  
//*****************************************************************************
BYTE PARTY::getPartySpeed()
{
  BYTE pspeed = 12;
  int i;

  for (i=0;i<numCharacters;i++)
  {
    BYTE charmove = characters[i].GetAdjMaxMovement();
    if (charmove < pspeed)
      pspeed = charmove;
  }

  return pspeed;
}

//*****************************************************************************
// NAME:    AddPooledMoneyToVault
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
void PARTY::AddPooledMoneyToVault(int WhichVault)
{
  // put all party funds into vault
  poolPartyGold();

  moneyPooled = FALSE;
  globalData.TransferMoney(WhichVault, poolSack);
}

//*****************************************************************************
// NAME:    RemovePooledMoneyFromVault
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
void PARTY::RemovePooledMoneyFromVault(int WhichVault)
{
  if ((WhichVault < 0) || (WhichVault >= MAX_GLOBAL_VAULTS))
  {
    TRACE("Bogus vault index in RemovePooledMoneyFromVault()\n");
    ASSERT(FALSE);    
    return;
  }

  // share vault funds among party members
  
  // put vault funds into pool vars
  moneyPooled = TRUE;
  poolSack.Transfer(globalData.vault[WhichVault].money);
  AutoUpConvertPoolMoney();
  // now share funds
  sharePartyGold();
}


//*****************************************************************************
// NAME: IsPartyDrunk
//
// PURPOSE:  
//*****************************************************************************
BOOL PARTY::IsPartyDrunk()
{
  return (drinkPoints >= DRUNK_THRESHOLD);
}


//*****************************************************************************
// NAME: UpdatePartyDrinkPoints
//
// PURPOSE:  
//*****************************************************************************
void PARTY::UpdatePartyDrinkPoints()
{
  static int lastHour = -1;
  static int lastDay = -1;

  if (drinkPoints <= 0)
    return;

  if (lastHour == -1)
  {
    lastHour = hours;
    lastDay = days;
  }

  while ((lastDay != days) && (drinkPoints > 0))
  {
    lastDay++;
    drinkPoints -= (DRINK_POINT_DEC_PER_HOUR*24);
  }

  while ((lastHour != hours) && (drinkPoints > 0))
  {
    lastHour++;
    drinkPoints -= DRINK_POINT_DEC_PER_HOUR;
  }

  if (drinkPoints < 0)
    drinkPoints = 0;
}


//*****************************************************************************
// NAME:    getCharWeaponText
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
void PARTY::getCharWeaponText(int index, CString &wpn, CString &dmg)
{
  characters[index].getCharWeaponText(wpn, dmg);
}

//*****************************************************************************
// NAME: addTempToParty
//
// PURPOSE:  
//*****************************************************************************
void PARTY::addTempToParty(CHARACTER &luckyDude)
{
   // To leave room for future NPC additions to party, 
   // only allow up to GetMaxPartyCharacters() of player
   // created characters into the party list.
   //
  for (int i=0;i<numCharacters;i++)
  {
    if (characters[i].IsSameCharacter(luckyDude))
      return;
  }

  if (numCharacters < GetMaxPartyCharacters())
  {
    characters[numCharacters] = luckyDude;
    characters[numCharacters].marchOrder = GetNextMarchOrder();
    characters[numCharacters].SetPartyMember();
    if (characters[numCharacters].GetType()!=NPC_TYPE)
      characters[numCharacters].origIndex = -1;
    numCharacters++;
  }
}


//*****************************************************************************
// NAME: PARTY::TeleportParty
//
// PURPOSE:  
//*****************************************************************************
BOOL PARTY::TeleportParty(TRANSFER_DATA &dest)
{
  BOOL success = TRUE;

  int currLevel = GetCurrentLevel();

  int dx,dy,df;
  if (dest.destLevel != currLevel)
  {
    // this data gets wiped when the new level is loaded
    TRANSFER_DATA temp;
    temp = dest;

    // transfer to new level
    if (!loadLevel(temp.destLevel))
    {
      success = FALSE;
      SetMiscError(LevelLoadError);
    }
    else
    {
      WriteDebugString("Teleporting: level %u data loaded successfully\n", temp.destLevel);

      // need to use entry point data from loaded level
      if (temp.destEP >= 0)
      {
        dx = GetEntryPtX(temp.destEP);
        dy = GetEntryPtY(temp.destEP);
        // entry point dialog doesn't allow facing to be given yet
        //df = globalData.levelInfo.GetEPFacing(temp.destEP);
        df = temp.facing;
      }
      else
      {
        dx = temp.destX;
        dy = temp.destY;
        df = temp.facing;
      }

      if (   (dx < 0) || (dx >= CurrLevelWidth())
          || (dy < 0) || (dy >= CurrLevelHeight()))
      {
        // that entry point is bogus
        WriteDebugString("Incorrect entry point data in TeleportParty()\n");
        success = FALSE;
        // reload old level?
      }
      else
      {
        facing = df;
        setPartyAdventureState();
        setPartyLevelState(temp.destLevel, dx, dy, df);
      }
    }
  }
  else // transfer within same level
  {
    // use entry point data for current level
    if (dest.destEP >= 0)
    {
      dx = GetEntryPtX(dest.destEP);
      dy = GetEntryPtY(dest.destEP);
      // entry point dialog doesn't allow facing to be given yet
      //df = globalData.levelInfo.GetEPFacing(dest.destEP);
      df = dest.facing;
    }
    else
    {
      dx = dest.destX;
      dy = dest.destY;
      df = dest.facing;
    }

    facing = df;
    setPartyAdventureState();
    setPartyLevelState(dest.destLevel, dx, dy, df);
  }

  return success;
}

#ifdef UAFEngine


//*****************************************************************************
// NAME: PARTY::GetCharThatNeedsHealedFirst
//
// PURPOSE:  fill in CHARACTER with party member that needs the most
//           healing and return TRUE. If no party members need healing, 
//           return FALSE.
//*****************************************************************************
BOOL PARTY::GetCharThatNeedsHealedFirst(CHARACTER &dude)
{
  int i;

  // look for unconscious chars first
  for (i=0;i<numCharacters;i++)
  {
    if (   ((characters[i].GetAdjStatus() == Unconscious) || (characters[i].GetAdjStatus() == Dying))
        && (characters[i].GetAdjHitPoints() <= 0))
    {
      dude = characters[i];   
      return TRUE;
    }
  }

  DWORD worst = 0;
  DWORD total;
  int index = -1;

  // find char with biggest hp deficit
  for (i=0;i<numCharacters;i++)
  {
    if (characters[i].GetAdjStatus() == Okay)
    {
      total = characters[i].GetHealingPointsNeeded();
      if (total > worst)
      {
        worst = total;
        index = i;
      }
    }
  }

  if (index >= 0)
  {
    dude = characters[index];
    return TRUE;
  }

  return FALSE;

}

//*****************************************************************************
// NAME: PARTY::FixParty
//
// PURPOSE:  
//*****************************************************************************
void PARTY::FixParty(BOOL cast)
{
  int i;
  for (i=0;i<numCharacters;i++)
    FixParty(characters[i], cast);
}


//*****************************************************************************
// NAME: PARTY::FixParty
//
// PURPOSE:  
//*****************************************************************************
void PARTY::FixParty(CHARACTER &caster, BOOL cast)
{
  if (!caster.canCastSpells())
    return;
  
  POSITION p1 = caster.spells.list.GetHeadPosition(); // access ordered list
  while (p1 != NULL)
  {
    // find memorized spells that match a spell in globalData.fixSpellBook;
    if (caster.spells.list.GetAt(p1).IsMemorized())
    {
      POSITION p2 = globalData.fixSpellBook.list.GetHeadPosition();
      while (p2 != NULL)
      {
        if (globalData.fixSpellBook.list.GetAt(p2).spell == caster.spells.list.GetAt(p1).spell)
        {
          // we have a match, and the spell is memorized by this caster
          CHARACTER recipient;
          if (GetCharThatNeedsHealedFirst(recipient))
          {
            WriteDebugString("%s is fixing %s\n", caster.GetName(), recipient.GetName());
            // if there is a character that needs healing
            // now cast the spell
            caster.RemoveAllTargets(); // clear target data
            caster.AddTarget(recipient); // add recipient as target
            caster.CastSpell(caster.spells.list.GetKeyAt(p1)); // invoke spell on target
          }
        }

        globalData.fixSpellBook.list.GetNext(p2);
      }
    }

    caster.spells.list.GetNext(p1);
  }

  caster.RemoveAllTargets(); // make sure targ list is cleaned up
}

DWORD PARTY::CalcRestTime(void)
{
  // figure out which party member has the longest rest
  // period required for memorizing the selected spells
  DWORD max=0;
  for (int i=0;i<numCharacters;i++)
  {
    DWORD t = characters[i].CalcRestTime();
    if (t > max) 
      max = t;
  }
  return max;
}

void PARTY::RemoveSpellEffect(DWORD parent, SPELL_DATA *pSpell)
{
  for (int i=0;i<numCharacters;i++)
    characters[i].RemoveSpellEffect(parent,pSpell);
}

// this function is called each time GameEvent::OnCycle() is called,
// which basically amounts to all of the time, regardless of
// whether the player provides input.
bool PARTY::ProcessTimeSensitiveData(LONGLONG currTime)
{
  static int lastDay=0;
  static LONGLONG lastUpdateTime=-1;
  static int minutesRested=0;
  static int minutesUntilAutoHeal=1440; // 24 hours
  static DWORD minuteTotal=0;
  static LONGLONG currGameTime=0;

  bool UpdateScreen = false;
  

  // special flag that tells us to reset these values,
  // such as when loading a saved game.
  if (currTime <= 0)
  {
    lastDay=0;
    lastUpdateTime=-1;
    minutesRested=0;
    minutesUntilAutoHeal=1440; // 24 hours
    minuteTotal=0;
    currGameTime=0;
    DayMusic=-1;
    return false;
  }

  // update once per minute (game minutes, NOT computer clock minutes)
  currGameTime = getElapsedMinutes();//get24HourClock(); // hours:mins (no days)
  if (currGameTime == 0) currGameTime=2400; 
  if (lastUpdateTime != currGameTime)
  {     
    // NewDay does not mean that 24 hours have passed, it only
    // means that the party time clock has incremented to the
    // next day
    bool NewDay=false;
    if (lastDay != days) // days is member of PARTY
    {
      NewDay=true;
      lastDay=days;
    }

    if (lastUpdateTime != -1)
    {
      if (currGameTime < lastUpdateTime) currGameTime+=2400;
      //int gmins = (currGameTime%100) + ((currGameTime/100)*60);
      //int lmins = (lastUpdateTime%100) + ((lastUpdateTime/100)*60);
      //int inc = gmins-lmins;      
      int inc = currGameTime-lastUpdateTime;
      inc = max(0,inc);

      if (inc > 0)
      {
        minuteTotal += inc;
        minutesUntilAutoHeal -= inc;
        
        if (resting) // set to TRUE when 'Rest' menu option is used to pass time
          minutesRested += inc;
        else
          minutesRested=0;

        UpdatePartyDrinkPoints();
        ActiveSpellData.ProcessTimeSensitiveData(inc);
        //PendingSpellData.ProcessTimeSensitiveData(inc); // not needed?

        CheckAndPlayBackgroundMusic(DayMusic);

        int i;
        if (resting)
        {
          for (int mt=0; mt<inc; mt++)
          {
            for (i=0;i<numCharacters;i++)
            {
              if (characters[i].canCastSpells())
              {
                if (characters[i].spells.SpellsKnown() > 0)
                {
                  // add time delta to all spells being memorized
                  // returns TRUE if a spell was memorized
                  if (characters[i].spells.incAllMemorizedTime(1))
                  {
                    // find newly memorized spell and announce it
                    POSITION p = characters[i].spells.list.GetHeadPosition(); // access ordered list
                    while (p != NULL)
                    {
                      if (characters[i].spells.list.GetAt(p).JustMemorized)
                      {
                        characters[i].spells.list.GetAt(p).JustMemorized=FALSE;
                        CString tmp;
                        tmp.Format("%s memorizes %s", 
                                    characters[i].GetName(),
                                    spellData.GetSpellName(characters[i].spells.list.GetAt(p).spell));
                        FormatPausedText(pausedTextData, tmp);
                        DisplayPausedText(pausedTextData);
                      }
                      characters[i].spells.list.GetNext(p);
                    }
                  }
                  else
                  {
                    ClearFormattedText(pausedTextData);
                  }
                }
              }
            }
          }
        }

        for (i=0;i<numCharacters;i++)
        {
          characters[i].ClearQueuedSpecAb();
          characters[i].CheckForExpiredSpellEffects();

          if (NewDay)
          {
            characters[i].myItems.ResetItemCharges(TRUE);
            characters[i].HasLayedOnHandsToday=FALSE;
          }
			
		  // If party is resting and characters are unconscious (from battle, traps, etc.)
		  // wake that character up.
		  if ((resting) && (characters[i].GetStatus() == Unconscious)){
			  characters[i].SetHitPoints(1);
			  characters[i].SetStatus(Okay);
		  }

          if (characters[i].GetStatus() == Okay)
          {
            if (characters[i].GetAdjSpecAb(SA_Poisoned))
            {
              double rateofloss = 0.25;
              BOOL IsSlowed = characters[i].GetAdjSpecAb(SA_SlowPoison);
              if (IsSlowed) rateofloss = 0.12;
              
              double currhp = characters[i].GetHitPoints();
              double hploss = currhp * rateofloss;
              
              hploss += 0.5; // round up
              hploss = floor(hploss);              
              if (hploss < 1.0) hploss = 1.0;

              int newhptotal = currhp - hploss;
              
              if ((IsSlowed) && (newhptotal < 1))
                newhptotal = 1;

              characters[i].SetHitPoints( newhptotal );
              
              if (newhptotal <= 0)
                characters[i].SetStatus(Dead);

              if (newhptotal != currhp)
                PlayCharHit();

              UpdateScreen=true;
            }
          }
        }
      }
    }

    //if ((minuteTotal>0) && (NewDay)) // 24 hours
    //if ((minuteTotal>0) && ((minuteTotal%1440)==0))
    if (minutesUntilAutoHeal <= 0)
    {      
      // the subtraction above may have taken it below
      // zero, and the difference figures into the new
      // time remaining
      minutesUntilAutoHeal = 1440 - abs(minutesUntilAutoHeal);
      int hp = ((minutesRested>=1440)?3:1);
      minutesRested=0;
      for (int i=0;i<numCharacters;i++)
      {
        if (characters[i].IsAlive())
        {
          int currhp=characters[i].GetHitPoints();
          characters[i].SetHitPoints(currhp+hp);
          UpdateScreen=true;
        }
      }
    }

    lastUpdateTime=getElapsedMinutes();//get24HourClock(); // hours:mins (no days)
  }

  return UpdateScreen;
}

#endif // ifdef UAFEngine

void InsertPartyASL(
       PARTY& data,
       const CString& key,
       const CString& value,
       int flags)
{
  data.party_asl.Insert(key, value, flags);
}

void InsertCharacterASL(
       PARTY& data,
       int memberNumber,
       const CString& key,
       const CString& value,
       int flags)
{
  data.characters[memberNumber].char_asl.Insert(key, value, flags);
}

void DeletePartyASL(PARTY& data, const CString& key)
{
  data.party_asl.Delete(key);
}

CString LookupPartyASL(
           PARTY& data,
           const CString& key)
{
  ASL_VALUE AV;
  data.party_asl.Lookup(key, AV);
  return AV.Value();
}

CString LookupCharacterASL(
           PARTY& data,
           int memberNumber,
           const CString& key)
{
  ASL_VALUE AV;
  data.characters[memberNumber].char_asl.Lookup(key, AV);
  return AV.Value();
}


BOOL ExistsPartyASL(
           PARTY& data,
           const CString& key)
{
  return data.party_asl.Exists(key);
}

genderType GetCharacterGender(PARTY& data, int memberNumber)
{
  return data.characters[memberNumber].GetAdjGender();
}

CString GetCharacterName(PARTY& data, int memberNumber)
{
  return data.characters[memberNumber].GetName();
}

int GetPartySize(PARTY& data)
{
  return data.numCharacters;
}

//*****************************************************************************

//*****************************************************************************
//    NAME: VISIT_DATA::VISIT_DATA
//
// PURPOSE: 
//
//*****************************************************************************
VISIT_DATA::VISIT_DATA()
{ 
  for (int i=0;i<MAX_LEVELS;i++)
    visited[i] = NULL;

  Clear(); 
}

//*****************************************************************************
//    NAME: ~VISIT_DATA::VISIT_DATA
//
// PURPOSE: 
//
//*****************************************************************************
VISIT_DATA::~VISIT_DATA() 
{ 
  for (int i=0;i<MAX_LEVELS;i++)
  {
    if (visited[i] != NULL) 
      delete visited[i];
    visited[i]=NULL; 
  }
}

//*****************************************************************************
//    NAME: VISIT_DATA::operator=
//
// PURPOSE: 
//
//*****************************************************************************
VISIT_DATA& VISIT_DATA::operator=(const VISIT_DATA& src) 
{ 
  if (&src == this) return *this;
  
  Clear();

  for (int i=0;i<MAX_LEVELS;i++)
  {
    if (src.visited[i] != NULL)
    {
      visited[i] = new TAG_LIST_2D(src.visited[i]->GetWidth(), 
                                   src.visited[i]->GetHeight());

      *visited[i] = *src.visited[i];
    }
  }
  
  return *this; 
}

//*****************************************************************************
//    NAME: VISIT_DATA::Clear
//
// PURPOSE: 
//
//*****************************************************************************
void VISIT_DATA::Clear()
{ 
  for (int i=0;i<MAX_LEVELS;i++)
  {
    if (visited[i] != NULL) 
      delete visited[i];

    visited[i] = NULL;
  }

  //visited = new TAG_LIST_2D(MAX_AREA_WIDTH, MAX_AREA_HEIGHT);
}
//*****************************************************************************
//    NAME: VISIT_DATA::IsVisited
//
// PURPOSE: 
//
//*****************************************************************************
BOOL VISIT_DATA::IsVisited(int level, int x, int y) 
{ 
  if ((level < 0) || (level >= MAX_LEVELS)) return FALSE;
  
  if (visited[level] == NULL)
    return FALSE;

  return (visited[level]->Get(x, y));
}
//*****************************************************************************
//    NAME: VISIT_DATA::SetVisited
//
// PURPOSE: 
//
//*****************************************************************************
void VISIT_DATA::SetVisited(int level, int x, int y) 
{ 
  if ((level < 0) || (level >= MAX_LEVELS)) return;
  
  if (visited[level] == NULL)
    visited[level] = new TAG_LIST_2D(MAX_AREA_WIDTH, MAX_AREA_HEIGHT);

  visited[level]->Set(x, y);
}

//*****************************************************************************
// NAME:    VISIT_DATA::Serialize
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
void VISIT_DATA::Serialize(CArchive &ar)
{
  CObject::Serialize(ar);

  if (ar.IsStoring())
  {
    CString DataBlockTag("VISIT_DATA"); 
    AS(ar, DataBlockTag);

    for (int i=0;i<MAX_LEVELS;i++)
    {
      ar << i; // which level this info is for      

      if (visited[i] != NULL)
      {
        LPBYTE bytes = visited[i]->GetBytes();  
        ASSERT( bytes != NULL );
        if (bytes == NULL) continue;

        ar << visited[i]->GetByteSize(); // how many bytes in data block
        ar.Write(bytes, visited[i]->GetByteSize()); // write the data block
      }
      else
      {
        ar << 0; // zero data bytes for this level
      }
    }
  }
  else
  {
    Clear();
    
    if (globalData.SaveGameVersion < _VERSION_0911_)
    {
      // In older versions only the information for a single level
      // was stored (bug). The data block was always allocated and had
      // a fixed size.       
      
      visited[0] = new TAG_LIST_2D(MAX_AREA_WIDTH, MAX_AREA_HEIGHT);
      
      // determine number of bytes in data block
      int count = visited[0]->GetByteSize();

      // get a pointer to the data block (only 1 in this case)
      LPBYTE bytes = visited[0]->GetBytes();
      ASSERT( bytes != NULL );
      if (bytes == NULL) return;      

      ar.Read(bytes, count);
    }
    else
    {      
      int level;
      int count;

      // sanity check, make sure we are located at the correct
      // offset in the data file
      CString DataBlockTag; 
      DAS(ar, DataBlockTag);
      ASSERT( DataBlockTag.Compare("VISIT_DATA") == 0 );

      for (int i=0;i<MAX_LEVELS;i++)
      {
        ar >> level;
        ar >> count;

        if (count > 0)
        {
          visited[level] = new TAG_LIST_2D(MAX_AREA_WIDTH, MAX_AREA_HEIGHT);
          ASSERT( visited[level]->GetByteSize() == count );

          LPBYTE pbytes = visited[level]->GetBytes();
          ASSERT( pbytes != NULL );
          if (pbytes == NULL) continue; 

          ar.Read(pbytes, count);
        }
      }
    }
  }
}

//*****************************************************************************

void STEP_COUNTER::Clear() 
{ 
  for (int i=0;i<MAX_ZONES;i++) 
    stepCount[i]=0; 
}
STEP_COUNTER& STEP_COUNTER::operator =(const STEP_COUNTER& src) 
{ 
  if (&src == this)
    return *this;
  for (int i=0;i<MAX_ZONES; i++)
    stepCount[i] = src.stepCount[i];
  return *this;
}

STEP_COUNTER::STEP_COUNTER(const STEP_COUNTER& src)
{
  for (int i=0;i<MAX_ZONES; i++)
    stepCount[i] = src.stepCount[i];
}

//*****************************************************************************

TRIGGER_FLAGS& TRIGGER_FLAGS::operator =(const TRIGGER_FLAGS& src) 
{ 
  if (&src == this)
    return *this;
  eventResult=src.eventResult; 
  return *this; 
}
TRIGGER_FLAGS::TRIGGER_FLAGS(const TRIGGER_FLAGS& src)
{ 
  *this = src; 
}
void TRIGGER_FLAGS::Serialize(CArchive &ar)
{
  CObject::Serialize(ar);
  if (ar.IsStoring())
  {
    ar << (int)eventStatusUnused;
    ar << (int)eventResult;
  }
  else
  {
    int temp;
    ar >> temp;
    eventStatusUnused = temp;
    ar >> temp;
    eventResult = (eventResultType)temp;
  }
}

//*****************************************************************************

LEVEL_FLAG_DATA& LEVEL_FLAG_DATA::operator =(const LEVEL_FLAG_DATA& src) 
{  
  if (&src == this)
    return *this;
  stepData = src.stepData;
  flagData = src.flagData;
  flags=src.flags;
  return *this; 
}
LEVEL_FLAG_DATA::LEVEL_FLAG_DATA(const LEVEL_FLAG_DATA& src) 
{ 
  *this = src; 
}

void LEVEL_FLAG_DATA::IncZoneStepCount(int zone) 
{ 
  (stepData.stepCount[zone])++; 
}
unsigned long LEVEL_FLAG_DATA::GetZoneStepCount(int zone) 
{ 
  return (stepData.stepCount[zone]); 
}
BOOL LEVEL_FLAG_DATA::HasEventHappened(DWORD event)
{
  if (flagData.Find(flags, event)) return flags.HasEventHappened();
  else return FALSE;
}

void LEVEL_FLAG_DATA::markEventHappened(DWORD event)
{
  if (flagData.Find(flags, event))
  {
    flags.markEventHappened();
    flagData.Update(flags, event);
  }
  else
  {
    flags.markEventHappened();
    flagData.Insert(flags, event);
  }
}

//*****************************************************************************

EVENT_TRIGGER_DATA& EVENT_TRIGGER_DATA::operator =(const EVENT_TRIGGER_DATA& src)
{
  if (&src == this)
    return *this;
  Clear();
  data.SetSize(src.data.GetSize());
  for (int i=0; i<src.data.GetSize(); i++)
    data[i] = src.data[i];
  return *this;
}
EVENT_TRIGGER_DATA::EVENT_TRIGGER_DATA(const EVENT_TRIGGER_DATA& src) 
{ 
  *this = src; 
}

void EVENT_TRIGGER_DATA::Clear()
{
  for (int i=0;i<data.GetSize(); i++)
    data[i].Clear();
}

BOOL EVENT_TRIGGER_DATA::CheckLevel(int level)
{
  if (level < 0)
    return FALSE;

  if (level >= data.GetSize())
  {
    data.SetSize(level+1);
    LEVEL_FLAG_DATA temp;
    data[level]=temp;
  }
  return TRUE;
}

void EVENT_TRIGGER_DATA::IncZoneStepCount(int level, int zone) 
{ 
  if (CheckLevel(level))
    data[level].IncZoneStepCount(zone);
}

unsigned long EVENT_TRIGGER_DATA::GetZoneStepCount(int level, int zone) 
{ 
  if (CheckLevel(level))
    return data[level].GetZoneStepCount(zone);
  else
    return 0;
}
void EVENT_TRIGGER_DATA::MarkEventHappened(int level, DWORD event) 
{
  if (CheckLevel(level))
    data[level].markEventHappened(event);
}
BOOL EVENT_TRIGGER_DATA::HasEventHappened(int level, DWORD event)
{
  if (CheckLevel(level))
    return data[level].HasEventHappened(event);
  else
    return FALSE;
}

//*****************************************************************************

int PARTY::NumNPCs()
{
  int numNPC = 0;
  for (int i=0;i<numCharacters; i++)
  {
    if (characters[i].GetType() == NPC_TYPE)
      numNPC++;
  }
  return numNPC;
}

BOOL PARTY::CanAddNPC()
{
  return (numCharacters < GetMaxPartyMembers());
}

void PARTY::DecCharacterOrder(void)
{
  if (numCharacters <= 1) return;

  CHARACTER tempCHARACTER;
  int src = activeCharacter;
  int dst;
  
  if (activeCharacter > 0)
  {
    // if not already in first slot
    dst = activeCharacter-1;

    // swap active with active-1
    tempCHARACTER = characters[dst];
    characters[dst] = characters[src];
    characters[src] = tempCHARACTER;
  }
  else
  {
    // already in first slot.
    // move all characters up 1 and
    // put active into slot numCharacters-1
    dst = numCharacters-1;
    // save current
    tempCHARACTER = characters[src];
    // now move everybody up 1
    for (int i=0;i<numCharacters-1;i++)
      characters[i] = characters[i+1];
    characters[dst] = tempCHARACTER;
  }
  activeCharacter = dst;
}

void PARTY::IncCharacterOrder(void)
{
  if (numCharacters <= 1) return;
  CHARACTER tempCHARACTER;
  int src = activeCharacter;
  int dst;
  
  if (activeCharacter >= numCharacters-1)
  {
    // already in last slot.
    // move all characters down 1 and
    // put active into slot 0
    dst = 0;
    // save current
    tempCHARACTER = characters[src];
    // now move everybody down 1
    // start from end and move backwards
    for (int i=numCharacters-1;i>0;i--)
      characters[i] = characters[i-1];
    characters[dst] = tempCHARACTER;
  }
  else
  {
    // swap active with active+1
    dst = activeCharacter+1;    
    tempCHARACTER = characters[dst];
    characters[dst] = characters[src];
    characters[src] = tempCHARACTER;
  }
  activeCharacter = dst;
}

//   NoPartyMember, EntireParty, ActiveChar,
//   OneAtRandom, ChanceOnEach 
void PARTY::HealParty(HEAL_PARTY_DATA &data)
{
  int rndDude = rollDice(numCharacters, 1) - 1;
  switch (data.who)
  {
  case NoPartyMember:
    return;
  case EntireParty:
    if (data.HealHP)
    {
      for (int i=0;i<numCharacters;i++)
      {
        int currHp = characters[i].GetHitPoints();
        int totalHp;

        if (data.LiteralOrPercent==0) // add literal
        {
          totalHp = currHp+data.HowMuchHP;
        }
        else if (data.LiteralOrPercent==1)// add percent of max
        {
          double maxHp = characters[i].GetMaxHitPoints();
          maxHp *= ((double)data.HowMuchHP * 0.01);
          totalHp = currHp+maxHp;
        }
        else if (data.LiteralOrPercent==2)// set to percent of max
        {
          double maxHp = characters[i].GetMaxHitPoints();
          maxHp *= ((double)data.HowMuchHP * 0.01);
          totalHp = maxHp;
          if (totalHp < currHp) totalHp=currHp; // no change
        }
        else
          return;

        characters[i].SetHitPoints(totalHp);
        if (characters[i].GetHitPoints() > 0)
          characters[i].SetStatus(Okay);
      }
    }

    if (data.HealCurse)
    {
      for (int i=0;i<numCharacters;i++)
      {        
        POSITION pos = characters[i].myItems.GetHeadPosition();
        while (pos != NULL)
        {
          if (characters[i].myItems.GetAt(pos).cursed)
            characters[i].myItems.GetAt(pos).cursed = FALSE;
          characters[i].myItems.GetNext(pos);
        }
      }
    }
    if (data.HealDrain)
    {
      WriteDebugString("Heal Drain not coded yet\n");
    }
    break;
  case ActiveChar:
    if (data.HealHP)
    {
      int currHp = characters[activeCharacter].GetHitPoints();
      int totalHp;

      if (data.LiteralOrPercent==0) // literal
      {
        totalHp = currHp+data.HowMuchHP;
      }
      else if (data.LiteralOrPercent==1)// percent of max
      {
        double maxHp = characters[activeCharacter].GetMaxHitPoints();
        maxHp *= ((double)data.HowMuchHP * 0.01);
        totalHp = currHp+maxHp;
      }
      else if (data.LiteralOrPercent==2)// set to percent of max
      {
        double maxHp = characters[activeCharacter].GetMaxHitPoints();
        maxHp *= ((double)data.HowMuchHP * 0.01);
        totalHp = maxHp;
        if (totalHp < currHp) totalHp=currHp; // no change
      }
      else
        return;
      
      characters[activeCharacter].SetHitPoints(totalHp);
      if (characters[activeCharacter].GetHitPoints() > 0)
        characters[activeCharacter].SetStatus(Okay);
    }

    if (data.HealCurse)
    {
      POSITION pos = characters[activeCharacter].myItems.GetHeadPosition();
      while (pos != NULL)
      {
        if (characters[activeCharacter].myItems.GetAt(pos).cursed)
          characters[activeCharacter].myItems.GetAt(pos).cursed = FALSE;
        characters[activeCharacter].myItems.GetNext(pos);
      }
    }
    if (data.HealDrain)
    {
      WriteDebugString("Heal Drain not coded yet\n");
    }
    break;
  case OneAtRandom:
    if (data.HealHP)
    {
      int currHp = characters[rndDude].GetHitPoints();
      int totalHp;

      if (data.LiteralOrPercent==0) // literal
      {
        totalHp = currHp+data.HowMuchHP;
      }
      else if (data.LiteralOrPercent==1)// percent of max
      {
        double maxHp = characters[rndDude].GetMaxHitPoints();
        maxHp *= ((double)data.HowMuchHP * 0.01);
        totalHp = currHp+maxHp;
      }
      else if (data.LiteralOrPercent==2)// set to percent of max
      {
        double maxHp = characters[rndDude].GetMaxHitPoints();
        maxHp *= ((double)data.HowMuchHP * 0.01);
        totalHp = maxHp;
        if (totalHp < currHp) totalHp=currHp; // no change
      }
      else
        return;

      characters[rndDude].SetHitPoints(totalHp);
      if (characters[rndDude].GetHitPoints() > 0)
        characters[rndDude].SetStatus(Okay);
    }

    if (data.HealCurse)
    {
      POSITION pos = characters[rndDude].myItems.GetHeadPosition();
      while (pos != NULL)
      {
        if (characters[rndDude].myItems.GetAt(pos).cursed)
          characters[rndDude].myItems.GetAt(pos).cursed = FALSE;
        characters[rndDude].myItems.GetNext(pos);
      }
    }
    if (data.HealDrain)
    {
      WriteDebugString("Heal Drain not coded yet\n");
    }
    break;
  case ChanceOnEach:
    if (data.HealHP)
    {
      for (int i=0;i<numCharacters;i++)
      {
        if (rollDice(100, 1, 0) <= data.chance)
        {
          int currHp = characters[i].GetHitPoints();
          int totalHp;

          if (data.LiteralOrPercent==0) // add literal
          {
            totalHp = currHp+data.HowMuchHP;
          }
          else if (data.LiteralOrPercent==1)// add percent of max
          {
            double maxHp = characters[i].GetMaxHitPoints();
            maxHp *= ((double)data.HowMuchHP * 0.01);
            totalHp = currHp+maxHp;
          }
          else if (data.LiteralOrPercent==2)// set to percent of max
          {
            double maxHp = characters[i].GetMaxHitPoints();
            maxHp *= ((double)data.HowMuchHP * 0.01);
            totalHp = maxHp;
            if (totalHp < currHp) totalHp=currHp; // no change
          }
          else
            return;

          characters[i].SetHitPoints(totalHp);
          if (characters[i].GetHitPoints() > 0)
            characters[i].SetStatus(Okay);
        }
      }
    }

    if (data.HealCurse)
    {
      for (int i=0;i<numCharacters;i++)
      {        
        if (rollDice(100, 1, 0) <= data.chance)
        {
          POSITION pos = characters[i].myItems.GetHeadPosition();
          while (pos != NULL)
          {
            if (characters[i].myItems.GetAt(pos).cursed)
              characters[i].myItems.GetAt(pos).cursed = FALSE;
            characters[i].myItems.GetNext(pos);
          }
        }
      }
    }
    if (data.HealDrain)
    {
      WriteDebugString("Heal Drain not coded yet\n");
    }
    break;
  }
}
