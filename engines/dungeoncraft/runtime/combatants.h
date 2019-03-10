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

#ifndef DUNGEONCRAFT_SHARED_COMBATANTS
#define DUNGEONCRAFT_SHARED_COMBATANTS

#include "dungeoncraft/shared/GameEvent.h"
#include "dungeoncraft/shared/Combatant.h"

namespace DungeonCraft {

enum combatVictorType { NoWinner, PartyWins, PartyRanAway, MonsterWins };

const int MAX_COMBATANTS = 100;
const int MAX_CHARS_IN_ROW = 3; // how many chars wide the battle line can be
const int MAX_COMBAT_IDLE_ROUNDS = 20;

enum CombatStateType 
{ 
  CST_None,
  CST_Casting,
  CST_Attacking, 
  CST_Guarding, 
  CST_Bandaging, 
  CST_Using,
  CST_Moving,
  CST_Turning,
  CST_Fleeing,
  CST_CombatRoundDelay,
  CST_CombatOver,
  CST_Delaying,
  CST_EndingTurn,
  CST_ForcingWin,
  CST_StartNewRound,
  CST_ActivateSpell
};

const int NUM_COMBATANTS_STATES = 16;
extern char *CombatantsStateText[NUM_COMBATANTS_STATES];

//*****************************************************************************
//*****************************************************************************
//*****************************************************************************

struct COMBATANT_DATA 
{
  COMBATANT_DATA() { Clear(); }
  ~COMBATANT_DATA() {  }
  void Clear();
  void InitCombatData(COMBAT_EVENT_DATA *event);
  void AddCombatants();
  void AddCharsToCombatants();
  void AddMonstersToCombatants();

  void DetermineIfCombatOver();
  void DetermineVictoryExpPoints();
  void getNumCombatants(int &numParty, int &numMons);  
  COMBATANT& getActiveCombatCharacter();  
  COMBATANT& getCombatPartyCharacter(int PartyIdx);
  COMBATANT *getCombatantPtr(int index);

  COMBATANT *getActiveCastingCharacter();
  void SetActiveCastingCharacter(int dude);

  int getNextCombatant();

  void ForcePartyVictory(); // force immediate party victory
  void DetermineCombatInitiative();

  combatantStateType GetCombatantState();
  void determineInitCombatPos();
  int determinePartyOrientation(int x, int y);
  void GetNextDirection(eventDirType &currDir, int &facing);
  int GetMonsterDistance(eventDirType dir);
  void getNextMonsterCombatPos(COMBATANT &monster);
  void GetMonsterCoords(int &x, int &y, int dist, int dir);
  void getNextCharCombatPos(COMBATANT &dude, BOOL &reset);
  BOOL PlaceCharacter(COMBATANT &dude, int &x, int &y);
  BOOL PlaceCharacterLeft(COMBATANT &dude, int &x, int &y);
  BOOL PlaceCharacterRight(COMBATANT &dude, int &x, int &y);
  BOOL PlaceCharacterNorth(COMBATANT &dude, int &x, int &y);
  BOOL PlaceCharacterSouth(COMBATANT &dude, int &x, int &y);
  BOOL PlaceCharacterHorz(COMBATANT &dude, int &x, int &y);
  BOOL PlaceCharacterVert(COMBATANT &dude, int &x, int &y);

  BOOL IsCombatOver() { return CombatOver; }

  BOOL IsNextCombatant();
  BOOL IsFreeAttacker();

  BOOL UpdateCombat();
  void CheckForDeadGuys();
  void DisplayCombatants();
  void displayCursor(BOOL coverFullIcon);
  void HandleCombatRoundMsgExpired();
  void HandleTimeDelayMsgExpired();
  void HandleTimeDelayMsgBegin();
  void HandleCurrState();

  void RestoreToParty();

  void CheckDyingCombatants();
  void CheckMorale();
  BOOL CheckIdleTime();
  void StartNewRound();
  
  void AddTarget(int target) ;
  int GetCurrTarget() ;
  int GetNextTarget() ;
  BOOL CanTarget() ;
  int GetNextAim();
  int GetPrevAim();
  int GetCurrAim() { return aimPos; }
  void ResetAim(); 
  void RemoveCurrTarget();
  
  BOOL CanSeeParty(int x, int y);
  BOOL IsAttacking();
  double AttacksRemaining();
  BOOL CanMove();  
  BOOL CanCast();  
  BOOL CanUse();   
  BOOL CanGuard(); 
  BOOL CanAttack(int target) ;
  BOOL CanTurnUndead();  
  BOOL CanDelay(); 
  BOOL CanBandage();
  void Bandage();
  void EndTurn(combatantStateType newState=CS_None); 
  void StopMoving(); 
  void Quick(BOOL Enable); 
  void Guard(); 
  void TurnUndead();
  void StartTurnUndead();
  void StartInitialSpellCasting(int key);
  void EndInitialSpellCasting();
  void StopSpellCasting();
  void DelayAction(); 
  void AttackOver(); 
  void MakeAttack(); 
  void StartAttack(); 

  int GetDistanceBetween_ByPartyIndex(int PartyMember1Idx, int PartyMember2Idx);

  int InvokePendingSpellOnCombatant(int caster, int targ, int activeSpellKey, int spellDbKey=NO_SPELL);
  int InvokePendingSpellOnMap(int caster, int activeSpellKey);    
  void RemoveSpellEffect(DWORD parent, SPELL_DATA *pSpell);

  BOOL NeedMissileAnimation();
  void InitMissileAnimation();
  BOOL NeedHitAnimation();
  void InitHitAnimation();
  BOOL NeedSpellCastAnimation(int targ);
  void InitSpellCastAnimation(int targ);
  BOOL NeedSpellInRouteAnimation(int x, int y);
  void InitSpellInRouteAnimation(int x, int y);
  BOOL NeedSpellCoverageAnimation(int x, int y);
  void InitSpellCoverageAnimation(int x, int y);
  BOOL NeedSpellHitAnimation(int targ);
  void InitSpellHitAnimation(int targ);
  BOOL NeedSpellLingerAnimation();
  void InitSpellLingerAnimation(int mapx, int mapy);

  BOOL toggleReadyItem(int item);
  BOOL delCharacterItem(int index, int qty);
  void determineAvailAttacks();
  
  bool ProcessTimeSensitiveData(LONGLONG currTime);

  CombatStateType GetCombatState();

  void getCharWeaponText(int index, CString &wpn, CString &dmg);
  BOOL OnAuto(); 
  void SetUpdateFlag() { NeedUpdate = TRUE; }

  CString FormatTimeDelayMsg(CombatStateType state=CST_None);
  CString FormatSpecAbMsg(DWORD sa_state);

  void moveCursorLeft();
  void moveCursorRight();
  void moveCursorForward();
  void moveCursorBackward();
  void PlaceCursorOnCurrentDude(BOOL ForceCenter=FALSE); // on currently active dude
  void PlaceCursorOnDude(int dude,BOOL ForceCenter); // on specified dude

  void moveLeft();
  void moveRight();
  void moveForward();
  void moveBackward();
  void moveNW();
  void moveNE();
  void moveSW();
  void moveSE();
  BOOL CurrMoveWouldFleeMap();

  void PlayCombatDeath();
  void PlayCombatMove();
  void PlayCombatTurnUndead();

  void LoadSounds();
  void ClearSounds();

  int GetCurrCombatant();

  COMBAT_EVENT_DATA *pEvent;

  int hCharDeathSound;
  int hCharMoveSound;
  int hTurnUndeadSound;

  BOOL StartingNewCombatRound;
  BOOL CombatActive;
  BOOL lastRound;
  BOOL CombatOver;
  BOOL NeedUpdate;
 	combatVictorType combatVictor;
  int expPointTotal;
  int currCaster;
  eventTurnUndeadModType undeadTurnModifier;

  int moveToX, moveToY;
	int startTerrainX, startTerrainY;
	int cursorX, cursorY;
  int partyStartX, partyStartY;
  int prevRndCombatant;
	int currInitiative;
	int currRound;
  int currTurn;
  int aimPos;
  eventSurpriseType surprise;

  int ActiveScripter;
  ActorType GetContext(int i);

  // used for morale checks
  int numMonFlee;
  int numMonSlain;
  int numFrndFlee;
  int numFrndSlain;

  // flags that alter normal combat behavior
  BOOL MonsterNoMove;
  BOOL MonsterAlwaysMiss;
  BOOL MonsterAlwaysHit;
  BOOL MonsterNoCast;
  BOOL MonsterPlyrControl;
  BOOL MonsterNoFlee;
  BOOL MonsterNoDeath;
  BOOL PartyAlwaysMiss;
  BOOL PartyAlwaysHit;
  BOOL NoMagic;
  BOOL PartyNeverDies; 
  BOOL PartyNoExperience;
  
  // which combat map format is used
  BOOL IsWilderness;

  QueuedCombatantData QComb;

  // used for clipping
  RECT combatRect;
  RECT fullRect;

	int numCombatants;
	CArray<COMBATANT,  COMBATANT&> combatants;
	CArray<COMBATANT,  COMBATANT&> tempCOMBATANTS;
};

CString FormatCombatMoveText();
CString FormatSpellHitText();

//extern COMBAT_MSGS combatMsgs;
extern COMBATANT_DATA combatData;

} // End of namespace DungeonCraft

#endif
