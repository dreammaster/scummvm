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

//
//  Run-time event handler calls
//

#ifndef AESOP_EVENT_H
#define AESOP_EVENT_H

#include "aesop/defs.h"
#include "aesop/shared.h"

namespace Aesop {

#define EV_QSIZE 128             // max # of queued events (circular)
#define NR_LSIZE 768             // max # of event notification requests

#define NSX_IN_REGION  0x100     // notification status flags (high word)
#define NSX_OUT_REGION 0x200
#define NSX_TYPE       0x00FF    // notification event type mask (low word)

typedef struct NREQ {
	LONG  next;
	LONG  prev;
	LONG  client;
	ULONG message;
	LONG  parameter;
	LONG  status;
} NREQ;                            // notification request list entry

typedef struct {
	LONG type;
	LONG owner;
	LONG parameter;
} EVENT;

extern LONG ENABLED;

extern NREQ NR_list[NR_LSIZE];
extern LONG NR_first[NUM_EVTYPES];

extern LONG current_event_type;

//
// Internal calls
//

void init_notify_list();
void add_notify_request(LONG client, LONG message, ULONG event, LONG
	parameter);
void delete_notify_request(LONG client, LONG message, ULONG event,
	LONG parameter);
void cancel_entity_requests(LONG client);
void init_event_queue();
EVENT *find_event(LONG type, LONG parameter);
void remove_event(LONG type, LONG parameter, LONG owner);
void add_event(LONG type, LONG parameter, LONG owner);
EVENT *next_event();
EVENT *fetch_event();
void dump_event_queue();

void DISABLE();
void ENABLE();

//
// AESOP code resource calls
//

void notify(LONG argcnt, ULONG index, ULONG message, LONG event,
	LONG parameter);
void cancel(LONG argcnt, ULONG index, ULONG message, LONG event,
	LONG parameter);
void drain_event_queue();
void post_event(LONG argcnt, ULONG owner, LONG event, LONG parameter);
void send_event(LONG argcnt, ULONG owner, LONG event, LONG parameter);
ULONG peek_event();
void dispatch_event();
void flush_event_queue(LONG argcnt, LONG owner, LONG event, LONG parameter);

} // namespace Aesop

#endif
