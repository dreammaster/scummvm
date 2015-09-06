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

#include "common/scummsys.h"
#include "common/events.h"
#include "common/system.h"
#include "engines/util.h"
#include "graphics/cursorman.h"
#include "aesop/aesop.h"
#include "aesop/events.h"
#include "aesop/shared.h"

namespace Aesop {

enum ButtonFlag { LEFT_BUTTON = 1, RIGHT_BUTTON = 2 };


static const char *const strs[] = {"SYS_FREE",
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
                                   "SYS_KEYDOWN"
                                  };

/*----------------------------------------------------------------*/

Events::Events(AesopEngine *vm) : _vm(vm) {
	_heartbeat = 1;
	_priorFrameTime = 0;
	_interfaceActive = true;
	_btnLeft = _btnRight = false;
	_btnLastLeft = _btnLastRight = false;
	_locked = 0;

	initNotifyList();
}

Events::~Events() {
}

void Events::initNotifyList(void) {
	for (int i = 0; i < NUM_EVTYPES; i++)
		NR_first[i] = -1;

	NR_first[SYS_FREE] = 0;

	for (int i = 0; i < NR_LSIZE; i++) {
		NR_list[i].next = i + 1;
		NR_list[i].prev = i - 1;
	}

	NR_list[NR_LSIZE - 1].next = -1;
}

void Events::showMouse() {
	CursorMan.showMouse(true);
}

void Events::hideMouse() {
	CursorMan.showMouse(false);
}

void Events::lockMouse() {
	++_locked;
}

void Events::unlockMouse() {
	assert(_locked > 0);
	--_locked;
}

uint Events::getXY() const {
	return ((ULONG)_mousePos.y << 16) + _mousePos.x;
}

uint Events::setXY(const Common::Point &pt) {
	_mousePos = pt;
	return getXY();
}

bool Events::isCursorVisible() const {
	return CursorMan.isVisible();
}

void Events::pollEvents() {
	checkForNextFrameCounter();

	Common::Event event;
	while (g_system->getEventManager()->pollEvent(event)) {
		// Handle events
		switch (event.type) {
		case Common::EVENT_QUIT:
		case Common::EVENT_RTL:
			return;

		case Common::EVENT_KEYDOWN:
			// Check for debugger
			if (event.kbd.keycode == Common::KEYCODE_d && (event.kbd.flags & Common::KBD_CTRL)) {
				// Attach to the debugger
				_vm->_debugger->attach();
				_vm->_debugger->onFrame();
			} else {
				keyboardEventHandler(event.kbd);
			}
			return;
		case Common::EVENT_KEYUP:
			return;
		case Common::EVENT_LBUTTONDOWN:
			_btnLeft = true;
			mouseButtonEventHandler();
			return;
		case Common::EVENT_LBUTTONUP:
			_btnLeft = false;
			mouseButtonEventHandler();
			return;
		case Common::EVENT_RBUTTONDOWN:
			_btnRight = true;
			mouseButtonEventHandler();
			return;
		case Common::EVENT_RBUTTONUP:
			_btnRight = false;
			mouseButtonEventHandler();
			return;
		default:
			break;
		}
	}
}

void Events::pollEventsAndWait() {
	pollEvents();
	g_system->delayMillis(10);
}

bool Events::checkForNextFrameCounter() {
	// Check for next game frame
	uint32 milli = g_system->getMillis();
	if ((milli - _priorFrameTime) >= GAME_FRAME_TIME) {
		timerCallback();
		_priorFrameTime = milli;

		// Give time to the debugger
		_vm->_debugger->onFrame();

		// Display the frame
		_vm->_screen->update();

		return true;
	}

	return false;
}

void Events::clearEvents() {
	_btnLeft = _btnRight = false;
	mouseButtonEventHandler();
}

void Events::delay(uint32 time) {
	// Different handling for really short versus extended times
	if (time < 10) {
		// For really short periods, simply delay by the desired amount
		pollEvents();
		g_system->delayMillis(time);
	} else {
		// For long periods go into a loop where we delay by 10ms at a time and then
		// check for events. This ensures for longer delays that responsiveness is
		// maintained
		uint32 delayEnd = g_system->getMillis() + time;

		while (!_vm->shouldQuit() && g_system->getMillis() < delayEnd) {
			pollEventsAndWait();
		}
	}
}

void Events::getKey() {
	while (!_vm->shouldQuit() && !findEvent(SYS_KEYDOWN, -1))
		delay(5);

	removeEvent(SYS_KEYDOWN, -1L, -1);
}

void Events::timerCallback() {
	AesopEvent *EV;
	++_heartbeat;

	// Generate SYS_TIMER events every alternate timer call, so there are 30 per second
	if (!(_heartbeat & 1)) {
		if ((EV = findEvent(SYS_TIMER, -1)) == nullptr)
			addEvent(SYS_TIMER, _heartbeat >> 1, -1);
		else
			EV->parameter = _heartbeat >> 1;
	}
}

AesopEvent *Events::findEvent(int type, int parameter) {
	for (Common::List<AesopEvent>::iterator i = _eventsQueue.begin(); i != _eventsQueue.end(); ++i) {
		AesopEvent &EV = *i;

		if (EV.type != type)
			continue;

		if (!matchParameter(EV.type, EV.parameter, parameter)) continue;

		return &EV;
	}

	return nullptr;
}

void Events::removeEvent(int type, int parameter, int owner) {
	for (Common::List<AesopEvent>::iterator i = _eventsQueue.begin(); i != _eventsQueue.end(); ++i) {
		AesopEvent &EV = *i;

		if ((owner == -1) || (EV.owner == owner))
			if ((type == -1) || (EV.type == type))
				if (matchParameter(EV.type, EV.parameter, parameter)) {
					i = _eventsQueue.erase(i);
					continue;
				}

		++i;
	}
}

void Events::addRegionEvent(int type, int owner) {
	WORD nxt;
	ULONG r;
	NREQ *NR;

	nxt = NR_first[type];
	while (nxt != -1) {
		NR = &NR_list[nxt];
		nxt = NR->next;
		r = NR->parameter;

		if (mouseInWindow(r))
			addEvent(type, r, owner);
	}
}

void Events::addEvent(int type, int parameter, int owner) {
	_eventsQueue.push_back(AesopEvent(type, owner, parameter));
}

const AesopEvent *Events::nextEvent() const {
	return _eventsQueue.empty() ? nullptr : &_eventsQueue.front();
}

bool Events::fetchEvent(AesopEvent &ev) {
	if (_eventsQueue.empty())
		return nullptr;

	ev = _eventsQueue.front();
	_eventsQueue.erase(_eventsQueue.begin());

	return true;
}

void Events::cancelEntityRequests(int client) {
	int event, nxt, cur, prev;
	int fnxt, fcur = 0;
	NREQ *NR;

	// for all event types...
	for (event = 1; event < NUM_EVTYPES; event++) {
		nxt = NR_first[event];                      // start at chain beginning

		while (nxt != -1) {                         // while not at end of chain
			cur = nxt;                               // get links
			NR = &NR_list[cur];
			nxt = NR->next;
			prev = NR->prev;

			if (client == -1) {                      // for all clients?
				if ((NR->client >= NUM_ENTITIES) ||   // yes, filter out all non-
				        (NR->client == -1))               // entity clients
					continue;
			} else                                   // else match specified
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
			if (fnxt == -1) {
				NR_first[SYS_FREE] = cur;
				NR->prev = -1;
			} else {
				while (fnxt != -1)
					fnxt = NR_list[fcur = fnxt].next;

				NR_list[fcur].next = cur;
				NR->prev = fcur;
			}
		}
	}
}

AesopEvent *Events::scanEventRange(int firstType, int lastType) {
	for (Common::List<AesopEvent>::iterator i = _eventsQueue.begin(); i != _eventsQueue.end(); ++i) {
		AesopEvent &EV = *i;

		if ((EV.type < firstType) || (EV.type > lastType))
			continue;

		return &EV;
	}

	return nullptr;
}

int Events::matchParameter(int eventType, int eventParameter, int testParameter) {
	int match;

	if (eventType == SYS_FREE)
		return 0;

	if (testParameter == -1L)
		return 1;

	switch (eventType) {
	case SYS_TIMER:
		match = (eventParameter >= testParameter);
		break;

	default:
		match = (eventParameter == testParameter);
		break;
	}

	return match;
}

void Events::mouseEventHandler() {
	WORD nxt;
	LONG r;
	NREQ *NR;
	AesopEvent *EV;

	if ((EV = findEvent(SYS_MOUSEMOVE, -1L)) == NULL)
		addEvent(SYS_MOUSEMOVE, ((ULONG)_mousePos.y << 16) | (ULONG)_mousePos.x, -1);
	else
		EV->parameter = ((ULONG)_mousePos.y << 16) | (ULONG)_mousePos.x;

	nxt = NR_first[SYS_ENTER_REGION];
	while (nxt != -1) {
		NR = &NR_list[nxt];
		nxt = NR->next;
		r = NR->parameter;

		if (mouseInWindow(r)) {
			if (NR->status & NSX_IN_REGION) continue;

			NR->status |= NSX_IN_REGION;

			addEvent(SYS_ENTER_REGION, r, -1);
		} else
			NR->status &= (~NSX_IN_REGION);
	}

	nxt = NR_first[SYS_LEAVE_REGION];
	while (nxt != -1) {
		NR = &NR_list[nxt];
		nxt = NR->next;
		r = NR->parameter;

		if (!mouseInWindow(r)) {
			if (!(NR->status & NSX_OUT_REGION)) continue;

			NR->status &= (~NSX_OUT_REGION);

			addEvent(SYS_LEAVE_REGION, r, -1);
		} else
			NR->status |= NSX_OUT_REGION;
	}
}

void Events::mouseButtonEventHandler() {
	if (_btnLeft != _btnLastLeft) {
		addEvent(_btnLeft ? SYS_CLICK : SYS_RELEASE, 0, -1);
		addEvent(_btnLeft ? SYS_LEFT_CLICK : SYS_LEFT_RELEASE, 0, -1);
		addRegionEvent(_btnLeft ?
		               SYS_LEFT_CLICK_REGION : SYS_LEFT_RELEASE_REGION, -1);
		addRegionEvent(_btnLeft ?
		               SYS_CLICK_REGION : SYS_RELEASE_REGION, -1);

		_btnLastLeft = _btnLeft;
	}

	if (_btnRight != _btnLastRight) {
		addEvent(_btnRight ? SYS_CLICK : SYS_RELEASE, 0, -1);
		addEvent(_btnRight ? SYS_RIGHT_CLICK : SYS_RIGHT_RELEASE, 0, -1);
		addRegionEvent(_btnRight ?
		               SYS_RIGHT_CLICK_REGION : SYS_RIGHT_RELEASE_REGION, -1);
		addRegionEvent(_btnRight ? SYS_CLICK_REGION : SYS_RELEASE_REGION, -1);

		_btnLastRight = _btnRight;
	}
}

void Events::keyboardEventHandler(const Common::KeyState &keyState) {
	int key = keyState.keycode;
	uint ascii = keyState.ascii;

	if ((key == Common::KEYCODE_KP5) || (ascii == '\0') || (ascii == 0xe0))
		addEvent(SYS_KEYDOWN, key, -1);
	else
		addEvent(SYS_KEYDOWN, ascii, -1);
}

bool Events::mouseInWindow(uint wnd) {
	return _vm->_screen->panes(wnd).contains(_mousePos);
}

void Events::addNotifyRequest(int client, uint32 message, int event, int parameter) {
	int i, nxt, cur = 0;
	NREQ *NR;

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

	if (nxt == -1) {
		NR_first[event] = i;
		NR->prev = -1;
	} else {
		while (nxt != -1)
			nxt = NR_list[cur = nxt].next;

		NR_list[cur].next = i;
		NR->prev = cur;
	}
}

void Events::deleteNotifyRequest(int client, uint32 message, int event, int parameter) {
	LONG nxt, prev;
	LONG fnxt;
	LONG cur = 0, fcur = 0;
	NREQ *NR;
	LONG all_events;

	if (event == (unsigned) - 1) {
		event = 1;
		all_events = 1;
	} else {
		all_events = 0;
	}

	do {
		nxt = NR_first[event];                      // start at chain beginning

		while (nxt != -1) {                         // while not at end of chain
			cur = nxt;                               // get links
			NR = &NR_list[cur];
			nxt = NR->next;
			prev = NR->prev;

			if (NR->client != client) continue;      // match specified parms

			if (((ULONG)message != (unsigned) - 1) && ((ULONG)message != NR->message))
				continue;

			if (!matchParameter(event, NR->parameter, parameter)) continue;

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
			if (fnxt == -1) {
				NR_first[SYS_FREE] = cur;
				NR->prev = -1;
			} else {
				while (fnxt != -1)
					fnxt = NR_list[fcur = fnxt].next;

				NR_list[fcur].next = cur;
				NR->prev = fcur;
			}
		}
	} while (all_events && (++event < NUM_EVTYPES));
}

void Events::notify(uint index, uint message, int event, int parameter) {
	addNotifyRequest(index, message, event, parameter);
}

void Events::cancel(uint index, uint message, int eventType, int parameter) {
	deleteNotifyRequest(index, message, eventType, parameter);
}

bool Events::peekEvent() const {
	return nextEvent() != nullptr;
}

void Events::dispatchEvent() {
	Interpreter &interp = *_vm->_interpreter;
	AesopEvent EV;
	NREQ *NR;
	int nxt;
	int typ;
	int par;
	int own;

	if (!fetchEvent(EV))
		return;

	typ = EV.type;
	par = EV.parameter;
	own = EV.owner;

	if (typ == SYS_FREE)
		return;

	if ((typ >= FIRST_SYS_EVENT) && (typ <= LAST_SYS_EVENT) &&
	        (scanEventRange(FIRST_APP_EVENT, LAST_APP_EVENT) != nullptr)) {
		addEvent(typ, par, own);
		return;
	}

	_currentEventType = typ;

	nxt = NR_first[typ];
	while (nxt != -1) {
		NR = &NR_list[nxt];
		nxt = NR->next;

		if ((NR->status & NSX_TYPE) != typ) break;
		if (NR->client == -1)               break;
		if (typ != _currentEventType)       break;

		if (matchParameter(typ, par, NR->parameter)) {
			interp.addArgument(par);
			interp.addArgument(own);
			interp.execute(NR->client, NR->message);
		}
	}
}

void Events::drainEventQueue() {
	while (peekEvent())
		dispatchEvent();
}

void Events::flushEventQueue(uint owner, int eventType, int parameter) {
	removeEvent(eventType, parameter, owner);
}

void Events::flushInputEvents() {
	for (int idx = FIRST_INPUT_EVENT; idx <= LAST_INPUT_EVENT; ++idx)
		removeEvent(idx, -1, -1);

	if ((_currentEventType >= FIRST_INPUT_EVENT) && (_currentEventType <= LAST_INPUT_EVENT))
		_currentEventType = SYS_FREE;
}

void Events::postEvent(uint owner, int eventType, int parameter) {
	addEvent(eventType, parameter, owner);
}

void Events::sendEvent(uint owner, int eventType, int parameter) {
	addEvent(eventType, parameter, owner);
	drainEventQueue();
}

} // End of namespace Sherlock
