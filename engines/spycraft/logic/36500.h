//Started by Ka Wah
//Foster's office, with solving the KAT and met Max
//Copyright Activision 1995
//Finished by Ka Wah

#ifndef SPYCRAFT_LOGIC_36500_H
#define SPYCRAFT_LOGIC_36500_H

#include "made.h"
#include "game.h"
#include "feature.h"
#include "actor.h"
#include "globals.h"
#include "inset.h"

class Rm36500 : public Room
{
public:
	Rm36500();
	~Rm36500();
	void cue();
	void init ( void );
	int handleEvent ( MADEEventStamp * );
};

class SouthExit36500 : public SouthExit
{
public:
	SouthExit36500();
  	void respond ( void );
	int doVerb ( int );
};
	  
/* scripts */
class EnterScript36500 : public Script
{
 public:
 	EnterScript36500(); 
	void changeState( int );
};

/* dialogs */
class DoneHaveHat3 : public DItem
{
public:
	DoneHaveHat3();
	void doit( void );
};

class NumberofYuri3 : public DItem
{
public:
	NumberofYuri3();
	void doit( void );
};

class AgainThanks3 : public DItem
{
public:
	AgainThanks3();
	void doit( void );
};

#endif			
