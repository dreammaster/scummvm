//Started by Tom DeSalvo
//Foster's desk drawer for lock pick puzzle.  
//Copyright Activision 1995
//Finished by Ka Wah

#ifndef SPYCRAFT_LOGIC_36200_H
#define SPYCRAFT_LOGIC_36200_H

#include "made.h"
#include "game.h"
#include "feature.h"
#include "actor.h"
#include "inset.h"
#include "flag.h"

class Rm36200 : public Room
{
public:
	Rm36200();
	~Rm36200();
	void init();
	int handleEvent ( MADEEventStamp * );
};

class SouthExit36200 : public ExitFeature
{
public:
	SouthExit36200();
  	void respond( void );
	int doVerb( int );
};

/* features */
class Lock36200 : public Feature
{
public:
	Lock36200();
	void respond( void );
	int doVerb( int );
};

/* scripts */
class LockPickScript36200 : public Script
{
 public:
 	LockPickScript36200(); 
	void changeState( int );
};

#endif
