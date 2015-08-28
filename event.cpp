/* RogueVM - RPG Engines
 *
 * RogueVM is the legal property of its developers, whose names
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

#include "common/file.h"
#include "aesop/aesop.h"
#include "aesop/defs.h"
#include "aesop/shared.h"
#include "aesop/event.h"
#include "aesop/resources.h"
#include "aesop/intrface.h"
#include "aesop/rt.h"
#include "aesop/rtsystem.h"
#include "aesop/rtmsg.h"
//#include "aesop/modsnd32.h"
#include "aesop/stubs.h"

namespace Aesop {

LONG ENABLED;

NREQ NR_list[NR_LSIZE];
LONG NR_first[NUM_EVTYPES];

LONG current_event_type;

EVENT EV_queue[EV_QSIZE];

ULONG EV_head;
ULONG EV_tail;

static char *strs[] = {"SYS_FREE",
                       "SYS_TIMER",
                       "SYS_MOUSEMOVE",
                       "SYS_ENTER_REGION",
                       "SYS_LEAVE_REGION",
                       "SYS_LEFT_CLICK",
                       "SYS_LEFT_RELEASE",
                       "SYS_RIGHT_CLICK",
                       "SYS_RIGHT_RELEASE",
                       "SYS_CLICK",
                       "SYS_RELEASE",
                       "SYS_LEFT_CLICK_REGION",
                       "SYS_LEFT_RELEASE_REGION",
                       "SYS_RIGHT_CLICK_REGION",
                       "SYS_RIGHT_RELEASE_REGION",
                       "SYS_CLICK_REGION",
                       "SYS_RELEASE_REGION",
                       "SYS_KEYDOWN"};

/*********************************************************/
//
// Enable/disable asynchronous interrupt events
//
/*********************************************************/

void ENABLE(void)
{
   ++ENABLED;
}

void DISABLE(void)
{
   --ENABLED;
}

/*********************************************************/
//
// Most event notifications are triggerable when/if the
// event's parameter is equal to the notify request's
// parameter.  SYS_TIMER events, however, become recognizable
// if the event parameter (heartbeat) is greater than or equal
// to the notification parameter.
//
// SYS_FREE events do not match any notification type
//
// A notification parameter of -1 matches all event parameters
//
/*********************************************************/

LONG match_parameter(LONG event_type, LONG event_parameter,
   LONG test_parameter)
{
   LONG match;

   if (event_type == SYS_FREE)
      return 0;

   if (test_parameter == -1L) return 1;

   switch (event_type)
      {
      case SYS_TIMER:
         match = (event_parameter >= test_parameter);
         break;

      default:
         match = (event_parameter == test_parameter);
         break;
      }

   return match;
}


/*********************************************************/
void init_notify_list(void)
{
   ULONG i;

   DISABLE();

   for (i=0;i<NUM_EVTYPES;i++)
      NR_first[i] = -1;

   NR_first[SYS_FREE] = 0;

   for (i=0;i<NR_LSIZE;i++)
      {
      NR_list[i].next = i+1;
      NR_list[i].prev = i-1;
      }

   NR_list[i-1].next = -1;

   ENABLE();
}

/*********************************************************/
//
// Append notification request to end of list
//
// Handlers for a given event are called in order of registration
//
/*********************************************************/

void add_notify_request(LONG client, ULONG message, LONG event, LONG parameter)
{
   LONG i,nxt,cur = 0;
   NREQ *NR;

   DISABLE();

   i = NR_first[SYS_FREE];

   if (i == -1)
	   error("event.dbg write");

   NR = &NR_list[i];

   NR_first[SYS_FREE] = NR->next;

   NR->next = -1;
   NR->client = client;
   NR->message = message;
   NR->parameter = parameter;
   NR->status = (0 & ~NSX_TYPE) | event;

   nxt = NR_first[event];

   if (nxt == -1)
      {
      NR_first[event] = i;
      NR->prev = -1;
      }
   else
      {
      while (nxt != -1)
         nxt = NR_list[cur = nxt].next;

      NR_list[cur].next = i;
      NR->prev = cur;
      }

   ENABLE();
}

/*********************************************************/
//
// Cancel a specific notification request for a given entity or program  
// object
//
/*********************************************************/

void delete_notify_request(LONG client, ULONG message, LONG event, LONG parameter)
{
   LONG nxt,prev;
   LONG fnxt;
   LONG cur = 0, fcur = 0;
   NREQ *NR;
   LONG all_events;

   DISABLE();

   if (event==(unsigned)-1)
      {
      event = 1;
      all_events = 1;
      }
   else
      all_events = 0;

   do
      {
      nxt = NR_first[event];                      // start at chain beginning

      while (nxt != -1)                           // while not at end of chain
         {
         cur = nxt;                               // get links
         NR = &NR_list[cur];               
         nxt = NR->next;
         prev = NR->prev;

         if (NR->client != client) continue;      // match specified parms

		 if (((ULONG)message != (unsigned)-1) && ((ULONG)message != NR->message))
            continue;

         if (!match_parameter(event,NR->parameter,parameter)) continue;

         NR->client = -1;                         // invalidate the NREQ
         NR->status = (NR->status & ~NSX_TYPE) | SYS_FREE;

         if (nxt != -1)                           // close its links
            NR_list[nxt].prev = prev;

         if (prev != -1)                          
            NR_list[prev].next = nxt;
         else
            NR_first[event] = nxt;

         NR->next = -1;                           // append NREQ to FREE
         fnxt = NR_first[SYS_FREE];               // chain...
         if (fnxt == -1)
            {
            NR_first[SYS_FREE] = cur;
            NR->prev = -1;
            }
         else
            {
            while (fnxt != -1)
               fnxt = NR_list[fcur = fnxt].next;

            NR_list[fcur].next = cur;
            NR->prev = fcur;
            }
         }
      }
   while (all_events && (++event < NUM_EVTYPES));

   ENABLE();
}

/*********************************************************/
//
// Cancel all notification requests for a given entity or program object
// (-1 to cancel all requests for all entity objects)
//
/*********************************************************/

void cancel_entity_requests(LONG client)
{
   LONG event,nxt,cur,prev;
   LONG fnxt,fcur = 0;
   NREQ *NR;

   DISABLE();

   for (event=1;event<NUM_EVTYPES;event++)        // for all event types...
      {
      nxt = NR_first[event];                      // start at chain beginning

      while (nxt != -1)                           // while not at end of chain
         {
         cur = nxt;                               // get links
         NR = &NR_list[cur];               
         nxt = NR->next;
         prev = NR->prev;

         if (client == -1)                        // for all clients?
            {
            if ((NR->client >= NUM_ENTITIES) ||   // yes, filter out all non-
                (NR->client == -1))               // entity clients
                  continue;
            }
         else                                     // else match specified
            if (NR->client != client) continue;   // client

         NR->client = -1;                         // invalidate the NREQ
         NR->status = (NR->status & ~NSX_TYPE) | SYS_FREE;

         if (nxt != -1)                           // close its links
            NR_list[nxt].prev = prev;

         if (prev != -1)                          
            NR_list[prev].next = nxt;
         else
            NR_first[event] = nxt;

         NR->next = -1;                           // append NREQ to FREE
         fnxt = NR_first[SYS_FREE];               // chain...
         if (fnxt == -1)
            {
            NR_first[SYS_FREE] = cur;
            NR->prev = -1;
            }
         else
            {
            while (fnxt != -1)
               fnxt = NR_list[fcur = fnxt].next;

            NR_list[fcur].next = cur;
            NR->prev = fcur;
            }
         }
      }

   ENABLE();
}

/*********************************************************/
void init_event_queue(void)
{
   EV_head = 0;
   EV_tail = 0;
}

/*********************************************************/
EVENT *find_event(LONG type, LONG parameter)
{
   ULONG t;
   EVENT *EV;

   DISABLE();

   t = EV_tail;

   while (t != EV_head)
      {
      EV = &EV_queue[t];
      t = ++t % EV_QSIZE;

      if (EV->type != type)
         continue;

      if (!match_parameter(EV->type,EV->parameter,parameter)) continue;

      ENABLE();
      return EV;
      }

   ENABLE();
   return NULL;
}

/*********************************************************/
void remove_event(LONG type, LONG parameter, LONG owner)
{
   ULONG t;
   EVENT *EV;

   DISABLE();

   for (t=0;t<EV_QSIZE;t++)
      {
      EV = &EV_queue[t];
     
      if ((owner != -1) && (EV->owner != owner))
         continue;

      if ((type != -1) && (EV->type != type))
         continue;

      if (!match_parameter(EV->type,EV->parameter,parameter)) continue;

      EV->type = SYS_FREE;
      }

   ENABLE();
}

/*********************************************************/
void add_event(LONG type, LONG parameter, LONG owner)
{
   DISABLE();

   EV_queue[EV_head].type = type;
   EV_queue[EV_head].owner = owner;
   EV_queue[EV_head].parameter = parameter;

   EV_head = ++EV_head % EV_QSIZE;

   if (EV_head == EV_tail)
      EV_tail = ++EV_tail % EV_QSIZE;

   ENABLE();
}

/*********************************************************/
EVENT *next_event(void)
{
   EVENT *EV;

   DISABLE();

   if (EV_tail != EV_head)
      EV = &EV_queue[EV_tail];
   else
      EV = NULL;

   ENABLE();
   return EV;
}

/*********************************************************/
EVENT *fetch_event(void)
{
   EVENT *EV;
   
   DISABLE();
   
   if (EV_tail != EV_head)
      {
      EV = &EV_queue[EV_tail];
      EV_tail = ++EV_tail % EV_QSIZE;
      }
   else
      EV = NULL;

   ENABLE();

   return EV;
}

/*********************************************************/
EVENT *scan_event_range(LONG first_type, LONG last_type)
{
   EVENT *EV;
   ULONG t;

   DISABLE();

   t = EV_tail;

   while (t != EV_head)
      {
      EV = &EV_queue[t];
      t = ++t % EV_QSIZE;

      if ((EV->type < first_type) || (EV->type > last_type))
         continue;

      ENABLE();
      return EV;
      }

   ENABLE();
   return NULL;
}

/*********************************************************/
void dump_event_queue(void)
{
   EVENT *e;

   while ((e = fetch_event()) != NULL)
      if (e->type <= SYS_KEYDOWN)
         debugN("Event %s, parameter %ld\n",strs[e->type],e->parameter);
      else
		  debugN("User event, parameter %ld\n", e->parameter);
}

/*********************************************************/
//
// Send /message/ to /index/ when /event/ of type /parameter/
// occurs
//
/*********************************************************/


void notify(LONG argcnt, ULONG index, ULONG message, LONG event,
   LONG parameter)
#pragma on (unreferenced)
{
   add_notify_request(index, message, event, parameter);
}

/*********************************************************/
//
// Cancel a notification request set up by notify() above
//
/*********************************************************/


void cancel(LONG argcnt, ULONG index, ULONG message, LONG event,
   LONG parameter)
#pragma on (unreferenced)
{
   delete_notify_request(index, message, event, parameter);
}

/*********************************************************/
//
// Return 1 if an event is queued and ready to be dispatched
//
/*********************************************************/

ULONG peek_event(void)
{
   EVENT *EV;

   PollMod();

   EV = next_event();

#if 0
   if (kbhit())
      {
      hide_mouse();
      lock_mouse();
      GIL_set_video_mode(3);
      dump_event_queue();
      getch();
      GIL_set_video_mode(19);
      unlock_mouse();
      refresh_window(0,PAGE2,PAGE1);
      show_mouse();
      }
#endif

   return (EV != NULL);
}

/*********************************************************/
//
// Fetch the next event from the FIFO queue and fulfill any of its
// notification requests
//
// System and input events are reentered into the queue if any application 
// events are pending; this prevents user- and system-level actions from 
// pre-empting or invalidating application responses to earlier actions
//
// Stop processing notification chain if current event cancels request
//
/*********************************************************/

void dispatch_event(void)
{
   EVENT *EV;
   NREQ *NR;
   LONG nxt;
   LONG typ;
   LONG par;
   LONG own;
   static struct
      {
      LONG parameter;
      LONG owner;
      }
   event_message_descriptor;

   PollMod();

   if ((EV = fetch_event()) == NULL)
      return;

   typ = EV->type;
   par = EV->parameter;
   own = EV->owner;

   if (typ == SYS_FREE)
      return;

   if ((typ >= FIRST_SYS_EVENT) &&
       (typ <= LAST_SYS_EVENT) &&
       (scan_event_range(FIRST_APP_EVENT,LAST_APP_EVENT) != NULL))
      {
      add_event((LONG) typ,(LONG) par,(LONG) own);
      return;
      }

   event_message_descriptor.parameter = par;
   event_message_descriptor.owner = own;

   current_event_type = typ;

   nxt = NR_first[typ];
   while (nxt != -1)
      {
      NR = &NR_list[nxt];
      nxt = NR->next;

      if ((NR->status & NSX_TYPE) != typ) break;
      if  (NR->client == -1)              break;
      if  (typ != current_event_type)     break;

      if (match_parameter((LONG)typ,(LONG)par,(LONG)NR->parameter))
         {
         RT_arguments(&event_message_descriptor,
            sizeof(event_message_descriptor));

         RT_execute((LONG)NR->client,(LONG)NR->message, (ULONG)-1);
         }
      }
}

/*********************************************************/
//
// Dispatch any queued events
//
/*********************************************************/

void drain_event_queue(void)
{
   while (peek_event()) dispatch_event();
}

/*********************************************************/
//
// Place an explicit event in the event queue and return to 
// caller
//
/*********************************************************/


void post_event(LONG argcnt, ULONG owner, LONG event, LONG parameter)
#pragma on (unreferenced)
{
   add_event(event,parameter,owner);
}

/*********************************************************/
//
// Place an explicit event in the event queue and wait until
// all queued events have been dispatched
//
/*********************************************************/


void send_event(LONG argcnt, ULONG owner, LONG event, LONG parameter)
#pragma on (unreferenced)
{
   add_event(event,parameter,owner);
   drain_event_queue();
}

/*********************************************************/
//
// Unconditionally remove all pending events of the specified type
// from the queue
//
/*********************************************************/


void flush_event_queue(LONG argcnt, LONG owner, LONG event, LONG parameter)
#pragma on (unreferenced)
{
   remove_event(event,parameter,owner);
}

/*********************************************************/
//
// Unconditionally remove all input events from the event queue
//
// If event currently being dispatched is an input event, invalidate it
// to keep it from being processed any further
//
/*********************************************************/

void flush_input_events(void)
{
   LONG i;

   for (i=FIRST_INPUT_EVENT;i<=LAST_INPUT_EVENT;i++)
      remove_event(i,-1,-1);

   if ((current_event_type >= FIRST_INPUT_EVENT)
        &&
       (current_event_type <= LAST_INPUT_EVENT))
      current_event_type = SYS_FREE;
}

} // End of namespace Aesop
