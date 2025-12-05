//Started by Ka Wah
//Bullpen - Bullpen from 25000
//Copyright Activision 1995
//Finished by Ka Wah

#ifndef SPYCRAFT_LOGIC_25100_H
#define SPYCRAFT_LOGIC_25100_H

#include "game.h"
#include "feature.h"
#include "actor.h"
#include "globals.h"
#include "inset.h"

class Rm25100 : public Room
{					
public:
	Rm25100();
	~Rm25100();
	void init ( void );
	int handleEvent ( MADEEventStamp * );
};

class WestExit25100 : public ExitFeature
{
public:
	WestExit25100();
  	void respond ( void );
	int doVerb ( int );
};

#endif

