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

#ifndef SPYCRAFT_LOGIC_2000_H
#define SPYCRAFT_LOGIC_2000_H

#include "made.h"
#include "game.h"
#include "feature.h"
#include "actor.h"
#include "globals.h"
#include "inset.h"
#include "plistobj.h"

namespace Spycraft {

/* rooms */
class Rm2001 : public Room
{
public:
	Rm2001();
	void init();
};

class Rm2000 : public Room
{
public:
	Rm2000();
	~Rm2000();
	void init(void);
	void furnishRoom(void);
};

class Rm2100 : public Room
{
public:
	Rm2100();
	void init();
};

/* exit features */
class SouthExit2000 : public ExitFeature
{
public:
	SouthExit2000();
	void respond(void);
	int doVerb(int);
};

class SouthExit2100 : public ExitFeature
{
public:
	SouthExit2100();
	void respond(void);
	int doVerb(int);
};

class SouthExit2300 : public ExitFeature
{
public:
	SouthExit2300();
	void respond(void);
	int doVerb(int);
};

/* features */
// 2000
class Desk2000 : public Feature
{
public:
	Desk2000();
	void respond(void);
	int doVerb(int);
};

class Door2000 : public Feature
{
public:
	Door2000();
	void respond(void);
	int doVerb(int);
};

class Target2000 : public Feature
{
public:
	Target2000();
	void respond(void);
	int doVerb(int);
};

class GunRack2000 : public Feature
{
public:
	GunRack2000();
	void respond(void);
	int doVerb(int);
};

// 2100

//class LeftDesk2100 : public Feature
//{
//public:
//	LeftDesk2100();
//	void respond( void );
//	int doVerb( int );
//};

class MidDesk2100 : public Feature
{
public:
	MidDesk2100();
	void respond(void);
	int doVerb(int);
};

class RightDesk2100 : public Feature
{
public:
	RightDesk2100();
	void respond(void);
	int doVerb(int);
};

class PictureColby2200 : public Feature
{
public:
	PictureColby2200();
	void respond(void);
	int doVerb(int);
};

class PictureHarmonica2200 : public Feature
{
public:
	PictureHarmonica2200();
	void respond(void);
	int doVerb(int);
};

/* inset features */
class PhoneButton2100 : public InsetFeature
{
public:
	PhoneButton2100();
	void respond(void);
	int doVerb(int);
};

class Button1_2100 : public InsetFeature
{
public:
	Button1_2100();
	void respond(void);
	int doVerb(int);
};

class CursorArrowizer2100 : public InsetFeature
{
public:
	CursorArrowizer2100();
};

/* inset views */
class LEDInsetView2100 : public InsetView
{
public:
	LEDInsetView2100();
};

class BigButtonInsetView2100 : public InsetView
{
public:
	BigButtonInsetView2100();
};

class Button1InsetView2100 : public InsetView
{
public:
	Button1InsetView2100();
};

/* insets */
class TargetInset2000 : public Inset
{
public:
	TargetInset2000();
};

class GunRackInset2000 : public Inset
{
public:
	GunRackInset2000();
};

class MidDeskInset2100 : public Inset
{
public:
	MidDeskInset2100();
	int doVerb(int);
};

class OpenManualInset2100 : public Inset
{
public:
	OpenManualInset2100();
	int doVerb(int);
};

class RightDeskInset2100 : public Inset
{
public:
	RightDeskInset2100();
	void dispose();
};

class PictureInset2200 : public Inset
{
public:
	int walkingSoundPlaying; // flag
	PictureInset2200();
};

class PictureColbyInset2200 : public PictureInset2200
{
public:
	PictureColbyInset2200();
};

class PictureHarmonicaInset2200 : public PictureInset2200
{
public:
	PictureHarmonicaInset2200();
};

/* scripts */
class TravelInScript2001 : public Script
{
public:
	TravelInScript2001() {
		name = "TravelInScript2001";
	};
	void changeState(int);
};

class EnterScript2000 : public Script
{
public:
	EnterScript2000() {
		name = "EnterScript2000";
	};
	void changeState(int);
};

class LEDBlinkScript2100 : public Script
{
public:
	LEDBlinkScript2100() {
		name = "LEDBlinkScript2100";
	};
	void changeState(int);
};

class BigButtonPressScript2100 : public Script
{
public:
	BigButtonPressScript2100() {
		name = "BigButtonPressScript2100";
	};
	//~BigButtonPressScript2100();
	void changeState(int);
};

class Button1PressScript2100 : public Script
{
public:
	Button1PressScript2100() {
		name = "Button1PressScript2100";
	};
	//~Button1PressScript2100();
	void changeState(int);
};

class PhoneMessagesScript2100 : public Script
{
public:
	PhoneMessagesScript2100() {
		name = "PhoneMessagesScript2100";
	};
	void changeState(int);
};

class PhoneWomanScript2100 : public Script
{
public:
	PhoneWomanScript2100() {
		name = "PhoneWomanScript2100";
	};
	void changeState(int);
};

class AdamsPhoneScript2100 : public Script
{
public:
	AdamsPhoneScript2100() {
		name = "AdamsPhoneScript2100";
	};
	void changeState(int);
};

class FrankSadScript2000 : public Script
{
public:
	FrankSadScript2000() {
		name = "FrankSadScript2000";
	};
	void changeState(int);
};

class FrankComingScript2200 : public Script
{
public:
	FrankComingScript2200() {
		name = "FrankComingScript2200";
	};
	void changeState(int);
};

class ColbyPictScript2200 : public Script
{
public:
	ColbyPictScript2200() {
		name = "ColbyPictScript2200";
	};
	void changeState(int);
};

class HarmonicaPictScript2200 : public Script
{
public:
	HarmonicaPictScript2200() {
		name = "HarmonicaPictScript2200";
	};
	void changeState(int);
};

class FrankIntroScript2200 : public Script
{
public:
	FrankIntroScript2200() {
		name = "FrankIntroScript2200";
	};
	void changeState(int);
};

/* misc */
class FrankTimer2000 : public Script
{
public:
	FrankTimer2000();
	void changeState(int);
	int	firstTime;	// flag
};

class Blinker2000 : public PermListObject
{
public:
	void init(void);
	void dispose(void);
};

class HurryUp2000 : public Object
{
public:
	HurryUp2000() {
		name = "HurryUp2000";
	};
	void cue(void);
};

} // namespace Spycraft

#endif
