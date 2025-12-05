//Started by Ka Wah
//Foster's office, Ying
//Copyright Activision 1995
//Finished by Ka Wah

#ifndef SPYCRAFT_LOGIC_36700_H
#define SPYCRAFT_LOGIC_36700_H

#include "made.h"
#include "game.h"
#include "feature.h"
#include "actor.h"
#include "globals.h"
#include "inset.h"

class Rm36700 : public Room
{
public:
	Rm36700();
	~Rm36700();
	void cue();
	void init ( void );
	int handleEvent ( MADEEventStamp * );
};
	  
/* scripts */
class EnterScript36700 : public Script
{
 public:
 	EnterScript36700(); 
	void changeState( int );
};

/* dialog */
class DowntoBullpen : public DItem
{
public:
	DowntoBullpen();
	void doit( void );
};

class DowntoInterrogation : public DItem
{
public:
	DowntoInterrogation();
	void doit( void );
};

#endif			
