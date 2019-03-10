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

///////////////////////////////////////////////////////////////////////////////
// 
//             The Life of an Event
//
// Put on task list                  Call OnEnterQueue
//
// Check trigger condition           Call OnTestTrigger
//   If trigger condition not met    Call ChainNotHappen (remove or replace yourself)
//   and check new top-of-list
//
// Becomes available for execution   Mark invalid
//                                   Call OnInitialEvent 
//                                   Call OnDraw if still invalid
//                                   Mark valid
//
// While input is available          Mark invalid
//                                   Call OnKeypress 
//                                   Call OnDraw if still invalid
//                                   Mark valid
//
// While no input is available       Call OnIdle until it returns true
//
// Pops to top of queue again        Mark invalid
//                                   Call OnReturnToTopOfQueue 
//                                   Call OnDraw if still invalid
//                                   Mark as valid
//
// Once per scheduler loop           Call OnCycle
//
// Leaves task list                  Call OnLeaveQueue
//
///////////////////////////////////////////////////////////////////////////////

//#include "dungeoncraft/shared/..\Shared\stdafx.h"

#include "dungeoncraft/shared/GetInput.h"
#include "dungeoncraft/shared/CProcInp.h"
#include "dungeoncraft/shared/party.h"
#include "dungeoncraft/shared/globaldata.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


TASKENTRY& TASKENTRY::operator = (TASKENTRY& src)
{
  if (this==&src) return *this;
  m_event=src.m_event;
  m_state=src.m_state;
  m_disposition=src.m_disposition;
  m_invalid=src.m_invalid;
  m_poppedToTop=src.m_poppedToTop;
  return *this;
}

TASKLIST::TASKLIST(void)
{
#ifdef TASKTRACE
  m_traceFile = NULL;
  TaskTrace("TASKLIST");
#endif
  m_hash = 0;
  for (int i=0; i<MAXTASK; i++)
    m_tasklist[i].Clear();
}

TASKLIST::~TASKLIST(void)
{
  Clear();
}

void TASKLIST::Clear(void)
{
#ifdef TASKTRACE
  TaskTrace("Clear");
#endif
  m_hash += m_HASH_Clear;
  for (int i=0; i<MAXTASK; i++) 
  {
    if (m_tasklist[i].m_event!=NULL)
    {
      m_tasklist[i].m_event->OnLeaveQueue();
      if (m_tasklist[i].m_disposition==DeleteEvent) 
      {
         delete m_tasklist[i].m_event;
         m_tasklist[i].m_event = NULL;
      }
    }
    m_tasklist[i].Clear();
  }
}

#ifdef TASKTRACE
void TASKLIST::ToggleTrace(void)
{
  if (m_traceFile != NULL)
  {
    fclose(m_traceFile);
    m_traceFile = NULL;
  }
  else
  {
    m_traceFile = fopen("TaskTrace.txt","w");
  };
}

void TASKLIST::TaskTrace(char *id, int flags)
{
  if (m_traceFile == NULL) return;
  fprintf(m_traceFile,"%I64u %s %08x\n",virtualGameTime,id, flags);
}

#endif

void TASKLIST::RemoveAll(void)
{
#ifdef TASKTRACE
  TaskTrace("RemoveAll");
#endif
  m_hash += m_HASH_RemoveAll;
  for (int i=0; i<MAXTASK; i++)
  {
    if (m_tasklist[i].m_event!=NULL)
    {
      m_tasklist[i].m_event->OnLeaveQueue();
      if (m_tasklist[i].m_disposition==DeleteEvent)
      {
        delete m_tasklist[i].m_event;      
        m_tasklist[i].m_event = NULL;
      }
    }
    m_tasklist[i].Clear();
  };
}


void TASKLIST::MakeSureEventIsReady(void)
{
#ifdef TASKTRACE
  //TaskTrace("MakeSureEventIsReady", m_tasklist[0].m_state);
#endif
  m_hash += m_HASH_MakeSureEventIsReady;
  while (1)
  {
    if (m_tasklist[0].m_state==TASK_Deleted)
    {
      PopEvent(m_tasklist[0].m_event);
      continue;
    };
    if (m_tasklist[0].m_state==TASK_uninitialized)
    {
      ASSERT(m_tasklist[0].m_event!=NULL);
      // Test the game state (quest variables, facing, etc. ) and
      // let us know if you want to be triggered.
      m_tasklist[0].m_poppedToTop=false;
      if (m_tasklist[0].m_event->OnTestTrigger())
      {
        m_tasklist[0].m_state=TASK_initialized;
        if (m_tasklist[0].m_event->IsGlobalEvent())
          globalData.eventData.markEventHappened(m_tasklist[0].m_event);    
        else
          levelData.eventData.markEventHappened(m_tasklist[0].m_event);    
        // Now it is safe to 
        m_tasklist[0].m_invalid=true;
        m_tasklist[0].m_event->OnInitialEvent(); // may replace self with a new event
        // If it pops an event we will call OnDraw an extra time...no harm.
        // If it pushes an event then m_invalid will be false.
        if ((m_tasklist[0].m_invalid) && (m_tasklist[0].m_state!=TASK_Deleted))
        {
          m_tasklist[0].m_event->OnUpdateUI();
          m_tasklist[0].m_event->OnDraw();
        }
        m_tasklist[0].m_invalid=false;
        //return;
      }
      else
      {
        // You were not triggered.  So you should tell us what 
        // to do.  Many events have a Chain-if-not-happened condition
        // or a chain-always condition.  If so, they
        // should replace themselves on the stack with the 
        // chained event.  If no such event exists then you should
        // simply pop yourself off the stack and that will be the end
        // of the chain.
        m_tasklist[0].m_event->OnChainNotHappened();
      };
      continue;
    };
    if (m_tasklist[0].m_poppedToTop)
    {
      m_tasklist[0].m_poppedToTop=false;
      m_tasklist[0].m_invalid=true;
      m_tasklist[0].m_event->OnReturnToTopOfQueue();
      if ((m_tasklist[0].m_invalid) && (m_tasklist[0].m_state!=TASK_Deleted))
      {
        m_tasklist[0].m_event->OnUpdateUI();
        m_tasklist[0].m_event->OnDraw();
      }
      m_tasklist[0].m_invalid=false;
      continue;
    };
    break; // Event is ready to run.
  };
}


void TASKLIST::OnCycle(void)
{
#ifdef TASKTRACE
  //TaskTrace("OnCycle");
#endif
  m_hash += m_HASH_OnCycle;
  ASSERT(m_tasklist[0].m_event!=NULL);
  if (m_tasklist[0].m_state==TASK_uninitialized) return;
  MakeSureEventIsReady();
  bool draw = m_tasklist[0].m_event->OnCycle() | m_tasklist[0].m_invalid;
  if (draw)
  {
    m_tasklist[0].m_event->OnDraw();
    m_tasklist[0].m_invalid=false;
  }
}


//void TASKLIST::CheckForDraw(void)
//{
//  ASSERT(m_tasklist[0].m_event!=NULL);
//  if (m_tasklist[0].m_state==TASK_uninitialized) return;
//  if (m_tasklist[0].m_invalid)
//  {
//    m_tasklist[0].m_event->OnDraw();
//    m_tasklist[0].m_invalid=false;
//    m_tasklist[0].m_event->OnPostDraw();
//  }  
//}

//void TASKLIST::Invalidate(GameEvent *active)
//{
//  ASSERT(m_tasklist[0].m_event!=NULL);
//  ASSERT(m_tasklist[0].m_event==active);
//  m_tasklist[0].m_invalid=true;
//  m_tasklist[0].m_event->OnInvalidate();
//}

void TASKLIST::SetEventState(TASKSTATE state, GameEvent *active)
{
#ifdef TASKTRACE
  TaskTrace("SetEventState");
#endif
  m_hash += m_HASH_SetEventState;
  for (int i=0; i<MAXTASK; i++)
  {
//    if (m_tasklist[i].m_event==NULL) break;
    if (m_tasklist[i].m_event==active) break;
  };
  ASSERT (i<MAXTASK);
  if (i>=MAXTASK) return;
  ASSERT (m_tasklist[i].m_event!=NULL);
  if (m_tasklist[i].m_event==NULL) return;
  ASSERT (m_tasklist[i].m_state!=TASK_Deleted);
  m_tasklist[i].m_state=state;
}

TASKSTATE TASKLIST::GetEventState(GameEvent *active)
{
#ifdef TASKTRACE
  TaskTrace("GetEventState");
#endif
  m_hash += m_HASH_GetEventState;
  ASSERT(m_tasklist[0].m_event!=NULL);
  ASSERT(m_tasklist[0].m_event==active);
  return m_tasklist[0].m_state;
}

void TASKLIST::PushEvent(GameEvent *event, 
                         EventDisposition disposition)
{
#ifdef TASKTRACE
  TaskTrace("PushEvent");
#endif
  m_hash += m_HASH_PushEvent;
  ASSERT(event!=NULL);
  // Inefficient way of doing things.  We can change it however
  // we please later.  I did it this way so I can look at the top
  // entries in the debugger by looking at m_tasklist[0], [1], [2].
  if (m_tasklist[MAXTASK-1].m_event!=NULL)
  {
    for (int i=0;i<MAXTASK;i++)
    {
      if (m_tasklist[i].m_event!=NULL)
        WriteDebugString("TASKLIST[%i]=%i(%i,%i)\n", 
                         i, 
                         m_tasklist[i].m_event->GetEventId(),
                         m_tasklist[i].m_event->GetEventX(),
                         m_tasklist[i].m_event->GetEventY());
      else
        WriteDebugString("TASKLIST[%i]=NULL\n", i);
    }
    MsgBoxError("TASKLIST overflow (PushEvent)");
  };
  for (int i=MAXTASK-1; i>0; i--) m_tasklist[i]=m_tasklist[i-1];
  m_tasklist[0].m_event=event;
  m_tasklist[0].m_state=TASK_uninitialized;
  m_tasklist[0].m_disposition=disposition;
  m_tasklist[0].m_invalid=false;
  event->OnEnterQueue();
}

void TASKLIST::InsertEvent(GameEvent *event, 
                           EventDisposition disposition)
{
#ifdef TASKTRACE
  TaskTrace("InsertEvent");
#endif
  m_hash += m_HASH_InsertEvent;
  if (m_tasklist[MAXTASK-1].m_event!=NULL)
  {
    for (int i=0;i<MAXTASK;i++)
    {
      if (m_tasklist[i].m_event!=NULL)
        WriteDebugString("TASKLIST[%i]=%i(%i,%i)\n", 
                         i, 
                         m_tasklist[i].m_event->GetEventId(),
                         m_tasklist[i].m_event->GetEventX(),
                         m_tasklist[i].m_event->GetEventY());
      else
        WriteDebugString("TASKLIST[%i]=NULL\n", i);
    }
    MsgBoxError("TASKLIST overflow (InsertEvent)");
  };
  for (int i=MAXTASK-1; i>1; i--) m_tasklist[i]=m_tasklist[i-1];
  m_tasklist[1].m_event=event;
  m_tasklist[1].m_state=TASK_uninitialized;
  m_tasklist[1].m_disposition=disposition;
  m_tasklist[1].m_invalid=false;
  event->OnEnterQueue();
}

void TASKLIST::PopEvent(GameEvent *active)
{
#ifdef TASKTRACE
  TaskTrace("PopEvent");
#endif
  m_hash += m_HASH_PopEvent;
  ASSERT((m_tasklist[0].m_event!=NULL)||(m_tasklist[0].m_state==TASK_Deleted));
  ASSERT((m_tasklist[0].m_event==active)||(m_tasklist[0].m_state==TASK_Deleted));
  if (m_tasklist[0].m_state!=TASK_Deleted)
  {
    m_tasklist[0].m_event->OnLeaveQueue();
    if (m_tasklist[0].m_disposition==DeleteEvent) 
    {
      delete m_tasklist[0].m_event;
      m_tasklist[0].m_event = NULL;
    }
  };
  for (int i=0; i<MAXTASK-1; i++) m_tasklist[i]=m_tasklist[i+1];
  m_tasklist[MAXTASK-1].Clear();
  if (m_tasklist[0].m_state!=TASK_uninitialized)
  {
    ASSERT((m_tasklist[0].m_event!=NULL||m_tasklist[0].m_state==TASK_Deleted));
    if (m_tasklist[0].m_state != TASK_Deleted)
    {
      m_tasklist[0].m_poppedToTop=true;
      m_tasklist[0].m_invalid=false;
    };
  };
}

void TASKLIST::StackTrace(void)
{
  WriteDebugString("TASKLIST STACK TRACE: level %i hash %u in_combat %i\n", 
                   GetCurrentLevel(),m_hash, IsCombatActive());
  for (int i=0;i<MAXTASK;i++)
  {
    if (m_tasklist[i].m_event!=NULL)
      WriteDebugString("TASKLIST[%i]= evtype %i evid %i at %i,%i, evsrc %i, tstate %i tdispo %i\n", 
                       i, 
                       m_tasklist[i].m_event->GetEventType(),
                       m_tasklist[i].m_event->GetEventId(),
                       m_tasklist[i].m_event->GetEventX(),
                       m_tasklist[i].m_event->GetEventY(),
                       m_tasklist[i].m_event->GetEventSource(),
                       m_tasklist[i].m_state,
                       m_tasklist[i].m_disposition);
    else
      WriteDebugString("TASKLIST[%i]=NULL\n", i);
  }
  WriteDebugString("TASKLIST STACK TRACE END\n");
}

void TASKLIST::ReplaceEvent(GameEvent *event, 
                            GameEvent *active, 
                            EventDisposition disposition) 
{
#ifdef TASKTRACE
  TaskTrace("ReplaceEvent");
#endif
  m_hash += m_HASH_ReplaceEvent;
  ASSERT((m_tasklist[0].m_event!=NULL)||(m_tasklist[0].m_state==TASK_Deleted));
  ASSERT((m_tasklist[0].m_event==active)||(m_tasklist[0].m_state==TASK_Deleted));
  if (m_tasklist[0].m_state!=TASK_Deleted)
  {
    m_tasklist[0].m_event->OnLeaveQueue();
    if (m_tasklist[0].m_disposition==DeleteEvent) 
    {
      delete m_tasklist[0].m_event;
      m_tasklist[0].m_event = NULL;
    }
  };
  m_tasklist[0].m_event=event;
  m_tasklist[0].m_disposition=disposition;
  m_tasklist[0].m_state=TASK_uninitialized;
  m_tasklist[0].m_invalid=false;
  event->OnEnterQueue();
}

void TASKLIST::TaskMessage(TASKMESSAGE msg)
{
  unsigned int result;
#ifdef TASKTRACE
  TaskTrace("TaskMessage");
#endif
  m_hash += m_HASH_TaskMessage;
  for (int i=0; i<MAXTASK; i++)
  {
    if (m_tasklist[i].m_event!=NULL) 
    {
      if (m_tasklist[i].m_state!=TASK_Deleted)
      {
        result=m_tasklist[i].m_event->OnTaskMessage(msg);
        // I think maybe we should try to doubleguess any level
        // events.  If a teleport between levels occurred then
        // things will go very badly if level events do not delete
        // themselves.
        ASSERT (   (msg!=TASKMSG_TeleportBetweenLevels) 
                || (result&TASKMSG_DELETE) 
                || (m_tasklist[i].m_disposition!=LevelEvent));
        if (result & TASKMSG_DELETE) 
        {
          m_tasklist[i].m_state=TASK_Deleted;
          // We will do everything PopEvent does except that
          // we will leave the entry on the stack.
          ASSERT(m_tasklist[i].m_event!=NULL);
          // Since you asked to be deleted we assume you have
          // cleaned up after yourself.  You can call OnLeaveQueue yourself
          // if you like.
          // m_tasklist[0].m_event->OnLeaveQueue();
          if (m_tasklist[i].m_disposition==DeleteEvent) 
            delete m_tasklist[i].m_event;
          m_tasklist[i].m_event=NULL;
        };
        if (result & TASKMSG_STOP) break;
      };
    };
  };
}

bool TASKLIST::OnIdle(void)
{
  bool result;
#ifdef TASKTRACE
  //TaskTrace("OnIdle");
#endif
  m_hash += m_HASH_OnIdle;
  MakeSureEventIsReady();
  result = m_tasklist[0].m_event->OnIdle();
  return result;
}

void TASKLIST::OnKeypress(key_code key, char ascii)
{
  bool more;
#ifdef TASKTRACE
  TaskTrace("OnKeypress");
#endif
  m_hash += m_HASH_OnKeypress;
  do 
  { // This allows filter to generate multiple keys
    MakeSureEventIsReady();
    more=m_tasklist[0].m_event->OnFilterKeypress(&key, &ascii);
    if (key!=KC_NONE) 
    {
      m_tasklist[0].m_invalid=true;
      m_tasklist[0].m_event->OnKeypress(key, ascii);
      // The topmost event on the stack may have removed
      // itself from the stack in OnKeypress(). The new
      // topmost event is not always present. It may
      // be deleted (m_event=NULL) with state=TASK_Deleted.
      if ((m_tasklist[0].m_invalid) && (m_tasklist[0].m_state!=TASK_Deleted))
      {
        m_tasklist[0].m_event->OnUpdateUI();
        m_tasklist[0].m_event->OnDraw();
      }
      m_tasklist[0].m_invalid=false;
      if (m_tasklist[0].m_state!=TASK_Deleted)
      {
        if ((!m_tasklist[0].m_event->IsAChainEvent()) && (key==KC_RETURN))
          flushOldImages();
      }
      key=KC_NONE;
    }
    else
    {
      m_tasklist[0].m_invalid=true;
      m_tasklist[0].m_event->OnUpdateUI();
      m_tasklist[0].m_event->OnDraw();
      m_tasklist[0].m_invalid=false;
    }    
  } while (more);
}

void TASKLIST::OnMouseClickLeft(int x, int y)
{
#ifdef TASKTRACE
  TaskTrace("OnMouseClickLeft");
#endif
  m_hash += m_HASH_OnMouseClickLeft;
  MakeSureEventIsReady();
  m_tasklist[0].m_event->OnMouseClickLeft(x, y);
}
void TASKLIST::OnMouseMove(int x, int y)
{
#ifdef TASKTRACE
  TaskTrace("OnMouseMove");
#endif
  m_hash += m_HASH_OnMouseMove;
  MakeSureEventIsReady();
  m_tasklist[0].m_event->OnMouseMove(x, y);
  m_tasklist[0].m_invalid=true;
}

void TASKLIST::OnTimer(UINT timerId)
{
#ifdef TASKTRACE
  TaskTrace("OnTimer");
#endif
  m_hash += m_HASH_OnTimer;
  MakeSureEventIsReady();
  if (   (m_tasklist[0].m_event!=NULL) 
      && (m_tasklist[0].m_state!=TASK_Deleted))
  {
    m_tasklist[0].m_event->OnTimer(timerId);
  }
}

void stuff (unsigned int *pack, unsigned int val, int bits)
{
  unsigned int mask;
  mask  = 0xffffffff << bits;
  *pack = *pack << bits;
  *pack = *pack & mask;
  val   = val & (~mask);
  *pack = *pack | val;
}
  
unsigned int unstuff(unsigned int *pack, int bits)
{
  unsigned int temp, mask;
  mask = ~(0xffffffff << bits);
  temp = *pack & mask;
  *pack = *pack >> bits;
  return temp;
}

bool TASKLIST::SaveTaskStack(void)
{
  unsigned int i;
  unsigned int temp=0;

#ifdef TASKTRACE
  TaskTrace("SaveTaskStack");
#endif
  m_hash += m_HASH_SaveTaskStack;

  // clear all stateStack entries and count event stack depth
  for (i=0; i<MAXTASK; i++)
  {
    party.stateStack.tdata[i].Clear();
    if (m_tasklist[i].m_event != NULL)
      temp++;
  }
  party.stateStack.count=temp;
  ASSERT( party.stateStack.count > 0 );

  // don't save top event, which should be CONTROL_Save
  for (i=1; i<party.stateStack.count; i++)
  {
    temp=0;
    ASSERT( m_tasklist[i].m_event != NULL );
    WriteDebugString("SaveTaskStack: Saving event %u\n", 
                     m_tasklist[i].m_event->event);

    // Encode several pieces of information into party.stateStack;    
    stuff(&temp, m_tasklist[i].m_event->event, 13);
    stuff (&temp, m_tasklist[i].m_disposition, 4); 
    stuff (&temp, m_tasklist[i].m_invalid,     1); // bool
    stuff (&temp, m_tasklist[i].m_poppedToTop, 1); // bool
    stuff (&temp, m_tasklist[i].m_state,      13); // 8192 states

    party.stateStack.tdata[i].flags=temp;
    party.stateStack.tdata[i].id=m_tasklist[i].m_event->GetEventId();
    party.stateStack.tdata[i].datacount = m_tasklist[i].m_event->OnSaveGame(party.stateStack.tdata[i].data);
    ASSERT( party.stateStack.tdata[i].datacount <= MAX_TASK_STATE_SAVE_BYTES );
  }
  return true;
}

void TASKLIST::PopAllButTopEvent()
{
  // clear out all but top event
  for (int i=1; i<MAXTASK; i++)
  {
    if (m_tasklist[i].m_event!=NULL)
    {
      m_tasklist[i].m_event->OnLeaveQueue();
      if (m_tasklist[i].m_disposition==DeleteEvent)
      {
        delete m_tasklist[i].m_event;      
        m_tasklist[i].m_event = NULL;
      }
    }
    m_tasklist[i].Clear();
  }
}

bool TASKLIST::LoadTaskStack(void) 
{
  unsigned int i;
  GameEvent ev;
  eventType evType;
  unsigned int temp;

#ifdef TASKTRACE
  TaskTrace("LoadTaskStack");
#endif
  m_hash += m_HASH_LoadTaskStack;

  ASSERT( party.stateStack.count > 0 );

  // clear out all but top event
  PopAllButTopEvent();

  for (i=1; i<party.stateStack.count; i++)
  {
    temp=party.stateStack.tdata[i].flags;
    m_tasklist[i].m_state       = (TASKSTATE)unstuff(&temp,13);
    m_tasklist[i].m_poppedToTop = unstuff(&temp,1) != 0;
    m_tasklist[i].m_invalid     = unstuff(&temp,1) != 0;
    m_tasklist[i].m_disposition = (EventDisposition)unstuff(&temp,4);
    evType=(eventType)unstuff(&temp,13);
    if (evType!=NoEvent)
    {
      WriteDebugString("LoadTaskStack: Creating %u event\n", evType);
      m_tasklist[i].m_event = 
              ev.EventType2EventPointer(evType, party.stateStack.tdata[i].id);
      ASSERT (m_tasklist[i].m_event!=NULL);
      if (m_tasklist[i].m_event==NULL) return false;
      // All events are created dynamically on reload.
      if (m_tasklist[i].m_disposition == KeepEvent)
        m_tasklist[i].m_disposition = DeleteEvent;
    }
    else
    {
      m_tasklist[i].m_event=NULL;
    };
    if (m_tasklist[i].m_event!=NULL) 
      party.stateStack.tdata[i].datacount = m_tasklist[i].m_event->OnLoadGame(party.stateStack.tdata[i].data);
    ASSERT( party.stateStack.tdata[i].datacount <= MAX_TASK_STATE_SAVE_BYTES );
  };
  return true;
}
  
int TASKLIST::GetEventType() 
{ 
#ifdef TASKTRACE
  TaskTrace("GetEventType");
#endif
  m_hash += m_HASH_GetEventType;
  if (m_tasklist[0].m_event!=NULL) 
    return (m_tasklist[0].m_event->GetEventType()); 
  else 
    return NoEvent; 
}


TASKLIST taskList;
SIMULATEDKEYQUEUE simulatedKeyQueue;


CInput *CProcessInput::GetInputInstance(HWND hwnd)
{
  if (local == NULL)
  {
    local = new CInput(hwnd);
    if (local != NULL)
      local->ClearInput();
  }
  return local;
}


void CProcessInput::OnMouseMove(int x, int y)
{
  taskList.OnMouseMove(x, y);
}

void CProcessInput::StackTrace(void)
{
  taskList.StackTrace();
}

//#define TRACE_EVENT_TIMING

#ifdef TRACE_EVENT_TIMING
FILE *pOutput=NULL;
DWORD start;
DWORD stop;
#endif

BOOL CProcessInput::ProcessInput(HWND hwnd)
{
#ifdef TRACE_EVENT_TIMING
  if (pOutput==NULL)
    pOutput=fopen("timing.txt", "wt+");  
#endif

  if (ExitSignaled) return FALSE;
  UINT vkey;
  BOOL inputExists=FALSE;
  local = GetInputInstance(hwnd);
  if (local==NULL) return FALSE;

#ifdef TRACE_EVENT_TIMING  
  start = timeGetTime();  
#endif

  WORD counter=0;
  do
  {
    if (!ExitSignaled)
      inputExists=local->GetInput(m_inputData);

    if ((!ExitSignaled) && (inputExists))
    {
      taskList.OnMouseMove(m_inputData.mouse.x, m_inputData.mouse.y);

      if ((!ExitSignaled) && (m_inputData.key.code!=KC_NONE))
      {
        taskList.OnKeypress(m_inputData.key.code,m_inputData.key.key);
      };
      if ((!ExitSignaled) && (m_inputData.mouse.button1))
      {
        taskList.OnMouseClickLeft(m_inputData.mouse.x, m_inputData.mouse.y);
      };
      //if (m_inputData.mouse.button2)
      //{
      //  taskList.OnMouseClickRight(m_inputData.mouse.x, m_inputData.mouse.y);
      //};
      if ((!ExitSignaled) && (m_inputData.timer_expired))
      {
        taskList.OnTimer(m_inputData.timerId);
      };
    }
    else if (!ExitSignaled)
    {
      while (!taskList.OnIdle())
        ;  // While not needing input, loop until TRUE is returned

      vkey=simulatedKeyQueue.PopKey();
      if (vkey!=0xffffffff) 
      {
        KeySimulate(vkey);
        inputExists=TRUE;
      };
    };
    
    if ((!ExitSignaled) && (miscError != NoError))
    {
      // really just formats the error text
      displayMiscError(miscError);
      miscError = NoError;
    }
    counter++;
    ASSERT( counter < 100 ); // should not loop this much
  } while ((!ExitSignaled) && (inputExists));

  if (!ExitSignaled)
  {
    taskList.OnIdle();
    taskList.OnCycle();
  }

#ifdef TRACE_EVENT_TIMING
  stop = timeGetTime()-start;
  fprintf(pOutput, "procinp %I64u, event %u, used %u\n", 
          virtualGameTime, taskList.GetEventType(), stop);
  if (ExitSignaled)
    fclose(pOutput);
#endif

  return TRUE;
}



//*****************************************************************************
//    NAME: CProcessInput::CProcessInput
//
// PURPOSE: 
//
//*****************************************************************************
CProcessInput::CProcessInput() : local(NULL)
{
  PartyList.RemoveAll();
  HaveWarp=FALSE;
  memset(&m_inputData, 0, sizeof(m_inputData));
  wx=wy=wl=wf=-1;
}

// First state used after design files are loaded
void CProcessInput::InitializeToSplash(void)
{
  taskList.RemoveAll();
  taskList.PushEvent(new EXIT_DATA, DeleteEvent);
  taskList.PushEvent(new SPLASH_DATA, DeleteEvent);
}

// Used when party dies to go back to main menu screen,
// or if splash screen not loaded during startup
void CProcessInput::InitializeToMain(void)
{
  taskList.RemoveAll();
  taskList.PushEvent(new EXIT_DATA, DeleteEvent);
  taskList.PushEvent(new MAIN_MENU_DATA(NULL), DeleteEvent);
}

// used when party dies to go back to main menu screen
void CProcessInput::InitializeToDesignSelect(void)
{
  taskList.RemoveAll();
  taskList.PushEvent(new EXIT_DATA, DeleteEvent);
  taskList.PushEvent(new DESIGN_SELECT_MENU_DATA, DeleteEvent);
}

//*****************************************************************************
//    NAME: CProcessInput::AddPartyMember
//
// PURPOSE: 
//
//*****************************************************************************
void CProcessInput::AddPartyMember(CString PlayerCharacter)
{
  PartyList.AddTail(PlayerCharacter);
}

//*****************************************************************************
//    NAME: CProcessInput::SetWarp
//
// PURPOSE: 
//
//*****************************************************************************
void CProcessInput::SetWarp(int x, int y, int level, int facing)
{
  WriteDebugString("SetWarp to %i,%i on level %i, facing %i\n",
                   x,y,level,facing);
  HaveWarp = TRUE;
  wx=x;
  wy=y;
  wl=level;
  wf=facing;
}

//*****************************************************************************
// NAME:    CProcessInput::KeyPressed
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
void CProcessInput::KeyPressed(UINT vkey)
{
  if (local != NULL)
    local->KeyPressed(vkey);
}

//*****************************************************************************
//    NAME: CProcessInput::KeySimulate
//
// PURPOSE: 
//
//*****************************************************************************
void CProcessInput::KeySimulate(UINT vkey)
{
  if (local != NULL)
    local->KeySimulate(vkey);
}

//*****************************************************************************
// NAME:    CProcessInput::handleTimer
//
// PURPOSE: 
//
// RETURNS: None
//*****************************************************************************
BOOL CProcessInput::handleTimer(UINT timerId)
{
  // We need to pass the timer to the current event
  if (local != NULL)
    local->TimerExpired(timerId); 
  return TRUE;
}

//*****************************************************************************
//    NAME: CProcessInput::HandleMouseClick
//
// PURPOSE: 
//
//*****************************************************************************
void CProcessInput::HandleMouseClick(long x, long y)
{
  if (local != NULL)
    local->SetMouseLeftClick(x, y);
}

//*****************************************************************************
//    NAME: CProcessInput::HandleMouseMove
//
// PURPOSE: 
//
//*****************************************************************************
void CProcessInput::HandleMouseMove(long x, long y)
{
  if (local != NULL)
    local->SetMouseMove(x, y);
}

//*****************************************************************************
// NAME: BackupPartyOneStep
//
// PURPOSE:  
//*****************************************************************************
void CProcessInput::BackupPartyOneStep()
{
  party.movePartyBackward();
}

//*****************************************************************************
//    NAME: CProcessInput::GetCurrEventType
//
// PURPOSE: 
//
//*****************************************************************************
eventType CProcessInput::GetCurrEventType(void)
{
  return ((eventType)taskList.GetEventType());
}

SIMULATEDKEYQUEUE::SIMULATEDKEYQUEUE(void)
{
  for (int i=0; i<10; i++) keystrokes[i]=0xffffffff;
}

UINT SIMULATEDKEYQUEUE::PopKey(void)
{
  UINT temp;
  temp=keystrokes[0];
  for (int i=0; i<9; i++) keystrokes[i]=keystrokes[i+1];
  keystrokes[9]=0xffffffff;
  return temp;
}

void SIMULATEDKEYQUEUE::PushKey(UINT key)
{
  for (int i=9; i>0; i--) keystrokes[i]=keystrokes[i-1];
  keystrokes[0]=key;
}



//****************************************************************
//
//****************************************************************
//
//****************************************************************
//
//****************************************************************
//
//****************************************************************
//
//****************************************************************
//
//****************************************************************
//
//****************************************************************
//
//****************************************************************
//
//****************************************************************
//
// All of this is an early attempt to provide a mechanism
// for recording gameplay in such a way that it can be played
// back exactly in order to recreate hard-to-reproduce problems,
// provide 'movies' of gameplay for neophytes, etc.
//
// In order to do this it is necessary to capture the time
// of each input.  And to make that practical, it is necessary
// that all input be routed through one place.  This includes
// the passage of time.
//
// virtualGameTime is kept in units of (virtualGametimeUnits) milliseconds.
// There is no reason to attempt to keep virtualGameTime in 
// milliseconds except as a kind of convenience to avoid conversions
// in the code and in the mind.  We could just as well (and perhaps
// should) simply say  virtualGameTime++.
// Each time interval is treated as follows:
// 1 - Increment virtualGameTime
// 2 - Call scheduler()
// 3 - For each queued message during that interval
//     a - post the message
//     b - call scheduler();
//
// 10 March 2002 Paul Stevens
// Initial definition of class INPUTQUEUE.
// Route CMainFrame::OnKeyDown messages through the INPUTQUEUE.
//
//

void scheduler(void);
extern LONGLONG gameTime;

INPUTQUEUE inputQueue;


PLAYBACKFILE::PLAYBACKFILE(void)
{
  m_file = NULL;
  m_inputEOF = false;
  m_dataReady = false;
}

PLAYBACKFILE::~PLAYBACKFILE(void)
{
  if (m_file != NULL) fclose(m_file);
}

void PLAYBACKFILE::Open(void)
{
  if (m_file != NULL) return;
  m_file = fopen("Playfile.log","r");
  if (m_file == NULL) return;
  m_dataReady = false;
  m_inputEOF = false;
}

void PLAYBACKFILE::Close(void)
{
  if (m_file == NULL) return;
  fclose(m_file);
  m_file = NULL;
  m_dataReady = false;
}

bool PLAYBACKFILE::m_ProcessCommand(char *line)
{ //Return true if playback should continue.
  char fields[10][80];
  int numField, col;
  int len;
  len = strlen(line);
  for (numField=0; numField<10; numField++)
  {
    //Skip leading blanks.
    while (len > 0)
    {
      if (   (*line != ' ')
          && (*line != '\t')
          && (*line != '\n')
         ) break;
      line++;
      len--;
      continue;
    };
    if (len == 0) break; //No more fields
    col = 0;
    while (len > 0)
    {
      if (   (*line == ' ')
          || (*line == '\t')
          || (*line == '\n')
         ) break;
      fields[numField][col] = *line;
      col++;
      line++;
      len--;
      if (col >= 79) break;
    };
    fields[numField][col] = 0;
  }; //For numField
  if (numField == 0) return true;
  if (strcmp(fields[0],"#VERSION") == 0)
  {
    if (numField < 2) return false;
    char version[20];
    double ver = ENGINE_VER;
    sprintf(version,"%+016.10f", ver);
    if (strcmp(version, fields[1]) != 0) return false;
    return true;
  };
  if (strcmp(fields[0], "#SEEDMT") == 0)
  {
    if (numField < 3) return true;
    DWORD randomSeed;
    unsigned int hash;
    if (sscanf(fields[1],"%08x",&randomSeed) != 1) return true;
    seedMT(randomSeed);
    if (sscanf(fields[2],"%08x",&hash) != 1) return true;
    taskList.Hash(hash);
    return true;
  };
  return true; //treat as a comment
};


void PLAYBACKFILE::Read(INPUTQUEUE_ENTRY *ent)
{
  ent->m_entryType = INPUTQUEUE_ENTRY::IQE_UNKNOWN;
  if (m_file == NULL) return;
  while (!m_dataReady) // We may use this for backspace someday.
  {
    if (m_inputEOF) return;
    if (fgets(m_inputLine, 80, m_file) != NULL)
    {
      if (m_inputLine[0] == '#')
      {
        m_ProcessCommand(m_inputLine); //Perhaps a comment.
        continue;
      };
      if (sscanf(m_inputLine,
                 "%016I64x %02x %08x %08x",
                 &m_lineTime,
                 &m_lineType,
                 &m_lineData,
                 &m_lineHash) == 4) 
      {
        m_dataReady = true;
      }
      else
      {
        m_inputEOF = true;
        return;
      };
    };
    if (!m_dataReady) return;
    if (m_lineType == 0xff)
    {
      virtualGameTime = m_lineTime;
      m_dataReady = false;
      continue;
    };
  };
  ASSERT(m_lineTime >= virtualGameTime);
  if (m_lineTime < virtualGameTime)
  {
    MessageBox(NULL,"Timing Error","Playback Error",MB_OK|MB_TOPMOST);
    Close();
    return;
  };
  if (m_lineTime > virtualGameTime) return;
  ASSERT(m_lineHash == taskList.Hash());
  if (m_lineHash != taskList.Hash())
  {
    MessageBox(NULL,"Sequence Error","Playback Error",MB_OK|MB_TOPMOST);
    Close();
    return;
  };
  ent->m_entryType = 
         (INPUTQUEUE_ENTRY::INPUTQUEUE_ENTRY_TYPE)m_lineType;
  ent->m_data = m_lineData;
  ent->m_timeOfEntry = m_lineTime;
  m_dataReady = false;
  return;
}

bool PLAYBACKFILE::IsEntryAvailable(void)
{
  if (m_file == NULL) return false;
  while (!m_dataReady) // We may use this for backspace someday.
  {
    if (m_inputEOF) return false;
    if (fgets(m_inputLine, 80, m_file) != NULL)
    {
      if (m_inputLine[0] == '#')
      {
        m_ProcessCommand(m_inputLine); //Perhaps a comment.
        continue;
      };
      if (sscanf(m_inputLine,
                 "%016I64x %02x %08x %08x",
                 &m_lineTime,
                 &m_lineType,
                 &m_lineData,
                 &m_lineHash) == 4) 
      {
        m_dataReady = true;
      }
      else
      {
        m_inputEOF = true;
        return false;
      };
    };
    if (!m_dataReady) return false;
    if (m_lineType == 0xff)
    {
      virtualGameTime = m_lineTime;
      m_dataReady = false;
      continue;
    };
  };
  ASSERT(m_lineTime >= virtualGameTime);
  if (m_lineTime > virtualGameTime) return false;
  return true;
}


bool PLAYBACKFILE::IsPlaying(void)
{
  return m_file != NULL;
}


RECORDFILE::RECORDFILE(void)
{
  m_file = NULL;
}

RECORDFILE::~RECORDFILE(void)
{
  if (m_file != NULL) fclose(m_file);
  m_file = NULL;
}


bool RECORDFILE::IsRecording(void)
{
  return m_file != NULL;;
}

void RECORDFILE::Open(void)
{
  int recordNum;
  char recordName[20];
  if (m_file != NULL) return;
  for (recordNum=0; recordNum<99; recordNum++)
  {
    sprintf(recordName,"Record%03d.log",recordNum);
    m_file = fopen(recordName,"r");
    if (m_file == NULL) break;
    fclose(m_file);
    m_file = NULL;
  };
  if (recordNum < 99) 
  {
    m_file = fopen(recordName,"w");
    if (m_file == NULL) return;
    double ver = ENGINE_VER;
    sprintf(recordName,"%+016.10f", ver);
    fprintf(m_file,
            "#VERSION %s Remove this line to ignore version\n",
            recordName);
    fprintf(m_file,"%016I64x ff 00000000 00000000\n",virtualGameTime);
    DWORD randomSeed = GetTickCount();
    seedMT(randomSeed);
    fprintf(m_file, "#SEEDMT %08x %08x\n",randomSeed, taskList.Hash());
  };
}

void RECORDFILE::Write(INPUTQUEUE_ENTRY *ent)
{
  if (m_file != NULL)
  {
    fprintf(m_file,
            "%016I64x %02x %08x %08x\n",
            virtualGameTime,
            ent->m_entryType,
            ent->m_data,
            taskList.Hash()); 
  };
}








INPUTQUEUE_ENTRY::INPUTQUEUE_ENTRY(void)
{
  m_nextEntry   = -1;
  m_timeOfEntry = 0;
  m_entryType   = IQE_UNKNOWN;
  m_data = 0;
}

INPUTQUEUE_ENTRY::~INPUTQUEUE_ENTRY(void)
{
}


void INPUTQUEUE_ENTRY::destruct(void)
{
  switch (m_entryType)
  {
  case IQE_ON_KEY_DOWN:
  case IQE_ON_LBUTTON_DOWN:
    break; // These are easy
  default:
    ASSERT(false);
  };
  m_entryType = IQE_UNKNOWN;
  m_nextEntry = -1;
  m_data = 0;
}

INPUTQUEUE::INPUTQUEUE(void)
{
  int i;
  m_firstInQueue = -1;
  m_lastInQueue  = -1;
  m_available    = -1;
  m_recordOK     = true;
  m_playOK       = true;
  m_previousTime = 0;
  for (i=0; i<INPUTQUEUE_MAX_ENTRIES; i++)
  {
    m_queue[i].m_entryType = INPUTQUEUE_ENTRY::IQE_UNKNOWN;
    m_queue[i].m_nextEntry = -1;
    m_releaseEntry(i);
  };
}

INPUTQUEUE::~INPUTQUEUE(void)
{
  int i;
  while ((i=m_removeEntry()) != -1) 
  {
    m_queue[i].destruct();
    m_releaseEntry(i);
  };
}

void INPUTQUEUE::m_addEntry(int i)
{ // Add entry to end of queue
  ASSERT(i >= 0);
  ASSERT(i < INPUTQUEUE_MAX_ENTRIES);
  ASSERT(m_queue[i].m_nextEntry == -1);
  if (m_firstInQueue == -1)
  {
    m_firstInQueue = i;
  }
  else
  {
    ASSERT(m_queue[m_lastInQueue].m_nextEntry == -1);
    m_queue[m_lastInQueue].m_nextEntry = i;
  };
  m_lastInQueue = i;
}

int INPUTQUEUE::m_removeEntry(void)
{ //Remove the first entry from queue.
  int result;
  if (m_playFile.IsPlaying())
  {
    int index;
    index = inputQueue.m_allocateEntry();
    if (index == -1) return -1;
    m_playFile.Read(&inputQueue.m_queue[index]);
    if (inputQueue.m_queue[index].m_entryType
                  == INPUTQUEUE_ENTRY::IQE_UNKNOWN)
    {
      inputQueue.m_releaseEntry(index);
      return -1;
    };
    return index;
  };                           
  if (m_firstInQueue == -1) return -1;
  result = m_firstInQueue;
  ASSERT(result >= 0);
  ASSERT(result < INPUTQUEUE_MAX_ENTRIES);
  m_firstInQueue = m_queue[result].m_nextEntry;
  if (m_firstInQueue == -1) m_lastInQueue = -1;
  return result;
}

bool INPUTQUEUE::m_isEntryAvailable(void)
{
  if (m_playFile.IsPlaying())
  {
    return m_playFile.IsEntryAvailable();
  };
  return m_firstInQueue != -1;
}

int INPUTQUEUE::m_allocateEntry(bool important)
{ // Return -1 if no entry available.
  // Right now we ignore the 'important' parameter.  But we may have 
  // to add an important field to the queue entries and
  // be prepared to discard non-important entries to make
  // room for important entries.
  // We could also allocate entries from the heap and thereby
  // make room for a very large number of them.  I don't think
  // this is a good idea.
  int result;
  ASSERT(m_available != -1);
  if (m_available == -1) return -1;
  result = m_available;
  ASSERT(result >= 0);
  ASSERT(result < INPUTQUEUE_MAX_ENTRIES);
  m_available = m_queue[result].m_nextEntry;
  m_queue[result].m_nextEntry = -1;
  return result;
}

void INPUTQUEUE::m_releaseEntry(int i)
{ // Put into available list
  ASSERT(i >= 0);
  ASSERT(i < INPUTQUEUE_MAX_ENTRIES); 
  ASSERT(m_queue[i].m_entryType == INPUTQUEUE_ENTRY::IQE_UNKNOWN);
  ASSERT(m_queue[i].m_nextEntry == -1); //Destruct it before releasing it!!!
  m_queue[i].m_nextEntry = m_available;
  m_available = i;
}

int INPUTQUEUE::m_adjustGoal(void)
{ //returns distance to goal
  DWORD curTime;
  int distanceToGoal;
  int elapsedTime;
  curTime = timeGetTime();
  elapsedTime = curTime - m_previousTime; // A signed value!
  m_previousTime = curTime;
  if (elapsedTime < 0) elapsedTime = 0; //Might happen first time.
  m_goalTime += elapsedTime;
  distanceToGoal = m_goalTime - virtualGameTime;
  // We can chop virtualGametimeUnits off the distance
  // each time around this loop.  Of course, we may add 
  // some distance in the process!
  if (distanceToGoal < 0) distanceToGoal = 0;
  //Average over a few periods.  But not too many.
  if (distanceToGoal >= 5*virtualGametimeUnits)
  {
    distanceToGoal = 5*virtualGametimeUnits;
    m_goalTime = virtualGameTime + distanceToGoal;
  };
  return distanceToGoal;
}


//#define TRACE_TIMER_DETAILS


#ifdef TRACE_TIMER_DETAILS
class DETAIL_TRACE
{
public:
  DETAIL_TRACE(void);
  ~DETAIL_TRACE(void);
  DWORD    m_id[1000];
  DWORD    m_data[1000];
  DWORD    m_time[1000];
  DWORD    m_virtualTime[1000];
  int      m_nextIndex;
  void     trace (DWORD id, DWORD data);
};

DETAIL_TRACE::DETAIL_TRACE(void)
{
  m_nextIndex = 0;
  memset(m_id, 0, sizeof m_id);
}

DETAIL_TRACE::~DETAIL_TRACE(void)
{
}

void DETAIL_TRACE::trace(DWORD id, DWORD data)
{
  if (id != 0xffffffff)
  {
    m_id[m_nextIndex] = id;
    m_data[m_nextIndex] = data;
    m_time[m_nextIndex] = timeGetTime();
    m_virtualTime[m_nextIndex] = (DWORD)virtualGameTime;
    m_nextIndex = (m_nextIndex +1)%1000;
  }
  else
  {
    int i = m_nextIndex;
    do
    {
      if (m_id[i] != 0)
      {
        WriteDebugString("%08x %08x %08x %08x\n",
                         m_time[i],m_virtualTime[i],m_id[i], m_data[i]);
      };
      i = (i+1)%1000;
    } while (i != m_nextIndex);
  };
}

DETAIL_TRACE detailTrace;

#endif //TRACE_TIMER_DETAILS

#ifdef TRACE_TIMER_DETAILS
#define DETAILTRACE(a,b) detailTrace.trace((a),(b));
#else
#define DETAILTRACE(a,b)
#endif


DWORD INPUTQUEUE::DistributeInput(void)
{
  // We will call scheduler only if:
  //   1 - A queue entry has been removed and processed.
  //   2 - virtualGameTime has incremented.
  // It is silly to call scheduler for any other
  // reason.  If nothing has changed then nothing needs to
  // be done!  Otherwise, we need to fix scheduler so that
  // it does everything that needs doing at any one instant
  // of time.
  //
  // Return value: how long to wait before calling again.
  int n, distanceToGoal;
  DWORD result = virtualGametimeUnits;
  DETAILTRACE(0xed1,0); // Enter DistributeInput
  if ((n = m_removeEntry()) != -1)
  {
    m_recordFile.Write(&m_queue[n]);
    switch(m_queue[n].m_entryType)
    {
    case INPUTQUEUE_ENTRY::IQE_ON_KEY_DOWN:
      DETAILTRACE(0xd10dc,m_queue[n].m_data); //Distribute input On Down Call
      ProcInput.KeyPressed(m_queue[n].m_data);
      DETAILTRACE(0xd10d0,0); //Distribute input On Down Out
      break;
    case INPUTQUEUE_ENTRY::IQE_ON_LBUTTON_DOWN:
      DETAILTRACE(0xd10dc1,m_queue[n].m_data); //Distribute input On LButton Call
      ProcInput.HandleMouseClick(
                     (m_queue[n].m_data>>16)&0xffff, //x 
                      m_queue[n].m_data&0xffff);     //y
      DETAILTRACE(0xd10d01,0); //Distribute input On LButton Out
      break;
    default:
      ASSERT(false);
    };
    m_queue[n].destruct();
    m_releaseEntry(n);
    DETAILTRACE(0xd15ca,0);//Distribute Input Scheduler Call a
    scheduler();
    DETAILTRACE(0xd150a,0);//Distribute Input Scheduler Out a
    if (ExitSignaled) return false;
  };
  distanceToGoal = m_adjustGoal();
  if (distanceToGoal >= virtualGametimeUnits)
  {
    virtualGameTime += virtualGametimeUnits;
    distanceToGoal -= virtualGametimeUnits;
    DETAILTRACE(0xd15cb,0); // Distribute Input Scheduler Call b
    scheduler();  //Somebody may have added something to the queue.
    DETAILTRACE(0xd150b,0);// Distribute Input Scheduler Out b
    distanceToGoal = m_adjustGoal();
  };
  if (distanceToGoal >= virtualGametimeUnits)
  {
    result = 0;
  }
  else
  {
    result = virtualGametimeUnits - distanceToGoal;
  };  
  if (m_isEntryAvailable()) result = 0;
  // Now you may find that some Windows messages are not handled
  // as quickly as they ought to be.  That is probably because they
  // are not being processed by inputQueue and therefore they only
  // get processed once every 30 milliseconds.
  return result;
} 

void INPUTQUEUE::OnKeyDown(unsigned int nChar)
{
  static bool CONTROL = false;
  int i;
  if (CONTROL && (nChar == 0x52)) //'R'
  { //Start recording
    if (m_recordOK)
    {
      m_recordFile.Open();
      m_recordOK = false;
    };
  };
  if (CONTROL && (nChar == 0x50)) // 'P'
  { //Start playback
    if (m_playOK)
    {
      m_playFile.Open();
      m_playOK = false;
    };
  };
#ifdef TASKTRACE
  if (CONTROL && (nChar == 0x54)) // 'T'
  { // Toggle Task Tracing
    taskList.ToggleTrace();
    return;
  };
#endif
  CONTROL = nChar == 0x11;
  if (m_playFile.IsPlaying())
  {
    if (nChar == 0x1b) 
    {
      m_playFile.Close();
      DETAILTRACE(0xffffffff,0);
      return;
    };
  };
  i = m_allocateEntry();
  if (i==-1) return; // Sorry about that.
  m_queue[i].m_data = nChar;
  m_queue[i].m_entryType = INPUTQUEUE_ENTRY::IQE_ON_KEY_DOWN;
  m_addEntry(i);
}

void INPUTQUEUE::OnLButtonDown(int x, int y)
{
  int i;
  i = m_allocateEntry();
  if (i==-1) return; // Sorry about that.
  m_queue[i].m_data = ((x&0xffff) << 16) | (y&0xffff);
  m_queue[i].m_entryType = INPUTQUEUE_ENTRY::IQE_ON_LBUTTON_DOWN;
  m_addEntry(i);
}

