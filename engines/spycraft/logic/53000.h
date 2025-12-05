//Started by Kawah Wong
//Vilnius Center
//Copyright Activision 1995
//Finished by Ka Wah

#ifndef SPYCRAFT_LOGIC_53000_H
#define SPYCRAFT_LOGIC_53000_H

#include "game.h"
#include "feature.h"
#include "actor.h"
#include "globals.h"
#include "inset.h"

class Rm53000 : public Room
{					
public:
	Rm53000();
	~Rm53000();
	void init ( void );
	int handleEvent ( MADEEventStamp * );
};

/* scripts */
class EnterScript53000 : public Script
{
 public:
 	EnterScript53000(); 
	void changeState( int );
};

#endif

