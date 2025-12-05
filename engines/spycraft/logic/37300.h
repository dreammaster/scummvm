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

#ifndef SPYCRAFT_LOGIC_37300_H
#define SPYCRAFT_LOGIC_37300_H

#include "spycraft/game/game.h"
#include "spycraft/game/feature.h"
#include "spycraft/game/actor.h"
#include "spycraft/game/globals.h"
#include "spycraft/game/inset.h"

namespace Spycraft {

class Rm37300 : public Room
{
public:
	Rm37300();
	~Rm37300();
	void init(void);
	int handleEvent(MADEEventStamp *);
};

class EastExit37300 : public ExitFeature
{
public:
	EastExit37300();
	void respond(void);
	int doVerb(int);
};
/*
class SouthExit37300 : public SouthExit
{
public:
	SouthExit37300();
	void respond ( void );
	int doVerb ( int );
};
*/
/* features */
class EgyptianPhotos37300 : public Feature
{
public:
	EgyptianPhotos37300();
	void respond(void);
	int doVerb(int);
};

/* insets */
class EgyptianPhotosInset37300 : public Inset
{
public:
	EgyptianPhotosInset37300();
	//	void respond();
};

} // namespace Spycraft

#endif

