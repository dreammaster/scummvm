//Started by Ka Wah
//Max's dialog 1)completed KAT
//Copyright Activision 1995
//Finished by Ka Wah

#ifndef SPYCRAFT_LOGIC_36600_H
#define SPYCRAFT_LOGIC_36600_H

#include "made.h"
#include "game.h"
#include "feature.h"
#include "actor.h"
#include "globals.h"
#include "inset.h"

class Rm36600 : public Room
{
public:
	Rm36600();
	~Rm36600();
	void init ( void );
	void cue ( void );
	int handleEvent ( MADEEventStamp * );
};

class SouthExit36600 : public SouthExit
{
public:
	SouthExit36600();
  	void respond ( void );
	int doVerb ( int );
};
	  
/* scripts */
class EnterScript36600 : public Script
{
 public:
 	EnterScript36600(); 
	void changeState( int );
};

/* dialogs */
class InfoDeed : public DItem
{
public:
	InfoDeed();
	void doit( void );
};

class TellAboutBS : public DItem
{
public:
	TellAboutBS();
	void doit( void );
};

class DoneHaveHat : public DItem
{
public:
	DoneHaveHat();
	void doit( void );
};

class NumberofYuri : public DItem
{
public:
	NumberofYuri();
	void doit( void );
};

class AddMeetBird : public DItem
{
public:
	AddMeetBird();
	void doit( void );
};

class AddEveryoneNoise : public DItem
{
public:
	AddEveryoneNoise();
	void doit( void );
};

#endif			
