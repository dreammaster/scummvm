/*********************************************
* Header file.
* Alley near unsafe house.        
* Copyright Activision 1995
* Started by Daniel Kinzek   June, 1995
**********************************************/

#ifndef SPYCRAFT_LOGIC_32300_H
#define SPYCRAFT_LOGIC_32300_H

#include "made.h"
#include "game.h"
#include "feature.h"
#include "actor.h"
#include "inset.h"
#include "flag.h"

class Rm32300 : public Room
{
public:
	Rm32300();
	void init();
};

/* exit features */
class SouthExit32300 : public SouthExit
{
public:
	SouthExit32300();
  	void respond( void );
	int doVerb( int );
};

class NorthWestExit32300 : public ExitFeature
{
public:
	NorthWestExit32300();
  	void respond( void );
	int doVerb( int );
};

#endif
