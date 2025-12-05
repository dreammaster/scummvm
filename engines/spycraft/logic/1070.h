//Started by Tom DeSalvo
//Tundra Est & Cluster

#ifndef SPYCRAFT_LOGIC_1070_H
#define SPYCRAFT_LOGIC_1070_H

#include "made.h"
#include "game.h"
#include "feature.h"
#include "actor.h"
#include "globals.h"

class Rm1070 : public Room
{
public:
	Rm1070();
	~Rm1070();
	void init ( void );
	int handleEvent ( MADEEventStamp * );
};

class EnterScript1070 : public Script
{
 public:
 	EnterScript1070(); 
	void changeState( int );
};


/*class SouthExit1070 : public ExitFeature
{
public:
	SouthExit1070();
  	void respond ( void );
	int doVerb ( int );
};

class Tundra3D1070 : public TextFeature
{
public:
	Tundra3D1070();
	int doVerb ( int );
};

class JumpJet1070 : public TextFeature
{
public:
	JumpJet1070();
	int doVerb ( int );
};*/

#endif

