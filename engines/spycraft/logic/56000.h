//Started by Tom DeSalvo
//Interior est. Shot
//Copyright Activision 1995
//Finished by Ka Wah

#ifndef SPYCRAFT_LOGIC_56000_H
#define SPYCRAFT_LOGIC_56000_H

#include "made.h"
#include "game.h"
#include "feature.h"
#include "actor.h"
#include "inset.h"
#include "globals.h"

class Rm56000 : public Room
{
public:
	Rm56000();
	~Rm56000();
	void init ( void );
	int handleEvent ( MADEEventStamp * );
};

class NorthExit56000 : public ExitFeature
{
public:
	NorthExit56000();
  	void respond ( void );
	int doVerb ( int );
};

class SouthExit56000 : public SouthExit
{
public:
	SouthExit56000();
  	void respond ( void );
	int doVerb ( int );
};

class Rm56001 : public Room
{
public:
	Rm56001();
	~Rm56001();
	void init ( void );
	int handleEvent ( MADEEventStamp * );
};

class EnterBlackScreenScript56001 : public Script
{
public:
	EnterBlackScreenScript56001();
	void changeState ( int );
};

#endif

