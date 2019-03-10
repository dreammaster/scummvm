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
//#include "dungeoncraft/shared/..\UAFWinEd\UAFWinEd.h"
#else
#include "dungeoncraft/shared/..\UAFWin\Dungeon.h"
#include "dungeoncraft/shared/GameEvent.h"
#include "dungeoncraft/shared/..\UAFWin\disptext.h"
#include "dungeoncraft/shared/..\UAFWin\FormattedText.h"
#endif

#include "dungeoncraft/shared/items.h"
#include "dungeoncraft/shared/char.h"
#include "dungeoncraft/shared/monster.h"
#include "dungeoncraft/shared/class.h"

#include "dungeoncraft/shared/GlobalData.h"
#include "dungeoncraft/shared/traits.h"
#include "dungeoncraft/shared/GameRules.h"

#ifdef UAFEngine
#include "dungeoncraft/shared/RunTimeIF.h"
#include "dungeoncraft/shared/..\UAFWin\FormattedText.h"
#include <math.h>
#endif

#include "dungeoncraft/shared/RunTimeIF.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_SERIAL( CHARACTER, CObject, 1 )
IMPLEMENT_SERIAL( CHAR_LIST, CObject, 1 )

CHARACTER character;

#ifdef _DEBUG // debug only diagnostic
#define DUMP_CHAR_SPELL_ATTS 1
#endif

//*****************************************************************************
// NAME:    addSpecialItemsToItemList
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
#ifdef UAFEngine
void addSpecialItemsToItemList(ITEM_TEXT_LIST &list)
{
  POSITION pos;
  pos = globalData.specialItemData.GetHeadPosition();
  while (pos != NULL)
  {
    if (party.hasSpecialItem(globalData.specialItemData.GetAt(pos).id))
    {
      CString name;
      if (EditorRunning) // add diagnostic data if testing design from editor
        name.Format("%s: stage %u", 
                    GetSpecialItemName(globalData.specialItemData.GetAt(pos).id),
                    globalData.specialItemData.GetAt(pos).stage);
      else
        name = GetSpecialItemName(globalData.specialItemData.GetAt(pos).id);

      list.Add(1,
               SpecialItemType,
               name,
               globalData.specialItemData.GetAt(pos).id);
    }
    globalData.specialItemData.GetNext(pos);
  }

  pos = globalData.keyData.GetHeadPosition();
  while (pos != NULL)
  {
    if (party.hasSpecialKey(globalData.keyData.GetAt(pos).id))
    {
      CString name;
      if (EditorRunning) // add diagnostic data if testing design from editor
        name.Format("%s: stage %u", 
                    GetSpecialKeyName(globalData.keyData.GetAt(pos).id),
                    globalData.keyData.GetAt(pos).stage);
      else
        name = GetSpecialKeyName(globalData.keyData.GetAt(pos).id);

      list.Add(1,
               SpecialKeyType,
               name,
               globalData.keyData.GetAt(pos).id);
    }
    globalData.keyData.GetNext(pos);
  }

  // add diagnostic data if testing design from editor.
  // quests only show up in inventory while testing design.
  if (EditorRunning)
  {
    pos = globalData.questData.GetHeadPosition();
    while (pos != NULL)
    {
      if (globalData.questData.IsPresent(globalData.questData.GetAt(pos).id))
      {
        CString name;
        name.Format("%s: stage %u", 
                    globalData.questData.GetAt(pos).name,
                    globalData.questData.GetAt(pos).stage);
        list.Add(1,
                 QuestType,
                 name,
                 globalData.questData.GetAt(pos).id);
      }
      globalData.questData.GetNext(pos);
    }
  }

  ItemsOnPage = list.GetCount();
	ItemsOnPage = min(ItemsOnPage, Items_Per_Page);
}

#endif
//*****************************************************************************
// NAME:    tradeItemMoney
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
void tradeItemMoney(itemClassType type, int qty, CHARACTER& giver, CHARACTER& taker)
{
  if (!itemIsMoney(type))
  {
    WriteDebugString("Wrong item type passed to tradeItemMoney()\n");
    return;
  }

  if ((qty + taker.GetEncumbrance()) > taker.GetAdjMaxEncumbrance())
  {
		SetMiscError(TooMuchWeight);
    return;
  }
  int i;
  switch (type) 
  {
  case GemType:
    {
      POSITION pos = giver.money.Gems.GetHeadPosition();
      i=0;
      while ( (i < qty) && (pos != NULL))
      {
        GEM_TYPE gem = giver.money.Gems.GetAt(pos);
        taker.money.AddGem(gem);

        giver.money.Gems.GetNext(pos);
        i++;
      }
    }
    break;
  case JewelryType:
    {
      POSITION pos = giver.money.Gems.GetHeadPosition();
      i=0;
      while ( (i < qty) && (pos != NULL))
      {
        GEM_TYPE gem = giver.money.Gems.GetAt(pos);
        taker.money.AddGem(gem);
        
        giver.money.Gems.GetNext(pos);
        i++;
      }
    }
    break;
  default:
    taker.money.Add(type, qty);
    giver.money.Subtract(type, qty);
    break;
  }

 	taker.SetEncumbrance(taker.determineEffectiveEncumbrance());
  giver.SetEncumbrance(giver.determineEffectiveEncumbrance());
	taker.determineMaxMovement();
  giver.determineMaxMovement();
}

//*****************************************************************************
// NAME:    tradeItem
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
void tradeItem(int item, int qty, int charges, int id, int g_index, CHARACTER& giver, CHARACTER& taker)
{
  if (giver.marchOrder==taker.marchOrder)
  {
    // trade with self (form of inventory re-arrangement by player)
    giver.delCharacterItem(g_index, qty);
    taker.addCharacterItem(item, qty, charges, id, giver.myItems.GetPurchasePrice(g_index));
  }
  else
  {
    if (taker.addCharacterItem(item, qty, charges, id, giver.myItems.GetPurchasePrice(g_index)))
      giver.delCharacterItem(g_index, qty);
  }
}

//*****************************************************************************
//*****************************************************************************
//*****************************************************************************


//*****************************************************************************
//    NAME: BlockageDataType::Serialize
//
// PURPOSE: 
//
//*****************************************************************************
void BlockageDataType::Serialize(CArchive &ar)
{
  if (ar.IsStoring())
    ar << level << x << y << Stats.StatsFull;
  else
    ar >> level >> x >> y >> Stats.StatsFull;
}

//*****************************************************************************
//    NAME: BlockageDataType::Serialize
//
// PURPOSE: 
//
//*****************************************************************************
void BlockageDataType::Serialize(CAR &ar)
{
  if (ar.IsStoring())
  {
    ar << level;
    ar << x;
    ar << y;
    ar << Stats.StatsFull;
  }
  else
  {
    ar >> level; 
    ar >> x;
    ar >> y;
    ar >> Stats.StatsFull;
  }
}

//*****************************************************************************
//    NAME: BLOCKAGE_STATUS::operator==
//
// PURPOSE: 
//
//*****************************************************************************
bool BLOCKAGE_STATUS::operator==(const BLOCKAGE_STATUS& src)
{
  if (data.GetCount() != src.data.GetCount()) return false;
  POSITION pos = data.GetHeadPosition();
  bool success = true;
  while (pos != NULL)
  {
    POSITION spos = src.data.GetHeadPosition();
    bool found = false;
    while (spos != NULL)
    {
      if (src.data.GetAt(spos).Stats.StatsFull == data.GetAt(pos).Stats.StatsFull)
      {
        found = true;
        break;
      }
      src.data.GetNext(spos);
    }
    if (!found)
    {
      success = false;
      break;
    }
    data.GetNext(pos);
  }
  return success;
}

//*****************************************************************************
//    NAME: BLOCKAGE_STATUS::operator=
//
// PURPOSE: 
//
//*****************************************************************************
BLOCKAGE_STATUS& BLOCKAGE_STATUS::operator=(const BLOCKAGE_STATUS& src) 
{ 
  if (&src == this)
    return *this;

  Clear();

  POSITION pos = src.data.GetHeadPosition();
  BlockageDataType temp;

  while (pos != NULL)
  {
    temp = src.data.GetNext(pos);
    data.AddTail(temp);
  }  
  return *this; 
}

//*****************************************************************************
//    NAME: BLOCKAGE_STATUS::Serialize
//
// PURPOSE: 
//
//*****************************************************************************
void BLOCKAGE_STATUS::Serialize(CArchive &ar)
{
  CObject::Serialize(ar);
  POSITION pos;

  if (ar.IsStoring())  
  {
    ar << data.GetCount();
    pos = data.GetHeadPosition();
    while (pos != NULL)
    {
      data.GetAt(pos).Serialize(ar);
      data.GetNext(pos);
    }
  }
  else
  {
    BlockageDataType temp;
    int count;
    ar >> count;

    for (int i=0;i<count;i++)
    {
       temp.Serialize(ar);
       data.AddTail(temp);
    }
  }
}

//*****************************************************************************
//    NAME: BLOCKAGE_STATUS::Serialize
//
// PURPOSE: 
//
//*****************************************************************************
void BLOCKAGE_STATUS::Serialize(CAR &ar)
{
  POSITION pos;
  if (ar.IsStoring())  
  {
    ar << data.GetCount();
    pos = data.GetHeadPosition();
    while (pos != NULL)
    {
      data.GetAt(pos).Serialize(ar);
      data.GetNext(pos);
    }
  }
  else
  {
    BlockageDataType temp;
    int count;
    ar >> count;

    for (int i=0;i<count;i++)
    {
       temp.Serialize(ar);
       data.AddTail(temp);
    }
  }
}

//*****************************************************************************
//    NAME: BLOCKAGE_STATUS::FindData
//
// PURPOSE: 
//
//*****************************************************************************
POSITION BLOCKAGE_STATUS::FindData(int level, int x, int y)
{
  POSITION pos = data.GetHeadPosition();
  BOOL found = FALSE;
  while ((pos != NULL) && (!found))
  {
    BlockageDataType temp = data.GetAt(pos);
    if (   (temp.x == x)
        && (temp.y == y)
        && (temp.level == level))
    {
      found = TRUE;
    }
    else
      data.GetNext(pos);
  }
  if (!found)
    pos = NULL;
  return pos;
}

//*****************************************************************************
//    NAME: BLOCKAGE_STATUS::IsSecret
//
// PURPOSE: 
//
//*****************************************************************************
BOOL BLOCKAGE_STATUS::IsSecret(int level, int x, int y, int facing)
{ 
  POSITION pos = FindData(level, x, y);

  if (pos != NULL)
  {
    switch (facing)
    {
    case FACE_NORTH:
      return (data.GetAt(pos).Stats.flags.NorthSecret > 0); 
    case FACE_EAST:
      return (data.GetAt(pos).Stats.flags.EastSecret > 0); 
    case FACE_SOUTH:
      return (data.GetAt(pos).Stats.flags.SouthSecret > 0); 
    case FACE_WEST:
      return (data.GetAt(pos).Stats.flags.WestSecret > 0); 
    }
  }

  // not found means party has not cleared secret bit for this spot yet
  return TRUE;
}

//*****************************************************************************
//    NAME: BLOCKAGE_STATUS::ClearSecret
//
// PURPOSE: 
//
//*****************************************************************************
void BLOCKAGE_STATUS::ClearSecret(int level, int x, int y, int facing)
{ 
  POSITION pos = FindData(level, x, y);

  if (pos != NULL) // already have data for this spot
  {
    switch (facing)
    {
    case FACE_NORTH:
      data.GetAt(pos).Stats.flags.NorthSecret = 0;
      break;
    case FACE_EAST:
      data.GetAt(pos).Stats.flags.EastSecret = 0; 
      break;
    case FACE_SOUTH:
      data.GetAt(pos).Stats.flags.SouthSecret = 0; 
      break;
    case FACE_WEST:
      data.GetAt(pos).Stats.flags.WestSecret = 0; 
      break;    
    }
  }
  else // add data for this spot with secret bit clear
  {
    BlockageDataType temp; // all bits default to 1
    temp.level = level;
    temp.x = x;
    temp.y = y;
    switch (facing) // clear secret bit
    {
    case FACE_NORTH:
      temp.Stats.flags.NorthSecret = 0;
      break;
    case FACE_EAST:
      temp.Stats.flags.EastSecret = 0; 
      break;
    case FACE_SOUTH:
      temp.Stats.flags.SouthSecret = 0; 
      break;
    case FACE_WEST:
      temp.Stats.flags.WestSecret = 0; 
      break;    
    }
    data.AddTail(temp);
  }
}

//*****************************************************************************
//    NAME: BLOCKAGE_STATUS::IsLocked
//
// PURPOSE: 
//
//*****************************************************************************
BOOL BLOCKAGE_STATUS::IsLocked(int level, int x, int y, int facing)
{ 
  POSITION pos = FindData(level, x, y);

  if (pos != NULL)
  {
    switch (facing)
    {
    case FACE_NORTH:
      return (data.GetAt(pos).Stats.flags.NorthLocked > 0); 
    case FACE_EAST:
      return (data.GetAt(pos).Stats.flags.EastLocked > 0); 
    case FACE_SOUTH:
      return (data.GetAt(pos).Stats.flags.SouthLocked > 0); 
    case FACE_WEST:
      return (data.GetAt(pos).Stats.flags.WestLocked > 0); 
    }
  }
  return TRUE;
}

//*****************************************************************************
//    NAME: BLOCKAGE_STATUS::ClearLocked
//
// PURPOSE: 
//
//*****************************************************************************
void BLOCKAGE_STATUS::ClearLocked(int level, int x, int y, int facing)
{ 
  POSITION pos = FindData(level, x, y);

  if (pos != NULL) // already have data for this spot
  {
    switch (facing)
    {
    case FACE_NORTH:
      data.GetAt(pos).Stats.flags.NorthLocked = 0;
      break;
    case FACE_EAST:
      data.GetAt(pos).Stats.flags.EastLocked = 0; 
      break;
    case FACE_SOUTH:
      data.GetAt(pos).Stats.flags.SouthLocked = 0; 
      break;
    case FACE_WEST:
      data.GetAt(pos).Stats.flags.WestLocked = 0; 
      break;    
    }
  }
  else // add data for this spot with locked bit clear
  {
    BlockageDataType temp; // all bits default to 1
    temp.level = level;
    temp.x = x;
    temp.y = y;
    switch (facing) // clear locked bit
    {
    case FACE_NORTH:
      temp.Stats.flags.NorthLocked = 0;
      break;
    case FACE_EAST:
      temp.Stats.flags.EastLocked = 0; 
      break;
    case FACE_SOUTH:
      temp.Stats.flags.SouthLocked = 0; 
      break;
    case FACE_WEST:
      temp.Stats.flags.WestLocked = 0; 
      break;    
    }
    data.AddTail(temp);
  }
}

//*****************************************************************************
//    NAME: BLOCKAGE_STATUS::IsSpelled
//
// PURPOSE: 
//
//*****************************************************************************
BOOL BLOCKAGE_STATUS::IsSpelled(int level, int x, int y, int facing)
{ 
  POSITION pos = FindData(level, x, y);

  if (pos != NULL)
  {
    switch (facing)
    {
    case FACE_NORTH:
      return (data.GetAt(pos).Stats.flags.NorthSpelled > 0); 
    case FACE_EAST:
      return (data.GetAt(pos).Stats.flags.EastSpelled > 0); 
    case FACE_SOUTH:
      return (data.GetAt(pos).Stats.flags.SouthSpelled > 0); 
    case FACE_WEST:
      return (data.GetAt(pos).Stats.flags.WestSpelled > 0); 
    }
  }
  return TRUE;
}

//*****************************************************************************
//    NAME: BLOCKAGE_STATUS::IsSpelledSecret
//
// PURPOSE: 
//
//*****************************************************************************
BOOL BLOCKAGE_STATUS::IsSpelledSecret(int level, int x, int y, int facing)
{ 
  POSITION pos = FindData(level, x, y);

  if (pos != NULL)
  {
    switch (facing)
    {
    case FACE_NORTH:
      return    (data.GetAt(pos).Stats.flags.NorthSpelled > 0) 
             || (data.GetAt(pos).Stats.flags.NorthSecret > 0); 
    case FACE_EAST:
      return    (data.GetAt(pos).Stats.flags.EastSpelled > 0) 
             || (data.GetAt(pos).Stats.flags.EastSecret > 0); 
    case FACE_SOUTH:
      return    (data.GetAt(pos).Stats.flags.SouthSpelled > 0) 
             || (data.GetAt(pos).Stats.flags.SouthSecret > 0); 
    case FACE_WEST:
      return    (data.GetAt(pos).Stats.flags.WestSpelled > 0) 
             || (data.GetAt(pos).Stats.flags.WestSecret > 0); 
    }
  }
  return TRUE;
}

//*****************************************************************************
//    NAME: BLOCKAGE_STATUS::IsLockedSecret
//
// PURPOSE: 
//
//*****************************************************************************
BOOL BLOCKAGE_STATUS::IsLockedSecret(int level, int x, int y, int facing)
{ 
  POSITION pos = FindData(level, x, y);

  if (pos != NULL)
  {
    switch (facing)
    {
    case FACE_NORTH:
      return    (data.GetAt(pos).Stats.flags.NorthSecret > 0) 
             || (data.GetAt(pos).Stats.flags.NorthLocked > 0); 
    case FACE_EAST:
      return    (data.GetAt(pos).Stats.flags.EastSecret > 0) 
             || (data.GetAt(pos).Stats.flags.EastLocked > 0); 
    case FACE_SOUTH:
      return    (data.GetAt(pos).Stats.flags.SouthSecret > 0) 
             || (data.GetAt(pos).Stats.flags.SouthLocked > 0); 
    case FACE_WEST:
      return    (data.GetAt(pos).Stats.flags.WestSecret > 0) 
             || (data.GetAt(pos).Stats.flags.WestLocked > 0); 
    }
  }
  return TRUE;
}

//*****************************************************************************
//    NAME: BLOCKAGE_STATUS::ClearSpelled
//
// PURPOSE: 
//
//*****************************************************************************
void BLOCKAGE_STATUS::ClearSpelled(int level, int x, int y, int facing)
{ 
  POSITION pos = FindData(level, x, y);

  if (pos != NULL) // already have data for this spot
  {
    switch (facing)
    {
    case FACE_NORTH:
      data.GetAt(pos).Stats.flags.NorthSpelled = 0;
      break;
    case FACE_EAST:
      data.GetAt(pos).Stats.flags.EastSpelled = 0; 
      break;
    case FACE_SOUTH:
      data.GetAt(pos).Stats.flags.SouthSpelled = 0; 
      break;
    case FACE_WEST:
      data.GetAt(pos).Stats.flags.WestSpelled = 0; 
      break;    
    }
  }
  else // add data for this spot with spelled bit clear
  {
    BlockageDataType temp; // all bits default to 1
    temp.level = level;
    temp.x = x;
    temp.y = y;
    switch (facing) // clear spelled bit
    {
    case FACE_NORTH:
      temp.Stats.flags.NorthSpelled = 0;
      break;
    case FACE_EAST:
      temp.Stats.flags.EastSpelled = 0; 
      break;
    case FACE_SOUTH:
      temp.Stats.flags.SouthSpelled = 0; 
      break;
    case FACE_WEST:
      temp.Stats.flags.WestSpelled = 0; 
      break;    
    }
    data.AddTail(temp);
  }
}

//*****************************************************************************
//*****************************************************************************
//*****************************************************************************
//*****************************************************************************
//
//
//  CHARACTER functions
//
//

//*****************************************************************************
//    NAME: CHARACTER::HaveSpell
//
// PURPOSE: 
//
//*****************************************************************************
BOOL CHARACTER::HaveSpell(int spelldbkey, BOOL checkmemorized)
{
  if (spelldbkey < 0) return FALSE;
  POSITION pos = spells.list.GetHeadPosition();
  while (pos != NULL)
  {
    if (spells.list.GetAt(pos).spell == spelldbkey)
    {
      if (checkmemorized)
        return (spells.list.GetAt(pos).IsMemorized());
      else
        return TRUE;
    }
    spells.list.GetNext(pos);
  }
  return FALSE;
}

//*****************************************************************************
//    NAME: CHARACTER::GetSpellBookKey
//
// PURPOSE: 
//
//*****************************************************************************
int CHARACTER::GetSpellBookKey(int spelldbkey, BOOL checkmemorized)
{
  if (spelldbkey < 0) return -1;
  POSITION pos = spells.list.GetHeadPosition();
  while (pos != NULL)
  {
    if (spells.list.GetAt(pos).spell == spelldbkey)
    {
      if (checkmemorized)
      {
        if (spells.list.GetAt(pos).IsMemorized())
          return (spells.list.GetKeyAt(pos));
        else
          return -1;
      }
      else
        return (spells.list.GetKeyAt(pos));
    }
    spells.list.GetNext(pos);
  }
  return -1;
}

//*****************************************************************************
//    NAME: CHARACTER::operator=
//
// PURPOSE: 
//
//*****************************************************************************
CHARACTER& CHARACTER::operator=(const CHARACTER& dude)
{
  if (&dude == this) return *this;
  Clear();
   type = dude.type;
   race(dude.race());
   gender = dude.gender;
   charClass = dude.charClass;
   alignment = dude.alignment;
   allowInCombat = dude.allowInCombat;
   status = dude.status;
   undead = dude.undead;
   creatureSize = dude.creatureSize;
   if (!dude.name.IsEmpty())
      name = dude.name;
   else
      name="";

   DisableTalkIfDead=dude.DisableTalkIfDead;
   TalkEvent=dude.TalkEvent;
   TalkLabel=dude.TalkLabel;
   ExamineEvent=dude.ExamineEvent;
   ExamineLabel=dude.ExamineLabel;

   FighterExperience = dude.FighterExperience;
	ClericExperience = dude.ClericExperience;
	RangerExperience =  dude.RangerExperience;
	PaladinExperience = dude.PaladinExperience;
	MUExperience = dude.MUExperience;
	ThiefExperience = dude.ThiefExperience;
   DruidExperience = dude.DruidExperience;

   THACO = dude.THACO;
   morale = dude.morale;
   encumbrance = dude.encumbrance;
   maxEncumbrance = dude.maxEncumbrance;
   AC = dude.AC;
   hitPoints = dude.hitPoints;
   maxHitPoints = dude.maxHitPoints;
   nbrHitDice = dude.nbrHitDice;
   age = dude.age;
   maxAge = dude.maxAge;
   birthday=dude.birthday;
   maxCureDisease = dude.maxCureDisease;
   unarmedDieS = dude.unarmedDieS;
   unarmedNbrDieS = dude.unarmedNbrDieS;
   unarmedBonus = dude.unarmedBonus;
   unarmedDieL = dude.unarmedDieL;
   unarmedNbrDieL = dude.unarmedNbrDieL;
   ItemMask = dude.ItemMask;
   maxMovement = dude.maxMovement;
   readyToTrain = dude.readyToTrain;
   canTradeItems=dude.canTradeItems;
   strength = dude.strength;
   strengthMod = dude.strengthMod;
   intelligence = dude.intelligence;
   wisdom = dude.wisdom;
   dexterity = dude.dexterity;
   constitution = dude.constitution;
   charisma = dude.charisma;
   strengthKey=dude.strengthKey;
   intelligenceKey=dude.intelligenceKey;
   wisdomKey=dude.wisdomKey;
   dexterityKey=dude.dexterityKey;
   constitutionKey=dude.constitutionKey;
   charismaKey=dude.charismaKey;
   openDoors = dude.openDoors;
   openMagicDoors = dude.openMagicDoors;
   BB_LG = dude.BB_LG;
   automatic = dude.automatic;
   allowPlayerControl=dude.allowPlayerControl;
   //detectingMagic=dude.detectingMagic;
   detectingInvisible=dude.detectingInvisible;
   detectingTraps=dude.detectingTraps;
   hitBonus = dude.hitBonus;
   dmgBonus = dude.dmgBonus;
   magicResistance = dude.magicResistance;
   saveVsParPsnDMag = dude.saveVsParPsnDMag;
   saveVsPetPoly = dude.saveVsPetPoly;
   saveVsRdStWnd = dude.saveVsRdStWnd;
   saveVsBreath = dude.saveVsBreath;
   saveVsSpell = dude.saveVsSpell;
   pickPockets = dude.pickPockets;
   openLocks = dude.openLocks;
   findRemoveTrap = dude.findRemoveTrap;
   moveSilent = dude.moveSilent;
   hideInShadows = dude.hideInShadows;
   hearNoise = dude.hearNoise;
   climbWalls = dude.climbWalls;
   readLanguages = dude.readLanguages;
   FighterLevel = dude.FighterLevel;
   ClericLevel = dude.ClericLevel;
   RangerLevel = dude.RangerLevel;
   PaladinLevel = dude.PaladinLevel;
   MULevel = dude.MULevel;
   ThiefLevel = dude.ThiefLevel;
   DruidLevel = dude.DruidLevel;
   prevFighterLevel = dude.prevFighterLevel;
   prevClericLevel = dude.prevClericLevel;
   prevRangerLevel = dude.prevRangerLevel;
   prevPaladinLevel = dude.prevPaladinLevel;
   prevMULevel = dude.prevMULevel;
   prevThiefLevel = dude.prevThiefLevel;
   prevDruidLevel = dude.prevDruidLevel;
   pdFighterLevel = dude.pdFighterLevel;
   pdClericLevel = dude.pdClericLevel;
   pdRangerLevel = dude.pdRangerLevel;
   pdPaladinLevel = dude.pdPaladinLevel;
   pdMULevel = dude.pdMULevel;
   pdThiefLevel = dude.pdThiefLevel;
   pdDruidLevel = dude.pdDruidLevel;
   IsPreGen = dude.IsPreGen;
   CanBeSaved = dude.CanBeSaved;
   HasLayedOnHandsToday=dude.HasLayedOnHandsToday;
   money = dude.money;
   NbrAttacks = dude.NbrAttacks;
   myItems = dude.myItems;
   key = dude.key;
   icon = dude.icon;
   iconIndex=dude.iconIndex;
   smallPic = dude.smallPic;
   origIndex = dude.origIndex;
   marchOrder = dude.marchOrder;
   specAbs = dude.specAbs;   
   blockageData = dude.blockageData;
   spells = dude.spells;
   char_asl=dude.char_asl;
   *m_traits=*dude.m_traits;
   spellEffects.RemoveAll();
   POSITION pos = dude.spellEffects.GetHeadPosition();
   SPELL_EFFECTS_DATA tmp;
   while (pos != NULL)
   {     
     const SPELL_EFFECTS_DATA tmpref=dude.spellEffects.GetNext(pos);
     tmp=tmpref;
     spellEffects.AddTail(tmp);
   }

  specAbQueue.RemoveAll();
  pos = dude.specAbQueue.GetHeadPosition();
  while (pos!=NULL)
  {
    SQueuedSpecAb data = dude.specAbQueue.GetNext(pos);
    specAbQueue.AddTail(data);
  }

   //m_Properties = dude.m_Properties;
   
   return *this;
}

//*****************************************************************************
//    NAME: CHARACTER::operator==
//
// PURPOSE: 
//
//*****************************************************************************
BOOL CHARACTER::operator==(const CHARACTER& dude)
{
  if (&dude == this) return TRUE;
  if (GetType() != dude.GetType()) return FALSE;
  if (race() != dude.race()) return FALSE;
  if (gender != dude.gender) return FALSE;
  if (charClass != dude.charClass) return FALSE;
  if (alignment != dude.alignment) return FALSE;
  if (allowInCombat != dude.allowInCombat) return FALSE;
  if (status != dude.status) return FALSE;
  if (undead != dude.undead) return FALSE;
  if (creatureSize != dude.creatureSize) return FALSE;
  if (name != dude.name) return FALSE;
  if (FighterExperience != dude.FighterExperience) return FALSE;
	if (ClericExperience != dude.ClericExperience) return FALSE;
	if (RangerExperience !=  dude.RangerExperience) return FALSE;
	if (PaladinExperience != dude.PaladinExperience) return FALSE;
	if (MUExperience != dude.MUExperience) return FALSE;
	if (ThiefExperience != dude.ThiefExperience) return FALSE;
  if (DruidExperience != dude.DruidExperience) return FALSE;
  if (THACO != dude.THACO) return FALSE;
  if (morale != dude.morale) return FALSE;
  if (encumbrance != dude.encumbrance) return FALSE;
  if (maxEncumbrance != dude.maxEncumbrance) return FALSE;
  if (AC != dude.AC) return FALSE;
  if (hitPoints != dude.hitPoints) return FALSE;
  if (maxHitPoints != dude.maxHitPoints) return FALSE;
  if (!(nbrHitDice == dude.nbrHitDice)) return FALSE;
  if (age != dude.age) return FALSE;
  if (maxAge != dude.maxAge) return FALSE;
  if (birthday!=dude.birthday) return FALSE;
  if (maxCureDisease != dude.maxCureDisease) return FALSE;
  if (unarmedDieS != dude.unarmedDieS) return FALSE;
  if (unarmedNbrDieS != dude.unarmedNbrDieS) return FALSE;
  if (unarmedBonus != dude.unarmedBonus) return FALSE;
  if (unarmedDieL != dude.unarmedDieL) return FALSE;
  if (unarmedNbrDieL != dude.unarmedNbrDieL) return FALSE;
  if (ItemMask != dude.ItemMask) return FALSE;
  if (maxMovement != dude.maxMovement) return FALSE;
  if (readyToTrain != dude.readyToTrain) return FALSE;
  if (canTradeItems != dude.canTradeItems) return FALSE;
  if (strength != dude.strength) return FALSE;
  if (strengthMod != dude.strengthMod) return FALSE;
  if (intelligence != dude.intelligence) return FALSE;
  if (wisdom != dude.wisdom) return FALSE;
  if (dexterity != dude.dexterity) return FALSE;
  if (constitution != dude.constitution) return FALSE;
  if (charisma != dude.charisma) return FALSE;
  if (strengthKey != dude.strengthKey) return FALSE;
  if (intelligenceKey != dude.intelligenceKey) return FALSE;
  if (wisdomKey != dude.wisdomKey) return FALSE;
  if (dexterityKey != dude.dexterityKey) return FALSE;
  if (constitutionKey != dude.constitutionKey) return FALSE;
  if (charismaKey != dude.charismaKey) return FALSE;
  if (openDoors != dude.openDoors) return FALSE;
  if (openMagicDoors != dude.openMagicDoors) return FALSE;
  if (BB_LG != dude.BB_LG) return FALSE;
  if (automatic != dude.automatic) return FALSE;
  if (allowPlayerControl != dude.allowPlayerControl) return FALSE;
  //if (detectingMagic != dude.detectingMagic) return FALSE;
  if (detectingInvisible!=dude.detectingInvisible) return FALSE;
  if (detectingTraps!=dude.detectingTraps) return FALSE;
  if (hitBonus != dude.hitBonus) return FALSE;
  if (dmgBonus != dude.dmgBonus) return FALSE;
  if (magicResistance != dude.magicResistance) return FALSE;
  if (saveVsParPsnDMag != dude.saveVsParPsnDMag) return FALSE;
  if (saveVsPetPoly != dude.saveVsPetPoly) return FALSE;
  if (saveVsRdStWnd != dude.saveVsRdStWnd) return FALSE;
  if (saveVsBreath != dude.saveVsBreath) return FALSE;
  if (saveVsSpell != dude.saveVsSpell) return FALSE;
  if (pickPockets != dude.pickPockets) return FALSE;
  if (openLocks != dude.openLocks) return FALSE;
  if (findRemoveTrap != dude.findRemoveTrap) return FALSE;
  if (moveSilent != dude.moveSilent) return FALSE;
  if (hideInShadows != dude.hideInShadows) return FALSE;
  if (hearNoise != dude.hearNoise) return FALSE;
  if (climbWalls != dude.climbWalls) return FALSE;
  if (readLanguages != dude.readLanguages) return FALSE;
  if (FighterLevel != dude.FighterLevel) return FALSE;
  if (ClericLevel != dude.ClericLevel) return FALSE;
  if (RangerLevel != dude.RangerLevel) return FALSE;
  if (PaladinLevel != dude.PaladinLevel) return FALSE;
  if (MULevel != dude.MULevel) return FALSE;
  if (ThiefLevel != dude.ThiefLevel) return FALSE;
  if (DruidLevel != dude.DruidLevel) return FALSE;
  if (prevFighterLevel != dude.prevFighterLevel) return FALSE;
  if (prevClericLevel != dude.prevClericLevel) return FALSE;
  if (prevRangerLevel != dude.prevRangerLevel) return FALSE;
  if (prevPaladinLevel != dude.prevPaladinLevel) return FALSE;
  if (prevMULevel != dude.prevMULevel) return FALSE;
  if (prevThiefLevel != dude.prevThiefLevel) return FALSE;
  if (prevDruidLevel != dude.prevDruidLevel) return FALSE;
  if (pdFighterLevel != dude.pdFighterLevel) return FALSE;
  if (pdClericLevel != dude.pdClericLevel) return FALSE;
  if (pdRangerLevel != dude.pdRangerLevel) return FALSE;
  if (pdPaladinLevel != dude.pdPaladinLevel) return FALSE;
  if (pdMULevel != dude.pdMULevel) return FALSE;
  if (pdThiefLevel != dude.pdThiefLevel) return FALSE;
  if (pdDruidLevel != dude.pdDruidLevel) return FALSE;
  if (IsPreGen != dude.IsPreGen) return FALSE;
  if (CanBeSaved != dude.CanBeSaved) return FALSE;
  if (HasLayedOnHandsToday != dude.HasLayedOnHandsToday) return FALSE;
  if (DisableTalkIfDead != dude.DisableTalkIfDead) return FALSE;
  if (TalkEvent != dude.TalkEvent) return FALSE;
  if (TalkLabel!=dude.TalkLabel) return FALSE;
  if (ExamineEvent != dude.ExamineEvent) return FALSE;
  if (ExamineLabel!=dude.ExamineLabel) return FALSE;
  if (! (money==dude.money) ) return FALSE;
  if (!(NbrAttacks == dude.NbrAttacks)) return FALSE;
  if (key != dude.key) return FALSE;
  if (origIndex != dude.origIndex) return FALSE;
  if (marchOrder != dude.marchOrder) return FALSE;

  if ((char_asl == dude.char_asl)==FALSE) return FALSE;
  if ((*m_traits == *dude.m_traits)==FALSE) return FALSE;
  if ((specAbs == dude.specAbs)==FALSE) return FALSE;
  if ((blockageData == dude.blockageData)==FALSE) return FALSE;
  if ((myItems.operator==(dude.myItems))==FALSE) return FALSE;    
  if ((spells == dude.spells)==FALSE) return FALSE;  

  if (!(icon == dude.icon)) return FALSE;
  if (iconIndex != dude.iconIndex) return FALSE;
  if ((smallPic == dude.smallPic)==FALSE) return FALSE;
#ifdef UAFEngine
  if (spellEffects.GetCount() != dude.spellEffects.GetCount()) return false;
  POSITION pos = dude.spellEffects.GetHeadPosition();
  while (pos != NULL)
    if (!HaveEffectAttr(dude.spellEffects.GetNext(pos))) return false;
#endif

    //if (m_Properties != dude.m_Properties) return false;
    
  return TRUE;
}

//*****************************************************************************
//    NAME: CHARACTER::IsSameCharacter
//
// PURPOSE: When testing for duplicate characters, not all fields can
//          be compared, and an exact comparison is not desired.
//          For this test, only compare data that will not change
//          over time. Things that change as a character adventures
//          and levels up should not be included.
//
//*****************************************************************************
BOOL CHARACTER::IsSameCharacter(const CHARACTER& dude)
{
  if (&dude == this) return TRUE;
  if (GetType() != dude.GetType()) return FALSE;
  if (IsPreGen != dude.IsPreGen) return FALSE;
  if (name != dude.name) return FALSE;  
  return TRUE;
}

//*****************************************************************************
// NAME:    CHARACTER::CHARACTER
//
// PURPOSE: Constructor for CHARACTER class
//
// RETURNS: None
//*****************************************************************************
CHARACTER::CHARACTER(void) : 
  char_asl("CHARACTER_ATTRIBUTES"),
  TalkEvent(0),
  ExamineEvent(0)
{
  m_traits=new TRAITS;
  Clear(TRUE);

  /*
  m_Properties.Clear();
  for (int i=FIRST_CHAR_KEYWORD; i<=LAST_CHAR_KEYWORD; i++)
  {
    if (!IsDataReadOnly(i))
    {
      CBaseProperty prop(NumToKeywordText(i));
      m_Properties.UpdateProperty(prop);
    }
  }
  */
}
//*****************************************************************************
// NAME:    CHARACTER::CHARACTER
//
// PURPOSE: Copy Constructor for CHARACTER class
//
// RETURNS: None
//*****************************************************************************
CHARACTER::CHARACTER(const CHARACTER& dude) : 
  char_asl("CHARACTER_ATTRIBUTES"),
  TalkEvent(0),
  ExamineEvent(0)
{
  m_traits=new TRAITS;
  Clear(TRUE);  
  *this = dude;
}

//*****************************************************************************
// NAME:    CHARACTER::~CHARACTER
//
// PURPOSE: Destructor of CHARACTER class
//
// RETURNS: None
//*****************************************************************************
CHARACTER::~CHARACTER(void)
{
  Clear(TRUE);
  if (m_traits!=NULL) delete m_traits;
  m_traits=NULL;
}

//*****************************************************************************
// NAME:    CHARACTER::Clear
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
void CHARACTER::Clear(BOOL IsConstructor)
{
   //if (TalkEvent > 0)
   //  globalData.eventData.DeleteEvents(TalkEvent);
  DisableTalkIfDead=TRUE;
   TalkEvent=0;
   TalkLabel="TALK";
   //if (ExamineEvent > 0)
   //  globalData.eventData.DeleteEvents(ExamineEvent);
   ExamineEvent=0;
   ExamineLabel="EXAMINE";
   key = 0;
   type = CHAR_TYPE;
   race(Human);
   gender = Male;
   charClass = Fighter;
   alignment = ChaoticNeutral;
   allowInCombat = TRUE;
   status = Unconscious;
   undead = NotUndead;
   creatureSize = Medium;
   name="";
   FighterExperience = 0;
	 ClericExperience = 0;
	 RangerExperience =  0;
	 PaladinExperience = 0;
	 MUExperience = 0;
	 ThiefExperience = 0;
   DruidExperience = 0;
   THACO = 20;
   morale = 50; // 0.100%
   encumbrance = 0;
   maxEncumbrance = 0;
   AC = 10;
   hitPoints = 0;
   maxHitPoints = 0;
   nbrHitDice = 0;
   age = 0;
   maxAge = 0;
   SetBirthday(1);
   maxCureDisease = 0;
   usedCureDisease=0;
   unarmedDieS = 0;
   unarmedNbrDieS = 0;
   unarmedBonus = 0;
   unarmedDieL = 0;
   unarmedNbrDieL = 0;
   ItemMask = 0;
   maxMovement = 0;
   readyToTrain = FALSE;
   canTradeItems=TRUE;
   strength = 0;
   strengthMod = 0;
   intelligence = 0;
   wisdom = 0;
   dexterity = 0;
   constitution = 0;
   charisma = 0;
   openDoors = 0;
   openMagicDoors = 0;
   BB_LG = 0;
   automatic = FALSE;
   allowPlayerControl=TRUE;
   //detectingMagic=FALSE;
   detectingInvisible=FALSE;
   detectingTraps=FALSE;
   hitBonus = 0;
   dmgBonus = 0;
   magicResistance = 0;
   saveVsParPsnDMag = 0;
   saveVsPetPoly = 0;
   saveVsRdStWnd = 0;
   saveVsBreath = 0;
   saveVsSpell = 0;
   pickPockets = 0;
   openLocks = 0;
   findRemoveTrap = 0;
   moveSilent = 0;
   hideInShadows = 0;
   hearNoise = 0;
   climbWalls = 0;
   readLanguages = 0;
   FighterLevel = 0;
   ClericLevel = 0;
   RangerLevel = 0;
   PaladinLevel = 0;
   MULevel = 0;
   ThiefLevel = 0;
   DruidLevel = 0;
   prevFighterLevel = 0;
   prevClericLevel = 0;
   prevRangerLevel = 0;
   prevPaladinLevel = 0;
   prevMULevel = 0;
   prevThiefLevel = 0;
   prevDruidLevel = 0;
   pdFighterLevel = 0;
   pdClericLevel = 0;
   pdRangerLevel = 0;
   pdPaladinLevel = 0;
   pdMULevel = 0;
   pdThiefLevel = 0;
   pdDruidLevel = 0;
   IsPreGen = FALSE;
   CanBeSaved=TRUE;
   HasLayedOnHandsToday=FALSE;
   money.Clear();
   NbrAttacks = 0.0;
   icon.Clear();
   iconIndex=1;
   smallPic.Clear();
   origIndex = -1;
   marchOrder = 0;
   specAbs.Clear();
   specAbQueue.RemoveAll();
   blockageData.Clear();
   spells.Clear();
   myItems.Clear();
   char_asl.Clear();
   spellEffects.RemoveAll();
   //m_Properties.ClearAllMods();

   if (!IsConstructor)
   {
     strengthKey     = abilityData.GetKeyByIndex(0);
     intelligenceKey = abilityData.GetKeyByIndex(1);
     wisdomKey       = abilityData.GetKeyByIndex(2);
     dexterityKey    = abilityData.GetKeyByIndex(3);
     constitutionKey = abilityData.GetKeyByIndex(4);
     charismaKey     = abilityData.GetKeyByIndex(5);
   }
   else
   {
     strengthKey     = abilityUnknown;
     intelligenceKey = abilityUnknown;
     wisdomKey       = abilityUnknown;
     dexterityKey    = abilityUnknown;
     constitutionKey = abilityUnknown;
     charismaKey     = abilityUnknown;
   }
}

//*****************************************************************************
// NAME:    CHARACTER::Serialize
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
void CHARACTER::Serialize(CArchive &ar, double version)
{
  // This data can be serialized along with the globalData, and
  // also separately as a 'CharName.CHR' file.
  // The version specified distinguishes between saved char file 
  // version and game.dat file version
  //
  CObject::Serialize(ar);
  int temp;

  if (ar.IsStoring())
  {
    ar << key;
    ar << type;
    temp = m_race;
    ar << temp;
    temp = gender;
    ar << temp;
    temp = charClass;
    ar << temp;
    temp = alignment;
    ar << temp;
    
    ar << allowInCombat;

    temp = status;
    ar << temp;
    temp = undead;
    ar << temp;
    temp = creatureSize;
    ar << temp;

    AS(ar,name);

    ar << FighterExperience;
    ar << ClericExperience;
    ar << RangerExperience;
    ar << PaladinExperience;
    ar << MUExperience;
    ar << ThiefExperience;
    ar << DruidExperience;
    ar << THACO;
    ar << morale;
    ar << encumbrance;
    ar << maxEncumbrance;
    ar << AC;
    ar << hitPoints;
    ar << maxHitPoints;
    ar << nbrHitDice;
    ar << age;
    ar << maxAge;
    ar << birthday;
    ar << maxCureDisease;
    ar << unarmedDieS;
    ar << unarmedNbrDieS;
    ar << unarmedBonus;
    ar << unarmedDieL;
    ar << unarmedNbrDieL;
    ar << ItemMask;
    ar << maxMovement;
    ar << readyToTrain;
    ar << canTradeItems;   
    
    ar << strength;
    ar << strengthMod;
    ar << intelligence;
    ar << wisdom;
    ar << dexterity;
    ar << constitution;
    ar << charisma;
    ar << openDoors;
    ar << openMagicDoors;
    ar << BB_LG;
    ar << hitBonus;
    ar << dmgBonus;
    ar << magicResistance;
    ar << saveVsParPsnDMag;
    ar << saveVsPetPoly;
    ar << saveVsRdStWnd;
    ar << saveVsBreath;
    ar << saveVsSpell;
    ar << pickPockets;
    ar << openLocks;
    ar << findRemoveTrap;
    ar << moveSilent;
    ar << hideInShadows;
    ar << hearNoise;
    ar << climbWalls;
    ar << readLanguages;
    ar << FighterLevel;
    ar << ClericLevel;
    ar << RangerLevel;
    ar << PaladinLevel;
    ar << MULevel;
    ar << ThiefLevel;
    ar << DruidLevel;
    ar << prevFighterLevel;
    ar << prevClericLevel;
    ar << prevRangerLevel;
    ar << prevPaladinLevel;
    ar << prevMULevel;
    ar << prevThiefLevel;
    ar << prevDruidLevel;
    ar << pdFighterLevel;
    ar << pdClericLevel;
    ar << pdRangerLevel;
    ar << pdPaladinLevel;
    ar << pdMULevel;
    ar << pdThiefLevel;
    ar << pdDruidLevel;
    ar << IsPreGen;
    ar << CanBeSaved;
    ar << HasLayedOnHandsToday;
    money.Serialize(ar, version);
    ar << NbrAttacks;
    icon.Serialize(ar,version);
    ar << iconIndex;
    ar << origIndex;
    ar << marchOrder;
    ar << DisableTalkIfDead;
    ar << TalkEvent;
    AS(ar, TalkLabel);
    ar << ExamineEvent;
    AS(ar, ExamineLabel);
    
    //ar << spells;
    spells.Serialize(ar, version);

    //ar << detectingMagic;
    ar << detectingInvisible;
    ar << detectingTraps;
    ar << spellEffects.GetCount();
    POSITION pos = spellEffects.GetHeadPosition();
    while (pos!=NULL)
      spellEffects.GetNext(pos).Serialize(ar,version);
    blockageData.Serialize(ar);
  }
  else
  {
    ar >> key;
    ar >> type;
    ar >> temp;
    race((raceType)temp);
    ar >> temp;
    gender = (genderType)temp;   
    ar >> temp;
    charClass = (classType)temp;   
    ar >> temp;
    alignment = (alignmentType)temp;   
    
    if (version >= _VERSION_0912_)
      ar >> allowInCombat;

    ar >> temp;
    status = (charStatusType)temp;   
    ar >> temp;
    undead = (undeadType)temp;
    ar >> temp;
    creatureSize = (creatureSizeType)temp;   
    DAS(ar,name);
    ar >> FighterExperience;
    ar >> ClericExperience;
    ar >> RangerExperience;
    ar >> PaladinExperience;
    ar >> MUExperience;
    ar >> ThiefExperience;
    ar >> DruidExperience;
    ar >> THACO;
    ar >> morale;
    ar >> encumbrance;
    ar >> maxEncumbrance;
    ar >> AC;
    ar >> hitPoints;
    ar >> maxHitPoints;
    ar >> nbrHitDice;
    ar >> age;
    ar >> maxAge;
    if (version >= _VERSION_0830_)
    {
      ar >> birthday;
      SetBirthday(birthday);
    }
    ar >> maxCureDisease;
    ar >> unarmedDieS;
    ar >> unarmedNbrDieS;
    ar >> unarmedBonus;
    ar >> unarmedDieL;
    ar >> unarmedNbrDieL;
    ar >> ItemMask;
    ar >> maxMovement;
    ar >> readyToTrain;
    if (version >= _VERSION_0695_)
      ar >> canTradeItems;

    ar >> strength;
    ar >> strengthMod;
    ar >> intelligence;
    ar >> wisdom;
    ar >> dexterity;
    ar >> constitution;
    ar >> charisma;
    ar >> openDoors;
    ar >> openMagicDoors;
    ar >> BB_LG;
    ar >> hitBonus;
    ar >> dmgBonus;
    ar >> magicResistance;
    ar >> saveVsParPsnDMag;
    ar >> saveVsPetPoly;
    ar >> saveVsRdStWnd;
    ar >> saveVsBreath;
    ar >> saveVsSpell;
    ar >> pickPockets;
    ar >> openLocks;
    ar >> findRemoveTrap;
    ar >> moveSilent;
    ar >> hideInShadows;
    ar >> hearNoise;
    ar >> climbWalls;
    ar >> readLanguages;
    ar >> FighterLevel;
    ar >> ClericLevel;
    ar >> RangerLevel;
    ar >> PaladinLevel;
    ar >> MULevel;
    ar >> ThiefLevel;
    ar >> DruidLevel;
    ar >> prevFighterLevel;
    ar >> prevClericLevel;
    ar >> prevRangerLevel;
    ar >> prevPaladinLevel;
    ar >> prevMULevel;
    ar >> prevThiefLevel;
    ar >> prevDruidLevel;
    ar >> pdFighterLevel;
    ar >> pdClericLevel;
    ar >> pdRangerLevel;
    ar >> pdPaladinLevel;
    ar >> pdMULevel;
    ar >> pdThiefLevel;
    ar >> pdDruidLevel;
    ar >> IsPreGen;
    
    if (version >= _VERSION_0698_)
      ar >> CanBeSaved;

    if (version >= _VERSION_0900_)
      ar >> HasLayedOnHandsToday;

    if (version < _VERSION_0661_)
    {
      ar >> temp; money.Coins[PlatinumType] = temp;
      ar >> temp; money.Coins[GoldType] = temp;
      ar >> temp; money.Coins[ElectrumType] = temp;
      ar >> temp; money.Coins[SilverType] = temp;
      ar >> temp; money.Coins[CopperType] = temp;
      ar >> temp; { for (int g=0;g<temp;g++) money.AddGem(); }
      ar >> temp; { for (int j=0;j<temp;j++) money.AddJewelry(); }
      if (version >= _VERSION_0660_)
      {
        ar >> temp; money.Coins[CoinType6] = temp;
        ar >> temp; money.Coins[CoinType7] = temp;
        ar >> temp; money.Coins[CoinType8] = temp;
        ar >> temp; money.Coins[CoinType9] = temp;
        ar >> temp; money.Coins[CoinType10] = temp;
      }
    }
    else
    {
      money.Serialize(ar, version);
    }
    ar >> NbrAttacks;
    
    if (version < _VERSION_0640_)
    {
      CString tmp;
      long notused;
      DAS(ar,tmp);
      ar >> notused;
      AddDesignFolderToPath(tmp);
      icon.filename = tmp;
      icon.picType = IconDib;
      icon.SetDefaults();
    }
    else
    {
      icon.Serialize(ar, version);
      if (version <= _VERSION_0840_)
        icon.SetPre0840NonLoopDefaults();
    }    
    if (version >= _VERSION_0640_)
      ar >> iconIndex;
    ar >> origIndex;
    ar >> marchOrder;

    if (version >= _VERSION_0870_)
      ar >> DisableTalkIfDead;
    if (version >= _VERSION_0662_)
      ar >> TalkEvent;
    if (version >= _VERSION_0710_)
      DAS(ar,TalkLabel);
    if (version >= _VERSION_0800_)
    {
      ar >> ExamineEvent;
      DAS(ar,ExamineLabel);
    }

    spells.Serialize(ar, version);

    if (version >= _VERSION_06991_)
    {
      if (version < _VERSION_0850_)
      {
        BOOL unused1;
        ar >> unused1;
      }
      ar >> detectingInvisible;
      ar >> detectingTraps;
    }
    if (version >= _VERSION_0630_)
    {
      if (version < _VERSION_06991_)
      {
        spellEffects.Serialize(ar);
      }
      else
      {
        spellEffects.RemoveAll();
        int count;
        ar >> count;
        for (int i=0;i<count;i++)
        {
          SPELL_EFFECTS_DATA data;
          data.Serialize(ar,version);
          spellEffects.AddTail(data);
          WriteDebugString("Loading spell effect for %s: %s\n",
                           GetName(), data.affectedAttr);
        }
      }
    }
    if (version >= _VERSION_0696_)
      blockageData.Serialize(ar);

    UpdateSpellAbility();
    spells.list.SortSpells();
  }

  smallPic.Serialize(ar, version);
  if (!ar.IsStoring())
  {
    if (version <= _VERSION_0840_)
      smallPic.SetPre0840LoopForeverDefaults();
  }

  myItems.Serialize(ar,version);
  specAbs.Serialize(ar, version);
  //specAbQueue: no need to save this data
  char_asl.Serialize(ar);

  POSITION pos = spells.list.GetHeadPosition();
  while (pos!=NULL)
  {
    SPELL_DATA *pData = spellData.GetSpellData(spells.list.GetAt(pos).spell);
    if (pData!=NULL)
    {
      WriteDebugString("%s Spell \'%s\', db key %u, sb key %u\n", 
                       GetName(),
                       pData->Name,
                       pData->key,
                       spells.list.GetAt(pos).spell);
    }
    else
      WriteDebugString("%s Spell missing, sb key %u\n", 
                       GetName(),
                       spells.list.GetAt(pos).spell);

    spells.list.GetNext(pos);
  }


  //m_Properties.Serialize(ar, version);

  // just to make sure we catch this for some older
  // character's that might come from saved games
  strengthKey     = abilityData.GetKeyByIndex(0);
  intelligenceKey = abilityData.GetKeyByIndex(1);
  wisdomKey       = abilityData.GetKeyByIndex(2);
  dexterityKey    = abilityData.GetKeyByIndex(3);
  constitutionKey = abilityData.GetKeyByIndex(4);
  charismaKey     = abilityData.GetKeyByIndex(5);

  // design settings that affect encumbrance might
  // have changed since character was last saved
  encumbrance = determineEffectiveEncumbrance();
  
  DisplayCurrSpellEffects("Serialize");
}


//*****************************************************************************
//    NAME: CHARACTER::Serialize
//
// PURPOSE: 
//
//*****************************************************************************
void CHARACTER::Serialize(CAR &ar, double version)
{
  // This data can be serialized along with the globalData, and
  // also separately as a 'CharName.CHR' file.
  // The version specified distinguishes between saved char file 
  // version and game.dat file version
  //
  int temp;
  if (ar.IsStoring())
  {
    ar << key;
    ar << type;
    temp = m_race;
    ar << temp;
    temp = gender;
    ar << temp;
    temp = charClass;
    ar << temp;
    temp = alignment;
    ar << temp;
    ar << allowInCombat;
    temp = status;
    ar << temp;
    temp = undead;
    ar << temp;
    temp = creatureSize;
    ar << temp;

    AS(ar,name);
    ar << FighterExperience;
    ar << ClericExperience;
    ar << RangerExperience;
    ar << PaladinExperience;
    ar << MUExperience;
    ar << ThiefExperience;
    ar << DruidExperience;
    ar << THACO;
    ar << morale;
    ar << encumbrance;
    ar << maxEncumbrance;
    ar << AC;
    ar << hitPoints;
    ar << maxHitPoints;
    ar << nbrHitDice;
    ar << age;
    ar << maxAge;
    ar << birthday;
    ar << maxCureDisease;
    ar << unarmedDieS;
    ar << unarmedNbrDieS;
    ar << unarmedBonus;
    ar << unarmedDieL;
    ar << unarmedNbrDieL;
    ar << ItemMask;
    ar << maxMovement;
    ar << readyToTrain;
    ar << canTradeItems;

    ar << strength;
    ar << strengthMod;
    ar << intelligence;
    ar << wisdom;
    ar << dexterity;
    ar << constitution;
    ar << charisma;
    ar << openDoors;
    ar << openMagicDoors;
    ar << BB_LG;
    ar << hitBonus;
    ar << dmgBonus;
    ar << magicResistance;
    ar << saveVsParPsnDMag;
    ar << saveVsPetPoly;
    ar << saveVsRdStWnd;
    ar << saveVsBreath;
    ar << saveVsSpell;
    ar << pickPockets;
    ar << openLocks;
    ar << findRemoveTrap;
    ar << moveSilent;
    ar << hideInShadows;
    ar << hearNoise;
    ar << climbWalls;
    ar << readLanguages;
    ar << FighterLevel;
    ar << ClericLevel;
    ar << RangerLevel;
    ar << PaladinLevel;
    ar << MULevel;
    ar << ThiefLevel;
    ar << DruidLevel;
    ar << prevFighterLevel;
    ar << prevClericLevel;
    ar << prevRangerLevel;
    ar << prevPaladinLevel;
    ar << prevMULevel;
    ar << prevThiefLevel;
    ar << prevDruidLevel;
    ar << pdFighterLevel;
    ar << pdClericLevel;
    ar << pdRangerLevel;
    ar << pdPaladinLevel;
    ar << pdMULevel;
    ar << pdThiefLevel;
    ar << pdDruidLevel;
    ar << IsPreGen;
    ar << CanBeSaved;
    ar << HasLayedOnHandsToday;
    money.Serialize(ar, version);
    ar << NbrAttacks;
    icon.Serialize(ar, version);
    ar << iconIndex;
    ar << origIndex;
    ar << marchOrder;
    ar << DisableTalkIfDead;
    ar << TalkEvent;
    AS(ar,TalkLabel);
    ar << ExamineEvent;
    AS(ar,ExamineLabel);

    spells.Serialize(ar, version);

    //ar << detectingMagic;
    ar << detectingInvisible;
    ar << detectingTraps;
    ar << spellEffects.GetCount();
    POSITION pos = spellEffects.GetHeadPosition();
    while (pos!=NULL)
      spellEffects.GetNext(pos).Serialize(ar,version);
    blockageData.Serialize(ar);
  }
  else
  {
    ar >> key;
    ar >> type;
    ar >> temp;
    race((raceType)temp);
    ar >> temp;
    gender = (genderType)temp;   
    ar >> temp;
    charClass = (classType)temp;   
    ar >> temp;
    alignment = (alignmentType)temp;   
    
    if (version >= _VERSION_0912_)
      ar >> allowInCombat;

    ar >> temp;
    status = (charStatusType)temp;   
    ar >> temp;
    undead = (undeadType)temp;
    ar >> temp;
    creatureSize = (creatureSizeType)temp;   
    DAS(ar,name);
    ar >> FighterExperience;
    ar >> ClericExperience;
    ar >> RangerExperience;
    ar >> PaladinExperience;
    ar >> MUExperience;
    ar >> ThiefExperience;
    ar >> DruidExperience;
    ar >> THACO;
    ar >> morale;
    ar >> encumbrance;
    ar >> maxEncumbrance;
    ar >> AC;
    ar >> hitPoints;
    ar >> maxHitPoints;
    ar >> nbrHitDice;
    ar >> age;
    ar >> maxAge;
    if (version >= _VERSION_0830_)
    {
      ar >> birthday;
      SetBirthday(birthday); // force a range check
    }
    ar >> maxCureDisease;
    ar >> unarmedDieS;
    ar >> unarmedNbrDieS;
    ar >> unarmedBonus;
    ar >> unarmedDieL;
    ar >> unarmedNbrDieL;
    ar >> ItemMask;
    ar >> maxMovement;
    ar >> readyToTrain;
    if (version >= _VERSION_0695_)
      ar >> canTradeItems;

    ar >> strength;
    ar >> strengthMod;
    ar >> intelligence;
    ar >> wisdom;
    ar >> dexterity;
    ar >> constitution;
    ar >> charisma;
    ar >> openDoors;
    ar >> openMagicDoors;
    ar >> BB_LG;
    ar >> hitBonus;
    ar >> dmgBonus;
    ar >> magicResistance;
    ar >> saveVsParPsnDMag;
    ar >> saveVsPetPoly;
    ar >> saveVsRdStWnd;
    ar >> saveVsBreath;
    ar >> saveVsSpell;
    ar >> pickPockets;
    ar >> openLocks;
    ar >> findRemoveTrap;
    ar >> moveSilent;
    ar >> hideInShadows;
    ar >> hearNoise;
    ar >> climbWalls;
    ar >> readLanguages;
    ar >> FighterLevel;
    ar >> ClericLevel;
    ar >> RangerLevel;
    ar >> PaladinLevel;
    ar >> MULevel;
    ar >> ThiefLevel;
    ar >> DruidLevel;
    ar >> prevFighterLevel;
    ar >> prevClericLevel;
    ar >> prevRangerLevel;
    ar >> prevPaladinLevel;
    ar >> prevMULevel;
    ar >> prevThiefLevel;
    ar >> prevDruidLevel;
    ar >> pdFighterLevel;
    ar >> pdClericLevel;
    ar >> pdRangerLevel;
    ar >> pdPaladinLevel;
    ar >> pdMULevel;
    ar >> pdThiefLevel;
    ar >> pdDruidLevel;
    ar >> IsPreGen;

    if (version >= _VERSION_0698_)
      ar >> CanBeSaved;
    
    if (version >= _VERSION_0900_)
      ar >> HasLayedOnHandsToday;

    if (version < _VERSION_0661_)
    {
      ar >> temp; money.Coins[PlatinumType] = temp;
      ar >> temp; money.Coins[GoldType] = temp;
      ar >> temp; money.Coins[ElectrumType] = temp;
      ar >> temp; money.Coins[SilverType] = temp;
      ar >> temp; money.Coins[CopperType] = temp;
      ar >> temp; { for (int g=0;g<temp;g++) money.AddGem(); }
      ar >> temp; { for (int j=0;j<temp;j++) money.AddJewelry(); }
      if (version >= _VERSION_0660_)
      {
        ar >> temp; money.Coins[CoinType6] = temp;
        ar >> temp; money.Coins[CoinType7] = temp;
        ar >> temp; money.Coins[CoinType8] = temp;
        ar >> temp; money.Coins[CoinType9] = temp;
        ar >> temp; money.Coins[CoinType10] = temp;
      }
    }
    else
    {
      money.Serialize(ar, version);
    }
    ar >> NbrAttacks;
    if (version < _VERSION_0640_)
    {
      CString tmp;
      long notused;
      DAS(ar,tmp);
      ar >> notused;
      AddDesignFolderToPath(tmp);
      icon.filename = tmp;
      icon.picType = IconDib;
      icon.SetDefaults();
    }
    else
    {
      icon.Serialize(ar, version);
      if (version <= _VERSION_0840_)
        icon.SetPre0840NonLoopDefaults();
    }
    if (version >= _VERSION_0640_)
      ar >> iconIndex;
    ar >> origIndex;
    ar >> marchOrder;

    if (version >= _VERSION_0870_)
      ar >> DisableTalkIfDead;
    if (version >= _VERSION_0662_)
      ar >> TalkEvent;
    if (version >= _VERSION_0710_)
      DAS(ar,TalkLabel);
    if (version >= _VERSION_0800_)
    {
      ar >> ExamineEvent;
      DAS(ar,ExamineLabel);
    }

    //ar >> spells;
    spells.Serialize(ar, version);


    if (version >= _VERSION_06991_)
    { 
      if (version < _VERSION_0850_)
      {
        BOOL unused1;
        ar >> unused1;
      }
      ar >> detectingInvisible;
      ar >> detectingTraps;
    }
    if (version >= _VERSION_0630_)
    {
      if (version < _VERSION_06991_)
      {
        spellEffects.Serialize(ar);
      }
      else
      {
        spellEffects.RemoveAll();
        int count;
        ar >> count;
        for (int i=0;i<count;i++)
        {
          SPELL_EFFECTS_DATA data;
          data.Serialize(ar,version);
          spellEffects.AddTail(data);
          WriteDebugString("Loading spell effect for %s: %s\n",
                           GetName(), data.affectedAttr);
        }
      }
    }
    if (version >= _VERSION_0696_)
      blockageData.Serialize(ar);

    UpdateSpellAbility();
    spells.list.SortSpells();
  }

  smallPic.Serialize(ar, version);
  if (!ar.IsStoring())
  {
    if (version <= _VERSION_0840_)
      smallPic.SetPre0840LoopForeverDefaults();
  }
  myItems.Serialize(ar,version);
  specAbs.Serialize(ar, version);
  // specAbQueue: no need to save this data
  char_asl.Serialize(ar);


  POSITION pos = spells.list.GetHeadPosition();
  while (pos!=NULL)
  {
    SPELL_DATA *pData = spellData.GetSpellData(spells.list.GetAt(pos).spell);
    if (pData!=NULL)
    {
      WriteDebugString("%s Spell \'%s\', db key %u, sb key %u\n", 
                       GetName(),
                       pData->Name,
                       pData->key,
                       spells.list.GetAt(pos).spell);
    }
    spells.list.GetNext(pos);
  }

  //m_Properties.Serialize(ar, version);
  
  // just to make sure we catch this for some older
  // character's that might come from saved games
  strengthKey     = abilityData.GetKeyByIndex(0);
  intelligenceKey = abilityData.GetKeyByIndex(1);
  wisdomKey       = abilityData.GetKeyByIndex(2);
  dexterityKey    = abilityData.GetKeyByIndex(3);
  constitutionKey = abilityData.GetKeyByIndex(4);
  charismaKey     = abilityData.GetKeyByIndex(5);

  // design settings that affect encumbrance might
  // have changed since character was last saved
  encumbrance = determineEffectiveEncumbrance();

  DisplayCurrSpellEffects("Serialize");
}

#ifdef UAFEDITOR
// event descriptive text for editor
//*****************************************************************************
//    NAME: CHARACTER::GetNPCEventText
//
// PURPOSE: 
//
//*****************************************************************************
CString CHARACTER::GetNPCEventText() 
{ 
  CString tmp;
  tmp.Format("NPC Event: %s", GetName());
  return tmp;
}
#endif

//*****************************************************************************
// NAME:    CHARACTER::IsReadyToTrain
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
BOOL CHARACTER::IsReadyToTrain()
{
  if (GetType() == MONSTER_TYPE) return FALSE;

  classType ctype = GetAdjClass();
  if (isFighter(ctype))
  {
    if (getFighterLevel(GetAdjFighterExp()) > FighterLevel)
      readyToTrain = TRUE;
  }
  if (isCleric(ctype))
  {
    if (getClericLevel(ClericExperience) > ClericLevel)
      readyToTrain = TRUE;
  }
  if (isMagicUser(ctype))
  {
    if (getMagicUserLevel(MUExperience) > MULevel)
      readyToTrain = TRUE;
  }
  if (isThief(ctype))
  {
    if (getThiefLevel(ThiefExperience) > ThiefLevel)
      readyToTrain = TRUE;
  }
  if (isRanger(ctype))
  {
    if (getRangerLevel(RangerExperience) > RangerLevel)
      readyToTrain = TRUE;
  }
  if (isPaladin(ctype))
  {
    if (getPaladinLevel(PaladinExperience) > PaladinLevel)
      readyToTrain = TRUE;
  }
  if (isDruid(ctype))
  {
    if (getDruidLevel(DruidExperience) > DruidLevel)
      readyToTrain = TRUE;
  }

  return GetAdjReadyToTrain();
}

//*****************************************************************************
// NAME: CHARACTER::GetHealingPointsNeeded
//
// PURPOSE:  
//*****************************************************************************
int CHARACTER::GetHealingPointsNeeded()
{
  return (GetMaxHitPoints() - GetAdjHitPoints());
}

//*****************************************************************************
// NAME: CHARACTER::CanBeModified
//
// PURPOSE:  
//*****************************************************************************
BOOL CHARACTER::CanBeModified()
{
  if (GetType() == MONSTER_TYPE) return FALSE;
  if (GetStatus()!=Okay) return FALSE;

  if (globalData.startExpType==START_EXP_VALUE)
  {
    int total = FighterExperience+
                ClericExperience+
                RangerExperience+
                PaladinExperience+
                MUExperience+
                ThiefExperience+
                DruidExperience;

    if (total > 0)
      return (total == globalData.startExp);
  }
  else
  {
    int total = FighterLevel+
                ClericLevel+
                RangerLevel+
                PaladinLevel+
                MULevel+
                ThiefLevel+
                DruidLevel;

    if (total > 0)
      return (total == globalData.startExp);
  }
  return TRUE;
}


//*****************************************************************************
// NAME: CHARACTER::GetClassLevel
//
// PURPOSE:  lookup current level based on class flag
//*****************************************************************************
// dont pass type charClass to this function!
BYTE CHARACTER::GetClassLevel(WORD ClassFlag)
{
  ASSERT( NumClassBits(ClassFlag) == 1 );

  if ((IsDualClass())&&(CanUsePrevClass()))
  {
    switch (ClassFlag)
    {
      case MagicUserFlag: if (prevMULevel>0) return prevMULevel; else return MULevel;
      case ClericFlag: if (prevClericLevel>0) return prevClericLevel; else return ClericLevel;
      case ThiefFlag: if (prevThiefLevel>0) return prevThiefLevel; else return ThiefLevel;
      case FighterFlag: if (prevFighterLevel>0) return prevFighterLevel; else return FighterLevel;
      case PaladinFlag: if (prevPaladinLevel>0) return prevPaladinLevel; else return PaladinLevel;
      case RangerFlag: if (prevRangerLevel>0) return prevRangerLevel; else return RangerLevel;
      case DruidFlag: if (prevDruidLevel>0) return prevDruidLevel; else return DruidLevel;
      default: ASSERT(FALSE); return 0;
    }
  }
  else
  {
    switch (ClassFlag)
    {
      case MagicUserFlag: return MULevel;
      case ClericFlag: return ClericLevel;
      case ThiefFlag: return ThiefLevel;
      case FighterFlag: return FighterLevel;
      case PaladinFlag: return PaladinLevel;
      case RangerFlag: return RangerLevel;
      case DruidFlag: return DruidLevel;
      default: ASSERT(FALSE); return 0;
    }
  }
}
// dont pass type charClass to this function!
//*****************************************************************************
//    NAME: CHARACTER::GetClassPrevLevel
//
// PURPOSE: 
//
//*****************************************************************************
BYTE CHARACTER::GetClassPrevLevel(WORD ClassFlag)
{
  ASSERT( NumClassBits(ClassFlag) == 1 );
  switch (ClassFlag)
  {
    case MagicUserFlag:      
      return prevMULevel;
    case ClericFlag:
      return prevClericLevel;
    case ThiefFlag:
      return prevThiefLevel;
    case FighterFlag:
      return prevFighterLevel;
    case PaladinFlag:
      return prevPaladinLevel;
    case RangerFlag:
      return prevRangerLevel;
    case DruidFlag:
      return prevDruidLevel;
    default:
      ASSERT(FALSE);
      return 0;
  }
}
// dont pass type charClass to this function!
//*****************************************************************************
//    NAME: CHARACTER::GetClassPreDrainLevel
//
// PURPOSE: 
//
//*****************************************************************************
BYTE CHARACTER::GetClassPreDrainLevel(WORD ClassFlag)
{
  ASSERT( NumClassBits(ClassFlag) == 1 );
  switch (ClassFlag)
  {
    case MagicUserFlag:      
      return pdMULevel;
    case ClericFlag:
      return pdClericLevel;
    case ThiefFlag:
      return pdThiefLevel;
    case FighterFlag:
      return pdFighterLevel;
    case PaladinFlag:
      return pdPaladinLevel;
    case RangerFlag:
      return pdRangerLevel;
    case DruidFlag:
      return pdDruidLevel;
    default:
      ASSERT(FALSE);
      return 0;
  }
}

// dont pass type charClass to this function!
//*****************************************************************************
//    NAME: CHARACTER::GetCurrentLevel
//
// PURPOSE: 
//
//*****************************************************************************
BYTE CHARACTER::GetCurrentLevel(WORD ClassFlag)
{
  //ASSERT(ClassFlag!=0);
  if (GetType() == MONSTER_TYPE) 
  {
    MONSTER_DATA *pData = monsterData.GetMonsterData(origIndex);
    if (pData == NULL) return 1;

    double hd = pData->Hit_Dice; 

    if (!pData->UseHitDice) // represents hit points instead of hit dice
      hd = (hd/FIGHTER_HIT_DIE)+1;

    if (hd < 1.0) hd = 1.0;    
    return hd;    
  }

  BYTE val=0;
  if (ClassFlag==0)
  {
    classType myClass = GetAdjClass();
    if (isMultiClass(myClass))
    {
      if (isFighter(myClass))
        val=max(val,GetClassLevel(FighterFlag));

      if (isCleric(myClass))
         val=max(val,GetClassLevel(ClericFlag));

      if (isRanger(myClass))
         val=max(val,GetClassLevel(RangerFlag));

      if (isPaladin(myClass))
         val=max(val,GetClassLevel(PaladinFlag));

      if (isMagicUser(myClass))
         val=max(val,GetClassLevel(MagicUserFlag));

      if (isThief(myClass))
         val=max(val,GetClassLevel(ThiefFlag));

      if (isDruid(myClass))
         val=max(val,GetClassLevel(DruidFlag));
    }
    else if (IsDualClass())
    {
      if (CanUsePrevClass())
      {
        val = GetPreviousClassLevel();
        BYTE val2 = GetClassLevel(CharClassToClassFlag(myClass));
        val = max(val, val2);
      }
      else
      {
        val = GetClassLevel(CharClassToClassFlag(myClass));
      }
    }
    else
    {
      val = GetClassLevel(CharClassToClassFlag(myClass));
    }
  }
  else
    val = GetClassLevel(ClassFlag);
  return val;
}
// dont pass type charClass to this function!
//*****************************************************************************
//    NAME: CHARACTER::GetPrevLevel
//
// PURPOSE: 
//
//*****************************************************************************
BYTE CHARACTER::GetPrevLevel(WORD ClassFlag)
{
  ASSERT(ClassFlag!=0);
  BYTE val=0;
  val = GetClassPrevLevel(ClassFlag);
  return val;
}
// dont pass type charClass to this function!
//*****************************************************************************
//    NAME: CHARACTER::GetPreDrainLevel
//
// PURPOSE: 
//
//*****************************************************************************
BYTE CHARACTER::GetPreDrainLevel(WORD ClassFlag)
{
  ASSERT(ClassFlag!=0);
  BYTE val=0;
  val = GetClassPreDrainLevel(ClassFlag);
  return val;
}

// dont pass type charClass to this function!
//*****************************************************************************
//    NAME: CHARACTER::SetCurrentLevel
//
// PURPOSE: 
//
//*****************************************************************************
void CHARACTER::SetCurrentLevel(WORD ClassFlag, int lvl)
{
  ASSERT( NumClassBits(ClassFlag) == 1 );

  switch (ClassFlag)
  {
    case MagicUserFlag:      
      MULevel=lvl; break;
    case ClericFlag:
      ClericLevel=lvl; break;
    case ThiefFlag:
      ThiefLevel=lvl; break;
    case FighterFlag:
      FighterLevel=lvl; break;
    case PaladinFlag:
      PaladinLevel=lvl;break;
    case RangerFlag:
      RangerLevel=lvl;break;
    case DruidFlag:
      DruidLevel=lvl; break;
    default:
      ASSERT(FALSE);
      break;
  }
}
// dont pass type charClass to this function!
//*****************************************************************************
//    NAME: CHARACTER::SetPrevLevel
//
// PURPOSE: 
//
//*****************************************************************************
void CHARACTER::SetPrevLevel(WORD ClassFlag, int lvl)
{
  ASSERT( NumClassBits(ClassFlag) == 1 );

  switch (ClassFlag)
  {
    case MagicUserFlag:      
      prevMULevel=lvl;
      break;
    case ClericFlag:
      prevClericLevel=lvl;
      break;
    case ThiefFlag:
      prevThiefLevel=lvl;
      break;
    case FighterFlag:
      prevFighterLevel=lvl;
      break;
    case PaladinFlag:
      prevPaladinLevel=lvl;
      break;
    case RangerFlag:
      prevRangerLevel=lvl;
      break;
    case DruidFlag:
      prevDruidLevel=lvl;
      break;
    default:
      ASSERT(FALSE);
      break;
  }
}
// dont pass type charClass to this function!
//*****************************************************************************
//    NAME: CHARACTER::SetPreDrainLevel
//
// PURPOSE: 
//
//*****************************************************************************
void CHARACTER::SetPreDrainLevel(WORD ClassFlag, int lvl)
{
  ASSERT( NumClassBits(ClassFlag) == 1 );
  switch (ClassFlag)
  {
    case MagicUserFlag:      
      pdMULevel=lvl;
      break;
    case ClericFlag:
      pdClericLevel=lvl;
      break;
    case ThiefFlag:
      pdThiefLevel=lvl;
      break;
    case FighterFlag:
      pdFighterLevel=lvl;
      break;
    case PaladinFlag:
      pdPaladinLevel=lvl;
      break;
    case RangerFlag:
      pdRangerLevel=lvl;
      break;
    case DruidFlag:
      pdDruidLevel=lvl;
      break;
    default:
      ASSERT(FALSE);
      break;
  }
}
// dont pass type charClass to this function!
//*****************************************************************************
//    NAME: CHARACTER::IncCurrentLevel
//
// PURPOSE: 
//
//*****************************************************************************
int CHARACTER::IncCurrentLevel(WORD ClassFlag, int inc)
{
  ASSERT( NumClassBits(ClassFlag) == 1 );
  switch (ClassFlag)
  {
    case MagicUserFlag:      
      MULevel+=inc;return MULevel;
    case ClericFlag:
      ClericLevel+=inc;return ClericLevel;
    case ThiefFlag:
      ThiefLevel+=inc;return ThiefLevel;
    case FighterFlag:
      FighterLevel+=inc;return FighterLevel;
    case PaladinFlag:
      PaladinLevel+=inc;return PaladinLevel;
    case RangerFlag:
      RangerLevel+=inc;return RangerLevel;
    case DruidFlag:
      DruidLevel+=inc;return DruidLevel;
    default:
      ASSERT(FALSE);
      return 0;
  }
}
// dont pass type charClass to this function!
//*****************************************************************************
//    NAME: CHARACTER::IncPrevLevel
//
// PURPOSE: 
//
//*****************************************************************************
int CHARACTER::IncPrevLevel(WORD ClassFlag, int inc)
{
  ASSERT( NumClassBits(ClassFlag) == 1 );
  switch (ClassFlag)
  {
    case MagicUserFlag:      
      prevMULevel+=inc;return prevMULevel;
    case ClericFlag:
      prevClericLevel+=inc;return prevClericLevel;
    case ThiefFlag:
      prevThiefLevel+=inc;return prevThiefLevel;
    case FighterFlag:
      prevFighterLevel+=inc;return prevFighterLevel;
    case PaladinFlag:
      prevPaladinLevel+=inc;return prevPaladinLevel;
    case RangerFlag:
      prevRangerLevel+=inc;return prevRangerLevel;
    case DruidFlag:
      prevDruidLevel+=inc;return prevDruidLevel;
    default:
      ASSERT(FALSE);
      return 0;
  }
}
// dont pass type charClass to this function!
//*****************************************************************************
//    NAME: CHARACTER::IncPreDrainLevel
//
// PURPOSE: 
//
//*****************************************************************************
int CHARACTER::IncPreDrainLevel(WORD ClassFlag, int inc)
{
  ASSERT( NumClassBits(ClassFlag) == 1 );
  switch (ClassFlag)
  {
    case MagicUserFlag:      
      pdMULevel+=inc;return pdMULevel;
    case ClericFlag:
      pdClericLevel+=inc;return pdClericLevel;
    case ThiefFlag:
      pdThiefLevel+=inc;return pdThiefLevel;
    case FighterFlag:
      pdFighterLevel+=inc;return pdFighterLevel;
    case PaladinFlag:
      pdPaladinLevel+=inc;return pdPaladinLevel;
    case RangerFlag:
      pdRangerLevel+=inc;return pdRangerLevel;
    case DruidFlag:
      pdDruidLevel+=inc;return pdDruidLevel;
    default:
      ASSERT(FALSE);
      return 0;
  }
}
// dont pass type charClass to this function!
//*****************************************************************************
//    NAME: CHARACTER::IsUsingClass
//
// PURPOSE: 
//
//*****************************************************************************
BOOL CHARACTER::IsUsingClass(WORD ClassFlag)
{
  ASSERT( NumClassBits(ClassFlag) == 1 );
  switch (ClassFlag)
  {
    case MagicUserFlag:      
      return isMagicUser(GetAdjClass());
    case ClericFlag:
      return isCleric(GetAdjClass());
    case ThiefFlag:
      return isThief(GetAdjClass());
    case FighterFlag:
      return isFighter(GetAdjClass());
    case PaladinFlag:
      return isPaladin(GetAdjClass());
    case RangerFlag:
      return isRanger(GetAdjClass());
    case DruidFlag:
      return isDruid(GetAdjClass());
    default:
      ASSERT(FALSE);
      return FALSE;
  }
}

//*****************************************************************************
// NAME: CHARACTER::GetBestMemorizedHealingSpell
//
// PURPOSE:  
//*****************************************************************************
DWORD CHARACTER::GetBestMemorizedHealingSpell(int &index)
{
  double total;
  double best = 0.0;
  int bestIndex = -1;

  index = NO_SPELL;
  SPELL_DATA *data;
  POSITION pos = spells.list.GetHeadPosition();

  while (pos != NULL)
  {
    if (spells.list.GetAt(pos).memorized)
    {
      if ((data = spellData.GetSpellData(spells.list.GetAt(pos).spell)) != NULL)
      {
        POSITION epos = data->m_EffectedAttributes.GetHeadPosition();
        while (epos != NULL)
        {
          SPELL_EFFECTS_DATA effect = data->m_EffectedAttributes.GetAt(epos);
          if (effect.affectedAttr=="$CHAR_HITPOINTS")
          {
            if (effect.changeData.MaxRoll(total))
            {
              if (total > best)
              {
                best = total;
                bestIndex = spells.list.GetAt(pos).spell;
              }
            }
          }
          data->m_EffectedAttributes.GetNext(epos);
        }
        //total = (data->Heal_Dice * data->Heal_Dice_Nbr) + data->Heal_Bonus;
        //total += (GetCurrentLevel(data->ClassMask) * data->Heal_Bonus_Level);
        //if (total > (DWORD)data->Heal_Max) total = data->Heal_Max;
      }
    }
    spells.list.GetNext(pos);
  }
  
  if (best > 0.0)
    index = bestIndex;

  return (DWORD)best;
}

//*****************************************************************************
// NAME:    CHARACTER::DetermineHitDiceBonus
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
int CHARACTER::DetermineHitDiceBonus()
{
   if (GetType() == MONSTER_TYPE) return 0;
   int bonus = determineHitDiceBonus(GetAdjCon(), isFighter(GetAdjClass()));
   return bonus;
}

//*****************************************************************************
// NAME:    CHARACTER::getCharExpWorth
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
int CHARACTER::getCharExpWorth()
{
  int totexp = 0;

  totexp += FighterExperience;
  totexp += PaladinExperience;
  totexp += RangerExperience;
  totexp += ClericExperience;
  totexp += DruidExperience;
  totexp += MUExperience;
  totexp += ThiefExperience;

  //totexp += (int)money.ConvertToDefault(money.Total(), money.GetBaseType());
  //totexp += money.TotalGemValue();
  //totexp += money.TotalJewelryValue();

  POSITION pos = myItems.GetHeadPosition();
  while (pos != NULL)
    totexp += itemData.GetItemExpWorth(myItems.GetNext(pos).item);
  
  double mod = GetAllExpMod();
  if (mod != 0.0) totexp += ((mod/100.0) * (double)totexp);
  if (totexp < 0) totexp=0;
  
  return totexp;
}


//*****************************************************************************
// NAME: CHARACTER::TrainCharacter
//
// PURPOSE:  
//*****************************************************************************
void CHARACTER::TrainCharacter()
{
  if (GetType() == MONSTER_TYPE) return;

  // need to train only eligible classes 
  // if a multi-class character
  //
  getNewCharLevel();

  DetermineCharHitDice();
  DetermineCharMaxHitPoints();
  hitPoints = maxHitPoints;  
  readyToTrain = FALSE;
  UpdateLevelBasedStats();
  getThiefSkillDexAdjustments();
  UpdateSpellAbility();
}

//*****************************************************************************
// NAME:    CHARACTER::generateNewCharacter
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
void CHARACTER::generateNewCharacter(DWORD StartExperience, int StartExpType)
{  
  strengthKey     = abilityData.GetKeyByIndex(0);
  intelligenceKey = abilityData.GetKeyByIndex(1);
  wisdomKey       = abilityData.GetKeyByIndex(2);
  dexterityKey    = abilityData.GetKeyByIndex(3);
  constitutionKey = abilityData.GetKeyByIndex(4);
  charismaKey     = abilityData.GetKeyByIndex(5);

  SetCharContext(GetContext());
  
  rollStats();

  FighterExperience = 0;
  ClericExperience = 0;
  RangerExperience =  0;
  PaladinExperience = 0;
  MUExperience = 0;
  ThiefExperience = 0;
  DruidExperience = 0;
  
  if ((GetType()==MONSTER_TYPE) || (StartExpType == START_EXP_VALUE))
  {
    ASSERT(StartExpType == START_EXP_VALUE);
    giveCharacterExperience(StartExperience, FALSE);
  }
  else // startexp is minimum level
  {
    ASSERT(GetType()!=MONSTER_TYPE);    
    
    int destlvl = StartExperience;
    if (numClasses(charClass) > 1)
      destlvl = destlvl / numClasses(charClass);

    if (isFighter(charClass))
      FighterExperience = GetMinFighterExpForLevel(destlvl);
    if (isCleric(charClass))
      ClericExperience = GetMinClericExpForLevel(destlvl);
    if (isRanger(charClass))
      RangerExperience = GetMinRangerExpForLevel(destlvl);
    if (isPaladin(charClass))
      PaladinExperience = GetMinPaladinExpForLevel(destlvl);
    if (isMagicUser(charClass))
      MUExperience = GetMinMagicUserExpForLevel(destlvl);
    if (isThief(charClass))
      ThiefExperience = GetMinThiefExpForLevel(destlvl);
    if (isDruid(charClass))
      DruidExperience = GetMinDruidExpForLevel(destlvl);
  }

  getNewCharLevel();

  DetermineCharMaxAge();  
  DetermineCharStartAge(); //(START_AGE + (rand() % (AGE_RANGE+1)));
  age = min( maxAge,  age);
  SetBirthday(rollDice(365,1));

  status = Okay;
  myItems.Clear();

  getNewCharStartingMoney();
  getNewCharStartingEquip();
  determineNbrAttacks(); 
  DetermineUnarmedHitDice();

  maxHitPoints = 0;
  DetermineNewCharMaxHitPoints();
  hitPoints = maxHitPoints;  
  UpdateStats(TRUE);

  ClearCharContext();
}

//*****************************************************************************
//    NAME: CHARACTER::UpdateStats
//
// PURPOSE: 
//
//*****************************************************************************
void CHARACTER::UpdateStats(BOOL IsNewChar)
{
  SetCharContext(GetContext());

  checkNewCharRaceScores(IsNewChar);
  checkNewCharClassScores();
  ItemMask = DetermineCharItemMask();
  UpdateSkillBasedStats();
  UpdateLevelBasedStats();

  ClearCharContext();
}

//*****************************************************************************
//    NAME: CHARACTER::UpdateLevelBasedStats
//
// PURPOSE: 
//
//*****************************************************************************
void CHARACTER::UpdateLevelBasedStats(void)
{
  SetCharContext(GetContext());

  getCharTHACO();
  getCharSavingThrows();
  getThiefSkills();
  getThiefSkillRaceAdjustments();
  DetermineCharHitDice();
  DetermineMaxCureDisease();

  ClearCharContext();
}

//*****************************************************************************
//    NAME: CHARACTER::UpdateSkillBasedStats
//
// PURPOSE: 
//
//*****************************************************************************
void CHARACTER::UpdateSkillBasedStats(void)
{
  SetCharContext(GetContext());

	maxEncumbrance = determineMaxEncumbrance();
	encumbrance = determineEffectiveEncumbrance();
	determineMaxMovement();
		
  int hb,db;
  BYTE od,omd,bl;
  determineStrengthProperties(GetStr(), GetStrMod(),
                              hb, db, 
                              od, omd, bl); 
  SetHitBonus(hb);
  SetDmgBonus(db);
  SetOpenDoors(od);
  SetOpenMagicDoors(omd);
  SetBendBarsLiftGates(bl);  
  getThiefSkillDexAdjustments();
  UpdateSpellAbility();
  getCharBaseAC();

  if (isPaladin(GetClass()))
  {
    DWORD src=0;
    DWORD result = GetAdjSpecAb(SA_ProtFromEvil, &src);
    if ((result==0) || (src != SPELL_EFFECTS_DATA::EFFECT_CHARSPECAB))
    {
      SetSpecAb(SA_ProtFromEvil, TRUE, SPELL_EFFECTS_DATA::EFFECT_CHARSPECAB);
    }
  }

  ClearCharContext();
}

//*****************************************************************************
// NAME:    CHARACTER::rollStats
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
void CHARACTER::rollStats()
{
  SetCharContext(GetContext());

	strength = rollSkillDie(strengthKey);
	intelligence = rollSkillDie(intelligenceKey);
	wisdom = rollSkillDie(wisdomKey);
	dexterity = rollSkillDie(dexterityKey);
	constitution = rollSkillDie(constitutionKey);
	charisma = rollSkillDie(charismaKey);

	if (strength==18)
  {
    if ((isFighter(GetClass())) || (isRanger(GetClass())) || (isPaladin(GetClass())))
		  strengthMod = (BYTE)rollDice(100,1);
  }
	else
		strengthMod = 0;

  ClearCharContext();
}


//*****************************************************************************
// NAME:    CHARACTER::DetermineCharItemMask
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
BYTE CHARACTER::DetermineCharItemMask()
{
  if (GetType() == MONSTER_TYPE)
    return monsterData.GetMonsterItemMask(origIndex);

  BYTE mask = 0;
  classType prevClass = BogusClass;

  if (CanUsePrevClass())
    prevClass = GetPreviousClass();
  classType ctype = GetAdjClass();

  if (isFighter(ctype))
     mask |= FighterFlag;

  if (isCleric(ctype))
     mask |= ClericFlag;

  if (isRanger(ctype))
     mask |= RangerFlag;

  if (isPaladin(ctype))
     mask |= PaladinFlag;

  if (isMagicUser(ctype))
     mask |= MagicUserFlag;

  if (isThief(ctype))
     mask |= ThiefFlag;

  if (isDruid(ctype))
     mask |= DruidFlag;

  if (isFighter(prevClass))
     mask |= FighterFlag;

  if (isCleric(prevClass))
     mask |= ClericFlag;

  if (isRanger(prevClass))
     mask |= RangerFlag;

  if (isPaladin(prevClass))
     mask |= PaladinFlag;

  if (isMagicUser(prevClass))
     mask |= MagicUserFlag;

  if (isThief(prevClass))
     mask |= ThiefFlag;

  if (isDruid(prevClass))
     mask |= DruidFlag;

  return mask;
}

//*****************************************************************************
// NAME:    CHARACTER::getCharBaseAC
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
void CHARACTER::getCharBaseAC()
{
  if (GetType() == MONSTER_TYPE)
  {
    AC = monsterData.GetMonsterAC(origIndex);
  }
  else
  {
    BYTE dex = GetAdjDex();
	  if (dex > 14)
		  AC = 10 - (dex - 14);
	  else
		  AC = 10;
    if (AC >  10) AC =  10;
    if (AC < -10) AC = -10;
  }
}


//*****************************************************************************
// NAME: CHARACTER::SetCharAC
//
// PURPOSE:  
//*****************************************************************************
int CHARACTER::SetCharAC()
{
  getCharBaseAC();
  AC += myItems.GetProtectModForRdyItems();
  if (AC >  10) AC =  10;
  if (AC < -10) AC = -10;
  return AC;
}

//*****************************************************************************
// NAME:    CHARACTER::DetermineUnarmedHitDice
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
void CHARACTER::DetermineUnarmedHitDice()
{
  if (GetType() == MONSTER_TYPE)
  {
    // Mostly a safety net. In combat the actual
    // attackData array should be used to determine
    // values for natural attacks (no weapons)
    monsterData.GetMonsterDamageDice(origIndex,
                                     0, // first attack
                                     unarmedNbrDieS,
                                     unarmedDieS,
                                     unarmedBonus);
	  unarmedDieL = unarmedDieS;
	  unarmedNbrDieL = unarmedNbrDieS;
  }
  else
  {
	  unarmedDieS = 2;
	  unarmedNbrDieS = 1;
	  unarmedBonus = 0;
	  unarmedDieL = 2;
	  unarmedNbrDieL = 1;
  }
}

//*****************************************************************************
// NAME:    CHARACTER::DetermineCharHitDice
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
void CHARACTER::DetermineCharHitDice()
{
  if (GetType() == MONSTER_TYPE)
  {
    MONSTER_DATA *pData = monsterData.GetMonsterData(origIndex);
    if (pData == NULL) return;    
    double hd = pData->Hit_Dice;     
    if (!pData->UseHitDice) // represents hit points instead of hit dice
      hd = (hd/FIGHTER_HIT_DIE)+1;    
    if (hd < 1.0) hd = 1.0;    
    SetNbrHD(hd);       
    return;
  }

  int dice;
  int nbrDice;
  int constant;
  int level;

  classType prevClass = BogusClass;
  classType ctype = GetAdjClass();

  if (CanUsePrevClass())
    prevClass = GetPreviousClass();

  int totalDice=0;
  
  if (isFighter(ctype))
  {
    for (level=1; level <= FighterLevel; level++)
    {
      getFighterHitDice(FighterLevel, dice,nbrDice,constant);
      totalDice += nbrDice;
    }
  }

  if (isCleric(ctype))
  {
    for (level=1; level <= ClericLevel; level++)
    {
      getFighterHitDice(ClericLevel, dice,nbrDice,constant);
      totalDice += nbrDice;
    }
  }

  if (isRanger(ctype))
  {
    for (level=1; level <= RangerLevel; level++)
    {
      getFighterHitDice(RangerLevel, dice,nbrDice,constant);
      totalDice += nbrDice;
    }
  }

  if (isPaladin(ctype))
  {
    for (level=1; level <= PaladinLevel; level++)
    {
      getFighterHitDice(PaladinLevel, dice,nbrDice,constant);
      totalDice += nbrDice;
    }
  }

  if (isMagicUser(ctype))
  {
    for (level=1; level <= MULevel; level++)
    {
      getFighterHitDice(MULevel, dice,nbrDice,constant);
      totalDice += nbrDice;
    }
  }

  if (isThief(ctype))
  {
    for (level=1; level <= ThiefLevel; level++)
    {
      getFighterHitDice(ThiefLevel, dice,nbrDice,constant);
      totalDice += nbrDice;
    }
  }

  if (isDruid(ctype))
  {
    for (level=1; level <= DruidLevel; level++)
    {
      getFighterHitDice(DruidLevel, dice,nbrDice,constant);
      totalDice += nbrDice;
    }
  }

  if (isFighter(prevClass))
  {

    for (level=1; level <= FighterLevel; level++)
    {
      getFighterHitDice(FighterLevel, dice,nbrDice,constant);
      totalDice += nbrDice;
    }
  }

  if (isCleric(prevClass))
  {
    for (level=1; level <= ClericLevel; level++)
    {
      getFighterHitDice(ClericLevel, dice,nbrDice,constant);
      totalDice += nbrDice;
    }
  }

  if (isRanger(prevClass))
  {
    for (level=1; level <= RangerLevel; level++)
    {
      getFighterHitDice(RangerLevel, dice,nbrDice,constant);
      totalDice += nbrDice;
    }
  }

  if (isPaladin(prevClass))
  {
    for (level=1; level <= PaladinLevel; level++)
    {
      getFighterHitDice(PaladinLevel, dice,nbrDice,constant);
      totalDice += nbrDice;
    }
  }

  if (isMagicUser(prevClass))
  {
    for (level=1; level <= MULevel; level++)
    {
      getFighterHitDice(MULevel, dice,nbrDice,constant);
      totalDice += nbrDice;
    }
  }

  if (isThief(prevClass))
  {
    for (level=1; level <= ThiefLevel; level++)
    {
      getFighterHitDice(ThiefLevel, dice,nbrDice,constant);
      totalDice += nbrDice;
    }
  }

  if (isDruid(prevClass))
  {
    for (level=1; level <= DruidLevel; level++)
    {
      getFighterHitDice(DruidLevel, dice,nbrDice,constant);
      totalDice += nbrDice;
    }
  }

  int totalClasses = numClasses(ctype) + numClasses(prevClass);
  dice = totalDice / totalClasses;

  SetNbrHD(dice);
}

//*****************************************************************************
// NAME:    CHARACTER::DetermineCharMaxHitPoints
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
void CHARACTER::DetermineCharMaxHitPoints()
{
  if (GetType() == MONSTER_TYPE)
  {
    MONSTER_DATA *pData = monsterData.GetMonsterData(origIndex);
    if (pData == NULL) return;    
    double hd = pData->Hit_Dice;     
    int bonus = pData->Hit_Dice_Bonus;
    if (!pData->UseHitDice) // represents hit points instead of hit dice
    {
      maxHitPoints = hd;
    }
    else
    {    
      if (hd < 1.0)
        maxHitPoints = rollDice(int(8.0*hd), 1, bonus);
      else 
        maxHitPoints = rollDice(8, (int)hd, bonus);
    }
    maxHitPoints = max(1.0, maxHitPoints);
    return;
  }

  int dice;
  int nbrDice;
  int constant;

  classType prevClass = BogusClass;
  classType ctype = GetAdjClass();

  if (CanUsePrevClass())
    prevClass = GetPreviousClass();

  int totalHP = 0;
  int totalClasses = numClasses(ctype) + numClasses(prevClass);

  if (isFighter(ctype))
  {
     getFighterHitDice(FighterLevel, dice,nbrDice,constant);
     totalHP += 
        rollDice(dice, nbrDice, DetermineHitDiceBonus()) + constant;
  }

  if (isCleric(ctype))
  {
     getClericHitDice(ClericLevel, dice,nbrDice,constant);
     totalHP += 
        rollDice(dice, nbrDice, DetermineHitDiceBonus()) + constant;
  }

  if (isRanger(ctype))
  {
     getRangerHitDice(RangerLevel, dice,nbrDice,constant);
     totalHP += 
        rollDice(dice, nbrDice, DetermineHitDiceBonus()) + constant;
  }

  if (isPaladin(ctype))
  {
     getPaladinHitDice(PaladinLevel, dice,nbrDice,constant);
     totalHP += 
        rollDice(dice, nbrDice, DetermineHitDiceBonus()) + constant;
  }

  if (isMagicUser(ctype))
  {
     getMagicUserHitDice(MULevel, dice,nbrDice,constant);
     totalHP += 
        rollDice(dice, nbrDice, DetermineHitDiceBonus()) + constant;
  }

  if (isThief(ctype))
  {
     getThiefHitDice(ThiefLevel, dice,nbrDice,constant);
     totalHP += 
        rollDice(dice, nbrDice, DetermineHitDiceBonus()) + constant;
  }

  if (isDruid(ctype))
  {
     getDruidHitDice(DruidLevel, dice,nbrDice,constant);
     totalHP += 
        rollDice(dice, nbrDice, DetermineHitDiceBonus()) + constant;
  }

  if (isFighter(prevClass))
  {
     getFighterHitDice(FighterLevel, dice,nbrDice,constant);
     totalHP += 
        rollDice(dice, nbrDice, DetermineHitDiceBonus()) + constant;
  }

  if (isCleric(prevClass))
  {
     getClericHitDice(ClericLevel, dice,nbrDice,constant);
     totalHP += 
        rollDice(dice, nbrDice, DetermineHitDiceBonus()) + constant;
  }

  if (isRanger(prevClass))
  {
     getRangerHitDice(RangerLevel, dice,nbrDice,constant);
     totalHP += 
        rollDice(dice, nbrDice, DetermineHitDiceBonus()) + constant;
  }

  if (isPaladin(prevClass))
  {
     getPaladinHitDice(PaladinLevel, dice,nbrDice,constant);
     totalHP += 
        rollDice(dice, nbrDice, DetermineHitDiceBonus()) + constant;
  }

  if (isMagicUser(prevClass))
  {
     getMagicUserHitDice(MULevel, dice,nbrDice,constant);
     totalHP += 
        rollDice(dice, nbrDice, DetermineHitDiceBonus()) + constant;
  }

  if (isThief(prevClass))
  {
     getThiefHitDice(ThiefLevel, dice,nbrDice,constant);
     totalHP += 
        rollDice(dice, nbrDice, DetermineHitDiceBonus()) + constant;
  }

  if (isDruid(prevClass))
  {
     getDruidHitDice(DruidLevel, dice,nbrDice,constant);
     totalHP += 
        rollDice(dice, nbrDice, DetermineHitDiceBonus()) + constant;
  }

  maxHitPoints += (totalHP / totalClasses);
  maxHitPoints = max( 1, maxHitPoints );
}

//*****************************************************************************
//    NAME: CHARACTER::DetermineNewCharMaxHitPoints
//
// PURPOSE: 
//
//*****************************************************************************
void CHARACTER::DetermineNewCharMaxHitPoints()
{
  if (GetType() == MONSTER_TYPE)
  {
    MONSTER_DATA *pData = monsterData.GetMonsterData(origIndex);
    if (pData == NULL) return;    
    double hd = pData->Hit_Dice;     
    int bonus = pData->Hit_Dice_Bonus;
    if (!pData->UseHitDice) // represents hit points instead of hit dice
    {
      maxHitPoints = hd;
    }
    else
    {    
      if (hd < 1.0)
        maxHitPoints = rollDice(int(8.0*hd), 1, bonus);
      else 
        maxHitPoints = rollDice(8, (int)hd, bonus);
    }
    maxHitPoints = max(1.0, maxHitPoints);
    return;
  }

  int dice;
  int nbrDice;
  int constant;

  int totalHP = 0;
  classType ctype = GetAdjClass();
  int totalClasses = numClasses(ctype);

  if (isFighter(ctype))
  {
    for (int j=1;j<=FighterLevel;j++)
    {
      getFighterHitDice(j, dice,nbrDice,constant);
      totalHP += 
        rollDice(dice, nbrDice, DetermineHitDiceBonus()) + constant;
    }
  }

  if (isCleric(ctype))
  {
    for (int j=1;j<=ClericLevel;j++)
    {
      getClericHitDice(j, dice,nbrDice,constant);
      totalHP += 
        rollDice(dice, nbrDice, DetermineHitDiceBonus()) + constant;
    }
  }

  if (isRanger(ctype))
  {
    for (int j=1;j<=RangerLevel;j++)
    {
      getRangerHitDice(j, dice,nbrDice,constant);
      totalHP += 
        rollDice(dice, nbrDice, DetermineHitDiceBonus()) + constant;
    }
  }

  if (isPaladin(ctype))
  {
    for (int j=1;j<=PaladinLevel;j++)
    {
      getPaladinHitDice(j, dice,nbrDice,constant);
      totalHP += 
        rollDice(dice, nbrDice, DetermineHitDiceBonus()) + constant;
    }
  }

  if (isMagicUser(ctype))
  {
    for (int j=1;j<=MULevel;j++)
    {
      getMagicUserHitDice(j, dice,nbrDice,constant);
      totalHP += 
        rollDice(dice, nbrDice, DetermineHitDiceBonus()) + constant;
    }
  }

  if (isThief(ctype))
  {
    for (int j=1;j<=ThiefLevel;j++)
    {
      getThiefHitDice(j, dice,nbrDice,constant);
      totalHP += 
        rollDice(dice, nbrDice, DetermineHitDiceBonus()) + constant;
    }
  }

  if (isDruid(ctype))
  {
    for (int j=1;j<=DruidLevel;j++)
    {
      getDruidHitDice(j, dice,nbrDice,constant);
      totalHP += 
        rollDice(dice, nbrDice, DetermineHitDiceBonus()) + constant;
    }
  }

  maxHitPoints += (totalHP / totalClasses);
  maxHitPoints = max( 1, maxHitPoints );
}

//*****************************************************************************
// NAME:    CHARACTER::getNewCharLevel
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
void CHARACTER::getNewCharLevel()
{
  if (GetType() == MONSTER_TYPE)
  {
    MONSTER_DATA *pData = monsterData.GetMonsterData(origIndex);
    if (pData == NULL) return;    
    double hd = pData->Hit_Dice;     
    int bonus = pData->Hit_Dice_Bonus;
    if (!pData->UseHitDice) // represents hit points instead of hit dice
      hd = (hd/FIGHTER_HIT_DIE)+1;

    FighterLevel = (BYTE)hd;
    FighterLevel = max(FighterLevel, 1);  
    int hdBonus = bonus;
    if (hdBonus > 0)
    {
     int bonus = (hdBonus / 4) + 1;
     if ((hdBonus % 4) == 0)
       bonus--;
     FighterLevel += bonus;
    }
  }

  if (isFighter(charClass))
    FighterLevel = getFighterLevel(GetFighterExp());

  if (isCleric(charClass))
     ClericLevel = getClericLevel(GetClericExp());

  if (isRanger(charClass))
     RangerLevel = getRangerLevel(GetRangerExp());

  if (isPaladin(charClass))
     PaladinLevel = getPaladinLevel(GetPaladinExp());

  if (isMagicUser(charClass))
     MULevel = getMagicUserLevel(GetMagicUserExp());

  if (isThief(charClass))
     ThiefLevel = getThiefLevel(GetThiefExp());

  if (isDruid(charClass))
     DruidLevel = getDruidLevel(GetDruidExp());
}

//*****************************************************************************
// NAME:    CHARACTER::checkNewCharClassScores
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
void CHARACTER::checkNewCharClassScores()
{
  if (GetType() == MONSTER_TYPE) return;

  CHARSKILLDATA data;
  data.str = strength;
  data.intel = intelligence;
  data.wis = wisdom;
  data.dex = dexterity;
  data.con = constitution;
  data.cha = charisma;
  data.strKey = strengthKey;
  data.intelKey = intelligenceKey;
  data.wisKey = wisdomKey;
  data.dexKey = dexterityKey;
  data.conKey = constitutionKey;
  data.chaKey = charismaKey;

  CheckNewCharClassScores(GetClass(), data);

  strength = data.str;
  intelligence = data.intel;
  wisdom = data.wis;
  dexterity = data.dex;
  constitution = data.con;
  charisma = data.cha;
}

//*****************************************************************************
// NAME:    CHARACTER::checkNewCharRaceScores
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
void CHARACTER::checkNewCharRaceScores(BOOL ApplyModifiers)
{
  if (GetType() == MONSTER_TYPE) return;

  CHARSKILLDATA data;
  data.str = strength;
  data.intel = intelligence;
  data.wis = wisdom;
  data.dex = dexterity;
  data.con = constitution;
  data.cha = charisma;
  data.strKey = strengthKey;
  data.intelKey = intelligenceKey;
  data.wisKey = wisdomKey;
  data.dexKey = dexterityKey;
  data.conKey = constitutionKey;
  data.chaKey = charismaKey;

  //if (ApplyModifiers)
  //  ApplyNewCharRaceScoreModifiers(race(), data);
  CheckNewCharRaceScores(race(), data);

  strength = data.str;
  intelligence = data.intel;
  wisdom = data.wis;
  dexterity = data.dex;
  constitution = data.con;
  charisma = data.cha;
}

//*****************************************************************************
//    NAME: CHARACTER::DetermineCharStartAge
//
// PURPOSE: 
//
//*****************************************************************************
void CHARACTER::DetermineCharStartAge()
{
  if (GetType() == MONSTER_TYPE)
    age = START_AGE;
  else
    age = determineCharStartAge(race());
}

//*****************************************************************************
// NAME:    CHARACTER::DetermineCharMaxAge
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
void CHARACTER::DetermineCharMaxAge()
{
  if (GetType() == MONSTER_TYPE)
  {
    maxAge = 1500;
    return;
  }
  maxAge = determineCharMaxAge(race());
}

//*****************************************************************************
// NAME:    CHARACTER::getNewCharStartingMoney
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
void CHARACTER::getNewCharStartingMoney()
{
  if (GetType() == MONSTER_TYPE) return;

  money.Clear();
  money.Add(money.GetDefaultType(), GetDesignStartPlatinum()); 
  for (int g=0;g<GetDesignStartGems();g++) money.AddGem();
  for (int j=0;j<GetDesignStartJewelry();j++) money.AddJewelry();
}

//*****************************************************************************
//    NAME: CHARACTER::getNewCharStartingEquip
//
// PURPOSE: 
//
//*****************************************************************************
void CHARACTER::getNewCharStartingEquip()
{
  if (GetType() == MONSTER_TYPE) return;
  myItems.Clear();

  POSITION pos = globalData.startEquip.GetHeadPosition();
  while (pos != NULL)
    myItems.AddItem(globalData.startEquip.GetNext(pos));    

  if (isFighter(charClass))
  {
    pos = globalData.FighterEquip.GetHeadPosition();
    while (pos != NULL)
      myItems.AddItem(globalData.FighterEquip.GetNext(pos));    
  }
  if (isCleric(charClass))
  {
    pos = globalData.ClericEquip.GetHeadPosition();
    while (pos != NULL)
      myItems.AddItem(globalData.ClericEquip.GetNext(pos));    
  }
  if (isRanger(charClass))
  {
    pos = globalData.RangerEquip.GetHeadPosition();
    while (pos != NULL)
      myItems.AddItem(globalData.RangerEquip.GetNext(pos));    
  }
  if (isPaladin(charClass))
  {
    pos = globalData.PaladinEquip.GetHeadPosition();
    while (pos != NULL)
      myItems.AddItem(globalData.PaladinEquip.GetNext(pos));    
  }
  if (isMagicUser(charClass))
  {
    pos = globalData.MagicUserEquip.GetHeadPosition();
    while (pos != NULL)
      myItems.AddItem(globalData.MagicUserEquip.GetNext(pos));    
  }
  if (isThief(charClass))
  {
    pos = globalData.ThiefEquip.GetHeadPosition();
    while (pos != NULL)
      myItems.AddItem(globalData.ThiefEquip.GetNext(pos));    
  }
  if (isDruid(charClass))
  {
    pos = globalData.DruidEquip.GetHeadPosition();
    while (pos != NULL)
      myItems.AddItem(globalData.DruidEquip.GetNext(pos));    
  }
}

//*****************************************************************************
// NAME:    CHARACTER::determineMaxMovement
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
int CHARACTER::determineMaxMovement()
{
  if (GetType() == MONSTER_TYPE)
  {
    int val = monsterData.GetMonsterMovement(origIndex);
    SetMaxMovement(val);
    return GetAdjMaxMovement();
  }

	int ewt = determineEffectiveEncumbrance();	
  int normalEncumbrance = determineNormalEncumbrance();
  int val = 12;

  if (ewt <= normalEncumbrance) val = 12;
  else if (ewt <= (normalEncumbrance*2)) val = 9;
  else if (ewt <= (normalEncumbrance*3)) val = 6;
  else if (ewt <= (normalEncumbrance*4)) val = 3;
  else val = 1;

  SetMaxMovement(val);
  return GetAdjMaxMovement();
}

//*****************************************************************************
// NAME:    CHARACTER::determineMaxEncumbrance
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
int CHARACTER::determineMaxEncumbrance()
{
  if (GetType() == MONSTER_TYPE)
    return 100000; // monsters are not encumbered in combat?

  return (determineNormalEncumbrance()*5);
}

//*****************************************************************************
// NAME:    CHARACTER::getCharSavingThrows
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
void CHARACTER::getCharSavingThrows()
{
   int level=1;
   int ppd=20;int rsw=20; int pp=20; int br=20; int sp=20;

  classType prevClass = BogusClass;
  classType ctype = GetAdjClass();

  if (CanUsePrevClass())
    prevClass = GetPreviousClass();

   if (   (isFighter(ctype))
       || (isRanger(ctype))
       || (isPaladin(ctype)))
   {
      level = max(FighterLevel, RangerLevel);
      level = max(level, PaladinLevel);

		if (level >= 17)
		{
      ppd = min(ppd, 3);
      rsw = min(rsw, 5);
      pp  = min(pp,  4);
      br  = min(br,  4);
      sp  = min(sp,  6);
		}
		else if (level >= 15)
		{
      ppd = min(ppd, 4);
      rsw = min(rsw, 6);
      pp  = min(pp,  5);
      br  = min(br,  4);
      sp  = min(sp,  7);
		}
		else if (level >= 13)
		{
      ppd = min(ppd, 5);
      rsw = min(rsw, 7);
      pp  = min(pp,  6);
      br  = min(br,  5);
      sp  = min(sp,  8);
		}
		else if (level >= 11)
		{
      ppd = min(ppd, 7);
      rsw = min(rsw, 9);
      pp  = min(pp,  8);
      br  = min(br,  8);
      sp  = min(sp,  10);
		}
		else if (level >= 9)
		{
      ppd = min(ppd, 8);
      rsw = min(rsw, 10);
      pp  = min(pp,  9);
      br  = min(br,  9);
      sp  = min(sp,  11);
		}
		else if (level >= 7)
		{
      ppd = min(ppd, 10);
      rsw = min(rsw, 12);
      pp  = min(pp,  11);
      br  = min(br,  12);
      sp  = min(sp,  13);
		}
		else if (level >= 5)
		{
      ppd = min(ppd, 11);
      rsw = min(rsw, 13);
      pp  = min(pp,  12);
      br  = min(br,  13);
      sp  = min(sp,  14);
		}
		else if (level >= 3)
		{
      ppd = min(ppd, 13);
      rsw = min(rsw, 15);
      pp  = min(pp,  14);
      br  = min(br,  16);
      sp  = min(sp,  16);
		}
		else if (level >= 1)
		{
      ppd = min(ppd, 14);
      rsw = min(rsw, 16);
      pp  = min(pp,  15);
      br  = min(br,  17);
      sp  = min(sp,  17);
		}
		else 
    {
      ppd = min(ppd, 16);
      rsw = min(rsw, 18);
      pp  = min(pp,  17);
      br  = min(br,  20);
      sp  = min(sp,  19);
		}
   }

   if (   (isCleric(ctype))
       || (isDruid(ctype)))
   {
      level = max(ClericLevel, DruidLevel);

		if (level >= 19)
		{
         ppd = min(ppd, 2);
         rsw = min(rsw, 6);
         pp  = min(pp,  5);
         br  = min(br,  8);
         sp  = min(sp,  7);
		}
		else if (level >= 16)
		{
         ppd = min(ppd, 4);
         rsw = min(rsw, 8);
         pp  = min(pp,  7);
         br  = min(br, 10);
         sp  = min(sp,  9);
		}
		else if (level >= 13)
		{
         ppd = min(ppd, 5);
         rsw = min(rsw, 9);
         pp  = min(pp,  8);
         br  = min(br, 11);
         sp  = min(sp, 10);
		}
		else if (level >= 10)
		{
         ppd = min(ppd, 6);
         rsw = min(rsw,10);
         pp  = min(pp,  9);
         br  = min(br, 12);
         sp  = min(sp, 11);
		}
		else if (level >= 7)
		{
         ppd = min(ppd, 7);
         rsw = min(rsw,11);
         pp  = min(pp, 10);
         br  = min(br, 13);
         sp  = min(sp, 12);
		}
		else if (level >= 4)
		{
         ppd = min(ppd, 9);
         rsw = min(rsw,13);
         pp  = min(pp, 12);
         br  = min(br, 15);
         sp  = min(sp, 14);
		}
		else 
      {
         ppd = min(ppd,10);
         rsw = min(rsw,14);
         pp  = min(pp, 13);
         br  = min(br, 16);
         sp  = min(sp, 15);
		}
   }

   if (isMagicUser(ctype))
   {
		if (MULevel >= 21)
		{
         ppd = min(ppd, 8);
         rsw = min(rsw, 3);
         pp  = min(pp,  5);
         br  = min(br,  7);
         sp  = min(sp,  4);
		}
		else if (MULevel >= 16)
		{
         ppd = min(ppd,10);
         rsw = min(rsw, 5);
         pp  = min(pp,  7);
         br  = min(br,  9);
         sp  = min(sp,  6);
		}
		else if (MULevel >= 11)
		{
         ppd = min(ppd,11);
         rsw = min(rsw, 7);
         pp  = min(pp,  9);
         br  = min(br, 11);
         sp  = min(sp,  8);
		}
		else if (MULevel >= 6)
		{
         ppd = min(ppd,13);
         rsw = min(rsw, 9);
         pp  = min(pp, 11);
         br  = min(br, 13);
         sp  = min(sp, 10);
		}
		else 
      {
         ppd = min(ppd, 14);
         rsw = min(rsw, 11);
         pp  = min(pp,  13);
         br  = min(br,  15);
         sp  = min(sp,  12);
		}
   }

   if (isThief(ctype))
   {
		if (ThiefLevel >= 21)
		{
         ppd = min(ppd, 8);
         rsw = min(rsw, 7);
         pp  = min(pp,  4);
         br  = min(br, 11);
         sp  = min(sp,  5);
		}
		else if (ThiefLevel >= 17)
		{
         ppd = min(ppd, 9);
         rsw = min(rsw, 6);
         pp  = min(pp,  8);
         br  = min(br, 12);
         sp  = min(sp,  7);
		}
		else if (ThiefLevel >= 13)
		{
         ppd = min(ppd,10);
         rsw = min(rsw, 8);
         pp  = min(pp,  9);
         br  = min(br, 13);
         sp  = min(sp,  9);
		}
		else if (ThiefLevel >= 9)
		{
         ppd = min(ppd,11);
         rsw = min(rsw,10);
         pp  = min(pp, 10);
         br  = min(br, 14);
         sp  = min(sp, 11);
		}
		else if (ThiefLevel >= 5)
		{
         ppd = min(ppd,12);
         rsw = min(rsw,12);
         pp  = min(pp, 11);
         br  = min(br, 15);
         sp  = min(sp, 13);
		}
		else 
      {
         ppd = min(ppd,13);
         rsw = min(rsw,14);
         pp  = min(pp, 12);
         br  = min(br, 16);
         sp  = min(sp, 15);
		}
   }

   if (   (isFighter(prevClass))
       || (isRanger(prevClass))
       || (isPaladin(prevClass)))
   {
      level = max(prevFighterLevel, prevRangerLevel);
      level = max(level, prevPaladinLevel);

		if (level >= 17)
		{
      ppd = min(ppd, 3);
      rsw = min(rsw, 5);
      pp  = min(pp,  4);
      br  = min(br,  4);
      sp  = min(sp,  6);
		}
		else if (level >= 15)
		{
      ppd = min(ppd, 4);
      rsw = min(rsw, 6);
      pp  = min(pp,  5);
      br  = min(br,  4);
      sp  = min(sp,  7);
		}
		else if (level >= 13)
		{
      ppd = min(ppd, 5);
      rsw = min(rsw, 7);
      pp  = min(pp,  6);
      br  = min(br,  5);
      sp  = min(sp,  8);
		}
		else if (level >= 11)
		{
      ppd = min(ppd, 7);
      rsw = min(rsw, 9);
      pp  = min(pp,  8);
      br  = min(br,  8);
      sp  = min(sp,  10);
		}
		else if (level >= 9)
		{
      ppd = min(ppd, 8);
      rsw = min(rsw, 10);
      pp  = min(pp,  9);
      br  = min(br,  9);
      sp  = min(sp,  11);
		}
		else if (level >= 7)
		{
      ppd = min(ppd, 10);
      rsw = min(rsw, 12);
      pp  = min(pp,  11);
      br  = min(br,  12);
      sp  = min(sp,  13);
		}
		else if (level >= 5)
		{
      ppd = min(ppd, 11);
      rsw = min(rsw, 13);
      pp  = min(pp,  12);
      br  = min(br,  13);
      sp  = min(sp,  14);
		}
		else if (level >= 3)
		{
      ppd = min(ppd, 13);
      rsw = min(rsw, 15);
      pp  = min(pp,  14);
      br  = min(br,  16);
      sp  = min(sp,  16);
		}
		else if (level >= 1)
		{
      ppd = min(ppd, 14);
      rsw = min(rsw, 16);
      pp  = min(pp,  15);
      br  = min(br,  17);
      sp  = min(sp,  17);
		}
		else 
    {
      ppd = min(ppd, 16);
      rsw = min(rsw, 18);
      pp  = min(pp,  17);
      br  = min(br,  20);
      sp  = min(sp,  19);
		}
   }

   if (   (isCleric(prevClass))
       || (isDruid(prevClass)))
   {
      level = max(prevClericLevel, prevDruidLevel);

		if (level >= 19)
		{
         ppd = min(ppd, 2);
         rsw = min(rsw, 6);
         pp  = min(pp,  5);
         br  = min(br,  8);
         sp  = min(sp,  7);
		}
		else if (level >= 16)
		{
         ppd = min(ppd, 4);
         rsw = min(rsw, 8);
         pp  = min(pp,  7);
         br  = min(br, 10);
         sp  = min(sp,  9);
		}
		else if (level >= 13)
		{
         ppd = min(ppd, 5);
         rsw = min(rsw, 9);
         pp  = min(pp,  8);
         br  = min(br, 11);
         sp  = min(sp, 10);
		}
		else if (level >= 10)
		{
         ppd = min(ppd, 6);
         rsw = min(rsw,10);
         pp  = min(pp,  9);
         br  = min(br, 12);
         sp  = min(sp, 11);
		}
		else if (level >= 7)
		{
         ppd = min(ppd, 7);
         rsw = min(rsw,11);
         pp  = min(pp, 10);
         br  = min(br, 13);
         sp  = min(sp, 12);
		}
		else if (level >= 4)
		{
         ppd = min(ppd, 9);
         rsw = min(rsw,13);
         pp  = min(pp, 12);
         br  = min(br, 15);
         sp  = min(sp, 14);
		}
		else 
      {
         ppd = min(ppd,10);
         rsw = min(rsw,14);
         pp  = min(pp, 13);
         br  = min(br, 16);
         sp  = min(sp, 15);
		}
   }

   if (isMagicUser(prevClass))
   {
		if (prevMULevel >= 21)
		{
         ppd = min(ppd, 8);
         rsw = min(rsw, 3);
         pp  = min(pp,  5);
         br  = min(br,  7);
         sp  = min(sp,  4);
		}
		else if (prevMULevel >= 16)
		{
         ppd = min(ppd,10);
         rsw = min(rsw, 5);
         pp  = min(pp,  7);
         br  = min(br,  9);
         sp  = min(sp,  6);
		}
		else if (prevMULevel >= 11)
		{
         ppd = min(ppd,11);
         rsw = min(rsw, 7);
         pp  = min(pp,  9);
         br  = min(br, 11);
         sp  = min(sp,  8);
		}
		else if (prevMULevel >= 6)
		{
         ppd = min(ppd,13);
         rsw = min(rsw, 9);
         pp  = min(pp, 11);
         br  = min(br, 13);
         sp  = min(sp, 10);
		}
		else 
    {
         ppd = min(ppd, 14);
         rsw = min(rsw, 11);
         pp  = min(pp,  13);
         br  = min(br,  15);
         sp  = min(sp,  12);
		}
   }

   if (isThief(prevClass))
   {
		if (prevThiefLevel >= 21)
		{
         ppd = min(ppd, 8);
         rsw = min(rsw, 7);
         pp  = min(pp,  4);
         br  = min(br, 11);
         sp  = min(sp,  5);
		}
		else if (prevThiefLevel >= 17)
		{
         ppd = min(ppd, 9);
         rsw = min(rsw, 6);
         pp  = min(pp,  8);
         br  = min(br, 12);
         sp  = min(sp,  7);
		}
		else if (prevThiefLevel >= 13)
		{
         ppd = min(ppd,10);
         rsw = min(rsw, 8);
         pp  = min(pp,  9);
         br  = min(br, 13);
         sp  = min(sp,  9);
		}
		else if (prevThiefLevel >= 9)
		{
         ppd = min(ppd,11);
         rsw = min(rsw,10);
         pp  = min(pp, 10);
         br  = min(br, 14);
         sp  = min(sp, 11);
		}
		else if (prevThiefLevel >= 5)
		{
         ppd = min(ppd,12);
         rsw = min(rsw,12);
         pp  = min(pp, 11);
         br  = min(br, 15);
         sp  = min(sp, 13);
		}
		else 
    {
         ppd = min(ppd,13);
         rsw = min(rsw,14);
         pp  = min(pp, 12);
         br  = min(br, 16);
         sp  = min(sp, 15);
		}
   }

	SetSaveVsPPDM(ppd);
	SetSaveVsRSW(rsw);
	SetSaveVsPP(pp);
	SetSaveVsBr(br);
	SetSaveVsSp(sp);
}

//*****************************************************************************
// NAME:    CHARACTER::giveCharacterExperience
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
void CHARACTER::giveCharacterExperience(int exppts, BOOL UseLimits)
{
  //if (GetType() == MONSTER_TYPE) return;
  if (exppts == 0) return;

   classType ctype = GetAdjClass();
   if (isMultiClass(ctype))
   {
      if (exppts > 0)
      {
         exppts /= numClasses(ctype);
         if (exppts == 0) exppts = 1;
      }
      else
      {
         exppts /= numClasses(ctype);
         if (exppts == 0) exppts = -1;
      }
   }

   // check to make sure char doesn't 
   // jump more than one level of experience
   //
   if (isFighter(ctype))
   {
      IncCurrExp(FighterFlag, exppts);
      if (UseLimits)
      {
        int cexp = GetFighterExp();
        int mexp = GetMaxFighterExpForLevel(GetCurrentLevel(FighterFlag));
        int minxp = GetMinFighterExpForLevel(GetCurrentLevel(FighterFlag));
        cexp = max(cexp, minxp);
        SetCurrExp(FighterFlag,  min(cexp,mexp) );
      }
   }

   if (isPaladin(ctype))
   {
      IncCurrExp(PaladinFlag, exppts);
      if (UseLimits)
      {
        int cexp = GetPaladinExp();
        int mexp = GetMaxPaladinExpForLevel(GetCurrentLevel(PaladinFlag));
        int minxp = GetMinPaladinExpForLevel(GetCurrentLevel(PaladinFlag));
        cexp = max(cexp, minxp);
        SetCurrExp(PaladinFlag,  min(cexp,mexp) );
      }
   }

   if (isRanger(ctype))
   {
      IncCurrExp(RangerFlag, exppts);
      if (UseLimits)
      {
        int cexp = GetRangerExp();
        int mexp = GetMaxRangerExpForLevel(GetCurrentLevel(RangerFlag));
        int minxp = GetMinRangerExpForLevel(GetCurrentLevel(RangerFlag));
        cexp = max(cexp, minxp);
        SetCurrExp(RangerFlag,  min(cexp,mexp) );
      }
   }

   if (isCleric(ctype))
   {
      IncCurrExp(ClericFlag, exppts);
      if (UseLimits)
      {
        int cexp = GetClericExp();
        int mexp = GetMaxClericExpForLevel(GetCurrentLevel(ClericFlag));
        int minxp = GetMinClericExpForLevel(GetCurrentLevel(ClericFlag));
        cexp = max(cexp, minxp);
        SetCurrExp(ClericFlag,  min(cexp,mexp) );
      }
   }

   if (isDruid(ctype))
   {
      IncCurrExp(DruidFlag, exppts);
      if (UseLimits)
      {
        int cexp = GetDruidExp();
        int mexp = GetMaxDruidExpForLevel(GetCurrentLevel(DruidFlag));
        int minxp = GetMinDruidExpForLevel(GetCurrentLevel(DruidFlag));
        cexp = max(cexp, minxp);
        SetCurrExp(DruidFlag,  min(cexp,mexp) );
      }
   }

   if (isMagicUser(ctype))
   {
      IncCurrExp( MagicUserFlag, exppts);
      if (UseLimits)
      {
        int cexp = GetMagicUserExp();
        int mexp = GetMaxMagicUserExpForLevel(GetCurrentLevel(MagicUserFlag));
        int minxp = GetMinMagicUserExpForLevel(GetCurrentLevel(MagicUserFlag));
        cexp = max(cexp, minxp);
        SetCurrExp(MagicUserFlag,  min(cexp,mexp) );
      }
   }

   if (isThief(ctype))
   {
      IncCurrExp(ThiefFlag, exppts);
      if (UseLimits)
      {
        int cexp = GetThiefExp();
        int mexp = GetMaxThiefExpForLevel(GetCurrentLevel(ThiefFlag));
        int minxp = GetMinThiefExpForLevel(GetCurrentLevel(ThiefFlag));
        cexp = max(cexp, minxp);
        SetCurrExp(ThiefFlag,  min(cexp,mexp) );
      }
   }
}

//*****************************************************************************
// NAME:    CHARACTER::getCharTHACO
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
void CHARACTER::getCharTHACO()
{
  if (GetType() == MONSTER_TYPE)
  {
    THACO = monsterData.GetMonsterTHACO(origIndex);
    return;
  }

   int thaco = 20;

   classType prevClass = BogusClass;
   classType ctype = GetAdjClass();
   if (CanUsePrevClass())
     prevClass = GetPreviousClass();

   if (isFighter(ctype))
      thaco = min(thaco, (21 - FighterLevel));

   if (isPaladin(ctype))
      thaco = min(thaco, (21 - PaladinLevel));

   if (isRanger(ctype))
      thaco = min(thaco, (21 - RangerLevel));
  
   if (isCleric(ctype))
   {
		if (ClericLevel > 3)
			thaco = min( thaco, (20-(((ClericLevel-1)/3)*2)) );
   }
   
   if (isDruid(ctype))
   {
		if (DruidLevel > 3)
			thaco = min( thaco, (20-(((DruidLevel-1)/3)*2)) );
   }
   
   if (isMagicUser(ctype))
   {
		if (MULevel > 3)
			thaco = min( thaco, (20 - ((MULevel-1)/3)) );
   }

   if (isThief(ctype))
   {
		if (ThiefLevel > 2)
			thaco = min( thaco, (20 - ((ThiefLevel-1)/2)) );
   }

   if (isFighter(prevClass))
      thaco = min(thaco, (21 - prevFighterLevel));

   if (isPaladin(prevClass))
      thaco = min(thaco, (21 - prevPaladinLevel));

   if (isRanger(prevClass))
      thaco = min(thaco, (21 - prevRangerLevel));
  
   if (isCleric(prevClass))
   {
		if (prevClericLevel > 3)
			thaco = min( thaco, (20-(((prevClericLevel-1)/3)*2)) );
   }
   
   if (isDruid(prevClass))
   {
		if (prevDruidLevel > 3)
			thaco = min( thaco, (20-(((prevDruidLevel-1)/3)*2)) );
   }
   
   if (isMagicUser(prevClass))
   {
		if (prevMULevel > 3)
			thaco = min( thaco, (20 - ((prevMULevel-1)/3)) );
   }

   if (isThief(prevClass))
   {
		if (prevThiefLevel > 2)
			thaco = min( thaco, (20 - ((prevThiefLevel-1)/2)) );
   }


   THACO = thaco;

	if (THACO < 1)
		THACO = 1;
	if (THACO > 20)
		THACO = 20;
}

//*****************************************************************************
//    NAME: CHARACTER::GetTHACO
//
// PURPOSE: 
//
//*****************************************************************************
int CHARACTER::GetTHACO(void) const
{
  return THACO;
}

//*****************************************************************************
//    NAME: CHARACTER::determineNormalEncumbrance
//
// PURPOSE: 
//
//*****************************************************************************
int CHARACTER::determineNormalEncumbrance()
{
  return (DetermineNormalEncumbrance(GetAdjStr(), GetAdjStrMod()));
}

// ignore magical items
//*****************************************************************************
// NAME:    CHARACTER::determineEffectiveEncumbrance
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
int CHARACTER::determineEffectiveEncumbrance()
{
  if (GetType() == MONSTER_TYPE) return 0;

	int total = money.GetTotalWeight(); 

  POSITION pos = myItems.GetHeadPosition();
	while (pos != NULL)
  {
    //if (!isMagical(myItems.GetAt(pos).item))
    {
      total += getItemEncumbrance(myItems.GetAt(pos).item, 
                                  myItems.GetAt(pos).qty);
    }
    myItems.GetNext(pos);
  }
		
	return total;
}

// include all items, even magical
//*****************************************************************************
// NAME:    CHARACTER::determineEncumbrance
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
int CHARACTER::determineEncumbrance()
{
  if (GetType() == MONSTER_TYPE) return 0;

	int total = money.GetTotalWeight(); 

  POSITION pos = myItems.GetHeadPosition();
	while (pos != NULL)
  {
    total += getItemEncumbrance(myItems.GetAt(pos).item, 
                                myItems.GetAt(pos).qty);
    myItems.GetNext(pos);
  }

	return total;
}

//*****************************************************************************
// NAME:    CHARACTER::poolCharacterGold
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
int CHARACTER::poolCharacterGold()
{	
  return (int)money.ConvertToDefault(money.Total(), money.GetBaseType());
}

//*****************************************************************************
// NAME:    CHARACTER::getThiefSkills
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
void CHARACTER::getThiefSkills()
{
  int pp=0;
  int ol=0;
  int rt=0;
  int ms=0;
  int hs=0;
  int hn=0;
  int cw=0;
  int rl=0;

  classType ctype = GetAdjClass();
  classType prevClass = BogusClass;
  if (CanUsePrevClass())
    prevClass = GetPreviousClass();
  
  if (isThief(ctype) || isThief(prevClass))
  {
    int level = max(ThiefLevel, prevThiefLevel);

    switch (level)
    {
    case 1:
      pp=30;ol=25;rt=20;ms=15;hs=10;hn=10;cw=85;rl=0;
      break;
    case 2:
      pp=35;ol=29;rt=25;ms=21;hs=15;hn=10;cw=86;rl=0;
      break;
    case 3:
      pp=40;ol=33;rt=30;ms=27;hs=20;hn=15;cw=87;rl=0;
      break;
    case 4:
      pp=45;ol=37;rt=35;ms=33;hs=25;hn=15;cw=88;rl=20;
      break;
    case 5:
      pp=50;ol=42;rt=40;ms=40;hs=31;hn=20;cw=90;rl=25;
      break;
    case 6:
      pp=55;ol=47;rt=45;ms=47;hs=37;hn=20;cw=92;rl=30;
      break;
    case 7:
      pp=60;ol=52;rt=50;ms=55;hs=43;hn=25;cw=94;rl=35;
      break;
    case 8:
      pp=65;ol=57;rt=55;ms=62;hs=49;hn=25;cw=96;rl=40;
      break;
    case 9:
      pp=70;ol=62;rt=60;ms=70;hs=56;hn=30;cw=98;rl=45;
      break;
    case 10:
      pp=80;ol=67;rt=65;ms=78;hs=63;hn=30;cw=99;rl=50;
      break;
    case 11:
      pp=90;ol=72;rt=70;ms=86;hs=70;hn=35;cw=99;rl=55;
      break;
    case 12:
      pp=95;ol=77;rt=75;ms=94;hs=77;hn=35;cw=99;rl=60;
      break;
    case 13:
      pp=99;ol=82;rt=80;ms=99;hs=85;hn=40;cw=99;rl=65;
      break;
    case 14:
      pp=99;ol=87;rt=85;ms=99;hs=93;hn=40;cw=99;rl=70;
      break;
    case 15:
      pp=99;ol=92;rt=90;ms=99;hs=99;hn=50;cw=99;rl=75;
      break;
    case 16:
      pp=99;ol=97;rt=95;ms=99;hs=99;hn=50;cw=99;rl=80;
      break;
    default:
      pp=99;ol=99;rt=99;ms=99;hs=99;hn=55;cw=99;rl=80;
      break;  
    }
  }

	SetPickPockets(pp);
	SetOpenLocks(ol);
	SetFindTraps(rt);
	SetMoveSilent(ms);
	SetHideInShadows(hs);
	SetHearNoise(hn);
	SetClimbWalls(cw);
	SetReadLanguages(rl);
}

//*****************************************************************************
// NAME:    CHARACTER::getThiefSkillRaceAdjustments
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
void CHARACTER::getThiefSkillRaceAdjustments()
{
  classType prevClass = BogusClass;
  if (CanUsePrevClass())
    prevClass = GetPreviousClass();

  if ((!isThief(GetAdjClass())) && (!isThief(prevClass)))
    return;

  double pp=0.0;
  double ol=0.0;
  double rt=0.0;
  double ms=0.0;
  double hs=0.0;
  double hn=0.0;
  double cw=0.0;
  double rl=0.0;

  switch (race())
  {
  case Dwarf:
    pp=0.0;ol=0.10;rt=0.15;ms=0.0;hs=0.0;hn=0.0;cw=-0.10;rl=-0.05;
    break;
  case Elf:
    pp=0.05;ol=-0.05;rt=0.0;ms=0.05;hs=0.10;hn=0.05;cw=0.0;rl=0.0;
    break;
  case Gnome:
    pp=0.0;ol=0.05;rt=0.10;ms=0.05;hs=0.05;hn=0.10;cw=-0.15;rl=0.0;
    break;
  case HalfElf:
    pp=0.10;ol=0.0;rt=0.0;ms=0.0;hs=0.05;hn=0.0;cw=0.0;rl=0.0;
    break;
  case Halfling:
    pp=0.05;ol=0.05;rt=0.05;ms=0.10;hs=0.15;hn=0.05;cw=-0.15;rl=-0.05;
    break;
  default:
    break;
  }

	if (pp != 0) SetPickPockets((int)((double)GetPickPockets() * pp));
	if (ol != 0) SetOpenLocks((int)((double)GetOpenLocks() * ol));
	if (rt != 0) SetFindTraps((int)((double)GetFindTraps() * rt));
	if (ms != 0) SetMoveSilent((int)((double)GetMoveSilent() * ms));
	if (hs != 0) SetHideInShadows((int)((double)GetHideInShadows() * hs));
	if (hn != 0) SetHearNoise((int)((double)GetHearNoise() * hn));
	if (cw != 0) SetClimbWalls((int)((double)GetClimbWalls() * cw));
	if (rl != 0) SetReadLanguages((int)((double)GetReadLanguages() * rl));
}


//*****************************************************************************
// NAME:    CHARACTER::getThiefSkillDexAdjustments
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
void CHARACTER::getThiefSkillDexAdjustments()
{
  classType prevClass = BogusClass;
  if (CanUsePrevClass())
    prevClass = GetPreviousClass();

  if ((!isThief(GetAdjClass())) && (!isThief(prevClass)))
    return;

  double pp=0.0;
  double ol=0.0;
  double rt=0.0;
  double ms=0.0;
  double hs=0.0;

  switch (GetDex())
  {
  case 9:
    pp=-0.15;ol=-0.10;rt=-0.10;ms=-0.20;hs=-0.10;
    break;
  case 10:
    pp=-0.10;ol=-0.05;rt=-0.10;ms=-0.15;hs=-0.05;
    break;
  case 11:
    pp=-0.05;ol=0.0;rt=-0.05;ms=-0.10;hs=0.0;
    break;
  case 12:
    pp=0.0;ol=0.0;rt=0.0;ms=-0.05;hs=0.0;
    break;
  case 13:
  case 14:
  case 15:
    pp=0.0;ol=0.0;rt=0.0;ms=0.0;hs=0.0;
    break;
  case 16:
    pp=0.0;ol=0.05;rt=0.0;ms=0.0;hs=0.0;
    break;
  case 17:
    pp=0.05;ol=0.10;rt=0.0;ms=0.05;hs=0.05;
    break;
  case 18:
    pp=0.10;ol=0.15;rt=0.05;ms=0.10;hs=0.10;
    break;
  default:
    pp=0.15;ol=0.20;rt=0.10;ms=0.15;hs=0.15;
    break;
  }

	if (pp != 0) SetPickPockets((int)((double)GetPickPockets() * pp));
	if (ol != 0) SetOpenLocks((int)((double)GetOpenLocks() * ol));
	if (rt != 0) SetFindTraps((int)((double)GetFindTraps() * rt));
	if (ms != 0) SetMoveSilent((int)((double)GetMoveSilent() * ms));
	if (hs != 0) SetHideInShadows((int)((double)GetHideInShadows() * hs));
}

//*****************************************************************************
// NAME:    getThiefBackstabDamageMultiplier
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
int CHARACTER::getThiefBackstabDamageMultiplier()
{

  classType prevClass = BogusClass;
  if (CanUsePrevClass())
    prevClass = GetPreviousClass();

  if ((!isThief(GetAdjClass())) && (!isThief(prevClass)))
    return 0;

  int level = max(ThiefLevel, prevThiefLevel);

  if (level <= 4)
    return 2;
  else if (level <= 8)
    return 3;
  else if (level <= 12)
    return 4;
  else
    return 5;
}

//*****************************************************************************
// NAME:    CHARACTER::getThiefSkillArmorAdustments
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
void CHARACTER::getThiefSkillArmorAdustments()
{

}

//*****************************************************************************
// NAME:    CHARACTER::removeCharMoney
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
void CHARACTER::removeCharMoney(itemClassType itype, int qty)
{
  if (!itemIsMoney(itype))
  {
    WriteDebugString("Wrong item type passed to removeCharMoney()\n");
    return;
  }

  switch (itype) 
  {
  case GemType:
    money.RemoveMultGems(qty);
    break;
  case JewelryType:
    money.RemoveMultJewelry(qty);
    break;
  default:
    money.Subtract(itype, qty);
    break;
  }
 	encumbrance = determineEffectiveEncumbrance();
	determineMaxMovement();
}


//*****************************************************************************
// NAME:    CHARACTER::addCharMoneyToItemList
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
#ifdef UAFEngine
void CHARACTER::addCharMoneyToItemList(ITEM_TEXT_LIST &list)
{
  for (int i=0;i<money.NumCoinTypes();i++)
  {
    if (money.IsActive(globalData.moneyData.GetItemClass(i)))
    {
      itemClassType itype = MONEY_DATA_TYPE::GetItemClass(i);
      list.Add(money.Coins[i], itype, money.Name(itype));
    }
  }

  list.Add(money.NumGems(),GemType,globalData.moneyData.GetName(GemType));    
  list.Add(money.NumJewelry(), JewelryType,globalData.moneyData.GetName(JewelryType));

  ItemsOnPage = list.GetCount();
	ItemsOnPage = min(ItemsOnPage, Items_Per_Page);
}
#endif

//*****************************************************************************
// NAME:    CHARACTER::getCharWeaponText
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
void CHARACTER::getCharWeaponText(CString &wpn, CString &dmg)
{
  wpn = "";
  dmg = "";

	CString temp;
	CString temp2;

  temp = "";
  temp2 = "";
  int dmg_bonus=0;

  int wpnHand = myItems.GetReady(WeaponHand);

  if (wpnHand != NO_READY_ITEM)
  {
	  int item = myItems.GetItem(wpnHand);

    ITEM_DATA *data = itemData.GetItemData(item);

	  if (data != NULL)
	  {
      if ((isMagical(item)) && (myItems.GetId(wpnHand)))
		    wpn = data->Id_Name;
      else
		    wpn = data->Name; // The undecorated name.

      BOOL isMissile=FALSE;
      // if using bow or crossbow, use ammo damage
      if ((data->Wpn_Type == Bow) || (data->Wpn_Type == Crossbow))
      {
        int ammoHand = myItems.GetReady(AmmoQuiver);
        if (ammoHand != NO_READY_ITEM)
        {
          item = myItems.GetItem(ammoHand);
          data = itemData.GetItemData(item);
          isMissile=TRUE;
        }
      }

      if ((data->Dmg_Dice_Sm > 0) && (data->Nbr_Dice_Sm > 0))
      {
        dmg_bonus = data->Dmg_Bonus_Sm;

		    temp.Format("%iD%i", data->Nbr_Dice_Sm, data->Dmg_Dice_Sm);

        if ((isMagical(item)) && (myItems.GetId(wpnHand)))
          dmg_bonus += max(data->Attack_Bonus, data->Dmg_Bonus_Sm);
    
        if (!isMissile)
          dmg_bonus += GetAdjDmgBonus();

		    if (dmg_bonus > 0)
		    {
			    temp2.Format("+%i",dmg_bonus);
			    temp += temp2;
		    }
      }
	  
		  dmg = temp;
    }
	}
	else 
  {
		temp.Format("%iD%i", unarmedNbrDieS, unarmedDieS);

    dmg_bonus = unarmedBonus;
    dmg_bonus += GetAdjDmgBonus();

		if (dmg_bonus > 0)
		{
			temp2.Format("+%i",dmg_bonus);
			temp += temp2;
		}
	
		dmg = temp;
		wpn = "NONE";
	}
}

//*****************************************************************************
// NAME:    CHARACTER::getCharArmorText
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
void CHARACTER::getCharArmorText(CString &armor)
{
		int index = myItems.GetReady(BodyArmor); // dude's ready armor, if any

		if (index != NO_READY_ITEM)
    {
      if ((isMagical(myItems.GetItem(index))) && (myItems.GetId(index)))
			  armor = itemData.GetItemIdName(myItems.GetItem(index));
      else
			  armor = itemData.GetItemName(myItems.GetItem(index));
    }
		else
			armor = "";
}

//*****************************************************************************
// NAME:    CHARACTER::addCharacterItem
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
BOOL CHARACTER::addCharacterItem(int item, int qty, int numCharges, BOOL id, int cost)
{
  if ((getItemEncumbrance(item, qty) + GetEncumbrance()) > GetAdjMaxEncumbrance())
  {
		SetMiscError(TooMuchWeight);
    return FALSE;
  }

  if (!myItems.addItem(item, qty, numCharges, id, cost))
  {
    WriteDebugString("Failed to addItem to character\n");
    return FALSE;
  }

	encumbrance = determineEffectiveEncumbrance();
	determineMaxMovement();

	return TRUE;
}

//*****************************************************************************
// NAME:    CHARACTER::delCharacterItem
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
BOOL CHARACTER::delCharacterItem(int index, int qty)
{
  if (!myItems.deleteItem(index, qty))
  {
    WriteDebugString("Failed to deleteItem in delCharacterItem()\n");
    return FALSE;
  }

  encumbrance = determineEffectiveEncumbrance();
	determineMaxMovement();
  return TRUE;
}

//*****************************************************************************
// NAME:    CHARACTER::buyItem
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
void CHARACTER::buyItem(int item, costFactorType cf)
{
	if (GetAdjStatus() != Okay)
	{
		switch (GetAdjStatus()) 
		{
		case Unconscious: SetMiscError(UnconsciousChar);break;
		case Dead: SetMiscError(DeadChar);break;
		case Fled: SetMiscError(FledChar);break;
		case Petrified: SetMiscError(PetrifiedChar);break;
		case Gone: SetMiscError(GoneChar);break;
		case Animated: SetMiscError(AnimatedChar);break;
		case TempGone: SetMiscError(TempGoneChar);break;
		case Running: SetMiscError(RunningChar);break;
		case Dying: SetMiscError(DyingChar);break;
		default: SetMiscError(UnknownError);break;
		}
		return;
	}

  ITEM_DATA *data = itemData.GetItemData(item);
  if (data == NULL)
    return;

  DWORD cost = ApplyCostFactor(cf, data->Cost);

  if (!enoughMoney(cost,0,0,globalData.moneyData.GetDefaultType()))
  {
    SetMiscError(NotEnoughMoney);
    return;
  }

	if ((getItemEncumbrance(item, 1) + GetEncumbrance()) > GetAdjMaxEncumbrance())
		SetMiscError(TooMuchWeight);
	else 
  {
    int bundle = max(data->Bundle_Qty,1);
    // item is identified if purchased from shop
		if (!addCharacterItem(item, bundle, data->Num_Charges, TRUE, cost))
			SetMiscError(MaxItemsReached);
		else
			payForItem(cost, globalData.moneyData.GetDefaultType());
	}
}


//*****************************************************************************
// NAME:    CHARACTER::enoughMoney
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
BOOL CHARACTER::enoughMoney(int moneyCost, int gemCost, int jewelryCost, itemClassType moneyType)
{ 
  bool enough = true;

  if (moneyCost > 0)
  {
    enough=false;

    if (party.moneyPooled)
      enough = (party.poolSack.HaveEnough(moneyType, moneyCost)?true:false);    

    if (!enough) // character's money
      enough = (money.HaveEnough(moneyType, moneyCost)?true:false);
  }

  if ((enough) && (gemCost > 0))
    enough = (money.NumGems() >= gemCost);

  if ((enough) && (jewelryCost > 0))
    enough = (money.NumJewelry() >= jewelryCost);

  return enough; 
}


//*****************************************************************************
// NAME:    CHARACTER::payForItem
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
void CHARACTER::payForItem(int moneyCost, itemClassType moneyType, int gemCost, int jewelryCost)
{
  if (moneyCost > 0)
  {
    if ((party.moneyPooled) && (party.poolSack.HaveEnough(moneyType, moneyCost)))
      party.poolSack.Subtract(moneyType, moneyCost);
    else
      money.Subtract(moneyType, moneyCost);
  }

  if (gemCost > 0)
    money.RemoveMultGems(gemCost);

  if (jewelryCost > 0)
    money.RemoveMultJewelry(jewelryCost);

 	encumbrance = determineEffectiveEncumbrance();
	determineMaxMovement();
}


//*****************************************************************************
// NAME:    CHARACTER::toggleReadyItem
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
BOOL CHARACTER::toggleReadyItem(int item)
{
  BOOL success = FALSE;
  ITEM_DATA *data = itemData.GetItemData(myItems.GetItem(item));
  if (data == NULL) return FALSE;

  if (myItems.IsReady(item))
  {
    myItems.UnReady(item);
    success = !myItems.IsReady(item);
#ifdef UAFEngine
    data->specAbs.DisableAllFor(this);
#endif
  }
  else
  {
    switch (data->Location_Carried)
    {
		case WeaponHand:
      ReadyWeapon(item);
			break;
		case ShieldHand:
      ReadyShield(item);
			break;
		case BodyArmor:
      ReadyArmor(item);
			break;
		case Hands:
      ReadyGauntlets(item);
			break;
		case Head:
      ReadyHelm(item);
			break;
		case Waist:
      ReadyBelt(item);
			break;
		case BodyRobe:
      ReadyRobe(item);
			break;
		case Back:
      ReadyCloak(item);
			break;
		case Feet:
      ReadyBoots(item);
			break;
		case Fingers: // should separate this into ring1 and ring2?
      ReadyRing(item);
			break;
		case AmmoQuiver:
      ReadyAmmo(item);
			break;
    }
    success = myItems.IsReady(item);
	}

  if (success)
    SetCharAC();

  return success;
}

//*****************************************************************************
// NAME:    CHARACTER::serializeCharacter
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
BOOL CHARACTER::serializeCharacter(BOOL save, LPCSTR fname)
{
  // cannot load/store NPC or Monster
  if (GetType() == MONSTER_TYPE) return FALSE; 
  //if ((GetType() == NPC_TYPE) && (IsPreGen)) return FALSE;
  if (!CanBeSaved) return FALSE;

  BOOL success = TRUE;
  CFile myFile;
  CreateSaveDirectory(); 

  if (!save) // load character
  {     
    CString fullPath;

    // always load npc from design save folder
    if (GetType()==NPC_TYPE)
    {
      fullPath = GetSavePath();
      fullPath += "DCNPC_";
    }
    else // not an NPC
    {
      // first look in design save folder
      fullPath = GetSavePath();
      fullPath += fname;
      fullPath += ".chr";

      // if char doesn't exist in design save folder, 
      // try common save folder
      if (!FileExists(fullPath)) 
      {
        fullPath = GetCommonCharPath();
        if (fullPath.IsEmpty()) return FALSE; // common folder not defined
        fullPath += fname;
        fullPath += ".chr";
        if (!FileExists(fullPath)) return FALSE;
      }
    }    

    if (!myFile.Open(fullPath, CFile::modeRead))
    {
      success = FALSE;
      WriteDebugString("Unable to open the character file (read) %s\n", fullPath);
    }
    else
    {      
      // save original data
      CHARACTER tempCHARACTER = *this;

      BOOL VersionError = FALSE;
      
      try 
      {
        // check for save game version
        __int64 hdr=0;
        double ver = ENGINE_VER;

        myFile.Read(&hdr, sizeof(hdr));
        if (hdr == 0xFABCDEFABCDEFABF)
        {
          myFile.Read(&ver, sizeof(ver));
        }
        else
        {
          // special hdr not present, so reset archive to beginning
          myFile.SeekToBegin();
          // set version to last build that didn't save CHR version
          ver = _VERSION_0563_;
        }

        if (ver > ENGINE_VER)
        {
          VersionError = TRUE;
          WriteDebugString("Possible saved character version error: file %s, file ver %f, game ver %f",
                           fullPath, ver, ENGINE_VER);
        }

        CAR ar(&myFile, CArchive::load);
        Serialize(ar, ver);
        ar.Close();
        spells.list.SortSpells();
      }
      catch (...)
      {
        success = FALSE;
        WriteDebugString("Unable to load the character file %s\n", fullPath);
      }
      
      if (!success)        
      {
        if (VersionError)
          SetMiscError(SaveCharVersionError);
        else
          SetMiscError(FileLoadError);
        // restore original contents
        *this = tempCHARACTER;
      }
      else
      {
        // after loading saved character, make player choose
        // to put player on automatic combat AI
        automatic = FALSE;
      }
    }
  }
  else 
  { 
    // saved char/npc's always go in design save folder
    CString fullPath;
    fullPath = GetSavePath();
    if (GetType()==NPC_TYPE) fullPath += "DCNPC_";
    fullPath += fname;
    fullPath += ".chr";

    // remove read-only flags
    SetFileAttributes(fullPath, FILE_ATTRIBUTE_NORMAL);
    // save character    
    if (!myFile.Open(fullPath, CFile::modeWrite | CFile::modeCreate))
    {
      success = FALSE;
      WriteDebugString("Unable to open the save character file (write) %s\n", fullPath);
    }
    else
    {
      if (icon.filename != "")
      {
        CString tmpPath;
        CString newName="";
        CString tmp;
        tmpPath = GetSavePath();
        if (GetType()==NPC_TYPE) newName += "DCNPC_";
        newName += GetName();
        newName += "_icon";
        tmp = icon.filename;
        StripFilenamePath(tmp);
        int index = tmp.Find('.', 0);
        if (index > 0) newName += tmp.Right(tmp.GetLength()-(index));
        tmpPath += newName;
        // copy characters icon to save folder
        SetFileAttributes(tmpPath, FILE_ATTRIBUTE_NORMAL);
        if (CopyFile(icon.filename, tmpPath, FALSE))
          icon.filename = tmpPath;
      }
      if (smallPic.filename != "")
      {
        CString tmpPath;
        CString newName="";
        CString tmp;
        tmpPath = GetSavePath();
        if (GetType()==NPC_TYPE) newName += "DCNPC_";
        newName += GetName();
        newName += "_spic";
        tmp = smallPic.filename;
        StripFilenamePath(tmp);
        int index = tmp.Find('.', 0);
        if (index > 0) newName += tmp.Right(tmp.GetLength()-(index));
        tmpPath += newName;
        // copy characters small pic to save folder
        SetFileAttributes(tmpPath, FILE_ATTRIBUTE_NORMAL);
        if (CopyFile(smallPic.filename, tmpPath, FALSE))
          smallPic.filename = tmpPath;
      }
      CAR ar(&myFile, CArchive::store);         
      
      try 
      {
        // put signal value and version into saved character file
        __int64 hdr = 0xFABCDEFABCDEFABF;
        ar.Write(&hdr, sizeof(hdr));
        double ver = ENGINE_VER;
        ar.Write(&ver, sizeof(ver));

        Serialize(ar, ver);
      }
      catch (...)
      {
        success = FALSE;
        WriteDebugString("Unable to save the character file %s\n", fullPath);
      }
      
      ar.Close();
      if (!success)
        SetMiscError(FileSaveError);
    }
  }
  
  return success;
}


//*****************************************************************************
// NAME: CHARACTER::CanDualClassCleric
//
// PURPOSE:  
//*****************************************************************************
BOOL CHARACTER::CanDualClassCleric()
{
  if (GetType() == MONSTER_TYPE) return FALSE;

  if (isCleric(GetClass()))
    return FALSE;

  if (GetWis() < 17)
    return FALSE;

  return TRUE;
}


//*****************************************************************************
// NAME: CHARACTER::CanDualClassFighter
//
// PURPOSE:  
//*****************************************************************************
BOOL CHARACTER::CanDualClassFighter()
{
  if (GetType() == MONSTER_TYPE) return FALSE;

  if (isFighter(GetClass()))
    return FALSE;

  if (GetStr() < 17)
    return FALSE;

  return TRUE;
}


//*****************************************************************************
// NAME: CHARACTER::CanDualClassMagicUser
//
// PURPOSE:  
//*****************************************************************************
BOOL CHARACTER::CanDualClassMagicUser()
{
  if (GetType() == MONSTER_TYPE) return FALSE;

  if (isMagicUser(GetClass()))
    return FALSE;

  if (GetInt() < 17)
    return FALSE;

  return TRUE;
}


//*****************************************************************************
// NAME: CHARACTER::CanDualClassThief
//
// PURPOSE:  
//*****************************************************************************
BOOL CHARACTER::CanDualClassThief()
{
  if (GetType() == MONSTER_TYPE) return FALSE;

  if (isThief(GetClass()))
    return FALSE;

  if (GetDex() < 17)
    return FALSE;

  return TRUE;
}

//*****************************************************************************
// NAME: CHARACTER::CanDualClassDruid
//
// PURPOSE:  
//*****************************************************************************
BOOL CHARACTER::CanDualClassDruid()
{
  if (GetType() == MONSTER_TYPE) return FALSE;

  if (isDruid(GetClass()))
    return FALSE;

  if (GetCha() < 17) return FALSE;
  if (GetWis()   < 17) return FALSE;

  return TRUE;
}


//*****************************************************************************
// NAME: CHARACTER::CanDualClassRanger
//
// PURPOSE:  
//*****************************************************************************
BOOL CHARACTER::CanDualClassRanger()
{
  if (GetType() == MONSTER_TYPE) return FALSE;

  if (isRanger(GetClass()))
    return FALSE;

  if (GetStr()     < 17) return FALSE;
  if (GetWis()       < 17) return FALSE;
  if (GetInt() < 17) return FALSE;

  return TRUE;
}


//*****************************************************************************
// NAME: CHARACTER::CanDualClassPaladin
//
// PURPOSE:  
//*****************************************************************************
BOOL CHARACTER::CanDualClassPaladin()
{
  if (GetType() == MONSTER_TYPE) return FALSE;

  if (isPaladin(GetClass()))
    return FALSE;

  if (GetStr() < 17) return FALSE;
  if (GetWis()   < 17) return FALSE;

  return TRUE;
}


//*****************************************************************************
// NAME: CHARACTER::CanUsePrevClass
//
// PURPOSE:  
//*****************************************************************************
BOOL CHARACTER::CanUsePrevClass()
{
  if (!IsDualClass())
    return FALSE;

  int currLevel = 0;
  switch (GetClass())
  {
  case Fighter:
    currLevel = FighterLevel;
    break;
  case Cleric:
    currLevel = ClericLevel;
    break;
  case Ranger:
    currLevel = RangerLevel;
    break;
  case Paladin:
    currLevel = PaladinLevel;
    break;
  case MagicUser:
    currLevel = MULevel;
    break;
  case Thief:
    currLevel = ThiefLevel;
    break;
  case Druid:
    currLevel = DruidLevel;
    break;
  default: return FALSE;
  }

  int prevLevel = 0;
  switch (GetPreviousClass())
  {
  case Fighter:
    prevLevel = prevFighterLevel;
    break;
  case Cleric:
    prevLevel = prevClericLevel;
    break;
  case Ranger:
    prevLevel = prevRangerLevel;
    break;
  case Paladin:
    prevLevel = prevPaladinLevel;
    break;
  case MagicUser:
    prevLevel = prevMULevel;
    break;
  case Thief:
    prevLevel = prevThiefLevel;
    break;
  case Druid:
    prevLevel = prevDruidLevel;
    break;
  default: return FALSE;
  }

  if ((currLevel >= prevLevel) && (prevLevel > 0))
    return TRUE;
  
  return FALSE;
}


//*****************************************************************************
// NAME: CHARACTER::IsDualClass
//
// PURPOSE:  
//*****************************************************************************
BOOL CHARACTER::IsDualClass()
{
  if (prevFighterLevel > 0)
    return TRUE;
  if (prevClericLevel > 0)
    return TRUE;
  if (prevRangerLevel > 0)
    return TRUE;
  if (prevPaladinLevel > 0)
    return TRUE;
  if (prevMULevel > 0)
    return TRUE;
  if (prevThiefLevel > 0)
    return TRUE;
  if (prevDruidLevel > 0)
    return TRUE;

  return FALSE;
}

//*****************************************************************************
// NAME: CHARACTER::GetPreviousClass
//
// PURPOSE:  
//*****************************************************************************
classType CHARACTER::GetPreviousClass()
{
  if (prevFighterLevel > 0)
    return Fighter;
  if (prevClericLevel > 0)
    return Cleric;
  if (prevRangerLevel > 0)
    return Ranger;
  if (prevPaladinLevel > 0)
    return Paladin;
  if (prevMULevel > 0)
    return MagicUser;
  if (prevThiefLevel > 0)
    return Thief;
  if (prevDruidLevel > 0)
    return Druid;

  return BogusClass;
}


//*****************************************************************************
// NAME: CHARACTER::GetPreviousClassLevel
//
// PURPOSE:  
//*****************************************************************************
BYTE CHARACTER::GetPreviousClassLevel()
{
  switch (GetPreviousClass())
  {
  case Fighter:
    return prevFighterLevel;
  case Cleric:
    return prevClericLevel;
  case Ranger:
    return prevRangerLevel;
  case Paladin:
    return prevPaladinLevel;
  case MagicUser:
    return prevMULevel;
  case Thief:
    return prevThiefLevel;
  case Druid:
    return prevDruidLevel;
  default:
    return 0;
  }
}


//*****************************************************************************
// NAME: CHARACTER::CanChangeClassHuman
//
// PURPOSE:  
//*****************************************************************************
BOOL CHARACTER::CanChangeClassHuman()
{
  if (GetType() == MONSTER_TYPE) return FALSE;

  // must be human
  if (race() != Human)
    return FALSE;

  // if already changed classes
  if (IsDualClass())
    return FALSE;

  // must have min of 15 in primary attribute(s) of
  // current class
  if ((isFighter(GetClass())) && (GetStr() < 15))
   return FALSE;

  if (isPaladin(GetClass()))
  {
   if (GetStr() < 15) return FALSE;
   if (GetWis()   < 15) return FALSE;
  }

  if (isRanger(GetClass()))
  {
   if (GetStr()     < 15) return FALSE;
   if (GetWis()       < 15) return FALSE;
   if (GetInt() < 15) return FALSE;
  }

  if ((isCleric(GetClass())) && (GetWis() < 15))
   return FALSE;

  if (isDruid(GetClass()))
  {
   if (GetCha() < 15) return FALSE;
   if (GetWis()   < 15) return FALSE;
  }

  if ((isMagicUser(GetClass())) && (GetInt() < 15))
   return FALSE;

  if ((isThief(GetClass())) && (GetDex() < 15))
   return FALSE;

  // must have min of 17 in primary attribute(s) of
  // new class
  //
  // don't know new class yet, so just check each
  // possibility and verify that at least one is
  // available
  //

  if (CanDualClassCleric()) return TRUE;
  if (CanDualClassRanger()) return TRUE;
  if (CanDualClassDruid()) return TRUE;
  if (CanDualClassPaladin()) return TRUE;
  if (CanDualClassThief()) return TRUE;
  if (CanDualClassFighter()) return TRUE;
  if (CanDualClassMagicUser()) return TRUE;

  return FALSE;
}

//*****************************************************************************
//    NAME: CHARACTER::HumanChangeClass
//
// PURPOSE: 
//
//*****************************************************************************
void CHARACTER::HumanChangeClass(classType newClass)
{
  switch (GetClass())
  {
  case Cleric:
    prevClericLevel = ClericLevel;
    break;
  case MagicUser:
    prevMULevel = MULevel;
    break;
  case Druid:
    prevDruidLevel = DruidLevel;
    break;
  case Ranger:
    prevRangerLevel = RangerLevel;
    break;
  case Thief:
    prevThiefLevel = ThiefLevel;
    break;
  case Paladin:
    prevPaladinLevel = PaladinLevel;
    break;
  case Fighter:
    prevFighterLevel = FighterLevel;
    break;
  default: return;
  }

  SetClass(newClass);
}

//*****************************************************************************
// NAME:    CHARACTER::canCastSpells
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
BOOL CHARACTER::canCastSpells()
{
  if (GetType() == MONSTER_TYPE) return FALSE;
  if (GetAdjStatus() != Okay) return FALSE;

#ifdef UAFEngine
  DWORD src;
  DWORD skey;
  
  if (GetAdjSpecAb(SA_HoldPerson,&src,&skey))
  {
    SetNbrAttacks(0.0);
    QueueUsedSpecAb(SA_HoldPerson,src,skey);
    return FALSE;
  }
  else if (GetAdjSpecAb(SA_Sleep,&src,&skey))
  {
    SetNbrAttacks(0.0);
    QueueUsedSpecAb(SA_Sleep,src,skey);
    return FALSE;
  }
  else if (GetAdjSpecAb(SA_Silence,&src,&skey))
  {
    SetNbrAttacks(0.0);
    QueueUsedSpecAb(SA_Silence,src,skey);
    return FALSE;
  }  
#endif
  

  if (CanUsePrevClass())
  {
    switch (GetPreviousClass())
    {
    case Cleric:
    case MagicUser:
    case Druid:   return TRUE;
    case Ranger:  if (prevRangerLevel >= 8) return TRUE;  break;
    case Paladin: if (prevPaladinLevel >= 9) return TRUE; break;
    default: break;
    }
  }

  switch (GetClass())
  {
   case Cleric:
   case MagicUser:
   case Druid:
   case C_MU:
   case C_F:
   case C_R:
   case C_T:
   case F_D:
   case C_F_MU:
   case D_MU:
   case F_MU_D:
   case F_MU:
   case F_MU_T:
   case MU_T:
     return TRUE;

   case Ranger: if (RangerLevel >= 8) return TRUE;  break;
   case Paladin:if (PaladinLevel >= 9)return TRUE;  break;
   default: return FALSE;
  }
  return FALSE;
}

//
// this is used for the GiveDamage event. Normally,
// damage is given during combat in combatant.cpp
//
//*****************************************************************************
//    NAME: CHARACTER::giveCharacterDamage
//
// PURPOSE: 
//
//*****************************************************************************
void CHARACTER::giveCharacterDamage(
                    spellSaveEffectType eventSave, 
                    int attackTHACO, 
                    int dmgDice, 
                    int dmgDiceQty, 
                    int dmgBon, 
                    spellSaveVsType spellSave, 
                    int saveBonus)
{
  int result = 0;
  int need = 0;
  BOOL saved = FALSE;
  
  if (eventSave == UseTHACO)
  {
    int catt = attackTHACO - GetAC();
    need = max(catt, 1);
    need = min(need, 20);
    
    if (rollDice(20,1) >= need)
      result = rollDice(dmgDice, dmgDiceQty, dmgBon);      
  }
  else
  {
    result = rollDice(dmgDice, dmgDiceQty, dmgBon);          
    saved = didCharacterSaveVs(spellSave, saveBonus);
    
    if (saved)
    {
      switch (eventSave)
      {
      case NoSave:// full affect
        break;
      case SaveNegates:
        result = 0;
        break;
      case SaveForHalf:
        result /= 2;
        result = max(1,result); 
        break;
      default: result=0; break;
      }
    }
  }
  
  if (result > 0)
  {
    giveCharacterDamage(result);
#ifdef UAFEngine
    PlayCharHit();
    CString msg;
    msg.Format("%s takes %i damage",
               GetName(), result);
    FormatPausedText(pausedTextData, msg);
    DisplayPausedText(pausedTextData);    
#endif
  }
}

//*****************************************************************************
// NAME:    CHARACTER::giveCharacterDamage
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
void CHARACTER::giveCharacterDamage(int damage)
{   
   charStatusType stype = GetAdjStatus();

   if (   (stype == Okay)
       || (stype == Running)
       || (stype == Unconscious)
       || (stype == Animated)
       || (stype == Dying))
   {
      hitPoints -= damage;

      if (hitPoints < -10)
         hitPoints = -10;

      if (hitPoints > maxHitPoints)
        hitPoints = maxHitPoints;

      if (GetAdjHitPoints() <= 0)
        SetStatus(Dead);
   }
}

//*****************************************************************************
// NAME:    CHARACTER::didCharacterSaveVs
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
BOOL CHARACTER::didCharacterSaveVs(spellSaveVsType stype, int bonus)
{
   int mr = GetAdjMagicResistance();
   if (mr > 0)
   {
     int mr_roll = rollDice(100,1);
     if (mr_roll <= mr)
       return TRUE;
   }

   BOOL success = FALSE; 
   int score=0;
   int tmp;

   switch (stype)
   {
   case ParPoiDM:
      tmp=(GetAdjSaveVsPPDM() - bonus);
      score = max(tmp, 1);
      break;
   case PetPoly:
      tmp=(GetAdjSaveVsPP() - bonus);
      score = max(tmp, 1);
      break;
   case RodStaffWand:
      tmp=(GetAdjSaveVsRSW() - bonus);
      score = max(tmp, 1);
      break;
   case Sp:
      tmp=(GetAdjSaveVsSp() - bonus);
      score = max(tmp, 1);
      break;
   case BreathWeapon:
      tmp=(GetAdjSaveVsBr() - bonus);
      score = max(tmp, 1);      
      break;
   }

   score = min(score, 20);
   int roll = rollDice(20,1);

#ifdef UAFEngine
   ModifySaveRollAsTarget(this, roll);

   ActorType actor = GetTargetCharacterContext().GetContext();
   if (actor!=NULL_ACTOR)
   {
     CHARACTER *pAttacker = GetCurrentlyActiveContext(&actor);
     if (pAttacker != NULL)
     {
       if (pAttacker!=this)
        pAttacker->ModifySaveRoll(this, roll);
     }
   }
#endif

   if (roll <= score)
      success = FALSE;
   else
      success = TRUE;

   return success;
}


//*****************************************************************************
// NAME:    CHARACTER::HasItem
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
BOOL CHARACTER::HasItem(int item)
{
  return myItems.HaveItem(item);
}


//*****************************************************************************
// NAME: CHARACTER::SaveArt
//
// PURPOSE:  
//*****************************************************************************
void CHARACTER::SaveArt()
{
  icon.SavePicData(GLOBAL_ART);
  smallPic.SavePicData(GLOBAL_ART);
}

//*****************************************************************************
//    NAME: CHARACTER::GetPrimeSpellCastingScore
//
// PURPOSE: 
//
//*****************************************************************************
BYTE CHARACTER::GetPrimeSpellCastingScore(WORD ClassFlag)
{
  ASSERT( NumClassBits(ClassFlag) == 1 );

  switch (ClassFlag)
  {
  case MagicUserFlag: return GetAdjInt();
  case ClericFlag:    return GetAdjWis();
  case ThiefFlag:     return GetAdjWis();
  case FighterFlag:   return GetAdjWis();
  case PaladinFlag:   return GetAdjWis();
  case RangerFlag:    return GetAdjWis();
  case DruidFlag:     return GetAdjWis();
  }
  ASSERT(FALSE);
  return 0;
}

//*****************************************************************************
// NAME:    CHARACTER::UpdateSpellAbility
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
void CHARACTER::UpdateSpellAbility()
{
  if (GetType() == MONSTER_TYPE) return;
  int i;
  WORD ClassMask = SetClassBitsFromCharClass(GetClass());

  WriteDebugString("Spell book summary for %s, character class %s:\n", GetName(), ClassFlagToText(ClassMask));  
  
  WORD spellmask = 0;
  for (i=0;i<NumClassFlags;i++)
  {
    WORD flag = (ClassMask & (0x0001 << i));
    if (flag) 
    {
      SetSpellLimits(flag, GetPrimeSpellCastingScore(flag), spells.spellLimits);
      spellmask |= GetSpellCastingClassForCharClass(flag);
    }
  }

  for (i=0;i<NumClassFlags;i++)
  {
    WORD flag = (ClassMask & (0x0001 << i));
    if (flag) 
    {
      //int maxlevel = CurrSpellLevel(flag, GetClassLevel(flag), GetPrimeSpellCastingScore(flag), SpellClass);      
      WriteDebugString("\tCurrent level for class %s = %u\n", ClassFlagToText(flag), GetClassLevel(flag));
      //WriteDebugString("\tMax spell level as class %s = %u\n", ClassFlagToText(flag), maxlevel);
    }
  }

  for (i=0;i<NumClassFlags;i++)
  {
    WORD flag = (ClassMask & (0x0001 << i));
    if (flag) 
    {
      int maxlevel = CurrSpellLevel(flag, GetClassLevel(flag), GetPrimeSpellCastingScore(flag), MagicUserFlag);      
      WriteDebugString("\tMax MU spell level as class %s = %u\n", ClassFlagToText(flag), maxlevel);
      maxlevel = CurrSpellLevel(flag, GetClassLevel(flag), GetPrimeSpellCastingScore(flag), ClericFlag);      
      WriteDebugString("\tMax Cleric spell level as class %s = %u\n", ClassFlagToText(flag), maxlevel);
    }
  }

  int spellsabovelevel = 0;
  int spellsadded = 0;
  int spellsforclass = 0;
  int spellsforschool = 0;
  int spellsineligible = 0;
  int spellsnotscribeable = 0;
  int spelladdfailed = 0;

  // cleric spells automatically get added to spell book if
  // character is eligible to use the spell
  //
  // Magic Users will automatically get spells that have
  // AutoScribeAtLevelChange=TRUE and for which the
  // character is eligible
  //
  POSITION pos = spellData.GetHeadPosition();
  while (pos != NULL)
  {
#ifdef _DEBUG // just to help debugging
    SPELL_DATA &Spell = spellData.GetAt(pos);
#endif

    DWORD skey = spellData.GetKeyAt(pos);
    WORD SpellClass = spellData.GetSpellClass(skey);
    WORD CastClass = spellData.GetAt(pos).CastMask;    

    if (SpellClass & spellmask)
    {
      spellsforschool++;
      
      if (CastClass & ClassMask)
        spellsforclass++;

      if (!spellData.GetAt(pos).UsableByParty)
        spellsineligible++;
    }

    BOOL know = (   (spellData.GetAt(pos).UsableByParty) // party can memorize/cast this one
                 && (SpellClass & spellmask) // right spell casting school
                 && (CastClass & ClassMask)); // class allowed to cast spells in this school
  
    if ((know) && (SpellClass == MagicUserFlag) && (!spellData.GetAt(pos).AutoScribeAtLevelChange))
    {
      know = FALSE;
      spellsnotscribeable++;
    }

    if (know)
    {
      int spelllevel = spellData.GetSpellLevel(skey);

      for (i=0;i<NumClassFlags;i++)
      {
        WORD flag = (ClassMask & (0x0001 << i));
        if (flag) 
        {
          int maxlevel = CurrSpellLevel(flag, GetClassLevel(flag), GetPrimeSpellCastingScore(flag), SpellClass);
  
          //WriteDebugString("\tCurrent level for class %s = %u\n", ClassFlagToText(flag), GetClassLevel(flag));
          //WriteDebugString("\tMax spell level as class %s = %u\n", ClassFlagToText(flag), maxlevel);

          if (spelllevel <= maxlevel)
          {     
            if (!spells.knowSpell(flag, SpellClass, skey, spelllevel, TRUE))
            {
              WriteDebugString("Failed to knowSpell in UpdateSpellAbility for %s\n", GetName());
              spelladdfailed++;
            }
            else
            {
              spellsadded++;
            }
          }
          else
            spellsabovelevel++;
        }
      }
    }
    spellData.GetNext(pos);
  }
  
  WriteDebugString("\tSpells Avail by school = %u\n", spellsforschool);
  WriteDebugString("\tSpells Avail for class = %u\n", spellsforclass);
  WriteDebugString("\tSpells Added = %u\n", spellsadded);
  WriteDebugString("\tSpells Not Added (not usable by party) = %u\n", spellsineligible);
  WriteDebugString("\tSpells Not Added (too high level) = %u\n", spellsabovelevel);
  WriteDebugString("\tSpells Not Added (not auto given to MU) = %u\n", spellsnotscribeable);  
  WriteDebugString("\tSpells Not Added (failed call to KnowSpell) = %u\n", spelladdfailed);  
}

//*****************************************************************************
//    NAME: CHARACTER::race
//
// PURPOSE: 
//
//*****************************************************************************
void CHARACTER::race(raceType newrace)
{
  m_race=newrace;
  // m_traits.Update(CM_race);
}

//*****************************************************************************
//    NAME: CHARACTER::GetAbility
//
// PURPOSE: 
//
//*****************************************************************************
BYTE CHARACTER::GetAbility(abilityType atype)
{
  if (strengthKey==atype)
    return GetAdjStr();
  else if (intelligenceKey==atype)
    return GetAdjInt();
  else if (wisdomKey==atype)
    return GetAdjWis();
  else if (dexterityKey==atype)
    return GetAdjDex();
  else if (constitutionKey==atype)
    return GetAdjCon();
  else if (charismaKey==atype)
    return GetAdjCha();
  return 0;
}

//*****************************************************************************
//    NAME: CHARACTER::GetAbilityLimits
//
// PURPOSE: 
//
//*****************************************************************************
ABILITYLIMITS CHARACTER::GetAbilityLimits(abilityType abType) // Min/Max allowed for character
{
  return classData.GetAbilityLimits(GetClass(), abType);
}

//*****************************************************************************
//    NAME: CHARACTER::getAllowedAlignments
//
// PURPOSE: 
//
//*****************************************************************************
WORD CHARACTER::getAllowedAlignments(void)
{
  return classData.GetAllowedAlignments(GetClass());
}



int CHAR_LIST::GetNextKey()
{
  int key=0;
  if (GetCount() > 0)
  {
    key = chars.GetTail().key;
    if (key >= INT_MAX)
    {
      // Need to wrap key.
      // Search for non-sequential gap between current keys.
      // A gap is guaranteed since the key range is greater than
      // MAX_CHARACTERS.
      // Gaps occur when deletions are made to the list
      POSITION p = chars.GetHeadPosition();
      BOOL found = FALSE;        
      int prevKey = 0;
      int currKey;
      while ((p != NULL) && (!found))
      {
        currKey = chars.GetNext(p).key;
        if (currKey != prevKey+1)
        {
          key = prevKey+1;
          found = TRUE;
        }
        else
          prevKey = currKey; 
      }
      VERIFY(found); // should never happen
    }
    else
      key++;
  }
  else
    key = 1;
  return key;
}

CHAR_LIST::CHAR_LIST() 
{ 
  Clear(); 
}

CHAR_LIST::CHAR_LIST(const CHAR_LIST& src) 
{ 
  *this = src; 
}

CHAR_LIST::~CHAR_LIST() 
{ 
  Clear(); 
}  

void CHAR_LIST::Clear() 
{ 
  CSingleLock sLock(&m_CS, TRUE); 
  chars.RemoveAll(); 
  m_IsTempCharList=FALSE;
}

int CHAR_LIST::GetCount() 
{ 
  CSingleLock sLock(&m_CS, TRUE); 
  return chars.GetCount(); 
}

int CHAR_LIST::numChars() 
{ 
  return GetCount(); 
}

POSITION CHAR_LIST::GetHeadPosition() 
{ 
  return chars.GetHeadPosition(); 
}

CHARACTER& CHAR_LIST::GetAt(POSITION pos) 
{ 
  return chars.GetAt(pos); 
}

CHARACTER& CHAR_LIST::GetNext(POSITION &pos) 
{ 
  return chars.GetNext(pos); 
}

CHAR_LIST& CHAR_LIST::operator=(const CHAR_LIST& src)
{
  if (&src == this) return *this;
  Clear();
  POSITION pos = const_cast<CHAR_LIST&>(src).chars.GetHeadPosition();
  while (pos != NULL)
    AddCharacterWithCurrKey(const_cast<CHAR_LIST&>(src).GetNext(pos));
  return *this;
}

void CHAR_LIST::Serialize(CArchive &ar, double version)
{
  CObject::Serialize(ar);
  int i;    
  if (ar.IsStoring())
  {
    ar << GetCount();
    POSITION pos = chars.GetHeadPosition();
    while (pos != NULL)
      chars.GetNext(pos).Serialize(ar, version);
  }
  else
  {
    Clear();
    CHARACTER data;
    int temp;
    ar >> temp;
    for (i=0; i<temp; i++)
    {
      data.Clear();
      data.Serialize(ar, version);
      AddCharacterWithCurrKey(data);
    }
  }
}

void CHAR_LIST::Serialize(CAR &ar, double version)
{
  int i;    
  if (ar.IsStoring())
  {
    ar << GetCount();
    POSITION pos = chars.GetHeadPosition();
    while (pos != NULL)
      chars.GetNext(pos).Serialize(ar, version);
  }
  else
  {
    Clear();
    CHARACTER data;
    int temp;
    ar >> temp;
    for (i=0; i<temp; i++)
    {
      data.Clear();
      data.Serialize(ar, version);
      AddCharacterWithCurrKey(data);
    }
  }
}

#ifdef UAFEDITOR
BOOL CHAR_LIST::Validate(FILE *pFile, WORD flags)
{
  BOOL success = TRUE;
  POSITION pos = chars.GetHeadPosition();
  while (pos != NULL)
  {
    if (flags & VAL_ICON)
    {
      if (chars.GetAt(pos).icon.filename != "")
      {
        if (!FileExists(chars.GetAt(pos).icon.filename))
        {
          WriteValidateLog(pFile, "NPC %s missing Icon file %s\n",
                           chars.GetAt(pos).GetName(), chars.GetAt(pos).icon.filename);
          success = FALSE;
        }
      }
    }
    if (flags & VAL_SPIC)
    {
      if (chars.GetAt(pos).smallPic.filename != "")
      {
        if (!FileExists(chars.GetAt(pos).smallPic.filename))
        {
          WriteValidateLog(pFile, "NPC %s missing Small Pic file %s\n",
                           chars.GetAt(pos).GetName(), chars.GetAt(pos).smallPic.filename);
          success = FALSE;
        }
      }
    }
    chars.GetNext(pos);
  }
  return success;
}

BOOL CHAR_LIST::Validate(int index, FILE *pFile, WORD flags)
{
  BOOL success = TRUE;
  POSITION pos;
  if ((pos = chars.FindKeyPos(index)) != NULL)
  {
    if (flags & VAL_ICON)
    {
      if (chars.GetAt(pos).icon.filename != "")
      {
        if (!FileExists(chars.GetAt(pos).icon.filename))
        {
          WriteValidateLog(pFile, "NPC %s missing Icon file %s\n",
                           chars.GetAt(pos).GetName(), chars.GetAt(pos).icon.filename);
          success = FALSE;
        }
      }
    }
    if (flags & VAL_SPIC)
    {
      if (chars.GetAt(pos).smallPic.filename != "")
      {
        if (!FileExists(chars.GetAt(pos).smallPic.filename))
        {
          WriteValidateLog(pFile, "NPC %s missing Small Pic file %s\n",
                           chars.GetAt(pos).GetName(), chars.GetAt(pos).smallPic.filename);
          success = FALSE;
        }
      }
    }
  }
  else
  {
    WriteValidateLog(pFile, "Bogus NPC key %u\n", index);
    success = FALSE;
  }
  return success;
}
#endif
BOOL CHAR_LIST::HaveKey(int index)
{ 
  return (chars.FindKeyPos(index) != NULL); 
}

// CHAR_LIST is used for global NPCs,
// and also for temp characters during
// char generation and party building.
//
// called when loading from scripts, or
// when adding from editor GUI
int CHAR_LIST::AddCharacter(CHARACTER &data)
{
  CSingleLock sLock(&m_CS, TRUE);
  if (chars.GetCount() < MAX_CHARACTERS)
  {
    data.key = GetNextKey();
    //data.SetType(NPC_TYPE);
    data.SetPartyMember(FALSE);
    
    if (!IsTempCharList()) // don't update origIndex for temp char list
      data.origIndex = data.key;

    chars.Insert(data, data.key);
    return data.key;
  }
  else
    return 0;
}

// called when loading binary monster data that
// was saved with key info
int CHAR_LIST::AddCharacterWithCurrKey(CHARACTER &data)
{
  CSingleLock sLock(&m_CS, TRUE);
  if (chars.GetCount() < MAX_CHARACTERS)
  {
    data.SetPartyMember(FALSE);

    if (!IsTempCharList()) // don't update origIndex for temp char list
      data.origIndex = data.key;

    chars.Insert(data, data.key);
    return data.key;
  }
  else
    return 0;
}

CString CHAR_LIST::GetCharacterName(int index)
{
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = chars.FindKeyPos(index)) != NULL)
    return chars.GetAt(pos).GetName();
  else
    return "Unknown";
}

CHARACTER *CHAR_LIST::GetCharacterData(int index)
{
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = chars.FindKeyPos(index)) != NULL)
    return (&chars.GetAt(pos));
  else
    return NULL;
}

BOOL CHAR_LIST::SetCharacter(int index, CHARACTER &data)
{
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = chars.FindKeyPos(index)) != NULL)
  {
    chars.GetAt(pos) = data;
    return TRUE;
  }
  else
    return FALSE;
}

BOOL CHAR_LIST::DeleteCharacterEvents(int index)
{
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = chars.FindKeyPos(index)) != NULL)
  {
    globalData.eventData.DeleteEvents(chars.GetAt(pos).TalkEvent);
    chars.GetAt(pos).TalkLabel="TALK";
    globalData.eventData.DeleteEvents(chars.GetAt(pos).ExamineEvent);
    chars.GetAt(pos).ExamineLabel="EXAMINE";
    return TRUE;
  }
  else
    return FALSE;
}

BOOL CHAR_LIST::RemoveCharacter(int index)
{
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = chars.FindKeyPos(index)) != NULL)
  {
    //globalData.eventData.DeleteEvents(chars.GetAt(pos).TalkEvent);
    chars.RemoveAt(pos);
    return TRUE;
  }
  else
    return FALSE;
}

BOOL CHAR_LIST::GetCharacterData(int index, CHARACTER &data)
{
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = chars.FindKeyPos(index)) != NULL)
  {
    data = chars.GetAt(pos);
    return TRUE;
  }
  else
    return FALSE;
}  

// No locking... for internal use
int CHAR_LIST::m_FindNamei (const CString& name)
// Return key associated with the named object, or zero
{
  POSITION pos;
  pos=chars.GetHeadPosition(); 
  while (pos!=NULL)
  {
    if (chars.GetAt(pos).GetName().CompareNoCase(name)) 
          return chars.GetAt(pos).key;
    chars.GetNext(pos);
  };
  return 0;
}
// With locking...for public use
int CHAR_LIST::FindNamei(const CString& name)
// Return key associated with the named object, or zero
{
  CSingleLock sLock(&m_CS,TRUE);
  return m_FindNamei(name);
}

#ifdef UAFEngine
void CHAR_LIST::CastSpell(int index, int spell)
{
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = chars.FindKeyPos(index)) != NULL)
    chars.GetAt(pos).CastSpell(spell);
}

void CHAR_LIST::SpellActivate(int index, PENDING_SPELL &data)
{
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = chars.FindKeyPos(index)) != NULL)
    chars.GetAt(pos).SpellActivate(data);
}
#endif

//*****************************************************************************
// NAME: CHARACTER::getInfo
//
// PURPOSE: Provide string with lots of information about this character
//*****************************************************************************
CString CHARACTER::getInfo(void)
{
  CString result;
  result.Format("name=%s;"
                "pos=%d;"
                "THAC0=%d;"
                "AC=%d;"
                "age=%d;"
                "maxage=%d;"
                "HP=%d;"
                "maxHP=%d;"
                "enc=%d;"
                "maxenc=%d;"
                "maxmove=%d;"
                "STR=%d;"
                "INT=%d;"
                "WIS=%d;"
                "DEX=%d;"
                "CON=%d;"
                "CHA=%d;"
                "race=%s;"
                "gender=%s;"
                "class=%s;"
                "align=%s;",

                GetName(),
                marchOrder,
                GetAdjTHACO(),
                GetAdjAC(),
                GetAdjAge(),
                GetAdjMaxAge(),
                GetAdjHitPoints(),
                GetMaxHitPoints(),
                GetEncumbrance(),
                maxEncumbrance,
                GetAdjMaxMovement(),
                GetAdjStr(),
                GetAdjInt(),
                GetAdjWis(),
                GetAdjDex(),
                GetAdjCon(),
                GetAdjCha(),
                raceData.GetRaceName(m_race),
                GetAdjGender()==Male ? "male" : "female",
                classData[GetClass()].m_name,
                GetAlignmentName(GetAlignment()));
  return result;
}

//*****************************************************************************
//    NAME: CHARACTER::getItemList
//
// PURPOSE: 
//
//*****************************************************************************
CString CHARACTER::getItemList(int id)
{ //id is an identification number that the caller would like
  //associated with each of the items in this character's possession
  CString result;
  int itemId;
  CString temp;
  POSITION pos = myItems.GetHeadPosition();
	while (pos != NULL)
  {
    itemId = myItems.GetAt(pos).item;
    temp.Format( "{%s[%d]}", itemData.GetItemName(itemId), id);
    result += temp;
    myItems.GetNext(pos);
  }
  return result;
}
//*****************************************************************************
// NAME: CHARACTER::ReadyBestWpn
//
// PURPOSE: Select best weapon for auto dudes
//*****************************************************************************
void CHARACTER::ReadyBestWpn(int dist, BOOL isLargeTarget)
{
  ReadyWeapon(NO_READY_ITEM);

  // need to check for usable items
  // need to check for targets requiring magical weapons

  int dmgIdx = NO_READY_ITEM;
  int dmg = 0;
  int dmgSum = 0;
  int defIdx = NO_READY_ITEM;
  int def = 0;
  int attIdx = NO_READY_ITEM;
  int att = 0;
  //int minDist = 0;
  ITEM_DATA *data;

  // determine which avail item has best damage, 
  // attack, and protection values for given distance
  // to target
  //
  POSITION pos = myItems.GetHeadPosition();
  while (pos != NULL)
  {
    if ((data = itemData.GetItemData(myItems.GetAt(pos).item)) != NULL)
    {
      if (   (data->Location_Carried == WeaponHand)
          && (myItems.CanReady(myItems.GetKeyAt(pos), GetItemMask()))
          && (WpnCanAttackAtRange(myItems.GetAt(pos).item, dist)))
      {
        if (data->Protection_Base + data->Protection_Bonus <= def)
        {
          def = data->Protection_Base + data->Protection_Bonus;
          defIdx = myItems.GetKeyAt(pos);
        }
    
        if (isLargeTarget)
          dmgSum = (data->Dmg_Dice_Lg * data->Nbr_Dice_Lg) + data->Dmg_Bonus_Lg;
        else
          dmgSum = (data->Dmg_Dice_Sm * data->Nbr_Dice_Sm) + data->Dmg_Bonus_Sm;

        // check for fighter multiple attacks (swords/daggers only?)

        if (data->ROF_Per_Round > 0)
          dmgSum *= data->ROF_Per_Round;

        if (dmgSum >= dmg)
        {
          dmg = dmgSum;
          dmgIdx = myItems.GetKeyAt(pos);
        }
    
        if (data->Attack_Bonus >= att)
        {
          att = data->Attack_Bonus;
          attIdx = myItems.GetKeyAt(pos);
        }
      }
    }
    myItems.GetNext(pos);
  }

  // clear miscError that may have been set by CanReady()
  SetMiscError(NoError);

  // we now have best attack, protection, and damage items, if any,
  // for given distance to target

  // total damage potential per round
  int wpn_sum = dmgSum; // already includes rate of fire for weapon
  DetermineUnarmedHitDice();

  // ignore potential for natural attacks if
  // too far away to use them (distance to targ > 1)
  //
  if (dist <= 1)
  {
    int nonwpn_sum = 0;
    if (!isLargeTarget)
	    nonwpn_sum = (unarmedDieS * unarmedNbrDieS) + unarmedBonus;
    else
      nonwpn_sum = (unarmedDieL * unarmedNbrDieL) + unarmedBonus;

    nonwpn_sum *= GetNbrAttacks();

    // better off using natural attacks
    if (wpn_sum < nonwpn_sum) return;
  }

  // for now, choose item with most damage first
  int IdxToUse = dmgIdx;
  if (IdxToUse==NO_READY_ITEM)
    IdxToUse = attIdx;
  if (IdxToUse==NO_READY_ITEM)
    IdxToUse = defIdx;
  if (IdxToUse==NO_READY_ITEM)
    return;
 
  data = itemData.GetItemData(myItems.GetItem(IdxToUse));
  if (data != NULL)
  {
    if (data->Hands_to_Use > 1)
      myItems.UnReady(myItems.GetReady(ShieldHand));    
  }

  ReadyWeapon(IdxToUse);
  if (WpnUsesAmmo(myItems.GetItem(IdxToUse)))
    ReadyBestAmmo(isLargeTarget);
}

//*****************************************************************************
// NAME: CHARACTER::ReadyBestShield
//
// PURPOSE:  
//*****************************************************************************
void CHARACTER::ReadyBestShield()
{
  // if 2-handed weapon being used, can't ready a shield
  if (   (myItems.GetReady(ShieldHand) != NO_READY_ITEM)
      && (myItems.GetReady(ShieldHand) == myItems.GetReady(WeaponHand)))
    return;

  ReadyShield(NO_READY_ITEM);

  int defIdx = NO_READY_ITEM;
  int def = 0;
  ITEM_DATA *data;

  // determine which avail item has best
  // protection value
  //
  POSITION pos = myItems.GetHeadPosition();
  while (pos != NULL)
  {
    if ((data = itemData.GetItemData(myItems.GetAt(pos).item)) != NULL)
    {
      if (   (data->Location_Carried == ShieldHand)
          && (myItems.CanReady(myItems.GetKeyAt(pos), GetItemMask())))
      {
        if (data->Protection_Base + data->Protection_Bonus < def)
        {
          def = data->Protection_Base + data->Protection_Bonus;
          defIdx = myItems.GetKeyAt(pos);
        }        
      }
    }
    myItems.GetNext(pos);
  }

  // clear miscError that may have been set by CanReady()
  SetMiscError(NoError);

  // else try for best defensive bonus item
  if (defIdx != NO_READY_ITEM)
  {
    ReadyShield(defIdx);
    return;
  }
}


//*****************************************************************************
// NAME: CHARACTER::ReadyBestArmor
//
// PURPOSE:  
//*****************************************************************************
void CHARACTER::ReadyBestArmor()
{
  ReadyArmor(NO_READY_ITEM);

  int defIdx = NO_READY_ITEM;
  int def = 0;
  ITEM_DATA *data;

  // determine which avail item has best
  // protection value
  //
  POSITION pos = myItems.GetHeadPosition();
  while (pos != NULL)
  {
    if ((data = itemData.GetItemData(myItems.GetAt(pos).item)) != NULL)
    {
      if (   (data->Location_Carried == BodyArmor)
          && (myItems.CanReady(myItems.GetKeyAt(pos), GetItemMask())))
      {
        if (data->Protection_Base + data->Protection_Bonus < def)
        {
          def = data->Protection_Base + data->Protection_Bonus;
          defIdx = myItems.GetKeyAt(pos);
        }        
      }
    }
    myItems.GetNext(pos);
  }

  // clear miscError that may have been set by CanReady()
  SetMiscError(NoError);

  // else try for best defensive bonus item
  if (defIdx != NO_READY_ITEM)
  {
    ReadyArmor(defIdx);
    return;
  }
}

//*****************************************************************************
// NAME: CHARACTER::ReadyBestAmmo
//
// PURPOSE:  
//*****************************************************************************
void CHARACTER::ReadyBestAmmo(BOOL isLargeTarget)
{
  ReadyAmmo(NO_READY_ITEM);

  // need to check for targets requiring magical weapons

  int dmgIdx = NO_READY_ITEM;
  int dmg = 0;
  int dmgSum = 0;
  int defIdx = NO_READY_ITEM;
  int def = 0;
  int attIdx = NO_READY_ITEM;
  int att = 0;
  ITEM_DATA *data;

  POSITION pos = myItems.GetHeadPosition();
  while (pos != NULL)
  {
    if (   (myItems.CanReady(myItems.GetKeyAt(pos), GetItemMask()))
        && ((data = itemData.GetItemData(myItems.GetAt(pos).item)) != NULL))
    {
      if ((data->Location_Carried==AmmoQuiver)&&(data->Wpn_Type==Ammo))
      {
        if (data->Protection_Base + data->Protection_Bonus <= def)
        {
          def = data->Protection_Base + data->Protection_Bonus;
          defIdx = myItems.GetKeyAt(pos);
        }
    
        if (isLargeTarget)
          dmgSum = (data->Dmg_Dice_Lg * data->Nbr_Dice_Lg) + data->Dmg_Bonus_Lg;
        else
          dmgSum = (data->Dmg_Dice_Sm * data->Nbr_Dice_Sm) + data->Dmg_Bonus_Sm;

        // check for fighter multiple attacks (swords/daggers only?)
        dmgSum *= data->ROF_Per_Round;

        if (dmgSum >= dmg)
        {
          dmg = dmgSum;
          dmgIdx = myItems.GetKeyAt(pos);
        }
    
        if (data->Attack_Bonus >= att)
        {
          att = data->Attack_Bonus;
          attIdx = myItems.GetKeyAt(pos);
        }
      }
    }
    myItems.GetNext(pos);
  }

  // clear miscError that may have been set by CanReady()
  SetMiscError(NoError);

  // for now, choose item with most damage
  if (dmgIdx != NO_READY_ITEM)
  {
    ReadyAmmo(dmgIdx);
    return;
  }

  // else try for best attack bonus item
  if (attIdx != NO_READY_ITEM)
  {
    ReadyAmmo(attIdx);
    return;
  }

  // else try for best defensive bonus item
  if (defIdx != NO_READY_ITEM)
  {
    ReadyAmmo(defIdx);
    return;
  }
}

//*****************************************************************************
//    NAME: CHARACTER::UnreadyItemByType
//
// PURPOSE: 
//
//*****************************************************************************
void CHARACTER::UnreadyItemByType(itemLocationType loctype)
{
  int myitemidx = myItems.GetReady(loctype);
#ifdef UAFEngine
  ITEM_DATA *pData = itemData.GetItemData(myItems.GetItem(myitemidx));
  if (pData!=NULL) 
    pData->specAbs.DisableAllFor(this);
#endif
  myItems.UnReady(myitemidx); 
}

//*****************************************************************************
//    NAME: CHARACTER::DisplayCurrSpellEffects
//
// PURPOSE: 
//
//*****************************************************************************
void CHARACTER::DisplayCurrSpellEffects(const CString &SrcFunc)
{
#ifdef DUMP_CHAR_SPELL_ATTS

  if (spellEffects.GetCount() == 0)
    return;
  
  CString src;
  
  if (GetName().IsEmpty())
    return; // sometimes gets called for empty CHARACTER slots in PARTY struct

  Sleep(10);
  
  WriteDebugString("*** BEGIN SPELL_EFFECT records for \'%s\' from %s\n", GetName(),SrcFunc);

  POSITION pos = spellEffects.GetHeadPosition();
  while (pos != NULL)
  {
    SPELL_EFFECTS_DATA &data = spellEffects.GetNext(pos);

    if (data.flags & SPELL_EFFECTS_DATA::EFFECT_SPELL)
      src = "SPELL";
    else if (data.flags & SPELL_EFFECTS_DATA::EFFECT_CHARSPECAB)
      src = "CHARSPECAB";
    else if (data.flags & SPELL_EFFECTS_DATA::EFFECT_SPELLSPECAB)
      src = "SPELLSPECAB";
    else if (data.flags & SPELL_EFFECTS_DATA::EFFECT_ITEMSPECAB)
      src = "ITEMSPECAB";
    else
      src = "!!INVALID!!";


    WriteDebugString("   Attrib \'%s\', Parent %i, Src %s, Flags %u\n", 
            data.affectedAttr, data.parent, src, data.flags);
  }

  WriteDebugString("*** END SPELL_EFFECT records for \'%s\'\n", GetName());
#endif
}

//*****************************************************************************
//    NAME: CHARACTER::ReadyItemByType
//
// PURPOSE: 
//
//*****************************************************************************
void CHARACTER::ReadyItemByType(itemLocationType loctype, int index)
{
  if (index == NO_READY_ITEM)
  {
    // unready all items of this type
    UnreadyItemByType(loctype);
    return;
  }

  if (!myItems.CanReady(index, GetItemMask())) return;
  //UnreadyItemByType(loctype);
  myItems.Ready(index, GetItemMask()); 

#ifdef UAFEngine
  if (myItems.IsReady(index))
  {
    ITEM_DATA *pData = itemData.GetItemData(myItems.GetItem(index));
    if (pData!=NULL) 
    {
      int PreSpecAbCount = spellEffects.GetCount();

      pData->specAbs.EnableAllFor(this);

      int PostSpecAbCount = spellEffects.GetCount();
      int Diff = PostSpecAbCount - PreSpecAbCount;
      if (Diff > 0)
      {
        POSITION pos = spellEffects.GetTailPosition();  
        while ((pos != NULL) && (Diff > 0))
        {        
          CString SpecAbText = spellEffects.GetAt(pos).affectedAttr;
          spellEffects.GetAt(pos).flags |= SPELL_EFFECTS_DATA::EFFECT_ITEMSPECAB;
          spellEffects.GetAt(pos).key = pData->key;
          spellEffects.GetPrev(pos);
          Diff--;
          specialAbilitiesType sa = ConvertRuntimeIfTextToSpecAb(SpecAbText);
          QueueUsedSpecAb(sa, (WORD)SPELL_EFFECTS_DATA::EFFECT_ITEMSPECAB, pData->key);
        }

        CString msg = FormatSpecAbMsg(SPECAB_MSG_READY);
        while (msg.GetLength()>0)
        {
          FormatPausedText(pausedTextData,msg);
          DisplayPausedText(pausedTextData);
          msg = FormatSpecAbMsg(SPECAB_MSG_READY);
        }
      }
      DisplayCurrSpellEffects("ReadyItemByType");
    }
  }
#endif
}

//*****************************************************************************
//    NAME: CHARACTER::ReadyWeapon
//
// PURPOSE: 
//
//*****************************************************************************
void CHARACTER::ReadyWeapon(int index) 
{ 
  ReadyItemByType(WeaponHand, index);
}
//*****************************************************************************
//    NAME: CHARACTER::ReadyShield
//
// PURPOSE: 
//
//*****************************************************************************
void CHARACTER::ReadyShield(int index) 
{ 
  ReadyItemByType(ShieldHand, index);
}
//*****************************************************************************
//    NAME: CHARACTER::ReadyArmor
//
// PURPOSE: 
//
//*****************************************************************************
void CHARACTER::ReadyArmor(int index) 
{ 
  ReadyItemByType(BodyArmor, index);
}
//*****************************************************************************
//    NAME: CHARACTER::ReadyGauntlets
//
// PURPOSE: 
//
//*****************************************************************************
void CHARACTER::ReadyGauntlets(int index) 
{ 
  ReadyItemByType(Hands, index);
}
//*****************************************************************************
//    NAME: CHARACTER::ReadyHelm
//
// PURPOSE: 
//
//*****************************************************************************
void CHARACTER::ReadyHelm(int index) 
{ 
  ReadyItemByType(Head, index);
}
//*****************************************************************************
//    NAME: CHARACTER::ReadyBelt
//
// PURPOSE: 
//
//*****************************************************************************
void CHARACTER::ReadyBelt(int index) 
{ 
  ReadyItemByType(Waist, index);
}
//*****************************************************************************
//    NAME: CHARACTER::ReadyRobe
//
// PURPOSE: 
//
//*****************************************************************************
void CHARACTER::ReadyRobe(int index) 
{ 
  ReadyItemByType(BodyRobe, index);
}
//*****************************************************************************
//    NAME: CHARACTER::ReadyCloak
//
// PURPOSE: 
//
//*****************************************************************************
void CHARACTER::ReadyCloak(int index) 
{ 
  ReadyItemByType(Back, index);
}
//*****************************************************************************
//    NAME: CHARACTER::ReadyBoots
//
// PURPOSE: 
//
//*****************************************************************************
void CHARACTER::ReadyBoots(int index) 
{ 
  ReadyItemByType(Feet, index);
}
//*****************************************************************************
//    NAME: CHARACTER::ReadyRing
//
// PURPOSE: 
//
//*****************************************************************************
void CHARACTER::ReadyRing(int index) 
{ 
  ReadyItemByType(Fingers, index);
}
//*****************************************************************************
//    NAME: CHARACTER::ReadyAmmo
//
// PURPOSE: 
//
//*****************************************************************************
void CHARACTER::ReadyAmmo(int index) 
{ 
  ReadyItemByType(AmmoQuiver, index);
}

//*****************************************************************************
//    NAME: CHARACTER::determineNbrAttacks
//
// PURPOSE: 
//
//*****************************************************************************
float CHARACTER::determineNbrAttacks()
{
#ifdef UAFEngine
  DWORD src;
  DWORD skey;
  
  if (GetAdjSpecAb(SA_HoldPerson,&src,&skey))
  {
    SetNbrAttacks(0.0);
    QueueUsedSpecAb(SA_HoldPerson,src,skey);
    return GetNbrAttacks();
  }
  else if (GetAdjSpecAb(SA_Sleep,&src,&skey))
  {
    SetNbrAttacks(0.0);
    QueueUsedSpecAb(SA_Sleep,src,skey);
    return GetNbrAttacks();
  }
  
#endif
  if (GetType() != MONSTER_TYPE)
    SetNbrAttacks(1.0);
  else
    SetNbrAttacks(monsterData.GetMonsterNbrAttacks(origIndex));

  if (myItems.GetReady(WeaponHand) != NO_READY_ITEM)
  {
     double wpnAttacks = itemData.GetItemROF(myItems.GetItem(myItems.GetReady(WeaponHand)));
     if (wpnAttacks < 1.0) wpnAttacks = 1.0;
     SetNbrAttacks(wpnAttacks);
     // check for sweeps
  }
  return GetNbrAttacks();
}

//*****************************************************************************
//    NAME: CHARACTER::GetContext
//
// PURPOSE: 
//
//*****************************************************************************
ActorType CHARACTER::GetContext(WORD ClassFlag)
{
  ActorType data;
  data.Clear();

  if (IsPartyMember())
  {
    data.SetPartySrc();
  }
  else
  {
    switch (GetType())
    {
    case MONSTER_TYPE:
      data.SetMonsterSrc();
      break;
    case NPC_TYPE:
      data.SetNPCSrc();
      break;
    case CHAR_TYPE:
      // This will be used during character creation. The temp char
      // is displayed before it is added to the party.
      //return NULL_ACTOR;
      break;
    }
  }

  // if ClassFlag=0, GetCurrentLevel will
  // return the highest level attained from
  // all classes this character has
  data.Level = GetCurrentLevel(ClassFlag);
  ASSERT( (data.Level>=0) && (data.Level<=30) );

  data.Race = race();
  data.Class = GetClass();
  if (IsPartyMember())
    data.Instance = marchOrder; // assigned when added to party, never changes
  else
    data.Instance = origIndex;

  return data;
}

//*****************************************************************************
//    NAME: CHARACTER::EvalDiceAsClass
//
// PURPOSE: 
//
//*****************************************************************************
double CHARACTER::EvalDiceAsClass(DICEPLUS &dice, WORD ClassFlag)
{
  SetCharContext(GetContext(ClassFlag));
  double result;
  dice.Roll(result);
  ClearCharContext();
  return result;
}

#ifdef UAFEngine

//*****************************************************************************
//    NAME: CHARACTER::CalcRestTime
//
// PURPOSE: 
//
//*****************************************************************************
DWORD CHARACTER::CalcRestTime(void)
{
  if (GetAdjStatus() != Okay) return 0;
  return (spells.CalcRestTime());
}

//*****************************************************************************
//    NAME: CHARACTER::HaveEffectAttr
//
// PURPOSE: 
//
//*****************************************************************************
bool CHARACTER::HaveEffectAttr(const SPELL_EFFECTS_DATA &src) const
{
  POSITION pos = spellEffects.GetHeadPosition();
  while (pos != NULL)
  {
    if (spellEffects.GetAt(pos) == src) 
      return true;
    spellEffects.GetNext(pos);
  }
  return false;
}

//*****************************************************************************
//    NAME: CHARACTER::GetFirstEffectAttributeMod
//
// PURPOSE: 
//
//*****************************************************************************
POSITION CHARACTER::GetFirstEffectAttributeMod(CString attr)
{
  POSITION pos = spellEffects.GetHeadPosition();
  while (pos != NULL)
  {
    if (spellEffects.GetAt(pos).affectedAttr == attr) 
      return pos;
    spellEffects.GetNext(pos);
  }
  return NULL;
}
//*****************************************************************************
//    NAME: CHARACTER::GetNextEffectAttributeMod
//
// PURPOSE: 
//
//*****************************************************************************
POSITION CHARACTER::GetNextEffectAttributeMod(CString attr, POSITION start)
{
  POSITION pos = start;
  spellEffects.GetNext(pos);
  while (pos != NULL)
  {
    if (spellEffects.GetAt(pos).affectedAttr == attr) 
      return pos;
    spellEffects.GetNext(pos);
  }
  return NULL;
}

//*****************************************************************************
//    NAME: CHARACTER::IsAffectedBySpell
//
// PURPOSE: 
//
//*****************************************************************************
BOOL CHARACTER::IsAffectedBySpell(const CString& SpellName)
{
  // first, get spell db key
  DWORD spellkey = spellData.FindName(SpellName);
  if (spellkey < 0) 
  {
    WriteDebugString("IsAffectedBySpell(\'%s\') could not find spell name\n", SpellName);
    return FALSE;
  }

  POSITION pos = spellEffects.GetHeadPosition();  
  while (pos != NULL)
  {    
    if (spellEffects.GetAt(pos).key == spellkey)
    {
      return TRUE;
    }
    spellEffects.GetNext(pos);
  }
  return FALSE;
}

//*****************************************************************************
//    NAME: CHARACTER::IsAffectedBySpellAttribute
//
// PURPOSE: 
//
//*****************************************************************************
BOOL CHARACTER::IsAffectedBySpellAttribute(const CString& SpellAttribName)
{ 
  POSITION pos = spellEffects.GetHeadPosition();  
  while (pos != NULL)
  {    
    int spelldbkey = spellEffects.GetAt(pos).key;
    SPELL_DATA *pData = spellData.GetSpellData(spelldbkey);
    if (pData != NULL)
    {
      if (pData->spell_asl.Exists(SpellAttribName))
        return TRUE;
    }
    spellEffects.GetNext(pos);
  }

  return (char_asl.Exists(SpellAttribName));
}

// parent is the key from global list of active spells
//*****************************************************************************
//    NAME: CHARACTER::RemoveSpellEffect
//
// PURPOSE: 
//
//*****************************************************************************
BOOL CHARACTER::RemoveSpellEffect(DWORD parent, SPELL_DATA *pSpell)
{
  if (parent<0) return FALSE;
  bool found = false;
  
  POSITION pos = spellEffects.GetHeadPosition();  
  while (pos != NULL)
  {
    POSITION tmp=pos;
    spellEffects.GetNext(tmp);

    if (spellEffects.GetAt(pos).parent == parent)
    {
      // A few spell effects can linger longer than
      // parent spell, but most effects will return
      // TRUE here. Those that return FALSE will be
      // removed eventually by function CheckForExpiredSpellEffects().
      //
      if (spellEffects.GetAt(pos).IsReadyToExpire())
      {
        found = true;
        spellEffects.RemoveAt(pos);
      }
    }
    pos=tmp;
  }
  
  if (found)
  {
    if (pSpell!=NULL)
    {
      pSpell->specAbs.DisableAllFor(this);
      
      SetCharContext(GetContext());
      SetTargetContext(GetContext());
      
      pSpell->ExecuteSpellEndScript();
      
      ClearCharContext();
      ClearTargetContext();    
    }   
    
    WriteDebugString("Removing spell effects from %i for %s\n",
                     GetUniqueId(), pSpell->Name);
  }

  DisplayCurrSpellEffects("RemoveSpellEffect");
  return found;
}
//*****************************************************************************
//    NAME: CHARACTER::CheckForExpiredSpellEffects
//
// PURPOSE: 
//
//*****************************************************************************
void CHARACTER::CheckForExpiredSpellEffects()
{
  bool done = false;  

  while (!done)
  {
    BOOL removed = FALSE;
    POSITION pos = spellEffects.GetHeadPosition();  
    while (pos != NULL)
    {    
      // Look for spell effects in the list that have a duration different than
      // parent SPELL_DATA.
      // 
      if (spellEffects.GetAt(pos).StopTime > 0)
      {
        // remove spell effect (still checks expiration time)
        SPELL_DATA *pSpell = spellData.GetSpellData(spellEffects.GetAt(pos).key);

        removed = RemoveSpellEffect(spellEffects.GetAt(pos).parent, pSpell);
      }  

      if (!removed)
        spellEffects.GetNext(pos);
      else
        break;
    }   
    
    if (!removed)
      done = true;
  }
}

//*****************************************************************************
//    NAME: CHARACTER::UpdateSpellForDamage
//
// PURPOSE: 
//
//*****************************************************************************
void CHARACTER::UpdateSpellForDamage(int DamageTaken)
{
  if (DamageTaken<=0) return;

  bool found = false;
  POSITION pos = spellEffects.GetHeadPosition();  
  while (pos != NULL)
  {    
    int spelldbkey = spellEffects.GetAt(pos).key;

    SPELL_DATA *pSpell = spellData.GetSpellData(spelldbkey);
    if (pSpell != NULL)
    {
      if (pSpell->Duration_Rate == byDamageTaken)
      {
        int activespellkey = spellEffects.GetAt(pos).parent;
        ACTIVE_SPELL *pActive = ActiveSpellData.Get(activespellkey);
        if (pActive != NULL)
        {
          found = true;
          pActive->CountTime += DamageTaken;
          ActiveSpellData.Set(activespellkey, *pActive);
        }
      }
    }

    spellEffects.GetNext(pos);
  }

  if (found)
    ActiveSpellData.ProcessTimeSensitiveData(0);  
}

//*****************************************************************************
//    NAME: CHARACTER::UpdateSpellForAttacks
//
// PURPOSE: 
//
//*****************************************************************************
void CHARACTER::UpdateSpellForAttacks(int AttacksTaken)
{
  // this func is only called when a successful hit is 
  // made on this dude.

  if (AttacksTaken <= 0) return;

  bool found = false;
  POSITION pos = spellEffects.GetHeadPosition();  
  while (pos != NULL)
  {    
    int spelldbkey = spellEffects.GetAt(pos).key;
    
    SPELL_DATA *pSpell = spellData.GetSpellData(spelldbkey);
    if (pSpell != NULL)
    {
      if (pSpell->Duration_Rate == byNbrAttacks)
      {
        int activespellkey = spellEffects.GetAt(pos).parent;
        ACTIVE_SPELL *pActive = ActiveSpellData.Get(activespellkey);
        if (pActive != NULL)
        {
          found = true;
          pActive->CountTime += AttacksTaken;
          ActiveSpellData.Set(activespellkey, *pActive);
        }
      }
    }
    
    spellEffects.GetNext(pos);
  }

  if (GetAdjSpecAb(SA_MirrorImage))
  {
    // decrement counter used to keep track of number of mirror images
    AdjustEffectData("$CHAR_MIRRORIMAGE", -1);
  }

  if (found)
    ActiveSpellData.ProcessTimeSensitiveData(0);
}

//*****************************************************************************
//    NAME: CHARACTER::RemoveSpecAb
//
// PURPOSE: 
//
//*****************************************************************************
void CHARACTER::RemoveSpecAb(SPELL_EFFECTS_DATA &data)
{
  POSITION pos = spellEffects.GetHeadPosition();  
  while (pos != NULL)
  {
    POSITION tmp=pos;
    spellEffects.GetNext(tmp);
    // Don't remove the ones associated with an
    // active spell (parent > 0)
    //
    // Those placed here as spec abs will have
    // parent = 0. It doesn't matter which one
    // is removed. That is, there may be more
    // than one instance of the same effect
    // in this list. But this list is also
    // used for spells and their spell effects,
    // so make sure only a spec ab effect is
    // removed.

    if (spellEffects.GetAt(pos).parent<=0)
    {
      if (spellEffects.GetAt(pos).affectedAttr.CompareNoCase(data.affectedAttr)==0)
      {
        WriteDebugString("Removing special ability from %s: %s\n", 
                         GetName(),
                         spellEffects.GetAt(pos).affectedAttr);
        spellEffects.RemoveAt(pos);
        DisplayCurrSpellEffects("RemoveSpecAb");
        return;
      }
    }
    pos=tmp;
  }

  DisplayCurrSpellEffects("RemoveSpecAb");
}

//*****************************************************************************
//    NAME: CHARACTER::RemoveAllEffectsFor
//
// PURPOSE: 
//
//*****************************************************************************
void CHARACTER::RemoveAllEffectsFor(const CString &AttrName)
{
  POSITION pos = spellEffects.GetHeadPosition();  
  while (pos != NULL)
  {
    POSITION tmp=pos;
    spellEffects.GetNext(tmp);
    
    if (spellEffects.GetAt(pos).affectedAttr.CompareNoCase(AttrName)==0)
    {
      WriteDebugString("Removing (all) spell effect from %s: %s\n", 
                        GetName(),
                        AttrName);

      spellEffects.RemoveAt(pos);
    }

    pos=tmp;
  }
  DisplayCurrSpellEffects("RemoveAllEffectsFor");
}

//*****************************************************************************
//    NAME: CHARACTER::AddSpecAb
//
// PURPOSE: 
//
//*****************************************************************************
BOOL CHARACTER::AddSpecAb(SPELL_EFFECTS_DATA &data)
{
  WriteDebugString("Adding special ability to %s: %s\n", 
                   GetName(),
                   data.affectedAttr);

  spellEffects.AddTail(data);
  DisplayCurrSpellEffects("AddSpecAb");
  return TRUE;
}

//*****************************************************************************
//    NAME: CHARACTER::CalcSpellEffectChangeValue
//
// PURPOSE: 
//
//*****************************************************************************
void CHARACTER::CalcSpellEffectChangeValue(SPELL_DATA *pSdata,SPELL_EFFECTS_DATA &saData, BOOL friendlyFire)
{
  //
  // assumes that current character and target context are already set up
  //

  saData.ClearChangeResult();
  saData.GetChange(); // rolls delta value and saves for future lookup by ApplyChange()

  // Saving Throws:
  //
  //   It is assumed that no saving throw is needed when casting a spell
  //   on yourself, or on willing recipients. The party members are
  //   always assumed to be willing.
  //
  //   spellSaveVsType { ParPoiDM, PetPoly, RodStaffWand, Sp, BreathWeapon };
  //   
  //   Retrieve needed save value for target based on SaveVsType. Each type
  //   will have a single value that increases with target level. Roll 20-sided
  //   dice for target's saving throw. If roll is less than SaveVs value, no
  //   save is made and full spell affect is applied. If roll is equal to or
  //   greater than SaveVs, then SaveEffectType is used.
  //
  //   spellSaveEffectType { NoSave, SaveNegates, SaveForHalf, UseTHACO };
  //
  //   NoSave: full affect applied to target
  //   SaveNegates: no affect applied to target
  //   SaveForHalf: if numeric attribute affected, apply half, otherwise ignore
  //   UseTHACO: if saving throw less than THACO, full affect, else none

  // need to ignore this for self or party members!
  if (!friendlyFire)
  {
    int bonus = (int)saData.ExecuteSavingThrowScript();
    bool failed_st = false;

    if (pSdata->Save_Result == UseTHACO)
    {
      if (rollDice(20,1,bonus) >= GetAdjTHACO())
        saData.changeResult = 0.0;
      else
        failed_st = true;
    }
    else
    {
      if (didCharacterSaveVs(pSdata->Save_Vs, bonus))
      {
        switch (pSdata->Save_Result)
        {
        case NoSave: // full affect
          failed_st = true;
          break;
        case SaveNegates: // no affect
          saData.changeResult = 0.0;
          break;
        case SaveForHalf:
          saData.changeResult = saData.changeResult / 2.0;
          break;
        }
      }
      else
        failed_st = true;
    }

    if (failed_st)
      saData.ExecuteSavingThrowFailedScript();
    else
      saData.ExecuteSavingThrowSucceededScript();

  }
}

//*****************************************************************************
//    NAME: CHARACTER::AddSpellEffect
//
// PURPOSE: 
//
//*****************************************************************************
BOOL CHARACTER::AddSpellEffect(SPELL_EFFECTS_DATA &data)
{
  double delta=data.changeResult;

  // if not cumulative with other effects having same name
  if ((data.flags & SPELL_EFFECTS_DATA::EFFECT_CUMULATIVE) == 0)
  {
    // not cumulative, check for existing same spell effect in list
    if (GetFirstEffectAttributeMod(data.affectedAttr) != NULL)
    {
      WriteDebugString("NOT Applying spell effect (not cumulative): %s\n", 
        data.affectedAttr);
      return FALSE;
    }
  }

  if (data.flags & SPELL_EFFECTS_DATA::EFFECT_REMOVEALL)
  {
    // remove all that match
    POSITION pos = GetFirstEffectAttributeMod(data.affectedAttr);
    while (pos != NULL)
    {
      POSITION tmp = pos;
      GetNextEffectAttributeMod(data.affectedAttr, pos);

      // can't remove character special abilities, since they are intrinsic
      if ((spellEffects.GetAt(tmp).flags & SPELL_EFFECTS_DATA::EFFECT_CHARSPECAB) == 0)
      {        
        SPELL_DATA *pSpell = spellData.GetSpellData(spellEffects.GetAt(tmp).key);
        spellEffects.RemoveAt(tmp);
        
        if (pSpell!=NULL)
        {
          pSpell->specAbs.DisableAllFor(this);
    
          SetCharContext(GetContext());
          SetTargetContext(GetContext());
    
          pSpell->ExecuteSpellEndScript();
    
          ClearCharContext();
          ClearTargetContext();    
        }   
      }
    }
    DisplayCurrSpellEffects("AddSpellEffect");
    return TRUE;
  }  

  SPELL_DATA *pSdata = spellData.GetSpellData(data.key);
  if (pSdata==NULL) 
  {
    ASSERT(FALSE);
    WriteDebugString("NOT Applying spell effect (spell key bogus): %s, delta %f, cumulative %i\n", 
      data.affectedAttr, data.changeResult, (data.flags & SPELL_EFFECTS_DATA::EFFECT_CUMULATIVE));
    return FALSE;
  }

  bool isPerm = (pSdata->Duration_Rate == Instantaneous);

  int tmp = delta; // drop any fractional parts
  delta = tmp;
  data.changeResult = delta;

  if (!isPerm)
  {
    // if not permanent, store in spell effects list
    WriteDebugString("Adding non-perm spell effect to %s: %s, delta %f, cumulative %i\n", 
                     GetName(),
                     data.affectedAttr, 
                     data.changeResult, 
                     (data.flags & SPELL_EFFECTS_DATA::EFFECT_CUMULATIVE));

    spellEffects.AddTail(data);
  }
  else // else apply effects to attribute now
  {   
    // invalid attribute string
    if (data.affectedAttr.IsEmpty())
      return FALSE;

    ActorType Actor = GetContext();

    // determine which attribute is modified and
    // get delta value
    //
    double val=0.0;
    DWORD attKey = KeywordTextToNum(data.affectedAttr);
    BOOL IsVirtTrait = IsVirtualTrait(attKey);
    
    if (!IsVirtTrait)
    {
      // modify actual attribute value now
      BYTE attSize = GetDataSize(attKey);
      switch (attSize)
      {
      case IF_TYPE_STRING:
        // no spell affects for string types
        return FALSE;
        break;
      case IF_TYPE_BYTE:   val = GetDataBYTE(attKey,&Actor);   break;
      case IF_TYPE_WORD:   val = GetDataWORD(attKey,&Actor);   break;
      case IF_TYPE_DWORD:  val = GetDataDWORD(attKey,&Actor);  break;
      case IF_TYPE_FLOAT:  val = GetDataFLOAT(attKey,&Actor);  break;
      case IF_TYPE_UBYTE:  val = GetDataUBYTE(attKey,&Actor);  break;
      case IF_TYPE_UWORD:  val = GetDataUWORD(attKey,&Actor);  break;
      case IF_TYPE_UDWORD: val = GetDataUDWORD(attKey,&Actor); break;
      default: ASSERT(FALSE); return FALSE; break;
      }

      // apply delta value to affected attribute
      //val += delta;
      val = data.ApplyChange(val); // might be delta, percentage, or absolute change
      
      switch (attSize)
      {
      case IF_TYPE_BYTE:   SetDataBYTE(attKey, val,&Actor);   break;
      case IF_TYPE_WORD:   SetDataWORD(attKey, val,&Actor);   break;
      case IF_TYPE_DWORD:  SetDataDWORD(attKey, val,&Actor);  break;
      case IF_TYPE_FLOAT:  SetDataFLOAT(attKey, val,&Actor);  break;
      case IF_TYPE_UBYTE:  SetDataUBYTE(attKey, val,&Actor);  break;
      case IF_TYPE_UWORD:  SetDataUWORD(attKey, val,&Actor);  break;
      case IF_TYPE_UDWORD: SetDataUDWORD(attKey, val,&Actor); break;
      default: ASSERT(FALSE); break;
      }
  
      WriteDebugString("Applying perm spell effect to %s: %s, delta %f, result %f, cumulative %i\n", 
                       GetName(),
                       data.affectedAttr, 
                       delta, 
                       val, 
                       (data.flags&SPELL_EFFECTS_DATA::EFFECT_CUMULATIVE)?1:0);
    }
    else
    {
      // there is no CHARACTER value that represents this attribute, so
      // we need to store it in the spellEffects list
      
      // first find out if same attribute exists in list of active
      // then either apply mods to existing or add this new one
      //
      // only one such attribute should ever exist in the list of actives
      //

      POSITION pos = GetFirstEffectAttributeMod(data.affectedAttr);
      if (pos != NULL)
      {
        // get old attribute data
        SPELL_EFFECTS_DATA &olddata = spellEffects.GetAt(pos);
        // apply new modification to old mod value
        data.changeResult = data.ApplyChange(olddata.changeResult);
        // update old data
        spellEffects.GetAt(pos) = data;

        WriteDebugString("Updating perm virtual spell effect for %s: %s, delta %f, cumulative %i\n", 
                         GetName(),
                         data.affectedAttr, 
                         data.changeResult, 
                         (data.flags & SPELL_EFFECTS_DATA::EFFECT_CUMULATIVE));        
      }
      else
      {
        WriteDebugString("Adding perm virtual spell effect for %s: %s, delta %f, cumulative %i\n", 
                         GetName(),
                         data.affectedAttr, 
                         data.changeResult, 
                         (data.flags & SPELL_EFFECTS_DATA::EFFECT_CUMULATIVE));
        
        spellEffects.AddTail(data);
      }
    }

    // transfer spell attributes to character for permanent spell
    CString sakey;
    CString savalue;    
    POSITION pos = pSdata->spell_asl.GetStartPosition();
    while (pos != NULL)
    {      
      pSdata->spell_asl.GetNextAssoc(pos,sakey,savalue);
      // adds new value if doesn't exist, else updates existing
      char_asl.Insert(sakey, savalue, ASLF_MODIFIED);
    }
  }

  DisplayCurrSpellEffects("AddSpellEffect");
  return TRUE;
}

//*****************************************************************************
//    NAME: CHARACTER::ApplySpellEffectAdjustments
//
// PURPOSE: 
//
//*****************************************************************************
void CHARACTER::ApplySpellEffectAdjustments(DWORD flags, const char *akey, double &val)
{
  //AutoSetActor ctx(*this);
  POSITION pos = GetFirstEffectAttributeMod(akey);
  while (pos != NULL)
  {
    SPELL_EFFECTS_DATA &data = spellEffects.GetAt(pos);
    double src = val;
    // GetChange() should have been called at least once before now
    val = data.ApplyChange(src); // return accumulated delta
    //WriteDebugString("ApplySpellEffectAdj(%f) = %f for %s\n", src, val, akey);    
    pos = GetNextEffectAttributeMod(akey, pos);
  }
}

//*****************************************************************************
//    NAME: CHARACTER::AdjustEffectData
//
// PURPOSE: 
//
//*****************************************************************************
void CHARACTER::AdjustEffectData(const char *akey, int val)
{
  // adjust only the first effect that matches
  POSITION pos = GetFirstEffectAttributeMod(akey);
  if (pos == NULL)
    return;

  WriteDebugString("Adjusting data for effect %s by %i for %i\n", akey, val, GetUniqueId());

  SPELL_EFFECTS_DATA &data = spellEffects.GetAt(pos);

  data.data += val;

  bool RemoveSpell = false;
  specialAbilitiesType sa = ConvertRuntimeIfTextToSpecAb(akey);

  switch (sa)
  {
  case SA_MirrorImage:
    // check image count, if reduced to zero
    // then this sa has no more benefit
    RemoveSpell = (data.data == 0);
    break;
  }

  if (RemoveSpell)
  {    
    ACTIVE_SPELL *pSpell = ActiveSpellData.Get(data.parent);
    if (pSpell != NULL)
    {
      // if this is the only effect for this spell, might as well expire it
      // otherwise, wait for normal spell duration
      SPELL_DATA *pSpellData = spellData.GetSpellData(pSpell->spellKey);
      if (pSpellData != NULL)
      {
        WriteDebugString("Marking spell %s from %i in AdjustEffectData() for removal\n", pSpellData->Name, GetUniqueId());
        if (pSpellData->m_EffectedAttributes.GetCount() == 1)
          pSpell->StopTime = 0; // force spell to expire
      }
      else
      {
        pSpell->StopTime = 0; // force spell to expire
        WriteDebugString("Marking spell from %i in AdjustEffectData() for removal\n", GetUniqueId());
      }
    }
  }
}

#endif // engine-only magic code

//*****************************************************************************
//    NAME: CHARACTER::GetAdjTHACO
//
// PURPOSE: 
//
//*****************************************************************************
int CHARACTER::GetAdjTHACO(DWORD flags)
{  
#ifdef UAFEDITOR
  
  int tmp = GetTHACO();
  tmp -= GetAdjHitBonus(); // subtract strength bonus from base THACO

  int wpn = myItems.GetReady(WeaponHand);
  if (wpn != NO_READY_ITEM)
  {  
    int index = myItems.GetItem(wpn);
    ITEM_DATA *pData = itemData.GetItemData(index);
    if (pData != NULL)
      tmp -= pData->Attack_Bonus; // subtract weapon attack bonus
  }

  return tmp;

#else

  const char akey[]="$CHAR_THACO";
  
  double val = GetTHACO();
  val -= GetAdjHitBonus(); // subtract strength bonus from base THACO

  int wpn = myItems.GetReady(WeaponHand);
  if (wpn != NO_READY_ITEM)
  {  
    int index = myItems.GetItem(wpn);
    ITEM_DATA *pData = itemData.GetItemData(index);
    if (pData != NULL)
      val -= pData->Attack_Bonus; // subtract weapon attack bonus
  }

  ApplySpellEffectAdjustments(flags, akey, val);
  val = min(20,val);
  val = max(1,val);
  return val;

#endif
}

//*****************************************************************************
//    NAME: CHARACTER::GetAdjAC
//
// PURPOSE: 
//
//*****************************************************************************
int CHARACTER::GetAdjAC(DWORD flags)
{
#ifdef UAFEDITOR
  return GetAC();
#else
  const char akey[]="$CHAR_AC";
  double val = GetAC();
  ApplySpellEffectAdjustments(flags, akey, val);
  val = min(10,val);
  val = max(-10,val);
  return val;
#endif
}

//*****************************************************************************
//    NAME: CHARACTER::GetAdjHitPoints
//
// PURPOSE: 
//
//*****************************************************************************
int CHARACTER::GetAdjHitPoints(DWORD flags)
{
#ifdef UAFEDITOR
  return GetHitPoints();
#else
  const char akey[]="$CHAR_HITPOINTS";
  double val = GetHitPoints();
  ApplySpellEffectAdjustments(flags, akey, val);
  val = max(-10,val);
  val = min(val, GetMaxHitPoints());
  return val;
#endif
}

//*****************************************************************************
//    NAME: CHARACTER::GetAdjReadyToTrain
//
// PURPOSE: 
//
//*****************************************************************************
BOOL CHARACTER::GetAdjReadyToTrain(DWORD flags)
{
#ifdef UAFEDITOR
  return GetReadyToTrain();
#else
  const char akey[]="$CHAR_RDYTOTRAIN";
  double val = GetReadyToTrain();
  ApplySpellEffectAdjustments(flags, akey, val);
  val = max(FALSE,val);
  val = min(val,TRUE);
  return val;
#endif
}

//*****************************************************************************
//    NAME: CHARACTER::GetAdjClericExp
//
// PURPOSE: 
//
//*****************************************************************************
int CHARACTER::GetAdjClericExp(DWORD flags)
{
#ifdef UAFEDITOR
  return GetClericExp();
#else
  const char akey[]="$CHAR_CLERICEXP";
  double val = GetClericExp();
  ApplySpellEffectAdjustments(flags, akey, val);
  val = max(0,val);
  return val;
#endif
}
//*****************************************************************************
//    NAME: CHARACTER::GetAdjFighterExp
//
// PURPOSE: 
//
//*****************************************************************************
int CHARACTER::GetAdjFighterExp(DWORD flags)
{
#ifdef UAFEDITOR
  return GetFighterExp();
#else
  const char akey[]="$CHAR_FIGHTEREXP";
  double val = GetFighterExp();
  ApplySpellEffectAdjustments(flags, akey, val);
  val = max(0,val);
  return val;
#endif
}
//*****************************************************************************
//    NAME: CHARACTER::GetAdjMagicUserExp
//
// PURPOSE: 
//
//*****************************************************************************
int CHARACTER::GetAdjMagicUserExp(DWORD flags)
{
#ifdef UAFEDITOR
  return GetMagicUserExp();
#else
  const char akey[]="$CHAR_MAGICUSEREXP";
  double val = GetMagicUserExp();
  ApplySpellEffectAdjustments(flags, akey, val);
  val = max(0,val);
  return val;
#endif
}
//*****************************************************************************
//    NAME: CHARACTER::GetAdjThiefExp
//
// PURPOSE: 
//
//*****************************************************************************
int CHARACTER::GetAdjThiefExp(DWORD flags)
{
#ifdef UAFEDITOR
  return GetThiefExp();
#else
  const char akey[]="$CHAR_THIEFEXP";
  double val = GetThiefExp();
  ApplySpellEffectAdjustments(flags, akey, val);
  val = max(0,val);
  return val;
#endif
}
//*****************************************************************************
//    NAME: CHARACTER::GetAdjRangerExp
//
// PURPOSE: 
//
//*****************************************************************************
int CHARACTER::GetAdjRangerExp(DWORD flags)
{
#ifdef UAFEDITOR
  return GetRangerExp();
#else
  const char akey[]="$CHAR_RANGEREXP";
  double val = GetRangerExp();
  ApplySpellEffectAdjustments(flags, akey, val);
  val = max(0,val);
  return val;
#endif
}
//*****************************************************************************
//    NAME: CHARACTER::GetAdjPaladinExp
//
// PURPOSE: 
//
//*****************************************************************************
int CHARACTER::GetAdjPaladinExp(DWORD flags)
{
#ifdef UAFEDITOR
  return GetPaladinExp();
#else
  const char akey[]="$CHAR_PALADINEXP";
  double val = GetPaladinExp();
  ApplySpellEffectAdjustments(flags, akey, val);
  val = max(0,val);
  return val;
#endif
}
//*****************************************************************************
//    NAME: CHARACTER::GetAdjDruidExp
//
// PURPOSE: 
//
//*****************************************************************************
int CHARACTER::GetAdjDruidExp(DWORD flags)
{
#ifdef UAFEDITOR
  return GetDruidExp();
#else
  const char akey[]="$CHAR_DRUIDEXP";
  double val = GetDruidExp();
  ApplySpellEffectAdjustments(flags, akey, val);
  val = max(0,val);
  return val;
#endif
}

//*****************************************************************************
//    NAME: CHARACTER::GetAdjAge
//
// PURPOSE: 
//
//*****************************************************************************
int CHARACTER::GetAdjAge(DWORD flags)
{
#ifdef UAFEDITOR
  return GetAge();
#else
  const char akey[]="$CHAR_AGE";
  double val = GetAge();
  ApplySpellEffectAdjustments(flags, akey, val);
  val = max(1,val);
  return val;
#endif
}
//*****************************************************************************
//    NAME: CHARACTER::GetAdjMaxAge
//
// PURPOSE: 
//
//*****************************************************************************
int CHARACTER::GetAdjMaxAge(DWORD flags)
{
#ifdef UAFEDITOR
  return GetMaxAge();
#else
  const char akey[]="$CHAR_MAXAGE";
  double val = GetMaxAge();
  ApplySpellEffectAdjustments(flags, akey, val);
  val = max(1,val);
  return val;
#endif
}

//*****************************************************************************
//    NAME: CHARACTER::GetAdjMaxMovement
//
// PURPOSE: 
//
//*****************************************************************************
BYTE CHARACTER::GetAdjMaxMovement(DWORD flags)
{
#ifdef UAFEDITOR
  return GetMaxMovement();
#else
  
  if (IsCombatActive())
  {
    DWORD src;
    DWORD skey;
    
    if (GetAdjSpecAb(SA_Entangle,&src,&skey))
    {
      QueueUsedSpecAb(SA_Entangle,src,skey);
      return 0;
    }

    if (GetAdjSpecAb(SA_HoldPerson,&src,&skey))
    {
      QueueUsedSpecAb(SA_HoldPerson,src,skey);
      return 0;
    }

    if (GetAdjSpecAb(SA_Sleep,&src,&skey))
    {
      QueueUsedSpecAb(SA_Sleep,src,skey);
      return 0;
    }
    
    const char akey[]="$CHAR_MAXMOVE";
    double val = GetMaxMovement();
    ApplySpellEffectAdjustments(flags, akey, val);
    val = max(0,val);
    return val;    
  }
  else
  {
    const char akey[]="$CHAR_MAXMOVE";
    double val = GetMaxMovement();
    ApplySpellEffectAdjustments(flags, akey, val);
    val = max(0,val);
    return val;
  }
#endif
}
//*****************************************************************************
//    NAME: CHARACTER::GetAdjStr
//
// PURPOSE: 
//
//*****************************************************************************
BYTE CHARACTER::GetAdjStr(DWORD flags)
{
#ifdef UAFEDITOR
  return GetStr();
#else
  if (GetAdjSpecAb(SA_Enfeebled))
  {
    return (min(GetStr(), 5));
  }
  else
  {
    const char akey[]="$CHAR_STR";
    double val = GetStr();
    ApplySpellEffectAdjustments(flags, akey, val);
    val = max(1,val);
    return val;
  }
#endif
}
//*****************************************************************************
//    NAME: CHARACTER::GetAdjStrMod
//
// PURPOSE: 
//
//*****************************************************************************
BYTE CHARACTER::GetAdjStrMod(DWORD flags)
{
#ifdef UAFEDITOR
  return GetStrMod();
#else
  const char akey[]="$CHAR_STRMOD";
  double val = GetStrMod();
  ApplySpellEffectAdjustments(flags, akey, val);
  val = max(0,val);
  return val;
#endif
}
//*****************************************************************************
//    NAME: CHARACTER::GetAdjInt
//
// PURPOSE: 
//
//*****************************************************************************
BYTE CHARACTER::GetAdjInt(DWORD flags)
{
#ifdef UAFEDITOR
  return GetInt();
#else
  const char akey[]="$CHAR_INT";
  double val = GetInt();
  ApplySpellEffectAdjustments(flags, akey, val);
  val = max(1,val);
  return val;
#endif
}
//*****************************************************************************
//    NAME: CHARACTER::GetAdjWis
//
// PURPOSE: 
//
//*****************************************************************************
BYTE CHARACTER::GetAdjWis(DWORD flags)
{
#ifdef UAFEDITOR
  return GetWis();
#else
  const char akey[]="$CHAR_WIS";
  double val = GetWis();
  ApplySpellEffectAdjustments(flags, akey, val);
  val = max(1,val);
  return val;
#endif
}
//*****************************************************************************
//    NAME: CHARACTER::GetAdjDex
//
// PURPOSE: 
//
//*****************************************************************************
BYTE CHARACTER::GetAdjDex(DWORD flags)
{
#ifdef UAFEDITOR
  return GetDex();
#else
  const char akey[]="$CHAR_DEX";
  double val = GetDex();
  ApplySpellEffectAdjustments(flags, akey, val);
  val = max(1,val);
  return val;
#endif
}
//*****************************************************************************
//    NAME: CHARACTER::GetAdjCon
//
// PURPOSE: 
//
//*****************************************************************************
BYTE CHARACTER::GetAdjCon(DWORD flags)
{
#ifdef UAFEDITOR
  return GetCon();
#else
  const char akey[]="$CHAR_CON";
  double val = GetCon();
  ApplySpellEffectAdjustments(flags, akey, val);
  val = max(1,val);
  return val;
#endif
}
//*****************************************************************************
//    NAME: CHARACTER::GetAdjCha
//
// PURPOSE: 
//
//*****************************************************************************
BYTE CHARACTER::GetAdjCha(DWORD flags)
{
#ifdef UAFEDITOR
  return GetCha();
#else
  const char akey[]="$CHAR_CHA";
  double val = GetCha();
  ApplySpellEffectAdjustments(flags, akey, val);
  val = max(1,val);
  return val;
#endif
}
//*****************************************************************************
//    NAME: CHARACTER::GetAdjMaxEncumbrance
//
// PURPOSE: 
//
//*****************************************************************************
int CHARACTER::GetAdjMaxEncumbrance(DWORD flags)
{
#ifdef UAFEDITOR
  return GetMaxEncumbrance();
#else
  const char akey[]="$CHAR_MAXENC";
  double val = GetMaxEncumbrance();
  ApplySpellEffectAdjustments(flags, akey, val);
  val = max(1,val);
  return val;
#endif
}

//*****************************************************************************
//    NAME: CHARACTER::GetAdjGender
//
// PURPOSE: 
//
//*****************************************************************************
genderType CHARACTER::GetAdjGender(DWORD flags)
{
#ifdef UAFEDITOR
  return GetGender();
#else
  const char akey[]="$CHAR_GENDER";
  double val = (double)((int)GetGender());
  ApplySpellEffectAdjustments(flags, akey, val);
  if ((val < 0)||(val >= NUM_GENDER_TYPES))
    val = (double)((int)GetGender());
  return ((genderType)((int)val));
#endif
}
//*****************************************************************************
//    NAME: CHARACTER::GetAdjClass
//
// PURPOSE: 
//
//*****************************************************************************
classType CHARACTER::GetAdjClass(DWORD flags)
{
#ifdef UAFEDITOR
  return GetClass();
#else
  const char akey[]="$CHAR_CLASS";
  double val = (double)((int)GetClass());
  ApplySpellEffectAdjustments(flags, akey, val);
  if ((val < 0)||(val >= NUM_CLASS_TYPES))
    val = (double)((int)GetClass());
  return ((classType)((int)val));
#endif
}
//*****************************************************************************
//    NAME: CHARACTER::GetAdjAlignment
//
// PURPOSE: 
//
//*****************************************************************************
alignmentType CHARACTER::GetAdjAlignment(DWORD flags)
{
#ifdef UAFEDITOR
  return GetAlignment();
#else
  const char akey[]="$CHAR_ALIGNMENT";
  double val = (double)((int)GetAlignment());
  ApplySpellEffectAdjustments(flags, akey, val);
  if ((val < 0)||(val >= NUM_ALIGNMENT_TYPES))
    val = (double)((int)GetAlignment());
  return ((alignmentType)((int)val));
#endif
}

//*****************************************************************************
//    NAME: CHARACTER::IsAlignmentEvil
//
// PURPOSE: 
//
//*****************************************************************************
BOOL CHARACTER::IsAlignmentEvil()
{
  switch (GetAdjAlignment())
  {
  case LawfulEvil:
  case NeutralEvil:
  case ChaoticEvil:
    return TRUE;
  }
  return FALSE;
}

//*****************************************************************************
//    NAME: CHARACTER::IsAlignmentGood
//
// PURPOSE: 
//
//*****************************************************************************
BOOL CHARACTER::IsAlignmentGood()
{
  switch (GetAdjAlignment())
  {
  case LawfulGood:
  case NeutralGood:
  case ChaoticGood:
    return TRUE;
  }
  return FALSE;
}

//*****************************************************************************
//    NAME: CHARACTER::GetAdjStatus
//
// PURPOSE: 
//
//*****************************************************************************
charStatusType CHARACTER::GetAdjStatus(DWORD flags)
{
#ifdef UAFEDITOR
  return GetStatus();
#else
  const char akey[]="$CHAR_STATUS";
  double val = (double)((int)GetStatus());
  ApplySpellEffectAdjustments(flags, akey, val);
  if ((val < 0)||(val >= NUM_CHAR_STATUS_TYPES))
    val = (double)((int)GetStatus());
  return ((charStatusType)((int)val));
#endif
}
//*****************************************************************************
//    NAME: CHARACTER::GetAdjUndeadType
//
// PURPOSE: 
//
//*****************************************************************************
undeadType CHARACTER::GetAdjUndeadType(DWORD flags)
{
#ifdef UAFEDITOR
  return GetUndeadType();
#else
  const char akey[]="$CHAR_UNDEAD";
  double val = (double)((int)GetUndeadType());
  ApplySpellEffectAdjustments(flags, akey, val);
  if ((val < 0)||(val >= NUM_UNDEAD_TYPES))
    val = (double)((int)GetUndeadType());
  return ((undeadType)((int)val));
#endif
}
//*****************************************************************************
//    NAME: CHARACTER::GetAdjSize
//
// PURPOSE: 
//
//*****************************************************************************
creatureSizeType CHARACTER::GetAdjSize(DWORD flags)
{
#ifdef UAFEDITOR
  return GetSize();
#else
  const char akey[]="$CHAR_SIZE";
  double val = (double)((int)GetSize());
  ApplySpellEffectAdjustments(flags, akey, val);
  if ((val < 0)||(val >= NUM_SIZE_TYPES))
    val = (double)((int)GetSize());
  // lookup sa's Enlarge and Reduce
  //enum creatureSizeType { Small, Medium, Large };
  if (GetAdjSpecAb(SA_Enlarge))
    val++;
  if (GetAdjSpecAb(SA_Reduce))
    val--;

  if (val < Small) val=Small;
  if (val > Large) val=Large;

  return ((creatureSizeType)((int)val));
#endif
}

//*****************************************************************************
//    NAME: CHARACTER::GetAdjMagicResistance
//
// PURPOSE: 
//
//*****************************************************************************
int CHARACTER::GetAdjMagicResistance(DWORD flags)
{
#ifdef UAFEDITOR
  return GetMagicResistance();
#else
  const char akey[]="$CHAR_MAGICRESIST";
  double val = GetMagicResistance();
  ApplySpellEffectAdjustments(flags, akey, val);
  val = max(0,val);
  val = min(100,val);
  return val;
#endif
}
//*****************************************************************************
//    NAME: CHARACTER::GetAdjSaveVsPPDM
//
// PURPOSE: 
//
//*****************************************************************************
int CHARACTER::GetAdjSaveVsPPDM(DWORD flags)
{
#ifdef UAFEDITOR
  return GetSaveVsPPDM();
#else
  const char akey[]="$CHAR_SAVEVSPPDM";
  double val = GetSaveVsPPDM();
  ApplySpellEffectAdjustments(flags, akey, val);
  val = max(1,val);
  val = min(20,val);
  return val;
#endif
}
//*****************************************************************************
//    NAME: CHARACTER::GetAdjSaveVsPP
//
// PURPOSE: 
//
//*****************************************************************************
int CHARACTER::GetAdjSaveVsPP(DWORD flags)
{
#ifdef UAFEDITOR
  return GetSaveVsPP();
#else
  const char akey[]="$CHAR_SAVEVSPP";
  double val = GetSaveVsPP();
  ApplySpellEffectAdjustments(flags, akey, val);
  val = max(1,val);
  val = min(20,val);
  return val;
#endif
}
//*****************************************************************************
//    NAME: CHARACTER::GetAdjSaveVsRSW
//
// PURPOSE: 
//
//*****************************************************************************
int CHARACTER::GetAdjSaveVsRSW(DWORD flags)
{
#ifdef UAFEDITOR
  return GetSaveVsRSW();
#else
  const char akey[]="$CHAR_SAVEVSRSW";
  double val = GetSaveVsRSW();
  ApplySpellEffectAdjustments(flags, akey, val);
  val = max(1,val);
  val = min(20,val);
  return val;
#endif
}
//*****************************************************************************
//    NAME: CHARACTER::GetAdjSaveVsBr
//
// PURPOSE: 
//
//*****************************************************************************
int CHARACTER::GetAdjSaveVsBr(DWORD flags)
{
#ifdef UAFEDITOR
  return GetSaveVsBr();
#else
  const char akey[]="$CHAR_SAVEVSBR";
  double val = GetSaveVsBr();
  ApplySpellEffectAdjustments(flags, akey, val);
  val = max(1,val);
  val = min(20,val);
  return val;
#endif
}
//*****************************************************************************
//    NAME: CHARACTER::GetAdjSaveVsSp
//
// PURPOSE: 
//
//*****************************************************************************
int CHARACTER::GetAdjSaveVsSp(DWORD flags)
{
#ifdef UAFEDITOR
  return GetSaveVsSp();
#else
  const char akey[]="$CHAR_SAVEVSSP";
  double val = GetSaveVsSp();
  ApplySpellEffectAdjustments(flags, akey, val);
  val = max(1,val);
  val = min(20,val);
  return val;
#endif
}
//*****************************************************************************
//    NAME: CHARACTER::GetAdjHitBonus
//
// PURPOSE: 
//
//*****************************************************************************
int CHARACTER::GetAdjHitBonus(DWORD flags)
{
#ifdef UAFEDITOR
  return GetHitBonus();
#else
  const char akey[]="$CHAR_HITBONUS";
  double val = GetHitBonus();
  ApplySpellEffectAdjustments(flags, akey, val);
  return val;
#endif
}
//*****************************************************************************
//    NAME: CHARACTER::GetAdjDmgBonus
//
// PURPOSE: 
//
//*****************************************************************************
int CHARACTER::GetAdjDmgBonus(DWORD flags)
{
#ifdef UAFEDITOR
  return GetDmgBonus();
#else
  const char akey[]="$CHAR_DAMAGEBONUS";
  double val = GetDmgBonus();
  ApplySpellEffectAdjustments(flags, akey, val);
  return val;
#endif
}

//*****************************************************************************
//    NAME: CHARACTER::GetAdjMorale
//
// PURPOSE: 
//
//*****************************************************************************
int CHARACTER::GetAdjMorale(DWORD flags)
{
#ifdef UAFEDITOR
  return GetMorale();
#else
  const char akey[]="$CHAR_MORALE";
  double val = GetMorale();
  ApplySpellEffectAdjustments(flags, akey, val);
  val = max(0, val);
  val = min(100, val);
  return val;
#endif
}
//*****************************************************************************
//    NAME: CHARACTER::GetAdjOpenDoors
//
// PURPOSE: 
//
//*****************************************************************************
BYTE CHARACTER::GetAdjOpenDoors(DWORD flags)
{
#ifdef UAFEDITOR
  return GetOpenDoors();
#else
  const char akey[]="$CHAR_OPENDOORS";
  double val = GetOpenDoors();
  ApplySpellEffectAdjustments(flags, akey, val);
  val = max(0, val);
  val = min(100, val);
  return val;
#endif
}
//*****************************************************************************
//    NAME: CHARACTER::GetAdjOpenMagicDoors
//
// PURPOSE: 
//
//*****************************************************************************
BYTE CHARACTER::GetAdjOpenMagicDoors(DWORD flags)
{
#ifdef UAFEDITOR
  return GetOpenMagicDoors();
#else
  const char akey[]="$CHAR_OPENMAGICDOORS";
  double val = GetOpenMagicDoors();
  ApplySpellEffectAdjustments(flags, akey, val);
  val = max(0, val);
  val = min(100, val);
  return val;
#endif
}
//*****************************************************************************
//    NAME: CHARACTER::GetAdjBendBarsLiftGates
//
// PURPOSE: 
//
//*****************************************************************************
BYTE CHARACTER::GetAdjBendBarsLiftGates(DWORD flags)
{
#ifdef UAFEDITOR
  return GetBendBarsLiftGates();
#else
  const char akey[]="$CHAR_BENDLIFT";
  double val = GetBendBarsLiftGates();
  ApplySpellEffectAdjustments(flags, akey, val);
  val = max(0, val);
  val = min(100, val);
  return val;
#endif
}
//*****************************************************************************
//    NAME: CHARACTER::GetAdjPickPockets
//
// PURPOSE: 
//
//*****************************************************************************
int CHARACTER::GetAdjPickPockets(DWORD flags)
{
#ifdef UAFEDITOR
  return GetPickPockets();
#else
  const char akey[]="$CHAR_PICKPOCKETS";
  double val = GetPickPockets();
  ApplySpellEffectAdjustments(flags, akey, val);
  val = max(-100, val);
  val = min(100, val);
  return val;
#endif
}
//*****************************************************************************
//    NAME: CHARACTER::GetAdjOpenLocks
//
// PURPOSE: 
//
//*****************************************************************************
int CHARACTER::GetAdjOpenLocks(DWORD flags)
{
#ifdef UAFEDITOR
  return GetOpenLocks();
#else
  const char akey[]="$CHAR_OPENLOCKS";
  double val = GetOpenLocks();
  ApplySpellEffectAdjustments(flags, akey, val);
  val = max(-100, val);
  val = min(100, val);
  return val;
#endif
}
//*****************************************************************************
//    NAME: CHARACTER::GetAdjFindTraps
//
// PURPOSE: 
//
//*****************************************************************************
int CHARACTER::GetAdjFindTraps(DWORD flags)
{
#ifdef UAFEDITOR
  return GetFindTraps();
#else
  const char akey[]="$CHAR_FINDTRAPS";
  double val = GetFindTraps();
  ApplySpellEffectAdjustments(flags, akey, val);
  val = max(-100, val);
  val = min(100, val);
  return val;
#endif
}
//*****************************************************************************
//    NAME: CHARACTER::GetAdjMoveSilent
//
// PURPOSE: 
//
//*****************************************************************************
int CHARACTER::GetAdjMoveSilent(DWORD flags)
{
#ifdef UAFEDITOR
  return GetMoveSilent();
#else
  const char akey[]="$CHAR_MOVESILENT";
  double val = GetMoveSilent();
  ApplySpellEffectAdjustments(flags, akey, val);
  val = max(-100, val);
  val = min(100, val);
  return val;
#endif
}
//*****************************************************************************
//    NAME: CHARACTER::GetAdjHideInShadows
//
// PURPOSE: 
//
//*****************************************************************************
int CHARACTER::GetAdjHideInShadows(DWORD flags)
{
#ifdef UAFEDITOR
  return GetHideInShadows();
#else
  const char akey[]="$CHAR_HIDESHADOWS";
  double val = GetHideInShadows();
  ApplySpellEffectAdjustments(flags, akey, val);
  val = max(-100, val);
  val = min(100, val);
  return val;
#endif
}
//*****************************************************************************
//    NAME: CHARACTER::GetAdjHearNoise
//
// PURPOSE: 
//
//*****************************************************************************
int CHARACTER::GetAdjHearNoise(DWORD flags)
{
#ifdef UAFEDITOR
  return GetHearNoise();
#else
  const char akey[]="$CHAR_HEARNOISE";
  double val = GetHearNoise();
  ApplySpellEffectAdjustments(flags, akey, val);
  val = max(-100, val);
  val = min(100, val);
  return val;
#endif
}
//*****************************************************************************
//    NAME: CHARACTER::GetAdjClimbWalls
//
// PURPOSE: 
//
//*****************************************************************************
int CHARACTER::GetAdjClimbWalls(DWORD flags)
{
#ifdef UAFEDITOR
  return GetClimbWalls();
#else
  const char akey[]="$CHAR_CLIMBWALLS";
  double val = GetClimbWalls();
  ApplySpellEffectAdjustments(flags, akey, val);
  val = max(-100, val);
  val = min(100, val);
  return val;
#endif
}
//*****************************************************************************
//    NAME: CHARACTER::GetAdjReadLanguages
//
// PURPOSE: 
//
//*****************************************************************************
int CHARACTER::GetAdjReadLanguages(DWORD flags)
{
#ifdef UAFEDITOR
  return GetReadLanguages();
#else
  const char akey[]="$CHAR_READLANG";
  double val = GetReadLanguages();
  ApplySpellEffectAdjustments(flags, akey, val);
  val = max(-100, val);
  val = min(100, val);
  return val;
#endif
}
//*****************************************************************************
//    NAME: CHARACTER::GetAdjAllowPlayerControl
//
// PURPOSE: 
//
//*****************************************************************************
BOOL CHARACTER::GetAdjAllowPlayerControl(DWORD flags)
{
#ifdef UAFEDITOR
  return GetAllowPlayerControl();
#else
  const char akey[]="$CHAR_ALLOWPLAYERCONTROL";
  double val = GetAllowPlayerControl();
  ApplySpellEffectAdjustments(flags, akey, val);
  val = max(0, val);
  val = min(1, val);
  return val;
#endif
}
//*****************************************************************************
//    NAME: CHARACTER::GetAdjAutomatic
//
// PURPOSE: 
//
//*****************************************************************************
BOOL CHARACTER::GetAdjAutomatic(DWORD flags)
{
  // if player control not allowed, then automatic is TRUE
  if (!GetAdjAllowPlayerControl(flags)) return TRUE;
  return GetAutomatic();
}
//*****************************************************************************
//    NAME: CHARACTER::GetAdjDetectingInvisible
//
// PURPOSE: 
//
//*****************************************************************************
BOOL CHARACTER::GetAdjDetectingInvisible(DWORD flags)
{
#ifdef UAFEDITOR
  return GetDetectingInvisible();
#else
  const char akey[]="$CHAR_DETECTINVISIBLE";
  double val = GetDetectingInvisible();
  ApplySpellEffectAdjustments(flags, akey, val);
  val = max(0, val);
  val = min(1, val);
  return val;
#endif
}
//*****************************************************************************
//    NAME: CHARACTER::GetAdjDetectingTraps
//
// PURPOSE: 
//
//*****************************************************************************
BOOL CHARACTER::GetAdjDetectingTraps(DWORD flags)
{
#ifdef UAFEDITOR
  return GetDetectingTraps();
#else
  const char akey[]="$CHAR_DETECTTRAPS";
  double val = GetDetectingTraps();
  ApplySpellEffectAdjustments(flags, akey, val);
  val = max(0, val);
  val = min(1, val);
  return val;
#endif
}

//*****************************************************************************
//    NAME: CHARACTER::GetAdjSpecAb
//
// PURPOSE: 
//
//*****************************************************************************
int CHARACTER::GetAdjSpecAb(int sa, DWORD *pSource, DWORD *pSrcKey)
{
  if (pSource!=NULL) *pSource=0;
  if (pSrcKey!=NULL) *pSrcKey=0xFFFFFFFF;

  if ((sa<SA_Bless)||(sa>=NUM_SPECIAL_ABILITIES))
  {
    ASSERT(FALSE);
    return 0;
  }

#ifdef UAFEDITOR
  return 0;
#else
  CString akey=ConvertSpecAbToRuntimeIfText((specialAbilitiesType)sa);
  if (akey.GetLength()==0) return 0;

  double val = 0.0;
  POSITION pos = GetFirstEffectAttributeMod(akey);
  if (pos == NULL)
    return 0;

  SPELL_EFFECTS_DATA &data = spellEffects.GetAt(pos);
  val = data.ApplyChange(val);

  if (pSource!=NULL) *pSource = data.flags & SPELL_EFFECTS_DATA::ALLSOURCE_MASK;
  if (pSrcKey!=NULL) *pSrcKey=data.key;

  val = max(0, val);
  val = min(1, val);
  return val;
#endif
}

//*****************************************************************************
//    NAME: CHARACTER::SetSpecAb
//
// PURPOSE: 
//
//*****************************************************************************
void CHARACTER::SetSpecAb(int sa, BYTE enable, DWORD flags)
{
  // flags should specify the source of this ability (item, char/monster, spell)

  if ((sa<SA_Bless)||(sa>=NUM_SPECIAL_ABILITIES))
  {
    ASSERT(FALSE);
    return;
  }

  switch (sa)
  {
  case SA_Poisoned:
    if (HasPoisonImmunity()) return;
    break;
  case SA_HoldPerson:
  case SA_CharmPerson:
    if (!CanBeHeldOrCharmed()) return;
    break;
  }

  // cannot stack special abilities
  // if already have this one, just return
  if (GetAdjSpecAb(sa))
    return;

#ifdef UAFEDITOR
  return;
#else
  CString akey=ConvertSpecAbToRuntimeIfText((specialAbilitiesType)sa);
  if (akey.GetLength()==0) return;

  SPELL_EFFECTS_DATA data;
  data.Clear();
  data.affectedAttr = akey;
  data.changeData.DecodeText("1");
  data.GetChange();
  data.flags = flags | 
               SPELL_EFFECTS_DATA::EFFECT_CUMULATIVE |
               SPELL_EFFECTS_DATA::EFFECT_TARGET |
               SPELL_EFFECTS_DATA::EFFECT_ABS;

  switch (sa)
  {
  case SA_MirrorImage:
    data.data = rollDice(4, 1, 0);
    data.data += (GetCurrentLevel(MagicUserFlag) / 3);
    if (data.data > 8) data.data = 8;
    WriteDebugString("Setting num images for MirrorImage to %i\n", data.data);
    break;
  }

  if (enable)
    AddSpecAb(data);
  else
    RemoveSpecAb(data);
#endif
}

//*****************************************************************************
//    NAME: CHARACTER::SetHitBonus
//
// PURPOSE: 
//
//*****************************************************************************
void CHARACTER::SetHitBonus(int val)
{
  hitBonus=val;
}
//*****************************************************************************
//    NAME: CHARACTER::SetDmgBonus
//
// PURPOSE: 
//
//*****************************************************************************
void CHARACTER::SetDmgBonus(int val)
{
  dmgBonus=val;
}
//*****************************************************************************
//    NAME: CHARACTER::SetName
//
// PURPOSE: 
//
//*****************************************************************************
void CHARACTER::SetName(const char *val) 
{     
  name = val; 
  name.TrimLeft();
  name.TrimRight();
  if (name.GetLength() > MAX_CHAR_NAME)
    name = name.Left(MAX_CHAR_NAME);
}
//*****************************************************************************
//    NAME: CHARACTER::SetMagicResistance
//
// PURPOSE: 
//
//*****************************************************************************
void CHARACTER::SetMagicResistance(int val)
{
  if (GetType() != MONSTER_TYPE) val=0;
  val = max(0,val);
  val = min(100,val);
  magicResistance=val;
}

//*****************************************************************************
//    NAME: CHARACTER::SetSaveVsPPDM
//
// PURPOSE: 
//
//*****************************************************************************
void CHARACTER::SetSaveVsPPDM(int val)
{
  val = max(1,val);
  val = min(20,val);
  saveVsParPsnDMag = val;
}
//*****************************************************************************
//    NAME: CHARACTER::SetSaveVsPP
//
// PURPOSE: 
//
//*****************************************************************************
void CHARACTER::SetSaveVsPP(int val)
{
  val = max(1,val);
  val = min(20,val);
  saveVsPetPoly = val;
}
//*****************************************************************************
//    NAME: CHARACTER::SetSaveVsRSW
//
// PURPOSE: 
//
//*****************************************************************************
void CHARACTER::SetSaveVsRSW(int val)
{
  val = max(1,val);
  val = min(20,val);
  saveVsRdStWnd = val;
}
//*****************************************************************************
//    NAME: CHARACTER::SetSaveVsBr
//
// PURPOSE: 
//
//*****************************************************************************
void CHARACTER::SetSaveVsBr(int val)
{
  val = max(1,val);
  val = min(20,val);
  saveVsBreath = val;
}
//*****************************************************************************
//    NAME: CHARACTER::SetSaveVsSp
//
// PURPOSE: 
//
//*****************************************************************************
void CHARACTER::SetSaveVsSp(int val)
{
  val = max(1,val);
  val = min(20,val);
  saveVsSpell = val;
}

//*****************************************************************************
//    NAME: CHARACTER::SetStr
//
// PURPOSE: 
//
//*****************************************************************************
void CHARACTER::SetStr(BYTE val)
{
  val = min(25, val);
  val = max(3, val);
  strength = val;
  if (strength<18) SetStrMod(0);
  UpdateSkillBasedStats(); // spells can change skill stats, so update this data
}
//*****************************************************************************
//    NAME: CHARACTER::SetStrMod
//
// PURPOSE: 
//
//*****************************************************************************
void CHARACTER::SetStrMod(BYTE val)
{
  if ((!isFighter(GetClass())) && (!isRanger(GetClass())) && (!isPaladin(GetClass())))
    val=0;
  val = min(100, val);
  val = max(0, val);
  strengthMod = val;
  UpdateSkillBasedStats();
}
//*****************************************************************************
//    NAME: CHARACTER::SetInt
//
// PURPOSE: 
//
//*****************************************************************************
void CHARACTER::SetInt(BYTE val)
{
  val = min(25, val);
  val = max(3, val);
  intelligence = val;
  UpdateSkillBasedStats();
}
//*****************************************************************************
//    NAME: CHARACTER::SetWis
//
// PURPOSE: 
//
//*****************************************************************************
void CHARACTER::SetWis(BYTE val)
{
  val = min(25, val);
  val = max(3, val);
  wisdom = val;
  UpdateSkillBasedStats();
}
//*****************************************************************************
//    NAME: CHARACTER::SetDex
//
// PURPOSE: 
//
//*****************************************************************************
void CHARACTER::SetDex(BYTE val)
{
  val = min(25, val);
  val = max(3, val);
  dexterity = val;
  UpdateSkillBasedStats();
}
//*****************************************************************************
//    NAME: CHARACTER::SetCon
//
// PURPOSE: 
//
//*****************************************************************************
void CHARACTER::SetCon(BYTE val)
{
  val = min(25, val);
  val = max(3, val);
  constitution = val;
  UpdateSkillBasedStats();
}
//*****************************************************************************
//    NAME: CHARACTER::SetCha
//
// PURPOSE: 
//
//*****************************************************************************
void CHARACTER::SetCha(BYTE val)
{
  val = min(25, val);
  val = max(3, val);
  charisma = val;
  UpdateSkillBasedStats();
}

//*****************************************************************************
//    NAME: CHARACTER::SetMaxMovement
//
// PURPOSE: 
//
//*****************************************************************************
void CHARACTER::SetMaxMovement(BYTE val)
{
  if (GetType() != MONSTER_TYPE)
  {
    if (val > MAX_MOVE) val = MAX_MOVE;    
  }
  maxMovement = val;
}

//*****************************************************************************
//    NAME: CHARACTER::SetAge
//
// PURPOSE: 
//
//*****************************************************************************
void CHARACTER::SetAge(int val)
{
  int max = GetMaxAge();
  if (val > max) val=max;
  if (val < 1) val=1;
  age = val;
}

//*****************************************************************************
//    NAME: CHARACTER::SetTHACO
//
// PURPOSE: 
//
//*****************************************************************************
void CHARACTER::SetTHACO(int val)
{
  THACO = val;
  if (THACO < 0) THACO = 0;
  if (THACO > 20) THACO = 20;
}

//*****************************************************************************
//    NAME: CHARACTER::SetAC
//
// PURPOSE: 
//
//*****************************************************************************
void CHARACTER::SetAC(int val)
{
  AC=val;
  if (AC < -10) AC = -10;
  if (AC > 10) AC = 10;
}

//*****************************************************************************
//    NAME: CHARACTER::SetHitPoints
//
// PURPOSE: 
//
//*****************************************************************************
void CHARACTER::SetHitPoints(int val)
{
  // are we healing this character?
  if ((val > 0) && (val > hitPoints))
  {
    // disallow if diseased
    if (GetAdjSpecAb(SA_Diseased))
    {
      if (val > 1)
        val = 1;
    }
  }

  hitPoints=val;

  if (hitPoints > maxHitPoints)
    hitPoints = maxHitPoints;
  else if (hitPoints < -10)
    hitPoints = -10;

  if (hitPoints < 0)
  {
    if (!IsCombatActive()) // combat takes care of setting correct status
    {
      if (hitPoints == -10)
      {
        SetStatus(Dead);
      }
      else
      {
        // if okay, set to Dead, else leave status alone
        if (GetStatus() == Okay)
          SetStatus(Dead);
      }
    }
  }
}

//*****************************************************************************
//    NAME: CHARACTER::RestoreMaxHitPoints
//
// PURPOSE: 
//
//*****************************************************************************
void CHARACTER::RestoreMaxHitPoints()
{
  hitPoints=maxHitPoints;
}

//*****************************************************************************
//    NAME: CHARACTER::RestoreToBestCondition
//
// PURPOSE: 
//
//*****************************************************************************
void CHARACTER::RestoreToBestCondition()
{
  RestoreMaxHitPoints();
  SetStatus(Okay);
  spellEffects.RemoveAll();
}

//*****************************************************************************
//    NAME: CHARACTER::GetMaxHitPoints
//
// PURPOSE: 
//
//*****************************************************************************
int CHARACTER::GetMaxHitPoints()
{
  return maxHitPoints;
}

//*****************************************************************************
//    NAME: CHARACTER::SetMaxHitPoints
//
// PURPOSE: 
//
//*****************************************************************************
void CHARACTER::SetMaxHitPoints(int val)
{
  maxHitPoints=val;
}

//*****************************************************************************
//    NAME: CHARACTER::SetBirthday
//
// PURPOSE: 
//
//*****************************************************************************
void CHARACTER::SetBirthday(int bday)
{
  if (bday <= 0) bday=1;
  if (bday > 365) bday = 365;
  birthday = bday;
}

//*****************************************************************************
//    NAME: CHARACTER::GetCurrExp
//
// PURPOSE: 
//
//*****************************************************************************
int CHARACTER::GetCurrExp(int classflag)
{
  ASSERT( NumClassBits(classflag) == 1 );

  switch (classflag)
  {
  case MagicUserFlag: return GetAdjMagicUserExp();
  case ClericFlag:    return GetAdjClericExp();
  case ThiefFlag:     return GetAdjThiefExp();
  case FighterFlag:   return GetAdjFighterExp();
  case PaladinFlag:   return GetAdjPaladinExp();
  case RangerFlag:    return GetAdjRangerExp();
  case DruidFlag:     return GetAdjDruidExp();
  }
  ASSERT(FALSE);
  return 0;
}

//*****************************************************************************
//    NAME: CHARACTER::SetCurrExp
//
// PURPOSE: 
//
//*****************************************************************************
void CHARACTER::SetCurrExp(int classflag, int exp)
{
  ASSERT( NumClassBits(classflag) == 1 );
  switch (classflag)
  {
  case MagicUserFlag: MUExperience = exp; return;
  case ClericFlag:    ClericExperience = exp; return;
  case ThiefFlag:     ThiefExperience = exp; return;
  case FighterFlag:   FighterExperience = exp; return;
  case PaladinFlag:   PaladinExperience = exp; return;
  case RangerFlag:    RangerExperience = exp; return;
  case DruidFlag:     DruidExperience = exp; return;
  }
  ASSERT(FALSE);
}

//*****************************************************************************
//    NAME: CHARACTER::IncCurrExp
//
// PURPOSE: 
//
//*****************************************************************************
int CHARACTER::IncCurrExp(int classflag, int exp)
{
  ASSERT( NumClassBits(classflag) == 1 );
  switch (classflag)
  {
  case MagicUserFlag: MUExperience += exp; MUExperience=max(MUExperience,0); return MUExperience;
  case ClericFlag:    ClericExperience += exp; ClericExperience=max(ClericExperience,0); return ClericExperience;
  case ThiefFlag:     ThiefExperience += exp; ThiefExperience=max(ThiefExperience,0); return ThiefExperience;
  case FighterFlag:   FighterExperience += exp; FighterExperience=max(FighterExperience,0); return FighterExperience;
  case PaladinFlag:   PaladinExperience += exp; PaladinExperience=max(PaladinExperience,0); return PaladinExperience;
  case RangerFlag:    RangerExperience += exp; RangerExperience=max(RangerExperience,0); return RangerExperience;
  case DruidFlag:     DruidExperience += exp; DruidExperience=max(DruidExperience,0); return DruidExperience;
  }
  ASSERT(FALSE);
  return 0;
}

//*****************************************************************************
//    NAME: CHARACTER::CanCastDetectMagic
//
// PURPOSE: 
//
//*****************************************************************************
DWORD CHARACTER::CanCastDetectMagic()
{
  CHARACTER_SPELL cspell;
  SPELL_DATA *pSdata;

  POSITION pos = spells.list.GetHeadPosition();
  while (pos != NULL)
  {
    cspell = spells.list.GetNext(pos);
    if (cspell.IsMemorized())
    {
      pSdata = spellData.GetSpellData(cspell.spell);
      if (pSdata!=NULL)
      {
        // could be done using special abilities
        if (pSdata->CanPerformSpecAb(SA_DetectMagic))
          return cspell.spell;
        // or if user manually modifed this value with spell effect
        if (pSdata->CanEnableAffectAttr("$CHAR_DETECTMAGIC"))
          return cspell.spell;
      }
    }
  }
  return NO_SPELL;
}

//*****************************************************************************
//    NAME: CHARACTER::CanCastIdentify
//
// PURPOSE: 
//
//*****************************************************************************
DWORD CHARACTER::CanCastIdentify()
{
  CHARACTER_SPELL cspell;
  SPELL_DATA *pSdata;

  POSITION pos = spells.list.GetHeadPosition();
  while (pos != NULL)
  {
    cspell = spells.list.GetNext(pos);
    if (cspell.IsMemorized())
    {
      pSdata = spellData.GetSpellData(cspell.spell);
      if (pSdata!=NULL)
      {
        if (pSdata->CanEnableAffectAttr("$CHAR_IDENTIFY"))
          return cspell.spell;
      }
    }
  }
  return NO_SPELL;
}

#ifdef UAFEngine // engine-only magic code

//*****************************************************************************
//    NAME: CHARACTER::RemoveAllSpellEffectsByLevel
//
// PURPOSE: Removes only spells that have same level as lvl
//
//*****************************************************************************
int CHARACTER::RemoveAllSpellEffectsByLevel(int lvl)
{
  bool done = false; 
  int count=0;

  while (!done)
  {
    BOOL removed = FALSE;
    POSITION pos = spellEffects.GetHeadPosition();  

    while (pos != NULL)
    {    
      if (   ((spellEffects.GetAt(pos).flags & SPELL_EFFECTS_DATA::EFFECT_SPELL))
          || ((spellEffects.GetAt(pos).flags & SPELL_EFFECTS_DATA::EFFECT_SPELLSPECAB)) )
      {
        // Look for spell effects in the list that have a spell level == lvl
        //
        SPELL_DATA *pSpell = spellData.GetSpellData(spellEffects.GetAt(pos).key);        
        if ((pSpell!=NULL) && (pSpell->Level <= lvl))
        {
          count++;
          removed=true;
          spellEffects.RemoveAt(pos);
        }
      }
      else if ((spellEffects.GetAt(pos).flags & SPELL_EFFECTS_DATA::EFFECT_ITEMSPECAB))
      {
        // item special abilities require level 12 to remove in this manner
        if (lvl == 12)
        {
          count++;
          removed=true;
          spellEffects.RemoveAt(pos);
        }
      }
      // else it's a CHARSPECAB which cannot be removed

      if (!removed)
        spellEffects.GetNext(pos);
      else
        break;
    }   
    
    if (!removed)
      done = true;
  }

  DisplayCurrSpellEffects("RemoveAllSpellEffectsByLevel");
  return count;
}

//*****************************************************************************
//    NAME: CHARACTER::DispelAllSpellEffectsByLevel
//
// PURPOSE: Removes all spell effects that have spell level <= lvl, and
//          checks dispel flag for each spell
//
//*****************************************************************************
int CHARACTER::DispelSpellEffectsByLevel(int lvl)
{
  bool done = false; 
  int count=0;

  while (!done)
  {
    BOOL removed = FALSE;
    POSITION pos = spellEffects.GetHeadPosition();  

    while (pos != NULL)
    {    
      if (   ((spellEffects.GetAt(pos).flags & SPELL_EFFECTS_DATA::EFFECT_SPELL))
          || ((spellEffects.GetAt(pos).flags & SPELL_EFFECTS_DATA::EFFECT_SPELLSPECAB)) )
      {
        // Look for spell effects in the list that have a spell level == lvl
        //
        SPELL_DATA *pSpell = spellData.GetSpellData(spellEffects.GetAt(pos).key);        
        if ((pSpell!=NULL) && (pSpell->Level <= lvl) && (pSpell->CanBeDispelled))
        {
          count++;
          removed=true;
          spellEffects.RemoveAt(pos);
        }
      }
      else if ((spellEffects.GetAt(pos).flags & SPELL_EFFECTS_DATA::EFFECT_ITEMSPECAB))
      {
        // item special abilities require level 12 to remove in this manner
        if (lvl >= 12)
        {
          count++;
          removed=true;
          spellEffects.RemoveAt(pos);
        }
      }
      // else it's a CHARSPECAB which cannot be removed

      if (!removed)
        spellEffects.GetNext(pos);
      else
        break;
    }   
    
    if (!removed)
      done = true;
  }
  DisplayCurrSpellEffects("DispelSpellEffectsByLevel");
  return count;
}

//*****************************************************************************
//    NAME: CHARACTER::RemoveSpellByName
//
// PURPOSE: 
//
//*****************************************************************************
BOOL CHARACTER::RemoveSpellByName(const CString &sname)
{
  bool done = false; 
  int count=0;

  while (!done)
  {
    BOOL removed = FALSE;
    POSITION pos = spellEffects.GetHeadPosition();  

    while (pos != NULL)
    {    
      // Look for spell effects in the list that have a spell name == sname
      //
      // we have an effect that has a spell record in the spell database
      SPELL_DATA *pSpell = spellData.GetSpellData(spellEffects.GetAt(pos).key);        
      if ((pSpell!=NULL) && (pSpell->Name.CompareNoCase(sname)==0) && (pSpell->CanBeDispelled))
      {
        count++;
        removed=true;
        spellEffects.RemoveAt(pos);
      }

      if (!removed)
        spellEffects.GetNext(pos);
      else
        break;
    }   
    
    if (!removed)
      done = true;
  }
  DisplayCurrSpellEffects("RemoveSpellByName");
  return count;
}

//*****************************************************************************
//    NAME: CHARACTER::ClearAllItemCursedFlags
//
// PURPOSE: 
//
//*****************************************************************************
void CHARACTER::ClearAllItemCursedFlags()
{
  POSITION pos = myItems.GetHeadPosition();
  while (pos != NULL)
    myItems.GetNext(pos).cursed=FALSE;  
}

//*****************************************************************************
//    NAME: CHARACTER::RemoveAllTargets
//
// PURPOSE: 
//
//*****************************************************************************
void CHARACTER::RemoveAllTargets()
{
  targets.Clear();
}

//*****************************************************************************
//    NAME: CHARACTER::InitTargeting
//
// PURPOSE: 
//
//*****************************************************************************
void CHARACTER::InitTargeting(spellTargetingType ttype, BOOL friendfireok, BOOL partyfriend, DWORD targs, DWORD range)
{
  //ASSERT( IsCombatActive() == FALSE ); // combat magic handled in combat.cpp

  RemoveAllTargets();
  targets.m_type = ttype;
  targets.m_FriendlyFireOk = friendfireok;
  targets.m_IsPartyAligned = partyfriend;
  switch (ttype)
  {
  case Self:
    targets.m_SelectingUnits=TRUE;
    targets.m_MaxHitDice=0;
    targets.m_MaxTargets=1;
    targets.m_MaxRange=0;
    targets.m_MapTargetX=-1;
    targets.m_MapTargetY=-1;
    break;
  case SelectedByCount:
    targets.m_SelectingUnits=TRUE;
    targets.m_MaxHitDice=0;
    ASSERT( targs > 0);
    targets.m_MaxTargets=targs;
    if (range==0) range = 1000000;
    targets.m_MaxRange=range;
    targets.m_MapTargetX=-1;
    targets.m_MapTargetY=-1;
    break;
  case WholeParty:
    targets.m_SelectingUnits=TRUE;
    targets.m_MaxHitDice=0;
    targets.m_MaxTargets=party.numCharacters;
    targets.m_MaxRange=0;
    targets.m_MapTargetX=-1;
    targets.m_MapTargetY=-1;
    break;
  case TouchedTargets:
    targets.m_SelectingUnits=TRUE;
    targets.m_MaxHitDice=0;
    targets.m_MaxTargets=targs;
    targets.m_MaxRange=1;
    targets.m_MapTargetX=-1;
    targets.m_MapTargetY=-1;
    break;
  case AreaLinePickStart:
  case AreaLinePickEnd:
  case AreaSquare:     
  case AreaCone:       
  case AreaCircle:
    if (IsCombatActive())
    {
      targets.m_SelectingUnits=FALSE;
      targets.m_MaxTargets=targs;
      if (range==0) range = 1000000;
      targets.m_MaxRange=range;
    }
    else // acts like ttype=WholeParty
    {
      targets.m_SelectingUnits=TRUE; // will be whole party
      targets.m_MaxTargets=party.numCharacters;
      targets.m_MaxRange=0;
    }
    targets.m_MaxHitDice=0;    
    targets.m_MapTargetX=-1;
    targets.m_MapTargetY=-1;
    break;
  case SelectByHitDice:
    targets.m_SelectingUnits=TRUE;
    targets.m_MaxHitDice=targs;
    targets.m_MaxTargets=0;
    if (range==0) range = 1000000;
    targets.m_MaxRange=range;
    targets.m_MapTargetX=-1;
    targets.m_MapTargetY=-1;
    break;
  }
}

//*****************************************************************************
//    NAME: CHARACTER::AddTarget
//
// PURPOSE: 
//
//*****************************************************************************
BOOL CHARACTER::AddTarget(CHARACTER &targ, int range)
{
  ASSERT( IsCombatActive() == FALSE ); // combat magic handled in combat.cpp
  if (targets.m_SelectingUnits==FALSE) return FALSE;
  if ((targets.m_FriendlyFireOk==FALSE) && (targ.IsPartyMember())) return FALSE;
  return targets.AddTarget(targ.marchOrder, targ.GetNbrHD(), range);
}

//*****************************************************************************
//    NAME: CHARACTER::AddTargetSelf
//
// PURPOSE: 
//
//*****************************************************************************
BOOL CHARACTER::AddTargetSelf()
{
  return AddTarget(*this, 0);
}

//*****************************************************************************
//    NAME: CHARACTER::InvokeSpellOnTarget
//
// PURPOSE: 
//
//
// Check Combat_Only and Friendly_Fire_Ok flags.
//
// spellCastingTimeType { sctImmediate, sctInitiative, sctRounds, sctTurns }
// 
// long Casting_Time;
// spellCastingTimeType Casting_Time_Type;
//
// Casting Time (non-combat):
//   
//   not used, spells are activated immediately
//  
// Casting Time (combat):
//  
//   Each round has 10 initiatives
//   1 round = 1 minute
//   10 rounds = 1 Turn
//   Spells requiring 1 or more rounds/turns always get cast at the end of a round/turn
//   Any hit on caster during casting time voids the spell.
//
//   sctImmediate: spell activated immediately
//   sctInitiative: Casting_Time is added to current initiative. Spell is activated
//                  when initiative time is reached. Cannot go past current round.
//   sctRounds:     Casting_Time is added to current round. Spell is activated
//                  when result round is reached, at end of round.
//   sctTurns:      Casting_Time*10 is added to current round. Spell is activated
//                  when result turn is reached, at end of that round.
//   
// Spell targeting when not in combat will only
// involve party members.
//
// spellTargetingType { Self, SelectedByCount, WholeParty, TouchedTargets, AreaCircle, SelectByHitDice }
//
// Targeting (non-combat):
// 
//   Self: affects caster
//   WholeParty,AreaCircle: affects whole party 
//   SelectedByCount, TouchedTargets: affects x number of targeted party members
//   SelectByHitDice: affects targeted party members until HD limit reached
//
//   Self, WholeParty, and AreaCircle don't need target selection, cast immediately
//   SelectedByCount,TouchedTargets,SelectByHitDice require individual target selection
//
// Targeting (combat):
//
//   Self: affects caster
//   WholeParty: affects whole party 
//   AreaCircle: select target map square, all targets within range are affected
//   SelectedByCount: affects x number of targets
//   TouchedTargets: affects 1 target within range 1
//   SelectByHitDice: select targets until HD limit reached
//
//   Self and WholeParty don't need target selection, cast immediately
//   AreaCircle, SelectedByCount,TouchedTargets,SelectByHitDice require individual target selection
//
// After targets are selected (if needed), evaluate spell script (if present) in
// context of caster and each target. If script evaluates to TRUE=1 check target
// saving throw.
//
// Saving Throws:
//
//   spellSaveVsType { ParPoiDM, PetPoly, RodStaffWand, Sp, BreathWeapon };
//   
//   Retrieve needed save value for target based on SaveVsType. Each type
//   will have a single value that increases with target level. Roll 20-sided
//   dice for target's saving throw. If roll is less than SaveVs value, no
//   save is made and full spell affect is applied. If roll is equal to or
//   greater than SaveVs, then SaveEffectType is used.
//
//   spellSaveEffectType { NoSave, SaveNegates, SaveForHalf, UseTHACO };
//
//   NoSave: full affect applied to target
//   SaveNegates: no affect applied to target
//   SaveForHalf: if numeric attribute affected, apply half, otherwise ignore
//   UseTHACO: if saving throw less than THACO, full affect, else none
//
// Spell Duration (non-combat):
//
//   spellDurationType { inRounds, byDamageTaken, inHours, inDays, Instantaneous };
//   
//   Same duration applies to all affected targets, except for byDamageTaken.
//
//   inRounds: (rounds/10) minutes
//   byDamageTaken: track total hp damage done to target
//   inHours: (hours*60) minutes
//   inDays:  (days*24*60) minutes
//   Instantaneous: never expires, no need to track this one in ActiveSpell list
//
// Spell Duration (non-combat):
//
//   spellDurationType { inRounds, byDamageTaken, inHours, inDays, Instantaneous };
//   
//   Same duration applies to all affected targets, except for byDamageTaken.
//
//   The unit of time for duration is rounds. 1 round = 1 game minute
//
//   inRounds: rounds = minutes
//   byDamageTaken: track total hp damage done to target
//   inHours: (hours*60) = minutes 
//   inDays:  (days*24*60) = minutes
//   Instantaneous: never expires, no need to track this one in ActiveSpell list
//
//*****************************************************************************
int CHARACTER::InvokeSpellOnTarget(CHARACTER& targ, int activeSpellKey, int spellDbKey)
{
  SPELL_DATA *pSdata = spellData.GetSpellData(spellDbKey);
  if (pSdata==NULL) 
  {
    ASSERT(FALSE);
    return activeSpellKey;
  }

  bool SpellActivated = false;

  // check to see if target is already affected by same spell effect
  // and it is not allowed to be cumulative
  if (!pSdata->IsCumulative)
  {
    bool dupFound=false;
    POSITION saPos = targ.spellEffects.GetHeadPosition();
    while ((saPos != NULL) && (!dupFound))
    {
      SPELL_EFFECTS_DATA &saData = targ.spellEffects.GetNext(saPos);
      if (saData.key == pSdata->key)
        dupFound=true;
    }
    if (dupFound)
      return activeSpellKey;
  }

  SetCharContext(GetContext(pSdata->ClassMask));
  SetTargetContext( targ.GetContext() );
  
  if (!pSdata->ExecuteSpellBeginScript())
    return activeSpellKey;

  if (activeSpellKey < 0)
  {
    if (pSdata->Duration_Rate != Instantaneous)
      activeSpellKey=ActiveSpellData.GetNextKey();
  }
  
  SpellActivated = true;
  
  // run script for each effect
  POSITION pos = pSdata->m_EffectedAttributes.GetHeadPosition();
  if (pos != NULL)
  {
    while (pos != NULL)
    {
      SPELL_EFFECTS_DATA saData = pSdata->m_EffectedAttributes.GetNext(pos);
      saData.key = pSdata->key; // spell database key
      saData.parent = activeSpellKey;
      saData.flags |= SPELL_EFFECTS_DATA::EFFECT_SPELL;
    
      if (saData.flags & SPELL_EFFECTS_DATA::EFFECT_TARGET)
      {
        BOOL activate = TRUE;
        if (saData.activationScript.GetLength() > 0)
        {
          activate = (pSdata->ExecuteActivationScript(saData)?TRUE:FALSE);              
        }

        if (activate)
        {
          CalcSpellEffectChangeValue(pSdata,saData,TRUE);            
          if (targ.AddSpellEffect(saData))
          {
            // force a scan of the current char status
            targ.TakeDamage(0,FALSE);
          }      
        }          
      }
    }
  }
  else
  {
    if (activeSpellKey >= 0)
    {
      // no affected attributes, add placeholder
      //
      // I think this allows the spell end script to be executed
      // when the spell expires?
      //
      // no need to add unless this spell expires
      //
      SPELL_EFFECTS_DATA saData;
      saData.changeResult = 0.0;
      saData.key = pSdata->key; // spell database key
      saData.parent = activeSpellKey;
      saData.flags |= SPELL_EFFECTS_DATA::EFFECT_SPELL; 
      saData.affectedAttr = "NOP_" + pSdata->Name;
      targ.AddSpellEffect(saData);
    }
  }

  // add special abilities
  int PreSpecAbCount = targ.spellEffects.GetCount();
  pSdata->specAbs.EnableAllFor(&targ);
  int PostSpecAbCount = targ.spellEffects.GetCount();
  int Diff = PostSpecAbCount - PreSpecAbCount;
  if (Diff > 0)
  {
    POSITION pos2 = targ.spellEffects.GetTailPosition();  
    while ((pos2 != NULL) && (Diff > 0))
    {        
      targ.spellEffects.GetAt(pos2).flags |= SPELL_EFFECTS_DATA::EFFECT_SPELLSPECAB;
      targ.spellEffects.GetAt(pos2).key = pSdata->key;
      if (activeSpellKey >= 0)
        targ.spellEffects.GetAt(pos2).parent = activeSpellKey;
      targ.spellEffects.GetPrev(pos2);
      Diff--;
    }
  }

  ClearTargetContext();
  ClearCharContext();      

  if (SpellActivated)
  {
    if (pSdata->Duration_Rate != Instantaneous) // Instantaneous = permanent = never expires
    {            
      ASSERT( activeSpellKey >= 0);

      // only need to add one active spell record for each
      // spell cast by caster.
      if (ActiveSpellData.Get(activeSpellKey) == NULL)
      {
        ACTIVE_SPELL aspell;
        aspell.caster = GetContext(pSdata->ClassMask);
        aspell.key = activeSpellKey;
        aspell.spellKey = pSdata->key; // spell db key
        aspell.CountTime=0;
        aspell.StopTime = 0;
        aspell.Lingers = (IsCombatActive() ? pSdata->Lingers : FALSE); // can't linger if not in combat
        aspell.lingerData.OnceOnly = pSdata->LingerOnceOnly;

        if (aspell.Lingers)
        {
          // add affected map locations to active spell record
          aspell.lingerData.mapdata.RemoveAll();
          POSITION pos = targets.m_maptarglist.GetHeadPosition();
          while (pos != NULL)
            aspell.lingerData.mapdata.AddTail(targets.m_maptarglist.GetNext(pos));

          // add targets to active spell record so that linger spell checks
          // won't re-target same dudes once they move (unless allowed by OnceOnly flag)
          for (int i=0;i<targets.m_targets.GetSize();i++)
            aspell.lingerData.affectedtargs.AddTail(targets.m_targets[i]);
        }

        WriteDebugString("Adding active spell cast by %i\n", aspell.caster.Instance);

        double val = 0.0;
        // determine duration value
        val = EvalDiceAsClass(pSdata->Duration, pSdata->ClassMask);
        ASSERT( val > 0.0 );                 

        // convert duration value into game minutes
        switch (pSdata->Duration_Rate)
        {
        case inRounds:
          val = max(1,val); // 1 minute min
          aspell.StopTime = val+party.getElapsedMinutes();        
          break;
        case byNbrAttacks:
        case byDamageTaken:      
          aspell.StopTime = val;
          break;
        case inHours:
          val = val * 60.0;
          val = max(1,val); // 1 round min
          aspell.StopTime = val+party.getElapsedMinutes();        
          break;
        case inDays:
          val = val * 24.0 * 60.0;
          val = max(1,val); // 1 round min
          aspell.StopTime = val+party.getElapsedMinutes();        
          break;
        default: 
          ASSERT(FALSE); 
          break;
        }
        
        ActiveSpellData.AddWithCurrKey(aspell);
      }
    }
  }

  DisplayCurrSpellEffects("InvokeSpellOnTarget");
  return activeSpellKey;
}


//*****************************************************************************
//    NAME: CHARACTER::SpellActivate
//
// PURPOSE: This function is invoked from the ::SpellActivate() in globals.cpp
//
//*****************************************************************************
void CHARACTER::SpellActivate(PENDING_SPELL &data)
{
  // this is for non-combat only
  ASSERT( IsCombatActive() == FALSE );
  ASSERT( GetType() != MONSTER_TYPE );

  CHARACTER_SPELL cspell;
  if (!spells.list.GetSpell(data.key, cspell)) 
  {
    ASSERT(FALSE);
    return;
  }

  SPELL_DATA *pSdata = spellData.GetSpellData(cspell.spell);
  if (pSdata==NULL) 
  {
    ASSERT(FALSE);
    return;
  }

  int activeSpellKey=-1;
  if (pSdata->Duration_Rate != Instantaneous)
    activeSpellKey = ActiveSpellData.GetNextKey();

  // in non-combat mode, all targets are party members
  for (int i=0;i<targets.NumTargets();i++)
  {
    int targIndex=-1;
    for (int t=0;t<party.numCharacters;t++)
    {
      if (party.characters[t].marchOrder == targets.m_targets[i])
      {
        targIndex=t;
        break;
      }
    }            

    if (targIndex < 0)
      continue; // can't find matching target in party


    activeSpellKey = InvokeSpellOnTarget(party.characters[targIndex], activeSpellKey, pSdata->key);
  }

  // Spell Feedback:
  //
  //   Play the spell cast sound regardless of whether spell
  //   actually affected anybody. 
  //
  //   No graphical feedback needed when not in combat
  //
  WriteDebugString("Spell %s activated by %s\n", pSdata->Name, GetName());
  pSdata->PlayCast();
}

//*****************************************************************************
//    NAME: CHARACTER::CastSpell
//
// PURPOSE: 
//
//*****************************************************************************
BOOL CHARACTER::CastSpell(int spell)
{ 
  // 'spell' is spell book key
  ASSERT (IsCombatActive() == FALSE);
  if (IsCombatActive()) return FALSE;

  CHARACTER_SPELL cspell;
  if (!spells.list.GetSpell(spell, cspell)) return FALSE;
  SPELL_DATA *pSdata = spellData.GetSpellData(cspell.spell);
  if (pSdata==NULL) return FALSE;

  spells.list.DecMemorized(spell, 1);
  ClearCharContextStack();

  PENDING_SPELL data;
  data.caster = GetContext(pSdata->ClassMask);
  data.key = spell;
  data.waitUntil = 0;
  data.flags = pSdata->Casting_Time_Type;

  // combat not active, so cast the
  // spell immediately
  ::SpellActivate(data);
  return TRUE;
}

//*****************************************************************************
//    NAME: CHARACTER::QueueUsedSpecAb
//
// PURPOSE: 
//
//*****************************************************************************
void CHARACTER::QueueUsedSpecAb(int sa, WORD src, DWORD skey)
{
  bool found=false;
  POSITION pos=specAbQueue.GetHeadPosition();
  while (pos!=NULL)
  {
    // ignore source and key
    SQueuedSpecAb data = specAbQueue.GetNext(pos);
    if (data.sa==sa)
    {
      found=true;
      break;
    }
  }

  if (found==false)
  {
    SQueuedSpecAb data;
    data.key=skey;
    data.sa=sa;
    data.src=src;
    specAbQueue.AddTail(data);
  }
}


//*****************************************************************************
//    NAME: CHARACTER::ClearQueuedSpecAb
//
// PURPOSE: 
//
//*****************************************************************************
void CHARACTER::ClearQueuedSpecAb()
{
  specAbQueue.RemoveAll();
}

//*****************************************************************************
//    NAME: CHARACTER::FormatSpecAbMsg
//
// PURPOSE: 
//
//*****************************************************************************
CString CHARACTER::FormatSpecAbMsg(DWORD sa_state)
{
  if (sa_state == SPECAB_MSG_NONE) return "";
  if (specAbQueue.GetCount()==0) return "";

  CString msg("");

  BOOL found=FALSE;
  POSITION pos = specAbQueue.GetHeadPosition();
  while (pos != NULL)
  {
    SQueuedSpecAb data = specAbQueue.GetAt(pos);
    specialAbilitiesType stype = (specialAbilitiesType)(data.sa);

    SPECIAL_ABILITIES *pSpecAbSrc=NULL;
    switch (data.src)
    {
    case SPELL_EFFECTS_DATA::EFFECT_ITEMSPECAB:
      {
        ITEM_DATA *pItem = itemData.GetItemData(data.key);
        if (pItem!=NULL) pSpecAbSrc = &pItem->specAbs;
      }
      break;
    case SPELL_EFFECTS_DATA::EFFECT_SPELLSPECAB:
      {
        SPELL_DATA *pSpell = spellData.GetSpellData(data.key);
        if (pSpell!=NULL) pSpecAbSrc = &pSpell->specAbs;
      }
      break;
    case SPELL_EFFECTS_DATA::EFFECT_CHARSPECAB:
      {
        pSpecAbSrc=&specAbs;
      }
      break;
    }

    if (pSpecAbSrc!=NULL)
    {
      if (pSpecAbSrc->GetMsgTypes(stype) & sa_state)
      {
        msg = pSpecAbSrc->GetMsgFormat(stype, sa_state);        
        if (msg.GetLength()>0)
        {
          specAbQueue.RemoveAt(pos);
          found=TRUE;
          break;
        }
      }
    }
    specAbQueue.GetNext(pos);
  }

  if (!found) return "";

  CString User("");
  CString Target("self");

  User = GetName(); 

  msg.Replace("/u", User);
  msg.Replace("/t", Target);
  return msg;
}

// give this character a chance to modify the attack dice roll used against
// a target during combat
//*****************************************************************************
//    NAME: CHARACTER::ModifyAttackRollDice
//
// PURPOSE: 
//
//*****************************************************************************
BOOL CHARACTER::ModifyAttackRollDice(CHARACTER *pTarget,int &num, int &sides, int &bonus)
{
  BOOL modify=FALSE;
  DWORD src;
  DWORD skey;
  if (GetAdjSpecAb(SA_Bless,&src,&skey))
  {
    QueueUsedSpecAb(SA_Bless, src,skey);
    bonus++;
    modify=TRUE;
  }
  if (GetAdjSpecAb(SA_Curse,&src,&skey))
  {
    QueueUsedSpecAb(SA_Curse,src,skey);
    bonus--;
    modify=TRUE;
  }

  // I'm invisible, so give +2 to dice roll
  if (!pTarget->GetAdjDetectingInvisible())
  {
    if (GetAdjSpecAb(SA_Invisible,&src,&skey))
    {
      bonus += 2;
      QueueUsedSpecAb(SA_Invisible,src,skey);
      modify=TRUE;    
    }  
    else if (GetAdjSpecAb(SA_InvisibleToAnimals,&src,&skey))
    {
      if (pTarget->IsAnimal())
      {
        bonus += 2;
        QueueUsedSpecAb(SA_InvisibleToAnimals,src,skey);
        modify=TRUE;
      }
    }
    else if (GetAdjSpecAb(SA_InvisibleToUndead,&src,&skey))
    {
      if (pTarget->GetUndeadType()!=NotUndead)
      {
        bonus += 2;
        QueueUsedSpecAb(SA_InvisibleToUndead,src,skey);
        modify=TRUE;
      }
    }
  }
  
  // Target is invisible, so give -4 to dice roll
  if (!GetAdjDetectingInvisible())
  {
    if (pTarget->GetAdjSpecAb(SA_Invisible,&src,&skey))
    {
      bonus -= 4;
      pTarget->QueueUsedSpecAb(SA_Invisible,src,skey);
      modify=TRUE;    
    }  
    else if (pTarget->GetAdjSpecAb(SA_InvisibleToAnimals,&src,&skey))
    {
      if (IsAnimal())
      {
        bonus -= 4;
        pTarget->QueueUsedSpecAb(SA_InvisibleToAnimals,src,skey);
        modify=TRUE;
      }
    }
    else if (pTarget->GetAdjSpecAb(SA_InvisibleToUndead,&src,&skey))
    {
      if (GetUndeadType()!=NotUndead)
      {
        bonus -= 4;
        pTarget->QueueUsedSpecAb(SA_InvisibleToUndead,src,skey);
        modify=TRUE;
      }
    }
  }
  return modify;
}

// give this character a chance to modify the damage dice roll used against
// a target during combat
//*****************************************************************************
//    NAME: CHARACTER::ModifyAttackDamageDice
//
// PURPOSE: 
//
//*****************************************************************************
BOOL CHARACTER::ModifyAttackDamageDice(CHARACTER *pTarget,int &num, int &sides, int &bonus, BOOL &NonLethal)
{
  DWORD src;
  DWORD skey;
  BOOL modify=FALSE;
  if (GetAdjSpecAb(SA_Enlarge,&src,&skey))
  {
    double maxdmg = num*sides;
    double bonusdmg = ceil(maxdmg*0.33);
    if (bonusdmg < 1.0) bonusdmg=1.0;
    bonus += bonusdmg;
    QueueUsedSpecAb(SA_Enlarge,src,skey);
    modify=TRUE;
  }
  if (GetAdjSpecAb(SA_Reduce,&src,&skey))
  {
    double maxdmg = num*sides;
    double bonusdmg = ceil(maxdmg*0.33);
    if (bonusdmg < 1.0) bonusdmg=1.0;
    bonus -= bonusdmg;
    QueueUsedSpecAb(SA_Reduce,src,skey);
    modify=TRUE;
  }

  if (GetAdjClass() == Ranger)
  {   
    if (pTarget->HasRangerDmgPenalty())
    {
      bonus += GetCurrentLevel(RangerFlag);
    }
  }

  if (GetAdjSpecAb(SA_Enfeebled,&src,&skey))
  {
    bonus -= 1;
    QueueUsedSpecAb(SA_Enfeebled,src,skey);
    modify=TRUE;
  }
  
  return modify;
}

// give this character a chance to modify the attack THACO used against
// the target during combat
//*****************************************************************************
//    NAME: CHARACTER::ModifyAttackThaco
//
// PURPOSE: 
//
//*****************************************************************************
BOOL CHARACTER::ModifyAttackThaco(CHARACTER *pTarget,int &val)
{  
  //DWORD src;
  //DWORD skey;  
  BOOL modify=FALSE;

  return modify;
}

// give this character a chance to modify the attack dice roll used against
// it during combat
//*****************************************************************************
//    NAME: CHARACTER::ModifyAttackRollDiceAsTarget
//
// PURPOSE: 
//
//*****************************************************************************
BOOL CHARACTER::ModifyAttackRollDiceAsTarget(CHARACTER *pAttacker,int &num, int &sides, int &bonus)
{
  DWORD src;
  DWORD skey;
  BOOL modify=FALSE;
  if (pAttacker->IsAlignmentEvil())
  {
    if (GetAdjSpecAb(SA_ProtFromEvil,&src,&skey))
    {
      bonus -= 2;
      QueueUsedSpecAb(SA_ProtFromEvil,src,skey);
      modify=TRUE;
    }
  }
  else if (pAttacker->IsAlignmentGood())
  {
    if (GetAdjSpecAb(SA_ProtFromGood,&src,&skey))
    {
      bonus -= 2;
      QueueUsedSpecAb(SA_ProtFromGood,src,skey);
      modify=TRUE;
    }
  }

  if (GetAdjSpecAb(SA_Enfeebled,&src,&skey))
  {
    bonus -= 2;
    QueueUsedSpecAb(SA_Enfeebled,src,skey);
    modify=TRUE;
  }
  return modify;
}

// give this character a chance to modify the damage dice roll used against
// it during combat
//*****************************************************************************
//    NAME: CHARACTER::ModifyAttackDamageDiceAsTarget
//
// PURPOSE: 
//
//*****************************************************************************
BOOL CHARACTER::ModifyAttackDamageDiceAsTarget(CHARACTER *pAttacker,int &num, int &sides, int &bonus, BOOL &NonLethal)
{
  return FALSE;
}

// give this character a chance to modify the attack THACO used against
// it during combat
//
// this function is the last one that is always called in an attack sequence,
// the damage roll functions are only called if the attack is successful.
//
//*****************************************************************************
//    NAME: CHARACTER::ModifyAttackThacoAsTarget
//
// PURPOSE: 
//
//*****************************************************************************
BOOL CHARACTER::ModifyAttackThacoAsTarget(CHARACTER *pAttacker,int &val)
{
  //DWORD src;
  //DWORD skey;
  BOOL modify=FALSE;
  
  
  if (   (pAttacker->GetAdjSpecAb(SA_Invisible))
      || (pAttacker->GetAdjSpecAb(SA_InvisibleToUndead))
      || (pAttacker->GetAdjSpecAb(SA_InvisibleToAnimals)))
  {
    // attacking while invisible removes the invisibility
    pAttacker->RemoveAllEffectsFor("$CHAR_INVISIBLE");
    pAttacker->RemoveAllEffectsFor("$CHAR_INVISIBLETOANIMALS");
    pAttacker->RemoveAllEffectsFor("$CHAR_INVISIBLETOUNDEAD");
  }  

  return modify;
}

//*****************************************************************************
//    NAME: CHARACTER::ModifyACAsTarget
//
// PURPOSE: 
//
//*****************************************************************************
BOOL CHARACTER::ModifyACAsTarget(CHARACTER *pAttacker, int &ac, int itemkey)
{
  DWORD src;
  DWORD skey;
  BOOL modify=FALSE;
  if (ac > 2)
  {
    if (GetAdjSpecAb(SA_Shield,&src,&skey))
    {
      if (itemkey != NO_ITEM)
      {
        ITEM_DATA *pItem = itemData.GetItemData(itemkey);
        if (pItem!=NULL)
        {
          switch (pItem->Wpn_Type)
          {
          case NotWeapon: 
          case Bow:
          case Crossbow:
            break;

          case HandBlunt: ac=min(ac,4); QueueUsedSpecAb(SA_Shield,src,skey);modify=TRUE;break;
          case HandCutting: ac=min(ac,4); QueueUsedSpecAb(SA_Shield,src,skey);modify=TRUE;break;
          case HandThrow: ac=min(ac,2); QueueUsedSpecAb(SA_Shield,src,skey);modify=TRUE;break;
          case SlingNoAmmo: ac=min(ac,3); QueueUsedSpecAb(SA_Shield,src,skey);modify=TRUE;break;
          case Throw: ac=min(ac,2); QueueUsedSpecAb(SA_Shield,src,skey);modify=TRUE;break;
          case Ammo: ac=min(ac,3); QueueUsedSpecAb(SA_Shield,src,skey);modify=TRUE;break;
          }
        }
        else
        {
          ac = min(ac,4);
          QueueUsedSpecAb(SA_Shield,src,skey);
          modify=TRUE;
        }
      }
      else
      {
        ac = min(ac,4);
        QueueUsedSpecAb(SA_Shield,src,skey);
        modify=TRUE;
      }
    }
  }
  return modify;
}

//*****************************************************************************
//    NAME: CHARACTER::ModifySaveRoll
//
// PURPOSE: 
//
//*****************************************************************************
BOOL CHARACTER::ModifySaveRoll(CHARACTER *pTarget, int &roll)
{
  return FALSE;
}

//*****************************************************************************
//    NAME: CHARACTER::ModifySaveRollAsTarget
//
// PURPOSE: 
//
//*****************************************************************************
BOOL CHARACTER::ModifySaveRollAsTarget(CHARACTER *pAttacker, int &roll)
{
  DWORD src;
  DWORD skey;
  BOOL modify=FALSE;
  if (pAttacker->IsAlignmentEvil())
  {
    if (GetAdjSpecAb(SA_ProtFromEvil,&src,&skey))
    {
      roll += 2;
      QueueUsedSpecAb(SA_ProtFromEvil,src,skey);
      modify=TRUE;
    }
  }
  else if (pAttacker->IsAlignmentGood())
  {
    if (GetAdjSpecAb(SA_ProtFromGood,&src,&skey))
    {
      roll += 2;
      QueueUsedSpecAb(SA_ProtFromGood,src,skey);
      modify=TRUE;
    }
  }

  if (GetAdjSpecAb(SA_Shield,&src,&skey))
  {
    roll += 1;
    QueueUsedSpecAb(SA_Shield,src,skey);
    modify=TRUE;
  }

  if (GetAdjSpecAb(SA_Displacement,&src,&skey))
  {
    roll += 2;
    QueueUsedSpecAb(SA_Displacement,src,skey);
    modify=TRUE;
  }
  return modify;
}

//*****************************************************************************
//    NAME: CHARACTER::ModifyAttackRollDiceForItem
//
// PURPOSE: 
//
//*****************************************************************************
BOOL CHARACTER::ModifyAttackRollDiceForItem(CHARACTER *pTarget,int itemdbkey,int &num, int &sides, int &bonus)
{
  DWORD src;
  DWORD skey;
  BOOL modify=FALSE;
  if (GetAdjSpecAb(SA_Shillelagh,&src,&skey))
  {
    ITEM_DATA *pItem = itemData.GetItemData(itemdbkey);
    if (pItem==NULL) return FALSE;
    if (pItem->Wpn_Type==HandBlunt)
    {
      bonus++;
      QueueUsedSpecAb(SA_Shillelagh,src,skey);
      modify=TRUE;
    }
  }
  return modify;
}

//*****************************************************************************
//    NAME: CHARACTER::ModifyAttackRollDiceForItemAsTarget
//
// PURPOSE: 
//
//*****************************************************************************
BOOL CHARACTER::ModifyAttackRollDiceForItemAsTarget(CHARACTER *pAttacker,int itemdbkey,int &num, int &sides, int &bonus)
{
  return FALSE;
}

//*****************************************************************************
//    NAME: CHARACTER::ModifyAttackDamageDiceForItem
//
// PURPOSE: 
//
//*****************************************************************************
BOOL CHARACTER::ModifyAttackDamageDiceForItem(CHARACTER *pTarget,int itemdbkey,int &num, int &sides, int &bonus, BOOL &NonLethal)
{
  DWORD src;
  DWORD skey;
  BOOL modify=FALSE;
  if (GetAdjSpecAb(SA_Shillelagh,&src,&skey))
  {
    ITEM_DATA *pItem = itemData.GetItemData(itemdbkey);
    if (pItem==NULL) return FALSE;
    if (pItem->Wpn_Type==HandBlunt)
    {
      QueueUsedSpecAb(SA_Shillelagh,src,skey);
      modify=TRUE;
      if (pTarget->GetSize() > Medium)
      {
        num=1;
        sides=4;
        bonus++;
      }
      else
      {
        num=2;
        sides=4;
      }
    }
  }
  return modify;
}

//*****************************************************************************
//    NAME: CHARACTER::ModifyAttackDamageDiceForItemAsTarget
//
// PURPOSE: 
//
//*****************************************************************************
BOOL CHARACTER::ModifyAttackDamageDiceForItemAsTarget(CHARACTER *pAttacker,int itemdbkey,int &num, int &sides, int &bonus, BOOL &NonLethal)
{
  DWORD src;
  DWORD skey;
  BOOL modify=FALSE;
  if (GetAdjSpecAb(SA_HoldPerson,&src,&skey))
  {
    bonus = GetHitPoints()+1; // make sure this attack kills target
    //QueueUsedSpecAb(SA_HoldPerson,src,skey); // doesn't really make sense here
    modify=TRUE;    
  }
  else if (GetAdjSpecAb(SA_Sleep,&src,&skey))
  {
    bonus = GetHitPoints()+1; // make sure this attack kills target
    //QueueUsedSpecAb(SA_Sleep,src,skey); // doesn't really make sense here
    modify=TRUE;    
  }
    
  return modify;
}

#endif // UAFEngine code

//*****************************************************************************
//    NAME: CHARACTER::IsMammal
//
// PURPOSE: 
//
//*****************************************************************************
BOOL CHARACTER::IsMammal()
{
  if (GetType() == MONSTER_TYPE)
  {
    return ((monsterData.GetMonsterFormFlags(origIndex) & FormMammal) == FormMammal);
  }

  return TRUE;
}

//*****************************************************************************
//    NAME: CHARACTER::IsAnimal
//
// PURPOSE: 
//
//*****************************************************************************
BOOL CHARACTER::IsAnimal()
{
  if (GetType() == MONSTER_TYPE)
  {
    return ((monsterData.GetMonsterFormFlags(origIndex) & FormAnimal) == FormAnimal);
  }
  
  return FALSE; 
}

//*****************************************************************************
//    NAME: CHARACTER::IsSnake
//
// PURPOSE: 
//
//*****************************************************************************
BOOL CHARACTER::IsSnake()
{
  if (GetType() == MONSTER_TYPE)
  {
    return ((monsterData.GetMonsterFormFlags(origIndex) & FormSnake) == FormSnake);
  }
  
  return FALSE;  
}

//*****************************************************************************
//    NAME: CHARACTER::IsGiant
//
// PURPOSE: 
//
//*****************************************************************************
BOOL CHARACTER::IsGiant()
{
  if (GetType() == MONSTER_TYPE)
  {
    return ((monsterData.GetMonsterFormFlags(origIndex) & FormGiant) == FormGiant);
  }
  
  return FALSE;  
}

//*****************************************************************************
//    NAME: CHARACTER::IsAlwaysLarge
//
// PURPOSE: 
//
//*****************************************************************************
BOOL CHARACTER::IsAlwaysLarge()
{
  if (GetType() == MONSTER_TYPE)
  {
    return ((monsterData.GetMonsterFormFlags(origIndex) & FormLarge) == FormLarge);
  }
  
  return FALSE;  
}

//*****************************************************************************
//    NAME: CHARACTER::HasDwarfACPenalty
//
// PURPOSE: 
//
//*****************************************************************************
BOOL CHARACTER::HasDwarfACPenalty()
{
  if (GetType() == MONSTER_TYPE)
  {
    return ((monsterData.GetMonsterPenaltyFlags(origIndex) & PenaltyDwarfAC) == PenaltyDwarfAC); 
  }
  
  return FALSE;  
}

//*****************************************************************************
//    NAME: CHARACTER::HasGnomeACPenalty
//
// PURPOSE: 
//
//*****************************************************************************
BOOL CHARACTER::HasGnomeACPenalty()
{
  if (GetType() == MONSTER_TYPE)
  {
    return ((monsterData.GetMonsterPenaltyFlags(origIndex) & PenaltyGnomeAC) == PenaltyGnomeAC); 
  }
  
  return FALSE;    
}

//*****************************************************************************
//    NAME: CHARACTER::HasDwarfTHACOPenalty
//
// PURPOSE: 
//
//*****************************************************************************
BOOL CHARACTER::HasDwarfTHACOPenalty()
{
  if (GetType() == MONSTER_TYPE)
  {
    return ((monsterData.GetMonsterPenaltyFlags(origIndex) & PenaltyDwarfTHACO) == PenaltyDwarfTHACO); 
  }
  
  return FALSE;    
}

//*****************************************************************************
//    NAME: CHARACTER::HasGnomeTHACOPenalty
//
// PURPOSE: 
//
//*****************************************************************************
BOOL CHARACTER::HasGnomeTHACOPenalty()
{
  if (GetType() == MONSTER_TYPE)
  {
    return ((monsterData.GetMonsterPenaltyFlags(origIndex) & PenaltyGnomeTHACO) == PenaltyGnomeTHACO); 
  }
  
  return FALSE;    
}

//*****************************************************************************
//    NAME: CHARACTER::HasRangerDmgPenalty
//
// PURPOSE: 
//
//*****************************************************************************
BOOL CHARACTER::HasRangerDmgPenalty()
{
  if (GetType() == MONSTER_TYPE)
  {
    return ((monsterData.GetMonsterPenaltyFlags(origIndex) & PenaltyRangerDmg) == PenaltyRangerDmg); 
  }
  
  return FALSE;    
}

//*****************************************************************************
//    NAME: CHARACTER::HasPoisonImmunity
//
// PURPOSE: 
//
//*****************************************************************************
BOOL CHARACTER::HasPoisonImmunity()
{
  if (GetType() == MONSTER_TYPE)
  {
    return ((monsterData.GetMonsterImmunityFlags(origIndex) & ImmunePoison) == ImmunePoison); 
  }
  
  return FALSE;    
}

//*****************************************************************************
//    NAME: CHARACTER::HasDeathImmunity
//
// PURPOSE: 
//
//*****************************************************************************
BOOL CHARACTER::HasDeathImmunity()
{
  if (GetType() == MONSTER_TYPE)
  {
    return ((monsterData.GetMonsterImmunityFlags(origIndex) & ImmuneDeath) == ImmuneDeath); 
  }
  
  return FALSE;    
}

//*****************************************************************************
//    NAME: CHARACTER::HasConfusionImmunity
//
// PURPOSE: 
//
//*****************************************************************************
BOOL CHARACTER::HasConfusionImmunity()
{
  if (GetType() == MONSTER_TYPE)
  {
    return ((monsterData.GetMonsterImmunityFlags(origIndex) & ImmuneConfusion) == ImmuneConfusion); 
  }
  
  return FALSE;    
}

//*****************************************************************************
//    NAME: CHARACTER::HasVorpalImmunity
//
// PURPOSE: 
//
//*****************************************************************************
BOOL CHARACTER::HasVorpalImmunity()
{
  if (GetType() == MONSTER_TYPE)
  {
    return ((monsterData.GetMonsterImmunityFlags(origIndex) & ImmuneVorpal) == ImmuneVorpal); 
  }
  
  return FALSE;    
}

//*****************************************************************************
//    NAME: CHARACTER::CanBeHeldOrCharmed
//
// PURPOSE: 
//
//*****************************************************************************
BOOL CHARACTER::CanBeHeldOrCharmed()
{
  if (GetType() == MONSTER_TYPE)
  {
    return ((monsterData.GetMonsterOptionFlags(origIndex) & OptionCanBeHeldCharmed) == OptionCanBeHeldCharmed); 
  }
  
  return TRUE;    
}

//*****************************************************************************
//    NAME: CHARACTER::AffectedByDispelEvil
//
// PURPOSE: 
//
//*****************************************************************************
BOOL CHARACTER::AffectedByDispelEvil()
{
  if (GetType() == MONSTER_TYPE)
  {
    return ((monsterData.GetMonsterOptionFlags(origIndex) & OptionAffectedByDispelEvil) == OptionAffectedByDispelEvil); 
  }
  
  return FALSE;    
}
