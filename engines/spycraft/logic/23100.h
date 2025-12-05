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

#ifndef SPYCRAFT_LOGIC_23100_H
#define SPYCRAFT_LOGIC_23100_H

#include "spycraft/game/game.h"
#include "spycraft/game/feature.h"
#include "spycraft/game/actor.h"
#include "spycraft/game/globals.h"
#include "spycraft/game/inset.h"

namespace Spycraft {

class Rm23100 : public Room
{
public:
	Rm23100();
	//~Rm23100();
	void init(void);
	int handleEvent(MADEEventStamp *);
};

class SouthExit23100 : public SouthExit
{
public:
	SouthExit23100();
	void respond(void);
	int doVerb(int);
};

/* features */
class Microphone23100 : public Feature
{
public:
	Microphone23100();
	void respond(void);
	int doVerb(int);
};

class TapePlayer23100 : public Feature
{
public:
	TapePlayer23100();
	void respond(void);
	int doVerb(int);
};

class PolygraphFinger23100 : public Feature
{
public:
	PolygraphFinger23100();
	void respond(void);
	int doVerb(int);
};
/*
class PolygraphRead23100 : public TextFeature
{
public:
	PolygraphRead23100();
	void respond( void );
	int doVerb( int );
};
*/
/* insets */
class MicrophoneInset23100 : public Inset
{
public:
	MicrophoneInset23100();
	//	void respond();
};

class TapePlayerInset23100 : public Inset
{
public:
	TapePlayerInset23100();
	//	void respond();
};

class PolygraphFingerInset23100 : public Inset
{
public:
	PolygraphFingerInset23100();
	//	void respond();
};
/*
class PolygraphReadInset23100: public Inset
{
 public:
	PolygraphReadInset23100();
//	void respond();
};
*/

} // namespace Spycraft

#endif

