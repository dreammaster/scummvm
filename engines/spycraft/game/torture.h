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

#ifndef SPYCRAFT_GAME_TORTURE_H
#define SPYCRAFT_GAME_TORTURE_H

#include "spycraft/game/interface_puzzle.h"
#include "spycraft/game/movie.h"
#include "spycraft/dmade/mcimovie.h"
#include "spycraft/game/dialog.h"

namespace Spycraft {

//extern SouthCursor* southCursor;
extern BlinkCursor *normalCursor;

#define HANDLETOP			400
#define HANDLEBOTTOM		461

#define HANDLE_NUMFRAMES	12 
typedef struct
{
	int inFrame;
	int outFrame;
	int loopFrame;
}MovieData;

typedef struct
{
	int number;
	int maxMovies;
}Sequence;

/*class SouthExitYing : public IntrPuzzFeature
{
public:
	SouthExitYing();
	int handleEvent (MADEEventStamp* event);
};
*/
class QuestionBox : public IntrPuzzView
{
public:
	QuestionBox();
};

class Question;

class ShockSwitch : public Button
{
public:
	Question *thisQ;
	ShockSwitch(int theX, int theY);
	int handleEvent(MADEEventStamp *event);
	virtual void activate();
};

class ExitTorture : public Button
{
public:
	ExitTorture();
	virtual void activate();
};

class Question : public IntrPuzzTextFeature
{
public:
	unsigned int thresh;
	bool hilit;
	ShockSwitch *shockSwitch;

	void init(int theX, int theY, int width, int height);
	int handleEvent(MADEEventStamp *event);
};

class DItemLine : public DItem
{
public:
	int yingBaked();
};

class DItemLine3 : public DItemLine
{
public:
	void doit();
};

class DItemLine1 : public DItemLine
{
public:
	void doit();
};

class DItemLine2 : public DItemLine
{
public:
	void doit();
};

class Q1 : public Question
{
public:
	QuestionBox *questionbox;

	Q1();
};

class Q2 : public Question
{
public:
	Q2();
};

class Q3 : public Question
{
public:
	Q3();
};

class Shock : public IntrPuzzView
{
public:
	int buttonDown;
	int increment;
	unsigned long prevtime;
	unsigned long time;

	Shock();
	int handleEvent(MADEEventStamp *event);
	void activate();
	int getLevel();
};

class ShockIncrease : public Shock
{
public:
	ShockIncrease();
};

class ShockDecrease : public Shock
{
public:
	ShockDecrease();
};

class ShockMeter : public IntrPuzzView
{
public:
	ShockMeter();
};

class ControlPanel : public IntrPuzzView
{
public:
	ControlPanel();
};

class HeartMonitor : public IntrPuzzProp
{
public:

	HeartMonitor();
	void cue(void);
};

class Meter : public IntrPuzzView
{
public:
	Meter();
};

class YingHeart : public Object
{
public:
	IntrPuzzTimer *HeartRateTimer;
	unsigned int deathctr;
	unsigned int breakctr;
	unsigned int dropctr;

	YingHeart();
	void cue();
};

class FryHandle : public Slider
{
public:
	bool gotHandle;
	IntrPuzzTimer *handleDrop;
	IntrPuzzTimer *HeartRateTimer;
	unsigned int dropctr;
	unsigned int deathctr;
	unsigned int breakctr;

	FryHandle();
	int handleEvent(MADEEventStamp *event);
	void cue(void);
};

class HeartInfo : public Print
{
public:
	HeartInfo(char *);
};

class Torture : public IntrPuzz
{
public:
	unsigned int HeartRate;
	unsigned int rateIncrease;
	bool zapping;
	bool killYing;
	bool yingDead;
	bool yingBroke;
	bool yingQuestion;
	bool gotVoltage;

	Q1 *q1;
	Q2 *q2;
	Q3 *q3;

	Torture();
	~Torture();
	int handleEvent(MADEEventStamp *);
	int specialMovieCase(MovieData *movieFrames);
	void setRate();
	void cue(void);
};

class SwitchActivateScript : public Script
{
public:
	SwitchActivateScript() {
		name = "SwitchActivateScript";
	};
	void changeState(int);
};

class ScriptBeginTorture : public Script
{
public:
	ScriptBeginTorture() {
		name = "BeginTorture";
	};
	void changeState(int);
};

class ScriptBeginQuestion : public Script
{
public:
	ScriptBeginQuestion() {
		name = "BeginQuestion";
	};
	void changeState(int);
};

} // namespace Spycraft

#endif
