/*********************************************
* Header file.
* Blake's patio
* Rooms: 80000, 80100, 80200        
* Copyright Activision 1995
* Started by Tom DeSalvo 
* Continued by: Daniel Kinzek   July, 1995
**********************************************/

#ifndef SPYCRAFT_LOGIC_80000_H
#define SPYCRAFT_LOGIC_80000_H

#include "made.h"
#include "game.h"
#include "feature.h"
#include "actor.h"
#include "globals.h"
#include "inset.h"
#include "timer.h"
#include "sound.h"

/* rooms */
class Rm80000 : public Room
{
public:
	Rm80000();
	void init( void );
};

class Rm80100 : public Room
{
public:
	Rm80100();
	void init ( void );
};

class Rm80200 : public Room
{
public:
	Rm80200();
	void init ( void );
};

/* exit features */
class NorthExit80000 : public ExitFeature
{
 public:
 	NorthExit80000();
	void respond( void );
	int doVerb( int );
};

class SouthExit80000 : public SouthExit
{
public:
	SouthExit80000();
  	void respond ( void );
	int doVerb ( int );
};

class SouthExit80100 : public SouthExit
{
public:
	SouthExit80100();
  	void respond ( void );
	int doVerb ( int );
};

class SouthExit80200 : public SouthExit
{
public:
	SouthExit80200();
  	void respond ( void );
	int doVerb ( int );
};

/* features */
class Blake80000 : public Feature
{
public:
	Blake80000();
	int doVerb ( int );
};

class Table80000 : public Feature
{
public:
	Table80000();
  	void respond ( void );
	int doVerb ( int );
};

class Table80100 : public Feature
{
public:
	Table80100();
  	void respond ( void );
	int doVerb ( int );
};

class Parker_CD80100 : public Feature
{
public:
	Parker_CD80100();
  	void respond ( void );
	int doVerb ( int );
};

class Powerbook80200 : public Feature
{
public:
	Powerbook80200();
  	void respond ( void );
	int doVerb ( int );
};

class CDPlayer80200 : public Feature
{
public:
	CDPlayer80200();
  	void respond ( void );
	int doVerb ( int );
};

class Modem80200 : public Feature
{
public:
	Modem80200();
  	void respond ( void );
	int doVerb ( int );
};

class Hound80200 : public Feature
{
public:
	Hound80200();
  	void respond ( void );
	int doVerb ( int );
};

/* insets */
class PowerbookInset80200 : public Inset		
{
public:
	PowerbookInset80200();
	int doVerb( int);
	//void dispose( void );
};

class CDPlayerInset80200 : public Inset		
{
public:
	CDPlayerInset80200();
	int doVerb( int);
};

class ModemInset80200 : public Inset		
{
public:
	ModemInset80200();
	int doVerb( int);
};

class Parker_CDInset80100 : public Inset		
{
public:
	Parker_CDInset80100();
	int doVerb( int);
	//void dispose( void );
};

class HoundInset80200 : public Inset		
{
public:
	HoundInset80200();
	void respond();
	int doVerb( int);
	//void dispose( void );
};

/* scripts */
class IntroScript80000 : public Script
{
public:
	IntroScript80000() {name = "IntroScript80000";};
	void changeState ( int );
};

class BlakeDialogScript80000 : public Script
{
public:
	BlakeDialogScript80000() {name = "BlakeDialogScript80000";};
	void changeState ( int );
};

// global script
class BlakeReturnsScript80000 : public Script
{
 public:
 	int		pass;
	Sound*	soundptr;
 	BlakeReturnsScript80000() {name = "BlakeReturnsScript80000"; pass = 0;};
 	void changeState( int );	
};

// Dialogs
class BlakePatioDialog80000 : public Dialog
{
 public:
	void cue( void );
};

// DItems
class BlakePatioDItem80000 : public DItem
{
 public:
	virtual void doit( void );
};
// dialog doits
class WhatPrecaution_3 : public BlakePatioDItem80000
{
public:
  void doit();
};

class WhatPrecaution_6 : public BlakePatioDItem80000
{
public:
  void doit();
};

class YouKillM_3_5 : public BlakePatioDItem80000
{
public:
  void doit();
};

class YouKillM_4 : public BlakePatioDItem80000
{
public:
  void doit();
};

// patio
class Patio80000 : public Object
{
 public:
 	int	objectTouchCount;
	int	objectTouched;	// one of the enums below, obj we're looking at
	Patio80000();
	~Patio80000();
	void touchObject( int whichObject );
};

// misc
class WaitingTimer80000 : public Timer
{
 public:
	~WaitingTimer80000();	// take it off the permList
};

class AnyElseObj80000 : public Object
{
 public:
 	void cue( void );
};

enum{ COMPUTER80000, CD80000, BOOK80000, NO_OBJECT80000 };


#endif

