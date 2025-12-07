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

#ifndef SPYCRAFT_GAME_GOGGLES_H
#define SPYCRAFT_GAME_GOGGLES_H

#include "spycraft/game/globals.h"
#include "spycraft/logic/7000.h"
#include "spycraft/game/actor.h"
#include "spycraft/game/made.h"
#include "spycraft/game/game.h"
#include "spycraft/game/feature.h"
#include "spycraft/game/script.h"
#include "spycraft/game/sound.h"
#include "spycraft/game/colby.h"
#include "spycraft/game/flag.h"
#include "spycraft/game/gun.h"

namespace Spycraft {

extern MADEPt coords[19];

extern WestCursor *westCursor;
extern NorthCursor *northCursor;
extern NortheastCursor *northeastCursor;
extern NorthwestCursor *northwestCursor;
extern SouthCursor *southCursor;
extern SoutheastCursor *southeastCursor;
extern SouthwestCursor *southwestCursor;
extern BlinkCursor *normalCursor;
extern GunCursor *gunCursor;
extern ArrowCursor *arrowCursor;
extern Intrface *intrface;
extern Colby colby;
extern int grendlPos;

extern Gun *theGun;
extern int gridnumber;
extern int base;
extern int Hp;
extern int E_map[9][5];
extern int T_list[19][15];
extern int P_map[9][5];
extern int *baddies;
extern int	addshooters;
extern int	secOnes;
extern int	secTens;
extern int	minOnes;
extern int	minTens;

#define NOGO	1000

#define GUN_NORTH_EXIT_X 	100 //64
#define GUN_NORTH_EXIT_Y	16

#define GUN_SOUTH_EXIT_X 	100//64
#define GUN_SOUTH_EXIT_Y	300

#define GUN_NORTHEAST_EXIT_X 	411
#define GUN_NORTHEAST_EXIT_Y	16

#define GUN_NORTHWEST_EXIT_X 	0
#define GUN_NORTHWEST_EXIT_Y	16

#define GUN_SOUTHEAST_EXIT_X 	411
#define GUN_SOUTHEAST_EXIT_Y	250

#define GUN_SOUTHWEST_EXIT_X 	0
#define GUN_SOUTHWEST_EXIT_Y	250

#define SHOOTER_ABOVE	 	1
#define SHOOTER_ABOVE_LEFT 	2
#define SHOOTER_ABOVE_RIGHT	3
#define SHOOTER_LEFT 		4
#define SHOOTER_RIGHT	 	5
#define SHOOTER_BELOW	 	6
#define SHOOTER_BELOW_LEFT 	7
#define SHOOTER_BELOW_RIGHT	8
#define TRY_NOTHING		0
#define TRY_UP		 	1
#define TRY_UP_LEFT 	2
#define TRY_UP_RIGHT	3
#define TRY_DOWN	 	4
#define TRY_DOWN_LEFT 	5
#define TRY_DOWN_RIGHT	6

class Elapsed : public Object
{
public:
	Elapsed();
};

class Patrol : public Object
{
public:
	int alive;
	int row[6];
	int column[6];
	Patrol(int pos1, int pos2, int pos3, int pos4, int pos5, int pos6);

};

class CursorTrap : public Feature
{
public:
	int sup;
	CursorTrap();
	void respond(void);
};

class Seperator : public Print
{
public:
	Seperator();
};
class SecOnes : public Print
{
public:
	char buff[3];
	SecOnes();
};
class SecTens : public Print
{
public:
	char buff[3];
	SecTens();
};
class MinOnes : public Print
{
public:
	char buff[3];
	MinOnes();
};
class MinTens : public Print
{
public:
	char buff[3];
	MinTens();
};

class ShootBack : public View {
protected:
	const char *name;
public:
	ShootBack(int backnum);
};

class ShootText : public View {
protected:
	const char *name;
public:
	ShootText();
};

class Grid : public View {
protected:
	const char *name;
public:
	Grid();
	int doVerb(int);
};

class Rings : public Prop {
protected:
	const char *name;
public:
	CursorTrap *trap;
	Rings();
	void cue(void);
	void doit(void);
};

class Arrow : public View {
protected:
	const char *name;
public:
	ExitFeature *exit;
	Arrow(int start_loop, int start_x, int start_y, ExitFeature *thisexit);
	void doit(void);
};

class me : public View {
protected:
	const char *name;
public:
	me();
};

class LeftHP : public View {
protected:
	const char *name;
public:
	LeftHP();
};
class RightHP : public View {
protected:
	const char *name;
public:
	RightHP();
};

class bea : public View {
protected:
	const char *name;
public:
	bea();
	bea(int theX, int theY);
};

class eneme : public View {
protected:
	const char *name;
public:
	eneme();
};

class Gunman : public Prop {
protected:
	const char *name;
public:
	int dead;
	Sound *scream;
	Sound *bang;
	Gunman(int theview, int thex, int they);
	//	int doVerb ( int );
	int handleEvent(MADEEventStamp *event);
	void dispose(void);
};

class GunmanScript : public Script
{
public:
	//	Gunman *gman;
	GunmanScript() {
		name = "gunmanscript";
	};
	void changeState(int);
};

class DieScript : public Script
{
public:
	//	Gunman *gman;
	DieScript() {
		name = "diescript";
	};
	void changeState(int);
};

class DieScript2 : public Script
{
public:
	//	Gunman *gman;
	DieScript2() {
		name = "diescript2";
	};
	void changeState(int);
};

class LilGunmanScript : public Script
{
public:
	//	Gunman *gman;
	LilGunmanScript() {
		name = "lilgunmanscript";
	};
	void changeState(int);
};

class LilDieScript : public Script
{
public:
	//	Gunman *gman;
	LilDieScript() {
		name = "lildiescript";
	};
	void changeState(int);
};
class ManagerScript : public Script
{
public:
	ManagerScript() {
		name = "managerscript";
	};
	void changeState(int);
};
class WaitingScript : public Script
{
public:
	WaitingScript() {
		name = "waitingscript";
	};
	void changeState(int);
};
class TimerScript : public Script
{
public:
	TimerScript() {
		name = "timerscript";
	};
	void changeState(int);
};

class PulseScript : public Script
{
public:
	PulseScript() {
		name = "pulsescript";
	};
	void changeState(int);
};
class COScript : public Script
{
public:
	COScript() {
		name = "coscript";
	};
	void changeState(int);
};
class WPStartScript : public Script
{
public:
	WPStartScript() {
		name = "wpscript";
	};
	void changeState(int);
};
class RefScript : public Script
{
public:
	RefScript() {
		name = "refscript";
	};
	void changeState(int);
};
class TunisiaScript : public Script
{
public:
	TunisiaScript() {
		name = "tunisiascript";
	};
	void changeState(int);
};
class EndShooterScript : public Script
{
public:
	EndShooterScript() {
		name = "endshooterscript";
	};
	void changeState(int);
};

void gogglesStart(int row, int column);
void moveEnemy(int row, int column);
void gogglesClean(void);

void clearShot(void);
void endPuzzle(void);

extern int g1;
extern int g2;
extern int g3;
extern int g4;
extern int h1;
extern int h2;
extern int f1;
extern int f2;
extern Elapsed *elapsed;
extern SecOnes *psecOnes;
extern SecTens *psecTens;
extern MinOnes *pminOnes;
extern MinTens *pminTens;
extern Seperator *seperator;
extern MADEPt enePos[19];

} // namespace Spycraft

#endif
