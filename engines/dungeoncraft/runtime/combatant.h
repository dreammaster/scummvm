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

#ifndef DUNGEONCRAFT_SHARED_COMBATANT
#define DUNGEONCRAFT_SHARED_COMBATANT

#include "dungeoncraft/shared/SharedQueue.h"
#include "dungeoncraft/shared/monster.h"
#include "dungeoncraft/shared/spell.h"
#include "dungeoncraft/shared/Path.h"

namespace DungeonCraft {

// make sure this enum has the same order as CombatStateType
//
enum combatantStateType 
{ 
  CS_None, 
  CS_Casting, 
  CS_Attacking, 
  CS_Guarding, 
  CS_Bandaging, 
  CS_Using,
  CS_Moving,
  CS_Turning,
  CS_Fleeing 
};

const int NUM_COMBATANT_STATES = 9;
extern char *CombatantStateText[NUM_COMBATANT_STATES];

struct COMBATANT : public CHARACTER
{
  COMBATANT() : CHARACTER() { Clear(); }
  ~COMBATANT() {  }
  COMBATANT& operator=(COMBATANT& src);
  
  enum { MAX_COMBAT_TARGETS=100 };

  combatantStateType GetState() { return state; }
  int GetUniqueId() { return self; }

  void Clear();
  void InitFromCharData(int dude);
  void InitFromMonsterData(int dude, BOOL IsFriendly, ITEM_LIST &items, MONEY_SACK &msack);
  void InitFromNPCData(int dude, BOOL IsFriendly, ITEM_LIST &items, MONEY_SACK &msack);
  void determineAvailAttacks();
  void LoadCombatIcon();
  void determineIconSize();
  BOOL isLargeDude();
  void RollInitiative(eventSurpriseType surprise);
  BOOL charOnCombatMap();
  BOOL charUnconscious();
  BOOL charCanTakeAction();
  void GetDamageDice(int wpn, int &num, int &sides, int &bonus, BOOL &NonLethal, BOOL IsLarge, int &SpellDbKey);
  int GetAttackBonus();
  ActorType GetContext(WORD ClassFlag=0);

  BOOL CastSpell(int spell);
  void SpellActivate(PENDING_SPELL &data);
  void InstantSpellActivate(int spelldbkey, int target);    
  BOOL IsSpellPending();

  // one time only, used for temp bonuses on such attacks as backstabbing
  void SetTempAttackBonus(int att, int dmg) { tmpAttDiceBonus=att;tmpDmgDiceBonus=dmg; }

  void FaceAttacker(int attacker);
  void FaceLocation(int x, int y);
  void FaceDirection(PATH_DIR dir);
  BOOL CheckForGuardingEnemy();
  void FillTargeterList(PATH_DIR dir);
  
  void PlayHit();
  void PlayMiss();
  void PlayLaunch();
  void PlayCombatDeath();
  void PlayCombatMove();
  void PlayCombatTurnUndead();

  void TakeDamage(int dmg, BOOL IsNonLethal);
  void RestoreToParty();  
  
  BOOL UpdateCombatant();
  BOOL Think();
  void HandleCombatRoundMsgExpired();
  void HandleTimeDelayMsgExpired();
  void HandleTimeDelayMsgBegin();
  void PostCombatScreenUpdate();
  void HandleCurrState();

  void displayCombatSprite();
  void blitDeadSprite();
  void blitDyingSprite();

  void AddTarget(int newTarget);
  BOOL AddTarget(COMBATANT &dude, int range=0);
  BOOL AddMapTarget(int mapx, int mapy, PATH_DIR dir);
  BOOL AddTargetSelf();
  void AutoChooseSpellTargets();
  void RemoveAllTargets();
  void SetCurrTarget();
  void RemoveCurrTarget();
  int  GetCurrTarget();
  int  GetNextTarget();
  int  GetMaxTargets();
  int  GetNumTargets() const { return combattargets.GetCount(); }
  BOOL HaveTarget(int target);
  void RemoveTarget(int target);

  BOOL canAttack(int theAttackee);
  BOOL StartAttack(int target);
  void makeAttack(int target);
  void AttackOver();
  void StopAttack();
  double AttacksRemaining() const { return availAttacks; }
  BOOL UseAttackPose();

  BOOL NeedMissileAnimation();
  void InitMissileAnimation();
  BOOL NeedHitAnimation();
  void InitHitAnimation();
  BOOL NeedSpellCastAnimation();
  void InitSpellCastAnimation(int targ);
  BOOL NeedSpellInRouteAnimation(int x, int y);
  void InitSpellInRouteAnimation(int x, int y);
  BOOL NeedSpellCoverageAnimation();
  void InitSpellCoverageAnimation(int x, int y);
  BOOL NeedSpellHitAnimation();
  void InitSpellHitAnimation(int targ);
  BOOL NeedSpellLingerAnimation();
  void InitSpellLingerAnimation(int mapx, int mapy);

  BOOL CanTarget();
  BOOL CanMove();
  BOOL CanCast();
  BOOL CanUse();
  BOOL CanTurnUndead();
  BOOL CanDelay();
  BOOL CanBandage();
  BOOL CanGuard();

  void Bandage();
  void EndTurn(combatantStateType newState=CS_None);
  void Quick(BOOL Enable);
  void Guard();
  void DelayAction(); 
  void TurnUndead();
  void StartInitialSpellCasting(int key);
  void EndInitialSpellCasting();

  BOOL IsDone(); 
  BOOL IsAttacking() const { return (state == CS_Attacking); }
  BOOL IsGuarding() const { return (state == CS_Guarding); }

  BOOL IsCasting();
  void StopCasting(BOOL EndTurn);
  int  GetSpellBookKeyBeingCast() const;
  void SetSpellBookKeyBeingCast(int key);

  BOOL OnAuto() { return GetAdjAutomatic(); }
  void StopMoving() { state = CS_None; moveX=-1; moveY=-1;}  

  void moveLeft();
  void moveRight();
  void moveForward();
  void moveBackward();
  void moveNW();
  void moveNE();
  void moveSW();
  void moveSE();
  BOOL CurrMoveWouldFleeMap();

  BOOL MoveCombatant(int newX, int newY);
  BOOL FindPathTo(int dx, int dy, BOOL CheckOccupants);
  BOOL FindPathAwayFrom(int fleeFrom);
  int FindPathToMapNorthEdge(void);
  int FindPathToMapEastEdge(void);
  int FindPathToMapSouthEdge(void);
  int FindPathToMapWestEdge(void);
  BOOL FindPathToMapEdge();
  BOOL FindPathAlongLine(PATH_DIR dir, int dist);
  void ClearPath();
  BOOL TakeNextStep();
 
  void ReadyBestWpn(int targ);
  BOOL toggleReadyItem(int item);
  BOOL delCharacterItem(int index, int qty);

  int DetermineInitialFacing(int PartyFacing);
  void CheckMorale();

  DWORD MakeTargetKey(int dude, int dist) { return ((((DWORD)dist) << 16) | ((DWORD)dude)); }
  int   GetTargetKeyDude(DWORD k) const { return (k & 0x0000FFFF); }
  int   GetTargetKeyDist(DWORD k) const { return ((k >> 16) & 0x0000FFFF); }

  int GetCenterX();
  int GetCenterY();

  BOOL GetIsFriendly();
  
  void OnStartTurn();
  void OnEndTurn();
  void OnEnsureVisible();

  CString FormatSpecAbMsg(DWORD sa_state);


  // These ModifyXXX functions dynamically alter character
  // values based on spell effects or special abilities.
  BOOL ModifyAttackRollDice(CHARACTER *pTarget,int &num, int &sides, int &bonus);
  BOOL ModifyAttackDamageDice(CHARACTER *pTarget,int &num, int &sides, int &bonus, BOOL &NonLethal);
  BOOL ModifyACAsTarget(CHARACTER *pAttacker, int &ac, int itemkey);
  BOOL ModifyAttackThaco(CHARACTER *pTarget,int &val);
  BOOL ModifyAttackRollDiceAsTarget(CHARACTER *pAttacker,int &num, int &sides, int &bonus);
  BOOL ModifyAttackDamageDiceAsTarget(CHARACTER *pAttacker,int &num, int &sides, int &bonus, BOOL &NonLethal);
  BOOL ModifyAttackThacoAsTarget(CHARACTER *pAttacker,int &val);
  BOOL ModifySaveRoll(CHARACTER *pTarget, int &roll);
  BOOL ModifySaveRollAsTarget(CHARACTER *pAttacker, int &roll);
  BOOL ModifyAttackRollDiceForItem(CHARACTER *pTarget,int itemdbkey,int &num, int &sides, int &bonus);
  BOOL ModifyAttackRollDiceForItemAsTarget(CHARACTER *pAttacker,int itemdbkey,int &num, int &sides, int &bonus);
  BOOL ModifyAttackDamageDiceForItem(CHARACTER *pTarget,int itemdbkey,int &num, int &sides, int &bonus, BOOL &NonLethal);
  BOOL ModifyAttackDamageDiceForItemAsTarget(CHARACTER *pAttacker,int itemdbkey,int &num, int &sides, int &bonus, BOOL &NonLethal);

  int self;
	int x,y;
  int hPath;
	int initiative; // 1..10
	int target; // index of target
	int facing;
  int movement;
  int moveX;
  int moveY;
  int lastAttackRound;
  double availAttacks;
  BOOL continueAttack; // interrupt a series of multiple attacks?
  BOOL isFleeing;
  BOOL isTurned;  // this guy has been turned and is fleeing from the cleric that did it
  BOOL hasTurnedUndead; // only get to turn undead once per combat
  int  lastAttacker; // who tried to hit us most recently?
  int  lastAttacked; // who did we try to hit most recently?
	BOOL done;
  BOOL isBandaged;
  int  bandageWho;
  BOOL didMove;
  combatantStateType state;
	BOOL friendly;
  BOOL blitDeathTile;
  int spellBookKeyBeingCast;
  int width;
  int height;
  POSITION targetPos;
  int tmpAttDiceBonus;
  int tmpDmgDiceBonus;
  int origKey;
  int pendingSpellKey;
  int activeSpellKey;
  BOOL forceAttackPose;
  int AttDiceSides;
  int AttDiceNum;
  int AttDiceRoll;


  OrderedQueue<int, MAX_COMBAT_TARGETS> combattargets; // who am I targeting?
  // temp list to check perimeter of self for attackers.
  CList<int, int> targeters; 
};

struct QueuedCombatant
{
  QueuedCombatant() { Dude=NO_DUDE; AffectStats=TRUE; }
  QueuedCombatant(int dude, BOOL stats) { Dude=dude; AffectStats=stats; }

  BOOL AffectStats;
  int Dude;
};

struct QueuedCombatantData
{
  QueuedCombatantData() { Clear(); }
  void Clear() { QComb.RemoveAll(); }
  void Push(int dude, BOOL stats)
  {
    QueuedCombatant temp(dude,stats);
    QComb.AddHead(temp);
  }
  void PushTail(int dude, BOOL stats)
  {
    QueuedCombatant temp(dude,stats);
    QComb.AddTail(temp);
  }
  void Pop() { QComb.RemoveHead(); }
  
  void Remove(int dud)
  {
    POSITION pos = QComb.GetHeadPosition();
    while (pos != NULL)
    {
      if (QComb.GetAt(pos).Dude == dud)
      {
        QComb.RemoveAt(pos);
        return;
      }
      QComb.GetNext(pos);
    }
  }

  int Top() 
  { 
    if (QComb.GetCount() > 0) 
      return QComb.GetHead().Dude; 
    else 
      return NO_DUDE; 
  }

  BOOL ChangeStats()
  {
    if (QComb.GetCount() > 0) 
      return QComb.GetHead().AffectStats; 
    else 
      return FALSE; 
  }

private:
  CList<QueuedCombatant, QueuedCombatant&> QComb;
};

struct COMBAT_MSGS
{
  enum { MAX_COMBAT_MSGS=1 };

  ~COMBAT_MSGS() { msgs.RemoveAll(); }
  void Add(CString &msg) 
  { 
    if (msgs.GetCount() > MAX_COMBAT_MSGS) msgs.RemoveHead();
    msgs.AddTail(msg);
  }
     
  CList<CString, CString> msgs;
};

} // End of namespace DungeonCraft

#endif
