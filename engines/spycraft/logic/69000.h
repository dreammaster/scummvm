//Started by Tom DeSalvo
//Hotel Schlump Int.
//Copyright Activision 1995
//Finished by Linus Chen

#ifndef SPYCRAFT_LOGIC_69000_H
#define SPYCRAFT_LOGIC_69000_H

#include "made.h"
#include "game.h"
#include "feature.h"
#include "actor.h"
#include "globals.h"
#include "sound.h"


class Rm69002 : public Room
{
public:
	Rm69002();
	~Rm69002();
	void cue ( void );
	void init ( void );
	int handleEvent ( MADEEventStamp * );
};

class Rm69003 : public Room
{
public:
	Rm69003();
	~Rm69003();
	void cue ( void );
	void init ( void );
	int handleEvent ( MADEEventStamp * );
};

class Rm69000 : public Room
{
public:
	Rm69000();
	~Rm69000();
	void cue ( void );
	void init ( void );
	int handleEvent ( MADEEventStamp * );
};

class SouthExit69000 : public SouthExit
{
public:
	SouthExit69000();
  	void respond ( void );
	int doVerb ( int );
};

class EastExit69000 : public ExitFeature
{
public:
	EastExit69000();
  	void respond ( void );
	int doVerb ( int );
};

class WestExit69000 : public ExitFeature
{
public:
	WestExit69000();
  	void respond ( void );
	int doVerb ( int );
};

/* feature */

class Onyx69000 : public Feature
{
public:
	Onyx69000();
	int doVerb ( int );
	void respond ( void );
};

/* scripts */
class EnterScript69000 : public Script
{
 public:
 	EnterScript69000(); 
	void changeState( int );
};

class OnyxScript69000 : public Script
{
 public:
 	Sound *onyxSound;
 	OnyxScript69000(); 
	void changeState( int );
};

// views //

class Cuffs69000 : public View
{
public:
	Cuffs69000();
  	void respond ( void );
	int doVerb ( int );
};

class CuffCursor : public MadeCursor
{
public:
	CuffCursor();
};

// custom ditem //
class DialogFlag : public DItem
{
public:
	DialogFlag() { name = "DialogFlag"; };
	void doit( void );
};

#endif

