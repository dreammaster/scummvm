/*********************************************
* Header file.
* Farm exterior establishing
* Rooms: 1010        
* Copyright Activision 1995
* Started by Tom DeSalvo 
* Continued by:  Daniel Kinzek   August, 1995
**********************************************/

#ifndef SPYCRAFT_LOGIC_1010_H
#define SPYCRAFT_LOGIC_1010_H

#include "made.h"
#include "game.h"
#include "feature.h"
#include "actor.h"
#include "globals.h"

class Rm1010 : public Room
{
public:
	Rm1010();
	void init ( void );
};

/* features */
class Building1010 : public Feature
{
public:
	Building1010();
	void respond( void );
	int doVerb( int );
};

/* scripts */
class EstabScript1010: public Script
{
 public:
 	EstabScript1010() { name = "EstabScript1010"; };
 	void changeState( int );
};

#endif

