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

#ifndef SPYCRAFT_LOGIC_37000_H
#define SPYCRAFT_LOGIC_37000_H

#include "made.h"
#include "game.h"
#include "feature.h"
#include "actor.h"
#include "globals.h"
#include "inset.h"

namespace Spycraft {

class Rm37000 : public Room
{
public:
	Rm37000();
	~Rm37000();
	void init(void);
	int handleEvent(MADEEventStamp *);
};

class SouthExit37000 : public SouthExit
{
public:
	SouthExit37000();
	void respond(void);
	int doVerb(int);
};
/*
class NorthExit37000 : public ExitFeature
{
public:
	NorthExit37000();
	void respond ( void );
	int doVerb ( int );
};
*/
/*features*/
/*class KneeRoof37000 : public TextFeature
{
public:
	KneeRoof37000();
	int doVerb ( int );
};
*/

class PanelVan37000 : public Feature
{
public:
	PanelVan37000();
	void respond();
	int doVerb(int);
};

class PanelSign37000 : public Feature
{
public:
	PanelSign37000();
	void respond();
	int doVerb(int);
};
/*
class Tenement37000 : public TextFeature
{
public:
	Tenement37000();
	int doVerb ( int );
};
*/
/* insets */
class PanelSignInset37000 : public Inset
{
public:
	PanelSignInset37000();
	//	void respond();
};

/* script */
class EnterBadmanVanScript37000 : public Script
{
public:
	EnterBadmanVanScript37000();
	void changeState(int);
};

} // namespace Spycraft

#endif

