//Started by Ka Wah
//Foster's office, without solving the KAT
//asking about Birdsong
//Copyright Activision 1995
//Finished by Ka Wah

#ifndef SPYCRAFT_LOGIC_36400_H
#define SPYCRAFT_LOGIC_36400_H

#include "made.h"
#include "game.h"
#include "feature.h"
#include "actor.h"
#include "globals.h"
#include "inset.h"

class Rm36400 : public Room
{
public:
	Rm36400();
	~Rm36400();
	void cue();
	void init ( void );
	int handleEvent ( MADEEventStamp * );
};

class SouthExit36400 : public SouthExit
{
public:
	SouthExit36400();
  	void respond ( void );
	int doVerb ( int );
};
	  
/* scripts */
class EnterScript36400 : public Script
{
 public:
 	EnterScript36400(); 
	void changeState( int );
};

/* dialogs */
class ProveDeed : public DItem
{
public:
	ProveDeed();
	void doit( void );
};

class DoneHaveHat2 : public DItem
{
public:
	DoneHaveHat2();
	void doit( void );
};

class AddMeetBird2 : public DItem
{
public:
	AddMeetBird2();
	void doit( void );
};

class AddWeDo2 : public DItem
{
public:
	AddWeDo2();
	void doit( void );
};

#endif			
