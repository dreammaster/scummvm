/******************************
 *		Quit room
 *		room 94000
 *		created by: Linus Chen
 *****************************/ 	

#ifndef SPYCRAFT_LOGIC_94000_H
#define SPYCRAFT_LOGIC_94000_H

#include "made.h"
#include "game.h"
#include "feature.h"
#include "actor.h"
#include "globals.h"
#include "inset.h"

class Rm94000 : public Room
{
public:
	Rm94000();
	~Rm94000();
	void init ( void );
	void cue ( void );
	int handleEvent ( MADEEventStamp * );
};

class Script94000 : public Script
{
public:
	Script94000() {name = "Script94000";};
	void changeState ( int );
};

#endif
