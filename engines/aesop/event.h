/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef AESOP_EVENT_H
#define AESOP_EVENT_H

namespace Aesop {

#ifdef __cplusplus
extern "C" {
#endif

#define EV_QSIZE 128             // max # of queued events (circular)
#define NR_LSIZE 768             // max # of event notification requests

#define NSX_IN_REGION  0x100     // notification status flags (high word)
#define NSX_OUT_REGION 0x200
#define NSX_TYPE       0x00FF    // notification event type mask (low word)

typedef struct NREQ
{
   int32  next;
   int32  prev;
   int32  client;
   uint32 message;
   int32  parameter;
   int32  status;
}
NREQ;                            // notification request list entry

typedef struct
{
   int32 type;
   int32 owner;
   int32 parameter;
}
EVENT;

extern int32 ENABLED;

extern NREQ NR_list[NR_LSIZE];
extern int32 NR_first[NUM_EVTYPES];

extern int32 current_event_type;

//
// Internal calls
//

void init_notify_list(void);
void add_notify_request(int32 client, int32 message, int32 event, int32
   parameter);
void delete_notify_request(int32 client, int32 message, int32 event,
   int32 parameter);
void cancel_entity_requests(int32 client);
void init_event_queue(void);
EVENT *find_event(int32 type, int32 parameter);
void remove_event(int32 type, int32 parameter, int32 owner);
void add_event(int32 type, int32 parameter, int32 owner);
EVENT *next_event(void);
EVENT *fetch_event(void);
void dump_event_queue(void);

void DISABLE(void);
void ENABLE(void);

//
// AESOP code resource calls
//

void notify(int32 argcnt, uint32 index, uint32 message, int32 event,
   int32 parameter);
void cancel(int32 argcnt, uint32 index, uint32 message, int32 event,
   int32 parameter);
void drain_event_queue(void);
void post_event(int32 argcnt, uint32 owner, int32 event, int32 parameter);
void send_event(int32 argcnt, uint32 owner, int32 event, int32 parameter);
uint32 peek_event(void);
void dispatch_event(void);
void flush_event_queue(int32 argcnt, int32 owner, int32 event, int32 parameter);

#ifdef __cplusplus
}
#endif

} // namespace Aesop

#endif
