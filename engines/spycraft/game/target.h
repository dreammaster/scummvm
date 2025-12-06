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

#ifndef SPYCRAFT_GAME_TARGET_H
#define SPYCRAFT_GAME_TARGET_H

#include "spycraft/game/interface_puzzle.h"
#include "spycraft/dmade/mcimovie.h"
#include "spycraft/game/movie.h"
#include "spycraft/game/web.h"

namespace Spycraft {

class FreqScript : public Script
{
public:
	FreqScript() {
		name = "freqhscript";
	};
	void changeState(int);
};
class NahBlinkScript : public Script
{
public:
	NahBlinkScript() {
		name = "nahblinkscript";
	};
	void changeState(int);
};
class MeScript : public Script
{
public:
	MeScript() {
		name = "mescript";
	};
	void changeState(int);
};
class KillScript : public Script
{
public:
	KillScript() {
		name = "killhscript";
	};
	void changeState(int);
};
class LiveScript : public Script
{
public:
	LiveScript() {
		name = "livehscript";
	};
	void changeState(int);
};
class MobScript : public Script
{
public:
	MobScript() {
		name = "mobscript";
	};
	void changeState(int);
};
class TargetHScript : public Script
{
public:
	TargetHScript() {
		name = "targethscript";
	};
	void changeState(int);
};
class HarmScript : public Script
{
public:
	HarmScript() {
		name = "harmscript";
	};
	void changeState(int);
};
class TargetAudioScript : public Script
{
public:
	TargetAudioScript() {
		name = "targetaudioscript";
	};
	void changeState(int);
};
class TargetXtraScript : public Script
{
public:
	TargetXtraScript() {
		name = "targetxtrascript";
	};
	void changeState(int);
};
class Freqs : public IntrPuzzProp {
protected:
public:
	Freqs(int theX, int theY, int loop_id, int cel_id);
};
class Beat : public Script
{
public:
	Beat() {
		name = "Beat";
	};
	void changeState(int);
};

class Kill : public Button {
protected:
	char *name;
public:
	Kill();
	void dispose(void);
	void activate(void);
};
class Nah : public Button {
protected:
	char *name;
public:
	Nah();
	void dispose(void);
	void activate(void);
};

class Guard : public IntrPuzzActor {
public:
	char *name;
	int posts[5][2];
	int curPos;
	void init(void);
};

class Volpe : public Guard {
public:
	Volpe();
};
class Scharf : public Guard {
public:
	Scharf();
};
class Hwang : public Guard {
public:
	Hwang();
};
class Grenald : public Guard {
public:
	Grenald();
};
class Ehlko : public Guard {
public:
	Ehlko();
};
class Dalzel : public Guard {
public:
	Dalzel();
};

class Me : public IntrPuzzActor {
protected:
public:
	char *name;
	int 		destX;
	int 		destY;
	int 		tempdestX;
	int 		tempdestY;
	int 		diverted;
	Me();
	void activate(void);
	void doit(void);
	void cue(void);
};

class RedSquare : public IntrPuzzView
{
public:
	RedSquare();
};

class Live : public IntrPuzzView
{
public:
	Live();
};

class TabFeature : public IntrPuzzFeature
{
public:
	int handleEvent(MADEEventStamp *);
	virtual void activate(void) {
	};
};

class FreqTab : public TabFeature
{
public:
	FreqTab();
	void activate(void);
};

class GPSTab : public TabFeature
{
public:
	GPSTab();
	void activate(void);
};
class Target : public IntrPuzzPage
{
public:
	Kill *kill;
	Nah *nah;
	int GPS;
	int GPSClicked;
	int Freq;
	int FreqClicked;
	int curframe;
	int out;
	int harm;
	int end;
	QuitAlert *quito;
	Me *me;
	Volpe *volpe;
	Scharf *scharf;
	Hwang *hwang;
	Grenald *grenald;
	Ehlko *ehlko;
	Dalzel *dalzel;
	Live *live;
	Freqs *freqs[12];
	Freqs *red[4];

	Target();
	void doit(void);
	void leave(void);
	void returned(void);
	int handleEvent(MADEEventStamp *event);
	void showbuttons(void);
	void hidebuttons(void);
	void dispose(void);

};
extern Target *target;

} // namespace Spycraft

#endif
