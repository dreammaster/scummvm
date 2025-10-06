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

#ifndef SPYCRAFT_GAME_LIST_H
#define SPYCRAFT_GAME_LIST_H

#include "spycraft/game/made.h"
#include "spycraft/game/object.h"

namespace Spycraft {

class Node : public Object {
public:
	Node *prev = nullptr;
	Node *next = nullptr;
	Object *data = nullptr;

	Node();
};

class List : public Object {
	unsigned char reserved;
public:
	short  size = 0;
	Node *head = nullptr;
	Node *tail = nullptr;

	List();
	~List();

	void doit(void);
	void cue(void);
	int handleEvent(MADEEventStamp *);

	virtual void add(Object *obj);
	virtual void addToFront(Object *obj);
	virtual int contains(Object *obj);
	virtual int del(Object *obj);
	virtual int release(Object *obj);
	virtual Object *at(int theIndex);
	virtual int indexOf(Object *obj);
	virtual void releaseAll(void);
	virtual int isEmpty(void);
	virtual void dispose(void);

	void print(void);
};

class DeathList : public List {
public:
	DeathList();
};

extern DeathList *deathList;

class CueList : public List {
public:
	CueList();
};

extern CueList *cueList;

} // namespace Spycraft

#endif

