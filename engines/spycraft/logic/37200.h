//Started by Ka Wah
//Kneecaps Pad - Master of room without Birdsong
//Copyright Activision 1995
//Finished by Ka Wah

#ifndef SPYCRAFT_LOGIC_37200_H
#define SPYCRAFT_LOGIC_37200_H

#include "game.h"
#include "feature.h"
#include "actor.h"
#include "globals.h"
#include "inset.h"

class Rm37200 : public Room
{					
public:
	Rm37200();
	~Rm37200();
	void init ( void );
	int handleEvent ( MADEEventStamp * );
};

class SouthExit37200 : public SouthExit
{
public:
	SouthExit37200();
  	void respond ( void );
	int doVerb ( int );
};

class WestExit37200 : public ExitFeature
{
public:
	WestExit37200();
  	void respond ( void );
	int doVerb ( int );
};

/* scripts */
class EnterStickScript37200 : public Script
{
 public:
 	EnterStickScript37200(); 
	void changeState( int );
};

class EnterCarrotScript37200 : public Script
{
 public:
 	EnterCarrotScript37200(); 
	void changeState( int );
};

/* Room 37201 */
class Rm37201 : public Room
{					
public:
	Rm37201();
	~Rm37201();
	void init ( void );
	int handleEvent ( MADEEventStamp * );
};

class EnterBlackScreenScript37201 : public Script
{
 public:
 	EnterBlackScreenScript37201(); 
	void changeState( int );
};

#endif

