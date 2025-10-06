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

#ifndef SPYCRAFT_GAME_OBJECT_H
#define SPYCRAFT_GAME_OBJECT_H

#include "spycraft/game/made.h"

namespace Spycraft {

#define OBJECT_HIDDEN		0x0001
#define OBJECT_LOCALIZED	0x0002

extern int IsStatic(void *);

class Script;

class Object {
public:
	char *name;
	Script *script;
	unsigned int 	flag;

#ifdef DO_DEBUG

	void *operator new (size_t);
	void operator delete (void *);

#endif

	Object();
	virtual ~Object();

	virtual void cue(void) {
	};
	virtual void doit(void);
	virtual int handleEvent(MADEEventStamp *) {
		return false;
	};
	virtual void setScript(Script *);
	virtual void setScript(Script *, Object *);
	virtual void setScript(Script *, Object *, void *);
	virtual void dispose(void);
};

extern int _object_allocated;

} // namespace Spycraft

#endif
