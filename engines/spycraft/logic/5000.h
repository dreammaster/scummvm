/*********************************************
* Header file.
* Image Analysis
* Rooms: 5000, 5100, 5200        
* Copyright Activision 1995
* Started by Tom DeSalvo 
* Continued by:  Daniel Kinzek   July, 1995
**********************************************/

#ifndef SPYCRAFT_LOGIC_5000_H
#define SPYCRAFT_LOGIC_5000_H

#include "made.h"
#include "game.h"
#include "feature.h"
#include "actor.h"
#include "globals.h"
#include "inset.h"

/* rooms */

class Rm5000 : public Room
{
public:
	Rm5000();
	void init();
};

class Rm5100 : public Room
{
public:
	Rm5100();
	void init();
	void playIA();
};

class Rm5200 : public Room
{
public:
	Rm5200();
	void init();
};

/* exit features */
/* 5000 */
class SouthExit5000 : public SouthExit
{
public:
	SouthExit5000();
  void respond( void );
	int doVerb( int );
};
class WestExit5000 : public ExitFeature
{
public:
	WestExit5000();
	void respond( void );
	int doVerb( int );
};

/* 5100 */
class SouthExit5100 : public SouthExit
{
public:
	SouthExit5100();
  void respond( void );
	int doVerb( int );
};

/* 5200 */
class SouthExit5200 : public SouthExit
{
public:
	SouthExit5200();
  void respond( void );
	int doVerb( int );
};
class EastExit5200 : public ExitFeature
{
public:
	EastExit5200();
	void respond( void );
	int doVerb( int );
};

/* features */
class Computer5000 : public Feature
{
 public:
	Computer5000();
	int	doVerb( int );
	void respond( void );
};

/* props */
class BackMonitorProp5000 : public Prop
{
public:
	BackMonitorProp5000();
	int	doVerb( int );
	void respond( void );
	void cue( void );
};

class BlankBackMonitorProp5000 : public Prop
{
 public:
 	BlankBackMonitorProp5000();
	int doVerb( int );
	void cue( void );
};

/* scripts */
class TimeoutScript5000: public Script
{
 public:
 	TimeoutScript5000() { name = "TimeoutScript5000"; };
	~TimeoutScript5000();
	void changeState( int );	
};

class IntroScript5100 : public Script
{
 public:
 	IntroScript5100() { name = "IntroScript5100"; };
	void changeState( int );
};

class NewsScript5000Plus : public Script
{
 public:
 	NewsScript5000Plus() { name = "NewsScript5000Plus"; };
	void changeState( int );
};

class LicenseScript5100 : public Script
{
 public:
 	LicenseScript5100() { name = "LicenseScript5100"; };
	void changeState( int );
};

class TanksScript5100 : public Script
{
 public:
 	TanksScript5100() { name = "TanksScript5100"; };
	void changeState( int );
};

class AmesScript5100 : public Script
{
 public:
 	AmesScript5100() { name = "AmesScript5100"; };
	void changeState( int );
};

	

#endif

