//Started by Kawah Wong
//Dialog - Yuri talk about Onyx
//Copyright Activision 1995
//Finished by Ka Wah

#ifndef SPYCRAFT_LOGIC_48500_H
#define SPYCRAFT_LOGIC_48500_H

#include "game.h"
#include "feature.h"
#include "actor.h"
#include "globals.h"
#include "inset.h"

class Rm48500 : public Room
{					
public:
	Rm48500();
	~Rm48500();
	void init ( void );
	void cue ( void );
	int handleEvent ( MADEEventStamp * );
};

class EnterScript48500 : public Script
{
 public:
 	EnterScript48500(); 
	void changeState( int );
};
	  
#endif

