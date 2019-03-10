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

#include "dungeoncraft/shared/GameEvent.h"

//#include "dungeoncraft/shared/externs.h"
#include "dungeoncraft/shared/GetInput.h"
#include "dungeoncraft/shared/CProcInp.h"
#include "dungeoncraft/shared/Screen.h"
#include "dungeoncraft/shared/Dgngame.h"
#include "dungeoncraft/shared/Graphics.h"
#include "dungeoncraft/shared/Viewport.h"
#include "dungeoncraft/shared/path.h"
#include "dungeoncraft/shared/DrawTile.h"

#include "dungeoncraft/shared/Level.h"
#include "dungeoncraft/shared/party.h"
#include "dungeoncraft/shared/FormattedText.h"
#include "dungeoncraft/shared/DispText.h"
#include "dungeoncraft/shared/GlobalData.h"
#include "dungeoncraft/shared/monster.h"
#include "dungeoncraft/shared/SoundMgr.h"
#include "dungeoncraft/shared/GameMenu.h"
#include "dungeoncraft/shared/..\Shared\GPDLopCodes.h"
#include "dungeoncraft/shared/GPDLexec.h"
#include "dungeoncraft/shared/GPDLComp.h"
#include "dungeoncraft/shared/treas.h"
#include "dungeoncraft/shared/combatants.h"
#include "dungeoncraft/shared/RunTimeIf.h"
#include "dungeoncraft/shared/CharStatsForm.h"
#include "dungeoncraft/shared/RestTimeForm.h"
#include "dungeoncraft/shared/MouseThread.h"
#include "dungeoncraft/shared/ItemsForm.h"
#include "dungeoncraft/shared/SpellForm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CMyMenu menu;  

// make sure you declare the lvalue properly when using this
// function.
//
// CHARACTER temp = GetActiveChar() <--- BAD!!!, destructor will ruin source char
//
// CHARACTER &temp = GetActiveChar() <-- correct way
//
CHARACTER &GetActiveChar(GameEvent *pEvent=NULL, int PartyIdx=-1)
{
  if ((pEvent!=NULL) && (pEvent->GetEventType() == (eventType)Combat))
  {
    if (PartyIdx == -1)
      return combatData.getActiveCombatCharacter();
    else
      return combatData.getCombatPartyCharacter(PartyIdx);
  }
  else
  {
    if (PartyIdx == -1)
      return party.getActiveChar();
    else
      return party.getCharacter(PartyIdx);
  }
}

BOOL GameEvent::ShouldSimulateEnterKey()
{
  if (!WaitUntilReturnPressed()) 
    return TRUE;
  else if ((currPic.filename.IsEmpty()) && (textData.numLines==0))
    return TRUE;
  return FALSE;
}

void GameEvent::OnEnterQueue(void)
{
  // Default is to do nothing.
}

void GameEvent::OnInitialEvent(void)
{
  ClearFormattedText(textData);
  menu.reset();
}

void GameEvent::OnLeaveQueue(void)
{
  // Default is to do nothing.
}

bool GameEvent::OnTimer(UINT timerId)
{
  // default is to say that we did not process the timer
  return false;
}

bool GameEvent::OnIdle(void) 
{
  return true; // Say that we are waiting for input.
}

unsigned int GameEvent::OnTaskMessage(TASKMESSAGE msg)
{
  // default is to say we want off the stack when
  // teleporting
  switch (msg)
  {
  case TASKMSG_TeleportWithinLevel:
  case TASKMSG_TeleportBetweenLevels:
    return TASKMSG_DELETE;
  default: 
    return 0;
  }
}

void GameEvent::OnReturnToTopOfQueue(void)
{ 
  // Default is to ignore
}

bool GameEvent::CheckForMultiBoxText(void)
{
  if (miscError != NoError)
  {
    // really just formats the error text
    displayMiscError(miscError);
    miscError = NoError;
  }

  if (errorTextData.numLines > 0) 
    return false;

  if (TextIsMultiBox(textData))
  {
    if (!textData.IsLastBox())
    {
      menu.AddOverrideMenu("ENTER=Next, BACKSPACE=Prev, SPACE=Last",-1,TRUE);
      return true;
    }
    else
    {
      menu.RemoveOverrideMenu();
      return false;
    }
  }
  else
  {
    menu.RemoveOverrideMenu();
    menu.setAllItemsActive();
  }
  return false;
}

bool GameEvent::OnFilterKeypress(key_code *key, char *ascii)
{
  if (*key==KC_NONE) return false;

  if (TextIsMultiBox(textData))
  {
    switch (*key)
    {
    case KC_SPACE:
      // end the multibox text
      if (!textData.IsLastBox())
        textData.Quit();
      *key=KC_NONE;
      return false;

    case KC_RETURN:
      // goto next box of text
      if (!textData.IsLastBox())
      {
        textData.NextBox();      
        *key=KC_NONE;
        return false;
      }
      break;

    case KC_BACK:
      // goto prev box of text
      if (!textData.IsFirstBox())
        textData.PrevBox();      
      *key=KC_NONE;
      return false;

    default: // no input accepted while multibox is active
      if (!textData.IsLastBox())
        *key=KC_NONE;
      return false;
    }
  }

  if (*key==KC_FC1)
  {
    // user requested a screen capture
    CString path=GetDesignPath();
    path += "DCScreenCapture.bmp";
    DeleteFile(path);
    GraphicsMgr.DoScreenCapture(path);
  }

  int item = menu.ConvertKeyCodeToMenuItem(*key);
  if (item > 0)
  {
    menu.setCurrentItem(item);
    simulatedKeyQueue.PushKey(VK_RETURN);
  }
  return false;
}

void GameEvent::OnKeypress(key_code key, char ascii)
{ // Ignore the keystroke by default unless it is a KC_RETURN
  if (key==KC_RETURN) PopEvent();
}

void GameEvent::OnMouseClickLeft(int mx, int my)
{ // Default mouse processing assumes walking state with 
  // menu, party, and inventory
  MouseMenu(mx, my);
  MouseParty(mx, my);
  MouseInventory(mx, my);
}
void GameEvent::OnMouseMove(int mx, int my)
{ 
  // Default mouse uses pointer only
  CursorArt.SetFirstFrame();
  CursorArt.sx = mx;
  CursorArt.sy = my;
}

bool GameEvent::OnTestTrigger(void)
{
  // We must either return true or get ourselves off the top of the stack.
  bool result = false;
  if (EventShouldTrigger()) result = true;
#ifdef _DEBUG
  if (id > 0)
    WriteDebugString("Event %u at %i,%i level %i %s triggered\n",
                     id, x,y, globalData.currLevel, 
                     (result==true ? "is" : "not"));
#endif
  return result;
} 

void GameEvent::OnDraw(void)
{
  // Default is to draw the mouse cursor
  DisplayTextAndFlip();
}

void GameEvent::OnUpdateUI(void)
{
  // default is to enable all menu items
  CheckForMultiBoxText();
}

// default is to automatically set up small pics
// to animate if loaded.
bool GameEvent::OnCycle(void)
{
  // this can be called even before a design
  // is loaded (ex: DesignSelect)
  if (levelData.area_width <= 0) return false;

  LONGLONG currTime = virtualGameTime;//timeGetTime();
  bool result = false;
  // update drunk points, spell effects, etc
  if (IsCombatActive())
    result = combatData.ProcessTimeSensitiveData(currTime);
  else
    result = party.ProcessTimeSensitiveData(currTime);

  // this advances to next frame if timeDelay ms
  // have passed since last update.  
  if (currPic.AnimateNextFrame(currTime))
    result = true;

  if (UpdateAnimateList())
    result = true;
  
  if (errorTextData.numLines > 0) 
  {
    long diff = currTime - errorTextData.startTime;
    if ((diff > 2000) || (diff < 0))
    {
      ClearFormattedText(errorTextData);
      result=true;
    }
  }
  for (int i=0;i<party.numCharacters;i++)
  {
    // sets character name to different color when
    // ready to train.
    party.characters[i].SetReadyToTrain(party.characters[i].IsReadyToTrain());
    if (party.characters[i].GetAge() > party.characters[i].GetMaxAge())
      party.characters[i].SetStatus(Dead);
  }
  return result;
}

int GameEvent::OnSaveGame(unsigned int *saveArea)
{ // Perhaps you meant to override this.
  ASSERT(0);
  MsgBoxError("Illegal game state for save");
  return 0; // Make compiler happy.
}

int GameEvent::OnLoadGame(unsigned int *saveArea)
{
  ASSERT(0);
  MsgBoxError("Illegal game state in load");
  return 0;
}

void GameEvent::PushEvent(GameEvent *ev, EventDisposition disposition)
{
  ASSERT( ev != NULL );
  if (ev == NULL) return;
  ASSERT( ev->IsGlobalEvent() == FALSE );
  taskList.PushEvent(ev, disposition);
}
void GameEvent::PushGlobalEvent(GameEvent *ev, EventDisposition disposition)
{
  ASSERT( ev != NULL );
  if (ev == NULL) return;
  ASSERT( ev->IsGlobalEvent() == TRUE );
  taskList.PushEvent(ev, disposition);
}

void GameEvent::InsertEvent(GameEvent *ev, EventDisposition disposition)
{
  ASSERT( ev != NULL );
  if (ev == NULL) return;
  ASSERT( ev->IsGlobalEvent() == FALSE );
  taskList.InsertEvent(ev, disposition);
}
void GameEvent::InsertEvent(DWORD evid)
{
  InsertEvent(levelData.eventData.GetEvent(evid), LevelEvent);
}

void GameEvent::InsertGlobalEvent(GameEvent *ev, EventDisposition disposition)
{
  ASSERT( ev != NULL );
  if (ev == NULL) return;
  ASSERT( ev->IsGlobalEvent() == TRUE );
  taskList.InsertEvent(ev, disposition);
}
void GameEvent::InsertGlobalEvent(DWORD evid)
{
  InsertGlobalEvent(globalData.eventData.GetEvent(evid), LevelEvent);
}

void GameEvent::PushEvent(DWORD evid)
{
  PushEvent(levelData.eventData.GetEvent(evid), LevelEvent);
}
void GameEvent::PushGlobalEvent(DWORD evid)
{
  PushGlobalEvent(globalData.eventData.GetEvent(evid), LevelEvent);
}

void GameEvent::ReplaceEvent(GameEvent *ev, EventDisposition disposition)
{
  ASSERT( ev != NULL );
  if (ev == NULL) return;
  ASSERT( ev->IsGlobalEvent() == FALSE );
  taskList.ReplaceEvent(ev, this, disposition);
}
void GameEvent::ReplaceEvent(DWORD eid)
{
  ReplaceEvent(levelData.eventData.GetEvent(eid), LevelEvent);
}

void GameEvent::ReplaceGlobalEvent(GameEvent *ev, EventDisposition disposition)
{
  ASSERT( ev != NULL );
  if (ev == NULL) return;
  ASSERT( ev->IsGlobalEvent() == TRUE );
  taskList.ReplaceEvent(ev, this, disposition);
}
void GameEvent::ReplaceGlobalEvent(DWORD evid)
{
  ReplaceGlobalEvent(globalData.eventData.GetEvent(evid), LevelEvent);
}

void GameEvent::SetMyState(TASKSTATE state)
{
  taskList.SetEventState(state, this);
}

TASKSTATE GameEvent::GetMyState(void)
{
  return taskList.GetEventState(this);
}

void GameEvent::PopEvent(void)
{
  taskList.PopEvent(this);
}

void GameEvent::Draw(void)
{
   OnDraw();
}

void GameEvent::StandardKeyboardShortcutAction(key_code key, char ascii)
{
  if ((key != KC_CHAR) && (key != KC_NUM)) return;
  if (!UseKeyboardShortcuts) return;
  int item = menu.LookupShortcut(ascii);
  if (item >= 0)
  {
    menu.setCurrentItem(item+1);
    simulatedKeyQueue.PushKey(VK_RETURN);
  }
  else
  {
    // some small pics won't loop back to the first
    // frame until a key is pressed.
    if (character.smallPic.IsPicLoaded())
    {
      if (character.smallPic.flags & PIC_DATA::AF_KeypressBeforeLoop)
      {
        if (character.smallPic.IsLastFrame())
          character.smallPic.SetFirstFrame();
      }
    }
    if (character.icon.IsPicLoaded())
    {
      if (character.icon.flags & PIC_DATA::AF_KeypressBeforeLoop)
      {
        if (character.icon.IsLastFrame())
          character.icon.SetFirstFrame();
      }
    }
    if (currPic.IsPicLoaded())
    {
      if (currPic.flags & PIC_DATA::AF_KeypressBeforeLoop)
      {
        if (currPic.IsLastFrame())
          currPic.SetFirstFrame();
      }
    }    
  }
}

void GameEvent::StandardMenuKeyboardAction(key_code key, char ascii)
{
  switch (key)
  {
  case KC_UP:   menu.prevItem();  break;
  case KC_DOWN: menu.nextItem();  break;
  case KC_RIGHT:menu.nextItem();  break;
  case KC_LEFT: menu.prevItem();  break;
  case KC_NUM:
  case KC_CHAR: StandardKeyboardShortcutAction(key,ascii); break;
  default: 
    // some small pics won't loop back to the first
    // frame until a key is pressed.
    if (character.smallPic.IsPicLoaded())
    {
      if (character.smallPic.flags & PIC_DATA::AF_KeypressBeforeLoop)
      {
        if (character.smallPic.IsLastFrame())
          character.smallPic.SetFirstFrame();
      }
    }
    if (character.icon.IsPicLoaded())
    {
      if (character.icon.flags & PIC_DATA::AF_KeypressBeforeLoop)
      {
        if (character.icon.IsLastFrame())
          character.icon.SetFirstFrame();
      }
    }
    if (currPic.IsPicLoaded())
    {
      if (currPic.flags & PIC_DATA::AF_KeypressBeforeLoop)
      {
        if (currPic.IsLastFrame())
          currPic.SetFirstFrame();
      }
    }    
    break;
  };
}

void GameEvent::HMenuVPartyKeyboardAction(key_code key, char ascii)
{ // Horizontal menu / vertical party
  if (party.numCharacters <= 1) 
  {
    // if only one selectable party member, let all
    // arrows change menu options
    StandardMenuKeyboardAction(key, ascii);
    return;
  }
  switch (key)
  {
  case KC_UP:   party.prevCharacter(); break;
  case KC_DOWN: party.nextCharacter(); break;
  default: StandardMenuKeyboardAction(key, ascii);
  };
}

void GameEvent::VMenuHPartyKeyboardAction(key_code key, char ascii)
{ // Horizontal menu / vertical party
  if (party.numCharacters <= 1) 
  {
    // if only one selectable party member, let all
    // arrows change menu options
    StandardMenuKeyboardAction(key, ascii);
    return;
  }
  switch (key)
  {
  case KC_LEFT:   party.prevCharacter(); break;
  case KC_RIGHT: party.nextCharacter(); break;
  default: StandardMenuKeyboardAction(key, ascii);
  };
}

void GameEvent::HMenuVItemsKeyboardAction(key_code key, char ascii)
{ // Horizontal menu / vertical items
  switch (key)
  {
  case KC_UP:   party.prevItem(); break;
  case KC_DOWN: party.nextItem(); break;
  default: StandardMenuKeyboardAction(key, ascii);
  };
}

void GameEvent::HMenuVInventoryKeyboardAction(key_code key, char ascii)
{ // Horizontal menu / vertical inventory
  switch (key)
  {
  case KC_UP:   party.prevItem(); break;
  case KC_DOWN: party.nextItem(); break;
  default: StandardMenuKeyboardAction(key, ascii);
  };
}


void GameEvent::MouseMenu(int mx, int my)
{
  int item = menu.IntersectPointWithMenu(mx, my);
  if (item >= 0)
  {
    TRACE("Mouse click hits menu item %i\n",item);
    menu.setCurrentItem(item+1);
    simulatedKeyQueue.PushKey(VK_RETURN);
  };
}

void GameEvent::MouseParty(int mx, int my)
{
  int item;
  item = IntersectPointWithParty(mx, my);
  if (item >= 0)
  {
    TRACE("Mouse click hits party member %i\n",item);
    party.activeCharacter = item;
    OnUpdateUI();
    Draw();
  }
}

bool GameEvent::TABParty(key_code key)
{
  if (key != KC_TAB) return false;
  TRACE("TAB from party member %i\n",party.activeCharacter);
  party.activeCharacter++;
  if (party.activeCharacter >= party.numCharacters)
  {
    party.activeCharacter = 0;
  };
  OnUpdateUI();
  Draw();
  return true;
}


bool GameEvent::MouseInventory(int mx, int my)
{
  int item;
  item = IntersectPointWithInventory(mx, my);
  if (item >= 0)
  {
    TRACE("Mouse click hits inventory item %i\n",item);
    party.activeItem = item;
    OnUpdateUI();
    Draw();
    return true;
  }
  return false;
}

void GameEvent::MouseCombatMap(int mx, int my)
{
  int tx=0, ty=0;
  if (IntersectPointWithCombatMap(mx, my, tx, ty))
  {
    TRACE("mouse click hits combat map\n");
    combatData.moveToX = tx;
    combatData.moveToY = ty;
    simulatedKeyQueue.PushKey(VK_INSERT);
  }
}

void GameEvent::MouseCombatMapMenu(int mx, int my)
{
  MouseMenu(mx, my);
  MouseCombatMap(mx,my);
}

void GameEvent::MouseMenuInventory(int mx, int my)
{
  MouseMenu(mx, my);
  MouseInventory(mx, my);
}


void GameEvent::StandardMenuDraw(void)
{
  GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);
  drawBorder();
  menu.DisplayMenu();
  DisplayTextAndFlip();
}

void GameEvent::ChainHappened(void)
{
  GameEvent *newEvent;
  if ((control.chainTrigger==IfEventHappen) && (chainEventHappen > 0))
  {
    if (IsGlobalEvent())
    {
      newEvent = globalData.eventData.GetEvent(chainEventHappen);    
      ReplaceGlobalEvent(newEvent, LevelEvent);
    }
    else
    {
      newEvent = levelData.eventData.GetEvent(chainEventHappen);    
      ReplaceEvent(newEvent, LevelEvent);
    }
    if (newEvent==NULL) PopEvent();
  }
  else if ((control.chainTrigger==AlwaysChain) && (chainEventHappen > 0))
  {
    if (IsGlobalEvent())
    {
      newEvent = globalData.eventData.GetEvent(chainEventHappen);
      ReplaceGlobalEvent(newEvent, LevelEvent);
    }
    else
    {
      newEvent = levelData.eventData.GetEvent(chainEventHappen);
      ReplaceEvent(newEvent, LevelEvent);
    }
    if (newEvent==NULL) PopEvent();
  }
  else
  {
    PopEvent();
  };
}

// this is not called directly by any event, only by the
// task list manager if an event does not trigger
void GameEvent::OnChainNotHappened(void)
{
  GameEvent *newEvent;
  if ((control.chainTrigger == IfEventNotHappen) && (chainEventNotHappen > 0))
  {
    if (IsGlobalEvent())
    {
      newEvent = globalData.eventData.GetEvent(chainEventNotHappen);
      ReplaceGlobalEvent(newEvent, LevelEvent);
    }
    else
    {
      newEvent = levelData.eventData.GetEvent(chainEventNotHappen);
      ReplaceEvent(newEvent, LevelEvent);
    }
    if (newEvent==NULL) PopEvent();
  }
  else if ((control.chainTrigger == AlwaysChain)&&(chainEventHappen > 0))
  {
    if (IsGlobalEvent())
    {
      newEvent = globalData.eventData.GetEvent(chainEventHappen);
      ReplaceGlobalEvent(newEvent, LevelEvent);
    }
    else
    {
      newEvent = levelData.eventData.GetEvent(chainEventHappen);
      ReplaceEvent(newEvent, LevelEvent);
    }
    if (newEvent==NULL) PopEvent();
  }
  else
  {
    PopEvent();
  };
}

void GameEvent::ChainOrQuit(DWORD chainID)
{
  GameEvent *newEvent;
  if (chainID > 0)
  {
    if (IsGlobalEvent())
    {
      newEvent = globalData.eventData.GetEvent(chainID);
      if (newEvent != NULL)
        ReplaceGlobalEvent(newEvent, LevelEvent);
      else
        ChainHappened();
    }
    else
    {
      newEvent = levelData.eventData.GetEvent(chainID);
      if (newEvent != NULL)
        ReplaceEvent(newEvent, LevelEvent);
      else
        ChainHappened();
    }
  }
  else
  {
    ChainHappened();
  };
}

bool GameEvent::HandleTransfer(TRANSFER_DATA transferData, CString msg, PIC_DATA pic)
{ // msg is text to display after transfer is complete.
  // Note that it is call-by-value for a good reason.
  ProcInput.AllowInput(FALSE);
  
  TEXT_EVENT_DATA *textEvent;
  BOOL Execute = transferData.execEvent; // Because transferData will disappear!
  BOOL UseWaitCursor = FALSE;
  StopBackgroundMusic();
  if (transferData.destLevel==GetCurrentLevel())
  {        
    TaskMessage(TASKMSG_TeleportWithinLevel); // Deletes us, too.
  }
  else 
  {
    StopAllSounds();
    UseWaitCursor=TRUE;
    MouseRender.EnableWaitCursor();
    menu.AddOverrideMenu("LOADING...", -1, FALSE);
    OnDraw();
    TaskMessage(TASKMSG_TeleportBetweenLevels); // Deletes us, too.
  };
  //// load level
  party.TeleportParty(transferData);
  
  StartBackgroundMusic();
  if (UseWaitCursor)
    MouseRender.DisableWaitCursor();

  // if not allowed to use Area View in current zone, make sure it is turned off
  if ((levelData.zoneData.zones[levelData.area[party.Posy][party.Posx].zone].allowMap == NoMapping)
      && (!cmdLine.m_forceAV))
  {
    globalData.useAreaView = FALSE;
  }
  else
  {
    if (!cmdLine.m_forceAV)
    {
      switch (globalData.levelInfo.stats[globalData.currLevel].AVStyle)
      {
      case OnlyAreaView: globalData.useAreaView = TRUE;  break;   
      case Only3DView:   globalData.useAreaView = FALSE; break;
      default:           // do nothing 
        break;
      }    
    }
  }

  // turn this off with each transfer
  //globalData.useAreaView = FALSE;

  // I am gone.  'this' points to garbage.  We must be very
  // careful not to use any pointer to us either here or in CProcinp
  if (Execute)
  {
    InsertEvent(new BEGIN_XY_EVENT(
            party.Posx,
            party.Posy,
            GetCurrentLevel()), DeleteEvent);
  };

  if ((msg!="") || (!pic.filename.IsEmpty()))
  {
    textEvent=new TEXT_EVENT_DATA;
    textEvent->text=msg;
    textEvent->pic = pic;
    textEvent->HighlightText=FALSE;
    InsertEvent(textEvent,DeleteEvent);
  }
  PopEvent();
  if (transferData.destLevel!=GetCurrentLevel())
    menu.RemoveOverrideMenu();

  ProcInput.AllowInput(TRUE);
  return false; // Means nothing as far as I know.
}


bool GameEvent::SaveGame(void)
{
  return taskList.SaveTaskStack();
}

bool GameEvent::LoadGame(void)
{
  return taskList.LoadTaskStack();
}

void GameEvent::TaskMessage(TASKMESSAGE msg)
{
  taskList.TaskMessage(msg);
}

GameEvent *GameEvent::EventType2EventPointer(eventType evType, unsigned int evid)
{
  switch (evType)
  {
  case CONTROL_Move:      return new ADVENTURE_MENU_DATA; // was MOVE_MENU_DATA
  case CONTROL_Main:      return new MAIN_MENU_DATA(NULL);
  case CONTROL_Exit:      return new EXIT_DATA;
  case CONTROL_Encamp:    return new ENCAMP_MENU_DATA(NULL);
  case CONTROL_Adventure: return new ADVENTURE_MENU_DATA;
  case CONTROL_BeginStepEvent: return new BEGIN_STEP_EVENT(-1,-1,-1);
  //case CONTROL_BeginRestEvent: return new BEGIN_REST_EVENT(-1,-1);
  case CONTROL_BeginZoneEvent: return new BEGIN_ZONE_EVENT(-1,-1);
  default: 
    {
      ASSERT( (evType < 1000) && (evid > 0) );
      if ( (evType < 1000) && (evid > 0) )
      {
        if (IsGlobalEvent())
          return globalData.eventData.GetEvent(evid);
        else
          return levelData.eventData.GetEvent(evid);
      }
    }
  }
  WriteDebugString("EventType2EventPointer(%u,%u) returning NULL\n", evType,evid);
  ASSERT(FALSE);
  return NULL;
}

int GetTimeInc(BOOL searching)
{
  int inc = 0;
  if (globalData.levelInfo.stats[globalData.currLevel].overland)
  {
    if (searching)
      inc = globalData.WildernessSearchTimeDelta;
    else
      inc = globalData.WildernessTimeDelta;
  }
  else
  {
    if (searching)
      inc = globalData.DungeonSearchTimeDelta;
    else
      inc = globalData.DungeonTimeDelta;
  }
  return inc;
}

void GameEvent::CheckSecretDoors(void)
{
  if (levelData.haveSecretDoor(party.Posx, party.Posy))
  {
    // at least one side of this map square has a secret door
    for (int s=0;s<4;s++)
    {
      if (levelData.haveSecretDoor(party.Posx, party.Posy, s))
      {
        int need;
        for (int i=0;i<party.numCharacters;i++)
        {
          if (party.characters[i].race() == Elf)
          {
            if (party.searching)
              need = 5;
            else
              need = 2;
          }
          else
          {
            if (party.searching)
              need = 2;
            else
              need = 1;
          }

          int result = rollDice(6,1,0.0);
          if (result <= need)
          {
            // success
            party.blockageData.ClearSecret(GetCurrentLevel(), party.Posx, party.Posy, s);
            CString msg;
            CString dirText;
            switch (s) 
            {
            case FACE_NORTH:
               dirText = "North";
               break;
            case FACE_EAST:
               dirText = "East";
               break;
            case FACE_SOUTH:
               dirText = "South";
               break;
            case FACE_WEST:
               dirText = "West";
               break;
            }
            msg.Format("%s discovers a secret door to the %s!", 
                        party.characters[i].GetName(), dirText);
            FormatPausedText(pausedTextData, msg);
            DisplayPausedText(pausedTextData);
            ClearFormattedText(textData);
            break;
          }
        }
      }
    }
  }
}

void GameEvent::UpdatePartyMovementData(void)
{
  int inc = GetTimeInc(party.searching);
  party.incrementClock(inc);

  //for (int i=0;i<inc;i++)
  //  levelData.IncTimeEventTime();
  
  // update visit flag
  party.SetVisited(GetCurrentLevel(), party.Posx,party.Posy);  

  // set current zone into PrevZone so future zone
  // checks will know when zone changes happen
  int zone = levelData.GetCurrZone(party.Posx,party.Posy); 
  levelData.SetPrevZone(zone);

  // update zone step counter
  party.IncZoneStepCount(GetCurrentLevel(), zone);

  if (!cmdLine.m_forceAV)
  {
    // check global view settings for this level
    switch (globalData.levelInfo.stats[globalData.currLevel].AVStyle)
    {
    case OnlyAreaView: globalData.useAreaView = TRUE;  break;   
    case Only3DView:   globalData.useAreaView = FALSE; break;
    default:           // do nothing 
      break;
    }
  }

  // if not allowed to use Area View in current zone, make sure it is turned off
  if ((levelData.zoneData.zones[levelData.area[party.Posy][party.Posx].zone].allowMap == NoMapping)
      && (!cmdLine.m_forceAV))
    globalData.useAreaView = FALSE;

  // Make sure that at least one pc or npc is still alive!
  // Party will normally happen in combat, and is checked afterwards.
  // It can also happen due to spell effects or non-combat events, which
  // is why we need to check at least once per movement.
  int alive = 0;

  // Make sure that only alive dudes are considered active.
  // This only affects events that act upon the active dude.
  int firstAlive = 0;        

  for (int i=0;i<party.numCharacters;i++)
  {
    // sets character name to different color when
    // ready to train.
    party.characters[i].SetReadyToTrain(party.characters[i].IsReadyToTrain());

    if (party.characters[i].GetAge() > party.characters[i].GetMaxAge())
      party.characters[i].SetStatus(Dead);

    if (party.characters[i].GetAdjStatus() == Okay)
    {
      alive++;
      if (firstAlive == -1) firstAlive = i;
    }
  }

  if (!alive)
  {
    PushEvent(new PARTY_DIED_MENU_DATA, DeleteEvent);
    return;
  }
  
  if (GetActiveChar(this).GetAdjStatus() != Okay)
    party.activeCharacter = firstAlive;

  CheckSecretDoors();
}

//////////////////////////////////////////////////////////////////SPLASH_DATA
void SPLASH_DATA::OnInitialEvent(void)
{
  if (ProcInput.HaveWarp)
  {
    PerformWarp();
    return;
  }  

  GameVersionInfo.Format("Ver %01.3f", ENGINE_VER);
  currPos = globalData.titleData.Titles.GetHeadPosition();
  if ((currPos==NULL)||(!LoadNextScreen()))
    EndSplash();
}

bool SPLASH_DATA::LoadNextScreen()
{   
  if (currPos==NULL) 
  {
    if (hImage > 0)
    {
      // fade out current image
      GraphicsMgr.FadeToBlack();
    }
    return false;
  }

  TITLE_SCREEN data = globalData.titleData.Titles.GetNext(currPos);

  if (   (data.DisplayBy == TITLE_SCREEN::tsFadeIn)
      && (!IsFirstScreen))
  {
    // fade out current image
    GraphicsMgr.FadeToBlack();
  }
  
  GraphicsMgr.ReleaseSurface(hImage);

  SurfaceType stype;
  if (data.UseTrans)
    stype = TransBufferDib;
  else
    stype = TitleDib;

  bool usetimer=false;
  bool success=false;
  switch (data.DisplayBy)
  {
  case TITLE_SCREEN::tsSplash:
    hImage = GraphicsMgr.AddSurface(data.TitleBgArt, stype);
    success = (hImage >= 0);
    if (success)
      usetimer = true;
    break;
  case TITLE_SCREEN::tsFadeIn:
    success = (GraphicsMgr.FadeInImage(data.TitleBgArt, stype)?true:false);
    if (success)
    {
      hImage = GraphicsMgr.AddSurface(data.TitleBgArt, stype);
      if (hImage < 0)
        success = false;
      else
        usetimer = true;
    }
    break;
  case TITLE_SCREEN::tsMovie:
    PushEvent(new PLAY_MOVIE_DATA(data.TitleBgArt, PLAY_MOVIE_DATA::movieFull), DeleteEvent);
    success=true;
    break;
  default: 
    ASSERT(FALSE); 
    break;
  }

  if (globalData.titleData.Timeout < 30)
    usetimer=false;

  if ((success)&&(usetimer))
    ((CDungeonApp*)AfxGetApp())->SetIntervalTimer(TASKTIMER_GuidedTour,globalData.titleData.Timeout);

  return success;
}

bool SPLASH_DATA::OnTimer(UINT timerId)
{
  if (timerId != TASKTIMER_GuidedTour)
    return false;
  IsFirstScreen=false;
  if (!LoadNextScreen())
    EndSplash();
  return true;
}

void SPLASH_DATA::OnReturnToTopOfQueue(void)
{
  IsFirstScreen=false;
  if (!LoadNextScreen())
    EndSplash();
}

void SPLASH_DATA::OnDraw(void)
{
  // force image to stretch if needed...
  RECT dstRect;
  dstRect.top=dstRect.left=0;
  dstRect.bottom = SCREEN_HEIGHT;
  dstRect.right = SCREEN_WIDTH;

  if ((hImage >= 0) && (bActive))
  {
    GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);
    GraphicsMgr.BlitImage(0, 0, hImage, NULL, FALSE, BACKBUFFERNUM, &dstRect);
    DisplayTextAndFlip();

    // if first title screen
    if (IsFirstScreen)
    {
      // blit to front buffer, fonts are not loaded yet
      GraphicsMgr.DrawText(VersionText_x,VersionText_y, GameVersionInfo, RGB(255,184,0), FRONTBUFFERNUM);
    }
  }
}

void SPLASH_DATA::OnLeaveQueue(void)
{
  GraphicsMgr.ReleaseSurface(hImage);
}

void SPLASH_DATA::EndSplash()
{
  PlayIntro(FALSE);
  if (tempChars.numChars() > 0)
    ReplaceEvent(new MAIN_MENU_DATA(NULL), DeleteEvent);
  else
    ReplaceEvent(new START_MENU_DATA, DeleteEvent);
  ProcInput.MoveMouseTo(0,0);
}

void SPLASH_DATA::OnKeypress(key_code key, char ascii)
{ 
  if (key == KC_TIMER) return;

  if (ProcInput.HaveWarp)
  {
    PerformWarp();
    return;
  }  

  IsFirstScreen=false;
  if (!LoadNextScreen())
    EndSplash();
}

void SPLASH_DATA::OnMouseClickLeft(int mx, int my)
{
  OnKeypress(KC_RETURN,0);
}

void FindTestCharArt(CHARACTER &dude)
{
  if (dude.icon.filename.IsEmpty())
  {
    dude.icon.filename = DEFAULT_NPC_ICON;
    dude.icon.picType = IconDib;
    dude.icon.SetDefaults();
  }

  if (dude.smallPic.filename.IsEmpty())
  {
    dude.smallPic.filename = DEFAULT_NPC_PIC;
    dude.smallPic.picType = SmallPicDib;
    dude.smallPic.SetDefaults();
  }

  BOOL iexist = FileExists(dude.icon.filename);
  BOOL spexist = FileExists(dude.smallPic.filename);
  if (iexist && spexist)
    return; // no need to search
  
  // scan NPC list for art first
  if (globalData.charData.GetCount()>0)
  {
    POSITION pos = globalData.charData.GetHeadPosition();
    while ((pos != NULL) && ((!iexist)||(!spexist)) )
    {
      if (!iexist)
      {
        if (FileExists(globalData.charData.GetAt(pos).icon.filename))
        {
          dude.icon = globalData.charData.GetAt(pos).icon;
          iexist=TRUE;
        }
      }
      if (!spexist)
      {
        if (FileExists(globalData.charData.GetAt(pos).smallPic.filename))
        {
          dude.smallPic = globalData.charData.GetAt(pos).smallPic;
          spexist=TRUE;
        }
      }
      globalData.charData.GetNext(pos);
    }
  }

  if (iexist && spexist)
    return; // no need to search anymore

  // now try monster art
  if (monsterData.GetCount()>0)
  {
    POSITION pos = monsterData.GetHeadPosition();
    while ((pos != NULL) && (!iexist))
    {
      if (FileExists(monsterData.GetAt(pos).Icon.filename))
      {
        dude.icon = monsterData.GetAt(pos).Icon;
        iexist=TRUE;
      }
      monsterData.GetNext(pos);
    }
  }
}

void SPLASH_DATA::PerformWarp(void)
{
  PlayIntro(FALSE);
  //LoadOffscreenWalkingSurfaces();
  ProcInput.HaveWarp=FALSE;
  ClearFormattedText(textData);
  WriteDebugString("Warping to x,y,level,facing: %i, %i, %i, %i\n",
                   ProcInput.wx,ProcInput.wy,ProcInput.wl,ProcInput.wf);

  CList<CHARACTER, CHARACTER&> PartyMembers;
  CHARACTER dude;

  if (ProcInput.PartyList.IsEmpty())
  {
    // load temp character into party    
    dude.SetAlignment(ChaoticNeutral);
    dude.race((raceType)0);
    dude.SetGender(Male);
    dude.SetClass(Fighter);
    dude.generateNewCharacter(GetDesignCharStartExp(),GetDesignCharStartExpType());
    dude.SetName("Super Spud");
    FindTestCharArt(dude);  
    //dude.serializeCharacter(TRUE, dude.GetName());
    PartyMembers.AddTail(dude);
  }
  else
  {
    POSITION pos = ProcInput.PartyList.GetHeadPosition();
    while (pos != NULL)
    {
      dude.Clear();
      if (dude.serializeCharacter(FALSE, ProcInput.PartyList.GetAt(pos)))
      {
        PartyMembers.AddTail(dude);
      }
      else
      {
        WriteDebugString("Failed to load starting party member %s\n", ProcInput.PartyList.GetAt(pos));
      }

      ProcInput.PartyList.GetNext(pos);
    }
  }

  MouseRender.EnableWaitCursor();
  if (globalData.levelInfo.stats[ProcInput.wl].used)
  {
    if (levelExists(ProcInput.wl))
    {
      StopAllSounds();
      if (loadLevel(ProcInput.wl))
      {
        TRACE("Warp Design/Level data loaded successfully\n");
        party.setPartyAdventureState();
        party.setPartyLevelState(ProcInput.wl, ProcInput.wx, ProcInput.wy, ProcInput.wf);
        
        POSITION pos = PartyMembers.GetHeadPosition();
        while (pos != NULL)
          party.addTempToParty(PartyMembers.GetNext(pos));

        StartBackgroundMusic();
        ReplaceEvent(new ADVENTURE_MENU_DATA, DeleteEvent);
        ProcInput.MoveMouseTo(0,0);
      }
      else 
        WriteDebugString("Failed to load start warp level data\n");
    }
    else
      WriteDebugString("Bogus warp level data, start level does not exist\n");
  }
  else
    WriteDebugString("Bogus warp level data, start level not used\n");
  MouseRender.DisableWaitCursor();
}

bool SPLASH_DATA::OnIdle(void)
{
  if (ProcInput.HaveWarp) PerformWarp();
  return true; // warp failed if we get here
}

//////////////////////////////////////////////////////////////DO_NOTHING_EVENT
bool DO_NOTHING_EVENT::OnIdle(void)
{
  PopEvent();
  return false;
}

void DO_NOTHING_EVENT::OnInitialEvent(void)
{
  menu.reset();
  // this makes the DO_NOTHING_EVENT clear
  // quest text when no question is asked
  //ClearFormattedText(textData);
}

////////////////////////////////////////////////////////////////////EXIT_DATA
void EXIT_DATA::OnKeypress(key_code key, char ascii)
{
  GraphicsMgr.FadeToBlack();
  SignalShutdown();
  ExitSignaled=1;
}

void EXIT_DATA::OnInitialEvent(void)
{
  menu.reset();
  ClearFormattedText(textData);
  ClearFormattedText(errorTextData);
  currPic.Clear();
  StopAllSounds();

  splashScreenStart = virtualGameTime;//timeGetTime();  
  if (!Credits())
  {
    WriteDebugString("Credits screen failed to load\n");
    splashScreenStart += 8000; // don't wait around if credits screen not loaded
  }
}

bool EXIT_DATA::OnIdle(void)
{
  if (ExitSignaled) return true;
  LONGLONG currTime = virtualGameTime;//timeGetTime();
  if (   ((currTime - splashScreenStart) >= 7000)
      || (currTime < splashScreenStart)) // rollover
  {
    simulatedKeyQueue.PushKey(VK_RETURN);
  }  
  return true;  // Wait for input.  There should be none!  The game is over.
}

int EXIT_DATA::OnSaveGame(unsigned int *saveArea)
{
  return 0; // Nothing needs to be saved.
}

int EXIT_DATA::OnLoadGame(unsigned int *saveArea)
{
  return 0;
}

unsigned int EXIT_DATA::OnTaskMessage(TASKMESSAGE msg)
{
  // this one stays put even after a teleport msg
  return 0;
}

///////////////////////////////////////////////////////////////START_MENU_DATA
void START_MENU_DATA::OnKeypress(key_code key, char ascii)
{
  if (TABParty(key)) return;
  if (key != KC_RETURN) 
  {
    StandardMenuKeyboardAction(key, ascii);
    return;
  };
  
  switch (menu.currentItem()) 
  {    
  case 1: // Create new character
    if (tempChars.numChars() < MAX_TEMP_CHARS)
      PushEvent(new CREATE_CHARACTER_DATA, DeleteEvent);
    break;    
  case 2: // Add character to party
    if (party.numCharacters < GetMaxPartyCharacters())
      PushEvent(new ADD_CHARACTER_DATA, DeleteEvent);
    break;    
  case 3: // Load saved Game
    PushEvent(new LOADGAME_MENU_DATA, DeleteEvent);
    break;    
  case 4: // Exit      
    SetMyState(TASK_AskQuitGame);
    PushEvent(new ASK_YES_NO_MENU_DATA("QUIT THE GAME?",TRUE, CONTROL_Main), DeleteEvent);
    //ReplaceEvent(new EXIT_DATA, DeleteEvent);
    break;    
  default:
    WriteDebugString("Unhandled start menu item - handleMenu()\n");
    miscError = InvalidMenuState;
  };
}


void START_MENU_DATA::OnInitialEvent(void)
{
  if (ProcInput.HaveWarp)
  {
    PerformWarp();
    return;
  }  
  SetMyState(TASK_initialized);
  StopAllSounds();
  menu.setMenu(StartMenuData);
  menu.setVertOrient();
  menu.MapKeyCodeToMenuItem(KC_ESCAPE, 4);
  ClearFormattedText(textData);
  ClearFormattedText(errorTextData);
}

void START_MENU_DATA::OnReturnToTopOfQueue(void)
{
  if (GetMyState()==TASK_AskQuitGame)
  {
    if (party.tradeItem==1) // yes, quit the game
    {
      ReplaceEvent(new EXIT_DATA, DeleteEvent);
      return;
    } 
    else 
      OnInitialEvent();
  }
  else
  {
    if (party.numCharacters > 0) 
    {
      ReplaceEvent(new MAIN_MENU_DATA(NULL), DeleteEvent);
      return;
    }
    else 
      OnInitialEvent();
  }
}

void START_MENU_DATA::OnDraw(void)
{
  UpdateStartScreen();
}

void START_MENU_DATA::OnUpdateUI(void)
{

  if (CheckForMultiBoxText()) return;

  if (!globalData.allowCharCreate)
    menu.setItemInactive(1); // create
}
void START_MENU_DATA::PerformWarp(void)
{
  PlayIntro(FALSE);
  ProcInput.HaveWarp=FALSE;
  ClearFormattedText(textData);
  WriteDebugString("Warping to x,y,level,facing: %i, %i, %i, %i\n",
                   ProcInput.wx,ProcInput.wy,ProcInput.wl,ProcInput.wf);

  CList<CHARACTER, CHARACTER&> PartyMembers;
  CHARACTER dude;

  if (ProcInput.PartyList.IsEmpty())
  {
    // load temp character into party    
    dude.SetAlignment(ChaoticNeutral);
    dude.race((raceType)0);
    dude.SetGender(Male);
    dude.SetClass(Fighter);
    dude.generateNewCharacter(GetDesignCharStartExp(),GetDesignCharStartExpType());
    dude.SetName("Super Spud");
    FindTestCharArt(dude);  
    //dude.serializeCharacter(TRUE, dude.GetName());
    PartyMembers.AddTail(dude);
  }
  else
  {
    POSITION pos = ProcInput.PartyList.GetHeadPosition();
    while (pos != NULL)
    {
      dude.Clear();
      if (dude.serializeCharacter(FALSE, ProcInput.PartyList.GetAt(pos)))
      {
        PartyMembers.AddTail(dude);
      }
      else
      {
        WriteDebugString("Failed to load starting party member %s\n", ProcInput.PartyList.GetAt(pos));
      }

      ProcInput.PartyList.GetNext(pos);
    }
  }
  
  MouseRender.EnableWaitCursor();
  if (globalData.levelInfo.stats[ProcInput.wl].used)
  {
    if (levelExists(ProcInput.wl))
    {
      StopAllSounds();
      if (loadLevel(ProcInput.wl))
      {
        TRACE("Warp Design/Level data loaded successfully\n");
        party.setPartyAdventureState();
        party.setPartyLevelState(ProcInput.wl, ProcInput.wx, ProcInput.wy, ProcInput.wf);

        POSITION pos = PartyMembers.GetHeadPosition();
        while (pos != NULL)
          party.addTempToParty(PartyMembers.GetNext(pos));

        StartBackgroundMusic();
        ReplaceEvent(new ADVENTURE_MENU_DATA, DeleteEvent);

        BEGIN_ZONE_EVENT *zoneEvent=NULL;
        zoneEvent=new BEGIN_ZONE_EVENT(party.Posx,
                                       party.Posy);
        // this event will check for, and load, any events that
        // should trigger based on current x,y position in map
        BEGIN_XY_EVENT *xyEvent;
        xyEvent=new BEGIN_XY_EVENT(party.Posx,
                                   party.Posy,
                                   GetCurrentLevel());
        // this event will check for, and load, any step events
        // that are available.
        BEGIN_STEP_EVENT *stepEvent;
        stepEvent=new BEGIN_STEP_EVENT(party.Posx,
                                       party.Posy,
                                       GetCurrentLevel());

/*
        BEGIN_TIME_EVENT *timeEvent;
        timeEvent=new BEGIN_TIME_EVENT(party.Posx,
          party.Posy,
          GetCurrentLevel(),
          party.days, party.hours, party.minutes);
*/

        // queue up the events  
        //PushEvent(timeEvent, DeleteEvent);
        PushEvent(xyEvent,   DeleteEvent);
        PushEvent(stepEvent, DeleteEvent);
        PushEvent(zoneEvent, DeleteEvent);

        ProcInput.MoveMouseTo(0,0);
      }
      else 
        WriteDebugString("Failed to load start warp level data\n");
    }
    else
      WriteDebugString("Bogus warp level data, start level does not exist\n");
  }
  else
    WriteDebugString("Bogus warp level data, start level not used\n");
  MouseRender.DisableWaitCursor();
}

///////////////////////////////////////////////////////////////MAIN_MENU_DATA
void MAIN_MENU_DATA::OnKeypress(key_code key, char ascii)
{
  if (TABParty(key)) return;
  if (key != KC_RETURN) 
  {
    VMenuHPartyKeyboardAction(key,ascii);
    return;
  };
  
  /*
    ** original order ** 
   1 { "Create Character", 0 },
   2 { "Delete Character", 0 },
   3 { "Modify Character", 0 },
   4 { "Train Character", 0 },
   5 { "Human Change Class", 0 },
   6 { "View Character", 0 },
   7 { "Add Character", 0 },
   8 { "Remove Character", 0 },
   9 { "Load Saved Game", 0 },
   10 { "Save Current Game", 0 },
   11 { "Begin Adventuring", 0 },
   12 { "Exit From Game", 1 }

   ** new order ** 
   1 { "Add Character", 0 },
   2 { "Remove Character", 0 },
   3 { "Modify Character", 0 },
   4 { "Train Character", 0 },
   5 { "Human Change Class", 0 },
   6 { "View Character", 0 },
   7 { "Create Character", 0 },
   8 { "Delete Character", 0 },
   9 { "Load Saved Game", 0 },
   10 { "Save Current Game", 0 },
   11 { "Begin Adventuring", 0 },
   12 { "Exit From Game", 1 }

  */

  switch (menu.currentItem()) 
  { 
  case 1: // Add
    if (party.numCharacters < GetMaxPartyCharacters())
      PushEvent(new ADD_CHARACTER_DATA, DeleteEvent);
    break;
       
  case 2: // Remove, Drop
    {
      CString msg;
      msg.Format("REMOVE %s FROM PARTY?", GetActiveChar(this).GetName());
      SetMyState(TASK_MainMenuRemoveChar);
      PushEvent(new ASK_YES_NO_MENU_DATA(msg), DeleteEvent);      
    }
    break;

  case 3: // modify
    character = GetActiveChar(this);
    SetMyState(TASK_MainMenuModifyChar);
    PushEvent(new CHOOSESTATS_MENU_DATA(false), DeleteEvent); // false: use existing charcter
    break;
    
  case 4: // train
    if ((m_pOrigEvent != NULL) && (m_pOrigEvent->GetEventType() == TrainingHallEvent))
    {
      if (GetActiveChar(this).enoughMoney(((TRAININGHALL*)m_pOrigEvent)->Cost,0,0,globalData.moneyData.GetDefaultType()))
      {
        CHARACTER &tempCHAR = GetActiveChar(this);        
        tempCHAR.payForItem(((TRAININGHALL*)m_pOrigEvent)->Cost, globalData.moneyData.GetDefaultType());
        
        int flvl = tempCHAR.GetCurrentLevel(FighterFlag);
        int clvl = tempCHAR.GetCurrentLevel(ClericFlag);
        int rlvl = tempCHAR.GetCurrentLevel(RangerFlag);
        int plvl = tempCHAR.GetCurrentLevel(PaladinFlag);
        int mlvl = tempCHAR.GetCurrentLevel(MagicUserFlag);
        int tlvl = tempCHAR.GetCurrentLevel(ThiefFlag);
        int dlvl = tempCHAR.GetCurrentLevel(DruidFlag);

        tempCHAR.TrainCharacter();               

        int newflvl = tempCHAR.GetCurrentLevel(FighterFlag);
        int newclvl = tempCHAR.GetCurrentLevel(ClericFlag);
        int newrlvl = tempCHAR.GetCurrentLevel(RangerFlag);
        int newplvl = tempCHAR.GetCurrentLevel(PaladinFlag);
        int newmlvl = tempCHAR.GetCurrentLevel(MagicUserFlag);
        int newtlvl = tempCHAR.GetCurrentLevel(ThiefFlag);
        int newdlvl = tempCHAR.GetCurrentLevel(DruidFlag);

        CString temp;
        if (flvl != newflvl)
        {
          temp.Format("%s IS NOW A %i LEVEL %s",
                      tempCHAR.GetName(),
                      newflvl,
                      classData[Fighter].m_name);

          FormatPausedText(pausedTextData, temp);
          DisplayPausedText(pausedTextData);          
        }
        if (clvl != newclvl)
        {
          temp.Format("%s IS NOW A %i LEVEL %s",
            tempCHAR.GetName(),
            newclvl,
            classData[Cleric].m_name);

          FormatPausedText(pausedTextData, temp);
          DisplayPausedText(pausedTextData);          
        }
        if (rlvl != newrlvl)
        {
          temp.Format("%s IS NOW A %i LEVEL %s",
            tempCHAR.GetName(),
            newrlvl,
            classData[Ranger].m_name);
          
          FormatPausedText(pausedTextData, temp);
          DisplayPausedText(pausedTextData);          
        }
        if (plvl != newplvl)
        {
          temp.Format("%s IS NOW A %i LEVEL %s",
            tempCHAR.GetName(),
            newplvl,
            classData[Paladin].m_name);
          
          FormatPausedText(pausedTextData, temp);
          DisplayPausedText(pausedTextData);          
        }
        if (mlvl != newmlvl)
        {
          temp.Format("%s IS NOW A %i LEVEL %s",
            tempCHAR.GetName(),
            newmlvl,
            classData[MagicUser].m_name);
          
          FormatPausedText(pausedTextData, temp);
          DisplayPausedText(pausedTextData);          
        }
        if (tlvl != newtlvl)
        {
          temp.Format("%s IS NOW A %i LEVEL %s",
            tempCHAR.GetName(),
            newtlvl,
            classData[Thief].m_name);
          
          FormatPausedText(pausedTextData, temp);
          DisplayPausedText(pausedTextData);          
        }
        if (dlvl != newdlvl)
        {
          temp.Format("%s IS NOW A %i LEVEL %s",
            tempCHAR.GetName(),
            newdlvl,
            classData[Druid].m_name);
          
          FormatPausedText(pausedTextData, temp);
          DisplayPausedText(pausedTextData);          
        }

        BOOL PickSpells = CanCastMagicUserSpells(tempCHAR.GetClass());

        if (PickSpells)
        {
          SetMyState(TASK_MainMenuTrain_SpellPick);
          PushEvent(new NEW_MU_SPELL_LEVEL_MENU_DATA(tempCHAR), DeleteEvent);
        }
      }
    }
    break;

  case 5: // human change class    
    PushEvent(new HUMAN_CHANGE_CLASS_MENU_DATA(), DeleteEvent);
    break;

  case 6: // View
    PushEvent(new VIEW_CHARACTER_DATA(GetEventType(),NULL), DeleteEvent);
    break; 
           
  case 7: // create
    PushEvent(new CREATE_CHARACTER_DATA, DeleteEvent);
    break;
    
  case 8: // delete
    {
      CString msg;
      msg.Format("%s WILL BE PERMANENTLY REMOVED. CONTINUE?", GetActiveChar(this).GetName());
      SetMyState(TASK_MainMenuDeleteChar);
      PushEvent(new ASK_YES_NO_MENU_DATA(msg), DeleteEvent);      
    }
    break;

  case 9: // Load
    PushEvent(new LOADGAME_MENU_DATA, DeleteEvent);
    break;
    
  case 10: // Save
    PushEvent(new SAVEGAME_MENU_DATA, DeleteEvent);
    break;
    
  case 11: // Begin
    {
      miscError = NoError;
      MouseRender.EnableWaitCursor();
      if (m_pOrigEvent == NULL) // if beginning for first time
      {
        menu.AddOverrideMenu("LOADING...", -1, FALSE);
        OnDraw();
        if (globalData.levelInfo.stats[globalData.startLevel].used)
        {
          if (levelExists(globalData.startLevel))
          {
            if (loadLevel(globalData.startLevel))
            {
              TRACE("Design/Level data loaded successfully\n");
              party.setPartyAdventureState();
              party.setPartyLevelState(globalData.startLevel,
                         globalData.startX, globalData.startY,
                         globalData.startFacing);
              StartBackgroundMusic();
            }
            else 
            {
              WriteDebugString("Failed to load start level data\n");
              miscError = LevelLoadError;
            };
          }
          else
          {
            WriteDebugString("Bogus level data, start level does not exist\n");
            miscError = LevelLoadError;
          };
        }
        else
        {
          WriteDebugString("Bogus level data, start level not used\n");
          miscError = LevelLoadError;
        };
        menu.RemoveOverrideMenu();
      }
      MouseRender.DisableWaitCursor();
      if (NoError == miscError)
      {
        if (m_pOrigEvent != NULL)
        {
          // in this case, we are here because an event
          // pushed a MAIN_MENU_DATA onto the stack
          PopEvent();
        }
        else
        {
          ReplaceEvent(new ADVENTURE_MENU_DATA, DeleteEvent);

          BEGIN_ZONE_EVENT *zoneEvent=NULL;
          zoneEvent=new BEGIN_ZONE_EVENT(party.Posx,
                                         party.Posy);
          // this event will check for, and load, any events that
          // should trigger based on current x,y position in map
          BEGIN_XY_EVENT *xyEvent;
          xyEvent=new BEGIN_XY_EVENT(party.Posx,
                                     party.Posy,
                                     GetCurrentLevel());
          // this event will check for, and load, any step events
          // that are available.
          BEGIN_STEP_EVENT *stepEvent;
          stepEvent=new BEGIN_STEP_EVENT(party.Posx,
                                         party.Posy,
                                         GetCurrentLevel());

 /*
          BEGIN_TIME_EVENT *timeEvent;
           timeEvent=new BEGIN_TIME_EVENT(party.Posx,
             party.Posy,
             GetCurrentLevel(),
             party.days, party.hours, party.minutes);
 */
 
          // queue up the events  
          //PushEvent(restEvent, DeleteEvent);
          //PushEvent(timeEvent, DeleteEvent);
          PushEvent(xyEvent,   DeleteEvent);
          PushEvent(stepEvent, DeleteEvent);
          PushEvent(zoneEvent, DeleteEvent);
        }

        /*
        // test runtimeif lookup
        int i=0;
        char tmp[128];
        ActorType actor = GetActiveChar(this).GetContext();

        for (i=FIRST_CHAR_KEYWORD;i<=LAST_CHAR_KEYWORD;i++)
        {
          if (GetDataAsText(i, tmp, 127, &actor))
            WriteDebugString("CHAR_IF (%s): %s\n", NumToKeywordText(i), tmp);
          else
            ASSERT(FALSE);
        }
        for (i=FIRST_PARTY_KEYWORD;i<=LAST_PARTY_KEYWORD;i++)
        {
          if (GetDataAsText(i, tmp, 127, &actor))
            WriteDebugString("PARTY_IF (%s): %s\n", NumToKeywordText(i), tmp);
          else
            ASSERT(FALSE);
        }
        for (i=FIRST_GAME_KEYWORD;i<=LAST_GAME_KEYWORD;i++)
        {
          if (GetDataAsText(i, tmp, 127, &actor))
            WriteDebugString("GAME_IF (%s): %s\n", NumToKeywordText(i), tmp);
          else
            ASSERT(FALSE);
        }
        */
      }
    }
    break;
    
  case 12: // exit
    SetMyState(TASK_AskQuitGame);
    PushEvent(new ASK_YES_NO_MENU_DATA("QUIT THE GAME?",TRUE, CONTROL_Main), DeleteEvent);
    //ReplaceEvent(new EXIT_DATA, DeleteEvent);
    break;
  };
}

void MAIN_MENU_DATA::OnInitialEvent(void)
{ 
  if (ProcInput.HaveWarp)
  {
    PerformWarp();
    return;
  }  
  SetMyState(TASK_initialized);
  StopAllSounds();
  menu.setMenu(MainMenuData);
  menu.setVertOrient();
  menu.MapKeyCodeToMenuItem(KC_ESCAPE, 12);
  SetMyState(TASK_MainMenu);
  ClearFormattedText(textData);
  ClearFormattedText(errorTextData);
}

void MAIN_MENU_DATA::OnReturnToTopOfQueue(void)
{
  switch (GetMyState())
  {
  case TASK_MainMenuModifyChar:
    GetActiveChar(this) = character;
    break;
  case TASK_MainMenuDeleteChar:
    if (party.tradeItem==1) // yes, delete character
      party.purgeCharacter(party.activeCharacter);
    break;
  case TASK_MainMenuRemoveChar:    
    if (party.tradeItem==1) // yes, remove char
    {
      MouseRender.EnableWaitCursor();
      //GetActiveChar(m_pOrigEvent, party.activeCharacter).serializeCharacter(TRUE, GetActiveChar(m_pOrigEvent, party.activeCharacter).GetName());
      party.removeCharacter(party.activeCharacter);     
      MouseRender.DisableWaitCursor();
    }
    break;
  case TASK_AskQuitGame:
    if (party.tradeItem==1) // yes, quit the game
    {
      ReplaceEvent(new EXIT_DATA, DeleteEvent);
      return;
    } 
    break;
  case TASK_MainMenuTrain_SpellPick:
    break;

  default: break;
  }
  OnInitialEvent();
}

void MAIN_MENU_DATA::OnDraw(void)
{
  UpdateStartScreen();
}

int MAIN_MENU_DATA::OnSaveGame(unsigned int *saveArea)
{
  if (m_pOrigEvent==NULL)
  {
    saveArea[0]=0;
  }
  else
  {
    saveArea[0]=m_pOrigEvent->GetEventId();
  };
  return 1;
}

int MAIN_MENU_DATA::OnLoadGame(unsigned int *saveArea)
{
  if (saveArea[0]==0) 
    m_pOrigEvent=NULL;
  else 
    m_pOrigEvent=levelData.eventData.GetEvent(saveArea[0]);
  return 1;
}

void MAIN_MENU_DATA::OnUpdateUI(void)
{
  /*
    ** original order ** 
   1 { "Create Character", 0 },
   2 { "Delete Character", 0 },
   3 { "Modify Character", 0 },
   4 { "Train Character", 0 },
   5 { "Human Change Class", 0 },
   6 { "View Character", 0 },
   7 { "Add Character", 0 },
   8 { "Remove Character", 0 },
   9 { "Load Saved Game", 0 },
   10 { "Save Current Game", 0 },
   11 { "Begin Adventuring", 0 },
   12 { "Exit From Game", 1 }

   ** new order ** 
   1 { "Add Character", 0 },
   2 { "Remove Character", 0 },
   3 { "Modify Character", 0 },
   4 { "Train Character", 0 },
   5 { "Human Change Class", 0 },
   6 { "View Character", 0 },
   7 { "Create Character", 0 },
   8 { "Delete Character", 0 },
   9 { "Load Saved Game", 0 },
   10 { "Save Current Game", 0 },
   11 { "Begin Adventuring", 0 },
   12 { "Exit From Game", 1 }
  */

  if (CheckForMultiBoxText()) return;

  if (IsGlobalEvent())
    menu.setItemInactive(10); // save

  if (IsCombatActive())
    menu.setItemInactive(10); // save

  if (m_pOrigEvent != NULL)
  {
    if (m_pOrigEvent->IsGlobalEvent())
      menu.setItemInactive(10); // save

    if (m_pOrigEvent->GetEventType() != TrainingHallEvent)
    {
      menu.setItemInactive(4); // train
      menu.setItemInactive(5); // change class
    }
    else
    {
      if (!GetActiveChar(this).GetAdjReadyToTrain())
        menu.setItemInactive(4); // train

      if (GetActiveChar(this).poolCharacterGold() < ((TRAININGHALL*)m_pOrigEvent)->Cost)
        menu.setItemInactive(4); // train

      if (!GetActiveChar(this).CanChangeClassHuman())
        menu.setItemInactive(5); // change class

      //
      // not handling multi-class properly here!
      //
      if (   (!((TRAININGHALL*)m_pOrigEvent)->TrainCleric)
          && (GetActiveChar(this).GetClass() == Cleric))
        menu.setItemInactive(4); // train
      if (   (!((TRAININGHALL*)m_pOrigEvent)->TrainFighter)
          && (GetActiveChar(this).GetClass() == Fighter))
        menu.setItemInactive(4); // train
      if (   (!((TRAININGHALL*)m_pOrigEvent)->TrainMagicUser)
          && (GetActiveChar(this).GetClass() == MagicUser))
        menu.setItemInactive(4); // train
      if (   (!((TRAININGHALL*)m_pOrigEvent)->TrainThief)
          && (GetActiveChar(this).GetClass() == Thief))
        menu.setItemInactive(4); // train
      if (   (!((TRAININGHALL*)m_pOrigEvent)->TrainRanger)
          && (GetActiveChar(this).GetClass() == Ranger))
        menu.setItemInactive(4); // train
      if (   (!((TRAININGHALL*)m_pOrigEvent)->TrainPaladin)
          && (GetActiveChar(this).GetClass() == Paladin))
        menu.setItemInactive(4); // train
      if (   (!((TRAININGHALL*)m_pOrigEvent)->TrainDruid)
          && (GetActiveChar(this).GetClass() == Druid))
        menu.setItemInactive(4); // train
    }
  }
  else
  {
    menu.setItemInactive(4); // train
    menu.setItemInactive(5); // change class
  }

  int pcAvail=0;
  // check for active characters
  for (int i=0;((i<party.numCharacters) && (!pcAvail));i++)
  {
    if (party.characters[i].GetAdjHitPoints() < 0)
      party.characters[i].SetStatus(Dead);
    if (party.characters[i].GetAdjStatus() == Okay)
      pcAvail++;
  }
  if (!pcAvail)
    menu.setItemInactive(11); // Begin Adventuring

  if (!EditorRunning)
  {
    if (party.numCharacters < globalData.minPartySize)
      menu.setItemInactive(11); // Begin Adventuring
    if (party.numCharacters > globalData.maxPartySize)
      menu.setItemInactive(11); // Begin Adventuring
  }

  if (!GetActiveChar(this).CanBeModified())
    menu.setItemInactive(3); // modify

  if (party.numCharacters == 0)
  {
    menu.setItemInactive(8); // delete
    menu.setItemInactive(3); // modify
    menu.setItemInactive(6); // view
    menu.setItemInactive(2); // remove
    menu.setItemInactive(11); // begin
  }

  //if (tempChars.numChars() <= 0)
  //  menu.setItemInactive(1); // add
  
  if (party.numCharacters >= GetMaxPartyCharacters())
    menu.setItemInactive(1); // add
  if (party.numCharacters >= globalData.maxPartySize)
    menu.setItemInactive(1); // add

  if (!globalData.allowCharCreate)
    menu.setItemInactive(7); // create

  if (tempChars.numChars() >= MAX_TEMP_CHARS)
  {
    menu.setItemInactive(7); // create
    menu.setItemInactive(2); // remove
  }
  // NPC is highlighted
  if (GetActiveChar(this).GetType() == NPC_TYPE)
  {
    //menu.setItemInactive(8); // delete
    menu.setItemInactive(3); // modify    
  }
}

void MAIN_MENU_DATA::PerformWarp(void)
{
  PlayIntro(FALSE);
  ProcInput.HaveWarp=FALSE;
  ClearFormattedText(textData);
  WriteDebugString("Warping to x,y,level,facing: %i, %i, %i, %i\n",
                   ProcInput.wx,ProcInput.wy,ProcInput.wl,ProcInput.wf);

  CList<CHARACTER, CHARACTER&> PartyMembers;
  CHARACTER dude;

  if (ProcInput.PartyList.IsEmpty())
  {
    // load temp character into party    
    dude.SetAlignment(ChaoticNeutral);
    dude.race((raceType)0);
    dude.SetGender(Male);
    dude.SetClass(Fighter);
    dude.generateNewCharacter(GetDesignCharStartExp(),GetDesignCharStartExpType());
    dude.SetName("Super Spud");
    FindTestCharArt(dude);  
    //dude.serializeCharacter(TRUE, dude.GetName());
    PartyMembers.AddTail(dude);
  }
  else
  {
    POSITION pos = ProcInput.PartyList.GetHeadPosition();
    while (pos != NULL)
    {
      dude.Clear();
      if (dude.serializeCharacter(FALSE, ProcInput.PartyList.GetAt(pos)))
      {
        PartyMembers.AddTail(dude);
      }
      else
      {
        WriteDebugString("Failed to load starting party member %s\n", ProcInput.PartyList.GetAt(pos));
      }

      ProcInput.PartyList.GetNext(pos);
    }
  }

  MouseRender.EnableWaitCursor();
  if (globalData.levelInfo.stats[ProcInput.wl].used)
  {
    if (levelExists(ProcInput.wl))
    {
      StopAllSounds();
      if (loadLevel(ProcInput.wl))
      {
        TRACE("Warp Design/Level data loaded successfully\n");
        party.setPartyAdventureState();
        party.setPartyLevelState(ProcInput.wl, ProcInput.wx, ProcInput.wy, ProcInput.wf);

        POSITION pos = PartyMembers.GetHeadPosition();
        while (pos != NULL)
          party.addTempToParty(PartyMembers.GetNext(pos));

        StartBackgroundMusic();
        ReplaceEvent(new ADVENTURE_MENU_DATA, DeleteEvent);

        BEGIN_ZONE_EVENT *zoneEvent=NULL;
        zoneEvent=new BEGIN_ZONE_EVENT(party.Posx,
                                       party.Posy);
        // this event will check for, and load, any events that
        // should trigger based on current x,y position in map
        BEGIN_XY_EVENT *xyEvent;
        xyEvent=new BEGIN_XY_EVENT(party.Posx,
                                   party.Posy,
                                   GetCurrentLevel());
        // this event will check for, and load, any step events
        // that are available.
        BEGIN_STEP_EVENT *stepEvent;
        stepEvent=new BEGIN_STEP_EVENT(party.Posx,
                                       party.Posy,
                                       GetCurrentLevel());

/*
        BEGIN_TIME_EVENT *timeEvent;
        timeEvent=new BEGIN_TIME_EVENT(party.Posx,
          party.Posy,
          GetCurrentLevel(),
          party.days, party.hours, party.minutes);*/


        // queue up the events  
        //PushEvent(timeEvent, DeleteEvent);
        PushEvent(xyEvent,   DeleteEvent);
        PushEvent(stepEvent, DeleteEvent);
        PushEvent(zoneEvent, DeleteEvent);

        ProcInput.MoveMouseTo(0,0);
      }
      else 
        WriteDebugString("Failed to load start warp level data\n");
    }
    else
      WriteDebugString("Bogus warp level data, start level does not exist\n");
  }
  else
    WriteDebugString("Bogus warp level data, start level not used\n");
  MouseRender.DisableWaitCursor();
}
////////////////////////////////////////////////////////CREATE_CHARACTER_DATA
void CREATE_CHARACTER_DATA::OnInitialEvent(void)
{
  menu.reset();
  character.Clear();
  //GraphicsMgr.EnableFontColorTags(FALSE);
  SetMyState(TASK_Create_Character_Start);
  ClearFormattedText(textData);
}

bool CREATE_CHARACTER_DATA::OnIdle(void)
{
  if (m_AbortCharCreation)
  {
    //GraphicsMgr.EnableFontColorTags(TRUE);
    PopEvent(); // user signaled to exit from character creation
    return true; // break out of OnIdle loop
  }

  switch (GetMyState())
  {
  case TASK_Create_Character_Start:
      SetMyState(TASK_Create_Character_Race);
      PushEvent(new RACE_MENU_DATA(this),DeleteEvent);
      break;
  case TASK_Create_Character_Race:
      SetMyState(TASK_Create_Character_Gender);
      PushEvent(new GENDER_MENU_DATA(this), DeleteEvent);
      break;
  case TASK_Create_Character_Gender:
      SetMyState(TASK_Create_Character_Class);
      PushEvent(new CLASS_MENU_DATA(this), DeleteEvent);
      break;
  case TASK_Create_Character_Class:
      SetMyState(TASK_Create_Character_Alignment);
      PushEvent(new ALIGNMENT_MENU_DATA(this), DeleteEvent);
      break;
  case TASK_Create_Character_Alignment:
      SetMyState(TASK_Create_Character_ChooseStats);
      PushEvent(new CHOOSESTATS_MENU_DATA(true), DeleteEvent); // true: generate new character
      break;
  case TASK_Create_Character_ChooseStats:
      SetMyState(TASK_Create_Character_GetCharName);
      PushEvent(new GETCHARNAME_MENU_DATA, DeleteEvent);
      break;
  case TASK_Create_Character_GetCharName:
      SetMyState(TASK_Create_Character_IconPick);
      PushEvent(new GETCHARICON_MENU_DATA, DeleteEvent);
      break;
  case TASK_Create_Character_IconPick:
      SetMyState(TASK_Create_Character_SmallPicPick);
      PushEvent(new GETCHARSMALLPIC_MENU_DATA, DeleteEvent);
      break;
  case TASK_Create_Character_SmallPicPick:
    {
      character.UpdateSkillBasedStats();

      // Don't assume that newly created characters are first level. Default
      // settings may provide enough experience points to make the new 
      // character a higher level.      

      BOOL PickMUSpells = CanCastMagicUserSpells(character.GetClass());
      
      if (PickMUSpells)
      {
        SetMyState(TASK_Create_Character_SpellPick);
        PushEvent(new NEW_MU_SPELL_LEVEL_MENU_DATA(character), DeleteEvent);
      }
      else
      {        
        SetMyState(TASK_Create_Character_AskToSave);
        CString msg;
        msg.Format("DO YOU WANT TO SAVE %s?", character.GetName());
        PushEvent(new ASK_YES_NO_MENU_DATA(msg,TRUE, CONTROL_CreateCharacter), DeleteEvent);
      }
    }
    break;
  case TASK_Create_Character_SpellPick:      
    {
      SetMyState(TASK_Create_Character_AskToSave);
      CString msg;
      msg.Format("DO YOU WANT TO SAVE %s?", character.GetName());
      PushEvent(new ASK_YES_NO_MENU_DATA(msg,TRUE, CONTROL_CreateCharacter), DeleteEvent);
    }
    break;
  case TASK_Create_Character_AskToSave:
    if (party.tradeItem==1) // yes, save character
    {
      MouseRender.EnableWaitCursor();
      character.serializeCharacter(TRUE, character.GetName());
      MouseRender.DisableWaitCursor();
    }
    //GraphicsMgr.EnableFontColorTags(TRUE);
    PopEvent(); // All done
    break;

  default:
    //GraphicsMgr.EnableFontColorTags(TRUE);
    WriteDebugString("Bogus state in CREATE_CHARACTER_DATA\n");
    ASSERT(FALSE);
    PopEvent();
  };
  return false;
}

///////////////////////////////////////////////////////////ADD_CHARACTER_DATA
void ADD_CHARACTER_DATA::OnKeypress(key_code key, char ascii)
{  
  if (key != KC_RETURN)
  {
    StandardMenuKeyboardAction(key,ascii);
    return;
  };
  int count,item,i;
  char temp[20];
  CHARACTER tempCHARACTER;

  if (menu.currentItem() == menu.totalItems()) // exit menu
  {
    for (i=0;i<tempChars.numChars();i++)
    {
      tempCHARACTER = tempChars.GetCharacter(i);
      if (tempChars.InParty(i)) // add to party
      {
        party.addTempToParty(tempCHARACTER);
      }
      else // remove from party
      {
        for (int j=0;j<party.numCharacters;j++)
        {
          if (party.characters[j].IsSameCharacter(tempCHARACTER))
            party.removeCharacter(j,FALSE);
        }
      }
    }    
    PopEvent();
    return;
  }
  else // toggle in-party status for selected character
  {
    item=menu.currentItem()-1;    
    if (tempChars.InParty(item))
    {
      menu.changeMenuItem(item+1, tempChars.GetCharName(item));
      tempChars.SetInParty(item, FALSE);
    }
    else 
    {
      count=tempChars.GetInPartyCount();
      if (count  < GetMaxPartyCharacters())
      {
        sprintf(temp, "* %s", (LPCSTR)tempChars.GetCharName(item));
        menu.changeMenuItem(item+1, temp);
        tempChars.SetInParty(item, TRUE);
      }
    }
  }
}
  
void ADD_CHARACTER_DATA::OnInitialEvent(void)
{
  ClearFormattedText(textData);
  menu.reset();
  menu.setStartCoord(AddCharMenuX, AddCharMenuY);

  tempChars.Clear();
  // add saved characters, doesn't include PreGen NPCs
  tempChars.AddSavedCharacters();

  // add pre-gen NPC's
  POSITION pos = globalData.charData.GetHeadPosition();
  while (pos != NULL)
  {
    if (globalData.charData.GetAt(pos).IsPreGen)
      tempChars.addPreGenCharacter(globalData.charData.GetAt(pos), globalData.charData.GetAt(pos).key);    
    globalData.charData.GetNext(pos);
  }

  for (int i=0;i<tempChars.numChars();i++)
  {
    if (tempChars.InParty(i))
    {
      CString tmp;
      tmp.Format("* %s", tempChars.GetCharName(i));
      menu.addMenuItem(tmp);
    }
    else
      menu.addMenuItem(tempChars.GetCharName(i));
  }
  menu.addMenuItem("EXIT", 1);
  menu.MapKeyCodeToMenuItem(KC_ESCAPE, menu.totalItems());  
}

void ADD_CHARACTER_DATA::OnDraw(void)
{
  StandardMenuDraw();
}

////////////////////////////////////////////////////////////////RACE_MENU_DATA
void RACE_MENU_DATA::OnKeypress(key_code key, char ascii)
{
  if (key != KC_RETURN)
  {
    HMenuVItemsKeyboardAction(key, ascii);
    return;
  }
  switch (menu.currentItem()) 
  {
  case 1: // select
    {
      raceType rt = (raceType)spellListText.GetSpellIndex(currSpellTextIndex());
      character.race(rt);
      PopEvent();
    }
    break;    
  case 2: // next
    nextSpellPage();
    break;
  case 3: // prev
    prevSpellPage();
    break;
  case 4: // exit
    m_pOrig->m_AbortCharCreation=true;
    PopEvent(); // pop to exit event
    break;
  default:
    WriteDebugString("Unhandled menu item in RACE_MENU_DATA");
    break;
  }   
}
void RACE_MENU_DATA::OnMouseClickLeft(int mx, int my)
{  
  if (MouseInventory(mx, my))
    simulatedKeyQueue.PushKey(VK_RETURN);
  else
    MouseMenu(mx, my);
}

void RACE_MENU_DATA::OnInitialEvent(void)
{
  ClearFormattedText(textData);
  //menu.setMenu(RaceMenuData);
  //menu.setVertOrient();    

  menu.setMenu(SelectMenuData);
  menu.setHorzOrient();
  menu.MapKeyCodeToMenuItem(KC_ESCAPE, 4);

  clearSpellItemState();
  spellListText.FormatSpellListTitle("Choose a character race:");
  spellListText.m_flags = SHOW_DESIGN_FLAG;    
  POSITION pos = raceData.GetHeadPosition();
  while (pos!=NULL)
  {
    spellListText.Add(raceData.GetKeyAt(pos));
    raceData.GetNext(pos);
  }

  int scnt = spellListText.GetCount();
  ItemsOnPage = min(Items_Per_Page, scnt);
  if (ItemsOnPage > 0)
  {
    if (party.activeItem >= spellListText.GetCount())
      party.activeItem = (BYTE)(spellListText.GetCount()-1);
    if (party.activeItem >= (startItem+ItemsOnPage))
      party.activeItem = (BYTE)((startItem+ItemsOnPage)-1);
    if (party.activeItem < 0)
      party.activeItem = 0;
  }
  else
  {
    party.activeItem = 0;
    startItem = 0;
    //currShopItem = 0;
  }
}

void RACE_MENU_DATA::OnUpdateUI(void)
{
  if (CheckForMultiBoxText()) return;
  
  menu.setAllItemsActive();
  if (spellListText.GetCount() <= 0)
    menu.setItemInactive(1); // select    
  if (!allowSpellNextPage())
    menu.setItemInactive(2); // next
  if (!allowSpellPrevPage())
    menu.setItemInactive(3); // prev
}

void RACE_MENU_DATA::OnDraw(void)
{
  //StandardMenuDraw();
  UpdateRaceMenuScreen();
}

/////////////////////////////////////////////////////GETCHARSMALLPIC_MENU_DATA
void GETCHARSMALLPIC_MENU_DATA::OnInitialEvent(void)
{
  ClearFormattedText(textData);
  menu.setMenu(GetCharSmallPicMenuData);
  menu.setHorzOrient();

  // find all small pics in design folder
  int i=1;
  CString temp;
  PIC_DATA spic;
  spic.picType = SmallPicDib;
  spic.SetDefaults();  
  int origCount = m_SmallPicImport.GetCount();

  while (i <= 50)
  {
   temp.Format("prt_SPic%u.png", i);
   // uses root name to search for any matches
   // using valid image extensions (bmp, pcx, etc)
   if (FindImageWithValidExt(temp))
   {
     spic.filename = temp;
     // make sure this pic is not already in the list
     // only check 1..origCount
     POSITION pos = m_SmallPicImport.GetHeadPosition();
     int c=0;
     BOOL found=FALSE;
     while ((pos != NULL) && (c < origCount) && (!found))
     {
       if (m_SmallPicImport.GetAt(pos).filename == spic.filename)
         found = TRUE;
       c++;
       m_SmallPicImport.GetNext(pos);
     }
     if (!found)
       m_SmallPicImport.AddTail(spic);
   }
   i++;
  }

  numSmallPics = m_SmallPicImport.GetCount();
  character.smallPic.FreePic();
  currSmallPicSelected = 0;
  if (numSmallPics > 0)
  {
    for (i=0;i<numSmallPics;i++)
    {
      if (character.smallPic == m_SmallPicImport.GetAt(m_SmallPicImport.FindIndex(i)))
      {
        currSmallPicSelected = i;
        break;
      }
    }
    character.smallPic = m_SmallPicImport.GetAt(m_SmallPicImport.FindIndex(currSmallPicSelected));
    character.smallPic.picType = SmallPicDib;
    character.smallPic.LoadPicSurfaces("prt_");
  }
  else 
  {
    character.smallPic.Clear();
    miscError = NoSmallPics;
    //PopEvent();
  };
}
void GETCHARSMALLPIC_MENU_DATA::OnUpdateUI(void)
{
  if (CheckForMultiBoxText()) return;
  if (numSmallPics <= 1)
  {
    menu.setItemInactive(1); // next
    menu.setItemInactive(2); // prev
  }    
}

void GETCHARSMALLPIC_MENU_DATA::OnDraw(void)
{
  UpdateSmallPicScreen();
}

void GETCHARSMALLPIC_MENU_DATA::OnKeypress(key_code key, char ascii)
{
  if (key != KC_RETURN) 
  {
    StandardMenuKeyboardAction(key,ascii);
    return;
  };

  switch (menu.currentItem()) 
  {
  case 1: // next
    if (currSmallPicSelected < numSmallPics-1)
      currSmallPicSelected++;
    else
      currSmallPicSelected = 0;    
    character.smallPic.FreePic();
    character.smallPic = m_SmallPicImport.GetAt(m_SmallPicImport.FindIndex(currSmallPicSelected));
    character.smallPic.picType = SmallPicDib;
    character.smallPic.LoadPicSurfaces("prt_");
    break;    
  case 2: // prev
    if (currSmallPicSelected > 0)
      currSmallPicSelected--;
    else
      currSmallPicSelected = numSmallPics-1;

    character.smallPic.FreePic();
    character.smallPic = m_SmallPicImport.GetAt(m_SmallPicImport.FindIndex(currSmallPicSelected));
    character.smallPic.picType = SmallPicDib;
    character.smallPic.LoadPicSurfaces("prt_");
    break;    
  case 3: // select
    character.smallPic.FreePic();
    currPic.Clear();
    PopEvent();
    break;    
  default:
    WriteDebugString("Unhandled menu item in handleSmallPicPick");
    miscError = InvalidMenuState;
    PopEvent();
    break;
  }  
}
/////////////////////////////////////////////////////////GETCHARICON_MENU_DATA
void GETCHARICON_MENU_DATA::OnKeypress(key_code key, char ascii)
{
  if (key != KC_RETURN)
  {
    StandardMenuKeyboardAction(key, ascii);
    return;
  };
 
  switch (menu.currentItem()) 
  {
  case 1: // next
    if (currIconSelected < numIconPics-1)
      currIconSelected++;
    else
      currIconSelected = 0;

    character.icon.FreePic();
    character.icon = m_IconPicImport.GetAt(m_IconPicImport.FindIndex(currIconSelected));
    character.icon.picType = IconDib;
    character.icon.LoadPicSurfaces("cn_");
    break;
    
  case 2: // prev
    if (currIconSelected > 0)
      currIconSelected--;
    else
      currIconSelected = numIconPics-1;
    character.icon.FreePic();
    character.icon = m_IconPicImport.GetAt(m_IconPicImport.FindIndex(currIconSelected));
    character.icon.picType = IconDib;
    character.icon.LoadPicSurfaces("cn_");
    break;
    
  case 3: // select
    character.icon.FreePic();
    currPic.Clear();
    GraphicsMgr.ReleaseSurface(SpriteBkGndArt);
    PopEvent(); // All done.
    break;
    
  default:
    WriteDebugString("Unhandled menu item in handleIconPicPick");
    miscError = InvalidMenuState;
    PopEvent();
    break;
  }
}

void GETCHARICON_MENU_DATA::OnInitialEvent(void)
{
  ClearFormattedText(textData);
  menu.setMenu(GetCharIconMenuData);
  menu.setHorzOrient();        
  if (SpriteBkGndArt > 0)
    GraphicsMgr.ReleaseSurface(SpriteBkGndArt);
  character.icon.FreePic();

  // find all small pics in design folder
  int i=1;
  CString temp;
  PIC_DATA spic;
  spic.picType = IconDib;
  spic.SetDefaults();
  int origCount = m_IconPicImport.GetCount();

  while (i <= 50)
  {
     temp.Format("cn_Icon%u.png", i);
     // uses root name to search for any matches
     // using valid image extensions (bmp, pcx, etc)
     if (FindImageWithValidExt(temp))
     {
       spic.filename = temp;
       // make sure this pic is not already in the list
       // only check 1..origCount
       POSITION pos = m_IconPicImport.GetHeadPosition();
       int c=0;
       BOOL found=FALSE;
       
       while ((pos != NULL) && (c < origCount) && (!found))
       {
         CString ConfigIconFilename(m_IconPicImport.GetAt(pos).filename);
         StripFilenamePath(ConfigIconFilename);
         CString FoundIconFilename(spic.filename);
         StripFilenamePath(FoundIconFilename);

         if (ConfigIconFilename.CompareNoCase(FoundIconFilename) == 0)
           found = TRUE;

         c++;
         m_IconPicImport.GetNext(pos);
       }

       if (!found)
         m_IconPicImport.AddTail(spic);
     }
     i++;
  }

  numIconPics = m_IconPicImport.GetCount();
  character.icon.FreePic();
  currIconSelected = 0;
  if (numIconPics > 0)
  {
    for (i=0;i<numIconPics;i++)
    {
      if (character.icon == m_IconPicImport.GetAt(m_IconPicImport.FindIndex(i)))
      {
        currIconSelected = i;
        break;
      }
    }
    SpriteBkGndArt = GraphicsMgr.AddSurface(globalData.IconBgArt, CommonDib);
    character.icon = m_IconPicImport.GetAt(m_IconPicImport.FindIndex(currIconSelected));
    character.icon.picType=IconDib;
    character.icon.LoadPicSurfaces("cn_");
  }
  else 
  {
    character.icon.Clear();
    miscError = NoIcons;
    return;
  }
}

void GETCHARICON_MENU_DATA::OnUpdateUI(void)
{
  if (CheckForMultiBoxText()) return;
  if (numIconPics <= 1)
  {
    menu.setItemInactive(1); // next
    menu.setItemInactive(2); // prev
  }
}

void GETCHARICON_MENU_DATA::OnDraw(void)
{
  UpdateIconPickScreen();
}

/////////////////////////////////////////////////////////GETCHARNAME_MENU_DATA
void GETCHARNAME_MENU_DATA::OnKeypress(key_code key, char ascii)
{
  char temp[6];
  int i;
  if (key == KC_RETURN)
  {
	  char newName[50];
	  newName[0] = '\0';
    // ignore blank on the end
    for (i=1;i<menu.totalItems();i++)
      strcat(newName, menu.getMenuItem(i));
    character.SetName(newName);
    bool isdup=false;
    for (i=0;i<party.numCharacters;i++)
    {
      if (party.characters[i].IsSameCharacter(character))
        isdup=true;
    }
    if (tempChars.IsCharacterInTemps(character))
      isdup=true;
    if (!isdup)
    {
      if (menu.totalItems() > 1)
      {
        tempChars.Clear();
        PopEvent();
      }
    }
    else
    {
      name = character.GetName();
      CString msg;
      msg.Format("%s ALREADY EXISTS AS A SAVED CHARACTER, REPLACE?", name);
      PushEvent(new ASK_YES_NO_MENU_DATA(msg), DeleteEvent);      
      //miscError=DupCharName;
    }
    return; // Need at least one character
  }
  else if (( (key == KC_CHAR) 
          || (key == KC_NUM) 
          || (key == KC_SPACE))
             && (menu.totalItems() < MAX_CHAR_NAME+1)) // 15 chars + blank on end
  {
    // dont allow space to be first char in name
    if ((menu.totalItems() == 1)  && (key == KC_SPACE))
      return;
    sprintf(temp, "%c", ascii);
    menu.changeMenuItem(menu.totalItems(), temp);
    menu.addMenuItem(" ");
    menu.setCurrentItem(menu.totalItems());
    return;
  }
  else if (   ((key == KC_BACK) || (key == KC_LEFT))
           && (menu.totalItems() > 0) )
  {
    menu.deleteLastMenuItem();
    menu.changeMenuItem(menu.totalItems(), " ");
    menu.setItemActive(menu.totalItems());
    if (menu.totalItems() == 0)
      menu.addMenuItem(" ");    
    menu.setCurrentItem(menu.totalItems());
    return;
  };
  return;
}


void GETCHARNAME_MENU_DATA::OnInitialEvent(void)
{
  ClearFormattedText(textData);
  menu.reset();
  menu.setStartCoord(USE_DEFAULT_HORZ_MENU, 0);
  menu.setTitle(GetCharNameTitle);
  menu.setHorzOrient();
  menu.setItemSeparation(0);
  menu.addMenuItem(" ");
  menu.setCurrentItem(menu.totalItems());
  currPic = character.smallPic;
  currPic.LoadPicSurfaces("prt_");     
  
  tempChars.Clear();
  // add saved characters, doesn't include NPCs
  tempChars.AddSavedCharacters();
  // add pre-gen NPC's
  POSITION pos = globalData.charData.GetHeadPosition();
  while (pos != NULL)
  {
    if (globalData.charData.GetAt(pos).IsPreGen)
      tempChars.addPreGenCharacter(globalData.charData.GetAt(pos), globalData.charData.GetAt(pos).key);    
    globalData.charData.GetNext(pos);
  }
}

void GETCHARNAME_MENU_DATA::OnReturnToTopOfQueue(void)
{
  if (party.tradeItem!=1) // no, don't replace character
  {    
    ClearFormattedText(textData);
    currPic = character.smallPic;
    currPic.LoadPicSurfaces("prt_");     

    menu.reset();
    menu.setStartCoord(USE_DEFAULT_HORZ_MENU, 0);
    menu.setTitle(GetCharNameTitle);
    menu.setHorzOrient();
    menu.setItemSeparation(0);
    menu.addMenuItem(" ");
    menu.setCurrentItem(menu.totalItems());
    char temp[6];
    for (int i=0;i<name.GetLength();i++)
    {
      sprintf(temp, "%c", name[i]);
      menu.changeMenuItem(menu.totalItems(), temp);
      menu.addMenuItem(" ");
      menu.setCurrentItem(menu.totalItems());
    }
  }
  else
    PopEvent();

}

void GETCHARNAME_MENU_DATA::OnDraw(void)
{
  GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);
  drawBorder();

  drawViewportFrame(VPFrameXOffset, VPFrameY+VPFrameYOffset);
  if (currPic.key > 0)
  {
    if (currPic.picType == SmallPicDib)
    {
       GraphicsMgr.BlitSprite(VPFrameXOffset+(ViewportX-VPFrameX), 
                              VPFrameY+VPFrameYOffset+(ViewportY-VPFrameY), 
                              currPic.key);
    }
  }

  if (errorTextData.numLines == 0)
    displayCharStats(character);
  menu.DisplayMenu();
  DisplayTextAndFlip();
}

////////////////////////////////////////////////////////////////GENDER_MENU_DATA
void GENDER_MENU_DATA::OnKeypress(key_code key, char ascii)
{
  if (key != KC_RETURN)
  {
    HMenuVItemsKeyboardAction(key, ascii);
    return;
  }
  switch (menu.currentItem()) 
  {
  case 1: // select
    {
      genderType type = (genderType)spellListText.GetSpellIndex(currSpellTextIndex());
      character.SetGender(type);
      PopEvent();
    }
    break;    
  case 2: // next
    nextSpellPage();
    break;
  case 3: // prev
    prevSpellPage();
    break;
  case 4: // exit
    m_pOrig->m_AbortCharCreation=true;
    PopEvent(); // pop to exit event
    break;
  default:
    WriteDebugString("Unhandled menu item in GENDER_MENU_DATA");
    break;
  }
}

void GENDER_MENU_DATA::OnMouseClickLeft(int mx, int my)
{  
  if (MouseInventory(mx, my))
    simulatedKeyQueue.PushKey(VK_RETURN);
  else
    MouseMenu(mx, my);
}

//NUM_GENDER_TYPES
void GENDER_MENU_DATA::OnInitialEvent(void)
{  
  ClearFormattedText(textData);
  menu.setMenu(SelectMenuData);
  menu.setHorzOrient();
  menu.MapKeyCodeToMenuItem(KC_ESCAPE, 4);
  clearSpellItemState();

  CString title("Choose a character gender:");
  CString tmp;
  tmp.Format(" ( %s )", raceData.GetRaceName(character.race()));
  title += tmp;
  spellListText.FormatSpellListTitle(title);
  spellListText.m_flags = SHOW_DESIGN_FLAG;    
  
  spellListText.Add(Male);
  spellListText.Add(Female);

  int scnt = spellListText.GetCount();
  ItemsOnPage = min(Items_Per_Page, scnt);
  if (ItemsOnPage > 0)
  {
    if (party.activeItem >= spellListText.GetCount())
      party.activeItem = (BYTE)(spellListText.GetCount()-1);
    if (party.activeItem >= (startItem+ItemsOnPage))
      party.activeItem = (BYTE)((startItem+ItemsOnPage)-1);
    if (party.activeItem < 0)
      party.activeItem = 0;
  }
  else
  {
    party.activeItem = 0;
    startItem = 0;
    //currShopItem = 0;
  }
}
void GENDER_MENU_DATA::OnUpdateUI(void)
{
  if (CheckForMultiBoxText()) return;
  
  menu.setAllItemsActive();
  if (spellListText.GetCount() <= 0)
    menu.setItemInactive(1); // select    
  if (!allowSpellNextPage())
    menu.setItemInactive(2); // next
  if (!allowSpellPrevPage())
    menu.setItemInactive(3); // prev
}
void GENDER_MENU_DATA::OnDraw(void)
{
  //StandardMenuDraw();
  UpdateGenderMenuScreen();
}

/////////////////////////////////////////////////////////////ALIGNMENT_MENU_DATA
void ALIGNMENT_MENU_DATA::OnKeypress(key_code key, char ascii)
{
  if (key != KC_RETURN)
  {
    HMenuVItemsKeyboardAction(key, ascii);
    return;
  }
  switch (menu.currentItem()) 
  {
  case 1: // select
    {
      alignmentType type = (alignmentType)spellListText.GetSpellIndex(currSpellTextIndex());
      character.SetAlignment(type);
      character.generateNewCharacter(GetDesignCharStartExp(),GetDesignCharStartExpType());
      PopEvent();
    }
    break;    
  case 2: // next
    nextSpellPage();
    break;
  case 3: // prev
    prevSpellPage();
    break;
  case 4: // exit
    m_pOrig->m_AbortCharCreation=true;
    PopEvent(); // pop to exit event
    break;
  default:
    WriteDebugString("Unhandled menu item in ALIGNMENT_MENU_DATA");
    break;
  }
}
void ALIGNMENT_MENU_DATA::OnMouseClickLeft(int mx, int my)
{  
  if (MouseInventory(mx, my))
    simulatedKeyQueue.PushKey(VK_RETURN);
  else
    MouseMenu(mx, my);
}

void ALIGNMENT_MENU_DATA::OnInitialEvent(void)
{
  ClearFormattedText(textData);
  menu.setMenu(SelectMenuData);
  menu.setHorzOrient();
  menu.MapKeyCodeToMenuItem(KC_ESCAPE, 4);

  clearSpellItemState();
  CString title("Choose a character alignment:");
  CString tmp;
  tmp.Format(" ( %s, %s, %s )", 
               raceData.GetRaceName(character.race()),
               GetGenderName(character.GetGender()),
               classData[character.GetClass()].m_name);
  title+=tmp;

  spellListText.FormatSpellListTitle(title);
  spellListText.m_flags = SHOW_DESIGN_FLAG;    

  WORD allowedAlignments=character.getAllowedAlignments();
  for (int i=0; i<9; i++)
  {
    if ((allowedAlignments & (1 << i))!=0)
      spellListText.Add(i);  
  }

  int scnt = spellListText.GetCount();
  ItemsOnPage = min(Items_Per_Page, scnt);
  if (ItemsOnPage > 0)
  {
    if (party.activeItem >= spellListText.GetCount())
      party.activeItem = (BYTE)(spellListText.GetCount()-1);
    if (party.activeItem >= (startItem+ItemsOnPage))
      party.activeItem = (BYTE)((startItem+ItemsOnPage)-1);
    if (party.activeItem < 0)
      party.activeItem = 0;
  }
  else
  {
    party.activeItem = 0;
    startItem = 0;
    //currShopItem = 0;
  }
}

void ALIGNMENT_MENU_DATA::OnUpdateUI(void)
{
  if (CheckForMultiBoxText()) return;
  
  menu.setAllItemsActive();
  if (spellListText.GetCount() <= 0)
    menu.setItemInactive(1); // select    
  if (!allowSpellNextPage())
    menu.setItemInactive(2); // next
  if (!allowSpellPrevPage())
    menu.setItemInactive(3); // prev

  //WORD allowedAlignments=character.getAllowedAlignments();
  //for (int i=0; i<9; i++)
  //  if ((allowedAlignments & (1 << i))==0)
  //    menu.setItemInactive(i+1);
}

void ALIGNMENT_MENU_DATA::OnDraw(void)
{
  //StandardMenuDraw();
  UpdateAlignmentMenuScreen();
}

/////////////////////////////////////////////////////////////CHOOSESTATS_MENU_DATA
void CHOOSESTATS_MENU_DATA::OnKeypress(key_code key, char ascii)
{
  int rslt;
  if (key != KC_RETURN) 
  {
    rslt=handleChooseStatsInput(CHOOSESTATS_key,&character, key);
    if (rslt&2) return;
    StandardMenuKeyboardAction(key,ascii);
    return;
  };
  handleChooseStatsInput(CHOOSESTATS_exit); // Terminate modifications
  if (menu.currentItem() == 2) // don't re-roll
  {
      PopEvent();
      return;
  };
  if (m_CreateNewChar)
    character.generateNewCharacter(GetDesignCharStartExp(),GetDesignCharStartExpType());
  if (handleChooseStatsInput(CHOOSESTATS_initial, &character))
  {
  };
}

void CHOOSESTATS_MENU_DATA::OnMouseClickLeft(int mx, int my)
{
  int rslt;
  rslt=handleChooseStatsInput(CHOOSESTATS_mouseClick,
                              &character,
                              mx, 
                              my);
  if (rslt&1) ;
  else GameEvent::OnMouseClickLeft(mx, my);
}

extern bool AllowModifyStats;

void CHOOSESTATS_MENU_DATA::OnInitialEvent(void)
{
  ClearFormattedText(textData);
  currPic = character.smallPic;
  currPic.LoadPicSurfaces("prt_");
  menu.setMenu(ChooseStatsMenuData);
  menu.setHorzOrient();
  if (AllowModifyStats)
  {
    menu.setTitle("<TAB><Up/Down> Modify Character Stats");
  }
  else
  {
    menu.setTitle("Character Stats");
  };
  handleChooseStatsInput(CHOOSESTATS_initial, &character);
}

void CHOOSESTATS_MENU_DATA::OnDraw(void)
{
  GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);
  drawBorder();
  drawViewportFrame(VPFrameXOffset, VPFrameY+VPFrameYOffset);
  if (currPic.key > 0)
  {
    if (currPic.picType == SmallPicDib)
    {
       GraphicsMgr.BlitSprite(VPFrameXOffset+(ViewportX-VPFrameX), VPFrameY+VPFrameYOffset+(ViewportY-VPFrameY), 
                       currPic.key);
    }
  }

  handleChooseStatsInput(CHOOSESTATS_refresh, &character);
  menu.DisplayMenu();
  DisplayTextAndFlip();
}

void CHOOSESTATS_MENU_DATA::OnLeaveQueue(void)
{ // Needed if ESCAPE pressed and cleanup not performed. 
  handleChooseStatsInput(CHOOSESTATS_exit); // Terminate modifications
}


////////////////////////////////////////////////////////////////CLASS_MENU_DATA
void CLASS_MENU_DATA::OnKeypress(key_code key, char ascii)
{
  if (key != KC_RETURN)
  {
    HMenuVItemsKeyboardAction(key, ascii);
    return;
  }
  switch (menu.currentItem()) 
  {
  case 1: // select
    {
      classType ct = (classType)spellListText.GetSpellIndex(currSpellTextIndex());
      character.SetClass(ct);
      PopEvent();
    }
    break;    
  case 2: // next
    nextSpellPage();
    break;
  case 3: // prev
    prevSpellPage();
    break;
  case 4: // exit
    m_pOrig->m_AbortCharCreation=true;
    PopEvent(); // pop to exit event
    break;
  default:
    WriteDebugString("Unhandled menu item in CLASS_MENU_DATA");
    break;
  }   
}
void CLASS_MENU_DATA::OnMouseClickLeft(int mx, int my)
{  
  if (MouseInventory(mx, my))
    simulatedKeyQueue.PushKey(VK_RETURN);
  else
    MouseMenu(mx, my);
}

void CLASS_MENU_DATA::OnInitialEvent(void)
{
  ClearFormattedText(textData);
  menu.setMenu(SelectMenuData);
  menu.setHorzOrient();
  menu.MapKeyCodeToMenuItem(KC_ESCAPE, 4);

  CString title("Choose a character class:");
  CString tmp;
  tmp.Format(" ( %s, %s )", 
               raceData.GetRaceName(character.race()),
               GetGenderName(character.GetGender()));
  title+=tmp;
  //menu.setTitle(title);

  clearSpellItemState();
  spellListText.FormatSpellListTitle(title);

  spellListText.m_flags = SHOW_DESIGN_FLAG;    
   
  // Step through the loop to see possible classes.
  ALLOWED_CLASSES allowedClasses;
  allowedClasses.Initialize();  //Start with ALL classes
  allowedClasses.RestrictByRace(character.race());
  for (int i=0; i<allowedClasses.NumAllowedClasses(); i++)
  {
    classType ct = allowedClasses.GetAt(i);
    spellListText.Add((DWORD)ct);
  };

  int scnt = spellListText.GetCount();
  ItemsOnPage = min(Items_Per_Page, scnt);
  if (ItemsOnPage > 0)
  {
    if (party.activeItem >= spellListText.GetCount())
      party.activeItem = (BYTE)(spellListText.GetCount()-1);
    if (party.activeItem >= (startItem+ItemsOnPage))
      party.activeItem = (BYTE)((startItem+ItemsOnPage)-1);
    if (party.activeItem < 0)
      party.activeItem = 0;
  }
  else
  {
    party.activeItem = 0;
    startItem = 0;
    //currShopItem = 0;
  }
}

void CLASS_MENU_DATA::OnUpdateUI(void)
{
  if (CheckForMultiBoxText()) return;
  
  menu.setAllItemsActive();
  if (spellListText.GetCount() <= 0)
    menu.setItemInactive(1); // select    
  if (!allowSpellNextPage())
    menu.setItemInactive(2); // next
  if (!allowSpellPrevPage())
    menu.setItemInactive(3); // prev
}

void CLASS_MENU_DATA::OnDraw(void)
{
  //StandardMenuDraw();
  UpdateClassMenuScreen();
}

//////////////////////////////////////////////////////////ADVENTURE_MENU_DATA
void ADVENTURE_MENU_DATA::OnInitialEvent(void)
{
  //ClearFormattedText(textData);
  menu.setMenu(AdventureMenuData);
  menu.setHorzOrient();
  menu.setUseActive(FALSE);
  SetMyState(TASK_MoveAwaitKey);
  globalData.temp_asl.Clear(); // remove temp vars
}

void ADVENTURE_MENU_DATA::OnDraw(void)
{
  if (!cmdLine.m_forceAV)
  {
    // check global view settings for this level
    switch (globalData.levelInfo.stats[globalData.currLevel].AVStyle)
    {
    case OnlyAreaView: globalData.useAreaView = TRUE;  break;   
    case Only3DView:   globalData.useAreaView = FALSE; break;
    default:           // do nothing 
      break;
    }
  }

  UpdateAdventureScreen();
}

void ADVENTURE_MENU_DATA::OnMouseMove(int mx, int my)
{ 
  CursorArt.sx = mx;
  CursorArt.sy = my;

  if (globalData.levelInfo.stats[globalData.currLevel].overland)
  {
    // determine relative dir between mouse click and party position
    int MapClick=Mouse2OverlandMap(mx,my);
    // NW  N  NE
    // W      E
    // SW  S  SE
    switch (MapClick)
    {
    case 0: CursorArt.SetFrame(NW_CURSOR); return;
    case 1: CursorArt.SetFrame(N_CURSOR); return;
    case 2: CursorArt.SetFrame(NE_CURSOR); return;
    case 3: CursorArt.SetFrame(W_CURSOR); return;
    case 4: CursorArt.SetFrame(CIRCLE_CURSOR); return;
    case 5: CursorArt.SetFrame(E_CURSOR); return;
    case 6: CursorArt.SetFrame(SW_CURSOR); return;
    case 7: CursorArt.SetFrame(S_CURSOR); return;
    case 8: CursorArt.SetFrame(SE_CURSOR); return;
    }
  }
  else
  {
    if (!globalData.useAreaView)
    {
      int ViewPortClick=Mouse2Viewport(mx, my);
      // TL  F  TR
      // TL  F  TR
      //  L  B  R
      switch (ViewPortClick)
      {
      case 0:
      case 3: CursorArt.SetFrame(TURNLEFT_CURSOR); return; // turn left      
      case 1: 
      case 4: CursorArt.SetFrame(N_CURSOR); return; // move forward  
      case 2: 
      case 5: CursorArt.SetFrame(TURNRIGHT_CURSOR); return; // turn right
      case 6: CursorArt.SetFrame(W_CURSOR); return; // slide left
      case 7: CursorArt.SetFrame(S_CURSOR); return; // move back
      case 8: CursorArt.SetFrame(E_CURSOR); return; // slide right
      }
    }
  }
  // default to standard cursor
  CursorArt.SetFirstFrame();
}

void ADVENTURE_MENU_DATA::OnMouseClickLeft(int mx, int my)
{
  if (globalData.levelInfo.stats[globalData.currLevel].overland)
  {
    // determine relative dir between mouse click and party position
    int MapClick=Mouse2OverlandMap(mx,my);
    // NW  N  NE
    // W      E
    // SW  S  SE
    //
    // relative dir works according to:
    //
    // 4  0  5
    // 3     1
    // 6  2  7
    //
    switch (MapClick)
    {
    case 0: simulatedKeyQueue.PushKey(VK_NUMPAD7); return;
    case 1: simulatedKeyQueue.PushKey(VK_NUMPAD8); return;
    case 2: simulatedKeyQueue.PushKey(VK_NUMPAD9); return;
    case 3: simulatedKeyQueue.PushKey(VK_NUMPAD4); return;
    //case 4: simulatedKeyQueue.PushKey(VK_NUMPAD5); return;
    case 5: simulatedKeyQueue.PushKey(VK_NUMPAD6); return;
    case 6: simulatedKeyQueue.PushKey(VK_NUMPAD1); return; 
    case 7: simulatedKeyQueue.PushKey(VK_NUMPAD2); return;
    case 8: simulatedKeyQueue.PushKey(VK_NUMPAD3); return;
    };
  }
  else
  {
    if (!globalData.useAreaView)
    {
      int ViewPortClick=Mouse2Viewport(mx, my);
      // TL  F  TR
      // TL  F  TR
      //  L  B  R
      switch (ViewPortClick)
      {
      case 0:
      case 3: simulatedKeyQueue.PushKey(VK_LEFT); return; // turn left      
      case 1: 
      case 4: simulatedKeyQueue.PushKey(VK_UP); return; // move forward  
      case 2: 
      case 5: simulatedKeyQueue.PushKey(VK_RIGHT); return; // turn right

      case 6: simulatedKeyQueue.PushKey(VK_NUMPAD1); return; // slide left
      case 7: simulatedKeyQueue.PushKey(VK_NUMPAD5); return; // move back
      case 8: simulatedKeyQueue.PushKey(VK_NUMPAD3); return; // slide right
      }; // Otherwise it ain't in the viewport!
    }
  }

  MouseMenu(mx, my);
  MouseParty(mx, my);
}

// relative dir works according to:
//
// 4  0  5
// 3     1
// 6  2  7
//
void ADVENTURE_MENU_DATA::ProcessOverlandMoveRequest(key_code key, char ascii)
{
  int newX, newY;
  int relDir = 0;
  TASKSTATE newstate = TASK_MovePreMove0;

  switch (key)
  {
  case KC_NW:    newstate=TASK_MovePreMove4; relDir=4; break;
  case KC_UP:    newstate=TASK_MovePreMove0; relDir=0; break;
  case KC_NE:    newstate=TASK_MovePreMove5; relDir=5; break;
  case KC_LEFT:  newstate=TASK_MovePreMove3; relDir=3; break;
  //case KC_CENTER:newstate=TASK_MovePreMove0; relDir=0; break;
  case KC_RIGHT: newstate=TASK_MovePreMove1; relDir=1; break;
  case KC_SW:    newstate=TASK_MovePreMove6; relDir=6; break;
  case KC_DOWN:  newstate=TASK_MovePreMove2; relDir=2; break;
  case KC_SE:    newstate=TASK_MovePreMove7; relDir=7; break;
  default: return;
  }

  // just testing to see if party *can* move forward.
  if (party.newForwardPosition(relDir, newX, newY))
  {
    // We have the x and y coordiates of a square that the
    // party is about to move into.  It is possible that the square
    // contains an event that should be activated before the party
    // actually occupies the square.  If so, we will activate it now.
    SetMyState(newstate);  // Doing Pre-movement actions
    if (levelData.haveEvent(newX, newY))
    {
      GameEvent *ev=levelData.eventData.GetFirstEvent(newX, newY);
      if (ev!=NULL) 
      {
        // does this event want to trigger before the move?
        if (ev->OnTestPreMove())
        {
          PushEvent(ev, LevelEvent);
          return;
        };
      };
    };

    // No event triggered before the move.
    // The actual movement will happen after this
    // empty event pops off the queue in
    // OnReturnToTopOfQueue()
    PushEvent(new DO_NOTHING_EVENT, DeleteEvent);
    return;
  }
  else
  {
    // overland maps display a zone message from the 
    // zone in front when passage is blocked
    if (levelData.GetCurrZone(party.Posx,party.Posy) != levelData.GetCurrZone(newX,newY))
    {
      PushEvent(new BEGIN_ZONE_EVENT(newX,newY), DeleteEvent);
    }
    // impassable obstical in our way
    PlayPartyBump();
  }
}

void ADVENTURE_MENU_DATA::ProcessDungeonMoveRequest(key_code key, char ascii)
{
  int newX, newY;
  int relDir = 0;
  TASKSTATE newstate = TASK_MovePreMove0;

  switch(key)
  {
  case KC_UP:
  case KC_SE:
  case KC_CENTER:
  case KC_SW:
    {
      switch(key)
      { // Put direction in state to avoid having to save more stuff
      case KC_UP    : newstate=TASK_MovePreMove0; relDir=0; break;
      case KC_SE    : newstate=TASK_MovePreMove1; relDir=1; break;
      case KC_CENTER: newstate=TASK_MovePreMove2; relDir=2; break;
      case KC_SW    : newstate=TASK_MovePreMove3; relDir=3; break;
      };

      m_relDir=relDir;
      // just testing to see if party *can* move forward.
      if (party.newForwardPosition(relDir, newX, newY))
      {
        // We have the x and y coordinates of a square that the
        // party is about to move into.  It is possible that the square
        // contains an event that should be activated before the party
        // actually occupies the square.  If so, we will activate it now.
        SetMyState(newstate);  // Doing Pre-movement actions

        if (levelData.haveEvent(newX, newY))
        {
          GameEvent *ev=levelData.eventData.GetFirstEvent(newX, newY);
          if (ev!=NULL) 
          {
            // does this event want to trigger before the move?
            if (ev->OnTestPreMove())
            {
              PushEvent(ev, LevelEvent);
              return;
            };
          };
        };

        // No event triggered before the move.
        // The actual movement will happen after this
        // empty event pops off the queue in
        // OnReturnToTopOfQueue()
        PushEvent(new DO_NOTHING_EVENT, DeleteEvent);
        return;
      }
      else // impassable obstical in our path
      {
        // Check blockage for locked, or spell-locked door
        // Secret doors must be found before any attempt at
        // unlocking can be made.

        // check for locked door
        BOOL havelocked = levelData.haveLockedDoor(party.Posx, party.Posy, party.RelativeToActualFacing(relDir));
        BOOL havespelled = levelData.haveSpelledDoor(party.Posx, party.Posy, party.RelativeToActualFacing(relDir));

        if (havelocked || havespelled)
        {
          CHARACTER &dude = GetActiveChar(this);
          CString msg;
          bool alreadytriedbash = false;//!dude.blockageData.IsLocked(GetCurrentLevel(), party.Posx, party.Posy, party.RelativeToActualFacing(m_relDir));
          bool alreadytriedspell = true;
          BOOL havespell = false;

          if (havespelled)
          {
            alreadytriedspell = false;//!dude.blockageData.IsSpelled(GetCurrentLevel(), party.Posx, party.Posy, party.RelativeToActualFacing(m_relDir));
            int unlockspellkey = levelData.GetUnlockSpell(party.Posx, party.Posy, party.RelativeToActualFacing(m_relDir));
            havespell = dude.HaveSpell(unlockspellkey, TRUE);
          }
          
          if ((!alreadytriedbash) && (!alreadytriedspell) && (havespell))
          {            
            SetMyState(TASK_AskToBashOrUnspellDoor);
            PushEvent(new ASK_CUSTOM_MENU_DATA("THIS DOOR IS LOCKED:","BASH","USE MAGIC",CONTROL_Adventure), DeleteEvent);
          }
          else if (!alreadytriedbash)
          {
            SetMyState(TASK_AskToBashDoor);
            PushEvent(new ASK_CUSTOM_MENU_DATA("THIS DOOR IS LOCKED:","BASH","",CONTROL_Adventure), DeleteEvent);
          }
          else if ((!alreadytriedspell) && (havespell))
          {
            SetMyState(TASK_AskToUnspellDoor);
            PushEvent(new ASK_CUSTOM_MENU_DATA("THIS DOOR IS LOCKED:","USE MAGIC","",CONTROL_Adventure), DeleteEvent);
          }          
          else
          {
            SetMyState(TASK_AskToBashDoor);
            msg.Format("THIS DOOR IS LOCKED.");
            PlayPartyBump();
            PushEvent(new ASK_YES_NO_MENU_DATA(msg,FALSE,CONTROL_Adventure), DeleteEvent);
          }
        }    // check for spelled door      
        else
          PlayPartyBump();
      }
    }
    break; // Stay right here 

  case KC_RIGHT:
    {
      party.turnPartyRight();
      // check for events that trigger based on facing only
      PushEvent(new BEGIN_XY_FACING_EVENT(party.Posx,party.Posy,GetCurrentLevel()), DeleteEvent);
    }
    break;
  case KC_LEFT:
    {
      party.turnPartyLeft();
      // check for events that trigger based on facing only
      PushEvent(new BEGIN_XY_FACING_EVENT(party.Posx,party.Posy,GetCurrentLevel()), DeleteEvent);
    }
    break;
  case KC_DOWN:
    {
      party.turnParty180();
      // check for events that trigger based on facing only
      PushEvent(new BEGIN_XY_FACING_EVENT(party.Posx,party.Posy,GetCurrentLevel()), DeleteEvent);
    }
    break;
  default:
    break;
  }
}

void ADVENTURE_MENU_DATA::ProcessMenuRequest(key_code key, char ascii)
{
  switch (menu.currentItem()) 
  {
  case 1: // Area
    globalData.useAreaView = !globalData.useAreaView;
    break;
  case 2: // Cast
    PushEvent(new CAST_MENU_DATA(NULL), DeleteEvent);
    break;
  case 3: // View
    PushEvent(new VIEW_CHARACTER_DATA(GetEventType(),NULL), DeleteEvent);
    break;
  case 4: // Encamp
    globalData.useAreaView = FALSE;
    PushEvent(new ENCAMP_MENU_DATA(NULL), DeleteEvent);
    break;
  case 5: // Search
    party.searching = !party.searching;      
    break;
  case 6: // Look 
    party.looking=TRUE; // for one game loop, will trigger events based on search status
    party.incrementClock(GetTimeInc(TRUE));

    if (party.numCharacters == 1)
      FormatDisplayText(textData, "You look around...", TRUE, FALSE);
    else if (party.numCharacters == 2)
      FormatDisplayText(textData, "Both of you look around...", TRUE, FALSE);
    else
      FormatDisplayText(textData, "The party looks around...", TRUE, FALSE);

    // display the above text before triggering any events
    // in this square
    DisplayTextAndFlip();

    // ** no need to check for zone/step event since we haven't moved **

    // this event will check for, and load, any events that
    // should trigger based on current x,y position in map
    BEGIN_XY_EVENT *xyEvent;
    xyEvent=new BEGIN_XY_EVENT(party.Posx,
                               party.Posy,
                               GetCurrentLevel());
    PushEvent(xyEvent, DeleteEvent);
    break;
  }
  menu.setCurrentItem(1); // default back to first menu option
}

void ADVENTURE_MENU_DATA::OnKeypress(key_code key, char ascii)
{
  ClearFormattedText(textData);
  if (GetMyState()!=TASK_MoveAwaitKey) return; // discard keyin

  if (TABParty(key)) return;

  switch(key)
  {
  default:
    StandardKeyboardShortcutAction(key,ascii);
    break;

  case KC_NW:
  case KC_UP:
  case KC_NE:
  case KC_LEFT:  
  case KC_CENTER:
  case KC_RIGHT:
  case KC_SW:
  case KC_DOWN:
  case KC_SE:  
    if (globalData.levelInfo.stats[globalData.currLevel].overland)
      ProcessOverlandMoveRequest(key,ascii);
    else
      ProcessDungeonMoveRequest(key,ascii);
    break;

  case KC_RETURN: // only get here by mouse or menu shortcut key
    ProcessMenuRequest(key, ascii);
    break;
  }
}

void ADVENTURE_MENU_DATA::OnUpdateUI(void)
{
  if (CheckForMultiBoxText()) return;

  if (!cmdLine.m_forceAV)
  {
    if (globalData.levelInfo.stats[globalData.currLevel].AVStyle != AnyView)
      menu.setItemInactive(1); // area view  
    else if (levelData.zoneData.zones[levelData.area[party.Posy][party.Posx].zone].allowMap == NoMapping)
      menu.setItemInactive(1); // area view
  }

  if (!GetActiveChar(this).canCastSpells())
    menu.setItemInactive(2); // cast 
}

void ADVENTURE_MENU_DATA::OnReturnToTopOfQueue()
{
  if (party.looking)
    CheckSecretDoors();

  party.looking=FALSE;
  int reldir;
  switch (GetMyState())
  {
  case TASK_AskToBashDoor:
    {
      if (party.tradeItem == 1) // yes, bash the door
      {
        int need = GetActiveChar(this).GetAdjOpenDoors();
        GetActiveChar(this).blockageData.ClearLocked(GetCurrentLevel(), party.Posx, party.Posy, party.RelativeToActualFacing(m_relDir));
        int result = rollDice(20,1,0.0);
        if (result <= need)
        {
          // success, bashing clears the wizard lock if present also
          party.blockageData.ClearLocked(GetCurrentLevel(), party.Posx, party.Posy, party.RelativeToActualFacing(m_relDir));          
          party.blockageData.ClearSpelled(GetCurrentLevel(), party.Posx, party.Posy, party.RelativeToActualFacing(m_relDir));          
          FormatPausedText(pausedTextData, "THE DOOR IS OPENED!");
          DisplayPausedText(pausedTextData);
        }
        else
        {
          // fail
          FormatPausedText(pausedTextData, "THE DOOR IS STILL LOCKED");
          PlayPartyBump();
          DisplayPausedText(pausedTextData);
        }
      }
      OnInitialEvent(); return;
    }
    break;

  case TASK_AskToUnspellDoor:
    {
      // magic always works I think, no chance of failure
      if (party.tradeItem == 1) // yes, cast spell to unlock the door
      {
        CHARACTER &dude = GetActiveChar(this);
        dude.blockageData.ClearSpelled(GetCurrentLevel(), party.Posx, party.Posy, party.RelativeToActualFacing(m_relDir));
        int unlockspellkey = levelData.GetUnlockSpell(party.Posx, party.Posy, party.RelativeToActualFacing(m_relDir));
        int spellbookkey = dude.GetSpellBookKey(unlockspellkey, TRUE);
        dude.CastSpell(spellbookkey);

        // success, using magic clears the normal lock also
        party.blockageData.ClearLocked(GetCurrentLevel(), party.Posx, party.Posy, party.RelativeToActualFacing(m_relDir));          
        party.blockageData.ClearSpelled(GetCurrentLevel(), party.Posx, party.Posy, party.RelativeToActualFacing(m_relDir));          
        FormatPausedText(pausedTextData, "THE DOOR IS OPENED!");
        DisplayPausedText(pausedTextData);
      }
      OnInitialEvent(); return;
    }    
    break;

  case TASK_AskToBashOrUnspellDoor:
    {
      if (party.tradeItem == 1) // yes, bash the door
      {
        int need = GetActiveChar(this).GetAdjOpenDoors();
        GetActiveChar(this).blockageData.ClearLocked(GetCurrentLevel(), party.Posx, party.Posy, party.RelativeToActualFacing(m_relDir));
        int result = rollDice(20,1,0.0);
        if (result <= need)
        {
          // success
          party.blockageData.ClearLocked(GetCurrentLevel(), party.Posx, party.Posy, party.RelativeToActualFacing(m_relDir));          
          party.blockageData.ClearSpelled(GetCurrentLevel(), party.Posx, party.Posy, party.RelativeToActualFacing(m_relDir));          
          FormatPausedText(pausedTextData, "THE DOOR IS OPENED!");
          DisplayPausedText(pausedTextData);
        }
        else
        {
          // fail
          FormatPausedText(pausedTextData, "THE DOOR IS STILL LOCKED");
          PlayPartyBump();
          DisplayPausedText(pausedTextData);
        }
      }
      else if (party.tradeItem == 2) // yes, use magic to unlock door
      {
        // magic always works I think, no chance of failure
        CHARACTER &dude = GetActiveChar(this);
        dude.blockageData.ClearSpelled(GetCurrentLevel(), party.Posx, party.Posy, party.RelativeToActualFacing(m_relDir));
        int unlockspellkey = levelData.GetUnlockSpell(party.Posx, party.Posy, party.RelativeToActualFacing(m_relDir));
        int spellbookkey = dude.GetSpellBookKey(unlockspellkey, TRUE);
        dude.CastSpell(spellbookkey);
        
        // success
        party.blockageData.ClearLocked(GetCurrentLevel(), party.Posx, party.Posy, party.RelativeToActualFacing(m_relDir));          
        party.blockageData.ClearSpelled(GetCurrentLevel(), party.Posx, party.Posy, party.RelativeToActualFacing(m_relDir));          
        FormatPausedText(pausedTextData, "THE DOOR IS OPENED!");
        DisplayPausedText(pausedTextData);        
      }
      OnInitialEvent(); return;
    }    
    break;

  case TASK_MovePreMove0: reldir = 0; break;
  case TASK_MovePreMove1: reldir = 1; break;
  case TASK_MovePreMove2: reldir = 2; break;
  case TASK_MovePreMove3: reldir = 3; break;
  case TASK_MovePreMove4: reldir = 4; break;
  case TASK_MovePreMove5: reldir = 5; break;
  case TASK_MovePreMove6: reldir = 6; break;
  case TASK_MovePreMove7: reldir = 7; break;
  case TASK_MoveExecuteEvent:
    OnInitialEvent();
    BEGIN_XY_EVENT *xyEvent;
    xyEvent=new BEGIN_XY_EVENT(party.Posx,
                               party.Posy,
                               GetCurrentLevel());
    PushEvent(xyEvent, DeleteEvent);
    return;
    break;
  default:  
    OnInitialEvent(); 
    if (m_BackupPerformed)
    {
      party.movePartyBackward();
      m_BackupPerformed=FALSE;
    }
    return;
  };

  // perform the actual party movement now
  
  SetMyState(TASK_MoveAwaitKey);

  BOOL result = party.movePartyForward(reldir);
  ASSERT(result);

  // to filter out repeated requests by events
  // to backup the party one step on event exit.
  m_origPartyX=party.Posx;
  m_origPartyY=party.Posy;
  m_BackupPerformed=FALSE;

  QueueMapEvents();
  PushEvent(new PLAYSTEPSOUND_EVENT, DeleteEvent);

  UpdatePartyMovementData();
  ClearFormattedText(textData);
}

void ADVENTURE_MENU_DATA::QueueMapEvents(void)
{
  int currZone = levelData.GetCurrZone(party.Posx,party.Posy);  

  BEGIN_ZONE_EVENT *zoneEvent=NULL;
  if (currZone != levelData.GetPrevZone())
  {
    zoneEvent=new BEGIN_ZONE_EVENT(party.Posx,
                                   party.Posy);
    StartBackgroundZoneOnlyMusic();
  }

  // this event will check for, and load, any events that
  // should trigger based on current x,y position in map
  BEGIN_XY_EVENT *xyEvent;
  xyEvent=new BEGIN_XY_EVENT(party.Posx,
                             party.Posy,
                             GetCurrentLevel());

  // this event will check for, and load, any step events
  // that are available.
  BEGIN_STEP_EVENT *stepEvent;
  stepEvent=new BEGIN_STEP_EVENT(party.Posx,
                                 party.Posy,
                                 GetCurrentLevel());

  /*
  BEGIN_TIME_EVENT *timeEvent;
  timeEvent=new BEGIN_TIME_EVENT(party.Posx,
    party.Posy,
    GetCurrentLevel(),
    party.days, party.hours, party.minutes);
  */
  
  // queue up the events  
  //PushEvent(timeEvent, DeleteEvent);
  PushEvent(xyEvent, DeleteEvent);
  PushEvent(stepEvent, DeleteEvent);
  if (zoneEvent != NULL) PushEvent(zoneEvent, DeleteEvent);
}

unsigned int ADVENTURE_MENU_DATA::OnTaskMessage(TASKMESSAGE msg)
{
  switch (msg)
  {
  case TASKMSG_TeleportBetweenLevels:
  case TASKMSG_TeleportWithinLevel:
    m_BackupPerformed=FALSE;
    // intentional fall through
  case TASKMSG_CancelForwardMove:
    SetMyState(TASK_MoveAwaitKey); break;
  case TASKMSG_MovePartyBackward: 
    if ((!IsCombatActive())&&(!m_BackupPerformed))
    {
      if ((m_origPartyX==party.Posx)&&(m_origPartyY==party.Posy))
      {
        //party.movePartyBackward();
        m_BackupPerformed=TRUE;
      }
    }
    return TASKMSG_STOP; // Should remember where we were so this won't happen twice?
  case TASKMSG_ExecuteEvent:
    if (GetRunAsVersion(GuidedTourVersion) <= _VERSION_0697_)
      SetMyState(TASK_MovePreMove0);
    else
      SetMyState(TASK_MoveExecuteEvent);
    return TASKMSG_STOP;
  default:
    break;
  };
  return 0;
}

int ADVENTURE_MENU_DATA::OnSaveGame(unsigned int *saveArea)
{
  saveArea[0]=m_origPartyX;
  saveArea[1]=m_origPartyY;
  saveArea[2]=m_BackupPerformed;
  return 3;
}

int ADVENTURE_MENU_DATA::OnLoadGame(unsigned int *saveArea)
{
  m_origPartyX=saveArea[0];
  m_origPartyY=saveArea[1];
  m_BackupPerformed=saveArea[2];
  return 3;
}

////////////////////////////////////////////////////////////SAVEGAME_MENU_DATA
void SAVEGAME_MENU_DATA::OnInitialEvent(void)
{
  FormatDisplayText(textData, "CHOOSE WHICH SLOT TO SAVE GAME INTO", FALSE);
  menu.setMenu(SaveMenuData);
  menu.setHorzOrient();
  menu.MapKeyCodeToMenuItem(KC_ESCAPE, menu.totalItems());
}

void SAVEGAME_MENU_DATA::OnDraw(void)
{
  StandardMenuDraw();
}

void SAVEGAME_MENU_DATA::OnKeypress(key_code key, char ascii)
{
  if (key != KC_RETURN)
  {
    StandardMenuKeyboardAction(key, ascii);
    return;
  };
  int temp;
  if (menu.currentItem() != menu.totalItems())
  {
    temp = menu.currentItem()-1;
    MouseRender.EnableWaitCursor();
    SaveGame();  // Record event stack information    
    if (!serializeGame(TRUE, temp))      
      miscError = FileSaveError;
    MouseRender.DisableWaitCursor();
  }

  PopEvent();  // don't include this event in saved stack data
}

////////////////////////////////////////////////////////////LOADGAME_MENU_DATA
void LOADGAME_MENU_DATA::OnInitialEvent(void)
{  
  menu.setMenu(LoadMenuData);
  menu.setHorzOrient();
  int count = 0;
  for (int i=0;i<MAX_SAVE_GAME_SLOTS;i++)
  {
    if (SaveGameExists(i))
    {
      count++;
      break;
    }
  }
  if (count == 0)
    FormatDisplayText(textData, "THERE ARE NO SAVED GAMES AVAILABLE", FALSE);
  else
    FormatDisplayText(textData, "CHOOSE WHICH SLOT TO LOAD GAME FROM", FALSE);
  menu.MapKeyCodeToMenuItem(KC_ESCAPE, menu.totalItems());
}

void LOADGAME_MENU_DATA::OnUpdateUI(void)
{
  if (CheckForMultiBoxText()) return;
  for (int i=0;i<MAX_SAVE_GAME_SLOTS;i++)
  {
    if (!SaveGameExists(i))
      menu.setItemInactive(i+1);
  }
}

void LOADGAME_MENU_DATA::OnDraw(void)
{
  StandardMenuDraw();
}

void LOADGAME_MENU_DATA::OnKeypress(key_code key, char ascii)
{
  if (key != KC_RETURN)
  {
    StandardMenuKeyboardAction(key, ascii);
    return;
  };
  if (menu.currentItem() != menu.totalItems())
  {
    int temp,px,py,dir;  
    temp = menu.currentItem()-1; 
    MouseRender.EnableWaitCursor();
    if (serializeGame(FALSE, temp))
    {
      dir = party.facing;
      px   = party.Posx;
      py   = party.Posy;      

      // events in taskList may be pointing to events
      // that will be deleted when loadLevel() is called
      //
      // since we call event->OnLeaveQueue for each event
      // before cleaning it out of taskList, we need to do this
      // before deleting the level's events.
      taskList.PopAllButTopEvent(); // all but current event
      BOOL result = loadLevel(party.level);

      party.Posx = px;
      party.Posy = py;
      party.facing = (BYTE)dir;

      if (result)
      {
        // restore event stack from loaded data
        while (!LoadGame()) // ie: forever!
        {
          MsgBoxError("Cannot Load Saved State");
          miscError = LevelLoadError;
        }
        StartBackgroundMusic();
      }
      else
        miscError = LevelLoadError;
    }
    else
      miscError = LevelLoadError;
    MouseRender.DisableWaitCursor();
  }
  PopEvent(); // pop this event
}

////////////////////////////////////////////////////////////BEGIN_TIME_EVENT
BEGIN_TIME_EVENT::BEGIN_TIME_EVENT(int sx, int sy, int level, int day, int hour, int minute) : GameEvent()
{
  event=CONTROL_BeginTimeEvent;
  m_x=sx;
  m_y=sy;
  m_level=level;
  m_day = day;
  m_hour = hour;
  m_minute = minute;
}

BEGIN_TIME_EVENT& BEGIN_TIME_EVENT::operator = (BEGIN_TIME_EVENT& src)
{
  if (this==&src) return *this;
  GameEvent::operator = (src);
  m_x=src.m_x;
  m_y=src.m_y;
  m_level=src.m_level;
  m_day=src.m_day;
  m_hour = src.m_hour;
  m_minute = src.m_minute;
  return *this;
}

void BEGIN_TIME_EVENT::OnInitialEvent(void)
{ 
  
  PopEvent();

  TIME_EVENT *pTimeEvent = NULL;
  //if (levelData.haveTimeEvent(m_x, m_y, m_day, m_hour, m_minute, &pTimeEvent))
  //{
  //  PushEvent(pTimeEvent, LevelEvent);
  //}
}

int BEGIN_TIME_EVENT::OnSaveGame(unsigned int *saveArea)
{
  saveArea[0]=m_x;
  saveArea[1]=m_y;
  saveArea[2]=m_level;
  saveArea[3]=m_day;
  saveArea[4]=m_hour;
  saveArea[5]=m_minute;
  return 6;
}

int BEGIN_TIME_EVENT::OnLoadGame(unsigned int *saveArea)
{
  m_x = saveArea[0];
  m_y = saveArea[1];
  m_level = saveArea[2];
  m_day = saveArea[3];
  m_hour = saveArea[4];
  m_minute = saveArea[5];
  return 6;
}

////////////////////////////////////////////////////////////BEGIN_STEP_EVENT
BEGIN_STEP_EVENT::BEGIN_STEP_EVENT(int sx, int sy, int level) : GameEvent()
{
  event=CONTROL_BeginStepEvent;
  m_x=sx;
  m_y=sy;
  m_level=level;
}

BEGIN_STEP_EVENT& BEGIN_STEP_EVENT::operator = (BEGIN_STEP_EVENT& src)
{
  if (this==&src) return *this;
  GameEvent::operator = (src);
  m_x=src.m_x;
  m_y=src.m_y;
  m_level=src.m_level;
  return *this;
}

void BEGIN_STEP_EVENT::OnInitialEvent(void)
{
  if ( (m_x == -1) || (m_y == -1) || (m_level == -1) )
  {
     PopEvent();
     return;
  }

  int zone = levelData.GetCurrZone(m_x,m_y);  
  int stepCount = party.GetZoneStepCount(m_level, zone);
  int i=MAX_STEP_EVENTS-1; // push events in reverse order
  
  PopEvent();

  while (i>=0)
  {
    STEP_EVENT &data = levelData.stepEvents[i];
    // if this zone can trigger and count is multiple of step count
    if (   (data.stepTrigger[zone] > 0) // does this zone trigger?
        && (data.stepCount > 0)         // must have non-zero trigger count
        && ((stepCount % data.stepCount)==0) ) // does current step count match trigger count?
    {
      // if event data is present...
      if (levelData.eventData.IsValidEvent(data.stepEvent))
      {
        PushEvent(levelData.eventData.GetEvent(levelData.stepEvents[i].stepEvent), 
                  LevelEvent);
      }
    }
    i--;
  }
}

int BEGIN_STEP_EVENT::OnSaveGame(unsigned int *saveArea)
{
  saveArea[0]=m_x;
  saveArea[1]=m_y;
  saveArea[2]=m_level;
  return 3;
}

int BEGIN_STEP_EVENT::OnLoadGame(unsigned int *saveArea)
{
  m_x = saveArea[0];
  m_y = saveArea[1];
  m_level = saveArea[2];
  return 3;
}

/*
////////////////////////////////////////////////////////////BEGIN_REST_EVENT
BEGIN_REST_EVENT::BEGIN_REST_EVENT(int x, int y)
{
  event=CONTROL_BeginRestEvent;
  m_x=x;
  m_y=y;
}

BEGIN_REST_EVENT& BEGIN_REST_EVENT::operator = (BEGIN_REST_EVENT& src)
{
  if (this==&src) return *this;
  GameEvent::operator = (src);
  m_x=src.m_x;
  m_y=src.m_y;
  return *this;
}

void BEGIN_REST_EVENT::OnInitialEvent(void)
{
  if ( (m_x == -1) || (m_y == -1) )
  {
     PopEvent();
     return;
  }
  GameEvent *tempEvent=NULL;
  if (levelData.haveRestEvent(
           m_x,
           m_y,
           &tempEvent))
  {
    ReplaceEvent(tempEvent, LevelEvent);
  }
  else
  {
    PopEvent(); // No rest events here
  };
}

int BEGIN_REST_EVENT::OnSaveGame(unsigned int *saveArea)
{
  saveArea[0]=m_x;
  saveArea[1]=m_y;
  return 2;
}

int BEGIN_REST_EVENT::OnLoadGame(unsigned int *saveArea)
{
  m_x = saveArea[0];
  m_y = saveArea[1];
  return 2;
}
*/

////////////////////////////////////////////////////////////BEGIN_ZONE_EVENT
BEGIN_ZONE_EVENT::BEGIN_ZONE_EVENT(int zx, int zy)  : GameEvent()
{
  event=CONTROL_BeginZoneEvent;
  m_x=zx;
  m_y=zy;
}

BEGIN_ZONE_EVENT& BEGIN_ZONE_EVENT::operator = (BEGIN_ZONE_EVENT& src)
{
  if (this==&src) return *this;
  GameEvent::operator = (src);
  m_x=src.m_x;
  m_y=src.m_y;
  return *this;
}

void BEGIN_ZONE_EVENT::OnInitialEvent(void)
{
  if ( (m_x == -1) || (m_y == -1) )
  {
     PopEvent();
     return;
  }

  if (levelData.haveZoneMsg(m_x, m_y))
  {
    TRACE("Activating Zone Msg Event\n");
    FormatDisplayText(textData, levelData.zoneData.zones[levelData.area[m_y][m_x].zone].zoneMsg);
  }
  PopEvent();
}

int BEGIN_ZONE_EVENT::OnSaveGame(unsigned int *saveArea)
{
  saveArea[0]=m_x;
  saveArea[1]=m_y;
  return 2;
}

int BEGIN_ZONE_EVENT::OnLoadGame(unsigned int *saveArea)
{
  m_x = saveArea[0];
  m_y = saveArea[1];
  return 2;
}

////////////////////////////////////////////////////////////BEGIN_XY_EVENT
BEGIN_XY_EVENT::BEGIN_XY_EVENT(int ex, int ey, int level) : GameEvent()
{
  event=CONTROL_BeginXYEvent;
  m_x=ex;
  m_y=ey;
  m_level=level;
}

BEGIN_XY_EVENT& BEGIN_XY_EVENT::operator = (BEGIN_XY_EVENT& src)
{
  if (this==&src) return *this;
  GameEvent::operator = (src);
  m_x=src.m_x;
  m_y=src.m_y;
  m_level=src.m_level;
  return *this;
}

void BEGIN_XY_EVENT::OnInitialEvent(void)
{
  //int testflag=1;
  if (levelData.haveEvent(party.Posx,party.Posy))
  {
    // GetEvent will return the event that should be acted upon for current x,y.
    GameEvent *ev=levelData.eventData.GetFirstEvent(party.Posx, party.Posy);
    if (ev!=NULL) 
    {
      if (!ev->OnTestPreMove())
      {
        //if (testflag) flushOldImages();
        ReplaceEvent(ev, LevelEvent);
        return;
      };
    };
  };
  PopEvent(); // No events to be done here
}

unsigned int BEGIN_XY_EVENT::OnTaskMessage(TASKMESSAGE msg)
{
  switch (msg)
  {
  case TASKMSG_TeleportWithinLevel:
  case TASKMSG_TeleportBetweenLevels:
    return TASKMSG_DELETE;
  default:
    break;
  };
  return 0;
}

////////////////////////////////////////////////////////////BEGIN_XY_FACING_EVENT
BEGIN_XY_FACING_EVENT::BEGIN_XY_FACING_EVENT(int fx, int fy, int level)  : GameEvent()
{
  event=CONTROL_BeginXYEvent;
  m_x=fx;
  m_y=fy;
  m_level=level;
}

BEGIN_XY_FACING_EVENT& BEGIN_XY_FACING_EVENT::operator = (BEGIN_XY_FACING_EVENT& src)
{
  if (this==&src) return *this;
  GameEvent::operator = (src);
  m_x=src.m_x;
  m_y=src.m_y;
  m_level=src.m_level;
  return *this;
}

void BEGIN_XY_FACING_EVENT::OnInitialEvent(void)
{
  if (levelData.haveEvent(party.Posx,party.Posy))
  {
    // GetEvent will return the event that should be acted upon for current x,y.
    GameEvent *ev=levelData.eventData.GetFirstEvent(party.Posx, party.Posy);
    if (ev!=NULL) 
    {
      if (ev->control.eventTrigger == FacingDirectionAnyTime)
      {
        ReplaceEvent(ev, LevelEvent);
        return;
      }
    }
  }
  PopEvent(); // No events to be done here
}

unsigned int BEGIN_XY_FACING_EVENT::OnTaskMessage(TASKMESSAGE msg)
{
  switch (msg)
  {
  case TASKMSG_TeleportWithinLevel:
  case TASKMSG_TeleportBetweenLevels:
    return TASKMSG_DELETE;
  default:
    break;
  };
  return 0;
}

////////////////////////////////////////////////////////////PLAYSTEPSOUND_EVENT

void PLAYSTEPSOUND_EVENT::OnInitialEvent(void)
{
  if (!globalData.levelInfo.stats[globalData.currLevel].overland)
  {
     int slot=-1;
     switch (party.RelativeToActualFacing(party.PrevRelFacing)) 
     {      
     case FACE_NORTH:
        slot = levelData.area[party.Posy][party.Posx].southWall;
        break;      
     case FACE_EAST:
        slot = levelData.area[party.Posy][party.Posx].westWall;
        break;      
     case FACE_SOUTH:
         slot = levelData.area[party.Posy][party.Posx].northWall;
        break;      
     case FACE_WEST:
        slot = levelData.area[party.Posy][party.Posx].eastWall;
        break;
     }

    if ((slot > 0) && (slot < MAX_WALLSETS))
      WallSets[slot].PlaySlotSound();

    slot = levelData.area[party.Posy][party.Posx].backgrounds(party.facing);
    if ((slot < 0) || (slot >= MAX_BACKGROUNDS))
    {
      slot = 0;
      WriteDebugString("Bogus backdrop slot num %u in PLAYSTEPSOUND_EVENT()\n", slot);
    }

    if (BackgroundSets[slot].suppressStepSound)
    {
      if (slot > 0) BackgroundSets[slot].PlaySlotSound();
    }
    else
    {
      if (slot > 0)  BackgroundSets[slot].PlaySlotSound();
      PlayPartyStep();
    }
  }
  else
    PlayPartyStep();
  PopEvent();
}

///////////////////////////////////////////////////////////////TEXT_EVENT_DATA
void TEXT_EVENT_DATA::OnInitialEvent(void)
{
  // check to see GPDL should be invoked
  if (GetEventText().Left(10)=="**SHAZAM**")
  {
    menu.reset();
    ReplaceEvent(new GPDL_EVENT(this), DeleteEvent);
    return;
  };
  menu.setMenu(TextEventData);
  menu.setHorzOrient();
  FormatDisplayText(textData, GetEventText(), TRUE, HighlightText);

  currPic = GetEventPic();
  currPic.LoadPicSurfaces("sp_");
  if (currPic.picType == SpriteDib)
    currPic.SetFrame(distance);

  if (PlayEventSounds) hSound = PlaySound(sound); 
}

void TEXT_EVENT_DATA::OnKeypress(key_code key, char ascii)
{ 
  if (TABParty(key)) return;
  if (key != KC_RETURN) 
  {
    HMenuVPartyKeyboardAction(key,ascii);
    //StandardMenuKeyboardAction(key, ascii);
    return;
  };

  // all text has been displayed
  if (ForcePartyBackup()) TaskMessage(TASKMSG_MovePartyBackward);
  currPic.Clear();
  // let text continue being displayed if return not needed
  if (WaitUntilReturnPressed()) ClearFormattedText(textData);
  // let music/sound continue playing?
  if (KillSound) ClearSound(hSound);
  ChainHappened();
}

void TEXT_EVENT_DATA::OnDraw(void)
{
  UpdateTextEventScreen();
  if (GetMyState()!=TASK_TextDisplayed) 
    SetMyState(TASK_TextDisplayed);
}

void TEXT_EVENT_DATA::OnUpdateUI(void)
{
  if (CheckForMultiBoxText()) return;

  menu.setItemInactive(1); // Exit
  if (ShouldSimulateEnterKey()) 
  {
    menu.setItemInactive(2); // Press Enter to continue
    simulatedKeyQueue.PushKey(VK_RETURN);
  }
}

///////////////////////////////////////////////////////////////QUESTION_YES_NO

void QUESTION_YES_NO::OnInitialEvent(void)
{
  menu.setMenu(QuestionYesNoData);
  menu.setHorzOrient();
  FormatDisplayText(textData, GetEventText());
  currPic = GetEventPic();
  currPic.LoadPicSurfaces("sp_");
  SetMyState(TASK_QuestionYesNo_asking);
  sx=party.Posx;sy=party.Posy;
}

void QUESTION_YES_NO::OnKeypress(key_code key, char ascii)
{
  if (TABParty(key)) return;
  if (key != KC_RETURN)
  {
    HMenuVPartyKeyboardAction(key, ascii);
    return;
  };
  switch (GetMyState())
  {
  case TASK_QuestionYesNo_asking:
    if (menu.currentItem() == 1) // Yes
    {
      if (GetEventText2().GetLength() > 0)
      {
        menu.setMenu(QuestionYesData);
        menu.setHorzOrient();
        FormatDisplayText(textData, GetEventText2());
        currPic = GetEventPic();
        currPic.LoadPicSurfaces("sp_");
        SetMyState(TASK_QuestionYesNo_yes);
      }
      else
      {
        // skip the text and just chain
        SetMyState(TASK_QuestionYesNo_yesChain);
        if (IsGlobalEvent())
        {
          if (!globalData.eventData.IsValidEvent(YesChain))
            PushEvent(new DO_NOTHING_EVENT, DeleteEvent);
          else 
            PushGlobalEvent(YesChain);
        }
        else
        {
          if (!levelData.eventData.IsValidEvent(YesChain))
            PushEvent(new DO_NOTHING_EVENT, DeleteEvent);
          else 
            PushEvent(YesChain);
        }
        currPic.Clear();
        ClearFormattedText(textData);
      }
    }
    else // No
    {
      if (GetEventText3().GetLength() > 0)
      {
        menu.setMenu(QuestionNoData);
        menu.setHorzOrient();
        FormatDisplayText(textData, GetEventText3());
        currPic = GetEventPic();
        currPic.LoadPicSurfaces("sp_");
        SetMyState(TASK_QuestionYesNo_no);
      }
      else
      {
        // skip the text and just chain
        SetMyState(TASK_QuestionYesNo_noChain);
        
        if (IsGlobalEvent())
        {
          if (!globalData.eventData.IsValidEvent(NoChain))
            PushEvent(new DO_NOTHING_EVENT, DeleteEvent);
          else 
            PushGlobalEvent(NoChain);
        }
        else
        {
          if (!levelData.eventData.IsValidEvent(NoChain))
            PushEvent(new DO_NOTHING_EVENT, DeleteEvent);
          else 
            PushEvent(NoChain);
        }
        currPic.Clear();
        ClearFormattedText(textData);
      }
    };
    break;
  case TASK_QuestionYesNo_yes:
    SetMyState(TASK_QuestionYesNo_yesChain);

    if (IsGlobalEvent())
    {
      if (!globalData.eventData.IsValidEvent(YesChain))
        PushEvent(new DO_NOTHING_EVENT, DeleteEvent);
      else 
        PushGlobalEvent(YesChain);
    }
    else
    {
      if (!levelData.eventData.IsValidEvent(YesChain))
        PushEvent(new DO_NOTHING_EVENT, DeleteEvent);
      else 
        PushEvent(YesChain);
    }
    currPic.Clear();
    ClearFormattedText(textData);
    break;
  case TASK_QuestionYesNo_no:
    SetMyState(TASK_QuestionYesNo_noChain);
    if (IsGlobalEvent())
    {
      if (!globalData.eventData.IsValidEvent(NoChain))
        PushEvent(new DO_NOTHING_EVENT, DeleteEvent);
      else 
        PushGlobalEvent(NoChain);
    }
    else
    {
      if (!levelData.eventData.IsValidEvent(NoChain))
        PushEvent(new DO_NOTHING_EVENT, DeleteEvent);
      else 
        PushEvent(NoChain);
    }
    currPic.Clear();
    ClearFormattedText(textData);
    break;
  default:
    WriteDebugString("Bogus QUESTION_YES_NO state\n");
    ChainHappened();
  };
}

void QUESTION_YES_NO::OnDraw(void)
{
  UpdateQuestionYesNoScreen();
}

void QUESTION_YES_NO::OnReturnToTopOfQueue(void)
{
  currPic.Clear();
  ClearFormattedText(textData);
  switch (GetMyState())
  {
  case TASK_QuestionYesNo_yesChain:
    switch (YesChainAction)
    {
    case LabelDoNothing:
      ChainHappened();
      break;
    case LabelReturnToQuestion:
      // only if still in starting location
      if ((sx==party.Posx)&&(sy==party.Posy))
        OnInitialEvent();
      else
        ChainHappened();
      break;
    case LabelBackupOneStep:
      ChainHappened();
      if ((sx==party.Posx)&&(sy==party.Posy))
        TaskMessage(TASKMSG_MovePartyBackward);
      break;
    };
    break;
  case TASK_QuestionYesNo_noChain:
    switch (NoChainAction)
    {
    case LabelDoNothing:
      ChainHappened();
      break;
    case LabelReturnToQuestion:
      // only if still in starting location
      if ((sx==party.Posx)&&(sy==party.Posy))
        OnInitialEvent();
      else
        ChainHappened();
      break;
    case LabelBackupOneStep:
      if ((sx==party.Posx)&&(sy==party.Posy))
        TaskMessage(TASKMSG_MovePartyBackward);
      ChainHappened();
      break;
    };
    break;
  default:
    WriteDebugString("Bogus QuestioYesNoState\n");
    ChainHappened();
  };
}

int QUESTION_YES_NO::OnSaveGame(unsigned int *saveArea)
{
  // task state is automatically saved
  return 0; 
}

int QUESTION_YES_NO::OnLoadGame(unsigned int *saveArea)
{
  // task state is automatically restored
  return 0;
}

/////////////////////////////////////////////////////////////VAULT_EVENT_DATA

void VAULT_EVENT_DATA::OnInitialEvent(void)
{
  menu.setMenu(VaultMenuData);
  menu.setHorzOrient();
  menu.MapKeyCodeToMenuItem(KC_ESCAPE, 5);
  FormatDisplayText(textData, GetEventText());
  setTreasureEventState(this);
  currPic = GetEventPic();
  currPic.LoadPicSurfaces("sp_");
}

void VAULT_EVENT_DATA::OnKeypress(key_code key, char ascii)
{
  if (TABParty(key)) return;
  if (key != KC_RETURN)
  {
    HMenuVPartyKeyboardAction(key,ascii);
    return;
  };
  switch (menu.currentItem()-1) 
  {
  case 0: // View
    PushEvent(new VIEW_CHARACTER_DATA(GetEventType(),this), DeleteEvent);
    break;
  case 1: // Take
    setTreasureEventState(this);
    PushEvent(new TAKE_TREASURE_DATA(this), DeleteEvent);
    break;
  case 2: // Pool
    // move party funds into vault
    party.AddPooledMoneyToVault(WhichVault);
    setTreasureEventState(this);
    break;     
  case 3: // Share
    // distribute vault funds to party members
    party.RemovePooledMoneyFromVault(WhichVault);    
    // rebuild item list text
    setTreasureEventState(this);
    break;      
  case 4: // Items
    PushEvent(new ITEMS_MENU_DATA(GetEventType(),this),DeleteEvent);
    break;      
  case 5: // Exit
    // no point to auto-pooling money, the vault has the pooled money stored
    treasureItemListText.Clear();
    currPic.Clear();
    if (WaitUntilReturnPressed()) ClearFormattedText(textData);
    if (ForcePartyBackup()) TaskMessage(TASKMSG_MovePartyBackward);
    ChainHappened();
  };
}

void VAULT_EVENT_DATA::OnDraw(void)
{
  UpdateVaultScreen();
}

void VAULT_EVENT_DATA::OnUpdateUI(void)
{
  if (CheckForMultiBoxText()) return;

  if (!party.PartyHasMoney())
    menu.setItemInactive(3); // pool
  if (!globalData.VaultHasMoney(WhichVault)) 
    menu.setItemInactive(4); // share
  if (treasureItemListText.GetCount() <= 0)
    menu.setItemInactive(2); // take
}

void VAULT_EVENT_DATA::OnReturnToTopOfQueue(void)
{
  if (GetMyState()==TASK_AskLeaveMoneyBehind)
  {
    if (party.tradeItem==0) // no, don't leave money behind
    {
      // return to vault menu
      menu.setMenu(VaultMenuData);
      menu.setHorzOrient();
      setTreasureEventState(this);
      currPic = GetEventPic();
      currPic.LoadPicSurfaces("sp_");
      ClearFormattedText(textData);
    }
    else
    {
      party.moneyPooled=FALSE;
      party.poolSack.Clear(); // forfeit pooled funds

      treasureItemListText.Clear();
      currPic.Clear();
      ClearFormattedText(textData);
      if (ForcePartyBackup()) TaskMessage(TASKMSG_MovePartyBackward);
      ChainHappened();
    }
  }
  else
  {
    menu.setMenu(VaultMenuData);
    menu.setHorzOrient();
    setTreasureEventState(this);
    currPic = GetEventPic();
    currPic.LoadPicSurfaces("sp_");
    ClearFormattedText(textData);
  }
}

void VAULT_EVENT_DATA::OnLeaveQueue(void)
{
  //if (party.moneyPooled)
  //  party.sharePartyGold();
}

////////////////////////////////////////////////////////////GIVE_TREASURE_DATA

void GIVE_TREASURE_DATA::OnInitialEvent(void)
{
  if (SilentGiveToActiveChar)
  {
    CHARACTER &dude = GetActiveChar(this);

    POSITION pos = items.GetHeadPosition();
    while (pos!=NULL)
      dude.myItems.AddItem(items.GetNext(pos));

    dude.money.Transfer(money);

    ChainHappened();
    return;
  }

  SetMyState(TASK_initialized);
  menu.setMenu(GiveTreasureMenuData);
  menu.setHorzOrient();
  menu.MapKeyCodeToMenuItem(KC_ESCAPE, 5);
  FormatDisplayText(textData,"You Have Found Treasure!");

  // load treasure picture
  int zone = levelData.GetCurrZone(party.Posx,party.Posy);
  currPic = levelData.zoneData.zones[zone].treasurePicture;
  currPic.LoadPicSurfaces("sp_");
  
  moneyRemain.Clear();
  moneyRemain=money;

  itemsRemain = items;
  setTreasureEventState(this);

  m_detectSpellKey = GetActiveChar(this).CanCastDetectMagic();
}

void GIVE_TREASURE_DATA::OnKeypress(key_code key, char ascii)
{
  if (TABParty(key)) return;
  if (key != KC_RETURN)
  {
    HMenuVPartyKeyboardAction(key, ascii);
    return;
  };

  // the treasure message is only displayed once
  ClearFormattedText(textData);
  switch (menu.currentItem()-1) 
  {
  case 0: // View character
    SetMyState(TASK_initialized);
    PushEvent(new VIEW_CHARACTER_DATA(GetEventType(),this), DeleteEvent);
    break;
  case 1: // Take item
    setTreasureEventState(this);
    SetMyState(TASK_initialized);
    PushEvent(new TAKE_TREASURE_DATA(this), DeleteEvent);
    break;
  case 2: // Pool
    // put all party funds into treasure data
    party.poolPartyGold();
    moneyRemain.Transfer(party.poolSack);
    setTreasureEventState(this);
    break;
  case 3: // Share
    // share all platinum, even from treasure event
    party.poolSack.Transfer(moneyRemain);
    party.sharePartyGold();
    // restore any leftovers into treasure list
    moneyRemain.Transfer(party.poolSack);
    setTreasureEventState(this);
    break;
  case 4: // Detect
    {
      SetMyState(TASK_initialized);
      // cast detect spell by active dude    
      m_detectSpellKey = GetActiveChar(this).CanCastDetectMagic();
      PushEvent(new CAST_NON_COMBAT_SPELL_MENU_DATA(GetEventType(),NULL,&GetActiveChar(this),m_detectSpellKey), DeleteEvent);
    }
    break;
  case 5: // Exit
    if (party.moneyPooled)
    {      
      SetMyState(TASK_AskLeaveMoneyBehind);
      PushEvent(new ASK_YES_NO_MENU_DATA("LEAVE ALL TREASURE BEHIND?",TRUE, GiveTreasure), DeleteEvent);
    }
    else if (treasureItemListText.GetCount() > 0)
    {
      SetMyState(TASK_AskLeaveTreasureBehind);
      PushEvent(new ASK_YES_NO_MENU_DATA("THERE IS STILL TREASURE HERE\nDO YOU WANT TO LEAVE IT BEHIND?",TRUE, GiveTreasure), DeleteEvent);
    }
    else
    {
      clearTreasureItemState();
      currPic.Clear();
      if (WaitUntilReturnPressed()) ClearFormattedText(textData);
      ChainHappened();
    }
    break;
  default:
    WriteDebugString("Unhandled choice in GiveTreasureMenu()\n");
    miscError = InvalidMenuState;
    ChainHappened();
  }
}

void GIVE_TREASURE_DATA::OnDraw(void)
{
  UpdateGiveTreasureScreen();
}

void GIVE_TREASURE_DATA::OnUpdateUI(void)
{
  if (CheckForMultiBoxText()) return;

  // enable SHARE menu for two cases:
  //   - party funds are pooled
  //   - funds available in GiveTreasure
  //
  // this allows SHARE menu to auto distribute the money
  // given by the event without needing to force a character to
  // TAKE the money first.
  //
  // when party money is pooled, it goes into the
  // GiveTreasure money holder called moneyRemain.
  //

  // ** original code, broken **
  //if (party.moneyPooled)
  //{
  //  if (moneyRemain.Total()==0.0)
  //    party.moneyPooled=FALSE;
  //}
  
  party.moneyPooled = (moneyRemain.Total() > 0.0);

  if (party.moneyPooled)
    menu.setItemInactive(3); // pool
  else
    menu.setItemInactive(4); // share
  
  if (treasureItemListText.GetCount() <= 0)
    menu.setItemInactive(2); // take
  
  if (!levelData.zoneData.zones[levelData.area[party.Posy][party.Posx].zone].AllowMagic)
    menu.setItemInactive(5); // detect

  if (!GetActiveChar(this).canCastSpells())
  {
    menu.setItemInactive(5); // detect
  }
  else 
  {
    // check for char detect spell memorized
    //m_detectSpellKey = globalData.m_spellSpecialData.HasDetectMagicSpell(GetActiveChar());
    m_detectSpellKey = GetActiveChar(this).CanCastDetectMagic();
    if (m_detectSpellKey==NO_SPELL)
       menu.setItemInactive(5); // detect
  }
}

void GIVE_TREASURE_DATA::OnReturnToTopOfQueue(void)
{
  if (GetMyState()==TASK_AskLeaveMoneyBehind)
  {
    if (party.tradeItem==0) // no, don't leave money behind
    {
      //party.poolSack.Transfer(moneyRemain);
      //party.sharePartyGold(); 
      SetMyState(TASK_initialized);
      menu.setMenu(GiveTreasureMenuData);
      menu.setHorzOrient();
      int zone = levelData.GetCurrZone(party.Posx,party.Posy);
      currPic = levelData.zoneData.zones[zone].treasurePicture;
      currPic.LoadPicSurfaces("sp_");
      setTreasureEventState(this);
      ClearFormattedText(textData);
    }
    else
    {
      party.moneyPooled=FALSE;
      party.poolSack.Clear(); // forfeit pooled funds

      clearTreasureItemState();
      currPic.Clear();
      if (WaitUntilReturnPressed()) ClearFormattedText(textData);
      ChainHappened();
    }
  }
  else if (GetMyState()==TASK_AskLeaveTreasureBehind)
  {
    if (party.tradeItem==0) // no, don't leave treasure behind
    {
      SetMyState(TASK_initialized);
      menu.setMenu(GiveTreasureMenuData);
      menu.setHorzOrient();
      int zone = levelData.GetCurrZone(party.Posx,party.Posy);
      currPic = levelData.zoneData.zones[zone].treasurePicture;
      currPic.LoadPicSurfaces("sp_");
      setTreasureEventState(this);
      ClearFormattedText(textData);
    }
    else
    {
      clearTreasureItemState();
      currPic.Clear();
      if (WaitUntilReturnPressed()) ClearFormattedText(textData);
      ChainHappened();
    }
  }
  else
  {
    SetMyState(TASK_initialized);
    menu.setMenu(GiveTreasureMenuData);
    menu.setHorzOrient();
    int zone = levelData.GetCurrZone(party.Posx,party.Posy);
    currPic = levelData.zoneData.zones[zone].treasurePicture;
    currPic.LoadPicSurfaces("sp_");
    setTreasureEventState(this);
    ClearFormattedText(textData);
  }
}

void GIVE_TREASURE_DATA::OnLeaveQueue(void)
{
}

////////////////////////////////////////////////////////////TAKE_TREASURE_DATA

void TAKE_TREASURE_DATA::OnInitialEvent(void)
{
  ClearFormattedText(textData);
  party.tradeQty = 0;
  setTreasureEventState(m_pCallerEvent);
  menu.setMenu(TakeTreasureMenuData);
  menu.setHorzOrient();
  menu.MapKeyCodeToMenuItem(KC_ESCAPE, 6);
  SetMyState(TASK_TakeTreasure);
  handleItemsFormInput(ITEMSFORM_exit, treasureItemListText);
  handleItemsFormInput(ITEMSFORM_initial, treasureItemListText);
}


void TAKE_TREASURE_DATA::OnKeypress(key_code key, char ascii)
{
  if (key != KC_RETURN)
  {
    HMenuVItemsKeyboardAction(key,ascii);
    return;
  };
/*
  1 { "NEXT", 0 },
  2 { "PREV", 0 },
  3 { "TAKE", 0 },
  4 { "HALVE", 0 },
  5 { "JOIN", 0 },
  6 { "SHARE", 0 },
  7 { "EXIT", 1 }
};
*/
  int textIndex = currTreasureTextIndex();
  switch (menu.currentItem()) 
  {
  case 1: // next
    nextTreasurePage();
    break;
  case 2: // prev
    prevTreasurePage();
    break;
  case 3: // take
    if (itemIsMoney(treasureItemListText.GetItemClass(textIndex)))    
    {
       SetMyState(TASK_TakeTreasureGetMoneyQty);
       PushEvent(new GET_MONEY_QTY_DATA(GetEventType(),m_pCallerEvent),DeleteEvent);
       return;
    }
    else 
    { 
      int qty = treasureItemListText.GetItemQty(textIndex);
      int bq = itemData.GetItemBundleQty(treasureItemListText.GetItemIndex(textIndex));
      if (bq < 1) bq = 1;
      if (bq < qty) qty = bq;

      int cost=-1;
      switch (m_pCallerEvent->GetEventType())
      {
      case Vault:
        cost = globalData.vault[((VAULT_EVENT_DATA*)m_pCallerEvent)->WhichVault].items.GetPurchasePrice(treasureItemListText.GetCharIndex(textIndex));
        break;
      case Combat:
        {
          ASSERT(FALSE);
          int dude = treasureItemListText.GetItemData(textIndex);
          cost = combatData.combatants[dude].myItems.GetPurchasePrice(treasureItemListText.GetCharIndex(textIndex));
        }
        break;
      default:
        cost=-1;
        break;
      }

      // taking an item (not money, gems, jewelry)
      BOOL success = takeTreasure(treasureItemListText.GetItemIndex(textIndex),
                                  qty,//treasureItemListText.GetItemQty(textIndex),
                                  treasureItemListText.GetItemCharges(textIndex),
                                  treasureItemListText.GetItemIdentified(textIndex),
                                  GetActiveChar(m_pCallerEvent, party.activeCharacter),
                                  cost);
       
      if ((success) && (miscError == NoError))
      {
        switch (m_pCallerEvent->GetEventType())
        {
        case CombatTreasure:
        case GiveTreasure:
          ((GIVE_TREASURE_DATA *)m_pCallerEvent)->
                   itemsRemain.deleteItem(treasureItemListText.
                                          GetCharIndex(textIndex), 
                                          qty);
          break;
        case Vault:
          globalData.RemoveItemFromVault(((VAULT_EVENT_DATA*)m_pCallerEvent)->WhichVault, treasureItemListText.GetCharIndex(textIndex), qty);
          break;
          
        case Combat:
          ASSERT(FALSE);
          /*
          {
            // remove treasure from source combatant inventory
           int dude = treasureItemListText.GetItemData(textIndex); // original src char index
           int itemKey = treasureItemListText.GetCharIndex(textIndex); // original inv key
           combatData.combatants[dude].myItems.deleteItem(itemKey, qty);
          }
          */
          break;
        default:
          ASSERT(FALSE); // is this an error?
          break;
        }
        // rebuild item list text
        setTreasureEventState(m_pCallerEvent);
      }
    }
    break;
  case 4: // Halve
    ASSERT( (m_pCallerEvent != NULL) && (m_pCallerEvent->GetEventType() == Vault) );

    if (treasureItemListText.ItemCanBeHalved(textIndex))
    {
      globalData.vault[((VAULT_EVENT_DATA*)m_pCallerEvent)->WhichVault].items.halveItem(treasureItemListText.GetCharIndex(textIndex));     
      // rebuild item list text
      setTreasureEventState(m_pCallerEvent);
    }
    break;
  case 5: // Join
    ASSERT( (m_pCallerEvent != NULL) && (m_pCallerEvent->GetEventType() == Vault) );
    if (treasureItemListText.ItemCanBeJoined(textIndex))
    {
       globalData.vault[((VAULT_EVENT_DATA*)m_pCallerEvent)->WhichVault].items.joinItems(treasureItemListText.GetCharIndex(textIndex));
       // rebuild item list text
       setTreasureEventState(m_pCallerEvent);
    }
    break;
  case 6: // Exit    
    handleItemsFormInput(ITEMSFORM_exit, treasureItemListText);
    PopEvent();
    break;
  default:
    WriteDebugString("Unhandled choice in takeTreasureMenu()\n");
    miscError = InvalidMenuState;
    PopEvent();
    break;
  };
}

void TAKE_TREASURE_DATA::OnMouseClickLeft(int mx, int my)
{
  MouseMenuInventory(mx, my);
}

void TAKE_TREASURE_DATA::OnUpdateUI(void)
{
  if (CheckForMultiBoxText()) return;
/*
  1 { "NEXT", 0 },
  2 { "PREV", 0 },
  3 { "TAKE", 0 },
  4 { "HALVE", 0 },
  5 { "JOIN", 0 },
  6 { "EXIT", 1 }
};
*/
  if (treasureItemListText.GetCount() <= 0)
  {
    menu.setAllItemsInactive();
    menu.setItemActive(6); // exit
    return;
  }

  if (!allowTreasureItemNextPage())
    menu.setItemInactive(1); // next
  if (!allowTreasureItemPrevPage())
    menu.setItemInactive(2); // prev

  if (   (m_pCallerEvent != NULL) 
      && (m_pCallerEvent->GetEventType() == Vault))
  {
    switch (treasureItemListText.GetItemClass(currTreasureTextIndex()))
    {
    case itemType:   
      if (!treasureItemListText.ItemCanBeHalved(currTreasureTextIndex()))
      {
        menu.setItemInactive(4); // halve
        menu.setItemInactive(5); // join
      }  
      if (treasureItemListText.GetItemQty(currTreasureTextIndex()) <= 1)
      {
        menu.setItemInactive(4); // halve
      }
      break;
    case PlatinumType:
    case ElectrumType:
    case GoldType:
    case CopperType:
    case SilverType:
    case GemType:
    case JewelryType:
    case SpecialItemType:
    case SpecialKeyType:
    case CoinType6:
    case CoinType7:
    case CoinType8:
    case CoinType9:
    case CoinType10:
      menu.setItemInactive(4); // halve
      menu.setItemInactive(5); // join
      break;
    default:
      menu.setItemInactive(4); // halve
      menu.setItemInactive(5); // join
    };
  }
  else
  {
    // can never join/halve treasure except for vault event
    menu.setItemInactive(4); // halve
    menu.setItemInactive(5); // join
  }
}

void TAKE_TREASURE_DATA::OnDraw(void)
{
  GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);
  drawBorder();
  //displayTreasure();
  if (errorTextData.numLines == 0)
    handleItemsFormInput(ITEMSFORM_refresh, treasureItemListText);
  menu.DisplayMenu();
  DisplayTextAndFlip();
  //UpdateTakeTreasureScreen();
}

void TAKE_TREASURE_DATA::OnReturnToTopOfQueue(void)
{
  if ((party.tradeQty > 0) && (GetMyState() == TASK_TakeTreasureGetMoneyQty))
  {
    // returning here after GetMoneyQty pops itself
    //
    // add sum to character, and remove it from source
    BOOL success = takeTreasureMoney(treasureItemListText.GetItemClass(currTreasureTextIndex()),
                                     party.tradeQty,
                                     GetActiveChar(m_pCallerEvent,party.activeCharacter));
  
    // determine source of money
    if ((success) && (m_pCallerEvent!=NULL))
    {
      switch (m_pCallerEvent->GetEventType())
      {
      case Vault: // remove money from vault      
        globalData.UpdateMoneyInVault(((VAULT_EVENT_DATA*)m_pCallerEvent)->WhichVault,
                                      treasureItemListText.GetItemClass(currTreasureTextIndex()),
                                      -party.tradeQty);
        break;

      case ShopEvent:
      case TempleEvent: // remove from pooled funds
        party.deductPoolMoney(treasureItemListText.GetItemClass(currTreasureTextIndex()), party.tradeQty);
        break;

      case Combat:
        ASSERT(FALSE);
        /*
        {
          // remove treasure from source combatant inventory
         int dude = treasureItemListText.GetItemData(currTreasureTextIndex()); // original src char index
         itemClassType type = treasureItemListText.GetItemClass(currTreasureTextIndex());
          switch (type)
          {
          case ElectrumType:
          case GoldType:
          case SilverType:
          case CopperType:
          case CoinType6:
          case CoinType7:
          case CoinType8:
          case CoinType9:
          case CoinType10:
          case PlatinumType: // even pooled platinum is added to platinumRemain
            combatData.combatants[dude].money.Subtract(type, party.tradeQty);
            break;
          case GemType:
            combatData.combatants[dude].money.RemoveMultGems(party.tradeQty);
            break;
          case JewelryType:
            combatData.combatants[dude].money.RemoveMultJewelry(party.tradeQty);
            break;
          default:
            break;
          }
        }
        */
        break;

      case CombatTreasure:
      case GiveTreasure: // update treasureItemList money    
        {
          GIVE_TREASURE_DATA *data = 
               reinterpret_cast<GIVE_TREASURE_DATA*>(m_pCallerEvent);
          if (data != NULL)
          {
            // decrement temp event quantity
            switch (treasureItemListText.GetItemClass(currTreasureTextIndex()))
            {
            case ElectrumType:
            case GoldType:
            case SilverType:
            case CopperType:
            case CoinType6:
            case CoinType7:
            case CoinType8:
            case CoinType9:
            case CoinType10:
            case PlatinumType: // even pooled platinum is added to platinumRemain
              data->moneyRemain.Subtract(treasureItemListText.GetItemClass(currTreasureTextIndex()), party.tradeQty);
              break;
            case GemType:
              data->moneyRemain.RemoveMultGems(party.tradeQty);
              break;
            case JewelryType:
              data->moneyRemain.RemoveMultJewelry(party.tradeQty);
              break;
            default:
              break;
            }
          }
        }
        break;

      default:
        break;
      }
    }
  }

  OnInitialEvent();
}

////////////////////////////////////////////////////////////VIEW_CHARACTER_DATA

void VIEW_CHARACTER_DATA::OnInitialEvent(void)
{
  ClearFormattedText(textData);
  menu.setMenu(ViewCharMenuData);
  menu.setHorzOrient();
  menu.MapKeyCodeToMenuItem(KC_ESCAPE, 8);
  if ((m_pOrigEvent!=NULL) && (m_pOrigEvent->GetEventType()==(eventType)Combat))
  {
    // Need to set activeCharacter based upon current combatant
    //
    // If we get this far, the character being viewed must be a
    // party member.
    //
    // The party member may be an NPC pre-gen, or an original
    // created character.
    //

    int index = -1;
    // figure out which party member this equates to.
    for (int i=0;i<party.numCharacters;i++)
    {
      if (party.characters[i].IsSameCharacter(GetActiveChar(m_pOrigEvent)))
      {
        index = i;
        break;
      }
    }

    ASSERT( (index>=0) && (index < party.numCharacters) );
    if ( (index<0) || (index >= party.numCharacters) )
      index = 0;

    party.activeCharacter = index;
  }
  currPic = GetActiveChar(m_pOrigEvent,party.activeCharacter).smallPic;
  currPic.LoadPicSurfaces("prt_");

  CString examinetext(GetActiveChar(m_pOrigEvent,party.activeCharacter).ExamineLabel);
  if (examinetext.GetLength()>0)
  {
    menu.changeMenuItem(7, examinetext);
    menu.SetFirstLettersShortcuts(TRUE);
  }

  //GraphicsMgr.EnableFontColorTags(FALSE);
  SetMyState(TASK_initialized);
}

void VIEW_CHARACTER_DATA::OnKeypress(key_code key, char ascii)
{
  if (key != KC_RETURN)
  {
    StandardMenuKeyboardAction(key,ascii);
    return;
  };
  switch (menu.currentItem())
  {
  case 1: // Items
    PushEvent (new ITEMS_MENU_DATA(GetEventType(), m_pOrigEvent), DeleteEvent);
    break;
  case 2: // Display (was Spells - not used)
    PushEvent(new DISPLAY_SPELL_EFFECTS_MENU_DATA(m_pOrigEvent), DeleteEvent);    
    break;
  case 3: // Trade
    // never active - go to items screen to trade
    break;
  case 4: // Drop
    // never active - go to items screen to drop
    break;

    /*
    LAY is a healing ability of paladins.  Paladins may lay on hands and heal two
    hit points per level of damage a day.  Select the LAY command, then select the
    character to be healed.  This command is only displayed when a paladin has not
    yet laid on hands in a day.
    */
  case 5: // Lay On Hands - PALADIN ONLY
    {
      CHARACTER& dude = GetActiveChar(m_pOrigEvent);
      SetCharContext(dude.GetContext());
      SetTargetContext(dude.GetContext());

      traitType tdata = traitData.FindName("heal by laying on hands");
      double result=0.0;
      traitData.EvaluteTrait(tdata, result);

      if (result > 0.0)
        dude.SetHitPoints( dude.GetHitPoints() + result );

      // once per day
      dude.HasLayedOnHandsToday=TRUE;

      ClearTargetContext();
      ClearCharContext();
    }
    break;

    /*
    CURE is another healing ability of paladins--they may perform one Cure Disease
    per week for every 5 levels.  For example, at 1st to 5th
    levels, a paladin may perform one, at 6th to 10th levels two, etc.  This command
    is only displayed if the paladin has a cure available.
    */
  case 6: // Cure Disease - PALADIN ONLY
    break;


  case 7: // Examine chain
    {
      GameEvent *pEvent = globalData.eventData.GetEvent(GetActiveChar(m_pOrigEvent,party.activeCharacter).ExamineEvent);
      if ((pEvent==NULL)||(!globalData.eventData.IsValidEvent(pEvent)))
      {
        PushEvent(new DO_NOTHING_EVENT, DeleteEvent);
      }
      else 
      {
        if (AllowedAsGlobalEvent(pEvent->GetEventType()))
          PushGlobalEvent(pEvent, LevelEvent);
        else
          PushEvent(new DO_NOTHING_EVENT, DeleteEvent);
      }
    }
    break; 
  case 8: // Exit
    currPic.Clear();
    //GraphicsMgr.EnableFontColorTags(TRUE);
    PopEvent();
    break;
  };
}

void VIEW_CHARACTER_DATA::OnDraw(void)
{
  if (m_pOrigEvent!=NULL)
  {
    switch (m_pOrigEvent->GetEventType())
    {
    case Combat:
      UpdateCombatViewCharacterScreen();
      break;
    default:      
      UpdateViewCharacterScreen();
      break;
    }
  }
  else
    UpdateViewCharacterScreen();
}

void VIEW_CHARACTER_DATA::OnUpdateUI(void)
{
  if (CheckForMultiBoxText()) return;

  /*
  1   { "ITEMS", 0 },  
  2   { "DISPLAY", 0 },
  3   { "TRADE", 0 },  // always disabled, goto ITEMS menu for this
  4   { "DROP", 0 },   // always disabled, goto ITEMS menu for this
  5   { "LAY", 0 },
  6   { "CURE", 0 },
  7   { "EXAMINE", 0 },
  8   { "EXIT", 1 }

  in shop:
  in combat:
    always disable 5,6
  */

  // check conditional menu options
  menu.setItemInactive(4); // drop
  menu.setItemInactive(3); // trade

  if (!isPaladin(GetActiveChar(m_pOrigEvent,party.activeCharacter).GetClass()))
  {
    menu.setItemInactive(5); // lay on hands to heal
    menu.setItemInactive(6); // cure disease
  }
  else
  {
    if (GetActiveChar(m_pOrigEvent).HasLayedOnHandsToday)
      menu.setItemInactive(5); // lay
    else if (GetActiveChar(m_pOrigEvent).GetStatus() != Okay)
      menu.setItemInactive(5); // lay

    // disable this until it can actually do something
    menu.setItemInactive(6); // cure disease
  }

  // check unconditional menu disables
  if (m_pOrigEvent!=NULL)
  {
    switch (m_pOrigEvent->GetEventType())
    {
    case ShopEvent:
    case Combat:
      menu.setItemInactive(5); // lay
      menu.setItemInactive(6); // cure disease
      break;
    default:      
      break;
    }
  }

  if (   (!globalData.eventData.IsValidEvent(GetActiveChar(m_pOrigEvent,party.activeCharacter).ExamineEvent))
      || (GetActiveChar(m_pOrigEvent,party.activeCharacter).ExamineLabel.GetLength()==0))
    menu.setItemInactive(7); // examine
}

void VIEW_CHARACTER_DATA::OnReturnToTopOfQueue(void)
{
  OnInitialEvent();
}

////////////////////////////////////////////////////////////ITEMS_MENU_DATA

void ITEMS_MENU_DATA::OnInitialEvent(void)
{
  ClearFormattedText(textData);
  party.tradeQty = 0;
  menu.setMenu(ItemsMenuData);
  menu.setHorzOrient();
  menu.MapKeyCodeToMenuItem(KC_ESCAPE, 14);

  setCharItemTextState(GetActiveChar(m_pOrigEvent,party.activeCharacter));
  SetMyState(TASK_initialized);
  handleItemsFormInput(ITEMSFORM_exit, charItemListText);
  handleItemsFormInput(ITEMSFORM_initial, charItemListText);
}

void ITEMS_MENU_DATA::OnMouseClickLeft(int mx, int my)
{
  MouseMenuInventory(mx, my);
}

void ITEMS_MENU_DATA::OnKeypress(key_code key, char ascii)
{
  if (key != KC_RETURN)
  {
    HMenuVInventoryKeyboardAction(key, ascii);
    return;
  }
  int itemTextIndex = currCharItemsTextIndex();

  CHARACTER &dude = GetActiveChar(m_pOrigEvent,party.activeCharacter);

  switch (menu.currentItem()) 
  {
  case 1: // Ready
    if (itemCanBeReadied(charItemListText.GetItemClass(itemTextIndex)))
    {
      if ((m_pOrigEvent!=NULL) && (m_pOrigEvent->GetEventType()==(eventType)Combat))
        combatData.toggleReadyItem(charItemListText.GetCharIndex(itemTextIndex));
      else
        dude.toggleReadyItem(charItemListText.GetCharIndex(itemTextIndex));
      charItemListText.
        UpdateIsReady(itemTextIndex, dude.myItems.IsReady(charItemListText.GetCharIndex(itemTextIndex)));
    }
    break;
    
  case 2: // Use
    {
      ITEM data;
      int i = charItemListText.GetCharIndex(itemTextIndex);
      dude.myItems.GetItem(i ,data);

      // decrement available charges
      int ch = dude.myItems.GetCharges(i)-1;
      ch = max(0,ch);
      dude.myItems.SetCharges(i, ch);

      ITEM_DATA *idata = itemData.GetItemData(data.item);
      if (idata!=NULL)
      {
        if (idata->Spell != NO_SPELL)
        {
          dude.InvokeSpellOnTarget(dude, -1, idata->Spell);
        }
        
        if (!globalData.eventData.IsValidEvent(idata->event))
        {
          PushEvent(new DO_NOTHING_EVENT, DeleteEvent);
        }
        else 
        {
          if (GetRunAsVersion(ItemUseEventVersion) >= _VERSION_0681_)
          {
            GameEvent *pEvent = globalData.eventData.GetEvent(idata->event);
            if ((pEvent != NULL) && (AllowedAsGlobalEvent(pEvent->GetEventType())))
              PushGlobalEvent(idata->event);
            else
              PushEvent(new DO_NOTHING_EVENT, DeleteEvent);
          }
          else // not safe, didn't work properly in old versions
            PushEvent(new DO_NOTHING_EVENT, DeleteEvent);
        }
      }
    }
    break;

  case 3: // Trade
    if (itemCanBeTraded(charItemListText.GetItemClass(itemTextIndex)))
      PushEvent(new TRADE_TAKER_SELECT_MENU_DATA(m_pOrigEvent),DeleteEvent);
    break;

  case 4: // Drop item
    if (itemCanBeDropped(charItemListText.GetItemClass(itemTextIndex)))
    {
      if (!itemIsMoney(charItemListText.GetItemClass(itemTextIndex)))
      {
        if (!dude.myItems.IsReady(charItemListText.GetCharIndex(itemTextIndex)))
        {
          if ((m_pOrigEvent!=NULL) && (m_pOrigEvent->GetEventType()==(eventType)Combat))
          {
            combatData.delCharacterItem(charItemListText.GetCharIndex(itemTextIndex), 
                                  charItemListText.GetItemQty(itemTextIndex));
          }
          else
          {
            switch (charItemListText.GetItemClass(currCharItemsTextIndex()))
            {
            case itemType:
              dude.delCharacterItem(charItemListText.GetCharIndex(itemTextIndex), 
                                    charItemListText.GetItemQty(itemTextIndex));
              break;
            case SpecialItemType:
              {
                int i = charItemListText.GetCharIndex(itemTextIndex);
                if (party.hasSpecialItem(i))
                  party.removeSpecialItem(i);
              }
              break;
            case SpecialKeyType:
              {
                int i = charItemListText.GetCharIndex(itemTextIndex);
                if (party.hasSpecialKey(i))
                  party.removeSpecialKey(i);
              }
              break;
            }
          }
          // rebuild item list
          setCharItemTextState(dude);
        }
        else
          miscError = ItemIsReadied;
      }
      else 
      {
        PushEvent(new GET_MONEY_QTY_DATA(GetEventType(),m_pOrigEvent),DeleteEvent);
        return;
      }
    }
    break;

  case 5: // Deposit (vault only)
    ASSERT( m_pOrigEvent->GetEventType() == Vault );
    
    if (itemCanBeDeposited(charItemListText.GetItemClass(itemTextIndex)))
    {
      if (!itemIsMoney(charItemListText.GetItemClass(itemTextIndex)))
      {
        if (!dude.myItems.IsReady(charItemListText.GetCharIndex(itemTextIndex)))
        {
          ITEM data;
          dude.myItems.GetItem(charItemListText.GetCharIndex(itemTextIndex) ,data);
          globalData.AddItemToVault(((VAULT_EVENT_DATA*)m_pOrigEvent)->WhichVault, data);
          dude.delCharacterItem(charItemListText.GetCharIndex(itemTextIndex), 
                           charItemListText.GetItemQty(itemTextIndex));
          // rebuild item list
          setCharItemTextState(dude);
        }
        else
          miscError = ItemIsReadied;
      }
      else 
      {
        PushEvent(new GET_MONEY_QTY_DATA(GetEventType(),m_pOrigEvent),DeleteEvent);
        return;
      }
    }
    break;

  case 6: // Halve
    if (charItemListText.ItemCanBeHalved(itemTextIndex))
    {
      dude.myItems.halveItem(charItemListText.GetCharIndex(itemTextIndex));   
      // rebuild item list
      setCharItemTextState(dude);
    }
    break;

  case 7: // Join
    if (charItemListText.ItemCanBeJoined(itemTextIndex))
    {
       dude.myItems.joinItems(charItemListText.GetCharIndex(itemTextIndex));
       // rebuild item list
       setCharItemTextState(dude);
    }
    break;

  case 8: // sell
    if (m_pOrigEvent != NULL)
    {
      ASSERT(m_pOrigEvent->GetEventType()==ShopEvent);
      if (m_pOrigEvent->GetEventType()==ShopEvent)
      {
        SHOP *pData = (SHOP*)m_pOrigEvent;
        if (pData->buybackPercentage > 0)
        {          
          int i = charItemListText.GetCharIndex(itemTextIndex);          
          int paid = dude.myItems.GetPurchasePrice(i);
          ITEM data;
          dude.myItems.GetItem(i ,data);

          if (paid < 0)
            paid = itemData.GetItemCost(data.item);

          int bqty = itemData.GetItemBundleQty(data.item);
          double unitprice = (double)paid/(double)bqty;
          
          int worth = unitprice * (double)dude.myItems.GetQty(i);

          pData->sellprice = worth * (((double)pData->buybackPercentage)/100.0);
          if (pData->sellprice < 0) pData->sellprice=0;
          if (pData->sellprice > worth) pData->sellprice=worth;
          SetMyState(TASK_AskToSellItem);
          CString msg;
          msg.Format("I WILL GIVE YOU %i %s FOR THIS ITEM, SELL IT?",
                      pData->sellprice, globalData.moneyData.GetName(globalData.moneyData.GetDefaultType()));
          PushEvent(new ASK_YES_NO_MENU_DATA(msg,TRUE, CONTROL_ItemsMenu), DeleteEvent);
        }
      }
    }
    break;

  case 9: // id item
    if (m_pOrigEvent != NULL)
    {
      ASSERT(m_pOrigEvent->GetEventType()==ShopEvent);
      if (m_pOrigEvent->GetEventType()==ShopEvent)
      {
        SHOP *pData = (SHOP*)m_pOrigEvent;
        if (pData->costToId > 0)
        {
          SetMyState(TASK_AskToIdItem);
          CString msg;
          if (GetActiveChar(m_pOrigEvent,party.activeCharacter).enoughMoney(pData->costToId,0,0,globalData.moneyData.GetDefaultType()))
          {
            msg.Format("IDENTIFICATION WILL COST %i %s, PAY IT?", pData->costToId, globalData.moneyData.GetName(globalData.moneyData.GetDefaultType()));
            PushEvent(new ASK_YES_NO_MENU_DATA(msg,TRUE, CONTROL_ItemsMenu), DeleteEvent);      
          }
          else
          {
            msg.Format("THE COST OF IDENTIFICATION IS %i %s\nYOU DO NOT HAVE ENOUGH", pData->costToId, globalData.moneyData.GetName(globalData.moneyData.GetDefaultType()));
            PushEvent(new ASK_YES_NO_MENU_DATA(msg,FALSE, CONTROL_ItemsMenu), DeleteEvent);
          }        
        }
        else
        {
          // id is free
          int i = charItemListText.GetCharIndex(itemTextIndex);
          dude.myItems.SetId(i, TRUE);
        }
      }
    }
    break;
  case 10: // Examine chain
    {
      switch (charItemListText.GetItemClass(currCharItemsTextIndex()))
      {
        case itemType:
        {
          ITEM data;
          int i = charItemListText.GetCharIndex(itemTextIndex);
          dude.myItems.GetItem(i ,data);
          ITEM_DATA *idata = itemData.GetItemData(data.item);
          if (idata!=NULL)
          {
            GameEvent *pEvent = globalData.eventData.GetEvent(idata->ExamineEvent);
            if ((pEvent==NULL)||(!globalData.eventData.IsValidEvent(pEvent)))
            {
              PushEvent(new DO_NOTHING_EVENT, DeleteEvent);
            }
            else 
            {
              if (AllowedAsGlobalEvent(pEvent->GetEventType()))
                PushGlobalEvent(pEvent, LevelEvent);
              else
                PushEvent(new DO_NOTHING_EVENT, DeleteEvent);
            }
          }
        }
        break;
      }
    }
    break; 
  case 11: // Examine chain
    {
      switch (charItemListText.GetItemClass(currCharItemsTextIndex()))
      {
        case SpecialItemType:
        {
          int i = charItemListText.GetCharIndex(itemTextIndex);
          GameEvent *pEvent = globalData.eventData.GetEvent(globalData.specialItemData.GetExamineEvent(i));
          if ((pEvent==NULL)||(!globalData.eventData.IsValidEvent(pEvent)))
          {
            PushEvent(new DO_NOTHING_EVENT, DeleteEvent);
          }
          else 
          {
            if (AllowedAsGlobalEvent(pEvent->GetEventType()))
              PushGlobalEvent(pEvent, LevelEvent);
            else
              PushEvent(new DO_NOTHING_EVENT, DeleteEvent);
          }
        }
        break;

        case SpecialKeyType:
        {
          int i = charItemListText.GetCharIndex(itemTextIndex);
          GameEvent *pEvent = globalData.eventData.GetEvent(globalData.keyData.GetExamineEvent(i));
          if ((pEvent==NULL)||(!globalData.eventData.IsValidEvent(pEvent)))
          {
            PushEvent(new DO_NOTHING_EVENT, DeleteEvent);
          }
          else 
          {
            if (AllowedAsGlobalEvent(pEvent->GetEventType()))
              PushGlobalEvent(pEvent, LevelEvent);
            else
              PushEvent(new DO_NOTHING_EVENT, DeleteEvent);
          }
        }
        break;
      }
    }
    break; 
  case 12: // Next
    nextCharItemsPage();
    break;
  case 13: // Prev
    prevCharItemsPage();
    break;
  case 14: // Exit
    clearCharItemState();
    handleItemsFormInput(ITEMSFORM_exit, charItemListText);
    PopEvent();
    return;
  }
}

void ITEMS_MENU_DATA::OnDraw(void)
{
  GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);
  drawBorder();
  if (errorTextData.numLines == 0)
    handleItemsFormInput(ITEMSFORM_refresh, charItemListText);
  menu.DisplayMenu();
  DisplayTextAndFlip();
}

void ITEMS_MENU_DATA::OnUpdateUI(void)
{
  int i;
  if (CheckForMultiBoxText()) return;

  /*
   1{ "READY", 0 },
   2{ "USE", 0 },
   3{ "TRADE", 0 },
   4{ "DROP", 0 },
   5{ "DEPOSIT", 2 },
   6{ "HALVE", 0 },
   7{ "JOIN", 0 },
   8{ "SELL", 0 },
   9{ "ID ITEM", 0 },
   10{ "EXAMINE", -1 }, // normal items
   11{ "EXAMINE", -1 }, // special items and keys
   12{ "NEXT", 0 },
   13{ "PREV", 0 },
   14{ "EXIT", 1 }
  */

  if (charItemListText.GetCount() <= 0)
  {
    menu.setAllItemsInactive();
    menu.setItemActive(14);
    return;
  }

  CHARACTER &dude = GetActiveChar(m_pOrigEvent,party.activeCharacter);
  i = charItemListText.GetCharIndex(currCharItemsTextIndex());

  if (m_pOrigEvent!=NULL)
  {
    switch (m_pOrigEvent->GetEventType())
    {
    case ShopEvent:
      {
        menu.setItemInactive(5); // deposit (only active when in a vault)
        
        if ( ((SHOP*)m_pOrigEvent)->buybackPercentage <= 0 )
          menu.setItemInactive(8); // sell (won't buy back items)

        if (dude.myItems.IsReady(i))
          menu.setItemInactive(8); // sell (must not be readied)

        if ( !((SHOP*)m_pOrigEvent)->canId )
          menu.setItemInactive(9); // id (not allowed for this shop)
        
        if (((SHOP*)m_pOrigEvent)->buyItemsSoldOnly)
        {
          if (!((SHOP*)m_pOrigEvent)->itemsAvail.HaveItem(dude.myItems.GetItem(i)))
            menu.setItemInactive(8); // sell (must not be readied)
        }        
      }
      break;
    case Vault:
      menu.setItemInactive(4); // drop (only active when *not* in a vault)
      menu.setItemInactive(8); // sell
      menu.setItemInactive(9); // id
      break;
    default:
      menu.setItemInactive(5); // deposit (only active when in a vault)
      menu.setItemInactive(8); // sell
      menu.setItemInactive(9); // id
      break;
    }
  }
  else
  {
    menu.setItemInactive(5); // deposit (only active when in a vault)
    menu.setItemInactive(8); // sell    (only active when in a shop)
    menu.setItemInactive(9); // id      (only active when in a shop)
  }  

  if (!dude.IsAbleToTrade())
    menu.setItemInactive(3); // trade

  // item class checks
  if (!itemCanBeTraded(charItemListText.GetItemClass(currCharItemsTextIndex())))
    menu.setItemInactive(3); // trade  
  if (!itemCanBeDropped(charItemListText.GetItemClass(currCharItemsTextIndex())))
    menu.setItemInactive(4); // drop
  if (!itemCanBeDeposited(charItemListText.GetItemClass(currCharItemsTextIndex())))
    menu.setItemInactive(5); // deposit
  if (!itemCanBeSold(charItemListText.GetItemClass(currCharItemsTextIndex())))
    menu.setItemInactive(8); // sell
  
  // specific item database checks
  if (!itemCanBeTraded(dude.myItems.GetItem(i)))
    menu.setItemInactive(3); // trade  
  if (!itemCanBeDropped(dude.myItems.GetItem(i)))
    menu.setItemInactive(4); // drop
  if (!itemCanBeDeposited(dude.myItems.GetItem(i)))
    menu.setItemInactive(5); // deposit
  if (!itemCanBeSold(dude.myItems.GetItem(i)))
    menu.setItemInactive(8); // sell
  
  if (dude.myItems.GetCount() == 0)
  {
    menu.setItemInactive(8); // sell
    menu.setItemInactive(9); // id
  }

  switch (charItemListText.GetItemClass(currCharItemsTextIndex()))
  {
  case itemType:
    {     
      //i = charItemListText.GetCharIndex(currCharItemsTextIndex());

      ITEM_DATA *pdata = itemData.GetItemData(dude.myItems.GetItem(i));
      ASSERT(pdata != NULL);

      if (dude.myItems.IsReady(i))
      {
        menu.changeMenuItem(1, "UNREADY", 2);
        menu.setItemInactive(3); // trade
        menu.setItemInactive(4); // drop

        if (!itemData.GetItemUsable(dude.myItems.GetItem(i)))      
        {
          menu.setItemInactive(2); // use 
        }
        else 
        {
          // older versions didn't store the charge count
          if (GetRunAsVersion(ItemUseEventVersion) >= _VERSION_0690_)
          {
            if (dude.myItems.GetCharges(i) <= 0)
              menu.setItemInactive(2); // use 
          }
        }
      }
      else
      {
        if ((pdata==NULL) || (pdata->Location_Carried != CannotBeReadied))
        {
          // not 'useable' if not readied
          menu.setItemInactive(2); // use
          menu.changeMenuItem(1, "READY", 0);
        }
        else
        {
          // item that does not need to be readied
          menu.setItemInactive(1); // ready
          
          if (!itemData.GetItemUsable(dude.myItems.GetItem(i)))
            menu.setItemInactive(2); // use
        }
      }

      if (itemData.GetItemBundleQty(dude.myItems.GetItem(i)) <= 1)
      {
        menu.setItemInactive(6); // halve
        menu.setItemInactive(7); // join
      }  
      if (dude.myItems.GetQty(i) <= 1)
      {
        menu.setItemInactive(6); // halve
      }  
      if (dude.myItems.GetQty(i) >= MAX_ITEM_QTY)
      {
        menu.setItemInactive(7); // join
      } 
      if (!itemCanBeReadied(dude.myItems.GetItem(i)))
        menu.setItemInactive(1); // ready

      //ITEM_DATA *pdata = itemData.GetItemData(dude.myItems.GetItem(i));
      if (pdata != NULL)
      {
        if (   (!globalData.eventData.IsValidEvent(pdata->ExamineEvent))
            || (pdata->ExamineLabel.GetLength()==0))
        {
          menu.setItemInactive(10); // examine
        }
        else
        {
          CString examinetext(pdata->ExamineLabel);
          if (examinetext.GetLength()>0)
          {
            menu.changeMenuItem(10, examinetext);
            menu.SetFirstLettersShortcuts(TRUE);
          }
        }
      }
      else
        menu.setItemInactive(10); // examine
  
      menu.setItemInactive(11); // examine

      // cannot be used if charges are used up
      if (dude.myItems.GetCharges(i) <= 0)
        menu.setItemInactive(2); // use
    }
    break;
  case PlatinumType:
  case ElectrumType:
  case GoldType:
  case CopperType:
  case SilverType:
  case GemType:
  case JewelryType:
  case CoinType6:
  case CoinType7:
  case CoinType8:
  case CoinType9:
  case CoinType10:
    menu.setItemInactive(1); // ready
    menu.setItemInactive(2); // use
    menu.setItemInactive(6); // halve
    menu.setItemInactive(7); // join
    menu.setItemInactive(8); // sell
    menu.setItemInactive(9); // id
    menu.setItemInactive(10); // examine
    menu.setItemInactive(11); // examine
    break;
  case SpecialItemType:
    menu.setItemInactive(1); // ready
    menu.setItemInactive(2); // use
    menu.setItemInactive(3); // trade
    menu.setItemInactive(6); // halve
    menu.setItemInactive(7); // join
    menu.setItemInactive(8); // sell
    menu.setItemInactive(9); // id
    menu.setItemInactive(10); // examine
    if (!globalData.specialItemData.CanBeDropped(i))
      menu.setItemInactive(4); // drop

    if (   (!globalData.eventData.IsValidEvent(globalData.specialItemData.GetExamineEvent(i)))
        || (globalData.specialItemData.GetExamineLabel(i).GetLength()==0))
    {
      menu.setItemInactive(11); // examine
    }
    else
    {
      CString examinetext(globalData.specialItemData.GetExamineLabel(i));
      if (examinetext.GetLength()>0)
      {
        menu.changeMenuItem(11, examinetext);
        menu.SetFirstLettersShortcuts(TRUE);
      }
    }
    break;
  case SpecialKeyType:
    menu.setItemInactive(1); // ready
    menu.setItemInactive(2); // use
    menu.setItemInactive(3); // trade
    menu.setItemInactive(6); // halve
    menu.setItemInactive(7); // join
    menu.setItemInactive(8); // sell
    menu.setItemInactive(9); // id
    menu.setItemInactive(10); // examine
    if (!globalData.keyData.CanBeDropped(i))
      menu.setItemInactive(4); // drop

    if (   (!globalData.eventData.IsValidEvent(globalData.keyData.GetExamineEvent(i)))
        || (globalData.keyData.GetExamineLabel(i).GetLength()==0))
    {
      menu.setItemInactive(11); // examine
    }
    else
    {
      CString examinetext(globalData.keyData.GetExamineLabel(i));
      if (examinetext.GetLength()>0)
      {
        menu.changeMenuItem(11, examinetext);
        menu.SetFirstLettersShortcuts(TRUE);
      }
    }
    break;
  default: // empty item list
    menu.setItemInactive(1); // ready
    menu.setItemInactive(2); // use
    menu.setItemInactive(3); // trade
    menu.setItemInactive(4); // drop
    menu.setItemInactive(5); // deposit
    menu.setItemInactive(6); // halve
    menu.setItemInactive(7); // join
    menu.setItemInactive(8); // sell
    menu.setItemInactive(9); // id
    menu.setItemInactive(10); // examine
    menu.setItemInactive(11); // examine
    break;
  };

  if (!allowCharItemNextPage())
    menu.setItemInactive(12); // next
  if (!allowCharItemPrevPage())
    menu.setItemInactive(13); // prev
}

void ITEMS_MENU_DATA::OnReturnToTopOfQueue(void)
{
  switch (GetMyState())
  {
  case TASK_AskToIdItem:
    {
      if (party.tradeItem==1) // yes, id the item
      {
        CHARACTER &dude = GetActiveChar(m_pOrigEvent,party.activeCharacter);
        SHOP *pData = (SHOP*)m_pOrigEvent;
        int i = charItemListText.GetCharIndex(currCharItemsTextIndex());
        dude.payForItem(pData->costToId, globalData.moneyData.GetDefaultType());
        dude.myItems.SetId(i,TRUE);
      }
    }
    break;

  case TASK_AskToSellItem:
    {
      if (party.tradeItem==1) // yes, sell the item
      {
        CHARACTER &dude = GetActiveChar(m_pOrigEvent,party.activeCharacter);
        SHOP *pData = (SHOP*)m_pOrigEvent;
        int i = charItemListText.GetCharIndex(currCharItemsTextIndex());
        dude.money.Add(globalData.moneyData.GetDefaultType(), pData->sellprice);
        dude.myItems.DeleteItem(i);
      }
    }
    break;

  default:
    {
      if (party.tradeQty > 0)
      {
        // either dropping money or depositing it in the vault
        // and you can't drop in a vault
        if ((m_pOrigEvent != NULL) && (m_pOrigEvent->GetEventType() == Vault))
        {
          // add money to vault
          globalData.UpdateMoneyInVault(((VAULT_EVENT_DATA*)m_pOrigEvent)->WhichVault,
                                        charItemListText.GetItemClass(currCharItemsTextIndex()),
                                        party.tradeQty);                           
        }
        GetActiveChar(m_pOrigEvent,party.activeCharacter).removeCharMoney(charItemListText.GetItemClass(currCharItemsTextIndex()), party.tradeQty);
        party.tradeQty=0;
      }
    }
    break;
  }

  OnInitialEvent();
}

////////////////////////////////////////////////////////////GET_MONEY_QTY_DATA
void GET_MONEY_QTY_DATA::OnKeypress(key_code key, char ascii)
{
  if (   (key != KC_RETURN)
      && (key != KC_NUM)
      && (key != KC_BACK))
  {
    return;
  };
  char temp[10], temp2[10];
  int i, sum; 
  
  if (key == KC_RETURN)
  {
    temp[0] = '\0';
    if (menu.totalItems() > 0)
    {
      for (i=1;i<menu.totalItems();i++)
        strcat(temp, menu.getMenuItem(i));      
      sum = atoi(temp); 
      party.tradeQty = sum;
      PopEvent();
    };
  }
  else 
  {
    int qty;
    switch (m_CallerEventType) 
    {
    case CONTROL_TakeTreasure:
      qty = treasureItemListText.GetItemQty(currTreasureTextIndex());
      break;
    case CONTROL_ItemsMenu:
    case CONTROL_TradeTakerSelect:
//    case GS_CombatItemsMenu:
      qty = charItemListText.GetItemQty(currCharItemsTextIndex());
      break;
    default:
      WriteDebugString("Unhandled game state in GET_MONEY_QTY_DATA()\n");
      miscError = InvalidGameState;
      qty = 0;
    };
    if (key == KC_NUM)          
    {
      temp[0] = '\0';
      
      // ignore last char, it is a blank
      for (i=1;i<menu.totalItems();i++)
        strcat(temp, menu.getMenuItem(i));
      
      sprintf(temp2, "%c", ascii);
      strcat(temp, temp2);
      sum = atoi(temp);
      
      if ((sum > qty) || (sum < 0))
      {
        i=menu.totalItems();
        
        while (i > 0)
        {
          menu.deleteLastMenuItem();
          i--;
        }
        
        itoa(qty, temp, 10);
        
        for (i=0;i<(int)strlen(temp);i++)
        {
          strncpy(temp2, &temp[i], 1);
          temp2[1] = '\0';
          menu.addMenuItem(temp2);
        }
        
        menu.addMenuItem(" ");
        menu.setCurrentItem(menu.totalItems());
      }
      else 
      {
        if (menu.totalItems() == 0)
        {
          menu.addMenuItem(temp2);
          menu.addMenuItem(" ");
        }
        else 
        {
          menu.changeMenuItem(menu.totalItems(), temp2);
          menu.addMenuItem(" ");
        }
        menu.setCurrentItem(menu.totalItems());
      };

    }
    else if (   ((key == KC_BACK) || (key == KC_LEFT))
             && (menu.totalItems() > 0) )
    {
      menu.deleteLastMenuItem();
      menu.changeMenuItem(menu.totalItems(), " ");      
      if (menu.totalItems() == 0)
        menu.addMenuItem(" ");      
      menu.setCurrentItem(menu.totalItems());
    };
  };
}

void GET_MONEY_QTY_DATA::OnInitialEvent(void)
{
  ClearFormattedText(textData);
  party.tradeQty = 0;
  menu.setMenu(GetMoneyQtyMenuData);
  menu.setHorzOrient();
  menu.setItemSeparation(0);
  itemClassType theType;
  switch (m_CallerEventType) 
  {
  case CONTROL_TakeTreasure:
    theType = treasureItemListText.GetItemClass(currTreasureTextIndex());
    break;
  case CONTROL_TradeTakerSelect:
  case CONTROL_ItemsMenu:
    theType = charItemListText.GetItemClass(currCharItemsTextIndex());
    break;
  default:
    theType = itemType;
    WriteDebugString("Unhandled state in GET_MONEY_QTY_DATA\n");
    miscError = InvalidGameState;
    break;
  };
  CString tmp;
  switch (theType) 
  {
  case PlatinumType:
  case ElectrumType:
  case GoldType:
  case SilverType:
  case CopperType:
  case CoinType6:
  case CoinType7:
  case CoinType8:
  case CoinType9:
  case CoinType10:
  case GemType:
  case JewelryType:
    tmp.Format("HOW MANY %s?:", globalData.moneyData.GetName(theType));
    menu.setTitle(tmp);
    break;
  default:
    menu.setTitle("HOW MANY?:");
    break;
  };
  menu.addMenuItem(" ");
  menu.setCurrentItem(menu.totalItems());
}

void GET_MONEY_QTY_DATA::OnMouseClickLeft(int mx, int my)
{
  // no mouse input allowed
}

void GET_MONEY_QTY_DATA::OnDraw(void)
{
  switch (m_CallerEventType) 
  {
  case CONTROL_TakeTreasure:
    GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);
    drawBorder();
    if (errorTextData.numLines == 0)
      handleItemsFormInput(ITEMSFORM_refresh, treasureItemListText);
    menu.DisplayMenu();
    DisplayTextAndFlip();
    break;
  case CONTROL_TradeTakerSelect:
  case CONTROL_ItemsMenu:
    GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);
    drawBorder();
    if (errorTextData.numLines == 0)
      handleItemsFormInput(ITEMSFORM_refresh, charItemListText);
    menu.DisplayMenu();
    DisplayTextAndFlip();
    break;
  default:
    GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);
    drawBorder();
    if (errorTextData.numLines == 0)
      handleItemsFormInput(ITEMSFORM_refresh, charItemListText);
    menu.DisplayMenu();
    DisplayTextAndFlip();
    break;
  }
}

//////////////////////////////////////////////////////////ENCAMP_MENU_DATA
void ENCAMP_MENU_DATA::OnInitialEvent(void)
{    
  menu.setMenu(EncampMenuData);
  menu.setHorzOrient();    
  SetMyState(TASK_initialized);
  
  menu.MapKeyCodeToMenuItem(KC_ESCAPE, 10);  

  // if using Camp event
  if ((m_pOrigEvent != NULL) && (m_pOrigEvent->GetEventType() == Camp))
  {
    FormatDisplayText(textData, m_pOrigEvent->GetEventText());
    if (m_pOrigEvent->GetEventPic().picType != BogusDib)
    {
      currPic = m_pOrigEvent->GetEventPic();
      currPic.LoadPicSurfaces("sp_");
    }
    else
    {
      int zone = levelData.GetCurrZone(party.Posx, party.Posy);
      currPic = levelData.zoneData.zones[zone].campArt;
      currPic.LoadPicSurfaces("sp_");
    };
  }
  else
  {
    ClearFormattedText(textData);
    // no event, just 'Encamp' menu option
    int zone = levelData.GetCurrZone(party.Posx, party.Posy);
    currPic = levelData.zoneData.zones[zone].campArt;
    currPic.LoadPicSurfaces("sp_");
  }

  PlayCampMusic();
}

void ENCAMP_MENU_DATA::OnDraw(void)
{
  UpdateEncampScreen();
}

void ENCAMP_MENU_DATA::OnUpdateUI(void)
{
  if (CheckForMultiBoxText()) return;
  CHARACTER& dude = GetActiveChar(m_pOrigEvent);  

  if (!levelData.zoneData.zones[levelData.area[party.Posy][party.Posx].zone].AllowMagic)
    menu.setItemInactive(4); // magic
  
  //if (!dude.canCastSpells()) 
  //  menu.setItemInactive(4); // magic  

  int zone = levelData.GetCurrZone(party.Posx, party.Posy);
  if (!levelData.zoneData.zones[zone].restEvent.allowResting)
  {
    menu.setItemInactive(5); // rest
    menu.setItemInactive(7); // fix
  }

  if (   (!globalData.eventData.IsValidEvent(dude.TalkEvent))
      || (dude.TalkLabel.GetLength()==0))
  {
    menu.setItemInactive(8); // talk
  }
  else
  {
    menu.changeMenuItem(8,dude.TalkLabel);
    // try to give custom talk menu text a shortcut
    menu.SetFirstLettersShortcuts(TRUE);
  }

  if ((dude.DisableTalkIfDead)&&(dude.GetAdjStatus()!=Okay))
    menu.setItemInactive(8); // talk

  if (party.journal.GetCount() == 0)
    menu.setItemInactive(9); // journal

  if (m_pOrigEvent!=NULL)
  {
    if (m_pOrigEvent->IsGlobalEvent())
      menu.setItemInactive(1); // save
  }
  if (IsGlobalEvent())
  {
    menu.setItemInactive(1); // save
    menu.setItemInactive(2); // load
  }
  if (IsCombatActive())
  {
    menu.setItemInactive(1); // save
    menu.setItemInactive(2); // load
  }
}

void ENCAMP_MENU_DATA::OnKeypress(key_code key, char ascii)
{
  if (TABParty(key)) return;
  if (key != KC_RETURN)
  {
    HMenuVPartyKeyboardAction(key,ascii);
    return;
  };

  /*
1    { "SAVE", 0 },
2    { "LOAD", 0 },
3    { "VIEW", 0 },
4    { "MAGIC", 0 },
5    { "REST", 0 },
6    { "ALTER", 0 },
7    { "FIX", 0 },
8    { "TALK", 0 },
9    { "JOURNAL", 0 },
10   { "EXIT", 1 },
11   { "LEAVE GAME", 6 }
  */
  switch (menu.currentItem()) 
  {
  case 1: // Save
    PushEvent(new SAVEGAME_MENU_DATA, DeleteEvent);
    break;      
  case 2: // Load
    PushEvent(new LOADGAME_MENU_DATA, DeleteEvent);
    break;      
  case 3: // View
    PushEvent(new VIEW_CHARACTER_DATA(GetEventType(),NULL), DeleteEvent);
    break;
  case 4: // Magic
    PushEvent(new MAGIC_MENU_DATA(m_pOrigEvent), DeleteEvent);
    break;
  case 5: // Rest
    PushEvent(new REST_MENU_DATA(m_pOrigEvent), DeleteEvent);
    break;
  case 6: // Alter    
    PushEvent(new ALTER_GAME_MENU_DATA(), DeleteEvent);
    break;
  case 7: // Fix (using party spells)
    party.FixParty(TRUE);
    break;
  case 8: // Talk chain
    if (!globalData.eventData.IsValidEvent(GetActiveChar(m_pOrigEvent).TalkEvent))
    {
      PushEvent(new DO_NOTHING_EVENT, DeleteEvent);
    }
    else 
    {
      if (GetRunAsVersion(ItemUseEventVersion) >= _VERSION_0681_)
      {
        GameEvent *pEvent = globalData.eventData.GetEvent(GetActiveChar(m_pOrigEvent).TalkEvent);
        if ((pEvent != NULL) && (AllowedAsGlobalEvent(pEvent->GetEventType())))
          PushGlobalEvent(GetActiveChar(m_pOrigEvent).TalkEvent);
        else
          PushEvent(new DO_NOTHING_EVENT, DeleteEvent);
      }
      else // not safe, didn't work properly in old versions
        PushEvent(new DO_NOTHING_EVENT, DeleteEvent);
    }
    break; 
  case 9: // journal
    PushEvent( new DISPLAY_PARTY_JOURNAL_DATA(), DeleteEvent);
    break;

  case 10: // Exit menu
    {
      currPic.Clear();
    
      PlayCampMusic(FALSE);// if camp music playing, stop it

      if (WaitUntilReturnPressed()) ClearFormattedText(textData);
      PopEvent(); // CAMP_EVENT pushes ENCAMP and calls ChainHappened() upon return

      // restart background music if available
      StartBackgroundMusic();
    }
    break;

  case 11: // exit UAF
    SetMyState(TASK_AskQuitGame);
    PushEvent(new ASK_YES_NO_MENU_DATA("QUIT THE GAME?",TRUE, CONTROL_Encamp), DeleteEvent);
    //ReplaceEvent(new EXIT_DATA, DeleteEvent);
    break;
  };
}

void ENCAMP_MENU_DATA::OnReturnToTopOfQueue()
{
  if (GetMyState()==TASK_AskQuitGame)
  {
    if (party.tradeItem==1) // yes, quit the game
    {
      ReplaceEvent(new EXIT_DATA, DeleteEvent);
      return;
    } 
  }

  OnInitialEvent();
}

int ENCAMP_MENU_DATA::OnSaveGame(unsigned int *saveArea)
{
  if (m_pOrigEvent==NULL)
  {
    saveArea[0]=0;
  }
  else
  {
    saveArea[0]=m_pOrigEvent->GetEventId();
  };
  return 1;
}

int ENCAMP_MENU_DATA::OnLoadGame(unsigned int *saveArea)
{
  if (saveArea[0]==0) m_pOrigEvent=NULL;
  else m_pOrigEvent=levelData.eventData.GetEvent(saveArea[0]);
  return 1;
}

//////////////////////////////////////////////////////////TRANSFER_EVENT_DATA


void TRANSFER_EVENT_DATA::OnInitialEvent(void)
{
  FormatDisplayText(textData, GetEventText());
  currPic = GetEventPic();
  currPic.LoadPicSurfaces("sp_");

  if (askYesNo)
  {
    menu.setMenu(TransferMenuData);
    menu.setHorzOrient();
    SetMyState(TASK_TransferMenu);
  }
  else
  {
    //ClearFormattedText(textData);
    if ((textData.numLines==0) || (currPic.IsPicLoaded()))
    {
      menu.reset();
      SetMyState(TASK_TransferGo);
    }
    else
    {
      menu.setMenu(PressEnterMenuData);
      menu.setHorzOrient();
      SetMyState(TASK_TransferGo);
    }
  }
}

void TRANSFER_EVENT_DATA::OnDraw(void)
{
  if (activateBeforeEntry)
  {
    //if (GetMyState() != TASK_TransferGo)
      UpdateTransferMenuScreen();
  }
  else
  {
    UpdateTransferMenuScreen();
  }
}

void TRANSFER_EVENT_DATA::OnKeypress(key_code key, char ascii)
{
  if (TABParty(key)) return;
  if (key != KC_RETURN)
  {
    HMenuVPartyKeyboardAction(key,ascii);
    return;
  };
  switch (GetMyState())
  {
  case TASK_TransferGo:
    HandleTransfer(transferData, GetEventText2(), currPic);
    break;

  case TASK_TransferMenu:
    currPic.Clear();    
    ClearFormattedText(textData);
    switch (menu.currentItem()) 
    {
    case 1: // Yes
      if (transferOnYes)
      {
        SetMyState(TASK_TransferGo);
      }
      else
      {
        SetMyState(TASK_TransferStay);
      };
      break;
    case 2: // No
      if (!transferOnYes)
      {
        SetMyState(TASK_TransferGo);
      }
      else
      {
        SetMyState(TASK_TransferStay);
      };
      break;
    }; // Case MenuItem()
    break;

    default:
      break;
  };
}

bool TRANSFER_EVENT_DATA::OnIdle(void)
{
//  int newLevel, oldLevel;
  switch (GetMyState())
  {
  case TASK_TransferGo:
    if (textData.numLines==0)
    {
      HandleTransfer(transferData, GetEventText2(), currPic);
      return false;
    }
    break;

  case TASK_TransferStay:
    if (activateBeforeEntry)
    {
      TaskMessage(TASKMSG_CancelForwardMove);
    };
    PopEvent();
    return false;

  default:
    break;
  };
  return true;
}

unsigned int TRANSFER_EVENT_DATA::OnTaskMessage(TASKMESSAGE msg)
{
  switch (msg)
  {
  case TASKMSG_TeleportWithinLevel:
  case TASKMSG_TeleportBetweenLevels:
    return TASKMSG_DELETE; // I'm outta here.
  default:
    break;
  };
  return 0;
}
////////////////////////////////////////////////////////////////////////TAVERN
void TAVERN::OnInitialEvent(void)
{
  menu.setMenu(TavernMenuData);
  menu.setHorzOrient();
  menu.MapKeyCodeToMenuItem(KC_ESCAPE, 3);
  FormatDisplayText(textData, GetEventText());
  SetMyState(TASK_TavernMenu);
  currPic = GetEventPic();
  currPic.LoadPicSurfaces("sp_");

  for (int i=0; i<MAX_TALES; i++)
    tales[i].count = 0;
}

void TAVERN::OnDraw(void)
{
  switch (GetMyState())
  {
  case TASK_TavernDrink:
    UpdateTavernMenuScreen();
    break;
  case TASK_TavernMenu:
    UpdateTavernMenuScreen();
    //ClearFormattedText(textData);
    break;
  default:
    break;
  }; // switch (GetMyState())
}

void TAVERN::OnUpdateUI(void)
{
  int i;
  if (CheckForMultiBoxText()) return;
  switch (GetMyState())
  {
  case TASK_TavernDrink:
    for (i=0;i<MAX_DRINKS;i++)
    {
      if (drinks[i].name.GetLength() == 0)
        menu.setItemInactive(i+1);
    }
    break;
  case TASK_TavernMenu:
    if (!allowDrinks)
      menu.setItemInactive(2); // drinks
    if (!allowFights)
      menu.setItemInactive(1); // fights
    break;
  default:
    break;
  }; // switch (GetMyState())
}

BOOL IsUsableTale(const TALE &tale, BOOL OnceOnly)
{
  if (tale.tale.IsEmpty())
    return FALSE;

  if (OnceOnly)
  {
    if (tale.count > 0)
      return FALSE;
  }

  return TRUE;
}

void TAVERN::OnKeypress(key_code key, char ascii)
{
  static int LastRumor = -1;
  BOOL found;
  int i, index;

  if (TABParty(key)) return;
  if (key != KC_RETURN)
  {
    HMenuVPartyKeyboardAction(key, ascii);
    return;
  };
  switch (GetMyState())
  {
  case TASK_TavernMenu:
    switch (menu.currentItem()-1) 
    {
    case 0: // Fight
      if (levelData.eventData.IsValidEvent(fightChain))
      {
        currPic.Clear();
        ClearFormattedText(textData);
        ReplaceEvent(levelData.eventData.GetEvent(fightChain), LevelEvent);
        return;
      }
      else
      {
        FormatDisplayText(textData, "Everyone runs away. There's no one to fight!");
      };
      break;
    case 1: // Drink
      found = FALSE;
      i=0;
      while ((i < MAX_DRINKS) && (!found))
      {
        if (drinks[i].name.GetLength() > 0)
          found = TRUE;
        i++;
      }
      
      if (found)
      {
        SetMyState(TASK_TavernDrink);
        menu.reset();
        menu.setStartCoord(USE_DEFAULT_HORZ_MENU, 0);
        menu.setHorzOrient();
        menu.setItemSeparation(7);
        FormatDisplayText(textData, "What is your pleasure?");
        currPic = GetEventPic();
        currPic.LoadPicSurfaces("sp_");
        for (i=0;i<MAX_DRINKS;i++)
        {
          if (drinks[i].name.GetLength() > 0)
          {
            menu.addMenuItem(drinks[i].name);
          }
          else 
          {
            // need a placeholder to properly determine drink points
            // when handling menu selections. These empty drink names
            // will be disabled in OnUpdateUI
            int newItem = menu.addMenuItem(" ");
            menu.setItemInactive(newItem);
          }
        }
        menu.addMenuItem("DONE");

        menu.SetFirstLettersShortcuts();
        menu.setCurrentItem(0);        
        menu.MapKeyCodeToMenuItem(KC_ESCAPE, menu.totalItems());
      }
      break;
    case 2: // Talk
      if (taleOrder == InOrderTales)
      {
        int r;
        if (LastRumor == -1)
        {
          // search for non-empty tale
          r=0;
          while ((r < MAX_TALES) && (!IsUsableTale(tales[r], EachTaleOnceOnly))) //(tales[r].tale.GetLength() == 0))
            r++;
          if (r < MAX_TALES)
          {
            LastRumor = r;
            tales[r].count++;
            FormatDisplayText(textData, tales[r].tale);
          }
        }
        else
        {
          r=LastRumor+1;
          BOOL StartedSearchWithFirstRumor = FALSE;
          
          if (r >= MAX_TALES)
          {
            StartedSearchWithFirstRumor = TRUE;
            r = 0;
          }

          // search for non-empty tales in r..MAX_TALES-1
          while ((r < MAX_TALES) && (!IsUsableTale(tales[r], EachTaleOnceOnly))) //(tales[r].tale.GetLength() == 0))
            r++;

          if ((r >= MAX_TALES) && (!StartedSearchWithFirstRumor))
          {
            // no rumor in r..MAX_TALES-1, so look in 0..r-1
            // if we did not already start with r = 0
            r=0;
            while ((r < MAX_TALES) && (!IsUsableTale(tales[r], EachTaleOnceOnly))) //(tales[r].tale.GetLength() == 0))
              r++;
          }

          if (r < MAX_TALES)
          {
            LastRumor = r;
            tales[r].count++;
            FormatDisplayText(textData, tales[r].tale);
          }
          else
          {
            LastRumor = -1;
            ClearFormattedText(textData);
          }
        }
      }
      else // pick a random non-empty rumor
      {
        int ValidRumors[MAX_TALES];
        memset(ValidRumors, 0, sizeof(ValidRumors));
        int numValid=0;
        for (i=0; i<MAX_TALES; i++)
        {
          if (IsUsableTale(tales[i], EachTaleOnceOnly)) //(tales[i].tale.GetLength() > 0)
          {
            ValidRumors[numValid] = i;
            numValid++;
          };
        }
        if (numValid > 0)
        {
          // get random number 0..numValid-1
          int rd = (rollDice(numValid, 1, 0)-1);
          tales[ValidRumors[rd]].count++;
          FormatDisplayText(textData, tales[ValidRumors[rd]].tale);
        }
        else
        {
          FormatDisplayText(textData,"This seems to be a quiet crowd");
        };
      };
      break;

    case 3: // Exit
      party.setPartyAdventureState();
      if (ForcePartyBackup()) TaskMessage(TASKMSG_MovePartyBackward);
      currPic.Clear();
      ChainHappened();
      return;
      
    }; // case menuItem()
    break;

  case TASK_TavernDrink:
    //if (drinkPointTrigger <= 0)
    //  return; // Drink as much as you like
    index = menu.currentItem()-1;
    if ((index >= 0) && (index < MAX_DRINKS))
    {
      DRINK &drink = drinks[index];
      CString temp;
      temp.Format("You drink some %s.", drinks[index].name);
      FormatDisplayText(textData, temp);

      if (drinkPointTrigger > 0)
      {
        party.drinkPoints += drink.points;
        if (party.drinkPoints >= drinkPointTrigger)
        {
          if (levelData.eventData.IsValidEvent(drinkChain))
          {
            ReplaceEvent(levelData.eventData.GetEvent(drinkChain),LevelEvent);
          };
        }
      }
    }
    else
    {
      if (index == MAX_DRINKS) // Done
      {
        OnInitialEvent(); // Probably should not repeat entry text
      };
    };
    break;

  default:
    break;
  }; // switch (GetMyState())    
}

bool TAVERN::OnIdle(void)
{
  return true;
}

//////////////////////////////////////////////////////////////GIVE_DAMAGE_DATA
void GIVE_DAMAGE_DATA::OnInitialEvent(void)
{
  menu.setMenu(PressEnterMenuData);
  menu.setHorzOrient();
  FormatDisplayText(textData, GetEventText());
  currPic = GetEventPic();
  currPic.LoadPicSurfaces("sp_");
  if (currPic.picType == SpriteDib)
    currPic.SetFrame(distance);
  if (ShouldSimulateEnterKey()) 
    simulatedKeyQueue.PushKey(VK_RETURN);
}

void GIVE_DAMAGE_DATA::OnKeypress(key_code key, char ascii)
{
  if (TABParty(key)) return;
  if (key != KC_RETURN) return;
  int i,result;
  if ((who == ActiveChar)||(who==OneAtRandom))
    nbrAttacks=1;

  for (int a=0;a<nbrAttacks;a++)
  {
    switch (who)
    {
    case NoPartyMember:
      break;

    case EntireParty:
      for (i=0;i<party.numCharacters;i++)
      {
        SetCharContext(party.characters[i].GetContext());
        SetTargetContext(party.characters[i].GetContext());

        party.characters[i].
          giveCharacterDamage(eventSave, 
                              attackTHACO, 
                              dmgDice, 
                              dmgDiceQty, 
                              dmgBonus, 
                              spellSave, 
                              saveBonus);
        ClearTargetContext();
        ClearCharContext();
      };
      break;

    case ActiveChar:
      if (party.activeCharacter >= 0)
      {
        SetCharContext(party.characters[party.activeCharacter].GetContext());
        SetTargetContext(party.characters[party.activeCharacter].GetContext());

        party.characters[party.activeCharacter].
          giveCharacterDamage(eventSave, 
                              attackTHACO, 
                              dmgDice, 
                              dmgDiceQty, 
                              dmgBonus, 
                              spellSave, 
                              saveBonus);
        ClearTargetContext();
        ClearCharContext();
      };
      break;

     case OneAtRandom:
        i = rollDice(party.numCharacters, 1);

        SetCharContext(party.characters[i-1].GetContext());
        SetTargetContext(party.characters[i-1].GetContext());

        party.characters[i-1].
          giveCharacterDamage(eventSave, 
                              attackTHACO, 
                              dmgDice, 
                              dmgDiceQty, 
                              dmgBonus, 
                              spellSave, 
                              saveBonus);
        ClearTargetContext();
        ClearCharContext();
     break;

     case ChanceOnEach:
       for (i=0;i<party.numCharacters;i++)
       {
         result = rollDice(100,1);
         if (result <= chancePerAttack)
         {
            SetCharContext(party.characters[i].GetContext());
            SetTargetContext(party.characters[i].GetContext());

            party.characters[i].
              giveCharacterDamage(eventSave, 
                                  attackTHACO, 
                                  dmgDice, 
                                  dmgDiceQty, 
                                  dmgBonus, 
                                  spellSave, 
                                  saveBonus);
            ClearTargetContext();
            ClearCharContext();
         };
       };
       break;
     }; // switch (who)
  }; // for nbr of attacks

  party.setPartyAdventureState();      
  ChainHappened();   
}


void GIVE_DAMAGE_DATA::OnDraw(void)
{
  UpdateGiveDamageScreen();
}

//////////////////////////////////////////////////////////////GAIN_EXP_DATA
void GAIN_EXP_DATA::OnInitialEvent(void)
{
  menu.setMenu(PressEnterMenuData);
  menu.setHorzOrient();
  FormatDisplayText(textData, GetEventText());
  currPic = GetEventPic();
  currPic.LoadPicSurfaces("sp_");
  if (PlayEventSounds)
    hSound = PlaySound(sound);

  // check difficulty settings for exp mods
  double mod = GetAllExpMod(); 

  tempExp = experience;
  if (mod != 0.0) tempExp += ((mod/100.0)*tempExp);
  //if (tempExp < 0) tempExp=0;
}

void GAIN_EXP_DATA::OnKeypress(key_code key, char ascii)
{
  if (TABParty(key)) return;
  if (key != KC_RETURN) return;
  int i,result;  
  switch (who)
  {
  case NoPartyMember:
    break;
  case EntireParty:
    for (i=0;i<party.numCharacters;i++)
      party.characters[i].giveCharacterExperience(tempExp);
    break;
  case ActiveChar:
    i = party.activeCharacter;
    if (i >= 0)
      party.characters[i].giveCharacterExperience(tempExp);
    break;
  case OneAtRandom:
    i = party.numCharacters;
    result = rollDice(i,1);
    i = result;
    party.characters[i-1].giveCharacterExperience(tempExp);
    break;
  case ChanceOnEach:
    for (i=0;i<party.numCharacters;i++)
    {
      result = rollDice(100,1);
      if (result <= chance)
      {
        party.characters[i].giveCharacterExperience(tempExp);
      }
    }
    break;
  }; // switch (who)
  
  party.setPartyAdventureState();      
  currPic.Clear();
  ClearSound(hSound);

  for (i=0;i<party.numCharacters;i++)
  {
    // sets character name to different color when
    // ready to train.
    party.characters[i].SetReadyToTrain(party.characters[i].IsReadyToTrain());
  }

  ChainHappened();
}


void GAIN_EXP_DATA::OnDraw(void)
{
  UpdateGainExpScreen();
}

/////////////////////////////////////////////////////////////HEAL_PARTY_DATA
void HEAL_PARTY_DATA::OnInitialEvent(void)
{
  //if (IsCombatActive())
  //{
  //  party.HealParty(*this);
  //  ChainHappened();
  //}
  //else
  {
    menu.setMenu(PressEnterMenuData);
    menu.setHorzOrient();
    FormatDisplayText(textData, GetEventText());
    currPic = GetEventPic();
    currPic.LoadPicSurfaces("sp_");
  }
}

void HEAL_PARTY_DATA::OnKeypress(key_code key, char ascii)
{
  if (TABParty(key)) return;
  if (key != KC_RETURN) return;
  party.HealParty(*this);
  party.setPartyAdventureState();      
  currPic.Clear();
  ChainHappened();
}


void HEAL_PARTY_DATA::OnDraw(void)
{
  UpdateHealPartyScreen();
}

////////////////////////////////////////////////////////WHO_PAYS_EVENT_DATA
void WHO_PAYS_EVENT_DATA::OnInitialEvent(void)
{
  menu.setMenu(WhoPaysMenuData);
  menu.setHorzOrient();
  menu.enableTitle(FALSE);
  FormatDisplayText(textData,GetEventText());
  currPic = GetEventPic();
  currPic.LoadPicSurfaces("sp_");
  SetMyState(TASK_WhoPaysGet);
}

void WHO_PAYS_EVENT_DATA::OnKeypress(key_code key, char ascii)
{
  bool failed=false;  
  if (TABParty(key)) return;
  if (key != KC_RETURN) 
  {
    HMenuVPartyKeyboardAction(key, ascii);
    return;
  }
  CHARACTER &dude = GetActiveChar(this);
  switch (GetMyState())
  {
  case TASK_WhoPaysGet:
    // if impossible to pay 
    //   go to fail
    // else
    //   get selected char
    //   check char funds against event requirements
    //   if sufficient funds
    //     subtract funds from char
    //     go to success
    //   else
    //     go to fail
    //   end
    // end

    failed = false;
    if (menu.currentItem() == 1) // select
    {      
      if (!impossible)
      {
        if (!dude.enoughMoney(platinum,
                              gems,
                              jewels,
                              moneyType))
        {
          failed = true;
        }
      }
      else
        failed = true;
    }
    else
    {
      // nobody chosen to pay, exit event
      switch (failAction)
      {
      case NoAction:
        ChainHappened();
        return;
      case ChainEvent: // chained event will be detected later
        ChainOrQuit(failChain);
        return;    
      case Teleport: 
        currPic.Clear();
        HandleTransfer(failTransferData,"", currPic);
        break;      
      case BackupOneStep:
        TaskMessage(TASKMSG_MovePartyBackward);
        ChainHappened();
        break;
      }; // switch (failAction)
      return;
    }

    if (failed)
    {
      SetMyState(TASK_WhoPaysFailure);
      menu.setMenu(PressEnterMenuData);
      menu.setHorzOrient();
      menu.enableTitle(FALSE);
      FormatDisplayText(textData, GetEventText3());
      currPic = GetEventPic();
      currPic.LoadPicSurfaces("sp_");
      if (GetEventText3().GetLength() <= 0)
        simulatedKeyQueue.PushKey(VK_RETURN);
      return;
    }
    else
    {
      menu.setMenu(PressEnterMenuData);
      menu.setHorzOrient();
      menu.enableTitle(FALSE);
      FormatDisplayText(textData, GetEventText2());
      currPic = GetEventPic();
      currPic.LoadPicSurfaces("sp_");
      dude.payForItem(platinum, moneyType, gems, jewels);
      SetMyState(TASK_WhoPaysSuccess);
      if (GetEventText2().GetLength() <= 0)
        simulatedKeyQueue.PushKey(VK_RETURN);
      return;
    };
    break;

  case TASK_WhoPaysFailure:
    currPic.Clear();
    ClearFormattedText(textData);
    switch (failAction)
    {
    case NoAction:
      ChainHappened();
      return;
    case ChainEvent: // chained event will be detected later
      ChainOrQuit(failChain);
      return;    
    case Teleport: 
      currPic.Clear();
      HandleTransfer(failTransferData,"", currPic);
      break;      
    case BackupOneStep:
      TaskMessage(TASKMSG_MovePartyBackward);
      ChainHappened();
      break;
    }; // switch (failAction)
    break;
  case TASK_WhoPaysSuccess:
    currPic.Clear();
    ClearFormattedText(textData);
    switch (successAction)
    {
    case NoAction:
      ChainHappened();
      return;
    case ChainEvent: // chained event will be detected later
      ChainOrQuit(successChain);
      return;
    case Teleport: 
      currPic.Clear();
      HandleTransfer(successTransferData,"", currPic);    
      break;
    case BackupOneStep:
      TaskMessage(TASKMSG_MovePartyBackward);
      ChainHappened();
      break;
    }; // Switch (successAction)
    break;

  default:
    break;
  }; // switch (GetMyState())
}


void WHO_PAYS_EVENT_DATA::OnDraw(void)
{
  UpdateWhoPaysScreen();
}


////////////////////////////////////////////////////////////PASS_TIME_EVENT_DATA
void PASS_TIME_EVENT_DATA::OnInitialEvent(void)
{
  menu.setMenu(PressEnterMenuData);
  menu.setHorzOrient();
  FormatDisplayText(textData, GetEventText());
  currPic = GetEventPic();
  currPic.LoadPicSurfaces("sp_");
  if (ShouldSimulateEnterKey()) 
    simulatedKeyQueue.PushKey(VK_RETURN);
}

void PASS_TIME_EVENT_DATA::OnKeypress(key_code key, char ascii)
{
  if (TABParty(key)) return;
  if (key != KC_RETURN)
      return;
 
  currPic.Clear();
  if (WaitUntilReturnPressed()) ClearFormattedText(textData);  

  if (PassSilent)
  {
    DWORD minuteTotal = (this->Days*1440) + (this->Hours*60) + this->Minutes;
    for (DWORD d=0;d<minuteTotal;d++)
      party.incrementClock(1); 
    ChainHappened();
  }
  else if (SetTime)
  {
    DWORD minuteTotal=0;

    if (this->Days > 0)    minuteTotal += this->Days*1440;
    else minuteTotal += party.days*1440;

    if (this->Hours > 0)   minuteTotal += this->Hours*60;
    else minuteTotal += party.hours*60;

    if (this->Minutes > 0) minuteTotal += this->Minutes;
    else minuteTotal += party.minutes;

    DWORD currTotal = party.getElapsedMinutes();
    
    int delta = minuteTotal - currTotal;
    if (delta > 0)
    {
      for (int d=0;d<delta;d++)
        party.incrementClock(1); 
    }
    ChainHappened();
  }
  else
    PushEvent(new PASSING_TIME_EVENT_DATA(this->Days,this->Hours,this->Minutes,AllowStop), DeleteEvent);
}

void PASS_TIME_EVENT_DATA::OnDraw(void)
{
  UpdatePassTimeScreen();
}

void PASS_TIME_EVENT_DATA::OnReturnToTopOfQueue(void)
{
  ChainHappened();
}

////////////////////////////////////////////////////////////PASSING_TIME_EVENT_DATA

__int64 GetMinuteDelta(int days, int hours, int mins)
{
  __int64 minuteDelta = 15;
  __int64 minuteTotal = (days*1440) + (hours*60) + mins;
  
  if (minuteTotal <= 0) return 0;

  // pass time faster when longer time
  // remains to wait
  /*
  if (minuteTotal >= 181*1440)      // for anything greater than 180 days
  {
    double tmp = minuteTotal;
    tmp *= 0.05;                    //    5% of what's left
    minuteDelta = tmp;
  }
  else if (minuteTotal >= 180*1440) // 180 days
    minuteDelta = 1440*60;          //    60 days
  else if (minuteTotal >= 120*1440) // 120 days
    minuteDelta = 1440*30;          //    30 days
  else if (minuteTotal >= 90*1440)  // 90 days
    minuteDelta = 1440*20;          //    20 days
  else*/ if (minuteTotal >= 60*1440)  // 60 days
    minuteDelta = 1440*14;          //    14 days  
  else if (minuteTotal >= 30*1440)  // 30 days
    minuteDelta = 1440*2;           //    2 days
  else if (minuteTotal >= 2*1440)   // 2 days
    minuteDelta = 1440;             //    1 day
  else if (minuteTotal >= 1440)     // 24 hours
    minuteDelta = 120;              //    2 hours
  else if (minuteTotal >= 12*60)    // 12 hours
    minuteDelta = 60;               //    1 hour
  else if (minuteTotal >= 6*60)     // 6 hours
    minuteDelta = 30;               //    30 mins
  else if (minuteTotal >= 60)       // 1 hour
    minuteDelta = 15;               //    15 mins
  else if (minuteTotal >= 30)       // 1/2 hour
    minuteDelta = 5;                //    5 mins
  else if (minuteTotal >= 15)       // 1/4 hour
    minuteDelta = 1;                //    1 min
  else
    minuteDelta = 1;

  if (minuteTotal < minuteDelta)
    minuteDelta = 1;

  return minuteDelta;
}

void PASSING_TIME_EVENT_DATA::OnInitialEvent(void)
{
  menu.reset();
  if (AllowStop)
  {
    menu.setStartCoord(USE_DEFAULT_HORZ_MENU, 0);
    menu.addMenuItem("PRESS ENTER TO STOP", 7);    
    menu.setHorzOrient();
  }
  FormatDisplayText(textData, GetEventText());
}

void PASSING_TIME_EVENT_DATA::OnKeypress(key_code key, char ascii)
{ 
  if (TABParty(key)) return;
  if (AllowStop)
  {
    if (key != KC_RETURN)
      return;
    currPic.Clear();    
    PopEvent();
  }
}

bool PASSING_TIME_EVENT_DATA::OnCycle(void)
{  
  __int64 dleft = daysLeft;
  __int64 hleft = hoursLeft;
  __int64 mleft = minutesLeft;

  __int64 minuteDelta = GetMinuteDelta(dleft, hleft, mleft);
  __int64 minuteTotal = (dleft*1440) + (hleft*60) + mleft;

  for (__int64 delta=0;delta<minuteDelta;delta++)
    party.incrementClock(1);
  
  minuteTotal -= minuteDelta;
  if (minuteTotal > 0)
  {
    dleft = minuteTotal / 1440;
    minuteTotal %= 1440;
    hleft = minuteTotal / 60;
    minuteTotal %= 60;
    mleft = minuteTotal;
  }
  else
  {
    dleft = 0;
    hleft = 0;
    mleft = 0;
  }

  dleft = max(0, dleft);
  hleft = max(0, min(23,hleft));
  mleft = max(-1, min(59,mleft));

  daysLeft = dleft;
  hoursLeft = hleft;
  minutesLeft = mleft;

  GameEvent::OnCycle();
  return true;
}

void PASSING_TIME_EVENT_DATA::OnDraw(void)
{ 
  // format new message box text
  CString ptext;
  ptext.Format("Passing Time: %I64i days, %I64i hours, %I64i minutes...",
              daysLeft, hoursLeft, minutesLeft);
  FormatDisplayText(textData, ptext, FALSE); // turn off slow text    
  UpdatePassTimeScreen();

  if (!((daysLeft > 0) || (hoursLeft > 0) || (minutesLeft > 0)))
  {
    currPic.Clear();    
    PopEvent();
  }
}

////////////////////////////////////////////////////////////RANDOM_EVENT_DATA
void RANDOM_EVENT_DATA::OnInitialEvent(void)
{
  menu.setMenu(PressEnterMenuData);
  menu.setHorzOrient();
  FormatDisplayText(textData, GetEventText());
  currPic = GetEventPic();
  currPic.LoadPicSurfaces("sp_");

  if (ShouldSimulateEnterKey())
    simulatedKeyQueue.PushKey(VK_RETURN);
}

void RANDOM_EVENT_DATA::OnKeypress(key_code key, char ascii)
{
  if (TABParty(key)) return;
  if (key != KC_RETURN)
  {
    HMenuVPartyKeyboardAction(key,ascii);
    //StandardMenuKeyboardAction(key, ascii);
    return;
  }

  int i;
  int total=0;
  int count=0;
  DWORD eventlist[RANDOM_EVENT_DATA::MAX_RANDOM_EVENTS];
  DWORD chancelist[RANDOM_EVENT_DATA::MAX_RANDOM_EVENTS];

  memset(eventlist, 0, sizeof(eventlist));
  memset(chancelist, 0, sizeof(chancelist));

  for (i=1;i<RANDOM_EVENT_DATA::MAX_RANDOM_EVENTS;i++)
  {
    bool valid=false;
    if (IsGlobalEvent())
    {
      valid = (   (globalData.eventData.IsValidEvent(EventChains[i-1]))
               && (EventChance[i-1] > 0));
    }
    else
    {
      valid = (   (levelData.eventData.IsValidEvent(EventChains[i-1]))
               && (EventChance[i-1] > 0));
    }

    if (valid)
    {
      eventlist[count]=EventChains[i-1];
      chancelist[count]=EventChance[i-1];
      total += EventChance[i-1];
      count++;
    }
  }

  if ((count==0) || (total == 0))
  {
    ChainHappened();
    return;
  }

  int result = rollDice(total, 1, 0.0);
  total=0;
  int index = -1;

  for (i=0;i<count;i++)
  {
    total += chancelist[i];
    if (total >= result)
    {
      index = i;
      break;
    }
  }

  if (index >= 0)
  {
    if (IsGlobalEvent())
      ReplaceGlobalEvent(eventlist[index]);
    else
      ReplaceEvent(eventlist[index]);
  }
  else
    ChainHappened();
}

void RANDOM_EVENT_DATA::OnDraw(void)
{
  UpdateRandomEventScreen();
}

////////////////////////////////////////////////////////////SMALL_TOWN_DATA

void SMALL_TOWN_DATA::OnInitialEvent(void)
{
  menu.setMenu(SmallTownMenuData);
  menu.setHorzOrient();
  menu.MapKeyCodeToMenuItem(KC_ESCAPE, 7);
  FormatDisplayText(textData, GetEventText());
  currPic = GetEventPic();
  currPic.LoadPicSurfaces("sp_");
}

void SMALL_TOWN_DATA::OnKeypress(key_code key, char ascii)
{
  if (TABParty(key)) return;
  if (key != KC_RETURN)
  {
    HMenuVPartyKeyboardAction(key,ascii);
    //StandardMenuKeyboardAction(key, ascii);
    return;
  };

  switch (menu.currentItem())
  {
  case 1: // temple
    if (!levelData.eventData.IsValidEvent(TempleChain))
        PushEvent(new DO_NOTHING_EVENT, DeleteEvent);
    else PushEvent(TempleChain);
    currPic.Clear();
    ClearFormattedText(textData);
    break;
  case 2: // training hall
    if (!levelData.eventData.IsValidEvent(TrainingHallChain))
        PushEvent(new DO_NOTHING_EVENT, DeleteEvent);
    else PushEvent(TrainingHallChain);
    currPic.Clear();
    ClearFormattedText(textData);
    break;
  case 3: // shop
    if (!levelData.eventData.IsValidEvent(ShopChain))
        PushEvent(new DO_NOTHING_EVENT, DeleteEvent);
    else PushEvent(ShopChain);
    currPic.Clear();
    ClearFormattedText(textData);
    break;
  case 4: // Inn
    if (!levelData.eventData.IsValidEvent(InnChain))
        PushEvent(new DO_NOTHING_EVENT, DeleteEvent);
    else PushEvent(InnChain);
    currPic.Clear();
    ClearFormattedText(textData);
    break;
  case 5: // Tavern
    if (!levelData.eventData.IsValidEvent(TavernChain))
        PushEvent(new DO_NOTHING_EVENT, DeleteEvent);
    else PushEvent(TavernChain);
    currPic.Clear();
    ClearFormattedText(textData);
    break;
  case 6: // Vault
    if (!levelData.eventData.IsValidEvent(VaultChain))
        PushEvent(new DO_NOTHING_EVENT, DeleteEvent);
    else PushEvent(VaultChain);
    currPic.Clear();
    ClearFormattedText(textData);
    break;
  case 7: // exit
    currPic.Clear();
    if (WaitUntilReturnPressed()) ClearFormattedText(textData);
    ChainHappened();
    break;
  }
}

void SMALL_TOWN_DATA::OnDraw(void)
{
  UpdateSmallTownScreen();
}

void SMALL_TOWN_DATA::OnReturnToTopOfQueue(void)
{
  currPic.Clear();
  ClearFormattedText(textData);
  
  menu.setMenu(SmallTownMenuData);
  menu.setHorzOrient();
  FormatDisplayText(textData, GetEventText());
  currPic = GetEventPic();
  currPic.LoadPicSurfaces("sp_");
}

////////////////////////////////////////////////////////////ADD_NPC_DATA
void ADD_NPC_DATA::OnInitialEvent(void)
{
  menu.setMenu(AddNPCData);
  menu.setHorzOrient();
  FormatDisplayText(textData, GetEventText());
  currPic = GetEventPic();
  currPic.LoadPicSurfaces("prt_");
  if (currPic.picType == SpriteDib)
    currPic.SetFrame(distance);
  // no text and no pic? then auto exit
  if (ShouldSimulateEnterKey())
    simulatedKeyQueue.PushKey(VK_RETURN);
}

void ADD_NPC_DATA::OnKeypress(key_code key, char ascii)
{
  if (TABParty(key)) return;
  if (key != KC_RETURN)
  {
    HMenuVPartyKeyboardAction(key,ascii);
    //StandardMenuKeyboardAction(key,ascii);
    return;
  }

  currPic.Clear();
  if (WaitUntilReturnPressed()) ClearFormattedText(textData);

  if (!party.addNPCToParty(charAdded, hitPointMod, useOriginal))
  {
    WriteDebugString("Failed to add NPC in ADD_NPC_DATA::OnKeypress()\n");
    if (charAdded > 0) // if NPC specified (value is init to zero)
      miscError = NPCPartyLimitReached;
  }

  ChainHappened();
}

void ADD_NPC_DATA::OnDraw(void)
{
  UpdateAddNPCScreen();
}

////////////////////////////////////////////////////////////REMOVE_NPC_DATA
void REMOVE_NPC_DATA::OnInitialEvent(void)
{
  menu.setMenu(RemoveNPCData);
  menu.setHorzOrient();
  FormatDisplayText(textData, GetEventText());
  currPic = GetEventPic();
  currPic.LoadPicSurfaces("prt_");
  if (currPic.picType == SpriteDib)
    currPic.SetFrame(distance);
  // no text and no pic? then auto exit
  if (ShouldSimulateEnterKey())
    simulatedKeyQueue.PushKey(VK_RETURN);
}

void REMOVE_NPC_DATA::OnKeypress(key_code key, char ascii)
{
  if (TABParty(key)) return;
  if (key != KC_RETURN)
  {
    HMenuVPartyKeyboardAction(key,ascii);
    //StandardMenuKeyboardAction(key,ascii);
    return;
  }

  currPic.Clear();
  if (WaitUntilReturnPressed()) ClearFormattedText(textData);
  party.removeNPCFromParty(charRemoved);
  ChainHappened();
}

void REMOVE_NPC_DATA::OnDraw(void)
{
  UpdateRemoveNPCScreen();
}

////////////////////////////////////////////////////////////NPC_SAYS_DATA
void NPC_SAYS_DATA::OnInitialEvent(void)
{
  menu.setMenu(NPCSaysData);
  menu.setHorzOrient();
  FormatDisplayText(textData, GetEventText());
  currPic = GetEventPic();
  currPic.LoadPicSurfaces("prt_");
  if (currPic.picType == SpriteDib)
    currPic.SetFrame(distance);
  if (PlayEventSounds) hSound = PlaySound(sound);  
}

void NPC_SAYS_DATA::OnKeypress(key_code key, char ascii)
{
  if (TABParty(key)) return;
  if (key != KC_RETURN)
  {
    HMenuVPartyKeyboardAction(key,ascii);
    //StandardMenuKeyboardAction(key,ascii);
    return;
  }

  currPic.Clear();
  // let text continue being displayed if return not needed
  if (WaitUntilReturnPressed()) ClearFormattedText(textData);
  ClearSound(hSound);
  ChainHappened();
}

void NPC_SAYS_DATA::OnDraw(void)
{
  UpdateNPCSaysScreen();
}

void NPC_SAYS_DATA::OnUpdateUI(void)
{
  if (CheckForMultiBoxText()) return;
  if (ShouldSimulateEnterKey()) simulatedKeyQueue.PushKey(VK_RETURN);
}

////////////////////////////////////////////////////////////SHOP

void SHOP::OnInitialEvent(void)
{
  menu.setMenu(ShopMenuData);
  menu.setHorzOrient();
  menu.MapKeyCodeToMenuItem(KC_ESCAPE, 8);
  FormatDisplayText(textData, GetEventText());
  currPic = GetEventPic();
  currPic.LoadPicSurfaces("sp_");
}          

void SHOP::OnKeypress(key_code key, char ascii)
{
  if (TABParty(key)) return;
  if (key != KC_RETURN)
  {
    HMenuVPartyKeyboardAction(key,ascii);
    return;
  };

  switch (menu.currentItem())
  {
  case 1: // Buy    
    PushEvent(new BUY_ITEMS_DATA(GetEventType(),this),DeleteEvent);
    break;
  case 2: // Items (view items on character)
    PushEvent(new ITEMS_MENU_DATA(GetEventType(),this),DeleteEvent);
    break;
  case 3: // View (character stats page)
    PushEvent(new VIEW_CHARACTER_DATA(GetEventType(),this), DeleteEvent);
    break;
  case 4: // Take (pooled treasure/money)
    setTreasureEventState(this);
    PushEvent(new TAKE_TREASURE_DATA(this), DeleteEvent);
    break;
  case 5: // Pool (all money into lump sum)
    party.poolPartyGold();
    setTreasureEventState(this);
    break;
  case 6: // Share (evenly distribute pooled money)
    party.AutoUpConvertPoolMoney();
    party.sharePartyGold();
    // rebuild item list text
    setTreasureEventState(this);
    break;
  case 7: // Appraise (determine value of gem/jewelry)    
    PushEvent(new APPRAISE_SELECT_DATA(&GetActiveChar(this).money,canApprGems,canApprJewels), DeleteEvent);
    break;
  case 8: // exit the shop
    if (party.moneyPooled)
    {      
      SetMyState(TASK_AskLeaveMoneyBehind);
      PushEvent(new ASK_YES_NO_MENU_DATA("LEAVE YOUR MONEY BEHIND?",TRUE, ShopEvent), DeleteEvent);
    }
    else
    {
      treasureItemListText.Clear();
      itemListText.Clear();
      currPic.Clear();
      if (WaitUntilReturnPressed()) ClearFormattedText(textData);
      party.setPartyAdventureState();
      if (ForcePartyBackup()) TaskMessage(TASKMSG_MovePartyBackward);
      ChainHappened();
    }
    break;
  }
}

void SHOP::OnDraw(void)
{
  UpdateShopScreen();
}

void SHOP::OnUpdateUI(void)
{
  if (CheckForMultiBoxText()) return;
  
  if (party.moneyPooled)
  {
    menu.setItemInactive(5); // pool
  }
  else 
  {
    menu.setItemInactive(4); // take
    menu.setItemInactive(6); // share
  }

  if (GetActiveChar(this).GetAdjStatus() != Okay)
    menu.setItemInactive(1); // buy
  if ((canApprGems==FALSE)&&(canApprJewels==FALSE))
    menu.setItemInactive(7); // appraise
}

void SHOP::OnReturnToTopOfQueue(void)
{
  if (GetMyState()==TASK_AskLeaveMoneyBehind)
  {
    if (party.tradeItem==0) // no, don't leave money behind
    {
      //party.sharePartyGold(); 
      ClearFormattedText(textData);
      setTreasureEventState(this);
      menu.setMenu(ShopMenuData);
      menu.setHorzOrient();
      currPic = GetEventPic();
      currPic.LoadPicSurfaces("sp_");
    }
    else
    {
      party.moneyPooled=FALSE;
      party.poolSack.Clear(); // forfeit pooled funds
     
      treasureItemListText.Clear();
      itemListText.Clear();
      currPic.Clear();
      if (WaitUntilReturnPressed()) ClearFormattedText(textData);
      party.setPartyAdventureState();
      if (ForcePartyBackup()) TaskMessage(TASKMSG_MovePartyBackward);
      ChainHappened();
    }
  }
  else
  {
    ClearFormattedText(textData);
    setTreasureEventState(this);
    menu.setMenu(ShopMenuData);
    menu.setHorzOrient();
    currPic = GetEventPic();
    currPic.LoadPicSurfaces("sp_");
  }
}

void SHOP::OnLeaveQueue(void)
{
  //if (party.moneyPooled)
  //  party.sharePartyGold();
}

////////////////////////////////////////////////////////////BUY_ITEMS_DATA

void BUY_ITEMS_DATA::OnInitialEvent(void)
{
  ClearFormattedText(textData);
  ASSERT( m_pOrigEvent->event == ShopEvent );

  POSITION pos = ((SHOP*)m_pOrigEvent)->itemsAvail.GetHeadPosition();
  while (pos != NULL)
    ((SHOP*)m_pOrigEvent)->itemsAvail.GetNext(pos).identified = TRUE; // shops disclose full name

  setShopItemState(((SHOP*)m_pOrigEvent)->itemsAvail, 
                   ((SHOP*)m_pOrigEvent)->costFactor);
  
  menu.setMenu(BuyMenuData);
  menu.setHorzOrient();
  menu.MapKeyCodeToMenuItem(KC_ESCAPE, 4);
  handleItemsFormInput(ITEMSFORM_exit, itemListText);
  handleItemsFormInput(ITEMSFORM_initial, itemListText);
}          

void BUY_ITEMS_DATA::OnKeypress(key_code key, char ascii)
{
  if (key != KC_RETURN)
  {
    HMenuVItemsKeyboardAction(key,ascii);
    return;
  };

  switch (menu.currentItem())
  {
  case 1: // Buy (add to char inventory, adjust char money)
    GetActiveChar(m_pOrigEvent).buyItem(itemListText.GetItemIndex(currShopTextIndex()),
                                        ((SHOP*)m_pOrigEvent)->costFactor);
    break;
  case 2: // Next page of items
    nextShopPage();
    break;
  case 3: // Prev page of items
    prevShopPage();
    break;
  case 4: // exit to shop
    clearShopItemState();
    handleItemsFormInput(ITEMSFORM_exit, itemListText);
    PopEvent();
    break;
  }
}

void BUY_ITEMS_DATA::OnDraw(void)
{  
  //UpdateBuyItemsScreen();
  GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);
  drawBorder();        
  if (errorTextData.numLines == 0)
    handleItemsFormInput(ITEMSFORM_refresh, itemListText);
  menu.DisplayMenu();
  DisplayTextAndFlip();
}

void BUY_ITEMS_DATA::OnUpdateUI(void)
{
  if (CheckForMultiBoxText()) return;
  CHARACTER& dude = GetActiveChar(m_pOrigEvent);
  if ((!dude.enoughMoney(1,0,0,globalData.moneyData.GetDefaultType())) || (itemListText.GetCount() <= 0))
    menu.setItemInactive(1); // buy

  if (!allowShopItemNextPage())
    menu.setItemInactive(2); // next

  if (!allowShopItemPrevPage())
    menu.setItemInactive(3); // prev
}                               

////////////////////////////////////////////////////////////CAMP_EVENT_DATA

void CAMP_EVENT_DATA::OnInitialEvent(void)
{  
  menu.reset();
  PushEvent(new ENCAMP_MENU_DATA(this), DeleteEvent);
}          


void CAMP_EVENT_DATA::OnReturnToTopOfQueue(void)
{
  if (ForcePartyBackup()) TaskMessage(TASKMSG_MovePartyBackward);
  ChainHappened();
}

////////////////////////////////////////////////////////////CHAIN_EVENT

// The CHAIN_EVENT performs an IF condition THEN CHAIN A, ELSE CHAIN B.
// CHAIN A happens only if the CHAIN_EVENT trigger conditions are
// satisfied. CHAIN B is allowed to happen only if CHAIN A does not.
// Even if CHAIN B is set to 'Chain Always', it will not trigger if
// CHAIN A does.
//
void CHAIN_EVENT::OnInitialEvent(void)
{  
  menu.reset();
  // event trigger conditions met
  if (IsGlobalEvent())
  {
    if (globalData.eventData.IsValidEvent(Chain))
    {
      //ClearFormattedText(textData);
      ReplaceGlobalEvent(Chain); // replace yourself with CHAIN A.
    }
    else
      PopEvent(); // if event is not valid, don't continue chaining
  }
  else
  {
    if (levelData.eventData.IsValidEvent(Chain))
    {
      //ClearFormattedText(textData);
      ReplaceEvent(Chain); // replace yourself with CHAIN A.
    }
    else
      PopEvent(); // if event is not valid, don't continue chaining
  }
}          

bool CHAIN_EVENT::OnTestTrigger(void)
{
  if (!GameEvent::OnTestTrigger()) return false;
  
  //if (!levelData.eventData.IsValidEvent(Chain)) return false;
  return true;
} 

////////////////////////////////////////////////////////////UTILITIES_EVENT_DATA
void UTILITIES_EVENT_DATA::OnInitialEvent(void)
{
  menu.reset();
}

bool UTILITIES_EVENT_DATA::OnIdle(void)
{
  long temp;

  if (mathItemIndex >= 0)
  {
    switch (operation)
    {
    case MO_NoOperation:
      break;
    case MO_StoredIn:
      switch (mathItemType)
      {
      case ITEM_FLAG:
        globalData.specialItemData.SetStage(mathItemIndex,mathAmount);
        break;
      case KEY_FLAG:
        globalData.keyData.SetStage(mathItemIndex,mathAmount);
        break;
      case QUEST_FLAG:
        globalData.questData.SetStage(mathItemIndex,mathAmount);
        break;
      }
      break;
    case MO_AddedTo:
      switch (mathItemType)
      {
      case ITEM_FLAG:
        temp = globalData.specialItemData.GetStage(mathItemIndex) + mathAmount;
        temp = max( 0, min(temp, 65535) );
        globalData.specialItemData.SetStage(mathItemIndex, temp);
        break;
      case KEY_FLAG:
        temp = globalData.keyData.GetStage(mathItemIndex) + mathAmount;
        temp = max( 0, min(temp, 65535) );
        globalData.keyData.SetStage(mathItemIndex, temp);
        break;
      case QUEST_FLAG: // cannot add to a quest and make it fail
        globalData.questData.IncStage(mathItemIndex, mathAmount);
        break;
      }
      break;
    case MO_SubtractFrom:
      switch (mathItemType)
      {
      case ITEM_FLAG:
        temp = globalData.specialItemData.GetStage(mathItemIndex) - mathAmount;
        temp = max( 0, min(temp, 65535) );
        globalData.specialItemData.SetStage(mathItemIndex, temp);
        break;
      case KEY_FLAG:
        temp = globalData.keyData.GetStage(mathItemIndex) - mathAmount;
        temp = max( 0, min(temp, 65535) );
        globalData.keyData.SetStage(mathItemIndex, temp);
        break;
      case QUEST_FLAG:
        temp = globalData.questData.GetStage(mathItemIndex) - mathAmount;
        temp = max( 0, min(temp, 65535) );
        globalData.questData.SetStage(mathItemIndex, temp);
        break;
      }
      break;
    }
  }

  BOOL activate = FALSE;

  switch (itemCheck)
  {
  case MIC_None:
    break;
  case MIC_AllItems:
    {
      BOOL missing = FALSE;
      int i=0;
      while ((!missing) && (i < items.list.GetSize()))
      {
        if (items.list[i].index >= 0)
        {
          switch (items.list[i].ItemType)
          {
          case ITEM_FLAG:
            if (!party.hasSpecialItem(items.list[i].index))
              missing = TRUE;
            break;
          case KEY_FLAG:
            if (!party.hasSpecialKey(items.list[i].index))
              missing = TRUE;
            break;
          case QUEST_FLAG:
            if (!globalData.questData.IsPresent(items.list[i].index))
              missing = TRUE;
            break;
          }
        }
        i++;
      }
      if (items.list.GetSize() > 0)
        activate = (missing == FALSE);
      else
        activate = FALSE;
    }
    break;

  case MIC_AtLeastOneItem:
    {
      BOOL found = FALSE;
      int i=0;
      while ((!found) && (i < items.list.GetSize()))
      {
        if (items.list[i].index >= 0)
        {
          switch (items.list[i].ItemType)
          {
          case ITEM_FLAG:
            if (party.hasSpecialItem(items.list[i].index))
              found = TRUE;
            break;
          case KEY_FLAG:
            if (party.hasSpecialKey(items.list[i].index))
              found = TRUE;
            break;
          case QUEST_FLAG:
            if (globalData.questData.IsPresent(items.list[i].index))
              found = TRUE;
            break;
          }
        }
        i++;
      }
      if (items.list.GetSize() > 0)
        activate = found;
      else
        activate = FALSE;
    }
    break;
  }

  if (activate)
  {
    if (resultItemIndex >= 0)
    {
      switch (resultItemType)
      {
      case ITEM_FLAG:
        temp = globalData.specialItemData.GetStage(resultItemIndex) + 1;
        temp = max( 0, min(temp, 65535) );
        globalData.specialItemData.SetStage(resultItemIndex,temp);
        break;
      case KEY_FLAG:
        temp = globalData.keyData.GetStage(resultItemIndex) + 1;
        temp = max( 0, min(temp, 65535) );
        globalData.keyData.SetStage(resultItemIndex,temp);
        break;
      case QUEST_FLAG:
        globalData.questData.SetStage(resultItemIndex,1);
        break;
      }
    }
  }

  if (endPlay)
    ReplaceEvent(new EXIT_DATA, DeleteEvent);
  else
    ChainHappened();

  return false;
}

////////////////////////////////////////////////////////////SOUND_EVENT

void SOUND_EVENT::OnInitialEvent(void)
{
  menu.reset();
}

bool SOUND_EVENT::OnIdle(void)
{

  if ((pSndMgr != NULL) && (PlayEventSounds))
  {
    pSndMgr->StopQueue();

    POSITION pos = sounds.GetHeadPosition();

    while (pos != NULL)
      pSndMgr->QueueSound(sounds.GetNext(pos));

    pSndMgr->PlayQueue();
  }

  ChainHappened();
  return false;
} 

////////////////////////////////////////////////////////////SPECIAL_ITEM_KEY_EVENT_DATA

void SPECIAL_ITEM_KEY_EVENT_DATA::OnInitialEvent(void)
{
  menu.setMenu(PressEnterMenuData);
  menu.setHorzOrient();
  FormatDisplayText(textData, GetEventText());  
  currPic = GetEventPic();
  currPic.LoadPicSurfaces("sp_");

  if (ShouldSimulateEnterKey()) 
  {
    menu.setItemInactive(1); // Press Enter to continue
    simulatedKeyQueue.PushKey(VK_RETURN);
  }
}

void SPECIAL_ITEM_KEY_EVENT_DATA::OnKeypress(key_code key, char ascii)
{
  if (TABParty(key)) return;
  if (key != KC_RETURN)
  {
    HMenuVPartyKeyboardAction(key,ascii);
    //StandardMenuKeyboardAction(key,ascii);
    return;
  }
  
  int i;
  for (i=0; i<items.list.GetSize(); i++)
  {
    if (items.list[i].operation == SPECIAL_OBJECT_GIVE)
    {
      switch (items.list[i].ItemType)
      {
      case ITEM_FLAG:
        if (!party.hasSpecialItem(items.list[i].index))
          party.addSpecialItem(items.list[i].index);
        break;
      case KEY_FLAG:
        if (!party.hasSpecialKey(items.list[i].index))
          party.addSpecialKey(items.list[i].index);
        break;
      }
    }
    else if (items.list[i].operation == SPECIAL_OBJECT_TAKE)
    {
      switch (items.list[i].ItemType)
      {
      case ITEM_FLAG:
        if (party.hasSpecialItem(items.list[i].index))
          party.removeSpecialItem(items.list[i].index);
        break;
      case KEY_FLAG:
        if (party.hasSpecialKey(items.list[i].index))
          party.removeSpecialKey(items.list[i].index);
        break;
      }
    }
  }

  currPic.Clear();
  if (WaitUntilReturnPressed()) ClearFormattedText(textData);
  if (ForcePartyBackup()) TaskMessage(TASKMSG_MovePartyBackward);
  ChainHappened();
}

void SPECIAL_ITEM_KEY_EVENT_DATA::OnDraw(void)
{
  UpdateSpecialItemKeyScreen();
}

bool SPECIAL_ITEM_KEY_EVENT_DATA::OnTestTrigger(void)
{
  if (!GameEvent::OnTestTrigger())
    return false;

  bool canHappen = false;

  int i;
  for (i=0; (i<items.list.GetSize()) && (!canHappen); i++)
  {
    if (items.list[i].operation == SPECIAL_OBJECT_GIVE)
    {
      switch (items.list[i].ItemType)
      {
      case ITEM_FLAG:          
        if (!party.hasSpecialItem(items.list[i].index))
          canHappen = true;
        break;
      case KEY_FLAG:
        if (!party.hasSpecialKey(items.list[i].index))
          canHappen = true;
        break;
      }
    }
    else if (items.list[i].operation == SPECIAL_OBJECT_TAKE)
    {
      switch (items.list[i].ItemType)
      {
      case ITEM_FLAG:          
        if (party.hasSpecialItem(items.list[i].index))
          canHappen = true;
        break;
      case KEY_FLAG:
        if (party.hasSpecialKey(items.list[i].index))
          canHappen = true;
        break;
      }
    }
  }

#ifdef _DEBUG
  if (!canHappen)
    WriteDebugString("SPECIAL_ITEM event not triggered\n");
  else
    WriteDebugString("SPECIAL_ITEM event triggered\n");
#endif

  return canHappen;
}

//////////////////////////////////////////////////////////////GPDL_EVENT
void GPDL_EVENT::OnInitialEvent(void)
{
  ClearFormattedText(textData);
  menu.setMenu(GPDLEventMenuData);
  menu.setHorzOrient();
  menu.setUseActive(false);
  currPic = GetEventPic();
  currPic.LoadPicSurfaces("sp_");

  GPDL_STATE dscState;
	dscState=gpdl.State();	
  CString fname;

  if (dscState==GPDL_UNINITIALIZED) 
  {
	  CFile f;
    // use full path in case working directory differs
    // from design directory
    fname = GetDesignPath();
    fname += "talk.bin";
    try // CFile/CArchive throw many exceptions
    {
		  if (f.Open(fname,CFile::modeRead)) 
      {
			  CArchive ar(&f,CArchive::load);
			  dscState=gpdl.Load(ar);
			  ar.Close();
			  f.Close();
		  }
      else
      {
        WriteDebugString("GPDL event failed to open %s\n", fname);
        PopEvent();
        return;
      }
    }
    catch (...)
    {
      WriteDebugString("GPDL event failed to open %s\n", fname);
      PopEvent();
      return;
    }
	}

  SetCharContext( GetActiveChar(this).GetContext() );
	if (dscState!=GPDL_UNINITIALIZED) 
  {
		if (dscState==GPDL_IDLE) 
    {     
      CString funcCall;
      funcCall = GetEventText().Right(GetEventText().GetLength()-10);
 			gpdl.BeginExecute(funcCall, this);
      if (   (gpdl.State() != GPDL_WAIT_ACK)
           &&(gpdl.State() != GPDL_WAIT_INPUT) )
      {
        ClearCharContext();
        PopEvent();        
        return;
      }
		}
	}
}

void GPDL_EVENT::OnKeypress(key_code key, char ascii)
{
  if (gpdl.State()==GPDL_WAIT_ACK) 
  {
    if (TABParty(key)) return;
    switch (key)
    {
    case KC_SPACE: 
	    gpdl.Char(' ');
      currPic = GetEventPic();
      currPic.LoadPicSurfaces("sp_");
      break;
    case KC_UP:     gpdl.Char(UP_ARROW);        break;
    case KC_DOWN:   gpdl.Char(DOWN_ARROW);      break;
    case KC_RETURN: gpdl.Char(CARRIAGE_RETURN); break;
    default:
      break;
    }
	} 
  else if (gpdl.State()==GPDL_WAIT_INPUT) 
  {
    switch (key)
    {
    case KC_RETURN:
		  gpdl.Char(CARRIAGE_RETURN);
      currPic = GetEventPic();
      currPic.LoadPicSurfaces("sp_");
      break;
    case KC_SPACE: gpdl.Char(' ');        break;
    case KC_UP:    gpdl.Char(UP_ARROW);   break;
    case KC_BACK:  gpdl.Char(BACKSPACE);  break;
    case KC_DOWN:  gpdl.Char(DOWN_ARROW); break;
    default:       gpdl.Char(ascii);      break; // includes letters, numbers, punctuation
    }
	}

	if (gpdl.State()==GPDL_IDLE) 
  {
    if (TABParty(key)) return;
    if (ForcePartyBackup()) TaskMessage(TASKMSG_MovePartyBackward);
    ClearCharContext();
	  PopEvent();    
  } 
}

void GPDL_EVENT::OnDraw(void)
{
  UpdateGPDLScreen();
}

void GPDL_EVENT::OnUpdateUI(void)
{
  /*
   { "UP/Down scrolls text", -1 },
   { "SPACE to enter text", -1 },
   { "RETURN to quit", -1 },
   { "Enter Text followed by RETURN", -1 },
   { "(just RETURN to quit)", -1 }
  */
  if (CheckForMultiBoxText()) return;
  if (gpdl.State()==GPDL_WAIT_ACK) 
  {
	  menu.setItemInactive(4); // Text followed by RETURN
    menu.setItemInactive(5); // RETURN part 2
	} 
  else if (gpdl.State()==GPDL_WAIT_INPUT) 
  {
	  menu.setItemInactive(1); // Up/Down Arrow
	  menu.setItemInactive(2); // SPACE
    menu.setItemInactive(3); // RETURN
  }
}

////////////////////////////////////////////////////////////TAKE_PARTY_ITEMS_DATA

void TAKE_PARTY_ITEMS_DATA::OnInitialEvent(void)
{
  menu.setMenu(TextEventData);
  menu.setHorzOrient();
  FormatDisplayText(textData, GetEventText());
  currPic = GetEventPic();
  currPic.LoadPicSurfaces("sp_");  
}

void TAKE_PARTY_ITEMS_DATA::OnKeypress(key_code key, char ascii)
{
  if (TABParty(key)) return;
  if (key != KC_RETURN) 
  {
    HMenuVPartyKeyboardAction(key,ascii);
    //StandardMenuKeyboardAction(key, ascii);
    return;
  };

  party.TakePartyItems(*this);
  if (ForcePartyBackup()) TaskMessage(TASKMSG_MovePartyBackward);
  currPic.Clear();
  // let text continue being displayed if return not needed
  if (WaitUntilReturnPressed()) ClearFormattedText(textData);
  ChainHappened();
}

void TAKE_PARTY_ITEMS_DATA::OnDraw(void)
{
  UpdateTakePartyItemsScreen();
}

void TAKE_PARTY_ITEMS_DATA::OnUpdateUI(void)
{
  //{ "EXIT", 1 },
  //{ "PRESS ENTER TO CONTINUE", 7 }

  if (CheckForMultiBoxText()) return;

  menu.setItemInactive(1); // Exit

  if (ShouldSimulateEnterKey()) 
  {
    menu.setItemInactive(2); // Press Enter to continue
    simulatedKeyQueue.PushKey(VK_RETURN);
  }
}

////////////////////////////////////////////////////////////TRAININGHALL

void TRAININGHALL::OnInitialEvent(void)
{
  menu.setMenu(TrainWelcomeMenuData);
  menu.setHorzOrient();
  FormatDisplayText(textData, GetEventText());
  currPic = GetEventPic();
  currPic.LoadPicSurfaces("sp_");
}

void TRAININGHALL::OnKeypress(key_code key, char ascii)
{
  if (TABParty(key)) return;
  if (key != KC_RETURN)
  {
    HMenuVPartyKeyboardAction(key,ascii);
    //StandardMenuKeyboardAction(key,ascii);
    return;
  }

  currPic.Clear();
  if (WaitUntilReturnPressed()) ClearFormattedText(textData);

  switch (menu.currentItem()) 
  {
  case 1: // Yes
    PushEvent(new MAIN_MENU_DATA(this), DeleteEvent);
    break;
  case 2: // No
    if (ForcePartyBackup()) TaskMessage(TASKMSG_MovePartyBackward);
    ChainHappened();
    break;
  }
}

void TRAININGHALL::OnReturnToTopOfQueue()
{
  if (ForcePartyBackup()) TaskMessage(TASKMSG_MovePartyBackward);
  ChainHappened();
}

void TRAININGHALL::OnDraw(void)
{
  UpdateTrainHallWelcomeScreen();
}

int TRAININGHALL::OnSaveGame(unsigned int *saveArea)
{
  return 0; // Nothing needs to be saved.
}

int TRAININGHALL::OnLoadGame(unsigned int *saveArea)
{
  return 0;
}

////////////////////////////////////////////////////////WHO_TRIES_EVENT_DATA
void WHO_TRIES_EVENT_DATA::OnInitialEvent(void)
{
  switch (GetMyState())
  {
  case TASK_WhoTriesFailure:
    menu.setMenu(PressEnterMenuData);
    menu.setHorzOrient();
    menu.enableTitle(FALSE);
    FormatDisplayText(textData, GetEventText3());
    if (GetEventText3().GetLength() <= 0)
      simulatedKeyQueue.PushKey(VK_RETURN);
    break;
  
  case TASK_WhoTriesSuccess:
    menu.setMenu(PressEnterMenuData);
    menu.setHorzOrient();
    menu.enableTitle(FALSE);
    FormatDisplayText(textData, GetEventText2());
    if (GetEventText2().GetLength() <= 0)
      simulatedKeyQueue.PushKey(VK_RETURN);
    break;


  case TASK_uninitialized:
  case TASK_initialized:
    currTry=0;
    // intentional fall-through

  default: // 
    menu.setMenu(PressEnterMenuData);
    menu.setHorzOrient();
    menu.enableTitle(FALSE);
    FormatDisplayText(textData, GetEventText());
    currPic = GetEventPic();
    currPic.LoadPicSurfaces("sp_");
    SetMyState(TASK_WhoTriesGet);    
    break;
  }
}

void WHO_TRIES_EVENT_DATA::OnKeypress(key_code key, char ascii)
{
  if (key != KC_RETURN) 
  {
    HMenuVPartyKeyboardAction(key, ascii);
    return;
  }

  bool failed=false;  
  switch (GetMyState())
  {
  case TASK_WhoTriesGet:
    if (alwaysFails)
    {
      failed = true;
      currTry = NbrTries;
    }
    else if (alwaysSucceeds)
    {
      failed = false;
    }
    else
    {
      // determine success/fail based on skills/attributes
      CHARACTER &dude = GetActiveChar(this);
      int MinSuccessVal;
      if (compareToDie)
        MinSuccessVal = rollDice(compareDie,1,0);
      else
        MinSuccessVal = max(0,compareDie);

      if (checkSTR)
      {
        if (dude.GetAdjStr() < MinSuccessVal)
          failed = true;
        else if (dude.GetAdjStrMod() < strBonus)
          failed = true;
      }

      if ((checkINT) && (dude.GetAdjInt() < MinSuccessVal))
        failed = true;

      if ((checkWIS) && (dude.GetAdjWis() < MinSuccessVal))
        failed = true;

      if ((checkDEX) && (dude.GetAdjDex() < MinSuccessVal))
        failed = true;

      if ((checkCON) && (dude.GetAdjCon() < MinSuccessVal))
        failed = true;

      if ((checkCHA) && (dude.GetAdjCha() < MinSuccessVal))
        failed = true;

      if ((checkPP) && (dude.GetAdjPickPockets() < MinSuccessVal))
        failed = true;

      if ((checkOL) && (dude.GetAdjOpenLocks() < MinSuccessVal))
        failed = true;

      if ((checkFT) && (dude.GetAdjFindTraps() < MinSuccessVal))
        failed = true;

      if ((checkMS) && (dude.GetAdjMoveSilent() < MinSuccessVal))
        failed = true;

      if ((checkHS) && (dude.GetAdjHideInShadows() < MinSuccessVal))
        failed = true;

      if ((checkHN) && (dude.GetAdjHearNoise() < MinSuccessVal))
        failed = true;

      if ((checkCW) && (dude.GetAdjClimbWalls() < MinSuccessVal))
        failed = true;

      if ((checkRL) && (dude.GetAdjReadLanguages() < MinSuccessVal))
        failed = true;
    }

    currTry++;

    if (failed)
    {
      // only need retries when comparing to rollDice value
      if ((currTry >= NbrTries) || (!compareToDie))
        SetMyState(TASK_WhoTriesFailure);
      OnInitialEvent();
      return;
    }
    else
    {
      SetMyState(TASK_WhoTriesSuccess);
      OnInitialEvent();
      return;
    }
    break;

  case TASK_WhoTriesFailure:
    currPic.Clear();
    ClearFormattedText(textData);

    switch (failAction)
    {
    case NoAction:
      ChainHappened();
      break;
    case ChainEvent:
      ChainOrQuit(failChain);
      break;    
    case Teleport: 
      currPic.Clear();
      HandleTransfer(failTransferData,"", currPic);
      break;      
    case BackupOneStep:
      TaskMessage(TASKMSG_MovePartyBackward);
      ChainHappened();
      break;
    }
    break;

  case TASK_WhoTriesSuccess:
    currPic.Clear();
    ClearFormattedText(textData);
    switch (successAction)
    {
    case NoAction:
      ChainHappened();
      break;
    case ChainEvent:
      ChainOrQuit(successChain);
      break;
    case Teleport: 
      currPic.Clear();
      HandleTransfer(successTransferData,"", currPic);    
      break;
    case BackupOneStep:
      TaskMessage(TASKMSG_MovePartyBackward);
      ChainHappened();
      break;
    }
    break;

    default:
      break;
  }
}


void WHO_TRIES_EVENT_DATA::OnDraw(void)
{
  UpdateWhoTriesScreen();
}

////////////////////////////////////////////////////////////////////////TEMPLE
void TEMPLE::OnInitialEvent(void)
{
  switch (GetMyState())
  {
  case TASK_TempleMenu:
    WriteDebugString("OnInitialEvent: TempleMenu\n");
    menu.setMenu(TempleMenuData);
    menu.setHorzOrient();
    menu.MapKeyCodeToMenuItem(KC_ESCAPE, 7);
    FormatDisplayText(textData, GetEventText2());
    handleSpellFormInput(SPELLFORM_exit, spellListText);
    break;

  case TASK_TempleDonate:
    WriteDebugString("OnInitialEvent: TempleDonate\n");
    menu.setMenu(TempleDonateMenuData);
    menu.setHorzOrient();
    menu.MapKeyCodeToMenuItem(KC_ESCAPE, 6);
    FormatDisplayText(textData, GetEventText2());
    handleSpellFormInput(SPELLFORM_exit, spellListText);
    break;

  case TASK_TempleHeal:
    WriteDebugString("OnInitialEvent: TempleHeal\n");
    menu.setMenu(TempleHealMenuData);
    menu.setHorzOrient();
    menu.MapKeyCodeToMenuItem(KC_ESCAPE, 8);
    FormatDisplayText(textData, GetEventText2());
    handleSpellFormInput(SPELLFORM_exit, spellListText);
    break;

  case TASK_TempleGive: // enter money qty for donations
    WriteDebugString("OnInitialEvent: TempleGive\n");
    menu.reset();
    menu.setStartCoord(USE_DEFAULT_HORZ_MENU, 0);
    menu.setTitle(TempleGiveMenuTitle);
    menu.setHorzOrient();
    menu.setItemSeparation(0);
    menu.addMenuItem(" ");
    menu.setCurrentItem(menu.totalItems());
    ClearFormattedText(textData);
    handleSpellFormInput(SPELLFORM_exit, spellListText);
    break;

  case TASK_TempleCast:
    WriteDebugString("OnInitialEvent: TempleCast\n");
    {
      if (!globalData.charData.HaveKey(tcKey))
      {
        // create max level Cleric/MagicUser so any
        // spell can be cast.
        //
        DWORD StartExp = 1+GetMinClericExpForLevel(29);
        StartExp += 1+GetMinMagicUserExpForLevel(29);

        CHARACTER TempleCleric;
        TempleCleric.SetAlignment(ChaoticNeutral);
        TempleCleric.race(Human);
        TempleCleric.SetGender(Male);
        TempleCleric.SetClass(C_MU); // 
        TempleCleric.SetType(NPC_TYPE);
        TempleCleric.SetName("TempleClericMU");
        TempleCleric.generateNewCharacter(StartExp,0);  
        TempleCleric.SetStr(18);
        TempleCleric.SetInt(18);
        TempleCleric.SetWis(18);
        TempleCleric.SetDex(18);
        TempleCleric.SetCon(18);
        TempleCleric.SetCha(18);
        TempleCleric.spells = templeSpells;

        tcKey = globalData.charData.AddCharacter(TempleCleric);
      }

      CHARACTER *pTempleCleric = globalData.charData.GetCharacterData(tcKey);

      POSITION pos = pTempleCleric->spells.list.GetHeadPosition();
      while (pos!=NULL)
      {
        pTempleCleric->spells.list.GetAt(pos).memorized=255;
        pTempleCleric->spells.list.GetAt(pos).selected=255;
        pTempleCleric->spells.list.GetNext(pos);
      }    

      menu.setMenu(TempleCastMenuData);
      menu.setHorzOrient();
      menu.MapKeyCodeToMenuItem(KC_ESCAPE, 4);
      ClearFormattedText(textData);
      setSpellTempleCastItemState(*pTempleCleric, costFactor);
      handleSpellFormInput(SPELLFORM_exit, spellListText);
      handleSpellFormInput(SPELLFORM_initial, spellListText);
    }
    break;

  default:
    WriteDebugString("OnInitialEvent: default\n");
    SetMyState(TASK_TempleWelcome);
    menu.setMenu(PressEnterMenuData);
    menu.setHorzOrient();
    menu.MapKeyCodeToMenuItem(KC_ESCAPE, 1);
    FormatDisplayText(textData, GetEventText());
    currPic = GetEventPic();
    currPic.LoadPicSurfaces("sp_");
    if (ShouldSimulateEnterKey())    
      simulatedKeyQueue.PushKey(VK_RETURN);
    handleSpellFormInput(SPELLFORM_exit, spellListText);
    break;    
  }
}

void TEMPLE::OnDraw(void)
{
  switch (GetMyState())
  {
  case TASK_TempleCast:
    {
      //UpdateTempleCastScreen();
      GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);
      drawBorder();
      // draw middle horz bar, but 
      // not viewport and middle ver bar
      drawViewport(FALSE, FALSE);
      if (errorTextData.numLines == 0)
        handleSpellFormInput(SPELLFORM_refresh, spellListText);      
      menu.DisplayMenu();
      if (errorTextData.numLines == 0) 
        displayCharacterHP(GetActiveChar(this));
      DisplayTextAndFlip();
    }
    break;
  case TASK_TempleGive:
    UpdateTempleGiveScreen();
    break;
  default:
    UpdateTempleScreen();
    break;
  }  
}

void TEMPLE::OnUpdateUI(void)
{
  if (CheckForMultiBoxText()) return;
  switch (GetMyState())
  {
  case TASK_TempleMenu:    
    if (!allowDonations)
      menu.setItemInactive(2);

    if (party.moneyPooled)
      menu.setItemInactive(5); // pool
    else
      menu.setItemInactive(6); // share
  
    if (treasureItemListText.GetCount() <= 0)
      menu.setItemInactive(4); // take
    break;

  case TASK_TempleDonate:
    if (party.moneyPooled)
      menu.setItemInactive(2); // pool
    else
      menu.setItemInactive(3); // share
  
    if (treasureItemListText.GetCount() <= 0)
      menu.setItemInactive(1); // take
    int qty;
    if (party.moneyPooled)
      qty = !party.poolSack.IsEmpty(); //GetPoolGoldValue();
    else
      qty = !GetActiveChar(this).money.IsEmpty(); //poolCharacterGold();
    if (qty <= 0)
      menu.setItemInactive(5); // give
    break;

  case TASK_TempleHeal:
    if (costFactor != Free)
      menu.setItemInactive(3); // fix

    if (party.moneyPooled)
      menu.setItemInactive(5); // pool
    else
      menu.setItemInactive(6); // share
  
    if (treasureItemListText.GetCount() <= 0)
      menu.setItemInactive(4); // take
    break;

  case TASK_TempleCast:
    if (!levelData.zoneData.zones[levelData.area[party.Posy][party.Posx].zone].AllowMagic)
      menu.setItemInactive(1); // cast

    if (spellListText.GetCount() <= 0)
      menu.setItemInactive(1); // cast
      
    if (!allowSpellNextPage())
      menu.setItemInactive(2); // next

    if (!allowSpellPrevPage())
      menu.setItemInactive(3); // prev
    break;
  default:
    break;
  }
}

void TEMPLE::OnKeypress(key_code key, char ascii)
{
  switch (GetMyState())
  {
  case TASK_TempleWelcome:
    WriteDebugString("OnKeypress: TASK_TempleWelcome\n");

    if (TABParty(key)) return;
    if (key != KC_RETURN)
      return;
    ClearFormattedText(textData);
    SetMyState(TASK_TempleMenu);
    OnInitialEvent();
    break;

  case TASK_TempleMenu:
    WriteDebugString("OnKeypress: TASK_TempleMenu\n");
    if (TABParty(key)) return;
    if (key != KC_RETURN)
    {
      HMenuVPartyKeyboardAction(key, ascii);
      return;  
    }
 
    switch (menu.currentItem()) 
    {
    case 1: // Heal
      SetMyState(TASK_TempleHeal);
      OnInitialEvent();
      break;
    case 2: // Donate
      SetMyState(TASK_TempleDonate);
      OnInitialEvent();
      break;  
    case 3: // View
      PushEvent(new VIEW_CHARACTER_DATA(GetEventType(),this), DeleteEvent);
      break;
    case 4: // Take
      setTreasureEventState(this);
      PushEvent(new TAKE_TREASURE_DATA(this), DeleteEvent);
      break;
    case 5: // Pool
      party.poolPartyGold();
      setTreasureEventState(this);
      break;
    case 6: // Share
      party.AutoUpConvertPoolMoney();
      party.sharePartyGold();
      setTreasureEventState(this);
      break;
    case 7: // Exit
      if (party.moneyPooled)
      {      
        SetMyState(TASK_AskLeaveMoneyBehind);
        PushEvent(new ASK_YES_NO_MENU_DATA("LEAVE YOUR MONEY BEHIND?",TRUE, TempleEvent), DeleteEvent);
      }
      else
      {
        if (ForcePartyBackup()) TaskMessage(TASKMSG_MovePartyBackward);
        currPic.Clear();
        if (WaitUntilReturnPressed()) ClearFormattedText(textData);
        handleSpellFormInput(SPELLFORM_exit, spellListText);

        if (totalDonation >= donationTrigger)
        {
          totalDonation = 0;
          // set up for donation trigger
          if (levelData.eventData.IsValidEvent(donationChain))
            ReplaceEvent(donationChain);
          else
            ChainHappened();
        }
        else
          ChainHappened();
      }
      break;
    }
    break;

  case TASK_TempleCast:
    WriteDebugString("OnKeypress: TASK_TempleCast\n");
    if (key != KC_RETURN)
    {
      HMenuVItemsKeyboardAction(key, ascii);
      return;
    }
  
    switch (menu.currentItem()) 
    {
    case 1: // cast
      {      
        SPELL_DATA *pSpData = spellData.GetSpellData(spellListText.GetSpellDatabaseKey(currSpellTextIndex()));
        if (pSpData!=NULL)
        {
          CHARACTER *pTempleCleric = globalData.charData.GetCharacterData(tcKey);
          pTempleCleric->InitTargeting(TouchedTargets, //pSpData->Targeting, 
                                       TRUE, //pSpData->Friendly_Fire_Ok, 
                                       TRUE, //pTempleCleric->friendly,
                                       1, //targs, 
                                       1); //range);

          pTempleCleric->AddTarget(GetActiveChar(this));
          pTempleCleric->CastSpell(spellListText.GetSpellBookKey(currSpellTextIndex()));
        }
      }
      break;
    case 2: // next
      nextSpellPage();
      break;
    case 3: // prev
      prevSpellPage();
      break;
    case 4: // Exit
      SetMyState(TASK_TempleMenu);
      OnInitialEvent();
      break;
    }
    break;

  case TASK_TempleDonate:
    WriteDebugString("OnKeypress: TASK_TempleDonate\n");
    if (TABParty(key)) return;
    if (key != KC_RETURN)
    {
      HMenuVPartyKeyboardAction(key, ascii);
      return;  
    }
    
    switch (menu.currentItem()) 
    {
    case 1: // take
      setTreasureEventState(this);
      PushEvent(new TAKE_TREASURE_DATA(this), DeleteEvent);
      break;
    case 2: // pool
      party.poolPartyGold();
      setTreasureEventState(this);
      break;
    case 3: // share
      party.AutoUpConvertPoolMoney();
      party.sharePartyGold();
      setTreasureEventState(this);
      break;
    case 4: // appraise
      PushEvent(new APPRAISE_SELECT_DATA(&GetActiveChar(this).money), DeleteEvent);
      break;
    case 5: // give
      SetMyState(TASK_TempleGive);
      OnInitialEvent();
      break;
    case 6: // Exit
      SetMyState(TASK_TempleMenu);
      OnInitialEvent();
      break;
    }
    break;

  case TASK_TempleGive:
    WriteDebugString("OnKeypress: TASK_TempleGive\n");
    if (TABParty(key)) return;
    if (   (key != KC_RETURN)
        && (key != KC_NUM)
        && (key != KC_BACK))
      return;
  
    char temp[10], temp2[10];
    int i, sum;   
   
    if (key == KC_RETURN)
    {
      temp[0] = '\0';
    
      if (menu.totalItems() > 0)
      {
        for (i=1;i<menu.totalItems();i++)
          strcat(temp, menu.getMenuItem(i));
      
        sum = atoi(temp);

        if (sum > 0)
        {
          GetActiveChar(this).payForItem(sum,globalData.moneyData.GetDefaultType());
          totalDonation += sum;
        }
      }

      SetMyState(TASK_TempleMenu);
      OnInitialEvent();
    }
    else 
    {
      int qty;
      if (party.moneyPooled)
        qty = party.GetPoolGoldValue();
      else
        qty = GetActiveChar(this).poolCharacterGold();

      if (key == KC_NUM)          
      {
        temp[0] = '\0';
      
        // ignore last char, it is a blank
        for (i=1;i<menu.totalItems();i++)
          strcat(temp, menu.getMenuItem(i));
      
        sprintf(temp2, "%c", ascii);
        strcat(temp, temp2);
        sum = atoi(temp);
      
        if ((sum > qty) || (sum < 0))
        {
          i=menu.totalItems();
        
          while (i > 0)
          {
            menu.deleteLastMenuItem();
            i--;
          }
        
          itoa(qty, temp, 10);
        
          for (i=0;i<(int)strlen(temp);i++)
          {
            strncpy(temp2, &temp[i], 1);
            temp2[1] = '\0';
            menu.addMenuItem(temp2);
          }
        
          menu.addMenuItem(" ");
          menu.setCurrentItem(menu.totalItems());
        }
        else 
        {
          if (menu.totalItems() == 0)
          {
            menu.addMenuItem(temp2);
            menu.addMenuItem(" ");
          }
          else 
          {
            menu.changeMenuItem(menu.totalItems(), temp2);
            menu.addMenuItem(" ");
          }
          menu.setCurrentItem(menu.totalItems());
        }
      }
      else if (   ((key == KC_BACK) || (key == KC_LEFT))
               && (menu.totalItems() > 0))
      {
        menu.deleteLastMenuItem();
        menu.changeMenuItem(menu.totalItems(), " ");
      
        if (menu.totalItems() == 0)
          menu.addMenuItem(" ");
      
        menu.setCurrentItem(menu.totalItems());
      }
    }
    break;

  case TASK_TempleHeal:
    WriteDebugString("OnKeypress: TASK_TempleHeal\n");
    if (TABParty(key)) return;
    if (key != KC_RETURN)
    {
      HMenuVPartyKeyboardAction(key, ascii);
      return;  
    }
  
    switch (menu.currentItem()) 
    {
    case 1: // Heal
      SetMyState(TASK_TempleCast);
      OnInitialEvent();
      break;
    case 2: // View
      PushEvent(new VIEW_CHARACTER_DATA(GetEventType(),this), DeleteEvent);
      break;
    case 3: // Fix (using party spells)
      party.FixParty(TRUE);
      break;
    case 4: // Take
      setTreasureEventState(this);
      PushEvent(new TAKE_TREASURE_DATA(this), DeleteEvent);
      break;
    case 5: // Pool
      party.poolPartyGold();
      setTreasureEventState(this);
      break;
    case 6: // share
      party.AutoUpConvertPoolMoney();
      party.sharePartyGold();
      setTreasureEventState(this);
      break;
    case 7: // appraise
      PushEvent(new APPRAISE_SELECT_DATA(&GetActiveChar(this).money), DeleteEvent);
      break;
    case 8: // Exit
      SetMyState(TASK_TempleMenu);
      OnInitialEvent();
      break;
    }
    break;
    default:
      break;
  }
}

void TEMPLE::OnReturnToTopOfQueue()
{
  WriteDebugString("OnReturnToTopOfQueue\n");
  if (GetMyState()==TASK_AskLeaveMoneyBehind)
  {
    if (party.tradeItem==0) // no, don't leave money behind
    {
      party.AutoUpConvertPoolMoney();
      party.sharePartyGold();
      currPic = GetEventPic();
      currPic.LoadPicSurfaces("sp_");
      OnInitialEvent();
    }
    else
    {
      party.moneyPooled=FALSE;
      party.poolSack.Clear(); // forfeit pooled funds

      if (ForcePartyBackup()) TaskMessage(TASKMSG_MovePartyBackward);
      currPic.Clear();
      if (WaitUntilReturnPressed()) ClearFormattedText(textData);
      if (totalDonation >= donationTrigger)
      {
        totalDonation = 0;
        // set up for donation trigger
        if (levelData.eventData.IsValidEvent(donationChain))
          ReplaceEvent(donationChain);
        else
          ChainHappened();
      }
      else
        ChainHappened();
    }
  }
  else
  {
    currPic = GetEventPic();
    currPic.LoadPicSurfaces("sp_");
    OnInitialEvent();
  }
}

void TEMPLE::OnLeaveQueue(void)
{
  WriteDebugString("OnLeaveQueue\n");
  globalData.charData.RemoveCharacter(tcKey);
  //if (party.moneyPooled)
  //  party.sharePartyGold();
}

////////////////////////////////////////////////////////////////////////PASSWORD_DATA
void PASSWORD_DATA::OnInitialEvent(void)
{
  ClearFormattedText(textData);
  switch (GetMyState())
  {
  case TASK_PasswordFailure:
    menu.setMenu(PressEnterMenuData);
    menu.setHorzOrient();
    menu.enableTitle(FALSE);
    FormatDisplayText(textData, GetEventText3());
    if (GetEventText3().GetLength() <= 0)
      simulatedKeyQueue.PushKey(VK_RETURN);
    break;
  
  case TASK_PasswordSuccess:
    menu.setMenu(PressEnterMenuData);
    menu.setHorzOrient();
    menu.enableTitle(FALSE);
    FormatDisplayText(textData, GetEventText2());
    if (GetEventText2().GetLength() <= 0)
      simulatedKeyQueue.PushKey(VK_RETURN);
    break;

  case TASK_uninitialized:
  case TASK_initialized:
    currTry=0;
    FormatDisplayText(textData, GetEventText());
    // intentional fall-through

  default:
    menu.reset();
    menu.setStartCoord(USE_DEFAULT_HORZ_MENU, 0);
    menu.setTitle(GetPasswordTitle);
    menu.setHorzOrient();
    menu.setItemSeparation(0);
    menu.addMenuItem(" ");
    menu.setCurrentItem(menu.totalItems());
    currPic = GetEventPic();
    currPic.LoadPicSurfaces("sp_");
    SetMyState(TASK_PasswordGet);    
    break;
  }
}

void PASSWORD_DATA::OnKeypress(key_code key, char ascii)
{
  if (TABParty(key)) return;
  switch (GetMyState())
  {
  case TASK_PasswordFailure:
    if (key != KC_RETURN) 
    {
      HMenuVPartyKeyboardAction(key,ascii);
      //StandardMenuKeyboardAction(key, ascii);
      return;
    }
    currPic.Clear();
    ClearFormattedText(textData);

    switch (failAction)
    {
    case NoAction:
      ChainHappened();
      break;
    case ChainEvent:
      ChainOrQuit(failChain);
      break;    
    case Teleport: 
      currPic.Clear();
      HandleTransfer(failTransferData,"", currPic);
      break;      
    case BackupOneStep:
      TaskMessage(TASKMSG_MovePartyBackward);
      ChainHappened();
      break;
    }
    break;
  
  case TASK_PasswordSuccess:
    if (key != KC_RETURN) 
    {
      HMenuVPartyKeyboardAction(key,ascii);
      //StandardMenuKeyboardAction(key, ascii);
      return;
    }
    currPic.Clear();
    ClearFormattedText(textData);

    switch (successAction)
    {
    case NoAction:
      ChainHappened();
      break;
    case ChainEvent:
      ChainOrQuit(successChain);
      break;    
    case Teleport: 
      currPic.Clear();
      HandleTransfer(successTransferData,"", currPic);
      break;      
    case BackupOneStep:
      TaskMessage(TASKMSG_MovePartyBackward);
      ChainHappened();
      break;
    }
    break;

  case TASK_PasswordGet:
    {
    static BOOL showingFailText = FALSE;
    char temp[6];
    int i;
  
    if (showingFailText)
    {
      showingFailText = FALSE;
      ClearFormattedText(textData);
    }
  
    if (key == KC_RETURN)
    {
      CString pswd="";
      
      // ignore blank on the end
      for (i=1;i<menu.totalItems();i++)
        pswd += menu.getMenuItem(i);
    
      if (PasswordMatches(pswd))
      {
        SetMyState(TASK_PasswordSuccess);
        OnInitialEvent();
        return;
      }
      else
      {
        currTry++;
      
        if (currTry >= nbrTries)
        {
          SetMyState(TASK_PasswordFailure);
          OnInitialEvent();
          return;
        }
        else
        {
          // reset for next attempt
          OnInitialEvent();
          FormatDisplayText(textData, "That is not the correct answer");
          showingFailText = TRUE;
          return;
        }
      }
    }
    else if (   ((key == KC_CHAR) || (key == KC_NUM) || (key == KC_SPACE))
             && (menu.totalItems() < MAX_PSWD_TEXT))
    {
      sprintf(temp, "%c", ascii);
      menu.changeMenuItem(menu.totalItems(), temp);
      menu.addMenuItem(" ");
      menu.setCurrentItem(menu.totalItems());
      return;
    }
    else if (   ((key == KC_BACK) || (key == KC_LEFT))
             && (menu.totalItems() > 0))
    {
      menu.deleteLastMenuItem();
      menu.changeMenuItem(menu.totalItems(), " ");
      menu.setItemActive(menu.totalItems());      
      if (menu.totalItems() == 0)
        menu.addMenuItem(" ");      
      menu.setCurrentItem(menu.totalItems());
      return;
    }
    }
    break;
  default:
    break;
  }
}

void PASSWORD_DATA::OnDraw(void)
{
  UpdatePasswordScreen();
}

////////////////////////////////////////////////////////////////////////QUESTION_LIST_DATA

void QUESTION_LIST_DATA::OnInitialEvent(void)
{
  UserResult=NoUserResult;
  menu.reset();
  menu.setStartCoord(USE_DEFAULT_TEXTBOX_MENU, 0);
  menu.setVertOrient();
  menu.setTitle(buttons.title);
  menu.setItemSeparation(2);
  FormatDisplayText(textData, GetEventText());
  currPic = GetEventPic();
  currPic.LoadPicSurfaces("sp_");
  int count=0;
  int i;
  
  /*
  BOOL AllUnique=TRUE;
  for (i=0;i<MAX_BUTTONS;i++)
  {
    if (!buttons.buttons[i].label.IsEmpty())
    {
      for (int y=i+1;y<MAX_BUTTONS;y++)
      {
        if (!buttons.buttons[y].label.IsEmpty())
        {
          if (buttons.buttons[i].label[0] == buttons.buttons[y].label[0])
            AllUnique = FALSE;
        }
      }
    }
  }
  */

  for (i=0;i<MAX_BUTTONS;i++)
  {
    if (buttons.buttons[i].label != "")
    {
      //if (AllUnique)
      //  menu.addMenuItem(buttons.buttons[i].label,0);
      //else
        menu.addMenuItem(buttons.buttons[i].label);      
      count++;
    }
    else
    {
      int newItem = menu.addMenuItem(" "); // placeholder
      menu.setItemInactive(newItem);
    }
  }
  
  menu.SetFirstLettersShortcuts();
  menu.setCurrentItem(0);
  sx=party.Posx;sy=party.Posy;
  if (count == 0) ChainHappened();
}

void QUESTION_LIST_DATA::OnKeypress(key_code key, char ascii)
{
  if (TABParty(key)) return;
  if (key != KC_RETURN)
  {
    StandardMenuKeyboardAction(key, ascii);
    return;
  }
  currPic.Clear();
  if (WaitUntilReturnPressed()) ClearFormattedText(textData);
  UserResult = menu.currentItem();  
  QLIST_OPTION &button = buttons.buttons[UserResult-1];
  if (IsGlobalEvent())
  {
    if (!globalData.eventData.IsValidEvent(button.chain))
      PushEvent(new DO_NOTHING_EVENT, DeleteEvent);
    else PushGlobalEvent(button.chain);
  }
  else
  {
    if (!levelData.eventData.IsValidEvent(button.chain))
      PushEvent(new DO_NOTHING_EVENT, DeleteEvent);
    else PushEvent(button.chain);
  }
}

void QUESTION_LIST_DATA::OnDraw(void)
{
  UpdateQuestionListScreen();
}

void QUESTION_LIST_DATA::OnUpdateUI(void)
{
  if (CheckForMultiBoxText()) return;
  for (int i=0;i<MAX_BUTTONS;i++)
  {
    if (!buttons.buttons[i].present)
      menu.setItemInactive(i+1);
  }
}

void QUESTION_LIST_DATA::OnReturnToTopOfQueue(void)
{
  currPic.Clear();
  ClearFormattedText(textData);
  if (UserResult == NoUserResult)
  {
    ChainHappened();
    return;
  }
  QLIST_OPTION &button = buttons.buttons[UserResult-1];
  switch (button.postChainAction)
  {
  case LabelDoNothing:
    ChainHappened();
    break;
  case LabelReturnToQuestion:
    // only if still in starting location
    if ((sx==party.Posx)&&(sy==party.Posy))
      OnInitialEvent();
    else
      ChainHappened();
    break;
  case LabelBackupOneStep:
    ChainHappened();
    if ((sx==party.Posx)&&(sy==party.Posy))
      TaskMessage(TASKMSG_MovePartyBackward);
    break;
  }
}

int QUESTION_LIST_DATA::OnSaveGame(unsigned int *saveArea)
{
  saveArea[0]=UserResult;
  return 1;
}

int QUESTION_LIST_DATA::OnLoadGame(unsigned int *saveArea)
{
  UserResult=saveArea[0];
  return 1;
}
////////////////////////////////////////////////////////////////////////QUESTION_BUTTON_DATA

void QUESTION_BUTTON_DATA::OnInitialEvent(void)
{
  UserResult=NoUserResult;
  menu.reset();
  menu.setStartCoord(USE_DEFAULT_HORZ_MENU, 0);
  menu.setHorzOrient();
  menu.setItemSeparation(7);
  FormatDisplayText(textData, GetEventText());
  currPic = GetEventPic();
  currPic.LoadPicSurfaces("sp_");
  int count=0;
  int i;

  for (i=0;i<MAX_BUTTONS;i++)
  {
    if (buttons.buttons[i].label != "")
    {
      menu.addMenuItem(buttons.buttons[i].label);
      count++;
    }   
    else
    {
      int newItem = menu.addMenuItem(" "); // placeholder
      menu.setItemInactive(newItem);
    }
  }
  menu.SetFirstLettersShortcuts();
  menu.setCurrentItem(0);
  sx=party.Posx;sy=party.Posy;
  if (count == 0) ChainHappened();  
}

void QUESTION_BUTTON_DATA::OnKeypress(key_code key, char ascii)
{
  if (TABParty(key)) return;
  if (key != KC_RETURN)
  {
    HMenuVPartyKeyboardAction(key,ascii);
    //StandardMenuKeyboardAction(key, ascii);
    return;
  }
  currPic.Clear();
  if (WaitUntilReturnPressed()) ClearFormattedText(textData);
  UserResult = menu.currentItem();  
  QBUTTON_OPTION &button = buttons.buttons[UserResult-1];
  if (IsGlobalEvent())
  {
    if (!globalData.eventData.IsValidEvent(button.chain))
      PushEvent(new DO_NOTHING_EVENT, DeleteEvent);
    else PushGlobalEvent(button.chain);
  }
  else
  {
    if (!levelData.eventData.IsValidEvent(button.chain))
      PushEvent(new DO_NOTHING_EVENT, DeleteEvent);
    else PushEvent(button.chain);
  }
}

void QUESTION_BUTTON_DATA::OnDraw(void)
{
  UpdateQuestionButtonScreen();
}

void QUESTION_BUTTON_DATA::OnUpdateUI(void)
{
  if (CheckForMultiBoxText()) return;
  for (int i=0;i<MAX_BUTTONS;i++)
  {
    if (!buttons.buttons[i].present)
      menu.setItemInactive(i+1);
  }
}

void QUESTION_BUTTON_DATA::OnReturnToTopOfQueue(void)
{
  currPic.Clear();
  ClearFormattedText(textData);
  if (UserResult == NoUserResult)
  {
    ChainHappened();
    return;
  }

  QBUTTON_OPTION &button = buttons.buttons[UserResult-1];
  switch (button.postChainAction)
  {
  case LabelDoNothing:
    ChainHappened();
    break;
  case LabelReturnToQuestion:
    // only if still in starting location
    if ((sx==party.Posx)&&(sy==party.Posy))
      OnInitialEvent();
    else
      ChainHappened();
    break;
  case LabelBackupOneStep:
    ChainHappened();
    if ((sx==party.Posx)&&(sy==party.Posy))
      TaskMessage(TASKMSG_MovePartyBackward);
    break;
  }
}

int QUESTION_BUTTON_DATA::OnSaveGame(unsigned int *saveArea)
{
  saveArea[0]=UserResult;
  return 1;
}

int QUESTION_BUTTON_DATA::OnLoadGame(unsigned int *saveArea)
{
  UserResult=saveArea[0];
  return 1;
}

////////////////////////////////////////////////////////////////////////QUEST_EVENT_DATA
void QUEST_EVENT_DATA::OnInitialEvent(void)
{
  FormatDisplayText(textData, GetEventText());
  currPic = GetEventPic();
  currPic.LoadPicSurfaces("sp_");

  if ((accept == QA_ImpossibleAuto) || (accept == QA_AutoAccept))
  {
    if (textData.numLines == 0)
    {
      menu.reset();
    }
    else
    {
      menu.setMenu(PressEnterMenuData);
      menu.setHorzOrient();
    }
    //simulatedKeyQueue.PushKey(VK_RETURN); <--- called in OnUpdateUI
  }
  else
  {
    menu.setMenu(QuestMenuData);
    menu.setHorzOrient();
  }
}

void QUEST_EVENT_DATA::OnDraw(void)
{
  UpdateQuestScreen();
}

void QUEST_EVENT_DATA::OnUpdateUI(void)
{
  if (CheckForMultiBoxText()) return;
  if ((accept == QA_ImpossibleAuto) || (accept == QA_AutoAccept))
  {
    if (textData.numLines == 0)
      simulatedKeyQueue.PushKey(VK_RETURN);
  }
}

void QUEST_EVENT_DATA::OnKeypress(key_code key, char ascii)
{
  if (TABParty(key)) return;
  if (key != KC_RETURN)
  {
    HMenuVPartyKeyboardAction(key, ascii);
    return;
  }

  bool accepted = false;

  switch (accept)
  {
  case QA_AutoAccept:
  case QA_OnYesOrNo:
    accepted = true;
    break;
  case QA_ImpossibleAuto:
  case QA_Impossible:
    accepted = false;
    break;
  default:
    switch (menu.currentItem())
    {      
    case 1: // Yes
      accepted = (accept == QA_OnYes);
      break;
    case 2: // No
      accepted = (accept == QA_OnNo);
      break;
    }
  }

  if (accepted)
  {
    globalData.questData.SetStage(quest, stage);

    if (completeOnAccept)
      globalData.questData.SetComplete(quest);
    else if (stage == 1)
      globalData.questData.SetInProgress(quest);

    // could be automatic success, and there would be no
    // acceptChain so follow normal chain
    if (IsGlobalEvent())
    {
      if (!globalData.eventData.IsValidEvent(acceptChain))
      {
        if (accept == QA_AutoAccept)
          ChainHappened();
        else
          ReplaceGlobalEvent(new DO_NOTHING_EVENT, DeleteEvent);
      }
      else ReplaceGlobalEvent(acceptChain);
    }
    else
    {
      if (!levelData.eventData.IsValidEvent(acceptChain))
      {
        if (accept == QA_AutoAccept)
          ChainHappened();
        else
          ReplaceEvent(new DO_NOTHING_EVENT, DeleteEvent);
      }
      else ReplaceEvent(acceptChain);
    }
  }
  else
  {
    // could be automatic fail, and there would be no
    // rejectChain so follow normal chain
    if (failOnRejection)
      globalData.questData.SetFailed(quest);

    if (IsGlobalEvent())
    {
      if (!globalData.eventData.IsValidEvent(rejectChain))
      {
        if (accept == QA_ImpossibleAuto)
          ChainHappened();
        else
          ReplaceGlobalEvent(new DO_NOTHING_EVENT, DeleteEvent);
      }
      else ReplaceGlobalEvent(rejectChain);
    }
    else
    {
      if (!levelData.eventData.IsValidEvent(rejectChain))
      {
        if (accept == QA_ImpossibleAuto)
          ChainHappened();
        else
          ReplaceEvent(new DO_NOTHING_EVENT, DeleteEvent);
      }
      else ReplaceEvent(rejectChain);
    }
  }

  //currPic.Clear();
  //ClearFormattedText(textData);
}

bool QUEST_EVENT_DATA::OnTestTrigger(void)
{
  if (!GameEvent::OnTestTrigger()) return false;
  WORD CurrStage = globalData.questData.GetStage(quest);
  bool result = (CurrStage == (stage-1));
//#ifdef _DEBUG
  WriteDebugString("Quest Event %u at %i,%i level %i %s triggered (Curr=%i, Quest=%i)\n",
                    id, x,y, globalData.currLevel, 
                   (result==true ? "is" : "not"),
                   CurrStage, stage);
//#endif
  return result;
}

////////////////////////////////////////////////////////////////////////LOGIC_BLOCK_DATA
void LOGIC_BLOCK_DATA::OnInitialEvent(void)
{
  menu.reset();
}

void LogicBlockErrorMsg(CString& msg, LOGIC_BLOCK_DATA *data)
{
  CString LBMSG;
  LBMSG.Format("%s -- id=%d,x=%d,y=%d\n",msg,data->id,party.Posx,party.Posy);
  WriteDebugString(LBMSG);
}

CString LBsubst(const CString& p, CString *w)
{
  int index;
  index=p.Find('&');
  if (index<0) return p;
  CString result;
  int start=0;
  int col=0;
  int len=p.GetLength();
  while ( col < len )
  {
    if (p[col]=='&')
    {
      if ((col<len-1) && (p[col+1]>='A') && (p[col+1]<='L'))
      {
        result += p.Mid(start, col-start);
        col++;
        result += w[p[col]-'A'];
        col++;
        start=col;
      };
    }
    else col++;
  };
  if (col!=start) result += p.Mid(start, col-start);
  return result;
}

int LBseparateCharacter(CString& sp)
{ //Removes the character selector from sp and returns the
  //character index (or -1);
  int len, i, n;
  len = sp.GetLength();
  if ( (len==0) || (sp.GetAt(0)!='(') ) return party.activeCharacter;
  if (len < 3) return -1; // Shortest possible is (^) or (a) or (*)
  if (sp.GetAt(0) != '(') return -1;
  if (sp.Left(3) == "(*)")
  {
    sp = sp.Right(len-3);
    return 999;
  };
  if (sp.GetAt(1) == '^')
  { // A numbered character
    if (sp == "(^)") return party.activeCharacter;
    if (len < 4) return -1; // Should be (^3)
    if (sp.GetAt(3) != ')') return -1;
    if ((sp.GetAt(2)<'1') || (sp.GetAt(2)>'9')) return -1;
    n = sp.GetAt(2) - '0' - 1;
    if (n >= party.numCharacters) return -1;
    sp = sp.Right(len-4);
    return n;
  };
  for (i=1; i<len; i++)
  {
    if (sp.GetAt(i) == ')') break;
  };
  if (sp.GetAt(i) != ')') return -1;
  CString name = sp.Mid(1,i-1);
  for (n=0; n<party.numCharacters; n++)
  {
    if (name.CompareNoCase(party.characters[n].GetName())==0)
    {
      sp = sp.Right(len-i-1);
      return n;
    };
  };
  return -1;
}

/*
enum LOGIC_BLOCK_INPUT_TYPE {
  LBIT_literal = 0,
  LBIT_globalASL,
  LBIT_partySize,
  LBIT_charInfo,
  LBIT_dirFacing,
  LBIT_levelASL,
  LBIT_questStage,
  LBIT_itemList,
  LBIT_NPCList,
  LBIT_RunTimeIf,
  LBIT_NotImplemented = 0xff
}
*/


static int LBString2Integer(const CString& str)
{ //We accept 'dirty' numbers.
  CString clean;
  char *digits;
  int i, num;
  int result = 0;
  digits = (char *)_alloca(str.GetLength()+10);
  num = CleanNumber(str, digits);
  for (i=2; i<num; i++)
  {
    result=10*result + digits[i]-'0';
  };
  if (digits[0] == '-') result = -result;
  return result;
}

static void SplitLevelKey(const CString& str, int& level, CString& key)
{
  int i, len = str.GetLength();
  level = 0;
  if ( (len>1) && (str[0] == '/') )
  {
    for (i=1; i<len; i++)
    {
      if ( (str[i] >= '0') && (str[i] <= '9') ) 
      {
        level = 10 * level + str[i] - '0';
        continue;
      };
      if (str[i] == '/')
      {
        key = str.Right(len-i-1);
        return;
      };
    };
  };
  level = GetCurrentLevel();
  key = str;
}

// The re_registers are made kept here so that after doing a grep you
// can return and get the sub-fields matched (using Wiggle).
static re_registers grepReg;
CString MostRecentlyGreppedString;


void ProcessLBInput(BYTE *type,             // LOGIC_BLOCK_INPUT_TYPE
                    CString *param,         // source param
                    CString& result,        // result string
                    LOGIC_BLOCK_DATA *data, // original logic data
                    char inputName,         // input name letter
                    CString *w)             // array of result strings
{
  switch(*type)
  {
  case LBIT_literal: result=LBsubst(*param,w); break;
  case LBIT_questStage:
    result.Format("%d",globalData.questData.GetStage(LBsubst(*param,w))); 
    break;
  case LBIT_partySize:
    result.Format("%d",party.numCharacters);
    break;
  case LBIT_dirFacing:
    result.Format("%d",party.facing);
    break;
  case LBIT_globalASL:
    result=globalData.global_asl.Lookup((LPCSTR) LBsubst(*param,w));
    break;
  case LBIT_tempASL:
    result=globalData.temp_asl.Lookup((LPCSTR) LBsubst(*param,w));
    break;
  case LBIT_charASL:
    {
      CString sp;
      int n;
      sp = LBsubst(*param, w);
      n = LBseparateCharacter(sp); // sp<--ASL name;
      if ( (n < 0) || (n >= party.numCharacters) ) 
      {
        sp = "Bogus character selector in Input";
        LogicBlockErrorMsg(sp, data);
        result = "";
        break;
      };
      result = party.characters[n].char_asl.Lookup(sp);
      break;
    };
  case LBIT_partyASL:
    result=party.party_asl.Lookup((LPCSTR) LBsubst(*param,w));
    break;
  case LBIT_itemList:
    {
      result="";
      for (int charNum=0; charNum<party.numCharacters; charNum++)
      {
        result += party.characters[charNum].getItemList(charNum);
      };
    };
    break;
  case LBIT_levelASL:
    {
      int level;
      CString key;
      SplitLevelKey(LBsubst(*param,w), level, key);
      result=globalData.levelInfo.stats[level].level_asl.Lookup(key);
    };
    break;
  case LBIT_RunTimeIf:
    { 
      CString sp;
      int n;
      sp = LBsubst(*param, w);
      n = LBseparateCharacter(sp); // sp will have ASL name after return
      if ( (n < 0) || (n >= party.numCharacters) ) 
      {
        SetCharContext( GetActiveChar(NULL).GetContext() );
        SetTargetContext( GetActiveChar(NULL).GetContext() );
      }
      else
      {
        SetCharContext( party.characters[n].GetContext() );
        SetTargetContext( party.characters[n].GetContext() );
      }

      unsigned long key = KeywordTextToNum(sp);
      switch (GetDataSize(key))
      {
      case IF_TYPE_STRING:
          result = GetDataSTRING(key);
        break;
      case IF_TYPE_BYTE:
        {
          char tmp = GetDataBYTE(key);
          result.Format("%i", tmp);
        }
        break;
      case IF_TYPE_WORD:
        {
          short tmp = GetDataWORD(key);
          result.Format("%i", tmp);
        }
        break;
      case IF_TYPE_DWORD:
        {
          long tmp = GetDataDWORD(key);
          result.Format("%i", tmp);
        }
        break;
      case IF_TYPE_FLOAT:
        {
          float tmp = GetDataFLOAT(key);
          result.Format("%f", tmp);
        }
        break;
      case IF_TYPE_UBYTE:
        {
          unsigned char tmp = GetDataUBYTE(key);
          result.Format("%u", tmp);
        }
        break;
      case IF_TYPE_UWORD:
        {
          unsigned short tmp = GetDataUWORD(key);
          result.Format("%u", tmp);
        }
        break;
      case IF_TYPE_UDWORD:
        {
          unsigned long tmp = GetDataUDWORD(key);
          result.Format("%u", tmp);
        }
        break;
      default:
        CString msg;
        msg.Format("Bogus Logic Input Param for LBIT_RunTimeIf %s", param);
        LogicBlockErrorMsg(msg, data);
        break;
      }
      ClearCharContext();
      ClearTargetContext();
    }
    break;
  case LBIT_NPCList:
    {
      CString temp;
      result="";
      for (int i=0; i<party.numCharacters; i++)
      {
        if (party.characters[i].GetType() == NPC_TYPE)
        {
          temp.Format("%s/%s/%d/",result,party.characters[i].GetName(),i);
          result=temp;
        }
      };
    }
    break;
  case LBIT_charInfo:
    {
      result = "";
      for (int i=0; i<party.numCharacters; i++)
      {
        if (party.characters[i].GetType() == CHAR_TYPE)
        {
          result = result+"/" + party.characters[i].getInfo() + "/";
        };
      };
    };
    break;
  case LBIT_Wiggle:
    {
      int n;
      result = "";
      n = atoi ((LPCTSTR) LBsubst(*param,w));
      if ((n<0) || (n >= (int)grepReg.num_regs)) break;
      result = MostRecentlyGreppedString.Mid(grepReg.start[n],
                                             grepReg.end[n]-grepReg.start[n]);

    };
    break;
  case LBIT_SourceGPDL:
    {
      GPDLCOMP gpdlComp;
      const char *entries[] = {"input", NULL};
      CString sourcePgm, binaryPgm;
      sourcePgm = "$PUBLIC $FUNC input(A,B,C,D,E,F) {"
                  + *param
                  + "}input;";
      binaryPgm = gpdlComp.CompileScript(sourcePgm, entries);
      if (binaryPgm[0] != 0)
      { // error ... henceforth just use a literal ""
        WriteDebugString("%s",binaryPgm);
        result = "";
        *param = "";
        *type = LBIT_literal;
        break;
      };
      *param = binaryPgm;
      *type = LBIT_BinaryGPDL;
    };
    // !!!!! **** NOTE ***** !!!!!
    //    We fall through here.
    // !!!!! **** NOTE ***** !!!!!
  case LBIT_BinaryGPDL:
    result = gpdl.ExecuteScript(*param,1,w,6); 
    break;
  default:
    {
      CString msg;
      msg.Format("Bogus Logic Input-%c Type = 0x%02x",inputName,type);
      LogicBlockErrorMsg(msg, data);
    };
  };

  if ((data->m_Flags & LBF_RUNTIME_DEBUG) != 0)
  {
    CString msg;
    msg.Format("Logic Block Input %c = %s",inputName,result);
    LogicBlockErrorMsg(msg, data);
  }
}



static CString LBagreater(CString& s1, CString& s2)
{
  CString result;
  result = LongSubtract(s2, s1);
  if (result[0] =='-') return CString("1");
  else return CString("");
}

static CString LBaslash(CString& s1, CString& s2)
{
  CString quot, remain;
  LongDivide(s1, s2, quot, remain);
  return quot;
}

static CString LBamod(CString& s1, CString& s2)
{
  CString quot, remain;
  LongDivide(s1, s2, quot, remain);
  return remain;
}

static void ProcessLBGate(BYTE type,      // LOGIC_BLOCK_GATE_TYPE
                          CString& top,   // input 1
                          CString& side,  // input 2
                          CString& result,
                          LOGIC_BLOCK_DATA *data,
                          char gateName)
{
  switch(type)
  {
  
  case LBGT_top: result=top; break;
  case LBGT_side: result=side; break;
  case LBGT_land:
    result = (top!="")&&(side!="") ? "1" : "";
    break;
  case LBGT_lor:
    result = (top!="")||(side!="") ? "1" : "";
    break;
  case LBGT_stringEqual:
    result = (top==side) ? "1" : "";
    break;
  case LBGT_aplus:
    result=LongAdd(top,side);
    break;
  case LBGT_aminus:
    result=LongSubtract(top,side);
    break;
  case LBGT_agreater:
    result = LBagreater(top, side);
    break;
  case LBGT_agear:
    result = LongMultiply(top,side);
    break;
  case LBGT_aslash:
    result = LBaslash(top,side);
    break;
  case LBGT_amod:
    result = LBamod(top,side);
    break;
  case LBGT_true:
    result = "1";
    break;
  case LBGT_false:
    result = "";
    break;
  case LBGT_grep:
    {
      re_pattern_buffer grepPat;
      re_Set_EGrep();
      const char *m_grepCResult=re_compile(LPCTSTR(side), &grepPat);
      if (m_grepCResult!=NULL) 
      {
          CString msg;
          msg="Error compiling LogicBlock Grep pattern\n";
          msg+=side;
          msg+=CString(m_grepCResult);
          LogicBlockErrorMsg(msg, data);
          result="";
          break;
      };
      MostRecentlyGreppedString = top;
      int srchRslt=re_search(
                 &grepPat,
                 LPCTSTR(top),
                 top.GetLength(),
                 0,
                 top.GetLength(),
                 &grepReg);
      if (srchRslt<0) 
      {
        result="";
        break;
      };
      result=top.Mid(grepReg.start[0],grepReg.end[0]-grepReg.start[0]);
      break;
    };
    break;
  default:
    {
      CString msg;
      msg.Format("Bogus Logic Gate-%c Type = 0x%02x",gateName,type);
      LogicBlockErrorMsg(msg, data);
    };
  };

  if ((data->m_Flags & LBF_RUNTIME_DEBUG) != 0)
  {
    CString msg;
    msg.Format("Logic Block Gate %c = %s",gateName,result);
    LogicBlockErrorMsg(msg, data);
  };
}

static void ProcessLBNot(BYTE type, CString& input, CString& result)
{
  if (type==0) // not NOT, who's there?
  {
    result=input;
  }
  else // perform NOT
  {
    result = input=="" ? "1" : "";
  };
}

static bool SplitKeyValue(const CString& str, CString& key, CString& value, char token='=')
{
  int i;
  i=str.Find(token);
  if (i<0)
  {
    key=str;
    value="";
    return false;
  }
  else
  {
    key=str.Left(i);
    value=str.Right(str.GetLength()-i-1);    
  };
  return true;
}

/*
enum LOGIC_BLOCK_ACTION_TYPE {
  LBAT_nothing=0,
  LBAT_setGlobalASL,
  LBAT_setLevelASL,
  LBAT_removeGlobalASL,
  LBAT_removeLevelASL,
  LBAT_setQuestStage,
  LBAT_tempASL,
  LBAT_NotImplemented = 0xff
}
*/
static void ProcessLBAction(BYTE If,                // process action? // 0=if true, 1=if false, 2=always
                            BYTE Result,            // result so far
                            BYTE *type,             // LOGIC_BLOCK_ACTION_TYPE
                            CString *param,         // input param
                            LOGIC_BLOCK_DATA *data, // ptr to orig logic data
                            char actionName,        // name of action
                            CString *w)             // array of result strings
{
  if ((If == 0) && (Result == 0)) // only process if true?
  {
    CString msg;
    msg.Format("Logic Action-%c Type = 0x%02x not performed, result=0",actionName,type);
    LogicBlockErrorMsg(msg, data);
    return;
  }

  if ((If == 1) && (Result == 1)) // only process if false?
  {
    CString msg;
    msg.Format("Logic Action-%c Type = 0x%02x not performed, result=1",actionName,type);
    LogicBlockErrorMsg(msg, data);
    return;
  }

  switch(*type)
  {
  case LBAT_nothing: break;
  case LBAT_setQuestStage:
    {
      CString key, value;
      SplitKeyValue(LBsubst(*param,w), key, value);
      globalData.questData.SetStage(key, atoi(value)); 
    };
    break;
  case LBAT_setGlobalASL:
    {
      CString key, value;
      SplitKeyValue(LBsubst(*param,w), key, value);
      globalData.global_asl.Insert(key, value, 0);
    };
    break;
  case LBAT_removeGlobalASL:
    {
      globalData.global_asl.Delete(LBsubst(*param,w));
    };
    break;
  case LBAT_setLevelASL:
    {
      CString levelKey, key, value;
      int level;
      SplitKeyValue(LBsubst(*param,w), levelKey, value,'=');
      SplitLevelKey(levelKey, level, key);
      globalData.levelInfo.stats[level].level_asl.Insert(key, value, 0);
    };
    break;
  case LBAT_removeLevelASL:
    {
      CString key;
      int level;
      SplitLevelKey(LBsubst(*param,w), level, key);
      globalData.levelInfo.stats[level].level_asl.Delete(key);
    };
    break;
  case LBAT_setCharASL:
    {
      CString sp, key, value;
      int n;
      sp = LBsubst(*param,w);
      n = LBseparateCharacter(sp); // sp <-- asl key=value
      if (n < 0)
      {
        sp = "Bogus character identifier in Set Character ASL";
        LogicBlockErrorMsg(sp, data);
        break;
      };
      SplitKeyValue(sp, key, value);
      if (n < 999) // 999 means all party members
      {
        party.characters[n].char_asl.Insert(key, value, 0);
        break;
      };
      for (n=0; n<party.numCharacters; n++)
      {
        party.characters[n].char_asl.Insert(key, value, 0);
      };
      break;
    };
    break;
  case LBAT_setIconIndexByName:
    {
      CString key, value;
      SplitKeyValue(*param, key, value);
      for (int i=0;i<party.numCharacters;i++)
      {
        if (party.characters[i].GetName()==key)
        {
          party.characters[i].iconIndex=atoi(value);
          if (party.characters[i].iconIndex < 1)
            party.characters[i].iconIndex = 1;
          break;
        }
      }
    }
    break;
  case LBAT_setPartyASL:
    {
      CString key, value;
      SplitKeyValue(LBsubst(*param,w), key, value);
      party.party_asl.Insert(key, value, 0);
    };
    break;
  case LBAT_removePartyASL:
    {
      party.party_asl.Delete(LBsubst(*param,w));
    };
    break;
  case LBAT_tempASL:
    {
      CString key, value;
      SplitKeyValue(LBsubst(*param,w), key, value);
      globalData.temp_asl.Insert(key, value, 0);
    };
    break;
  case LBAT_SourceGPDL:
    {
      GPDLCOMP gpdlComp;
      const char *entries[] = {"action", NULL};
      CString sourcePgm, binaryPgm;
      sourcePgm = "$PUBLIC $FUNC action() {"
                  + *param
                  + "}action;";
      binaryPgm = gpdlComp.CompileScript(sourcePgm, entries);
      if (binaryPgm[0] != 0)
      { // error ... henceforth just do nothing
        WriteDebugString("%s",binaryPgm);
        //result = "";
        //*param = "";
        *type = LBAT_nothing;
        break;
      };
      *param = binaryPgm;
      *type = LBAT_BinaryGPDL;
    };
    // !!!!! **** NOTE ***** !!!!!
    //    We fall through here.
    // !!!!! **** NOTE ***** !!!!!
  case LBAT_BinaryGPDL:
    gpdl.ExecuteScript(*param,1,NULL,0); 
    break;
  default:
    {
      CString msg;
      msg.Format("Bogus Logic Action-%c Type = 0x%02x",actionName,type);
      LogicBlockErrorMsg(msg, data);
    };
  };
}


static void RecordLogicBlockValues(CString *w, LOGIC_BLOCK_DATA *data)
{
  int i, len, totalLen;
  char *buffer;
  for (len=0, i=0; i<12; i++) len += w[i].GetLength();
  if (len >30000) 
  {
    CString msg("Values too large to record");
    LogicBlockErrorMsg(msg,data);
    return;
  };
  totalLen = len+12*sizeof(int);
  CString str;
  buffer = str.GetBuffer(totalLen);// To avoid the 18 mallocs that would be required to
                                   // build the result using concatenation.
  for (len=0, i=0; i<12; i++)
  {
    len = w[i].GetLength();
    memcpy(buffer,(char *)(&len),sizeof(int));
    buffer += sizeof(int);
    memcpy(buffer,(LPCTSTR)w[i],len);
    buffer += len;
  };
  str.ReleaseBuffer(totalLen);
  globalData.global_asl.Insert("LOGICBLOCKVALUES", str, 0);
}

static BOOL ProcessLogicBlock(LOGIC_BLOCK_DATA *data)
{
  CString w[18];// 0=InputA, 1=InputB, 2=GateC, 3=InputD, 4=GateE;
                // 5=InputF, 6=InputG, 7=GateH, 8=GateI, 9=GateJ, 10=GateK, 11=GateL;
                // 12=NotC, 13=NotE, 14=NotH, 15=NotI, 16=NotJ, 17=NotK;
  /* 
      Inputs: A, B, D, F, G
       Gates: C, E, H, I, J, K, L

  Inputs    B              F
            |              |
            C  <-  A  ->   H
            |              |
           not?           not?
            |              |
            E  <-  D  ->   J
            |              |
           not?           not?
            |              |
            I  <-  G  ->   K
            |              |
           not?           not?
            |_______       |
                   |       |
                   L <-----|
                   |
        Action1---------Action2 // take action(s) based on result of gate L

   w0 = inputA
   w1 = inputB
   w2 = gateC(w0,w1)
   w12 = not(w2)
   w3 = inputD
   w4 = gateE(w3,w12)
   w13 = not(w4)
   w5 = inputF
   w6 = inputG
   w7 = gateH(w5,w0)
   w14 = not(w7)
   w8 = gateI(w6,w13)
   w15 = not(w8)
   w9 = gateJ(w3,w14)
   w16 = not(w9)
   w10 = gateK(w6,w16)
   w17 = not(w10)
   w11 = gateL(w15,w17)

  Inputs    w1          w5
            |           |
            w2 <- w0 -> w7
            |           |
            w12         w14
            |           |
            w4 <- w3 -> w9
            |           |
            w13         w16
            |           |
            w8 <- w6 -> w10
            |           |
            w15         w17
            |____       |
                |       |
               w11 <----|
                |
     Action1---------Action2  // take action(s) based on result of gate L
  */

  //ClearCharContextStack();
  //SetCharContext( GetActiveChar().GetContext() );

  ProcessLBInput (&data->m_InputTypeA, &data->m_InputParamA, w[0], data, 'A', w);
  ProcessLBInput (&data->m_InputTypeB, &data->m_InputParamB, w[1], data, 'B', w);
  ProcessLBGate  (data->m_GateTypeC, w[1], w[0],             w[2], data, 'C');   
  ProcessLBNot   (data->m_NotC, w[2], w[12]);                                  
  ProcessLBInput (&data->m_InputTypeD, &data->m_InputParamD, w[3], data, 'D', w);
  ProcessLBGate  (data->m_GateTypeE, w[12], w[3],            w[4], data, 'E');   
  ProcessLBNot   (data->m_NotE, w[4], w[13]);                                  
  ProcessLBInput (&data->m_InputTypeF, &data->m_InputParamF, w[5], data, 'F', w);
  ProcessLBInput (&data->m_InputTypeG, &data->m_InputParamG, w[6], data, 'G', w);
  ProcessLBGate  (data->m_GateTypeH, w[5], w[0],             w[7], data, 'H');   
  ProcessLBNot   (data->m_NotH, w[7], w[14]);                                  
  ProcessLBGate  (data->m_GateTypeI, w[13], w[6],            w[8], data, 'I');   
  ProcessLBNot   (data->m_NotI, w[8], w[15]);                                  
  ProcessLBGate  (data->m_GateTypeJ, w[14], w[3],            w[9], data, 'J');   
  ProcessLBNot   (data->m_NotJ, w[9], w[16]);                                  
  ProcessLBGate  (data->m_GateTypeK, w[16], w[6],            w[10],data, 'K');   
  ProcessLBNot   (data->m_NotK, w[10], w[17]);                                 
  ProcessLBGate  (data->m_GateTypeL, w[15], w[17],           w[11],data, 'L');   

  data->Result = w[11]=="" ? 0 : 1; // result of all logic ops


  // Now record the results for posterity
  if ((data->m_Flags & LBF_RECORD_VALUES) != 0)RecordLogicBlockValues(w, data);


  // perform conditional action based on result
  ProcessLBAction(data->m_IfTrue1, // 0=if true, 1=if false, 2=always
                  data->Result, 
                  &data->m_ActionType1, 
                  &data->m_ActionParam1, 
                  data, 
                  '1',
                  w);
  ProcessLBAction(data->m_IfTrue2,
                  data->Result, 
                  &data->m_ActionType2, 
                  &data->m_ActionParam2, 
                  data, 
                  '2',
                  w);
  //ClearCharContext();


  return data->Result;
}

bool LOGIC_BLOCK_DATA::OnIdle(void)
{
  Result=0;
  // Here we will compute the result
  ProcessLogicBlock(this);


  switch (m_NoChain)
  {
  case 0:  // No chain
    PopEvent();
    break;
  case 1:  // Chain Always
    ChainHappened();
    break;
  case 2:  // Chain conditionally
    switch (Result)
    {
    case 0:
      if (m_ChainIfFalse)
      {
        if (IsGlobalEvent())
        {
          if (!globalData.eventData.IsValidEvent(falseChain))
              ReplaceGlobalEvent(new DO_NOTHING_EVENT, DeleteEvent);
          else ReplaceGlobalEvent(falseChain);
        }
        else
        {
          if (!levelData.eventData.IsValidEvent(falseChain))
              ReplaceEvent(new DO_NOTHING_EVENT, DeleteEvent);
          else ReplaceEvent(falseChain);
        }
      }
      else
        PopEvent();
      break;
    case 1:
      if (m_ChainIfTrue)
      {
        if (IsGlobalEvent())
        {
          if (!globalData.eventData.IsValidEvent(trueChain))
              ReplaceGlobalEvent(new DO_NOTHING_EVENT, DeleteEvent);
          else ReplaceGlobalEvent(trueChain);
        }
        else
        {
          if (!levelData.eventData.IsValidEvent(trueChain))
              ReplaceEvent(new DO_NOTHING_EVENT, DeleteEvent);
          else ReplaceEvent(trueChain);
        }
      }
      else
        PopEvent();
      break;
    }
    break;
  default:
    PopEvent();
    break;
  }

  return false;
}

////////////////////////////////////////////////////////////////////////GUIDED_TOUR
const int TOUR_TIMEOUT = 1000;

void GUIDED_TOUR::OnEnterQueue(void)
{
  MouseRender.EnableWaitCursor();
}

void GUIDED_TOUR::OnLeaveQueue(void)
{
  MouseRender.DisableWaitCursor();
}

void GUIDED_TOUR::OnInitialEvent()
{
  menu.setMenu(PressEnterMenuData);
  menu.setHorzOrient();
  // GetEventText() is over-ridden for this
  // event to return text for each tour step,
  // so it's critical to init currStep before
  // calling GetEventText() for first time.
  if (GetRunAsVersion(GuidedTourVersion) <= _VERSION_0697_)
  {
    currStep=0;
    FormatDisplayText(textData, GetEventText());
  }
  else
  {
    currStep=-1;
  }

  currPic = GetEventPic();
  currPic.LoadPicSurfaces("sp_");
  if (useStartLoc)
  {
    if (   (tour_x < 0) || (tour_x >= levelData.area_width)
        || (tour_y < 0) || (tour_y >= levelData.area_height))
    {
      WriteDebugString("Invalid starting x,y in GUIDED_TOUR event\n");
      PopEvent();
      return;
    }
    party.Posx = tour_x;
    party.Posy = tour_y;
    party.facing = facing;
  }
  if (GetEventStep() != TStep_Pause) 
  {
    SetMyState(TASK_GuidedTourMoving);
    ((CDungeonApp*)AfxGetApp())->SetIntervalTimer(TASKTIMER_GuidedTour,TOUR_TIMEOUT);
  }
  else
    SetMyState(TASK_GuidedTourPausing);    
}

void GUIDED_TOUR::OnKeypress(key_code key, char ascii)
{  
  if (TABParty(key)) return;
  if (GetMyState() != TASK_GuidedTourPausing)
    return;

  if (key != KC_RETURN)
  {
    HMenuVPartyKeyboardAction(key,ascii);
    //StandardMenuKeyboardAction(key, ascii);
    return;
  }

  TakeNextStep();
}

void GUIDED_TOUR::OnDraw()
{
  menu.setAllItemsActive();
  if (GetMyState() == TASK_GuidedTourMoving)
  {
    menu.setAllItemsInactive();
    UpdateGuidedTourMoveScreen();
  }
  else
    UpdateGuidedTourScreen();
}

bool GUIDED_TOUR::OnTimer(UINT timerId)
{
  if (timerId != TASKTIMER_GuidedTour)
    return false;
  TakeNextStep();
  return true;
}

void GUIDED_TOUR::TakeNextStep()
{
  currStep++;
  // skip invalid steps
  while (   (currStep < MAX_TOUR_STEPS)
         && (GetEventStep() == TStep_NoMove))
    currStep++;

  if (!TourOver())
  {
    switch (GetEventStep())
    {
    case TStep_Pause:  // display text only      
      SetMyState(TASK_GuidedTourPausing);
      break;
    case TStep_Forward:// move party forward and display text
      party.movePartyForward();
      UpdatePartyMovementData();
      PushEvent(new PLAYSTEPSOUND_EVENT, DeleteEvent);
      SetMyState(TASK_GuidedTourMoving);
      break;
    case TStep_Left:   // move party left and display text
      party.turnPartyLeft();
      PushEvent(new DO_NOTHING_EVENT, DeleteEvent);
      SetMyState(TASK_GuidedTourMoving);
      break;
    case TStep_Right:  // move party right and display text
      party.turnPartyRight();
      PushEvent(new DO_NOTHING_EVENT, DeleteEvent);
      SetMyState(TASK_GuidedTourMoving);
      break;
    case TStep_NoMove: // shouldn't happen
      ASSERT(FALSE);
      break;
    }    
  }
  else
  {
    if (WaitUntilReturnPressed()) ClearFormattedText(textData);
    currPic.Clear();
    if (executeEvent)
      TaskMessage(TASKMSG_ExecuteEvent);
    ChainHappened();
    //local->ClearUserInput();    
  }
}

void GUIDED_TOUR::OnReturnToTopOfQueue(void)
{
  if (GetMyState() == TASK_GuidedTourMoving)
  {
    FormatDisplayText(textData, GetEventText());
    if (GetMyState() == TASK_GuidedTourMoving)
      ((CDungeonApp*)AfxGetApp())->SetIntervalTimer(TASKTIMER_GuidedTour,TOUR_TIMEOUT);
  }
}

////////////////////////////////////////////////////////////////////////ENCOUNTER_DATA
void ENCOUNTER_DATA::OnInitialEvent(void)
{
  menu.reset();
  menu.setStartCoord(USE_DEFAULT_HORZ_MENU, 0);
  menu.setHorzOrient();
  FormatDisplayText(textData, GetEventText());

  currDist = distance;

  int origX = party.Posx;
  int origY = party.Posy;

  int newX, newY;
  // check one square away to see if it is obstructed
  if (!party.newForwardPosition(0, newX, newY))
  {
    // move monsters closer since party is up against
    // an obstruction (a wall?)
    currDist = UpClose;
  }
  else
  {
    party.Posx = newX;
    party.Posy = newY;

    // check next square
    if (!party.newForwardPosition(0, newX, newY))
    {
      if (currDist == FarAway)
        currDist = Nearby;
    }

    party.Posx = origX;
    party.Posy = origY;
  }


  currPic = GetEventPic();  
  currPic.LoadPicSurfaces("sp_");

  if (currPic.picType == SpriteDib)
  {
    currPic.SetFrame(currDist);
    currPic.timeDelay = 0; // encounters use manual approach
  }

  int count=0;
  int i;
  for (i=0;i<MAX_BUTTONS;i++)
  {
    if (buttons.buttons[i].label != "")
    {
      menu.addMenuItem(buttons.buttons[i].label);
      count++;
    }
    else
    {
      int newItem = menu.addMenuItem(" "); // placeholder
      menu.setItemInactive(newItem);
    }
  }
  menu.SetFirstLettersShortcuts();
  menu.setCurrentItem(0);  
  if (count == 0) ChainHappened();  
}

void ENCOUNTER_DATA::OnDraw(void)
{
  UpdateEncounterScreen();
}

void ENCOUNTER_DATA::OnUpdateUI(void)
{
  if (CheckForMultiBoxText()) return;
  int i;
  for (i=0;i<MAX_BUTTONS;i++)
  {
    if (!buttons.buttons[i].present)
      menu.setItemInactive(i+1);
  }
  if (currDist == UpClose)
  {
    for (i=0;i<MAX_BUTTONS;i++)
    {
      if (!buttons.buttons[i].allowedUpClose)
        menu.setItemInactive(i+1);
    }
  }
  else
  {
    for (i=0;i<MAX_BUTTONS;i++)
    {
      if (buttons.buttons[i].onlyUpClose)
        menu.setItemInactive(i+1);
    }
  }
}

void ENCOUNTER_DATA::OnKeypress(key_code key, char ascii)
{
  if (TABParty(key)) return;
  if (key != KC_RETURN)
  {
    HMenuVPartyKeyboardAction(key,ascii);
    return;
  }  

  bool finished = false;
  TASKMESSAGE msg=TASKMSG_None;

  // This event provides a list of choices (buttons) to the player with
  // a sprite pic displayed at a specified distance.
  //
  // The event stays active until a button with a chain event is selected,
  // or until the sprite pic range is reduced to 0=UpClose. Selecting a button
  // may do nothing, chain to an event, or bring the sprite closer to the 
  // party. If no action has been taken when the sprite reaches 0 range, the
  // event may exit or branch to one of the zero range event chains.
  //
  ENCOUNTER_BUTTON_OPTION &button = buttons.buttons[menu.currentItem()-1];

  int prevDist = currDist;
  switch (button.optionResult)
  {
  case NoResult:
    // change nothing
    break;

  case DecreaseRange:
    // move sprite pic a little closer to party
    if (currDist > 0)
    {
      currDist--;
      if (currPic.picType == SpriteDib)
        currPic.SetFrame(currDist);
    }    
    break;

  case ChainToEvent:
    finished = true;
    if (levelData.eventData.IsValidEvent(button.chain))
      ReplaceEvent(button.chain);
    else
      ReplaceEvent(new DO_NOTHING_EVENT, DeleteEvent);
    break;
  case CombatNoSurprise:
    finished = true;
    if (levelData.eventData.IsValidEvent(combatChain))
    {
      InsertEvent(combatChain);
      msg = TASKMSG_Combat_NoSurpise;
    }
    else
      ReplaceEvent(new DO_NOTHING_EVENT, DeleteEvent);
    break;
  case CombatSlowPartySurprised:    
    finished = true;
    if (levelData.eventData.IsValidEvent(combatChain))
    {        
      InsertEvent(combatChain);
      if (party.getPartySpeed() < monsterSpeed)
        msg = TASKMSG_Combat_PartySurpised;
      else
        msg = TASKMSG_Combat_NoSurpise;
    }
    else
      ReplaceEvent(new DO_NOTHING_EVENT, DeleteEvent);
    break;
  case CombatSlowMonsterSurprised:    
    finished = true;
    if (levelData.eventData.IsValidEvent(combatChain))
    {        
      InsertEvent(combatChain);
      if (party.getPartySpeed() > monsterSpeed)
        msg = TASKMSG_Combat_MonstersSurpised;
      else
        msg = TASKMSG_Combat_NoSurpise;
    }
    else
      ReplaceEvent(new DO_NOTHING_EVENT, DeleteEvent);
    break;
  case Talk:
    finished = true;
    if (levelData.eventData.IsValidEvent(talkChain))
      ReplaceEvent(talkChain);
    else
      ReplaceEvent(new DO_NOTHING_EVENT, DeleteEvent);
    break;
  case EscapeIfFastPartyElseCombat:    
    finished = true;
    if (party.getPartySpeed() > monsterSpeed)
    {
      if (levelData.eventData.IsValidEvent(escapeChain))      
        ReplaceEvent(escapeChain);
      else
        ReplaceEvent(new DO_NOTHING_EVENT, DeleteEvent);
    }
    else
    {
      if (levelData.eventData.IsValidEvent(combatChain))      
      {
        InsertEvent(combatChain);
        msg = TASKMSG_Combat_NoSurpise;
      }
      else
        ReplaceEvent(new DO_NOTHING_EVENT, DeleteEvent);
    }
    break;
  default:
    WriteDebugString("Unhandled zeroRangeResult %i in encounter event\n",
                     zeroRangeResult);
    break;
  }

  // if parties have reached zero proximity.
  // UpClose must be displayed and allow user input at least once,
  // and this is tracked using prevDist.
  if ((!finished) && (prevDist == UpClose))
  {      
    switch (zeroRangeResult)
    {
    case NoResult:      
    case DecreaseRange: // has no meaning here
    case ChainToEvent:  // has no meaning here
      finished = true;
      ChainHappened();
      break;

    case CombatNoSurprise:
      finished = true;
      if (levelData.eventData.IsValidEvent(combatChain))
      {
        InsertEvent(combatChain);
        msg = TASKMSG_Combat_NoSurpise;
      }
      else
        ReplaceEvent(new DO_NOTHING_EVENT, DeleteEvent);
      break;
    case CombatSlowPartySurprised:      
      finished = true;
      if (levelData.eventData.IsValidEvent(combatChain))
      {        
        InsertEvent(combatChain);
        if (party.getPartySpeed() < monsterSpeed)
          msg = TASKMSG_Combat_PartySurpised;
        else
          msg = TASKMSG_Combat_NoSurpise;
      }
      else
        ReplaceEvent(new DO_NOTHING_EVENT, DeleteEvent);
      break;
    case CombatSlowMonsterSurprised:
      finished = true;
      if (levelData.eventData.IsValidEvent(combatChain))
      {        
        InsertEvent(combatChain);
        if (party.getPartySpeed() > monsterSpeed)
          msg = TASKMSG_Combat_MonstersSurpised;
        else
          msg = TASKMSG_Combat_NoSurpise;
      }
      else
        ReplaceEvent(new DO_NOTHING_EVENT, DeleteEvent);
      break;
    case Talk:
      finished = true;
      if (levelData.eventData.IsValidEvent(talkChain))
        ReplaceEvent(talkChain);
      else
        ReplaceEvent(new DO_NOTHING_EVENT, DeleteEvent);
      break;
    case EscapeIfFastPartyElseCombat:
      finished = true;
      if (party.getPartySpeed() > monsterSpeed)
      {
        if (levelData.eventData.IsValidEvent(escapeChain))      
          ReplaceEvent(escapeChain);
        else
          ReplaceEvent(new DO_NOTHING_EVENT, DeleteEvent);
      }
      else
      {
        if (levelData.eventData.IsValidEvent(combatChain))      
        {
          InsertEvent(combatChain);
          msg = TASKMSG_Combat_NoSurpise;
        }
        else
          ReplaceEvent(new DO_NOTHING_EVENT, DeleteEvent);
      }
      break;
    default:
      WriteDebugString("Unhandled zeroRangeResult %i in encounter event\n",
                       zeroRangeResult);
      break;
    }
  }

  if (finished)
  {
    currPic.Clear();
    if (WaitUntilReturnPressed()) ClearFormattedText(textData);
    if (msg != TASKMSG_None)
    {
      TaskMessage(msg);
      PopEvent();
    }
  }    
}

////////////////////////////////////////////////////////////////////////COMBAT_EVENT_DATA

void COMBAT_EVENT_DATA::OnInitialEvent(void)
{  
  menu.reset();
  TEMPsurprise = surprise;
  m_UseOutdoorMap = 
     ((globalData.levelInfo.stats[globalData.currLevel].overland) || (outdoors));

  // check to see if we need a combat welcome event
  if (   (GetEventText().GetLength()>0)
      || (GetEventPic().filename.GetLength()>0))
  {
    SetMyState(TASK_CombatWelcome);
    TEXT_EVENT_DATA *textEvent;
    textEvent=new TEXT_EVENT_DATA;
    textEvent->text=GetEventText();
    textEvent->HighlightText=FALSE;
    textEvent->pic = GetEventPic();
    textEvent->WaitForReturn = TRUE;
    textEvent->distance = distance;

    PushEvent(textEvent, DeleteEvent);
    return;
  }

  ClearFormattedText(textData);
  currPic.Clear();
  if (!LoadOffscreenCombatSurfaces(m_UseOutdoorMap))
  {
    WriteDebugString("LoadOffscreenCombatSurfaces failed\n");
    ChainHappened();
  }

  combatData.InitCombatData(this);
  //ChangeToTaskState(TASK_CombatMainMenu);  
  ChangeToTaskState(TASK_CombatStartBgMusic);
}

void COMBAT_EVENT_DATA::OnKeypress(key_code key, char ascii)
{
  // need to handle this regardless of state
  if (   (combatData.OnAuto())
      && (combatData.getActiveCombatCharacter().IsPartyMember()))
  {
    combatData.Quick(FALSE);
    return;
  }

  if (GetMyState() != TASK_CombatMainMenu)
    return;

  if (key != KC_RETURN)
  {
    StandardMenuKeyboardAction(key, ascii);
    return;
  }
/*
   1  { "MOVE", 0 },
   2  { "AIM", 0 },
   3  { "USE", 0 },
   4  { "CAST", 0 },
   5  { "TURN", 0 },
   6  { "GUARD", 0 },
   7  { "QUICK", 0 },
   8  { "DELAY", 0 },
   9  { "BANDAGE", 0 },
   10 { "VIEW", 0 },
   11 { "SPEED", 0 },
   12 { "WIN", 0 },
   13 { "END", 0 }
*/
  switch (menu.currentItem()) 
  {
  case 1: // move
    ChangeToTaskState(TASK_CombatMoveMenu);
    break;
  case 2: // aim
    ChangeToTaskState(TASK_CombatAimMenu);
    break;
  case 3: // use
#ifdef _DEBUG
    //ChangeToTaskState(TASK_CombatUseMenu);
#endif
    break;
  case 4: // cast
    ChangeToTaskState(TASK_CombatCastMenu);
    break;
  case 5: // turn
    ChangeToTaskState(TASK_CombatTurnMenu);
    break;
  case 6: // guard
    ChangeToTaskState(TASK_CombatGuardMenu); //(TASK_CombatGuarding);
    break;
  case 7: // quick
    ChangeToTaskState(TASK_CombatQuickMenu);
    break;
  case 8: // delay
    ChangeToTaskState(TASK_CombatDelayMenu);//(TASK_CombatDelaying);
    break;
  case 9: // bandage
    ChangeToTaskState(TASK_CombatBandageMenu);//(TASK_CombatBandaging);
    break;
  case 10: // view
    ChangeToTaskState(TASK_CombatViewCharMenu);
    break;
  case 11: // speed
    ChangeToTaskState(TASK_CombatSpeedMenu);
    break;
  case 12: // win
    ChangeToTaskState(TASK_CombatForceWinMenu);    
    break;
  case 13: // end
    ChangeToTaskState(TASK_CombatEndTurnMenu); //(TASK_CombatEndingTurn);
    break;
  default:
    WriteDebugString("Unhandled state in handleCombatMenu()\n");
    miscError = InvalidMenuState;
    break;
  }
}

void COMBAT_EVENT_DATA::ChangeToTaskState(TASKSTATE state)
{
  if (state == GetMyState())
    return;

  SetMyState(state);
  combatData.CombatActive=TRUE;

  switch (state)
  {
  case TASK_CombatStartBgMusic:
    if (   (pSndMgr != NULL) 
        && (PlayEventSounds) 
        && (bgSounds.backgroundSounds.sounds.GetCount()>0)
        && (pSndMgr->GetMusicEnable()))
    {    
      POSITION pos = bgSounds.backgroundSounds.sounds.GetHeadPosition();
      while (pos != NULL)
        pSndMgr->QueueBgndSound(bgSounds.backgroundSounds.sounds.GetNext(pos), TRUE);
      PushEvent(new DO_NOTHING_EVENT, DeleteEvent);
    }  
    break;
  case TASK_CombatMainMenu:
    menu.setMenu(CombatMenuData);
    menu.setHorzOrient();    
    menu.setCurrentItem(1);
    //combatData.PlaceCursorOnCurrentDude();
    combatData.SetActiveCastingCharacter(NO_DUDE);
    break;
  case TASK_CombatAttacking:
    combatData.StartAttack();
    PushEvent(new DO_NOTHING_EVENT, DeleteEvent);
    break;
  case TASK_CombatViewCharMenu:
    PushEvent(new VIEW_CHARACTER_DATA(GetEventType(),this), DeleteEvent);
    break;
  case TASK_CombatQuickMenu:
    combatData.Quick(TRUE);
    PushEvent(new DO_NOTHING_EVENT, DeleteEvent);
    break;
  case TASK_CombatSpeedMenu:
    PushEvent(new GAME_SPEED_MENU_DATA(this), DeleteEvent);
    break;
  case TASK_CombatAimMenu: // non-spell target selection
    PushEvent(new COMBAT_AIM_MENU_DATA(), DeleteEvent);
    break;

  case TASK_CombatCastMenu: 
    combatData.SetActiveCastingCharacter(combatData.GetCurrCombatant());
    PushEvent(new CAST_MENU_DATA(this), DeleteEvent);
    break;
  case TASK_CombatBeginCasting:
    {
      // spell is selected, now enter casting state
      COMBATANT &dude = combatData.getActiveCombatCharacter();
      combatData.SetActiveCastingCharacter(dude.self);
      int key = dude.GetSpellBookKeyBeingCast();
      combatData.StartInitialSpellCasting(key);       
      PushEvent(new TIME_DELAY_MSG_DATA(GetMsgTimeDelay(), combatData.GetCombatantState()), DeleteEvent);    
    }
    break;
  case TASK_CombatSelectSpellTargets:
    {
      COMBATANT *dude = combatData.getActiveCastingCharacter();
      if (dude!=NULL)
      {
        PushEvent(new CAST_COMBAT_SPELL_MENU_DATA(GetEventType(),
                                                  this,
                                                  dude, 
                                                  dude->GetSpellBookKeyBeingCast()), 
                  DeleteEvent);
      }
      else
        ChangeToTaskState(TASK_CombatActivateSpell);
    }
    break;
        
  case TASK_CombatActivateSpellMsg:
    {
      COMBATANT &dude = combatData.getActiveCombatCharacter();
      combatData.SetActiveCastingCharacter(dude.self);
      PushEvent(new TIME_DELAY_MSG_DATA(GetMsgTimeDelay(), CST_ActivateSpell), DeleteEvent);
    }
    break;

  case TASK_CombatActivateSpell:
    {
      //COMBATANT &dude = combatData.getActiveCombatCharacter();
      COMBATANT *dude = combatData.getActiveCastingCharacter();
      BOOL MoreTargets = FALSE;

      if (dude != NULL)
      {
        // caster may have killed himself with spell!
        if (dude->charOnCombatMap())
        {
          if (dude->targets.m_SelectingUnits)
            MoreTargets = (dude->targets.NumTargets()>0);
          else
            MoreTargets = ((dude->targets.m_MapTargetX>=0)&&(dude->targets.m_MapTargetY>=0));

          if (MoreTargets)
          {        
            ChangeToTaskState(TASK_CombatSpellMissileAnimate);
          }
          else
          {
            dude->activeSpellKey=-1;
            PushEvent(new DO_NOTHING_EVENT, DeleteEvent);
          }
        }
        else
        {
          dude->activeSpellKey=-1;
          PushEvent(new DO_NOTHING_EVENT, DeleteEvent);
        }
      }
      else
        PushEvent(new DO_NOTHING_EVENT, DeleteEvent);
    }
    break;

  case TASK_CombatSpellCastAnimate:
    {
      COMBATANT *dude = combatData.getActiveCastingCharacter();
      
      if (dude!=NULL)
      {
        // go ahead with spell animations for each target
        dude->forceAttackPose=TRUE;
        if (dude->targets.m_SelectingUnits)
        {
          if (dude->targets.NumTargets()==0)
          {
            // spell aborted, no targets chosen
            ChangeToTaskState(TASK_CombatActivateSpell);
          }
          else
          {
            int targ = dude->targets.m_targets[0];
            PushEvent(new COMBAT_SPELL_SPRITE_ANIMATE_DATA(dude->self, targ, COMBAT_SPELL_SPRITE_ANIMATE_DATA::CAST,""), DeleteEvent);          
          }
        }
        else // targeting a map location
        {
          if ((dude->targets.m_MapTargetX<0)||(dude->targets.m_MapTargetY<0))
          {
            // spell aborted, no targets chosen
            ChangeToTaskState(TASK_CombatActivateSpell);
          }
          else
          {
            PushEvent(new COMBAT_SPELL_SPRITE_ANIMATE_DATA(dude->self, dude->targets.m_MapTargetX, dude->targets.m_MapTargetY, COMBAT_SPELL_SPRITE_ANIMATE_DATA::CAST,""), DeleteEvent);
          }
        }
      }
      else
        ChangeToTaskState(TASK_CombatSpellFinalStage);
    }
    break;

  case TASK_CombatSpellMissileAnimate: 
    {
      COMBATANT *dude = combatData.getActiveCastingCharacter();
      if (dude != NULL)
      {
        dude->forceAttackPose=TRUE; // turn on attack pose while casting
        if (dude->targets.m_SelectingUnits)
        {
          int targ = dude->targets.m_targets[0];
          PushEvent(new COMBAT_SPELL_SPRITE_ANIMATE_DATA(dude->self, targ, COMBAT_SPELL_SPRITE_ANIMATE_DATA::MISSILE,""), DeleteEvent);          
        }
        else // targeting a map location
        {
          PushEvent(new COMBAT_SPELL_SPRITE_ANIMATE_DATA(dude->self, dude->targets.m_MapTargetX, dude->targets.m_MapTargetY, COMBAT_SPELL_SPRITE_ANIMATE_DATA::MISSILE,""), DeleteEvent);
        }
      }
      else
        ChangeToTaskState(TASK_CombatSpellFinalStage);
    }
    break;

  case TASK_CombatSpellCoverageAnimate:
    {      
      COMBATANT *dude = combatData.getActiveCastingCharacter();
      if (dude!=NULL)
      {
        dude->forceAttackPose=FALSE; // turn off attack pose
        if (dude->targets.m_SelectingUnits)
        {
          int targ = dude->targets.m_targets[0];
          PushEvent(new COMBAT_SPELL_SPRITE_ANIMATE_DATA(dude->self, targ, COMBAT_SPELL_SPRITE_ANIMATE_DATA::COVERAGE,""), DeleteEvent);          
        }
        else // targeting a map location
        {
          PushEvent(new COMBAT_SPELL_SPRITE_ANIMATE_DATA(dude->self, dude->targets.m_MapTargetX, dude->targets.m_MapTargetY, COMBAT_SPELL_SPRITE_ANIMATE_DATA::COVERAGE,""), DeleteEvent);
        }
      }
      else
        ChangeToTaskState(TASK_CombatSpellFinalStage);
    }
    break;
    
  case TASK_CombatSpellLingerAnimate:
    {
      //COMBATANT &dude = combatData.getActiveCombatCharacter();
      COMBATANT *dude = combatData.getActiveCastingCharacter();
      if (dude!=NULL)
      {
        if (dude->targets.m_SelectingUnits)
        {
          int targ = dude->targets.m_targets[0];
          PushEvent(new COMBAT_SPELL_SPRITE_ANIMATE_DATA(dude->self, targ, COMBAT_SPELL_SPRITE_ANIMATE_DATA::LINGER,""), DeleteEvent);          
        }
        else // targeting a map location
        {
          PushEvent(new COMBAT_SPELL_SPRITE_ANIMATE_DATA(dude->self, dude->targets.m_MapTargetX, dude->targets.m_MapTargetY, COMBAT_SPELL_SPRITE_ANIMATE_DATA::LINGER,""), DeleteEvent);
        }
      }
      else
        ChangeToTaskState(TASK_CombatSpellFinalStage);
    }
    break;

  case TASK_CombatSpellHitAnimate:
    {
      COMBATANT *dude = combatData.getActiveCastingCharacter();
      if (dude!=NULL)
      {
        if (dude->charOnCombatMap())
        {
          if (dude->targets.m_targets.GetSize() > 0)
          {
            CString msg = FormatSpellHitText();
            int targ = dude->targets.m_targets[0];
            PushEvent(new COMBAT_SPELL_SPRITE_ANIMATE_DATA(dude->self, targ, COMBAT_SPELL_SPRITE_ANIMATE_DATA::HIT,msg), DeleteEvent);          
          }
          else
            ChangeToTaskState(TASK_CombatSpellFinalStage);
        }
        else
          ChangeToTaskState(TASK_CombatSpellFinalStage);
      }
      else
        ChangeToTaskState(TASK_CombatSpellFinalStage);
    }
    break;

  case TASK_CombatSpellFinalStage:
    PushEvent(new DO_NOTHING_EVENT, DeleteEvent);
    break;

  //case TASK_CombatUseMenu:
    // PushEvent(new ITEMS_MENU_DATA(GetEventType(),this),DeleteEvent);
    // break;
  //case TASK_CombatUsing:
    //  PushEvent(new DO_NOTHING_EVENT, DeleteEvent);
    //  break;

  case TASK_CombatMoveMenu:
    PushEvent(new COMBAT_MOVE_MENU_DATA(this), DeleteEvent);
    break;   
  case TASK_CombatMoving:
    menu.setMenu(EmptyMenuData);
    menu.setTitle(FormatCombatMoveText());
    break;

  case TASK_CombatTurnMenu: 
    combatData.StartTurnUndead(); 
    PushEvent(new TIME_DELAY_MSG_DATA(GetMsgTimeDelay(), combatData.GetCombatantState()), DeleteEvent);
    break;
  case TASK_CombatTurning:  
    combatData.TurnUndead();    
    PushEvent(new DO_NOTHING_EVENT, DeleteEvent);
    break;

  case TASK_CombatGuardMenu: 
    combatData.Guard();
    PushEvent(new TIME_DELAY_MSG_DATA(GetMsgTimeDelay(), combatData.GetCombatantState()), DeleteEvent);
    break;
  case TASK_CombatGuarding:    
    PushEvent(new DO_NOTHING_EVENT, DeleteEvent);
    break;

  case TASK_CombatBandageMenu:
    combatData.Bandage();
    PushEvent(new TIME_DELAY_MSG_DATA(GetMsgTimeDelay(), combatData.GetCombatantState()), DeleteEvent);
    break;
  case TASK_CombatBandaging:   
    combatData.EndTurn();
    PushEvent(new DO_NOTHING_EVENT, DeleteEvent);
    break;

  case TASK_CombatDelayMenu:     
    PushEvent(new TIME_DELAY_MSG_DATA(GetMsgTimeDelay(), CST_Delaying), DeleteEvent);
    break;
  case TASK_CombatDelaying:  
    combatData.DelayAction();
    PushEvent(new DO_NOTHING_EVENT, DeleteEvent);
    break;

  case TASK_CombatEndTurnMenu: 
    // leave the End Turn delay at the default game speed of
    // 3 * 210 so that the last round of combat where each character
    // gets an extra turn doesn't take so long to cycle through.
    PushEvent(new TIME_DELAY_MSG_DATA(3*210, CST_EndingTurn), DeleteEvent);
    break;
  case TASK_CombatEndingTurn:
    combatData.EndTurn();
    PushEvent(new DO_NOTHING_EVENT, DeleteEvent);
    break;

  case TASK_CombatForceWinMenu:
    PushEvent(new TIME_DELAY_MSG_DATA(GetMsgTimeDelay(), CST_ForcingWin), DeleteEvent);
    break;
  case TASK_CombatForcingWin:
    MouseRender.ShowMouseCursor(FALSE);
    combatData.ForcePartyVictory();
    combatData.EndTurn();
    MouseRender.ShowMouseCursor(TRUE);
    PushEvent(new DO_NOTHING_EVENT, DeleteEvent);
    break;

  case TASK_CombatInRouteSpriteAnimating:
    combatData.InitMissileAnimation();
    PushEvent(new COMBAT_SPRITE_ANIMATE_DATA(), DeleteEvent);
    break;

  case TASK_CombatHitSpriteAnimating:
    combatData.InitHitAnimation();
    PushEvent(new COMBAT_SPRITE_ANIMATE_DATA(), DeleteEvent);
    break;

  case TASK_CombatTimeDelayMsg:
    PushEvent(new TIME_DELAY_MSG_DATA(GetMsgTimeDelay(), combatData.GetCombatantState()), DeleteEvent);
    break;

  case TASK_CombatRoundDelay:
    PushEvent(new COMBAT_ROUND_DELAY_DATA(600), DeleteEvent);
    break;

  case TASK_CombatOver:
    PushEvent(new COMBAT_RESULTS_MENU_DATA(this), DeleteEvent);
    break;

  default:
    ASSERT(FALSE);
    break;
  }
}

void COMBAT_EVENT_DATA::OnReturnToTopOfQueue(void)
{
  switch (GetMyState())
  {
  case TASK_CombatWelcome:
    ClearFormattedText(textData);
    currPic.Clear();
    if (LoadOffscreenCombatSurfaces(m_UseOutdoorMap))
      combatData.InitCombatData(this);
    else
      WriteDebugString("LoadOffscreenCombatSurfaces failed\n");
    //ChangeToTaskState(TASK_CombatMainMenu);    
    ChangeToTaskState(TASK_CombatStartBgMusic);
    break;

  case TASK_CombatStartBgMusic:
    pSndMgr->PlayBgndQueue();
    ChangeToTaskState(TASK_CombatMainMenu);
    break;

  case TASK_CombatMainMenu:
    // do nothing
    break;

  case TASK_CombatViewCharMenu: ChangeToTaskState(TASK_CombatMainMenu); break;
  case TASK_CombatQuickMenu: ChangeToTaskState(TASK_CombatMainMenu); break;
  case TASK_CombatSpeedMenu: ChangeToTaskState(TASK_CombatMainMenu); break;

  case TASK_CombatMoving: ChangeToTaskState(TASK_CombatMainMenu); break;
  case TASK_CombatMoveMenu:
    switch (combatData.GetCombatState())
    {
    case CST_Attacking:
      if (combatData.GetCurrTarget() != NO_DUDE)
      {
        ChangeToTaskState(TASK_CombatAttacking);
      }
      else
      {
        combatData.StopMoving();
        ChangeToTaskState(TASK_CombatMainMenu);
      }
      break;
    default:
      combatData.StopMoving();
      ChangeToTaskState(TASK_CombatMainMenu);
      break;
    }
    break;

  case TASK_CombatAimMenu:
    if (combatData.GetCurrTarget() != NO_DUDE)
    {
      if (combatData.NeedMissileAnimation())
        ChangeToTaskState(TASK_CombatInRouteSpriteAnimating);
      else
        ChangeToTaskState(TASK_CombatAttacking);
    }
    else
      ChangeToTaskState(TASK_CombatMainMenu);
    break; 

  case TASK_CombatCastMenu:
    /// if spell chosen to cast, then begin casting process
    if (combatData.getActiveCombatCharacter().GetSpellBookKeyBeingCast()==NO_SPELL)
      ChangeToTaskState(TASK_CombatMainMenu); 
    else
      ChangeToTaskState(TASK_CombatBeginCasting); 
    break;
  case TASK_CombatBeginCasting:
    {
      //COMBATANT &dude = combatData.getActiveCombatCharacter();
      COMBATANT *dude = combatData.getActiveCastingCharacter();
      if (dude != NULL)
      {
        if (dude->IsSpellPending())
        {
          combatData.EndInitialSpellCasting();
          ChangeToTaskState(TASK_CombatMainMenu);
        }
        else
          ChangeToTaskState(TASK_CombatActivateSpellMsg);
      }
      else
        ChangeToTaskState(TASK_CombatActivateSpell);
    }
    break;    
  case TASK_CombatSelectSpellTargets:
    ChangeToTaskState(TASK_CombatSpellCastAnimate); 
    break;

  case TASK_CombatActivateSpellMsg:
    {
      //COMBATANT &dude = combatData.getActiveCombatCharacter();
      COMBATANT *dude = combatData.getActiveCastingCharacter();
      if (dude!=NULL)
      {
        if (dude->targets.NumTargets()==0)
          ChangeToTaskState(TASK_CombatSelectSpellTargets);
        else // there could be preselected targets if on automatic
          ChangeToTaskState(TASK_CombatSpellCastAnimate);
      }
      else
        ChangeToTaskState(TASK_CombatActivateSpell);
    }
    break;

  case TASK_CombatActivateSpell:
    combatData.StopSpellCasting();
    ChangeToTaskState(TASK_CombatMainMenu); 
    break;

  case TASK_CombatAttacking: 
    {
      if (combatData.NeedHitAnimation())
        ChangeToTaskState(TASK_CombatHitSpriteAnimating);
      else
        ChangeToTaskState(TASK_CombatTimeDelayMsg);
      break;
    }
  
  //case TASK_CombatUseMenu: ChangeToTaskState(TASK_CombatUsing); break;      
  //case TASK_CombatUsing: ChangeToTaskState(TASK_CombatMainMenu); break;      

  case TASK_CombatTurnMenu: ChangeToTaskState(TASK_CombatTurning); break;
  case TASK_CombatTurning: ChangeToTaskState(TASK_CombatMainMenu); break;
 
  case TASK_CombatBandageMenu: ChangeToTaskState(TASK_CombatBandaging); break;
  case TASK_CombatBandaging: ChangeToTaskState(TASK_CombatMainMenu); break;

  case TASK_CombatGuardMenu: ChangeToTaskState(TASK_CombatGuarding); break;
  case TASK_CombatGuarding: ChangeToTaskState(TASK_CombatMainMenu); break;
  
  case TASK_CombatDelayMenu: ChangeToTaskState(TASK_CombatDelaying);break;
  case TASK_CombatDelaying: ChangeToTaskState(TASK_CombatMainMenu); break;  

  case TASK_CombatEndTurnMenu: ChangeToTaskState(TASK_CombatEndingTurn); break;  
  case TASK_CombatEndingTurn: ChangeToTaskState(TASK_CombatMainMenu); break;

  case TASK_CombatForceWinMenu: ChangeToTaskState(TASK_CombatForcingWin); break;  
  case TASK_CombatForcingWin: ChangeToTaskState(TASK_CombatMainMenu); break;

  case TASK_CombatInRouteSpriteAnimating: ChangeToTaskState(TASK_CombatAttacking); break;
  case TASK_CombatHitSpriteAnimating: ChangeToTaskState(TASK_CombatTimeDelayMsg); break;

  case TASK_CombatSpellCastAnimate: ChangeToTaskState(TASK_CombatActivateSpell); break;

  case TASK_CombatSpellMissileAnimate: ChangeToTaskState(TASK_CombatSpellCoverageAnimate); break;
  case TASK_CombatSpellCoverageAnimate:ChangeToTaskState(TASK_CombatSpellLingerAnimate); break;
  case TASK_CombatSpellLingerAnimate: ChangeToTaskState(TASK_CombatSpellHitAnimate); break;
  case TASK_CombatSpellHitAnimate: ChangeToTaskState(TASK_CombatSpellFinalStage); break; 

  case TASK_CombatSpellFinalStage:
    { 
      COMBATANT *dude = combatData.getActiveCastingCharacter();
      if (dude!=NULL)
      {
        if (dude->targets.NumTargets()>0) // targeting self may halt spell casting and  remove targets
        {
          dude->activeSpellKey = 
            combatData.InvokePendingSpellOnCombatant(dude->self, 
                                                     dude->targets.m_targets[0], 
                                                     dude->activeSpellKey);
        
          dude->targets.m_targets.RemoveAt(0);
        }

        if (!dude->targets.m_SelectingUnits)
        {
          // ****** need to do this before linger animation so that active
          // ****** spell key is available.          
          //
          //if (dude->targets.m_MapTargetX != -1)
          //{
          //  dude->activeSpellKey = 
          //    combatData.InvokePendingSpellOnMap(dude->self, dude->activeSpellKey);          
          //}

          dude->targets.m_MapTargetX=dude->targets.m_MapTargetY=-1;
        }        
        // if targeting a map x,y, the missile/coverage animation only plays once
        if ((dude->targets.m_SelectingUnits)||(dude->targets.NumTargets()==0))
          ChangeToTaskState(TASK_CombatActivateSpell);
        else
          ChangeToTaskState(TASK_CombatSpellHitAnimate);// hit animate needs unit targets
      }
      else
        ChangeToTaskState(TASK_CombatActivateSpell);
    }
    break;

  case TASK_CombatTimeDelayMsg: ChangeToTaskState(TASK_CombatMainMenu); break;
  case TASK_CombatRoundDelay: ChangeToTaskState(TASK_CombatMainMenu); break;

  case TASK_CombatOver: ChainHappened(); break; 
  
  
  default: ChangeToTaskState(TASK_CombatMainMenu); break;
  }  
}        

bool COMBAT_EVENT_DATA::OnIdle(void)
{
  static LONGLONG lastTime = 0;
  LONGLONG currTime = virtualGameTime;//timeGetTime();
  if (currTime < lastTime) currTime = lastTime; // rollover
  else if (currTime==lastTime) return true;

  if (combatData.OnAuto())
  {
    // need to throttle automatic dudes        
    DWORD interval = GetUserGameSpeed()*120;
    if (interval < 210) interval = 210;
    if (interval > 990) interval = 990;

    if ((currTime-lastTime) < interval)
      return true;
  }
  else if (combatData.GetCombatState()==CST_None) 
  {
    ChangeToTaskState(TASK_CombatMainMenu);
    return true;
  }

  combatData.UpdateCombat();  
  // for auto-combatants, the above can
  // return one of these states after Think()
  //
  // CS_None
  // CS_Moving
  // CS_Guarding
  // CS_Attacking
  //

  switch (combatData.GetCombatState())
  {
  case CST_None: // auto
    break;

  case CST_Attacking: // auto
    if ((GetMyState() != TASK_CombatInRouteSpriteAnimating) && (combatData.NeedMissileAnimation()))
      ChangeToTaskState(TASK_CombatInRouteSpriteAnimating);
    else
      ChangeToTaskState(TASK_CombatAttacking);
    break;

  case CST_Guarding: // auto
    ChangeToTaskState(TASK_CombatGuardMenu);
    break;

  case CST_Fleeing:
  case CST_Moving: // auto
    ChangeToTaskState(TASK_CombatMoving);
    break;

  case CST_CombatOver:
    ChangeToTaskState(TASK_CombatOver);      
    break;

  case CST_StartNewRound:
    TRACE("Start new round delay\n");
  case CST_CombatRoundDelay:
    TRACE("Round delay\n");
    ChangeToTaskState(TASK_CombatRoundDelay);
    break;

  case CST_Casting:
    {
      COMBATANT &dude = combatData.getActiveCombatCharacter();
      int key = dude.GetSpellBookKeyBeingCast();
      if (key == NO_SPELL)
      {
        combatData.EndTurn();
        ChangeToTaskState(TASK_CombatMainMenu);         
      }
      else
      {   
        ChangeToTaskState(TASK_CombatActivateSpellMsg);
      }
    }
    break;

  case CST_Bandaging:
    ChangeToTaskState(TASK_CombatBandageMenu);
    break;

  case CST_Using:
    //ChangeToTaskState(TASK_CombatUsing);
    break;

  case CST_Turning:
    ChangeToTaskState(TASK_CombatTurnMenu);
    break;

  case CST_Delaying:
    ChangeToTaskState(TASK_CombatDelayMenu);
    break;

  case CST_EndingTurn:
    ChangeToTaskState(TASK_CombatEndTurnMenu);
    break;

  default:
    ASSERT(FALSE);
    break;
  }

  // moves dude, or can end turn
  combatData.HandleCurrState();
  OnUpdateUI();
  OnDraw();
  lastTime=currTime;//timeGetTime();
  return true;
}

void COMBAT_EVENT_DATA::OnDraw(void)
{
  UpdateCombatMainMenuScreen();
}

bool COMBAT_EVENT_DATA::OnCycle(void)
{
  bool result = false;
  //GameEvent::OnCycle();
  LONGLONG currTime = virtualGameTime;//timeGetTime();

  // update drunk points, spell effects, etc
  combatData.ProcessTimeSensitiveData(currTime);
    
  if (UpdateAnimateList())
    result = true;
  
  if (errorTextData.numLines > 0) 
  {
    long diff = currTime - errorTextData.startTime;
    if ((diff > 2000) || (diff < 0))
    {
      ClearFormattedText(errorTextData);
    }
  }

  //OnUpdateUI();
  // auto combatants never leave this event, but
  // updates to the screen still need to be made
  //return true;
  return result;
}

void COMBAT_EVENT_DATA::OnLeaveQueue(void)
{
  combatData.CombatActive = FALSE;
  //combatData.RestoreToParty();
  combatData.Clear();
  StartBackgroundMusic();
}

void COMBAT_EVENT_DATA::OnUpdateUI(void)
{  
  // if in quick mode, no menus are displayed
  // if already in action, no menus are displayed
  // if no dude is active, no menus are displayed
  CombatStateType state = combatData.GetCombatState();
  int dude = combatData.GetCurrCombatant();

  if (dude == NO_DUDE)
  {
    menu.setAllItemsInactive();
    return;
  }

  if (state != CST_None)
  {
    if ((state == CST_Moving)&&(combatData.OnAuto()))
    {
      menu.setTitle(FormatCombatMoveText());
      return;
    }

    menu.setAllItemsInactive();
    return;
  }
  else if (combatData.OnAuto())
  {
    menu.setAllItemsInactive();
    return;
  }


/*
   1  { "MOVE", 0 },
   2  { "AIM", 0 },
   3  { "USE", 0 },
   4  { "CAST", 0 },
   5  { "TURN", 0 },
   6  { "GUARD", 0 },
   7  { "QUICK", 0 },
   8  { "DELAY", 0 },
   9  { "BANDAGE", 0 },
   10 { "VIEW", 0 },
   11 { "SPEED", 0 },
   12 { "WIN", 0 },
   13 { "END", 0 }
*/

  menu.setAllItemsActive();
  if (!combatData.CanMove())
    menu.setItemInactive(1); // move

  if (!levelData.zoneData.zones[levelData.area[party.Posy][party.Posx].zone].AllowMagic)
    menu.setItemInactive(4); // cast

  if (!combatData.CanCast())
    menu.setItemInactive(4); // cast

  if (!combatData.CanTurnUndead())
    menu.setItemInactive(5); // turn

  if (!combatData.CanGuard())
    menu.setItemInactive(6); // guard    

  if (!combatData.CanDelay())
    menu.setItemInactive(8); // delay      

  if (!combatData.CanBandage())
    menu.setItemInactive(9); // Bandage   

#ifndef _DEBUG
  if (!EditorRunning)
    menu.setItemInactive(12); // Win  
#endif

}

unsigned int COMBAT_EVENT_DATA::OnTaskMessage(TASKMESSAGE msg)
{
  TEMPsurprise = surprise;
  switch (msg)
  {
  case TASKMSG_Combat_MonstersSurpised:
    TEMPsurprise = MonsterSurprised;
    return TASKMSG_STOP;
  case TASKMSG_Combat_PartySurpised:
    TEMPsurprise = PartySurprised;
    return TASKMSG_STOP;
  case TASKMSG_Combat_NoSurpise:
    TEMPsurprise = Neither;
    return TASKMSG_STOP;
  default:
    break;
  }
  return 0;
}

void COMBAT_EVENT_DATA::OnMouseClickLeft(int mx, int my)
{
  MouseMenu(mx, my);
}

////////////////////////////////////////////////////////////////////////COMBAT_RESULTS_MENU_DATA
void COMBAT_RESULTS_MENU_DATA::OnKeypress(key_code key, char ascii)
{
  if (TABParty(key)) return;
  if (key != KC_RETURN)
  {
    HMenuVPartyKeyboardAction(key,ascii);
    return;
  }

  ClearFormattedText(textData);

  //if (ItemsOnPage > 0)
  //  PushEvent(new TAKE_TREASURE_DATA(m_pOrigEvent), DeleteEvent);
  if (m_pTreasEvent != NULL)
    PushEvent(m_pTreasEvent, DeleteEvent);
  else
    PopEvent();
}

void COMBAT_RESULTS_MENU_DATA::OnInitialEvent(void)
{
  ActiveSpellData.RemoveLingerSpells();

  if (!LoadOffscreenWalkingSurfaces())
    WriteDebugString("LoadOffscreenWalkingSurfaces failed\n");  

  menu.setMenu(CombatResultsMenuData);
  menu.setHorzOrient();

  currPic.Clear();
  ClearFormattedText(textData);
  clearTreasureItemState();
  
  exptotal = combatData.expPointTotal;
  combatData.expPointTotal = 0;

  combatData.RestoreToParty();
  
  if (exptotal > 0)
    party.distributeExpPoints(exptotal);

  CString tempText;
  switch (combatData.combatVictor)
  {
  case MonsterWins:
    if (combatData.PartyNeverDies)
    {
      globalData.global_asl.Insert("Combat Result", "LoseButNeverDies", ASLF_MODIFIED);
      if (party.numCharacters == 1)
        tempText = "YOU HAVE SURVIVED!";
      else
        tempText = "THE PARTY HAS SURVIVED!";
      FormatDisplayText(textData, tempText);
    }
    else
    {
      globalData.global_asl.Insert("Combat Result", "Lose", ASLF_MODIFIED);
      //combatData.RestoreToParty();
      bool allHaveDied = true;
      for (int i=0;i<party.numCharacters;i++)
      {
        if (party.characters[i].GetStatus() == Okay)
          allHaveDied = false;          
      }

      if (allHaveDied)
      {
        ReplaceEvent(new PARTY_DIED_MENU_DATA, DeleteEvent);
        return;
      }
      else
      {
        if (party.numCharacters == 1)
          FormatDisplayText(textData, "YOU HAVE LOST");
        else
          FormatDisplayText(textData, "THE PARTY HAS LOST");
      }      
    }
    break;   
  
  case PartyWins:
    globalData.global_asl.Insert("Combat Result", "Win", ASLF_MODIFIED);
    if (party.numCharacters == 1)
      tempText = "YOU HAVE WON!";
    else
      tempText = "THE PARTY HAS WON!";
    if (exptotal > 0)
    {
      CString tmp;
      if (party.numCharacters == 1)
        tmp.Format("\nYOU RECEIVE %i EXPERIENCE POINTS", 
                   exptotal);
      else
        tmp.Format("\nTHE PARTY RECEIVES %i EXPERIENCE POINTS", 
                   exptotal);
      tempText += tmp;

      // don't do this here
      //party.distributeExpPoints(combatData.expPointTotal);
      //exptotal = combatData.expPointTotal;
      //combatData.expPointTotal = 0;
    }           

    if (!((COMBAT_EVENT_DATA*)(m_pOrigEvent))->noMonsterTreasure)
    {
      //setTreasureEventState(m_pOrigEvent);
      m_pTreasEvent = new GIVE_TREASURE_DATA();

	    for (int i=0;i<combatData.numCombatants;i++)
	    {
        if ((!combatData.combatants[i].IsPartyMember()) && (combatData.combatants[i].GetAdjStatus() == Dead))
        {

          POSITION pos = combatData.combatants[i].myItems.GetHeadPosition();
          while (pos != NULL)
            m_pTreasEvent->items.AddItem(combatData.combatants[i].myItems.GetNext(pos));

          m_pTreasEvent->money.Add(combatData.combatants[i].money);
        }
      }

      //if (ItemsOnPage > 0)
      if ((m_pTreasEvent->items.GetHeadPosition() != NULL) || (m_pTreasEvent->money.Total() > 0.0))
      {
        CString tmp;
        if (party.numCharacters == 1)
          tmp += "\nYOU HAVE FOUND TREASURE!";
        else
          tmp += "\nTHE PARTY HAS FOUND TREASURE!";
        tempText += tmp;
      }
      else
      {
        delete m_pTreasEvent;
        m_pTreasEvent = NULL;
      }
    }
    FormatDisplayText(textData, tempText);
    break;

  case PartyRanAway:
    globalData.global_asl.Insert("Combat Result", "Flee", ASLF_MODIFIED);
    if (party.numCharacters == 1)
      FormatDisplayText(textData, "YOU HAVE RUN AWAY");
    else
      FormatDisplayText(textData, "THE PARTY HAS RUN AWAY");
    break;

  default: // likely to be an idle timeout
    FormatDisplayText(textData, "THIS COMBAT HAS NO WINNER");
    break;
  }     
}

void COMBAT_RESULTS_MENU_DATA::OnDraw(void)
{
  UpdateCombatResultsScreen();
}

void COMBAT_RESULTS_MENU_DATA::OnReturnToTopOfQueue(void)
{
  // returning from TAKE_ITEMS_EVENT
  PopEvent();
}

void COMBAT_RESULTS_MENU_DATA::OnLeaveQueue(void)
{
  //combatData.RestoreToParty();
  //if (exptotal > 0)
  //  party.distributeExpPoints(exptotal);
}

////////////////////////////////////////////////////////////////////////COMBAT_AIM_MENU_DATA
void COMBAT_AIM_MENU_DATA::OnInitialEvent(void)
{
  //ClearFormattedText(textData);
  combatData.RemoveCurrTarget();
  combatData.ResetAim();
  menu.setMenu(AimMenuData);
  menu.setHorzOrient();
  menu.MapKeyCodeToMenuItem(KC_ESCAPE, 6);
}
void COMBAT_AIM_MENU_DATA::OnKeypress(key_code key, char ascii)
{
  if (key != KC_RETURN)
  {
    StandardMenuKeyboardAction(key, ascii);
    return;
  }

  switch (menu.currentItem()) 
  {
  case 1: // next
    combatData.GetNextAim();
    break;
  case 2: // prev
    combatData.GetPrevAim();
    break;
  case 3: // manual
    ReplaceEvent(new COMBAT_AIM_MANUAL_MENU_DATA(), DeleteEvent);
    break;
  case 4: // target    
    if (combatData.CanAttack(combatData.GetCurrAim()))
    {
      combatData.AddTarget(combatData.GetCurrAim());
      //combatData.StartAttack();
      PopEvent();
      return;
    }
    else
      combatData.RemoveCurrTarget();
    break;
  case 5: // center
    EnsureVisibleCursor(TRUE);
    break;
  case 6: // exit, no target chosen
    //combatData.PlaceCursorOnDude();
    combatData.RemoveCurrTarget();
    PopEvent();
    break;
  default:
    WriteDebugString("Unhandled state in COMBAT_AIM_MENU_DATA::OnKeyPress()\n");
    miscError = InvalidMenuState;
    break;
  }
}

void COMBAT_AIM_MENU_DATA::OnDraw(void)
{
  UpdateCombatAimScreen();
}

void COMBAT_AIM_MENU_DATA::OnUpdateUI(void)
{
  menu.setAllItemsActive();
  if (!combatData.CanTarget())
  {
    menu.setItemInactive(4); // target
  }
  else
  {
    int target = getCombatantInCell(combatData.cursorX, combatData.cursorY, 1,1);
    if ((target != NO_DUDE) && (target < combatData.combatants.GetSize()))
    {
      if (!combatData.CanAttack(target))
        menu.setItemInactive(4); // target
    }
    else
      menu.setItemInactive(4); // target
  }
}

void COMBAT_AIM_MENU_DATA::OnMouseClickLeft(int mx, int my)
{
  MouseMenu(mx, my);
}

////////////////////////////////////////////////////////////////////////COMBAT_AIM_MANUAL_MENU_DATA
void COMBAT_AIM_MANUAL_MENU_DATA::OnInitialEvent(void)
{
  //ClearFormattedText(textData);
  combatData.RemoveCurrTarget();
  combatData.ResetAim();
  menu.setMenu(ExitMenuData);
  menu.setHorzOrient();
  menu.MapKeyCodeToMenuItem(KC_ESCAPE, 1);
}

void COMBAT_AIM_MANUAL_MENU_DATA::OnKeypress(key_code key, char ascii)
{
  switch (key)
  {
  case KC_RETURN: // exit aim
  {
    int target = getCombatantInCell(combatData.cursorX, combatData.cursorY, 1,1);
    if ((target != NO_DUDE) && (target < combatData.combatants.GetSize()))
    {
      if (combatData.CanTarget())
      {
        if (combatData.CanAttack(target))
          combatData.AddTarget(target);
        else
          combatData.RemoveCurrTarget();
      }
      else
        combatData.RemoveCurrTarget();
    }
    else
      combatData.RemoveCurrTarget();
    PopEvent();
    break;
  }

  case KC_POSITION: // Numpad_0 or Insert
  {
    if (!ValidTerrainViewportCoords(combatData.moveToX,combatData.moveToY))
    {
      combatData.moveToX=-1;
      combatData.moveToY=-1;
      break;
    }

    int oldX = combatData.cursorX;
    int oldY = combatData.cursorY;
    combatData.cursorX = combatData.moveToX;
    combatData.cursorY = combatData.moveToY;
    ForceMapToMoveAlong(combatData.moveToX, combatData.moveToY, 
                        GetDir(oldX, oldY, combatData.moveToX, combatData.moveToY),
                        1);
    combatData.moveToX=-1;
    combatData.moveToY=-1;
    break;
  }
  case KC_CENTER: // Numpad_5
    combatData.PlaceCursorOnCurrentDude(TRUE);
    break;
  case KC_LEFT:
    combatData.moveCursorLeft();
    break;
  case KC_RIGHT:
    combatData.moveCursorRight();
    break;
  case KC_UP:
    combatData.moveCursorForward();
    break;
  case KC_DOWN:
    combatData.moveCursorBackward();
    break;
  case KC_NW:
    combatData.moveCursorLeft();
    combatData.moveCursorForward();
    break;
  case KC_NE:
    combatData.moveCursorRight();
    combatData.moveCursorForward();
    break;
  case KC_SE:
    combatData.moveCursorRight();
    combatData.moveCursorBackward();
    break;
  case KC_SW:
    combatData.moveCursorLeft();
    combatData.moveCursorBackward();
    break;
  default:
    StandardMenuKeyboardAction(key,ascii);
    break;
  }
}

void COMBAT_AIM_MANUAL_MENU_DATA::OnDraw(void)
{
  UpdateCombatAimManualScreen();
}

void COMBAT_AIM_MANUAL_MENU_DATA::OnUpdateUI(void)
{
  menu.setAllItemsActive();
}

void COMBAT_AIM_MANUAL_MENU_DATA::OnMouseClickLeft(int mx, int my)
{
  MouseCombatMapMenu(mx,my);
}

////////////////////////////////////////////////////////////////////////COMBAT_SPELL_AIM_MENU_DATA
void COMBAT_SPELL_AIM_MENU_DATA::OnInitialEvent(void)
{
  if (m_caster==NULL)
  {
    ASSERT(FALSE);
    PopEvent();
    return;
  }
  if (!m_caster->targets.ValidNumTargets())
  {
    ASSERT(FALSE);
    PopEvent();
    return;
  }
  combatData.ResetAim();
  menu.setMenu(AimMenuData);
  menu.setHorzOrient();
  CString title(m_caster->targets.FormatRemainingTargsText());
  menu.setTitle(title);
  menu.MapKeyCodeToMenuItem(KC_ESCAPE, 6);
  SetMyState(TASK_initialized);
}

void COMBAT_SPELL_AIM_MENU_DATA::OnKeypress(key_code key, char ascii)
{
  if (key != KC_RETURN)
  {
    StandardMenuKeyboardAction(key, ascii);
    return;
  }

  switch (menu.currentItem()) 
  {
  case 1: // next
    combatData.GetNextAim();
    break;
  case 2: // prev
    combatData.GetPrevAim();
    break;
  case 3: // manual
    PushEvent(new COMBAT_SPELL_AIM_MANUAL_MENU_DATA(m_caster), DeleteEvent);
    break;
  case 4: // target
    {    
      // automatic aiming always points to a valid target
      int target = getCombatantInCell(combatData.cursorX, combatData.cursorY, 1,1);
      if (target != NO_DUDE)
      {
        COMBATANT *pTarget = combatData.getCombatantPtr(target);
        ASSERT(pTarget!=NULL);
        if (m_caster->targets.m_SelectingUnits)
        {
          // This version of 'distance' accounts for icons that are
          // larger than 1x1. The x,y is always the upper,left. The
          // distance is computed based on the edges of the icons that
          // are closest to each other.
          int targrange = distance(m_caster->self, m_caster->x,m_caster->y, 
                                   pTarget->self, pTarget->x,pTarget->y);
          if (!m_caster->AddTarget(*pTarget, targrange))
            WriteDebugString("Failed AddTarget() in COMBAT_SPELL_AIM_MENU_DATA\n");
        }
        else
        {
          PATH_DIR dir = GetSpellTargetingDir(m_caster->GetCenterX(), m_caster->GetCenterY(), 
                                              pTarget->GetCenterX(),pTarget->GetCenterY());
          if (!m_caster->AddMapTarget(pTarget->GetCenterX(), pTarget->GetCenterY(), dir))
            WriteDebugString("Failed AddTarget() in COMBAT_SPELL_AIM_MENU_DATA\n");
        }

        if (m_caster->targets.AllTargetsChosen())
        {
          PopEvent(); // all targets chosen
          return;
        }
        else
        {
          CString title(m_caster->targets.FormatRemainingTargsText());
          menu.setTitle(title);
          combatData.GetNextAim();
        }
      }
    }
    break;
  case 5: // center
    EnsureVisibleCursor(TRUE);
    break;
  case 6: // exit
    if (m_caster->targets.m_SelectingUnits)
    {
      if (m_caster->targets.NumTargets()==0)
      {
        SetMyState(TASK_CombatAskAbortSpell);
        PushEvent(new ASK_YES_NO_MENU_DATA("ABORT THIS SPELL?",TRUE, CONTROL_CombatSpellAimAuto), DeleteEvent);
        return;
      }
    }
    else
    {
      if ((m_caster->targets.m_MapTargetX<0)||(m_caster->targets.m_MapTargetY<0))
      {
        SetMyState(TASK_CombatAskAbortSpell);
        PushEvent(new ASK_YES_NO_MENU_DATA("ABORT THIS SPELL?",TRUE, CONTROL_CombatSpellAimAuto), DeleteEvent);
        return;
      }
    }
    PopEvent();
    break;
  default:
    WriteDebugString("Unhandled state in COMBAT_SPELL_AIM_MENU_DATA::OnKeyPress()\n");
    miscError = InvalidMenuState;
    break;
  }
}

void COMBAT_SPELL_AIM_MENU_DATA::OnReturnToTopOfQueue(void)
{
  if (GetMyState()==TASK_CombatAskAbortSpell)
  {
    if (party.tradeItem==1) // yes, abort current spell
    {
      PopEvent();
      return;
    }
    else
      OnInitialEvent();
  }
  else
  {
    if (m_caster->targets.AllTargetsChosen())
      PopEvent();
    else
      OnInitialEvent();
  }
}

void COMBAT_SPELL_AIM_MENU_DATA::OnDraw(void)
{
  UpdateCombatAimScreen();
}

void COMBAT_SPELL_AIM_MENU_DATA::OnUpdateUI(void)
{
  menu.setAllItemsActive();

  if (m_caster->targets.m_SelectingUnits)
  {
    int target = getCombatantInCell(combatData.cursorX, combatData.cursorY, 1,1);
    if (target != NO_DUDE)
    {
      COMBATANT *pTarget = combatData.getCombatantPtr(target);
      ASSERT(pTarget!=NULL);

      if (!HaveLineOfSight(m_caster->GetCenterX(), m_caster->GetCenterY(), 
                           pTarget->GetCenterX(), pTarget->GetCenterY()))
      {
        menu.setItemInactive(4); // target
      }
      
      if (!m_caster->targets.m_FriendlyFireOk)
      {
        if (pTarget->GetIsFriendly())
          menu.setItemInactive(4); // target
      }

      // these targeting types use the direction between caster and targeted
      // map square to determine which way to direct the spell
      //
      // the path will be PathBAD if the cursor is on the caster, which
      // gives an indeterminate direction from caster to targeted square.
      //      
      if ((m_caster->targets.m_type==AreaLinePickStart)||(m_caster->targets.m_type==AreaCone))
      {
        PATH_DIR dir = GetSpellTargetingDir(m_caster->GetCenterX(), m_caster->GetCenterY(), 
                                            pTarget->GetCenterX(),pTarget->GetCenterY());

        if (dir == PathBAD)
          menu.setItemInactive(4); // target
      }

      int targrange = distance(m_caster->self, m_caster->x,m_caster->y, 
                               pTarget->self, pTarget->x,pTarget->y);
      if (targrange > m_caster->targets.m_MaxRange)
        menu.setItemInactive(4); // target

      if (!m_caster->GetAdjDetectingInvisible())
      {
        if (pTarget->GetAdjSpecAb(SA_Invisible))
          menu.setItemInactive(4); // target
      
        if (m_caster->GetUndeadType() != NotUndead)
        {
          if (pTarget->GetAdjSpecAb(SA_InvisibleToUndead))
            menu.setItemInactive(4); // target
        }
      
        if (m_caster->IsAnimal())
        {
          if (pTarget->GetAdjSpecAb(SA_InvisibleToAnimals))
            menu.setItemInactive(4); // target
        }
      }
    }
    else
      menu.setItemInactive(4); // target
  }
  else
  {
    // these targeting types use the direction between caster and targeted
    // map square to determine which way to direct the spell
    //
    // the path will be PathBAD if the cursor is on the caster, which
    // gives an indeterminate direction from caster to targeted square.
    //
    if ((m_caster->targets.m_type==AreaLinePickStart)||(m_caster->targets.m_type==AreaCone))
    {
      PATH_DIR dir = GetSpellTargetingDir(m_caster->GetCenterX(), m_caster->GetCenterY(), 
                                          combatData.cursorX, combatData.cursorY);
      if (dir == PathBAD)
        menu.setItemInactive(4); // target
    }
  }
}

void COMBAT_SPELL_AIM_MENU_DATA::OnMouseClickLeft(int mx, int my)
{
  MouseMenu(mx, my);
}

////////////////////////////////////////////////////////////////////////COMBAT_SPELL_AIM_MANUAL_MENU_DATA
void COMBAT_SPELL_AIM_MANUAL_MENU_DATA::OnInitialEvent(void)
{
  combatData.ResetAim();
  menu.setMenu(AimMenuData);
  menu.setHorzOrient();
  CString title(m_caster->targets.FormatRemainingTargsText());
  menu.setTitle(title);
  menu.MapKeyCodeToMenuItem(KC_ESCAPE, 6);
}

void COMBAT_SPELL_AIM_MANUAL_MENU_DATA::OnKeypress(key_code key, char ascii)
{
  /*
   { "NEXT", 0 },
   { "PREV", 0 },
   { "MANUAL", 0 },
   { "TARGET", 0 },
   { "CENTER", 0 },
   { "EXIT", 1 }
   */
  switch (key)
  {
  case KC_RETURN:
  {
    switch (menu.currentItem()) 
    {
    case 1: // next disabled
      //combatData.GetNextAim();
      break;
    case 2: // prev disabled
      //combatData.GetPrevAim();
      break;
    case 3: // manual disabled
      //PushEvent(new COMBAT_SPELL_AIM_MANUAL_MENU_DATA(m_caster), DeleteEvent);
      break;
    case 4: // target
      {
        // manual aiming can point to map or character target
        if (m_caster->targets.m_SelectingUnits)
        {
          int target = getCombatantInCell(combatData.cursorX, combatData.cursorY, 1,1);
          if (target != NO_DUDE)
          {
            COMBATANT *pTarget = combatData.getCombatantPtr(target);
            ASSERT(pTarget!=NULL);

            // This version of 'distance' accounts for icons that are
            // larger than 1x1. The x,y is always the upper,left. The
            // distance is computed based on the edges of the icons that
            // are closest to each other.
            int targrange = distance(m_caster->self, m_caster->x,m_caster->y, 
                                     pTarget->self, pTarget->x,pTarget->y);
            if (!m_caster->AddTarget(*pTarget, targrange))
              WriteDebugString("Failed AddTarget() in COMBAT_SPELL_AIM_MENU_DATA\n");
          }
        }
        else
        {
          PATH_DIR dir = GetSpellTargetingDir(m_caster->GetCenterX(), m_caster->GetCenterY(), 
                                              combatData.cursorX, combatData.cursorY);
          if (!m_caster->AddMapTarget(combatData.cursorX, combatData.cursorY, dir))
            WriteDebugString("Failed AddTarget() in COMBAT_SPELL_AIM_MENU_DATA\n");
        }

        if (m_caster->targets.AllTargetsChosen())
        {
          PopEvent(); // all targets chosen
          return;
        }
        else
        {
          CString title(m_caster->targets.FormatRemainingTargsText());
          menu.setTitle(title);
        }
      }
      break;
    case 5: // center disabled
      //EnsureVisibleCursor(TRUE);
      break;
    case 6: // exit
      PopEvent();
      break;
    default:
      WriteDebugString("Unhandled state in COMBAT_SPELL_AIM_MANUAL_MENU_DATA::OnKeyPress()\n");
      miscError = InvalidMenuState;
      break;
    }
  }
  break;

  case KC_POSITION: // Numpad_0 or Insert
  {
    if (!ValidTerrainViewportCoords(combatData.moveToX,combatData.moveToY))
    {
      combatData.moveToX=-1;
      combatData.moveToY=-1;
      break;
    }

    int oldX = combatData.cursorX;
    int oldY = combatData.cursorY;
    combatData.cursorX = combatData.moveToX;
    combatData.cursorY = combatData.moveToY;
    ForceMapToMoveAlong(combatData.moveToX, combatData.moveToY, 
                        GetDir(oldX, oldY, combatData.moveToX, combatData.moveToY),
                        1);
    combatData.moveToX=-1;
    combatData.moveToY=-1;
    break;
  }
  case KC_CENTER: // Numpad_5
    combatData.PlaceCursorOnCurrentDude(TRUE);
    break;
  case KC_LEFT:
    combatData.moveCursorLeft();
    break;
  case KC_RIGHT:
    combatData.moveCursorRight();
    break;
  case KC_UP:
    combatData.moveCursorForward();
    break;
  case KC_DOWN:
    combatData.moveCursorBackward();
    break;
  case KC_NW:
    combatData.moveCursorLeft();
    combatData.moveCursorForward();
    break;
  case KC_NE:
    combatData.moveCursorRight();
    combatData.moveCursorForward();
    break;
  case KC_SE:
    combatData.moveCursorRight();
    combatData.moveCursorBackward();
    break;
  case KC_SW:
    combatData.moveCursorLeft();
    combatData.moveCursorBackward();
    break;
  default:
    StandardMenuKeyboardAction(key,ascii);
    break;
  }
}

void COMBAT_SPELL_AIM_MANUAL_MENU_DATA::OnDraw(void)
{
  UpdateCombatAimManualScreen();
}

void COMBAT_SPELL_AIM_MANUAL_MENU_DATA::OnUpdateUI(void)
{
  /*
   1 "NEXT"
   2 "PREV"
   3 "MANUAL"
   4 "TARGET"
   5 "CENTER"
   6 "EXIT"
   */

  menu.setAllItemsActive();
  menu.setItemInactive(1); // next
  menu.setItemInactive(2); // prev
  menu.setItemInactive(3); // manual
  menu.setItemInactive(5); // center

  // these targeting types use the direction between caster and targeted
  // map square to determine which way to direct the spell
  //
  // the path will be PathBAD if the cursor is on the caster, which
  // gives an indeterminate direction from caster to targeted square.
  //
  if ((m_caster->targets.m_type==AreaLinePickStart)||(m_caster->targets.m_type==AreaCone))
  {
    PATH_DIR dir = GetSpellTargetingDir(m_caster->GetCenterX(), m_caster->GetCenterY(), 
                                        combatData.cursorX, combatData.cursorY);
    if (dir == PathBAD)
      menu.setItemInactive(4); // target
  }

  if (m_caster->targets.m_SelectingUnits)
  {
    int target = getCombatantInCell(combatData.cursorX, combatData.cursorY, 1,1);
    if (target != NO_DUDE)
    {
      if (!m_caster->targets.m_FriendlyFireOk)
      {
        COMBATANT *pTarget = combatData.getCombatantPtr(target);
        ASSERT(pTarget!=NULL);

        if (!HaveLineOfSight(m_caster->GetCenterX(), m_caster->GetCenterY(), 
                             pTarget->GetCenterX(), pTarget->GetCenterY()))
        {
          menu.setItemInactive(4); // target
        }
        
        if (pTarget->GetIsFriendly())
          menu.setItemInactive(4); // target
      }
    }
    else
      menu.setItemInactive(4); // target

    int targrange = distance(m_caster->x,m_caster->y, 
                             combatData.cursorX, combatData.cursorY);
    if (targrange > m_caster->targets.m_MaxRange)
      menu.setItemInactive(4); // target
  }
}

void COMBAT_SPELL_AIM_MANUAL_MENU_DATA::OnMouseClickLeft(int mx, int my)
{
  MouseCombatMapMenu(mx,my);
}

////////////////////////////////////////////////////////////////////////COMBAT_MOVE_MENU_DATA
void COMBAT_MOVE_MENU_DATA::OnKeypress(key_code key, char ascii)
{
  switch (key)
  {
  case KC_RETURN: // exit move
    combatData.StopMoving();
    PopEvent();
    return;

  case KC_POSITION: // Numpad_0 or Insert
    if (!ValidTerrainViewportCoords(combatData.moveToX,combatData.moveToY))
    {
      combatData.moveToX=-1;
      combatData.moveToY=-1;
      break;
    }
    if (   (combatData.getActiveCombatCharacter().x == combatData.moveToX)
        && (combatData.getActiveCombatCharacter().y == combatData.moveToY))
    {
      combatData.moveToX=-1;combatData.moveToY=-1;
      break;
    }

    switch (GetDir(combatData.getActiveCombatCharacter().x,
                   combatData.getActiveCombatCharacter().y,
                   combatData.moveToX, combatData.moveToY))
    {
    case PathNorth:
      combatData.moveForward();
      break;
    case PathEast:
      combatData.moveRight();
      break;
    case PathSouth:
      combatData.moveBackward();
      break;
    case PathWest:
      combatData.moveLeft();
      break;
    case PathNW:
      combatData.moveNW();
      break;
    case PathNE:
      combatData.moveNE();
      break;
    case PathSW:
      combatData.moveSW();
      break;
    case PathSE:
      combatData.moveSE();
      break;
    default:
      break;
    }
    combatData.moveToX=-1;
    combatData.moveToY=-1;
    break;

  case KC_CENTER: // Numpad_5
    combatData.PlaceCursorOnCurrentDude(TRUE);
    combatData.moveToX=-1;
    combatData.moveToY=-1;
    break;

  case KC_LEFT:
    combatData.moveLeft();
    break;
  case KC_RIGHT:
    combatData.moveRight();
    break;
  case KC_UP:
    combatData.moveForward();
    break;
  case KC_DOWN:
    combatData.moveBackward();
    break;
  case KC_NW:
    combatData.moveNW();
    break;
  case KC_NE:
    combatData.moveNE();
    break;
  case KC_SE:
    combatData.moveSE();
    break;
  case KC_SW:
    combatData.moveSW();
    break;
  default:
    StandardMenuKeyboardAction(key,ascii);
    break;
  }

  if (combatData.CurrMoveWouldFleeMap())
  {
    SetMyState(TASK_AskBeforeFleeMap);
    PushEvent(new ASK_YES_NO_MENU_DATA("FLEE THE COMBAT MAP?",TRUE,CONTROL_CombatMove), DeleteEvent);
  }
  else
  {
    OnIdle();
  }
}

void COMBAT_MOVE_MENU_DATA::OnReturnToTopOfQueue(void)
{
  if (GetMyState() == TASK_AskBeforeFleeMap)
  {
    if (party.tradeItem==1) // yes, flee the map
    {
      SetMyState(TASK_initialized);
      OnIdle();
    }
    else // no, stay on the map (but still exit this move event)
    {
      combatData.moveToX=-1;
      combatData.moveToY=-1;    
      //SetMyState(TASK_initialized);
      combatData.StopMoving();
      PopEvent();
    }
  }
  else
    OnIdle();
}

void COMBAT_MOVE_MENU_DATA::OnInitialEvent(void)
{
  ClearFormattedText(textData);
  combatData.PlaceCursorOnCurrentDude();
  menu.setMenu(ExitMenuData);
  menu.setHorzOrient();
  menu.setTitle(FormatCombatMoveText());
  menu.MapKeyCodeToMenuItem(KC_ESCAPE, 1);
  SetMyState(TASK_initialized);
}

void COMBAT_MOVE_MENU_DATA::OnDraw(void)
{
  UpdateCombatMoveScreen();
}

void COMBAT_MOVE_MENU_DATA::OnUpdateUI(void)
{
  menu.setTitle(FormatCombatMoveText());
  menu.setAllItemsActive();
}

void COMBAT_MOVE_MENU_DATA::OnMouseClickLeft(int mx, int my)
{
  MouseCombatMapMenu(mx,my);
}

bool COMBAT_MOVE_MENU_DATA::OnIdle(void)
{
  if (GetMyState() != TASK_AskBeforeFleeMap)
  {
    combatData.UpdateCombat();
    combatData.HandleCurrState();
    CombatStateType state = combatData.GetCombatState();
    if ((state != CST_Moving) && (state != CST_None))
    {
      PopEvent();
      return false;
    }
    return true;
  }

  return false;  
}

////////////////////////////////////////////////////////////////////////COMBAT_SPRITE_ANIMATE_DATA
void COMBAT_SPRITE_ANIMATE_DATA::OnInitialEvent(void)
{
  ClearFormattedText(textData);
  menu.reset();    
  if (!combatSprite.picData.IsPicLoaded())
  {
    PopEvent();
    return;
  }

  EnableUserInput(FALSE);
}

void COMBAT_SPRITE_ANIMATE_DATA::OnDraw(void)
{
  UpdateSpriteAnimateScreen(NO_DUDE);// no combatant stats displayed
}

bool COMBAT_SPRITE_ANIMATE_DATA::OnCycle(void)
{
  //GameEvent::OnCycle();
  // Stay in this state until animation reaches target, or 
  // until last frame is displayed.
  // Moving sprites are moved along path by calls in DgnScrn.cpp
  bool result = false;

  if (UpdateAnimateList())
    result = true;

  if (combatSprite.IsDone())
  {
    combatSprite.Clear();
    EnableUserInput(TRUE);
    PopEvent();    
  }
  else
  {    
    if (combatSprite.Update()) // this function can change startTerrainX and Y
      result = true;
  }
  
  return result;
}

////////////////////////////////////////////////////////////////////////COMBAT_SPELL_SPRITE_ANIMATE_DATA
void COMBAT_SPELL_SPRITE_ANIMATE_DATA::OnInitialEvent(void)
{
  ClearFormattedText(textData);
  menu.reset();  
  
  if (!m_msg.IsEmpty())
    FormatCombatText(combatTextData, m_msg);

  //COMBATANT &dude = combatData.getActiveCombatCharacter();
  COMBATANT *dude = combatData.getActiveCastingCharacter();
  if (dude==NULL)
  {
    PopEvent();
    return;
  }
  int key = dude->GetSpellBookKeyBeingCast();
  CHARACTER_SPELL cspell;
  SPELL_DATA *pSdata=NULL;
  if (dude->spells.list.GetSpell(key, cspell))
    pSdata = spellData.GetSpellData(cspell.spell);   

  switch (m_type)
  {
  case CAST:
    if (combatData.NeedSpellCastAnimation(m_target))
      combatData.InitSpellCastAnimation(m_target);
    if (pSdata!=NULL) pSdata->PlayCast();
    break;

  case MISSILE:
    if (dude->targets.m_SelectingUnits)
    {
      COMBATANT *pTarget=GetCombatantPtr(m_target);
      if (pTarget!=NULL)
      {
        if (combatData.NeedSpellInRouteAnimation(pTarget->x,pTarget->y))
          combatData.InitSpellInRouteAnimation(pTarget->x,pTarget->y);
      }
    }
    else
    {
      if (combatData.NeedSpellInRouteAnimation(m_mapx,m_mapy))
        combatData.InitSpellInRouteAnimation(m_mapx,m_mapy);
    }
    if (pSdata!=NULL) pSdata->PlayMissile();
    break;

  case COVERAGE:
    if (dude->targets.m_SelectingUnits)
    {
      COMBATANT *pTarget=GetCombatantPtr(m_target);
      if (pTarget!=NULL)
      {
        if (combatData.NeedSpellCoverageAnimation(pTarget->x,pTarget->y))
          combatData.InitSpellCoverageAnimation(pTarget->x,pTarget->y);
      }
    }
    else
    {
      if (combatData.NeedSpellCoverageAnimation(m_mapx,m_mapy))
        combatData.InitSpellCoverageAnimation(m_mapx,m_mapy);
    }
    if (pSdata!=NULL) pSdata->PlayCoverage();
    break;

  case HIT:
    if (combatData.NeedSpellHitAnimation(m_target))
      combatData.InitSpellHitAnimation(m_target);
    if (pSdata!=NULL) pSdata->PlayHit();
    break;

  case LINGER:    
    if (combatData.NeedSpellLingerAnimation())
    {
      dude->activeSpellKey = 
        combatData.InvokePendingSpellOnMap(dude->self, dude->activeSpellKey);      

      combatData.InitSpellLingerAnimation(m_mapx,m_mapy);    
      if (pSdata!=NULL) pSdata->PlayLinger();
    }    
    break;

  default:
    ASSERT(FALSE); // unhandled animation type
    PopEvent();
    return;
  }

  m_picLoaded = combatSprite.picData.IsPicLoaded()?true:false;
  if ((!m_picLoaded)&&(m_msg.IsEmpty()))
  {
    PopEvent();
    return;
  }

  m_totalTime=0;
  EnableUserInput(FALSE);
}

void COMBAT_SPELL_SPRITE_ANIMATE_DATA::OnDraw(void)
{  
  switch (m_type)
  {
  case CAST:
    UpdateSpriteAnimateScreen(m_caster);
    break;
  case MISSILE:
    UpdateSpriteAnimateScreen(NO_DUDE);
    break;
  case COVERAGE:
    UpdateSpriteAnimateScreen(NO_DUDE);
    break;
  case HIT:
    UpdateSpriteAnimateScreen(m_target);
    break;
  case LINGER:
    UpdateSpriteAnimateScreen(NO_DUDE);
    break;
  default:
    UpdateSpriteAnimateScreen(NO_DUDE);
    return;
  }
}

bool COMBAT_SPELL_SPRITE_ANIMATE_DATA::OnCycle(void)
{
  //GameEvent::OnCycle();
  // Stay in this state until animation reaches target, or 
  // until last frame is displayed.
  // Moving sprites are moved along path by calls in DgnScrn.cpp
  bool result = false;

  if (UpdateAnimateList())
    result = true;

  static LONGLONG lastTime = 0;
  LONGLONG currTime = virtualGameTime;
  if (currTime < lastTime) currTime = lastTime; // rollover
  m_totalTime += (currTime-lastTime);
  
  if (m_picLoaded)
  {
    if (combatSprite.IsDone())
    {
      if (!m_msg.IsEmpty())
      {
        // Make sure the msg is displayed for the minimum
        // time, in case the animation is very short.
        if (m_totalTime >= GetMsgTimeDelay())
        {
          PopEvent();
          return result;
        }
      }
      else // no message
      {
        PopEvent();
        return result;
      }
    }
    else
    {    
      if (combatSprite.Update()) // this function can change startTerrainX and Y
        result = true;
    }
  }
  else
  {
    // no animation, but there might still be
    // a message to display
    ASSERT(!m_msg.IsEmpty());

    // Make sure the msg is displayed for the minimum time
    if (m_totalTime >= GetMsgTimeDelay())
    {
      PopEvent();
      return result;
    }
  }
  lastTime=currTime;
  return result;
}

void COMBAT_SPELL_SPRITE_ANIMATE_DATA::OnLeaveQueue(void)
{
  combatSprite.Clear();
  EnableUserInput(TRUE);      
  ClearFormattedText(combatTextData);
}

////////////////////////////////////////////////////////////////////////TIME_DELAY_MSG_DATA
void TIME_DELAY_MSG_DATA::OnInitialEvent(void)
{
  m_timerrcvd=false;
  ClearFormattedText(textData);
  menu.setMenu(EmptyMenuData); // starts empty
  m_msg = combatData.FormatTimeDelayMsg((CombatStateType)m_state);

  combatData.HandleTimeDelayMsgBegin(); // make attack if in CS_Attacking state

  CString specAbMsg("");
  DWORD MsgType=SPECAB_MSG_NONE;
  switch (m_state)
  {
  case CST_Casting:       MsgType=SPECAB_MSG_CASTING; break;
  case CST_ActivateSpell: MsgType=SPECAB_MSG_ACTIVATESPELL; break;
  case CST_Fleeing:       MsgType=SPECAB_MSG_FLEE; break;
  case CST_Turning:       MsgType=SPECAB_MSG_TURNUNDEAD; break;
  case CST_Bandaging:     MsgType=SPECAB_MSG_BANDAGE; break;
  case CST_Guarding:      MsgType=SPECAB_MSG_GUARD; break;
  case CST_Attacking:     MsgType=SPECAB_MSG_ATTACK; break;
  case CST_Moving:        MsgType=SPECAB_MSG_MOVE; break;
  case CST_EndingTurn:    MsgType=SPECAB_MSG_ENDTURN; break;
  case CST_Delaying:      MsgType=SPECAB_MSG_DELAY; break;
  }

  specAbMsg = combatData.FormatSpecAbMsg(MsgType);

  if (m_msg.GetLength()==0) 
  {
    // use spec ab msg instead if no regular msg
    m_msg=specAbMsg;
    if (m_msg.GetLength()>0)
    {
      menu.changeMenuItem(1, m_msg);
      menu.setHorzOrient();
      menu.setUseActive(FALSE); // no highlighting
      m_msg="";
    }
  }
  else
  {
    // use regular msg, then spec ab msg
    menu.changeMenuItem(1, m_msg);
    menu.setHorzOrient();
    menu.setUseActive(FALSE); // no highlighting
    m_msg=specAbMsg;
  }
  
  ((CDungeonApp*)AfxGetApp())->SetIntervalTimer(TASKTIMER_TimeDelayMsg, m_delay);
}

void TIME_DELAY_MSG_DATA::OnDraw(void)
{
  switch (m_state)
  {
  case CST_Attacking:  UpdateCombatTimeDelayMsg(combatData.GetCurrTarget()); break; // stats for target
  default:             UpdateCombatTimeDelayMsg(); break; // stats for curr dude
  }
}

bool TIME_DELAY_MSG_DATA::OnTimer(UINT TimerId)
{
  if (TimerId == TASKTIMER_TimeDelayMsg)
  {
    if (!m_msg.IsEmpty())
    {
      m_msgswitch=true;
      menu.setMenu(EmptyMenuData);
      menu.changeMenuItem(1, m_msg);
      menu.setHorzOrient();
      menu.setUseActive(FALSE);
      
      // get next spec ab msg, if any
      DWORD MsgType=SPECAB_MSG_NONE;
      switch (m_state)
      {
      case CST_Casting:       MsgType=SPECAB_MSG_CASTING; break;
      case CST_ActivateSpell: MsgType=SPECAB_MSG_ACTIVATESPELL; break;
      case CST_Fleeing:       MsgType=SPECAB_MSG_FLEE; break;
      case CST_Turning:       MsgType=SPECAB_MSG_TURNUNDEAD; break;
      case CST_Bandaging:     MsgType=SPECAB_MSG_BANDAGE; break;
      case CST_Guarding:      MsgType=SPECAB_MSG_GUARD; break;
      case CST_Attacking:     MsgType=SPECAB_MSG_ATTACK; break;
      case CST_Moving:        MsgType=SPECAB_MSG_MOVE; break;
      case CST_EndingTurn:    MsgType=SPECAB_MSG_ENDTURN; break;
      case CST_Delaying:      MsgType=SPECAB_MSG_DELAY; break;
      }
      m_msg = combatData.FormatSpecAbMsg(MsgType);

      ((CDungeonApp*)AfxGetApp())->SetIntervalTimer(TASKTIMER_TimeDelayMsg, m_delay);   
      return true;
    }
    else
    {    
      m_timerrcvd=true;

      if (!combatSprite.picData.IsPicLoaded())
      {
        combatSprite.Clear();
        combatData.CheckForDeadGuys();
        combatData.HandleTimeDelayMsgExpired(); // end attack if in CS_Attacking state
        PopEvent();
      }
      else if (combatSprite.IsDone())
      {
        combatSprite.Clear();
        combatData.CheckForDeadGuys();
        combatData.HandleTimeDelayMsgExpired(); // end attack if in CS_Attacking state
        PopEvent();
      }
    }
  }
  return m_timerrcvd;
}

bool TIME_DELAY_MSG_DATA::OnCycle(void)
{
  bool result = UpdateAnimateList();

  if (!combatSprite.picData.IsPicLoaded())
  {
    if (m_timerrcvd) 
    {
      combatSprite.Clear();
      combatData.CheckForDeadGuys();
      combatData.HandleTimeDelayMsgExpired(); // end attack if in CS_Attacking state
      PopEvent();
    }
    else
    {
      if (m_msgswitch==true)
      {
        m_msgswitch=false;
        result=true;
      }
    }
  }
  else if (!combatSprite.IsDone())
  {
    if (combatSprite.Update()) // this function can change startTerrainX and Y
      result = true;
    
    if (m_msgswitch==true)
    {
      m_msgswitch=false;
      result=true;
    }
  }
  else if (m_timerrcvd) 
  {
    combatSprite.Clear();
    combatData.CheckForDeadGuys();
    combatData.HandleTimeDelayMsgExpired(); // end attack if in CS_Attacking state
    PopEvent();
    result=true;
  }
  else
  {
    if (m_msgswitch==true)
    {
      m_msgswitch=false;
      result=true;
    }
  }
  return result;
}
////////////////////////////////////////////////////////////////////////COMBAT_ROUND_DELAY_DATA
void COMBAT_ROUND_DELAY_DATA::OnInitialEvent(void)
{
  ClearFormattedText(textData);
  menu.setMenu(EmptyMenuData); // erase menu
  menu.setHorzOrient();
  menu.setUseActive(FALSE); // no highlighting
  ((CDungeonApp*)AfxGetApp())->SetIntervalTimer(TASKTIMER_CombatRoundDelay, m_delay);
}

void COMBAT_ROUND_DELAY_DATA::OnDraw(void)
{
  UpdateCombatRoundDelayScreen();
}

bool COMBAT_ROUND_DELAY_DATA::OnTimer(UINT TimerId)
{
  if (TimerId == TASKTIMER_CombatRoundDelay)
  {
    if (IsCombatActive())
      combatData.HandleCombatRoundMsgExpired();
    PopEvent();
    return true;
  }
  return false;
}

////////////////////////////////////////////////////////////////////////GAME_SPEED_MENU_DATA
void GAME_SPEED_MENU_DATA::OnKeypress(key_code key, char ascii)
{
  if (TABParty(key)) return;
  if (key != KC_RETURN)
  {
    HMenuVPartyKeyboardAction(key,ascii);
    //StandardMenuKeyboardAction(key, ascii);
    return;
  }
  party.speed = (BYTE)menu.currentItem();
  PopEvent();
}

void GAME_SPEED_MENU_DATA::OnInitialEvent(void)
{
  ClearFormattedText(textData);
  menu.setMenu(SpeedMenuData);
  menu.setHorzOrient();
  menu.setCurrentItem(party.speed);
}

void GAME_SPEED_MENU_DATA::OnDraw(void)
{
  if (m_pOrigEvent != NULL)
  {
    if (m_pOrigEvent->GetEventType() == (eventType)Combat)
      UpdateSpeedCombatScreen();
    else
      UpdateSpeedScreen();
  }
  else
    UpdateSpeedScreen();
}

////////////////////////////////////////////////////////////////////////HUMAN_CHANGE_CLASS_MENU_DATA
void HUMAN_CHANGE_CLASS_MENU_DATA::OnKeypress(key_code key, char ascii)
{
  if (TABParty(key)) return;
  if (key != KC_RETURN)
  {
    HMenuVPartyKeyboardAction(key,ascii);
    //StandardMenuKeyboardAction(key, ascii);
    return;
  }

  CHARACTER& dude = GetActiveChar(this);

  switch (menu.currentItem()) 
  {
  case 1: // cleric
    dude.HumanChangeClass(Cleric);
    break;
  case 2: // druid
    dude.HumanChangeClass(Druid);
    break;
  case 3: // fighter
    dude.HumanChangeClass(Fighter);
    break;
  case 4: // ranger
    dude.HumanChangeClass(Ranger);
    break;
  case 5: // paladin
    dude.HumanChangeClass(Paladin);
    break;
  case 6: // Magic User
    dude.HumanChangeClass(MagicUser);
    break;
  case 7: // thief
    dude.HumanChangeClass(Thief);
    break;
  case 8: // exit;
    PopEvent(); 
    return;
  }
}

void HUMAN_CHANGE_CLASS_MENU_DATA::OnInitialEvent(void)
{
  ClearFormattedText(textData);
  menu.setMenu(ChangeClassMenuData);
  menu.setVertOrient();
  menu.MapKeyCodeToMenuItem(KC_ESCAPE, 8);
}

void HUMAN_CHANGE_CLASS_MENU_DATA::OnDraw(void)
{
  UpdateHumanChangeClassScreen();
}

void HUMAN_CHANGE_CLASS_MENU_DATA::OnUpdateUI(void)
{
  if (CheckForMultiBoxText()) return;
  CHARACTER& dude = GetActiveChar(this);
  if (!dude.CanDualClassCleric())
    menu.setItemInactive(1);
  if (!dude.CanDualClassDruid())
    menu.setItemInactive(2);
  if (!dude.CanDualClassFighter())
    menu.setItemInactive(3);
  if (!dude.CanDualClassRanger())
    menu.setItemInactive(4);
  if (!dude.CanDualClassPaladin())
    menu.setItemInactive(5);
  if (!dude.CanDualClassMagicUser())
    menu.setItemInactive(6);
  if (!dude.CanDualClassThief())
    menu.setItemInactive(7);
}

////////////////////////////////////////////////////////////////////////TRADE_TAKER_SELECT_MENU_DATA
void TRADE_TAKER_SELECT_MENU_DATA::OnKeypress(key_code key, char ascii)
{
  if (TABParty(key)) return;
  if (key != KC_RETURN)
  {
    HMenuVPartyKeyboardAction(key,ascii);
    if ((key == KC_UP) || (key == KC_DOWN))
    {
      CHARACTER &tempCHAR = GetActiveChar(m_pOrigEvent,party.activeCharacter);
      // avoid direct reference to character small pic data
      currPic = tempCHAR.smallPic;
      currPic.LoadPicSurfaces("prt_");
    }
    return;
  }

  switch (menu.currentItem()) 
  {
  case 1: // select char

    // trading to self allows re-arranging of inventory items
    //if (party.tradeGiver != party.activeCharacter)
    {
      if (!itemIsMoney(charItemListText.GetItemClass(party.tradeItem)))
      {
        if (!GetActiveChar(m_pOrigEvent,party.tradeGiver).myItems.IsReady(charItemListText.GetCharIndex(party.tradeItem)))
        {
          tradeItem(charItemListText.GetItemIndex(party.tradeItem), 
                    charItemListText.GetItemQty(party.tradeItem), 
                    charItemListText.GetItemCharges(party.tradeItem), 
                    charItemListText.GetItemIdentified(party.tradeItem), 
                    charItemListText.GetCharIndex(party.tradeItem), 
                    GetActiveChar(m_pOrigEvent, party.tradeGiver), 
                    GetActiveChar(m_pOrigEvent, party.activeCharacter));
        }
      }
      else
      {
        if (party.tradeGiver != party.activeCharacter)
        {
          if (party.tradeQty > 0)
          {
            tradeItemMoney(charItemListText.GetItemClass(party.tradeItem), 
                           party.tradeQty, 
                           GetActiveChar(m_pOrigEvent, party.tradeGiver),  // giver
                           GetActiveChar(m_pOrigEvent, party.activeCharacter) ); // taker
          }
        }
      }
    }
    break;
  case 2: // exit
    break;
  default:
    WriteDebugString("Unhandled menu state in handleTradeTaker()\n");
    miscError = InvalidMenuState;
    break;
  }

  PopEvent();
}

void TRADE_TAKER_SELECT_MENU_DATA::OnInitialEvent(void)
{
  ClearFormattedText(textData);
  int itemTextIndex = currCharItemsTextIndex();
  party.tradeItem = (BYTE)itemTextIndex;
  party.tradeGiver = party.activeCharacter;

  CHARACTER &tempCHAR = GetActiveChar(m_pOrigEvent,party.activeCharacter);
  currPic = tempCHAR.smallPic;
  currPic.LoadPicSurfaces("prt_");

  if (itemIsMoney(charItemListText.GetItemClass(itemTextIndex)))
  {
    menu.reset();
    party.tradeQty = 0;
    PushEvent(new GET_MONEY_QTY_DATA(GetEventType(),m_pOrigEvent),DeleteEvent);
    return;
  }

  party.tradeQty = 1;
  menu.setMenu(TradeTakerMenuData);
  menu.setHorzOrient();
  menu.MapKeyCodeToMenuItem(KC_ESCAPE, 2);
}

void TRADE_TAKER_SELECT_MENU_DATA::OnDraw(void)
{
  if ((m_pOrigEvent != NULL) && (m_pOrigEvent->GetEventType()==(eventType)Combat))
    UpdateCombatTradeTakerSelectScreen();
  else
    UpdateTradeTakerSelectScreen();
}

void TRADE_TAKER_SELECT_MENU_DATA::OnUpdateUI(void)
{
  // 1 { "SELECT", 0 },
  // 2 { "EXIT", 1 }

  if (CheckForMultiBoxText()) return;

  if (IsCombatActive())
  {
    // during combat, disallow trading to character that is
    // not directly adjacent to trade giver.
    int dist = combatData.GetDistanceBetween_ByPartyIndex(party.tradeGiver, party.activeCharacter);

    if (dist > 1)
      menu.setItemInactive(1); // select      
  }

  //if (party.tradeGiver == party.activeCharacter)
  //  menu.setItemInactive(1); // select
}

void TRADE_TAKER_SELECT_MENU_DATA::OnReturnToTopOfQueue(void)
{
  menu.setMenu(TradeTakerMenuData);
  menu.setHorzOrient();
}

void TRADE_TAKER_SELECT_MENU_DATA::OnLeaveQueue(void)
{
  party.activeCharacter = party.tradeGiver;
  party.tradeGiver = 0;
  party.tradeItem = 0;
  party.tradeQty = 0;  
}

////////////////////////////////////////////////////////////////////////ALTER_GAME_MENU_DATA
void ALTER_GAME_MENU_DATA::OnKeypress(key_code key, char ascii)
{
  if (TABParty(key)) return;
  if (key != KC_RETURN)
  {
    HMenuVPartyKeyboardAction(key, ascii);
    return;
  }

  character = GetActiveChar(this);    

  switch (menu.currentItem()) 
  {
  case 1:  // Order
    PushEvent(new ALTER_ORDER_MENU_DATA, DeleteEvent);
    break;
  case 2: // Drop character
    {
      CString msg;
      msg.Format("REMOVE %s FROM PARTY?", character.GetName());
      SetMyState(TASK_DropCharFromParty);
      PushEvent(new ASK_YES_NO_MENU_DATA(msg,TRUE,CONTROL_Encamp), DeleteEvent);
    }
    //party.removeCharacter(party.activeCharacter);
    break;
  case 3: // Speed
    PushEvent(new GAME_SPEED_MENU_DATA(this), DeleteEvent);
    break;
  case 4: // Icon  
    SetMyState(TASK_AlterCharacter);
    PushEvent(new GETCHARICON_MENU_DATA, DeleteEvent);
    break;
  case 5: // Small Pic    
    SetMyState(TASK_AlterCharacter);
    PushEvent(new GETCHARSMALLPIC_MENU_DATA, DeleteEvent);
    break;
  case 6: // Level    
    PushEvent(new ALTER_LEVEL_MENU_DATA(), DeleteEvent);
    break;
  case 7: // Sound    
    PushEvent(new ALTER_SOUND_MENU_DATA(), DeleteEvent);
    break;
  case 8: // Music
    SetMyState(TASK_AlterMusicPlayback);
    PushEvent(new ASK_YES_NO_MENU_DATA("PLAY BACKGROUND MUSIC?",TRUE,CONTROL_Adventure), DeleteEvent);
    break;
  case 9: // Exit
    PopEvent(); 
    return;
  }
}

void ALTER_GAME_MENU_DATA::OnInitialEvent(void)
{
  ClearFormattedText(textData);
  menu.setMenu(AlterMenuData);
  menu.setHorzOrient();
  menu.MapKeyCodeToMenuItem(KC_ESCAPE, 9);
  party.tradeItem=0;
  SetMyState(TASK_initialized);
}

void ALTER_GAME_MENU_DATA::OnDraw(void)
{
  UpdateAlterGameScreen();
}

void ALTER_GAME_MENU_DATA::OnUpdateUI(void)
{
  if (CheckForMultiBoxText()) return;
  if (party.numCharacters <= 1)
  {
    menu.setItemInactive(1); // order
    menu.setItemInactive(2); // drop
  }
}

void ALTER_GAME_MENU_DATA::OnReturnToTopOfQueue(void)
{
  if (GetMyState()==TASK_AlterMusicPlayback)
  {
    if (party.tradeItem==1) // yes, play background music
    {
      if (pSndMgr!=NULL) pSndMgr->SetMusicEnable(TRUE);
      StartBackgroundMusic();      
    }
    else
    {
      StopBackgroundMusic();
      if (pSndMgr!=NULL) pSndMgr->SetMusicEnable(FALSE);
    }
  }
  else if (GetMyState()==TASK_AlterCharacter)
  {
    // to make sure this works, init 'character' with
    // active party character before pushing an event.
    GetActiveChar(this) = character;
  }
  else if (GetMyState()==TASK_DropCharFromParty)
  {
    if (party.tradeItem==1) // yes, drop character
      party.removeCharacter(party.activeCharacter);
  }

  OnInitialEvent();
}

////////////////////////////////////////////////////////////////////////ALTER_LEVEL_MENU_DATA
void ALTER_LEVEL_MENU_DATA::OnKeypress(key_code key, char ascii)
{
  if (TABParty(key)) return;
  if (key != KC_RETURN)
  {
    HMenuVPartyKeyboardAction(key,ascii);
    //StandardMenuKeyboardAction(key, ascii);
    return;
  }
  party.skillLevel = menu.currentItem()-1; 
  PopEvent();
}

void ALTER_LEVEL_MENU_DATA::OnInitialEvent(void)
{
  ClearFormattedText(textData);
  //menu.setMenu(LevelMenuData);
  menu.reset();
  menu.setStartCoord(USE_DEFAULT_HORZ_MENU, 0);
  
  for (int i=0; i<DIFFICULTY_LEVEL_DATA::NUM_DIFF_LVLS; i++)
  {
    if (globalData.m_diffLvlData.m_data[i].m_Name.GetLength() > 0)
    {
      menu.addMenuItem(globalData.m_diffLvlData.m_data[i].m_Name);
    }
    else
    {
      int newItem = menu.addMenuItem(" ");
      menu.setItemInactive(newItem);
    }
  }

  menu.setHorzOrient();
  menu.SetFirstLettersShortcuts();
  menu.setCurrentItem(party.skillLevel+1);
}

void ALTER_LEVEL_MENU_DATA::OnUpdateUI(void)
{
  if (CheckForMultiBoxText()) return;
  menu.setAllItemsActive();

  for (int i=0; i<DIFFICULTY_LEVEL_DATA::NUM_DIFF_LVLS; i++)
  {
    if (globalData.m_diffLvlData.m_data[i].m_Name.GetLength() == 0)
      menu.setItemInactive(i);
  }   
}

void ALTER_LEVEL_MENU_DATA::OnDraw(void)
{
  UpdateAlterGameScreen();
}

////////////////////////////////////////////////////////////////////////ALTER_SOUND_MENU_DATA
void ALTER_SOUND_MENU_DATA::OnKeypress(key_code key, char ascii)
{
  if (TABParty(key)) return;
  if (key == KC_RETURN)
  {
    int slevel = menu.currentItem()-1;
    slevel *= 10;
    if (pSndMgr!=NULL) pSndMgr->SetMasterVolume(slevel);
    PopEvent();
  }

  switch (key)
  {
  case KC_NUM:   
    {
      if ((ascii >= '0')&&(ascii <= '9'))
      {
        int slevel = ascii-'0';
        slevel *= 10;
        if (pSndMgr!=NULL) pSndMgr->SetMasterVolume(slevel);
        slevel = (ascii-'0')+1;
        menu.setCurrentItem(slevel);
      }
    }
    break;
  default: 
    {
      HMenuVPartyKeyboardAction(key,ascii);
      //StandardMenuKeyboardAction(key, ascii); 
      int slevel = menu.currentItem()-1;
      slevel *= 10;
      if (pSndMgr!=NULL) pSndMgr->SetMasterVolume(slevel);
    }
    break;
  }

  PlayPartyBump();
}

void ALTER_SOUND_MENU_DATA::OnInitialEvent(void)
{
  ClearFormattedText(textData);
  menu.setMenu(SoundMenuData);
  menu.setHorzOrient();
  int slevel = 0;
  if (pSndMgr!=NULL) slevel = pSndMgr->GetMasterVolume();
  if (slevel > 0) slevel = slevel / 10;
  if (slevel < 0) slevel = 0;
  if (slevel > 10) slevel = 10;
  menu.setCurrentItem(slevel+1);
}

void ALTER_SOUND_MENU_DATA::OnDraw(void)
{
  UpdateAlterGameScreen();
}

////////////////////////////////////////////////////////////////////////ALTER_ORDER_MENU_DATA

void ALTER_ORDER_MENU_DATA::OnKeypress(key_code key, char ascii)
{
  if (TABParty(key)) return;
  if (key == KC_RETURN)
  {
    // prevents problems when alter menu gets to top of queue
    character = GetActiveChar(this);    
    PopEvent();
    return;
  }

  switch (key)
  {
  case KC_UP:   party.DecCharacterOrder(); break;
  case KC_DOWN: party.IncCharacterOrder(); break;
  default: StandardMenuKeyboardAction(key, ascii);
  }  
}

void ALTER_ORDER_MENU_DATA::OnInitialEvent(void)
{
  ClearFormattedText(textData);
  menu.setMenu(ExitMenuData);
  menu.setHorzOrient();
}

void ALTER_ORDER_MENU_DATA::OnDraw(void)
{
  UpdateAlterOrderScreen();
}

////////////////////////////////////////////////////////////////////////PARTY_DIED_MENU_DATA
void PARTY_DIED_MENU_DATA::OnKeypress(key_code key, char ascii)
{
  if (TABParty(key)) return;
  if (key != KC_RETURN)
  {
    HMenuVPartyKeyboardAction(key,ascii);
    //StandardMenuKeyboardAction(key, ascii);
    return;
  }

  ResetAdventureData(); // flags, quests, spells, etc
  PlayDeath(FALSE);
  currPic.Clear();
  ClearFormattedText(textData);
  ClearFormattedText(errorTextData);
  ProcInput.InitializeToMain();  
}

void PARTY_DIED_MENU_DATA::OnInitialEvent(void)
{
  menu.setMenu(PartyDiedMenuData);
  menu.setHorzOrient();  
  if (party.numCharacters == 1)
    FormatDisplayText(textData, "YOU HAVE DIED!");
  else
    FormatDisplayText(textData, "THE PARTY HAS DIED!");
  PlayDeath(TRUE);
}

void PARTY_DIED_MENU_DATA::OnDraw(void)
{
  UpdatePartyDiedScreen();
}

////////////////////////////////////////////////////////////////////////REST_MENU_DATA
void REST_MENU_DATA::OnKeypress(key_code key, char ascii)
{
  //if (GetMyState() == TASK_RestMenuInput)
  {
    int rslt;

    if (TABParty(key)) return;

    if (key != KC_RETURN) 
    {
      if (GetMyState() == TASK_RestMenuInput)
      {
        rslt=handleRestTimeInput(RESTTIME_key, this, key);
        if (rslt&2) return;
      }
      
      StandardMenuKeyboardAction(key,ascii);

      if (GetMyState() == TASK_RestMenuInput)
      {
        // make sure menu left/right navigation keeps
        // in synch with form fields
        switch (menu.currentItem()) 
        {
        case 2: // days
          handleRestTimeInput(RESTTIME_activate, this, RTF_DAYS_FIELD);
          break;
        case 3: // hours
          handleRestTimeInput(RESTTIME_activate, this, RTF_HOURS_FIELD);
          break;
        case 4: // mins
          handleRestTimeInput(RESTTIME_activate, this, RTF_MINUTES_FIELD);
          break;    
        }
      }
      return;
    }

    /*
     1 { "REST", 0 },
     2 { "DAYS", 0 },
     3 { "HOURS", 0 },
     4 { "MINS", 0 },
     5 { "ADD", 0 },
     6 { "SUB", 0 },
     7 { "EXIT", 1 }
    */
    switch (menu.currentItem()) 
    {
    case 1: // rest
      SetMyState(TASK_RestMenuEngage);
      party.resting=TRUE;
      break;
    case 2: // days
      handleRestTimeInput(RESTTIME_activate, this, RTF_DAYS_FIELD);
      break;
    case 3: // hours
      handleRestTimeInput(RESTTIME_activate, this, RTF_HOURS_FIELD);
      break;
    case 4: // mins
      handleRestTimeInput(RESTTIME_activate, this, RTF_MINUTES_FIELD);
      break;    
    case 5: // add
      handleRestTimeInput(RESTTIME_key, this, KC_PLUS);
      break;    
    case 6: // sub    
      handleRestTimeInput(RESTTIME_key, this, KC_MINUS);
      break;    
    case 7: // exit
      if (GetMyState() == TASK_RestMenuInput)
      {
        handleRestTimeInput(RESTTIME_exit, this); // Terminate modifications
        PopEvent();
      }
      else
      {
        handleRestTimeInput(RESTTIME_exit, this);
        SetMyState(TASK_RestMenuInput);        
        party.resting=FALSE;
        handleRestTimeInput(RESTTIME_initial, this);
      }
      break;    
    default:
      handleRestTimeInput(RESTTIME_exit,this); // Terminate modifications
      WriteDebugString("Unhandled menu item in REST_MENU_DATA");
      miscError = InvalidMenuState;
      currPic.Clear();
      PopEvent();
      break;
    } 
  }
}

void REST_MENU_DATA::OnMouseClickLeft(int mx, int my)
{
  if (GetMyState() == TASK_RestMenuInput)
  {
    int rslt=handleRestTimeInput(RESTTIME_mouseClick,
                             this,
                             mx, my);
    if (!(rslt&1))
    {
      GameEvent::OnMouseClickLeft(mx, my);
    }
    else
    {

    }
  }
  else
  {
    GameEvent::OnMouseClickLeft(mx, my);
  }
}

void REST_MENU_DATA::OnInitialEvent(void)
{
  SetMyState(TASK_RestMenuInput);
  party.resting=FALSE;
  ClearFormattedText(textData);
  menu.setMenu(RestMenuData);
  menu.setHorzOrient();

  DWORD MinuteInc = party.CalcRestTime();
  m_days = MinuteInc / 1440;
  MinuteInc %= 1440;
  m_hours  = MinuteInc / 60;
  MinuteInc %= 60;
  m_mins  = MinuteInc;
  
  // if using Camp event
  if ((m_pOrigEvent != NULL) && (m_pOrigEvent->GetEventType() == Camp))
  {
    if (m_pOrigEvent->GetEventPic().picType != BogusDib)
    {
      currPic = m_pOrigEvent->GetEventPic();
      currPic.LoadPicSurfaces("sp_");
    }
    else
    {
      int zone = levelData.GetCurrZone(party.Posx, party.Posy);
      currPic = levelData.zoneData.zones[zone].campArt;
      currPic.LoadPicSurfaces("sp_");
    };
  }
  else
  {    
    // no event, just 'Encamp' menu option
    int zone = levelData.GetCurrZone(party.Posx, party.Posy);
    currPic = levelData.zoneData.zones[zone].campArt;
    currPic.LoadPicSurfaces("sp_");
  }

  handleRestTimeInput(RESTTIME_initial, this);
}

bool REST_MENU_DATA::OnCycle(void)
{
  if (GetMyState() == TASK_RestMenuInput) 
  {
    return GameEvent::OnCycle();
  }
  else
  {
    __int64 dleft = m_days;
    __int64 hleft = m_hours;
    __int64 mleft = m_mins;

    __int64 minuteDelta = GetMinuteDelta(dleft, hleft, mleft);
    __int64 minuteTotal = (dleft*1440) + (hleft*60) + mleft;

    // update rest event minute tracker for this zone
    __int64 delta=0;
    GameEvent *tempEvent=NULL;
    while (delta < minuteDelta)
    {
      //levelData.IncTimeEventTime();
      levelData.IncRestEventTime(levelData.GetCurrZone(party.Posx, party.Posy), 1);

      party.incrementClock(1);

/*
      if (levelData.haveTimeEvent(
        party.Posx,
        party.Posy,
        party.days,
        party.hours,
        party.minutes,
        &tempEvent))
      {
        break;
      }
*/
      
      if (levelData.haveRestEvent(
               party.Posx,
               party.Posy,
               &tempEvent))
      {
        break;
      }

      delta++;
    }    

    minuteTotal -= delta;

    if (minuteTotal > 0)
    {
      dleft = minuteTotal / 1440;
      minuteTotal %= 1440;
      hleft = minuteTotal / 60;
      minuteTotal %= 60;
      mleft = minuteTotal;
    }
    else
    {
      dleft = 0;
      hleft = 0;
      mleft = 0;
    }

    dleft = max(0, dleft);
    hleft = max(0, min(23,hleft));
    mleft = max(-1, min(59,mleft));

    m_days = dleft;
    m_hours = hleft;
    m_mins = mleft;
   
    GameEvent::OnCycle();

    //GameEvent *tempEvent=NULL;
    //if (levelData.haveRestEvent(
    //         party.Posx,
    //         party.Posy,
    //         &tempEvent))
    if (tempEvent!=NULL)
    {
      handleRestTimeInput(RESTTIME_exit, this);

      FormatPausedText(pausedTextData, "YOUR REST IS INTERRUPTED!");
      DisplayPausedText(pausedTextData);
      ReplaceEvent(tempEvent, LevelEvent);
      return false;
    }

    if ((dleft > 0) || (hleft > 0) || (mleft > 0))
    {
      handleRestTimeInput(RESTTIME_refresh, this);      
    }
    else
    {
      SetMyState(TASK_RestMenuInput);
      party.resting=FALSE;
      menu.setCurrentItem(1);
    }

    return true;
  }
}

void REST_MENU_DATA::OnUpdateUI(void)
{
  if (CheckForMultiBoxText()) return;
  menu.setAllItemsActive();
  if (GetMyState() != TASK_RestMenuInput)
  {
    menu.setItemInactive(1); // rest
    menu.setItemInactive(2); // days
    menu.setItemInactive(3); // hours
    menu.setItemInactive(4); // minutes
    menu.setItemInactive(5); // add
    menu.setItemInactive(6); // sub
  }
}

void REST_MENU_DATA::OnDraw(void)
{
  handleRestTimeInput(RESTTIME_refresh, this);
}

////////////////////////////////////////////////////////////////////////NEW_MU_SPELL_LEVEL_MENU_DATA
void NEW_MU_SPELL_LEVEL_MENU_DATA::OnKeypress(key_code key, char ascii)
{
  if (key != KC_RETURN)
  {
    HMenuVItemsKeyboardAction(key, ascii);
    return;
  }
  /*
   { "SELECT", 0 },
   { "UNSELECT", 0 },
   { "REMOVE", 0 },
   { "NEXT", 0 },
   { "PREV", 0 },
   { "EXIT", 1 }
  */
  int currIndex = currSpellTextIndex();
  switch (menu.currentItem()) 
  {
  case 1: // select
    spellListText.ToggleSelected(currIndex);
    break;
  case 2: // unselect
    spellListText.ToggleSelected(currIndex);
    break;
  case 3: // remove
    // unused for this event type
    break;
  case 4: // next
    nextSpellPage();
    break;    
  case 5: // prev
    prevSpellPage();
    break;    
  case 6: // exit
    {
      BOOL success = FALSE;
      for (int i=0; i<spellListText.GetCount(); i++)
      {
        if (spellListText.IsSelected(i))
        {
          int SpellDbKey = spellListText.GetSpellIndex(i);

          if (!m_dude.spells.knowSpell( MagicUserFlag,
                                        spellData.GetSpellClass(SpellDbKey), 
                                        SpellDbKey, 
                                        spellData.GetSpellLevel(SpellDbKey),
                                        TRUE ))
          {
            WriteDebugString("failed to knowSpell that was selected in NEW_MU_SPELL_LEVEL_MENU_DATA\n");
          }
          else
            success = TRUE;
        }
      }

      if (success)
      {
        handleSpellFormInput(SPELLFORM_exit, spellListText);
        PopEvent();
      }
      else
      {
        PushEvent(new ASK_YES_NO_MENU_DATA("YOU DID NOT SELECT ANY SPELLS, EXIT ANYWAY?",TRUE, CONTROL_CreateCharacter), DeleteEvent);
      }
    }    
    break;    
  default:
    WriteDebugString("Unhandled menu item in NEW_MU_SPELL_LEVEL_MENU_DATA");
    miscError = InvalidMenuState;
    handleSpellFormInput(SPELLFORM_exit, spellListText);
    PopEvent();
    break;
  } 
}

void NEW_MU_SPELL_LEVEL_MENU_DATA::OnInitialEvent(void)
{
  ClearFormattedText(textData);
  menu.setMenu(MemorizeMenuData);
  menu.setHorzOrient();
  menu.MapKeyCodeToMenuItem(KC_ESCAPE, 6);
  spellListText.Clear();
  
  BOOL PopIt = FALSE;

  setSpellNewLevelItemState(m_dude);
  handleSpellFormInput(SPELLFORM_exit, spellListText);
  handleSpellFormInput(SPELLFORM_initial, spellListText);

  // character has no capacity for additional spells
  if (!spellListText.CanSelectAnything())
    PopIt = TRUE;

  if (PopIt)
  {
    handleSpellFormInput(SPELLFORM_exit, spellListText);
    PopEvent();
  }
}

void NEW_MU_SPELL_LEVEL_MENU_DATA::OnDraw(void)
{
  GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);
  drawBorder();
  // draw middle horz bar, but 
  // not viewport and middle ver bar
  drawViewport(FALSE, FALSE);
  if (errorTextData.numLines == 0)
    handleSpellFormInput(SPELLFORM_refresh, spellListText);
  menu.DisplayMenu();
  DisplayTextAndFlip();
}

void NEW_MU_SPELL_LEVEL_MENU_DATA::OnUpdateUI(void)
{
  /*
   { "SELECT", 0 },
   { "UNSELECT", 0 },
   { "REMOVE", 0 },
   { "NEXT", 0 },
   { "PREV", 0 },
   { "EXIT", 1 }
  */
  menu.setAllItemsActive();
  menu.setItemInactive(3); // remove

  if (spellListText.GetCount() <= 0)
  {
    menu.setItemInactive(1); // select
    menu.setItemInactive(2); // unselect
    menu.setItemInactive(4); // next
    menu.setItemInactive(5); // prev
    return;
  }
 
  int currIndex = currSpellTextIndex();

  if (spellListText.IsSelected(currIndex))
    menu.setItemInactive(1); // select
  else
    menu.setItemInactive(2); // unselect

  if (!spellListText.CanSelect(currIndex))
    menu.setItemInactive(1); // select

  if (!allowSpellNextPage())
    menu.setItemInactive(4); // next
  if (!allowSpellPrevPage())
    menu.setItemInactive(5); // prev
}

void NEW_MU_SPELL_LEVEL_MENU_DATA::OnReturnToTopOfQueue()
{
  if (party.tradeItem==1) // yes, exit anyway
  {
    handleSpellFormInput(SPELLFORM_exit, spellListText);
    PopEvent();
  }
  else
  {
    OnInitialEvent();
  }
}

////////////////////////////////////////////////////////////////////////MEMORIZE_MENU_DATA
void MEMORIZE_MENU_DATA::OnKeypress(key_code key, char ascii)
{
  if (key != KC_RETURN)
  {
    HMenuVItemsKeyboardAction(key, ascii);
    return;
  }
  /*
   { "SELECT", 0 },
   { "UNSELECT", 0 },
   { "REMOVE", 0 },
   { "NEXT", 0 },
   { "PREV", 0 },
   { "EXIT", 1 }
  */
  int currIndex = currSpellTextIndex();
  switch (menu.currentItem()) 
  {
  case 1: // select
    spellListText.IncSpellSelectedCount(currIndex);
    break;
  case 2: // unselect
    spellListText.DecSpellSelectedCount(currIndex);
    break;
  case 3: // remove
    spellListText.DecSpellMemorizedCount(currIndex);
    break;
  case 4: // next
    nextSpellPage();
    break;    
  case 5: // prev
    prevSpellPage();
    break;    
  case 6: // exit
    handleSpellFormInput(SPELLFORM_exit, spellListText);
    PopEvent();
    break;    
  default:
    WriteDebugString("Unhandled menu item in MEMORIZE_MENU_DATA");
    miscError = InvalidMenuState;
    handleSpellFormInput(SPELLFORM_exit, spellListText);
    PopEvent();
    break;
  } 
}

void MEMORIZE_MENU_DATA::OnInitialEvent(void)
{
  if (!GetActiveChar(this).canCastSpells())
  {
    PopEvent();
    return;
  }
  ClearFormattedText(textData);
  menu.setMenu(MemorizeMenuData);
  menu.setHorzOrient();
  menu.MapKeyCodeToMenuItem(KC_ESCAPE, 6);
  setSpellMemorizeItemState(GetActiveChar(this));
  handleSpellFormInput(SPELLFORM_exit, spellListText);
  handleSpellFormInput(SPELLFORM_initial, spellListText);
}

void MEMORIZE_MENU_DATA::OnDraw(void)
{
  //UpdateMemorizeMenuScreen();
  GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);
  drawBorder();
  // draw middle horz bar, but 
  // not viewport and middle ver bar
  drawViewport(FALSE, FALSE);
  if (errorTextData.numLines == 0)
    handleSpellFormInput(SPELLFORM_refresh, spellListText);
  menu.DisplayMenu();
  DisplayTextAndFlip();
}

void MEMORIZE_MENU_DATA::OnUpdateUI(void)
{
  /*
   { "SELECT", 0 },
   { "UNSELECT", 0 },
   { "REMOVE", 0 },
   { "NEXT", 0 },
   { "PREV", 0 },
   { "EXIT", 1 }
  */
  menu.setAllItemsActive();
  if (spellListText.GetCount() <= 0)
  {
    menu.setItemInactive(1); // select
    menu.setItemInactive(2); // unselect
    menu.setItemInactive(3); // remove
    menu.setItemInactive(4); // next
    menu.setItemInactive(5); // prev
    return;
  }
 
  int currIndex = currSpellTextIndex();
  int numSelect = spellListText.GetSpellSelectedCount(currIndex);
  int numMem = spellListText.GetSpellMemorizeCount(currIndex);
  
  //int level;
  //int school;
  //spellListText.GetSpellLevelAndSchoolIndex(currIndex, level, school);

  if (!spellListText.CanSelect(currIndex))
    menu.setItemInactive(1); // select
  if ((numSelect <= 0) || (numSelect <= numMem))
    menu.setItemInactive(2); // unselect
  if (numMem <= 0)
    menu.setItemInactive(3); // remove
  if (!allowSpellNextPage())
    menu.setItemInactive(4); // next
  if (!allowSpellPrevPage())
    menu.setItemInactive(5); // prev
}

////////////////////////////////////////////////////////////////////////DISPLAY_SPELL_EFFECTS_MENU_DATA
void DISPLAY_SPELL_EFFECTS_MENU_DATA::OnKeypress(key_code key, char ascii)
{
  if (key != KC_RETURN)
  {
    StandardMenuKeyboardAction(key,ascii);
    return;
  }
  /*
   { "NEXT", 0 },
   { "PREV", 0 },
   { "EXIT", 1 }
  */
  switch (menu.currentItem()) 
  {
  case 1: // next
    nextSpellPage();
    break;    
  case 2: // prev
    prevSpellPage();
    break;    
  case 3: // exit
    handleSpellFormInput(SPELLFORM_exit, spellListText);
    PopEvent();
    break;    
  default:
    WriteDebugString("Unhandled menu item in DISPLAY_SPELL_EFFECTS_MENU_DATA");
    miscError = InvalidMenuState;
    handleSpellFormInput(SPELLFORM_exit, spellListText);
    PopEvent();
    break;
  } 
}

void DISPLAY_SPELL_EFFECTS_MENU_DATA::OnInitialEvent(void)
{
  ClearFormattedText(textData);
  menu.setMenu(DisplaySpellEffectsMenuData);
  menu.setHorzOrient();
  menu.MapKeyCodeToMenuItem(KC_ESCAPE, 3);
  setSpellEffectItemState(GetActiveChar(m_pOrigEvent));
  handleSpellFormInput(SPELLFORM_exit, spellListText);
  handleSpellFormInput(SPELLFORM_initial, spellListText);
}

void DISPLAY_SPELL_EFFECTS_MENU_DATA::OnDraw(void)
{
  GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);
  drawBorder();
  if (errorTextData.numLines == 0)
    handleSpellFormInput(SPELLFORM_refresh, spellListText);
  menu.DisplayMenu();
  DisplayTextAndFlip();

}

void DISPLAY_SPELL_EFFECTS_MENU_DATA::OnUpdateUI(void)
{
  /*
   { "NEXT", 0 },
   { "PREV", 0 },
   { "EXIT", 1 }
  */   
  menu.setAllItemsActive();
  if (!allowSpellNextPage())
    menu.setItemInactive(1); // next
  if (!allowSpellPrevPage())
    menu.setItemInactive(2); // prev
}

////////////////////////////////////////////////////////////////////////MEMORIZE_VERIFY_MENU_DATA
void MEMORIZE_VERIFY_MENU_DATA::OnKeypress(key_code key, char ascii)
{
  if (key != KC_RETURN)
  {
    HMenuVItemsKeyboardAction(key, ascii);
    return;
  }
  /*
   { "KEEP", 0 },
   { "NEXT", 0 },
   { "PREV", 0 },
   { "RESELECT", 0 }
  */
  switch (menu.currentItem()) 
  {
  case 1: // keep
    PopEvent();
    break;    
  case 2: // next
    nextSpellPage();
    break;    
  case 3: // prev
    prevSpellPage();
    break;    
  case 4: // reselect
    GetActiveChar(this).spells.UnselectIfNotMemorized();
    handleSpellFormInput(SPELLFORM_exit, spellListText);
    PopEvent();
    break;    
  default:
    WriteDebugString("Unhandled menu item in MEMORIZE_VERIFY_MENU_DATA");
    miscError = InvalidMenuState;
    handleSpellFormInput(SPELLFORM_exit, spellListText);
    PopEvent();
    break;
  } 
}

void MEMORIZE_VERIFY_MENU_DATA::OnInitialEvent(void)
{
  ClearFormattedText(textData);
  menu.setMenu(MemorizeVerifyMenuData);
  menu.setHorzOrient();
  setSpellItemState(GetActiveChar(this), GetActiveChar(this).spells, (LEVEL_FLAG | SHOW_SELECTED_FLAG));
  handleSpellFormInput(SPELLFORM_exit, spellListText);
  handleSpellFormInput(SPELLFORM_initial, spellListText);
}

void MEMORIZE_VERIFY_MENU_DATA::OnDraw(void)
{
  //UpdateMemorizeVerifyMenuScreen();
  GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);
  drawBorder();
  //displaySpells();
  if (errorTextData.numLines == 0)
    handleSpellFormInput(SPELLFORM_refresh, spellListText);
  menu.DisplayMenu();
  DisplayTextAndFlip();
}

void MEMORIZE_VERIFY_MENU_DATA::OnUpdateUI(void)
{
  /*
   { "KEEP", 0 },
   { "NEXT", 0 },
   { "PREV", 0 },
   { "RESELECT", 0 }
  */
  menu.setAllItemsActive();
  if (!levelData.zoneData.zones[levelData.area[party.Posy][party.Posx].zone].AllowMagic)
    menu.setItemInactive(1); // cast

  if (spellListText.GetCount() <= 0)
    menu.setItemInactive(1); // cast
    
  if (!allowSpellNextPage())
    menu.setItemInactive(2); // next

  if (!allowSpellPrevPage())
    menu.setItemInactive(3); // prev
}

////////////////////////////////////////////////////////////////////////TARGET_SELECT_NONCOMBAT_EVENT_DATA

TARGET_SELECT_NONCOMBAT_EVENT_DATA::TARGET_SELECT_NONCOMBAT_EVENT_DATA(CHARACTER *data) : GameEvent()
{
  Clear();   
  event=CONTROL_TargetSelectNonCombat; 
  m_caster=data; 
  if (m_caster == NULL) 
    PopEvent();
}

TARGET_SELECT_NONCOMBAT_EVENT_DATA::~TARGET_SELECT_NONCOMBAT_EVENT_DATA()
{ 
  Clear(); 
}

void TARGET_SELECT_NONCOMBAT_EVENT_DATA::Clear() 
{ 
  GameEvent::Clear(); 
  m_caster=NULL;
}

void TARGET_SELECT_NONCOMBAT_EVENT_DATA::OnInitialEvent(void)
{
  if (m_caster==NULL)
  {
    PopEvent();
    return;
  }
  if (!m_caster->targets.ValidNumTargets())
  {
    WriteDebugString("ValidNumTargets() failed in TARGET_SELECT_NONCOMBAT_EVENT_DATA init\n");
    PopEvent();
    return;
  }
  menu.setMenu(TargetSelectNonCombatMenuData);
  menu.setHorzOrient();
  CString title(m_caster->targets.FormatRemainingTargsText());
  menu.setTitle(title);
  menu.MapKeyCodeToMenuItem(KC_ESCAPE, 2);
}

void TARGET_SELECT_NONCOMBAT_EVENT_DATA::OnKeypress(key_code key, char ascii)
{
  if (TABParty(key)) return;
  if (key != KC_RETURN) 
  {
    HMenuVPartyKeyboardAction(key, ascii);
    return;
  }
  CHARACTER &dude = GetActiveChar(this); // selected target character

  switch (menu.currentItem())
  {
  case 1: // select
    {
      if (m_caster != NULL)
      {
        if (!m_caster->AddTarget(dude))
          WriteDebugString("Failed AddTarget() in TARGET_SELECT_NONCOMBAT_EVENT_DATA\n");

        if (m_caster->targets.AllTargetsChosen())
        {
          PopEvent(); // all targets chosen
          return;
        }
        CString title(m_caster->targets.FormatRemainingTargsText());
        menu.setTitle(title);
      }
    }
    break;
  case 2: // exit
    PopEvent();
    break;
  }
}

void TARGET_SELECT_NONCOMBAT_EVENT_DATA::OnDraw(void)
{
  UpdateTargetSelectNonCombatScreen();
}

void TARGET_SELECT_NONCOMBAT_EVENT_DATA::OnUpdateUI(void)
{
  menu.setAllItemsActive();
  if (m_caster == NULL)
  {
    menu.setItemInactive(1); // select
  }
  else
  {
    if (!m_caster->targets.ValidNumTargets())
      menu.setItemInactive(1); // select
    if (m_caster->targets.AllTargetsChosen())
      menu.setItemInactive(1); // select
  }
}


////////////////////////////////////////////////////////////////////////TARGET_SELECT_COMBAT_EVENT_DATA
/*
TARGET_SELECT_COMBAT_EVENT_DATA::TARGET_SELECT_COMBAT_EVENT_DATA(GameEvent *pEvent,COMBATANT *data, int key) : GameEvent()
{
  Clear();   
  event=CONTROL_TargetSelectCombat; 
  m_caster=data; 
  m_pOrigEvent=pEvent;
  m_cspellKey=key; // spell database key
  if (m_caster == NULL) 
    PopEvent();
}

TARGET_SELECT_COMBAT_EVENT_DATA::~TARGET_SELECT_COMBAT_EVENT_DATA()
{ 
  Clear(); 
}

void TARGET_SELECT_COMBAT_EVENT_DATA::Clear() 
{ 
  GameEvent::Clear(); 
  m_caster=NULL;
}

void TARGET_SELECT_COMBAT_EVENT_DATA::OnInitialEvent(void)
{
  if (m_caster==NULL)
  {
    PopEvent();
    return;
  }
  if (!m_caster->targets.ValidNumTargets())
  {
    WriteDebugString("ValidNumTargets() failed in TARGET_SELECT_COMBAT_EVENT_DATA init\n");
    PopEvent();
    return;
  }

  if (combatData.OnAuto())
  {
    //m_caster->AutoChooseSpellTargets();
    PopEvent();
    return;
  }

  menu.setMenu(TargetSelectNonCombatMenuData);
  menu.setHorzOrient();
  CString title(m_caster->targets.FormatRemainingTargsText());
  menu.setTitle(title);
}

void TARGET_SELECT_COMBAT_EVENT_DATA::OnKeypress(key_code key, char ascii)
{
  if (key != KC_RETURN) 
  {
    HMenuVPartyKeyboardAction(key, ascii);
    return;
  }

  COMBATANT &dude = combatData.getActiveCombatCharacter(); // selected target character

  switch (menu.currentItem())
  {
  case 1: // select
    {
      if (m_caster != NULL)
      {
        int targrange = 0;
        if (!m_caster->AddTarget(dude, targrange))
          WriteDebugString("Failed AddTarget() in TARGET_SELECT_COMBAT_EVENT_DATA\n");

        if (m_caster->targets.AllTargetsChosen())
        {
          PopEvent(); // all targets chosen
          return;
        }
        CString title(m_caster->targets.FormatRemainingTargsText());
        menu.setTitle(title);
      }
    }
    break;
  case 2: // exit
    PopEvent();
    break;
  }
}

void TARGET_SELECT_COMBAT_EVENT_DATA::OnDraw(void)
{
  //UpdateTargetSelectCombatScreen();
}

void TARGET_SELECT_COMBAT_EVENT_DATA::OnUpdateUI(void)
{
  menu.setAllItemsActive();
  if (m_caster == NULL)
  {
    menu.setItemInactive(1); // select
  }
  else
  {
    if (!m_caster->targets.ValidNumTargets())
      menu.setItemInactive(1); // select
    if (m_caster->targets.AllTargetsChosen())
      menu.setItemInactive(1); // select
  }
}
*/
////////////////////////////////////////////////////////////////////////CAST_MENU_DATA
void CAST_MENU_DATA::OnKeypress(key_code key, char ascii)
{
  if (key != KC_RETURN)
  {
    HMenuVItemsKeyboardAction(key, ascii);
    return;
  }
  /*
   { "CAST", 0 },
   { "NEXT", 0 },
   { "PREV", 0 },
   { "EXIT", 1 }
  */
  switch (menu.currentItem()) 
  {
  case 1: // cast
    {
      int cspell = spellListText.GetSpellBookKey(currSpellTextIndex());
      if (GetActiveChar(m_pOrigEvent).spells.isMemorized(cspell))
      {
        if ((m_pOrigEvent != NULL) && (m_pOrigEvent->GetEventType()==(eventType)Combat))
        {
          combatData.getActiveCombatCharacter().SetSpellBookKeyBeingCast(cspell);
          PopEvent();
        }
        else
          PushEvent(new CAST_NON_COMBAT_SPELL_MENU_DATA(GetEventType(), m_pOrigEvent,&GetActiveChar(m_pOrigEvent), cspell), DeleteEvent);
      }
    }
    break;    
  case 2: // next
    nextSpellPage();
    break;    
  case 3: // prev
    prevSpellPage();
    break;    
  case 4: // Exit
    handleSpellFormInput(SPELLFORM_exit, spellListText);
    PopEvent();
    break;    
  default:
    WriteDebugString("Unhandled menu item in CAST_MENU_DATA");
    miscError = InvalidMenuState;
    handleSpellFormInput(SPELLFORM_exit, spellListText);
    PopEvent();
    break;
  }  
}

void CAST_MENU_DATA::OnInitialEvent(void)
{
  if (!GetActiveChar(m_pOrigEvent).canCastSpells())
  {
    PopEvent();
    return;
  }
  ClearFormattedText(textData);
  menu.setMenu(CastMenuData);
  menu.setHorzOrient();
  menu.MapKeyCodeToMenuItem(KC_ESCAPE, 4);
  SetMyState(TASK_CastSpellSpellSelect);
  setSpellCastItemState(GetActiveChar(m_pOrigEvent));
  handleSpellFormInput(SPELLFORM_exit, spellListText);
  handleSpellFormInput(SPELLFORM_initial, spellListText);
}

void CAST_MENU_DATA::OnReturnToTopOfQueue()
{
  OnInitialEvent();
}

void CAST_MENU_DATA::OnDraw(void)
{
  GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);
  drawBorder();
  if (errorTextData.numLines == 0)
    handleSpellFormInput(SPELLFORM_refresh, spellListText);
  menu.DisplayMenu();
  DisplayTextAndFlip();
}

void CAST_MENU_DATA::OnUpdateUI(void)
{
  /*
   { "CAST", 0 },
   { "NEXT", 0 },
   { "PREV", 0 },
   { "EXIT", 1 }
  */
  menu.setAllItemsActive();
  int zone = levelData.GetCurrZone(party.Posx,party.Posy);
  if (!levelData.zoneData.zones[zone].AllowMagic)
    menu.setItemInactive(1); // cast

  if (spellListText.GetCount() <= 0)
    menu.setItemInactive(1); // cast
    
  if (!allowSpellNextPage())
    menu.setItemInactive(2); // next

  if (!allowSpellPrevPage())
    menu.setItemInactive(3); // prev
}

////////////////////////////////////////////////////////////////////////CAST_NON_COMBAT_SPELL_MENU_DATA
//
// When not in combat, casting and targeting handled and
// then the actual casting takes place. There is no
// need for spell animations (just the casting sound)
//
void CAST_NON_COMBAT_SPELL_MENU_DATA::OnInitialEvent(void)
{
  ClearFormattedText(textData);
  menu.reset();

  // selecting spell targets will change party.activeCharacter
  tempActive=party.activeCharacter;
  if (tempCASTER==NULL)
  {
    WriteDebugString("NULL caster in CAST_NON_COMBAT_SPELL_MENU_DATA\n");
    PopEvent();
    return;
  }

  CHARACTER_SPELL spell;
  if (!tempCASTER->spells.list.GetSpell(m_cspellKey, spell))
  {
    WriteDebugString("bogus cspell key %u in CAST_NON_COMBAT_SPELL_MENU_DATA\n", m_cspellKey);
    PopEvent();
    return;
  }
  m_spellKey = spell.spell;

  if (!spellData.HaveSpell(m_spellKey))
  {
    WriteDebugString("bogus spell key %u in CAST_NON_COMBAT_SPELL_MENU_DATA\n", m_spellKey);
    PopEvent();
    return;    
  }

  SPELL_DATA *pSpData = spellData.GetSpellData(m_spellKey);

  if (pSpData==NULL)
  {
    WriteDebugString("NULL ptr to spell db data in CAST_NON_COMBAT_SPELL_MENU_DATA\n");
    PopEvent();
    return;    
  }

  if (pSpData->Combat_Only)
  {
    WriteDebugString("combat only spell %s in CAST_NON_COMBAT_SPELL_MENU_DATA\n", pSpData->Name);
    PopEvent();
    return;    
  }

  double result;
  result = tempCASTER->EvalDiceAsClass(pSpData->NumTargets, pSpData->ClassMask);
  DWORD targs = result;
  result = tempCASTER->EvalDiceAsClass(pSpData->TargetRange, pSpData->ClassMask);
  DWORD range = result;
  tempCASTER->InitTargeting(pSpData->Targeting, 
                           pSpData->Friendly_Fire_Ok, 
                           tempCASTER->IsPartyMember(),
                           targs, range);

  if (NeedSpellTargeting(pSpData))
  {
    if (targs == 0) WriteDebugString("Spell %s targets eval to 0 in CAST_NON_COMBAT_SPELL_MENU_DATA\n", pSpData->Name);
    PushEvent(new TARGET_SELECT_NONCOMBAT_EVENT_DATA(tempCASTER), DeleteEvent);
    return;
  }
  else
  {        
    if (pSpData->Targeting == Self)
    {          
      if (!tempCASTER->AddTargetSelf())
        WriteDebugString("Failed AddTargetSelf() in CAST_NON_COMBAT_SPELL_MENU_DATA\n");
    }
    else
    {
      // can be whole party, or area if combat not active
      for (int i=0;i<party.numCharacters;i++)
      {
        if (!tempCASTER->AddTarget(party.characters[i]))
          WriteDebugString("Failed AddTarget(<party member %i>) in CAST_NON_COMBAT_SPELL_MENU_DATA\n", i);
      }
    }
    if (tempCASTER->targets.NumTargets() > 0)
      tempCASTER->CastSpell(m_cspellKey);
    else
      WriteDebugString("No spell targets in CAST_NON_COMBAT_SPELL_MENU_DATA, not casting spell\n");
  }
  party.activeCharacter = tempActive;
  PopEvent();
}

void CAST_NON_COMBAT_SPELL_MENU_DATA::OnReturnToTopOfQueue()
{
  if ((tempCASTER!=NULL) && (tempCASTER->targets.NumTargets() > 0))
    tempCASTER->CastSpell(m_cspellKey);
  else
    WriteDebugString("No targets in CAST_NON_COMBAT_SPELL_MENU_DATA after target select menu, not casting\n");
  party.activeCharacter = tempActive;
  PopEvent();
}

void CAST_NON_COMBAT_SPELL_MENU_DATA::OnDraw(void)
{
  switch (m_origEventType)
  {
  case CONTROL_CastMenu:
    {
      GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);
      drawBorder();
      if (errorTextData.numLines == 0)
        handleSpellFormInput(SPELLFORM_refresh, spellListText);
      menu.DisplayMenu();
      DisplayTextAndFlip();
    }
    break;

  case GiveTreasure:
  case CombatTreasure:
    UpdateGiveTreasureScreen();
    break;
  }
}

////////////////////////////////////////////////////////////////////////CAST_COMBAT_SPELL_MENU_DATA
// When in combat, spell casting is begun and there is a
// time delay for the casting time. 
//
void CAST_COMBAT_SPELL_MENU_DATA::OnInitialEvent(void)
{
  ClearFormattedText(textData);
  menu.reset();
  
  if (tempCASTER==NULL)
  {
    WriteDebugString("NULL caster in CAST_COMBAT_SPELL_MENU_DATA\n");
    PopEvent();
    return;
  }

  if (!tempCASTER->canCastSpells())
  {
    PopEvent();
    return;
  }

  CHARACTER_SPELL spell;
  if (!tempCASTER->spells.list.GetSpell(m_cspellKey, spell))
  {
    WriteDebugString("bogus cspell key %u in CAST_COMBAT_SPELL_MENU_DATA\n", m_cspellKey);
    PopEvent();
    return;
  }
  m_spellKey = spell.spell;

  if (!spellData.HaveSpell(m_spellKey))
  {
    WriteDebugString("bogus spell key %u in CAST_COMBAT_SPELL_MENU_DATA\n", m_spellKey);
    PopEvent();
    return;    
  }

  SPELL_DATA *pSpData = spellData.GetSpellData(m_spellKey);

  if (pSpData==NULL)
  {
    WriteDebugString("NULL ptr to spell db data in CAST_COMBAT_SPELL_MENU_DATA\n");
    PopEvent();
    return;    
  }

  double result;
  result = tempCASTER->EvalDiceAsClass(pSpData->NumTargets, pSpData->ClassMask);
  DWORD targs = result;
  result = tempCASTER->EvalDiceAsClass(pSpData->TargetRange, pSpData->ClassMask);
  DWORD range = result;
  tempCASTER->InitTargeting(pSpData->Targeting, 
                           pSpData->Friendly_Fire_Ok, 
                           tempCASTER->GetIsFriendly(),
                           targs, range);

  if (NeedSpellTargeting(pSpData))
  {
    if (targs == 0) WriteDebugString("Spell %s targets eval to 0 in CAST_COMBAT_SPELL_MENU_DATA\n", pSpData->Name);
      ReplaceEvent(new COMBAT_SPELL_AIM_MENU_DATA(tempCASTER), DeleteEvent);
    return;
  }
  else // whole party or self targeting
  {        
    if (pSpData->Targeting == Self)
    {          
      if (!tempCASTER->AddTargetSelf())
        WriteDebugString("Failed AddTargetSelf() in CAST_COMBAT_SPELL_MENU_DATA\n");
    }
    else
    {
      // can only be whole party
      for (int i=0;i<combatData.numCombatants;i++)
      {
        if (combatData.combatants[i].IsPartyMember())
        {
          if (!tempCASTER->AddTarget(combatData.combatants[i]))
            WriteDebugString("Failed AddTarget(<party member %i>) in CAST_COMBAT_SPELL_MENU_DATA\n", i);
        }
      }
    }
  }
  PopEvent();
}

void CAST_COMBAT_SPELL_MENU_DATA::OnReturnToTopOfQueue()
{
  ASSERT(FALSE);
}

void CAST_COMBAT_SPELL_MENU_DATA::OnDraw(void)
{
}

////////////////////////////////////////////////////////////////////////MAGIC_MENU_DATA
void MAGIC_MENU_DATA::OnKeypress(key_code key, char ascii)
{  
  if (key != KC_RETURN) 
  {
    if (TABParty(key)) return;
    //StandardMenuKeyboardAction(key,ascii);
    HMenuVPartyKeyboardAction(key,ascii);
    return;
  };

  switch (menu.currentItem()) 
  {
  case 1: // cast
    PushEvent(new CAST_MENU_DATA(m_pOrigEvent), DeleteEvent);
    break;    
  case 2: // memorize
    PushEvent(new MEMORIZE_MENU_DATA, DeleteEvent);
    break;    
  case 3: // scribe    
    //PushEvent(new SCRIBE_MENU_DATA, DeleteEvent);
    break;    
  case 4: // display
    PushEvent(new DISPLAY_SPELL_EFFECTS_MENU_DATA(m_pOrigEvent), DeleteEvent);
    break;
  case 5: // rest
    PushEvent(new REST_MENU_DATA(m_pOrigEvent), DeleteEvent);
    break;
  case 6: // exit
    PopEvent();
    break;
  default:
    WriteDebugString("Unhandled menu item in MAGIC_MENU_DATA");
    miscError = InvalidMenuState;
    PopEvent();
    break;
  }  
}

void MAGIC_MENU_DATA::OnInitialEvent(void)
{
  ClearFormattedText(textData);
  menu.setMenu(MagicMenuData);
  menu.setHorzOrient();
  menu.MapKeyCodeToMenuItem(KC_ESCAPE, 6);
}

void MAGIC_MENU_DATA::OnDraw(void)
{
  if (   (m_pOrigEvent != NULL) 
      && (m_pOrigEvent->GetEventType() == (eventType)Combat))
  {
    UpdateCombatMagicMenuScreen();
  }
  else
  {
    UpdateMagicMenuScreen();
  }
}

void MAGIC_MENU_DATA::OnUpdateUI(void)
{
   //1 { "CAST", 0 },
   //2 { "MEMORIZE", 0 },
   //3 { "SCRIBE", 0 },
   //4 { "DISPLAY", 0 },
   //5 { "REST", 0 },
   //6 { "EXIT", 1 }

  int zone = levelData.GetCurrZone(party.Posx, party.Posy);

  if (!levelData.zoneData.zones[zone].AllowMagic)
  {
    menu.setItemInactive(1);
    menu.setItemInactive(2);
    menu.setItemInactive(3);
    menu.setItemInactive(5);
  }
  
  if (!levelData.zoneData.zones[zone].restEvent.allowResting)
  {
    menu.setItemInactive(5); // rest
  }

  menu.setAllItemsActive();
  if (   (m_pOrigEvent != NULL) 
      && (m_pOrigEvent->GetEventType() == (eventType)Combat))
  {
    menu.setItemInactive(2);
    menu.setItemInactive(3);
    menu.setItemInactive(5);
  }
  else
  {
    if (!GetActiveChar(m_pOrigEvent).canCastSpells()) 
    {
      menu.setItemInactive(1);
      menu.setItemInactive(2);
    }

  }
}

void MAGIC_MENU_DATA::OnReturnToTopOfQueue()
{
  OnInitialEvent();
}


////////////////////////////////////////////////////////////////////////APPRAISE_SELECT_DATA
void APPRAISE_SELECT_DATA::OnKeypress(key_code key, char ascii)
{
  if (key != KC_RETURN)
  {
    StandardMenuKeyboardAction(key,ascii);
    return;
  }
  /*
   { "GEMS", 0 },
   { "JEWELRY", 0 },
   { "EXIT", 1 }
  */
  int value;
  switch (menu.currentItem()) 
  {
  case 1: // Gems
    m_pMoney->RemoveMultGems(1);
    value = globalData.moneyData.GetGemValue();
    PushEvent(new APPRAISE_EVALUATE_DATA(GemType, globalData.moneyData.GetName(GemType), value), DeleteEvent);    
    break;    
  case 2: // Jewelry
    m_pMoney->RemoveMultJewelry(1);
    value = globalData.moneyData.GetJewelryValue();
    PushEvent(new APPRAISE_EVALUATE_DATA(JewelryType,globalData.moneyData.GetName(JewelryType), value), DeleteEvent);    
    break;    
  case 3: // Exit
    PopEvent();
    break;    
  default:
    WriteDebugString("Unhandled menu item in APPRAISE_SELECT_DATA");
    miscError = InvalidMenuState;
    PopEvent();
    break;
  } 
}

void APPRAISE_SELECT_DATA::OnInitialEvent(void)
{
  ClearFormattedText(textData);
  menu.setMenu(AppraiseSelectMenuData);
  menu.changeMenuItem(1, globalData.moneyData.GetName(GemType));
  menu.changeMenuItem(2, globalData.moneyData.GetName(JewelryType));
  menu.setHorzOrient();
  menu.MapKeyCodeToMenuItem(KC_ESCAPE, 3);
  CString msg;
  msg.Format("YOU HAVE\n%u %s\n%u %s\nNOT YET APPRAISED",
             m_pMoney->NumGems(),
             globalData.moneyData.GetName(GemType),
             m_pMoney->NumJewelry(),
             globalData.moneyData.GetName(JewelryType));            

  FormatDisplayText(textData, msg, FALSE, FALSE);
}

void APPRAISE_SELECT_DATA::OnDraw(void)
{
  UpdateAppraiseSelectMenuScreen();
}

void APPRAISE_SELECT_DATA::OnUpdateUI(void)
{
  /*
   { "GEMS", 0 },
   { "JEWELRY", 0 },
   { "EXIT", 1 }
  */   
  menu.setAllItemsActive();

  if (m_apprGems==FALSE)
    menu.setItemInactive(1); // gems
  if (m_pMoney->NumGems()==0)
    menu.setItemInactive(1); // gems

  if (m_apprJewelry==FALSE)
    menu.setItemInactive(2); // jewelry
  if (m_pMoney->NumJewelry()==0)
    menu.setItemInactive(2); // jewelry
}

void APPRAISE_SELECT_DATA::OnReturnToTopOfQueue(void)
{
  OnInitialEvent();
  if (party.tradeItem==1) // sell
  {
    GetActiveChar(this).money.Add(globalData.moneyData.GetDefaultType(), 
                                  party.tradeQty);
  }
  else // keep
  {
    itemClassType type = (itemClassType)party.tradeGiver;
    if (type == GemType)
    {
      GetActiveChar(this).addCharacterItem(ITEM_LIST::GemItem,1,party.tradeQty,FALSE,-1);
    }
    else if (type == JewelryType)
    {
      GetActiveChar(this).addCharacterItem(ITEM_LIST::JewelryItem,1,party.tradeQty,FALSE,-1);
    }
  }
}

////////////////////////////////////////////////////////////////////////APPRAISE_EVALUATE_DATA
void APPRAISE_EVALUATE_DATA::OnKeypress(key_code key, char ascii)
{
  if (key != KC_RETURN)
  {
    StandardMenuKeyboardAction(key,ascii);
    return;
  }
  /*
   { "SELL", 0 },
   { "KEEP", 0 },
  */
  switch (menu.currentItem()) 
  {
  case 1: // Sell
    party.tradeItem=1;
    party.tradeQty = m_value;
    break;    
  case 2: // Keep
    party.tradeItem=0;
    party.tradeQty = m_value;
    break;    
  default:
    WriteDebugString("Unhandled menu item in APPRAISE_EVALUATE_DATA");
    miscError = InvalidMenuState;
    break;
  } 
  PopEvent();
}

void APPRAISE_EVALUATE_DATA::OnInitialEvent(void)
{
  ClearFormattedText(textData);
  menu.setMenu(AppraiseEvaluateMenuData);
  menu.setHorzOrient();
  party.tradeItem=0;
  party.tradeQty=0;
  party.tradeGiver = m_type;
  CString msg;
  msg.Format("THIS %s IS VALUED AT %u %s",
             m_name,
             m_value,
             globalData.moneyData.GetName(globalData.moneyData.GetDefaultType()));
  FormatDisplayText(textData, msg, FALSE, FALSE);
}

void APPRAISE_EVALUATE_DATA::OnDraw(void)
{
  UpdateAppraiseEvaluateMenuScreen();
}

////////////////////////////////////////////////////////////////////////ASK_CUSTOM_MENU_DATA
void ASK_CUSTOM_MENU_DATA::OnKeypress(key_code key, char ascii)
{

  switch (m_OnDrawType)
  {
  case CONTROL_Adventure:
  case ShopEvent:
  case TempleEvent:
  case Vault:
  case GiveTreasure:
  case CONTROL_Encamp:  if (TABParty(key)) return;
  };
  
  if (key != KC_RETURN)
  {
    HMenuVPartyKeyboardAction(key,ascii);
    return;
  }

  switch (menu.currentItem()) 
  {
  case 1: // option 1
    party.tradeItem=1;
    break;    
  case 2: // option 2
    party.tradeItem=2;
    break;    
  default:
    WriteDebugString("Unhandled menu item in ASK_CUSTOM_MENU_DATA");
    miscError = InvalidMenuState;
    break;
  } 

  ClearFormattedText(textData);
  PopEvent();
}

void ASK_CUSTOM_MENU_DATA::OnInitialEvent(void)
{
  ClearFormattedText(textData);
  menu.reset();
  menu.setStartCoord(USE_DEFAULT_HORZ_MENU, 0);
  menu.setTitle(m_question);
  menu.setHorzOrient();
  //menu.setItemSeparation();
  
  menu.addMenuItem(m_option1);
  if (m_option2.GetLength() > 0)
    menu.addMenuItem(m_option2);
  
  menu.SetFirstLettersShortcuts();
  menu.setCurrentItem(0);
  party.tradeItem=0;  
}

void ASK_CUSTOM_MENU_DATA::OnDraw(void)
{
  switch (m_OnDrawType)
  {
  case CONTROL_Adventure: UpdateAdventureScreen(); break;
  case CONTROL_ItemsMenu: 
    GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);
    drawBorder();
    if (errorTextData.numLines == 0)
      handleItemsFormInput(ITEMSFORM_refresh, charItemListText);
    menu.DisplayMenu();
    DisplayTextAndFlip();
    break;
  case ShopEvent: UpdateShopScreen(); break;
  case TempleEvent: UpdateTempleScreen(); break;
  case Vault: UpdateVaultScreen(); break;
  case GiveTreasure: UpdateGiveTreasureScreen(); break;
  case CONTROL_Encamp: UpdateEncampScreen(); break;
  case CONTROL_CombatSpellAimAuto: UpdateCombatAimScreen(); break;

  case CONTROL_CreateCharacter:
  default: UpdateAskYesNoMenuScreen(); break;
  }
}

////////////////////////////////////////////////////////////////////////ASK_YES_NO_MENU_DATA
void ASK_YES_NO_MENU_DATA::OnKeypress(key_code key, char ascii)
{

  switch (m_OnDrawType)
  {
  case CONTROL_Adventure:
  case ShopEvent:
  case TempleEvent:
  case Vault:
  case GiveTreasure:
  case CONTROL_Encamp:  if (TABParty(key)) return;
  };
  
  if (key != KC_RETURN)
  {
    HMenuVPartyKeyboardAction(key,ascii);
    return;
  }

  if (m_IsYesNo)
  {
    /*
     { "YES", 0 },
     { "NO", 0 },
    */
    switch (menu.currentItem()) 
    {
    case 1: // Yes
      party.tradeItem=1;
      break;    
    case 2: // No
      party.tradeItem=0;
      break;    
    default:
      WriteDebugString("Unhandled menu item in ASK_YES_NO_MENU_DATA");
      miscError = InvalidMenuState;
      break;
    } 
  }
  else // 'Press Enter To Continue'
  {
    party.tradeItem=0;
  }

  ClearFormattedText(textData);
  PopEvent();
}

void ASK_YES_NO_MENU_DATA::OnInitialEvent(void)
{
  ClearFormattedText(textData);
  if (m_IsYesNo)
    menu.setMenu(QuestionYesNoData);
  else
    menu.setMenu(PressEnterMenuData);

  menu.setHorzOrient();
  party.tradeItem=0;  

  switch (m_OnDrawType)
  {
  case CONTROL_CombatMove:
  case CONTROL_CombatSpellAimAuto:
    menu.setTitle(m_question);
    break;

  default: FormatDisplayText(textData, m_question); break;
  }
}

void ASK_YES_NO_MENU_DATA::OnDraw(void)
{
  switch (m_OnDrawType)
  {
  case CONTROL_Adventure: UpdateAdventureScreen(); break;
  case CONTROL_ItemsMenu: 
    GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);
    drawBorder();
    if (errorTextData.numLines == 0)
      handleItemsFormInput(ITEMSFORM_refresh, charItemListText);
    menu.DisplayMenu();
    DisplayTextAndFlip();
    break;
  case ShopEvent: UpdateShopScreen(); break;
  case TempleEvent: UpdateTempleScreen(); break;
  case Vault: UpdateVaultScreen(); break;
  case GiveTreasure: UpdateGiveTreasureScreen(); break;
  case CONTROL_Encamp: UpdateEncampScreen(); break;
  case CONTROL_CombatSpellAimAuto: UpdateCombatAimScreen(); break;
  case CONTROL_CombatMove: UpdateCombatMoveScreen(); break;

  case CONTROL_CreateCharacter:
  default: UpdateAskYesNoMenuScreen(); break;
  }
}


////////////////////////////////////////////////////////////////////////DESIGN_SELECT_MENU_DATA
void DESIGN_SELECT_MENU_DATA::OnKeypress(key_code key, char ascii)
{
  if (key != KC_RETURN)
  {
    HMenuVItemsKeyboardAction(key, ascii);
    return;
  }


  if (GetMyState() == TASK_DisplayDesigns)
  {
    /*
     { "SELECT", 0 },
     { "NEXT", 0 },
     { "PREV", 0 },
     { "EXIT", 1 }
    */
    switch (menu.currentItem()) 
    {
    case 1: // select
      {
        int dindex = spellListText.GetSpellIndex(currSpellTextIndex());
        CString designPath = m_designs[dindex].path;
        strcpy(m_fileDir, designPath);
        strcpy(m_installDir, m_fileDir);
        strcpy(m_designFolder, m_fileDir);

        //WriteDebugString("Working directory = %s\n", m_fileDir);
      
        CString filename;
        filename.Format("%sconfig.txt", m_designFolder);

        if (!LoadConfigFile(filename))
        {
          WriteDebugString("Failed to load config.txt file in InitInstance()\n");
        }
        else
        {
          // pops all events, including this one, from stack
          // if design loaded ok.
          //
          InitGame(AfxGetMainWnd()->GetSafeHwnd()); 
        }
      }
      break;    
    case 2: // next
      nextSpellPage();
      break;
    case 3: // prev
      prevSpellPage();
      break;
    case 4: // exit app
      PopEvent(); // pop to exit event
      break;

    default:
      WriteDebugString("Unhandled menu item in DESIGN_SELECT_MENU_DATA");
      break;
    }   
  }
}

void DESIGN_SELECT_MENU_DATA::OnInitialEvent(void)
{
  //WriteDebugString("Beginning search for all designs...\n");
  ClearFormattedText(textData);
  menu.setMenu(SelectMenuData);
  menu.setHorzOrient();
  SetMyState(TASK_SearchForDesigns);

  char fullPath[_MAX_PATH+1];
  GetModuleFileName(NULL, fullPath, _MAX_PATH);
  CString location = fullPath;
  m_Drive = GetFilenameDrive(location);
  m_Drive.MakeUpper();

  OnDraw();
  if (!AddDesigns())
    PopEvent();
}

void DESIGN_SELECT_MENU_DATA::OnDraw(void)
{
  int StartX = 18;
  int StartY = 38;
  int MaxCharHeight = GraphicsMgr.GetMaxCharacterHeight();

  GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);

  if (GetMyState() == TASK_DisplayDesigns)
  {   
    int totalListItems = spellListText.GetCount();
    if (totalListItems == 0)
    {
      CString tmp;
      tmp.Format("NO DESIGNS AVAILABLE ON DRIVE %s, PRESS ENTER TO EXIT", m_Drive);
      displayText(StartX, StartY, tmp);
      menu.DisplayMenu(); 
      DisplayTextAndFlip();
      return;
    }

    displayText(StartX, StartY, spellListText.title);
    int currItem = startItem;
    int currY = StartY+MaxCharHeight+5;
    CString listtext;
    CString dname;

    while (currItem < totalListItems)
    {
      int d_index = spellListText.GetSpellIndex(currItem);    
      dname = m_designs[d_index].path;
      StripFilenamePath(dname);

      listtext.Format("%6.4f     %s   (%s)",
                  m_designs[d_index].ver,
                  m_designs[d_index].name,
                  dname);

      displayText(StartX, currY, listtext , White, shouldHighlight(currItem));
  
      if (totalListItems > Items_Per_Page)
      {
         if (! ((currItem+1) % Items_Per_Page) )
         {
            currY=StartY+MaxCharHeight+5;
            break;
         }
      }
      currItem++;
      currY += MaxCharHeight;
    }

    menu.DisplayMenu();  
  }
  else
  {
    static int tick=0;
    CString tmp;
    tmp.Format("PLEASE WAIT, SEARCHING FOR AVAILABLE DESIGNS ON DRIVE %s (ESC TO QUIT)", m_Drive);
    displayText(StartX, StartY, tmp);
    displayText(StartX, StartY+MaxCharHeight+5, "                           ");

    switch (tick)
    {
    case 0:tmp.Format("| DESIGNS FOUND: %u", NumDesigns()); break;
    case 1:tmp.Format("/ DESIGNS FOUND: %u", NumDesigns()); break;
    case 2:tmp.Format("- DESIGNS FOUND: %u", NumDesigns()); break;
    case 3:tmp.Format("\\ DESIGNS FOUND: %u", NumDesigns()); break;
    }
    
    displayText(StartX, StartY+MaxCharHeight+5, tmp);
    tick += 1; if (tick>3)tick=0;

  }
  DisplayTextAndFlip();
}

void DESIGN_SELECT_MENU_DATA::OnUpdateUI(void)
{
  menu.setAllItemsActive();
  if (spellListText.GetCount() <= 0)
    menu.setItemInactive(1); // select    
  if (!allowSpellNextPage())
    menu.setItemInactive(2); // next
  if (!allowSpellPrevPage())
    menu.setItemInactive(3); // prev
}

void DESIGN_SELECT_MENU_DATA::GetDesignVersion(const char *fname, double &ver, CString &name)
{
  CFile myFile;
  ver=0.0;
  name="";
  
  try
  {
    if (!myFile.Open(fname, CFile::modeRead))
    {
      WriteDebugString("Open Error: Unable to determine design version from file %s\n", fname);
      return;
    }   

    CArchive ar(&myFile, CArchive::load);  
    ar >> ver;
    DAS(ar,name);
//    ar >> name;
//    if (name == "*")
//      name="";
    ar.Close();
  }
  catch (...)
  {
    WriteDebugString("Unable to determine design version from file %s\n", fname);
  }

  if (ver <= 0.0)
    WriteDebugString("Error %u trying to determine design version from file %s\n", 
           GetLastError(), fname);
}

void DESIGN_SELECT_MENU_DATA::AddAllDesignsInFolder(CString &path)
{
  char fullPath[_MAX_PATH+1];
  CString dsnpath;

  if (path[path.GetLength()-1] != '\\')
    path += "\\";

  strcpy(fullPath, path);
  strcat(fullPath, "game.dat");
 
  //WriteDebugString("Adding designs from folder %s\n", path);

  WIN32_FIND_DATA findData;
  HANDLE hSearch = FindFirstFile(fullPath, &findData);

  if (hSearch != INVALID_HANDLE_VALUE)
  {
    do 
    {
 //     if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
 //     {
        if (strlen(findData.cFileName) > 0)
        {
          dsnpath = path;
 //         fullpath += findData.cFileName;
 //         if (fullpath[fullpath.GetLength()-1] != '\\')
 //           fullpath += "\\";

		  if (path.Find("\\BACKUP\\") == -1)	// Check if this is the design
												// backup directory. If it is, ignore it when
												// checking for designs.
		  {
			if (DesignAvail(path))
			{
				CString gamedat;
				gamedat.Format("%sgame.dat", path);
				double ver;
				CString temp;
				GetDesignVersion(gamedat, ver, temp);
				if (ver != 0.0)
				{
					DESIGN_DETAIL data;
					data.name = temp;
					data.path = dsnpath;
					data.ver = ver;
					//WriteDebugString("Adding design %s\n", fullpath);
					AddDesign(data);
					OnDraw();
				}
			}
        }
      }

      ExitSignaled = GetAsyncKeyState(VK_ESCAPE);
    } while ((!ExitSignaled) && (FindNextFile(hSearch, &findData)));
  }

  FindClose(hSearch);
}

BOOL DESIGN_SELECT_MENU_DATA::SearchForDesignFolders(const CString &path)
{
  if (path.GetLength() <= 0) return FALSE;

  CFileFind finder;
  CString subFolder="";
  CString searchPath(path);

  if (searchPath[searchPath.GetLength()-1] != '\\')
    searchPath += "\\";
  CString StartSubFolder(searchPath);

  searchPath += "*.*";
  BOOL found = finder.FindFile(searchPath);
  
  while ((found) && (!ExitSignaled))
  {
    found = finder.FindNextFile();

    if (finder.IsDots())
       continue;

    if (finder.IsDirectory())
    {
      OnDraw();
      CString filepath(finder.GetFilePath());
      filepath.MakeUpper();

      //WriteDebugString("Found folder %s\n", filepath);

      if (filepath.Find(".DSN") != -1) // design folder found
        AddAllDesignsInFolder(filepath);

      // recurse into sub-directory
      subFolder = StartSubFolder;
      // add sub folder onto path
      subFolder += finder.GetFileName();
      SearchForDesignFolders(subFolder);
      ExitSignaled = GetAsyncKeyState(VK_ESCAPE);
    }
  }
  return FALSE;
}

BOOL DESIGN_SELECT_MENU_DATA::AddDesigns()
{
  BOOL success = TRUE;

  //CString installDir = m_Drive;
  //if (installDir[installDir.GetLength()-1] != '\\')
  //  installDir += "\\";

  m_designs.RemoveAll();
  //AddAllDesignsInFolder(installDir);
  //SearchForDesignFolders(installDir);
  CString ExePath = m_fileDir;
  CString OneUpFromExePath;
  if (ExePath.GetLength() > 3)
    StripOneFolderFromFilenamePath(ExePath, OneUpFromExePath);

  SearchForDesignFolders(OneUpFromExePath); 

  for (int p=0; 
        (p < 5) && (NumDesigns()==0) && (OneUpFromExePath.GetLength()>3); 
          p++)
  {
    StripOneFolderFromFilenamePath(OneUpFromExePath, OneUpFromExePath);
    SearchForDesignFolders(OneUpFromExePath); 
  }

  // use list of designs to prepare text listing
  // for user to select from
  //if (success)
  {
    clearSpellItemState();
    spellListText.FormatSpellListTitle("SELECT A DESIGN TO PLAY");

    spellListText.m_flags = SHOW_DESIGN_FLAG;  
    for (int i=0;i<NumDesigns();i++)
      spellListText.Add(i);
    
    int scnt = spellListText.GetCount();
    ItemsOnPage = min(Items_Per_Page, scnt);
    if (ItemsOnPage > 0)
    {
      if (party.activeItem >= spellListText.GetCount())
        party.activeItem = (BYTE)(spellListText.GetCount()-1);
      if (party.activeItem >= (startItem+ItemsOnPage))
        party.activeItem = (BYTE)((startItem+ItemsOnPage)-1);
      if (party.activeItem < 0)
        party.activeItem = 0;
    }
    else
    {
      party.activeItem = 0;
      startItem = 0;
      //currShopItem = 0;
    }
  }
  SetMyState(TASK_DisplayDesigns);
  return success;
}

////////////////////////////////////////////////////////////////////////PLAY_MOVIE_DATA

void PLAY_MOVIE_DATA::OnInitialEvent(void)
{
  if (m_filename.IsEmpty())
  {
    ChainHappened();
    return;
  }
  menu.setMenu(PressEnterMenuData);
  menu.setHorzOrient();
  currPic.Clear();
  //ClearFormattedText(textData);

  RECT *pRect = NULL;
  switch (m_mode)
  {
  case movieFull:
    // do nothing, no rect means fullscreen playback
    break;

  case movieBig:
    pRect = new RECT;
    pRect->left=BigPic_x;
    pRect->top=BigPic_y;
    pRect->right=pRect->left+BigPic_w;
    pRect->bottom=pRect->top+BigPic_h;
    break;

  case movieSmall:
    pRect = new RECT;
    pRect->left=ViewportX;
    pRect->top=ViewportY;
    pRect->right=pRect->left+ViewportWidth;
    pRect->bottom=pRect->top+ViewportHeight;
    break;
  }

  if (pRect!=NULL)
  {
    if (   ((pRect->bottom - pRect->top) < 100)
        || ((pRect->right - pRect->left) < 100))
    {
      // something is wrong with screen coords, use
      // fullscreen
      delete pRect;
      pRect=NULL;
    }
  }

  switch (m_mode)
  {
  case movieFull:
    // do nothing, fullscreen playback
    break;
  case movieBig:
    UpdateMovieBigPicWindow();
    break;
  case movieSmall:
    UpdateMovieSmallPicWindow();
    break;
  }

  if (!GraphicsMgr.PlayMovie(m_filename, pRect, (pRect==NULL))) // NULL triggers fullscreen
  {
    if (pRect!=NULL) delete pRect;
    GraphicsMgr.StopMovie();
    ChainHappened(); // movie failed to load
    return;
  }

  if (pRect!=NULL) delete pRect;
  if (pSndMgr) pSndMgr->MuteVolume();
}

void PLAY_MOVIE_DATA::OnKeypress(key_code key, char ascii)
{
  // while the movie is playing, the game screen
  // area outside of the movie window is not updated.
  //if (m_mode == movieSmall)
  //  if (TABParty(key)) return;
  if (m_mode != movieFull)
  {
    if (key != KC_RETURN) 
      return;
  }

  // stop movie and exit event
  GraphicsMgr.StopMovie();
  if (pSndMgr) pSndMgr->UnMuteVolume();
  ChainHappened();
}

void PLAY_MOVIE_DATA::OnDraw(void)
{
  OnIdle();
}

bool PLAY_MOVIE_DATA::OnIdle(void)
{
  // draw next movie frame on front buffer
  if (!GraphicsMgr.UpdateMovie())
  {
    GraphicsMgr.StopMovie();
    if (pSndMgr) pSndMgr->UnMuteVolume();
    ChainHappened();
  }
  else
  {
    switch (m_mode)
    {
    case movieFull:
      // do nothing, fullscreen playback
      break;
    case movieBig:
      //UpdateMovieBigPicWindow();
      break;
    case movieSmall:
      //UpdateMovieSmallPicWindow();
      break;
    }
  }
  return true;
}

////////////////////////////////////////////////////////////////////////JOURNAL_EVENT

void JOURNAL_EVENT::OnInitialEvent(void)
{
  if (journal_entry < 0)
  {
    ChainHappened();
    return;
  }

  JOURNAL_ENTRY data;
  if (!globalData.journalData.Get(journal_entry, data))
  {
    ChainHappened();
    return;
  }

  data.origentry = data.entry; // save original journal key
  
  char *buffer=NULL;
  // substitute any ^ tokens with proper text before adding
  // text to journal
  PreProcessText(data.text, &buffer);
  data.text = buffer;
  if (buffer != NULL) delete [] buffer;

  party.journal.Add(data);
  ChainHappened();
}

////////////////////////////////////////////////////////////////////////DISPLAY_PARTY_JOURNAL_DATA

void DISPLAY_PARTY_JOURNAL_DATA::OnKeypress(key_code key, char ascii)
{
  if (key != KC_RETURN)
  {
    StandardMenuKeyboardAction(key,ascii);
    return;
  }
  switch (menu.currentItem()) 
  {
  case 1: // next
    if (!journalTextData.IsLastJournalBox())
      journalTextData.NextJournalBox();
    break;    
  case 2: // prev
    if (!journalTextData.IsFirstJournalBox())
      journalTextData.PrevJournalBox();
    break;    
  case 3: // first
    journalTextData.FirstJournalBox();
    break;
  case 4: // last
    journalTextData.LastJournalBox();
    break;
  case 5: // exit
    PopEvent();
    break;    
  default:
    WriteDebugString("Unhandled menu item in DISPLAY_PARTY_JOURNAL_DATA");
    miscError = InvalidMenuState;
    PopEvent();
    break;
  } 
}

void DISPLAY_PARTY_JOURNAL_DATA::OnInitialEvent(void)
{
  ClearFormattedText(textData);
  menu.setMenu(DisplayJournalMenuData);
  menu.setHorzOrient();
  menu.MapKeyCodeToMenuItem(KC_ESCAPE, 5);
  FormatJournalText(journalTextData, party.journal);
  journalTextData.LastJournalBox();
}

void DISPLAY_PARTY_JOURNAL_DATA::OnDraw(void)
{
  UpdateDisplayPartyJournalScreen();
}

void DISPLAY_PARTY_JOURNAL_DATA::OnUpdateUI(void)
{
  /*
   1{ "NEXT", 0 },
   2{ "PREV", 0 },
   3{ "FIRST", 0 },
   4{ "LAST", 0 },
   5{ "EXIT", 1 }
  */   
  menu.setAllItemsActive();
  if (journalTextData.IsLastJournalBox())
  {
    menu.setItemInactive(1); // next
    menu.setItemInactive(4); // last
  }
  if (journalTextData.IsFirstJournalBox())
  {
    menu.setItemInactive(2); // prev
    menu.setItemInactive(3); // first
  }
}
