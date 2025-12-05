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

#ifndef SPYCRAFT_LOGIC_27000_H
#define SPYCRAFT_LOGIC_27000_H

#include "spycraft/game/made.h"
#include "spycraft/game/game.h"
#include "spycraft/game/feature.h"
#include "spycraft/game/actor.h"
#include "spycraft/game/globals.h"
#include "spycraft/game/inset.h"

namespace Spycraft {

/* rooms */
class Rm27000 : public Room
{
public:
	Rm27000();
	void init(void);
};

class Rm27100 : public Room
{
public:
	Rm27100();
	void init();
};

class Rm27200 : public Room
{
public:
	Rm27200();
	void init();
};

/* exits */
// 27000
class SouthExit27000 : public ExitFeature
{
public:
	SouthExit27000();
	void respond(void);
	int doVerb(int);
};

class EastExit27000 : public ExitFeature
{
public:
	EastExit27000();
	void respond(void);
	int doVerb(int);
};

class WestExit27000 : public ExitFeature
{
public:
	WestExit27000();
	void respond(void);
	int doVerb(int);
};

// 27100
class SouthExit27100 : public ExitFeature
{
public:
	SouthExit27100();
	void respond(void);
	int doVerb(int);
};

class EastExit27100 : public ExitFeature
{
public:
	EastExit27100();
	void respond(void);
	int doVerb(int);
};

// 27200
class SouthExit27200 : public ExitFeature
{
public:
	SouthExit27200();
	void respond(void);
	int doVerb(int);
};

class WestExit27200 : public ExitFeature
{
public:
	WestExit27200();
	void respond(void);
	int doVerb(int);
};

/* features */
class Photo27000 : public Feature
{
public:
	Photo27000();
	void respond(void);
	int doVerb(int);
};

class Phone27000 : public Feature
{
public:
	Phone27000();
	void respond(void);
	int doVerb(int);
};

class Camera27000 : public Feature
{
public:
	Camera27000();
	void respond(void);
	int doVerb(int);
};

// 27100
class Phone27100 : public Feature
{
public:
	Phone27100();
	void respond(void);
	int doVerb(int);
};

class Camera27100 : public Feature
{
public:
	Camera27100();
	void respond(void);
	int doVerb(int);
};

class Folder27100 : public Feature
{
public:
	Folder27100();
	void respond(void);
	int doVerb(int);
};

// 27200


/* insets */
//27000
class PhotoInset27000 : public Inset
{
public:
	PhotoInset27000();
	void respond();
	int doVerb(int);
};

class PhoneInset27000 : public Inset
{
public:
	PhoneInset27000();
	void respond();
	int doVerb(int);
};

class CameraInset27000 : public Inset
{
public:
	CameraInset27000();
	void respond();
	int doVerb(int);
};

// 27100
class PhoneInset27100 : public Inset
{
public:
	PhoneInset27100();
	void respond();
	int doVerb(int);
};

class CameraInset27100 : public Inset
{
public:
	CameraInset27100();
	void respond();
	int doVerb(int);
};

class FolderInset27100 : public Inset
{
public:
	FolderInset27100();
};


/* Inset Features */

class PhoneIFeature27000 : public InsetFeature
{
public:
	PhoneIFeature27000();
	void respond(void);
	int doVerb(int);
};

class PhoneIFeature27100 : public InsetFeature
{
public:
	PhoneIFeature27100();
	void respond(void);
	int doVerb(int);
};

class CorrTabIFeature27100 : public InsetFeature
{
public:
	CorrTabIFeature27100();
	void respond(void);
	int doVerb(int);
};

class BSTabIFeature27100 : public InsetFeature
{
public:
	BSTabIFeature27100();
	void respond(void);
	int doVerb(int);
};

} // namespace Spycraft

#endif

