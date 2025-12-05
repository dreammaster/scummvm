//Started by Kawah Wong
//Yuri's room without gift box
//Copyright Activision 1995
//Finished by Ka Wah

#ifndef SPYCRAFT_LOGIC_48400_H
#define SPYCRAFT_LOGIC_48400_H

#include "game.h"
#include "feature.h"
#include "actor.h"
#include "globals.h"
#include "inset.h"

class Rm48400 : public Room
{					
public:
	Rm48400();
	~Rm48400();
	void init ( void );
	int handleEvent ( MADEEventStamp * );
};

class SouthExit48400 : public SouthExit
{
public:
	SouthExit48400();
  	void respond ( void );
	int doVerb ( int );
};

#endif

