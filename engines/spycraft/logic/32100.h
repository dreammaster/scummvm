/*********************************************
* Header file.
* Alley near unsafe house.        
* Copyright Activision 1995
* Started by Daniel Kinzek   June, 1995
**********************************************/

#ifndef SPYCRAFT_LOGIC_32100_H
#define SPYCRAFT_LOGIC_32100_H

#include "made.h"
#include "game.h"
#include "feature.h"
#include "actor.h"
#include "inset.h"
#include "flag.h"

class Rm32100 : public Room
{
public:
	Rm32100();
	void init();
};

/* exit features */
class NorthExit32100 : public ExitFeature
{
public:
	NorthExit32100();
  void respond( void );
	int doVerb( int );
};

class SouthExit32100 : public ExitFeature
{
public:
	SouthExit32100();
  void respond( void );
	int doVerb( int );
};

#endif
