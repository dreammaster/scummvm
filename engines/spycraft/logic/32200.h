/*********************************************
* Header file.
* Alley near unsafe house.        
* Copyright Activision 1995
* Started by Daniel Kinzek   June, 1995
**********************************************/

#ifndef SPYCRAFT_LOGIC_32200_H
#define SPYCRAFT_LOGIC_32200_H

#include "spycraft/game/made.h"
#include "spycraft/game/game.h"
#include "spycraft/game/feature.h"
#include "spycraft/game/actor.h"
#include "spycraft/game/inset.h"
#include "spycraft/game/flag.h"

class Rm32200 : public Room
{
public:
	Rm32200();
	void init();
};

/* exit features */
class NorthExit32200 : public ExitFeature
{
public:
	NorthExit32200();
  void respond( void );
	int doVerb( int );
};

class SouthExit32200 : public ExitFeature
{
public:
	SouthExit32200();
  void respond( void );
	int doVerb( int );
};

} // namespace Spycraft

#endif
