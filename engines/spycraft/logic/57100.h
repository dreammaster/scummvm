//Started by Tom DeSalvo
//Birdsong's appartment, left view.   
//Copyright Activision 1995
//Finished by Ka Wah

#ifndef SPYCRAFT_LOGIC_57100_H
#define SPYCRAFT_LOGIC_57100_H

#include "made.h"
#include "game.h"
#include "feature.h"
#include "actor.h"
#include "inset.h"
#include "flag.h"


class Rm57100 : public Room
{
public:
	Rm57100();
	~Rm57100();
	void init();
	void cue();
	int handleEvent ( MADEEventStamp * );
};

class EastExit57100 : public ExitFeature
{
public:
	EastExit57100();
  	void respond( void );
	int doVerb( int );
};

/* features */
class Calendar57100 : public Feature
{
public:
	Calendar57100();
	void respond( void );
	int doVerb( int );
};

/* insets */
class CalendarInset57100 : public Inset		
{
public:
	CalendarInset57100();
	int doVerb( int );
};

class GunLange57100 : public View
{
public:
	GunLange57100();
  	void respond ( void );
	int doVerb ( int );
};

/* dialogs */
class LangeShootThorn : public DItem
{
public:
	LangeShootThorn();
	void doit( void );
};

class ThornShootLange : public DItem
{
public:
	ThornShootLange();
	void doit( void );
};

/* scripts */
class LangeEnter57100 : public Script
{
public:
	LangeEnter57100();
	void changeState( int newState );
};

/*
class Rm57101 : public Room
{
public:
	Rm57101();
	~Rm57101();
	void init();
	void cue();
	int handleEvent ( MADEEventStamp * );
};

class EnterBlackScreenScript57101 : public Script
{
public:
	EnterBlackScreenScript57101();
	void changeState( int newState );
};
*/

#endif
