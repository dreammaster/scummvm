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

#ifndef AESOP_EVENTS_H
#define AESOP_EVENTS_H

#include "common/scummsys.h"
#include "common/events.h"
#include "common/keyboard.h"
#include "common/stack.h"

namespace Aesop {

#define GAME_FRAME_RATE 60
#define GAME_FRAME_TIME (1000 / GAME_FRAME_RATE)
#define EV_QSIZE 128             // max # of queued events (circular)
#define NR_LSIZE 768             // max # of event notification requests
#define NUM_EVTYPES 256

enum EventType {
	SYS_FREE				= 0,
	SYS_TIMER				= 1,
	SYS_MOUSEMOVE			= 2,
	SYS_ENTER_REGION		= 3,
	SYS_LEAVE_REGION		= 4,
	SYS_LEFT_CLICK			= 5,
	SYS_LEFT_RELEASE		= 6,
	SYS_RIGHT_CLICK			= 7,
	SYS_RIGHT_RELEASE		= 8,
	SYS_CLICK				= 9,
	SYS_RELEASE				= 10,
	SYS_LEFT_CLICK_REGION	= 11,
	SYS_LEFT_RELEASE_REGION	= 12,
	SYS_RIGHT_CLICK_REGION	= 13,
	SYS_RIGHT_RELEASE_REGION= 14,
	SYS_CLICK_REGION		= 15,
	SYS_RELEASE_REGION		= 16,
	SYS_KEYDOWN				= 17
};
enum NSX {
	NSX_IN_REGION  = 0x100,		// notification status flags (high word)
	NSX_OUT_REGION = 0x200,
	NSX_TYPE       = 0x00FF		// notification event type mask (low word)
};

class AesopEngine;

struct AesopEvent {
	int type;
	int owner;
	int parameter;

	AesopEvent() : type(0), owner(0), parameter(0) {}
	AesopEvent(int eventType, int eventOwner, int eventParameter) :
		type(eventType), owner(eventOwner), parameter(eventParameter) {}
};

// notification request list entry
struct NREQ {
	int next;
	int prev;
	int client;
	uint32 message;
	int parameter;
	int status;
};

class Events {
private:
	AesopEngine *_vm;
	uint32 _heartbeat;
	uint32 _priorFrameTime;
	Common::List<AesopEvent> _eventsQueue;
	NREQ NR_list[NR_LSIZE];
	int NR_first[NUM_EVTYPES];
	int _currentEventType;
	bool _interfaceActive;
	Common::Point _mousePos, _lastMousePos;
	bool _btnLeft, _btnRight;
	bool _btnLastLeft, _btnLastRight;
	int _locked;

	/**
	 * Initializes the notify list
	 */
	void initNotifyList(void);

	/**
	 * Check whether it's time to display the next screen frame
	 */
	bool checkForNextFrameCounter();

	/**
	 * Timer callback routine; check for pending keyboard characters 60 times per second, 
	 * and post SYS_TIMER events to the game's event queue 30 times per second.
	 *
	 * If a SYS_TIMER message is already in the queue, update its "heartbeat" parameter 
	 * instead of posting a new message
	 *
	 * Don't update system heartbeat count while hourglass cursor is active
	 * If a keyboard character is pending, read the scan code and ASCII/extended code,
	 * post a SYS_KEYDOWN event
	 */
	void timerCallback();

	AesopEvent *findEvent(int type, int parameter);

	/**
	 * Add an event to the event queue
	 */
	void addEvent(int type, int parameter, int owner);

	/**
	 * Remove an event from the event queue
	 */
	void removeEvent(int type, int parameter, int owner);

	void addRegionEvent(int type, int owner);

	/**
	 * Append notification request to end of list
	 * Handlers for a given event are called in order of registration
	 */
	void addNotifyRequest(int client, uint32 message, int event, int parameter);

	/**
	 * Cancel a specific notification request for a given entity or program object
	 */
	void deleteNotifyRequest(int client, uint32 message, int event, int parameter);

	AesopEvent *scanEventRange(int firstType, int lastType);

	/**
	 * Return the next pending game event, if any
	 */
	const AesopEvent *nextEvent() const;

	/**
	 * Return the next pending game event if any, and remove it from the pending list
	 */
	bool fetchEvent(AesopEvent &EV);

	/**
	 * Most event notifications are triggerable when/if the event's parameter 
	 * is equal to the notify request's parameter.  SYS_TIMER events, however, 
	 * become recognizable if the event parameter (heartbeat) is greater than
	 * or equal to the notification parameter.
	 *
	 * SYS_FREE events do not match any notification type
	 *
	 * A notification parameter of -1 matches all event parameters
	 */
	int matchParameter(int eventType, int eventParameter, int testParameter);

	/**
	 * Handles changes in the mouse position
	 */
	void mouseEventHandler();

	/**
	 * Handles changes in the left or right mouse button pressed state
	 */
	void mouseButtonEventHandler();

	/**
	 * Handles keyboard events
	 */
	void keyboardEventHandler(const Common::KeyState &keyState);
public:
	bool _pressed;
public:
	Events(AesopEngine *vm);
	~Events();

	/**
	 * Clear all pending events
	 */
	void clearEvents();

	/**
	 * Returns true if the mouse cursor is visible
	 */
	bool isCursorVisible() const;

	/**
	 * Show the mouse cursor
	 */
	void showMouse();

	/**
	 * Hide the mouse cursor
	 */
	void hideMouse();

	/**
	 * Lock the mouse, disabling all mouse event processing
	 */
	void lockMouse();

	/**
	 * Unlock the mouse, re-enabling mouse event processing
	 */
	void unlockMouse();

	/**
	 * Get the mouse position
	 */
	uint getXY() const;

	/**
	 * Set the mouse position
	 */
	uint setXY(const Common::Point &pt);

	/**
	 * Returns true if the mouse is within a given window
	 */
	bool mouseInWindow(uint wnd);

	/**
	 * Check for any pending events
	 */
	void pollEvents();

	/**
	 * Poll for events and introduce a small delay, to allow the system to
	 * yield to other running programs
	 */
	void pollEventsAndWait();

	/**
	 * Delay for a specified time
	 */
	void delay(uint32 time);

	/**
	 * Get a pending key
	 */
	void getKey();

	/**
	 * Return the current game frame counter
	 */
	uint32 frameCounter() const { return _heartbeat; }

	/**
	 * Cancel all notification requests for a given entity or program object
	 * (-1 to cancel all requests for all entity objects)
	 */
	void cancelEntityRequests(int client);

	/**
	 * Send / message / to / index / when / event / of type / parameter / occurs
	 */
	void notify(uint index, uint message, int event, int parameter);

	/**
	 * Cancel a notification request set up by notify() above
	 */
	void cancel(uint index, uint message, int eventType, int parameter);

	/**
	 * Returns true if there's a pending event to process
	 */
	bool peekEvent() const;

	/**
	 * Fetch the next event from the queue and fulfill any of its notification requests.
	 *
	 * System and input events are re-entered into the queue if any application 
	 * events are pending; this prevents user- and system-level actions from 
	 * pre-empting or invalidating application responses to earlier actions
	 *
	 * Stop processing notification chain if current event cancels request
	 */
	void dispatchEvent();

	/**
	 * Dispatch any queued events
	 */
	void drainEventQueue();

	/**
	 * Unconditionally remove all pending events of the specified type from the queue
	 */
	void flushEventQueue(uint owner, int eventType, int parameter);

	/**
	 * Unconditionally remove all input events from the event queue
	 *
	 * If event currently being dispatched is an input event, invalidate it
	 * to keep it from being processed any further
	 */
	void flushInputEvents();

	/**
	 * Place an explicit event in the event queue and return to caller
	 */
	void postEvent(uint owner, int eventType, int parameter);

	/**
	 * Place an explicit event in the event queue and wait until
	 * all queued events have been dispatched
	 */
	void sendEvent(uint owner, int eventType, int parameter);
};

} // End of namespace Aesop

#endif /* AESOP_EVENTS_H */
