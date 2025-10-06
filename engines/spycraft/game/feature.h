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

#ifndef SPYCRAFT_GAME_FEATURE_H
#define SPYCRAFT_GAME_FEATURE_H

#include "spycraft/game/made.h"
#include "spycraft/game/object.h"
#include "spycraft/game/list.h"
#include "spycraft/game/print.h"

namespace Spycraft {

#define MAX_RESPONSE_VERB		32

class Feature;
class View;
class Help;

class Hotspot : public Object {
private:
	short respond_size = 0;
	unsigned short respond_verbs[MAX_RESPONSE_VERB] = {};

public:
	Feature *client = nullptr;

	Hotspot();
	~Hotspot();

	void dispose(void);
	void addRespondVerb(int theVerb);
	int checkRespondVerb(void);
	int checkRespondVerb(int theVerb);
};

class Feature : public Object {
public:
	int x = 0;
	int y = 0;
	int left = 0;
	int top = 0;
	int right = 0;
	int bottom = 0;
	int verb = 0;
	Hotspot *hotspot = nullptr;
	Help *myHelp = nullptr;

	Feature();
	~Feature();

	void dispose(void);
	int handleEvent(MADEEventStamp *event);
	void setHelp(char *theText);
	void doit();
	virtual int doVerb(int);
	virtual void init(void) {
	};
	virtual void init(int, int) {
	};
	virtual void init(int theX, int theY, int theWidth, int theHeight);
	virtual int onMe(int theX, int theY);
	virtual int onMe(MADEEventStamp *event);
	virtual int onMe(Feature *obj);
	virtual void setHotspot(int theVerb);
	virtual void respond(void);
	virtual void aniCursor(void);
};

//TEMP FOR NAV SHELL
class TextFeature : public Feature {
public:
	Print *myPrint = nullptr;
	TextFeature();
	void init(int theX, int theY, int theWidth, int theHeight);
	void dispose();
};

class ExitFeature : public Feature {
public:
	ExitFeature();
	void aniCursor(void);
};

class SouthExit : public Feature {
public:
	unsigned short myDelay = 0;
	unsigned long time0 = 0;
	int active = 0;

	SouthExit();
	int onMe(int theX, int theY);
};

class FeatureList : public List {
public:
	FeatureList();

	int handleEvent(MADEEventStamp *);
	void updateRects(int x_offset, int y_offset);
};

class HotspotList : public List {
public:
	HotspotList();

	void doit(void);
};

class Help : public Object {
public:
	char *myText = nullptr;
	unsigned int myTime = 0;
	unsigned int waitTime = 0;
	int active = 0;
	Feature *client = nullptr;
	View *myView = nullptr;

	Help(char *, Feature *);

	void doit(void);
};

extern FeatureList *features;
extern HotspotList *hotspots;
extern Hotspot *hotObj;

} // namespace Spycraft

#endif

