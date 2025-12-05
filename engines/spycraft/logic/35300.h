//Started by Kawah Wong
//Phone on desk in old Pearson's office
//Copyright Activision 1995
//Finished by Ka Wah

#ifndef SPYCRAFT_LOGIC_35300_H
#define SPYCRAFT_LOGIC_35300_H

#include "game.h"
#include "feature.h"
#include "actor.h"
#include "globals.h"
#include "inset.h"

class Rm35300 : public Room
{					
public:
	Rm35300();
	~Rm35300();
	void init ( void );
	int handleEvent ( MADEEventStamp * );
};

class SouthExit35300 : public SouthExit
{
public:
	SouthExit35300();
  	void respond ( void );
	int doVerb ( int );
};

/* features */
class Light35300 : public Feature
{
public:
	Light35300();
	void respond( void );
	int doVerb( int );
};

class DigitOne35300 : public Feature
{
public:
	DigitOne35300();
	void respond( void );
	int doVerb( int );
};

/* scripts */
class EnterScriptYuriVO35000 : public Script
{
 public:
 	EnterScriptYuriVO35000(); 
	void changeState( int );
};

class CallScript35100 : public Script
{
 public:
 	CallScript35100(); 
	void changeState( int );
};

#endif

