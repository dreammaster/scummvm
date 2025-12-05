//Started by Kawah Wong
//Hotel Schlump - disguise kit, down on floor

#ifndef SPYCRAFT_LOGIC_69300_H
#define SPYCRAFT_LOGIC_69300_H

#include "game.h"
#include "feature.h"
#include "actor.h"
#include "globals.h"
#include "inset.h"

class Rm69300 : public Room
{					
public:
	Rm69300();
	//~Rm69300();
	void init ( void );
	int handleEvent ( MADEEventStamp * );
//	int doVerb ( int );
};

class SouthExit69300 : public SouthExit
{
public:
	SouthExit69300();
  	void respond ( void );
	int doVerb ( int );
};

/* features */
class DisguiseKit69300 : public Feature
{
public:
	DisguiseKit69300();
	void respond( void );
	int doVerb( int );
};

/* insets */
class DisguiseKitInset69320: public Inset
{
 public:
	DisguiseKitInset69320();
	void respond( void );
	int doVerb( int );
};

class DisguiseKitInset69340: public Inset
{
 public:
	DisguiseKitInset69340();
//	void respond( void );
//	int doVerb( int );
};

#endif

