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

#ifndef SPYCRAFT_LOGIC_26000_H
#define SPYCRAFT_LOGIC_26000_H

#include "made.h"
#include "game.h"
#include "feature.h"
#include "actor.h"
#include "inset.h"
#include "flag.h"

namespace Spycraft {

/* rooms */
class Rm26000 : public Room
{
public:
	Rm26000();
	void init();
};

class Rm26100 : public Room
{
public:
	Rm26100();
	void init();
};

/* exit features */
class SouthExit26000 : public ExitFeature
{
public:
	SouthExit26000();
	void respond(void);
	int doVerb(int);
};

class SouthExit26100 : public ExitFeature
{
public:
	SouthExit26100();
	void respond(void);
	int doVerb(int);
};

/* features */
class Desk26000 : public Feature
{
public:
	Desk26000();
	void respond(void);
	int doVerb(int);
};

class Photos26000 : public Feature
{
public:
	Photos26000();
	void respond(void);
	int doVerb(int);
};

class Map26000 : public Feature
{
public:
	Map26000();
	void respond(void);
	int doVerb(int);
};

class TravelLetter26100 : public Feature
{
public:
	TravelLetter26100();
	void respond(void);
	int doVerb(int);
};

class Folders26100 : public Feature
{
public:
	Folders26100();
	void respond(void);
	int doVerb(int);
};

/* insets */
class MapInset26000 : public Inset
{
public:
	MapInset26000();
};

class PhotosInset26000 : public Inset
{
public:
	PhotosInset26000();
};

class MelnachovInset26000 : public Inset
{
public:
	MelnachovInset26000();
	void dispose(void);
};

class TravelLetterInset26000 : public Inset
{
public:
	TravelLetterInset26000();
};

/* Inset Features */
class MelnachovIFeature26000 : public InsetFeature
{
public:
	MelnachovIFeature26000();
	void respond(void);
	int doVerb(int);
};

/* Scripts */
class FolderSndScript26100 : public Script
{
public:
	FolderSndScript26100() {
		name = "FolderSndScript26100";
	};
	void changeState(int);
};

} // namespace Spycraft

#endif
