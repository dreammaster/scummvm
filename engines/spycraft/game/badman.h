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

#ifndef SPYCRAFT_GAME_BADMAN_H
#define SPYCRAFT_GAME_BADMAN_H

#include "spycraft/game/made.h"
#include "spycraft/game/game.h"
#include "spycraft/game/feature.h"
#include "spycraft/game/script.h"
#include "spycraft/game/actor.h"
#include "spycraft/game/sound.h"
#include "spycraft/game/colby.h"
#include "spycraft/game/interface_puzzle.h"
#include "spycraft/game/web.h"

namespace Spycraft {

#define DOOR	10
#define HOLE	11
#define NONE	12

#define DORMANT		20
#define WAITING		21
#define WALKING		22
#define FIGHTING	23

#define WALK		24
#define SPECIAL		25

#define G1			100
#define G2			101
#define G3			102

#define YEL1		110
#define YEL2		111
#define YEL3		112

#define R1			120
#define R2			121
#define R3			122

#define DED			123

#define CIV			124
#define BIRD		125

#define LOFT		126
#define DOG			127

#define MUSIC		128
#define SALES		129
#define FUCK		130
#define FAM 		131
#define PARTY 		132
#define CRAZY 		133

#define BEEN		140
#define HALL		141

#define PHONES		150
#define BOILER		151

#define SUCCESS		999

#define BAD_NORTH   100
#define BAD_EAST	101
#define BAD_SOUTH   102
#define BAD_WEST	103


#define BAD_VERT	1
#define BAD_HORZ	2


class BadMan;
class Us;

struct FloorMap
{
	int   n;
	int   e;
	int   s;
	int   w;

	int *n_wall;
	int *e_wall;
	int *s_wall;
	int *w_wall;
};

class BadRoom : public IntrPuzzFeature
{
public:
	int floor_num;
	int room_num;
	int *n_wall;
	int *e_wall;
	int *s_wall;
	int *w_wall;
	int n_room;
	int e_room;
	int s_room;
	int w_room;
	int mid_x;
	int mid_y;
	int parent;
	int special;
	int combatants;

	void init(int theX, int theY, int theWidth, int theHeight, int *north, int *east, int *south, int *west,
		int n, int e, int s, int w, int spec, int num, int floor, int group);
	void drawHole(int hole_x, int hole_y);
	void check4Hole();
	int handleEvent(MADEEventStamp *event);
	void activate(void);
	int checkpath(Us *guy);
	int combat(int *enemy, Us *guy);
};

class Floor : public IntrPuzzPage
{
public:
	BadRoom *room[22];
	int *fwalls[40];
	int *fenemy[40];
};

class Base : public Floor
{
public:
	Base();
};

class Floor1 : public Floor
{
public:
	Floor1();
};

class Floor2 : public Floor
{
public:
	Floor2();
};

class EKG : public IntrPuzzProp {
protected:
public:

	Us *guy;
	EKG();
	void init(int x, int y, int pic_id, int loop_id, int cel_id);
	void cue(void);
	void dispose();
};

class Movement : public IntrPuzzProp {
protected:
public:

	void cue(void);
};
class Beacon : public Object {
protected:
	const char *name;
public:
	Us *guy;
	Beacon(Us *guy);
	void cue(void);
};
class 	Card : public WebView {
protected:
	const char *name;
public:
	Us *guy;
	Card();
	void cue(void);
};
class 	Selected : public WebView {
protected:
	const char *name;
public:
	Us *guy;
	void doit(void);
};
class 	StatButt : public WebView {
protected:
	const char *name;
public:
	Us *guy;
	StatButt();
	void cue(void);
};
class 	SpecButt : public WebButton {
protected:
	const char *name;
public:
	Us *guy;
	SpecButt();
	void activate(void);
};

class FloorBack : public IntrPuzzView {
protected:
	const char *name;
public:
	FloorBack(int num);
};

class Tactical : public WebButton {
protected:
public:
	int oakalydokaly;
	Tactical();
	void activate(void);
};

class BaseTab : public WebButton {
protected:
public:
	BaseTab();
	void activate(void);
};

class FirstTab : public WebButton {
protected:
public:
	FirstTab();
	void activate(void);
};

class SecondTab : public WebButton {
protected:
public:
	SecondTab();
	void activate(void);
};

class Hole : public IntrPuzzView {
protected:
public:
};

class walkScript : public Script
{
public:
	Us *thisguy;
	walkScript(Us *guy);
	void changeState(int);
};
class SpeechScript : public Script
{
public:
	SpeechScript() {
		name = "speechscript";
	};
	void changeState(int);
};

class flatScript : public Script
{
public:
	flatScript() {
		name = "flatscript";
	};
	void changeState(int);
};

class SuccessScript : public Script
{
public:
	SuccessScript() {
		name = "successscript";
	};
	void changeState(int);
};

class BadScript : public Script
{
public:
	BadScript() {
		name = "badscript";
	};
	void changeState(int);
};

class CopScript : public Script
{
public:
	CopScript() {
		name = "copscript";
	};
	void changeState(int);
};

class FlashScript : public Script
{
public:
	FlashScript() {
		name = "flashscript";
	};
	void changeState(int);
};

class WalkupScript : public Script
{
public:
	WalkupScript() {
		name = "walkupscript";
	};
	void changeState(int);
};

class CardFlashScript : public Script
{
public:
	CardFlashScript() {
		name = "cardflashscript";
	};
	void changeState(int);
};

class journeyScript : public Script
{
public:
	Us *thisguy;
	journeyScript(Us *guy);
	void changeState(int);
};

class Us : public IntrPuzzActor {
protected:
	const char *name;
public:

	int 		first;
	int 		dir;
	int			id;
	int			finaltarget;
	int			interimtarget;
	int 		floor;
	int 		room;
	int 		targetfloor;
	int 		state;
	int 		command;
	int			target;
	int 		destX;
	int 		destY;
	int 		halfdestX;
	int 		halfdestY;
	int 		room_special;
	int 		hitpoints;
	int 		bombing;
	int 		fighting;
	int 		talking;
	int 		talkingaboutphones;
	int 		alive;
	int			here;
	int 		holding;
	int			path[100];
	int			log[3][22];
	int *boomwall;
	EKG *ekg;
	StatButt *butt;
	SpecButt *spec;
	Card *card;
	BadRoom *boomroom;
	MADEPt		coords;
	Beacon *beacon;
	Selected *selected;

	void init(int x, int y, int pic_id, int loop_id, int cel_id);
	int handleEvent(MADEEventStamp *event);
	virtual void activate();
	virtual void special();
	void dispose();
	void startWalk(void);
	void startJourney(void);
	void cue(int dummy);
	void cue(void);
	int talk(int number);
	virtual void die(void);
	virtual int checkpath(void);
	void reset(void);
	int buildpath(int dir);
	virtual void announce(int what);
	void updateJourney(void);
	void secondJourney(void);
	void hide(void);
	void show(void);
	virtual void fight(void);
};

class Henderson : public Us {
protected:
	const char *name;
public:
	Henderson();
	void die(void);
	void special();
	void announce(int what);
	int checkpath(void);
	void fight(void);
};

class Washington : public Us {
protected:
	const char *name;
public:
	int 		grenades;
	Washington();
	void die(void);
	void announce(int what);
	void special();
	void fight(void);
};

class Aguila : public Us {
protected:
	const char *name;
public:
	Aguila();
	void die(void);
	void special();
	void announce(int what);
	void fight(void);
};

class Schiff : public Us {
protected:
	const char *name;
public:
	int 		bombs;
	Schiff();
	void die(void);
	void special();
	void announce(int what);
	int *check4wall(void);
	void fight(void);
};

class BadMan : public IntrPuzz
{
public:

	int 			walls[100];
	int 			enemy[100];
	int 			curFloorNum;
	int 			hideGuys;
	int 			checked;
	int 			done;
	int				casualties;
	int 			death;
	int 			min;
	int				coppers;
	int 			phones;
	int 			scanned;
	int 			oops;
	int 			whoops;
	int 			micOpen;
	int 			intro;
	int				somebodyistalkingaboutphones;
	Henderson *hend;
	Washington *wash;
	Aguila *ag;
	Schiff *schiff;
	Us *guy[4];
	Floor *Floors[3];
	Tactical *tact;
	Sound *speech;
	Sound *ambient;
	FloorBack *back;
	Movement *strikers[22];
	BadMan();
	~BadMan();
	void init(int pic_num);
	void initWalls(void);
	void doit(void);
	void initEnemy(void);
	int handleEvent(MADEEventStamp *);
	void cue(void);
	void cue(int dummy);
};

} // namespace Spycraft

#endif

