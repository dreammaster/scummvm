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

#ifndef SPYCRAFT_LOGIC_12000_H
#define SPYCRAFT_LOGIC_12000_H

#include "spycraft/game/made.h"
#include "spycraft/game/game.h"
#include "spycraft/game/feature.h"
#include "spycraft/game/actor.h"
#include "spycraft/game/globals.h"
#include "spycraft/game/inset.h"

namespace Spycraft {

class Rm12000 : public Room
{
public:
	Rm12000();
	void init(void);
	//int handleEvent ( MADEEventStamp * );
};

class SouthExit12000 : public ExitFeature
{
public:
	SouthExit12000();
	void respond(void);
	int doVerb(int);
};

class PictureOne12000 : public TextFeature
{
public:
	PictureOne12000();
	void respond(void);
	int doVerb(int);
};

class PictureTwo12000 : public TextFeature
{
public:
	PictureTwo12000();
	void respond(void);
	int doVerb(int);
};

class PictureThree12000 : public TextFeature
{
public:
	PictureThree12000();
	void respond(void);
	int doVerb(int);
};

class PictureFour12000 : public TextFeature
{
public:
	PictureFour12000();
	void respond(void);
	int doVerb(int);
};

class PictureFive12000 : public TextFeature
{
public:
	PictureFive12000();
	void respond(void);
	int doVerb(int);
};

class PictureOneInset12000 : public Inset
{
public:
	PictureOneInset12000();
	void respond();
};

class PictureTwoInset12000 : public Inset
{
public:
	PictureTwoInset12000();
	void respond();
};

class PictureThreeInset12000 : public Inset
{
public:
	PictureThreeInset12000();
	void respond();
};

class PictureFourInset12000 : public Inset
{
public:
	PictureFourInset12000();
	void respond();
};

class PictureFiveInset12000 : public Inset
{
public:
	PictureFiveInset12000();
	void respond();
};


class Script12000 : public Script
{
public:
	Script12000() {
		name = "Script12000";
	};
	void changeState(int);
};

} // namespace Spycraft

#endif

