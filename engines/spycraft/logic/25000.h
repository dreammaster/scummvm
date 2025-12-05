//Started by Tom DeSalvo
//Da BIID
//Copyright Activision 1995
//Finished by Ka Wah

#ifndef SPYCRAFT_LOGIC_25000_H
#define SPYCRAFT_LOGIC_25000_H

#include "made.h"
#include "game.h"
#include "feature.h"
#include "actor.h"
#include "globals.h"
#include "inset.h"

class Rm25000 : public Room
{
public:
	Rm25000();
	~Rm25000();
	void init ( void );
	int handleEvent ( MADEEventStamp * );
};

class SouthExit25000 : public ExitFeature
{
public:
	SouthExit25000();
  	void respond ( void );
	int doVerb ( int );
};

class EastExit25000 : public ExitFeature
{
public:
	EastExit25000();
  	void respond ( void );
	int doVerb ( int );
};

/* feature */
class ControlPanel25000 : public Feature
{
public:
	ControlPanel25000();
  	void respond ( void );
	int doVerb ( int );
};

/* inset */
class ControlPanelInset25000 : public Inset		
{
public:
	ControlPanelInset25000();
//	void respond();
//	int doVerb( int);
};

/* scripts */
class EnterScript25000 : public Script
{
 public:
 	EnterScript25000(); 
	void changeState( int );
};

#endif

