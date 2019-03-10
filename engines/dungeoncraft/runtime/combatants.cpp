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

#include "dungeoncraft/shared/Dungeon.h"

#include <math.h>
#include "dungeoncraft/shared/char.h"
#include "dungeoncraft/shared/GameEvent.h"
#include "dungeoncraft/shared/party.h"
#include "dungeoncraft/shared/Combatants.h"
#include "dungeoncraft/shared/DrawTile.h"
#include "dungeoncraft/shared/FileParse.h"
#include "dungeoncraft/shared/SoundMgr.h"
#include "dungeoncraft/shared/Viewport.h"
#include "dungeoncraft/shared/Screen.h"
#include "dungeoncraft/shared/GameMenu.h"
#include "dungeoncraft/shared/FormattedText.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


COMBATANT_DATA combatData;
COMBAT_MSGS combatMsgs;

char *CombatantsStateText[NUM_COMBATANTS_STATES] = 
{
  "CST_None",
  "CST_Casting",
  "CST_Attacking", 
  "CST_Guarding",
  "CST_Bandaging", 
  "CST_Using",
  "CST_Moving",
  "CST_Turning",
  "CST_Fleeing",
  "CST_CombatRoundDelay",
  "CST_CombatOver",
  "CST_Delaying",
  "CST_EndingTurn",
  "CST_ForcingWin",
  "CST_StartNewRound",
  "CST_ActivateSpell"
};


//*****************************************************************************
// NAME: InitCombatData
//
// PURPOSE:  
//*****************************************************************************
void COMBATANT_DATA::InitCombatData(COMBAT_EVENT_DATA *event)
{
   if (event==NULL) return;
   Clear();

   itemData.FreeArt();
   spellData.FreeArt();
   monsterData.ClearSounds();
   itemData.ClearSounds();
   spellData.ClearSounds();
   pathMgr.Clear();
   ClearEmptyCellSearch();
 
   CombatActive = TRUE;
   IsWilderness = event->m_UseOutdoorMap;

   combatRect.left = CombatScreenX;
   combatRect.top = CombatScreenY;
   combatRect.right = COMBAT_SCREEN_WIDTH;
   combatRect.bottom = COMBAT_SCREEN_HEIGHT;

   fullRect.left = 0;
   fullRect.top = 0;
   fullRect.right = SCREEN_WIDTH;
   fullRect.bottom = SCREEN_HEIGHT;

#ifdef _DEBUG
   ConfigFile.FindToken("Monster_NoMove", MonsterNoMove, false); // don't remove these tokens
   ConfigFile.FindToken("Monster_AlwaysMiss", MonsterAlwaysMiss, false);
   ConfigFile.FindToken("Monster_AlwaysHit", MonsterAlwaysHit, false);
   ConfigFile.FindToken("Monster_NoCast", MonsterNoCast, false);
   ConfigFile.FindToken("Monster_PlyrControl", MonsterPlyrControl, false);
   ConfigFile.FindToken("Monster_NoDeath", MonsterNoDeath, false);
   ConfigFile.FindToken("Party_AlwaysMiss", PartyAlwaysMiss, false);
   ConfigFile.FindToken("Party_AlwaysHit", PartyAlwaysHit, false);

   WriteDebugString("Combat Debug Settings:\n");
   WriteDebugString("       MonsNoMove = %u\n", MonsterNoMove);
   WriteDebugString("    MonsAlwaysHit = %u\n", MonsterAlwaysHit);
   WriteDebugString("   MonsAlwaysMiss = %u\n", MonsterAlwaysMiss);
   WriteDebugString("       MonsNoCast = %u\n", MonsterNoCast);
   WriteDebugString("      MonsNoDeath = %u\n", MonsterNoDeath);
   WriteDebugString("  MonsPlyrControl = %u\n", MonsterPlyrControl);
   WriteDebugString("  PartyAlwaysMiss = %u\n", PartyAlwaysMiss);
   WriteDebugString("   PartyAlwaysHit = %u\n\n", PartyAlwaysHit);
   WriteDebugString("     MapTileWidth = %u\n", TILES_HORZ);
   WriteDebugString("    MapTileHeight = %u\n\n", TILES_VERT);
#endif

   PartyNeverDies = event->partyNeverDies;
   PartyNoExperience = event->partyNoExperience;
   NoMagic = event->noMagic;
   surprise = event->TEMPsurprise;
   undeadTurnModifier = event->turningMod;

   pEvent = event;
   LoadSounds();

   partyStartX = party.Posx;
   partyStartY = party.Posy;

   // this function adjusts the start x,y to reflect
   // x,y in combat terrain map
   if (!IsWilderness)
     GenerateIndoorCombatMap(partyStartX, partyStartY, EventDirToPathDir(event->direction));
   else
     GenerateOutdoorCombatMap(partyStartX, partyStartY, EventDirToPathDir(event->direction));

   startTerrainX = 0;
   startTerrainY = 0;

   if (isObstical(partyStartX, partyStartY,1,1,FALSE))
   {
      WriteDebugString("Starting x,y is not empty\n");
      ASSERT(FALSE);
   }

   currInitiative = 1;
   currRound = 0;
   currTurn=0;
   lastRound = FALSE;
   CombatOver = FALSE;
  
   AddCombatants();
   determineInitCombatPos();

   int i;
   // make sure every monster can reach the party
   pathMgr.SetPathSize(1,1); // 1x1 good enough to let party reach
   for (i=0;i<combatants.GetSize();i++)
   {      
      if (!combatants[i].IsPartyMember())
      {
        int hPath = pathMgr.GetPath(combatants[i].x, combatants[i].y, 
                            partyStartX, partyStartY, FALSE);
        if (hPath < 0)
        {
          // no path from monster to party, so remove monster

          // remove from combat map
          placeCombatant(combatants[i].x, combatants[i].y,
                         NO_DUDE, 
                         combatants[i].width,
                         combatants[i].height);

          // signal that this gets removed
          combatants[i].x = -1;
        }
        else
          pathMgr.FreePath(hPath);
      }
      //else
      //  WriteDebugString("origIndex %i, origKey %i\n", combatants[i].origIndex, combatants[i].origKey);
   }   

   // some combatants may not have been placed on the map

   // transfer valid combatants to temp
   int count = 0;
   tempCOMBATANTS.RemoveAll();

   for (i=0;i<combatants.GetSize();i++)
   {
     if (combatants[i].x >= 0)
     {
       tempCOMBATANTS.SetAtGrow(count, combatants[i]);
       count++;
     }
   }

   // check to see if any were left out
   if (tempCOMBATANTS.GetSize() != combatants.GetSize())
   {
     TRACE("Adjusting combatant list from %i to %i\n",
                      combatants.GetSize(), tempCOMBATANTS.GetSize());

     // now we need to clean up the self index
     // and the terrain references to existing combatants
     for (i=0; i<tempCOMBATANTS.GetSize(); i++)
     {
       if (tempCOMBATANTS[i].self != i)
       {
         // this one needs updating
         tempCOMBATANTS[i].self = i;
         placeCombatant(tempCOMBATANTS[i].x, tempCOMBATANTS[i].y,
                        tempCOMBATANTS[i].self, 
                        tempCOMBATANTS[i].width,
                        tempCOMBATANTS[i].height);

       }
     }

     // now move corrected data back into combatants
     count=0;
     combatants.RemoveAll();
     for (i=0;i<tempCOMBATANTS.GetSize();i++)
     {
       combatants.SetAtGrow(count, tempCOMBATANTS[i]);
       TRACE("Start Data for combatant %i: self %i, x %i, y %i, f %i\n",
                        count, 
                        tempCOMBATANTS[i].self,
                        tempCOMBATANTS[i].x,tempCOMBATANTS[i].y,
                        tempCOMBATANTS[i].GetIsFriendly());
       count++;
     }
   }

   numCombatants = combatants.GetSize();
   WriteDebugString("Total combatants = %i\n", numCombatants);

   tempCOMBATANTS.RemoveAll();
   QComb.Clear();
   //StartNewRound();
   //prevRndCombatant = GetCurrCombatant();
   PlaceCursorOnDude(0,TRUE);   
   ClearEmptyCellSearch(); // don't need this data anymore
}

//*****************************************************************************
// NAME:    COMBATANT_DATA::AddCombatatants
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
void COMBATANT_DATA::AddCombatants()
{ 
  // always add party members first!
  AddCharsToCombatants(); 
  AddMonstersToCombatants();

  numCombatants = combatants.GetSize();
  for (int i=0;i<numCombatants;i++)
  {
    combatants[i].LoadCombatIcon();
    combatants[i].determineIconSize();
  }
}

//*****************************************************************************
// NAME:    COMBATANT_DATA::AddCharsToCombatants
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
void COMBATANT_DATA::AddCharsToCombatants()
{
  COMBATANT temp;  

  for (int i=0; i<party.numCharacters;i++)
  {
    charStatusType stype = party.characters[i].GetAdjStatus();
    if (   ((stype == Okay) || (stype == Dying))
        && (party.characters[i].GetAllowInCombat()))
    {       
       temp.Clear();
       temp.self = numCombatants;
       temp.InitFromCharData(i);       
       combatants.SetAtGrow(numCombatants, temp);     
       numCombatants++;
    }
  }
}

//*****************************************************************************
// NAME:    COMBATANT_DATA::AddMonstersToCombatants
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
void COMBATANT_DATA::AddMonstersToCombatants()
{
  COMBATANT temp;

  if (pEvent->monsters.GetCount() == 0)
  {
    WriteDebugString("No monsters in combat event %i\n", pEvent->GetEventId());
    return;
  }

  // how many monsters can we add?
  int MaxMonstersAllowed = MAX_COMBATANTS - combatants.GetSize() - 1;

  if (pEvent->randomMonster)
  {
    // pick just one monster/NPC from the list
    int result = rollDice(pEvent->monsters.GetCount(), 1, 0.0);
    result -= 1; // make zero-based index;
    result = max(0, result);

    int count;
    if (pEvent->monsters.monsters[result].UseQty == MONSTER_EVENT::meUseQty)
    {
      count = pEvent->monsters.monsters[result].qty;
    }
    else
    {
      count = rollDice(pEvent->monsters.monsters[result].qtyDiceSides,
                       pEvent->monsters.monsters[result].qtyDiceQty,
                       pEvent->monsters.monsters[result].qtyBonus);
      WriteDebugString("Random quantity of combat monster: %i\n", count);
    }

    double mod = GetMonsterQtyMod();
    if (mod != 0.0) count += ((mod/100.0)*count);
    if (count > MaxMonstersAllowed) count = MaxMonstersAllowed;
    if (count <= 0) count=1;

    for (int j=0;j<count;j++)
    {     
      temp.Clear();
      temp.self = numCombatants; // init relies on self index

      if (pEvent->monsters.monsters[result].type == MONSTER_TYPE)
        temp.InitFromMonsterData(pEvent->monsters.monsters[result].monster,
                                 pEvent->monsters.monsters[result].friendly,
                                 pEvent->monsters.monsters[result].items,
                                 pEvent->monsters.monsters[result].money);
      else
        temp.InitFromNPCData(pEvent->monsters.monsters[result].monster,
                             pEvent->monsters.monsters[result].friendly,
                             pEvent->monsters.monsters[result].items,
                             pEvent->monsters.monsters[result].money);

      if (temp.origKey != -1)
      {         
        temp.self = numCombatants;
        temp.SetMorale(pEvent->monsterMorale);
        temp.SetMorale( temp.GetMorale() + pEvent->monsters.monsters[result].moraleAdj );

        int PreSpecAbCount = temp.spellEffects.GetCount();
        // turn on the special abilities
        temp.specAbs.EnableAllFor(&temp);

        int PostSpecAbCount = temp.spellEffects.GetCount();
        int Diff = PostSpecAbCount - PreSpecAbCount;
        if (Diff > 0)
        {
          POSITION pos = temp.spellEffects.GetTailPosition();  
          while ((pos != NULL) && (Diff > 0))
          {        
            temp.spellEffects.GetAt(pos).flags |= SPELL_EFFECTS_DATA::EFFECT_CHARSPECAB;
            temp.spellEffects.GetAt(pos).key = temp.self;
            temp.spellEffects.GetPrev(pos);
            Diff--;
          }
        }

        combatants.SetAtGrow(numCombatants, temp);
        numCombatants++;
      }
    }
  }
  else
  {
    // add all monsters/NPCs
    for (int i=0;i<pEvent->monsters.GetCount();i++)
    {
      int count;
      if (pEvent->monsters.monsters[i].UseQty == MONSTER_EVENT::meUseQty)
      {
        count = pEvent->monsters.monsters[i].qty;
      }
      else
      {
        count = rollDice(pEvent->monsters.monsters[i].qtyDiceSides,
                         pEvent->monsters.monsters[i].qtyDiceQty,
                         pEvent->monsters.monsters[i].qtyBonus);
        WriteDebugString("Random quantity of combat monster: %i\n", count);
      }

      double mod = GetMonsterQtyMod();
      if (mod != 0.0) count += ((mod/100.0)*count);
      if (count <= 0) count=1;

      for (int j=0;j<count;j++)
      {     
        temp.Clear();
        temp.self = numCombatants; // init relies on self index

        if (pEvent->monsters.monsters[i].type == MONSTER_TYPE)
          temp.InitFromMonsterData(pEvent->monsters.monsters[i].monster,
                                   pEvent->monsters.monsters[i].friendly,
                                   pEvent->monsters.monsters[i].items,
                                   pEvent->monsters.monsters[i].money);
        else
          temp.InitFromNPCData(pEvent->monsters.monsters[i].monster,
                               pEvent->monsters.monsters[i].friendly,
                               pEvent->monsters.monsters[i].items,
                               pEvent->monsters.monsters[i].money);

        if (temp.origKey != -1)
        {         
          temp.self = numCombatants;
          temp.SetMorale(pEvent->monsterMorale);
          temp.SetMorale( temp.GetMorale() + pEvent->monsters.monsters[i].moraleAdj );

          int PreSpecAbCount = temp.spellEffects.GetCount();
          // turn on the special abilities
          temp.specAbs.EnableAllFor(&temp);

          int PostSpecAbCount = temp.spellEffects.GetCount();
          int Diff = PostSpecAbCount - PreSpecAbCount;
          if (Diff > 0)
          {
            POSITION pos = temp.spellEffects.GetTailPosition();  
            while ((pos != NULL) && (Diff > 0))
            {        
              temp.spellEffects.GetAt(pos).flags |= SPELL_EFFECTS_DATA::EFFECT_CHARSPECAB;
              temp.spellEffects.GetAt(pos).key = temp.self;
              temp.spellEffects.GetPrev(pos);
              Diff--;
            }
          }

          if (combatants.GetSize() < MaxMonstersAllowed)
          {
            combatants.SetAtGrow(numCombatants, temp);
            numCombatants++;
          }
        }
      }
    }
  }
}

//*****************************************************************************
// NAME: getActiveCombatCharacter
//
// PURPOSE:  
//*****************************************************************************
COMBATANT& COMBATANT_DATA::getActiveCombatCharacter()
{
  int curr = GetCurrCombatant();
  ASSERT(curr != NO_DUDE);
  
  // coding error
  if (curr==NO_DUDE) return combatants[0];

  // call getActiveCastingCharacter while casting a spell
  // since the caster may die. The caster info is stored
  // separately to make sure it is available until the 
  // spell effects have been delivered to all targets (if applicable).
  //ASSERT(combatants[curr].state!=CS_Casting);

  if (combatants[curr].GetType() == MONSTER_TYPE)
  {
    WriteDebugString("getActiveCombatCharacter() called for monster combatant\n");
  }

  return combatants[curr];
}

//*****************************************************************************
//    NAME: *COMBATANT_DATA::getActiveCastingCharacter
//
// PURPOSE: 
//
//*****************************************************************************
COMBATANT *COMBATANT_DATA::getActiveCastingCharacter()
{
  ASSERT(currCaster != NO_DUDE);
  if (currCaster==NO_DUDE) return NULL;
  return (&(combatants[currCaster]));
}

//*****************************************************************************
//    NAME: COMBATANT_DATA::SetActiveCastingCharacter
//
// PURPOSE: 
//
//*****************************************************************************
void COMBATANT_DATA::SetActiveCastingCharacter(int dude)
{
  currCaster = dude;
  if (currCaster==NO_DUDE) return;
  EnsureVisibleTarget(currCaster);
}


//*****************************************************************************
// NAME: COMBATANT_DATA::CanBandage
//
// PURPOSE:  
//*****************************************************************************
BOOL COMBATANT_DATA::CanBandage()
{
  int curr = GetCurrCombatant();
  if (curr == NO_DUDE)
    return FALSE;

  if (combatants[curr].CanBandage())
  {
    int i=0;    
    while (i < numCombatants)
    {
      if ((i != GetCurrCombatant()) && (combatants[i].GetAdjStatus() == Dying))
        return TRUE;  
      i++;
    }
  }

  return FALSE;
}


//*****************************************************************************
// NAME: COMBATANT_DATA::Bandage
//
// PURPOSE:  
//*****************************************************************************
void COMBATANT_DATA::Bandage()
{
  if (CanBandage())
  {
    int count=0;
    int btarg=-1;
	int mosthurt = 0;

    for (int i=0;i<numCombatants;i++)
    {
      if (combatants[i].GetAdjStatus() == Dying)
      {
		  // Figure out which PC is the most hurt, and set them to be bandaged first.
		  if (combatants[i].GetHitPoints() < combatants[mosthurt].GetHitPoints()) {
			mosthurt = i;
			btarg = i;
			count = 1;
		  }

	  }
	}
       
	// Now bandage the most hurt PC
	if (count ==1) {
		combatants[mosthurt].isBandaged = TRUE;
		combatants[mosthurt].SetHitPoints(0);
		combatants[mosthurt].SetStatus(Unconscious);        
        //combatants[i].bandageWho = i;
    }
    
    // sets state to cs_bandage so status message is displayed
    combatants[GetCurrCombatant()].Bandage();
    // status message displays bandage targ if only one targ
    if (count == 1) 
      combatants[GetCurrCombatant()].bandageWho = btarg;      
  }
}

//*****************************************************************************
// NAME: COMBATANT_DATA::ResetAim
//
// PURPOSE:  
//*****************************************************************************
void COMBATANT_DATA::ResetAim()
{
  SetUpdateFlag();
  aimPos = GetCurrCombatant();

  if ((aimPos >= 0) && (aimPos < numCombatants))
  {
    cursorX = combatants[aimPos].x;
    cursorY = combatants[aimPos].y;
    EnsureVisibleCursor();
  }
}

//*****************************************************************************
// NAME: GetNextAim
//
// PURPOSE:  
//*****************************************************************************
int COMBATANT_DATA::GetNextAim()
{
  if (numCombatants <= 0)
  {
    ASSERT(FALSE);
    aimPos = NO_DUDE;
    return NO_DUDE;
  }
  //combatData.SetUpdateFlag();

  if (aimPos == NO_DUDE)
    aimPos = GetCurrCombatant();

  int count=0;
  while (count < numCombatants)
  {
    if ((aimPos < 0) || (aimPos >= numCombatants))
      aimPos = 0;
    else
      aimPos++;
    
    if ((aimPos >= 0) && (aimPos < numCombatants))
    {
      if (combatants[aimPos].charOnCombatMap())
      {
        cursorX = combatants[aimPos].x;
        cursorY = combatants[aimPos].y;
        EnsureVisibleCursor();
        return aimPos;
      }
    }
    count++;
  }
  aimPos = GetCurrCombatant();
  return aimPos;
}

//*****************************************************************************
// NAME: GetPrevAim
//
// PURPOSE:  
//*****************************************************************************
int COMBATANT_DATA::GetPrevAim()
{
  if (numCombatants <= 0)
  {
    ASSERT(FALSE);
    aimPos = NO_DUDE;
    return NO_DUDE;
  }
  //combatData.SetUpdateFlag();

  if (aimPos == NO_DUDE)
    aimPos = GetCurrCombatant();

  int count=0;
  while (count < numCombatants)
  {
    if ((aimPos < 0) || (aimPos >= numCombatants))
      aimPos = numCombatants-1;
    else
      aimPos--;
    
    if ((aimPos >= 0) && (aimPos < numCombatants))
    {
      if (combatants[aimPos].charOnCombatMap())
      {
        cursorX = combatants[aimPos].x;
        cursorY = combatants[aimPos].y;
        EnsureVisibleCursor();
        return aimPos;
      }
    }
    count++;
  }
  aimPos = GetCurrCombatant();
  return aimPos;

}

//*****************************************************************************
// NAME: getNumCombatants
//
// PURPOSE:  
//*****************************************************************************
void COMBATANT_DATA::getNumCombatants(int &numParty, int &numMons)
{
   numParty = 0;
   numMons = 0;
 
   // count friendly pc's and npc's in numParty
   // count non-friendly pc's, npc's, and monsters in numMons
   // do not count friendly monsters in numParty
 
   for (int i=0; i < numCombatants; i++)
   {
     if (combatants[i].charOnCombatMap())
     {
        if ((combatants[i].IsPartyMember()) || (combatants[i].GetIsFriendly())) numParty++;
        else numMons++;    
     }
   }
}

//*****************************************************************************
// NAME: DetermineCombatInitiative
//
// PURPOSE:  
//*****************************************************************************
void COMBATANT_DATA::DetermineCombatInitiative(void)
{  
   for (int i=0;i<numCombatants;i++)
      combatants[i].RollInitiative(surprise);
   // if one side was initially surprised, it only
   // works for the first round so reset it.
   surprise = Neither;
}

//*****************************************************************************
// NAME: getNextCombatant
//
// PURPOSE:  
//*****************************************************************************
int COMBATANT_DATA::getNextCombatant()
{   
   int dude;   

   while ((dude = QComb.Top()) != NO_DUDE)
   {
     if (combatants[dude].IsDone())
     {
       QComb.Pop();
     }
     else
     {
       TRACE("getNextCombatant() returns %u (queued)\n", dude);
       return dude;
     }
   }
 
   int i=0;
   BOOL found = FALSE;
   dude = NO_DUDE;

   while ((currInitiative <= 10) && (!found))
   {
      i=0;
      while ((i < numCombatants) && (!found))
      {
         if (   (combatants[i].initiative == currInitiative) 
            && (!combatants[i].IsDone()))
         {
            dude = i;
            found = TRUE;
         }         
         i++;
      }
    
      if (!found)
         currInitiative++;
   }
 
   if (dude != NO_DUDE)
   {
     QComb.Push(dude, TRUE);
     //PlaceCursorOnCurrentDude();
     TRACE("getNextCombatant() returns %u\n", dude);
   }
   else
     TRACE("getNextCombatant() returns NO_DUDE\n");

   return dude;
}

//*****************************************************************************
// NAME: moveCharNW
//
// PURPOSE:  
//*****************************************************************************
void COMBATANT_DATA::moveNW()
{  
   ASSERT(GetCurrCombatant() != NO_DUDE);
   combatants[GetCurrCombatant()].moveNW(); 
}

//*****************************************************************************
// NAME: moveCharNE
//
// PURPOSE:  
//*****************************************************************************
void COMBATANT_DATA::moveNE()
{
   ASSERT(GetCurrCombatant() != NO_DUDE);
   combatants[GetCurrCombatant()].moveNE();
}

//*****************************************************************************
// NAME: moveCharSE
//
// PURPOSE:  
//*****************************************************************************
void COMBATANT_DATA::moveSE()
{
   ASSERT(GetCurrCombatant() != NO_DUDE);
   combatants[GetCurrCombatant()].moveSE();
}

//*****************************************************************************
// NAME: moveCharSW
//
// PURPOSE:  
//*****************************************************************************
void COMBATANT_DATA::moveSW()
{
   ASSERT(GetCurrCombatant() != NO_DUDE);
   combatants[GetCurrCombatant()].moveSW();
}

//*****************************************************************************
// NAME: moveCharLeft
//
// PURPOSE:  
//*****************************************************************************
void COMBATANT_DATA::moveLeft()
{
   ASSERT(GetCurrCombatant() != NO_DUDE);
   combatants[GetCurrCombatant()].moveLeft();
}

//*****************************************************************************
// NAME: moveCharRight
//
// PURPOSE:  
//*****************************************************************************
void COMBATANT_DATA::moveRight()
{
   ASSERT(GetCurrCombatant() != NO_DUDE);
   combatants[GetCurrCombatant()].moveRight();
}

//*****************************************************************************
// NAME: moveCharForward
//
// PURPOSE:  
//*****************************************************************************
void COMBATANT_DATA::moveForward()
{
   ASSERT(GetCurrCombatant() != NO_DUDE);
   combatants[GetCurrCombatant()].moveForward();
}

//*****************************************************************************
// NAME: moveCharBackward
//
// PURPOSE:  
//*****************************************************************************
void COMBATANT_DATA::moveBackward()
{
   ASSERT(GetCurrCombatant() != NO_DUDE);
   combatants[GetCurrCombatant()].moveBackward();
}

//*****************************************************************************
//    NAME: COMBATANT_DATA::CurrMoveWouldFleeMap
//
// PURPOSE: 
//
//*****************************************************************************
BOOL COMBATANT_DATA::CurrMoveWouldFleeMap()
{
  ASSERT(GetCurrCombatant() != NO_DUDE);
  return (combatants[GetCurrCombatant()].CurrMoveWouldFleeMap());
}

//*****************************************************************************
// NAME: moveCursorForward
//
// PURPOSE:  
//*****************************************************************************
void COMBATANT_DATA::moveCursorForward()
{
   int x = cursorX;
   int y = cursorY-1;
 
   if (y >= 0)
   {
     SetUpdateFlag();
     ForceMapToMoveAlong(x, y, PathNorth, 1);
     cursorY = y;
   }
}


//*****************************************************************************
// NAME: moveCursorBackward
//
// PURPOSE:  
//*****************************************************************************
void COMBATANT_DATA::moveCursorBackward()
{
   int x = cursorX;
   int y = cursorY+1;
 
   if (y < MAX_TERRAIN_HEIGHT)
   {
     SetUpdateFlag();
     ForceMapToMoveAlong(x, y, PathSouth, 1);      
     cursorY = y;
   }
}


//*****************************************************************************
// NAME: moveCursorLeft
//
// PURPOSE:  
//*****************************************************************************
void COMBATANT_DATA::moveCursorLeft()
{
   int x = cursorX-1;
   int y = cursorY;
 
   if (x >= 0)
   {
     SetUpdateFlag();
     ForceMapToMoveAlong(x, y, PathWest, 1);
     cursorX = x;
   }
}


//*****************************************************************************
// NAME: moveCursorRight
//
// PURPOSE:  
//*****************************************************************************
void COMBATANT_DATA::moveCursorRight()
{
   int x = cursorX+1;
   int y = cursorY;
 
   if (x < MAX_TERRAIN_WIDTH)
   {
     SetUpdateFlag();
     ForceMapToMoveAlong(x, y, PathEast, 1); 
     cursorX = x;
   }
}

//*****************************************************************************
// NAME:    COMBATANT_DATA::determinePartyOrientation
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
int COMBATANT_DATA::determinePartyOrientation(int x, int y)
{
   int facing = party.GetPartyFacing();
   int width = getHallWidth(x, y, facing);

   if (width < 2)
   {
      switch (facing) 
      {
      case FACE_NORTH:
      case FACE_SOUTH:
         facing = FACE_EAST;
         width = getHallWidth(x, y, facing);
         break;
      case FACE_EAST:
      case FACE_WEST:
         facing = FACE_NORTH;
         width = getHallWidth(x, y, facing);
         break;
      }
    
      if (width < 2)
         WriteDebugString("Cannot orient combat party properly, space too small!\n");
   }

   return facing;
}

//*****************************************************************************
// NAME: determineInitCombatPos
//
// PURPOSE:  
//*****************************************************************************
void COMBATANT_DATA::determineInitCombatPos()
{
   int i;   
   BOOL reset = TRUE;
 
   // PC's and NPC's should be listed in combatData by marching order
   for (i=0;i<combatants.GetSize();i++)
   {
      //if (combatants[i].IsPartyMember())
      if (combatants[i].GetIsFriendly())
      {
         getNextCharCombatPos(combatants[i], reset);

         if (combatants[i].x >= 0)
         {
           placeCombatant(combatants[i].x, combatants[i].y,
                          i, 
                          combatants[i].width,
                          combatants[i].height);
         }
      }
      else 
      {
         getNextMonsterCombatPos(combatants[i]);

         if (combatants[i].x >= 0)
         {
           placeCombatant(combatants[i].x, combatants[i].y, 
                          i, 
                          combatants[i].width,
                          combatants[i].height);
         }
      }
   }
}

//*****************************************************************************
// NAME:    COMBATANT_DATA::GetNextDirection
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
void COMBATANT_DATA::GetNextDirection(eventDirType &currDir, int &facing)
{
   // Get the next direction to place monsters in
   switch (pEvent->direction) 
   {
   case Any:
     switch (currDir)
     {
     case North:
        currDir = East;
        break;
     case East:
        currDir = South;
        break;
     case South:
        currDir = West;
        break;
     case West:
        currDir = North;
        break;
     default:
       switch (party.GetPartyFacing()) 
       {
       case FACE_NORTH:
          currDir = North;
          break;
       case FACE_EAST:
          currDir = East;
          break;
       case FACE_SOUTH:
          currDir = South;
          break;
       case FACE_WEST:
          currDir = West;
          break;
       }
       break;
     }
     break;

   case InFront:
     switch (party.GetPartyFacing()) 
     {
     case FACE_NORTH:
        currDir = North;
        break;
     case FACE_EAST:
        currDir = East;
        break;
     case FACE_SOUTH:
        currDir = South;
        break;
     case FACE_WEST:
        currDir = West;
        break;
     }
     break;

   case N_W_E:
      if (currDir == West)
         currDir = North;
      else if (currDir == North)
         currDir = East;
      else
         currDir = West;
      break;
   case North:
      currDir = North;
      break;
   case W_S_E:
      if (currDir == East)
         currDir = South;
      else if (currDir == South)
         currDir = West;
      else
         currDir = East;
      break;
   case South:
      currDir = South;
      break;
   case N_S_E:
      if (currDir == North)
         currDir = East;
      else if (currDir == East)
         currDir = South;
      else
         currDir = North;
      break;
   case East:
      currDir = East;
      break;
   case N_S_W:
      if (currDir == South)
         currDir = West;
      else if (currDir == West)
         currDir = North;
      else
         currDir = South;
      break;
   case West:
      currDir = West;
      break;
   case N_S:
      if (currDir == North)
         currDir = South;
      else
         currDir = North;
      break;
   case N_E:
      if (currDir == North)
         currDir = East;
      else
         currDir = North;
      break;
   case N_W:
      if (currDir == West)
         currDir = North;
      else
         currDir = West;
      break;
   case S_E:
      if (currDir == East)
         currDir = South;
      else
         currDir = East;
      break;
   case S_W:
      if (currDir == South)
         currDir = West;
      else
         currDir = South;
      break;
   case E_W:
      if (currDir == East)
         currDir = West;
      else
         currDir = East;
      break;
   }

   switch (currDir) 
   {
   case North:
     facing = FACE_WEST;
     TRACE("Placing monster NORTH of party\n");
     break;
   case East:
     facing = FACE_WEST;
     TRACE("Placing monster EAST of party\n");
     break;
    
   case South:
     facing = FACE_EAST;
     TRACE("Placing monster SOUTH of party\n");
     break;
   case West:
     facing = FACE_EAST;
     TRACE("Placing monster WEST of party\n");
     break;   
   }
}

//*****************************************************************************
// NAME:    COMBATANT_DATA::GetMonsterDistance
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
int COMBATANT_DATA::GetMonsterDistance(eventDirType dir)
{
   int dist = 0;
 
   switch (pEvent->distance) 
   {
   case UpClose:
      dist = 2;
      break;
   case Nearby:
      if ((dir == East) || (dir == West))
        dist = TILES_HORZ/2;
      else
        dist = TILES_VERT/2;
      break;
   case FarAway:
      if ((dir == East) || (dir == West))
        dist = TILES_HORZ-2;
      else
        dist = TILES_VERT-2;
      break;
   }
   TRACE("Placing monster %u tiles away from party\n", dist);
   return dist;
}

//*****************************************************************************
// NAME:    COMBATANT_DATA::GetMonsterCoords
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
void COMBATANT_DATA::GetMonsterCoords(int &x, int &y, int dist, int dir)
{
   // take a guess about start coords
   switch (dir) 
   {
   case North:
      // account for isometric orientation
      x = partyStartX - (dist+(dist/2));
      if (x < 0)
        x = 0;
      y = partyStartY-dist;
      if (y < 0)
         y = 0;
      break;
    
   case East:
      x = partyStartX+dist;
      y = partyStartY;
      if (x >= MAX_TERRAIN_WIDTH)
         x = MAX_TERRAIN_WIDTH-5;
      break;
    
   case South:
     // account for isometric orientation
      x = partyStartX + (dist+(dist/2));
      if (x >= MAX_TERRAIN_WIDTH)
         x = MAX_TERRAIN_WIDTH-1;
      y = partyStartY+dist;
      if (y >= MAX_TERRAIN_HEIGHT)
         y = MAX_TERRAIN_HEIGHT-1;
      break;
    
   case West:
      x = partyStartX-dist;
      y = partyStartY;
      if (x < 0)
         x = 0;
      break;   
   }
 // make sure coords are in same room as party
 BOOL look = TRUE;
 
 double dX = abs(partyStartX - x);
 dX /= (abs(partyStartY-y)-1);
 double deltaSum=0;

 while (   (look) 
        && (!isObstical(x, y, 1, 1, FALSE))
        && (!HaveLineOfSight(partyStartX, partyStartY, x, y)))

 {
   switch (dir) 
   {
   case North:
      deltaSum += dX;
      while (deltaSum >= 1.0)
      {
        x++;
        deltaSum -= 1.0;
      }
        
      y++;
      if (x == partyStartX)
      {
        x--;
        look = FALSE;
      }
      if (y == partyStartY)
      {
        y--;
        look = FALSE;
      }
      break;
  
   case East:
      x--;
      if (x == partyStartX)
      {
        x++;
        look = FALSE;
      }
      break;
  
   case South:
      deltaSum += dX;
      while (deltaSum >= 1.0)
      {
        x--;
        deltaSum -= 1.0;
      }

      y--;
      if (x == partyStartX)
      {
        x++;
        look = FALSE;
      }
      if (y == partyStartY)
      {
        y++;
        look = FALSE;
      }
      break;
  
   case West:
      x++;
      if (x == partyStartX)
      {
        x--;
        look = FALSE;
      }
      break;   
   }     
 
   if (x < 0) 
   {
     x = 0;
     look = FALSE;
   }
   if (y < 0)
   {
     look = FALSE;
     y = 0;
   }
   if (x >= MAX_TERRAIN_WIDTH)
   {
      x = MAX_TERRAIN_WIDTH-1;
      look = FALSE;
   }
   if (y >= MAX_TERRAIN_HEIGHT)
   {
      y = MAX_TERRAIN_HEIGHT-1;
      look = FALSE;
   }
 }

 // no line of sight in specified direction
 if (look == FALSE)
 {
   x=-1;y=-1;
 }
}

//*****************************************************************************
// NAME: getNextMonsterCombatPos
//
// PURPOSE:  
//*****************************************************************************
void COMBATANT_DATA::getNextMonsterCombatPos(COMBATANT &monster)
{
   static eventDirType currDir = Any;   
 
   eventDirType lastDir;   

   do 
   {
     lastDir = currDir;
     GetNextDirection(currDir, monster.facing);
     GetMonsterCoords(monster.x, monster.y, GetMonsterDistance(currDir), currDir);
   } while ((monster.x == -1) && (currDir != lastDir));


   if (monster.x < 0)
   {
     monster.x = partyStartX;
     monster.y = partyStartY;
     TRACE("Setting monster start combat pos at party loc\n");
   }
 
   // find first friendly combatant
   int dude = NO_DUDE;
   int i=0;
   while ((dude == NO_DUDE) && (i < numCombatants))
   {
     if (combatants[i].friendly)
       dude = i;
     i++;
   }

   PATH_DIR dir=PathBAD;
   
   // find direction from monster to party to help guide 
   // search for empty cell when specified cell is not empty
   if (dude != NO_DUDE)
     dir = GetDir(monster.x,
                  monster.y,
                  combatants[dude].x,
                  combatants[dude].y);

   TRACE("Attempting to place %ux%u monster at %u,%u (%u)\n", 
                    monster.width, monster.height, monster.x, monster.y, dir);

   // calling findEmptyCell() will search for empty cell
   // in expanding circle.

   // try to find something in direction of party first
   if (findEmptyCell(monster.x, monster.y, monster.width, monster.height, dir))
   {
      TRACE("Found empty cell for monster (%u,%u)\n", monster.x, monster.y);
   }
   else
   {
      // now try to put the monster anywhere we can
      // does not check for reachable path to party, just place in first non-obstical location
      TRACE("First call to findEmptyCell() failed - widening search...\n");
      if (findEmptyCell(monster.x, monster.y, monster.width, monster.height, PathBAD))
      {
        TRACE("Found empty cell for monster (%u,%u)\n", monster.x, monster.y);
      }
      else
      {
        WriteDebugString("Did NOT find empty cell for monster\n");
        ASSERT(FALSE);
      }
   }
}

//*****************************************************************************
// NAME:    COMBATANT_DATA::PlaceCharacter
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
BOOL COMBATANT_DATA::PlaceCharacter(COMBATANT &dude, int &x, int &y)
{
   if (!isObstical(x,y, dude.width, dude.height))
   {
      dude.x = x;
      dude.y = y;
      return TRUE;
   }
   return FALSE;
}

//*****************************************************************************
//    NAME: COMBATANT_DATA::CanSeeParty
//
// PURPOSE: 
//
//*****************************************************************************
BOOL COMBATANT_DATA::CanSeeParty(int x, int y)
{
  // determine if at least one party member is in
  // sight on combat map
  BOOL success=FALSE;
  int i=0;
  while ((!success) && (i<numCombatants))
  {
    if ((combatants[i].x >= 0) && (combatants[i].y >= 0))
      success = HaveLineOfSight(combatants[i].GetCenterX(), combatants[i].GetCenterY(), x, y);
    i++;
  }

  if (!success)
    success = HaveLineOfSight(partyStartX, partyStartY, x, y);

  return success;
}

//*****************************************************************************
// NAME:    COMBATANT_DATA::PlaceCharacterLeft
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
BOOL COMBATANT_DATA::PlaceCharacterLeft(COMBATANT &dude, int &x, int &y)
{
   if ((!isObstical(x-1,y, dude.width, dude.height))
       && (CanSeeParty(x-1, y)))
   {
      dude.x = x-1;
      dude.y = y;
      x = x-1;
      return TRUE;
   }
   return FALSE;
}

//*****************************************************************************
// NAME:    COMBATANT_DATA::PlaceCharacterRight
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
BOOL COMBATANT_DATA::PlaceCharacterRight(COMBATANT &dude, int &x, int &y)
{
   if ((!isObstical(x+1,y, dude.width, dude.height))
       && (CanSeeParty(x+1, y)))
   {
      dude.x = x+1;
      dude.y = y;
      x = x+1;
      return TRUE;
   }
   return FALSE;
}

//*****************************************************************************
// NAME:    COMBATANT_DATA::PlaceCharacterNorth
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
BOOL COMBATANT_DATA::PlaceCharacterNorth(COMBATANT &dude, int &x, int &y)
{
   if ((!isObstical(x,y-1, dude.width, dude.height))
       && (CanSeeParty(x, y-1)))
   {
      dude.x = x;
      dude.y = y-1;
      y = y-1;
      return TRUE;
   }
   return FALSE;
}

//*****************************************************************************
// NAME:    COMBATANT_DATA::PlaceCharacterSouth
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
BOOL COMBATANT_DATA::PlaceCharacterSouth(COMBATANT &dude, int &x, int &y)
{
   if ((!isObstical(x,y+1, dude.width, dude.height))
       && (CanSeeParty(x, y+1)))
   {
      dude.x = x;
      dude.y = y+1;
      y = y+1;
      return TRUE;
   }
   return FALSE;
}

//*****************************************************************************
// NAME:    COMBATANT_DATA::PlaceCharacterHorz
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
BOOL COMBATANT_DATA::PlaceCharacterHorz(COMBATANT &dude, int &x, int &y)
{
  if (PlaceCharacter(dude, x, y))
    return TRUE;

  // get east/west width
  int width = getHallWidth(x, y, FACE_NORTH);
  if (width < 1)
    return FALSE;

  int newX; 
  int newY=y;
  BOOL found = FALSE;
  int attempts = 0;
  int dist = 0;

  while ((!found) && (attempts < width))
  {
    newX = x-dist;
    if (PlaceCharacterLeft(dude, newX, newY))
    {
      found = TRUE;
      x = newX;
      y = newY;
    }
    else
      attempts++;

    newX = x+dist;
    if (PlaceCharacterRight(dude, newX, newY))
    {
      found = TRUE;
      x = newX;
      y = newY;
    }
    else
      attempts++;

    dist++;
  }

  return found;
}

//*****************************************************************************
// NAME:    COMBATANT_DATA::PlaceCharacterVert
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
BOOL COMBATANT_DATA::PlaceCharacterVert(COMBATANT &dude, int &x, int &y)
{
  if (PlaceCharacter(dude, x, y))
    return TRUE;

  // get north/south width
  int width = getHallWidth(x, y, FACE_EAST);
  if (width < 1)
    return FALSE;

  int newX = x; 
  int newY;
  BOOL found = FALSE;
  int attempts = 0;
  int dist = 0;

  while ((!found) && (attempts < width))
  {
    newY = y-dist;
    if (PlaceCharacterNorth(dude, newX, newY))
    {
      found = TRUE;
      x = newX;
      y = newY;
    }
    else
      attempts++;

    newY = y+dist;
    if (PlaceCharacterSouth(dude, newX, newY))
    {
      found = TRUE;
      x = newX;
      y = newY;
    }
    else
      attempts++;

    dist++;
  }

  return found;
}
//*****************************************************************************
// NAME: getNextCharCombatPos
//
// PURPOSE:  
//*****************************************************************************
void COMBATANT_DATA::getNextCharCombatPos(COMBATANT &dude, BOOL &reset)
{
   static int lastX = 0, lastY = 0;
   static int charsInRow = 0;
   BOOL found = FALSE;
 
   if (reset)
   {
      charsInRow = 0;
      lastX = partyStartX;
      lastY = partyStartY;
      reset = FALSE;
   }
   
   int facing = party.GetPartyFacing();

   switch (facing) 
   {      
   case FACE_NORTH:
   case FACE_SOUTH:
      while ((!found) && (lastY >= 0) && (lastY < MAX_TERRAIN_HEIGHT))
      {
         if (charsInRow < MAX_CHARS_IN_ROW)
         {
            found = PlaceCharacterHorz(dude, lastX, lastY);            
            if (!found)
               charsInRow = MAX_CHARS_IN_ROW; // advance to next row
         }
         else 
         {           
            charsInRow = 0;
            lastX = partyStartX;
            if (facing == FACE_NORTH)
               lastY++;
            else
               lastY--;
         }
      }
      break;
    
   case FACE_EAST:
   case FACE_WEST:
      while ((!found) && (lastX >= 0) && (lastX < MAX_TERRAIN_WIDTH))
      {
         if (charsInRow < MAX_CHARS_IN_ROW)
         {
            found = PlaceCharacterVert(dude, lastX, lastY);            
            if (!found)
               charsInRow = MAX_CHARS_IN_ROW; // advance to next row
         }
         else 
         {
            charsInRow = 0;
            lastY = partyStartY;
            if (facing == FACE_EAST)
               lastX--;
            else
               lastX++;
         }
      }
      break;
   }
 
   if (isObstical(dude.x, dude.y, dude.width, dude.height))
     WriteDebugString("getNextCharCombatPos() returns a cell containing a wall\n");
   else
     TRACE("Placing char at combat pos %u,%u\n", dude.x, dude.y);
}

//*****************************************************************************
// NAME:    COMBATANT_DATA::RestoreToParty
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
void COMBATANT_DATA::RestoreToParty()
{
  for (int i=0; i<numCombatants; i++)
    combatants[i].RestoreToParty();
}

//*****************************************************************************
//    NAME: COMBATANT_DATA::DetermineIfCombatOver
//
// PURPOSE: 
//
//*****************************************************************************
void COMBATANT_DATA::DetermineIfCombatOver()
{
  int dude = GetCurrCombatant();
  int numParty, numMons;
  getNumCombatants(numParty, numMons);      
  
  if (numMons == 0)
  {
    // party wins
    combatVictor = PartyWins;
    
    if (dude != NO_DUDE)
      lastRound = TRUE; 
    else
      CombatOver = TRUE;
  }
  else if (numParty == 0)
  {
    // monsters win, or party ran away
    combatVictor = MonsterWins;
    
    if (dude != NO_DUDE)
      lastRound = TRUE; 
    else
      CombatOver = TRUE;
  }
  else
    combatVictor = NoWinner;
}

//*****************************************************************************
//    NAME: COMBATANT_DATA::DetermineVictoryExpPoints
//
// PURPOSE: 
//
//*****************************************************************************
void COMBATANT_DATA::DetermineVictoryExpPoints()
{
  int lcv;
  if (combatVictor == PartyWins)
  {
    //WriteDebugString("Party wins combat\n");
    expPointTotal = 0;        
    if (!GetConfigPartyNoExperience())
    {
      for (lcv=0;lcv<numCombatants;lcv++)
      {
        if ((!combatants[lcv].GetIsFriendly())&&(!combatants[lcv].IsPartyMember()))
        {
          charStatusType status = combatants[lcv].GetAdjStatus();
          if (status == Dead)
            expPointTotal += combatants[lcv].getCharExpWorth();
        }
      }
      
      double mod = GetMonsterExpMod();
      if (mod != 0.0)
        expPointTotal += ((mod/100.0)*expPointTotal);
      if (expPointTotal < 0) expPointTotal=0;
    }
  }
  else if (combatVictor == MonsterWins)
  {
    // See if party fled, or all died
    lcv = 0;        
    while (lcv < numCombatants)
    {
      if (combatants[lcv].IsPartyMember())
      {
        if (combatants[lcv].GetAdjStatus()==Fled)
        {
          combatVictor = PartyRanAway;
          break;
        }
      }
      lcv++;
    }
  }
}

//*****************************************************************************
// NAME:    COMBATANT_DATA::UpdateCombat
//
// PURPOSE: 
//
// RETURNS: TRUE if input should be forced by ProcInput.cpp
//*****************************************************************************
BOOL COMBATANT_DATA::UpdateCombat()
{
  BOOL isUpdate = FALSE;
  static int prevDude = NO_DUDE;

  prevRndCombatant = prevDude;
  StartingNewCombatRound=FALSE;

  int dude = GetCurrCombatant();  

  //if (prevRndCombatant != dude)
  //  PlaceCursorOnCurrentDude();

  if (   (dude == NO_DUDE) 
      || ((dude != NO_DUDE) && (combatants[dude].IsDone())))
  {
    getNextCombatant();
    dude = GetCurrCombatant();
  
    if (dude == NO_DUDE)
    {  
      StartNewRound();  
      dude = GetCurrCombatant();
      DetermineIfCombatOver();
    }

    if (dude != NO_DUDE)
    {
      isUpdate = TRUE;
      combatants[dude].OnStartTurn();
      //PlaceCursorOnCurrentDude(TRUE);
    }
  } // if dude is done with his turn

   if (CheckIdleTime())
   {
     // force end to combat if nobody does anything for awhile
     CombatOver = TRUE;
   }

   if (IsCombatOver())
   {     
     DetermineVictoryExpPoints();
     isUpdate = TRUE;
   }
   else if (!IsNextCombatant()) 
   {
     // there is a delay between combatant updates, if prev combatant is not
     // current combatant     
     if (dude != NO_DUDE)
      isUpdate |= combatants[dude].UpdateCombatant();
   }

   prevDude = GetCurrCombatant();

   isUpdate |= NeedUpdate;
   NeedUpdate = FALSE;

   //ASSERT(GetCurrCombatant() != NO_DUDE);
   return isUpdate;
}

//*****************************************************************************
// NAME: COMBATANT_DATA::CheckIdleTime
//
// PURPOSE:  
//*****************************************************************************
BOOL COMBATANT_DATA::CheckIdleTime()
{
  int minIdleTime = 1000000;
  int diff;
  for (int i=0;i<numCombatants;i++)
  {
    diff = currRound - combatants[i].lastAttackRound;
    if (diff < minIdleTime)
      minIdleTime = diff;
  }

  return (minIdleTime > MAX_COMBAT_IDLE_ROUNDS);
}


//*****************************************************************************
// NAME: COMBATANT_DATA::CheckMorale
//
// PURPOSE:  
//*****************************************************************************
void COMBATANT_DATA::CheckMorale()
{
  for (int i=0;i<numCombatants;i++)
    combatants[i].CheckMorale();
}

//*****************************************************************************
// NAME:    COMBATANT_DATA::StartNewRound
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
void COMBATANT_DATA::StartNewRound()
{
  SetUpdateFlag();

  if (lastRound)
    CombatOver = TRUE;

  //party.incrementClock(1); // handled in ProcessTimeSensitiveData()
  TRACE("Start new combat round\n");

  // battle rages on
  CheckDyingCombatants();
  CheckMorale();

  for (int i=0;i<numCombatants;i++)
  {
    if ((combatants[i].charCanTakeAction()) && (combatants[i].IsDone()))
    {      
      if (combatants[i].state != CS_Casting)
      {
        combatants[i].done = FALSE;
        // stay guarding until told otherwise
        if (combatants[i].OnAuto())
        {
          if (combatants[i].state != CS_Guarding)
            combatants[i].state = CS_None;
        }
        else
          combatants[i].state = CS_None;
      }

      double leftoverAttacks=combatants[i].availAttacks;             
      combatants[i].determineAvailAttacks();
      double maxAttacks = ceil(combatants[i].availAttacks);
      combatants[i].availAttacks += leftoverAttacks;
      combatants[i].availAttacks = min(combatants[i].availAttacks, maxAttacks);
      combatants[i].movement = 0;      
    }

    if (combatants[i].charOnCombatMap())
    {
      if (ActiveSpellData.LingerSpellAffectsTarget(combatants[i].self, combatants[i].x,combatants[i].y))
      {
        // A lingering spell is active at the location 
        // this dude is located at.
        TRACE("%i re-targeted by at least one active linger spell at location %i,%i\n",
              combatants[i].self, combatants[i].x,combatants[i].y);

        ActiveSpellData.ActivateLingerSpellsOnTarget(combatants[i].self, combatants[i].x,combatants[i].y);
      }    
    }
  }

  currInitiative = 1;
  currRound++;
  if (currRound%10 == 0) 
    currTurn++;

  DetermineCombatInitiative();
  getNextCombatant();

  prevRndCombatant = GetCurrCombatant();
  StartingNewCombatRound=TRUE;  
}

//*****************************************************************************
// NAME:    COMBATANT_DATA::CheckDyingCombatants
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
void COMBATANT_DATA::CheckDyingCombatants()
{
  for (int i=0;i<numCombatants;i++)
  {
    if (combatants[i].GetAdjStatus() == Dying)        
    {      
      if (!combatants[i].isBandaged)
        combatants[i].TakeDamage(1,FALSE);
    }
  }
}


//*****************************************************************************
//    NAME: COMBATANT_DATA::CheckForDeadGuys
//
// PURPOSE: 
//
//*****************************************************************************
void COMBATANT_DATA::CheckForDeadGuys()
{
  for (int i=0;i<numCombatants;i++)
  {
    if (combatants[i].GetAdjHitPoints() <= 0)
      combatants[i].blitDeadSprite();
  }
}

//*****************************************************************************
// NAME:    displayCursor
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
BYTE aval=100;
void COMBATANT_DATA::displayCursor(BOOL coverFullIcon)
{
  int i,j;
  int x = TerrainToScreenCoordX(cursorX);
  int y = TerrainToScreenCoordY(cursorY);

  if (x < 0) return;
  if (y < 0) return;

  // make sure entire cursor will fit in display area
  if (   ((x+(CursorSrc.right-CursorSrc.left)) <= (COMBAT_SCREEN_WIDTH))
      && ((y+(CursorSrc.bottom-CursorSrc.top)) <= (SCREEN_HEIGHT-COMBAT_MENU_HEIGHT)))
  {
    int dude = getCombatantInCell(cursorX, cursorY, 1,1);
  
    if (coverFullIcon)
    {
      if ((dude != NO_DUDE) && (dude < combatants.GetSize()))
      {
        for (i=0;i<combatants[dude].height;i++)
        {
          for (j=0;j<combatants[dude].width;j++)
          {
            GraphicsMgr.SetClipRect(&combatRect);              
            GraphicsMgr.BlitAlphaImage(aval, // 50%
                                      TerrainToScreenCoordX(combatants[dude].x+j),
                                      TerrainToScreenCoordY(combatants[dude].y+i),
                                      CombatCursorArt.surface,
                                      &CursorSrc);
            GraphicsMgr.SetClipRect(&fullRect);
          }
        }      
        combatants[dude].displayCombatSprite();
      }
    }
    else 
    {
      GraphicsMgr.SetClipRect(&combatRect);  
      // destX, destY, SrcSurfaceNum, src RECT
      GraphicsMgr.BlitAlphaImage(aval, x,y, CombatCursorArt.surface, &CursorSrc);
      GraphicsMgr.SetClipRect(&fullRect);
      
      if ((dude != NO_DUDE) && (dude < combatants.GetSize()))
        combatants[dude].displayCombatSprite();
    }
  }  
}

//*****************************************************************************
// NAME:    COMBATANT_DATA::PlaceCursorOnCurrentDude
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
void COMBATANT_DATA::PlaceCursorOnCurrentDude(BOOL ForceCenter)
{
  int dude = GetCurrCombatant();
  if ((dude < 0) || (dude >= numCombatants)) return;
  cursorX = combatants[dude].x;
  cursorY = combatants[dude].y;
  EnsureVisibleTarget(dude, ForceCenter);
}

//*****************************************************************************
//    NAME: COMBATANT_DATA::PlaceCursorOnDude
//
// PURPOSE: 
//
//*****************************************************************************
void COMBATANT_DATA::PlaceCursorOnDude(int dude, BOOL ForceCenter)
{
  if ((dude < 0) || (dude >= numCombatants)) return;
  cursorX = combatants[dude].x;
  cursorY = combatants[dude].y;
  EnsureVisibleTarget(dude, ForceCenter);
}

//*****************************************************************************
// NAME: COMBATANT_DATA::DisplayCombatants
//
// PURPOSE:  
//*****************************************************************************
void COMBATANT_DATA::DisplayCombatants()
{
  int i;
  // display dying dudes first  
  for (i=0;i<combatants.GetSize();i++)
  {
    if (combatants[i].charUnconscious())
      combatants[i].displayCombatSprite();
  }  

  for (i=0;i<combatants.GetSize();i++)
  {
    if (!combatants[i].charUnconscious())
      combatants[i].displayCombatSprite();
  }
}

//*****************************************************************************
//    NAME: COMBATANT_DATA::HandleCurrState
//
// PURPOSE: 
//
//*****************************************************************************
void COMBATANT_DATA::HandleCurrState()
{
  int curr = GetCurrCombatant();
  if (curr != NO_DUDE)
    combatants[curr].HandleCurrState();
}

//*****************************************************************************
// NAME:    COMBATANT_DATA::HandleTimeDelayMsgExpired
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
void COMBATANT_DATA::HandleTimeDelayMsgExpired()
{
  //PlaceCursorOnCurrentDude();
  int curr = GetCurrCombatant();
  if (curr != NO_DUDE)
    combatants[curr].HandleTimeDelayMsgExpired();
}

//*****************************************************************************
// NAME:    COMBATANT_DATA::HandleTimeDelayMsgBegin
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
void COMBATANT_DATA::HandleTimeDelayMsgBegin()
{  
  int curr = GetCurrCombatant();
  if (curr != NO_DUDE)
    combatants[curr].HandleTimeDelayMsgBegin();
}

//*****************************************************************************
// NAME: COMBATANT_DATA::getCharWeaponText
//
// PURPOSE:  
//*****************************************************************************
void COMBATANT_DATA::getCharWeaponText(int index, CString &wpn, CString &dmg)
{
  combatants[index].getCharWeaponText(wpn, dmg);
}

//*****************************************************************************
//    NAME: COMBATANT_DATA::NeedMissileAnimation
//
// PURPOSE: 
//
//*****************************************************************************
BOOL COMBATANT_DATA::NeedMissileAnimation()
{
  int curr = GetCurrCombatant();
  if (curr != NO_DUDE)
    return combatants[curr].NeedMissileAnimation();
  else
    return FALSE;
}

//*****************************************************************************
//    NAME: COMBATANT_DATA::InitMissileAnimation
//
// PURPOSE: 
//
//*****************************************************************************
void COMBATANT_DATA::InitMissileAnimation()
{
  int curr = GetCurrCombatant();
  if (curr != NO_DUDE)
  {
    combatants[curr].InitMissileAnimation();  
    //PlaceCursorOnCurrentDude();
  }
}
//*****************************************************************************
//    NAME: COMBATANT_DATA::NeedHitAnimation
//
// PURPOSE: 
//
//*****************************************************************************
BOOL COMBATANT_DATA::NeedHitAnimation()
{
  int curr = GetCurrCombatant();
  if (curr != NO_DUDE)
    return combatants[curr].NeedHitAnimation();
  else
    return FALSE;
}

//*****************************************************************************
//    NAME: COMBATANT_DATA::InitHitAnimation
//
// PURPOSE: 
//
//*****************************************************************************
void COMBATANT_DATA::InitHitAnimation()
{
  int curr = GetCurrCombatant();
  if (curr != NO_DUDE)
  {
    combatants[curr].InitHitAnimation();  
    //PlaceCursorOnDude(combatants[curr].target, FALSE);
  }
}

//*****************************************************************************
//    NAME: COMBATANT_DATA::NeedSpellCastAnimation
//
// PURPOSE: 
//
//*****************************************************************************
BOOL COMBATANT_DATA::NeedSpellCastAnimation(int targ)
{
  int curr = currCaster;//GetCurrCombatant();
  ASSERT(curr != NO_DUDE);
  if (curr != NO_DUDE)
  {
    if (targ < 0) return FALSE;
    if (!combatants[targ].charOnCombatMap()) return FALSE;
    return (combatants[curr].NeedSpellCastAnimation());
  }
  else
    return FALSE;
}

//*****************************************************************************
//    NAME: COMBATANT_DATA::InitSpellCastAnimation
//
// PURPOSE: 
//
//*****************************************************************************
void COMBATANT_DATA::InitSpellCastAnimation(int targ)
{
  int curr = currCaster;//GetCurrCombatant();
  ASSERT(curr != NO_DUDE);
  if (curr != NO_DUDE)
  {
    combatants[curr].InitSpellCastAnimation(targ);  
  }
}

//*****************************************************************************
//    NAME: COMBATANT_DATA::NeedSpellInRouteAnimation
//
// PURPOSE: 
//
//*****************************************************************************
BOOL COMBATANT_DATA::NeedSpellInRouteAnimation(int x, int y)
{
  int curr = currCaster;//GetCurrCombatant();
  ASSERT(curr != NO_DUDE);
  if (curr != NO_DUDE)
    return (combatants[curr].NeedSpellInRouteAnimation(x,y));
  else
    return FALSE;
}

//*****************************************************************************
//    NAME: COMBATANT_DATA::InitSpellInRouteAnimation
//
// PURPOSE: 
//
//*****************************************************************************
void COMBATANT_DATA::InitSpellInRouteAnimation(int x, int y)
{
  int curr = currCaster;//GetCurrCombatant();
  ASSERT(curr != NO_DUDE);

  if (curr != NO_DUDE)
  {
    combatants[curr].InitSpellInRouteAnimation(x, y);  
  }
}

//*****************************************************************************
//    NAME: COMBATANT_DATA::NeedSpellCoverageAnimation
//
// PURPOSE: 
//
//*****************************************************************************
BOOL COMBATANT_DATA::NeedSpellCoverageAnimation(int x, int y)
{
  int curr = currCaster;//GetCurrCombatant();
  ASSERT(curr != NO_DUDE);

  if (curr != NO_DUDE)
    return combatants[curr].NeedSpellCoverageAnimation();
  else
    return FALSE;
}

//*****************************************************************************
//    NAME: COMBATANT_DATA::InitSpellCoverageAnimation
//
// PURPOSE: 
//
//*****************************************************************************
void COMBATANT_DATA::InitSpellCoverageAnimation(int x, int y)
{
  int curr = currCaster;//GetCurrCombatant();
  ASSERT(curr != NO_DUDE);

  if (curr != NO_DUDE)
  {
    combatants[curr].InitSpellCoverageAnimation(x, y);  
    //PlaceCursorOnCurrentDude();
  }
}

//*****************************************************************************
//    NAME: COMBATANT_DATA::NeedSpellHitAnimation
//
// PURPOSE: 
//
//*****************************************************************************
BOOL COMBATANT_DATA::NeedSpellHitAnimation(int targ)
{
  int curr = currCaster;//GetCurrCombatant();
  ASSERT(curr != NO_DUDE);

  if (curr != NO_DUDE)
    return combatants[curr].NeedSpellHitAnimation();
  else
    return FALSE;
}

//*****************************************************************************
//    NAME: COMBATANT_DATA::InitSpellHitAnimation
//
// PURPOSE: 
//
//*****************************************************************************
void COMBATANT_DATA::InitSpellHitAnimation(int targ)
{
  int curr = currCaster;//GetCurrCombatant();
  ASSERT(curr != NO_DUDE);

  if (curr != NO_DUDE)
  {
    combatants[curr].InitSpellHitAnimation(targ);  
    //PlaceCursorOnCurrentDude();
  }
}

//*****************************************************************************
//    NAME: COMBATANT_DATA::NeedSpellLingerAnimation
//
// PURPOSE: 
//
//*****************************************************************************
BOOL COMBATANT_DATA::NeedSpellLingerAnimation()
{
  int curr = currCaster;//GetCurrCombatant();
  ASSERT(curr != NO_DUDE);

  if (curr != NO_DUDE)
    return combatants[curr].NeedSpellLingerAnimation();
  else
    return FALSE;
}

//*****************************************************************************
//    NAME: COMBATANT_DATA::InitSpellLingerAnimation
//
// PURPOSE: 
//
//*****************************************************************************
void COMBATANT_DATA::InitSpellLingerAnimation(int mapx, int mapy)
{
  int curr = currCaster;//GetCurrCombatant();
  ASSERT(curr != NO_DUDE);

  if (curr != NO_DUDE)
  {
    combatants[curr].InitSpellLingerAnimation(mapx,mapy);  
    //PlaceCursorOnCurrentDude();
  }
}

//*****************************************************************************
//    NAME: FormatCombatDelayText
//
// PURPOSE: 
//
//*****************************************************************************
CString FormatCombatDelayText()
{ 
  int dude = combatData.GetCurrCombatant();
  ASSERT(dude != NO_DUDE);  

  CString temp;
  temp.Format("%s is delaying...",
              combatData.combatants[dude].GetName());
  return temp;
}

//*****************************************************************************
//    NAME: FormatCombatEndTurnText
//
// PURPOSE: 
//
//*****************************************************************************
CString FormatCombatEndTurnText()
{ 
  int dude = combatData.GetCurrCombatant();
  ASSERT(dude != NO_DUDE);  

  CString temp;
  temp.Format("%s ends turn...",
              combatData.combatants[dude].GetName());
  return temp;
}

//*****************************************************************************
//    NAME: FormatCombatForceWinText
//
// PURPOSE: 
//
//*****************************************************************************
CString FormatCombatForceWinText()
{
  CString temp;
  temp = "The party has forced a victory";
  return temp;
}

//*****************************************************************************
//    NAME: FormatCombatMoveText
//
// PURPOSE: 
//
//*****************************************************************************
CString FormatCombatMoveText()
{  
  ASSERT(combatData.GetCurrCombatant() != NO_DUDE);

  int moves = combatData.combatants[combatData.GetCurrCombatant()].GetAdjMaxMovement() - 
                combatData.combatants[combatData.GetCurrCombatant()].movement;

  ASSERT( moves >= 0 );

  CString temp;
  temp.Format("MOVES LEFT: %u", moves);
  return temp;
}

//*****************************************************************************
//    NAME: FormatCombatAttackText
//
// PURPOSE: 
//
//*****************************************************************************
CString FormatCombatAttackText()
{ 
  int dude = combatData.GetCurrCombatant();
  ASSERT(dude != NO_DUDE);  
  int target = combatData.combatants[dude].GetCurrTarget();
  CString msg("attacks");
  CString temp;

  if (combatData.combatants[dude].GetType()==MONSTER_TYPE)
  {    
    int inv_item = combatData.combatants[dude].myItems.GetReady(WeaponHand);
    if (inv_item != NO_READY_ITEM)
    {
      // using a weapon attack
      int itemdbkey = combatData.combatants[dude].myItems.GetItem(inv_item);
      ITEM_DATA *idata = itemData.GetItemData(itemdbkey);
      if (idata!=NULL) msg = idata->attackMsg;
    }
    else
    {
      // only use monster attack msgs when using natural, non-weapon, attacks
      int nbrattacks = combatData.combatants[dude].GetNbrAttacks();
      int availattacks = combatData.combatants[dude].availAttacks;
      int currAttack = nbrattacks - availattacks;

      ASSERT( (currAttack>=0) && (currAttack<nbrattacks) );
      if ((currAttack < 0)||(currAttack>=nbrattacks)) currAttack=0;
      monsterData.GetMonsterAttackMsg(combatData.combatants[dude].origKey, currAttack, msg);
    }
  }
  else
  {
    int inv_item = combatData.combatants[dude].myItems.GetReady(WeaponHand);
    if (inv_item != NO_READY_ITEM)
    {      
      int itemdbkey = combatData.combatants[dude].myItems.GetItem(inv_item);
      ITEM_DATA *idata = itemData.GetItemData(itemdbkey);
      if (idata!=NULL) msg = idata->attackMsg;      
    }
  }

  if (msg.GetLength()==0) msg = "attacks";
  temp.Format("%s %s %s", 
              combatData.combatants[dude].GetName(),
              msg,
              combatData.combatants[target].GetName());

  return temp;
}

//*****************************************************************************
//    NAME: FormatCombatGuardText
//
// PURPOSE: 
//
//*****************************************************************************
CString FormatCombatGuardText()
{ 
  int dude = combatData.GetCurrCombatant();
  ASSERT(dude != NO_DUDE);  

  CString temp;
  temp.Format("%s is guarding...",
              combatData.combatants[dude].GetName());
  return temp;
}

//*****************************************************************************
//    NAME: FormatCombatBandageText
//
// PURPOSE: 
//
//*****************************************************************************
CString FormatCombatBandageText()
{ 
  int dude = combatData.GetCurrCombatant();
  ASSERT(dude != NO_DUDE);  

  CString temp;
  int bandagetarget = combatData.combatants[dude].bandageWho;
  if (bandagetarget >= 0)
  {    
    temp.Format("%s is bandaging %s...",
                combatData.combatants[dude].GetName(),
                combatData.combatants[bandagetarget].GetName());
    combatData.combatants[dude].bandageWho = -1;
  }
  else
    temp.Format("%s is bandaging...",
                combatData.combatants[dude].GetName());
  
  return temp;
}

//*****************************************************************************
//    NAME: FormatCombatTurnUndeadText
//
// PURPOSE: 
//
//*****************************************************************************
CString FormatCombatTurnUndeadText()
{ 
  int dude = combatData.GetCurrCombatant();
  ASSERT(dude != NO_DUDE);  

  CString temp;
  temp.Format("%s turns undead...",
              combatData.combatants[dude].GetName());
  return temp;
}

//*****************************************************************************
//    NAME: FormatCombatFleeText
//
// PURPOSE: 
//
//*****************************************************************************
CString FormatCombatFleeText()
{ 
  int dude = combatData.GetCurrCombatant();
  ASSERT(dude != NO_DUDE);  

  CString temp;
  temp.Format("%s runs away...",
              combatData.combatants[dude].GetName());
  return temp;
}

//*****************************************************************************
//    NAME: FormatCombatCastText
//
// PURPOSE: 
//
//*****************************************************************************
CString FormatCombatCastText()
{ 
  int dude = combatData.GetCurrCombatant();
  ASSERT(dude != NO_DUDE);  
  int charspellbookkey = combatData.combatants[dude].GetSpellBookKeyBeingCast();
  CHARACTER_SPELL cspell;
  if (!combatData.combatants[dude].spells.list.GetSpell(charspellbookkey, cspell))
    return CString("");

  CString name(spellData.GetSpellName(cspell.spell));
  if (name.IsEmpty()) name = "Unknown!";
  CString temp;
  temp.Format("%s begins casting %s...",
              combatData.combatants[dude].GetName(),
              name);
  return temp;
}

//*****************************************************************************
//    NAME: FormatActivateSpellText
//
// PURPOSE: 
//
//*****************************************************************************
CString FormatActivateSpellText()
{
  int dude = combatData.GetCurrCombatant();
  ASSERT(dude != NO_DUDE);  
  int charspellbookkey = combatData.combatants[dude].GetSpellBookKeyBeingCast();
  CHARACTER_SPELL cspell;
  if (!combatData.combatants[dude].spells.list.GetSpell(charspellbookkey, cspell))
    return CString("");

  CString name(spellData.GetSpellName(cspell.spell));
  if (name.IsEmpty()) name = "Unknown!";
  CString temp;
  temp.Format("%s casts %s...",
              combatData.combatants[dude].GetName(),
              name);
  return temp;
}

//*****************************************************************************
//    NAME: FormatSpellHitText
//
// PURPOSE: 
//
//*****************************************************************************
CString FormatSpellHitText()
{
  CString msg("");

  COMBATANT *pTarget=NULL;
  COMBATANT *pCaster = combatData.getActiveCastingCharacter();
  ASSERT(pCaster!=NULL);  
  if (pCaster==NULL) return msg;
  if (pCaster->targets.m_SelectingUnits)
  {
    int target = pCaster->targets.m_targets[0];
    pTarget = combatData.getCombatantPtr(target);
    ASSERT(pTarget!=NULL);  
    if (pTarget==NULL) return msg;
  }//else target is a map coordinate

  int charspellbookkey = pCaster->GetSpellBookKeyBeingCast();
  CHARACTER_SPELL cspell;
  if (!pCaster->spells.list.GetSpell(charspellbookkey, cspell))
    return msg;
  SPELL_DATA *pSpell = spellData.GetSpellData(cspell.spell);
  ASSERT(pSpell!=NULL);  
  if (pSpell==NULL) return msg;

  msg = pSpell->CastMsg;
  msg.Replace("/s", pSpell->Name);
  msg.Replace("/c", pCaster->GetName());
  if (pTarget!=NULL)
    msg.Replace("/t", pTarget->GetName());
  else
    msg.Replace("/t", "");

  return msg;
}

//*****************************************************************************
//    NAME: COMBATANT_DATA::FormatSpecAbMsg
//
// PURPOSE: 
//
//*****************************************************************************
CString COMBATANT_DATA::FormatSpecAbMsg(DWORD sa_state)
{
  int dude = GetCurrCombatant();
  ASSERT(dude != NO_DUDE);  
  return (combatants[dude].FormatSpecAbMsg(sa_state));
}

//*****************************************************************************
//    NAME: COMBATANT_DATA::FormatTimeDelayMsg
//
// PURPOSE: 
//
//*****************************************************************************
CString COMBATANT_DATA::FormatTimeDelayMsg(CombatStateType state)
{
  if (state == CST_None)
    state = (CombatStateType)GetCombatantState();

  CString msg="";
  switch (state)
  {
  case CST_Casting:
    msg = FormatCombatCastText();
    break;
  case CST_ActivateSpell:
    msg=FormatActivateSpellText();
    break;
  case CST_Attacking:
    msg = FormatCombatAttackText();
    break;
  case CST_Guarding: 
    msg = FormatCombatGuardText();
    break;
  case CST_Bandaging:
    msg = FormatCombatBandageText();
    break;
  case CST_Using:
    break;
  case CST_Moving: // shouldn't happen
    ASSERT(FALSE);
    //msg = FormatCombatMoveText();
    break;
  case CST_Turning:
    msg = FormatCombatTurnUndeadText();
    break;
  case CST_Fleeing:
    msg = FormatCombatFleeText();
    break;
  case CST_Delaying:
    msg = FormatCombatDelayText();
    break;
  case CST_EndingTurn:
    msg = FormatCombatEndTurnText();
    break;
  case CST_ForcingWin:
    msg = FormatCombatForceWinText();
    break;
  }
  return msg;
}

//*****************************************************************************
//    NAME: COMBATANT_DATA::Clear
//
// PURPOSE: 
//
//*****************************************************************************
void COMBATANT_DATA::Clear()
{
  StartingNewCombatRound=FALSE;
  IsWilderness=FALSE;
  CombatActive=FALSE;
  ResetAim();
  ClearSounds();
  hCharDeathSound=-1;hCharMoveSound=-1;hTurnUndeadSound=-1;
  lastRound=FALSE; CombatOver=FALSE;
  combatVictor=NoWinner;
  expPointTotal=0;
  startTerrainX=-1;startTerrainY=-1;
  cursorX=-1;cursorY=-1;
  partyStartX=-1;partyStartY=-1;
  currInitiative=-1;currRound=-1;currTurn=-1;
  numCombatants=0; NeedUpdate=FALSE;
  MonsterNoMove=FALSE;MonsterAlwaysMiss=FALSE;
  MonsterAlwaysHit=FALSE; MonsterNoCast=FALSE;
  MonsterNoFlee=FALSE;
  MonsterPlyrControl=FALSE;
  MonsterNoDeath=FALSE;
  numMonFlee=0;numMonSlain=0;
  numFrndFlee=0;numFrndSlain=0;
  PartyAlwaysMiss=FALSE; PartyAlwaysHit=FALSE;
  PartyNeverDies=FALSE;NoMagic=FALSE;
  currCaster=-1;
  QComb.Clear();
  combatants.RemoveAll();
  tempCOMBATANTS.RemoveAll();
}

//*****************************************************************************
//    NAME: COMBATANT_DATA::GetCombatantState
//
// PURPOSE: 
//
//*****************************************************************************
combatantStateType COMBATANT_DATA::GetCombatantState()
{
  int curr = GetCurrCombatant();
  if (curr != NO_DUDE)
    return combatants[curr].GetState(); 
  else
    return CS_None;
}

//*****************************************************************************
//    NAME: COMBATANT_DATA::AddTarget
//
// PURPOSE: 
//
//*****************************************************************************
void COMBATANT_DATA::AddTarget(int target) 
{ 
  int curr = GetCurrCombatant();
  if (curr != NO_DUDE)
    combatants[curr].AddTarget(target); 
}
//*****************************************************************************
//    NAME: COMBATANT_DATA::GetCurrTarget
//
// PURPOSE: 
//
//*****************************************************************************
int COMBATANT_DATA::GetCurrTarget() 
{ 
  int curr = GetCurrCombatant();
  if (curr != NO_DUDE)
    return combatants[curr].GetCurrTarget(); 
  else
    return NO_DUDE;
}
//*****************************************************************************
//    NAME: COMBATANT_DATA::GetNextTarget
//
// PURPOSE: 
//
//*****************************************************************************
int COMBATANT_DATA::GetNextTarget() 
{ 
  int curr = GetCurrCombatant();
  if (curr != NO_DUDE)
    return combatants[curr].GetNextTarget(); 
  else
    return NO_DUDE;
}

//*****************************************************************************
//    NAME: COMBATANT_DATA::RemoveCurrTarget
//
// PURPOSE: 
//
//*****************************************************************************
void COMBATANT_DATA::RemoveCurrTarget()
{
  int curr = GetCurrCombatant();
  if (curr != NO_DUDE)
    combatants[curr].RemoveCurrTarget(); 
}

//*****************************************************************************
//    NAME: COMBATANT_DATA::AttacksRemaining
//
// PURPOSE: 
//
//*****************************************************************************
double COMBATANT_DATA::AttacksRemaining()
{
  int curr = GetCurrCombatant();
  if (curr != NO_DUDE)
    return combatants[curr].AttacksRemaining(); 
  else
    return 0.0;
}

//*****************************************************************************
//    NAME: COMBATANT_DATA::IsAttacking
//
// PURPOSE: 
//
//*****************************************************************************
BOOL COMBATANT_DATA::IsAttacking() 
{ 
  int curr = GetCurrCombatant();
  if (curr != NO_DUDE)
    return combatants[curr].IsAttacking(); 
  else
    return FALSE;
}

//*****************************************************************************
//    NAME: COMBATANT_DATA::CanTarget
//
// PURPOSE: 
//
//*****************************************************************************
BOOL COMBATANT_DATA::CanTarget() 
{ 
  int curr = GetCurrCombatant();
  if (curr != NO_DUDE)
    return combatants[curr].CanTarget(); 
  else
    return FALSE;
}

//*****************************************************************************
//    NAME: COMBATANT_DATA::CanMove
//
// PURPOSE: 
//
//*****************************************************************************
BOOL COMBATANT_DATA::CanMove()  
{ 
  int curr = GetCurrCombatant();
  if (curr != NO_DUDE)
    return combatants[curr].CanMove(); 
  else
    return FALSE;
}
//*****************************************************************************
//    NAME: COMBATANT_DATA::CanCast
//
// PURPOSE: 
//
//*****************************************************************************
BOOL COMBATANT_DATA::CanCast()  
{ 
  int curr = GetCurrCombatant();
  if (curr != NO_DUDE)
    return combatants[curr].CanCast(); 
  else
    return FALSE;
}
//*****************************************************************************
//    NAME: COMBATANT_DATA::CanUse
//
// PURPOSE: 
//
//*****************************************************************************
BOOL COMBATANT_DATA::CanUse()   
{
  int curr = GetCurrCombatant();
  if (curr != NO_DUDE)    
    return combatants[curr].CanUse(); 
  else
    return FALSE;
}
//*****************************************************************************
//    NAME: COMBATANT_DATA::CanGuard
//
// PURPOSE: 
//
//*****************************************************************************
BOOL COMBATANT_DATA::CanGuard() 
{ 
  int curr = GetCurrCombatant();
  if (curr != NO_DUDE)
    return combatants[curr].CanGuard(); 
  else
    return FALSE;
}
//*****************************************************************************
//    NAME: COMBATANT_DATA::CanAttack
//
// PURPOSE: 
//
//*****************************************************************************
BOOL COMBATANT_DATA::CanAttack(int target) 
{ 
  int curr = GetCurrCombatant();
  if (curr != NO_DUDE)
    return combatants[curr].canAttack(target); 
  else
    return FALSE;
}
//*****************************************************************************
//    NAME: COMBATANT_DATA::CanTurnUndead
//
// PURPOSE: 
//
//*****************************************************************************
BOOL COMBATANT_DATA::CanTurnUndead()  
{ 
  int curr = GetCurrCombatant();
  if (curr != NO_DUDE)
    return combatants[curr].CanTurnUndead(); 
  else
    return FALSE;
}
//*****************************************************************************
//    NAME: COMBATANT_DATA::CanDelay
//
// PURPOSE: 
//
//*****************************************************************************
BOOL COMBATANT_DATA::CanDelay() 
{ 
  int curr = GetCurrCombatant();
  if (curr != NO_DUDE)
    return combatants[curr].CanDelay(); 
  else
    return FALSE;
}
//*****************************************************************************
//    NAME: COMBATANT_DATA::EndTurn
//
// PURPOSE: 
//
//*****************************************************************************
void COMBATANT_DATA::EndTurn(combatantStateType newState) 
{ 
  int curr = GetCurrCombatant();
  if (curr != NO_DUDE)
  {
    combatants[curr].EndTurn(newState);     
  }
}
//*****************************************************************************
//    NAME: COMBATANT_DATA::StopMoving
//
// PURPOSE: 
//
//*****************************************************************************
void COMBATANT_DATA::StopMoving() 
{ 
  int curr = GetCurrCombatant();
  if (curr != NO_DUDE)
    combatants[curr].StopMoving(); 
}
//*****************************************************************************
//    NAME: COMBATANT_DATA::Quick
//
// PURPOSE: 
//
//*****************************************************************************
void COMBATANT_DATA::Quick(BOOL Enable) 
{ 
  int curr = GetCurrCombatant();
  if (curr != NO_DUDE)
    combatants[curr].Quick(Enable); 
}
//*****************************************************************************
//    NAME: COMBATANT_DATA::Guard
//
// PURPOSE: 
//
//*****************************************************************************
void COMBATANT_DATA::Guard() 
{
  int curr = GetCurrCombatant();
  if (curr != NO_DUDE)
    combatants[curr].Guard(); 
}
//*****************************************************************************
//    NAME: COMBATANT_DATA::StartInitialSpellCasting
//
// PURPOSE: 
//
//*****************************************************************************
void COMBATANT_DATA::StartInitialSpellCasting(int key)
{
  int curr = currCaster;//GetCurrCombatant();
  ASSERT(curr!=NO_DUDE);
  if (curr != NO_DUDE)
    combatants[curr].StartInitialSpellCasting(key);
}
//*****************************************************************************
//    NAME: COMBATANT_DATA::EndInitialSpellCasting
//
// PURPOSE: 
//
//*****************************************************************************
void COMBATANT_DATA::EndInitialSpellCasting()
{
  int curr = currCaster;//GetCurrCombatant();
  ASSERT(curr!=NO_DUDE);
  if (curr != NO_DUDE)
    combatants[curr].EndInitialSpellCasting();
}

//*****************************************************************************
//    NAME: COMBATANT_DATA::StopSpellCasting
//
// PURPOSE: 
//
//*****************************************************************************
void COMBATANT_DATA::StopSpellCasting()
{
  int curr = currCaster;//GetCurrCombatant();
  ASSERT(curr!=NO_DUDE);
  if (curr != NO_DUDE)
  {
    combatants[curr].StopCasting(TRUE); // end turn
    //combatants[curr].EndTurn();
  }
}

//*****************************************************************************
//    NAME: COMBATANT_DATA::StartTurnUndead
//
// PURPOSE: 
//
//*****************************************************************************
void COMBATANT_DATA::StartTurnUndead()
{
  int curr = GetCurrCombatant();
  if (curr != NO_DUDE)
    combatants[curr].TurnUndead();
}

//*****************************************************************************
//    NAME: COMBATANT_DATA::TurnUndead
//
// PURPOSE: 
//
//*****************************************************************************
void COMBATANT_DATA::TurnUndead() 
{
  int curr = GetCurrCombatant();
  if (curr == NO_DUDE) return;

  if (!combatants[curr].IsPartyMember()) 
  {
    combatants[curr].EndTurn();
    return;
  }
  
  int level = combatants[curr].GetCurrentLevel(ClericFlag);
  if (isPaladin(combatants[curr].GetClass()))
  {
    int plevel = combatants[curr].GetCurrentLevel(PaladinFlag)-2;
    level = max(level,plevel);
  }

  menu.setHorzOrient();
  menu.setUseActive(FALSE); // no highlighting

  for (int i=0;i<numCombatants;i++)
  {
    if ((combatants[i].IsPartyMember())||(combatants[i].GetIsFriendly())) continue;

    int result = GetUndeadTurnValue(level, combatants[i].GetAdjUndeadType(), undeadTurnModifier);
    switch (result)
    {
    case UNDEAD_NOAFFECT:
    case UNDEAD_TURNED: 
    case UNDEAD_DESTROY:
      break;
    default:
      // should be minimum dice roll for success
      if ((result>=1)&&(result<=20))
      {
        if (result >= rollDice(20,1,0.0))
          result = UNDEAD_TURNED;
      }
      break;
    }

    CString turnMsg("");
    switch (result)
    {
    case UNDEAD_TURNED: // undead will flee the combat map (status=Running, then Fled)
      menu.setMenu(EmptyMenuData); // starts empty
      PlaceCursorOnDude(i,FALSE); // calls ensure visible
      //EnsureVisibleTarget(i);        
      UpdateCombatTimeDelayMsg();
      theApp.AppSleep(225); // give time to see normal icon before death animation


      turnMsg.Format("%s is turned by %s...", 
                     combatants[i].GetName(),combatants[curr].GetName());
      menu.changeMenuItem(1, turnMsg);
      UpdateCombatTimeDelayMsg();
      combatants[i].PlayCombatTurnUndead();

      theApp.AppSleep(500); // give time to see normal icon

      combatants[i].EndTurn();
      combatants[i].SetStatus(Running);
      combatants[i].isTurned=TRUE;
      combatants[i].lastAttacker = curr; // so it knows who to flee from
      TRACE("Turn Undead: %i is turned and fleeing\n", combatants[i].self);
      break;
    case UNDEAD_DESTROY: // undead will be removed from the map (status=Gone)
      menu.setMenu(EmptyMenuData); // starts empty
      PlaceCursorOnDude(i,FALSE); // calls ensure visible
      //EnsureVisibleTarget(i);
      UpdateCombatTimeDelayMsg();
      theApp.AppSleep(225); // give time to see normal icon before death animation
      
      turnMsg.Format("%s is destroyed by %s...", 
                     combatants[i].GetName(),combatants[curr].GetName());
      menu.changeMenuItem(1, turnMsg);
      UpdateCombatMoveScreen();        

      combatants[i].EndTurn();
      combatants[i].SetStatus(Gone);
      combatants[i].SetHitPoints(-10);
      combatants[i].blitDeadSprite(); // built-in delay at the end, plays death sound
      placeCombatant(combatants[i].x, combatants[i].y, NO_DUDE, combatants[i].width, combatants[i].height);
      TRACE("Turn Undead: %i is turned and destroyed\n", combatants[i].self);
      break;
    default:
      // do nothing
      break;
    }
  }
  PlaceCursorOnCurrentDude();
  combatants[curr].EndTurn();
}
//*****************************************************************************
//    NAME: COMBATANT_DATA::DelayAction
//
// PURPOSE: 
//
//*****************************************************************************
void COMBATANT_DATA::DelayAction() 
{ 
  int curr = GetCurrCombatant();
  if (curr != NO_DUDE)
    combatants[curr].DelayAction(); 
}
//*****************************************************************************
//    NAME: COMBATANT_DATA::AttackOver
//
// PURPOSE: 
//
//*****************************************************************************
void COMBATANT_DATA::AttackOver() 
{ 
  int curr = GetCurrCombatant();
  if (curr != NO_DUDE)
    combatants[curr].AttackOver(); 
}
//*****************************************************************************
//    NAME: COMBATANT_DATA::MakeAttack
//
// PURPOSE: 
//
//*****************************************************************************
void COMBATANT_DATA::MakeAttack() 
{ 
  int curr = GetCurrCombatant();
  if (curr != NO_DUDE)
    combatants[curr].makeAttack(combatants[curr].GetCurrTarget()); 
}
//*****************************************************************************
//    NAME: COMBATANT_DATA::StartAttack
//
// PURPOSE: 
//
//*****************************************************************************
void COMBATANT_DATA::StartAttack() 
{ 
  int curr = GetCurrCombatant();
  if (curr != NO_DUDE)
    combatants[curr].StartAttack(combatants[curr].GetCurrTarget()); 
}


//*****************************************************************************
//    NAME: COMBATANT_DATA::GetCombatState
//
// PURPOSE: 
//
//*****************************************************************************
CombatStateType COMBATANT_DATA::GetCombatState() 
{ 
  ASSERT( numCombatants > 0 );

  // first check for some special cases
  // that override the current combatant state
  //
  if (IsCombatOver())
  {
    return CST_CombatOver;
  }

  if (StartingNewCombatRound)
  {    
    return CST_StartNewRound;
  }

  if ((IsNextCombatant()) && (!IsFreeAttacker()))
  {
    return CST_CombatRoundDelay;
  }

  return (CombatStateType)GetCombatantState();
}

// have we changed to the next active combatant
//*****************************************************************************
//    NAME: COMBATANT_DATA::IsNextCombatant
//
// PURPOSE: 
//
//*****************************************************************************
BOOL COMBATANT_DATA::IsNextCombatant() 
{ 
  int dude = GetCurrCombatant();
  return ((prevRndCombatant != dude));  
}

// is there a queued combatant that is waiting to make a free
// attack.
//*****************************************************************************
//    NAME: COMBATANT_DATA::IsFreeAttacker
//
// PURPOSE: 
//
//*****************************************************************************
BOOL COMBATANT_DATA::IsFreeAttacker() 
{ 
  return ((QComb.Top() != NO_DUDE) && (!QComb.ChangeStats())); 
}

//*****************************************************************************
//    NAME: COMBATANT_DATA::HandleCombatRoundMsgExpired
//
// PURPOSE: 
//
//*****************************************************************************
void COMBATANT_DATA::HandleCombatRoundMsgExpired()
{
  int curr = GetCurrCombatant();
  if (curr != NO_DUDE)
    combatants[curr].HandleCombatRoundMsgExpired(); 
  prevRndCombatant = curr;
}

//*****************************************************************************
//    NAME: COMBATANT_DATA::OnAuto
//
// PURPOSE: 
//
//*****************************************************************************
BOOL COMBATANT_DATA::OnAuto() 
{ 
  int curr = GetCurrCombatant();
  if (curr != NO_DUDE)
    return combatants[curr].OnAuto(); 
  else
    return FALSE;
}

//*****************************************************************************
//    NAME: COMBATANT_DATA::toggleReadyItem
//
// PURPOSE: 
//
//*****************************************************************************
BOOL COMBATANT_DATA::toggleReadyItem(int item)
{
  int curr = GetCurrCombatant();
  if (curr != NO_DUDE)
    return combatants[curr].toggleReadyItem(item); 
  else
    return FALSE;
}

//*****************************************************************************
//    NAME: COMBATANT_DATA::delCharacterItem
//
// PURPOSE: 
//
//*****************************************************************************
BOOL COMBATANT_DATA::delCharacterItem(int index, int qty)
{
  int curr = GetCurrCombatant();
  if (curr != NO_DUDE)
    return combatants[curr].delCharacterItem(index, qty); 
  else
    return FALSE;
}

//*****************************************************************************
//    NAME: COMBATANT_DATA::determineAvailAttacks
//
// PURPOSE: 
//
//*****************************************************************************
void COMBATANT_DATA::determineAvailAttacks()
{
  int curr = GetCurrCombatant();
  if (curr != NO_DUDE)
    combatants[curr].determineAvailAttacks(); 
}

//*****************************************************************************
//    NAME: COMBATANT_DATA::PlayCombatDeath
//
// PURPOSE: 
//
//*****************************************************************************
void COMBATANT_DATA::PlayCombatDeath() 
{ 
  if (!PlaySoundEffects) return;
  if (!ValidSound(hCharDeathSound))
    hCharDeathSound = LoadSound(pEvent->DeathSound);
  if (!PlaySound(hCharDeathSound))
    hCharDeathSound=-1;
}
//*****************************************************************************
//    NAME: COMBATANT_DATA::PlayCombatMove
//
// PURPOSE: 
//
//*****************************************************************************
void COMBATANT_DATA::PlayCombatMove() 
{ 
  if (!PlaySoundEffects) return;
  if (!ValidSound(hCharMoveSound))
    hCharMoveSound = LoadSound(pEvent->MoveSound);
  if (!PlaySound(hCharMoveSound))
    hCharMoveSound=-1;
}
//*****************************************************************************
//    NAME: COMBATANT_DATA::PlayCombatTurnUndead
//
// PURPOSE: 
//
//*****************************************************************************
void COMBATANT_DATA::PlayCombatTurnUndead() 
{ 
  if (!PlaySoundEffects) return;
  if (!ValidSound(hTurnUndeadSound))
    hTurnUndeadSound = LoadSound(pEvent->TurnUndeadSound);
  if (!PlaySound(hTurnUndeadSound))
    hTurnUndeadSound=-1;
}

//*****************************************************************************
//    NAME: COMBATANT_DATA::LoadSounds
//
// PURPOSE: 
//
//*****************************************************************************
void COMBATANT_DATA::LoadSounds()
{
  ClearSounds();
  hCharDeathSound = LoadSound(pEvent->DeathSound);
  hCharMoveSound = LoadSound(pEvent->MoveSound);
  hTurnUndeadSound = LoadSound(pEvent->TurnUndeadSound);
}
//*****************************************************************************
//    NAME: COMBATANT_DATA::ClearSounds
//
// PURPOSE: 
//
//*****************************************************************************
void COMBATANT_DATA::ClearSounds()
{
  if (hCharDeathSound >= 0)
    ClearSound(hCharDeathSound);
  if (hCharMoveSound >= 0)
    ClearSound(hCharMoveSound);
  if (hTurnUndeadSound >= 0)
    ClearSound(hTurnUndeadSound);
  hCharDeathSound = -1;
  hCharMoveSound = -1;
  hTurnUndeadSound=-1;
}

//
// 1 round = 1 minute
// 1 round has 10 initiative steps
// 10 rounds = 1 turn
//
//*****************************************************************************
//    NAME: COMBATANT_DATA::GetCurrCombatant
//
// PURPOSE: 
//
//*****************************************************************************
int COMBATANT_DATA::GetCurrCombatant() 
{ 
  return QComb.Top(); 
}

//*****************************************************************************
//    NAME: CurrCombatTurn
//
// PURPOSE: 
//
//*****************************************************************************
int CurrCombatTurn() 
{ 
  return combatData.currTurn; 
}

//*****************************************************************************
//    NAME: CurrCombatRound
//
// PURPOSE: 
//
//*****************************************************************************
int CurrCombatRound()
{
  return combatData.currRound;
}

//*****************************************************************************
//    NAME: CurrCombatInitiative
//
// PURPOSE: 
//
//*****************************************************************************
int CurrCombatInitiative() 
{ 
  return combatData.currInitiative; 
}

//*****************************************************************************
//    NAME: COMBATANT_DATA::getCombatPartyCharacter
//
// PURPOSE: 
//
//*****************************************************************************
COMBATANT& COMBATANT_DATA::getCombatPartyCharacter(int PartyIdx)
{
  if ((PartyIdx < 0) || (PartyIdx >= party.numCharacters))
    return getActiveCombatCharacter();

  for (int i=0;i<numCombatants;i++)
  {
    if ((combatants[i].IsPartyMember()) && (combatants[i].origKey == PartyIdx))
    {
      if (combatants[GetCurrCombatant()].GetType() == MONSTER_TYPE)
        WriteDebugString("getActiveCombatCharacter() called for monster combatant\n");
      return combatants[i];
    }
  }

  ASSERT( FALSE ); // shouldn't get here
  return combatants[0];
}

//*****************************************************************************
//    NAME: *COMBATANT_DATA::getCombatantPtr
//
// PURPOSE: 
//
//*****************************************************************************
COMBATANT *COMBATANT_DATA::getCombatantPtr(int index)
{
  if ((index < 0) || (index >= numCombatants))
  {
    ASSERT(FALSE);
    return NULL;
  }
  return &combatants[index];
}

//*****************************************************************************
//    NAME: COMBATANT_DATA::ProcessTimeSensitiveData
//
// PURPOSE: 
//
//*****************************************************************************
bool COMBATANT_DATA::ProcessTimeSensitiveData(LONGLONG currTime)
{
  static int lastUpdateRound=-1;
  static int lastUpdateInitiative=-1;
  if (IsCombatOver()) return false;
  // update once per combat round
  int currRnd = CurrCombatRound();
  int currInit = CurrCombatInitiative();
  bool UpdateScreen = false;

  bool update = ((lastUpdateRound != currRnd) || (lastUpdateInitiative != currInit));
  if (update)
  {     
    if ((lastUpdateRound >= 0) || (lastUpdateInitiative >= 0))
    {
      int roundDelta = currRnd-lastUpdateRound;

      if (roundDelta > 0) // 1 round per minute
      {
        ActiveSpellData.ProcessTimeSensitiveData(roundDelta);
        PendingSpellData.ProcessTimeSensitiveData(roundDelta, currInit);        
        party.incrementClock(roundDelta);        

        for (int i=0;i<numCombatants;i++)
        {          
          combatants[i].CheckForExpiredSpellEffects();
          
          if (combatants[i].GetAdjSpecAb(SA_Poisoned))
          {
            double currhp = combatants[i].GetHitPoints();
            if (currhp > 0)
            {
              double rateofloss = 0.25;
              BOOL IsSlowed = combatants[i].GetAdjSpecAb(SA_SlowPoison);
              if (IsSlowed)              
                rateofloss = 0.12;
            
              double hploss = currhp * rateofloss;
              hploss += 0.5; // round up
              hploss = floor(hploss);
              if (hploss < 1.0)
                hploss = 1.0;
              int newhptotal = currhp - hploss;
              if ((IsSlowed) && (newhptotal < 1))
                newhptotal = 1;   
              hploss = currhp - newhptotal;              
              if (hploss > 0)
              {
                PlaceCursorOnDude(i,FALSE); // calls ensure visible
                CString msg;
                msg.Format("%s takes poison damage", combatants[i].GetName());
                if (combatants[i].IsSpellPending())
                  msg += ", pending spell is lost";
                FormatCombatText(combatTextData, msg);
                UpdateCombatTimeDelayMsg();
                combatants[i].PlayHit();
                combatants[i].TakeDamage(hploss,FALSE);
                theApp.AppSleep(GetMsgTimeDelay());
                ClearFormattedText(combatTextData);
              }
            }
          }
        }
      }
    }
    lastUpdateRound=currRnd;
    lastUpdateInitiative = currInit;
  }

  return UpdateScreen;
}

//*****************************************************************************
//    NAME: COMBATANT_DATA::GetContext
//
// PURPOSE: 
//
//*****************************************************************************
ActorType COMBATANT_DATA::GetContext(int i)
{
  if ((i>=0)&&(i<numCombatants)) return combatants[i].GetContext();
  else return NULL_ACTOR;
}

// force immediate party victory
//*****************************************************************************
//    NAME: COMBATANT_DATA::ForcePartyVictory
//
// PURPOSE: 
//
//*****************************************************************************
void COMBATANT_DATA::ForcePartyVictory()
{
  int partycnt = 0;
  for (int i=0;i<numCombatants;i++)
  {
    if ((!combatants[i].GetIsFriendly()) && (combatants[i].charOnCombatMap()) && (!combatants[i].IsPartyMember()))
    {
      PlaceCursorOnDude(i,FALSE); // calls ensure visible
      UpdateCombatTimeDelayMsg();      
      combatants[i].TakeDamage(2000000000, FALSE);
      theApp.AppSleep(GetMsgTimeDelay());
    }
    else
    {
      if (combatants[i].IsAlive())
        partycnt++;
    }
  }
  if (partycnt > 0) lastRound = TRUE;
  PlaceCursorOnCurrentDude();
}

// apply the spell effects to targets
//*****************************************************************************
//    NAME: COMBATANT_DATA::InvokePendingSpellOnCombatant
//
// PURPOSE: 
//
//*****************************************************************************
int COMBATANT_DATA::InvokePendingSpellOnCombatant(int caster, int targ, int activeSpellKey, int spellDbKey)
{  
  if (caster == NO_DUDE) return activeSpellKey;  
  if (targ == NO_DUDE) return activeSpellKey;
  
  COMBATANT &cdude = combatants[caster];
  COMBATANT &tdude = combatants[targ];
 
  int spelldbkey = spellDbKey;

  if (spelldbkey == NO_SPELL)
  {
    if (cdude.GetType()!=MONSTER_TYPE)
    {
      CHARACTER_SPELL cspell;
      if (!cdude.spells.list.GetSpell(cdude.GetSpellBookKeyBeingCast(), cspell)) 
      {
        ASSERT(FALSE);
        return activeSpellKey;
      }
      spelldbkey = cspell.spell;
    }
    else
    {
      // monsters don't have spell books
      spelldbkey = cdude.GetSpellBookKeyBeingCast();

      // better check this code when it's finally ready to be
      // executed for the first time. I'm not certain that the key will be
      // correct.
      ASSERT(FALSE);
    }
  }

  SPELL_DATA *pSdata = spellData.GetSpellData(spelldbkey);
  if (pSdata==NULL) 
  {
    ASSERT(FALSE);
    return activeSpellKey;
  }

  WriteDebugString("InvokePendingSpellOnCombatant(): caster %i, targ %i, key %i, spell %s\n", 
                    caster, targ, activeSpellKey, pSdata->Name);
  
  activeSpellKey = cdude.InvokeSpellOnTarget(tdude, activeSpellKey, spelldbkey);
  
  return activeSpellKey;
}

//*****************************************************************************
//    NAME: COMBATANT_DATA::InvokePendingSpellOnMap
//
// PURPOSE: Casting spell with area effect, and it has no current targets
//          in map area. Still need to create an active spell record.
//
//*****************************************************************************
int COMBATANT_DATA::InvokePendingSpellOnMap(int caster, int activeSpellKey)
{
  WriteDebugString("InvokePendingSpellOnMap(): caster %i, key %i\n", 
                   caster, activeSpellKey);
  
  if (caster == NO_DUDE) return -1;  
  
  COMBATANT &cdude = combatants[caster];
 
  int spelldbkey = -1;
  if (cdude.GetType()!=MONSTER_TYPE)
  {
    CHARACTER_SPELL cspell;
    if (!cdude.spells.list.GetSpell(cdude.GetSpellBookKeyBeingCast(), cspell)) 
    {
      ASSERT(FALSE);
      return -1;
    }
    spelldbkey = cspell.spell;
  }
  else
  {
    // monsters don't have spell books
    spelldbkey = cdude.GetSpellBookKeyBeingCast();
  }

  SPELL_DATA *pSdata = spellData.GetSpellData(spelldbkey);
  if (pSdata==NULL) 
  {
    ASSERT(FALSE);
    return -1;
  }

  if (activeSpellKey < 0)
  {
    if (pSdata->Duration_Rate != Instantaneous)
      activeSpellKey=ActiveSpellData.GetNextKey();
  }

  if (pSdata->Duration_Rate != Instantaneous)
  {            
    ASSERT( activeSpellKey >= 0 );    
    ASSERT( ActiveSpellData.Get(activeSpellKey) == NULL );
      
    ACTIVE_SPELL aspell;
    aspell.caster = cdude.GetContext(pSdata->ClassMask);
    aspell.key = activeSpellKey;
    aspell.spellKey = pSdata->key; // spell db key
    aspell.CountTime = 0;
    aspell.StopTime = 0;
    aspell.Lingers = pSdata->Lingers;
    aspell.lingerData.OnceOnly = pSdata->LingerOnceOnly;    

    
    // add affected map locations to active spell record
    aspell.lingerData.mapdata.RemoveAll();
    POSITION pos = cdude.targets.m_maptarglist.GetHeadPosition();
    while (pos != NULL)
      aspell.lingerData.mapdata.AddTail(cdude.targets.m_maptarglist.GetNext(pos));
    
    // add targets to active spell record so that linger spell checks
    // won't re-target same dudes once they move (unless allowed by OnceOnly flag)
    for (int i=0;i<cdude.targets.m_targets.GetSize();i++)
      aspell.lingerData.affectedtargs.AddTail(cdude.targets.m_targets[i]);
    
    WriteDebugString("Adding active spell cast by %i\n", aspell.caster.Instance);

    double val = 0.0;
    // determine duration value
    val = cdude.EvalDiceAsClass(pSdata->Duration, pSdata->ClassMask);
    ASSERT( val > 0.0 );                 

    // convert duration value into game minutes
    switch (pSdata->Duration_Rate)
    {
    case inRounds:
      val = max(1,val); // 1 round min
      aspell.StopTime = val + party.getElapsedMinutes();
      break;
    case byNbrAttacks:
    case byDamageTaken: 
      aspell.StopTime = val;
      break;
    case inHours:
      val = val * 60.0;
      val = max(1,val); // 1 round min
      aspell.StopTime = val + party.getElapsedMinutes();      
      break;
    case inDays:
      val = val * 24.0 * 60.0;
      val = max(1,val); // 1 round min
      aspell.StopTime = val + party.getElapsedMinutes();      
      break;
    default: 
      ASSERT(FALSE); 
      break;
    }

    ActiveSpellData.AddWithCurrKey(aspell);
  }

  return activeSpellKey;
}


//*****************************************************************************
//    NAME: COMBATANT_DATA::RemoveSpellEffect
//
// PURPOSE: 
//
//*****************************************************************************
void COMBATANT_DATA::RemoveSpellEffect(DWORD parent, SPELL_DATA *pSpell)
{
  for (int i=0;i<numCombatants;i++)
    combatants[i].RemoveSpellEffect(parent,pSpell);
}

//*****************************************************************************
//    NAME: *GetLastAttackerOf
//
// PURPOSE: 
//
//*****************************************************************************
COMBATANT *GetLastAttackerOf(COMBATANT *pDude)
{
  if (pDude == NULL) return NULL;
  for (int i=0;i<combatData.numCombatants;i++)
  {
    if (combatData.combatants[i].lastAttacked == pDude->self)
    {
      return (&combatData.combatants[i]);
    }
  }
  return NULL;
}

//*****************************************************************************
//    NAME: *GetLeastDamagedFriendly
//
// PURPOSE: 
//
//*****************************************************************************
COMBATANT *GetLeastDamagedFriendly()
{
  int HighestHP = -1;
  int HighestHPIndex = -1;  

  for (int i=0;i<combatData.numCombatants;i++)
  {
    if (combatData.combatants[i].GetIsFriendly())
    {
      int hp = combatData.combatants[i].GetAdjHitPoints();
      if (hp > HighestHP)
      {
        HighestHP = hp;
        HighestHPIndex = i;
      }      
    }
  }

  if (HighestHPIndex >= 0)
    return (&combatData.combatants[HighestHPIndex]);

  return NULL;  
}

//*****************************************************************************
//    NAME: *GetLeastDamagedEnemy
//
// PURPOSE: 
//
//*****************************************************************************
COMBATANT *GetLeastDamagedEnemy()
{
  int HighestHP = -1;
  int HighestHPIndex = -1;  
  
  for (int i=0;i<combatData.numCombatants;i++)
  {
    if (!combatData.combatants[i].GetIsFriendly())
    {
      int hp = combatData.combatants[i].GetAdjHitPoints();
      if (hp > HighestHP)
      {
        HighestHP = hp;
        HighestHPIndex = i;
      }      
    }
  }
  
  if (HighestHPIndex >= 0)
    return (&combatData.combatants[HighestHPIndex]);
  
  return NULL;  
}

//*****************************************************************************
//    NAME: *GetMostDamagedFriendly
//
// PURPOSE: 
//
//*****************************************************************************
COMBATANT *GetMostDamagedFriendly()
{
  int LowestHP = 999999999;
  int LowestHPIndex = -1;  
  
  for (int i=0;i<combatData.numCombatants;i++)
  {
    if (combatData.combatants[i].GetIsFriendly())
    {
      int hp = combatData.combatants[i].GetAdjHitPoints();
      if (hp < LowestHP)
      {
        LowestHP = hp;
        LowestHPIndex = i;
      }      
    }
  }
  
  if (LowestHPIndex >= 0)
    return (&combatData.combatants[LowestHPIndex]);
  
  return NULL;  
}

//*****************************************************************************
//    NAME: *GetMostDamagedEnemy
//
// PURPOSE: 
//
//*****************************************************************************
COMBATANT *GetMostDamagedEnemy()
{
  int LowestHP = 999999999;
  int LowestHPIndex = -1;  
  
  for (int i=0;i<combatData.numCombatants;i++)
  {
    if (!combatData.combatants[i].GetIsFriendly())
    {
      int hp = combatData.combatants[i].GetAdjHitPoints();
      if (hp < LowestHP)
      {
        LowestHP = hp;
        LowestHPIndex = i;
      }      
    }
  }
  
  if (LowestHPIndex >= 0)
    return (&combatData.combatants[LowestHPIndex]);
  
  return NULL;  
}


//*****************************************************************************
//    NAME: *GetNearestTo
//
// PURPOSE: 
//
//*****************************************************************************
COMBATANT *GetNearestTo(COMBATANT *pDude)
{
  if (pDude == NULL) return NULL;

  int NearestDist = 999999999;
  int NearestIndex = -1;  
  
  for (int i=0;i<combatData.numCombatants;i++)
  {
    int dist = distance(pDude->self, 
                        pDude->x, 
                        pDude->y,
                        combatData.combatants[i].self,
                        combatData.combatants[i].x,
                        combatData.combatants[i].y);
    if (dist < NearestDist)
    {
      NearestDist = dist;
      NearestIndex = i;
    }      
  }
  
  if (NearestIndex >= 0)
    return (&combatData.combatants[NearestIndex]);
  
  return NULL;  
}

//*****************************************************************************
//    NAME: *GetNearestEnemyTo
//
// PURPOSE: 
//
//*****************************************************************************
COMBATANT *GetNearestEnemyTo(COMBATANT *pDude)
{
  if (pDude == NULL) return NULL;
  
  int NearestDist = 999999999;
  int NearestIndex = -1;  
  
  for (int i=0;i<combatData.numCombatants;i++)
  {
    if (!combatData.combatants[i].GetIsFriendly())
    {
      int dist = distance(pDude->self, 
                          pDude->x, 
                          pDude->y,
                          combatData.combatants[i].self,
                          combatData.combatants[i].x,
                          combatData.combatants[i].y);

      if (dist < NearestDist)
      {
        NearestDist = dist;
        NearestIndex = i;
      }      
    }
  }
  
  if (NearestIndex >= 0)
    return (&combatData.combatants[NearestIndex]);
  
  return NULL;  
}

//*****************************************************************************
//    NAME: GetDistanceBetween_ByPartyIndex
//
// PURPOSE: 
//
//*****************************************************************************
int COMBATANT_DATA::GetDistanceBetween_ByPartyIndex(int PartyMember1Idx, int PartyMember2Idx)
{
  if (PartyMember1Idx == PartyMember2Idx)
    return 0; // distance from me to me

  int CombatantIdx1 = -1;
  int CombatantIdx2 = -1;

  for (int i=0;i<numCombatants;i++)
  {
    if (    (combatData.combatants[i].IsPartyMember())
        && ((combatData.combatants[i].GetType() == CHAR_TYPE) || (combatData.combatants[i].GetType() == NPC_TYPE))
        &&  (combatData.combatants[i].origKey >= 0))
    {
      if (combatData.combatants[i].origKey == PartyMember1Idx)
        CombatantIdx1 = i;
      else if (combatData.combatants[i].origKey == PartyMember2Idx)
        CombatantIdx2 = i;
    }
  }

  if ((CombatantIdx1 >= 0) && (CombatantIdx2 >= 0))
  {
    int dist = distance(combatData.combatants[CombatantIdx1].self, 
                        combatData.combatants[CombatantIdx1].x,
                        combatData.combatants[CombatantIdx1].y, 

                        combatData.combatants[CombatantIdx2].self, 
                        combatData.combatants[CombatantIdx2].x,
                        combatData.combatants[CombatantIdx2].y);

    return (dist);
  }

  ASSERT(FALSE);
  return 1000;
}
