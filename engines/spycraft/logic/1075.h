//Started by Tom DeSalvo
//Oil Refinery
//Copyright Activision 1995
//Finished by Ka Wah

#ifndef SPYCRAFT_LOGIC_1075_H
#define SPYCRAFT_LOGIC_1075_H

#include "made.h"
#include "game.h"
#include "feature.h"
#include "actor.h"
#include "globals.h"

class Rm1075 : public Room
{
public:
	Rm1075();
	~Rm1075();
	void cue ( void );
	void init ( void );
	int handleEvent ( MADEEventStamp * );
};
/*
class NorthExit1075 : public ExitFeature
{
public:
	NorthExit1075();
  	void respond ( void );
	int doVerb ( int );
};
*/

/* scripts */
class EnterScript1075 : public Script
{
 public:
 	EnterScript1075(); 
	void changeState( int );
};

class PitSaleGood : public DItem
{
public:
	PitSaleGood();
	void doit( void );
};

#endif
