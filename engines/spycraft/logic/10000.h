//Started by Tom DeSalvo
//Continued by Daniel Kinzek
//Colby House Exterior

#ifndef SPYCRAFT_LOGIC_10000_H
#define SPYCRAFT_LOGIC_10000_H

#include "made.h"
#include "game.h"
#include "feature.h"
#include "actor.h"
#include "globals.h"

class Rm10000 : public Room
{
public:
	Rm10000();
	void init ( void );
	//int handleEvent ( MADEEventStamp * );
};

class Rm10100 : public Room
{
public:
	Rm10100();
	void init ( void );
};

class Rm10200 : public Room
{
public:
	Rm10200();
	void init ( void );
};

/* exit features */
class SouthExit10000 : public ExitFeature
{
public:
	SouthExit10000();
  	void respond ( void );
	int doVerb ( int );
};

class SouthExit10100 : public ExitFeature
{
public:
	SouthExit10100();
  	void respond ( void );
	int doVerb ( int );
};

class SouthExit10200 : public ExitFeature
{
public:
	SouthExit10200();
  	void respond ( void );
	int doVerb ( int );
};

/* features */
class House10000 : public Feature
{
public:
	House10000();
	void respond ( void );
	int	doVerb ( int );
};

class Door10100 : public Feature
{
public:
	Door10100();
	void respond ( void );
	int	doVerb ( int );
};

class Knocker10200 : public Feature
{
public:
	Knocker10200();
	void respond ( void );
	int	doVerb ( int );
};

/* views */
class HouseView10100 : public View
{
public:
	HouseView10100();
};

class DoorView10200 : public View
{
public:
	DoorView10200();
};

/* scripts */
class KnockScript10200 : public Script
{
 public:
 	Sound*	soundptr;
 	KnockScript10200() { name = "KnockScript10200"; };
	void changeState( int );
};

#endif

