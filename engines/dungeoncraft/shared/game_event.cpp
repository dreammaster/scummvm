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

//#include "dungeoncraft/shared/externs.h"
#include "dungeoncraft/shared/GameEvent.h"
#include "dungeoncraft/shared/monster.h"
#include "dungeoncraft/shared/Level.h"
#include "dungeoncraft/shared/party.h"
#include "dungeoncraft/shared/GlobalData.h"
#include "dungeoncraft/shared/SoundMgr.h"
#include "dungeoncraft/shared/..\Shared\GPDLopCodes.h"
#include "dungeoncraft/shared/GPDLexec.h"
#include "dungeoncraft/shared/GPDLComp.h"

#ifdef UAFEngine
#include "dungeoncraft/shared/..\UAFWin\Combatants.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



// base event data structures
IMPLEMENT_SERIAL( EVENT_CONTROL, CObject, 1)
IMPLEMENT_SERIAL( GameEvent, CObject, 1)
IMPLEMENT_SERIAL( GameEventList, CObject, 1)

// event support data
IMPLEMENT_SERIAL( TRANSFER_DATA, CObject, 1)
IMPLEMENT_SERIAL( MONSTER_EVENT, CObject, 1)
IMPLEMENT_SERIAL( MONSTER_EVENT_DATA, CObject, 1)
IMPLEMENT_SERIAL( TOUR_STEP, CObject, 1)
IMPLEMENT_SERIAL( ENCOUNTER_BUTTON_OPTION, CObject, 1)
IMPLEMENT_SERIAL( ENCOUNTER_BUTTON_DATA, CObject, 1)
IMPLEMENT_SERIAL( QLIST_OPTION, CObject, 1)
IMPLEMENT_SERIAL( QLIST_DATA, CObject, 1)
IMPLEMENT_SERIAL( QBUTTON_OPTION, CObject, 1)
IMPLEMENT_SERIAL( QBUTTON_DATA, CObject, 1)
IMPLEMENT_SERIAL( TALE, CObject, 1)
IMPLEMENT_SERIAL( DRINK, CObject, 1)
IMPLEMENT_SERIAL( REST_EVENT, CObject, 1)
IMPLEMENT_SERIAL( TIME_EVENT, CObject, 1)
IMPLEMENT_SERIAL( STEP_EVENT, CObject, 1)
IMPLEMENT_SERIAL( SPECIAL_OBJECT_EVENT_DATA, CObject, 1)
IMPLEMENT_SERIAL( SPECIAL_OBJECT_EVENT_LIST, CObject, 1)

// events
IMPLEMENT_SERIAL( ADD_NPC_DATA, GameEvent, 1)
IMPLEMENT_SERIAL( REMOVE_NPC_DATA, GameEvent, 1)
IMPLEMENT_SERIAL( CAMP_EVENT_DATA, GameEvent, 1)
IMPLEMENT_SERIAL( COMBAT_EVENT_DATA, GameEvent, 1)
IMPLEMENT_SERIAL( GUIDED_TOUR, GameEvent, 1)
IMPLEMENT_SERIAL( QUESTION_YES_NO, GameEvent, 1)
IMPLEMENT_SERIAL( ENCOUNTER_DATA, GameEvent, 1)
IMPLEMENT_SERIAL( PASSWORD_DATA, GameEvent, 1)
IMPLEMENT_SERIAL( GAIN_EXP_DATA, GameEvent, 1)
IMPLEMENT_SERIAL( GIVE_TREASURE_DATA, GameEvent, 1)
IMPLEMENT_SERIAL( GIVE_DAMAGE_DATA, GameEvent, 1)
IMPLEMENT_SERIAL( NPC_SAYS_DATA, GameEvent, 1)
IMPLEMENT_SERIAL( QUESTION_LIST_DATA, GameEvent, 1)
IMPLEMENT_SERIAL( QUESTION_BUTTON_DATA, GameEvent, 1)
IMPLEMENT_SERIAL( TEXT_EVENT_DATA, GameEvent, 1)
IMPLEMENT_SERIAL( GPDL_EVENT, GameEvent, 1)
IMPLEMENT_SERIAL( TRANSFER_EVENT_DATA, GameEvent, 1)
IMPLEMENT_SERIAL( WHO_PAYS_EVENT_DATA, GameEvent, 1)
IMPLEMENT_SERIAL( WHO_TRIES_EVENT_DATA, GameEvent, 1)
IMPLEMENT_SERIAL( SPECIAL_ITEM_KEY_EVENT_DATA, GameEvent, 1)
IMPLEMENT_SERIAL( PASS_TIME_EVENT_DATA, GameEvent, 1)
IMPLEMENT_SERIAL( VAULT_EVENT_DATA, GameEvent, 1)
IMPLEMENT_SERIAL( TAVERN, GameEvent, 1)
IMPLEMENT_SERIAL( TEMPLE, GameEvent, 1)
IMPLEMENT_SERIAL( SHOP, GameEvent, 1)
IMPLEMENT_SERIAL( TRAININGHALL, GameEvent, 1)
IMPLEMENT_SERIAL( SMALL_TOWN_DATA, GameEvent, 1)
IMPLEMENT_SERIAL( CHAIN_EVENT, GameEvent, 1)
IMPLEMENT_SERIAL( QUEST_EVENT_DATA, GameEvent, 1)
IMPLEMENT_SERIAL( UTILITIES_EVENT_DATA, GameEvent, 1)
IMPLEMENT_SERIAL( SOUND_EVENT, GameEvent, 1)
IMPLEMENT_SERIAL( TAKE_PARTY_ITEMS_DATA, GameEvent, 1)
IMPLEMENT_SERIAL( HEAL_PARTY_DATA, GameEvent, 1)
IMPLEMENT_SERIAL( LOGIC_BLOCK_DATA, GameEvent, 1)
IMPLEMENT_SERIAL( RANDOM_EVENT_DATA, GameEvent, 1)
IMPLEMENT_SERIAL( PLAY_MOVIE_DATA, GameEvent, 1)
IMPLEMENT_SERIAL( JOURNAL_EVENT, GameEvent, 1)

double LoadingVersion;

#ifdef UAFEDITOR
GameEventList CopyEventData; // temp storage for copy/paste of events
#endif


void WarnOfClearingWithChainsPresent(GameEvent *pEvent)
{
#ifdef UAFEDITOR
  if (pEvent->IsChained())
  {
    CString msg;
    msg.Format("An event is being cleared which contains chained events\n%s\n"
               "Please make a note of your current activity and send\n"
               "this information to CocoaSpud@hotmail.com",
               GetEventIdDescription(pEvent->id, pEvent->GetEventSource()));
    MsgBoxError(msg, "Invalid Editor State");
  }
#endif
}

BOOL CheckValidEvent(GameEvent *pEvent)
{
  if (pEvent==NULL) return FALSE;
  /*
  if (pEvent->IsGlobalEvent())
    return globalData.eventData.IsValidEvent(pEvent->id);
  else
    return levelData.eventData.IsValidEvent(pEvent->id);
  */

  switch (pEvent->GetEventSource())
  {
  case LevelEventSrc:
    return levelData.eventData.IsValidEvent(pEvent->id);
    break;
  case GlobalEventSrc:
    return globalData.eventData.IsValidEvent(pEvent->id);
    break;
#ifdef UAFEDITOR
  case CopyEventSrc:
    return CopyEventData.IsValidEvent(pEvent->id);
    break;
#endif
  default:
    ASSERT(FALSE);
    return FALSE;
  }
}

BOOL CheckValidEvent(GameEvent *pEvent, DWORD id) // id might be a chain id
{
  if (pEvent==NULL) return FALSE;

  /*
  if (pEvent->IsGlobalEvent())
    return globalData.eventData.IsValidEvent(id);
  else
    return levelData.eventData.IsValidEvent(id);
  */

  switch (pEvent->GetEventSource())
  {
  case LevelEventSrc:
    return levelData.eventData.IsValidEvent(id);
    break;
  case GlobalEventSrc:
    return globalData.eventData.IsValidEvent(id);
    break;
#ifdef UAFEDITOR
  case CopyEventSrc:
    return CopyEventData.IsValidEvent(id);
    break;
#endif
  default:
    ASSERT(FALSE);
    return FALSE;
  }
}
/*
  These events can push SAVEGAME_MENU and
  are not allowed as global events.

  QUESTION_BUTTON_DATA (QuestionButton)
  QUESTION_LIST_DATA   (QuestionList)
  SMALL_TOWN_DATA      (SmallTown)
  ENCAMP_MENU_DATA     N/A
  QUESTION_YES_NO      (QuestionYesNo)
  BEGIN_STEP_EVENT     N/A
  BEGIN_TIME_EVENT     N/A
  ADVENTURE_MENU_DATA  N/A
  MAIN_MENU_DATA       N/A
  TRAININGHALL         (TrainingHallEvent)
  CAMP_EVENT_DATA      (Camp)
  START_MENU_DATA      N/A
*/

BOOL AllowedAsGlobalEvent(eventType type)
{
  BOOL result = TRUE;

#ifdef UAFEngine
  if (IsCombatActive())
    return (AllowedAsGlobalCombatEvent(type));
#endif

  return result;
}

BOOL AllowedAsGlobalCombatEvent(eventType type)
{
  if (type >= (eventType)CONTROL_Splash) return FALSE;

  BOOL result = TRUE;
  switch (type)
  {
    // global events are triggered by NPC 'Talk' and Item 'Use'.
    // the 'Use' trigger can happen during combat, and needs further restrictions
   case NoEvent:
   case AddNpc:
   case Camp:
   case Combat:
   case EncounterEvent:
   case ShopEvent:
   case TempleEvent:
   case TavernTales:
   case TavernEvent:
   case Stairs:
   case Teleporter:
   case TransferModule:
   case Vault:
   case TrainingHallEvent:
   case SmallTown:
   case PickOneCombat:
   case InnEvent:
   case Damage:
   case EnterPassword:
   case GainExperience:
   case CombatTreasure:
   case GiveTreasure:
   case GuidedTour:
   case NPCSays:
   case QuestionList:
   case QuestionButton:
   case PassTime:
   case QuestionYesNo:
   case RemoveNPCEvent:
   case TextStatement:
   case WhoPays:
   case WhoTries:
   case SpecialItem:   
   case QuestStage:   
   case TakePartyItems:   
   case GPDLEvent:
   case RandomEvent:
   case PlayMovieEvent:
   case JournalEvent:
     result = FALSE;
     break;

   case Utilities:
   case Sounds:
   case LogicBlock:
   case HealParty:   
     result = TRUE;
     break;

   default:
     WriteDebugString("Unhandled event type %u in AllowedAsGlobalEvent()\n", type);
     result = FALSE;
     break;
  }  
  return result;
}

void SPECIAL_OBJECT_EVENT_DATA::Serialize(CArchive &ar)
{
 CObject::Serialize(ar);
 if (ar.IsStoring())
 {
   ar << ItemType;
   ar << operation;
   ar << index;
   ar << id;
 }
 else
 {
   ar >> ItemType;
   ar >> operation;
   ar >> index;
   ar >> id;
 }
}

void SPECIAL_OBJECT_EVENT_DATA::Serialize(CAR &ar)
{
 //CObject::Serialize(ar);
 if (ar.IsStoring())
 {
   ar << ItemType;
   ar << operation;
   ar << index;
   ar << id;
 }
 else
 {
   ar >> ItemType;
   ar >> operation;
   ar >> index;
   ar >> id;
 }
}

SPECIAL_OBJECT_EVENT_DATA& SPECIAL_OBJECT_EVENT_DATA::operator =(const SPECIAL_OBJECT_EVENT_DATA& src)
{ 
 if (&src == this) 
   return *this;
 ItemType=src.ItemType;
 operation=src.operation;
 index=src.index; 
 id=src.id; 
 return *this; 
}

void SPECIAL_OBJECT_EVENT_LIST::Serialize(CArchive &ar)
{
 int i;
 CObject::Serialize(ar);
 if (ar.IsStoring())
 {
   ar << list.GetSize();
   for (i=0;i<list.GetSize();i++)
     list[i].Serialize(ar);
 }
 else
 {
   int count;
   ar >> count;
   list.SetSize(count);
   for (i=0;i<count;i++)
     list[i].Serialize(ar);
 }      
}

void SPECIAL_OBJECT_EVENT_LIST::Serialize(CAR &ar)
{
 int i;
 //CObject::Serialize(ar);
 if (ar.IsStoring())
 {
   ar << list.GetSize();
   for (i=0;i<list.GetSize();i++)
     list[i].Serialize(ar);
 }
 else
 {
   int count;
   ar >> count;
   list.SetSize(count);
   for (i=0;i<count;i++)
     list[i].Serialize(ar);
 }      
}

SPECIAL_OBJECT_EVENT_LIST& SPECIAL_OBJECT_EVENT_LIST::operator =(const SPECIAL_OBJECT_EVENT_LIST& src)
{
 if (&src == this)
   return *this;
 list.RemoveAll();
 list.SetSize(src.list.GetSize());
 for (int i=0;i<src.list.GetSize(); i++)
   list[i] = src.list[i];
 return *this;
}

BOOL SPECIAL_OBJECT_EVENT_LIST::Get(int id, SPECIAL_OBJECT_EVENT_DATA &data)
{
 if ((id >= 0) && (id < list.GetSize()))
 {
   data = list[id];
   return TRUE;
 }
 return FALSE;
}

int SPECIAL_OBJECT_EVENT_LIST::Add(SPECIAL_OBJECT_EVENT_DATA &data)
{
 int id = list.GetSize();
 data.id = id;
 list.SetAtGrow(id, data);
 return id;
}

BOOL SPECIAL_OBJECT_EVENT_LIST::Delete(int id)
{
 if ((id >= 0) && (id < list.GetSize()))
 {
   list.RemoveAt(id);
   return TRUE;
 }
 return FALSE;
}

#ifdef UAFEDITOR
void WriteValidateLog(FILE *pFile, const char *text, ... )
{
  // 1024 is limit on TRACE()
  static char errorText[512+1];

  if (pFile == NULL)
    return;

  va_list marker;	
  memset(errorText, 0, sizeof(errorText));

  va_start(marker, text);  
  int count = _vsnprintf(errorText, 512, text, marker);
  va_end(marker);

  if (count < 0)
  {
    strcpy(errorText, "WriteValidateLog() msg exceeded buffer capacity\n");
    fprintf(pFile, errorText);
    TRACE(errorText);
  }
  else if (count == 0)
  {
    strcpy(errorText, "WriteValidateLog() msg is empty\n");
    fprintf(pFile, errorText);
    TRACE(errorText);
  }
  else
  {
    errorText[512] = '\0'; // guarantee null terminator
    fprintf(pFile, errorText);
    TRACE(errorText);
  }
}
#endif


void EVENT_CONTROL::Clear()
{
  eventStatusUnused=0;
  eventResultUnused=0;
  onceOnly = FALSE;
  chainTrigger = AlwaysChain;
  eventTrigger = AlwaysTrigger;
  facing = Any;
  race = Human;
  charClass = Cleric;
  item = -1;
  quest = -1;
  chance = 100;
  gender=Male;
  specialItem = -1;
  specialKey = -1;
  npc=-1;
  gpdlData="";
  gpdlIsBinary=false;
  partyX=partyY=-1;
  memorizedSpellDbKey=NO_SPELL;
  memorizedSpellClass = 0;
  memorizedSpellLevel = 0;
  eventcontrol_asl.Clear();
  temp_asl.Clear();
}

EVENT_CONTROL& EVENT_CONTROL::operator =(const EVENT_CONTROL& src)
{
  if (&src == this)
    return *this;
  onceOnly = src.onceOnly;
  chainTrigger = src.chainTrigger;
  eventTrigger = src.eventTrigger;
  facing = src.facing;
  race = src.race;
  charClass = src.charClass;
  item = src.item;
  quest = src.quest;
  chance = src.chance;
  specialItem = src.specialItem;
  specialKey = src.specialKey;
  npc=src.npc;
  gpdlData=src.gpdlData;
  gpdlIsBinary=src.gpdlIsBinary;
  eventcontrol_asl = src.eventcontrol_asl;
  gender=src.gender;
  partyX=src.partyX;
  partyY=src.partyY;
  memorizedSpellDbKey=src.memorizedSpellDbKey;
  memorizedSpellClass=src.memorizedSpellClass;
  memorizedSpellLevel=src.memorizedSpellLevel;
  return *this;
}

EVENT_CONTROL::EVENT_CONTROL(const EVENT_CONTROL& src) :
    eventcontrol_asl("EVENTCONT_ATTR"),
    temp_asl("EVENTCONT_ATTR")
{
  *this = src;
}

BOOL EVENT_CONTROL::EventShouldTrigger(DWORD id)
{
  BOOL shouldTrigger = FALSE;

  switch (eventTrigger) 
  {           
  case AlwaysTrigger:
    if (id != 0)
    {
      WriteDebugString("Event %d triggered: always triggers\n", id);
    };
    shouldTrigger = TRUE;
    break;
      
  case PartyHaveItem: 
    if (item > 0)
      shouldTrigger = party.PartyHasItem(item);
    else if (specialItem >= 0)
      shouldTrigger = party.hasSpecialItem(specialItem);
    else if (specialKey >= 0)
      shouldTrigger = party.hasSpecialKey(specialKey);
    else if (quest >= 0)
      shouldTrigger = globalData.questData.IsPresent(quest);

    if (shouldTrigger)
       WriteDebugString("Event %d triggered: party has item\n", id);
    else
       WriteDebugString("Event %d NOT triggered: party does not have item\n", id);
    break;
      
  case PartyNotHaveItem:      
    if (item > 0)
      shouldTrigger = !party.PartyHasItem(item);
    else if (specialItem >= 0)
      shouldTrigger = !party.hasSpecialItem(specialItem);
    else if (specialKey >= 0)
      shouldTrigger = !party.hasSpecialKey(specialKey);
    else if (quest >= 0)
      shouldTrigger = !globalData.questData.IsPresent(quest);

    if (shouldTrigger)      
      WriteDebugString("Event %d triggered: party does not have item\n", id);
    else
      WriteDebugString("Event %d NOT triggered: party has item\n", id);
    break;
      
  case Daytime:
    shouldTrigger = party.PartyInDaytime();
    if (shouldTrigger)
      WriteDebugString("Event %d triggered: daytime\n", id);
    else
      WriteDebugString("Event %d NOT triggered: not daytime\n", id);
    break;
      
  case Nighttime:
    shouldTrigger = !party.PartyInDaytime();
    if (shouldTrigger)
      WriteDebugString("Event %d triggered: night-time\n", id);
    else
      WriteDebugString("Event %d NOT triggered: not night-time\n", id);
    break;
      
  case RandomChance:
    shouldTrigger = (rollDice(100, 1, 0) <= chance);
    if (shouldTrigger)
      WriteDebugString("Event %d triggered: rnd chance\n", id);
    else 
      WriteDebugString("Event %d NOT triggered: rnd chance\n", id);
    break;

  case PartySearching:
    shouldTrigger = party.PartyIsSearching();
    shouldTrigger |= party.looking;
    if (shouldTrigger)
      WriteDebugString("Event %d triggered: party is searching or looking\n", id);
    else
      WriteDebugString("Event %d NOT triggered: party is not searching or looking\n", id);
    break;

  case PartyNotSearching:
    shouldTrigger = !party.PartyIsSearching();
    if (shouldTrigger)
      WriteDebugString("Event %d NOT triggered: party is searching\n", id);
    else
      WriteDebugString("Event %d triggered: party is not searching\n", id);
    break;

  case FacingDirection:
  case FacingDirectionAnyTime:
    if ((facing == Any) || (facing == InFront)) // InFront shouldn't happen
    {
      shouldTrigger = TRUE;
    }
    else
    {
      switch (party.GetPartyFacing())
      {
      case FACE_NORTH:
        shouldTrigger = (   (facing == North)
                         || (facing == N_S)
                         || (facing == N_E)
                         || (facing == N_W)
                         || (facing == N_S_E)
                         || (facing == N_S_W)
                         || (facing == N_W_E));
        break;
      case FACE_EAST:
        shouldTrigger = (   (facing == East)
                         || (facing == N_E)
                         || (facing == S_E)
                         || (facing == E_W)
                         || (facing == N_S_E)
                         || (facing == N_W_E)
                         || (facing == W_S_E));
        break;
      case FACE_SOUTH:
        shouldTrigger = (   (facing == South)
                         || (facing == N_S)
                         || (facing == S_E)
                         || (facing == S_W)
                         || (facing == N_S_E)
                         || (facing == N_S_W)
                         || (facing == W_S_E));
        break;
      case FACE_WEST:
        shouldTrigger = (   (facing == West)
                         || (facing == N_W)
                         || (facing == S_W)
                         || (facing == E_W)
                         || (facing == N_S_W)
                         || (facing == N_W_E)
                         || (facing == W_S_E));
         break;
      }
    }
    if (shouldTrigger)
      WriteDebugString("Event %d triggered: party facing\n", id);
    else
      WriteDebugString("Event %d NOT triggered: party facing\n", id);
    break;

  case QuestPresent:
    shouldTrigger = globalData.questData.IsPresent(quest);
    if (shouldTrigger)
      WriteDebugString("Event %d triggered: quest present\n", id);
    else
      WriteDebugString("Event %d NOT triggered: quest not present\n", id);
    break;

  case QuestNotPresent:
    shouldTrigger = !globalData.questData.IsPresent(quest);
    if (shouldTrigger)
      WriteDebugString("Event %d triggered: quest not present\n", id);
    else
      WriteDebugString("Event %d NOT triggered: quest present\n", id);
    break;

  case QuestComplete:
    shouldTrigger = globalData.questData.IsComplete(quest);
    if (shouldTrigger)
      WriteDebugString("Event %d triggered: quest complete\n", id);
    else
      WriteDebugString("Event %d NOT triggered: quest not complete\n", id);
    break;

  case QuestFailed: 
    shouldTrigger = globalData.questData.IsFailed(quest);
    if (shouldTrigger)
      WriteDebugString("Event %d triggered: quest failed\n", id);
    else
      WriteDebugString("Event %d NOT triggered: quest not failed\n", id);
    break;

  case QuestInProgress:
    shouldTrigger = globalData.questData.IsInProgress(quest);
    if (shouldTrigger)
      WriteDebugString("Event %d triggered: quest in progress\n", id);
    else
      WriteDebugString("Event %d NOT triggered: quest not in progress\n", id);
    break;

  case PartyDetectingTraps:
    shouldTrigger = party.PartyIsDetectingTraps();
    if (shouldTrigger)
      WriteDebugString("Event %d triggered: party is detecting traps\n", id);
    else
      WriteDebugString("Event %d NOT triggered: party is not detecting traps\n", id);
    break;

  case PartyNotDetectingTraps:
    shouldTrigger = !party.PartyIsDetectingTraps();
    if (shouldTrigger)
      WriteDebugString("Event %d triggered: party is not detecting traps\n", id);
    else
      WriteDebugString("Event %d NOT triggered: party is detecting traps\n", id);
    break;

  case PartySeeInvisible:
    shouldTrigger = party.PartyIsDetectingInvisible();
    if (shouldTrigger)
      WriteDebugString("Event %d triggered: party can see invisible\n", id);
    else
      WriteDebugString("Event %d NOT triggered: party can not see invisible\n", id);
    break;

  case PartyNotSeeInvisible:
    shouldTrigger = !party.PartyIsDetectingInvisible();
    if (shouldTrigger)
      WriteDebugString("Event %d triggered: party can not see invisible\n", id);
    else
      WriteDebugString("Event %d NOT triggered: party can see invisible\n", id);
    break;

  case ClassInParty:
    shouldTrigger = party.PartyHasClass(charClass);
    if (shouldTrigger)
      WriteDebugString("Event %d triggered: class in party\n", id);
    else
      WriteDebugString("Event %d NOT triggered: class not in party\n", id);
    break;         

  case ClassNotInParty:
    shouldTrigger = !party.PartyHasClass(charClass);
    if (shouldTrigger)
      WriteDebugString("Event %d triggered: class not in party\n", id);
    else
      WriteDebugString("Event %d NOT triggered: class in party\n", id);
    break;
      
  case RaceInParty:
    shouldTrigger = party.PartyHasRace(race);
    if (shouldTrigger)
      WriteDebugString("Event %d triggered: race in party\n", id);
    else
      WriteDebugString("Event %d NOT triggered: race not in party\n", id);
    break;
      
  case RaceNotInParty:
    shouldTrigger = !party.PartyHasRace(race);
    if (shouldTrigger)
      WriteDebugString("Event %d triggered: race not in party\n", id);
    else
      WriteDebugString("Event %d NOT triggered: race in party\n", id);
    break;

  case GenderInParty:
    shouldTrigger = party.PartyHasGender(gender);
    if (shouldTrigger)
      WriteDebugString("Event %d triggered: gender in party\n", id);
    else
      WriteDebugString("Event %d NOT triggered: gender not in party\n", id);
    break;

  case GenderNotInParty:
    shouldTrigger = !party.PartyHasGender(gender);
    if (shouldTrigger)
      WriteDebugString("Event %d triggered: gender not in party\n", id);
    else
      WriteDebugString("Event %d NOT triggered: gender in party\n", id);
    break;

  case NPCInParty:
    shouldTrigger = party.PartyHasNPC(npc);
    if (shouldTrigger)
      WriteDebugString("Event %d triggered: npc in party\n", id);
    else
      WriteDebugString("Event %d NOT triggered: npc not in party\n", id);
    break;

  case NPCNotInParty:
    shouldTrigger = !party.PartyHasNPC(npc);
    if (shouldTrigger)
      WriteDebugString("Event %d triggered: npc not in party\n", id);
    else
      WriteDebugString("Event %d NOT triggered: npc in party\n", id);
    break;

  case ExecuteGPDL:
    {
      if (gpdlData.GetLength()==0) return FALSE;
      if (!gpdlIsBinary)
      {       
        GPDLCOMP gpdlComp;
        CString funcname;
        funcname.Format("GameEventTrigger_%u", GetTickCount());
        CString sourcePgm, binaryPgm;
        const char *entries[] = {NULL, NULL};
        entries[0] = (LPCSTR)funcname;
        CString begin;
        begin.Format("$PUBLIC $FUNC %s() {", funcname);
        CString end;
        end.Format("}%s;",funcname);
        sourcePgm =   begin
                    + gpdlData
                    + end;
        binaryPgm = gpdlComp.CompileScript(sourcePgm, entries);
        if (binaryPgm[0] != 0)
        { // error
          WriteDebugString("Event %u NOT triggered: GPDL compile error \'%s\'",id, gpdlData);
          gpdlData="";
          return FALSE;
        }
        gpdlData=binaryPgm;
        gpdlIsBinary=true;
      }      

      if (IsCombatActive())
      {
#ifdef UAFEngine
        SetCharContext( combatData.getActiveCombatCharacter().GetContext() );
        SetTargetContext( combatData.getActiveCombatCharacter().GetContext() );
#else
        ASSERT(FALSE);
#endif
      }
      else
      {
        SetCharContext( party.getActiveChar().GetContext() );
        SetTargetContext( party.getActiveChar().GetContext() );
      }

      shouldTrigger = (gpdl.ExecuteScript(gpdlData,1)=="1");

      ClearTargetContext();
      ClearCharContext();      

      if (shouldTrigger)
        WriteDebugString("Event %d triggered: GPDL script returns true\n", id);
      else
        WriteDebugString("Event %d NOT triggered: GPDL script returns false\n", id);
    }  
    break;

  case SpellMemorized:    
    shouldTrigger = party.PartyHasSpellMemorized(memorizedSpellDbKey);
    if (shouldTrigger)
      WriteDebugString("Event %d triggered: party has spell memorized\n", id);
    else
      WriteDebugString("Event %d NOT triggered: party does not have spell memorized\n", id);
    break;

  case PartyAtXY:
    shouldTrigger = ((party.Posx == partyX) && (party.Posy == partyY));
    if (shouldTrigger)
      WriteDebugString("Event %d triggered: party at x,y\n", id);
    else
      WriteDebugString("Event %d NOT triggered: party not at x,y\n", id);
    break;

  default:
    WriteDebugString("Event %d NOT triggered: unhandled eventTrigger type\n", id);
    return FALSE;
  }
      
  return shouldTrigger;
}

void EVENT_CONTROL::PreSerialize(BOOL IsStoring)
{
  if (IsStoring)
  {
    // move fields that need to be serialized
    // as attributes into the attribute map  
    StoreIntAsASL(eventcontrol_asl, (int)gender, "Gen");
    StoreIntAsASL(eventcontrol_asl, specialItem, "SpIt");
    StoreIntAsASL(eventcontrol_asl, specialKey, "SpKy");
  }
}

void EVENT_CONTROL::PostSerialize(BOOL IsStoring)
{
  if (!IsStoring)
  {
    // retrieve fields that are serialized as
    // attributes from the attribute map
    int temp;
    RetrieveIntFromASL(eventcontrol_asl, temp, "Gen");
    gender = (genderType)temp;
    RetrieveIntFromASL(eventcontrol_asl, specialItem, "SpIt");
    RetrieveIntFromASL(eventcontrol_asl, specialKey, "SpKy");
  }
}

void EVENT_CONTROL::Serialize( CArchive &ar )
{
  PreSerialize(ar.IsStoring());

  CObject::Serialize(ar);

  if (ar.IsStoring())
  {
     ar << (int)eventStatusUnused;
     ar << (int)eventResultUnused;
     ar << (int)onceOnly;
     ar << (int)chainTrigger;
     ar << (int)eventTrigger;
     ar << item;
     ar << quest;
     ar << chance;
     ar << (int)facing;
     ar << (int)race;
     ar << (int)charClass;
     ar << npc;
     eventcontrol_asl.Serialize(ar);
     AS(ar, gpdlData);
     ar << gpdlIsBinary;
     ar << partyX;
     ar << partyY;
     ar << memorizedSpellDbKey;
     ar << memorizedSpellClass;
     ar << memorizedSpellLevel;
  }
  else
  {
     int temp;
   
     ar >> temp; 
     eventStatusUnused = temp;  
     ar >> temp;
     eventResultUnused = temp; 
     ar >> temp;
     onceOnly = temp;   
     ar >> temp;
     chainTrigger = (chainTriggerType)temp;   
     ar >> temp;
     eventTrigger = (eventTriggerType)temp;
     ar >> item;
     ar >> quest;
     ar >> chance;   
     ar >> temp;
     facing = (eventDirType)temp;
     ar >> temp;
     race = (raceType)temp;
     ar >> temp;
     charClass = (classType)temp;

     if (LoadingVersion >= _VERSION_0820_)
       ar >> npc;

     if (LoadingVersion >= _VERSION_0566_)
      eventcontrol_asl.Serialize(ar);

     if (LoadingVersion >= _VERSION_0880_)
     {
       DAS(ar, gpdlData);
       ar >> gpdlIsBinary;
     }

     if (LoadingVersion >= _VERSION_0911_)
     {
       ar >> partyX;
       ar >> partyY;
       ar >> memorizedSpellDbKey;
       ar >> memorizedSpellClass;
       ar >> memorizedSpellLevel;
     }
  }   

  PostSerialize(ar.IsStoring());
}

void EVENT_CONTROL::Serialize( CAR &ar )
{
  PreSerialize(ar.IsStoring());

  //CObject::Serialize(ar);

  if (ar.IsStoring())
  {
     ar << (int)eventStatusUnused;
     ar << (int)eventResultUnused;
     ar << (int)onceOnly;
     ar << (int)chainTrigger;
     ar << (int)eventTrigger;
     ar << item;
     ar << quest;
     ar << chance;
     ar << (int)facing;
     ar << (int)race;
     ar << (int)charClass;
     ar << npc;
     ar << eventcontrol_asl;
     AS(ar, gpdlData);
     ar << gpdlIsBinary;
     ar << partyX;
     ar << partyY;
     ar << memorizedSpellDbKey;
     ar << memorizedSpellClass;
     ar << memorizedSpellLevel;
  }
  else
  {
     int temp;   
     ar >> temp; 
     eventStatusUnused = temp;  
     ar >> temp;
     eventResultUnused = temp; 
     ar >> temp;
     onceOnly = temp;   
     ar >> temp;
     chainTrigger = (chainTriggerType)temp;   
     ar >> temp;
     eventTrigger = (eventTriggerType)temp;
     ar >> item;
     ar >> quest;
     ar >> chance;   
     ar >> temp;
     facing = (eventDirType)temp;
     ar >> temp;
     race = (raceType)temp;
     ar >> temp;
     charClass = (classType)temp;

     if (LoadingVersion >= _VERSION_0820_)
       ar >> npc;
     if (LoadingVersion >= _VERSION_0566_)
      ar >> eventcontrol_asl;

     if (LoadingVersion >= _VERSION_0880_)
     {
       DAS(ar, gpdlData);
       ar >> gpdlIsBinary;
     }

     if (LoadingVersion >= _VERSION_0911_)
     {
       ar >> partyX;
       ar >> partyY;
       ar >> memorizedSpellDbKey;
       ar >> memorizedSpellClass;
       ar >> memorizedSpellLevel;
     }
  }   

  PostSerialize(ar.IsStoring());
}


//*****************************************************************************
//*****************************************************************************
//*****************************************************************************
//*****************************************************************************
//*****************************************************************************


GameEvent::GameEvent() : 
  chainEventHappen(0),
  chainEventNotHappen(0),
  event_asl("EVENT_DATA_ATTR"),
  temp_asl("EVENT_DATA_ATTR")
{
  GameEvent::Clear();
}

GameEvent& GameEvent::operator =(const GameEvent& src)
{
  if (this == &src)
    return *this;

   id = src.id;
   //IsGlobal=src.IsGlobal;
   m_source=src.m_source;
   event = src.event;
   x = src.x;
   y = src.y;
   control = src.control;
   pic = src.pic; 
   pic2 = src.pic2; 
   if (!src.text.IsEmpty())
      text = src.text;
   else
      text="";
   if (!src.text2.IsEmpty())
      text2 = src.text2;
   else
      text2="";
   if (!src.text3.IsEmpty())
      text3 = src.text3;
   else
      text3="";

   chainEventHappen = src.chainEventHappen;
   chainEventNotHappen = src.chainEventNotHappen;
   event_asl = src.event_asl;
   temp_asl = src.temp_asl;

   return *this;
}


GameEvent::GameEvent(const GameEvent& src) :
  chainEventHappen(0),
  chainEventNotHappen(0),
  event_asl("EVENT_DATA_ATTR"),
  temp_asl("EVENT_DATA_ATTR")
{
  *this = src;
}

GameEvent::GameEvent(const GameEvent *src) :
  chainEventHappen(0),
  chainEventNotHappen(0),
  event_asl("EVENT_DATA_ATTR"),
  temp_asl("EVENT_DATA_ATTR")
{
  *this = *src;
}

void GameEvent::Clear()
{   
  WarnOfClearingWithChainsPresent(this);

  event = NoEvent;
  id = 0;
  x = -1;
  y = -1;
  text="";
  text2="";
  text3="";
  pic.Clear();
  pic2.Clear();
  control.Clear();  
  chainEventHappen = 0;
  chainEventNotHappen = 0;
  //IsGlobal=FALSE;
  m_source=LevelEventSrc;
  event_asl.Clear();
  temp_asl.Clear();
}

BOOL GameEvent::EventShouldTrigger()
{
   BOOL shouldTrigger=FALSE;
   //eventLocationRestriction location;
   
   if (event == NoEvent)
   {
      WriteDebugString("Event NOT triggered: bogus event type\n");
      return FALSE;
   }
   
   //
   // location = GetEventLocationRestriction(event);
   //
   // if (location != AllowAnywhere)
   // {
   //    if ((InWilderness) && (location == AllowInCities))
   //      shouldTrigger = FALSE;
   //    if ((!InWilderness) && (location == AllowInWilderness))
   //      shouldTrigger = FALSE;
   // }
   //
   
   //if (!shouldTrigger)
   {
     // onceOnly should never be set in evets of type CONTROL_xxxxxx
     // Otherwise, we could check for event >= CONTROL_Splash
     BOOL alreadyHappened;

     /*
     if (IsGlobalEvent())
       alreadyHappened = globalData.eventData.HasEventHappened(id);
     else
       alreadyHappened = levelData.eventData.HasEventHappened(id);
      */

    switch (GetEventSource())
    {
    case LevelEventSrc:
      alreadyHappened = levelData.eventData.HasEventHappened(id);
      break;
    case GlobalEventSrc:
      alreadyHappened = globalData.eventData.HasEventHappened(id);
      break;
  #ifdef UAFEDITOR
    case CopyEventSrc:
      alreadyHappened = CopyEventData.HasEventHappened(id);
      break;
  #endif
    default:
      ASSERT(FALSE);
      return FALSE;
    }

     if (control.onceOnly && alreadyHappened)
       WriteDebugString("Event %d NOT triggered: Once Only\n", id);
     else
       shouldTrigger = control.EventShouldTrigger(id);
   }
   return shouldTrigger;
}

BOOL GameEvent::IsChained()
{
  if ((control.chainTrigger == AlwaysChain) && (chainEventHappen > 0))
    return TRUE;

  if ((control.chainTrigger == IfEventHappen) && (chainEventHappen > 0))
    return TRUE;

  if ((control.chainTrigger == IfEventNotHappen) && (chainEventNotHappen > 0))
    return TRUE;

  return FALSE;
}

#if defined UAFEDITOR
BOOL GameEvent::GetChainedEvent(DWORD &cid)
{
  BOOL found = FALSE;

  if ((control.chainTrigger == AlwaysChain) && (chainEventHappen > 0))
  {
    if (chainEventNotHappen > 0)
    {
      ASSERT(FALSE);
      if (chainEventHappen!=chainEventNotHappen) ASSERT(FALSE); // shouldn't happen
      chainEventNotHappen=0;
    }
    cid = chainEventHappen;
    found = TRUE;
  }
  else if ((control.chainTrigger == IfEventHappen) && (chainEventHappen > 0))
  {
    if (chainEventNotHappen > 0)
    {
      ASSERT(FALSE);
      if (chainEventHappen!=chainEventNotHappen) ASSERT(FALSE); // shouldn't happen
      chainEventNotHappen=0;        
    }
    cid = chainEventHappen;
    found = TRUE;
  }
  else if ((control.chainTrigger == IfEventNotHappen) && (chainEventNotHappen > 0))
  {
    if (chainEventHappen > 0)
    {
      ASSERT(FALSE);
      if (chainEventHappen!=chainEventNotHappen) ASSERT(FALSE); // shouldn't happen
      chainEventHappen=0;        
    }
    cid = chainEventNotHappen;
    found = TRUE;
  }

  return found;
}
#endif // UAFEDITOR


void GameEvent::GetChainCount(int &curr, int &total)
{
  curr=0;
  total=1;
  if (chainEventHappen > 0)
    curr++;
  else if (chainEventNotHappen > 0)
    curr++;
}

void GameEvent::FreeEventResources()
{
  pic.FreePic();
  pic2.FreePic();
}

void GameEvent::SaveEventResources(int level)
{
  pic.SavePicData(level);
  pic2.SavePicData(level);  
}

void GameEvent::PostSerialize(BOOL IsStoring)
{
  if (!IsStoring)
  {
    switch (GetEventType())
    {
    case CombatTreasure: event=GiveTreasure; break;    

    case Stairs:
    case Teleporter: event=TransferModule; break;

    case TavernTales: 
    case InnEvent: ASSERT(FALSE); break;

    default:
      // do nothing
      break;
    }
  }
}

void GameEvent::Serialize( CArchive &ar )
{
  PreSerialize(ar.IsStoring());

  CObject::Serialize(ar);
  control.Serialize(ar);
  pic.Serialize(ar, LoadingVersion);
  pic2.Serialize(ar, LoadingVersion);

  if (ar.IsStoring())
  {
     ar << (int)event;
     ar << id;
     ar << x;
     ar << y;
     ar << chainEventHappen;
     ar << chainEventNotHappen;
  
     AS(ar,text)
//     if (text.IsEmpty())
//     {
//        text = "*";
//        ar << text;
//        text="";
//     }
//     else
//        ar << text;

     AS(ar,text2)
//     if (text2.IsEmpty())
//     {
//        text2 = "*";
//        ar << text2;
//        text2="";
//     }
//     else
//        ar << text2;

     AS(ar,text3)
//     if (text3.IsEmpty())
//     {
//        text3 = "*";
//        ar << text3;
//        text3="";
//     }
//     else
//        ar << text3;

     //ar << IsGlobal;
     event_asl.Serialize(ar);
  }
  else
  {
     int temp;
     DWORD tid;
     ar >> temp;
     event = (eventType)temp;
     ar >> tid;
     id = tid;
     ar >> x;
     ar >> y;
     ar >> chainEventHappen;
     ar >> chainEventNotHappen;

     DAS(ar,text)
     DAS(ar,text2)
     DAS(ar,text3)
//     ar >> text;
//     ar >> text2;
//     ar >> text3;
//
//     if (text == "*")
//       text = "";
//     if (text2 == "*")
//       text2 = "";
//     if (text3 == "*")
//       text3 = "";

     // load event attributes if version indicates
     // their presence
     if (LoadingVersion >= _VERSION_0564_)
       event_asl.Serialize(ar);
  }

  PostSerialize(ar.IsStoring());
}

void GameEvent::Serialize( CAR &ar )
{
  PreSerialize(ar.IsStoring());

  //CObject::Serialize(ar);
  control.Serialize(ar);
  pic.Serialize(ar, LoadingVersion);
  pic2.Serialize(ar, LoadingVersion);

  if (ar.IsStoring())
  {
     ar << (int)event;
     ar << id;
     ar << x;
     ar << y;
     ar << chainEventHappen;
     ar << chainEventNotHappen;
  
     AS(ar,text);
//     if (text.IsEmpty())
//     {
//        text = "*";
//        ar << text;
//        text="";
//     }
//     else
//        ar << text;

     AS(ar,text2);
//     if (text2.IsEmpty())
//     {
//        text2 = "*";
//        ar << text2;
//        text2="";
//     }
//     else
//        ar << text2;

     AS(ar,text3);
//     if (text3.IsEmpty())
//     {
//        text3 = "*";
//        ar << text3;
//        text3="";
//     }
//     else
//        ar << text3;

     //ar << IsGlobal;
     ar << event_asl;
  }
  else
  {
     int temp;
     DWORD tid;
     ar >> temp;
     event = (eventType)temp;
     ar >> tid;
     id = tid;
     ar >> x;
     ar >> y;
     ar >> chainEventHappen;
     ar >> chainEventNotHappen;

     DAS(ar,text);
     DAS(ar,text2);
     DAS(ar,text3);
//     ar >> text;
//     ar >> text2;
//     ar >> text3;
//
//     if (text == "*")
//       text = "";
//     if (text2 == "*")
//       text2 = "";
//     if (text3 == "*")
//       text3 = "";

     // load event attributes if version indicates
     // their presence
     if (LoadingVersion >= _VERSION_0564_)
       ar >> event_asl;
  }

  PostSerialize(ar.IsStoring());
}

void GameEventList::PreSerialize(BOOL IsStoring)
{
  POSITION p = GetHeadPosition();
  while (p!=NULL) 
    GetNext(p)->PreSerialize(IsStoring);
}

void GameEventList::PostSerialize(BOOL IsStoring)
{
  POSITION p = GetHeadPosition();
  while (p!=NULL) 
    GetNext(p)->PostSerialize(IsStoring);
}

void GameEventList::Save(CArchive& ar) 
{
	int count;
	POSITION p;

	count=GetCount();
	ar << count;
	p=GetHeadPosition();
	while (p!=NULL) 
	{
		ar << GetAt(p)->id;
		GetAt(p)->Save(ar);
		GetNext(p);
	}
}

void GameEventList::Restore(CArchive& ar) 
{
	int count;
	DWORD id;
	POSITION p;
	GameEvent junk;
	// Let events know they will be asked to CommitRestore.  They
	// can make a copy of their data or otherwise note that 
	// if no Restore comes along they do something special.
	// Probably will want to clear their temporary copy of
	// ATTRIBUTES in case no restore operation clears it.
	p=GetHeadPosition();
  while (p!=NULL) 
    GetNext(p)->PrepareForRestore();

	ar >> count;

	for (int i=0; i<count; i++) 
  {
		ar >> id;

		p=GetHeadPosition();
		while (p!=NULL) 
    {
			if (id == GetAt(p)->id)
			{
				GetAt(p)->Restore(ar);
				break;
			}
			GetNext(p);
		}
		if (p==NULL) 
		{	
			CString msg;
			msg.Format("SAVE-FILE GameEvent \"%u\" does not exist in design\n",
				         id);
			WriteDebugString(msg);
			junk.Restore(ar); // Discard saved data.  Any better ideas?
		}
	}
}

void GameEventList::CommitRestore() 
{
	POSITION p;
	p=GetHeadPosition();
	while (p!=NULL) 
    GetNext(p)->CommitRestore();
}

#ifdef UAFEDITOR
char *EventSourceText[] = 
{
  "Level",
  "Global",
  "Copy"
};
#endif

void GameEventList::Clear() 
{ 
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos = GetHeadPosition();
  if (pos!=NULL)
    WriteDebugString("GameEventList::Clear() deleting all events\n");

  while (pos != NULL)
  {
#ifdef UAFEDITOR
    WriteDebugString("Deleting '%s' %s event\n",
                     GetEventIdDescription(m_events.GetAt(pos)->id,GetEventSource()),
                     EventSourceText[(int)GetEventSource()]);
#endif
    delete m_events.GetAt(pos);
    m_events.GetNext(pos);
  }
  m_events.RemoveAll(); 
  m_nextKey=1;
  m_ListType=LevelEventSrc;
}

int GameEventList::AddEvent(GameEvent *data)
{
  CSingleLock sLock(&m_CS, TRUE);
  if (GetCount() < MAX_EVENTS)
  {
    data->id = GetNextKey();
    //data->SetGlobalEvent( IsGlobalList() );
    data->SetEventSource( GetEventSource() );
    m_events.Insert(data, data->id);
#if ((defined UAFEDITOR) && (defined _DEBUG))
    DWORD count = GetCount();
    count = max(0,count);
    WriteDebugString("Added '%s' %s event: x %i, y %i - (%u total)\n",
                    GetEventIdDescription(data->id,GetEventSource()), 
                    //(IsGlobalList()?"global":"level"),
                    EventSourceText[(int)data->GetEventSource()],
                    data->x, data->y,
                    count);
#endif
    return data->id;
  }
  else
    return 0;
}

// called when loading binary monster data that
// was saved with key info
int GameEventList::AddEventWithCurrKey(GameEvent *data)
{
  CSingleLock sLock(&m_CS, TRUE);
  if (GetCount() < MAX_EVENTS)
  {
    //data->SetGlobalEvent(IsGlobalList());
    data->SetEventSource( GetEventSource() );
    m_events.Insert(data, data->id);
    if (m_nextKey<=data->id) m_nextKey=data->id+1;
#if ((defined UAFEDITOR) && (defined _DEBUG))
    DWORD count = GetCount();
    count = max(0,count);

    WriteDebugString("Added '%s' %s event: x %i, y %i - (%u total)\n",
                    GetEventIdDescription(data->id,GetEventSource()), 
                    //(IsGlobalList()?"global":"level"),
                    EventSourceText[(int)data->GetEventSource()],
                    data->x, data->y,
                    count);
#endif
    return data->id;
  }
  else
    return 0;
}

GameEvent *GameEventList::GetEvent(DWORD id) 
{ 
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = m_events.FindKeyPos(id)) != NULL)
    return m_events.GetAt(pos);
  else
    return NULL;
}

BOOL GameEventList::IsChained(GameEvent *event)
{ 
  CSingleLock sLock(&m_CS, TRUE);
  if (IsValidEvent(event->id))
    return (event->IsChained()); 
  else
    return FALSE;
}

BOOL GameEventList::IsValidEvent(DWORD num) 
{ 
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = m_events.FindKeyPos(num)) != NULL)
    return (IsValidEvent(m_events.GetAt(pos))); 
  else
    return FALSE;
}

BOOL GameEventList::IsValidEventChainWord(DWORD num) 
{ // Zero or a valid event
  // Many times the chain words of an event did not get cleared
  // when the chained event was deleted.  So I added ASSERTs
  // in EventViewer's PopulateTree to help discover those cases.  PRS
  // I also added code to use event IDs only once.  We used to reuse
  // an event ID if the last event was deleted and then a new event added.
  if (num==0) return TRUE;
  CSingleLock sLock(&m_CS, TRUE);
  if (m_events.FindKeyPos(num) != NULL) return TRUE;
  return FALSE;
}


BOOL GameEventList::IsValidEvent(GameEvent *event) const 
{ 
  if (event != NULL) 
    return (event->IsValidEvent());
  else
    return FALSE;
}

void GameEventList::DeleteEvents(DWORD id)
{
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = m_events.FindKeyPos(id)) != NULL)
    DeleteEvents(m_events.GetAt(pos));
}

void GameEventList::DeleteEvents(GameEvent *event)
{
  CSingleLock sLock(&m_CS, TRUE);
  DeleteEventAndAllChains(event);
}

// recursive, so make sure event chains don't loop back to
// themselves
void GameEventList::DeleteEventAndAllChains(GameEvent *event)
{
   if (!IsValidEvent(event))
      return;

   DeleteEvents(event->chainEventHappen);
   event->chainEventHappen = 0;

   DeleteEvents(event->chainEventNotHappen);
   event->chainEventNotHappen = 0;

   DeleteEvent(event);
}

void GameEventList::DeleteEvent(GameEvent *event)
{
   if (!IsValidEvent(event))
      return;

   if (event->IsChained())
   {
      int i;
      switch (event->GetEventType())
      {
      case TavernEvent:
        {
          //ASSERT(event->IsKindOf(RUNTIME_CLASS(TAVERN)));
          TAVERN *data = (TAVERN*)(event);        

          DeleteEvents(data->drinkChain);
          data->drinkChain = 0;
          DeleteEvents(data->fightChain);
          data->fightChain = 0;          
        }
        break;

      case TempleEvent:
        {
          //ASSERT(event->IsKindOf(RUNTIME_CLASS(TEMPLE)));
          TEMPLE *data = (TEMPLE*)(event);        

          DeleteEvents(data->donationChain);
          data->donationChain = 0;
        }
        break;

      case QuestStage:
        {
          //ASSERT(event->IsKindOf(RUNTIME_CLASS(QUEST_EVENT_DATA)));
          QUEST_EVENT_DATA *data = (QUEST_EVENT_DATA*)(event);        

          DeleteEvents(data->acceptChain);
          data->acceptChain = 0;
          DeleteEvents(data->rejectChain);
          data->rejectChain = 0;
        }
        break;

      case ChainEventType:
        {
          //ASSERT(event->IsKindOf(RUNTIME_CLASS(CHAIN_EVENT)));
          CHAIN_EVENT *data = (CHAIN_EVENT*)(event);        

          DeleteEvents(data->Chain);
          data->Chain = 0;
        }
        break;

      case RandomEvent:
        {
          RANDOM_EVENT_DATA *data = (RANDOM_EVENT_DATA*)(event);
          for (i=1;i<RANDOM_EVENT_DATA::MAX_RANDOM_EVENTS;i++)
          {
            DeleteEvents(data->EventChains[i-1]);
            data->EventChains[i-1]=0;
          }
        }
        break;

      case SmallTown:
        {
          //ASSERT(event->IsKindOf(RUNTIME_CLASS(SMALL_TOWN_DATA)));
          SMALL_TOWN_DATA *data = (SMALL_TOWN_DATA*)(event);        

          DeleteEvents(data->TempleChain);
          data->TempleChain = 0;
          DeleteEvents(data->TrainingHallChain);
          data->TrainingHallChain = 0;
          DeleteEvents(data->ShopChain);
          data->ShopChain = 0;
          DeleteEvents(data->InnChain);
          data->InnChain = 0;
          DeleteEvents(data->TavernChain);
          data->TavernChain = 0;
          DeleteEvents(data->VaultChain);
          data->VaultChain = 0;
        }
        break;

	   case EncounterEvent:
       {
         //ASSERT(event->IsKindOf(RUNTIME_CLASS(ENCOUNTER_DATA)));
         ENCOUNTER_DATA *data = (ENCOUNTER_DATA*)(event);        

		     for (i=0;i<MAX_BUTTONS;i++)
		     {
           DeleteEvents(data->buttons.buttons[i].chain);
           data->buttons.buttons[i].chain = 0;
		     }
         DeleteEvents(data->combatChain);
         data->combatChain=0;
         DeleteEvents(data->talkChain);
         data->talkChain=0;
         DeleteEvents(data->escapeChain);
         data->escapeChain=0;
       }
       break;

      case QuestionList:
        {
           //ASSERT(event->IsKindOf(RUNTIME_CLASS(QUESTION_LIST_DATA)));
           QUESTION_LIST_DATA *data = (QUESTION_LIST_DATA*)(event);        

           for (i=0;i<MAX_BUTTONS;i++)
           {
             DeleteEvents(data->buttons.buttons[i].chain);
             data->buttons.buttons[i].chain = 0;
           }
        }
        break;
   
      case QuestionButton:
        {
           //ASSERT(event->IsKindOf(RUNTIME_CLASS(QUESTION_BUTTON_DATA)));
           QUESTION_BUTTON_DATA *data = (QUESTION_BUTTON_DATA*)(event);        

           for (i=0;i<MAX_BUTTONS;i++)
           {
             DeleteEvents(data->buttons.buttons[i].chain);
             data->buttons.buttons[i].chain = 0;
           }
        }
        break;

      case QuestionYesNo:
        {
          //ASSERT(event->IsKindOf(RUNTIME_CLASS(QUESTION_YES_NO)));
          QUESTION_YES_NO *data = (QUESTION_YES_NO*)(event);        

          DeleteEvents(data->YesChain);
          data->YesChain = 0;
          DeleteEvents(data->NoChain);
          data->NoChain = 0;
        }
        break;

      case EnterPassword:
        {
           //ASSERT(event->IsKindOf(RUNTIME_CLASS(PASSWORD_DATA)));
           PASSWORD_DATA *data = (PASSWORD_DATA*)(event);        

           DeleteEvents(data->successChain);
           data->successChain = 0;
           DeleteEvents(data->failChain);
           data->failChain = 0;
        }
        break;

      case WhoPays:
        {
           //ASSERT(event->IsKindOf(RUNTIME_CLASS(WHO_PAYS_EVENT_DATA)));
           WHO_PAYS_EVENT_DATA *data = (WHO_PAYS_EVENT_DATA*)(event);        

           DeleteEvents(data->successChain);
           data->successChain = 0;
           DeleteEvents(data->failChain);
           data->failChain = 0;
        }
        break;

      case WhoTries:
        {
           //ASSERT(event->IsKindOf(RUNTIME_CLASS(WHO_TRIES_EVENT_DATA)));
           WHO_TRIES_EVENT_DATA *data = (WHO_TRIES_EVENT_DATA*)(event);        
          
           DeleteEvents(data->successChain);
           data->successChain = 0;
           DeleteEvents(data->failChain);
           data->failChain = 0;
        }
        break;

      case LogicBlock:
        {
           LOGIC_BLOCK_DATA *data = (LOGIC_BLOCK_DATA*)(event);        
          
           DeleteEvents(data->falseChain);
           data->falseChain = 0;
           DeleteEvents(data->trueChain);
           data->trueChain = 0;
        }
        break;

      default:
        // do nothing
        break;
      }
   }

   DWORD OldId = event->id;
   POSITION pos = m_events.FindKeyPos(OldId);
   if (pos != NULL)
   {
#if ((defined UAFEDITOR) && (defined _DEBUG))
     DWORD count = GetCount()-1;
     count = max(0,count);
     WriteDebugString("Deleted '%s' %s event - %u remaining\n",
                      GetEventIdDescription(OldId,GetEventSource()),
                      //(IsGlobalList()?"global":"level"),
                      EventSourceText[(int)GetEventSource()],
                      count);
#endif
    //event->Clear();
    delete event;
    m_events.RemoveAt(pos);
   }
}

EVENT_CONTROL &GameEventList::GetEventControl(DWORD id)  
{ 
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = m_events.FindKeyPos(id)) != NULL)
    return m_events.GetAt(pos)->GetEventControl();
  else
    return NullEvent.GetEventControl();
}

eventType &GameEventList::GetEventType(DWORD id)  
{ 
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = m_events.FindKeyPos(id)) != NULL)
    return m_events.GetAt(pos)->GetEventType();
  else
    return NullEvent.GetEventType();
}  

PIC_DATA &GameEventList::GetEventPic(DWORD id)  
{ 
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = m_events.FindKeyPos(id)) != NULL)
    return m_events.GetAt(pos)->GetEventPic();
  else
    return NullEvent.GetEventPic();
}

PIC_DATA &GameEventList::GetEventPic2(DWORD id)  
{ 
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = m_events.FindKeyPos(id)) != NULL)
    return m_events.GetAt(pos)->GetEventPic2();
  else
    return NullEvent.GetEventPic2();
}

CString &GameEventList::GetEventText(DWORD id)  
{ 
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = m_events.FindKeyPos(id)) != NULL)
    return m_events.GetAt(pos)->GetEventText();
  else
    return NullEvent.GetEventText();
}

CString &GameEventList::GetEventText2(DWORD id)  
{ 
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = m_events.FindKeyPos(id)) != NULL)
    return m_events.GetAt(pos)->GetEventText2();
  else
    return NullEvent.GetEventText2();
}

CString &GameEventList::GetEventText3(DWORD id)  
{ 
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = m_events.FindKeyPos(id)) != NULL)
    return m_events.GetAt(pos)->GetEventText3();
  else
    return NullEvent.GetEventText3();
}

int &GameEventList::GetEventX(DWORD id)  
{ 
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = m_events.FindKeyPos(id)) != NULL)
    return m_events.GetAt(pos)->GetEventX();
  else
    return NullEvent.GetEventX();
}

int &GameEventList::GetEventY(DWORD id)  
{ 
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = m_events.FindKeyPos(id)) != NULL)
    return m_events.GetAt(pos)->GetEventY();
  else
    return NullEvent.GetEventY();
}

DWORD GameEventList::GetNextKey()
{
  DWORD key=0;
  if (GetCount() > 0)
  {
    key = m_events.GetTail()->id;
    if (key < m_nextKey) key=m_nextKey;
    if (key >= MAX_EVENTS)
    {
      // Need to wrap key.
      // Search for non-sequential gap between current keys.
      POSITION pos = m_events.GetHeadPosition();
      BOOL found = FALSE;        
      DWORD prevKey = 0;
      DWORD currKey;
      while ((pos != NULL) && (!found))
      {
        currKey = m_events.GetNext(pos)->id;
        if (currKey != prevKey+1)
        {
          key = prevKey+1;
          found = TRUE;
        }
        else
          prevKey = currKey; 
      }
      VERIFY(found); // 16 million events is too many events
    }
    else
    {
      if (key < MAX_EVENTS) m_nextKey=key+1;
    };
  }
  else
  {
    if (m_nextKey==0) key=1;
    else key=m_nextKey;
    m_nextKey=key+1;
  };
  return key;
}

BOOL GameEventList::ValidEventNum(DWORD num) 
{ 
  POSITION pos = m_events.FindKeyPos(num);
  if (pos == NULL) return FALSE;
  if (m_events.GetAt(pos) == NULL) return FALSE;
  return TRUE;
}

void GameEvent::Save(CArchive& ar) 
{
	event_asl.Save(ar);
}

void GameEvent::PrepareForRestore() 
{
	// In case no Restore comes along then we will simply use what we have.
	temp_asl.Clear();
}


void GameEvent::Restore(CArchive& ar) 
{
	temp_asl.Serialize(ar);
}

void GameEvent::CommitRestore(void) 
{
	event_asl.CommitRestore(temp_asl);
	temp_asl.Clear();
}


#if (defined UAFEDITOR)

void GameEvent::ChainToEventId(int num, DWORD cid)
{
  if (cid != NO_EVENT)
  {
    ASSERT( CheckValidEvent(this, cid) );
  }

  if (num == 0)
  {
    switch (control.chainTrigger)
    {
    case AlwaysChain:
    case IfEventHappen:
      if (chainEventNotHappen > 0)
      {
        // these two are mutually exclusive
        ASSERT(FALSE);
        if (chainEventHappen!=chainEventNotHappen) ASSERT(FALSE); // shouldn't happen
        chainEventNotHappen=0;
      }
      chainEventHappen = cid;
      break;

    case IfEventNotHappen:
      if (chainEventHappen > 0)
      {
        // these two are mutually exclusive
        ASSERT(FALSE);
        if (chainEventHappen!=chainEventNotHappen) ASSERT(FALSE); // shouldn't happen
        chainEventHappen=0;
      }
      chainEventNotHappen = cid;      
      break;
    }
  }
}

int GameEvent::UnchainToEventId(int num, DWORD eid)
{ // Called when an event is deleted.  We should not point to it anymore.
  // Returns number of chain words cleared.
  ASSERT(num<1); // This ASSERTion fails if you forget to override
                 // this function in a derived class with additional
                 // chain words.  See ENCOUNTER_DATA::UnchainToEvent()
                 // for an example.
  // We ignore num.  We get called when the chained event is deleted.
  // Nothing should point to it.  So it is quicker, easier,
  // and safer (IMHO) to do it this way. PRS
  int result=0;
  if (chainEventNotHappen==eid) {result++;chainEventNotHappen=0;};
  if (chainEventHappen==eid) {result++;chainEventHappen=0;};
  return result;
}


BOOL GameEvent::GetEVChainText(int num, char *ctext)
{
  if (num == 0)
  {
    switch (control.chainTrigger)
    {
    case AlwaysChain:
      sprintf(ctext, 
              "%s: %s", 
              "Normal Chain",
              GetEventIdDescription(chainEventHappen,GetEventSource()));
      break;
    case IfEventHappen:
      sprintf(ctext, 
              "%s: %s", 
              "Normal Chain",
              GetEventIdDescription(chainEventHappen,GetEventSource()));
      break;
    case IfEventNotHappen:
      sprintf(ctext, 
              "%s: %s", 
              "Normal Chain",
              GetEventIdDescription(chainEventNotHappen,GetEventSource()));
      break;
    }
    return TRUE;
  }
  return FALSE;
}

#endif

void GameEventList::markEventHappened(GameEvent *event)
{
  CSingleLock sLock(&m_CS, TRUE);
  if (!IsValidEvent(event))
    return;
  party.MarkEventHappened(m_level, event->id);
  WriteDebugString("Marking event %u happened\n", event->id);
}

BOOL GameEventList::HasEventHappened(DWORD eventID)
{
  return party.HasEventHappened(m_level, eventID);
}

GameEvent *GameEventList::GetFirstEvent(int x, int y)
{ 
  CSingleLock sLock(&m_CS, TRUE);
  // find first event in chain at x,y
  POSITION pos = GetHeadPosition();

  while (pos != NULL)
  {
    if (   (GetAt(pos)->x == x)
        && (GetAt(pos)->y == y))
    {      
       return (GetAt(pos));
    }
    GetNext(pos);
  }

  return NULL;
}
void GameEventList::Serialize( CArchive &ar, double version )
{
  CSingleLock sLock(&m_CS, TRUE);

  LoadingVersion = version;

  CObject::Serialize(ar);
  
  if (ar.IsStoring())
  {
    ar << m_level;
    ar << GetCount();
    POSITION pos = GetHeadPosition();

    while (pos != NULL)
    {
      ar << (int)(GetAt(pos)->GetEventType());
      GetNext(pos)->Serialize(ar);
    }
  }
  else
  {
    GameEvent *data;
    int temp;
    int count;

    ar >> m_level;
    ar >> count;

    for (int i=0; i<count; i++)
    {
      ar >> temp;
      data = CreateNewEvent(temp);

      if (data != NULL)
      {
        data->Serialize(ar);
        AddEventWithCurrKey(data);
      }
    }
  }
}

void GameEventList::Serialize( CAR &ar, double version )
{
  CSingleLock sLock(&m_CS, TRUE);
  //CObject::Serialize(ar);

  LoadingVersion = version;

  
  if (ar.IsStoring())
  {
    ar << m_level;
    ar << GetCount();
    POSITION pos = GetHeadPosition();

    while (pos != NULL)
    {
      ar << (int)(GetAt(pos)->GetEventType());
      GetNext(pos)->Serialize(ar);
    }
  }
  else
  {
    GameEvent *data;
    int temp;
    int count;

    ar >> m_level;
    ar >> count;

    for (int i=0; i<count; i++)
    {
      ar >> temp;
      data = CreateNewEvent(temp);

      if (data != NULL)
      {
        data->Serialize(ar);
        AddEventWithCurrKey(data);
      }
    }
  }
}

GameEventList& GameEventList::operator =(GameEventList& src)
{
  if (this == &src) return *this;
  WriteDebugString("GameEventList::operator= calling Clear()\n");
  Clear();

  m_level = src.m_level;
  m_ListType = src.m_ListType;

  POSITION pos = src.GetHeadPosition();
  while (pos != NULL)
  {
    ASSERT(src.GetAt(pos) != NULL);
    GameEvent *pNewEvent = CreateNewEvent(src.GetAt(pos)->GetEventType());
    if (pNewEvent != NULL)
    {
      GameEvent *pSrcEvent = src.GetAt(pos);
      //*pDst = *pSrc;

      switch (pSrcEvent->GetEventType())
      {
       case AddNpc:            *((ADD_NPC_DATA*)pNewEvent)=*((ADD_NPC_DATA*)pSrcEvent);  break;    
       case Camp:              *((CAMP_EVENT_DATA*)pNewEvent)=*((CAMP_EVENT_DATA*)pSrcEvent);  break;
       case Combat:
       case PickOneCombat:     *((COMBAT_EVENT_DATA*)pNewEvent)=*((COMBAT_EVENT_DATA*)pSrcEvent);  break;
       case Damage:            *((GIVE_DAMAGE_DATA*)pNewEvent)=*((GIVE_DAMAGE_DATA*)pSrcEvent);  break;
       case EncounterEvent:    *((ENCOUNTER_DATA*)pNewEvent)=*((ENCOUNTER_DATA*)pSrcEvent);  break;
       case EnterPassword:     *((PASSWORD_DATA*)pNewEvent)=*((PASSWORD_DATA*)pSrcEvent);  break;
       case GainExperience:    *((GAIN_EXP_DATA*)pNewEvent)=*((GAIN_EXP_DATA*)pSrcEvent);  break;
       case CombatTreasure:
       case GiveTreasure:      *((GIVE_TREASURE_DATA*)pNewEvent)=*((GIVE_TREASURE_DATA*)pSrcEvent);  break;
       case GuidedTour:        *((GUIDED_TOUR*)pNewEvent)=*((GUIDED_TOUR*)pSrcEvent);  break;
       case NPCSays:           *((NPC_SAYS_DATA*)pNewEvent)=*((NPC_SAYS_DATA*)pSrcEvent);  break;
       case QuestionList:      *((QUESTION_LIST_DATA*)pNewEvent)=*((QUESTION_LIST_DATA*)pSrcEvent);  break;
       case QuestionButton:    *((QUESTION_BUTTON_DATA*)pNewEvent)=*((QUESTION_BUTTON_DATA*)pSrcEvent);  break;
       case PassTime:          *((PASS_TIME_EVENT_DATA*)pNewEvent)=*((PASS_TIME_EVENT_DATA*)pSrcEvent);  break;
       case QuestionYesNo:     *((QUESTION_YES_NO*)pNewEvent)=*((QUESTION_YES_NO*)pSrcEvent);  break;
       case RemoveNPCEvent:    *((REMOVE_NPC_DATA*)pNewEvent)=*((REMOVE_NPC_DATA*)pSrcEvent);  break;
       case ShopEvent:         *((SHOP*)pNewEvent)=*((SHOP*)pSrcEvent);  break;
       case TempleEvent:       *((TEMPLE*)pNewEvent)=*((TEMPLE*)pSrcEvent);  break;
       case TavernEvent:       *((TAVERN*)pNewEvent)=*((TAVERN*)pSrcEvent);  break;
       case TextStatement:     *((TEXT_EVENT_DATA*)pNewEvent)=*((TEXT_EVENT_DATA*)pSrcEvent);  break;
       case Stairs: 
       case Teleporter: 
       case TransferModule:    *((TRANSFER_EVENT_DATA*)pNewEvent)=*((TRANSFER_EVENT_DATA*)pSrcEvent);  break;
       case WhoPays:           *((WHO_PAYS_EVENT_DATA*)pNewEvent)=*((WHO_PAYS_EVENT_DATA*)pSrcEvent);  break;
       case WhoTries:          *((WHO_TRIES_EVENT_DATA*)pNewEvent)=*((WHO_TRIES_EVENT_DATA*)pSrcEvent);  break;
       case SpecialItem:       *((SPECIAL_ITEM_KEY_EVENT_DATA*)pNewEvent)=*((SPECIAL_ITEM_KEY_EVENT_DATA*)pSrcEvent);  break;
       case Vault:             *((VAULT_EVENT_DATA*)pNewEvent)=*((VAULT_EVENT_DATA*)pSrcEvent);  break;
       case TrainingHallEvent: *((TRAININGHALL*)pNewEvent)=*((TRAININGHALL*)pSrcEvent);  break;
       case SmallTown:         *((SMALL_TOWN_DATA*)pNewEvent)=*((SMALL_TOWN_DATA*)pSrcEvent);  break;
       case RandomEvent:       *((RANDOM_EVENT_DATA*)pNewEvent)=*((RANDOM_EVENT_DATA*)pSrcEvent);  break;
       case ChainEventType:    *((CHAIN_EVENT*)pNewEvent)=*((CHAIN_EVENT*)pSrcEvent);  break;
       case QuestStage:        *((QUEST_EVENT_DATA*)pNewEvent)=*((QUEST_EVENT_DATA*)pSrcEvent);  break;
       case Utilities:         *((UTILITIES_EVENT_DATA*)pNewEvent)=*((UTILITIES_EVENT_DATA*)pSrcEvent);  break;
       case Sounds:            *((SOUND_EVENT*)pNewEvent)=*((SOUND_EVENT*)pSrcEvent);  break;
       case TakePartyItems:    *((TAKE_PARTY_ITEMS_DATA*)pNewEvent)=*((TAKE_PARTY_ITEMS_DATA*)pSrcEvent);  break;
       case HealParty:         *((HEAL_PARTY_DATA*)pNewEvent)=*((HEAL_PARTY_DATA*)pSrcEvent);  break;
       case LogicBlock:        *((LOGIC_BLOCK_DATA*)pNewEvent)=*((LOGIC_BLOCK_DATA*)pSrcEvent);  break;
       case PlayMovieEvent:    *((PLAY_MOVIE_DATA*)pNewEvent)=*((PLAY_MOVIE_DATA*)pSrcEvent);  break;    
       case JournalEvent:      *((JOURNAL_EVENT*)pNewEvent)=*((JOURNAL_EVENT*)pSrcEvent);  break;
       default:
          ASSERT(FALSE);
          WriteDebugString("Unhandled event type %i in GameEventList::operator=()\n", pSrcEvent->GetEventType());
          break;
      }  

      AddEventWithCurrKey(pNewEvent);
    }
    src.GetNext(pos);
  }
  return *this;
}
void GameEventList::saveUsedEventArt(int level)
{
  if (GetCount() <= 0)
    return;

  POSITION pos = GetHeadPosition();
  while (pos != NULL)
    GetNext(pos)->SaveEventResources(level);
}

void GameEventList::saveUsedEventSounds(int level)
{
  if (GetCount() <= 0)
    return;

  POSITION pos = GetHeadPosition();
  while (pos != NULL)
    GetNext(pos)->SaveEventSounds(level);
}

GameEvent *GameEventList::CreateNewEvent(int type)
{
  if (type == NoEvent) return NULL;
  GameEvent *event = NULL;

  switch (type)
  {
   case AddNpc:            event = new ADD_NPC_DATA;                break;    
   case Camp:              event = new CAMP_EVENT_DATA;             break;  
   case Combat:            event = new COMBAT_EVENT_DATA;           break;    
   case PickOneCombat:     event = new COMBAT_EVENT_DATA; event->event=PickOneCombat; break;   /* what a hack! */
   case Damage:            event = new GIVE_DAMAGE_DATA;            break;    
   case EncounterEvent:    event = new ENCOUNTER_DATA;              break;    
   case EnterPassword:     event = new PASSWORD_DATA;               break;    
   case GainExperience:    event = new GAIN_EXP_DATA;               break;    
   case CombatTreasure:
   case GiveTreasure:      event = new GIVE_TREASURE_DATA;          break;    
   case GuidedTour:        event = new GUIDED_TOUR;                 break;    
   case NPCSays:           event = new NPC_SAYS_DATA;               break;   
   case QuestionList:      event = new QUESTION_LIST_DATA;          break;
   case QuestionButton:    event = new QUESTION_BUTTON_DATA;        break;
   case PassTime:          event = new PASS_TIME_EVENT_DATA;        break;
   case QuestionYesNo:     event = new QUESTION_YES_NO;             break;    
   case RemoveNPCEvent:    event = new REMOVE_NPC_DATA;             break;   
   case ShopEvent:         event = new SHOP;                        break;
   case TempleEvent:       event = new TEMPLE;                      break;
 
#ifdef UAFEDITOR
   case TavernTales:       event = new TAVERN;event->event=TavernTales;  break;
#else
   case TavernTales: // only used for UA importing, tales are part of TAVERN event
#endif
   case TavernEvent:       event = new TAVERN;                      break;

   case TextStatement:     event = new TEXT_EVENT_DATA;             break;
   case Stairs: 
   case Teleporter: 
   case TransferModule:    event = new TRANSFER_EVENT_DATA;         break;
   case WhoPays:           event = new WHO_PAYS_EVENT_DATA;         break;
   case WhoTries:          event = new WHO_TRIES_EVENT_DATA;        break;          
   case SpecialItem:       event = new SPECIAL_ITEM_KEY_EVENT_DATA; break;
   case Vault:             event = new VAULT_EVENT_DATA;            break;  
   case TrainingHallEvent: event = new TRAININGHALL;                break;
   case SmallTown:         event = new SMALL_TOWN_DATA;             break;
   case RandomEvent:       event = new RANDOM_EVENT_DATA;           break;
   case ChainEventType:    event = new CHAIN_EVENT;                 break;
   case QuestStage:        event = new QUEST_EVENT_DATA;            break;    
   case Utilities:         event = new UTILITIES_EVENT_DATA;        break;
   case Sounds:            event = new SOUND_EVENT;                 break;
   case TakePartyItems:    event = new TAKE_PARTY_ITEMS_DATA;       break;
   case HealParty:         event = new HEAL_PARTY_DATA;             break;
   case LogicBlock:        event = new LOGIC_BLOCK_DATA;            break;
   case PlayMovieEvent:    event = new PLAY_MOVIE_DATA;             break;
   case JournalEvent:      event = new JOURNAL_EVENT;               break;
   
   case InnEvent: // never
   default:
      ASSERT(FALSE);
      WriteDebugString("Unhandled event type %i in CreateNewEvent()\n", type);
      break;       
  }
  
  ASSERT( event != NULL );
  if (event != NULL)
    ASSERT( event->GetEventType() != NoEvent );
  return event;
}

#ifdef UAFEDITOR

DWORD GameEventList::GetLastEventInNormalChain(DWORD SrcId)
{  
  BOOL search = TRUE;
  DWORD lastId = SrcId;
  DWORD currId = SrcId;

  while (search)
  {
    GameEvent *pEvent = GetEvent(currId);
    if (pEvent!=NULL)
    {
      if (pEvent->GetChainedEvent(currId))
        lastId = currId;
      else
        search=FALSE;
    }
    else
      search=FALSE;
  }
  return lastId;
}

GameEvent* GameEventList::CreateEventCopy(GameEvent *pSrcEvent)
{
  if (pSrcEvent==NULL) return NULL;
  ASSERT (IsValidEvent(pSrcEvent));
  GameEvent *pNewEvent = CreateNewEvent(pSrcEvent->GetEventType());
  if (pNewEvent == NULL) return NULL;

  switch (pSrcEvent->GetEventType())
  {
   case AddNpc:            *((ADD_NPC_DATA*)pNewEvent)=*((ADD_NPC_DATA*)pSrcEvent);  break;    
   case Camp:              *((CAMP_EVENT_DATA*)pNewEvent)=*((CAMP_EVENT_DATA*)pSrcEvent);  break;
   case Combat:
   case PickOneCombat:     *((COMBAT_EVENT_DATA*)pNewEvent)=*((COMBAT_EVENT_DATA*)pSrcEvent);  break;
   case Damage:            *((GIVE_DAMAGE_DATA*)pNewEvent)=*((GIVE_DAMAGE_DATA*)pSrcEvent);  break;
   case EncounterEvent:    *((ENCOUNTER_DATA*)pNewEvent)=*((ENCOUNTER_DATA*)pSrcEvent);  break;
   case EnterPassword:     *((PASSWORD_DATA*)pNewEvent)=*((PASSWORD_DATA*)pSrcEvent);  break;
   case GainExperience:    *((GAIN_EXP_DATA*)pNewEvent)=*((GAIN_EXP_DATA*)pSrcEvent);  break;
   case CombatTreasure:
   case GiveTreasure:      *((GIVE_TREASURE_DATA*)pNewEvent)=*((GIVE_TREASURE_DATA*)pSrcEvent);  break;
   case GuidedTour:        *((GUIDED_TOUR*)pNewEvent)=*((GUIDED_TOUR*)pSrcEvent);  break;
   case NPCSays:           *((NPC_SAYS_DATA*)pNewEvent)=*((NPC_SAYS_DATA*)pSrcEvent);  break;
   case QuestionList:      *((QUESTION_LIST_DATA*)pNewEvent)=*((QUESTION_LIST_DATA*)pSrcEvent);  break;
   case QuestionButton:    *((QUESTION_BUTTON_DATA*)pNewEvent)=*((QUESTION_BUTTON_DATA*)pSrcEvent);  break;
   case PassTime:          *((PASS_TIME_EVENT_DATA*)pNewEvent)=*((PASS_TIME_EVENT_DATA*)pSrcEvent);  break;
   case QuestionYesNo:     *((QUESTION_YES_NO*)pNewEvent)=*((QUESTION_YES_NO*)pSrcEvent);  break;
   case RemoveNPCEvent:    *((REMOVE_NPC_DATA*)pNewEvent)=*((REMOVE_NPC_DATA*)pSrcEvent);  break;
   case ShopEvent:         *((SHOP*)pNewEvent)=*((SHOP*)pSrcEvent);  break;
   case TempleEvent:       *((TEMPLE*)pNewEvent)=*((TEMPLE*)pSrcEvent);  break;
   case TavernEvent:       *((TAVERN*)pNewEvent)=*((TAVERN*)pSrcEvent);  break;
   case TextStatement:     *((TEXT_EVENT_DATA*)pNewEvent)=*((TEXT_EVENT_DATA*)pSrcEvent);  break;
   case Stairs: 
   case Teleporter: 
   case TransferModule:    *((TRANSFER_EVENT_DATA*)pNewEvent)=*((TRANSFER_EVENT_DATA*)pSrcEvent);  break;
   case WhoPays:           *((WHO_PAYS_EVENT_DATA*)pNewEvent)=*((WHO_PAYS_EVENT_DATA*)pSrcEvent);  break;
   case WhoTries:          *((WHO_TRIES_EVENT_DATA*)pNewEvent)=*((WHO_TRIES_EVENT_DATA*)pSrcEvent);  break;
   case SpecialItem:       *((SPECIAL_ITEM_KEY_EVENT_DATA*)pNewEvent)=*((SPECIAL_ITEM_KEY_EVENT_DATA*)pSrcEvent);  break;
   case Vault:             *((VAULT_EVENT_DATA*)pNewEvent)=*((VAULT_EVENT_DATA*)pSrcEvent);  break;
   case TrainingHallEvent: *((TRAININGHALL*)pNewEvent)=*((TRAININGHALL*)pSrcEvent);  break;
   case SmallTown:         *((SMALL_TOWN_DATA*)pNewEvent)=*((SMALL_TOWN_DATA*)pSrcEvent);  break;
   case RandomEvent:       *((RANDOM_EVENT_DATA*)pNewEvent)=*((RANDOM_EVENT_DATA*)pSrcEvent);  break;
   case ChainEventType:    *((CHAIN_EVENT*)pNewEvent)=*((CHAIN_EVENT*)pSrcEvent);  break;
   case QuestStage:        *((QUEST_EVENT_DATA*)pNewEvent)=*((QUEST_EVENT_DATA*)pSrcEvent);  break;
   case Utilities:         *((UTILITIES_EVENT_DATA*)pNewEvent)=*((UTILITIES_EVENT_DATA*)pSrcEvent);  break;
   case Sounds:            *((SOUND_EVENT*)pNewEvent)=*((SOUND_EVENT*)pSrcEvent);  break;
   case TakePartyItems:    *((TAKE_PARTY_ITEMS_DATA*)pNewEvent)=*((TAKE_PARTY_ITEMS_DATA*)pSrcEvent);  break;
   case HealParty:         *((HEAL_PARTY_DATA*)pNewEvent)=*((HEAL_PARTY_DATA*)pSrcEvent);  break;
   case LogicBlock:        *((LOGIC_BLOCK_DATA*)pNewEvent)=*((LOGIC_BLOCK_DATA*)pSrcEvent);  break;
   case PlayMovieEvent:    *((PLAY_MOVIE_DATA*)pNewEvent)=*((PLAY_MOVIE_DATA*)pSrcEvent);  break;    
   case JournalEvent:      *((JOURNAL_EVENT*)pNewEvent)=*((JOURNAL_EVENT*)pSrcEvent);  break;
   default:
      ASSERT(FALSE);
      WriteDebugString("Unhandled event type %i in CreateNewEvent()\n", pSrcEvent->GetEventType());
      break;
  }  
  return pNewEvent;
}

void GameEventList::CopyEvent(DWORD &RootId, DWORD SrcId, GameEventList *pSrcList)
{
  if (pSrcList==NULL) return;
  if (!pSrcList->IsValidEvent(SrcId)) return;
  //if (!IsValidEvent(RootId)) return;

  GameEvent *pSrcEvent = pSrcList->GetEvent(SrcId);
  if (pSrcEvent==NULL) return;
  GameEvent *pNewEvent = pSrcList->CreateEventCopy(pSrcEvent);
  if (pNewEvent==NULL) return;

  AddEvent(pNewEvent);
  RootId = pNewEvent->GetEventId();
#if ((defined UAFEDITOR) && (defined _DEBUG))
  WriteDebugString("Copied '%s' %s event to %s list - %u events total\n",
                   GetEventIdDescription(SrcId,pSrcList->GetEventSource()),
                   EventSourceText[(int)pSrcList->GetEventSource()],
                   EventSourceText[(int)GetEventSource()],
                   GetCount());
#endif
  if (pSrcEvent->IsChained())
  {
    switch (pSrcEvent->control.chainTrigger)
    {
    case AlwaysChain:
    case IfEventHappen:
      CopyEvent(pNewEvent->chainEventHappen, pSrcEvent->chainEventHappen, pSrcList);
      break;
    case IfEventNotHappen:
      CopyEvent(pNewEvent->chainEventNotHappen, pSrcEvent->chainEventNotHappen, pSrcList);
      break;
    }

    int i;
    switch (pSrcEvent->GetEventType())
    {
    case TavernEvent:
      {
        TAVERN *sdata = (TAVERN*)(pSrcEvent);     
        TAVERN *ndata = (TAVERN*)(pNewEvent);     
        CopyEvent(ndata->drinkChain, sdata->drinkChain, pSrcList);
        CopyEvent(ndata->fightChain, sdata->fightChain, pSrcList);
      }
      break;

    case TempleEvent:
      {
        TEMPLE *sdata = (TEMPLE*)(pSrcEvent);
        TEMPLE *ndata = (TEMPLE*)(pNewEvent);
        CopyEvent(ndata->donationChain, sdata->donationChain, pSrcList);
      }
      break;

    case QuestStage:
      {
        QUEST_EVENT_DATA *sdata = (QUEST_EVENT_DATA*)(pSrcEvent);        
        QUEST_EVENT_DATA *ndata = (QUEST_EVENT_DATA*)(pNewEvent);
        CopyEvent(ndata->acceptChain, sdata->acceptChain, pSrcList);
        CopyEvent(ndata->rejectChain, sdata->rejectChain, pSrcList);
      }
      break;

    case ChainEventType:
      {
        CHAIN_EVENT *sdata = (CHAIN_EVENT*)(pSrcEvent);        
        CHAIN_EVENT *ndata = (CHAIN_EVENT*)(pNewEvent);
        CopyEvent(ndata->Chain, sdata->Chain, pSrcList);
      }
      break;

    case RandomEvent:
      {
        RANDOM_EVENT_DATA *sdata = (RANDOM_EVENT_DATA*)(pSrcEvent);
        RANDOM_EVENT_DATA *ndata = (RANDOM_EVENT_DATA*)(pNewEvent);
        for (i=1;i<RANDOM_EVENT_DATA::MAX_RANDOM_EVENTS;i++)
          CopyEvent(ndata->EventChains[i-1], sdata->EventChains[i-1], pSrcList);
      }
      break;

    case SmallTown:
      {
        SMALL_TOWN_DATA *sdata = (SMALL_TOWN_DATA*)(pSrcEvent);        
        SMALL_TOWN_DATA *ndata = (SMALL_TOWN_DATA*)(pNewEvent);

        CopyEvent(ndata->TempleChain, sdata->TempleChain, pSrcList);
        CopyEvent(ndata->TrainingHallChain, sdata->TrainingHallChain, pSrcList);
        CopyEvent(ndata->ShopChain, sdata->ShopChain, pSrcList);
        CopyEvent(ndata->InnChain, sdata->InnChain, pSrcList);
        CopyEvent(ndata->TavernChain, sdata->TavernChain, pSrcList);
        CopyEvent(ndata->VaultChain, sdata->VaultChain, pSrcList);
      }
      break;

   case EncounterEvent:
     {
       ENCOUNTER_DATA *sdata = (ENCOUNTER_DATA*)(pSrcEvent);    
       ENCOUNTER_DATA *ndata = (ENCOUNTER_DATA*)(pNewEvent);

		   for (i=0;i<MAX_BUTTONS;i++)
         CopyEvent(ndata->buttons.buttons[i].chain, sdata->buttons.buttons[i].chain, pSrcList);
       CopyEvent(ndata->combatChain, sdata->combatChain, pSrcList);
       CopyEvent(ndata->talkChain, sdata->talkChain, pSrcList);
       CopyEvent(ndata->escapeChain, sdata->escapeChain, pSrcList);
     }
     break;

    case QuestionList:
      {
         QUESTION_LIST_DATA *sdata = (QUESTION_LIST_DATA*)(pSrcEvent);        
         QUESTION_LIST_DATA *ndata = (QUESTION_LIST_DATA*)(pNewEvent);

         for (i=0;i<MAX_BUTTONS;i++)
           CopyEvent(ndata->buttons.buttons[i].chain, sdata->buttons.buttons[i].chain, pSrcList);
      }
      break;

    case QuestionButton:
      {
         QUESTION_BUTTON_DATA *sdata = (QUESTION_BUTTON_DATA*)(pSrcEvent);        
         QUESTION_BUTTON_DATA *ndata = (QUESTION_BUTTON_DATA*)(pNewEvent);

         for (i=0;i<MAX_BUTTONS;i++)
           CopyEvent(ndata->buttons.buttons[i].chain, sdata->buttons.buttons[i].chain, pSrcList);
      }
      break;

    case QuestionYesNo:
      {
        QUESTION_YES_NO *sdata = (QUESTION_YES_NO*)(pSrcEvent);        
        QUESTION_YES_NO *ndata = (QUESTION_YES_NO*)(pNewEvent);

        CopyEvent(ndata->YesChain, sdata->YesChain, pSrcList);
        CopyEvent(ndata->NoChain, sdata->NoChain, pSrcList);
      }
      break;

    case EnterPassword:
      {
         PASSWORD_DATA *sdata = (PASSWORD_DATA*)(pSrcEvent);    
         PASSWORD_DATA *ndata = (PASSWORD_DATA*)(pNewEvent);

         CopyEvent(ndata->successChain, sdata->successChain, pSrcList);
         CopyEvent(ndata->failChain, sdata->failChain, pSrcList);
      }
      break;

    case WhoPays:
      {
         WHO_PAYS_EVENT_DATA *sdata = (WHO_PAYS_EVENT_DATA*)(pSrcEvent);        
         WHO_PAYS_EVENT_DATA *ndata = (WHO_PAYS_EVENT_DATA*)(pNewEvent);

         CopyEvent(ndata->successChain, sdata->successChain, pSrcList);
         CopyEvent(ndata->failChain, sdata->failChain, pSrcList);
      }
      break;

    case WhoTries:
      {
         WHO_TRIES_EVENT_DATA *sdata = (WHO_TRIES_EVENT_DATA*)(pSrcEvent);        
         WHO_TRIES_EVENT_DATA *ndata = (WHO_TRIES_EVENT_DATA*)(pNewEvent);

         CopyEvent(ndata->successChain, sdata->successChain, pSrcList);
         CopyEvent(ndata->failChain, sdata->failChain, pSrcList);
      }
      break;

    case LogicBlock:
      {
         LOGIC_BLOCK_DATA *sdata = (LOGIC_BLOCK_DATA*)(pSrcEvent);        
         LOGIC_BLOCK_DATA *ndata = (LOGIC_BLOCK_DATA*)(pNewEvent);

         CopyEvent(ndata->falseChain, sdata->falseChain, pSrcList);
         CopyEvent(ndata->trueChain, sdata->trueChain, pSrcList);
      }
      break;

    default:
      // do nothing
      break;
    }
  } // if has chained events      
}

BOOL GameEventList::IsEventTypeUsed(eventType type)
{
  if (type >= CONTROL_Splash) return FALSE;
  BOOL used = TRUE;
  switch (type)
  {
    // these events cannot be created or used
  case TavernTales:
  case InnEvent:
  case PickOneCombat:
  case GPDLEvent:
  case NoEvent:
    used = FALSE;
    break;

  default:
    used = TRUE;
    break;
  }
  return used;
}

BOOL GameEventList::Validate()
{
  BOOL success = TRUE;
  FILE *pFile = NULL;  
  char filename[_MAX_PATH+1];
  
  GetDesignPath(filename);
  strcat(filename, "Verify.txt");

  SetFileAttributes(filename, FILE_ATTRIBUTE_NORMAL);
  pFile = fopen(filename, "wt");
  if (pFile == NULL)
  {
    WriteDebugString("Unable to open validation logfile %s\n", filename);
    return FALSE;
  }

  if (!monsterData.Validate(pFile)) success = FALSE;
  if (!spellData.Validate(pFile)) success = FALSE;
  if (!itemData.Validate(pFile)) success = FALSE;
  if (!globalData.charData.Validate(pFile)) success = FALSE;

  POSITION gpos = m_events.GetHeadPosition();
  
  while (gpos != NULL)
  {
    if (!m_events.GetAt(gpos)->Validate(pFile))
      success = FALSE;
    m_events.GetNext(gpos);
  }

  if (pFile != NULL)
    fclose(pFile);
#ifndef _DEBUG
  if (success) 
    DeleteFile(filename);
#endif
  return success;
}

void GameEventList::LogEventText()
{
  FILE *pFile = NULL;  
  char filename[_MAX_PATH+1];  
  GetDesignPath(filename);
  CString file;

  bool IsGlobalSrc=false;
  if (GetEventSource()==LevelEventSrc)
  {
    file.Format("Level%03i_TextLog.txt", globalData.currLevel);
    strcat(filename, file);
  }
  else if (GetEventSource()==GlobalEventSrc)
  {
    file = "Global_TextLog.txt";
    strcat(filename, file);
    IsGlobalSrc=true;
  }
  else
    return;

  SetFileAttributes(filename, FILE_ATTRIBUTE_NORMAL);
  pFile = fopen(filename, "wt");
  if (pFile == NULL)
  {    
    CString temp;
    temp.Format("Unable to open event text log: %s\n", filename);
    MsgBoxError(temp);
    return;
  }

  if (GetEventSource()==LevelEventSrc)
  {
    fprintf(pFile, "Event text for level %i\n\n", globalData.currLevel);
    fprintf(pFile, "The 'at x,y' indicates event location on map\n");
    fprintf(pFile, "and 'at -1,-1' indicates a chained event\n\n");
  }
  else
  {
    fprintf(pFile, "Global event text (items, NPCs)\n\n");
    fprintf(pFile, "The 'at -1,-1' can be ignored for global events\n\n");
  }
  fprintf(pFile, "The number in parens is unique for each event\n");
  fprintf(pFile, "and matches the same number shown in the event dialog.\n\n");
  fprintf(pFile, "Text 1 is the main text that all events can display.\n");
  fprintf(pFile, "Text 2 and 3 are optional for events that have\n");
  fprintf(pFile, "multiple text boxes.\n");
  fprintf(pFile, " Ex: Password Event has: Text2=Success Text, Text3=Fail Text\n\n");

  POSITION gpos = m_events.GetHeadPosition();  
  bool hasText;
  while (gpos != NULL)
  {
    hasText=false;
    GameEvent *pEvent = m_events.GetAt(gpos);
    if (pEvent==NULL) continue;    
    fprintf(pFile, "\nEvent '%s' at %i,%i:\n",
            GetEventIdDescription(pEvent->id,GetEventSource()),
            pEvent->x,pEvent->y);

    char label[512+1];
    for (int c=0;c<15;c++)
    {
      if (pEvent->GetEVChainText(c, label))
        fprintf(pFile, "\t%s\n", label);
    }

    if (pEvent->IsAChainEvent())
    {
      POSITION srchpos = m_events.GetHeadPosition();
      bool found = false;
      DWORD pid=0;
      while ((found==false)&&(srchpos!=NULL))
      {
        // scan chains for match
        for (int c=0;c<15;c++)
        {
          if (m_events.GetAt(srchpos)->GetEVChain(c, pid))
          {
            if (pid == pEvent->GetEventId())
            {
              found=true;
              pid = m_events.GetAt(srchpos)->GetEventId();
              break;
            }
          }
        }

        m_events.GetNext(srchpos);
      }

      if (found==false)
      {
        if (IsGlobalSrc)
        {
          fprintf(pFile, "\tParent Event Id: This chain event has no parent\n");
        }
        else
        {
          // no event claims to chain to this one
          fprintf(pFile, "\tParent Event Id: *** ERROR *** This chain event has no parent\n");
        }
      }
      else
      {
        fprintf(pFile, "\tParent Event Id: %u\n", pid);

      }
    }

    if (pEvent->GetEventText() != "")
    {
      fprintf(pFile, "Text 1: '%s'\n", pEvent->GetEventText());
      hasText=true;
    }
    if (pEvent->GetEventText2() != "")
    {
      fprintf(pFile, "Text 2: '%s'\n", pEvent->GetEventText2());
      hasText=true;
    }
    if (pEvent->GetEventText3() != "")
    {
      fprintf(pFile, "Text 3: '%s'\n", pEvent->GetEventText3());
      hasText=true;
    }
    if (!hasText)
      fprintf(pFile, "No Event Text\n");

    if (pEvent->GetEventType()==TavernEvent)
    {
      TAVERN *pTavern = (TAVERN*)pEvent;
      int i;
      for (i=0;i<MAX_TALES;i++)
        fprintf(pFile, "Tavern Tale %i: %s\n", i+1, pTavern->tales[i].tale);
      for (i=0;i<MAX_DRINKS;i++)
        fprintf(pFile, "Drink Name %i: %s\n", i+1, pTavern->drinks[i].name);
    }
    else if (   (pEvent->GetEventType()==GiveTreasure)
             || (pEvent->GetEventType()==CombatTreasure))
    {
      GIVE_TREASURE_DATA *pTreasure = (GIVE_TREASURE_DATA*)pEvent;
      POSITION ipos = pTreasure->items.GetHeadPosition();
      while (ipos!=NULL)
      {
        ITEM it = pTreasure->items.GetAt(ipos);
        CString name = itemData.GetItemIdName(it.item);
        fprintf(pFile, "GiveTreasure Item: %s\n", name);
        pTreasure->items.GetNext(ipos);
      }
    }
    else if (pEvent->GetEventType()==ShopEvent)
    {
      SHOP *pData = (SHOP*)pEvent;
      POSITION ipos = pData->itemsAvail.GetHeadPosition();
      while (ipos!=NULL)
      {
        ITEM it = pData->itemsAvail.GetAt(ipos);
        CString name = itemData.GetItemIdName(it.item);
        fprintf(pFile, "SHOP Item: %s\n", name);
        pData->itemsAvail.GetNext(ipos);
      }
    }

    m_events.GetNext(gpos);
  }

#ifdef _DEBUG
  fprintf(pFile, "\n\n");
  if (IsGlobalSrc)
  {
    CMap<DWORD, DWORD, DWORD, DWORD> eids;
    DWORD tmpid;

    POSITION ipos = itemData.GetHeadPosition();
    while (ipos!=NULL)
    {
      ITEM_DATA &item = itemData.GetAt(ipos);
      
      if (item.ExamineEvent > 0)
      {
        if (eids.Lookup(item.ExamineEvent, tmpid))
        {
          fprintf(pFile, "Item %s Examine Event '%s' *** uses duplicate id ***\n",
                  item.GetItemName(),
                  GetEventIdDescription(item.ExamineEvent, GlobalEventSrc));
        }
        else
        {
          fprintf(pFile, "Item %s Examine Event '%s'\n",
                  item.GetItemName(),
                  GetEventIdDescription(item.ExamineEvent, GlobalEventSrc));
          eids[item.ExamineEvent] = item.ExamineEvent;
        }
      }

      // 'Use' event
      if (item.event > 0)
      {
        if (eids.Lookup(item.event, tmpid))
        {
          fprintf(pFile, "Item %s Use Event '%s' *** uses duplicate id ***\n",
                  item.GetItemName(),
                  GetEventIdDescription(item.event, GlobalEventSrc));
        }
        else
        {
          fprintf(pFile, "Item %s Use Event '%s'\n",
                  item.GetItemName(),
                  GetEventIdDescription(item.event, GlobalEventSrc));
          eids[item.event] = item.event;
        }
      }

      itemData.GetNext(ipos);
    }

    ipos = globalData.charData.GetHeadPosition();
    while (ipos!=NULL)
    {
      CHARACTER &item = globalData.charData.GetAt(ipos);
      
      if (item.ExamineEvent > 0)
      {
        if (eids.Lookup(item.ExamineEvent, tmpid))
        {
          fprintf(pFile, "Character %s Examine Event '%s' *** uses duplicate id ***\n",
                  item.GetName(),
                  GetEventIdDescription(item.ExamineEvent, GlobalEventSrc));
        }
        else
        {
          fprintf(pFile, "Character %s Examine Event '%s'\n",
                  item.GetName(),
                  GetEventIdDescription(item.ExamineEvent, GlobalEventSrc));
          eids[item.ExamineEvent] = item.ExamineEvent;
        }
      }

      if (item.TalkEvent > 0)
      {
        if (eids.Lookup(item.TalkEvent, tmpid))
        {
          fprintf(pFile, "Character %s Talk Event '%s' *** uses duplicate id ***\n",
                  item.GetName(),
                  GetEventIdDescription(item.TalkEvent, GlobalEventSrc));
        }
        else
        {
          fprintf(pFile, "Character %s Talk Event '%s'\n",
                  item.GetName(),
                  GetEventIdDescription(item.TalkEvent, GlobalEventSrc));
          eids[item.TalkEvent] = item.TalkEvent;
        }
      }
      globalData.charData.GetNext(ipos);
    }


    ipos = globalData.specialItemData.GetHeadPosition();
    while (ipos!=NULL)
    {
      SPECIAL_OBJECT_DATA &item = globalData.specialItemData.GetAt(ipos);
      
      if (item.ExamineEvent > 0)
      {
        if (eids.Lookup(item.ExamineEvent, tmpid))
        {
          fprintf(pFile, "Special Item %s Examine Event '%s' *** uses duplicate id ***\n",
                  item.name,
                  GetEventIdDescription(item.ExamineEvent, GlobalEventSrc));
        }
        else
        {
          fprintf(pFile, "Special Item %s Examine Event '%s'\n",
                  item.name,
                  GetEventIdDescription(item.ExamineEvent, GlobalEventSrc));
          eids[item.ExamineEvent] = item.ExamineEvent;
        }
      }

      globalData.specialItemData.GetNext(ipos);
    }

    ipos = globalData.keyData.GetHeadPosition();
    while (ipos!=NULL)
    {
      SPECIAL_OBJECT_DATA &item = globalData.keyData.GetAt(ipos);
      
      if (item.ExamineEvent > 0)
      {
        if (eids.Lookup(item.ExamineEvent, tmpid))
        {
          fprintf(pFile, "Special Key %s Examine Event '%s' *** uses duplicate id ***\n",
                  item.name,
                  GetEventIdDescription(item.ExamineEvent, GlobalEventSrc));
        }
        else
        {
          fprintf(pFile, "Special Key %s Examine Event '%s'\n",
                  item.name,
                  GetEventIdDescription(item.ExamineEvent, GlobalEventSrc));
          eids[item.ExamineEvent] = item.ExamineEvent;
        }
      }

      globalData.keyData.GetNext(ipos);
    }
  }
#endif // _DEBUG

  if (pFile != NULL)
    fclose(pFile);
}
#endif


GPDL_EVENT::GPDL_EVENT(TEXT_EVENT_DATA *txtEvnt) : GameEvent()
{
  GameEvent::operator = (*txtEvnt);
  WaitForReturn=txtEvnt->WaitForReturn;
  HighlightText=txtEvnt->HighlightText;
  ForceBackup=txtEvnt->ForceBackup;
  KillSound=txtEvnt->KillSound;
  sound=txtEvnt->sound;
  hSound=-1;
}

void TRANSFER_DATA::Serialize(CArchive &ar)
{
   CObject::Serialize(ar);

   if (ar.IsStoring())
   {
      ar << execEvent;
      ar << destEP;
      ar << destLevel;
      ar << destX;
      ar << destY;
      ar << (int)facing;
   }
   else
   {
      int temp;
      ar >> execEvent;
      ar >> destEP;
      ar >> destLevel;
      ar >> destX;
      ar >> destY;
      ar >> temp;
      facing = (eventFacingType)temp;
   }
}

void TRANSFER_DATA::Serialize(CAR &ar)
{
   //CObject::Serialize(ar);
   if (ar.IsStoring())
   {
      ar << execEvent;
      ar << destEP;
      ar << destLevel;
      ar << destX;
      ar << destY;
      ar << (int)facing;
   }
   else
   {
      int temp;
      ar >> execEvent;
      ar >> destEP;
      ar >> destLevel;
      ar >> destX;
      ar >> destY;
      ar >> temp;
      facing = (eventFacingType)temp;
   }
}

void MONSTER_EVENT::Serialize(CArchive &ar)
{
   CObject::Serialize(ar);

   if (ar.IsStoring())
   {
      ar << qty;
      ar << type;
      ar << monster;
      ar << friendly;
      ar << moraleAdj;

      ar << qtyDiceSides;
      ar << qtyDiceQty;
      ar << qtyBonus;
      ar << UseQty;

      money.Serialize(ar, LoadingVersion);
   }
   else
   {
      ar >> qty;
      ar >> type;
      ar >> monster;
      ar >> friendly;
      if (LoadingVersion >= _VERSION_0690_)
        ar >> moraleAdj;  

      if (LoadingVersion >= _VERSION_0910_)
      {
        ar >> qtyDiceSides;
        ar >> qtyDiceQty;
        ar >> qtyBonus;
        ar >> UseQty;
      }

      if (LoadingVersion >= _VERSION_0740_)
        money.Serialize(ar, LoadingVersion);
   }

   items.Serialize(ar,LoadingVersion);
}

void MONSTER_EVENT::Serialize(CAR &ar)
{
   //CObject::Serialize(ar);
   if (ar.IsStoring())
   {
      ar << qty;
      ar << type;
      ar << monster;
      ar << friendly;
      ar << moraleAdj;

      ar << qtyDiceSides;
      ar << qtyDiceQty;
      ar << qtyBonus;
      ar << UseQty;

      money.Serialize(ar, LoadingVersion);
   }
   else
   {
      ar >> qty;
      ar >> type;
      ar >> monster;
      ar >> friendly;
      if (LoadingVersion >= _VERSION_0690_)
        ar >> moraleAdj;        

      if (LoadingVersion >= _VERSION_0910_)
      {
        ar >> qtyDiceSides;
        ar >> qtyDiceQty;
        ar >> qtyBonus;
        ar >> UseQty;
      }

      if (LoadingVersion >= _VERSION_0740_)
        money.Serialize(ar, LoadingVersion);
   }

   items.Serialize(ar,LoadingVersion);
}

void MONSTER_EVENT_DATA::Serialize(CArchive &ar)
{
   CObject::Serialize(ar);
   int count, i;

   if (ar.IsStoring())
   {
      monsters.FreeExtra();
      count = GetCount();
      ar << count;
   }
   else
   {
      ar >> count;
      monsters.SetSize(count);
   }

   for (i=0;i<count;i++)
     monsters[i].Serialize(ar);
}
void MONSTER_EVENT_DATA::Serialize(CAR &ar)
{
   //CObject::Serialize(ar);
   int count, i;

   if (ar.IsStoring())
   {
      monsters.FreeExtra();
      count = GetCount();
      ar << count;
   }
   else
   {
      ar >> count;
      monsters.SetSize(count);
   }

   for (i=0;i<count;i++)
     monsters[i].Serialize(ar);
}
void TOUR_STEP::Serialize(CArchive &ar)
{
   CObject::Serialize(ar);

   if (ar.IsStoring())
   {
     AS(ar,text);
//     if (text.IsEmpty())
//     {
//        text = "*";
//        ar << text;
//        text="";
//     }
//     else
//        ar << text;
     ar << (int)step;
   }
   else
   {
     int temp;
     DAS(ar,text);
//     ar >> text;
//     if (text == "*")
//       text = "";
     ar >> temp;
     step = (eventStepType)temp;
   }
}
void TOUR_STEP::Serialize(CAR &ar)
{
   //CObject::Serialize(ar);

   if (ar.IsStoring())
   {
     AS(ar,text);
//     if (text.IsEmpty())
//     {
//        text = "*";
//        ar << text;
//        text="";
//     }
//     else
//        ar << text;
     ar << (int)step;
   }
   else
   {
     int temp;
     DAS(ar,text);
//     ar >> text;
//     if (text == "*")
//       text = "";
     ar >> temp;
     step = (eventStepType)temp;
   }
}
void ENCOUNTER_BUTTON_OPTION::Serialize(CArchive &ar)
{
   CObject::Serialize(ar);

   if (ar.IsStoring())
   {
     AS(ar,label);
//     if (label.IsEmpty())
//     {
//        label = "*";
//        ar << label;
//        label="";
//     }
//     else
//        ar << label;
//
     ar << present;
     ar << allowedUpClose;
     ar << (int)optionResult;
     ar << chain;
     ar << onlyUpClose;
   }
   else
   {
     int temp;

     DAS(ar,label);
//     ar >> label;
//     if (label == "*")
//       label = "";
     ar >> present;
     ar >> allowedUpClose;
     ar >> temp;
     optionResult = (encounterButtonResultType)temp;
     ar >> chain;
     if (LoadingVersion >= _VERSION_0890_)
      ar >> onlyUpClose;
   }
}
void ENCOUNTER_BUTTON_OPTION::Serialize(CAR &ar)
{
   //CObject::Serialize(ar);

   if (ar.IsStoring())
   {
     AS(ar,label);
//     if (label.IsEmpty())
//     {
//        label = "*";
//        ar << label;
//        label="";
//     }
//     else
//        ar << label;
     ar << present;
     ar << allowedUpClose;
     ar << (int)optionResult;
     ar << chain;
     ar << onlyUpClose;
   }
   else
   {
     int temp;

     DAS(ar,label);
//     ar >> label;
//     if (label == "*")
//       label = "";
     ar >> present;
     ar >> allowedUpClose;
     ar >> temp;
     optionResult = (encounterButtonResultType)temp;
     ar >> chain;
     if (LoadingVersion >= _VERSION_0890_)
       ar >> onlyUpClose;
   }
}

void ENCOUNTER_BUTTON_DATA::Serialize(CArchive &ar)
{
   CObject::Serialize(ar);
   int i;

   if (ar.IsStoring())
   {
     ar << numButtons;
   }
   else
   {
     ar >> numButtons;
   }

   for (i=0;i<MAX_BUTTONS;i++)
     buttons[i].Serialize(ar);
}
void ENCOUNTER_BUTTON_DATA::Serialize(CAR &ar)
{
   //CObject::Serialize(ar);
   int i;

   if (ar.IsStoring())
   {
     ar << numButtons;
   }
   else
   {
     ar >> numButtons;
   }

   for (i=0;i<MAX_BUTTONS;i++)
     buttons[i].Serialize(ar);
}

void QLIST_OPTION::Serialize(CArchive &ar)
{
   CObject::Serialize(ar);

   if (ar.IsStoring())
   {

     AS(ar,label);
//     if (label.IsEmpty())
//     {
//        label = "*";
//        ar << label;
//        label="";
//     }
//     else
//        ar << label;
     ar << present;
     ar << (int)postChainAction;
     ar << chain;
   }
   else
   {
     int temp;

     DAS(ar,label);
//     ar >> label;
//     if (label == "*")
//       label = "";
     ar >> present;
     ar >> temp;
     postChainAction = (labelPostChainOptionsType)temp;
     ar >> chain;
   }
}
void QLIST_OPTION::Serialize(CAR &ar)
{
   //CObject::Serialize(ar);

   if (ar.IsStoring())
   {
     AS(ar,label);
//     if (label.IsEmpty())
//     {
//        label = "*";
//        ar << label;
//        label="";
//     }
//     else
//        ar << label;
//
     ar << present;
     ar << (int)postChainAction;
     ar << chain;
   }
   else
   {
     int temp;
     DAS(ar,label);
//     ar >> label;
//     if (label == "*")
//       label = "";
     ar >> present;
     ar >> temp;
     postChainAction = (labelPostChainOptionsType)temp;
     ar >> chain;
   }
}

void QLIST_DATA::Serialize(CArchive &ar)
{
   CObject::Serialize(ar);

   if (ar.IsStoring())
   {
     AS(ar,title);
//     if (title.IsEmpty())
//     {
//        title = "*";
//        ar << title;
//        title="";
//     }
//     else
//        ar << title;
//
     ar << numListButtons;
   }
   else
   {
     DAS(ar,title);
//     ar >> title;
//     if (title == "*")
//       title = "";
     ar >> numListButtons;
   }

   for (int i=0;i<MAX_BUTTONS;i++)
     buttons[i].Serialize(ar);
}
void QLIST_DATA::Serialize(CAR &ar)
{
   //CObject::Serialize(ar);

   if (ar.IsStoring())
   {
     AS(ar,title);
//     if (title.IsEmpty())
//     {
//        title = "*";
//        ar << title;
//        title="";
//     }
//     else
//        ar << title;
//
     ar << numListButtons;
   }
   else
   {
     DAS(ar,title);
//     ar >> title;
//     if (title == "*")
//       title = "";
     ar >> numListButtons;
   }

   for (int i=0;i<MAX_BUTTONS;i++)
     buttons[i].Serialize(ar);
}

void QBUTTON_OPTION::Serialize(CArchive &ar)
{
   CObject::Serialize(ar);

   if (ar.IsStoring())
   {
     AS(ar,label);
//     if (label.IsEmpty())
//     {
//        label = "*";
//        ar << label;
//        label="";
//     }
//     else
//        ar << label;
//
     ar << present;
     ar << (int)postChainAction;
     ar << chain;
   }
   else
   {
     int temp;
     DAS(ar,label);
//     ar >> label;
//     if (label == "*")
//       label = "";
     ar >> present;
     ar >> temp;
     postChainAction = (labelPostChainOptionsType)temp;
     ar >> chain;
   }
}
void QBUTTON_OPTION::Serialize(CAR &ar)
{
   //CObject::Serialize(ar);

   if (ar.IsStoring())
   {
     AS(ar,label);
//     if (label.IsEmpty())
//     {
//        label = "*";
//        ar << label;
//        label="";
//     }
//     else
//        ar << label;
     ar << present;
     ar << (int)postChainAction;
     ar << chain;
   }
   else
   {
     int temp;
     DAS(ar,label);
//     ar >> label;
//     if (label == "*")
//       label = "";
     ar >> present;
     ar >> temp;
     postChainAction = (labelPostChainOptionsType)temp;
     ar >> chain;
   }
}

void QBUTTON_DATA::Serialize(CArchive &ar)
{
   CObject::Serialize(ar);

   if (ar.IsStoring())
   {
     ar << numListButtons;
   }
   else
   {
     ar >> numListButtons;
   }

   for (int i=0;i<MAX_BUTTONS;i++)
      buttons[i].Serialize(ar);
}
void QBUTTON_DATA::Serialize(CAR &ar)
{
   //CObject::Serialize(ar);

   if (ar.IsStoring())
   {
     ar << numListButtons;
   }
   else
   {
     ar >> numListButtons;
   }

   for (int i=0;i<MAX_BUTTONS;i++)
      buttons[i].Serialize(ar);
}

void TALE::Serialize(CArchive &ar)
{
   CObject::Serialize(ar);

   if (ar.IsStoring())
   {
     AS(ar,tale);
     ar << count;
   }
   else
   {
     DAS(ar,tale);
     if (LoadingVersion >= _VERSION_0910_)
       ar >> count;
   }
}
void TALE::Serialize(CAR &ar)
{
   if (ar.IsStoring())
   {
     AS(ar,tale);
     ar << count;
   }
   else
   {
     DAS(ar,tale);
     if (LoadingVersion >= _VERSION_0910_)
       ar >> count;
   }
}

void DRINK::Serialize(CArchive &ar)
{
   CObject::Serialize(ar);

   if (ar.IsStoring())
   {
     AS(ar,name);
//     if (name.IsEmpty())
//     {
//        name = "*";
//        ar << name;
//        name="";
//     }
//     else
//        ar << name;
     ar << points;
   }
   else
   {
     DAS(ar,name);
//     ar >> name;
//     if (name == "*")
//       name = "";
     ar >> points;
   }
}
void DRINK::Serialize(CAR &ar)
{
   //CObject::Serialize(ar);

   if (ar.IsStoring())
   {
     AS(ar,name);
//     if (name.IsEmpty())
//     {
//        name = "*";
//        ar << name;
//        name="";
//     }
//     else
//        ar << name;
     ar << points;
   }
   else
   {
     DAS(ar,name);
//     ar >> name;
//     if (name == "*")
//       name = "";
     ar >> points;
   }
}

void REST_EVENT::Serialize(CArchive &ar)
{
  CObject::Serialize(ar);

  if (ar.IsStoring())
  {
    ar << allowResting;
    ar << restEvent;
    ar << chance;
    ar << everyMin;
    ar << prevMinChecked;
  }
  else
  {
    ar >> allowResting;
    ar >> restEvent;
    ar >> chance;
    ar >> everyMin;
    ar >> prevMinChecked;
  }
}
void REST_EVENT::Serialize(CAR &ar)
{
  //CObject::Serialize(ar);

  if (ar.IsStoring())
  {
    ar << allowResting;
    ar << restEvent;
    ar << chance;
    ar << everyMin;
    ar << prevMinChecked;
  }
  else
  {
    ar >> allowResting;
    ar >> restEvent;
    ar >> chance;
    ar >> everyMin;
    ar >> prevMinChecked;
  }
}

void TIME_EVENT::PreSerialize(BOOL IsStoring)
{
  if (IsStoring)
  {
    // move fields that need to be serialized
    // as attributes into the attribute map  
  }
}

void TIME_EVENT::PostSerialize(BOOL IsStoring)
{
  if (!IsStoring)
  {
    // retrieve fields that are serialized as
    // attributes from the attribute map    
  }
}

void TIME_EVENT::Serialize(CArchive &ar, double version)
{
  PreSerialize(ar.IsStoring());
  
  CObject::Serialize(ar);
  
  if (ar.IsStoring())
  {
    ar << timeEvent;
    ar << triggerType;
    ar << day;
    ar << hour;
    ar << minute;
    
    timeevent_asl.Serialize(ar);
  }
  else
  {
    ar >> timeEvent;
    ar >> triggerType;
    ar >> day;
    ar >> hour;
    ar >> minute;

    timeevent_asl.Serialize(ar);
  }
  
  PostSerialize(ar.IsStoring());
}

void TIME_EVENT::Serialize(CAR &ar, double version)
{
  PreSerialize(ar.IsStoring());
  //CObject::Serialize(ar);
  if (ar.IsStoring())
  {
    ar << timeEvent;
    ar << triggerType;
    ar << day;
    ar << hour;
    ar << minute;
    
    timeevent_asl.Serialize(ar);
  }
  else
  {
    ar >> timeEvent;
    ar >> triggerType;
    ar >> day;
    ar >> hour;
    ar >> minute;
    
    timeevent_asl.Serialize(ar);
  }
  
  PostSerialize(ar.IsStoring());
}


void STEP_EVENT::PreSerialize(BOOL IsStoring)
{
  if (IsStoring)
  {
    // move fields that need to be serialized
    // as attributes into the attribute map  
    StoreIntAsASL(stepevent_asl, stepCount, "StpCnt");
  }
}

void STEP_EVENT::PostSerialize(BOOL IsStoring)
{
  if (!IsStoring)
  {
    // retrieve fields that are serialized as
    // attributes from the attribute map
    RetrieveIntFromASL(stepevent_asl, stepCount, "StpCnt");
  }
}

void STEP_EVENT::Serialize(CArchive &ar, double version)
{
  PreSerialize(ar.IsStoring());

  CObject::Serialize(ar);
  int i;
  
  if (ar.IsStoring())
  {
    ar << stepEvent;
    ar << MAX_ZONES;
    for (i=0;i<MAX_ZONES;i++)
      ar << stepTrigger[i];

    stepevent_asl.Serialize(ar);
  }
  else
  {
    ar >> stepEvent;
    if (version >= _VERSION_05661_)
    {
      int count;
      ar >> count;
      for (i=0;i<count;i++)
        ar >> stepTrigger[i];
    }
    else
    {
      for (i=0;i<8;i++) // load old quantity of zones (= 8)
        ar >> stepTrigger[i];
    }

    if (version >= _VERSION_0566_)
      stepevent_asl.Serialize(ar);
  }

  PostSerialize(ar.IsStoring());
}

void STEP_EVENT::Serialize(CAR &ar, double version)
{
  PreSerialize(ar.IsStoring());
  //CObject::Serialize(ar);
  int i;  
  if (ar.IsStoring())
  {
    ar << stepEvent;
    ar << MAX_ZONES;
    for (i=0;i<MAX_ZONES;i++)
      ar << stepTrigger[i];

    ar << stepevent_asl;
  }
  else
  {
    ar >> stepEvent;
    if (version >= _VERSION_05661_)
    {
      int count;
      ar >> count;
      for (i=0;i<count;i++)
        ar >> stepTrigger[i];
    }
    else
    {
      for (i=0;i<8;i++) // load old quantity of zones (= 8)
        ar >> stepTrigger[i];
    }

    if (version >= _VERSION_0566_)
      ar >> stepevent_asl;
  }

  PostSerialize(ar.IsStoring());
}

TRANSFER_DATA& TRANSFER_DATA::operator =(const TRANSFER_DATA& src)
{
  if (&src == this)
    return *this;
  execEvent = src.execEvent;
  facing = src.facing;
  destEP = src.destEP;
  destLevel = src.destLevel;
  destX = src.destX;
  destY = src.destY;
  return *this;
}

MONSTER_EVENT& MONSTER_EVENT::operator =(const MONSTER_EVENT& src)
{
  if (&src == this)
    return *this;
  qty = src.qty;
  type = src.type;
  monster = src.monster;
  friendly = src.friendly;
  items = src.items;
  moraleAdj = src.moraleAdj;
  qtyDiceSides = src.qtyDiceSides;
  qtyDiceQty = src.qtyDiceQty;
  qtyBonus = src.qtyBonus;
  UseQty = src.UseQty;

  money=src.money;
  return *this;
}

MONSTER_EVENT_DATA& MONSTER_EVENT_DATA::operator =(const MONSTER_EVENT_DATA& src)
{
  if (&src == this)
    return *this;

  Clear();
  for (int i=0;i<src.monsters.GetSize();i++)
    Add(src.monsters[i]);
  return *this;
}

TOUR_STEP& TOUR_STEP::operator =(const TOUR_STEP& src)
{
  if (&src == this)
    return *this;
  text = src.text;
  step = src.step;
  return *this;
}

ENCOUNTER_BUTTON_OPTION& ENCOUNTER_BUTTON_OPTION::operator =(const ENCOUNTER_BUTTON_OPTION& src)
{
  if (&src == this)
    return *this;

  label = src.label;
  present = src.present;
  allowedUpClose = src.allowedUpClose;
  optionResult = src.optionResult;
  chain = src.chain;
  onlyUpClose=src.onlyUpClose;
  return *this;
}

ENCOUNTER_BUTTON_DATA& ENCOUNTER_BUTTON_DATA::operator =(const ENCOUNTER_BUTTON_DATA& src)
{
  if (&src == this)
    return *this;

  numButtons = src.numButtons;
  for (int i=0;i<MAX_BUTTONS;i++)
    buttons[i] = src.buttons[i];
  return *this;
}

QLIST_OPTION& QLIST_OPTION::operator =(const QLIST_OPTION& src)
{
  if (&src == this)
    return *this;

  label = src.label;
  postChainAction = src.postChainAction;
  present = src.present;
  chain = src.chain;
  return *this;
}

QLIST_DATA& QLIST_DATA::operator =(const QLIST_DATA& src)
{
  if (&src == this)
    return *this;

  title = src.title;
  numListButtons = src.numListButtons;
  for (int i=0;i<MAX_BUTTONS; i++)
    buttons[i] = src.buttons[i];
  return *this;
}

QBUTTON_OPTION& QBUTTON_OPTION::operator =(const QBUTTON_OPTION& src)
{
  if (&src == this)
    return *this;

  label = src.label;
  postChainAction = src.postChainAction;
  present = src.present;
  chain = src.chain;
  return *this;
}

QBUTTON_DATA& QBUTTON_DATA::operator =(const QBUTTON_DATA& src)
{
  if (&src == this)
    return *this;

  numListButtons = src.numListButtons;
  for (int i=0;i<MAX_BUTTONS; i++)
    buttons[i] = src.buttons[i];
  return *this;
}

TALE& TALE::operator =(const TALE& src)
{
  if (&src == this)
    return *this;

  tale = src.tale;
  count = src.count;
  return *this;
}

DRINK& DRINK::operator =(const DRINK& src)
{
  if (&src == this)
    return *this;

  name = src.name;
  points = src.points;
  return *this;
}

REST_EVENT& REST_EVENT::operator =(const REST_EVENT& src)
{
  if (&src == this)
    return *this;

  allowResting = src.allowResting;
  restEvent = src.restEvent;
  chance = src.chance;
  everyMin = src.everyMin;
  prevMinChecked = src.prevMinChecked;
  return *this;
}

TIME_EVENT& TIME_EVENT::operator =(const TIME_EVENT& src)
{
  if (&src == this)
    return *this;
  
  timeEvent = src.timeEvent;
  triggerType = src.triggerType;
  day = src.day;
  hour = src.hour;
  minute = src.minute;

  dayCounter = src.dayCounter;
  hourCounter = src.hourCounter;
  minuteCounter = src.minuteCounter;

  timeevent_asl=src.timeevent_asl;
  temp_asl=src.temp_asl;
  return *this;
}

STEP_EVENT& STEP_EVENT::operator =(const STEP_EVENT& src)
{
  if (&src == this)
    return *this;

  stepEvent = src.stepEvent;
  stepCount = src.stepCount;
  for (int i=0;i<MAX_ZONES;i++)
    stepTrigger[i] = src.stepTrigger[i];
  stepevent_asl=src.stepevent_asl;
  temp_asl=src.temp_asl;
  return *this;
}

void PLAY_MOVIE_DATA::SaveEventResources(int level)
{
  GameEvent::SaveEventResources(level);
  saveArt(m_filename,BufferDib,level,TRUE);
}

void PLAY_MOVIE_DATA::Serialize(CArchive &ar)
{
  GameEvent::Serialize(ar);
  if (ar.IsStoring())
  {
    AS(ar, m_filename);
    ar << m_mode;
  }
  else
  {
    DAS(ar, m_filename);
    if (LoadingVersion >= _VERSION_0790_)
      ar >> m_mode;
  }
}

void PLAY_MOVIE_DATA::Serialize(CAR &ar)
{
  GameEvent::Serialize(ar);
  if (ar.IsStoring())
  {
    AS(ar, m_filename);
    ar << m_mode;
  }
  else
  {
    DAS(ar, m_filename);
    if (LoadingVersion >= _VERSION_0790_)
      ar >> m_mode;
  }
}

void ADD_NPC_DATA::Serialize(CArchive &ar)
{
  int temp;
  GameEvent::Serialize(ar);
  if (ar.IsStoring())
  {
    temp = distance;
    ar << temp;
    ar << charAdded;
    ar << hitPointMod;
    ar << useOriginal;
  }
  else
  {
    ar >> temp;
    distance = (eventDistType)temp;
    ar >> charAdded;
    ar >> hitPointMod;
    if (LoadingVersion >= _VERSION_0695_)
      ar >> useOriginal;
  }
}
void ADD_NPC_DATA::Serialize(CAR &ar)
{
  int temp;
  GameEvent::Serialize(ar);
  if (ar.IsStoring())
  {
    temp = distance;
    ar << temp;
    ar << charAdded;
    ar << hitPointMod;
    ar << useOriginal;
  }
  else
  {
    ar >> temp;
    distance = (eventDistType)temp;
    ar >> charAdded;
    ar >> hitPointMod;
    if (LoadingVersion >= _VERSION_0695_)
      ar >> useOriginal;
  }
}

void REMOVE_NPC_DATA::Serialize(CArchive &ar)
{
  int temp;
  GameEvent::Serialize(ar);
  if (ar.IsStoring())
  {
    temp = distance;
    ar << temp;
    ar << charRemoved;
  }
  else
  {
    ar >> temp;
    distance = (eventDistType)temp;
    ar >> charRemoved;
  }
}
void REMOVE_NPC_DATA::Serialize(CAR &ar)
{
  int temp;
  GameEvent::Serialize(ar);
  if (ar.IsStoring())
  {
    temp = distance;
    ar << temp;
    ar << charRemoved;
  }
  else
  {
    ar >> temp;
    distance = (eventDistType)temp;
    ar >> charRemoved;
  }
}

void CAMP_EVENT_DATA::Serialize(CArchive &ar)
{
  GameEvent::Serialize(ar);
  if (ar.IsStoring())
  {
    ar << ForceExit;
  }
  else
  {
    ar >> ForceExit;
  }
}
void CAMP_EVENT_DATA::Serialize(CAR &ar)
{
  GameEvent::Serialize(ar);
  if (ar.IsStoring())
  {
    ar << ForceExit;
  }
  else
  {
    ar >> ForceExit;
  }
}

COMBAT_EVENT_DATA::COMBAT_EVENT_DATA() : GameEvent()
{ 
  Clear(); event=Combat;
}

COMBAT_EVENT_DATA::~COMBAT_EVENT_DATA() 
{ 
  //Clear(); 
}

void COMBAT_EVENT_DATA::Clear() 
{
   GameEvent::Clear();
   
   m_UseOutdoorMap=FALSE;
   distance=FarAway;direction=Any;surprise=Neither;
   autoApproach=FALSE;outdoors=FALSE;noMonsterTreasure=FALSE;
   partyNeverDies=FALSE;noMagic=FALSE;monsterMorale=50;
   partyNoExperience=FALSE;
   randomMonster=FALSE;
   turningMod=None;
   monsters.Clear();
   DeathSound = "";
   MoveSound = "";
   TurnUndeadSound="";
   bgSounds.Clear(); TEMPsurprise=Neither;
#if (defined UAFEDITOR)
   DeathSound.Format("%s%s", EditorArt, DEF_CHAR_DEATH_SOUND);
   MoveSound.Format("%s%s", EditorArt, DEF_CHAR_MOVE_SOUND);
   TurnUndeadSound.Format("%s%s", EditorArt, DEF_CAST_SOUND);
#endif
}

void COMBAT_EVENT_DATA::SaveEventSounds(int level) 
{ 
  GameEvent::SaveEventSounds(level);
  saveSound(DeathSound, level); 
  saveSound(MoveSound, level); 
  saveSound(TurnUndeadSound, level);
  bgSounds.backgroundSounds.SaveSounds(level);
  bgSounds.nightSounds.SaveSounds(level);
}

void COMBAT_EVENT_DATA::Serialize(CArchive &ar)
{
  GameEvent::Serialize(ar);
  if (ar.IsStoring())
  {
    StripFilenamePath(DeathSound);
    StripFilenamePath(MoveSound);
    StripFilenamePath(TurnUndeadSound);

    AS(ar,DeathSound);
    AS(ar,MoveSound);
    AS(ar,TurnUndeadSound);

    AddDesignFolderToPath(DeathSound);
    AddDesignFolderToPath(MoveSound);
    AddDesignFolderToPath(TurnUndeadSound);

    ar << (int)distance;
    ar << (int)direction;
    ar << (int)surprise;
    ar << autoApproach;
    ar << outdoors;
    ar << noMonsterTreasure;
    ar << partyNeverDies;
    ar << noMagic;
    ar << monsterMorale;
    ar << (int)turningMod;
    ar << randomMonster;
    ar << partyNoExperience;

    bgSounds.Serialize(ar);
  }
  else
  {
    DAS(ar,DeathSound);
    DAS(ar,MoveSound);

    if (LoadingVersion >= _VERSION_0750_)
    {
      DAS(ar, TurnUndeadSound);
    }
    else
      TurnUndeadSound = DEF_CAST_SOUND;

    AddDesignFolderToPath(DeathSound);
    AddDesignFolderToPath(MoveSound);
    AddDesignFolderToPath(TurnUndeadSound);

    int temp;
    ar >> temp;
    distance = (eventDistType)temp;   
    ar >> temp;
    direction = (eventDirType)temp;   
    ar >> temp;
    surprise = (eventSurpriseType)temp;   
    ar >> autoApproach;
    ar >> outdoors;
    ar >> noMonsterTreasure;
    ar >> partyNeverDies;
    ar >> noMagic;
    ar >> monsterMorale;   
    ar >> temp;
    turningMod = (eventTurnUndeadModType)temp;
    if (LoadingVersion >= _VERSION_0690_)
      ar >> randomMonster;
    if (LoadingVersion >= _VERSION_0860_)
      ar >> partyNoExperience;
    if (LoadingVersion >= _VERSION_0790_)
      bgSounds.Serialize(ar);
  }
  monsters.Serialize(ar);
}
void COMBAT_EVENT_DATA::Serialize(CAR &ar)
{
  GameEvent::Serialize(ar);
  if (ar.IsStoring())
  {
    StripFilenamePath(DeathSound);
    StripFilenamePath(MoveSound);
    StripFilenamePath(TurnUndeadSound);

    AS(ar,DeathSound);
    AS(ar,MoveSound);
    AS(ar,TurnUndeadSound);
      
    AddDesignFolderToPath(DeathSound);
    AddDesignFolderToPath(MoveSound);
    AddDesignFolderToPath(TurnUndeadSound);

    ar << (int)distance;
    ar << (int)direction;
    ar << (int)surprise;
    ar << autoApproach;
    ar << outdoors;
    ar << noMonsterTreasure;
    ar << partyNeverDies;
    ar << noMagic;
    ar << monsterMorale;
    ar << (int)turningMod;
    ar << randomMonster;
    ar << partyNoExperience;
    bgSounds.Serialize(ar);
  }
  else
  {
    DAS(ar,DeathSound);
    DAS(ar,MoveSound);

    if (LoadingVersion >= _VERSION_0750_)
    {
      DAS(ar, TurnUndeadSound);
    }
    else
      TurnUndeadSound=DEF_CAST_SOUND;

    AddDesignFolderToPath(DeathSound);
    AddDesignFolderToPath(MoveSound);
    AddDesignFolderToPath(TurnUndeadSound);

    int temp;
    ar >> temp;
    distance = (eventDistType)temp;   
    ar >> temp;
    direction = (eventDirType)temp;   
    ar >> temp;
    surprise = (eventSurpriseType)temp;   
    ar >> autoApproach;
    ar >> outdoors;
    ar >> noMonsterTreasure;
    ar >> partyNeverDies;
    ar >> noMagic;
    ar >> monsterMorale;   
    ar >> temp;
    turningMod = (eventTurnUndeadModType)temp;
    if (LoadingVersion >= _VERSION_0690_)
      ar >> randomMonster;
    if (LoadingVersion >= _VERSION_0860_)
      ar >> partyNoExperience;
    if (LoadingVersion >= _VERSION_0790_)
      bgSounds.Serialize(ar);
  }
  monsters.Serialize(ar);
}

void GUIDED_TOUR::Serialize(CArchive &ar)
{
  GameEvent::Serialize(ar);

  if (ar.IsStoring())
  {
    ar << tour_x;
    ar << tour_y;
    ar << facing;
    ar << useStartLoc;   
    ar << executeEvent;
  }
  else
  {
    ar >> tour_x;
    ar >> tour_y;
    ar >> facing;
    ar >> useStartLoc;   
    ar >> executeEvent;
  }

  for (int i=0;i<MAX_TOUR_STEPS;i++)
     steps[i].Serialize(ar);
}
void GUIDED_TOUR::Serialize(CAR &ar)
{
  GameEvent::Serialize(ar);

  if (ar.IsStoring())
  {
    ar << tour_x;
    ar << tour_y;
    ar << facing;
    ar << useStartLoc;   
    ar << executeEvent;
  }
  else
  {
    ar >> tour_x;
    ar >> tour_y;
    ar >> facing;
    ar >> useStartLoc;   
    ar >> executeEvent;
  }

  for (int i=0;i<MAX_TOUR_STEPS;i++)
     steps[i].Serialize(ar);
}

void QUESTION_YES_NO::Serialize(CArchive &ar)
{
  GameEvent::Serialize(ar);
  int temp;

  if (ar.IsStoring())
  {  
    temp = YesChainAction;
    ar << temp;
    temp = NoChainAction;
    ar << temp;
    ar << YesChain;
    ar << NoChain;
  }
  else
  {
    ar >> temp;
    YesChainAction = (labelPostChainOptionsType)temp;
    ar >> temp;
    NoChainAction = (labelPostChainOptionsType)temp;
    ar >> YesChain;
    ar >> NoChain;
  }
}
void QUESTION_YES_NO::Serialize(CAR &ar)
{
  GameEvent::Serialize(ar);
  int temp;

  if (ar.IsStoring())
  {  
    temp = YesChainAction;
    ar << temp;
    temp = NoChainAction;
    ar << temp;
    ar << YesChain;
    ar << NoChain;
  }
  else
  {
    ar >> temp;
    YesChainAction = (labelPostChainOptionsType)temp;
    ar >> temp;
    NoChainAction = (labelPostChainOptionsType)temp;
    ar >> YesChain;
    ar >> NoChain;
  }
}

void ENCOUNTER_DATA::Serialize(CArchive &ar)
{
  GameEvent::Serialize(ar);
  int temp;

  if (ar.IsStoring())
  {
    temp = distance;
    ar << temp;
    ar << monsterSpeed;
    temp = zeroRangeResult;
    ar << temp;
    ar << combatChain;
    ar << talkChain;
    ar << escapeChain;
  }
  else
  {
    ar >> temp;
    distance = (eventDistType)temp;
    ar >> monsterSpeed;   
    ar >> temp;
    zeroRangeResult = (encounterButtonResultType)temp;
    ar >> combatChain;
    ar >> talkChain;
    ar >> escapeChain;
  }

  buttons.Serialize(ar);
}
void ENCOUNTER_DATA::Serialize(CAR &ar)
{
  GameEvent::Serialize(ar);
  int temp;

  if (ar.IsStoring())
  {
    temp = distance;
    ar << temp;
    ar << monsterSpeed;
    temp = zeroRangeResult;
    ar << temp;
    ar << combatChain;
    ar << talkChain;
    ar << escapeChain;
  }
  else
  {
    ar >> temp;
    distance = (eventDistType)temp;
    ar >> monsterSpeed;   
    ar >> temp;
    zeroRangeResult = (encounterButtonResultType)temp;
    ar >> combatChain;
    ar >> talkChain;
    ar >> escapeChain;
  }

  buttons.Serialize(ar);
}

BOOL PASSWORD_DATA::PasswordMatches(CString typed)
{
  BOOL success = FALSE;
  switch (matchCriteria)
  {
  case PswdInTyped:
    if (matchCase)
    {
      success = (strstr(typed, password) != NULL);
    }
    else
    {
      CString p = password;
      p.MakeLower();
      typed.MakeLower();
      success = (strstr(typed, p) != NULL);
    }
    break;
  case TypedInPswd:
    if (matchCase)
    {
      success = (strstr(password, typed) != NULL);
    }
    else
    {
      CString p = password;
      p.MakeLower();
      typed.MakeLower();
      success = (strstr(p, typed) != NULL);
    }
    break;
  default: // should be ExactMatch
    if (matchCase)
      success = (strcmp(typed, password) == 0);
    else
      success = (stricmp(typed, password) == 0);
    break;
  }
  return success;
}

void PASSWORD_DATA::PreSerialize(BOOL IsStoring)
{
  if (IsStoring)
  {
    // move fields that need to be serialized
    // as attributes into the attribute map  
    StoreIntAsASL(event_asl, (int)matchCriteria, "MtchCri");
    StoreIntAsASL(event_asl, (int)matchCase, "MtchCse");
  }
}

void PASSWORD_DATA::PostSerialize(BOOL IsStoring)
{
  GameEvent::PostSerialize(IsStoring);
  if (!IsStoring)
  {
    // retrieve fields that are serialized as
    // attributes from the attribute map
    int temp;
    RetrieveIntFromASL(event_asl, temp, "MtchCri");
    matchCriteria = (passwordMatchType)temp;
    RetrieveIntFromASL(event_asl, temp, "MtchCse");
    matchCase = (BOOL)temp;
  }
}

void PASSWORD_DATA::Serialize(CArchive &ar)
{
  GameEvent::Serialize(ar);
  int temp;  

  if (ar.IsStoring())
  {
    ar << nbrTries;
    ar << successChain;
    ar << failChain;
    temp = successAction;
    ar << temp;
    temp = failAction;
    ar << temp;

    AS(ar,password);
//    if (password.IsEmpty())
//    {
//      password = "*";
//      ar << password;
//      password="";
//    }
//    else
//      ar << password;
  }
  else
  {
    ar >> nbrTries;
    ar >> successChain;
    ar >> failChain;
    ar >> temp;
    successAction = (passwordActionType)temp;   
    ar >> temp;
    failAction = (passwordActionType)temp;
    DAS(ar,password);
//    ar >> password;
//    if (password == "*")
//     password = "";
  }

  successTransferData.Serialize(ar);
  failTransferData.Serialize(ar);
}
void PASSWORD_DATA::Serialize(CAR &ar)
{
  GameEvent::Serialize(ar);
  int temp;  

  if (ar.IsStoring())
  {
    ar << nbrTries;
    ar << successChain;
    ar << failChain;
    temp = successAction;
    ar << temp;
    temp = failAction;
    ar << temp;

    AS(ar,password);
//    if (password.IsEmpty())
//    {
//      password = "*";
//      ar << password;
//      password="";
//    }
//    else
//      ar << password;
  }
  else
  {
    ar >> nbrTries;
    ar >> successChain;
    ar >> failChain;
    ar >> temp;
    successAction = (passwordActionType)temp;   
    ar >> temp;
    failAction = (passwordActionType)temp;
    DAS(ar,password);
//    ar >> password;
//    if (password == "*")
//     password = "";
  }

  successTransferData.Serialize(ar);
  failTransferData.Serialize(ar);
}

void GAIN_EXP_DATA::SaveEventSounds(int level) 
{ 
  GameEvent::SaveEventSounds(level);
  saveSound(sound, level); 
}

void GAIN_EXP_DATA::Serialize(CArchive &ar)
{
  GameEvent::Serialize(ar);
  int temp;

  if (ar.IsStoring())
  {   
    ar << experience;
    
    StripFilenamePath(sound);

    AS(ar,sound);
//    if (sound == "")
//    {
//      sound = "*";
//      ar << sound;
//      sound = "";
//    }
//    else
//      ar << sound;
    AddDesignFolderToPath(sound);

    ar << chance;
    temp = who;
    ar << temp;
  }
  else
  {
    ar >> experience;

    DAS(ar,sound);
//    ar >> sound;
//    if (sound == "*")
//      sound = "";
    AddDesignFolderToPath(sound);
    
    ar >> chance;
    ar >> temp;
    who = (eventPartyAffectType)temp;
  }
}
void GAIN_EXP_DATA::Serialize(CAR &ar)
{
  GameEvent::Serialize(ar);
  int temp;

  if (ar.IsStoring())
  {   
    ar << experience;
    
    StripFilenamePath(sound);

    AS(ar,sound);
//    if (sound == "")
//    {
//      sound = "*";
//      ar << sound;
//      sound = "";
//    }
//    else
//      ar << sound;
    AddDesignFolderToPath(sound);

    ar << chance;
    temp = who;
    ar << temp;
  }
  else
  {
    ar >> experience;

    DAS(ar,sound);
//    ar >> sound;
//    if (sound == "*")
//      sound = "";
    AddDesignFolderToPath(sound);
    
    ar >> chance;
    ar >> temp;
    who = (eventPartyAffectType)temp;
  }
}

void GIVE_TREASURE_DATA::Clear() 
{ 
  GameEvent::Clear();
  
  SilentGiveToActiveChar=FALSE; m_detectSpellKey=0;
  money.Clear();
  items.Clear(); 
  moneyRemain.Clear();
  itemsRemain.Clear();
}

void GIVE_TREASURE_DATA::Serialize(CArchive &ar)
{
  GameEvent::Serialize(ar);

  if (ar.IsStoring())
  {
    money.Serialize(ar, LoadingVersion);
    ar << SilentGiveToActiveChar;
  }
  else
  {
    if (LoadingVersion < _VERSION_0740_)
    {
      itemClassType coinType=PlatinumType;
      if (LoadingVersion >= _VERSION_0670_)
      {
        int tmp;
        ar >> tmp; coinType=(itemClassType)tmp;
      }
      int plat,gems,jewels;
      ar >> plat;
      ar >> gems;
      ar >> jewels;
      money.Add(coinType, plat);
      for (int g=0;g<gems;g++) money.AddGem();
      for (int j=0;j<jewels;j++) money.AddGem();
    }
    else
    {
      money.Serialize(ar, LoadingVersion);
    }

    if (LoadingVersion >= _VERSION_0890_)
      ar >> SilentGiveToActiveChar;
  }

  items.Serialize(ar,LoadingVersion);
}
void GIVE_TREASURE_DATA::Serialize(CAR &ar)
{
  GameEvent::Serialize(ar);

  if (ar.IsStoring())
  {
    money.Serialize(ar, LoadingVersion);
    ar << SilentGiveToActiveChar;
  }
  else
  {
    if (LoadingVersion < _VERSION_0740_)
    {
      itemClassType coinType=PlatinumType;
      if (LoadingVersion >= _VERSION_0670_)
      {
        int tmp;
        ar >> tmp; coinType=(itemClassType)tmp;
      }
      int plat,gems,jewels;
      ar >> plat;
      ar >> gems;
      ar >> jewels;
      money.Add(coinType, plat);
      for (int g=0;g<gems;g++) money.AddGem();
      for (int j=0;j<jewels;j++) money.AddGem();
    }
    else
    {
      money.Serialize(ar, LoadingVersion);
    }

    if (LoadingVersion >= _VERSION_0890_)
      ar >> SilentGiveToActiveChar;
  }

  items.Serialize(ar,LoadingVersion);
}

void GIVE_DAMAGE_DATA::Serialize(CArchive &ar)
{
  GameEvent::Serialize(ar);
  int temp;  

  if (ar.IsStoring())
  {
    ar << nbrAttacks;
    ar << chancePerAttack;
    ar << dmgDice;
    ar << dmgDiceQty;
    ar << dmgBonus;
    ar << saveBonus;
    ar << attackTHACO;
    temp = eventSave;
    ar << temp;
    temp = spellSave;
    ar << temp;
    temp = who;
    ar << temp;
    temp = distance;
    ar << temp;
  }
  else
  {
    ar >> nbrAttacks;
    ar >> chancePerAttack;
    ar >> dmgDice;
    ar >> dmgDiceQty;
    ar >> dmgBonus;
    ar >> saveBonus;
    ar >> attackTHACO;   
    ar >> temp;
    eventSave = (spellSaveEffectType)temp;   
    ar >> temp;
    spellSave = (spellSaveVsType)temp;   
    ar >> temp;
    who = (eventPartyAffectType)temp;   
    ar >> temp;
    distance = (eventDistType)temp;
  }
}
void GIVE_DAMAGE_DATA::Serialize(CAR &ar)
{
  GameEvent::Serialize(ar);
  int temp;  

  if (ar.IsStoring())
  {
    ar << nbrAttacks;
    ar << chancePerAttack;
    ar << dmgDice;
    ar << dmgDiceQty;
    ar << dmgBonus;
    ar << saveBonus;
    ar << attackTHACO;
    temp = eventSave;
    ar << temp;
    temp = spellSave;
    ar << temp;
    temp = who;
    ar << temp;
    temp = distance;
    ar << temp;
  }
  else
  {
    ar >> nbrAttacks;
    ar >> chancePerAttack;
    ar >> dmgDice;
    ar >> dmgDiceQty;
    ar >> dmgBonus;
    ar >> saveBonus;
    ar >> attackTHACO;   
    ar >> temp;
    eventSave = (spellSaveEffectType)temp;   
    ar >> temp;
    spellSave = (spellSaveVsType)temp;   
    ar >> temp;
    who = (eventPartyAffectType)temp;   
    ar >> temp;
    distance = (eventDistType)temp;
  }
}

void NPC_SAYS_DATA::SaveEventSounds(int level) 
{ 
  GameEvent::SaveEventSounds(level);
  saveSound(sound, level); 
}

void NPC_SAYS_DATA::Serialize(CArchive &ar)
{
  GameEvent::Serialize(ar);
  int temp;
  if (ar.IsStoring())
  {
    ar << partySeesChar;
    temp = distance;
    ar << temp;
    
    StripFilenamePath(sound);
    AS(ar,sound);
//    if (sound == "")
//    {
//      sound = "*";
//      ar << sound;
//      sound = "";
//    }
//    else
//      ar << sound;
    AddDesignFolderToPath(sound);

    ar << mustHitReturn;
    ar << highlight;
  }
  else
  {
    ar >> partySeesChar;
    ar >> temp;
    distance = (eventDistType)temp;
  
    DAS(ar,sound);
//    ar >> sound;
//    if (sound == "*")
//      sound = "";
    AddDesignFolderToPath(sound);

    ar >> mustHitReturn;
    ar >> highlight;
  }
}
void NPC_SAYS_DATA::Serialize(CAR &ar)
{
  GameEvent::Serialize(ar);
  int temp;
  if (ar.IsStoring())
  {
    ar << partySeesChar;
    temp = distance;
    ar << temp;
    
    StripFilenamePath(sound);

    AS(ar,sound);
//    if (sound == "")
//    {
//      sound = "*";
//      ar << sound;
//      sound = "";
//    }
//    else
//      ar << sound;
    AddDesignFolderToPath(sound);

    ar << mustHitReturn;
    ar << highlight;
  }
  else
  {
    ar >> partySeesChar;
    ar >> temp;
    distance = (eventDistType)temp;
  
    DAS(ar,sound);
//    ar >> sound;
//    if (sound == "*")
//      sound = "";
    AddDesignFolderToPath(sound);

    ar >> mustHitReturn;
    ar >> highlight;
  }
}


void QUESTION_LIST_DATA::Serialize(CArchive &ar)
{
  GameEvent::Serialize(ar);
  buttons.Serialize(ar);
}
void QUESTION_LIST_DATA::Serialize(CAR &ar)
{
  GameEvent::Serialize(ar);
  buttons.Serialize(ar);
}

void QUESTION_BUTTON_DATA::Serialize(CArchive &ar)
{
  GameEvent::Serialize(ar);
  buttons.Serialize(ar);
}
void QUESTION_BUTTON_DATA::Serialize(CAR &ar)
{
  GameEvent::Serialize(ar);
  buttons.Serialize(ar);
}

void TAKE_PARTY_ITEMS_DATA::Clear()
{
  GameEvent::Clear();
  
  StoreItems = FALSE;
  mustHitReturn = TRUE;
  takeItems = TakeInventoryAction;
  takeAffects = TakeAffectsActiveChar;
  itemSelectFlags = TakeSpecifiedQty;
  platinumSelectFlags = TakeSpecifiedQty;
  gemsSelectFlags = TakeSpecifiedQty;
  jewelrySelectFlags = TakeSpecifiedQty;
  platinum = 0;
  gems = 0;
  jewelry = 0;
  itemPcnt = 0;
  moneyType = PlatinumType;
  items.Clear();
  WhichVault=0;
}

TAKE_PARTY_ITEMS_DATA& TAKE_PARTY_ITEMS_DATA::operator =(const TAKE_PARTY_ITEMS_DATA& src)
{
  if (&src == this)
    return *this;
  GameEvent::operator =(src);
  StoreItems = src.StoreItems;
  mustHitReturn = src.mustHitReturn;
  takeItems = src.takeItems;
  takeAffects = src.takeAffects;
  itemSelectFlags = src.itemSelectFlags;
  platinumSelectFlags = src.platinumSelectFlags;
  gemsSelectFlags = src.gemsSelectFlags;
  jewelrySelectFlags = src.jewelrySelectFlags;
  platinum = src.platinum;
  gems = src.gems;
  jewelry = src.jewelry;
  itemPcnt = src.itemPcnt;
  moneyType = src.moneyType;
  items = src.items;
  WhichVault=src.WhichVault;
  return *this;
}

void TAKE_PARTY_ITEMS_DATA::Serialize(CArchive &ar)
{
  GameEvent::Serialize(ar);

  if (ar.IsStoring())
  {
    ar << StoreItems;
    ar << mustHitReturn;
    ar << takeItems;
    ar << (int)takeAffects;
    ar << (int)itemSelectFlags;
    ar << (int)platinumSelectFlags;
    ar << (int)gemsSelectFlags;
    ar << (int)jewelrySelectFlags;
    ar << platinum;
    ar << gems;
    ar << jewelry;
    ar << itemPcnt;
    ar << (int)moneyType;
    ar << WhichVault;
  }
  else
  {
    int temp;
    ar >> StoreItems;
    ar >> mustHitReturn;
    ar >> takeItems;
    ar >> temp;
    takeAffects = (takeItemsAffectsType)temp;
    ar >> temp;
    itemSelectFlags = (takeItemQtyType)temp;
    ar >> temp;
    platinumSelectFlags = (takeItemQtyType)temp;
    ar >> temp;
    gemsSelectFlags = (takeItemQtyType)temp;
    ar >> temp;
    jewelrySelectFlags = (takeItemQtyType)temp;
    ar >> platinum;
    ar >> gems;
    ar >> jewelry; 
    ar >> itemPcnt;
    ar >> temp;
    moneyType = (itemClassType)temp;

    if (LoadingVersion >= _VERSION_0910_)
      ar >> WhichVault;
  }

  items.Serialize(ar,LoadingVersion);
}
void TAKE_PARTY_ITEMS_DATA::Serialize(CAR &ar)
{
  GameEvent::Serialize(ar);

  if (ar.IsStoring())
  {
    ar << StoreItems;
    ar << mustHitReturn;
    ar << takeItems;
    ar << (int)takeAffects;
    ar << (int)itemSelectFlags;
    ar << (int)platinumSelectFlags;
    ar << (int)gemsSelectFlags;
    ar << (int)jewelrySelectFlags;
    ar << platinum;
    ar << gems;
    ar << jewelry;
    ar << itemPcnt;
    ar << (int)moneyType;
    ar << WhichVault;
  }
  else
  {
    int temp;
    ar >> StoreItems;
    ar >> mustHitReturn;
    ar >> takeItems;
    ar >> temp;
    takeAffects = (takeItemsAffectsType)temp;
    ar >> temp;
    itemSelectFlags = (takeItemQtyType)temp;
    ar >> temp;
    platinumSelectFlags = (takeItemQtyType)temp;
    ar >> temp;
    gemsSelectFlags = (takeItemQtyType)temp;
    ar >> temp;
    jewelrySelectFlags = (takeItemQtyType)temp;
    ar >> platinum;
    ar >> gems;
    ar >> jewelry; 
    ar >> itemPcnt;
    ar >> temp;
    moneyType = (itemClassType)temp;

    if (LoadingVersion >= _VERSION_0910_)
      ar >> WhichVault;
  }

  items.Serialize(ar,LoadingVersion);
}

void TEXT_EVENT_DATA::SaveEventSounds(int level) 
{ 
  GameEvent::SaveEventSounds(level);
  saveSound(sound, level); 
}

void TEXT_EVENT_DATA::PreSerialize(BOOL IsStoring)
{
  if (IsStoring)
  {
    // move fields that need to be serialized
    // as attributes into the attribute map  
    StoreIntAsASL(event_asl, (int)KillSound, "KSnd");
  }
}

void TEXT_EVENT_DATA::PostSerialize(BOOL IsStoring)
{
  GameEvent::PostSerialize(IsStoring);
  if (!IsStoring)
  {
    // retrieve fields that are serialized as
    // attributes from the attribute map
    int temp;
    RetrieveIntFromASL(event_asl, temp, "KSnd");
    KillSound = (BOOL)temp;
  }
}

void TEXT_EVENT_DATA::Serialize(CArchive &ar)
{
  GameEvent::Serialize(ar);

  if (ar.IsStoring())
  {
    ar << WaitForReturn;
    ar << ForceBackup;
    ar << HighlightText;
    ar << distance;

    StripFilenamePath(sound);

    AS(ar,sound);
    AddDesignFolderToPath(sound);
  }
  else
  {
    ar >> WaitForReturn;
    ar >> ForceBackup;
    ar >> HighlightText;

    if (LoadingVersion >= _VERSION_0908_)
    {
      int temp;
      ar >> temp;
      distance = (eventDistType)temp;
    }
    
    DAS(ar,sound);
    AddDesignFolderToPath(sound);
  }
}
void TEXT_EVENT_DATA::Serialize(CAR &ar)
{
  GameEvent::Serialize(ar);

  if (ar.IsStoring())
  {
    ar << WaitForReturn;
    ar << ForceBackup;
    ar << HighlightText;
    ar << distance;

    StripFilenamePath(sound);
    AS(ar,sound);
    AddDesignFolderToPath(sound);
  }
  else
  {
    ar >> WaitForReturn;
    ar >> ForceBackup;
    ar >> HighlightText;

    if (LoadingVersion >= _VERSION_0908_)
    {
      int temp;
      ar >> temp;
      distance = (eventDistType)temp;
    }
    
    DAS(ar,sound);
    AddDesignFolderToPath(sound);
  }
}

void TRANSFER_EVENT_DATA::Serialize(CArchive &ar)
{
  GameEvent::Serialize(ar);

  if (ar.IsStoring())
  {
    ar << askYesNo;
    ar << transferOnYes;
    ar << destroyDrow;
    ar << activateBeforeEntry;
  }
  else
  {
    ar >> askYesNo;
    ar >> transferOnYes;
    ar >> destroyDrow;
    ar >> activateBeforeEntry;
  }

  transferData.Serialize(ar);
}
void TRANSFER_EVENT_DATA::Serialize(CAR &ar)
{
  GameEvent::Serialize(ar);

  if (ar.IsStoring())
  {
    ar << askYesNo;
    ar << transferOnYes;
    ar << destroyDrow;
    ar << activateBeforeEntry;
  }
  else
  {
    ar >> askYesNo;
    ar >> transferOnYes;
    ar >> destroyDrow;
    ar >> activateBeforeEntry;
  }

  transferData.Serialize(ar);
}

void WHO_PAYS_EVENT_DATA::Serialize(CArchive &ar)
{
  GameEvent::Serialize(ar);

  if (ar.IsStoring())
  {
    ar << impossible;
    ar << gems;
    ar << jewels;
    ar << platinum;
    ar << successChain;
    ar << (int)successAction;
    ar << (int)failAction;
    ar << failChain;
    ar << (int)moneyType;
  }
  else
  {
    int temp;
    ar >> impossible;
    ar >> gems;
    ar >> jewels;
    ar >> platinum;
    ar >> successChain;
    ar >> temp;
    successAction = (passwordActionType)temp;
    ar >> temp;
    failAction = (passwordActionType)temp;
    ar >> failChain;

    if (LoadingVersion >= _VERSION_0912_)
    {
      int temp;
      ar >> temp;
      moneyType = (itemClassType)temp;
    }
  }

  successTransferData.Serialize(ar);
  failTransferData.Serialize(ar);
}
void WHO_PAYS_EVENT_DATA::Serialize(CAR &ar)
{
  GameEvent::Serialize(ar);

  if (ar.IsStoring())
  {
    ar << impossible;
    ar << gems;
    ar << jewels;
    ar << platinum;
    ar << successChain;
    ar << (int)successAction;
    ar << (int)failAction;
    ar << failChain;
    ar << moneyType;
  }
  else
  {
    int temp;
    ar >> impossible;
    ar >> gems;
    ar >> jewels;
    ar >> platinum;
    ar >> successChain;
    ar >> temp;
    successAction = (passwordActionType)temp;
    ar >> temp;
    failAction = (passwordActionType)temp;
    ar >> failChain;

    if (LoadingVersion >= _VERSION_0912_)
    {
      int temp;
      ar >> temp;
      moneyType = (itemClassType)temp;
    }
  }

  successTransferData.Serialize(ar);
  failTransferData.Serialize(ar);
}

void WHO_TRIES_EVENT_DATA::Serialize(CArchive &ar)
{
  GameEvent::Serialize(ar);

  if (ar.IsStoring())
  {
    ar << alwaysSucceeds;
    ar << alwaysFails;
    ar << checkSTR;
    ar << checkINT;
    ar << checkWIS;
    ar << checkDEX;
    ar << checkCON;
    ar << checkCHA;
    ar << checkPP;
    ar << checkOL;
    ar << checkFT;
    ar << checkMS;
    ar << checkHS;
    ar << checkHN;
    ar << checkCW;
    ar << checkRL;
    ar << strBonus;
    ar << compareToDie;
    ar << compareDie;
    ar << NbrTries;
    ar << successChain;
    ar << (int)successAction;
    ar << (int)failAction;
    ar << failChain;
  }
  else
  {
    int temp;
    ar >> alwaysSucceeds;
    ar >> alwaysFails;
    ar >> checkSTR;
    ar >> checkINT;
    ar >> checkWIS;
    ar >> checkDEX;
    ar >> checkCON;
    ar >> checkCHA;
    ar >> checkPP;
    ar >> checkOL;
    ar >> checkFT;
    ar >> checkMS;
    ar >> checkHS;
    ar >> checkHN;
    ar >> checkCW;
    ar >> checkRL;
    ar >> strBonus;
    ar >> compareToDie;
    ar >> compareDie;
    ar >> NbrTries;
    ar >> successChain;
    ar >> temp;
    successAction = (passwordActionType)temp;
    ar >> temp;
    failAction = (passwordActionType)temp;
    ar >> failChain;
  }

  successTransferData.Serialize(ar);
  failTransferData.Serialize(ar);
}
void WHO_TRIES_EVENT_DATA::Serialize(CAR &ar)
{
  GameEvent::Serialize(ar);

  if (ar.IsStoring())
  {
    ar << alwaysSucceeds;
    ar << alwaysFails;
    ar << checkSTR;
    ar << checkINT;
    ar << checkWIS;
    ar << checkDEX;
    ar << checkCON;
    ar << checkCHA;
    ar << checkPP;
    ar << checkOL;
    ar << checkFT;
    ar << checkMS;
    ar << checkHS;
    ar << checkHN;
    ar << checkCW;
    ar << checkRL;
    ar << strBonus;
    ar << compareToDie;
    ar << compareDie;
    ar << NbrTries;
    ar << successChain;
    ar << (int)successAction;
    ar << (int)failAction;
    ar << failChain;
  }
  else
  {
    int temp;
    ar >> alwaysSucceeds;
    ar >> alwaysFails;
    ar >> checkSTR;
    ar >> checkINT;
    ar >> checkWIS;
    ar >> checkDEX;
    ar >> checkCON;
    ar >> checkCHA;
    ar >> checkPP;
    ar >> checkOL;
    ar >> checkFT;
    ar >> checkMS;
    ar >> checkHS;
    ar >> checkHN;
    ar >> checkCW;
    ar >> checkRL;
    ar >> strBonus;
    ar >> compareToDie;
    ar >> compareDie;
    ar >> NbrTries;
    ar >> successChain;
    ar >> temp;
    successAction = (passwordActionType)temp;
    ar >> temp;
    failAction = (passwordActionType)temp;
    ar >> failChain;
  }

  successTransferData.Serialize(ar);
  failTransferData.Serialize(ar);
}

void SPECIAL_ITEM_KEY_EVENT_DATA::Serialize(CArchive &ar)
{
  GameEvent::Serialize(ar);
  items.Serialize(ar);
  if (ar.IsStoring())
  {
    ar << forceExit;
    ar << WaitForReturn;
  }
  else
  {
    if (LoadingVersion >= _VERSION_0830_)
    {
      ar >> forceExit;
      ar >> WaitForReturn;
    }
  }
}
void SPECIAL_ITEM_KEY_EVENT_DATA::Serialize(CAR &ar)
{
  GameEvent::Serialize(ar);
  items.Serialize(ar);
  if (ar.IsStoring())
  {
    ar << forceExit;
    ar << WaitForReturn;
  }
  else
  {
    if (LoadingVersion >= _VERSION_0830_)
    {
      ar >> forceExit;
      ar >> WaitForReturn;
    }
  }
}

void PASS_TIME_EVENT_DATA::Serialize(CArchive &ar)
{
  GameEvent::Serialize(ar);

  if (ar.IsStoring())
  {
    ar << Days;
    ar << Hours;
    ar << Minutes;
    ar << AllowStop;
    ar << SetTime;
    ar << PassSilent;
  }
  else
  {
    ar >> Days;
    ar >> Hours;
    ar >> Minutes;
    if (LoadingVersion >= _VERSION_0830_)
    {
      ar >> AllowStop;
      ar >> SetTime;
      ar >> PassSilent;
    }
  }
}
void PASS_TIME_EVENT_DATA::Serialize(CAR &ar)
{
  GameEvent::Serialize(ar);

  if (ar.IsStoring())
  {
    ar << Days;
    ar << Hours;
    ar << Minutes;
    ar << AllowStop;
    ar << SetTime;
    ar << PassSilent;
  }
  else
  {
    ar >> Days;
    ar >> Hours;
    ar >> Minutes;
    if (LoadingVersion >= _VERSION_0830_)
    {
      ar >> AllowStop;
      ar >> SetTime;
      ar >> PassSilent;
    }
  }
}

void VAULT_EVENT_DATA::Serialize(CArchive &ar)
{
  GameEvent::Serialize(ar);

  if (ar.IsStoring())
  {
    ar << ForceBackup;
    ar << WhichVault;
  }
  else
  {
    ar >> ForceBackup;

    if (LoadingVersion >= _VERSION_0910_)
      ar >> WhichVault;
  }
}
void VAULT_EVENT_DATA::Serialize(CAR &ar)
{
  GameEvent::Serialize(ar);

  if (ar.IsStoring())
  {
    ar << ForceBackup;
    ar << WhichVault;
  }
  else
  {
    ar >> ForceBackup;

    if (LoadingVersion >= _VERSION_0910_)
      ar >> WhichVault;
  }
}

void QUEST_EVENT_DATA::Serialize(CArchive &ar)
{
  GameEvent::Serialize(ar);
  int temp;

  if (ar.IsStoring())
  {
    ar << (int)accept;
    ar << completeOnAccept;
    ar << failOnRejection;
    ar << quest;
    ar << stage;
    ar << acceptChain;
    ar << rejectChain;
  }
  else
  {
    ar >> temp;
    accept = (QuestAcceptType)temp;
    ar >> completeOnAccept;
    ar >> failOnRejection;
    ar >> quest;
    ar >> stage;
    ar >> acceptChain;
    ar >> rejectChain;
  }
}
void QUEST_EVENT_DATA::Serialize(CAR &ar)
{
  GameEvent::Serialize(ar);
  int temp;

  if (ar.IsStoring())
  {
    ar << (int)accept;
    ar << completeOnAccept;
    ar << failOnRejection;
    ar << quest;
    ar << stage;
    ar << acceptChain;
    ar << rejectChain;
  }
  else
  {
    ar >> temp;
    accept = (QuestAcceptType)temp;
    ar >> completeOnAccept;
    ar >> failOnRejection;
    ar >> quest;
    ar >> stage;
    ar >> acceptChain;
    ar >> rejectChain;
  }
}


void TAVERN::Serialize(CArchive &ar)
{
  GameEvent::Serialize(ar);
  int temp;

  if (ar.IsStoring())
  {
    ar << forceExit;
    ar << inflation;
    ar << barkeep;
    ar << allowFights;
    ar << allowDrinks;
    ar << fightChain;
    ar << drinkChain;
    ar << drinkPointTrigger;
    temp = (int)taleOrder;
    ar << temp;
    ar << EachTaleOnceOnly;
  }
  else
  {
    ar >> forceExit;
    ar >> inflation;
    ar >> barkeep;
    ar >> allowFights;
    ar >> allowDrinks;
    ar >> fightChain;
    ar >> drinkChain;
    ar >> drinkPointTrigger;
    ar >> temp;
    taleOrder = (taleOrderType)temp;
    if (LoadingVersion >= _VERSION_0910_)
      ar >> EachTaleOnceOnly;
  }

  int i;

  if (ar.IsStoring())
  {
    ar << MAX_TALES;
    for (i=0;i<MAX_TALES;i++)
      tales[i].Serialize(ar);
  }
  else
  {
    if (LoadingVersion < _VERSION_0910_)
    {
      int count = 10; // old number of tales

      for (i=0;i<10;i++)
        tales[i].Serialize(ar);
    }
    else
    {
      int tmp;
      ar >> tmp;
      
      ASSERT( tmp >= 10 );
      ASSERT( tmp <= MAX_TALES );
      
      if (tmp > MAX_TALES)
        tmp = MAX_TALES;

      for (i=0;i<tmp;i++)
        tales[i].Serialize(ar);
    }
  }

  for (i=0;i<MAX_DRINKS;i++)
    drinks[i].Serialize(ar);
}
void TAVERN::Serialize(CAR &ar)
{
  GameEvent::Serialize(ar);
  int temp;

  if (ar.IsStoring())
  {
    ar << forceExit;
    ar << inflation;
    ar << barkeep;
    ar << allowFights;
    ar << allowDrinks;
    ar << fightChain;
    ar << drinkChain;
    ar << drinkPointTrigger;
    temp = (int)taleOrder;
    ar << temp;
    ar << EachTaleOnceOnly;
  }
  else
  {
    ar >> forceExit;
    ar >> inflation;
    ar >> barkeep;
    ar >> allowFights;
    ar >> allowDrinks;
    ar >> fightChain;
    ar >> drinkChain;
    ar >> drinkPointTrigger;
    ar >> temp;
    taleOrder = (taleOrderType)temp;
    if (LoadingVersion >= _VERSION_0910_)
      ar >> EachTaleOnceOnly;
  }

  int i;
  if (ar.IsStoring())
  {
    ar << MAX_TALES;
    for (i=0;i<MAX_TALES;i++)
      tales[i].Serialize(ar);
  }
  else
  {
    if (LoadingVersion < _VERSION_0910_)
    {
      int count = 10; // old number of tales

      for (i=0;i<10;i++)
        tales[i].Serialize(ar);
    }
    else
    {
      int tmp;
      ar >> tmp;
      
      ASSERT( tmp >= 10 );
      ASSERT( tmp <= MAX_TALES );
      
      if (tmp > MAX_TALES)
        tmp = MAX_TALES;

      for (i=0;i<tmp;i++)
        tales[i].Serialize(ar);
    }
  }
  
  for (i=0;i<MAX_DRINKS;i++)
    drinks[i].Serialize(ar);
}

TEMPLE::TEMPLE(void) :  
   GameEvent(), 
   donationChain(0),
   templeSpells(*(new spellBookType)),
   tcKey(-1)
{
  Clear(); 
  event=TempleEvent;
}

TEMPLE::TEMPLE(const TEMPLE &src) : 
    GameEvent(src), 
    donationChain(0), 
    templeSpells(*(new spellBookType)),
    tcKey(-1)
{ 
  *this=src; 
}

TEMPLE::~TEMPLE(void) 
{ 
  //Clear(); 
  delete (&templeSpells);
}

void TEMPLE::Clear()
{ 
  GameEvent::Clear();
  
  forceExit=TRUE;
  allowDonations=TRUE;
  costFactor=Normal;
  maxLevel=7;
  donationTrigger=0;
  donationChain=0;
  templeSpells.Clear();
  totalDonation=0; 
}

void TEMPLE::Serialize(CArchive &ar)
{
  GameEvent::Serialize(ar);

  if (ar.IsStoring())
  {
    ar << forceExit;
    ar << allowDonations;
    ar << (int) costFactor;
    ar << maxLevel;
    ar << donationTrigger;
    ar << donationChain;

    //ar << templeSpells;
    templeSpells.Serialize(ar, LoadingVersion);

    ar << totalDonation;
  }
  else
  {
    int temp;
    ar >> forceExit;
    ar >> allowDonations;
    ar >> temp;
    costFactor = (costFactorType)temp;
    ar >> maxLevel;
    ar >> donationTrigger;
    ar >> donationChain;

    //ar >> templeSpells;
    templeSpells.Serialize(ar, LoadingVersion);

    ar >> totalDonation;
  }
}
void TEMPLE::Serialize(CAR &ar)
{
  GameEvent::Serialize(ar);

  if (ar.IsStoring())
  {
    ar << forceExit;
    ar << allowDonations;
    ar << (int) costFactor;
    ar << maxLevel;
    ar << donationTrigger;
    ar << donationChain;

    //ar << templeSpells;
    templeSpells.Serialize(ar, LoadingVersion);

    ar << totalDonation;
  }
  else
  {
    int temp;
    ar >> forceExit;
    ar >> allowDonations;
    ar >> temp;
    costFactor = (costFactorType)temp;
    ar >> maxLevel;
    ar >> donationTrigger;
    ar >> donationChain;

    //ar >> templeSpells;
    templeSpells.Serialize(ar, LoadingVersion);

    ar >> totalDonation;
  }
}

void RANDOM_EVENT_DATA::Serialize(CArchive &ar)
{
  GameEvent::Serialize(ar);
  int i;
  if (ar.IsStoring())
  {
    for (i=1;i<MAX_RANDOM_EVENTS;i++)
    {
      ar << EventChains[i-1];
      ar << EventChance[i-1];
    }
  }
  else
  {
    for (i=1;i<MAX_RANDOM_EVENTS;i++)
    {
      ar >> EventChains[i-1];
      ar >> EventChance[i-1];
    }
  }
}

void RANDOM_EVENT_DATA::Serialize(CAR &ar)
{
  GameEvent::Serialize(ar);
  int i;
  if (ar.IsStoring())
  {
    for (i=1;i<MAX_RANDOM_EVENTS;i++)
    {
      ar << EventChains[i-1];
      ar << EventChance[i-1];
    }
  }
  else
  {
    for (i=1;i<MAX_RANDOM_EVENTS;i++)
    {
      ar >> EventChains[i-1];
      ar >> EventChance[i-1];
    }
  }
}

void SMALL_TOWN_DATA::Serialize(CArchive &ar)
{
  GameEvent::Serialize(ar);

  if (ar.IsStoring())
  {
    ar << Unused;
    ar << TempleChain;
    ar << TrainingHallChain;
    ar << ShopChain;
    ar << InnChain;
    ar << TavernChain;
    ar << VaultChain;

  }
  else
  {
    ar >> Unused;
    ar >> TempleChain;
    ar >> TrainingHallChain;
    ar >> ShopChain;
    ar >> InnChain;
    ar >> TavernChain;
    ar >> VaultChain;
  }
}
void SMALL_TOWN_DATA::Serialize(CAR &ar)
{
  GameEvent::Serialize(ar);

  if (ar.IsStoring())
  {
    ar << Unused;
    ar << TempleChain;
    ar << TrainingHallChain;
    ar << ShopChain;
    ar << InnChain;
    ar << TavernChain;
    ar << VaultChain;

  }
  else
  {
    ar >> Unused;
    ar >> TempleChain;
    ar >> TrainingHallChain;
    ar >> ShopChain;
    ar >> InnChain;
    ar >> TavernChain;
    ar >> VaultChain;
  }
}

BOOL JOURNAL_EVENT::EventShouldTrigger()
{
  if (!GameEvent::EventShouldTrigger())
    return FALSE;
  return TRUE;
}

void JOURNAL_EVENT::Serialize(CArchive &ar)
{
  GameEvent::Serialize(ar);

  if (ar.IsStoring())
    ar << journal_entry;
  else
    ar >> journal_entry;
}
void JOURNAL_EVENT::Serialize(CAR &ar)
{
  GameEvent::Serialize(ar);

  if (ar.IsStoring())
    ar << journal_entry;
  else
    ar >> journal_entry;
}


void CHAIN_EVENT::Serialize(CArchive &ar)
{
  GameEvent::Serialize(ar);

  if (ar.IsStoring())
    ar << Chain;
  else
    ar >> Chain;
}
void CHAIN_EVENT::Serialize(CAR &ar)
{
  GameEvent::Serialize(ar);

  if (ar.IsStoring())
    ar << Chain;
  else
    ar >> Chain;
}

void TRAININGHALL::Serialize(CArchive &ar)
{
  GameEvent::Serialize(ar);


  if (ar.IsStoring())
  {
 	  ar  << forceExit
        << TrainCleric
        << TrainFighter
        << TrainMagicUser
        << TrainThief
        << TrainRanger
        << TrainPaladin
        << TrainDruid
	      << Cost;
  }
  else
  {
 	  ar  >> forceExit
        >> TrainCleric
        >> TrainFighter
        >> TrainMagicUser
        >> TrainThief
        >> TrainRanger
        >> TrainPaladin
        >> TrainDruid
	      >> Cost;
  }
}
void TRAININGHALL::Serialize(CAR &ar)
{
  GameEvent::Serialize(ar);


  if (ar.IsStoring())
  {
 	  ar  << forceExit
        << TrainCleric
        << TrainFighter
        << TrainMagicUser
        << TrainThief
        << TrainRanger
        << TrainPaladin
        << TrainDruid
	      << Cost;
  }
  else
  {
 	  ar  >> forceExit
        >> TrainCleric
        >> TrainFighter
        >> TrainMagicUser
        >> TrainThief
        >> TrainRanger
        >> TrainPaladin
        >> TrainDruid
	      >> Cost;
  }
}


void SHOP::Serialize(CArchive &ar)
{
  GameEvent::Serialize(ar);

  // no need to store sellprice
  if (ar.IsStoring())
  {
    ar << forceExit
       << (int)costFactor
       << costToId
       << buybackPercentage;
    ar << canId;
    ar << canApprGems;
    ar << canApprJewels;
    ar << buyItemsSoldOnly;
  }
  else
  {
    int temp;
    ar >> forceExit
       >> temp;
    costFactor = (costFactorType)temp;
    if (LoadingVersion >= _VERSION_0696_)
    {
      ar >> costToId;
      ar >> buybackPercentage;
    }
    if (LoadingVersion >= _VERSION_0740_)
    {
      ar >> canId;
      ar >> canApprGems;
      ar >> canApprJewels;
    }

    if (LoadingVersion >= _VERSION_0910_)
      ar >> buyItemsSoldOnly;
  }

  itemsAvail.Serialize(ar,LoadingVersion);
}

void SHOP::Serialize(CAR &ar)
{
  GameEvent::Serialize(ar);

  // no need to store sellprice
  if (ar.IsStoring())
  {
    ar << forceExit
       << (int)costFactor
       << costToId
       << buybackPercentage;
    ar << canId;
    ar << canApprGems;
    ar << canApprJewels;
    ar << buyItemsSoldOnly;
  }
  else
  {
    int temp;
    ar >> forceExit
       >> temp;
    costFactor = (costFactorType)temp;
    if (LoadingVersion >= _VERSION_0696_)
    {
      ar >> costToId;
      ar >> buybackPercentage;
    }
    if (LoadingVersion >= _VERSION_0740_)
    {
      ar >> canId;
      ar >> canApprGems;
      ar >> canApprJewels;
    }

    if (LoadingVersion >= _VERSION_0910_)
      ar >> buyItemsSoldOnly;
  }

  itemsAvail.Serialize(ar,LoadingVersion);
}

void UTILITIES_EVENT_DATA::Clear() 
{
  GameEvent::Clear();
  
  items.Clear();
  endPlay=FALSE;operation=MO_NoOperation;itemCheck=MIC_None;mathItemType=0;resultItemType=0;
  mathAmount=0;mathItemIndex=-1;resultItemIndex=-1;
}
void UTILITIES_EVENT_DATA::Serialize(CArchive &ar)
{
  GameEvent::Serialize(ar);
  if (ar.IsStoring())
  {
    ar << endPlay;
    ar << (int)operation;
    ar << (int)itemCheck;
    ar << mathItemType;
    ar << resultItemType;
    ar << mathAmount;
    ar << mathItemIndex;
    ar << resultItemIndex;
  }
  else
  {
    int temp;
    ar >> endPlay;
    ar >> temp;
    operation = (MathOperationType)temp;
    ar >> temp;
    itemCheck = (MultiItemCheckType)temp;
    ar >> mathItemType;
    ar >> resultItemType;
    ar >> mathAmount;
    ar >> mathItemIndex;
    ar >> resultItemIndex;
  }
  items.Serialize(ar);
}
void UTILITIES_EVENT_DATA::Serialize(CAR &ar)
{
  GameEvent::Serialize(ar);
  if (ar.IsStoring())
  {
    ar << endPlay;
    ar << (int)operation;
    ar << (int)itemCheck;
    ar << mathItemType;
    ar << resultItemType;
    ar << mathAmount;
    ar << mathItemIndex;
    ar << resultItemIndex;
  }
  else
  {
    int temp;
    ar >> endPlay;
    ar >> temp;
    operation = (MathOperationType)temp;
    ar >> temp;
    itemCheck = (MultiItemCheckType)temp;
    ar >> mathItemType;
    ar >> resultItemType;
    ar >> mathAmount;
    ar >> mathItemIndex;
    ar >> resultItemIndex;
  }
  items.Serialize(ar);
}

BOOL NPC_SAYS_DATA::EventShouldTrigger()
{
  if (!GameEvent::EventShouldTrigger())
    return FALSE;

  BOOL shouldTrigger = party.isNPCinParty(partySeesChar);
  if (!shouldTrigger)
    WriteDebugString("NPC_SAYS event not triggerd, NPC not in party\n");
  return shouldTrigger;
}

BOOL ADD_NPC_DATA::EventShouldTrigger()
{
  if (!GameEvent::EventShouldTrigger())
    return FALSE;

  BOOL shouldTrigger = ((!party.isNPCinParty(charAdded)) && (party.CanAddNPC()));
  
  if (!shouldTrigger)
    WriteDebugString("ADD_NPC event not triggerd\n");
  return shouldTrigger;
}

BOOL REMOVE_NPC_DATA::EventShouldTrigger()
{
  if (!GameEvent::EventShouldTrigger())
    return FALSE;

  BOOL shouldTrigger = party.isNPCinParty(charRemoved);
  
  if (!shouldTrigger)
    WriteDebugString("REMOVE_NPC event not triggerd, NPC not in party\n");
  return shouldTrigger;
}

UTILITIES_EVENT_DATA& UTILITIES_EVENT_DATA::operator =(const UTILITIES_EVENT_DATA& src)
{
  //ASSERT(src.IsKindOf(RUNTIME_CLASS(UTILITIES_EVENT_DATA)));

  if (&src == this)
    return *this;

  GameEvent::operator =(src);
  const UTILITIES_EVENT_DATA &data = (const UTILITIES_EVENT_DATA&)(src);
  endPlay = data.endPlay;
  operation = data.operation;
  itemCheck = data.itemCheck;
  mathItemType = data.mathItemType;
  resultItemType = data.resultItemType;
  mathAmount = data.mathAmount;
  mathItemIndex = data.mathItemIndex;
  resultItemIndex = data.resultItemIndex;
  items = data.items;
  return *this;
}


PLAY_MOVIE_DATA& PLAY_MOVIE_DATA::operator =(const PLAY_MOVIE_DATA& src)
{
  if (&src == this) return *this;

  GameEvent::operator =(src);
  const PLAY_MOVIE_DATA &data = (const PLAY_MOVIE_DATA&)(src);
  m_filename = data.m_filename;
  m_mode = data.m_mode;
  return *this;
}

ADD_NPC_DATA& ADD_NPC_DATA::operator =(const ADD_NPC_DATA& src)
{
  //ASSERT(src.IsKindOf(RUNTIME_CLASS(ADD_NPC_DATA)));

  if (&src == this)
    return *this;

  GameEvent::operator =(src);
  const ADD_NPC_DATA &data = (const ADD_NPC_DATA&)(src);

  distance = data.distance;
  charAdded = data.charAdded;
  hitPointMod = data.hitPointMod;
  useOriginal=data.useOriginal;
  return *this;
}

REMOVE_NPC_DATA& REMOVE_NPC_DATA::operator =(const REMOVE_NPC_DATA& src)
{
  //ASSERT(src.IsKindOf(RUNTIME_CLASS(REMOVE_NPC_DATA)));

  if (&src == this)
    return *this;

  GameEvent::operator =(src);
  const REMOVE_NPC_DATA &data = (const REMOVE_NPC_DATA&)(src);

  distance = data.distance;
  charRemoved = data.charRemoved;
  return *this;
}

CAMP_EVENT_DATA& CAMP_EVENT_DATA::operator =(const CAMP_EVENT_DATA& src)
{
  //ASSERT(src.IsKindOf(RUNTIME_CLASS(CAMP_EVENT_DATA)));

  if (&src == this)
    return *this;

  GameEvent::operator =(src);
  const CAMP_EVENT_DATA &data = (const CAMP_EVENT_DATA&)(src);

  ForceExit = data.ForceExit;
  return *this;
}

COMBAT_EVENT_DATA& COMBAT_EVENT_DATA::operator =(const COMBAT_EVENT_DATA& src)
{
  //ASSERT(src.IsKindOf(RUNTIME_CLASS(COMBAT_EVENT_DATA)));

  if (&src == this)
    return *this;

  GameEvent::operator =(src);
  const COMBAT_EVENT_DATA &data = (const COMBAT_EVENT_DATA&)(src);

  m_UseOutdoorMap = data.m_UseOutdoorMap;
  distance = data.distance;
  direction = data.direction;
  surprise = data.surprise;
  autoApproach = data.autoApproach;
  outdoors = data.outdoors;
  noMonsterTreasure = data.noMonsterTreasure;
  partyNeverDies = data.partyNeverDies;
  partyNoExperience=data.partyNoExperience;
  noMagic = data.noMagic;
  monsterMorale = data.monsterMorale;
  turningMod = data.turningMod;
  monsters = data.monsters;
  DeathSound = data.DeathSound;
  MoveSound = data.MoveSound;
  TurnUndeadSound=data.TurnUndeadSound;
  randomMonster = data.randomMonster;
  TEMPsurprise=data.TEMPsurprise;
  bgSounds = data.bgSounds;
  return *this;
}

GUIDED_TOUR& GUIDED_TOUR::operator =(const GUIDED_TOUR& src)
{
  //ASSERT(src.IsKindOf(RUNTIME_CLASS(GUIDED_TOUR)));

  if (&src == this)
    return *this;

  GameEvent::operator =(src);
  const GUIDED_TOUR &data = (const GUIDED_TOUR&)(src);

  tour_x = data.tour_x;
  tour_y = data.tour_y;
  facing = data.facing;
  useStartLoc = data.useStartLoc;
  currStep = data.currStep;
  executeEvent = data.executeEvent;
  for (int i=0;i<MAX_TOUR_STEPS;i++)
    steps[i] = data.steps[i];
  return *this;
}

QUESTION_YES_NO& QUESTION_YES_NO::operator =(const QUESTION_YES_NO& src)
{
  //ASSERT(src.IsKindOf(RUNTIME_CLASS(QUESTION_YES_NO)));

  if (&src == this)
    return *this;

  GameEvent::operator =(src);
  const QUESTION_YES_NO &data = (const QUESTION_YES_NO&)(src);

  YesChainAction = data.YesChainAction;
  NoChainAction = data.NoChainAction;
  YesChain = data.YesChain;
  NoChain = data.NoChain;
  sx=data.sx;
  sy=data.sy;
  //Unused = data.Unused;
  return *this;
}

ENCOUNTER_DATA& ENCOUNTER_DATA::operator =(const ENCOUNTER_DATA& src)
{
  //ASSERT(src.IsKindOf(RUNTIME_CLASS(ENCOUNTER_DATA)));

  if (&src == this)
    return *this;

  GameEvent::operator =(src);
  const ENCOUNTER_DATA &data = (const ENCOUNTER_DATA&)(src);

  currDist = data.currDist;
  distance = data.distance;
  monsterSpeed = data.monsterSpeed;
  zeroRangeResult = data.zeroRangeResult;
  combatChain = data.combatChain;
  talkChain = data.talkChain;
  escapeChain = data.escapeChain;
  //Unused = data.Unused;
  buttons = data.buttons;
  return *this;
}

PASSWORD_DATA& PASSWORD_DATA::operator =(const PASSWORD_DATA& src)
{
  //ASSERT(src.IsKindOf(RUNTIME_CLASS(PASSWORD_DATA)));

  if (&src == this)
    return *this;

  GameEvent::operator =(src);
  const PASSWORD_DATA &data = (const PASSWORD_DATA&)(src);

  password = data.password;
  successAction = data.successAction;
  failAction = data.failAction;
  nbrTries = data.nbrTries;
  currTry = data.currTry;
  //Unused = data.Unused;
  successChain = data.successChain;
  failChain = data.failChain;
  successTransferData = data.successTransferData;
  failTransferData = data.failTransferData;
  matchCase = data.matchCase;
  matchCriteria = data.matchCriteria;
  return *this;
}

GAIN_EXP_DATA& GAIN_EXP_DATA::operator =(const GAIN_EXP_DATA& src)
{
  //ASSERT(src.IsKindOf(RUNTIME_CLASS(GAIN_EXP_DATA)));

  if (&src == this)
    return *this;

  GameEvent::operator =(src);
  const GAIN_EXP_DATA &data = (const GAIN_EXP_DATA&)(src);

  experience = data.experience;
  sound = data.sound;
  chance = data.chance;
  who = data.who;
  hSound = data.hSound;
  tempExp=data.tempExp;
  return *this;
}

GIVE_TREASURE_DATA& GIVE_TREASURE_DATA::operator =(const GIVE_TREASURE_DATA& src)
{
  if (&src == this)
    return *this;

  GameEvent::operator =(src);
  const GIVE_TREASURE_DATA &data = (const GIVE_TREASURE_DATA&)(src);

  SilentGiveToActiveChar=data.SilentGiveToActiveChar;
  money=data.money;
  items = data.items;
  moneyRemain=data.moneyRemain;
  itemsRemain=data.itemsRemain;
  m_detectSpellKey=data.m_detectSpellKey;
  return *this;
}

void GIVE_DAMAGE_DATA::Clear()
{ 
  GameEvent::Clear();
  
  nbrAttacks=1;
  chancePerAttack=100;
  dmgDice=6;
  dmgDiceQty=1;
  dmgBonus=0;
  attackTHACO=0;
  saveBonus=0; 
  eventSave=NoSave;
  spellSave=ParPoiDM;
  who=ActiveChar;
  distance=FarAway; 
}
 

GIVE_DAMAGE_DATA& GIVE_DAMAGE_DATA::operator =(const GIVE_DAMAGE_DATA& src)
{
  //ASSERT(src.IsKindOf(RUNTIME_CLASS(GIVE_DAMAGE_DATA)));
  if (&src == this)
    return *this;

  GameEvent::operator =(src);
  const GIVE_DAMAGE_DATA &data = (const GIVE_DAMAGE_DATA&)(src);

  nbrAttacks = data.nbrAttacks;
  chancePerAttack = data.chancePerAttack;
  dmgDice = data.dmgDice;
  dmgDiceQty = data.dmgDiceQty;
  dmgBonus = data.dmgBonus;
  saveBonus = data.saveBonus;
  attackTHACO = data.attackTHACO;
  eventSave = data.eventSave;
  spellSave = data.spellSave;
  who = data.who;
  distance = data.distance;
  return *this;
}

NPC_SAYS_DATA& NPC_SAYS_DATA::operator =(const NPC_SAYS_DATA& src)
{
  //ASSERT(src.IsKindOf(RUNTIME_CLASS(NPC_SAYS_DATA)));
  if (&src == this)
    return *this;

  GameEvent::operator =(src);
  const NPC_SAYS_DATA &data = (const NPC_SAYS_DATA&)(src);

  partySeesChar = data.partySeesChar;
  distance = data.distance;
  sound = data.sound;
  mustHitReturn = data.mustHitReturn;
  highlight = data.highlight;
  hSound = data.hSound;
  return *this;
}

QUESTION_LIST_DATA& QUESTION_LIST_DATA::operator =(const QUESTION_LIST_DATA& src)
{
  //ASSERT(src.IsKindOf(RUNTIME_CLASS(QUESTION_LIST_DATA)));
  if (&src == this)
    return *this;

  GameEvent::operator =(src);
  const QUESTION_LIST_DATA &data = (const QUESTION_LIST_DATA&)(src);

  UserResult = data.UserResult;
  buttons = data.buttons;
  sx=data.sx;
  sy=data.sy;
  return *this;
}

QUESTION_BUTTON_DATA& QUESTION_BUTTON_DATA::operator =(const QUESTION_BUTTON_DATA& src)
{
  //ASSERT(src.IsKindOf(RUNTIME_CLASS(QUESTION_BUTTON_DATA)));
  if (&src == this)
    return *this;

  GameEvent::operator =(src);
  const QUESTION_BUTTON_DATA &data = (const QUESTION_BUTTON_DATA&)(src);

  UserResult = data.UserResult;
  buttons = data.buttons;
  sx=data.sx;
  sy=data.sy;
  return *this;
}

TEXT_EVENT_DATA& TEXT_EVENT_DATA::operator =(const TEXT_EVENT_DATA& src)
{
  //ASSERT(src.IsKindOf(RUNTIME_CLASS(TEXT_EVENT_DATA)));
  if (&src == this)
    return *this;

  GameEvent::operator =(src);
  const TEXT_EVENT_DATA &data = (const TEXT_EVENT_DATA&)(src);

  WaitForReturn = data.WaitForReturn;
  HighlightText = data.HighlightText;
  ForceBackup = data.ForceBackup;
  KillSound = data.KillSound;
  sound = data.sound;
  hSound = data.hSound;
  distance = data.distance;
  return *this;
}

TRANSFER_EVENT_DATA& TRANSFER_EVENT_DATA::operator =(const TRANSFER_EVENT_DATA& src)
{
  //ASSERT(src.IsKindOf(RUNTIME_CLASS(TRANSFER_EVENT_DATA)));
  if (&src == this)
    return *this;

  GameEvent::operator =(src);
  const TRANSFER_EVENT_DATA &data = (const TRANSFER_EVENT_DATA&)(src);

  askYesNo = data.askYesNo;
  transferOnYes = data.transferOnYes;
  destroyDrow = data.destroyDrow;
  activateBeforeEntry = data.activateBeforeEntry;
  transferData = data.transferData;
  return *this;
}

WHO_PAYS_EVENT_DATA& WHO_PAYS_EVENT_DATA::operator =(const WHO_PAYS_EVENT_DATA& src)
{
  //ASSERT(src.IsKindOf(RUNTIME_CLASS(WHO_PAYS_EVENT_DATA)));
  if (&src == this)
    return *this;

  GameEvent::operator =(src);
  const WHO_PAYS_EVENT_DATA &data = (const WHO_PAYS_EVENT_DATA&)(src);

  impossible = data.impossible;
  successAction = data.successAction;
  failAction = data.failAction;
  gems = data.gems;
  jewels = data.jewels;
  platinum = data.platinum;
  //Unused = data.Unused;
  successChain = data.successChain;
  failChain = data.failChain;
  successTransferData = data.successTransferData;
  failTransferData = data.failTransferData;
  moneyType = data.moneyType;
  return *this;
}

WHO_TRIES_EVENT_DATA& WHO_TRIES_EVENT_DATA::operator =(const WHO_TRIES_EVENT_DATA& src)
{
  //ASSERT(src.IsKindOf(RUNTIME_CLASS(WHO_TRIES_EVENT_DATA)));
  if (&src == this)
    return *this;

  GameEvent::operator =(src);
  const WHO_TRIES_EVENT_DATA &data = (const WHO_TRIES_EVENT_DATA&)(src);

  alwaysSucceeds = data.alwaysSucceeds;
  alwaysFails = data.alwaysFails;
  checkSTR = data.checkSTR;
  checkINT = data.checkINT;
  checkWIS = data.checkWIS;
  checkDEX = data.checkDEX;
  checkCON = data.checkCON;
  checkCHA = data.checkCHA;
  checkPP = data.checkPP;
  checkOL = data.checkOL;
  checkFT = data.checkFT;
  checkMS = data.checkMS;
  checkHS = data.checkHS;
  checkHN = data.checkHN;
  checkCW = data.checkCW;
  checkRL = data.checkRL;
  strBonus = data.strBonus;
  compareToDie = data.compareToDie;
  successAction = data.successAction;
  failAction = data.failAction;
  compareDie = data.compareDie;
  NbrTries = data.NbrTries;
  currTry = data.currTry;
  //Unused = data.Unused;
  successChain = data.successChain;
  failChain = data.failChain;
  successTransferData = data.successTransferData;
  failTransferData = data.failTransferData;
  return *this;
}

SPECIAL_ITEM_KEY_EVENT_DATA& SPECIAL_ITEM_KEY_EVENT_DATA::operator =(const SPECIAL_ITEM_KEY_EVENT_DATA& src)
{
  //ASSERT(src.IsKindOf(RUNTIME_CLASS(SPECIAL_ITEM_KEY_EVENT_DATA)));
  if (&src == this)
    return *this;

  GameEvent::operator =(src);
  const SPECIAL_ITEM_KEY_EVENT_DATA &data = (const SPECIAL_ITEM_KEY_EVENT_DATA&)(src);
  items = data.items;
  forceExit=data.forceExit;
  WaitForReturn=data.WaitForReturn;
  return *this;
}

PASS_TIME_EVENT_DATA& PASS_TIME_EVENT_DATA::operator =(const PASS_TIME_EVENT_DATA& src)
{
  //ASSERT(src.IsKindOf(RUNTIME_CLASS(PASS_TIME_EVENT_DATA)));
  if (&src == this)
    return *this;

  GameEvent::operator =(src);
  const PASS_TIME_EVENT_DATA &data = (const PASS_TIME_EVENT_DATA&)(src);

  Days = data.Days;
  Hours = data.Hours;
  Minutes = data.Minutes;
  AllowStop=data.AllowStop;
  SetTime=data.SetTime;
  PassSilent=data.PassSilent;
  //daysLeft = data.daysLeft;
  //hoursLeft = data.hoursLeft;
  //minutesLeft = data.minutesLeft;
  return *this;
}

VAULT_EVENT_DATA& VAULT_EVENT_DATA::operator =(const VAULT_EVENT_DATA& src)
{
  //ASSERT(src.IsKindOf(RUNTIME_CLASS(VAULT_EVENT_DATA)));
  if (&src == this)
    return *this;

  GameEvent::operator =(src);
  const VAULT_EVENT_DATA &data = (const VAULT_EVENT_DATA&)(src);

  ForceBackup = data.ForceBackup;
  WhichVault = data.WhichVault;

  return *this;
}

QUEST_EVENT_DATA& QUEST_EVENT_DATA::operator =(const QUEST_EVENT_DATA& src)
{
  //ASSERT(src.IsKindOf(RUNTIME_CLASS(QUEST_EVENT_DATA)));
  if (&src == this)
    return *this;

  GameEvent::operator =(src);
  const QUEST_EVENT_DATA &data = (const QUEST_EVENT_DATA&)(src);
  
  accept = data.accept;
  completeOnAccept = data.completeOnAccept;
  failOnRejection = data.failOnRejection;
  quest = data.quest;
  stage = data.stage;
  acceptChain = data.acceptChain;
  rejectChain = data.rejectChain;
  //Unused = data.Unused;
  return *this;
}

TAVERN& TAVERN::operator =(const TAVERN& src)
{
  //ASSERT(src.IsKindOf(RUNTIME_CLASS(TAVERN)));
  if (&src == this)
    return *this;

  GameEvent::operator =(src);
  const TAVERN &data = (const TAVERN&)(src);

  forceExit = data.forceExit;
  allowFights = data.allowFights;
  allowDrinks = data.allowDrinks;
  inflation = data.inflation;
  barkeep = data.barkeep;
  fightChain = data.fightChain;
  drinkChain = data.drinkChain;
  drinkPointTrigger = data.drinkPointTrigger;
  //Unused = data.Unused;
  taleOrder = data.taleOrder;
  EachTaleOnceOnly=data.EachTaleOnceOnly;
  int i;
  for (i=0;i<MAX_TALES;i++)
    tales[i] = data.tales[i];
  for (i=0;i<MAX_DRINKS;i++)
    drinks[i] = data.drinks[i];
  return *this;
}

TEMPLE& TEMPLE::operator =(const TEMPLE& src)
{
  //ASSERT(src.IsKindOf(RUNTIME_CLASS(TEMPLE)));
  if (&src == this)
    return *this;

  GameEvent::operator =(src);
  const TEMPLE &data = (const TEMPLE&)(src);

  forceExit = data.forceExit;
  allowDonations = data.allowDonations;
  totalDonation = data.totalDonation;
  costFactor = data.costFactor;
  maxLevel = data.maxLevel;
  donationTrigger = data.donationTrigger;
  donationChain = data.donationChain;
  templeSpells = data.templeSpells;
  tcKey=data.tcKey;
  //Unused = data.Unused;
  return *this;
}

RANDOM_EVENT_DATA& RANDOM_EVENT_DATA::operator =(const RANDOM_EVENT_DATA& src)
{
  if (&src==this) return *this;
  GameEvent::operator =(src);
  const RANDOM_EVENT_DATA &data = (const RANDOM_EVENT_DATA&)(src);
  for (int i=1;i<MAX_RANDOM_EVENTS;i++)
  {
    EventChains[i-1] = data.EventChains[i-1];
    EventChance[i-1] = data.EventChance[i-1];
  }
  return *this;
}

SMALL_TOWN_DATA& SMALL_TOWN_DATA::operator =(const SMALL_TOWN_DATA& src)
{
  //ASSERT(src.IsKindOf(RUNTIME_CLASS(SMALL_TOWN_DATA)));
  if (&src == this)
    return *this;

  GameEvent::operator =(src);
  const SMALL_TOWN_DATA &data = (const SMALL_TOWN_DATA&)(src);

  //Unused = data.Unused;
  TempleChain = data.TempleChain;
  TrainingHallChain = data.TrainingHallChain;
  ShopChain = data.ShopChain;
  InnChain = data.InnChain;
  TavernChain = data.TavernChain;
  VaultChain = data.VaultChain;  
  return *this;
}


JOURNAL_EVENT& JOURNAL_EVENT::operator =(const JOURNAL_EVENT& src)
{
  if (&src == this) return *this;
  GameEvent::operator =(src);
  const JOURNAL_EVENT &data = (const JOURNAL_EVENT&)(src);
  journal_entry = data.journal_entry;
  return *this;
}

CHAIN_EVENT& CHAIN_EVENT::operator =(const CHAIN_EVENT& src)
{
  //ASSERT(src.IsKindOf(RUNTIME_CLASS(CHAIN_EVENT)));
  if (&src == this)
    return *this;

  GameEvent::operator =(src);
  const CHAIN_EVENT &data = (const CHAIN_EVENT&)(src);

  Chain = data.Chain;
  return *this;
}

TRAININGHALL& TRAININGHALL::operator =(const TRAININGHALL& src)
{
  //ASSERT(src.IsKindOf(RUNTIME_CLASS(TRAININGHALL)));
  if (&src == this)
    return *this;

  GameEvent::operator =(src);
  const TRAININGHALL &data = (const TRAININGHALL&)(src);

  forceExit = data.forceExit;
  TrainCleric = data.TrainCleric;
  TrainFighter = data.TrainFighter;
  TrainMagicUser = data.TrainMagicUser;
  TrainThief = data.TrainThief;
  TrainRanger = data.TrainRanger;
  TrainPaladin = data.TrainPaladin;
  TrainDruid = data.TrainDruid;
  Cost = data.Cost;
  return *this;
}

SHOP& SHOP::operator =(const SHOP& src)
{
  //ASSERT(src.IsKindOf(RUNTIME_CLASS(SHOP)));
  if (&src == this)
    return *this;

  GameEvent::operator =(src);
  const SHOP &data = (const SHOP&)(src);

  forceExit = data.forceExit;
  costFactor = data.costFactor;
  itemsAvail = data.itemsAvail;
  costToId=data.costToId;
  buybackPercentage = data.buybackPercentage;
  canId=data.canId;
  canApprGems=data.canApprGems;
  canApprJewels=data.canApprJewels;
  sellprice=data.sellprice;
  buyItemsSoldOnly=data.buyItemsSoldOnly;

  return *this;
}


void SOUND_EVENT::SaveEventSounds(int level) 
{ 
  GameEvent::SaveEventSounds(level);
  POSITION pos = sounds.GetHeadPosition();
  while (pos != NULL)
    saveSound(sounds.GetNext(pos), level);
};

void SOUND_EVENT::Serialize(CArchive &ar)
{
  GameEvent::Serialize(ar);
  int count;
  CString temp;

  if (ar.IsStoring())
  {      
    count = sounds.GetCount();      
    ar << count;
    
    POSITION pos = sounds.GetHeadPosition();
    while (pos != NULL)
    {
      temp = sounds.GetAt(pos);
      StripFilenamePath(temp);
      AS(ar,temp);
      AddDesignFolderToPath(temp);
      sounds.SetAt(pos, temp);
      sounds.GetNext(pos);

//      if (temp == "")
//      {
//        temp = "*";
//        ar << temp;
//      }
//      else
//        ar << temp;        
    }
  }
  else
  {
    ar >> count;
    for (int i=0;i<count;i++)
    {
      DAS(ar,temp);
//      ar >> temp;
//      if (temp == "*")
//        temp = "";
      AddDesignFolderToPath(temp);
      sounds.AddTail(temp);
    }
  }
}
void SOUND_EVENT::Serialize(CAR &ar)
{
  GameEvent::Serialize(ar);
  int count;
  CString temp;

  if (ar.IsStoring())
  {      
    count = sounds.GetCount();      
    ar << count;
    
    POSITION pos = sounds.GetHeadPosition();
    while (pos != NULL)
    {
      temp = sounds.GetAt(pos);
      StripFilenamePath(temp);
      AS(ar,temp);
      AddDesignFolderToPath(temp);
      sounds.SetAt(pos, temp);
      sounds.GetNext(pos);

//      if (temp == "")
//      {
//        temp = "*";
//        ar << temp;
//      }
//      else
//        ar << temp;        
    }
  }
  else
  {
    ar >> count;
    for (int i=0;i<count;i++)
    {
      DAS(ar,temp);
//      ar >> temp;
//      if (temp == "*")
//        temp = "";
      AddDesignFolderToPath(temp);
      sounds.AddTail(temp);
    }
  }
}

SOUND_EVENT& SOUND_EVENT::operator =(const SOUND_EVENT& src)
{
  if (&src == this)
    return *this;
  //ASSERT(src.IsKindOf(RUNTIME_CLASS(SOUND_EVENT)));

  GameEvent::operator =(src);
  const SOUND_EVENT &data = (const SOUND_EVENT&)(src);

  sounds.RemoveAll();
  POSITION pos = data.sounds.GetHeadPosition();

  CString temp;
  while (pos != NULL)
  {
    temp = data.sounds.GetNext(pos);
    sounds.AddTail(temp);
  }
  return *this;
}

#if (defined UAFEDITOR)

void QUEST_EVENT_DATA::ChainToEventId(int num, DWORD cid)
{
  if (cid != NO_EVENT)
  {
    ASSERT( CheckValidEvent(this, cid) );
  }
  
  switch (num)
  {
  case 0:
    GameEvent::ChainToEventId(num, cid);
    break;
  case 1:
    acceptChain = cid;
    break;
  case 2:
    rejectChain = cid;
    break;
  }
}

int QUEST_EVENT_DATA::UnchainToEventId(int num, DWORD eid)
{ // Called when an event is deleted.  We had best not point to it anymore.
  int result=0;
  if (num<1) result+=GameEvent::UnchainToEventId(num, eid);
  if (acceptChain == eid) {acceptChain=0;result++;};
  if (rejectChain == eid) {rejectChain=0;result++;};
  return result;
}



BOOL QUEST_EVENT_DATA::GetEVChainText(int num, char *ptext)
{
  if (num == 0)
    return GameEvent::GetEVChainText(num, ptext);

  if (num == 1)
  {
    sprintf(ptext, 
            "%s: %s", 
            "Accept Chain",
            GetEventIdDescription(acceptChain,GetEventSource()));
    return TRUE;
  }
  else if (num == 2)
  {
    sprintf(ptext, 
            "%s: %s", 
            "Reject Chain",
            GetEventIdDescription(rejectChain,GetEventSource()));
    return TRUE;
  }

  return FALSE;
}

BOOL QUEST_EVENT_DATA::GetEVChain(int num, DWORD &cid)
{
  if (num == 0)
    return GameEvent::GetEVChain(num, cid);

  if (num == 1)
  {
    cid=acceptChain;
    return TRUE;
  }
  else if (num == 2)
  {
    cid=rejectChain;
    return TRUE;
  }

  return FALSE;
}

#endif


#if (defined UAFEDITOR)

void TAVERN::ChainToEventId(int num, DWORD cid)
{
  if (cid != NO_EVENT)
  {
    ASSERT( CheckValidEvent(this, cid) );
  }

  switch (num)
  {
  case 0:
    GameEvent::ChainToEventId(num, cid);
    break;
  case 1:
    fightChain = cid;
    break;
  case 2:
    drinkChain = cid;
    break;
  }
}

int TAVERN::UnchainToEventId(int num, DWORD eid)
{ // Called when an event is deleted.  We had best not point to it anymore.
  int result=0;
  if (num<1) result+=GameEvent::UnchainToEventId(num, eid);
  if (fightChain == eid) {fightChain=0;result++;};
  if (drinkChain == eid) {drinkChain=0;result++;};
  return result;
}



BOOL TAVERN::GetEVChainText(int num, char *ptext)
{
  if (num == 0)
    return GameEvent::GetEVChainText(num, ptext);

  if (num == 1)
  {
    sprintf(ptext, 
            "%s: %s", 
            "Fight Chain",
            GetEventIdDescription(fightChain,GetEventSource()));
    return TRUE;
  }
  else if (num == 2)
  {
    sprintf(ptext, 
            "%s: %s", 
            "Drunk Chain",
            GetEventIdDescription(drinkChain,GetEventSource()));
    return TRUE;
  }

  return FALSE;
}

BOOL TAVERN::GetEVChain(int num, DWORD &cid)
{
  if (num == 0)
    return GameEvent::GetEVChain(num, cid);

  if (num == 1)
  {
    cid=fightChain;
    return TRUE;
  }
  else if (num == 2)
  {
    cid=drinkChain;
    return TRUE;
  }

  return FALSE;
}

#endif
void TEMPLE::GetChainCount(int &curr, int &total)
{
  curr=0;
  total=1;  
  if (donationChain > 0)
    curr++;
}

BOOL TEMPLE::IsChained()
{
  if (GameEvent::IsChained())
    return TRUE;

  if (donationChain > 0)
    return TRUE;

  return FALSE;
}


#if (defined UAFEDITOR)

void TEMPLE::ChainToEventId(int num, DWORD cid)
{
  if (cid != NO_EVENT)
  {
    ASSERT( CheckValidEvent(this, cid) );
  }
  
  switch (num)
  {
  case 0:
    GameEvent::ChainToEventId(num, cid);
    break;
  case 1:
    donationChain = cid;
    break;
  }
}

int TEMPLE::UnchainToEventId(int num, DWORD eid)
{ // Called when an event is deleted.  We had best not point to it anymore.
  int result=0;
  if (num<1) result+=GameEvent::UnchainToEventId(num, eid);
  if (donationChain == eid) {donationChain=0;result++;};
  return result;
}



BOOL TEMPLE::GetEVChainText(int num, char *ptext)
{
  if (num == 0)
    return GameEvent::GetEVChainText(num, ptext);

  if (num == 1)
  {
    sprintf(ptext, 
            "%s: %s", 
            "Donate Chain",
            GetEventIdDescription(donationChain,GetEventSource()));
    return TRUE;
  }

  return FALSE;
}

BOOL TEMPLE::GetEVChain(int num, DWORD &cid)
{
  if (num == 0)
    return GameEvent::GetEVChain(num, cid);

  if (num == 1)
  {
    cid=donationChain;
    return TRUE;
  }

  return FALSE;
}
#endif

void RANDOM_EVENT_DATA::GetChainCount(int &curr, int &total)
{
  curr=0;
  total=MAX_RANDOM_EVENTS-1;
  for (int i=1;i<MAX_RANDOM_EVENTS;i++)
  {
    if (EventChains[i-1] > 0)
      curr++;
  }
}
BOOL RANDOM_EVENT_DATA::IsChained()
{
  if (GameEvent::IsChained()) return TRUE;
  for (int i=1;i<MAX_RANDOM_EVENTS;i++)
  {
    if (EventChains[i-1] > 0)
      return TRUE;
  }
  return FALSE;
}
#if (defined UAFEDITOR)

void RANDOM_EVENT_DATA::ChainToEventId(int num, DWORD cid)
{
  if (cid != NO_EVENT)
  {
    ASSERT( CheckValidEvent(this, cid) );
  }
  
  switch (num)
  {
  case 0:
    GameEvent::ChainToEventId(num, cid);
    break;
  default:
    EventChains[num-1]=cid;
    break;
  }
}

int RANDOM_EVENT_DATA::UnchainToEventId(int num, DWORD eid)
{ 
  // Called when an event is deleted.  We had best not point to it anymore.
  int result=0;
  if (num<1) result+=GameEvent::UnchainToEventId(num, eid);
  for (int i=1;i<MAX_RANDOM_EVENTS;i++)
  {
    if (EventChains[i-1] == eid) 
    {
      EventChains[i-1]=0;  
      result++;
    };
  }
  return result;
}

BOOL RANDOM_EVENT_DATA::GetEVChainText(int num, char *ptext)
{
  if (num == 0)
    return GameEvent::GetEVChainText(num, ptext);

  if ((num>0) && (num < MAX_RANDOM_EVENTS))
  {
    sprintf(ptext, 
            "%s %i: %s", 
            "Random Chain",
            num,
            GetEventIdDescription(EventChains[num-1],GetEventSource()));
    return TRUE;
  }

  return FALSE;
}

BOOL RANDOM_EVENT_DATA::GetEVChain(int num, DWORD &cid)
{
  if (num == 0)
    return GameEvent::GetEVChain(num, cid);

  if ((num>0) && (num < MAX_RANDOM_EVENTS))
  {
    cid = EventChains[num-1];
    return TRUE;
  }
  return FALSE;
}
#endif

void SMALL_TOWN_DATA::GetChainCount(int &curr, int &total)
{
  curr=0;
  total=6;  
  if (TempleChain > 0)
    curr++;
  if (TrainingHallChain > 0)
    curr++;
  if (ShopChain > 0)
    curr++;
  if (InnChain > 0)
    curr++;
  if (TavernChain > 0)
    curr++;
  if (VaultChain > 0)
    curr++;
}

BOOL SMALL_TOWN_DATA::IsChained()
{
  if (GameEvent::IsChained())
    return TRUE;

  if (TempleChain > 0)
    return TRUE;
  if (TrainingHallChain > 0)
    return TRUE;
  if (ShopChain > 0)
    return TRUE;
  if (InnChain > 0)
    return TRUE;
  if (TavernChain > 0)
    return TRUE;
  if (VaultChain > 0)
    return TRUE;

  return FALSE;
}


#if (defined UAFEDITOR)

void SMALL_TOWN_DATA::ChainToEventId(int num, DWORD cid)
{
  if (cid != NO_EVENT)
  {
    ASSERT( CheckValidEvent(this, cid) );
  }
  
  switch (num)
  {
  case 0:
    GameEvent::ChainToEventId(num, cid);
    break;
  case 1:
    TempleChain = cid;
    break;
  case 2:
    TrainingHallChain = cid;
    break;
  case 3:
    ShopChain = cid;
    break;
  case 4:
    InnChain = cid;
    break;
  case 5:
    TavernChain = cid;
    break;
  case 6:
    VaultChain = cid;
    break;
  }
}

int SMALL_TOWN_DATA::UnchainToEventId(int num, DWORD eid)
{ // Called when an event is deleted.  We had best not point to it anymore.
  int result=0;
  if (num<1) result+=GameEvent::UnchainToEventId(num, eid);
  if (TempleChain == eid)       {TempleChain=0;      result++;};
  if (TrainingHallChain == eid) {TrainingHallChain=0;result++;};
  if (ShopChain == eid)         {ShopChain=0;        result++;};
  if (InnChain == eid)          {InnChain=0;         result++;};
  if (TavernChain == eid)       {TavernChain=0;      result++;};
  if (VaultChain == eid)        {VaultChain=0;       result++;};
  return result;
}



BOOL SMALL_TOWN_DATA::GetEVChainText(int num, char *ptext)
{
  if (num == 0)
    return GameEvent::GetEVChainText(num, ptext);

  switch (num)
  {
  case 1:
    sprintf(ptext, 
            "%s: %s", 
            "Temple Chain",
            GetEventIdDescription(TempleChain,GetEventSource()));
    return TRUE;
    break;
  case 2:
    sprintf(ptext, 
            "%s: %s", 
            "Training Hall Chain",
            GetEventIdDescription(TrainingHallChain,GetEventSource()));
    return TRUE;
    break;  
  case 3:
    sprintf(ptext, 
            "%s: %s", 
            "Shop Chain",
            GetEventIdDescription(ShopChain,GetEventSource()));
    return TRUE;
    break;  
  case 4:
    sprintf(ptext, 
            "%s: %s", 
            "Inn Chain",
            GetEventIdDescription(InnChain,GetEventSource()));
    return TRUE;
    break;  
  case 5:
    sprintf(ptext, 
            "%s: %s", 
            "Tavern Chain",
            GetEventIdDescription(TavernChain,GetEventSource()));
    return TRUE;
    break;  
  case 6:
    sprintf(ptext, 
            "%s: %s", 
            "Vault Chain",
            GetEventIdDescription(VaultChain,GetEventSource()));
    return TRUE;
    break;
  }
  return FALSE;
}

BOOL SMALL_TOWN_DATA::GetEVChain(int num, DWORD &cid)
{
  if (num == 0)
    return GameEvent::GetEVChain(num, cid);

  switch (num)
  {
  case 1:
    cid=TempleChain;
    return TRUE;
    break;
  case 2:
    cid=TrainingHallChain;
    return TRUE;
    break;
  case 3:
    cid=ShopChain;
    return TRUE;
    break;
  case 4:
    cid=InnChain;
    return TRUE;
    break;
  case 5:
    cid=TavernChain;
    return TRUE;
    break;
  case 6:
    cid=VaultChain;
    return TRUE;
    break;
  }
  return FALSE;
}
#endif

void CHAIN_EVENT::GetChainCount(int &curr, int &total)
{
  curr=0;
  total=1;  
  if (Chain > 0)
    curr++;
}

BOOL CHAIN_EVENT::IsChained()
{
  if (GameEvent::IsChained())
    return TRUE;

  if (Chain > 0)
    return TRUE;

  return FALSE;
}

#if (defined UAFEDITOR)

void CHAIN_EVENT::ChainToEventId(int num, DWORD cid)
{
  if (cid != NO_EVENT)
  {
    ASSERT( CheckValidEvent(this, cid) );
  }
  
  switch (num)
  {
  case 0:
    GameEvent::ChainToEventId(num, cid);
    break;
  case 1:
    Chain = cid;
    break;
  }
}

int CHAIN_EVENT::UnchainToEventId(int num, DWORD eid)
{ // Called when an event is deleted.  We had best not point to it anymore.
  int result=0;
  if (num<1) result+=GameEvent::UnchainToEventId(num, eid);
  if (Chain == eid) {Chain=0;result++;};
  return result;
}



BOOL CHAIN_EVENT::GetEVChainText(int num, char *ptext)
{
  if (num == 0)
    return GameEvent::GetEVChainText(num, ptext);

  switch (num)
  {
  case 1:
    sprintf(ptext, 
            "%s: %s", 
            "Chained Event",
            GetEventIdDescription(Chain,GetEventSource()));
    return TRUE;
    break;
  }

  return FALSE;
}

BOOL CHAIN_EVENT::GetEVChain(int num, DWORD &cid)
{
  if (num == 0)
    return GameEvent::GetEVChain(num, cid);

  switch (num)
  {
  case 1:
    cid=Chain;
    return TRUE;
    break;
  }
  return FALSE;
}
#endif

void QUEST_EVENT_DATA::GetChainCount(int &curr, int &total)
{
  curr=0;
  total=2;  
  if (acceptChain > 0)
    curr++;
  if (rejectChain > 0)
    curr++;
}
void TAVERN::GetChainCount(int &curr, int &total)
{
  curr=0;
  total=2;  
  if (fightChain > 0)
    curr++;
  if (drinkChain > 0)
    curr++;
}


#if (defined UAFEDITOR)

void ENCOUNTER_DATA::ChainToEventId(int num, DWORD cid)
{
  if (cid != NO_EVENT)
  {
    ASSERT( CheckValidEvent(this, cid) );
  }
  
  if ((num < 0) || (num > MAX_BUTTONS+3))
    return;

  if (num == 0)
  {
    GameEvent::ChainToEventId(num, cid);
    return;
  }

  if (num <= MAX_BUTTONS)
    buttons.buttons[num-1].chain = cid;

  if (num==MAX_BUTTONS+1)
    combatChain = cid;
  if (num==MAX_BUTTONS+2)
    talkChain = cid;
  if (num==MAX_BUTTONS+3)
    escapeChain = cid;
}

int ENCOUNTER_DATA::UnchainToEventId(int num, DWORD eid)
{ // Called when an event is deleted.  We had best not point to it anymore.
  int result=0;
  if (num<1) result+=GameEvent::UnchainToEventId(num, eid);
  for (int i=0; i<MAX_BUTTONS; i++)
    if (buttons.buttons[i].chain==eid) 
    {
      buttons.buttons[i].chain=0;
      result++;
    };
  if (combatChain==eid) {result++;combatChain=0;};
  if (talkChain==eid)   {result++;talkChain=0;};
  if (escapeChain==eid) {result++;escapeChain=0;};
  return result;
}


BOOL ENCOUNTER_DATA::GetEVChainText(int num, char *ptext)
{
  if ((num < 0) || (num > MAX_BUTTONS+3))
    return FALSE;

  if (num == 0)
    return GameEvent::GetEVChainText(num, ptext);
  if (num <= MAX_BUTTONS)
  {
    sprintf(ptext, 
            "Button %i Chain: %s", 
            num,
            GetEventIdDescription(buttons.buttons[num-1].chain,GetEventSource()));
    return TRUE;
  };
  if (num==MAX_BUTTONS+1)
  {
    sprintf(ptext,"Combat Chain: %s",
            GetEventIdDescription(combatChain,GetEventSource()));
    return TRUE;
  };
  if (num==MAX_BUTTONS+2)
  {
    sprintf(ptext,"Talk Chain: %s",
            GetEventIdDescription(talkChain,GetEventSource()));
    return TRUE;
  };
  if (num==MAX_BUTTONS+3)
  {
    sprintf(ptext,"Escape Chain: %s",
            GetEventIdDescription(escapeChain,GetEventSource()));
    return TRUE;
  };
  return FALSE;
}

BOOL ENCOUNTER_DATA::GetEVChain(int num, DWORD &cid)
{
  if ((num < 0) || (num > MAX_BUTTONS+3))
    return FALSE;

  if (num == 0)
    return GameEvent::GetEVChain(num, cid);
  if (num<=MAX_BUTTONS)
  {
    cid = buttons.buttons[num-1].chain;
    return TRUE;
  };
  if (num==MAX_BUTTONS+1)
  {
    cid=combatChain;
    return TRUE;
  };
  if (num==MAX_BUTTONS+2)
  {
    cid=talkChain;
    return TRUE;
  };
  if (num==MAX_BUTTONS+3)
  {
    cid=escapeChain;
    return TRUE;
  };
  return FALSE;
}

#endif

void ENCOUNTER_DATA::GetChainCount(int &curr, int &total)
{
  curr=0;
  total=MAX_BUTTONS+3;

  for (int i=0;i<MAX_BUTTONS;i++)
  {
    if (buttons.buttons[i].chain > 0)
      curr++;
  }
  if (combatChain > 0) curr++;
  if (talkChain > 0) curr++;
  if (escapeChain > 0) curr++;
}

#if (defined UAFEDITOR)

void PASSWORD_DATA::ChainToEventId(int num, DWORD cid)
{
  if (cid != NO_EVENT)
  {
    ASSERT( CheckValidEvent(this, cid) );
  }
  
  switch (num)
  {
  case 0:
    GameEvent::ChainToEventId(num, cid);
    break;
  case 1:
    successChain = cid;
    break;
  case 2:
    failChain = cid;
    break;
  }
}


int PASSWORD_DATA::UnchainToEventId(int num, DWORD eid)
{ // Called when an event is deleted.  We had best not point to it anymore.
  int result=0;
  if (num<1) result+=GameEvent::UnchainToEventId(num, eid);
  if (successChain == eid) {successChain=0;result++;};
  if (failChain == eid)    {failChain=0;   result++;};
  return result;
}


BOOL PASSWORD_DATA::GetEVChainText(int num, char *ptext)
{
  if (num == 0)
    return GameEvent::GetEVChainText(num, ptext);

  if (num == 1)
  {
    sprintf(ptext, 
            "%s: %s", 
            "Pswd Success Chain",
            GetEventIdDescription(successChain,GetEventSource()));
    return TRUE;
  }
  else if (num == 2)
  {
    sprintf(ptext, 
            "%s: %s", 
            "Pswd Fail Chain",
            GetEventIdDescription(failChain,GetEventSource()));
    return TRUE;
  }

  return FALSE;
}

BOOL PASSWORD_DATA::GetEVChain(int num, DWORD &cid)
{
  if (num == 0)
    return GameEvent::GetEVChain(num, cid);

  if (num == 1)
  {
    cid=successChain;
    return TRUE;
  }
  else if (num == 2)
  {
    cid=failChain;
    return TRUE;
  }

  return FALSE;
}

#endif

void PASSWORD_DATA::GetChainCount(int &curr, int &total)
{
  curr=0;
  total=2;

  if (successChain > 0)
    curr++;
  if (failChain > 0)
    curr++;
}

#if (defined UAFEDITOR)

void WHO_PAYS_EVENT_DATA::ChainToEventId(int num, DWORD cid)
{
  if (cid != NO_EVENT)
  {
    ASSERT( CheckValidEvent(this, cid) );
  }
  
  switch (num)
  {
  case 0:
    GameEvent::ChainToEventId(num, cid);
    break;
  case 1:
    successChain = cid;
    break;
  case 2:
    failChain = cid;
    break;
  }
}

int WHO_PAYS_EVENT_DATA::UnchainToEventId(int num, DWORD eid)
{ // Called when an event is deleted.  We had best not point to it anymore.
  int result=0;
  if (num<1) result+=GameEvent::UnchainToEventId(num, eid);
  if (successChain == eid) {successChain=0;result++;};
  if (failChain == eid)    {failChain=0;   result++;};
  return result;
}



BOOL WHO_PAYS_EVENT_DATA::GetEVChainText(int num, char *ptext)
{
  if (num == 0)
    return GameEvent::GetEVChainText(num, ptext);

  if (num == 1)
  {
    sprintf(ptext, 
            "%s: %s", 
            "Pay Success Chain",
            GetEventIdDescription(successChain,GetEventSource()));
    return TRUE;
  }
  else if (num == 2)
  {
    sprintf(ptext, 
            "%s: %s", 
            "Pay Fail Chain",
            GetEventIdDescription(failChain,GetEventSource()));
    return TRUE;
  }

  return FALSE;
}

BOOL WHO_PAYS_EVENT_DATA::GetEVChain(int num, DWORD &cid)
{
  if (num == 0)
    return GameEvent::GetEVChain(num, cid);

  if (num == 1)
  {
    cid=successChain;
    return TRUE;
  }
  else if (num == 2)
  {
    cid=failChain;
    return TRUE;
  }

  return FALSE;
}

#endif

void WHO_PAYS_EVENT_DATA::GetChainCount(int &curr, int &total)
{
  curr=0;
  total=2;

  if (successChain > 0)
    curr++;
  if (failChain > 0)
    curr++;
}

#if (defined UAFEDITOR)

void WHO_TRIES_EVENT_DATA::ChainToEventId(int num, DWORD cid)
{
  if (cid != NO_EVENT)
  {
    ASSERT( CheckValidEvent(this, cid) );
  }
  
  switch (num)
  {
  case 0:
    GameEvent::ChainToEventId(num, cid);
    break;
  case 1:
    successChain = cid;
    break;
  case 2:
    failChain = cid;
    break;
  }
}

int WHO_TRIES_EVENT_DATA::UnchainToEventId(int num, DWORD eid)
{ // Called when an event is deleted.  We had best not point to it anymore.
  int result=0;
  if (num<1) result+=GameEvent::UnchainToEventId(num, eid);
  if (successChain == eid) {successChain=0;result++;};
  if (failChain == eid)    {failChain=0;   result++;};
  return result;
}



BOOL WHO_TRIES_EVENT_DATA::GetEVChainText(int num, char *ptext)
{
  if (num == 0)
    return GameEvent::GetEVChainText(num, ptext);

  if (num == 1)
  {
    sprintf(ptext, 
            "%s: %s", 
            "Try Success Chain",
            GetEventIdDescription(successChain,GetEventSource()));
    return TRUE;
  }
  else if (num == 2)
  {
    sprintf(ptext, 
            "%s: %s", 
            "Try Fail Chain",
            GetEventIdDescription(failChain,GetEventSource()));
    return TRUE;
  }

  return FALSE;
}


BOOL WHO_TRIES_EVENT_DATA::GetEVChain(int num, DWORD &cid)
{
  if (num == 0)
    return GameEvent::GetEVChain(num, cid);

  if (num == 1)
  {
    cid=successChain;
    return TRUE;
  }
  else if (num == 2)
  {
    cid=failChain;
    return TRUE;
  }

  return FALSE;
}

#endif

void WHO_TRIES_EVENT_DATA::GetChainCount(int &curr, int &total)
{
  curr=0;
  total=2;

  if (successChain > 0)
    curr++;
  if (failChain > 0)
    curr++;
}

#if (defined UAFEDITOR)

void QUESTION_BUTTON_DATA::ChainToEventId(int num, DWORD cid)
{
  if (cid != NO_EVENT)
  {
    ASSERT( CheckValidEvent(this, cid) );
  }
  
  if ((num < 0) || (num > MAX_BUTTONS))
    return;

  if (num == 0)
  {
    GameEvent::ChainToEventId(num, cid);
    return;
  }

  buttons.buttons[num-1].chain = cid;
}

int QUESTION_BUTTON_DATA::UnchainToEventId(int num, DWORD eid)
{ // Called when an event is deleted.  We had best not point to it anymore.
  int result=0;
  if (num<1) result+=GameEvent::UnchainToEventId(num, eid);
  for (int i=0; i<MAX_BUTTONS; i++)
    if (buttons.buttons[i].chain==eid)
    {
      buttons.buttons[i].chain=0;
      result++;
    };
  return result;
}



BOOL QUESTION_BUTTON_DATA::GetEVChainText(int num, char *ptext)
{
  if ((num < 0) || (num > MAX_BUTTONS))
    return FALSE;

  if (num == 0)
    return GameEvent::GetEVChainText(num, ptext);

    sprintf(ptext, 
            "Button %i Chain: %s", 
            num,
            GetEventIdDescription(buttons.buttons[num-1].chain,GetEventSource()));

  return TRUE;
}


BOOL QUESTION_BUTTON_DATA::GetEVChain(int num, DWORD &cid)
{
  if ((num < 0) || (num > MAX_BUTTONS))
    return FALSE;

  if (num == 0)
    return GameEvent::GetEVChain(num, cid);

  cid = buttons.buttons[num-1].chain;  
  return TRUE;
}

#endif

void QUESTION_BUTTON_DATA::GetChainCount(int &curr, int &total)
{
  curr=0;
  total=MAX_BUTTONS;

  for (int i=0;i<MAX_BUTTONS;i++)
  {
    if (buttons.buttons[i].chain > 0)
      curr++;
  }
}

#if (defined UAFEDITOR)



void QUESTION_LIST_DATA::ChainToEventId(int num, DWORD cid)
{
  if (cid != NO_EVENT)
  {
    ASSERT( CheckValidEvent(this, cid) );
  }
  
  if ((num < 0) || (num > MAX_BUTTONS))
    return;

  if (num == 0)
  {
    GameEvent::ChainToEventId(num, cid);
    return;
  }

  buttons.buttons[num-1].chain = cid;
}

int QUESTION_LIST_DATA::UnchainToEventId(int num, DWORD eid)
{ // Called when an event is deleted.  We had best not point to it anymore.
  int result=0;
  if (num<1) result+=GameEvent::UnchainToEventId(num, eid);
  for (int i=0; i<MAX_BUTTONS; i++)
    if (buttons.buttons[i].chain==eid) 
    {
      buttons.buttons[i].chain=0;
      result++;
    };
  return result;
}


BOOL QUESTION_LIST_DATA::GetEVChainText(int num, char *ptext)
{
  if ((num < 0) || (num > MAX_BUTTONS))
    return FALSE;

  if (num == 0)
    return GameEvent::GetEVChainText(num, ptext);

    sprintf(ptext, 
            "Button %i Chain: %s", 
            num,
            GetEventIdDescription(buttons.buttons[num-1].chain,GetEventSource()));

  return TRUE;
}

BOOL QUESTION_LIST_DATA::GetEVChain(int num, DWORD &cid)
{
  if ((num < 0) || (num > MAX_BUTTONS))
    return FALSE;

  if (num == 0)
    return GameEvent::GetEVChain(num, cid);

  cid = buttons.buttons[num-1].chain;  
  return TRUE;
}

#endif

void QUESTION_LIST_DATA::GetChainCount(int &curr, int &total)
{
  curr=0;
  total=MAX_BUTTONS;

  for (int i=0;i<MAX_BUTTONS;i++)
  {
    if (buttons.buttons[i].chain > 0)
      curr++;
  }
}

#if (defined UAFEDITOR)

void QUESTION_YES_NO::ChainToEventId(int num, DWORD cid)
{
  if (cid != NO_EVENT)
  {
    ASSERT( CheckValidEvent(this, cid) );
  }
  
  switch (num)
  {
  case 0:
    GameEvent::ChainToEventId(num, cid);
    break;
  case 1:
    YesChain = cid;
    break;
  case 2:
    NoChain = cid;
    break;
  }
}

int QUESTION_YES_NO::UnchainToEventId(int num, DWORD eid)
{ // Called when an event is deleted.  We had best not point to it anymore.
  int result=0;
  if (num<1) result+=GameEvent::UnchainToEventId(num, eid);
  if (YesChain == eid) {YesChain=0;result++;};
  if (NoChain == eid)  {NoChain=0; result++;};
  return result;
}



BOOL QUESTION_YES_NO::GetEVChainText(int num, char *ptext)
{
  if (num == 0)
    return GameEvent::GetEVChainText(num, ptext);

  if (num == 1)
  {
    sprintf(ptext, 
            "%s: %s", 
            "Yes Chain",
            GetEventIdDescription(YesChain,GetEventSource()));
    return TRUE;
  }
  else if (num == 2)
  {
    sprintf(ptext, 
            "%s: %s", 
            "No Chain",
            GetEventIdDescription(NoChain,GetEventSource()));
    return TRUE;
  }

  return FALSE;
}

BOOL QUESTION_YES_NO::GetEVChain(int num, DWORD &cid)
{
  if (num == 0)
    return GameEvent::GetEVChain(num, cid);

  if (num == 1)
  {
    cid=YesChain;
    return TRUE;
  }
  else if (num == 2)
  {
    cid=NoChain;
    return TRUE;
  }

  return FALSE;
}

#endif

void QUESTION_YES_NO::GetChainCount(int &curr, int &total)
{
  curr=0;
  total=2;

  if (YesChain > 0)
    curr++;
  if (NoChain > 0)
    curr++;
}
BOOL QUEST_EVENT_DATA::IsChained()
{
  if (GameEvent::IsChained())
    return TRUE;

  if (acceptChain > 0)
    return TRUE;
  if (rejectChain > 0)
    return TRUE;

  return FALSE;
}

BOOL TAVERN::IsChained()
{
  if (GameEvent::IsChained())
    return TRUE;

  if (drinkChain > 0)
    return TRUE;
  if (fightChain > 0)
    return TRUE;

  return FALSE;
}

BOOL ENCOUNTER_DATA::IsChained()
{
  if (GameEvent::IsChained())
    return TRUE;

	for (int i=0;i<MAX_BUTTONS;i++)
	{
    if (buttons.buttons[i].chain > 0)
			return TRUE;
	}

  return FALSE;
}

BOOL QUESTION_LIST_DATA::IsChained()
{
  if (GameEvent::IsChained())
    return TRUE;

	for (int i=0;i<MAX_BUTTONS;i++)
	{
    if (buttons.buttons[i].chain > 0)
			return TRUE;
	}

  return FALSE;
}

BOOL QUESTION_BUTTON_DATA::IsChained()
{
  if (GameEvent::IsChained())
    return TRUE;

	for (int i=0;i<MAX_BUTTONS;i++)
	{
    if (buttons.buttons[i].chain > 0)
			return TRUE;
	}

  return FALSE;
}

BOOL QUESTION_YES_NO::IsChained()
{
  if (GameEvent::IsChained())
    return TRUE;

  if (YesChain > 0)
    return TRUE;
  if (NoChain > 0)
    return TRUE;

  return FALSE;
}

BOOL PASSWORD_DATA::IsChained()
{
  if (GameEvent::IsChained())
    return TRUE;

  if (successChain > 0)
    return TRUE;
  if (failChain > 0)
    return TRUE;

  return FALSE;
}

BOOL WHO_PAYS_EVENT_DATA::IsChained()
{
  if (GameEvent::IsChained())
    return TRUE;

  if (successChain > 0)
    return TRUE;
  if (failChain > 0)
    return TRUE;

  return FALSE;
}

BOOL WHO_TRIES_EVENT_DATA::IsChained()
{
  if (GameEvent::IsChained())
    return TRUE;

  if (successChain > 0)
    return TRUE;
  if (failChain > 0)
    return TRUE;

  return FALSE;
}

void HEAL_PARTY_DATA::Clear()
{
  GameEvent::Clear();
  
  HealHP=TRUE;
  HealDrain=FALSE;
  HealCurse=FALSE;
  chance = 100;
  who=EntireParty;
  HowMuchHP=100;
  LiteralOrPercent=1;
}

HEAL_PARTY_DATA& HEAL_PARTY_DATA::operator =(const HEAL_PARTY_DATA& src)
{
  if (&src == this)
    return *this;
  GameEvent::operator =(src);
  HealHP = src.HealHP;
  HealDrain = src.HealDrain;
  HealCurse = src.HealCurse;
  chance = src.chance;
  who = src.who;
  HowMuchHP=src.HowMuchHP;
  LiteralOrPercent=src.LiteralOrPercent;
  return *this;
}

void HEAL_PARTY_DATA::Serialize(CArchive &ar)
{
   GameEvent::Serialize(ar);
   if (ar.IsStoring())
   {
      ar << HealHP;
      ar << HealDrain;
      ar << HealCurse;
      ar << chance;
      ar << (int)who;
      ar << HowMuchHP;
      ar << LiteralOrPercent;
   }
   else
   {
      int temp;
      ar >> HealHP;
      ar >> HealDrain;
      ar >> HealCurse;
      ar >> chance;
      ar >> temp;
      who = (eventPartyAffectType)temp;
      if (LoadingVersion >= _VERSION_0882_)
      {
        ar >> HowMuchHP;
        ar >> LiteralOrPercent;
      }
   }  
}
void HEAL_PARTY_DATA::Serialize(CAR &ar)
{
   GameEvent::Serialize(ar);
   if (ar.IsStoring())
   {
      ar << HealHP;
      ar << HealDrain;
      ar << HealCurse;
      ar << chance;
      ar << (int)who;
      ar << HowMuchHP;
      ar << LiteralOrPercent;
   }
   else
   {
      int temp;
      ar >> HealHP;
      ar >> HealDrain;
      ar >> HealCurse;
      ar >> chance;
      ar >> temp;
      who = (eventPartyAffectType)temp;
      if (LoadingVersion >= _VERSION_0882_)
      {
        ar >> HowMuchHP;
        ar >> LiteralOrPercent;
      }
   }  
}

LOGIC_BLOCK_DATA::LOGIC_BLOCK_DATA(void) : GameEvent(), falseChain(0),trueChain(0) 
{
  Clear();
  event=LogicBlock;
}

LOGIC_BLOCK_DATA::~LOGIC_BLOCK_DATA(void)
{
  //Clear();
}

void LOGIC_BLOCK_DATA::Clear(void)
{
  GameEvent::Clear();
  
  falseChain=0;
  trueChain=0;
	m_InputParamA="";
	m_InputParamB="";
	m_InputParamD="";
	m_InputParamF="";
	m_InputParamG="";
	m_ActionParam1="";
	m_ActionParam2="";
	m_GateTypeC=0;
	m_GateTypeE=0;
	m_GateTypeH=0;
	m_GateTypeI=0;
	m_GateTypeJ=0;
	m_GateTypeK=0;
	m_GateTypeL=0;
	m_InputTypeA=0;
	m_InputTypeB=0;
	m_InputTypeD=0;
	m_InputTypeF=0;
	m_InputTypeG=0;
	m_ActionType1=0;
	m_ActionType2=0;
	m_ChainIfFalse=0;
	m_ChainIfTrue=0;
	m_NoChain=1;
	m_NotC=0;
	m_NotE=0;
	m_NotH=0;
	m_NotI=0;
	m_NotJ=0;
	m_NotK=0;
  m_IfTrue1=0;
  m_IfTrue2=0;
  m_Flags=0;
  m_Misc="";Result=-1;
}

LOGIC_BLOCK_DATA& LOGIC_BLOCK_DATA::operator = (const LOGIC_BLOCK_DATA& src)
{
  if (&src == this) return *this;

  GameEvent::operator = (src);
  Result=src.Result;
  falseChain=src.falseChain;
  trueChain=src.trueChain;
	m_InputParamA=src.m_InputParamA;
	m_InputParamB=src.m_InputParamB;
	m_InputParamD=src.m_InputParamD;
	m_InputParamF=src.m_InputParamF;
	m_InputParamG=src.m_InputParamG;
	m_ActionParam1=src.m_ActionParam1;
	m_ActionParam2=src.m_ActionParam2;
	m_GateTypeC=src.m_GateTypeC;
	m_GateTypeE=src.m_GateTypeE;
	m_GateTypeH=src.m_GateTypeH;
	m_GateTypeI=src.m_GateTypeI;
	m_GateTypeJ=src.m_GateTypeJ;
	m_GateTypeK=src.m_GateTypeK;
	m_GateTypeL=src.m_GateTypeL;
	m_InputTypeA=src.m_InputTypeA;
	m_InputTypeB=src.m_InputTypeB;
	m_InputTypeD=src.m_InputTypeD;
	m_InputTypeF=src.m_InputTypeF;
	m_InputTypeG=src.m_InputTypeG;
	m_ActionType1=src.m_ActionType1;
	m_ActionType2=src.m_ActionType2;
	m_ChainIfFalse=src.m_ChainIfFalse;
	m_ChainIfTrue=src.m_ChainIfTrue;
	m_NoChain=src.m_NoChain;
	m_NotC=src.m_NotC;
	m_NotE=src.m_NotE;
	m_NotH=src.m_NotH;
	m_NotI=src.m_NotI;
	m_NotJ=src.m_NotJ;
	m_NotK=src.m_NotK;
  m_IfTrue1=src.m_IfTrue1;
  m_IfTrue2=src.m_IfTrue2;
  m_Flags=src.m_Flags;
  m_Misc=src.m_Misc;
  return *this;
}

void LOGIC_BLOCK_DATA::Serialize(CArchive& ar)
{
  GameEvent::Serialize(ar);
  if (ar.IsStoring())
  {
    ar << falseChain;
    ar << trueChain;
  	ar << m_InputParamA;
	  ar << m_InputParamB;
	  ar << m_InputParamD;
	  ar << m_InputParamF;
	  ar << m_InputParamG;
	  ar << m_ActionParam1;
	  ar << m_ActionParam2;
	  ar << m_GateTypeC;
	  ar << m_GateTypeE;
	  ar << m_GateTypeH;
	  ar << m_GateTypeI;
	  ar << m_GateTypeJ;
	  ar << m_GateTypeK;
	  ar << m_GateTypeL;
	  ar << m_InputTypeA;
	  ar << m_InputTypeB;
	  ar << m_InputTypeD;
	  ar << m_InputTypeF;
	  ar << m_InputTypeG;
	  ar << m_ActionType1;
	  ar << m_ActionType2;
	  ar << m_ChainIfFalse;
	  ar << m_ChainIfTrue;
	  ar << m_NoChain;
	  ar << m_NotC;
	  ar << m_NotE;
	  ar << m_NotH;
	  ar << m_NotI;
	  ar << m_NotJ;
	  ar << m_NotK;
    ar << m_IfTrue1;
    ar << m_IfTrue2;
    ar << m_Flags;
    ar << m_Misc;
  }
  else
  {
    ar >> falseChain;
    ar >> trueChain;
  	ar >> m_InputParamA;
  	ar >> m_InputParamB;
  	ar >> m_InputParamD;
  	ar >> m_InputParamF;
  	ar >> m_InputParamG;
  	ar >> m_ActionParam1;
  	ar >> m_ActionParam2;
  	ar >> m_GateTypeC;
  	ar >> m_GateTypeE;
  	ar >> m_GateTypeH;
  	ar >> m_GateTypeI;
  	ar >> m_GateTypeJ;
  	ar >> m_GateTypeK;
  	ar >> m_GateTypeL;
  	ar >> m_InputTypeA;
  	ar >> m_InputTypeB;
  	ar >> m_InputTypeD;
  	ar >> m_InputTypeF;
  	ar >> m_InputTypeG;
  	ar >> m_ActionType1;
  	ar >> m_ActionType2;
  	ar >> m_ChainIfFalse;
  	ar >> m_ChainIfTrue;
  	ar >> m_NoChain;
  	ar >> m_NotC;
  	ar >> m_NotE;
  	ar >> m_NotH;
  	ar >> m_NotI;
  	ar >> m_NotJ;
  	ar >> m_NotK;
    ar >> m_IfTrue1;
    ar >> m_IfTrue2;
    ar >> m_Flags;
    ar >> m_Misc;
  };
}
void LOGIC_BLOCK_DATA::Serialize(CAR& ar)
{
  GameEvent::Serialize(ar);
  if (ar.IsStoring())
  {
    ar << falseChain;
    ar << trueChain;
  	ar << m_InputParamA;
	  ar << m_InputParamB;
	  ar << m_InputParamD;
	  ar << m_InputParamF;
	  ar << m_InputParamG;
	  ar << m_ActionParam1;
	  ar << m_ActionParam2;
	  ar << m_GateTypeC;
	  ar << m_GateTypeE;
	  ar << m_GateTypeH;
	  ar << m_GateTypeI;
	  ar << m_GateTypeJ;
	  ar << m_GateTypeK;
	  ar << m_GateTypeL;
	  ar << m_InputTypeA;
	  ar << m_InputTypeB;
	  ar << m_InputTypeD;
	  ar << m_InputTypeF;
	  ar << m_InputTypeG;
	  ar << m_ActionType1;
	  ar << m_ActionType2;
	  ar << m_ChainIfFalse;
	  ar << m_ChainIfTrue;
	  ar << m_NoChain;
	  ar << m_NotC;
	  ar << m_NotE;
	  ar << m_NotH;
	  ar << m_NotI;
	  ar << m_NotJ;
	  ar << m_NotK;
    ar << m_IfTrue1;
    ar << m_IfTrue2;
    ar << m_Flags;
    ar << m_Misc;
  }
  else
  {
    ar >> falseChain;
    ar >> trueChain;
  	ar >> m_InputParamA;
  	ar >> m_InputParamB;
  	ar >> m_InputParamD;
  	ar >> m_InputParamF;
  	ar >> m_InputParamG;
  	ar >> m_ActionParam1;
  	ar >> m_ActionParam2;
  	ar >> m_GateTypeC;
  	ar >> m_GateTypeE;
  	ar >> m_GateTypeH;
  	ar >> m_GateTypeI;
  	ar >> m_GateTypeJ;
  	ar >> m_GateTypeK;
  	ar >> m_GateTypeL;
  	ar >> m_InputTypeA;
  	ar >> m_InputTypeB;
  	ar >> m_InputTypeD;
  	ar >> m_InputTypeF;
  	ar >> m_InputTypeG;
  	ar >> m_ActionType1;
  	ar >> m_ActionType2;
  	ar >> m_ChainIfFalse;
  	ar >> m_ChainIfTrue;
  	ar >> m_NoChain;
  	ar >> m_NotC;
  	ar >> m_NotE;
  	ar >> m_NotH;
  	ar >> m_NotI;
  	ar >> m_NotJ;
  	ar >> m_NotK;
    ar >> m_IfTrue1;
    ar >> m_IfTrue2;
    ar >> m_Flags;
    ar >> m_Misc;
  };
}


BOOL LOGIC_BLOCK_DATA::IsChained()
{
  if (GameEvent::IsChained()) return TRUE;
  if (falseChain > 0) return TRUE;
  if (trueChain > 0) return TRUE;
  return FALSE;
}

void LOGIC_BLOCK_DATA::GetChainCount(int &curr, int &total)
{
  curr=0;
  total=2;
  if (falseChain > 0) curr++;
  if (trueChain > 0)  curr++;
}

#if (defined UAFEDITOR)

void LOGIC_BLOCK_DATA::ChainToEventId(int num, DWORD cid)
{
  if (cid != NO_EVENT)
  {
    ASSERT( CheckValidEvent(this, cid) );
  }
  
  if (num==0) GameEvent::ChainToEventId(num, cid);
  if (num==1) falseChain = cid;
  if (num==2) trueChain = cid;
}

int LOGIC_BLOCK_DATA::UnchainToEventId(int num, DWORD cid)
{ // Called when an event is deleted.  We had best not point to it anymore.
  int result=0;
  if (num<1) result+=GameEvent::UnchainToEventId(num, cid);
  if (num==1) if (falseChain==cid) {falseChain=0; result++;};
  if (num==2) if (trueChain ==cid) {trueChain =0; result++;};
  return result;
}



BOOL LOGIC_BLOCK_DATA::GetEVChainText(int num, char *ctext)
{
  if ((num < 0) || (num > 2))
    return FALSE;

  if (num==0) return GameEvent::GetEVChainText(num, ctext);
  if (num==1) sprintf(ctext,
                      "False Chain: %s",
                      GetEventIdDescription(falseChain,GetEventSource()));
  if (num==2) sprintf(ctext,
                      "True Chain: %s",
                      GetEventIdDescription(trueChain,GetEventSource()));
  return TRUE;
}


BOOL LOGIC_BLOCK_DATA::GetEVChain(int num, DWORD &cid)
{
  if ((num < 0) || (num > 2))
    return FALSE;
  if (num == 0)
    return GameEvent::GetEVChain(num, cid);
  if (num==1) cid=falseChain;
  if (num==2) cid=trueChain;
  return TRUE;
}


BOOL GameEvent::Validate(FILE *pFile)
{
  BOOL success = TRUE;
  // default is to validate pic and pic1 if needed
  if (pic.filename != "")
  {
    if (!FileExists(pic.filename))
    {
      if (IsAChainEvent())
        WriteValidateLog(pFile, "%s chain event missing image: %s\n",
                 GetEventIdDescription(this->id,GetEventSource()), pic.filename);
      else
        WriteValidateLog(pFile, "%s event at %i,%i missing image: %s\n",
                 GetEventIdDescription(this->id,GetEventSource()), x,y, pic.filename);
      success = FALSE;
    }
  }
  if (pic2.filename != "")
  {
    if (!FileExists(pic2.filename))
    {
      if (IsAChainEvent())
        WriteValidateLog(pFile, "%s chain event missing image: %s\n",
                 GetEventIdDescription(this->id,GetEventSource()), pic2.filename);
      else
        WriteValidateLog(pFile, "%s event at %i,%i missing image: %s\n",
                 GetEventIdDescription(this->id,GetEventSource()), x,y, pic2.filename);
      success = FALSE;
    }
  }
  return success;
}

BOOL COMBAT_EVENT_DATA::Validate(FILE *pFile)
{
  BOOL success = GameEvent::Validate(pFile);

  if (!FileExists(DeathSound))
  {
    WriteValidateLog(pFile, "%s event at %i,%i missing death sound: %s\n",
             GetEventIdDescription(this->id,GetEventSource()), x,y, DeathSound);
    success = FALSE;
  }
  if (!FileExists(MoveSound))
  {
    WriteValidateLog(pFile, "%s event at %i,%i missing move sound: %s\n",
             GetEventIdDescription(this->id,GetEventSource()), x,y, MoveSound);
    success = FALSE;
  }

  
  if (!FileExists(TurnUndeadSound))
  {
    WriteValidateLog(pFile, "%s event at %i,%i missing turn undead sound: %s\n",
             GetEventIdDescription(this->id,GetEventSource()), x,y, TurnUndeadSound);
    success = FALSE;
  }

  POSITION pos = bgSounds.backgroundSounds.sounds.GetHeadPosition();
  while (pos != NULL)
  {
    if (!FileExists(bgSounds.backgroundSounds.sounds.GetAt(pos)))
    {
      WriteValidateLog(pFile, "%s event at %i,%i missing combat background sound: %s\n",
                       GetEventIdDescription(this->id,GetEventSource()), x,y, bgSounds.backgroundSounds.sounds.GetAt(pos));
      success = FALSE;
    }
    bgSounds.backgroundSounds.sounds.GetNext(pos);
  }

  for (int i=0;i<monsters.GetCount();i++)
  {
    if (monsters.monsters[i].type == MONSTER_TYPE)
    {
      if (!monsterData.Validate(monsters.monsters[i].monster, pFile))
      {
        WriteValidateLog(pFile, "%s event at %i,%i failed monster validation\n",
                 GetEventIdDescription(this->id,GetEventSource()), x,y);
        success = FALSE;
      }
      pos = monsters.monsters[i].items.GetHeadPosition();
      while (pos != NULL)
      {
        if (!itemData.Validate(monsters.monsters[i].items.GetAt(pos).item, pFile))
        {
          WriteValidateLog(pFile, "%s event at %i,%i failed item validation\n",
                   GetEventIdDescription(this->id,GetEventSource()), x,y);
          success = FALSE;
        }
        monsters.monsters[i].items.GetNext(pos);
      }
    }
    else
    {
      if (!globalData.charData.Validate(monsters.monsters[i].monster, pFile, VAL_ICON))
      {
        WriteValidateLog(pFile, "%s event at %i,%i failed NPC validation\n",
                 GetEventIdDescription(this->id,GetEventSource()), x,y);
        success = FALSE;
      }
      
      pos = monsters.monsters[i].items.GetHeadPosition();
      while (pos != NULL)
      {
        if (!itemData.Validate(monsters.monsters[i].items.GetAt(pos).item, pFile))
        {
          WriteValidateLog(pFile, "%s event at %i,%i failed item validation\n",
                   GetEventIdDescription(this->id,GetEventSource()), x,y);
          success = FALSE;
        }
        monsters.monsters[i].items.GetNext(pos);
      }
    }
  }

  return success;
}

BOOL TRANSFER_DATA::Validate(FILE *pFile)
{
  int dx,dy;
  BOOL success = TRUE;

  if ((destLevel < 0) || (destLevel >= MAX_LEVELS))
  {
    success = FALSE;
    WriteValidateLog(pFile, "Invalid dest level %i in TRANSFER DATA\n", 
      destLevel+1);
  }
  else
  {
    if (!globalData.levelInfo.stats[destLevel].used)
    {
      success = FALSE;
      WriteValidateLog(pFile, "Dest level %i in TRANSFER DATA does not exist\n", 
        destLevel+1);
    }
    else
    {
      if (destEP < 0) // use x,y
      {
        dx = destX;
        dy = destY;
      }
      else
      {
        dx = globalData.levelInfo.stats[destLevel].entryPoints[destEP].x;
        dy = globalData.levelInfo.stats[destLevel].entryPoints[destEP].y;
      }

      if ((dx < 0) || (dx >= globalData.levelInfo.stats[destLevel].area_width))
      {
        success = FALSE;
        WriteValidateLog(pFile, "Dest x=%i for level %i in TRANSFER DATA not valid\n", 
          dx, destLevel+1);
      }
      if ((dy < 0) || (dy >= globalData.levelInfo.stats[destLevel].area_height))
      {
        success = FALSE;
        WriteValidateLog(pFile, "Dest y=%i for level %i in TRANSFER DATA not valid\n", 
          dy, destLevel+1);
      }
    }
  }
  return success;
}

BOOL TRANSFER_EVENT_DATA::Validate(FILE *pFile)
{
  BOOL success = GameEvent::Validate(pFile);
  if (!transferData.Validate(pFile))
  {
    WriteValidateLog(pFile, "TRANSFER DATA in %s at %i,%i not valid\n", GetEventIdDescription(this->id,GetEventSource()),x,y);
    success = FALSE;
  }
  return success;
}

BOOL PASSWORD_DATA::Validate(FILE *pFile)
{
  BOOL success = GameEvent::Validate(pFile);
  success &= successTransferData.Validate(pFile);
  success &= failTransferData.Validate(pFile);
  if ((ChainEvent == successAction) && (!CheckValidEvent(this, successChain)))
  {
    success = FALSE;
    WriteValidateLog(pFile, "success chain in %s at %i,%i not valid\n", GetEventIdDescription(this->id,GetEventSource()),x,y);
  }
  if ((ChainEvent == failAction) && (!CheckValidEvent(this,failChain)))
  {
    success = FALSE;
    WriteValidateLog(pFile, "fail chain in %s event at %i,%i not valid\n",GetEventIdDescription(this->id,GetEventSource()), x,y);
  }
  return success;
}

BOOL WHO_TRIES_EVENT_DATA::Validate(FILE *pFile)
{
  BOOL success = GameEvent::Validate(pFile);
  success &= successTransferData.Validate(pFile);
  success &= failTransferData.Validate(pFile);
  if ((ChainEvent == successAction) && (!CheckValidEvent(this,successChain)))
  {
    success = FALSE;
    WriteValidateLog(pFile, "success chain in %s event at %i,%i not valid\n",GetEventIdDescription(this->id,GetEventSource()), x,y);
  }
  if ((ChainEvent == failAction) && (!CheckValidEvent(this,failChain)))
  {
    success = FALSE;
    WriteValidateLog(pFile, "fail chain in %s event at %i,%i not valid\n",GetEventIdDescription(this->id,GetEventSource()), x,y);
  }
  return success;
}

BOOL WHO_PAYS_EVENT_DATA::Validate(FILE *pFile)
{
  BOOL success = GameEvent::Validate(pFile);
  success &= successTransferData.Validate(pFile);
  success &= failTransferData.Validate(pFile);
  if ((ChainEvent == successAction) && (!CheckValidEvent(this,successChain)))
  {
    success = FALSE;
    WriteValidateLog(pFile, "success chain in %s event at %i,%i not valid\n",GetEventIdDescription(this->id,GetEventSource()), x,y);
  }
  if ((ChainEvent == failAction) && (!CheckValidEvent(this,failChain)))
  {
    success = FALSE;
    WriteValidateLog(pFile, "fail chain in %s event at %i,%i not valid\n",GetEventIdDescription(this->id,GetEventSource()), x,y);
  }
  return success;
}

#endif

void PLAY_MOVIE_DATA::Clear() 
{ 
  GameEvent::Clear();   
  m_filename="";
  m_mode=movieFull; 
}
void ADD_NPC_DATA::Clear() 
{ 
  GameEvent::Clear(); 
  
  distance=FarAway;
  charAdded=0;
  hitPointMod=100;
  useOriginal=TRUE; 
}
void REMOVE_NPC_DATA::Clear() 
{
  GameEvent::Clear(); 
  
  distance=FarAway; 
  charRemoved=0; 
}
void CAMP_EVENT_DATA::Clear() 
{ 
  GameEvent::Clear();
  
  ForceExit=FALSE; 
}
void GUIDED_TOUR::Clear() 
{ 
  GameEvent::Clear();
  
  tour_x=0;tour_y=0;
  facing=0;
  useStartLoc=FALSE;
  currStep=0;
  executeEvent=FALSE;
  for (int i=0;i<MAX_TOUR_STEPS;i++) 
    steps[i].Clear(); 
}
void ENCOUNTER_DATA::Clear() 
{ 
  GameEvent::Clear();
  
  currDist=0;
  distance=FarAway;
  monsterSpeed=12;
  zeroRangeResult=NoResult;
  combatChain=0;talkChain=0;escapeChain=0;
  buttons.Clear(); 
}
void PASSWORD_DATA::Clear() 
{ 
  GameEvent::Clear();
  
  password="";
  nbrTries=0;
  currTry=0;
  successChain=0;
  failChain=0;
  successAction=NoAction;failAction=NoAction;
  successTransferData.Clear(); failTransferData.Clear(); 
  matchCriteria = ExactMatch; matchCase = TRUE; 
}
void GAIN_EXP_DATA::Clear() 
{ 
  GameEvent::Clear();
  
  tempExp=0;
  experience=0;
  hSound=-1;
  sound="";
  chance=100; 
  who=EntireParty; 
}
void NPC_SAYS_DATA::Clear() 
{ 
  GameEvent::Clear();
  
  partySeesChar=0;
  distance=FarAway;
  hSound=-1;
  sound="";
  mustHitReturn=TRUE;
  highlight=TRUE; 
}
void QUESTION_YES_NO::Clear() 
{ 
  GameEvent::Clear();
  
  YesChainAction=LabelDoNothing; 
  NoChainAction=LabelDoNothing;
  YesChain=0;
  NoChain=sx=sy=0; 
}
void QUESTION_LIST_DATA::Clear() 
{ 
  GameEvent::Clear();
  
  UserResult=NoUserResult;
  buttons.Clear();
  sx=sy=0; 
}
void QUESTION_BUTTON_DATA::Clear() 
{ 
  GameEvent::Clear();
  
  UserResult=NoUserResult;
  buttons.Clear();
  sx=sy=0; 
}
void TEXT_EVENT_DATA::Clear() 
{ 
  GameEvent::Clear();
  
  hSound=-1;
  sound="";
  WaitForReturn=TRUE;
  HighlightText=FALSE;
  ForceBackup=FALSE;
  KillSound=FALSE; 
  distance = UpClose;
}
void TRANSFER_EVENT_DATA::Clear() 
{ 
  GameEvent::Clear();
  
  askYesNo=TRUE;
  transferOnYes=TRUE;
  destroyDrow=FALSE;
  activateBeforeEntry=FALSE;
  transferData.Clear(); 
}
void WHO_PAYS_EVENT_DATA::Clear() 
{ 
  GameEvent::Clear();
  
  impossible=FALSE;
  gems=0;
  jewels=0;
  platinum=0;
  successChain=0;
  failChain=0;
  successAction=NoAction;
  failAction=NoAction;
  successTransferData.Clear(); 
  failTransferData.Clear(); 
  moneyType = PlatinumType;
}
void WHO_TRIES_EVENT_DATA::Clear() 
{ 
  GameEvent::Clear();
  
  alwaysSucceeds=FALSE;alwaysFails=FALSE;
  checkSTR=FALSE;checkINT=FALSE;checkWIS=FALSE;checkDEX=FALSE;checkCON=FALSE;checkCHA=FALSE;
  checkPP=FALSE;checkOL=FALSE;checkFT=FALSE;checkMS=FALSE;checkHS=FALSE;checkHN=FALSE;
  checkCW=FALSE;checkRL=FALSE;strBonus=0;compareToDie=FALSE;compareDie=0;NbrTries=0;currTry=0;
  successChain=0;failChain=0;successAction=NoAction;failAction=NoAction;
  successTransferData.Clear(); failTransferData.Clear(); 
}
void SPECIAL_ITEM_KEY_EVENT_DATA::Clear() 
{
  GameEvent::Clear();
  
  items.Clear();
  forceExit=FALSE;
  WaitForReturn=TRUE;
}
void QUEST_EVENT_DATA::Clear() 
{
  GameEvent::Clear();
  
  accept=QA_OnYes;completeOnAccept=FALSE;failOnRejection=FALSE;quest=-1;
  stage=1;acceptChain=0;rejectChain=0;
}
void PASS_TIME_EVENT_DATA::Clear() 
{ 
  GameEvent::Clear();
  
  PassSilent=FALSE;
  AllowStop=FALSE;
  SetTime=FALSE;
  Days=0;Hours=0;Minutes=0;
}
void PASSING_TIME_EVENT_DATA::Clear() 
{ 
  GameEvent::Clear();
  
  AllowStop=FALSE;
  daysLeft=0;hoursLeft=0;minutesLeft=0; 
}
void VAULT_EVENT_DATA::Clear() 
{ 
  GameEvent::Clear();
  WhichVault=0;
  ForceBackup=TRUE; 
}
void TAVERN::Clear() 
{ 
  GameEvent::Clear();
  
  forceExit=TRUE;
  allowFights=TRUE;
  allowDrinks=FALSE;
  inflation=0;
  barkeep=0;
  fightChain=0;
  drinkChain=0;
  drinkPointTrigger=0;
  taleOrder=InOrderTales;
  EachTaleOnceOnly=FALSE;
  for (int t=0;t<MAX_TALES;t++) 
    tales[t].Clear();
  for (int d=0;d<MAX_DRINKS;d++) 
    drinks[d].Clear(); 
}
void TRAININGHALL::Clear() 
{ 
  GameEvent::Clear();
  
  forceExit=FALSE;
  TrainCleric=TRUE;TrainFighter=TRUE;TrainMagicUser=TRUE;
  TrainThief=TRUE; TrainRanger=TRUE; TrainPaladin=TRUE; TrainDruid=TRUE;
  Cost=2000; 
}
void SHOP::Clear() 
{ 
  GameEvent::Clear();
  
  sellprice=0;
  buybackPercentage=50;
  costToId=20;
  forceExit=TRUE;
  itemsAvail.Clear();
  costFactor=Normal;
  canId=canApprGems=canApprJewels=TRUE;
  buyItemsSoldOnly=FALSE;
}
void SMALL_TOWN_DATA::Clear() 
{ 
  GameEvent::Clear(); 
  
  TempleChain=0; 
  ShopChain=0;
  InnChain=0; 
  TavernChain=0; 
  VaultChain=0; 
  TrainingHallChain=0; 
}
void RANDOM_EVENT_DATA::Clear() 
{ 
  GameEvent::Clear(); 
  
  for (int i=0;i<MAX_RANDOM_EVENTS;i++) 
  {
    EventChains[i]=0; 
    EventChance[i]=0; 
  }
}
void CHAIN_EVENT::Clear() 
{ 
  GameEvent::Clear(); 
  
  Chain=0; 
}
void JOURNAL_EVENT::Clear() 
{ 
  GameEvent::Clear(); 
  
  journal_entry=-1; 
}
void SOUND_EVENT::Clear() 
{ 
  GameEvent::Clear(); 
  
  sounds.RemoveAll(); 
}