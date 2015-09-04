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

#ifndef AESOP_EVENT_H
#define AESOP_EVENT_H

#include "aesop/shared.h"

namespace Aesop {

//
//  Run-time event handler calls
//

#define EV_QSIZE 128             // max # of queued events (circular)
#define NR_LSIZE 768             // max # of event notification requests

#define NSX_IN_REGION  0x100     // notification status flags (high word)
#define NSX_OUT_REGION 0x200
#define NSX_TYPE       0x00FF    // notification event type mask (low word)

// notification request list entry
struct NREQ {
   LONG  next;
   LONG  prev;
   LONG  client;
   ULONG message;
   LONG  parameter;
   LONG  status;
};

struct EVENT {
   LONG type;
   LONG owner;
   LONG parameter;
};

extern LONG ENABLED;

extern NREQ NR_list[NR_LSIZE];
extern LONG NR_first[NUM_EVTYPES];

extern LONG current_event_type;

//
// Internal calls
//

extern void init_notify_list(void);
extern void add_notify_request(LONG client, ULONG message, ULONG event, LONG
   parameter);
extern void delete_notify_request(LONG client, ULONG message, ULONG event,
   LONG parameter);
extern void cancel_entity_requests(LONG client);
extern void init_event_queue(void);
extern EVENT *find_event(LONG type, LONG parameter);
extern void remove_event(LONG type, LONG parameter, LONG owner);
extern void add_event(LONG type, LONG parameter, LONG owner);
extern EVENT *next_event(void);
extern EVENT *fetch_event(void);
extern void dump_event_queue(void);

extern void DISABLE(void);
extern void ENABLE(void);

//
// AESOP code resource calls
//

extern void notify(LONG argcnt, ULONG index, ULONG message, LONG event,
   LONG parameter);
extern void cancel(LONG argcnt, ULONG index, ULONG message, LONG event,
   LONG parameter);
extern void drain_event_queue(void);
extern void post_event(LONG argcnt, ULONG owner, LONG event, LONG parameter);
extern void send_event(LONG argcnt, ULONG owner, LONG event, LONG parameter);
extern ULONG peek_event(void);
extern void dispatch_event(void);
extern void flush_event_queue(LONG argcnt, LONG owner, LONG event, LONG parameter);

} // End of namespace Aesop

#endif

