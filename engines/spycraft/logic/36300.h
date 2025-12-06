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

#ifndef SPYCRAFT_LOGIC_36300_H
#define SPYCRAFT_LOGIC_36300_H

#include "spycraft/game/made.h"
#include "spycraft/game/game.h"
#include "spycraft/game/feature.h"
#include "spycraft/game/actor.h"
#include "spycraft/game/inset.h"
#include "spycraft/game/flag.h"
#include "spycraft/game/invent.h"

namespace Spycraft {

class Rm36300 : public Room
{
public:
	Rm36300();
	~Rm36300();
	void init();
	int handleEvent(MADEEventStamp *);
};

class SouthExit36300 : public ExitFeature
{
public:
	SouthExit36300();
	void respond(void);
	int doVerb(int);
};

/* features */
class Drawer36300 : public Feature
{
public:
	Drawer36300();
	void respond(void);
	int doVerb(int);
	int	playerHasAllFolders(Invent *inventory);
};

/* insets */
class DrawerWFolderInset36300 : public Inset
{
public:
	DrawerWFolderInset36300();
};
/*
class EmptyDrawerInset36300 : public Inset
{
public:
	EmptyDrawerInset36300();
};
*/
/* inset features */
/*
class FolderInsetFeature36300 : public InsetFeature
{
public:
	FolderInsetFeature36300();
	void respond( void );
	int doVerb ( int );
};

class PaymentsInsetFeature36300 : public InsetFeature
{
public:
	PaymentsInsetFeature36300();
	void respond( void );
	int doVerb ( int );
};

class CitationInsetFeature36300 : public InsetFeature
{
public:
	CitationInsetFeature36300();
	void respond( void );
	int doVerb ( int );
};

class ZhironevInsetFeature36300 : public InsetFeature
{
public:
	ZhironevInsetFeature36300();
	void respond( void );
	int doVerb ( int );
};

class OrdersInsetFeature36300 : public InsetFeature
{
public:
	OrdersInsetFeature36300();
	void respond( void );
	int doVerb ( int );
};

class ShustakInsetFeature36300 : public InsetFeature
{
public:
	ShustakInsetFeature36300();
	void respond( void );
	int doVerb ( int );
};
*/
class Rm36301 : public Room
{
public:
	Rm36301();
	~Rm36301();
	void init();
	int handleEvent(MADEEventStamp *);
};

class SouthExit36301 : public ExitFeature
{
public:
	SouthExit36301();
	void respond(void);
	int doVerb(int);
};

class Rm36302 : public Room
{
public:
	Rm36302();
	~Rm36302();
	void init();
	int handleEvent(MADEEventStamp *);
};

class SouthExit36302 : public ExitFeature
{
public:
	SouthExit36302();
	void respond(void);
	int doVerb(int);
};

class OpenDrawer36300 : public Feature
{
public:
	OpenDrawer36300();
	void respond(void);
	int doVerb(int);
};

class Rm36303 : public Room
{
public:
	Rm36303();
	~Rm36303();
	void init();
	int handleEvent(MADEEventStamp *);
};

class SouthExit36303 : public ExitFeature
{
public:
	SouthExit36303();
	void respond(void);
	int doVerb(int);
};

} // namespace Spycraft

#endif
