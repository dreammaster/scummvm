//Started by Tom DeSalvo
//Continued by Daniel Kinzek
//Crisis staging area

#ifndef SPYCRAFT_LOGIC_7000_H
#define SPYCRAFT_LOGIC_7000_H

#include "made.h"
#include "game.h"
#include "feature.h"
#include "actor.h"
#include "globals.h"

class Rm7000 : public Room
{
public:
	Rm7000();
	void init ( void );
};

class SouthExit7000 : public SouthExit
{
public:
	SouthExit7000();
	void respond( void );
	int doVerb( int );
};

class WPScript7000 : public Script
{
 public:
 	WPScript7000() { name = "WPScript7000"; };
	void changeState( int );
};

class COScript7000 : public Script
{
  public:
 	COScript7000() { name = "COScript7000"; };
	void changeState( int );
};

extern int	theArenaResult;

#endif

