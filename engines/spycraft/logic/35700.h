//Started by Kawah Wong
//Dialog - Warn Max about Harmonica in the alley
//Copyright Activision 1995
//Finished by Ka Wah

#ifndef SPYCRAFT_LOGIC_35700_H
#define SPYCRAFT_LOGIC_35700_H

#include "game.h"
#include "feature.h"
#include "actor.h"
#include "globals.h"
#include "inset.h"

class Rm35700 : public Room
{					
public:
	Rm35700();
	~Rm35700();
	void init ( void );
	void cue ( void );
	int handleEvent ( MADEEventStamp * );
};

class ScriptMaxSetsTrap35700 : public Script
{
public:
	ScriptMaxSetsTrap35700();
	void changeState ( int );
};

class AnotherScript35700 : public Script
{
public:
	AnotherScript35700();
	void changeState ( int );
};

/* dialog */
class MaxSetTrap : public DItem
{
public:
	MaxSetTrap();
	void doit( void );
};

class MeetHarmonica : public DItem
{
public:
	MeetHarmonica();
	void doit( void );
};

#endif

