/******************************
 *		Ext. Desert 
 *		room 91000
 *		created by: Linus Chen
 *****************************/ 	

#ifndef SPYCRAFT_LOGIC_91000_H
#define SPYCRAFT_LOGIC_91000_H

#include "made.h"
#include "game.h"
#include "feature.h"
#include "actor.h"
#include "globals.h"
#include "inset.h"

class Rm91000 : public Room
{
public:
	Rm91000();
	~Rm91000();
	void init ( void );
	void cue ( void );
	int handleEvent ( MADEEventStamp * );
};

class Script91000 : public Script
{
public:
	Script91000() {name = "Script91000";};
	void changeState ( int );
};

class GoTargetScript : public Script
{
public:
	GoTargetScript() {name = "gotargetscript";};
	void changeState ( int );
};

class ArrestedScript91000 : public Script
{
public:
	ArrestedScript91000() {name = "ArrestedScript91000";};
	void changeState ( int );
};

#endif
