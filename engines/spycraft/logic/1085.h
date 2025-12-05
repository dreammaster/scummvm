/*********************************************
* Header file.
* Dacha establishing.
* Rooms: 1085        
* Copyright Activision 1995
* Started by Tom DeSalvo 
* Continued by:  Daniel Kinzek   August, 1995
**********************************************/

#ifndef SPYCRAFT_LOGIC_1085_H
#define SPYCRAFT_LOGIC_1085_H

#include "made.h"
#include "game.h"
#include "feature.h"
#include "actor.h"
#include "globals.h"

class Rm1085 : public Room
{
public:
	Rm1085();
	void init ( void );
};

class NorthExit1085 : public ExitFeature
{
public:
	NorthExit1085();
  	void respond ( void );
	int doVerb ( int );
};

#endif

