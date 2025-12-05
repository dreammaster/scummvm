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

#ifndef SPYCRAFT_LOGIC_36000_H
#define SPYCRAFT_LOGIC_36000_H

#include "made.h"
#include "game.h"
#include "feature.h"
#include "actor.h"
#include "globals.h"
#include "inset.h"
#include "list.h"
#include "plistobj.h"

namespace Spycraft {

class Rm36000 : public Room
{
public:
	Rm36000();
	~Rm36000();
	void cue();
	void init(void);
	int handleEvent(MADEEventStamp *);
};

class NorthExit36000 : public ExitFeature
{
public:
	NorthExit36000();
	void respond(void);
	int doVerb(int);
};

class SouthExit36000 : public SouthExit
{
public:
	SouthExit36000();
	void respond(void);
	int doVerb(int);
};

/* features */
class Camera36000 : public Feature
{
public:
	Camera36000();
	void respond(void);
	int doVerb(int);
};

/* insets */
class CameraInset36000 : public Inset
{
public:
	CameraInset36000();
	//	void respond();
};

/* scripts */
class BackgroundScript36000 : public Script
{
public:
	BackgroundScript36000();
	void changeState(int);
};

class MinuteScript36000 : public Script
{
public:
	MinuteScript36000();
	void changeState(int);
};

/* props */
class MinuteMax36000 : public Prop
{
public:
	MinuteMax36000();
	void cue(void);
};

/*class ProcatTimer : public PermListObject
{
 public:
	ProcatTimer();
	void dispose( void );
};*/

} // namespace Spycraft

#endif

