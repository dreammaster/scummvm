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

#include "dungeoncraft/shared/char.h"
#include "dungeoncraft/shared/GlobalData.h"
#include "dungeoncraft/shared/party.h"
#include "dungeoncraft/shared/DispText.h"
#include "dungeoncraft/shared/Combatant.h"
#include "dungeoncraft/shared/DrawTile.h"
#include "dungeoncraft/shared/Viewport.h"
#include "dungeoncraft/shared/Screen.h"
#include "dungeoncraft/shared/GameMenu.h"
#include "dungeoncraft/shared/FormattedText.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

char *CombatantStateText[NUM_COMBATANT_STATES] = 
{
  "CS_None",
  "CS_Casting",
  "CS_Attacking",
  "CS_Guarding",
  "CS_Bandaging",
  "CS_Using",
  "CS_Moving",
  "CS_Turning",
  "CS_Fleeing"
};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//
//
//  COMBATANT functions
//
//
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

//*****************************************************************************
//    NAME: COMBATANT::Clear
//
// PURPOSE: 
//
//*****************************************************************************
void COMBATANT::Clear()
{
  CHARACTER::Clear();
  self=-1;
  x=-1; y=-1;
  hPath=-1;
  initiative=0;
  target=-1;
  facing=0;
  movement=0;
  moveX=-1;moveY=-1;
  lastAttackRound=0;
  availAttacks=0.0;
  continueAttack=FALSE;
  done=FALSE;
  isBandaged=FALSE;
  bandageWho=-1;
  origKey=-1;
  didMove=FALSE;
  state=CS_None;
  friendly=TRUE;
  blitDeathTile=FALSE;
  width=0;height=0;
  targetPos=NULL;  
  tmpAttDiceBonus=0;
  tmpDmgDiceBonus=0;
  //partyMember=FALSE;
  isFleeing = FALSE;
  isTurned=FALSE;
  hasTurnedUndead=FALSE;
  lastAttacker=-1;
  lastAttacked=-1;
  RemoveAllTargets();
  targeters.RemoveAll();
  spellBookKeyBeingCast=NO_SPELL;
  pendingSpellKey=-1;
  activeSpellKey=-1;
  forceAttackPose=FALSE;
  AttDiceSides=0;
  AttDiceNum=0;
  AttDiceRoll=0;
}

//*****************************************************************************
//    NAME: COMBATANT::operator=
//
// PURPOSE: 
//
//*****************************************************************************
COMBATANT& COMBATANT::operator=(COMBATANT& src)
{
  if (&src == this)
    return *this;

  CHARACTER::operator =(src);
  self = src.self;
	x = src.x;
  y = src.y;
  hPath = src.hPath;
	initiative = src.initiative; // 1..10
	target = src.target; // index of target
	facing = src.facing;
  movement = src.movement;
  moveX = src.moveX;
  moveY = src.moveY;
  lastAttackRound = src.lastAttackRound;
  availAttacks = src.availAttacks;
  isFleeing = src.isFleeing;
  isTurned=src.isTurned;
  hasTurnedUndead=src.hasTurnedUndead;
  lastAttacker=src.lastAttacker;
  lastAttacked=src.lastAttacked;
	done = src.done;
  isBandaged = src.isBandaged;
  bandageWho = src.bandageWho;
  origKey=src.origKey;
  didMove = src.didMove;
  state = src.state;
	friendly = src.friendly;
  blitDeathTile = src.blitDeathTile;
  width = src.width;
  height = src.height;
  targetPos = src.targetPos;
  //partyMember = src.partyMember;
  tmpAttDiceBonus = src.tmpAttDiceBonus;
  tmpDmgDiceBonus = src.tmpDmgDiceBonus;
  continueAttack=src.continueAttack;
  spellBookKeyBeingCast=src.spellBookKeyBeingCast;
  pendingSpellKey=src.pendingSpellKey;
  activeSpellKey=src.activeSpellKey;
  forceAttackPose=src.forceAttackPose;
  AttDiceSides=src.AttDiceSides;
  AttDiceNum=src.AttDiceNum;
  AttDiceRoll=src.AttDiceRoll;

  combattargets.RemoveAll();
  POSITION pos = src.combattargets.GetHeadPosition();
  while (pos != NULL)
  {
    DWORD data = src.combattargets.GetNext(pos);
    int dude = GetTargetKeyDude(data);
    combattargets.Insert(dude, data);
  }

  //targeters is a temp list that should not need to be copied

  return *this;
}

//*****************************************************************************
//    NAME: COMBATANT::GetContext
//
// PURPOSE: 
//
//*****************************************************************************
ActorType COMBATANT::GetContext(WORD ClassFlag)
{
  ActorType data;
  data.Clear();

  if (IsPartyMember())
  {
    data.SetCombatPartySrc(friendly);
  }
  else
  {
    if (GetType() == MONSTER_TYPE)
      data.SetCombatMonsterSrc(friendly);
    else if (GetType() == NPC_TYPE)
      data.SetCombatNPCSrc(friendly);
  }  

  // This character will not always have a
  // level value for ClassFlag. For instance,
  // a cleric casts a spell on a fighter. The
  // ClassFlag will be ClericFlag, and the fighter's
  // target context will have a zero cleric
  // level.
  data.Level = GetCurrentLevel(ClassFlag);
  ASSERT( (data.Level>=0) && (data.Level<=30) );

  data.Race = race();
  data.Class = GetClass();  
  data.Instance = self;

  return data;
}

//*****************************************************************************
// NAME: COMBATANT::CanCast
//
// PURPOSE:  
//*****************************************************************************
BOOL COMBATANT::CanCast()
{
  if (GetConfigNoMagic()) return FALSE;
  if ((!IsPartyMember()) && (GetConfigMonsterNoCast())) return FALSE;
  if (IsDone()) return FALSE;
  return canCastSpells();
}

//*****************************************************************************
//    NAME: COMBATANT::IsCasting
//
// PURPOSE: 
//
//*****************************************************************************
BOOL COMBATANT::IsCasting()
{
  return (state==CS_Casting);
}
//*****************************************************************************
//    NAME: COMBATANT::IsSpellPending
//
// PURPOSE: 
//
//*****************************************************************************
BOOL COMBATANT::IsSpellPending() 
{ 
  return ((IsCasting()) && (pendingSpellKey>=0)); 
}

//*****************************************************************************
//    NAME: COMBATANT::StartInitialSpellCasting
//
// PURPOSE: 
//
//*****************************************************************************
void COMBATANT::StartInitialSpellCasting(int skey)
{
  WriteDebugString("StartInitialSpellCasting for %i, sbkey %i\n", self,skey);
  forceAttackPose=FALSE;
  activeSpellKey=-1;
  targets.Clear();
  SetSpellBookKeyBeingCast(skey);
  state = CS_Casting;  
  if (!CastSpell(skey))
  {
    WriteDebugString("StartInitialSpellCasting: CastSpell() FAILED for %i, sbkey %i\n", self,skey);
    SetSpellBookKeyBeingCast(NO_SPELL);
    EndTurn();
  }
}
//*****************************************************************************
//    NAME: COMBATANT::EndInitialSpellCasting
//
// PURPOSE: 
//
//*****************************************************************************
void COMBATANT::EndInitialSpellCasting()
{
  WriteDebugString("EndInitialSpellCasting() for %i\n", self);
  EndTurn(CS_Casting);
}

//*****************************************************************************
//    NAME: COMBATANT::StopCasting
//
// PURPOSE: 
//
//*****************************************************************************
void COMBATANT::StopCasting(BOOL EndYourTurn)
{
  WriteDebugString("StopCasting() for %i, EndYourTurn %i\n", self,EndYourTurn);
  targets.Clear();  
  SetSpellBookKeyBeingCast(NO_SPELL);
  
  if (state == CS_Casting)
    state = CS_None;

  if (pendingSpellKey >= 0)
  {
    PendingSpellData.Remove(pendingSpellKey);
    pendingSpellKey=-1;
  }
  activeSpellKey=-1;
  forceAttackPose=FALSE;
  if (EndYourTurn)
    EndTurn(state);
}

//*****************************************************************************
//    NAME: COMBATANT::GetSpellBookKeyBeingCast
//
// PURPOSE: 
//
//*****************************************************************************
int COMBATANT::GetSpellBookKeyBeingCast() const 
{ 
  return spellBookKeyBeingCast; 
}

//*****************************************************************************
//    NAME: COMBATANT::SetSpellBookKeyBeingCast
//
// PURPOSE: 
//
//*****************************************************************************
void COMBATANT::SetSpellBookKeyBeingCast(int skey) 
{ 
  // NO_SPELL to clear
  spellBookKeyBeingCast=skey; 
} 

// queue up this spell on the pending list
//*****************************************************************************
//    NAME: COMBATANT::CastSpell
//
// PURPOSE: 
//
//*****************************************************************************
BOOL COMBATANT::CastSpell(int spell)
{ 
  WriteDebugString("CastSpell() for %i, sbkey %i\n", self,spell);
  ASSERT (IsCombatActive() == TRUE);

  CHARACTER_SPELL cspell;
  if (!spells.list.GetSpell(spell, cspell)) return FALSE;
  SPELL_DATA *pSdata = spellData.GetSpellData(cspell.spell);
  if (pSdata==NULL) return FALSE;

  spells.list.DecMemorized(spell, 1);

  int rnd = CurrCombatRound();

  PENDING_SPELL data;
  data.caster = GetContext();
  data.key = spell;
  data.waitUntil = max(rnd-1, 0);
  data.flags = pSdata->Casting_Time_Type;

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

  pSdata->Casting_Time = max(0, pSdata->Casting_Time);
  switch (pSdata->Casting_Time_Type)
  {
  case sctImmediate:
    data.waitUntil = initiative; // immediate activation
    break;
  case sctInitiative:
    data.waitUntil = initiative+pSdata->Casting_Time;
    if (data.waitUntil > 10) 
    {
      data.flags = sctRounds;
      data.waitUntil += (rnd+1); // wait until next round
    }
    if (data.waitUntil == (DWORD)initiative)
    {
      data.flags = sctImmediate;
      data.waitUntil = initiative;
    }
    break;
  case sctRounds:
    data.waitUntil = (rnd+pSdata->Casting_Time);
    if (data.waitUntil == (DWORD)rnd)
    {
      data.flags = sctImmediate;
      data.waitUntil = initiative;
    }
    break;
  case sctTurns:
    data.waitUntil = (rnd+(pSdata->Casting_Time*10));
    if (data.waitUntil == (DWORD)rnd)
    {
      data.flags = sctImmediate;
      data.waitUntil = initiative;
    }
    break;
  default:
    ASSERT(FALSE);
    data.flags = sctImmediate;
    data.waitUntil = initiative;
    break;
  }

  if (pSdata->Casting_Time_Type!=sctImmediate)
  {
    WriteDebugString("CastSpell() add pending spell for %i, sdbkey %i\n", self,data.key);
    pendingSpellKey = PendingSpellData.Add(data);
  }
  else
    pendingSpellKey=-1;
  return TRUE;
}


// pending spell is ready for activation
//*****************************************************************************
//    NAME: COMBATANT::SpellActivate
//
// PURPOSE: 
//
//*****************************************************************************
void COMBATANT::SpellActivate(PENDING_SPELL &data)
{
  WriteDebugString("SpellActivate() for %i, sdbkey %i\n", self,data.key);
  ASSERT(state==CS_Casting);
  done=false;
  QueuedCombatantData &qcomb = GetQueuedCombatants();
  qcomb.PushTail(self, TRUE);
  // no longer valid
  pendingSpellKey=-1;
}

// doesn't come from spell book
//*****************************************************************************
//    NAME: COMBATANT::InstantSpellActivate
//
// PURPOSE: 
//
//*****************************************************************************
void COMBATANT::InstantSpellActivate(int spelldbkey, int target)
{
  if (spelldbkey < 0) return;
  if (target == NO_DUDE) return;

  if (GetType() != MONSTER_TYPE)
  {
    ASSERT(FALSE);
    return;
  }

  COMBATANT *pTarget = GetCombatantPtr(target);
  if (pTarget==NULL)
  {
    ASSERT(FALSE);
    return;
  }
    
  SPELL_DATA *pSdata = spellData.GetSpellData(spelldbkey);
  if (pSdata==NULL) 
  {
    ASSERT(FALSE);
    return;
  } 

  SetSpellBookKeyBeingCast(spelldbkey);

  InvokeSpellOnCombatant(self, target, -1);
}

//*****************************************************************************
// NAME: COMBATANT::UpdateCombatant
//
// PURPOSE:  
//*****************************************************************************
BOOL COMBATANT::UpdateCombatant()
{
  /* this will fill up the log window if used
  WriteDebugString("updating %s combatant %u (%s)\n", 
                   OnAuto() ? "auto" : "manual",
                   self, 
                   CombatantStateText[state]);
  */

  combatantStateType oldState = state;

  if (OnAuto())
  {
    if (!Think()) 
      return FALSE; 
  }

  //HandleCurrState();

  if ((state != CS_None)&&(state!=oldState))
  {
    TRACE("done updating %s combatant %u (%s)\n", 
                     OnAuto() ? "auto" : "manual",
                     self, 
                     CombatantStateText[state]);
  }
  return TRUE;
}

//*****************************************************************************
// NAME:    COMBATANT::Think
//
// PURPOSE: 
//*****************************************************************************
// 
// can return one of these states
/*
  CS_Moving
  CS_Guarding
  CS_Attacking
*/
BOOL COMBATANT::Think()
{
  // no user input allowed while dude is on auto
  if (!IsPartyMember())
    EnableUserInput(FALSE);
  else
    EnableUserInput(TRUE);

  TRACE("thinking for auto combatant %u (%s)\n", self, CombatantStateText[state]);

  // make sure current path has not been exhausted
  if (hPath >= 0)
  {
    if (pathMgr.GetStepsRemaining(hPath)==0)
      ClearPath();
  }  

  if (isFleeing)
  {
    state = CS_Moving;
    TRACE("thinking result for auto combatant %u (%s=fleeing)\n", self, CombatantStateText[state]);
    // find path to closest map edge

    BOOL fled=FALSE;
    // if already on map edge, flee off the map
    if (   (x==0) || (x==(MAX_TERRAIN_WIDTH-1))
        || (y==0) || (y==(MAX_TERRAIN_HEIGHT-1)) )
    {
      fled=TRUE;
    }

    if (fled) 
    {
      if (friendly)
        IncNumFriendFlee(); //combatData.numFrndFlee++;
      else
        IncNumMonsterFlee(); //combatData.numMonFlee++;

      menu.setMenu(EmptyMenuData); // starts empty
      menu.setHorzOrient();
      menu.setUseActive(FALSE); // no highlighting      

      CString msg("");
      msg.Format("%s has fled...", GetName());
      menu.changeMenuItem(1, msg);
      UpdateCombatTimeDelayMsg();
      theApp.AppSleep(GetMsgTimeDelay()); // give time to see normal icon      

      EndTurn();
      SetStatus(Fled);
      placeCombatant(x, y, NO_DUDE, width, height);
      TRACE("combatant %i has fled the map\n", self);
      return TRUE;
    }
    if (CanMove())
    {
      if (hPath < 0)
      {
        // find path to farthest corner of room away from cleric that turned me
        if (!FindPathAwayFrom(lastAttacker))    
        {
          WriteDebugString("Fleeing combatant %i failed to find exit path\n", self);
          state = CS_Guarding;
        }
      }
    }
    else
      state = CS_Guarding;    
    return TRUE;
  }

  if (isTurned)
  {
    state = CS_Moving;
    TRACE("thinking result for auto combatant %u (%s=turned/fleeing %i)\n", 
                     self, CombatantStateText[state], lastAttacker);

    BOOL fled=FALSE;
    // if already on map edge, flee off the map
    if (   (x==0) || (x==(MAX_TERRAIN_WIDTH-1))
        || (y==0) || (y==(MAX_TERRAIN_HEIGHT-1)) )
    {
      fled=TRUE;
    }

    if (fled) 
    {
      if (friendly)
        IncNumFriendFlee(); //combatData.numFrndFlee++;
      else
        IncNumMonsterFlee(); //combatData.numMonFlee++;

      menu.setMenu(EmptyMenuData); // starts empty
      menu.setHorzOrient();
      menu.setUseActive(FALSE); // no highlighting      

      CString msg("");
      msg.Format("%s has fled...", GetName());
      menu.changeMenuItem(1, msg);
      UpdateCombatTimeDelayMsg();
      theApp.AppSleep(GetMsgTimeDelay()); // give time to see normal icon      

      EndTurn();
      SetStatus(Fled);
      placeCombatant(x, y, NO_DUDE, width, height);
      TRACE("Turned Undead %i has fled the map\n", self);
      return TRUE;
    }
    if (CanMove())
    {
      if (hPath < 0)
      {
        // find path to farthest corner of room away from cleric that turned me
        if (!FindPathAwayFrom(lastAttacker))    
        {
          WriteDebugString("Fleeing combatant %i failed to find exit path\n", self);
          state = CS_Guarding;
        }
      }
    }
    else
      state = CS_Guarding;
    return TRUE;
  }

  // make sure current target is still on the map
  int dude = GetCurrTarget();
  COMBATANT *tempCOMBATANT=NULL;
  if (dude != NO_DUDE)
  {
    tempCOMBATANT = GetCombatantPtr(dude);
    ASSERT(tempCOMBATANT!=NULL);
    if (tempCOMBATANT==NULL) return FALSE;

    if (!canAttack(dude))
    {
      // target is not attackable for some reason,
      // so force new target acquisition
      dude = NO_DUDE;
    }
  }

  if (   (dude == NO_DUDE) 
      || ((tempCOMBATANT != NULL) && (!tempCOMBATANT->charOnCombatMap())))
  {
    // if no target or it's gone, acquire new target (if any)
    RemoveAllTargets();

    int i;
    // try for combat targets with line of sight first
    // combat targets are ordered by distance, but the closest
    // target in terms of distance may be on the other side
    // of a wall. Using line of sight helps to ensure we will attack
    // closest target that is also the shortest path distance.
    for (i=0; i < GetNumCombatants()/*combatData.numCombatants*/; i++)
    {
      tempCOMBATANT = GetCombatantPtr(i);
      ASSERT(tempCOMBATANT!=NULL);
      if (tempCOMBATANT==NULL) return FALSE;
      if (   (i != self) 
          && (tempCOMBATANT->friendly != GetIsFriendly())
          && (tempCOMBATANT->charOnCombatMap()))
      {
        if (HaveLineOfSight(GetCenterX(),GetCenterY(), tempCOMBATANT->GetCenterX(), tempCOMBATANT->GetCenterY()))
          AddTarget(i);
      }
    }

    if (GetCurrTarget() == NO_DUDE)
    {
      // no combat targets within line of sight
      // so use any avail target
      for (i=0; i < GetNumCombatants()/*combatData.numCombatants*/; i++)
      {
        tempCOMBATANT = GetCombatantPtr(i);
        ASSERT(tempCOMBATANT!=NULL);
        if (tempCOMBATANT==NULL) return FALSE;
        if (   (i != self) 
            && (tempCOMBATANT->friendly != GetIsFriendly())
            && (tempCOMBATANT->charOnCombatMap()))
          AddTarget(i);
      }
    }
  }  

  // might use current target when determining best
  // items to make ready.
  ReadyBestArmor();
  ReadyBestShield();
  ReadyBestWpn(GetCurrTarget());

  dude = GetCurrTarget();

  // no combattargets, sit tight and guard
  if (dude == NO_DUDE)
  {
    state = CS_Guarding;
    TRACE("thinking result for auto combatant %u (%s) - no targets\n", self, CombatantStateText[state]);
    return TRUE;
  }
  
  BOOL CanAttack = canAttack(dude);
  tempCOMBATANT = GetCombatantPtr(dude);
  ASSERT(tempCOMBATANT!=NULL);
  if (tempCOMBATANT==NULL) return FALSE;
  BOOL found = FALSE;

  // no need to find a path if we can attack from current spot
  if (!CanAttack)
  {    
    BOOL repath = TRUE;

    // check to see if existing path to target needs to change
    CPathFinder* pathPtr = pathMgr.GetPath(hPath);

    if (pathPtr != NULL) 
    {
      CPoint stepPtr = pathPtr->GetLastStep();    

      if (   (tempCOMBATANT->x == stepPtr.x)
          && (tempCOMBATANT->y == stepPtr.y))
      {
        // use same path to target
        repath = FALSE;
        found = TRUE;
        TRACE("Using same path for combatant %i to target %i\n", self, dude);
      }
      else
        TRACE("Existing path for combatant %i does not end at curr target, re-pathing\n", self);
    }        

    // find path to closest enemy
    // need to change to closest enemy with shortest path!
    if ((repath) && (CanMove()))
    {
      ClearPath();
      SetCurrTarget(); // setup for iteration
      int targetCount = 0;
      dude = GetCurrTarget();

      while (   (!found) 
             && (dude != NO_DUDE) 
             && (targetCount < GetNumTargets()))
      {
        tempCOMBATANT = GetCombatantPtr(dude);
        ASSERT(tempCOMBATANT!=NULL);
        if (tempCOMBATANT==NULL) return FALSE;
        if (FindPathTo(tempCOMBATANT->x,
                       tempCOMBATANT->y, TRUE))
        {
          found = TRUE;
          RemoveAllTargets();
          AddTarget(dude);
        }
        else
        {
          dude = GetNextTarget();
        }
        targetCount++;
      }
    }
  }

  if (!found) // do we have a path to take?
  {
    if (GetCurrTarget() == NO_DUDE)
    {
      state = CS_Guarding;
    }
    else
    {
      if (CanAttack)
        state = CS_Attacking;
      else
        state = CS_Guarding;
    }
  }
  else
    state = CS_Moving;

  TRACE("thinking result for auto combatant %u (%s)\n", self, CombatantStateText[state]);
  return TRUE;
}


//*****************************************************************************
// NAME: COMBATANT::CheckMorale
//
// PURPOSE:  
//*****************************************************************************
void COMBATANT::CheckMorale()
{
  if ((isFleeing)||(isTurned))
    return;

  // monsters with no INT never flee
  if ((GetType() == MONSTER_TYPE) && (GetAdjInt() <= 1))
    return;

  static int fflee = 0;
  static int fslain =0;
  static int mflee = 0;
  static int mslain = 0;
  static BOOL SuperiorForceModMade = FALSE;

  int mod = 0;
  int numParty, numMons;
  BOOL Flee = FALSE;

  GetNumCombatants(numParty,numMons);
  //combatData.getNumCombatants(numParty, numMons);

  if (friendly)
  {
    if (fflee != GetNumFriendFlee() /*combatData.numFrndFlee*/)
      mod += ((GetNumFriendFlee()-fflee) * 5);
    if (fslain != GetNumFriendSlain())
      mod += ((GetNumFriendSlain()-fslain) * 10);
    if (((numParty * 3) <= numMons) && (!SuperiorForceModMade))
    {
      mod += 20;
      SuperiorForceModMade = TRUE;
    }
  }
  else
  {
    if (mflee != GetNumMonsterFlee()/*combatData.numMonFlee*/)
      mod += ((GetNumMonsterFlee()-mflee) * 5);
    if (mslain != GetNumMonsterSlain())
      mod += ((GetNumMonsterSlain()-mslain) * 10);
    if (((numMons * 3) <= numParty) && (!SuperiorForceModMade))
    {
      mod += 20;
      SuperiorForceModMade = TRUE;
    }
  }

  SetMorale( GetMorale()-mod );

  //int cur_morale = GetAdjMorale();
  Flee = FALSE; //(rollDice(100, 1, 0) > cur_morale);

  if (Flee)
  { 
    // only auto combatants choose to flee
    if (OnAuto())
    {      
      if (!IsPartyMember())
      {
        if (!GetConfigMonsterNoFlee())
          isFleeing = TRUE;
      }
      else
        isFleeing = TRUE;
    }
  }

  if (isFleeing)
  {
    ClearPath();
    RemoveAllTargets();
    SetStatus(Running);
    state = CS_Moving;
    // stop any casting in progress
    StopCasting(FALSE);
  }

  fflee = GetNumFriendFlee();
  fslain = GetNumFriendSlain();
  mflee = GetNumMonsterFlee();
  mslain = GetNumMonsterSlain();
}

//*****************************************************************************
// NAME: COMBATANT::toggleReadyItem
//
// PURPOSE:  
//*****************************************************************************
BOOL COMBATANT::toggleReadyItem(int item)
{  
  // nbr attacks remaining
  double attacksRemaining = availAttacks;

  // nbr attacks with current weapon
  determineAvailAttacks();
  double oldAttacks = availAttacks;

  // how many attacks made prior to changing to new item
  double attacksUsed = oldAttacks-attacksRemaining;

  if (CHARACTER::toggleReadyItem(item))
  {
    determineAvailAttacks();
    availAttacks -= attacksUsed;
    if (availAttacks < 0.0) availAttacks = 0.0;
    return TRUE;
  }
  else // nothing changed, restore original data
    availAttacks = attacksRemaining;

  return FALSE;
}

//*****************************************************************************
//    NAME: COMBATANT::delCharacterItem
//
// PURPOSE: 
//
//*****************************************************************************
BOOL COMBATANT::delCharacterItem(int index, int qty)
{
  if (CHARACTER::delCharacterItem(index, qty))
  {
    determineAvailAttacks();
    return TRUE;
  }
  return FALSE;
}

//*****************************************************************************
//    NAME: COMBATANT::CurrMoveWouldFleeMap
//
// PURPOSE: 
//
//*****************************************************************************
BOOL COMBATANT::CurrMoveWouldFleeMap()
{
  if ((moveX>=0)&&(moveY>=0))
    return (coordsOnMap(moveX, moveY, width, height) == FALSE);
  return FALSE;
}

//*****************************************************************************
//    NAME: COMBATANT::HandleCurrState
//
// PURPOSE: 
//
//*****************************************************************************
void COMBATANT::HandleCurrState()
{
  int dude;
  switch (state)
  {
  case CS_None:
    break;
  case CS_Casting:
    break;   
  case CS_Using:
    break;
  case CS_Guarding:
    //if (!CanGuard())
    //  EndTurn();
    break;
  case CS_Bandaging:    
    //if (!CanBandage())
    //  EndTurn();
    break;
  case CS_Turning:
    //if (!CanTurnUndead())
    //  EndTurn();
    break;

  case CS_Attacking:
    dude = GetCurrTarget();
    if (dude == NO_DUDE)
      EndTurn();
    else if (!canAttack(dude))
      EndTurn();
    break;

  case CS_Moving:    
    if (hPath < 0) 
    {
      // if actually have a destination
      if ((moveX>=0)&&(moveY>=0))
      {
        if (coordsOnMap(moveX, moveY, width, height))
        {
          // find path to moveX, moveY
          FindPathTo(moveX, moveY, TRUE);
        }
        else 
        {
          // This guy is fleeing the map. Paths of length 1 result
          // in instant flee. Longer paths need to move dude to
          // edge of map before flee is performed.
          isFleeing = TRUE;
          
          if (friendly)
            IncNumFriendFlee(); //combatData.numFrndFlee++;
          else
            IncNumMonsterFlee(); //combatData.numMonFlee++;
          
          menu.setMenu(EmptyMenuData); // starts empty
          menu.setHorzOrient();
          menu.setUseActive(FALSE); // no highlighting      
          
          CString msg("");
          msg.Format("%s has fled...", GetName());
          menu.changeMenuItem(1, msg);
          UpdateCombatTimeDelayMsg();
          theApp.AppSleep(GetMsgTimeDelay()); // give time to see flee message      
          
          EndTurn();
          SetStatus(Fled);
          placeCombatant(x, y, NO_DUDE, width, height);
          TRACE("%i has fled the map\n", self);
        }
      }
    }

    if (hPath >= 0)
    {
      if (!TakeNextStep())
      {
        // unable to move/attack target
        if ((OnAuto()) && (state == CS_Moving))
        {
          // end your turn but continue
          // guarding if possible          
          if (CanGuard())
          {
            TRACE("Auto combatant %i can't move, ending turn and guarding\n", self);
            EndTurn(CS_Guarding);
          }
          else
          {
            TRACE("Auto combatant %i can't move, ending turn\n", self);
            EndTurn();
          }
        }
      }
    }
//    else // can't find a path
//      EndTurn();

    moveX = -1; moveY = -1;
    if (state != CS_Moving)
       ClearPath();
    break;

  default:
    break;
  }
}

//*****************************************************************************
// NAME: COMBATANT::TakeNextStep
//
// PURPOSE:  
//*****************************************************************************
BOOL COMBATANT::TakeNextStep()
{
  BOOL isUpdate = FALSE;
  CPathFinder *pathPtr = pathMgr.GetPath(hPath);

  if (pathPtr != NULL) 
  {
    CPoint stepPtr = pathPtr->GetNextStep();

    if (stepPtr.x >= 0)
    {
      TRACE("%u taking next step of path %u to %u,%u\n", 
                  self, hPath, stepPtr.x, stepPtr.y);
      isUpdate = MoveCombatant(stepPtr.x, stepPtr.y);
    }
    else
    {
      WriteDebugString("Bogus step data for path %u in TakeNextStep()\n", hPath);
      ClearPath();
    }

    if ((pathPtr->GetStepCount() <= 0) || (!isUpdate))
      ClearPath();
  }
  else
  {
    WriteDebugString("Bogus hPath %u in TakeNextStep()\n", hPath);
    ClearPath();
  }

  return isUpdate;
}

//*****************************************************************************
// NAME: COMBATANT::ReadyBestWpn
//
// PURPOSE: Select best weapon for auto dudes
//*****************************************************************************
void COMBATANT::ReadyBestWpn(int targ)
{
  BOOL isLargeTarget=FALSE;
  int dist=1;

  // get distance to target
  if (targ == NO_DUDE)
  {
    // no combat targets, nobody to shoot at, so just ready 
    // a hand-to-hand weapon
    dist = 1;
    isLargeTarget = FALSE;
  }
  else
  {
    COMBATANT *targCOMBATANT;
    targCOMBATANT = GetCombatantPtr(targ);
    ASSERT(targCOMBATANT!=NULL);
    if (targCOMBATANT!=NULL)
    {
      isLargeTarget = targCOMBATANT->isLargeDude();
      dist = distance(self, x, y, 
                      targCOMBATANT->self, targCOMBATANT->x, targCOMBATANT->y);  
    }
  }  

  CHARACTER::ReadyBestWpn(dist, isLargeTarget);

  // if target is distant but don't have distance weapon
  // then ready hand-to-hand weapon
  if ((dist > 1) && (myItems.GetReady(WeaponHand) == NO_READY_ITEM))
    CHARACTER::ReadyBestWpn(1, isLargeTarget);
}

//*****************************************************************************
// NAME:    COMBATANT::PostCombatScreenUpdate
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
void COMBATANT::PostCombatScreenUpdate()
{
  // this is called after primary screen has been
  // blitted
}

//*****************************************************************************
// NAME:    COMBATANT::PlayCombatMove
//
// PURPOSE: 
//
//*****************************************************************************
void COMBATANT::PlayCombatMove()
{
  if (GetType() == MONSTER_TYPE)
    monsterData.PlayMove(origKey);
  else
    ::PlayCombatMove(); //combatData.PlayCombatMove();
}

//*****************************************************************************
//    NAME: COMBATANT::PlayCombatTurnUndead
//
// PURPOSE: 
//
//*****************************************************************************
void COMBATANT::PlayCombatTurnUndead()
{
  if (GetType() == MONSTER_TYPE) return;
  ::PlayCombatTurnUndead(); //combatData.PlayCombatTurnUndead();
}

//*****************************************************************************
// NAME:    COMBATANT::PlayCombatDeath
//
// PURPOSE: 
//
//*****************************************************************************
void COMBATANT::PlayCombatDeath()
{
  if (GetType() == MONSTER_TYPE)
    monsterData.PlayDeath(origKey);
  else
    ::PlayCombatDeath();//combatData.PlayCombatDeath();
}

//*****************************************************************************
// NAME: COMBATANT::ClearPath
//
// PURPOSE:  
//*****************************************************************************
void COMBATANT::ClearPath()
{
  //TRACE("Combatant %i ClearPath(%i)\n", self, hPath);
  if (hPath >= 0) 
    pathMgr.FreePath(hPath); 
  hPath = -1; 
}


//*****************************************************************************
// NAME: COMBATANT::GetCurrTarget
//
// PURPOSE:  
//*****************************************************************************
int COMBATANT::GetCurrTarget()
{ 
  if (target != NO_DUDE)
  {    
    COMBATANT *targCOMBATANT;
    targCOMBATANT = GetCombatantPtr(target);
    if (targCOMBATANT==NULL) return NO_DUDE;
    if (!targCOMBATANT->charOnCombatMap())
      RemoveCurrTarget();
  }
  return target; 
}

//*****************************************************************************
// NAME: StartAttack
//
// PURPOSE:  
//*****************************************************************************
BOOL COMBATANT::StartAttack(int targ)
{
   ASSERT(self != NO_DUDE);
   continueAttack = FALSE;
   if (targ == NO_DUDE)
   {
     ASSERT(FALSE);
     WriteDebugString("StartAttack() for %i called with invalid target\n", self);
     return FALSE;
   }
   if (availAttacks <= 0) 
   {
     ASSERT(FALSE);
     WriteDebugString("StartAttack() for %i called with availAttacks=0\n", self);
     return FALSE;
   }
   if (!charOnCombatMap())
   {
     ASSERT(FALSE);
     WriteDebugString("StartAttack() for %i called when combatant not on map\n", self);
     return FALSE;
   }

   state = CS_Attacking;
   StopCasting(FALSE);
   //PlaceCursorOnCurrentDude();
   EnsureVisibleTarget(targ);
   continueAttack=TRUE;
   PlayLaunch();
   return TRUE;
}

//*****************************************************************************
//    NAME: COMBATANT::NeedMissileAnimation
//
// PURPOSE: 
//
//*****************************************************************************
BOOL COMBATANT::NeedMissileAnimation()
{
  COMBATANT *targCOMBATANT;
  targCOMBATANT = GetCombatantPtr(target);
  ASSERT(targCOMBATANT!=NULL);
  if (targCOMBATANT==NULL) return FALSE;
  BOOL result = (distance(self, x, y, 
                   target, targCOMBATANT->x, targCOMBATANT->y) > 1);
  TRACE("NeedMissileAnimation() %i for combatant %i, target %i\n", result,self,target);
  return result;
}

//*****************************************************************************
//    NAME: COMBATANT::InitMissileAnimation
//
// PURPOSE: 
//
//*****************************************************************************
void COMBATANT::InitMissileAnimation()
{
  combatSprite.Clear();
  // lookup itemData key to determine missile animation image for
  // the weapon being used.
  int itemdbkey = myItems.GetItem(myItems.GetReady(WeaponHand));

  if (WpnUsesAmmo(itemdbkey))
    itemdbkey = myItems.GetItem(myItems.GetReady(AmmoQuiver));

  ITEM_DATA *data = itemData.GetItemData(itemdbkey);
  if (data == NULL)
  {
    WriteDebugString("InitMissileAnimation() for combatant %i failed item data lookup\n", self);
    return;  
  }

  COMBATANT *targCOMBATANT;
  targCOMBATANT = GetCombatantPtr(target);
  ASSERT(targCOMBATANT!=NULL);
  if (targCOMBATANT==NULL) return;
  combatSprite.Init(data->MissileArt, 
                    TRUE, // use path
                    "it_",
                    GetCenterX(), GetCenterY(), 
                    targCOMBATANT->GetCenterX(), targCOMBATANT->GetCenterY()); 
  EnsureVisibleTarget(self);
}

//*****************************************************************************
//    NAME: COMBATANT::NeedHitAnimation
//
// PURPOSE: 
//
//*****************************************************************************
BOOL COMBATANT::NeedHitAnimation()
{
  int wpn = myItems.GetReady(WeaponHand);
  if (wpn == NO_READY_ITEM) return FALSE;
  
  int index = myItems.GetItem(wpn);
  if (WpnUsesAmmo(index))
    index = myItems.GetItem(myItems.GetReady(AmmoQuiver));
  ITEM_DATA *data = itemData.GetItemData(index);
  if (data == NULL)
  {
    WriteDebugString("NeedHitAnimation() for combatant %i failed item data lookup\n", self);
    return FALSE;  
  }
  return (data->HitArt.filename.IsEmpty() == FALSE);
}
//*****************************************************************************
//    NAME: COMBATANT::InitHitAnimation
//
// PURPOSE: 
//
//*****************************************************************************
void COMBATANT::InitHitAnimation()
{
  combatSprite.Clear();
  // lookup itemData key to determine hit animation image for
  // the weapon being used.
  int index = myItems.GetItem(myItems.GetReady(WeaponHand));

  if (WpnUsesAmmo(index))
    index = myItems.GetItem(myItems.GetReady(AmmoQuiver));

  ITEM_DATA *data = itemData.GetItemData(index);
  if (data == NULL)
  {
    WriteDebugString("InitHitAnimation() for combatant %i failed item data lookup\n", self);
    return;  
  }

  COMBATANT *targCOMBATANT;
  targCOMBATANT = GetCombatantPtr(target);
  ASSERT(targCOMBATANT!=NULL);
  if (targCOMBATANT==NULL) return;
  combatSprite.Init(data->HitArt, 
                    FALSE, // is stationary
                    "it_",
                    targCOMBATANT->GetCenterX(), targCOMBATANT->GetCenterY(),
                    0,0);

  EnsureVisibleTarget(target);
}

//*****************************************************************************
//    NAME: COMBATANT::NeedSpellCastAnimation
//
// PURPOSE: 
//
//*****************************************************************************
BOOL COMBATANT::NeedSpellCastAnimation()
{
  CHARACTER_SPELL cspell;
  if (!spells.list.GetSpell(GetSpellBookKeyBeingCast(), cspell)) return FALSE;
  SPELL_DATA *pSdata = spellData.GetSpellData(cspell.spell);
  if (pSdata==NULL) return FALSE;
  return (pSdata->CastArt.filename.IsEmpty() == FALSE);
}

//*****************************************************************************
//    NAME: COMBATANT::InitSpellCastAnimation
//
// PURPOSE: 
//
//*****************************************************************************
void COMBATANT::InitSpellCastAnimation(int targ)
{
  combatSprite.Clear();

  CHARACTER_SPELL cspell;
  if (!spells.list.GetSpell(GetSpellBookKeyBeingCast(), cspell)) return;
  SPELL_DATA *pSdata = spellData.GetSpellData(cspell.spell);
  if (pSdata==NULL) return;

  combatSprite.Init(pSdata->CastArt, 
                    FALSE, // is stationary
                    "spl_",
                    GetCenterX(), GetCenterY(),
                    0,0);

  EnsureVisibleTarget(self, TRUE);
}

//*****************************************************************************
//    NAME: COMBATANT::NeedSpellInRouteAnimation
//
// PURPOSE: 
//
//*****************************************************************************
BOOL COMBATANT::NeedSpellInRouteAnimation(int mapx, int mapy)
{
  CHARACTER_SPELL cspell;
  if (!spells.list.GetSpell(GetSpellBookKeyBeingCast(), cspell)) return FALSE;
  SPELL_DATA *pSdata = spellData.GetSpellData(cspell.spell);
  if (pSdata==NULL) return FALSE;
  if ((x==mapx)&&(y==mapy)) return FALSE;
  return (pSdata->MissileArt.filename.IsEmpty() == FALSE);  
}

//*****************************************************************************
//    NAME: COMBATANT::InitSpellInRouteAnimation
//
// PURPOSE: 
//
//*****************************************************************************
void COMBATANT::InitSpellInRouteAnimation(int mapx, int mapy)
{
  combatSprite.Clear();
  CHARACTER_SPELL cspell;
  if (!spells.list.GetSpell(GetSpellBookKeyBeingCast(), cspell)) return;
  SPELL_DATA *pSdata = spellData.GetSpellData(cspell.spell);
  if (pSdata==NULL) return;

  FaceLocation(mapx,mapy);

  if (targets.m_type != AreaLinePickStart)
  {
    // animation starts at caster
    combatSprite.Init(pSdata->MissileArt, 
                      TRUE, // use path
                      "spl_",
                      GetCenterX(), GetCenterY(), 
                      mapx, mapy); 

    EnsureVisibleTarget(self);
  }
  else
  {
    // animation starts at mapx,mapy
    int sx = mapx;
    int sy = mapy;
    int length = targets.m_MaxRange;
    PATH_DIR dir = GetSpellTargetingDir(GetCenterX(),GetCenterY(), mapx,mapy);
    int dx=sx,dy=sy;

    switch (dir)
    {
    case PathNorth:
      dx = sx;
      dy = sy-length;
      break;
    case PathNE:
      dx = sx+length;
      dy = sy-length;
      break;
    case PathEast:
      dx = sx+length;
      dy = sy;
      break;
    case PathSE:
      dx = sx+length;
      dy = sy+length;
      break;
    case PathSouth:
      dx = sx;
      dy = sy+length;
      break;
    case PathSW:
      dx = sx-length;
      dy = sy+length;
      break;
    case PathWest:
      dx = sx-length;
      dy = sy;
      break;
    case PathNW:
      dx = sx-length;
      dy = sy-length;
      break;
    }

    combatSprite.Init(pSdata->MissileArt, 
                      TRUE, // use path
                      "spl_",
                      sx, sy, 
                      dx, dy); 

    EnsureVisible(mapx, mapy, FALSE);
  }  
}

//*****************************************************************************
//    NAME: COMBATANT::NeedSpellCoverageAnimation
//
// PURPOSE: 
//
//*****************************************************************************
BOOL COMBATANT::NeedSpellCoverageAnimation()
{
  CHARACTER_SPELL cspell;
  if (!spells.list.GetSpell(GetSpellBookKeyBeingCast(), cspell)) return FALSE;
  SPELL_DATA *pSdata = spellData.GetSpellData(cspell.spell);
  if (pSdata==NULL) return FALSE;
  return (pSdata->TargCoverageArt.filename.IsEmpty() == FALSE);
}

//*****************************************************************************
//    NAME: COMBATANT::InitSpellCoverageAnimation
//
// PURPOSE: 
//
//*****************************************************************************
void COMBATANT::InitSpellCoverageAnimation(int mapx, int mapy)
{
  combatSprite.Clear();

  CHARACTER_SPELL cspell;
  if (!spells.list.GetSpell(GetSpellBookKeyBeingCast(), cspell)) return;
  SPELL_DATA *pSdata = spellData.GetSpellData(cspell.spell);
  if (pSdata==NULL) return;

  if (pSdata->TargCoverageArt.style == PIC_DATA::AS_Directional)
  {
    combatSprite.Init(pSdata->TargCoverageArt, 
                      "spl_",
                      mapx, mapy,
                      GetSpellTargetingDir(GetCenterX(), GetCenterY(), mapx,mapy));
    combatSprite.picData.AllowCentering=FALSE;
  }
  else
  {
    combatSprite.Init(pSdata->TargCoverageArt, 
                      FALSE, // is stationary
                      "spl_",
                      mapx, mapy,
                      0,0);
  }

  EnsureVisible(mapx, mapy, TRUE);
}

//*****************************************************************************
//    NAME: COMBATANT::NeedSpellHitAnimation
//
// PURPOSE: 
//
//*****************************************************************************
BOOL COMBATANT::NeedSpellHitAnimation()
{
  CHARACTER_SPELL cspell;
  if (!spells.list.GetSpell(GetSpellBookKeyBeingCast(), cspell)) return FALSE;
  SPELL_DATA *pSdata = spellData.GetSpellData(cspell.spell);
  if (pSdata==NULL) return FALSE;
  return (pSdata->TargHitArt.filename.IsEmpty() == FALSE);
}

//*****************************************************************************
//    NAME: COMBATANT::InitSpellHitAnimation
//
// PURPOSE: 
//
//*****************************************************************************
void COMBATANT::InitSpellHitAnimation(int targ)
{
  combatSprite.Clear();

  CHARACTER_SPELL cspell;
  if (!spells.list.GetSpell(GetSpellBookKeyBeingCast(), cspell)) return;
  SPELL_DATA *pSdata = spellData.GetSpellData(cspell.spell);
  if (pSdata==NULL) return;

  COMBATANT *targCOMBATANT;
  targCOMBATANT = GetCombatantPtr(targ);
  ASSERT(targCOMBATANT!=NULL);
  if (targCOMBATANT==NULL) return;
  combatSprite.Init(pSdata->TargHitArt, 
                    FALSE, // is stationary
                    "spl_",
                    targCOMBATANT->GetCenterX(), targCOMBATANT->GetCenterY(),
                    0,0);

  EnsureVisibleTarget(targ);
}

//*****************************************************************************
//    NAME: COMBATANT::NeedSpellLingerAnimation
//
// PURPOSE: 
//
//*****************************************************************************
BOOL COMBATANT::NeedSpellLingerAnimation()
{
  CHARACTER_SPELL cspell;
  if (!spells.list.GetSpell(GetSpellBookKeyBeingCast(), cspell)) return FALSE;
  SPELL_DATA *pSdata = spellData.GetSpellData(cspell.spell);
  if (pSdata==NULL) return FALSE;
  if (!pSdata->Lingers) return FALSE;
  return (pSdata->TargLingerArt.filename.IsEmpty() == FALSE);
}

//*****************************************************************************
//    NAME: COMBATANT::InitSpellLingerAnimation
//
// PURPOSE: 
//
//*****************************************************************************
void COMBATANT::InitSpellLingerAnimation(int mapx, int mapy)
{
  //combatSprite.Clear();
  
  CHARACTER_SPELL cspell;
  if (!spells.list.GetSpell(GetSpellBookKeyBeingCast(), cspell)) return;
  SPELL_DATA *pSdata = spellData.GetSpellData(cspell.spell);
  if (pSdata==NULL) return;
  if (targets.m_maptarglist.GetCount() == 0)
  {
    ASSERT(FALSE);
    return;
  }

  POSITION pos = targets.m_maptarglist.GetHeadPosition();
  while (pos != NULL)
  {
    COMBAT_SPRITE_DATA *pSprite = new COMBAT_SPRITE_DATA;
    pSprite->Init(pSdata->TargLingerArt, 
                  "spl_",
                  targets.m_maptarglist.GetAt(pos).x, targets.m_maptarglist.GetAt(pos).y);

    WriteDebugString("Adding spell animation with active spell key %i\n", activeSpellKey);
    pSprite->activeSpellKey = activeSpellKey;
    AnimateList.AddTail(pSprite);

    targets.m_maptarglist.GetNext(pos);
  }
  
  EnsureVisible(targets.m_MapTargetX,
                targets.m_MapTargetY, 
                TRUE);
}

//*****************************************************************************
// NAME: makeAttack
//
// PURPOSE:  
//*****************************************************************************
void COMBATANT::makeAttack(int targ)
{
   ASSERT(self != NO_DUDE);
   CombatMsg="";
   ClearFormattedText(combatTextData);
    
   if (targ == NO_DUDE) return;
   if (availAttacks <= 0) return;

   int thaco;//, ac;
   int dmgDiceQty, dmgDiceSides, dmgDiceBonus;
   int attDiceBonus;
   BOOL IsNonLethal;
   
   EnsureVisibleTarget(targ);

   lastAttackRound = GetCurrentRound();
   state = CS_Attacking;
   StopCasting(FALSE);
 
   // make sure combatants are facing towards each other
   FaceAttacker(targ);
   COMBATANT *targCOMBATANT;
   targCOMBATANT = GetCombatantPtr(targ);
   ASSERT(targCOMBATANT!=NULL);
   if (targCOMBATANT==NULL) return;
   if (targCOMBATANT->GetStatus()==Dead) return; // not on the map anymore
   targCOMBATANT->FaceAttacker(self);   


  BOOL decQty = FALSE;
  int wpn = myItems.GetReady(WeaponHand);
  int itemkey=NO_ITEM;

  if (wpn != NO_READY_ITEM)
  {
    itemkey = myItems.GetItem(wpn);
    int dist = distance(self, x, y, 
                        targCOMBATANT->self, targCOMBATANT->x, targCOMBATANT->y);  
    decQty = WpnConsumesAmmoAtRange(itemkey, dist);
    
    // do we decrement weapon qty or ammo qty?
    if ((decQty) && (!WpnConsumesSelfAsAmmo(itemkey)))
    {
      // ammo is readied and must be decremented
      wpn = myItems.GetReady(AmmoQuiver);
      itemkey = myItems.GetItem(wpn);
    }
  }
  
   attDiceBonus = GetAttackBonus() + tmpAttDiceBonus;
  
   SetCharContext(this->GetContext());
   SetTargetContext(targCOMBATANT->GetContext());

   AttDiceSides=20;
   AttDiceNum=1;

   ModifyAttackRollDice(targCOMBATANT,AttDiceNum,AttDiceSides,attDiceBonus);
   ModifyAttackRollDiceForItem(targCOMBATANT, wpn, AttDiceNum,AttDiceSides,attDiceBonus);
   targCOMBATANT->ModifyAttackRollDiceAsTarget(this,AttDiceNum,AttDiceSides,attDiceBonus);
   targCOMBATANT->ModifyAttackRollDiceForItemAsTarget(this, wpn, AttDiceNum,AttDiceSides,attDiceBonus);

   thaco = GetAdjTHACO() - attDiceBonus;
   
   int targAC = targCOMBATANT->GetAdjAC();
   targCOMBATANT->ModifyACAsTarget(this, targAC, itemkey);

   thaco -= targAC;
   ModifyAttackThaco(targCOMBATANT,thaco);
   targCOMBATANT->ModifyAttackThacoAsTarget(this,thaco);

   if (thaco < 0)
     thaco = 0;

   int dmg = 0;  
 
   AttDiceRoll = rollDice(AttDiceSides, AttDiceNum, attDiceBonus);

   if (AttDiceRoll < 1) 
     AttDiceRoll = 1;
   if (AttDiceRoll > 20)
     AttDiceRoll = 20;
   
   TRACE("%i attacks %i: rolls %i, needs %i\n", 
                    self, targ, AttDiceRoll, thaco);

   if (friendly)
   {
     if (GetConfigPartyAlwaysMiss())
       AttDiceRoll = thaco - 1;
     else if (GetConfigPartyAlwaysHit())
       AttDiceRoll = thaco;
   }
   else
   {
      if (GetConfigMonsterAlwaysMiss())
        AttDiceRoll = thaco - 1;
      else if (GetConfigMonsterAlwaysHit())
        AttDiceRoll = thaco;
   }

   int DidHit = (AttDiceRoll >= thaco);
   int SpellDbKey=-1;

   if (DidHit)
   {     
     GetDamageDice(wpn, dmgDiceQty, dmgDiceSides, dmgDiceBonus, IsNonLethal, targCOMBATANT->isLargeDude(),SpellDbKey);
     dmgDiceBonus += tmpDmgDiceBonus; // for backstabbing
     
     ModifyAttackDamageDice(targCOMBATANT,dmgDiceQty,dmgDiceSides,dmgDiceBonus,IsNonLethal);
     ModifyAttackDamageDiceForItem(targCOMBATANT,wpn,dmgDiceQty,dmgDiceSides,dmgDiceBonus,IsNonLethal);
     targCOMBATANT->ModifyAttackDamageDiceAsTarget(this,dmgDiceQty,dmgDiceSides,dmgDiceBonus,IsNonLethal);
     targCOMBATANT->ModifyAttackDamageDiceForItemAsTarget(this,wpn,dmgDiceQty,dmgDiceSides,dmgDiceBonus,IsNonLethal);

     dmg = rollDice(dmgDiceSides, dmgDiceQty, dmgDiceBonus);
     
     if (dmg <= 0) 
       dmg = 1;

     TRACE("%i rolls %i damage using %i sides, %i qty, %i bonus (%s)\n",
                      self, dmg, dmgDiceSides, dmgDiceQty, dmgDiceBonus,
                      (IsNonLethal?"NonLethal":"Lethal"));

     InstantSpellActivate(SpellDbKey, targ);

     targCOMBATANT->UpdateSpellForAttacks(1);

     PlayHit();

     if (dmg > 0)
     {        
       //if (   (isThief())
       //    && ( is behind attackee ))
       //  dmg *= getThiefBackstabDamageMultiplier(level);        
       targCOMBATANT->TakeDamage(dmg, IsNonLethal);
       
       if (targCOMBATANT->GetStatus()==Dead)
       {
         // no longer a valid target
         RemoveCurrTarget();
       }
     }

     continueAttack=FALSE;
   }
   else
   {
     PlayMiss();
     continueAttack=TRUE;
   }

   if (DidHit)
   {
     if (SpellDbKey > 0)
     {
       CString samsg;
       SPELL_DATA *pSpell = spellData.GetSpellData(SpellDbKey);
       ASSERT(pSpell!=NULL);  
       if (pSpell!=NULL)
       {       
         samsg = pSpell->CastMsg;
         samsg.Replace("/s", pSpell->Name);
         samsg.Replace("/c", GetName());
         if (targCOMBATANT!=NULL)
           samsg.Replace("/t", targCOMBATANT->GetName());
         else
           samsg.Replace("/t", ""); 
         
         CombatMsg = samsg;
       }
     }
     else
     {
       CombatMsg.Format("Rolls %i and hits for %i dmg",
                        AttDiceRoll, dmg);
     }
   }
   else
     CombatMsg.Format("Rolls %i and misses", AttDiceRoll);

   FormatCombatText(combatTextData, CombatMsg);
   //combatMsgs.msgs.RemoveAll();
   //combatMsgs.Add(CombatMsg);

   if (decQty)
   {
     // dec item qty by 1, item won't show up in post-combat treasure list
     if (wpn != NO_READY_ITEM)
     {
       myItems.AdjustQty(wpn, -1);
       if (!myItems.HaveItem(wpn)) // if deleted because of zero quantity
       {
         // item removed, disable special abilities granted by it (if any)
         ITEM_DATA *pItem = itemData.GetItemData(itemkey);
         if (pItem!=NULL) pItem->specAbs.DisableAllFor(this);
       }

       // re-calculate movement/encumbrance
       SetEncumbrance(determineEffectiveEncumbrance());
       determineMaxMovement();    
     }
   }

   targCOMBATANT->lastAttacker = self;
   lastAttacked=targCOMBATANT->self;
   tmpAttDiceBonus = 0;
   tmpDmgDiceBonus = 0;

   ClearTargetContext();
   ClearCharContext();
}

//*****************************************************************************
// NAME:    COMBATANT::AttackOver
//
// PURPOSE: 
//
//*****************************************************************************
void COMBATANT::AttackOver()
{
  availAttacks -= 1.0;
  float temp = availAttacks;

  ClearFormattedText(combatTextData);
  TRACE("Attack over for %u, attacks remaining %f\n",
                   self, temp);

  if (availAttacks <= 0)
  {
    continueAttack=FALSE;
    availAttacks = 0;
    EndTurn();
    return;
  }

  // if manual control, and first attack hits, then stop
  // attacks if multiple allowed
  if ((!continueAttack) && (!OnAuto()))
  {
    StopAttack();
    return;
  }
}

//*****************************************************************************
// NAME:    COMBATANT::AttackOver
//
// PURPOSE: 
//
//*****************************************************************************
void COMBATANT::StopAttack()
{
  // stop casting spells if in progress
  StopCasting(FALSE);
  state = CS_None;
}

//*****************************************************************************
// NAME:    COMBATANT::HandleTimeDelayMsgExpired
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
void COMBATANT::HandleTimeDelayMsgExpired()
{  
  switch (state)
  {
  case CS_Attacking:
    TRACE("Handle TimeDelayMsgExpired: AttackOver\n");
    AttackOver();
    break;
  case CS_Bandaging:
    TRACE("Handle TimeDelayMsgExpired: Bandage Over\n");    
    break;
  case CS_Guarding:
    TRACE("Handle TimeDelayMsgExpired: Guarding continues\n");
    EndTurn(CS_Guarding);
    break;
  case CS_Turning:
    TRACE("Handle TimeDelayMsgExpired: Turn Undead Over\n");
    // message preceeds action in this case
    //EndTurn();
    break;
  }
}

//*****************************************************************************
//    NAME: COMBATANT::HandleCombatRoundMsgExpired
//
// PURPOSE: 
//
//*****************************************************************************
void COMBATANT::HandleCombatRoundMsgExpired()
{
  TRACE("Handle HandleCombatRoundMsgExpired for %i - state %s\n", 
         self,CombatantStateText[state]);
  PlaceCursorOnCurrentDude();  
  // do nothing yet
}

//*****************************************************************************
// NAME:    COMBATANT::HandleTimeDelayMsgBegin
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
void COMBATANT::HandleTimeDelayMsgBegin()
{
  switch (state)
  {
  case CS_Attacking:
    makeAttack(GetCurrTarget());
    break;

  default:
    //PlaceCursorOnCurrentDude();
    break;
  }
}

//*****************************************************************************
//    NAME: COMBATANT::UseAttackPose
//
// PURPOSE: 
//
//*****************************************************************************
BOOL COMBATANT::UseAttackPose()
{
  if (state==CS_Attacking) return TRUE;
  if (state==CS_Using) return TRUE;
  if (state==CS_Casting) return forceAttackPose;
  return FALSE;
}

//*****************************************************************************
// NAME:    displayCombatSprite
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
void COMBATANT::displayCombatSprite()
{  
   charStatusType cstat = GetAdjStatus();
   if (charUnconscious())
   {
      // blit dying combat tile
      blitDyingSprite();
   }
   else
   {
     if (!charOnCombatMap()) return;

     if (!GraphicsMgr.ValidSprite(icon.key))
     {
        LoadCombatIcon();
        if (!GraphicsMgr.ValidSprite(icon.key))
        {
          WriteDebugString("No icon surface to use in displayCombatSprite\n");
          return;
        }
     }
   
     int startTerX, startTerY;
     GetStartTerrain(startTerX, startTerY);
     if (   (x < startTerX + TILES_HORZ)
         && (y < startTerY + TILES_VERT))
     {
        int tx = x;
        int ty = y;

        // Check for partial icon display
        //
        // For icons larger than 1x1, the starting x,y may
        // be off of the left,top but still the partially visible
        // part of the icon should be displayed.
        //
        // This isn't a problem for the right,bottom since
        // if the x,y isn't within those bounds then none
        // of the icon will be either.
        if (tx < startTerX)
        {
          int ex = (tx+width)-1;
          // none of the icon is visible
          if (ex < startTerX)
            return;
          BOOL visible = FALSE;
          while ((tx <= ex) && (!visible))
          {
            if (tx >= startTerX)
              visible = TRUE;
            else
              tx++;
          }
          if (!visible)
            return;
        }
           
        if (ty < startTerY)
        {
          int ey = (ty+height)-1;
          // none of the icon is visible
          if (ey < startTerY)
            return;
          BOOL visible = FALSE;
          while ((ty <= ey) && (!visible))
          {
            if (ty >= startTerY)
              visible = TRUE;
            else
              ty++;
          }
          if (!visible)
            return;
        }

        RECT srcRect;  // clipping rect for source bitmap
        RECT dstRect;

        // width and height in pixels of a single tile
        int iw = width * COMBAT_TILE_WIDTH;
        int ih = height * COMBAT_TILE_HEIGHT;

        // offset in pixels to icon pair indicated by iconIndex
        int offset = (iconIndex-1)*(iw*2);
        if (offset < 0) offset=0;
                  
        if (IsCurrentCombatant(self))
        {
          if (UseAttackPose())
          {
            // display attack pose
            srcRect.left = offset+iw;
          }
          else
          {
            // display ready pose
            srcRect.left = offset;
          }
        }
        else
          srcRect.left = offset;
    
        srcRect.right  = srcRect.left + iw;
        srcRect.top    = 0;
        srcRect.bottom = ih;
        
        BOOL flip = (facing == FACE_WEST);

        // take into account any clipping done against
        // top, left for partially visible icons
        if (tx > x)
        {
          if (flip)
            srcRect.right -= ((tx-x)*COMBAT_TILE_WIDTH);
          else
            srcRect.left += ((tx-x)*COMBAT_TILE_WIDTH);
        }
        if (ty > y)
          srcRect.top += ((ty-y)*COMBAT_TILE_HEIGHT);

        dstRect.left=TerrainToScreenCoordX(tx);
        dstRect.top=TerrainToScreenCoordY(ty);
        dstRect.right  = dstRect.left+(srcRect.right-srcRect.left);
        dstRect.bottom = dstRect.top+(srcRect.bottom-srcRect.top);

        /*
        if (dude.GetType() == CHAR_TYPE)
          TRACE("Blitting pc icon\n");
        else if (dude.GetType() == NPC_TYPE)
          TRACE("Blitting npc icon\n");
        else if (dude.GetType() == monster)
          TRACE("Blitting monster icon\n");
        */

        GraphicsMgr.SetClipRect(&GetCombatRect());  

        // blit the source tile from the art bitmap to the back buffer
        GraphicsMgr.BlitSpriteImage(dstRect.left, dstRect.top,
                              icon.key,
                              &srcRect, 
                              flip,
                              BACKBUFFERNUM,
                              &dstRect); 

        GraphicsMgr.SetClipRect(&GetFullRect());
     }
   }
}

//*****************************************************************************
// NAME:    blitDyingSprite
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
void COMBATANT::blitDyingSprite()
{
  if (!blitDeathTile)
    blitDeadSprite();
  
  if (!IsPartyMember()) return;    
  //UpdateCombatTimeDelayMsg();

  if (!ValidTerrainViewportCoords(x, y)) 
  {
    //ASSERT(FALSE);
    return;
  }
  
  //EnsureVisible(GetCenterX(),GetCenterY());

  int sx = TerrainToScreenCoordX(x);
  int sy = TerrainToScreenCoordY(y);
 
  GraphicsMgr.SetClipRect(&GetCombatRect());  
  GraphicsMgr.BlitImage(sx,sy,CombatDeathIconArt.surface,&DeadCharSrcA);
  GraphicsMgr.SetClipRect(&GetFullRect());
}

//*****************************************************************************
// NAME:    blitDeadSprite
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
void COMBATANT::blitDeadSprite()
{
   if (blitDeathTile) return; // already done
   blitDeathTile = TRUE;

   placeCombatant(x, y, NO_DUDE, width, height);
   placeDyingCombatant(x,y,self,width,height);
   
   UpdateCombatTimeDelayMsg();

   PlayCombatDeath();

   if (!ValidTerrainViewportCoords(x, y)) 
   {
     //ASSERT(FALSE);
     return;
   }

   EnsureVisible(GetCenterX(),GetCenterY());
   GraphicsMgr.SetClipRect(&GetCombatRect());

   int i,j;
   for (i=0;i<height;i++)
   {
     for (j=0;j<width;j++)
     {
       GraphicsMgr.BlitImage(TerrainToScreenCoordX(x+j),
                             TerrainToScreenCoordY(y+i),
                             CombatDeathArt.surface,
                             &DeadCharSrcA,
                             FALSE,FRONTBUFFERNUM);
     }
   }      

   theApp.AppSleep(200);

   for (i=0;i<height;i++)
   {
     for (j=0;j<width;j++)
     {
       GraphicsMgr.BlitImage(TerrainToScreenCoordX(x+j),
                             TerrainToScreenCoordY(y+i),
                             CombatDeathArt.surface,
                             &DeadCharSrcB,
                             FALSE,FRONTBUFFERNUM);
     }
   } 
   
   theApp.AppSleep(200);

   for (i=0;i<height;i++)
   {
     for (j=0;j<width;j++)
     {
       GraphicsMgr.BlitImage(TerrainToScreenCoordX(x+j),
                             TerrainToScreenCoordY(y+i),
                             CombatDeathArt.surface,
                             &DeadCharSrcA,
                             FALSE,FRONTBUFFERNUM);
     }
   }      

   theApp.AppSleep(200);

   for (i=0;i<height;i++)
   {
     for (j=0;j<width;j++)
     {
       GraphicsMgr.BlitImage(TerrainToScreenCoordX(x+j),
                             TerrainToScreenCoordY(y+i),
                             CombatDeathArt.surface,
                             &DeadCharSrcB,
                             FALSE,FRONTBUFFERNUM);
     }
   }      

   theApp.AppSleep(200);

   for (i=0;i<height;i++)
   {
     for (j=0;j<width;j++)
     {
       GraphicsMgr.BlitImage(TerrainToScreenCoordX(x+j),
                             TerrainToScreenCoordY(y+i),
                             CombatDeathArt.surface,
                             &DeadCharSrcA,
                             FALSE,FRONTBUFFERNUM);
     }
   }      

   GraphicsMgr.SetClipRect(&GetFullRect());
   theApp.AppSleep(500);
}

//*****************************************************************************
// NAME: COMBATANT::EndTurn
//
// PURPOSE:  
//*****************************************************************************
void COMBATANT::EndTurn(combatantStateType newState)
{   
  state = newState;
  QueuedCombatantData &qcomb = GetQueuedCombatants();

  if (qcomb.Top() == self)
  {
    if (qcomb.ChangeStats())
    {
      TRACE("EndTurn(%s), %i done\n", CombatantStateText[state], self);
      done = TRUE;
    }
    else
      TRACE("EndTurn(%s), %i (no change to done)\n", CombatantStateText[state], self);

    // input was disabled if auto combatant
    EnableUserInput(TRUE);
    qcomb.Pop();
  }
  else
  {
    qcomb.Remove(self);
    TRACE("Forced EndTurn(%s), %i done\n", CombatantStateText[state], self);
    done = TRUE;
  }
  forceAttackPose=FALSE;
  OnEndTurn();
  //combatMsgs.msgs.RemoveAll(); // not set up for scrolling msgs yet
}


//*****************************************************************************
// NAME: COMBATANT::IsDone
//
// PURPOSE:  
//*****************************************************************************
BOOL COMBATANT::IsDone()
{ 
  QueuedCombatantData &qcomb = GetQueuedCombatants();
  if (   (qcomb.Top() == self)
      && (!qcomb.ChangeStats()))
  {
    return FALSE; // queued up for free attack
  }

  if (GetAdjSpecAb(SA_Sleep))
  {
    return TRUE;
  }
  
  if (GetAdjSpecAb(SA_HoldPerson))
  {
    return TRUE;
  }

  if (!charOnCombatMap())
    done = TRUE;

  return done; 
}

//*****************************************************************************
// NAME: COMBATANT::Quick
//
// PURPOSE:  
//*****************************************************************************
void COMBATANT::Quick(BOOL Enable)
{
  // can't change quick status if not allowed to control
  if (GetAdjAllowPlayerControl())
  {
    // if turning off auto, stop doing whatever
    // the AI had you doing.
    if (!Enable)
    {
      ClearPath();
      combattargets.RemoveAll();
      state = CS_None;
      StopCasting(FALSE);
    }
    SetAutomatic(Enable);
    if (!IsPartyMember())
      EnableUserInput(!GetAdjAutomatic());
    else
      EnableUserInput(TRUE);
  }
}

//*****************************************************************************
// NAME: COMBATANT::TurnUndead
//
// PURPOSE:  
//*****************************************************************************
void COMBATANT::TurnUndead()
{
  // end any spell casting in progress
  StopCasting(FALSE);
  hasTurnedUndead=TRUE;
  state = CS_Turning;
}

//*****************************************************************************
// NAME: COMBATANT::CanBandage
//
// PURPOSE:  
//*****************************************************************************
BOOL COMBATANT::CanBandage()
{
  return (!IsDone());
}

//*****************************************************************************
// NAME:    COMBATANT::Bandage
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
void COMBATANT::Bandage()
{ 
  // end any spell casting in progress
  StopCasting(FALSE);
  state = CS_Bandaging;
}

//*****************************************************************************
// NAME:    COMBATANT::CanGuard
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
BOOL COMBATANT::CanGuard()
{
  if (IsDone()) return FALSE;

  // can guard only when melee weapon is readied, if is a
  // party member. Monsters can guard always
  if (!IsPartyMember())
    return TRUE;

  int mywpnidx = myItems.GetReady(WeaponHand);
  if (mywpnidx != NO_READY_ITEM)
  {  
    // using a weapon rather than natural attack types (ie claws/jaws/fists)
     if (myItems.GetQty(mywpnidx) <= 0)
        return FALSE;    
 
     return (WpnCanAttackAtRange(myItems.GetItem(mywpnidx), 1));
  }
  // no weapon readied
  return FALSE;
}

//*****************************************************************************
// NAME: COMBATANT::Guard
//
// PURPOSE:  
//*****************************************************************************
void COMBATANT::Guard()
{
  // end any spell casting in progress
  StopCasting(FALSE);
  state = CS_Guarding;
}

//*****************************************************************************
// NAME: COMBATANT::CanMove
//
// PURPOSE:  
//*****************************************************************************
BOOL COMBATANT::CanMove()
{
  if (GetType() == MONSTER_TYPE)
  {
    if (GetConfigMonsterNoMove())
      return FALSE;
  }

  if (IsDone()) 
    return FALSE;
  if (movement >= GetAdjMaxMovement()) 
    return FALSE;
  return TRUE;
}

//*****************************************************************************
// NAME:    COMBATANT::FindPathTo
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
BOOL COMBATANT::FindPathTo(int dx, int dy, BOOL CheckOccupants)
{  
  if (!OnAuto())
  {
    if ((abs(dx-x) > 1) || (abs(dy-y) > 1))
      TRACE("Moving more than 1 square\n");
  }

  //WriteDebugString("FindPathTo(%u,%u)\n", dx, dy);
  ClearPath();

  if (!CanMove()) return FALSE;

  pathMgr.SetPathSize((BYTE)width, (BYTE)height);
  hPath = pathMgr.GetPath(x, y, dx, dy, CheckOccupants);
  return (hPath >= 0);
}

//*****************************************************************************
//    NAME: COMBATANT::FindPathAwayFrom
//
// PURPOSE: 
//
//*****************************************************************************
BOOL COMBATANT::FindPathAwayFrom(int fleeFrom)
{  
  ClearPath();
  if (!CanMove()) return FALSE;
  if (fleeFrom < 0) return FALSE;

  /*
  pathMgr.SetPathSize((BYTE)width, (BYTE)height);

  // locate path to position in room that is farthest
  // from indicated fleeFrom character

  int fx = combatData.combatants[fleeFrom].x;
  int fy = combatData.combatants[fleeFrom].y;

  // get direction for moving away from 'fleeFrom'
  PATH_DIR dir = GetDir(x, y, fx, fy);
  switch (dir)
  {
  case PathNorth: dir = PathSouth; break;
  case PathNE: dir = PathSW; break;
  case PathEast: dir = PathWest; break;
  case PathSE: dir = PathNW; break;
  case PathSouth: dir = PathNorth; break;
  case PathSW: dir = PathNE; break;
  case PathWest: dir = PathEast; break;
  case PathNW: dir = PathSE; break;
  default: dir = PathNorth; break;
  }

  PATH_DIR pleft = PathDirLeft(dir);
  PATH_DIR pleft2 = PathDirLeft(pleft);
  PATH_DIR pleft3 = PathDirLeft(pleft2);
  PATH_DIR pright = PathDirRight(dir);
  PATH_DIR pright2 = PathDirRight(pright);
  PATH_DIR pright3 = PathDirRight(pright2);

  // max movement distance may not allow straight-line pathing
  // but go with it for now.
  int maxmove = GetMaxMovement();
  if (FindPathAlongLine(dir, maxmove))
    return TRUE;
  if (FindPathAlongLine(pleft, maxmove))
    return TRUE;
  if (FindPathAlongLine(pright, maxmove))
    return TRUE;
  if (FindPathAlongLine(pleft2, maxmove))
    return TRUE;
  if (FindPathAlongLine(pright2, maxmove))
    return TRUE;
  if (FindPathAlongLine(pleft3, maxmove))
    return TRUE;
  if (FindPathAlongLine(pright3, maxmove))
    return TRUE;
  */
  // no corner in room to flee to, just flee the map if possible
  if (FindPathToMapEdge())
    return TRUE;

  return FALSE;
}

//*****************************************************************************
//    NAME: COMBATANT::FindPathAlongLine
//
// PURPOSE: 
//
//*****************************************************************************
BOOL COMBATANT::FindPathAlongLine(PATH_DIR dir, int dist)
{
  BOOL found = FALSE;
  int maxdist = dist;
  int halfdist = maxdist/2;
  int dx=0;
  int dy=0;

  switch (dir)
  {
    case PathNorth: dx=x;dy=y-maxdist; break;
    case PathNE: dx=x+halfdist;dy=y-halfdist; break;
    case PathEast: dx=x+maxdist;dy=y; break;
    case PathSE: dx=x+halfdist;dy=y+halfdist; break;
    case PathSouth: dx=x;dy=y+maxdist; break;
    case PathSW: dx=x-halfdist;dy=y+halfdist; break;
    case PathWest: dx=x-maxdist;dy=y; break;
    case PathNW: dx=x-halfdist;dy=y-halfdist; break;
    default: return FALSE;
  }

  if (dx < 0) dx=0;
  if (dx >= MAX_TERRAIN_WIDTH) dx = MAX_TERRAIN_WIDTH-1;
  if (dy < 0) dy=0;
  if (dy >= MAX_TERRAIN_HEIGHT) dy = MAX_TERRAIN_HEIGHT-1;

  if (HaveLineOfSight(GetCenterX(),GetCenterY(),dx,dy))
  {
    hPath = pathMgr.GetPath(x, y, dx, dy, TRUE);
    found = (hPath>=0);
  }

  return found;
}

//*****************************************************************************
//    NAME: COMBATANT::FindPathToMapNorthEdge
//
// PURPOSE: 
//
//*****************************************************************************
int COMBATANT::FindPathToMapNorthEdge(void)
{
  int path=-1;
  BOOL found = FALSE;

  // find a path to one of the four map sides
  int dx=0;
  int dy=0;
  pathMgr.SetPathSize((BYTE)width, (BYTE)height);

  // north side, find empty, non-blocked, 
  // square along north map edge.
  // start with center and move towards edges
  int count = 0;
  int max = MAX_TERRAIN_WIDTH/2;
  int center = MAX_TERRAIN_WIDTH/2;

  dy=0;
  while ((!found) && (count<=max))
  {
    if (!isObstical(center+count, dy, 1, 1, TRUE))
    {
      dx=center+count;
      path = pathMgr.GetPath(x, y, dx, dy, TRUE);
      found = (path >= 0);      
    }
    else if (!isObstical(center-count, dy, 1, 1, TRUE))
    {
      dx=center-count;
      path = pathMgr.GetPath(x, y, dx, dy, TRUE);
      found = (path >= 0);      
    }
    count++;
  }
  return path;
}

//*****************************************************************************
//    NAME: COMBATANT::FindPathToMapEastEdge
//
// PURPOSE: 
//
//*****************************************************************************
int COMBATANT::FindPathToMapEastEdge(void)
{
  int path=-1;
  BOOL found = FALSE;

  // find a path to one of the four map sides
  int dx=0;
  int dy=0;
  pathMgr.SetPathSize((BYTE)width, (BYTE)height);

  // north side, find empty, non-blocked, 
  // square along north map edge.
  // start with center and move towards edges
  int count = 0;
  int max = MAX_TERRAIN_HEIGHT/2;
  int center = MAX_TERRAIN_HEIGHT/2;

  dx=MAX_TERRAIN_WIDTH-1;
  while ((!found) && (count<=max))
  {
    if (!isObstical(dx, center+count, 1, 1, TRUE))
    {
      dy=center+count;
      path = pathMgr.GetPath(x, y, dx, dy, TRUE);
      found = (path >= 0);      
    }
    else if (!isObstical(dx, center-count, 1, 1, TRUE))
    {
      dy=center-count;
      path = pathMgr.GetPath(x, y, dx, dy, TRUE);
      found = (path >= 0);      
    }
    count++;
  }
  return path;
}

//*****************************************************************************
//    NAME: COMBATANT::FindPathToMapSouthEdge
//
// PURPOSE: 
//
//*****************************************************************************
int COMBATANT::FindPathToMapSouthEdge(void)
{
  int path=-1;
  BOOL found = FALSE;

  // find a path to one of the four map sides
  int dx=0;
  int dy=0;
  pathMgr.SetPathSize((BYTE)width, (BYTE)height);

  // north side, find empty, non-blocked, 
  // square along north map edge.
  // start with center and move towards edges
  int count = 0;
  int max = MAX_TERRAIN_WIDTH/2;
  int center = MAX_TERRAIN_WIDTH/2;

  dy=MAX_TERRAIN_HEIGHT-1;
  while ((!found) && (count<=max))
  {
    if (!isObstical(center+count, dy, 1, 1, TRUE))
    {
      dx=center+count;
      path = pathMgr.GetPath(x, y, dx, dy, TRUE);
      found = (path >= 0);      
    }
    else if (!isObstical(center-count,dy, 1, 1, TRUE))
    {
      dx=center-count;
      path = pathMgr.GetPath(x, y, dx, dy, TRUE);
      found = (path >= 0);      
    }
    count++;
  }
  return path;
}

//*****************************************************************************
//    NAME: COMBATANT::FindPathToMapWestEdge
//
// PURPOSE: 
//
//*****************************************************************************
int COMBATANT::FindPathToMapWestEdge(void)
{
  int path=-1;
  BOOL found = FALSE;

  // find a path to one of the four map sides
  int dx=0;
  int dy=0;
  pathMgr.SetPathSize((BYTE)width, (BYTE)height);

  // north side, find empty, non-blocked, 
  // square along north map edge.
  // start with center and move towards edges
  int count = 0;
  int max = MAX_TERRAIN_HEIGHT/2;
  int center = MAX_TERRAIN_HEIGHT/2;

  dx=0;
  while ((!found) && (count<=max))
  {
    if (!isObstical(dx, center+count, 1, 1, TRUE))
    {
      dy=center+count;
      path = pathMgr.GetPath(x, y, dx, dy, TRUE);
      found = (path >= 0);      
    }
    else if (!isObstical(dx,center-count,1, 1, TRUE))
    {
      dy=center-count;
      path = pathMgr.GetPath(x, y, dx, dy, TRUE);
      found = (path >= 0);      
    }
    count++;
  }
  return path;
}

//*****************************************************************************
//    NAME: COMBATANT::FindPathToMapEdge
//
// PURPOSE: 
//
//*****************************************************************************
BOOL COMBATANT::FindPathToMapEdge(void)
{    
  ClearPath();
  if (!CanMove()) return FALSE;
  int shortestpath=-1;
  int shortestdist=INT_MAX;
  int ndist=INT_MAX;
  int edist=INT_MAX;
  int sdist=INT_MAX;
  int wdist=INT_MAX;

  int npath = FindPathToMapNorthEdge();
  if (npath >= 0) ndist = pathMgr.GetStepsRemaining(npath); 
  if (ndist <= 10)
  {
    // close enough, stop searching
    hPath = npath;
    return TRUE;
  }

  int epath = FindPathToMapEastEdge();
  if (epath >= 0) edist = pathMgr.GetStepsRemaining(epath); 
  if (edist <= 10)
  {
    // close enough, stop searching
    hPath = epath;
    return TRUE;
  }
  
  int spath = FindPathToMapSouthEdge();
  if (spath >= 0) sdist = pathMgr.GetStepsRemaining(spath); 
  if (sdist <= 10)
  {
    // close enough, stop searching
    hPath = spath;
    return TRUE;
  }

  int wpath = FindPathToMapWestEdge(); 
  if (wpath >= 0) wdist = pathMgr.GetStepsRemaining(wpath); 
  if (wdist <= 10)
  {
    // close enough, stop searching
    hPath = wpath;
    return TRUE;
  }

  // all are more than 10 squares away, pick the shortest
  if (ndist < shortestdist)
  {
    shortestdist = ndist;
    shortestpath = npath;
  }
  if (edist < shortestdist)
  {
    shortestdist = edist;
    shortestpath = epath;
  }
  if (sdist < shortestdist)
  {
    shortestdist = sdist;
    shortestpath = spath;
  }
  if (wdist < shortestdist)
  {
    shortestdist = wdist;
    shortestpath = wpath;
  }

  if (shortestpath != npath) pathMgr.FreePath(npath);
  if (shortestpath != epath) pathMgr.FreePath(epath);
  if (shortestpath != spath) pathMgr.FreePath(spath);
  if (shortestpath != wpath) pathMgr.FreePath(wpath);

  if (shortestpath >= 0)
  {
    hPath = shortestpath;
    return TRUE;
  }
  return FALSE;
}

//*****************************************************************************
// NAME: COMBATANT::CanUse
//
// PURPOSE:  
//*****************************************************************************
BOOL COMBATANT::CanUse()
{
  if (IsDone()) return FALSE;
  return TRUE;
}


//*****************************************************************************
// NAME: COMBATANT::CanTurnUndead
//
// PURPOSE:  
//*****************************************************************************
BOOL COMBATANT::CanTurnUndead()
{
  if (hasTurnedUndead) return FALSE;
  if (IsDone()) return FALSE;

  if (isCleric(GetClass()))
    return TRUE;

  if ((isPaladin(GetClass())) && (GetCurrentLevel(PaladinFlag) >= 3))
    return TRUE;

  return FALSE;
}


//*****************************************************************************
// NAME: COMBATANT::CanDelay
//
// PURPOSE:  
//*****************************************************************************
BOOL COMBATANT::CanDelay()
{
  if (IsDone()) return FALSE;
  return (initiative < 10);
}
  
 
//*****************************************************************************
// NAME: COMBATANT::DelayAction
//
// PURPOSE:  
//*****************************************************************************
void COMBATANT::DelayAction()
{
  if (CanDelay())
  {
    initiative = 10;
    state = CS_None;
    QueuedCombatantData &qcomb = GetQueuedCombatants();

    if (qcomb.Top() == self)
    {
      TRACE("DelayAction() for %i\n", self);
      // input was disabled if auto combatant
      EnableUserInput(TRUE);
      qcomb.Pop();
    }
  }
}

//*****************************************************************************
// NAME: COMBATANT::GetMaxTargets
//
// PURPOSE:  
//*****************************************************************************
int COMBATANT::GetMaxTargets()
{
  int max = 0;
  int mywpnidx;
  switch (state)
  {
  case CS_Casting:
    ASSERT(FALSE); // handled elsewhere
    break;

  default:
    mywpnidx = myItems.GetReady(WeaponHand);
    if (mywpnidx != NO_READY_ITEM)
    {
      // item is readied    
     // check for spell abilities
      if (   (state == CS_Using)
          && (myItems.GetCharges(mywpnidx) > 0))
      {
        ITEM_DATA *data = itemData.GetItemData(myItems.GetItem(mywpnidx));
        if (data != NULL)
        {
          if (data->Spell >= 0)
          {
            // get spell combattargets
          }
        }
      }
      else
      {
        // make sure it is a weapon
        if (isWeapon(myItems.GetItem(mywpnidx)))
        {
          if (GetType() != MONSTER_TYPE)
          {
            // check for sweeps
          }
          else
            max = 1;
        }
      }
    }
    else // using fists, claws, etc.
      max = 1;
    break;
  }

  return max;
}


//*****************************************************************************
// NAME: COMBATANT::CanTarget
//
// PURPOSE:  
//*****************************************************************************
BOOL COMBATANT::CanTarget()
{
  if (IsDone()) return FALSE;

  if ((state == CS_Casting) || (state == CS_Using))
  {
    ASSERT(FALSE); // spell targeting uses different data structures
    return FALSE;
  }
  else
    return TRUE;
}

//*****************************************************************************
// NAME: COMBATANT::HaveTarget
//
// PURPOSE:  
//*****************************************************************************
BOOL COMBATANT::HaveTarget(int targ)
{
  if (combattargets.GetCount() == 0)
    return FALSE;

  POSITION pos = combattargets.GetHeadPosition();
  while (pos != NULL)
  {
    if (combattargets.GetNext(pos) == targ)
      return TRUE;
  }
  return FALSE;
}


//*****************************************************************************
// NAME: COMBATANT::RemoveTarget
//
// PURPOSE:  
//*****************************************************************************
void COMBATANT::RemoveTarget(int targ)
{
  if (combattargets.GetCount() == 0)
    return;

  POSITION pos = combattargets.GetHeadPosition();
  while (pos != NULL)
  {
    if (combattargets.GetAt(pos) == targ)
    {
      combattargets.RemoveAt(pos);
      return;
    }
    else
      combattargets.GetNext(pos);
  }
}

//*****************************************************************************
// NAME: COMBATANT::AddTarget
//
// PURPOSE:  
//*****************************************************************************
void COMBATANT::AddTarget(int newTarget) 
{ 
  if (newTarget == NO_DUDE)
    return;

  RemoveTarget(newTarget);

  if (CanTarget())
  {
    // if using weapon, change to new target (only one allowed)
    //if (!combattargets.IsEmpty())
    //  combattargets.RemoveTail();

    COMBATANT *targCOMBATANT;
    targCOMBATANT = GetCombatantPtr(newTarget);
    ASSERT(targCOMBATANT!=NULL);
    if (targCOMBATANT==NULL) return;

    int dist = distance(self, x, y, 
                        targCOMBATANT->self, targCOMBATANT->x, targCOMBATANT->y);  

    combattargets.Insert(newTarget, 
                   MakeTargetKey(newTarget, dist));
  }

  SetCurrTarget(); 
}

//*****************************************************************************
//    NAME: COMBATANT::AddTarget
//
// PURPOSE: 
//
//*****************************************************************************
BOOL COMBATANT::AddTarget(COMBATANT &targ, int range)
{
  ASSERT( IsCombatActive() == TRUE ); // non-combat magic handled in char.cpp
  if (targets.m_SelectingUnits==FALSE) 
  {
    WriteDebugString("AddTarget() fails, SelectingUnits is FALSE\n");
    return FALSE;
  }
  if ((targets.m_FriendlyFireOk==FALSE) && (targ.GetIsFriendly())) 
  {
    WriteDebugString("AddTarget() fails, IsFriendly && FriendlyFireNotAllowed\n");
    return FALSE;
  }
  if (!HaveLineOfSight(GetCenterX(), GetCenterY(),targ.GetCenterX(), targ.GetCenterY()))
  {
    WriteDebugString("AddTarget() fails, no line of sight from caster to target\n");
    return FALSE;    
  }
  return (targets.AddTarget(targ.self, targ.GetNbrHD(), range));
}

//*****************************************************************************
//    NAME: COMBATANT::AddMapTarget
//
// PURPOSE: 
//
//*****************************************************************************
BOOL COMBATANT::AddMapTarget(int mapx, int mapy, PATH_DIR dir)
{
  ASSERT( IsCombatActive() == TRUE ); // non-combat magic handled in char.cpp
  if (targets.m_SelectingUnits==TRUE) return FALSE;

  if (!HaveLineOfSight(GetCenterX(), GetCenterY(), mapx, mapy))
  {
    WriteDebugString("AddMapTarget() fails, no line of sight from caster to mapx,mapy\n");
    return FALSE;    
  }
  
  targets.m_maptarglist.RemoveAll();

  //int range = targets.m_MaxRange;
  if (targets.AddMapTarget(mapx, mapy))
  {    
    // figure out which targets are in range
    MAPTARGETLIST targlist;
    MAPLOCATIONLIST maplist;
    int maxtargs=100000;
    int VisCheckX=0,VisCheckY=0;
    bool screenself=false;
    switch (targets.m_type)
    {
    case AreaCircle: // circle is centered at x,y
      maxtargs = targets.m_MaxTargets;
      GetCombatantsInCircle(targlist, 
                            mapx,mapy, 
                            targets.m_MaxRange);
      GetMapTilesInCircle(maplist,//targets.m_maptarglist, 
                          mapx,mapy, 
                          targets.m_MaxRange);      
      VisCheckX=mapx;
      VisCheckY=mapy;
      break;

    case AreaLinePickStart: // line starts at mapx,mapy, dir is from caster to mapx,mapy
      screenself=true;
      GetCombatantsInLine(targlist,  
                          mapx,mapy,            // start
                          targets.m_MaxRange,   // length
                          targets.m_MaxTargets, // width
                          dir);
      GetMapTilesInLine(maplist,  
                        mapx,mapy,            // start
                        targets.m_MaxRange,   // length
                        targets.m_MaxTargets, // width
                        dir);      
      VisCheckX=mapx;
      VisCheckY=mapy;
      break;

    case AreaLinePickEnd: // line starts at caster, towards selected map location
      screenself=true;
      GetCombatantsInLine(targlist, 
                          this->x,this->y,       // start
                          mapx,mapy,             // end
                          targets.m_MaxTargets); // width
      GetMapTilesInLine(maplist, 
                        this->x,this->y,       // start
                        mapx,mapy,             // end
                        targets.m_MaxTargets); // width      
      VisCheckX=this->x;
      VisCheckY=this->y;
      break;

    case AreaSquare: // square is centered at x,y
      GetCombatantsInRectangle(targlist, 
                               mapx,mapy, 
                               targets.m_MaxTargets,  // width
                               targets.m_MaxRange);   // height                          
      GetMapTilesInRectangle(maplist, 
                             mapx,mapy, 
                             targets.m_MaxTargets,  // width
                             targets.m_MaxRange);   // height                                
      VisCheckX=mapx;
      VisCheckY=mapy;
      break;

    case AreaCone: // cone starts at x,y, extends in 'dir' direction
      screenself=true;
      GetCombatantsInCone(targlist, 
                          mapx,mapy,            // start
                          targets.m_MaxRange,   // length
                          targets.m_MaxTargets, // width at end of cone
                          dir);      
      GetMapTilesInCone(maplist, 
                        mapx,mapy,            // start
                        targets.m_MaxRange,   // length
                        targets.m_MaxTargets, // width at end of cone
                        dir);            
      VisCheckX=mapx;
      VisCheckY=mapy;
      break;

    default: ASSERT(FALSE); break;
    }

    // now add them to the target list
    POSITION pos=targlist.GetHeadPosition();
    int targcount=0;
    while ((pos!=NULL)&&(targcount < maxtargs))
    {
      int index = targets.m_targets.GetSize();
      int dude = targlist.GetAt(pos);

      // target selection code does not care about walls or other map
      // obstructions, so weed out the targets that are not visible from
      // the source location of the spell.
      if (HaveLineOfSight(VisCheckX,VisCheckY, GetCombatantPtr(dude)->x,GetCombatantPtr(dude)->y))
      {
        if (screenself)
        {
          if (dude!=self)
          {
            targets.m_targets.SetAtGrow(index, dude);
            targcount++;
          }
        }
        else
        {
          targets.m_targets.SetAtGrow(index, dude);
          targcount++;
        }
      }

      targlist.GetNext(pos);      
    }

    // now add them to the map target list
    pos=maplist.GetHeadPosition();
    while (pos!=NULL)
    {
      POINT pt = maplist.GetNext(pos);
      
      // target selection code does not care about walls or other map
      // obstructions, so weed out the targets that are not visible from
      // the source location of the spell.
      if (HaveLineOfSight(VisCheckX,VisCheckY, pt.x, pt.y))
        targets.m_maptarglist.AddTail(pt);     
    }

    if (targcount > 0)
    {
      ASSERT( targets.m_maptarglist.GetCount() > 0 );
    }
  }
  return (targets.m_targets.GetSize()>0);
}

//*****************************************************************************
//    NAME: COMBATANT::AddTargetSelf
//
// PURPOSE: 
//
//*****************************************************************************
BOOL COMBATANT::AddTargetSelf()
{
  return (AddTarget(*this, 0));
}

//*****************************************************************************
//    NAME: COMBATANT::AutoChooseSpellTargets
//
// PURPOSE: 
//
//*****************************************************************************
void COMBATANT::AutoChooseSpellTargets()
{

}

//*****************************************************************************
// NAME: COMBATANT::RemoveAllTargets
//
// PURPOSE:  
//*****************************************************************************
void COMBATANT::RemoveAllTargets() 
{ 
  combattargets.RemoveAll();
  SetCurrTarget();
}

//*****************************************************************************
// NAME: COMBATANT::SetCurrTarget
//
// PURPOSE:  
//*****************************************************************************
void COMBATANT::SetCurrTarget()
{  
  if (!combattargets.IsEmpty())
  {
    targetPos = combattargets.GetHeadPosition();
    target = combattargets.GetAt(targetPos);
  }
  else
  {
    target = NO_DUDE;
    targetPos = NULL;
  }
}

//*****************************************************************************
// NAME: COMBATANT::RemoveCurrTarget
//
// PURPOSE:  
//*****************************************************************************
void COMBATANT::RemoveCurrTarget()
{
  if (!combattargets.IsEmpty())
    combattargets.RemoveHead();

  SetCurrTarget();
}


//*****************************************************************************
// NAME: COMBATANT::GetNextTarget
//
// PURPOSE:  
//*****************************************************************************
int COMBATANT::GetNextTarget()
{
  int dude = NO_DUDE;

  if (combattargets.IsEmpty())
    return NO_DUDE;

  if (targetPos == NULL)
  {
    targetPos = combattargets.GetHeadPosition();
  }
  else
  {
    combattargets.GetNext(targetPos);
    if (targetPos == NULL)
      targetPos = combattargets.GetHeadPosition();
  }

  dude = combattargets.GetAt(targetPos);
  return dude;
}

//*****************************************************************************
// NAME:    COMBATANT::FaceAttacker
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
void COMBATANT::FaceAttacker(int attacker)
{
  COMBATANT *tempCOMBATANT;
  tempCOMBATANT = GetCombatantPtr(attacker);
  ASSERT(tempCOMBATANT!=NULL);
  if (tempCOMBATANT==NULL) return;
  int ax = tempCOMBATANT->x;
  int ay = tempCOMBATANT->y;
  PATH_DIR dir;

  // get direction to attacker
  if (x > ax) // nw, w, sw
  {
    if (y > ay) dir = PathNW;
    else if (y < ay) dir =  PathSW;
    else dir = PathWest;
  }
  else if (x < ax) // ne, e, se
  {
    if (y > ay) dir = PathNE;
    else if (y < ay) dir = PathSE;
    else dir = PathEast;
  }
  else // x == ax
  {
    if (y > ay) dir = PathNorth;
    else if (y < ay) dir = PathSouth;
    else  dir = PathBAD; // curr == dest
  }

  // if backstabbing, don't face attacker

  FaceDirection(dir);
}

//*****************************************************************************
//    NAME: COMBATANT::FaceLocation
//
// PURPOSE: 
//
//*****************************************************************************
void COMBATANT::FaceLocation(int fx, int fy)
{
  int ax = fx;
  int ay = fy;
  PATH_DIR dir;

  // get direction to x,y
  if (x > ax) // nw, w, sw
  {
    if (y > ay) dir = PathNW;
    else if (y < ay) dir =  PathSW;
    else dir = PathWest;
  }
  else if (x < ax) // ne, e, se
  {
    if (y > ay) dir = PathNE;
    else if (y < ay) dir = PathSE;
    else dir = PathEast;
  }
  else // x == ax
  {
    if (y > ay) dir = PathNorth;
    else if (y < ay) dir = PathSouth;
    else  dir = PathBAD; // curr == dest
  }

  FaceDirection(dir);
}

//*****************************************************************************
//    NAME: COMBATANT::FaceDirection
//
// PURPOSE: 
//
//*****************************************************************************
void COMBATANT::FaceDirection(PATH_DIR dir)
{
  // translate attacker direction into
  // east/west icon facing
  switch (dir)
  {
  case PathNW:
  case PathWest:
  case PathSW:
    facing = FACE_WEST;
    break;
  case PathNE:
  case PathEast:
  case PathSE:
    facing = FACE_EAST;
    break;

  default:
    // if north/south attacker, no need to change facing.
    break;
  }
}

//*****************************************************************************
// NAME:    COMBATANT::GetDamageDice
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
void COMBATANT::GetDamageDice(int wpn, int &num, int &sides, int &bonus, BOOL &NonLethal, BOOL IsLarge, int &SpellDbKey)
{
  NonLethal=FALSE;
  SpellDbKey=-1;
  
  if (wpn != NO_READY_ITEM)
  {  
     ITEM_DATA *idata = itemData.GetItemData(myItems.GetItem(wpn));

     if (idata != NULL)
     {      
        NonLethal = idata->IsNonLethal;

        if (IsLarge)
        {
           num = idata->Nbr_Dice_Lg;
           sides = idata->Dmg_Dice_Lg;
           bonus = idata->Dmg_Bonus_Lg + GetAdjDmgBonus() + idata->Attack_Bonus;
        }
        else 
        {
           num = idata->Nbr_Dice_Sm;
           sides = idata->Dmg_Dice_Sm;
           bonus = idata->Dmg_Bonus_Sm + GetAdjDmgBonus() + idata->Attack_Bonus;
        }
     }
     else 
     {
        WriteDebugString("Bogus item num in GetDamageDice()\n");
     }
  }
  else 
  {
    if (GetType()==MONSTER_TYPE)
    {
      int nbrAttacks = GetNbrAttacks();
      int currAttack = nbrAttacks - availAttacks;
      ASSERT( (currAttack>=0) && (currAttack<nbrAttacks) );
      
      MONSTER_DATA *pData = monsterData.GetMonsterData(origKey);
      ASSERT( pData!=NULL );

      if (pData == NULL) 
      {
        num   = unarmedNbrDieL;
        sides = unarmedDieL;
        bonus = GetAdjDmgBonus();
        return;
      }

      if (pData->attackData.GetCount() == 0)
      {
        WriteDebugString("Monster %s has zero attacks defined\n", pData->Name);
        num   = unarmedNbrDieL;
        sides = unarmedDieL;
        bonus = GetAdjDmgBonus();
        return;
      }

      if ((currAttack < 0)||(currAttack>=pData->attackData.GetCount()))
        currAttack=0;

      num = pData->attackData.GetAtIndex(currAttack).nbr;
      sides = pData->attackData.GetAtIndex(currAttack).sides;
      bonus = pData->attackData.GetAtIndex(currAttack).bonus;
      SpellDbKey = pData->attackData.GetAtIndex(currAttack).spelldbkey;
    }
    else
    {
      if (IsLarge)
      {
        num   = unarmedNbrDieL;
        sides = unarmedDieL;
        bonus = GetAdjDmgBonus();
      }
      else 
      {
        num   = unarmedNbrDieS;
        sides = unarmedDieS;
        bonus = unarmedBonus + GetAdjDmgBonus();
      }
    }
  }
}

//*****************************************************************************
// NAME:    COMBATANT::LoadCombatIcon
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
void COMBATANT::LoadCombatIcon()
{
  if (icon.filename.IsEmpty()) return;

  icon.FreePic();
  CString prefix;
  switch (GetType())
  {
  case MONSTER_TYPE: prefix="cm_"; break;
  default: prefix="cn_";break;
  }

  // load icon data as a sprite
  if (!icon.LoadPicSurfaces(prefix))
  {
    WriteDebugString("Failed to load icon file %s in LoadCombatIcon\n", icon.filename);

    // one last try
    icon.filename = DEFAULT_MONSTER_ICON;
    icon.picType=IconDib;
    icon.SetDefaults();
    if (!icon.LoadPicSurfaces(prefix))
    {
      SetMiscError(MissingMonsterCombatIcons);
      WriteDebugString("Failed to load default combat icon %s in LoadCombatIcon()\n",
                       icon.filename);
      icon.FreePic();
      ASSERT(FALSE);
    }
  }
}

//*****************************************************************************
// NAME:    COMBATANT::determineIconSize
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
void COMBATANT::determineIconSize()
{
  int w,h,bits;
  if (GraphicsMgr.GetSpriteSurfaceStats(icon.key, w, h, bits))
  {
    int imagewidth=w; // pixel width

    // (width / frame_width) / 2_frames_per_icon
    w = (w / COMBAT_TILE_WIDTH) / 2;

    // 2 frame minimum (one of each pose, rdy/attack)
    // frame count is multiple of 2
    // the icon frame pair used is indicated by iconIndex
    // adjust for number of frames in icon
    w /= (icon.NumFrames/2);
    
    h = h / COMBAT_TILE_HEIGHT;
    
    if (w < 1) w = 1;
    if (h < 1) h = 1;

    width = w;
    height= h;
    int offset = (iconIndex-1)*((width*COMBAT_TILE_WIDTH)*2);
    if ((offset+(width*COMBAT_TILE_WIDTH)) >= (imagewidth-width))
      iconIndex=1;
  }
  else
    icon.FreePic();
}

//*****************************************************************************
// NAME:    COMBATANT::GetAttackBonus
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
int COMBATANT::GetAttackBonus()
{
  int bonus = 0;

  if (myItems.GetReady(WeaponHand) != NO_READY_ITEM)
  {  
     ITEM_DATA *idata = itemData.GetItemData(myItems.GetItem(myItems.GetReady(WeaponHand)));
     if (idata != NULL)
     {      
       bonus += idata->Attack_Bonus;
     }
     else 
     {
        WriteDebugString("Bogus item num in GetAttackBonus()\n");
     }
  }

  bonus += GetAdjHitBonus();

  return bonus;
}

//*****************************************************************************
// NAME:    COMBATANT::TakeDamage
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
void COMBATANT::TakeDamage(int dmg, BOOL IsNonLethal)
{  
  // don't beat a dead horse!
  // *but!* allow dying to take 1 hp damage per turn if not yet bandaged
  //
  charStatusType stype = GetAdjStatus();
  if (   (stype == Fled)
      || (stype == Gone)
      || (stype == TempGone)
      || (stype == Dead))
    return;

  if (IsNonLethal) dmg=0;

  // adjust raw hitpoints
  SetHitPoints(GetHitPoints()-dmg);

  int trueHp = GetHitPoints();
  int startTrueHp = trueHp;

  // can't go below -10
  // and 10 = dead
  if (trueHp < -10)
    trueHp = -10;

  if (!IsPartyMember())
  {
    // monster die when hp reaches zero
    if (trueHp <= 0)
      trueHp = -10;
    // unless configured to not die
    if ((trueHp < 1) && (GetConfigMonsterNoDeath())) // debug flag
      trueHp = 1;
    if ((trueHp < 1) && (HasDeathImmunity())) // monster immunity flag
      trueHp = 1; 
  }

  // set to modified value if needed
  if (trueHp != startTrueHp)
    SetHitPoints(trueHp);

  // now get adjusted hp value, which can
  // magically change the current hp
  //
  int adjHp = GetAdjHitPoints();

  if (!IsPartyMember())
  {
    if (adjHp < 0)
      adjHp = -10;

    // still can't let monsters die, even if hp is 
    // magically below 0
    if ((adjHp < 1) && (GetConfigMonsterNoDeath()))
      adjHp = 1;
    if ((adjHp < 1) && (HasDeathImmunity())) // monster immunity flag
      adjHp = 1;     
  }

  if (adjHp <= -10)
  {
    SetStatus(Dead);
    placeCombatant(x, y, NO_DUDE, width, height);
    placeDyingCombatant(x,y,NO_DUDE,width,height);
    TRACE("%i takes %i hp damage, -10 left, and is dead\n", self, dmg);
    blitDeadSprite();
  }
  else if (adjHp <= 0)
  {
    ASSERT( IsPartyMember() );
    placeCombatant(x, y, NO_DUDE, width, height);
    placeDyingCombatant(x,y,self,width,height);
    SetStatus(Dying);
    isBandaged = FALSE;
    bandageWho=-1;
    TRACE("%i takes %i hp damage, %i left, and is dying\n", self, dmg, adjHp);
    blitDyingSprite();
  }

  if (adjHp <= 0)
  {
    // update stats used for monster morale
    if (friendly)
     IncNumFriendSlain();
    else
     IncNumMonsterSlain();

    // if dead, this dude can't do anything else    
    StopCasting(TRUE); // also calls EndTurn()
    movement = GetAdjMaxMovement();          
  }
  else if (dmg != 0) // cancel any pending spell casting if damage taken
    StopCasting(FALSE); // does not call EndTurn()

  UpdateSpellForDamage(dmg);
}

//*****************************************************************************
// NAME:    COMBATANT::PlayHit
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
void COMBATANT::PlayHit()
{
  if (myItems.GetReady(WeaponHand) != NO_READY_ITEM)
  {  
     itemData.PlayHit(myItems.GetItem(myItems.GetReady(WeaponHand)));
  }
  else
  { 
    if (GetType() == MONSTER_TYPE)
      monsterData.PlayHit(origKey);
    else
      PlayCharHit();
  }
}

//*****************************************************************************
// NAME:    COMBATANT::PlayMiss
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
void COMBATANT::PlayMiss()
{
  if (myItems.GetReady(WeaponHand) != NO_READY_ITEM)
  {  
     itemData.PlayMiss(myItems.GetItem(myItems.GetReady(WeaponHand)));
  }
  else
  { 
    if (GetType() == MONSTER_TYPE)
      monsterData.PlayMiss(origKey);
    else
      PlayCharMiss();
  }
}

//*****************************************************************************
//    NAME: COMBATANT::PlayLaunch
//
// PURPOSE: 
//
//*****************************************************************************
void COMBATANT::PlayLaunch()
{
  if (myItems.GetReady(WeaponHand) != NO_READY_ITEM)
  {  
     itemData.PlayLaunch(myItems.GetItem(myItems.GetReady(WeaponHand)));
  }
}

//*****************************************************************************
// NAME: canAttack
//
// PURPOSE:  
// check non-magical attack capabilities
//*****************************************************************************
BOOL COMBATANT::canAttack(int targ)
{  
  if (availAttacks <= 0)
    return FALSE;

  if (targ == NO_DUDE)
    return FALSE;

  if (   (availAttacks < 1.0)
     && (CurrCombatRound() - lastAttackRound <= 1))
    return FALSE;

  // cannot attack yourself
  if (targ == self)
    return FALSE;  

  COMBATANT *targCOMBATANT;
  targCOMBATANT = GetCombatantPtr(targ);
  ASSERT(targCOMBATANT!=NULL);
  if (targCOMBATANT==NULL) return FALSE;
  int aX = targCOMBATANT->x;
  int aY = targCOMBATANT->y;

  // if attacking dude on same side of battle
  if (targCOMBATANT->GetIsFriendly() == GetIsFriendly())
  {
    // AI will never attack dudes on same side
    if (OnAuto()) // includes all monsters (always auto), and auto dudes.
    {
      return FALSE;
    }
    else if (targCOMBATANT->GetType() == CHAR_TYPE)
    {
      // cannot attack pc party members with weapon
      return FALSE;
    }
    else if ((targCOMBATANT->GetType() == NPC_TYPE)&&(!targCOMBATANT->IsPreGen))
    {      
      // if player chooses to attack friendly npc, the npc changes sides,
      // even if unable to perform attack due to following checks.
      targCOMBATANT->friendly = !targCOMBATANT->friendly;
      // npc will leave the battle (and the party after combat)?
      //targCOMBATANT.SetMorale(0);
    }
  }

  int dis = distance(self, x, y, targ, aX, aY);  
  int mywpnitemidx = myItems.GetReady(WeaponHand);
  if (mywpnitemidx != NO_READY_ITEM)
  {       
     // using a weapon rather than natural attack types (ie claws/jaws/fists)
     int wpnitemdbkey = myItems.GetItem(mywpnitemidx);
 
     if (myItems.GetQty(mywpnitemidx) <= 0)
        return FALSE;
 
     if (!WpnCanAttackAtRange(wpnitemdbkey, dis))
       return FALSE;

     switch (itemData.GetWpnType(wpnitemdbkey)) 
     {
     case NotWeapon: // not weapon, must be used, invokes special abilities
        if (isMagical(wpnitemdbkey))
        {
           if (myItems.GetCharges(mywpnitemidx) <= 0)
              return FALSE;
        }
        else
           return FALSE;
        break;

     case HandBlunt: // hand, no throw, blunt
     case HandCutting: // hand, no throw, cutting
     case HandThrow: // hand or throw
     case SlingNoAmmo: // sling
       //
       // handled by WpnCanAttackAtRange()
       //
       break;

     case Bow: // bow
     case Crossbow: // crossbow
       {
         int myammoitemidx = myItems.GetReady(AmmoQuiver);         

         // arrows or bolts must be readied in AmmoQuiver
         if (myammoitemidx == NO_READY_ITEM)
           return FALSE;

         int ammoitemdbkey = myItems.GetItem(myammoitemidx);

         if (itemData.GetWpnType(ammoitemdbkey) != Ammo)
           return FALSE;

         // ammo class must match between weapon and ammo
         CString myammoclass(myItems.GetAmmoClass(myammoitemidx));
         if (myammoclass.IsEmpty()) return FALSE; // might be wand,potion,amulet,etc

         if (myammoclass != myItems.GetAmmoClass(mywpnitemidx))
           return FALSE;

         // must have non-zero qty of ammo
         if (myItems.GetQty(myammoitemidx) <= 0)
           return FALSE;
       }
       break;

     case Throw:
       //
       // handled by WpnCanAttackAtRange()
       //
       /*
        if (dis < 2)
           return FALSE;
        */
        break;

     case Ammo: // ammo - arrows or bolts
        return FALSE;

     default:
        WriteDebugString("Unhandled item Wpn_Type in canAttack()\n");
        return FALSE;
     }
  }
  else
  {
     // use natural attack weapons (ie claws, jaws, paws, fists)
     if (dis > 1)
        return FALSE;
  }
 
  // passed all tests so far, now check for line of sight
  if (!HaveLineOfSight(GetCenterX(), GetCenterY(), aX, aY))
    return FALSE;

  if (dis > 1)
  {
    if (!GetAdjDetectingInvisible())
    {
      // cannot attack invisible targets with ranged weapons
      if (targCOMBATANT->GetAdjSpecAb(SA_Invisible))
        return FALSE;

      if (GetUndeadType() != NotUndead)
      {
        if (targCOMBATANT->GetAdjSpecAb(SA_InvisibleToUndead))
          return FALSE;
      }

      if (IsAnimal())
      {
        if (targCOMBATANT->GetAdjSpecAb(SA_InvisibleToAnimals))
          return FALSE;
      }
    }
  }

  return TRUE;
}

//*****************************************************************************
// NAME:    COMBATANT::moveNW
//
// PURPOSE: 
//*****************************************************************************
void COMBATANT::moveNW()
{
   StopCasting(FALSE);
   state = CS_Moving;
   //combatData.SetUpdateFlag();

   if (facing != FACE_WEST)
      facing = FACE_WEST;
   else 
   {
     moveX = x-1;
     moveY = y-1;
   }   
}

//*****************************************************************************
// NAME:    COMBATANT::moveNE
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
void COMBATANT::moveNE()
{
   StopCasting(FALSE);
   state = CS_Moving;
   //combatData.SetUpdateFlag();

   if (facing != FACE_EAST)
      facing = FACE_EAST;
   else 
   {
     moveX = x+1;
     moveY = y-1;
   }   
}

//*****************************************************************************
// NAME:    COMBATANT::moveSE
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
void COMBATANT::moveSE()
{
   StopCasting(FALSE);
   state = CS_Moving;
   //combatData.SetUpdateFlag();

   if (facing != FACE_EAST)
      facing = FACE_EAST;
   else 
   {
     moveX = x+1;
     moveY = y+1;
   }   
}

//*****************************************************************************
// NAME:    COMBATANT::moveSW
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
void COMBATANT::moveSW()
{
   StopCasting(FALSE);
   state = CS_Moving;
   //combatData.SetUpdateFlag();

   if (facing != FACE_WEST)
      facing = FACE_WEST;
   else 
   {
     moveX = x-1;
     moveY = y+1;
   }   
}

//*****************************************************************************
// NAME:    COMBATANT::moveForward
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
void COMBATANT::moveForward()
{
   StopCasting(FALSE);
  state = CS_Moving;
   //combatData.SetUpdateFlag();
   moveX = x;
   moveY = y-1;
}

//*****************************************************************************
// NAME:    COMBATANT::moveBackward
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
void COMBATANT::moveBackward()
{
   StopCasting(FALSE);
   state = CS_Moving;
   //combatData.SetUpdateFlag();
   moveX = x;
   moveY = y+1;
}

//*****************************************************************************
// NAME:    COMBATANT::moveLeft
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
void COMBATANT::moveLeft()
{
   StopCasting(FALSE);
   state = CS_Moving;
   //combatData.SetUpdateFlag();

   if (facing != FACE_WEST)
      facing = FACE_WEST;
   else 
   {
     moveX = x-1;
     moveY = y;
   }   
}

//*****************************************************************************
// NAME:    COMBATANT::moveRight
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
void COMBATANT::moveRight()
{
   StopCasting(FALSE);
   state = CS_Moving;
   //combatData.SetUpdateFlag();

   if (facing != FACE_EAST)
      facing = FACE_EAST;
   else 
   {
     moveX = x+1;
     moveY = y;
   }   
}

//*****************************************************************************
// NAME:    COMBATANT::MoveCombatant
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
BOOL COMBATANT::MoveCombatant(int newX, int newY)
{
  int dude;
  BOOL isUpdate = FALSE;

  // if already at destination
  if ((newX == x) && (newY == y))
    return FALSE;

  if (GetType() == MONSTER_TYPE)
  {
    if (GetConfigMonsterNoMove())
      return FALSE;
  }

  PATH_DIR dir = GetDir(x, y, newX, newY);
  int movePoints = GetDist(dir);
  FaceDirection(dir);

  if (movement < GetAdjMaxMovement()-(movePoints-1))
  {     
    if (coordsOnMap(newX, newY, width, height))
    {     
      dude = getCombatantInCell(newX, newY, width, height, self);      

      if (dude != NO_DUDE)
      {        
        if (canAttack(dude))
        {
          AddTarget(dude);
          StartAttack(dude);
          isUpdate = TRUE;
        }
        /*
        else
        {
          // unable to take step, or to attack
          // dude blocking path, or no attacks remaining
          if (OnAuto())
          {
            // maybe should call Think() here?
            TRACE("Auto combatant %i cannot attack targ in path, end turn + guard\n", self);
            EndTurn(CS_Guarding); // sets state to none, done=true
          }
        }
        */
      }
      // check for walls and such
      else if (!isObstical(newX, newY, width, height, FALSE))
      {        
        // if moving away from an enemy that has no target,
        // the enemy gets to make an attack at this dude's
        // retreating backside.

        PlayCombatMove();        
        movement += movePoints;

        // remove dude from old location
        placeCombatant(x, y, NO_DUDE, width, height);

        TRACE("MoveCombatant() from %u,%u to %u,%u\n",
                    x,y,newX,newY);
      
        if ((abs(newX-x) > 1) || (abs(newY-y) > 1))
          TRACE("Moving more than 1 square\n");

        x = newX;
        y = newY;

        // place dude at new location
        placeCombatant(x, y, self, width, height);

        if (ActiveSpellData.LingerSpellAffectsTarget(self, x,y))
        {
          // A lingering spell is active at the location 
          // just moved to. 
          TRACE("%i moved into at least one active linger spell at location %i,%i\n",
                self, x,y);

          ActiveSpellData.ActivateLingerSpellsOnTarget(self, x,y);
        }

        // if just moved into square with adjacent guarding
        // enemy, the enemy gets to make an attack
        if (CheckForGuardingEnemy())
        {
          // This guys turn isn't over, but once the free
          // attacks are over, he will not start out in CS_Move.
          state = CS_None;
        }
        isUpdate = TRUE;
      }      
    }
    else 
    { 
       isUpdate = TRUE;
       // dude fled map
       SetStatus(Fled);
       if (friendly)
         IncNumFriendFlee(); //combatData.numFrndFlee++;
       else
         IncNumMonsterFlee(); //combatData.numMonFlee++;
       EndTurn();
    }
  }

  if ((isUpdate) && (charOnCombatMap()))
  {    
    ForceMapToMoveAlong(x, y, dir, max(width, height));
    PlaceCursorOnCurrentDude();  
  }

  return isUpdate;
}

//*****************************************************************************
//    NAME: COMBATANT::CheckForGuardingEnemy
//
// PURPOSE: 
//
//*****************************************************************************
BOOL COMBATANT::CheckForGuardingEnemy()
{
  // check clockwise around current location
  // looking for enemy in guarding state that
  // can make an attack.
  BOOL found = FALSE;
  int dude;
  POSITION pos;
  int d = PathNorth;

  COMBATANT *tempCOMBATANT;
  QueuedCombatantData &qcomb = GetQueuedCombatants();

  while (d <= PathNW)
  {
    FillTargeterList((PATH_DIR)d);
    pos = targeters.GetHeadPosition();
    while (pos != NULL)
    {
      dude = targeters.GetAt(pos);
      tempCOMBATANT = GetCombatantPtr(dude);
      ASSERT(tempCOMBATANT!=NULL);
      if (tempCOMBATANT==NULL) return FALSE;

      if (tempCOMBATANT!=NULL)
      {
        if (   (tempCOMBATANT->GetIsFriendly() != GetIsFriendly())          
            && (tempCOMBATANT->state == CS_Guarding)
            && (tempCOMBATANT->canAttack(self)))
        {
          // queue up the attackers
          qcomb.Push(dude, FALSE);
          // give self as target to each attacker
          tempCOMBATANT->AddTarget(self);        
          // set each attacker in attack mode (if possible)
          tempCOMBATANT->StartAttack(self);
          found = TRUE;
        }
      }
      targeters.GetNext(pos);
    }
    d++;
  }
  return found;
}

//*****************************************************************************
//    NAME: COMBATANT::FillTargeterList
//
// PURPOSE: 
//
//*****************************************************************************
void COMBATANT::FillTargeterList(PATH_DIR dir)
{
  targeters.RemoveAll();
  int i;
  int dude;

  switch (dir)
  {
  case PathNorth:
    for (i=x; i < x+width; i++)
    {
      if (coordsOnMap(i, y-1, 1, 1))
      {      
        if ((dude = getCombatantInCell(i, y-1, 1, 1, self)) != NO_DUDE)
          targeters.AddTail(dude);
      }
    }
    break;
  case PathNE:
    if (coordsOnMap(x+width, y-1, 1, 1))
    {      
      if ((dude = getCombatantInCell(x+width, y-1, 1, 1, self)) != NO_DUDE)
        targeters.AddTail(dude);
    }
    break;

  case PathEast:
    for (i=y; i < y+height; i++)
    {
      if (coordsOnMap(x+width, i, 1, 1))
      {      
        if ((dude = getCombatantInCell(x+width, i, 1, 1, self)) != NO_DUDE)
          targeters.AddTail(dude);
      }
    }
    break;
  case PathSE:
    if (coordsOnMap(x+width, y+height, 1, 1))
    {      
      if ((dude = getCombatantInCell(x+width, y+height, 1, 1, self)) != NO_DUDE)
        targeters.AddTail(dude);
    }
    break;
  case PathSouth:
    for (i=x; i < x+width; i++)
    {
      if (coordsOnMap(i, y+height, 1, 1))
      {      
        if ((dude = getCombatantInCell(i, y+height, 1, 1, self)) != NO_DUDE)
          targeters.AddTail(dude);
      }
    }
    break;
  case PathSW:
    if (coordsOnMap(x-1, y+height, 1, 1))
    {      
      if ((dude = getCombatantInCell(x-1, y+height, 1, 1, self)) != NO_DUDE)
        targeters.AddTail(dude);
    }
    break;
  case PathWest:
    for (i=y; i < y+height; i++)
    {
      if (coordsOnMap(x-1, i, 1, 1))
      {      
        if ((dude = getCombatantInCell(x-1, i, 1, 1, self)) != NO_DUDE)
          targeters.AddTail(dude);
      }
    }
    break;
  case PathNW:
    if (coordsOnMap(x-1, y-1, 1, 1))
    {      
      if ((dude = getCombatantInCell(x-1, y-1, 1, 1, self)) != NO_DUDE)
        targeters.AddTail(dude);
    }
    break;
  default:
    break;
  }
}
//*****************************************************************************
// NAME: charOnCombatMap
//
// PURPOSE:  
//*****************************************************************************
BOOL COMBATANT::charOnCombatMap()
{
  charStatusType stype = GetAdjStatus();
   if (   (charUnconscious())
       || (stype == Fled)
       || (stype == Gone)
       || (stype == TempGone)
       || (stype == Dead))
     return FALSE;

  return TRUE;
}

//*****************************************************************************
//    NAME: COMBATANT::charUnconscious
//
// PURPOSE: 
//
//*****************************************************************************
BOOL COMBATANT::charUnconscious()
{
  charStatusType stype = GetAdjStatus();
  if ((stype == Dying) || (stype == Unconscious)) return TRUE;
  return FALSE;
}

//*****************************************************************************
//    NAME: COMBATANT::charCanTakeAction
//
// PURPOSE: 
//
//*****************************************************************************
BOOL COMBATANT::charCanTakeAction()
{
  charStatusType stype = GetAdjStatus();
   if ((stype == Okay) || (stype == Running) || (stype == Animated))
     return TRUE;
  return FALSE;  
}

//*****************************************************************************
// NAME:    COMBATANT::RestoreToParty
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
void COMBATANT::RestoreToParty()
{
  if (!IsPartyMember())
    return;

  if (   ((GetType() == CHAR_TYPE) || (GetType() == NPC_TYPE))
      && (origKey >= 0))
  {     
    // need to restore original morale to characters
    // so that they begin each battle with normal morale.
    int origMorale = party.characters[origKey].GetMorale();

    // unless of course if the NPC has decided to leave
    // the party
    if ((GetType() == NPC_TYPE) && (!IsPreGen) && (GetMorale() <= 0)) 
      origMorale = 0;

    // if PartyNeverDies flag is set and dude has 0 or less hp,
    // restore to original hp with Okay status
    if (GetConfigPartyNeverDies())
    {
      SetStatus(party.characters[origKey].GetStatus());
      if (GetHitPoints() <= 0) SetHitPoints(1);
    }

    party.characters[origKey] = *((CHARACTER*)(this));
    party.characters[origKey].SetMorale(origMorale);
    
    // if dying when combat ends (between -10 and 0 hitpoints)
    // set to 0 hp and unconscious status
    if (   (party.characters[origKey].GetStatus() == Dying)
        || (party.characters[origKey].GetStatus() == Unconscious))
    {
      party.characters[origKey].SetStatus(Unconscious);
      if (GetHitPoints() < 0) 
        SetHitPoints(0);
    }

    // spells may have altered control status
    party.characters[origKey].SetAllowPlayerControl((GetType()==CHAR_TYPE) || (IsPreGen)); 
  }
}


//*****************************************************************************
// NAME: COMBATANT::DetermineInitialFacing
//
// PURPOSE:  
//*****************************************************************************
int COMBATANT::DetermineInitialFacing(int PartyFacing)
{   
  switch (PartyFacing)
  {
  case FACE_NORTH:
  case FACE_SOUTH:
    if (rollDice(100, 1, 0) <= 50) 
      return FACE_EAST;
    else 
      return FACE_WEST;

  case FACE_EAST:
    return FACE_EAST;
  case FACE_WEST:
    return FACE_WEST;
  }
  return FACE_EAST;
}

//*****************************************************************************
// NAME:    COMBATANT::InitFromCharData
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
void COMBATANT::InitFromCharData(int dude)
{
  *((CHARACTER*)(this)) = party.characters[dude];

  SetPartyMember(TRUE);
  state = CS_None;
  origKey = dude;
  //WriteDebugString("add 1: origIndex %i, origKey %i\n", origIndex, origKey);
  movement = 0;

  friendly = TRUE;  
  // NPC in party cannot be controlled in combat unless it is a 
  // pre-generated character
  SetAllowPlayerControl( (GetType()==CHAR_TYPE) || (IsPreGen) );
  facing = DetermineInitialFacing(party.facing);

  determineNbrAttacks(); // lookup max possible defaults
  if (GetAdjAutomatic())
  {
    ReadyBestArmor();
    ReadyBestWpn(NO_DUDE);
    ReadyBestShield();  
  }
  determineAvailAttacks(); // now take ready wpn into account
  //WriteDebugString("add 2: origIndex %i, origKey %i\n", origIndex, origKey);
}

//*****************************************************************************
// NAME:    COMBATANT::InitFromMonsterData
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
void COMBATANT::InitFromMonsterData(int dude, BOOL IsFriendly, ITEM_LIST &items, MONEY_SACK &msack)
{
  MONSTER_DATA *data;

  state = CS_None;
  if ((data = monsterData.GetMonsterData(dude)) == NULL)
  {
    WriteDebugString("Bogus monster index %u in InitFromMonsterData()\n", dude);    
    Clear();
    origKey = NO_DUDE;
    return;
  }

  //partyMember = FALSE;
  SetPartyMember(FALSE);
  SetType(MONSTER_TYPE);
  origKey =  dude;
  origIndex = dude;
  friendly = IsFriendly; 

  generateNewCharacter(data->XP_Value, START_EXP_VALUE); // determines monster AC,THACO,Hit Points,etc
  SetStatus(Okay);
  //SetAutomatic(!combatData.MonsterPlyrControl);
  SetAllowPlayerControl(GetConfigMonsterPlyrControl());

  SetName(data->Name);
  SetInt(data->Intelligence);
  SetMagicResistance(data->Magic_Resistance);
  SetSize(data->Size);
  SetMorale(data->Morale);  
  ItemMask = data->ItemMask;
  specAbs = data->specAbs;  
  SetUndead(data->undeadStatus);

  // items specified in combat event for this monster
  myItems = items;

  // default monster items in monster database
  POSITION pos = data->myItems.GetHeadPosition();
  while (pos!=NULL)
  {
    myItems.AddItem(data->myItems.GetNext(pos));
  }

  money = msack; // combat additional money
  money.Add(data->money); // monster default money

  icon = data->Icon;
  determineNbrAttacks();
  ReadyBestArmor();
  ReadyBestWpn(NO_DUDE);
  ReadyBestShield();  
  determineAvailAttacks();
  determineMaxMovement();

  if (GetConfigMonsterNoMove())
  {
    SetMaxMovement(0);
    movement = 0;
  }

  ASSERT(GetAdjHitPoints() > 0);
}

//*****************************************************************************
// NAME: COMBATANT::InitFromNPCData
//
// PURPOSE:  
//*****************************************************************************
void COMBATANT::InitFromNPCData(int dude, BOOL IsFriendly, ITEM_LIST &items, MONEY_SACK &msack)
{
  CHARACTER data;
  if (!globalData.charData.GetCharacterData(dude, data))
  {
    WriteDebugString("invalid NPC combatant index %i in InitFromNPCData()\n", dude);
    Clear();
    origKey = NO_DUDE;
    return;
  }

  *((CHARACTER*)(this)) = data;

  POSITION pos = items.GetHeadPosition();
  while (pos != NULL)
    myItems.AddItem(items.GetNext(pos));

  money = msack;
  //partyMember = FALSE;
  SetPartyMember(FALSE);
  state = CS_None;
  origKey = dude;
  origIndex = dude;
  SetType(NPC_TYPE);
  friendly = IsFriendly;
  movement = 0;
  SetStatus(Okay);
  //SetAutomatic(!combatData.MonsterPlyrControl);
  SetAllowPlayerControl(GetConfigMonsterPlyrControl());
  determineNbrAttacks();
  ReadyBestArmor();
  ReadyBestWpn(NO_DUDE);
  ReadyBestShield();  
  determineAvailAttacks();
  determineMaxMovement();

  if (GetConfigMonsterNoMove())
  {
    SetMaxMovement(0);
    movement = 0;
  }
}

//*****************************************************************************
// NAME: determineAvailAttacks
//
// PURPOSE:  
//*****************************************************************************
void COMBATANT::determineAvailAttacks()
{
  determineNbrAttacks(); // lookup max attacks
  availAttacks = GetNbrAttacks();

  float temp = availAttacks;
  TRACE("Setting num attacks for %i to %f\n", self, temp);     
}

//*****************************************************************************
// NAME: isLargeDude
//
// PURPOSE:  
//*****************************************************************************
BOOL COMBATANT::isLargeDude()
{
  creatureSizeType ctype = GetAdjSize();

  if (ctype == Large) return TRUE;
  if ((width > 1) || (height > 1)) return TRUE;
  if (IsAlwaysLarge()) return TRUE;
  
  return FALSE;
}

//*****************************************************************************
// NAME:    COMBATANT::RollInitiative
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
void COMBATANT::RollInitiative(eventSurpriseType surprise)
{
  BOOL partymember = (IsPartyMember() || friendly);
  switch (surprise)
  {
  case Neither:
    initiative = rollDice(10,1,0);
    if (GetAdjSpecAb(SA_Blindness))
      initiative += 2;
    break;
  case PartySurprised: // monsters go first
    initiative = (partymember ? 10 : 1);
    break;
  case MonsterSurprised: // party members go first
    initiative = (partymember ? 1 : 10);
    break;
  }

  if (initiative < 1) initiative = 1;
  if (initiative > 10) initiative = 10;
}

//*****************************************************************************
//    NAME: COMBATANT::GetCenterX
//
// PURPOSE: 
//
//*****************************************************************************
int COMBATANT::GetCenterX()
{
  if (width<=1) return x;
  if (facing == FACE_WEST)
    return (x+(width/2)-1);
  else
    return (x+(width/2));
}

//*****************************************************************************
//    NAME: COMBATANT::GetCenterY
//
// PURPOSE: 
//
//*****************************************************************************
int COMBATANT::GetCenterY()
{
  if (height<=1) return y;
  return (y+(height/2)-1);
}

//*****************************************************************************
//    NAME: COMBATANT::OnStartTurn
//
// PURPOSE: 
//
//*****************************************************************************
void COMBATANT::OnStartTurn()
{
  TRACE("OnStartTurn for %i\n", self);
  ClearQueuedSpecAb();
  PlaceCursorOnCurrentDude();
  
  CombatMsg="";
  ClearFormattedText(combatTextData);
  CString tmp;
  if (GetAdjSpecAb(SA_Curse))
  {
    tmp.Format(" \nCursed");
    CombatMsg += tmp;
  }
  if (GetAdjSpecAb(SA_CharmPerson))
  {
    tmp.Format(" \nCharmed");
    CombatMsg += tmp;
  }
  if (GetAdjSpecAb(SA_Sleep))
  {
    tmp.Format(" \nSleeping");
    CombatMsg += tmp;
  }
  if (GetAdjSpecAb(SA_Entangle))
  {
    tmp.Format(" \nEntangled");
    CombatMsg += tmp;
  }
  if (GetAdjSpecAb(SA_HoldPerson))
  {
    tmp.Format(" \nHeld");
    CombatMsg += tmp;
  }
  if (GetAdjSpecAb(SA_Silence))
  {
    tmp.Format(" \nSilenced");
    CombatMsg += tmp;
  }
  if (GetAdjSpecAb(SA_Poisoned))
  {
    tmp.Format(" \nPoisoned");
    CombatMsg += tmp;
  }
  if (GetAdjSpecAb(SA_Blindness))
  {
    tmp.Format(" \nBlinded");
    CombatMsg += tmp;
  }
  
  if (!CombatMsg.IsEmpty())
  {
    FormatCombatText(combatTextData, CombatMsg);
    UpdateCombatTimeDelayMsg();
    theApp.AppSleep(GetMsgTimeDelay());
    ClearFormattedText(combatTextData);
  }
}

//*****************************************************************************
//    NAME: COMBATANT::OnEndTurn
//
// PURPOSE: 
//
//*****************************************************************************
void COMBATANT::OnEndTurn()
{
  TRACE("OnEndTurn for %i\n", self);
}

//*****************************************************************************
//    NAME: COMBATANT::OnEnsureVisible
//
// PURPOSE: 
//
//*****************************************************************************
void COMBATANT::OnEnsureVisible()
{
  TRACE("OnEnsureVisible for %i\n", self);
}

//*****************************************************************************
//    NAME: COMBATANT::FormatSpecAbMsg
//
// PURPOSE: 
//
//*****************************************************************************
CString COMBATANT::FormatSpecAbMsg(DWORD sa_state)
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
        if (data.key==(DWORD)self)
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
  CString Target("");

  User = GetName();
  
  int targ = lastAttacked;//GetCurrTarget(); ** targ may be dead by this time
  if (targ != NO_DUDE)
  {
    COMBATANT *tempCOMBATANT=GetCombatantPtr(targ);
    if (tempCOMBATANT!=NULL)
      Target = tempCOMBATANT->GetName();
  }
  else
  {
    Target="the enemy";
  }

  msg.Replace("/u", User);
  msg.Replace("/t", Target);
  return msg;
}

//*****************************************************************************
//    NAME: COMBATANT::ModifyAttackRollDice
//
// PURPOSE: 
//
//*****************************************************************************
BOOL COMBATANT::ModifyAttackRollDice(CHARACTER *pTarget,int &num, int &sides, int &bonus)
{
  return CHARACTER::ModifyAttackRollDice(pTarget,num,sides,bonus);
}
//*****************************************************************************
//    NAME: COMBATANT::ModifyAttackDamageDice
//
// PURPOSE: 
//
//*****************************************************************************
BOOL COMBATANT::ModifyAttackDamageDice(CHARACTER *pTarget,int &num, int &sides, int &bonus, BOOL &NonLethal)
{
  return CHARACTER::ModifyAttackDamageDice(pTarget,num,sides,bonus,NonLethal);
}
//*****************************************************************************
//    NAME: COMBATANT::ModifyACAsTarget
//
// PURPOSE: 
//
//*****************************************************************************
BOOL COMBATANT::ModifyACAsTarget(CHARACTER *pAttacker, int &ac, int itemkey)
{
  DWORD src;
  DWORD skey;
  BOOL modify = CHARACTER::ModifyACAsTarget(pAttacker,ac,itemkey);

  if (GetAdjSpecAb(SA_Displacement,&src, &skey))
  {
    COMBATANT *pa = (COMBATANT*)pAttacker;
    QueueUsedSpecAb(SA_Displacement,src,skey);
    modify=TRUE;
    if (pa->self != lastAttacker) // if first attack on me
      ac=21; // force a miss
    else
      ac -= 2; // +2 bonus to armor class
  }
  
  if (GetAdjSpecAb(SA_MirrorImage,&src, &skey))
  {
    QueueUsedSpecAb(SA_MirrorImage,src,skey);
    // decrement counter used to keep track of number of mirror images
    //AdjustEffectData("$CHAR_MIRRORIMAGE", -1);
    modify=TRUE;
    ac=21; // force a miss
  }

  if (GetAdjSpecAb(SA_Blindness,&src, &skey))
  {
    QueueUsedSpecAb(SA_Blindness,src,skey);
    modify=TRUE;
    ac += 4;
  }
  
  if (race() == Dwarf)
  {
    if (pAttacker->HasDwarfACPenalty())
    {
      ac -= 4; // +4 bonus to armor class
      modify=TRUE;
    }
  }
  else if (race() == Gnome)
  {
    if (pAttacker->HasGnomeACPenalty())
    {
      ac -= 4; // +4 bonus to armor class    
      modify=TRUE;
    }
  }
  return modify;
}
//*****************************************************************************
//    NAME: COMBATANT::ModifyAttackThaco
//
// PURPOSE: 
//
//*****************************************************************************
BOOL COMBATANT::ModifyAttackThaco(CHARACTER *pTarget,int &val)
{
  DWORD src;
  DWORD skey;
  BOOL modify = CHARACTER::ModifyAttackThacoAsTarget(pTarget,val);

  if (pTarget->GetAdjSpecAb(SA_Sanctuary,&src, &skey))
  {
    if (!didCharacterSaveVs(Sp,0))
    {
      val=50; // cannot hit target
      RemoveAllTargets();
      pTarget->QueueUsedSpecAb(SA_Sanctuary,src,skey);
      modify=TRUE;
    }
  }
  
  if (GetAdjSpecAb(SA_Blindness,&src, &skey))
  {
    QueueUsedSpecAb(SA_Blindness,src,skey);
    modify=TRUE;
    val += 4; // -4 penalty to THACO if blinded
  }

  if (race() == Dwarf)
  {
    if (pTarget->HasDwarfTHACOPenalty())
    {
      val -= 1; // +1 bonus to THACO
      modify=TRUE;
    }
  }
  else if (race() == Gnome)
  {
    if (pTarget->HasGnomeTHACOPenalty())
    {
      val -= 1; // +1 bonus to THACO
      modify=TRUE;
    }
  }
  
  return modify;
}
//*****************************************************************************
//    NAME: COMBATANT::ModifyAttackRollDiceAsTarget
//
// PURPOSE: 
//
//*****************************************************************************
BOOL COMBATANT::ModifyAttackRollDiceAsTarget(CHARACTER *pAttacker,int &num, int &sides, int &bonus)
{
  return CHARACTER::ModifyAttackRollDiceAsTarget(pAttacker,num,sides,bonus);
}
//*****************************************************************************
//    NAME: COMBATANT::ModifyAttackDamageDiceAsTarget
//
// PURPOSE: 
//
//*****************************************************************************
BOOL COMBATANT::ModifyAttackDamageDiceAsTarget(CHARACTER *pAttacker,int &num, int &sides, int &bonus, BOOL &NonLethal)
{
  return CHARACTER::ModifyAttackDamageDiceAsTarget(pAttacker,num,sides,bonus,NonLethal);
}
//*****************************************************************************
//    NAME: COMBATANT::ModifyAttackThacoAsTarget
//
// PURPOSE: 
//
//*****************************************************************************
BOOL COMBATANT::ModifyAttackThacoAsTarget(CHARACTER *pAttacker,int &val)
{
  return CHARACTER::ModifyAttackThacoAsTarget(pAttacker,val);
}
//*****************************************************************************
//    NAME: COMBATANT::ModifySaveRoll
//
// PURPOSE: 
//
//*****************************************************************************
BOOL COMBATANT::ModifySaveRoll(CHARACTER *pTarget, int &roll)
{
  return CHARACTER::ModifySaveRoll(pTarget,roll);
}
//*****************************************************************************
//    NAME: COMBATANT::ModifySaveRollAsTarget
//
// PURPOSE: 
//
//*****************************************************************************
BOOL COMBATANT::ModifySaveRollAsTarget(CHARACTER *pAttacker, int &roll)
{
  return CHARACTER::ModifySaveRollAsTarget(pAttacker,roll);
}
//*****************************************************************************
//    NAME: COMBATANT::ModifyAttackRollDiceForItem
//
// PURPOSE: 
//
//*****************************************************************************
BOOL COMBATANT::ModifyAttackRollDiceForItem(CHARACTER *pTarget,int itemdbkey,int &num, int &sides, int &bonus)
{
  return CHARACTER::ModifyAttackRollDiceForItem(pTarget,itemdbkey,num,sides,bonus);
}

//*****************************************************************************
//    NAME: COMBATANT::ModifyAttackRollDiceForItemAsTarget
//
// PURPOSE: 
//
//*****************************************************************************
BOOL COMBATANT::ModifyAttackRollDiceForItemAsTarget(CHARACTER *pAttacker,int itemdbkey,int &num, int &sides, int &bonus)
{
  return CHARACTER::ModifyAttackRollDiceForItemAsTarget(pAttacker,itemdbkey,num,sides,bonus);
}

//*****************************************************************************
//    NAME: COMBATANT::ModifyAttackDamageDiceForItem
//
// PURPOSE: 
//
//*****************************************************************************
BOOL COMBATANT::ModifyAttackDamageDiceForItem(CHARACTER *pTarget,int itemdbkey,int &num, int &sides, int &bonus, BOOL &NonLethal)
{
  return CHARACTER::ModifyAttackDamageDiceForItem(pTarget,itemdbkey,num,sides,bonus,NonLethal);
}

//*****************************************************************************
//    NAME: COMBATANT::ModifyAttackDamageDiceForItemAsTarget
//
// PURPOSE: 
//
//*****************************************************************************
BOOL COMBATANT::ModifyAttackDamageDiceForItemAsTarget(CHARACTER *pAttacker,int itemdbkey,int &num, int &sides, int &bonus, BOOL &NonLethal)
{
  ITEM_DATA *pItem = itemData.GetItemData(itemdbkey);
  if (pItem==NULL) return FALSE;

  DWORD src;
  DWORD skey;
  BOOL modify = CHARACTER::ModifyAttackDamageDiceForItemAsTarget(pAttacker,itemdbkey,num,sides,bonus,NonLethal);
  
  COMBATANT *pa = (COMBATANT*)pAttacker;
  // if vorpal item readied (attacker has ability enabled), and using item that confers ability
  if (   (pa->AttDiceRoll==20)
      && (pa->GetAdjSpecAb(SA_VorpalAttack,&src, &skey))
      && (pItem->specAbs.HaveSpecAb(SA_VorpalAttack)))
  {    
    if (!HasVorpalImmunity())
    {
      bonus = GetHitPoints()+1; // make sure this attack kills target
      pa->QueueUsedSpecAb(SA_VorpalAttack,src,skey);
      modify=TRUE;
    }
  }
  return modify;
}

//*****************************************************************************
//    NAME: COMBATANT::GetIsFriendly
//
// PURPOSE: 
//
//*****************************************************************************
BOOL COMBATANT::GetIsFriendly()
{
  BOOL AdjFriendly = GetAdjSpecAb(SA_CharmPerson);
  if (AdjFriendly) 
    return (!friendly);
  else
    return (friendly);
}