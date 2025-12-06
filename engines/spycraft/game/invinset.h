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

#ifndef SPYCRAFT_GAME_INV_INSET_H
#define SPYCRAFT_GAME_INV_INSET_H

#include "spycraft/game/made.h"
#include "spycraft/game/object.h"
#include "spycraft/game/list.h"
#include "spycraft/game/print.h"
#include "spycraft/game/actor.h"
#include "spycraft/game/feature.h"

namespace Spycraft {

class InvInsetView : public View
{
public:
	void init(int x_start, int y_start);
};

class InvInsetFeature : public TextFeature
{
public:
	~InvInsetFeature();
	int handleEvent(MADEEventStamp *event);
	void init(int theX, int theY, int theWidth, int theHeight);
	void setHotspot(int theVerb);
	void dispose();
};

class InvInsetSouthExit : public InvInsetFeature
{
public:
	InvInsetSouthExit();
	void respond(void);
	int doVerb(int);
};

class InvInsetEastExit : public InvInsetFeature
{
public:
	InvInsetEastExit();
	void respond(void);
	int doVerb(int);
};

class InvInsetWestExit : public InvInsetFeature
{
public:
	InvInsetWestExit();
	void respond(void);
	int doVerb(int);
};

class InvInset : public Object
{
public:
	int 			x;
	int 			y;
	int 			left;
	int 			top;
	int 			right;
	int 			bottom;
	int 			verb;
	int				oldFocus;
	InvInsetView *invInsetView;

	InvInset();
	~InvInset();

	void dispose(void);
	int handleEvent(MADEEventStamp *event);

	int doVerb(int);
	void init(int Id, int theX, int theY, int theWidth, int theHeight);
	int onMe(int theX, int theY);
	int onMe(MADEEventStamp *event);
	int onMe(Feature *obj);
	void respond(void);
};

class InvInsetHotspotList : public List
{
public:
	InvInsetHotspotList();

	void doit(void);
};

extern	InvInset *theInvInset;
extern	FeatureList *InvInsetFeatureList;
//extern scratchGlobal;

} // namespace Spycraft

#endif
