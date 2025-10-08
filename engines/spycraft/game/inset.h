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

#ifndef SPYCRAFT_INSET_H
#define SPYCRAFT_INSET_H

#include "spycraft/game/made.h"
#include "spycraft/game/object.h"
#include "spycraft/game/list.h"
#include "spycraft/game/print.h"
#include "spycraft/game/actor.h"
#include "spycraft/game/feature.h"

namespace Spycraft {

class InsetView : public View {
public:
	void init(int x_start, int y_start);
};

class InsetFeature : public TextFeature {
public:
	~InsetFeature();
	int handleEvent(MADEEventStamp *event);
	void init(int theX, int theY, int theWidth, int theHeight);
	void setHotspot(int theVerb);
	void dispose();
};

class InsetSouthExit : public InsetFeature {
public:
	InsetSouthExit();
	void respond(void);
	int doVerb(int);
};

class InsetEastExit : public InsetFeature {
public:
	InsetEastExit();
	void respond(void);
	int doVerb(int);
};

class InsetWestExit : public InsetFeature {
public:
	InsetWestExit();
	void respond(void);
	int doVerb(int);
};

class Inset : public Object {
public:
	int 			x;
	int 			y;
	int 			left;
	int 			top;
	int 			right;
	int 			bottom;
	int 			verb;
	Hotspot *hotspot;
	InsetView *insetView;
	Object *whoToCue;

	Inset();
	~Inset();

	void dispose(void);
	int handleEvent(MADEEventStamp *event);

	virtual int doVerb(int);
	virtual void init(void) {
	};
	virtual void init(int, int) {
	};
	//virtual void init ( int picId);
	virtual void init(int Id, int theX, int theY, int theWidth, int theHeight);
	virtual void init(int Id, int theX, int theY, int theWidth, int theHeight, Object *cue);
	virtual int onMe(int theX, int theY);
	virtual int onMe(MADEEventStamp *event);
	virtual int onMe(Feature *obj);
	//	virtual void setHotspot ( int theVerb );
	virtual void respond(void) {
	};
};

class InsetHotspotList : public List {
public:
	InsetHotspotList();

	void doit(void);
};

extern	Inset *theInset;
extern	FeatureList *insetFeatureList;

} // namespace Spycraft

#endif
