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

#include "common/debug.h"
#include "spycraft/game/list.h"
#include "spycraft/game/macro.h"

namespace Spycraft {


extern int quitting;

DeathList TheDeathList;
DeathList *deathList;

CueList TheCueList;
CueList *cueList;

Object *debugListObj;

Node::Node() {
	name = "Node";
	prev = nullptr;
	next = nullptr;
	data = nullptr;
}

List::List() {
	name = "List";
	head = nullptr;
	tail = nullptr;
	size = 0;
	reserved = false;
}

List::~List() {
	Node *t;
	Node *n = head;

	if (reserved) {
		if (!quitting)
			sfxPrintf(" Error list %s reserved: %d from destruct", name, reserved); //DEBUG
#ifdef DO_DEBUG
		LOG_ERROR(LIST_ERROR, reserved);
#endif
		return;
	}

	reserved = 1;

	while (n) {
		if (n->data)
			delete n->data;
		t = n;
		n = n->next;
		delete t;
	}

	head = nullptr;
	tail = nullptr;
	size = 0;

	reserved = false;
}

void List::doit() {
	Node *n = head;
	Node *tmp;

	if (reserved) {
		if (!quitting)
			sfxPrintf(" Error list %s reserved: %d from doit", name, reserved); //DEBUG

#ifdef DO_DEBUG

		LOG_ERROR(LIST_ERROR, reserved);
#endif
		return;
	}

	reserved = 2;

	while (n) {
		tmp = n;
		n = n->next;
		tmp->data->doit();
		ADV_ASSERT(tmp->next == n, 0);
	}

	reserved = false;
	Object::doit();
}

void List::cue() {
	Node *n = head;
	Node *tmp;
	if (reserved) {
		if (!quitting)
			sfxPrintf(" Error list %s reserved: %d from cue", name, reserved); //DEBUG

#ifdef DO_DEBUG

		LOG_ERROR(LIST_ERROR, reserved);
#endif
		return;
	}

	reserved = 10;

	while (n) {
		tmp = n;
		n = n->next;
		tmp->data->cue();
		ADV_ASSERT(tmp->next == n, 0);
	}

	reserved = false;

}

int List::handleEvent(MADEEventStamp *event) {
	Node *n = head;
	Node *tmp;

	while (n) {
		tmp = n;
		n = n->next;
		//DEBUG
		/*if ( n )
			debugListObj = n->data;
		else
			debugListObj = nullptr;*/
			//DEBUG
		if (tmp->data->handleEvent(event)) {
			debugListObj = nullptr;
			return true;
		}
	}
	debugListObj = nullptr;
	return false;
}

void List::add(Object *obj) {
	if (contains(obj))
		return;

	Node *n = new Node;
	ADV_ASSERT(n, 0);
	n->data = obj;

	if (head == nullptr) {
		head = n;
		tail = n;
	} else {
		tail->next = n;
		n->prev = tail;
		tail = n;
	}
	++size;
}

void List::addToFront(Object *obj) {
	if (contains(obj))
		return;

	Node *n = new Node;
	ADV_ASSERT(n, 0);
	n->data = obj;

	if (head == nullptr) {
		head = n;
		tail = n;
	} else {
		n->next = head;
		head->prev = n;
		head = n;
	}
	++size;
}

int List::contains(Object *obj) {
	Node *n = head;

	if (reserved) {
		if (!quitting)
			sfxPrintf("list %s reserved: %d from contains", name, reserved); //DEBUG

#ifdef DO_DEBUG
		LOG_ERROR(LIST_ERROR, reserved);
#endif
		return false;
	}

	reserved = 4;

	while (n) {
		if (obj == n->data) {
			reserved = false;
			return true;
		}
		n = n->next;
	}

	reserved = false;

	return false;
}

int List::del(Object *obj) {
	Node *n = head;

	if (reserved) {
		if (!quitting)
			sfxPrintf(" Error list %s reserved: %d from del", name, reserved); //DEBUG

#ifdef DO_DEBUG
		LOG_ERROR(LIST_ERROR, reserved);
#endif
		return false;
	}

	reserved = 5;

	while (n) {
		if (obj == n->data) {
			if (n->prev) {
				if (n->next) {
					n->prev->next = n->next;
					n->next->prev = n->prev;
				} else {
					n->prev->next = nullptr;
					tail = n->prev;
				}
			} else {
				if (n->next) {
					n->next->prev = nullptr;
					head = n->next;
				} else {
					head = nullptr;
					tail = nullptr;
				}
			}

			if (n->data)
				delete n->data;

			delete n;

			size--;
			reserved = false;
			return true;
		}
		n = n->next;
	}

	reserved = false;

	return false;
}

int List::release(Object *obj) {
	Node *n = head;
	//DEBUG
	//if (obj == debugListObj)	
		//sfxPrintf("Possibly trying to release next node of a list being traversed");

	//DEBUG
	if (reserved) {
		if (!quitting)
			sfxPrintf(" Error list %s reserved: %d from release", name, reserved); //DEBUG
#ifdef DO_DEBUG
		LOG_ERROR(LIST_ERROR, reserved);
#endif
		return false;
	}

	reserved = 3;
	while (n) {
		if (obj == n->data) {
			if (n->prev) {
				if (n->next) {
					n->prev->next = n->next;
					n->next->prev = n->prev;
				} else {
					n->prev->next = nullptr;
					tail = n->prev;
				}
			} else {
				if (n->next) {
					n->next->prev = nullptr;
					head = n->next;
				} else {
					head = nullptr;
					tail = nullptr;
				}
			}

			delete n;

			size--;
			reserved = false;
			return true;
		}
		n = n->next;
	}

	reserved = false;

	return false;
}

Object *List::at(int i) {
	Node *n = head;
	int p = 0;

	if (reserved) {
#ifdef DO_DEBUG
		LOG_ERROR(LIST_ERROR, reserved);
#endif
		return nullptr;
	}

	reserved = 6;

	while (n) {
		if (p == i)
		{
			reserved = false;

			return (n->data);

		}
		n = n->next;
		p++;
	}

	reserved = false;

	return nullptr;
}

int List::indexOf(Object *obj) {
	Node *n = head;
	int p = 0;

	if (reserved) {
#ifdef DO_DEBUG
		LOG_ERROR(LIST_ERROR, reserved);
#endif
		return -1;
	}

	reserved = 7;

	while (n) {
		if (n->data == obj)
		{
			reserved = false;

			return p;
		}
		n = n->next;
		p++;
	}

	reserved = false;

	return -1;
}

void List::releaseAll() {
	Node *t;
	Node *n = head;

	if (reserved) {
#ifdef DO_DEBUG
		LOG_ERROR(LIST_ERROR, reserved);
#endif
		return;
	}

	reserved = 8;

	while (n) {
		t = n;
		n = n->next;
		delete t;
	}

	head = nullptr;
	tail = nullptr;
	size = 0;

	reserved = false;
}

int List::isEmpty() {
	if (head)
		return false;
	else
		return true;
}

void List::dispose() {
	Node *t;
	Node *n = head;

	if (reserved) {
		if (!quitting)
			sfxPrintf(" Error list %s reserved: %d from destruct", name, reserved); //DEBUG
#ifdef DO_DEBUG
		LOG_ERROR(LIST_ERROR, reserved);
#endif
		return;
	}

	reserved = 9;

	while (n) {
		if (n->data)

			//sfxPrintf("delete: %s", n->data->name);

			delete n->data;

		t = n;
		n = n->next;
		delete t;
	}

	head = nullptr;
	tail = nullptr;
	size = 0;

	reserved = false;
}

void List::print() {
	Node *n = head;

	debugN("%s contains: \n", name);

	while (n) {

		sfxPrintf("Node has %p name->%s\n", n->data, n->data->name);
		n = n->next;
	}

	debugN("-- END OF LIST -- \n");
}

DeathList::DeathList() {
	name = "deathList";
	deathList = this;
}

CueList::CueList() {
	name = "cueList";
	cueList = this;
}

} // namespace Spycraft
