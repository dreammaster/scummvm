//Started by Kawah Wong
//Dialog - Max and BS about carrot and stick
//Copyright Activision 1995
//Finished by Ka Wah

#ifndef SPYCRAFT_LOGIC_35200_H
#define SPYCRAFT_LOGIC_35200_H

#include "game.h"
#include "feature.h"
#include "actor.h"
#include "globals.h"
#include "inset.h"

class Rm35200 : public Room
{					
public:
	Rm35200();
	~Rm35200();
	void init ( void );
	void cue ( void );
	int handleEvent ( MADEEventStamp * );
};

class EnterScript35200 : public Script
{
 public:
 	EnterScript35200(); 
	void changeState( int );
};

class StickPath : public DItem
{
public:
	StickPath();
	void doit( void );
};

class CarrotPath : public DItem
{
public:
	CarrotPath();
	void doit( void );
};

class CarrotAfterStickPath : public DItem
{
public:
	CarrotAfterStickPath();
	void doit( void );
};

#endif

